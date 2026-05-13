#include <stdint.h>

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

/*Renders a point at (x,y)*/
int renderPoint(renderContext *rc, int x, int y);

/*Renders line from (x1,y1) to (x2,y2)*/
int renderLine(renderContext *rc, int x1, int y1, int x2, int y2);

/*
 * Renders a circle of radius r with center at (x,y).
 * Uses Jesko's method to reduce the number of overall operations,
 * optimizing for low performance systems
 */
int renderCircle(renderContext *rc, int x, int y, int r);

/*
 * Renders a circle of radius r with center at (x,y) rotated about the axis
 * (0(z),1(x),2(y)) through its center at theta degrees.
 */
int renderAngledCircle(renderContext *rc, int x, int y, int r, float theta,
                       int axis);

/*Returns an equivalent index of (x,y) in a linear buffer*/
int get_index(frameBuffer *fb, int x, int y);

/*Sets the color of the pixel at index (x,y) of the frame buffer*/
void set_pixel(frameBuffer *fb, pixelBuffer pb, int x, int y);

/*Returns the color at the index of (x,y) from the frame buffer*/
pixelBuffer get_pixel(frameBuffer *fb, int x, int y);

/*Flushes the pixelBuffer data*/
int flushPixelBuffer(pixelBuffer *pb, int width, int height);

/*Returns an instance of frameBuffer with width*height*/
frameBuffer *createFrameBuffer(int width, int height);

/*Destroys an instance of the frameBuffer*/
int destroyFrameBuffer(frameBuffer *fb);
