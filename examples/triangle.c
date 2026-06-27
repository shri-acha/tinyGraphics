#include "../graphics.h"
#include <stdio.h>
#include <unistd.h>

int format_frame_buffer(frameBuffer *fb) {

  printf("\x1B[2J");
  printf("\x1B[H");
  
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
  frameBuffer *fb = createFrameBuffer(51, 51);
  renderContext rc = {.frame_buffer = fb, .render_mode = FILLED};
  Point2 p1 = {.x = 25, .y = 0};
  Point2 p2 = {.x = 12, .y = 25};
  Point2 p3 = {.x = 50, .y = 25};
  Point2 *points[3] = { &p1, &p2, &p3};
  while (1) {
	 flushPixelBuffer(fb->buffer, fb->width, fb->height);
    renderTriangle(&rc, points);
	 format_frame_buffer(fb);
    usleep(1e5);
  }
  destroyFrameBuffer(fb);
  return 0;
}
