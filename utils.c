#include "utils.h"
#include "types.h"
#include <stdlib.h>

/*Sets the value of the pixel for the pixelBuffer at (x,y) */
void set_pixel(frameBuffer *fb, pixelBuffer pb, int x, int y) {
  fb->buffer[get_index(fb, x, y)] = pb;
}

/*Gets the value of the pixel for the pixelBuffer at (x,y) */
pixelBuffer get_pixel(frameBuffer *fb, int x, int y) {
  return fb->buffer[get_index(fb, x, y)];
}

/* General comparison operations*/
int compare_value(Operator op, int p1, int p2) {
  switch (op) {
  case EQ:
    if (p1 == p2) {
      return 0;
    }
    break;
  case GT:
    if (p1 > p2) {
      return 0;
    }
    break;
  case LT:
    if (p1 < p2) {
      return 0;
    }
    break;
  default:
    return 1;
  }
  return 1;
}

int get_index(frameBuffer *fb, int x, int y) {
  if (x < 0 || x >= fb->width || y < 0 || y >= fb->height)
    return -1;
  return y * fb->width + x;
}

int sort_point2(Point2 **points, size_t points_len) {
  Point2 temp;
  for (int i = 0; i < points_len - 2; ++i) {
    for (int j = 1; j < points_len - 1; ++j) {
      if (compare_value(GT, (*points[j]).y, (*points[i]).y)) {
        temp = *points[i];
        points[i] = points[i + 1];
        *points[i + 1] = temp;
      }
    }
  }
  return 0;
}

Point2 rotate_point(int x, int y, float theta, axis ax) {
  Point2 rt_pair;
  switch (ax) {
  case Z:
    rt_pair.x = y * sin(theta) + x * cos(theta);
    rt_pair.y = -x * sin(theta) + y * cos(theta);
    break;
  case X:
    rt_pair.x = x;
    rt_pair.y = y * cos(theta);
    break;
  case Y:
    rt_pair.x = x * cos(theta);
    rt_pair.y = y;
    break;
  default:
	 break;
  }
  return rt_pair;
}
