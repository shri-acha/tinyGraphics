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

/*Renders a point at (x,y)*/
int renderPoint (frameBuffer* fb, int x, int y);

/*Renders line from (x1,y1) to (x2,y2)*/
int renderLine (frameBuffer* fb, int x1, int y1,int x2,int y2);

/*Renders a circle of radius r with center at (x,y)*/
int renderCircle (frameBuffer* fb, int x, int y,int r);

/*Returns an equivalent index of (x,y) in a linear buffer*/
int get_index(frameBuffer* fb,int x,int y);

