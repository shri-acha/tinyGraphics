#include "utils.h"
#include "types.h"

/*Sets the value of the pixel for the pixelBuffer at (x,y) */
void set_pixel(frameBuffer *fb, pixelBuffer pb, int x, int y) {
  fb->buffer[get_index(fb, x, y)] = pb;
}

/*Gets the value of the pixel for the pixelBuffer at (x,y) */
pixelBuffer get_pixel(frameBuffer *fb, int x, int y) {
  return fb->buffer[get_index(fb, x, y)];
}

/* General comparison operations for Point2 types */ 
int compare_point2(Operator op, Point2 p1, Point2 p2) {
  switch (op) {
  case EQ:
    if (p1.x == p2.x && p1.y == p2.y) {
      return 0;
    }
    break;
  case GT:
    if (p1.x > p2.x && p1.y > p2.y) {
      return 0;
    }
    break;
  case LT:
    if (p1.x < p2.x && p1.y < p2.y) {
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
