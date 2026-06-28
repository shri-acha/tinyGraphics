#pragma once
#include "types.h"
#include "utils.h"
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>

/*Renders a point at (x,y)*/
int renderPoint(renderContext *rc, Point2 p);

/*Renders line from (x1,y1) to (x2,y2)*/
int renderLine(renderContext *rc, Point2 p1, Point2 p2);

/* This directly renders the line without using the conventional bressenham
 * approach, this method is usually superior to bressenham approach as it
 * doesn't move in y direction implicitly */
void renderHorizontalLine(renderContext *rc, int x1, int x2, int y);

/*
 * Renders a circle of radius r with center at (x,y).
 * Uses Jesko's method to reduce the number of overall operations,
 * optimizing for low performance systems
 */
int renderCircle(renderContext *rc, Point2 p, int r);

/*
 * Renders a circle of radius r with center at (x,y) rotated about the axis
 * (0(z),1(x),2(y)) through its center at theta degrees.
 */
int renderAngledCircle(renderContext *rc, Point2 p, int r, float theta,
                       int axis);
/*Renders a triangle*/
int renderTriangle(renderContext *rc, Point2* points[3]);

/*Renders a triangle at an angle of theta with axis*/
int renderAngledTriangle(renderContext *rc,Point2 *points[3], float theta, int axis);

/*Returns an equivalent index of (x,y) in a linear buffer*/
int get_index(frameBuffer *fb, int x, int y);

/*Sets the color of the pixel at index (x,y) of the frame buffer*/
void set_pixel(frameBuffer *fb, pixelBuffer pb, int x, int y);

/*Returns the color at the index of (x,y) from the frame buffer*/
pixelBuffer get_pixel(frameBuffer *fb, int x, int y);

/*Renders a point at (x,y)*/
Point2 rotate_point(int x, int y,float theta,int axis);

/*Flushes the pixelBuffer data*/
int flushPixelBuffer(pixelBuffer *pb, int width, int height);

/*Returns an instance of frameBuffer with width*height*/
frameBuffer *createFrameBuffer(int width, int height);

/*Destroys an instance of the frameBuffer*/
int destroyFrameBuffer(frameBuffer *fb);
