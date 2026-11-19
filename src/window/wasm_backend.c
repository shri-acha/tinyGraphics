/*
 * WebAssembly (Emscripten) window backend.
 *
 * tinyGraphics still rasterizes everything on the CPU. This backend only
 * converts the finished RGB565 frameBuffer to RGBA8 and blits it onto an
 * HTML <canvas id="canvas"> with putImageData. Keyboard and mouse input is
 * forwarded into the renderContext event queue.
 */
#ifdef __EMSCRIPTEN__

#include "../../window.h"
#include "../../graphics.h"
#include "../events.h"
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdlib.h>
#include <stdint.h>

#define TINY_WASM_CANVAS "#canvas"

typedef struct {
    uint8_t* rgba;   /* width * height * 4 staging buffer */
    int width;
    int height;
} WasmBackendData;

EM_JS(void, tiny_wasm_blit, (const uint8_t* ptr, int w, int h), {
    var canvas = Module['canvas'] || document.getElementById('canvas');
    if (!canvas) return;
    if (!Module.__tinyCtx || Module.__tinyCanvas !== canvas) {
        Module.__tinyCanvas = canvas;
        Module.__tinyCtx = canvas.getContext('2d');
    }
    var img = new ImageData(new Uint8ClampedArray(HEAPU8.buffer, ptr, w * h * 4), w, h);
    Module.__tinyCtx.putImageData(img, 0, 0);
});

static EM_BOOL wasm_key_cb(int type, const EmscriptenKeyboardEvent* e, void* user) {
    tinyWindow* win = (tinyWindow*)user;
    if (!win || !win->rc) return EM_FALSE;
    tinyPushKeyEvent(win->rc, (int)e->keyCode,
                     type == EMSCRIPTEN_EVENT_KEYUP ? UP : DOWN);
    return EM_FALSE; /* don't swallow browser shortcuts */
}

static EM_BOOL wasm_mouse_cb(int type, const EmscriptenMouseEvent* e, void* user) {
    tinyWindow* win = (tinyWindow*)user;
    if (!win || !win->rc) return EM_FALSE;

    /* Map CSS pixels (the canvas may be scaled up) to framebuffer pixels. */
    double css_w = 0, css_h = 0;
    emscripten_get_element_css_size(TINY_WASM_CANVAS, &css_w, &css_h);
    int x = (int)e->targetX, y = (int)e->targetY;
    if (css_w > 0 && css_h > 0) {
        x = (int)(e->targetX * win->width / css_w);
        y = (int)(e->targetY * win->height / css_h);
    }
    Point2 pos = { .x = x, .y = y };
    tinyPushMouseEvent(win->rc, e->button == 2 ? RIGHT : LEFT,
                       type == EMSCRIPTEN_EVENT_MOUSEUP ? UP : DOWN, pos);
    return EM_TRUE;
}

static int wasm_backend_init(tinyWindow* win, int width, int height, const char* title, void* user_data) {
    (void)user_data;
    WasmBackendData* bd = (WasmBackendData*)calloc(1, sizeof(WasmBackendData));
    if (!bd) return 0;
    bd->rgba = (uint8_t*)malloc((size_t)width * height * 4);
    if (!bd->rgba) { free(bd); return 0; }
    bd->width = width;
    bd->height = height;
    win->backend_data = bd;

    emscripten_set_canvas_element_size(TINY_WASM_CANVAS, width, height);
    if (title) emscripten_set_window_title(title);

    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, win, EM_FALSE, wasm_key_cb);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, win, EM_FALSE, wasm_key_cb);
    emscripten_set_mousedown_callback(TINY_WASM_CANVAS, win, EM_FALSE, wasm_mouse_cb);
    emscripten_set_mouseup_callback(TINY_WASM_CANVAS, win, EM_FALSE, wasm_mouse_cb);

    win->is_open = 1;
    return 1;
}

static int wasm_backend_should_close(tinyWindow* win) {
    return win ? !win->is_open : 1;
}

static void wasm_backend_poll_events(tinyWindow* win) {
    if (win && win->rc && win->rc->event_handler.f) {
        processEvents(win->rc);
    }
}

static void wasm_backend_swap_buffers(tinyWindow* win) {
    (void)win; /* the browser composites the canvas itself */
}

static void wasm_backend_present_frame(tinyWindow* win, frameBuffer* fb) {
    if (!win || !fb) return;
    WasmBackendData* bd = (WasmBackendData*)win->backend_data;
    if (bd && bd->width == fb->width && bd->height == fb->height) {
        const int n = fb->width * fb->height;
        uint8_t* out = bd->rgba;
        for (int i = 0; i < n; i++) {
            uint16_t c = fb->buffer[i].color.literal;
            uint8_t r = (c >> 11) & 0x1F;
            uint8_t g = (c >> 5) & 0x3F;
            uint8_t b = c & 0x1F;
            out[0] = (uint8_t)((r << 3) | (r >> 2));
            out[1] = (uint8_t)((g << 2) | (g >> 4));
            out[2] = (uint8_t)((b << 3) | (b >> 2));
            out[3] = 255;
            out += 4;
        }
        tiny_wasm_blit(bd->rgba, fb->width, fb->height);
    }

    if (win->rc && win->rc->event_handler.f) {
        processEvents(win->rc);
    }
    flushPixelBuffer(fb->buffer, fb->width, fb->height);
    if (fb->depth_buffer) {
        flushDepthBuffer(fb->depth_buffer, fb->width, fb->height);
    }
    if (win->rc && win->rc->scene_context) {
        flushSceneContext(win->rc->scene_context);
    }
}

static void wasm_backend_destroy(tinyWindow* win) {
    if (!win) return;
    WasmBackendData* bd = (WasmBackendData*)win->backend_data;
    if (bd) {
        free(bd->rgba);
        free(bd);
        win->backend_data = NULL;
    }
    win->is_open = 0;
}

static const tinyWindowBackend g_wasm_backend = {
    .name = "WebAssembly/Canvas",
    .init = wasm_backend_init,
    .should_close = wasm_backend_should_close,
    .poll_events = wasm_backend_poll_events,
    .swap_buffers = wasm_backend_swap_buffers,
    .present_frame = wasm_backend_present_frame,
    .destroy = wasm_backend_destroy
};

const tinyWindowBackend* tinyGetWasmBackend(void) {
    return &g_wasm_backend;
}

#else
/* Keep the translation unit non-empty for native builds (ISO C). */
typedef int tiny_wasm_backend_unused;
#endif /* __EMSCRIPTEN__ */
