#pragma once

/* 
 * OpenGL Extension Macros for tinyGraphics
 * 
 * These macros simplify displaying a tinyGraphics frameBuffer to an OpenGL texture
 * without adding any build or link dependency on OpenGL to the core tinyGraphics library.
 */

/* Initializes an OpenGL 2D texture matching the dimensions and format of frameBuffer */
#ifndef TINY_GL_INIT_TEXTURE
#define TINY_GL_INIT_TEXTURE(fb, tex_id) do { \
    glGenTextures(1, &(tex_id)); \
    glBindTexture(GL_TEXTURE_2D, (tex_id)); \
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); \
    glColor3f(1.0f, 1.0f, 1.0f); \
    glEnable(GL_TEXTURE_2D); \
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); \
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); \
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (fb)->width, (fb)->height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL); \
} while(0)
#endif

/* Updates OpenGL texture with current contents of frameBuffer */
#ifndef TINY_GL_UPDATE_TEXTURE
#define TINY_GL_UPDATE_TEXTURE(fb, tex_id) do { \
    glBindTexture(GL_TEXTURE_2D, (tex_id)); \
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (fb)->width, (fb)->height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, (fb)->buffer); \
} while(0)
#endif

/* Draws a screen-filling quad textured with frameBuffer */
#ifndef TINY_GL_DRAW_QUAD
#define TINY_GL_DRAW_QUAD() do { \
    glBegin(GL_QUADS); \
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f); \
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f); \
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f); \
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f); \
    glEnd(); \
} while(0)
#endif

/* 
 * Full per-frame presentation macro:
 * Updates GL texture, clears screen, draws quad, swaps buffers, polls events, and flushes frameBuffer & sceneContext.
 */
#ifndef TINY_GL_PRESENT
#define TINY_GL_PRESENT(window, fb, rc_ptr, tex_id) do { \
    TINY_GL_UPDATE_TEXTURE((fb), (tex_id)); \
    glClear(GL_COLOR_BUFFER_BIT); \
    TINY_GL_DRAW_QUAD(); \
    glfwSwapBuffers((window)); \
    glfwPollEvents(); \
    flushPixelBuffer((fb)->buffer, (fb)->width, (fb)->height); \
    if ((rc_ptr) && (rc_ptr)->scene_context) { \
        flushSceneContext((rc_ptr)->scene_context); \
    } \
} while(0)
#endif
