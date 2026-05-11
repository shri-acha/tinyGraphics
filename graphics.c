#include "graphics.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int get_index(frameBuffer *fb, int x, int y) {
  if (x == 0 && y <= 1) {
    return 0;
  }
  return (y - 1) * fb->width + (x - 1);
}

frameBuffer *createFrameBuffer(int width, int height) {
  pixelBuffer *buffer = calloc(width * height, sizeof(pixelBuffer));
  frameBuffer *fb = (frameBuffer *)malloc(sizeof(frameBuffer));
  fb->height = height;
  fb->width = width;
  fb->buffer = buffer;
  return fb;
}

int renderPoint(frameBuffer *fb, int x, int y) {
  if (x > fb->width || y > fb->height || x < 0 || y < 0) {
    return -1;
  }
  fb->buffer[get_index(fb, x, y)].color.literal = (uint16_t)0xFFFFFFFFFFFFFFFF;
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

int renderCircle(frameBuffer *fb, int x, int y, int r) {
  int x1 = r;
  int y1 = 0;
  float t1 = r >> 4;

  while (x1 > y1) {

    renderPoint(fb, x + x1, y + y1);
    renderPoint(fb, x - x1, y + y1);
    renderPoint(fb, x + x1, y - y1);
    renderPoint(fb, x - x1, y - y1);
    renderPoint(fb, y + y1, x + x1);
    renderPoint(fb, y + y1, x - x1);
    renderPoint(fb, y - y1, x + x1);
    renderPoint(fb, y - y1, x - x1);

    y1++;
    t1 += y1;
    float t2 = t1 - x1;
    if (t2 >= 0) {
      t1 = t2;
      x1--;
    }
  }
  return 0;
}

typedef struct {
  int x;
  int y;
} Point2;

Point2 rotate_point(int x, int y, float theta, int axis) {
  Point2 rt_pair;
  switch (axis) {
  case 0:
    rt_pair.x = y * sin(theta) + x * cos(theta);
    rt_pair.y = -x * sin(theta) + y * cos(theta);
    break;
  case 1:
    rt_pair.x = x;
    rt_pair.y = y * cos(theta);
    break;
  case 2:
    rt_pair.x = x * cos(theta);
    rt_pair.y = y;
    break;
  default:
  }
  return rt_pair;
}

int renderAngledCircle(frameBuffer *fb, int x, int y, int r, float theta,
                       int axis) {
  int x1 = r;
  int y1 = 0;
  float t1 = r >> 4;

  while (x1 > y1) {

    Point2 rt_points_0 = rotate_point(x1, y1, theta, axis);
    Point2 rt_points_1 = rotate_point(-x1, y1, theta, axis);
    Point2 rt_points_2 = rotate_point(x1, -y1, theta, axis);
    Point2 rt_points_3 = rotate_point(-x1, -y1, theta, axis);
    Point2 rt_points_4 = rotate_point(y1, x1, theta, axis);
    Point2 rt_points_5 = rotate_point(y1, -x1, theta, axis);
    Point2 rt_points_6 = rotate_point(-y1, x1, theta, axis);
    Point2 rt_points_7 = rotate_point(-y1, -x1, theta, axis);

    renderPoint(fb, x + rt_points_0.x, y + rt_points_0.y);
    renderPoint(fb, x + rt_points_1.x, y + rt_points_1.y);
    renderPoint(fb, x + rt_points_2.x, y + rt_points_2.y);
    renderPoint(fb, x + rt_points_3.x, y + rt_points_3.y);
    renderPoint(fb, y + rt_points_4.x, x + rt_points_4.y);
    renderPoint(fb, y + rt_points_5.x, x + rt_points_5.y);
    renderPoint(fb, y + rt_points_6.x, x + rt_points_6.y);
    renderPoint(fb, y + rt_points_7.x, x + rt_points_7.y);

    y1++;
    t1 += y1;
    float t2 = t1 - x1;
    if (t2 >= 0) {
      t1 = t2;
      x1--;
    }
  }
  return 0;
}

int destroyFrameBuffer(frameBuffer *fb) {
  free(fb->buffer);
  free(fb);
  return 0;
}
/*Sets the value of the pixel for the pixelBuffer at (x,y) */
void set_pixel(frameBuffer *fb, pixelBuffer pb, int x, int y) {
  fb->buffer[get_index(fb, x, y)] = pb;
}

/*Gets the value of the pixel for the pixelBuffer at (x,y) */
pixelBuffer get_pixe(frameBuffer *fb, int x, int y) {
  return fb->buffer[get_index(fb, x, y)];
}

/*Flushes the pixel buffer of width * height*/
int flushPixelBuffer(pixelBuffer *pb, int width, int height) {
  memset((void *)pb, 0, height * width * sizeof(pixelBuffer));
  return 0;
}
