/*
 * Poly fishes: a low-poly aquarium rendered entirely by tinyGraphics.
 *
 * Every pixel (water gradient, light rays, seaweed, fish, bubbles, sand) is
 * rasterized on the CPU by tinyGraphics. The window backend only presents the
 * finished framebuffer:
 *   - Web (Emscripten): tinyGetWasmBackend() blits onto <canvas id="canvas">
 *   - Native:           the default backend (GLFW when available)
 *
 * Click the canvas to release a burst of bubbles.
 *
 * Build (web):    make wasm_fishes   -> build/web/{index.html,fishes.js,fishes.wasm}
 * Build (native): make native_fishes
 */
#include "../graphics.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define W 480
#define H 270
#define NUM_FISH 11
#define NUM_BUBBLES 48
#define NUM_WEEDS 9
#define SAND_TOP (H - 34)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ------------------------------------------------------------------------ */
/* Helpers                                                                  */
/* ------------------------------------------------------------------------ */

static Color rgb(int r, int g, int b) {
    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;
    return (Color){ .literal = (uint16_t)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)) };
}

typedef struct { int r, g, b; } RGB;

static Color shade(RGB c, float k) {
    return rgb((int)(c.r * k), (int)(c.g * k), (int)(c.b * k));
}

static float frand(float lo, float hi) {
    return lo + (hi - lo) * ((float)rand() / (float)RAND_MAX);
}

static double now_seconds(void) {
#ifdef __EMSCRIPTEN__
    return emscripten_get_now() / 1000.0;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
#endif
}

/* renderTriangle2D sorts its pointer array in place, so build a fresh one per call. */
static void tri(renderContext* rc, float ax, float ay, float bx, float by,
                float cx, float cy, Color c) {
    Point2 a = { .x = (int)lroundf(ax), .y = (int)lroundf(ay) };
    Point2 b = { .x = (int)lroundf(bx), .y = (int)lroundf(by) };
    Point2 d = { .x = (int)lroundf(cx), .y = (int)lroundf(cy) };
    Point2* pts[3] = { &a, &b, &d };
    renderTriangle2D(rc, pts, c);
}

static void quad(renderContext* rc, float x0, float y0, float x1, float y1,
                 float x2, float y2, float x3, float y3, Color c) {
    tri(rc, x0, y0, x1, y1, x2, y2, c);
    tri(rc, x0, y0, x2, y2, x3, y3, c);
}

/* ------------------------------------------------------------------------ */
/* Scene state                                                              */
/* ------------------------------------------------------------------------ */

typedef struct {
    float x, y;       /* body center */
    float base_y;
    float len;        /* half body length */
    float speed;      /* px / second */
    float phase;
    RGB body;
    RGB fin;
} Fish;

typedef struct {
    float x, y, speed, wobble;
    int r;
    int alive;
} Bubble;

typedef struct {
    float x, height, phase;
    RGB color;
} Weed;

typedef struct {
    renderContext* rc;
    Fish fish[NUM_FISH];
    Bubble bubbles[NUM_BUBBLES];
    Weed weeds[NUM_WEEDS];
    double last;
    float t;
} Scene;

static Scene g_scene;

static const RGB FISH_PALETTE[][2] = {
    { {255, 140,  40}, {255, 230, 120} },  /* clownfish orange  */
    { { 60, 200, 230}, {240, 250, 120} },  /* cyan / yellow     */
    { {250,  90, 120}, {255, 200, 210} },  /* coral pink        */
    { {255, 210,  60}, { 60, 120, 230} },  /* yellow tang       */
    { {150, 110, 240}, {255, 160, 230} },  /* purple            */
    { { 90, 230, 140}, {230, 255, 200} },  /* mint              */
};
#define PALETTE_N ((int)(sizeof(FISH_PALETTE) / sizeof(FISH_PALETTE[0])))

