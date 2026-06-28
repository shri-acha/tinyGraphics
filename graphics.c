#include "graphics.h"

frameBuffer *createFrameBuffer(int width, int height) {
  pixelBuffer *buffer = calloc(width * height, sizeof(pixelBuffer));
  frameBuffer *fb = (frameBuffer *)malloc(sizeof(frameBuffer));
  fb->height = height;
  fb->width = width;
  fb->buffer = buffer;
  return fb;
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
