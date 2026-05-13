#include "../graphics.h"
#include <stdio.h>
#include <unistd.h>

int format_frame_buffer(frameBuffer *fb) {
  for (int j = 0; j < fb->width; j++) {
    for (int i = 0; i < fb->height; i++) {
      if (fb->buffer[j * fb->width + i].color.literal) {
        printf("\e[32m%d\e[0m\t", fb->buffer[(j * (fb->width)) + i].color.literal);
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
  frameBuffer* fb = createFrameBuffer(50, 50);
  renderContext rc = {.frame_buffer = fb, .render_mode = FILLED };
  // renderLine(fb, 2, 2, 11, 12);
  renderCircle(&rc,25,25,10);
  format_frame_buffer(fb);
  destroyFrameBuffer(fb);
  return 0;
}