static void respawn_fish(Fish* f, int initial) {
    f->len = frand(9.0f, 22.0f);
    /* bigger fish are "closer", so they swim faster (parallax) */
    f->speed = 18.0f + f->len * frand(1.8f, 3.0f);
    f->base_y = frand(30.0f, SAND_TOP - 30.0f);
    f->y = f->base_y;
    f->x = initial ? frand(-40.0f, W) : -f->len * 2.5f - frand(0.0f, 120.0f);
    f->phase = frand(0.0f, 2.0f * (float)M_PI);
    int p = rand() % PALETTE_N;
    f->body = FISH_PALETTE[p][0];
    f->fin = FISH_PALETTE[p][1];
}

static int cmp_fish(const void* a, const void* b) {
    float la = ((const Fish*)a)->len, lb = ((const Fish*)b)->len;
    return (la > lb) - (la < lb);
}

static void spawn_bubble(float x, float y) {
    for (int i = 0; i < NUM_BUBBLES; i++) {
        Bubble* b = &g_scene.bubbles[i];
        if (!b->alive) {
            b->alive = 1;
            b->x = x;
            b->y = y;
            b->r = 1 + rand() % 3;
            b->speed = frand(18.0f, 40.0f);
            b->wobble = frand(0.0f, 6.28f);
            return;
        }
    }
}

static void on_event(Event e) {
    if (e.ev_typ == MOUSE && e.me.state == DOWN) {
        for (int i = 0; i < 8; i++) {
            spawn_bubble(e.me.pos.x + frand(-6, 6), e.me.pos.y + frand(-6, 6));
        }
    }
}

/* ------------------------------------------------------------------------ */
/* Drawing                                                                  */
/* ------------------------------------------------------------------------ */

static void draw_water(renderContext* rc, float t) {
    /* vertical gradient: bright teal at the surface -> deep navy */
    for (int y = 0; y < H; y++) {
        float k = (float)y / (float)H;
        int r = (int)(20 + (4 - 20) * k);
        int g = (int)(120 + (30 - 120) * k);
        int b = (int)(170 + (70 - 170) * k);
        renderHorizontalLine2D(rc, 0, W - 1, y, rgb(r, g, b));
    }
    /* slanted god-rays, slowly drifting */
    for (int i = 0; i < 4; i++) {
        float x = 60.0f + i * 115.0f + sinf(t * 0.25f + i * 1.7f) * 18.0f;
        float w = 14.0f + 6.0f * i;
        Color c = rgb(40 + 4 * i, 145, 185);
        quad(rc, x, 0, x + w, 0, x + w + 90, SAND_TOP, x + 60, SAND_TOP, c);
    }
    /* surface ripples */
    for (int x = 0; x < W; x += 24) {
        float y = 3.0f + sinf(t * 2.0f + x * 0.08f) * 2.0f;
        tri(rc, x, 0, x + 24, 0, x + 12, y + 3, rgb(90, 200, 220));
    }
}

static void draw_weed(renderContext* rc, const Weed* w, float t) {
    const int segs = 7;
    float seg_h = w->height / segs;
    float px = w->x, py = SAND_TOP + 4, pw = 5.0f;
    for (int i = 1; i <= segs; i++) {
        float sway = sinf(t * 1.3f + w->phase + i * 0.55f) * i * 1.6f;
        float nx = w->x + sway;
        float ny = SAND_TOP + 4 - seg_h * i;
        float nw = 5.0f * (1.0f - (float)i / (segs + 1));
        float k = 0.75f + 0.25f * (i & 1);
        quad(rc, px - pw, py, px + pw, py, nx + nw, ny, nx - nw, ny, shade(w->color, k));
        px = nx; py = ny; pw = nw;
    }
}

