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


pixelBuffer get_pixel(frameBuffer *fb, int x, int y);

int compare_value(Operator op, int p1, int p2);

int get_index(frameBuffer *fb, int x, int y);

int sort_point2(Point2** points,size_t points_len);
