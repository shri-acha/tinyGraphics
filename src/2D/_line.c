#include "../_graphics.h"

void _renderLine2D(renderContext *rc,Point2 p1, Point2 p2,Color color){
  int x1 = p1.x,y1 = p1.y;
  int x2 = p2.x,y2 = p2.y;

  int dx = abs(x2 - x1);
  int dy = -abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;

  while (1) {
    _renderPoint2D(rc,(Point2){.x=x1,.y=y1},color);
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
}
void _renderHorizontalLine2D(renderContext *rc, int x1, int x2, int y, Color color) {
  if (y < 0 || y >= rc->frame_buffer->height) return;
  if (x1 > x2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
  }
  int width = rc->frame_buffer->width;
  if (x2 < 0 || x1 >= width) return;

  if (x1 < 0) x1 = 0;
  if (x2 >= width) x2 = width - 1;

  for (int i = x1; i <= x2; i++) {
    _renderPoint2D(rc, (Point2){.x=i, .y=y}, color);
  }
}
