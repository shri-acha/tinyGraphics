#include "graphics.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

frameBuffer *createFrameBuffer(int width, int height) {
  pixelBuffer *buffer = calloc(width * height, sizeof(pixelBuffer));
  frameBuffer *fb = (frameBuffer *)malloc(sizeof(frameBuffer));
  fb->height = height;
  fb->width = width;
  fb->buffer = buffer;
  return fb;
}

int renderPoint(renderContext *rc, int x, int y) {
  int width = rc->frame_buffer->width;
  int height = rc->frame_buffer->height;
  if (x > width || y > height || x < 0 || y < 0) {
    return -1;
  }
  rc->frame_buffer->buffer[get_index(rc->frame_buffer, x, y)].color.literal =
      (uint16_t)0xFFFF;
  return 0;
}

int renderLine(renderContext *rc, int x1, int y1, int x2, int y2) {
  int dx = abs(x2 - x1);
  int dy = -abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;

  while (1) {
    renderPoint(rc, x1, y1);
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

/* This directly renders the line without using the conventional bressenham
 * approach, this method is usually superior to bressenham approach as it
 * doesn't move in y direction implicitly */
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

int renderCircle(renderContext *rc, int x, int y, int r) {
  int x1 = r;
  int y1 = 0;
  float t1 = r >> 4;
  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {

      renderPoint(rc, x + x1, y + y1);
      renderPoint(rc, x - x1, y + y1);
      renderPoint(rc, x + x1, y - y1);
      renderPoint(rc, x - x1, y - y1);
      renderPoint(rc, y + y1, x + x1);
      renderPoint(rc, y + y1, x - x1);
      renderPoint(rc, y - y1, x + x1);
      renderPoint(rc, y - y1, x - x1);

      y1++;
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
    break;
  case FILLED:
    while (x1 >= y1) {

      renderHorizontalLine(rc, x - x1, x + x1, y + y1);
      renderHorizontalLine(rc, x - x1, x + x1, y - y1);
      renderHorizontalLine(rc, x - y1, x + y1, y + x1);
      renderHorizontalLine(rc, x - y1, x + y1, y - x1);

      y1++;
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
    break;
  }
  return 0;
}

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

int renderAngledCircle(renderContext *rc, int x, int y, int r, float theta,
                       int axis) {
  int x1 = r;
  int y1 = 0;
  float t1 = r >> 4;

  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {

      Point2 rt_points_0 = rotate_point(x1, y1, theta, axis);
      Point2 rt_points_1 = rotate_point(-x1, y1, theta, axis);
      Point2 rt_points_2 = rotate_point(x1, -y1, theta, axis);
      Point2 rt_points_3 = rotate_point(-x1, -y1, theta, axis);
      Point2 rt_points_4 = rotate_point(y1, x1, theta, axis);
      Point2 rt_points_5 = rotate_point(y1, -x1, theta, axis);
      Point2 rt_points_6 = rotate_point(-y1, x1, theta, axis);
      Point2 rt_points_7 = rotate_point(-y1, -x1, theta, axis);

      renderPoint(rc, x + rt_points_0.x, y + rt_points_0.y);
      renderPoint(rc, x + rt_points_1.x, y + rt_points_1.y);
      renderPoint(rc, x + rt_points_2.x, y + rt_points_2.y);
      renderPoint(rc, x + rt_points_3.x, y + rt_points_3.y);
      renderPoint(rc, y + rt_points_4.x, x + rt_points_4.y);
      renderPoint(rc, y + rt_points_5.x, x + rt_points_5.y);
      renderPoint(rc, y + rt_points_6.x, x + rt_points_6.y);
      renderPoint(rc, y + rt_points_7.x, x + rt_points_7.y);

      y1++;
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
    break;
  case FILLED:
    while (x1 >= y1) {

      Point2 rt_points_0 = rotate_point(x1, y1, theta, axis);
      Point2 rt_points_1 = rotate_point(-x1, y1, theta, axis);

      Point2 rt_points_2 = rotate_point(x1, -y1, theta, axis);
      Point2 rt_points_3 = rotate_point(-x1, -y1, theta, axis);

      Point2 rt_points_4 = rotate_point(y1, x1, theta, axis);
      Point2 rt_points_5 = rotate_point(y1, -x1, theta, axis);

      Point2 rt_points_6 = rotate_point(-y1, x1, theta, axis);
      Point2 rt_points_7 = rotate_point(-y1, -x1, theta, axis);

      renderHorizontalLine(rc, x + rt_points_1.x, x + rt_points_0.x,
                           y + rt_points_0.y);
      renderHorizontalLine(rc, x + rt_points_1.x, x + rt_points_0.x,
                           y + rt_points_2.y);
      renderHorizontalLine(rc, x + rt_points_6.x, x + rt_points_4.x,
                           y + rt_points_4.y);
      renderHorizontalLine(rc, x + rt_points_7.x, x + rt_points_5.x,
                           y + rt_points_5.y);

      y1++;
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
  }

  return 0;
}

int renderTriangle(renderContext *rc, Point2 *points[3]) {

 Point2 p1 = *(points[0]);
 Point2 p2 = *(points[1]);
 Point2 p3 = *(points[2]);

 switch (rc->render_mode) {
 case WIREFRAME:
   renderLine(rc, p1.x, p1.y, p2.x, p2.y);
   renderLine(rc, p2.x, p2.y, p3.x, p3.y);
   renderLine(rc, p3.x, p3.y, p1.x, p1.y);
   break;
 case FILLED:
   sort_point2(points,3); 
   break;
 }
  return 0;
}

/* Frees the fat pointer of the framebuffer */
int destroyFrameBuffer(frameBuffer *fb) {
  free(fb->buffer);
  free(fb);
  return 0;
}

/*Flushes the pixel buffer of width * height */
int flushPixelBuffer(pixelBuffer *pb, int width, int height) {
  memset((void *)pb, 0, height * width * sizeof(pixelBuffer));
  return 0;
}
