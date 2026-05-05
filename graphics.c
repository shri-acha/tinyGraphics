#include "graphics.h"
#include <stdint.h>
#include <stdlib.h>

int get_index(frameBuffer *fb, int x, int y) {
  if (x == 0 && y <= 1) {
    return 0;
  }
  return (y - 1) * fb->width + (x - 1);
}

frameBuffer *createFrameBuffer(int width, int height) {
  uint16_t *buffer = calloc(width * height, sizeof(pixelBuffer));
  frameBuffer *fb = (frameBuffer *)malloc(sizeof(frameBuffer));
  return fb;
}

int renderPoint(frameBuffer *fb, int x, int y) {
  if (x > fb->width || y > fb->height || x < 0 || y < 0) {
    return -1;
  }
  fb->buffer[get_index(fb, x, y)].intensity = 1;
  return 0;
}

int renderLine(frameBuffer *fb, int x1, int y1, int x2, int y2) {
  if (x1 > fb->width || y1 > fb->height || x2 > fb->width || y2 > fb->height ||
      x2 < 0 || y2 < 0) {
    return -1;
  }
  float delta_x = x2 - x1;
  float delta_y = y2 - y1;
  float D = 0;
  float y = y1;
  float x = x1;
  if (delta_x > delta_y) {
    D = 2 * delta_y - delta_x;
    for (int i = x1; i <= x2; i++) {
      renderPoint(fb, i, y);
      if (D > 0) {
        y++;
        D += (2 * (delta_y - delta_x));
      } else {
        D = D + 2 * delta_y;
      }
    }
  } else {
    D = 2 * delta_x - delta_y;
    for (int i = y1; i <= y2; i++) {
      renderPoint(fb, x, i);
      if (D > 0) {
        x++;
        D += (2 * (delta_x - delta_y));
      } else {
        D = D + 2 * delta_x;
      }
    }
  }
  return 0;
}

int renderOctant(frameBuffer *fb, int x, int y, int r) {
  int x1 = r;
  int y1 = 0;
  float t1 = r << 4;

  while (x1 > y1) {

    renderPoint(fb, x + x1, y + y1);
    renderPoint(fb, x - x1, y + y1);
    renderPoint(fb, x + x1, y - y1);
    renderPoint(fb, x - x1, y - y1);

    y1++;
    t1 += y1;
    float t2 = t1 - x1;
    if (t2 >= 0) {
      t1 = t2;
      x1--;
    }
  }

  x1 = 0;
  y1 = r;
  t1 = r << 4;

  while (x1 < y1) {

    renderPoint(fb, x + x1, y + y1);
    renderPoint(fb, x - x1, y + y1);
    renderPoint(fb, x + x1, y - y1);
    renderPoint(fb, x - x1, y - y1);

    x1++;
    t1 += y1;
    float t2 = t1 - x1;
    if (t2 >= 0) {
      t1 = t2;
      y1--;
    }
  }
  return 0;
}

int renderCircle(frameBuffer *fb, int x, int y, int r) {

  /*   .
   * (   )
   *   .
   * */
  renderOctant(fb, x, y, r);
  return 0;
}
