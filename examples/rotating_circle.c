#include "../graphics.h"
#include <math.h>
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
  float theta = 0.0;
  // renderLine(fb, 2, 2, 11, 12);
  while (1) {
    renderAngledCircle(fb, 25, 25, 10, theta);
    format_frame_buffer(fb);
    usleep(1e5);
    flushPixelBuffer(fb->buffer, fb->width, fb->height);
    theta += M_PI / 12;
  }
  destroyFrameBuffer(fb);
  return 0;
}
