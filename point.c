#include "graphics.h"

int renderPoint(renderContext *rc, Point2 p) {
  int x = p.x+ rc->origin.x ,y = p.y + rc->origin.y;
  int width = rc->frame_buffer->width;
  int height = rc->frame_buffer->height;

  if (x > width || y > height || x < 0 || y < 0) {
    return -1;
  }
  rc->frame_buffer->buffer[get_index(rc->frame_buffer, x, y)].color.literal = (uint16_t)0xFFFF;
  return 0;
}
