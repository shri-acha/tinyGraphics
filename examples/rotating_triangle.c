#include "../graphics.h"
#include <stdio.h>
#include <unistd.h>

int format_frame_buffer(frameBuffer *fb) {
  for (int j = 0; j < fb->width; j++) {
    for (int i = 0; i < fb->height; i++) {
      if (fb->buffer[j * fb->width + i].color.literal) {
        printf("\e[32m%d\e[0m\t",
               fb->buffer[(j * (fb->width)) + i].color.literal);
      } else {
        printf("%d\t", fb->buffer[(j * (fb->width)) + i].color.literal);
      }
    }
    printf("\n");
  }
  printf("\x1B[2J");
  printf("\x1B[H");
  return 0;
}

int clear_frame_buffer(frameBuffer *fb) {
  printf("\n");
  for (int j = 0; j < fb->width; j++) {
    for (int i = 0; i < fb->height; i++) {
      fb->buffer[(j * (fb->width)) + i].color.literal = 0;
    }
    printf("\n");
  }
  return 0;
}

int main() {
  frameBuffer *fb = createFrameBuffer(50, 50);
  renderContext rc = {.frame_buffer = fb, .render_mode = FILLED };
  float theta = 0.0;

  Point2 p1 = {.x = 25, .y = 0};
  Point2 p2 = {.x = 12, .y = 25};
  Point2 p3 = {.x = 50, .y = 50}; 
  
  Point2 *points[3] = { &p1, &p2, &p3 };
  while (1) {
    renderAngledTriangle(&rc, points, theta, 1);
    format_frame_buffer(fb);
    usleep(1e5);
    flushPixelBuffer(fb->buffer, fb->width, fb->height);
    theta += M_PI / 12;
  }
  destroyFrameBuffer(fb);
  return 0;
}
