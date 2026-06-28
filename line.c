#include "graphics.h"

int renderLine(renderContext *rc, Point2 p1, Point2 p2) {

  int x1 = p1.x,y1 = p1.y;
  int x2 = p2.x,y2 = p2.y;

  int dx = abs(x2 - x1);
  int dy = -abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;

  while (1) {
    renderPoint(rc,(Point2){.x=x1,.y=y1});
    if (x1 == x2 && y1 == y2)
      break;
    int e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x1 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y1 += sy;
    }
  }
  return 0;
}

void renderHorizontalLine(renderContext *rc, int x1, int x2, int y) {
  if (x1 > x2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
  }

  if (y < 0 || y >= rc->frame_buffer->height)
    return;

  if (x1 < 0)
    x1 = 0;
  if (x2 >= rc->frame_buffer->width)
    x2 = rc->frame_buffer->width - 1;

  pixelBuffer *ptr =
      &rc->frame_buffer->buffer[y * rc->frame_buffer->width + x1];
  for (int i = x1; i <= x2; i++) {
    ptr->color.literal = 0xFFFF;
    ptr++;
  }
}
