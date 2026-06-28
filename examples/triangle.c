#include "../graphics.h"
#include <stdio.h>
#include <unistd.h>

int format_frame_buffer(frameBuffer *fb) {
  printf("\x1B[2J");
  printf("\x1B[H");
  
  for (int y = 0; y < fb->height; y++) {
    for (int x = 0; x < fb->width; x++) {
      int index = (y * fb->width) + x;
      if (fb->buffer[index].color.literal) {
        printf("\e[32m%d\e[0m\t", fb->buffer[index].color.literal);
      } else {
        printf(".\t");
      }
    }
    printf("\n");
  }
  return 0;
}

int clear_frame_buffer(frameBuffer *fb) {
  for (int y = 0; y < fb->height; y++) {
    for (int x = 0; x < fb->width; x++) {
      fb->buffer[(y * fb->width) + x].color.literal = 0;
    }
  }
  return 0;
}

int main() {
  frameBuffer *fb = createFrameBuffer(50, 50);
  renderContext rc = {.frame_buffer = fb, .render_mode = FILLED};
  
  Point2 p1 = {.x = 25, .y = 0};
  Point2 p2 = {.x = 12, .y = 25};
  Point2 p3 = {.x = 50, .y = 50}; 
  
  Point2 *points[3] = { &p1, &p2, &p3 };
  
  while (1) {
	 flushPixelBuffer(fb->buffer, fb->width, fb->height);
    renderTriangle(&rc, points);
    format_frame_buffer(fb);
    usleep(100000);
  }
  
  destroyFrameBuffer(fb);
  return 0;
}
