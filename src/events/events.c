#include "../events.h"
#include <string.h>

#ifdef TINY_HAS_GLFW
#include <GLFW/glfw3.h>
#endif

/* ========================================================================= */
/* Ring Buffer Operations                                                    */
/* ========================================================================= */

void ringBufferInit(EventRingBuffer *rb) {
    if (!rb) return;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    memset(rb->events, 0, sizeof(rb->events));
}

int ringBufferPush(EventRingBuffer *rb, Event ev) {
    if (!rb) return 0;
    if (rb->count == EVENT_BUFFER_CAPACITY) {
        /* Buffer is full: overwrite oldest event by advancing tail */
        rb->events[rb->head] = ev;
        rb->head = (rb->head + 1) % EVENT_BUFFER_CAPACITY;
        rb->tail = (rb->tail + 1) % EVENT_BUFFER_CAPACITY;
        return 1;
    }
    rb->events[rb->head] = ev;
    rb->head = (rb->head + 1) % EVENT_BUFFER_CAPACITY;
    rb->count++;
    return 1;
}

int ringBufferPop(EventRingBuffer *rb, Event *ev) {
    if (!rb || rb->count == 0) return 0;
    if (ev) {
        *ev = rb->events[rb->tail];
    }
    rb->tail = (rb->tail + 1) % EVENT_BUFFER_CAPACITY;
    rb->count--;
    return 1;
}

int ringBufferIsEmpty(const EventRingBuffer *rb) {
    return (!rb || rb->count == 0);
}

int ringBufferIsFull(const EventRingBuffer *rb) {
    return (rb && rb->count >= EVENT_BUFFER_CAPACITY);
}

int ringBufferCount(const EventRingBuffer *rb) {
    return rb ? rb->count : 0;
}

void ringBufferClear(EventRingBuffer *rb) {
    if (!rb) return;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

/* ========================================================================= */
/* Render Context Event Operations                                           */
/* ========================================================================= */

void initEventContext(renderContext *rc) {
    if (!rc) return;
    ringBufferInit(&rc->event_queue);
}

int pushEvent(renderContext *rc, Event ev) {
    if (!rc) return 0;
    return ringBufferPush(&rc->event_queue, ev);
}

int pollEvent(renderContext *rc, Event *ev) {
    if (!rc) return 0;
    return ringBufferPop(&rc->event_queue, ev);
}

void processEvents(renderContext *rc) {
    if (!rc) return;
    Event ev;
    while (pollEvent(rc, &ev)) {
        if (rc->event_handler.f) {
            rc->event_handler.f(ev);
        }
    }
}

void registerEventHandler(renderContext *rc, void (*handler)(Event e)) {
    if (!rc) return;
    rc->event_handler.f = handler;
}

/* ========================================================================= */
/* GLFW Event Integration                                                    */
/* ========================================================================= */

#ifdef TINY_HAS_GLFW

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    renderContext* rc = (renderContext*)glfwGetWindowUserPointer(window);
    if (!rc) return;

    Event e;
    e.ev_typ = KEYBOARD;
    e.ke.keycode = key;
    e.ke.state = (action == GLFW_RELEASE) ? UP : DOWN;
    pushEvent(rc, e);
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void)mods;
    renderContext* rc = (renderContext*)glfwGetWindowUserPointer(window);
    if (!rc) return;

    double xpos = 0.0, ypos = 0.0;
    glfwGetCursorPos(window, &xpos, &ypos);

    Event e;
    e.ev_typ = MOUSE;
    e.me.state = (action == GLFW_RELEASE) ? UP : DOWN;
    e.me.btn = (button == GLFW_MOUSE_BUTTON_LEFT) ? LEFT : RIGHT;
    e.me.pos = (Point2){ .x = (int)xpos, .y = (int)ypos };
    pushEvent(rc, e);
}

void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    (void)window;
    (void)xpos;
    (void)ypos;
    /* Optional: can track continuous cursor motion if needed */
}

void registerGLFWCallbacks(GLFWwindow* window, renderContext* rc) {
    if (!window || !rc) return;
    initEventContext(rc);
    glfwSetWindowUserPointer(window, rc);
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
}

#else

/* Stubs when GLFW is not available */
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)window; (void)key; (void)scancode; (void)action; (void)mods;
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void)window; (void)button; (void)action; (void)mods;
}

void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    (void)window; (void)xpos; (void)ypos;
}

void registerGLFWCallbacks(GLFWwindow* window, renderContext* rc) {
    (void)window; (void)rc;
}

#endif
