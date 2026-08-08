#include "utils.h"
#include "types.h"
#include <stdlib.h>

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
  for (size_t i = 0; i < points_len - 1; ++i) {
    for (size_t j = 0; j < points_len - 1 - i; ++j) {
      if (!compare_value(GT, points[j]->y, points[j + 1]->y)) {
        Point2 *temp = points[j];
        points[j] = points[j + 1];
        points[j + 1] = temp;
      }
    }
  }
  return 0;
}

Vector4 point_to_vector(Point3 p, int w) {
	return (Vector4) {
		.inner = { (float)p.x, (float)p.y, (float)p.z, (float)w }
	};
} 

