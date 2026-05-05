#include "../graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int format_frame_buffer(frameBuffer *fb) {
  printf("\x1B[2J");
  printf("\x1B[H");
  for (int j = 0; j < fb->width; j++) {
    for (int i = 0; i < fb->height; i++) {
      if (fb->buffer[j * fb->width + i].intensity) {
        printf("\e[32m%d\e[0m\t", fb->buffer[(j * (fb->width)) + i].intensity);
      } else {
        printf("%d\t", fb->buffer[(j * (fb->width)) + i].intensity);
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
      fb->buffer[(j * (fb->width)) + i].intensity = 0;
    }
    printf("\n");
  }
  return 0;
}

int main() {
  frameBuffer fb = {.buffer = (pixelBuffer *)calloc(50 * 50, sizeof(pixelBuffer)),
                    .height = 50,
                    .width = 50};
  // renderLine(&fb, 2, 2, 11, 12);
  renderCircle(&fb,25,25,10);
  format_frame_buffer(&fb);
  return 0;
}
