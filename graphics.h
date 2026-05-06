#include <stdint.h>

typedef struct { uint16_t literal; } Color; // 00000(R) 000000(G) 00000(B)

typedef struct
__attribute__((__packed__))
{
  Color color;
} pixelBuffer;

typedef struct {
  int width;
  int height;
  pixelBuffer *buffer;
} frameBuffer;

/*Renders a point at (x,y)*/
int renderPoint(frameBuffer *fb, int x, int y);

/*Renders line from (x1,y1) to (x2,y2)*/
int renderLine(frameBuffer *fb, int x1, int y1, int x2, int y2);

/*
 * Renders a circle of radius r with center at (x,y).
 * Uses Jesko's method to reduce the number of overall operations,
 * optimizing for low performance systems
 */
int renderCircle(frameBuffer *fb, int x, int y, int r);

/*Returns an equivalent index of (x,y) in a linear buffer*/
int get_index(frameBuffer *fb, int x, int y);

/*Returns an instance of frameBuffer with width*height*/
frameBuffer *createFrameBuffer(int width, int height);

/*Destroys an instance of the frameBuffer*/
int destroyFrameBuffer(frameBuffer* fb);
