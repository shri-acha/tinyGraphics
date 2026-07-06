#include "graphics.h"
#include "types.h"
#include <sys/types.h>
#include "_graphics.h"

frameBuffer *createFrameBuffer(int width, int height) {
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

	if ( rc == NULL ) {
		return -1;
	}
	if (rc->scene_context == NULL) {
		destroyFrameBuffer(rc->frame_buffer);
		free(rc);
	}else {
		sceneContext* sc = rc->scene_context;
		for (int i=0;i<sc->no_of_objs;i++) {
			free(sc->objs);
		}

		destroyFrameBuffer(rc->frame_buffer);
		free(rc);
		free(sc);
	}
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

int drawPixel(renderContext *rc, int x, int y, int z,Color color) {
  int width = rc->frame_buffer->width;
  int height = rc->frame_buffer->height;

  if (x >= width || y >= height || x < 0 || y < 0) {
    return -1;
  }

  rc->frame_buffer->buffer[get_index(rc->frame_buffer, x, y)].color = color;
  return 0;
}
