#pragma once
#include "../types.h"

#if defined(__has_include)
#  if __has_include(<GLFW/glfw3.h>)
#    include <GLFW/glfw3.h>
#    define TINY_HAS_GLFW 1
#  endif
#endif

#ifndef TINY_HAS_GLFW
/* Forward declaration when GLFW headers are not included */
typedef struct GLFWwindow GLFWwindow;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= */
/* Ring Buffer Operations                                                    */
/* ========================================================================= */

/**
 * Initializes the event ring buffer.
 */
void ringBufferInit(EventRingBuffer *rb);

/**
 * Pushes an event into the ring buffer.
 * If the buffer is full, the oldest event is overwritten to maintain fresh input.
 * Returns 1 on success, 0 on error.
 */
int ringBufferPush(EventRingBuffer *rb, Event ev);

/**
 * Pops the oldest event from the ring buffer into `ev`.
 * Returns 1 if an event was popped, 0 if the buffer is empty.
 */
int ringBufferPop(EventRingBuffer *rb, Event *ev);

/**
 * Checks if the ring buffer is empty.
 * Returns 1 if empty, 0 otherwise.
 */
int ringBufferIsEmpty(const EventRingBuffer *rb);

/**
 * Checks if the ring buffer is full.
 * Returns 1 if full, 0 otherwise.
 */
int ringBufferIsFull(const EventRingBuffer *rb);

/**
 * Returns the current number of events stored in the ring buffer.
 */
int ringBufferCount(const EventRingBuffer *rb);

/**
 * Clears all events in the ring buffer.
 */
void ringBufferClear(EventRingBuffer *rb);

/* ========================================================================= */
/* Render Context Event Operations                                           */
/* ========================================================================= */

/**
 * Initializes the event queue inside the renderContext.
 */
void initEventContext(renderContext *rc);

/**
 * Pushes an event into the renderContext's event queue.
 */
int pushEvent(renderContext *rc, Event ev);

/**
 * Polls the next event from the renderContext's event queue.
 * Returns 1 if an event was retrieved, 0 if the queue is empty.
 */
int pollEvent(renderContext *rc, Event *ev);

/**
 * Drains all queued events from the renderContext's event queue and invokes
 * the registered event handler callback (if rc->event_handler.f is non-NULL).
 */
void processEvents(renderContext *rc);

/**
 * Sets the event handler callback for the renderContext.
 */
void registerEventHandler(renderContext *rc, void (*handler)(Event e));

/* ========================================================================= */
/* GLFW Event Integration                                                    */
/* ========================================================================= */

/**
 * GLFW key callback that converts key events to tinyGraphics KeyboardEvent
 * and pushes them into the renderContext event queue.
 */
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

/**
 * GLFW mouse button callback that converts mouse button clicks to tinyGraphics
 * MouseEvent and pushes them into the renderContext event queue.
 */
void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

/**
 * GLFW cursor position callback that tracks cursor movement and can record
 * position updates.
 */
void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos);

/**
 * Registers all GLFW event callbacks on `window` and attaches `rc` via
 * glfwSetWindowUserPointer so callbacks can access the renderContext event queue.
 */
void registerGLFWCallbacks(GLFWwindow* window, renderContext* rc);

#ifdef __cplusplus
}
#endif
