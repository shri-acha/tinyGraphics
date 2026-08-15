#pragma once
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tinyWindow tinyWindow;

/**
 * Generic Window Backend Interface (vtable).
 * 
 * Any window framework (GLFW, SDL2, SDL3, SFML, GLUT, Win32, X11, Raylib, WebAssembly, Custom)
 * can implement this structure to provide window lifecycle, event polling, and presentation.
 */
typedef struct tinyWindowBackend {
    const char* name;
    int  (*init)(tinyWindow* win, int width, int height, const char* title, void* user_data);
    int  (*should_close)(tinyWindow* win);
    void (*poll_events)(tinyWindow* win);
    void (*swap_buffers)(tinyWindow* win);
    void (*present_frame)(tinyWindow* win, frameBuffer* fb);
    void (*destroy)(tinyWindow* win);
} tinyWindowBackend;

struct tinyWindow {
    void* handle;                  
    void* backend_data;            
    const tinyWindowBackend* backend;
    renderContext* rc;
    int width;
    int height;
    int is_open;
    void* user_data;
};

tinyWindow* tinyCreateWindow(const tinyWindowBackend* backend, int width, int height, const char* title, renderContext* rc, void* user_data);

int tinyWindowShouldClose(tinyWindow* win);

void tinyWindowPollEvents(tinyWindow* win);

void tinyWindowSwapBuffers(tinyWindow* win);

void tinyWindowPresent(tinyWindow* win);

void tinyDestroyWindow(tinyWindow* win);

void tinyPushEvent(renderContext* rc, Event ev);

void tinyPushKeyEvent(renderContext* rc, int keycode, KeyState state);

void tinyPushMouseEvent(renderContext* rc, MouseButton btn, KeyState state, Point2 pos);


const tinyWindowBackend* tinyGetGLFWBackend(void);

const tinyWindowBackend* tinyGetCustomBackend(void);

const tinyWindowBackend* tinyGetTerminalBackend(void);

#ifdef __cplusplus
}
#endif
