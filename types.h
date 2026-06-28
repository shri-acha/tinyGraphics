#pragma once
#include <stdint.h>

typedef struct {
  int x;
  int y;
} Point2;

typedef enum {
	X,
	Y,
	Z,
}axis;

typedef struct {
  uint16_t literal;
} Color; // 00000(R) 000000(G) 00000(B)

typedef struct __attribute__((__packed__)) {
  Color color;
} pixelBuffer;

typedef struct {
  int width;
  int height;
  pixelBuffer *buffer;
} frameBuffer;

typedef enum {
  WIREFRAME,
  FILLED,
} renderMode;

typedef struct {
  frameBuffer *frame_buffer;
  renderMode render_mode;
} renderContext;
