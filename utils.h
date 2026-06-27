#pragma once
#include "types.h"
#include <stdlib.h>

typedef enum {
  EQ,
  GT,
  LT,
} Operator;


void set_pixel(frameBuffer *fb, pixelBuffer pb, int x, int y);

pixelBuffer get_pixel(frameBuffer *fb, int x, int y);

int compare_point2(Operator op, Point2 p1, Point2 p2);

int get_index(frameBuffer *fb, int x, int y);

int sort_point2(Point2** points,size_t points_len);
