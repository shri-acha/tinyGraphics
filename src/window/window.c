#include "../../window.h"
#include "../../graphics.h"
#include "../events.h"
#include "../../gl_ext.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(__has_include)
#  if __has_include(<GLFW/glfw3.h>)
#    include <GLFW/glfw3.h>
#    define TINY_HAS_GLFW 1
#  endif
#endif


void tinyPushEvent(renderContext* rc, Event ev) {
    if (!rc) return;
    pushEvent(rc, ev);
}

void tinyPushKeyEvent(renderContext* rc, int keycode, KeyState state) {
    if (!rc) return;
    Event ev;
    ev.ev_typ = KEYBOARD;
    ev.ke.keycode = keycode;
    ev.ke.state = state;
    pushEvent(rc, ev);
}

void tinyPushMouseEvent(renderContext* rc, MouseButton btn, KeyState state, Point2 pos) {
    if (!rc) return;
    Event ev;
    ev.ev_typ = MOUSE;
    ev.me.btn = btn;
    ev.me.state = state;
    ev.me.pos = pos;
    pushEvent(rc, ev);
}

typedef struct {
    unsigned int texture_id;
} GLFWBackendData;

static int glfw_backend_init(tinyWindow* win, int width, int height, const char* title, void* user_data) {
    (void)user_data;
#ifdef TINY_HAS_GLFW
    if (!glfwInit()) return 0;
    GLFWwindow* handle = glfwCreateWindow(width, height, title ? title : "tinyGraphics Window", NULL, NULL);
    if (!handle) {
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(handle);
    glfwSwapInterval(1);

    win->handle = (void*)handle;

    if (win->rc) {
        registerGLFWCallbacks(handle, win->rc);
        if (win->rc->frame_buffer) {
            GLFWBackendData* bd = (GLFWBackendData*)malloc(sizeof(GLFWBackendData));
            if (bd) {
                TINY_GL_INIT_TEXTURE(win->rc->frame_buffer, bd->texture_id);
                win->backend_data = bd;
            }
        }
    }
    win->is_open = 1;
    return 1;
#else

    (void)win; (void)width; (void)height; (void)title;
    fprintf(stderr, "tinyGraphics: GLFW is not available in this build.\n");
    return 0;
#endif
}

static int glfw_backend_should_close(tinyWindow* win) {
#ifdef TINY_HAS_GLFW
    if (win && win->handle) {
        return glfwWindowShouldClose((GLFWwindow*)win->handle);
    }
#endif
    return !win->is_open;
}

static void glfw_backend_poll_events(tinyWindow* win) {
    (void)win;
#ifdef TINY_HAS_GLFW
    glfwPollEvents();
#endif
}

static void glfw_backend_swap_buffers(tinyWindow* win) {
#ifdef TINY_HAS_GLFW
    if (win && win->handle) {
        glfwSwapBuffers((GLFWwindow*)win->handle);
    }
#else
    (void)win;
#endif
}

static void glfw_backend_present_frame(tinyWindow* win, frameBuffer* fb) {
#ifdef TINY_HAS_GLFW
    if (!win || !win->handle || !fb) return;
    GLFWwindow* handle = (GLFWwindow*)win->handle;
    GLFWBackendData* bd = (GLFWBackendData*)win->backend_data;
    unsigned int tex_id = bd ? bd->texture_id : 0;

    TINY_GL_UPDATE_TEXTURE(fb, tex_id);
    glClear(GL_COLOR_BUFFER_BIT);
    TINY_GL_DRAW_QUAD();
    glfwSwapBuffers(handle);
    glfwPollEvents();

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
#else
    (void)win; (void)fb;
#endif
}

static void glfw_backend_destroy(tinyWindow* win) {
#ifdef TINY_HAS_GLFW
    if (win) {
        if (win->backend_data) {
            free(win->backend_data);
            win->backend_data = NULL;
        }
        if (win->handle) {
            glfwDestroyWindow((GLFWwindow*)win->handle);
            win->handle = NULL;
        }
        glfwTerminate();
    }
#else
    (void)win;
#endif
}

static const tinyWindowBackend g_glfw_backend = {
    .name = "GLFW/OpenGL",
    .init = glfw_backend_init,
    .should_close = glfw_backend_should_close,
    .poll_events = glfw_backend_poll_events,
    .swap_buffers = glfw_backend_swap_buffers,
    .present_frame = glfw_backend_present_frame,
    .destroy = glfw_backend_destroy
};

const tinyWindowBackend* tinyGetGLFWBackend(void) {
    return &g_glfw_backend;
}

static int custom_backend_init(tinyWindow* win, int width, int height, const char* title, void* user_data) {
    (void)width; (void)height; (void)title; (void)user_data;
    win->is_open = 1;
    return 1;
}

static int custom_backend_should_close(tinyWindow* win) {
    return win ? !win->is_open : 1;
}

static void custom_backend_poll_events(tinyWindow* win) {
    if (win && win->rc && win->rc->event_handler.f) {
        processEvents(win->rc);
    }
}

static void custom_backend_swap_buffers(tinyWindow* win) {
    (void)win;
}

static void custom_backend_present_frame(tinyWindow* win, frameBuffer* fb) {
    if (!win || !fb) return;
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

static void custom_backend_destroy(tinyWindow* win) {
    if (win) {
        win->is_open = 0;
    }
}

static const tinyWindowBackend g_custom_backend = {
    .name = "Custom/Manual",
    .init = custom_backend_init,
    .should_close = custom_backend_should_close,
    .poll_events = custom_backend_poll_events,
    .swap_buffers = custom_backend_swap_buffers,
    .present_frame = custom_backend_present_frame,
    .destroy = custom_backend_destroy
};

const tinyWindowBackend* tinyGetCustomBackend(void) {
    return &g_custom_backend;
}

static int terminal_backend_init(tinyWindow* win, int width, int height, const char* title, void* user_data) {
    (void)width; (void)height; (void)title; (void)user_data;
    win->is_open = 1;
    return 1;
}

static int terminal_backend_should_close(tinyWindow* win) {
    return win ? !win->is_open : 1;
}

static void terminal_backend_poll_events(tinyWindow* win) {
    if (win && win->rc && win->rc->event_handler.f) {
        processEvents(win->rc);
    }
}

static void terminal_backend_swap_buffers(tinyWindow* win) {
    (void)win;
}

static void terminal_backend_present_frame(tinyWindow* win, frameBuffer* fb) {
    if (!win || !fb) return;
    formatBuffer(fb);
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

static void terminal_backend_destroy(tinyWindow* win) {
    if (win) {
        win->is_open = 0;
    }
}

static const tinyWindowBackend g_terminal_backend = {
    .name = "Terminal ASCII",
    .init = terminal_backend_init,
    .should_close = terminal_backend_should_close,
    .poll_events = terminal_backend_poll_events,
    .swap_buffers = terminal_backend_swap_buffers,
    .present_frame = terminal_backend_present_frame,
    .destroy = terminal_backend_destroy
};

const tinyWindowBackend* tinyGetTerminalBackend(void) {
    return &g_terminal_backend;
}

tinyWindow* tinyCreateWindow(const tinyWindowBackend* backend, int width, int height, const char* title, renderContext* rc, void* user_data) {
    tinyWindow* win = (tinyWindow*)calloc(1, sizeof(tinyWindow));
    if (!win) return NULL;

    if (!backend) {
#ifdef TINY_HAS_GLFW
        backend = tinyGetGLFWBackend();
#else
        backend = tinyGetCustomBackend();
#endif
    }

    win->backend = backend;
    win->width = width;
    win->height = height;
    win->rc = rc;
    win->user_data = user_data;

    if (win->backend && win->backend->init) {
        if (!win->backend->init(win, width, height, title, user_data)) {
            free(win);
            return NULL;
        }
    } else {
        win->is_open = 1;
    }

    return win;
}

int tinyWindowShouldClose(tinyWindow* win) {
    if (!win) return 1;
    if (win->backend && win->backend->should_close) {
        return win->backend->should_close(win);
    }
    return !win->is_open;
}

void tinyWindowPollEvents(tinyWindow* win) {
    if (!win) return;
    if (win->backend && win->backend->poll_events) {
        win->backend->poll_events(win);
    }
}

void tinyWindowSwapBuffers(tinyWindow* win) {
    if (!win) return;
    if (win->backend && win->backend->swap_buffers) {
        win->backend->swap_buffers(win);
    }
}

void tinyWindowPresent(tinyWindow* win) {
    if (!win) return;
    frameBuffer* fb = win->rc ? win->rc->frame_buffer : NULL;
    if (win->backend && win->backend->present_frame) {
        win->backend->present_frame(win, fb);
    }
}

void tinyDestroyWindow(tinyWindow* win) {
    if (!win) return;
    if (win->backend && win->backend->destroy) {
        win->backend->destroy(win);
    }
    free(win);
}
