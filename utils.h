#pragma once
#include "types.h"
#include <stdlib.h>
#include <unitypes.h>
#include <math.h>

typedef enum {
  EQ,
  GT,
  LT,
} Operator;


void set_pixel(frameBuffer *fb, pixelBuffer pb, int x, int y);

pixelBuffer get_pixel(frameBuffer *fb, int x, int y);

int compare_value(Operator op, int p1, int p2);

int get_index(frameBuffer *fb, int x, int y);

int sort_point2(Point2** points,size_t points_len);

Point2 rotate_point(int x, int y, float theta, axis ax); 
