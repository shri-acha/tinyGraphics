#pragma once
#include "types.h"
#include "utils.h"
#include "vendor/fast_obj/fast_obj.h"
#include "./gl_ext.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*Renders a point at (x,y)*/
void renderPoint2D(renderContext *rc, Point2 p,Color color);

/*Renders line from (x1,y1) to (x2,y2)*/
void renderLine2D(renderContext *rc, Point2 p1, Point2 p2,Color color);

/*Renders line from (x1,y1,z1) to (x2,y2,z2) with a projection*/
void renderLine3D(renderContext *rc, Point3 p1, Point3 p2,Color color);

/* This directly renders the line without using the conventional bressenham
 * approach, this method is usually superior to bressenham approach as it
 * doesn't move in y direction implicitly */
void renderHorizontalLine2D(renderContext *rc, int x1, int x2, int y, Color color);

/*
 * Renders a circle of radius r with center at (x,y).
 * Uses Jesko's method to reduce the number of overall operations,
 * optimizing for low performance systems
 */
void renderCircle2D(renderContext *rc, Point2 p, int r, Color color);

/*
 * Renders a circle of radius r with center at (x,y,z).
 */
void renderCircle3D(renderContext *rc, Point3 p, int r, Color color);

/*
 * Renders a circle of radius r with center at (x,y) rotated about the axis
 * through its center at theta degrees.
 */
void renderAngledCircle2D(renderContext *rc, Point2 p, int r, float theta,
                       axis ax,Color color);

/*
 * Renders a circle of radius r with center at (x,y,z) rotated about the axis
 * through its center at theta degrees.
 */
void renderAngledCircle3D(renderContext *rc, Point3 p, int r, float theta, axis ax,Color color);

/*Renders a triangle*/
void renderTriangle2D(renderContext *rc, Point2* points[3],Color color);

/*Renders a triangle in 3D space*/
void renderTriangle3D(renderContext *rc, Point3* points[3],Color color);

/*Renders a triangle at an angle of theta with axis*/
void renderAngledTriangle2D(renderContext *rc,Point2 *points[3], float theta, axis ax,Color color);


/*Renders a triangle in 3D space at an angle of theta with axis*/
void renderAngledTriangle3D(renderContext *rc,Point3 *points[3], float theta, axis ax,Color color);

/*Returns an equivalent index of (x,y) in a linear buffer*/
int get_index(frameBuffer *fb, int x, int y);


/*Returns the color at the index of (x,y) from the frame buffer*/
pixelBuffer get_pixel(frameBuffer *fb, int x, int y);

/*Flushes the pixel buffer of width * height */
int flushPixelBuffer(pixelBuffer *pb, int width, int height);

/*Flushes the depth buffer of width * height */
int flushDepthBuffer(int *depth_buffer, int width, int height);

/*Flushes the sceneContext*/
int flushSceneContext(sceneContext* sc);

/*Returns an instance of frameBuffer with width*height with 5R6G5B color*/
frameBuffer *createFrameBuffer(int width, int height);

/* Frees the fat pointer of the framebuffer */
int destroyFrameBuffer(frameBuffer *fb);

/* Frees all the context associated memory */
int destroyContext(renderContext* rc);

/* Outputs the pixelBuffer into the stdout*/
void formatBuffer(frameBuffer *fb);

/*Sets the origin within the context of render*/
void set_origin(renderContext* rc, Point2 origin);

/*Rotates a the given point about a given axis given the renderContext containing the origin*/
Point2 rotatePoint2D(renderContext* rc,int x, int y, float theta, axis ax); 

/* Allocates and initializes a new sceneContext with a list of renderedObjects */
sceneContext* newSceneContext();

void renderMesh3D(renderContext *rc, fastObjMesh *mesh, Point3 offset, Color color, float scale);
