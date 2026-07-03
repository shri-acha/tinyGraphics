#include "graphics.h"
#include "types.h"
#include <sys/types.h>

frameBuffer *createFrameBuffer(int width, int height, uint16_t color_lit) {
  pixelBuffer *buffer = calloc(width * height, sizeof(pixelBuffer));
  frameBuffer *fb = (frameBuffer *)malloc(sizeof(frameBuffer));
  fb->height = height;
  fb->width = width;
  fb->buffer = buffer;
  return fb;
}

sceneContext* newSceneContext() {
	sceneContext* sc = malloc(sizeof(sceneContext));
	sc->no_of_objs = 0;
	sc->objs = NULL;
	return sc;
}

int destroyFrameBuffer(frameBuffer *fb) {

	if (fb == NULL) {
		return -1;
	}

  free(fb->buffer);
  free(fb);
  return 0;
}

int destroyContext(renderContext* rc) {
	sceneContext* sc = rc->scene_context;
	if (sc == NULL) {
		return -1;
	}
	for (int i=0;i<sc->no_of_objs;i++) {
		free(sc->objs);
	}
	free(sc);
	free(rc);
  return 0;
}

int flushPixelBuffer(pixelBuffer *pb, int width, int height) {
  memset((void *)pb, 0, height * width * sizeof(pixelBuffer));
  return 0;
}

int flushSceneContext(sceneContext *sc){
	sc->no_of_objs = 0;
}

void formatBuffer(frameBuffer *fb) {
  for (int j = 0; j < fb->width; j++) {
    for (int i = 0; i < fb->height; i++) {
      if (fb->buffer[j * fb->width + i].color.literal) {
			uint16_t literal = fb->buffer[(j * (fb->width)) + i].color.literal;
			int r = ((literal & 0x0F80 ) >> 11 ),
				 g= ((literal & 0x07E0) >> 5 ),
				 b = (literal  & 0x001F);
			r = (r << 3) | (r >> 2);
			g = (g << 2) | (g >> 4);
			b = (b << 3) | (b >> 2);
        printf("\033[38;2;%d;%d;%dmX\033[0m\t", r,g,b);
      } else {
        printf(".\t");
      }
    }
    printf("\n");
  }
  printf("\x1B[2J");
  printf("\x1B[H");
}

Point2 rotatePoint(renderContext* rc,int x, int y, float theta, axis ax) {
  Point2 rt_pair;
  switch (ax) {
  case Z:
    rt_pair.x = (y) * sin(theta) + (-x) * cos(theta);
    rt_pair.y = (x) * sin(theta) + (y) * cos(theta);
    break;
  case X:
    rt_pair.x = x;
    rt_pair.y = (y)* cos(theta);
    break;
  case Y:
    rt_pair.x = (x) * cos(theta);
    rt_pair.y = y;
    break;
  default:
	 break;
  }
  return rt_pair;
}
