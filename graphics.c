#include "graphics.h"

frameBuffer *createFrameBuffer(int width, int height) {
  pixelBuffer *buffer = calloc(width * height, sizeof(pixelBuffer));
  frameBuffer *fb = (frameBuffer *)malloc(sizeof(frameBuffer));
  fb->height = height;
  fb->width = width;
  fb->buffer = buffer;
  return fb;
}

int destroyFrameBuffer(frameBuffer *fb) {
  free(fb->buffer);
  free(fb);
  return 0;
}

int flushPixelBuffer(pixelBuffer *pb, int width, int height) {
  memset((void *)pb, 0, height * width * sizeof(pixelBuffer));
  return 0;
}

void formatBuffer(frameBuffer *fb) {
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
}
