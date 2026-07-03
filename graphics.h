#pragma once
#include "types.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>

/*Renders a point at (x,y)*/
int renderPoint(renderContext *rc, Point2 p);

/*Renders line from (x1,y1) to (x2,y2)*/
void renderLine(renderContext *rc, Point2 p1, Point2 p2);

/* This directly renders the line without using the conventional bressenham
 * approach, this method is usually superior to bressenham approach as it
 * doesn't move in y direction implicitly */
void renderHorizontalLine(renderContext *rc, int x1, int x2, int y);

/*
 * Renders a circle of radius r with center at (x,y).
 * Uses Jesko's method to reduce the number of overall operations,
 * optimizing for low performance systems
 */
void renderCircle(renderContext *rc, Point2 p, int r);

/*
 * Renders a circle of radius r with center at (x,y) rotated about the axis
 * through its center at theta degrees.
 */
void renderAngledCircle(renderContext *rc, Point2 p, int r, float theta,
                       axis ax);
/*Renders a triangle*/
void renderTriangle(renderContext *rc, Point2* points[3]);

/*Renders a triangle at an angle of theta with axis*/
void renderAngledTriangle(renderContext *rc,Point2 *points[3], float theta, axis ax);

/*Returns an equivalent index of (x,y) in a linear buffer*/
int get_index(frameBuffer *fb, int x, int y);

/*Sets the color of the pixel at index (x,y) of the frame buffer*/
void set_pixel(frameBuffer *fb, pixelBuffer pb, int x, int y);

/*Returns the color at the index of (x,y) from the frame buffer*/
pixelBuffer get_pixel(frameBuffer *fb, int x, int y);

/*Flushes the pixel buffer of width * height */
int flushPixelBuffer(pixelBuffer *pb, int width, int height);

/*Flushes the sceneContext*/
int flushSceneContext(sceneContext* sc);

/*Returns an instance of frameBuffer with width*height with 5R6G5B color*/
frameBuffer *createFrameBuffer(int width, int height,uint16_t color);

/* Frees the fat pointer of the framebuffer */
int destroyFrameBuffer(frameBuffer *fb);

/* Frees all the context associated memory */
int destroyContext(renderContext* rc);

/* Outputs the pixelBuffer into the stdout*/
void formatBuffer(frameBuffer *fb);

/*Sets the origin within the context of render*/
void set_origin(renderContext* rc, Point2 origin);

/*Rotates a the given point about a given axis given the renderContext containing the origin*/
Point2 rotatePoint(renderContext* rc,int x, int y, float theta, axis ax); 

/* Allocates and initializes a new sceneContext with a list of renderedObjects */
sceneContext* newSceneContext();