static void draw_fish(renderContext* rc, const Fish* f, float t) {
    float L = f->len;
    float h = L * 0.55f;
    float x = f->x, y = f->y;
    float wig = sinf(t * 9.0f + f->phase) * h * 0.45f;

    float nose_x = x + L, back_x = x - L * 0.75f;
    float top_y = y - h, bot_y = y + h * 0.9f;

    /* tail (behind body) */
    float tail_x = back_x - L * 0.75f;
    tri(rc, back_x + 2, y, tail_x, y - h * 0.85f + wig, tail_x + L * 0.15f, y + wig * 0.3f, shade(f->fin, 0.95f));
    tri(rc, back_x + 2, y, tail_x + L * 0.15f, y + wig * 0.3f, tail_x, y + h * 0.85f + wig, shade(f->fin, 0.75f));

    /* dorsal fin */
    tri(rc, x - L * 0.35f, top_y + 2, x + L * 0.25f, top_y + 3, x - L * 0.25f, top_y - h * 0.6f, shade(f->fin, 0.85f));

    /* faceted body: four triangles lit from upper-front */
    float mid_x = x + L * 0.1f;
    tri(rc, mid_x, top_y, nose_x, y, mid_x, y, shade(f->body, 1.00f));   /* upper front */
    tri(rc, back_x, y, mid_x, top_y, mid_x, y, shade(f->body, 0.82f));  /* upper back  */
    tri(rc, mid_x, y, nose_x, y, mid_x, bot_y, shade(f->body, 0.68f));  /* lower front */
    tri(rc, back_x, y, mid_x, y, mid_x, bot_y, shade(f->body, 0.55f));  /* lower back  */

    /* stripe facet */
    tri(rc, x - L * 0.15f, top_y + h * 0.25f, x + L * 0.05f, y, x - L * 0.15f, bot_y - h * 0.25f, shade(f->fin, 0.9f));

    /* pectoral fin flapping */
    float flap = sinf(t * 7.0f + f->phase * 2.0f) * h * 0.25f;
    tri(rc, x + L * 0.25f, y + h * 0.15f, x - L * 0.05f, y + h * 0.55f + flap, x + L * 0.05f, y + h * 0.1f, shade(f->fin, 0.7f));

    /* eye */
    int er = L > 14 ? 2 : 1;
    Point2 eye = { .x = (int)(x + L * 0.6f), .y = (int)(y - h * 0.25f) };
    renderCircle2D(rc, eye, er + 1, rgb(250, 250, 250));
    renderCircle2D(rc, (Point2){ .x = eye.x + 1, .y = eye.y }, er - 1 > 0 ? er - 1 : 1, rgb(10, 10, 20));
}

static void draw_bubbles(renderContext* rc, float t) {
    rc->render_mode = WIREFRAME;
    for (int i = 0; i < NUM_BUBBLES; i++) {
        Bubble* b = &g_scene.bubbles[i];
        if (!b->alive) continue;
        Point2 p = { .x = (int)(b->x + sinf(t * 3.0f + b->wobble) * 2.0f), .y = (int)b->y };
        renderCircle2D(rc, p, b->r + 1, rgb(190, 235, 250));
    }
    rc->render_mode = FILLED;
}

static void draw_seabed(renderContext* rc) {
    /* rocks */
    tri(rc, 70, SAND_TOP + 6, 105, SAND_TOP - 22, 140, SAND_TOP + 6, rgb(70, 80, 105));
    tri(rc, 105, SAND_TOP - 22, 140, SAND_TOP + 6, 150, SAND_TOP - 6, rgb(50, 58, 80));
    tri(rc, 330, SAND_TOP + 6, 352, SAND_TOP - 14, 380, SAND_TOP + 6, rgb(80, 70, 95));
    tri(rc, 352, SAND_TOP - 14, 380, SAND_TOP + 6, 392, SAND_TOP, rgb(58, 50, 72));

    /* faceted sand dunes */
    const int step = 32;
    for (int x = 0; x < W; x += step) {
        float y0 = SAND_TOP + 6 * sinf(x * 0.045f);
        float y1 = SAND_TOP + 6 * sinf((x + step) * 0.045f);
        float ym = SAND_TOP + 10 + 3 * cosf(x * 0.09f);
        tri(rc, x, y0, x + step, y1, x + step / 2.0f, ym, rgb(214, 186, 120));
        quad(rc, x, y0, x + step / 2.0f, ym, x + step / 2.0f, H, x, H, rgb(196, 166, 104));
        quad(rc, x + step / 2.0f, ym, x + step, y1, x + step, H, x + step / 2.0f, H, rgb(178, 148, 90));
    }
}

