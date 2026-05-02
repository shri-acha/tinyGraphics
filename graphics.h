#include <stdint.h>

typedef struct {
   int R;
   int G;
   int B;
}Color;

typedef struct {
  Color color;
  uint16_t intensity; 
}pixelBuffer;

typedef struct {
  int width;
  int height;
  pixelBuffer* buffer;
}frameBuffer;

int renderPoint (frameBuffer* fb, int x, int y);

int renderLine (frameBuffer* fb, int x1, int y1,int x2,int y2);

int renderCircle (frameBuffer* fb, int x, int y,int r);

int get_index(frameBuffer* fb,int x,int y);