/* ------------------------------------------------------------------------ */
/* Frame                                                                    */
/* ------------------------------------------------------------------------ */

static void frame(tinyWindow* win, void* arg) {
    (void)win;
    Scene* s = (Scene*)arg;
    renderContext* rc = s->rc;

    double now = now_seconds();
    float dt = (float)(now - s->last);
    s->last = now;
    if (dt > 0.1f) dt = 0.1f;   /* tab was hidden: don't teleport */
    if (dt < 0.0f) dt = 0.0f;
    s->t += dt;
    float t = s->t;

    /* update */
    for (int i = 0; i < NUM_FISH; i++) {
        Fish* f = &s->fish[i];
        f->x += f->speed * dt;
        f->y = f->base_y + sinf(t * 1.4f + f->phase) * 6.0f;
        if (f->x - f->len * 2.5f > W) respawn_fish(f, 0);
        if (rand() % 400 == 0) spawn_bubble(f->x + f->len, f->y - 2);
    }
    for (int i = 0; i < NUM_BUBBLES; i++) {
        Bubble* b = &s->bubbles[i];
        if (!b->alive) continue;
        b->y -= b->speed * dt;
        if (b->y < -4) b->alive = 0;
    }
    if (rand() % 12 == 0) spawn_bubble(frand(0, W), SAND_TOP);

    /* draw (painter's order) */
    draw_water(rc, t);
    for (int i = 0; i < NUM_WEEDS; i += 2) draw_weed(rc, &s->weeds[i], t);
    for (int i = 0; i < NUM_FISH; i++) draw_fish(rc, &s->fish[i], t);
    for (int i = 1; i < NUM_WEEDS; i += 2) draw_weed(rc, &s->weeds[i], t);
    draw_bubbles(rc, t);
    draw_seabed(rc);
    /* tinyWindowRunLoop presents + clears the framebuffer after this returns */
}

int main(void) {
    srand((unsigned)time(NULL));

    frameBuffer* fb = createFrameBuffer(W, H);
    static renderContext rc;
    rc.frame_buffer = fb;
    rc.render_mode = FILLED;
    rc.origin = (Index){ .x = 0, .y = 0, .z = 0 };
    rc.scene_context = NULL; /* immediate mode: no scene bookkeeping needed */
    initEventContext(&rc);
    registerEventHandler(&rc, on_event);

    g_scene.rc = &rc;
    for (int i = 0; i < NUM_FISH; i++) respawn_fish(&g_scene.fish[i], 1);
    qsort(g_scene.fish, NUM_FISH, sizeof(Fish), cmp_fish); /* small/far first */
    for (int i = 0; i < NUM_WEEDS; i++) {
        Weed* w = &g_scene.weeds[i];
        w->x = 20.0f + i * (W - 40.0f) / (NUM_WEEDS - 1) + frand(-12, 12);
        w->height = frand(50.0f, 120.0f);
        w->phase = frand(0.0f, 6.28f);
        w->color = (i & 1) ? (RGB){ 40, 170, 90 } : (RGB){ 30, 120, 80 };
    }
    g_scene.last = now_seconds();

    tinyWindow* win = tinyCreateWindow(NULL, W, H, "tinyGraphics - poly fishes", &rc, NULL);
    if (!win) return 1;

    tinyWindowRunLoop(win, frame, &g_scene);

    /* Only reached natively (the browser never returns from the run loop). */
    tinyDestroyWindow(win);
    destroyFrameBuffer(fb);
    return 0;
}
