#include "../graphics.h"
#include <stdio.h>
#include <unistd.h>
// Prints the frame buffer
int format_frame_buffer(frameBuffer *fb) {
	for (int j = 0; j < fb->width; j++) {
		for (int i = 0; i < fb->height; i++) {
			if (fb->buffer[j * fb->width + i].color.literal) {
				printf("\e[32m%d\e[0m\t", fb->buffer[(j * (fb->width)) + i].color.literal);
			} else {
				printf("%d\t", fb->buffer[(j * (fb->width)) + i].color.literal);
			}
		}
		printf("\n");
	}
  printf("\x1B[2J");
  printf("\x1B[H");
	return 0;
}

int clear_frame_buffer(frameBuffer *fb) {
	printf("\n");
	for (int j = 0; j < fb->width; j++) {
		for (int i = 0; i < fb->height; i++) {
			fb->buffer[(j * (fb->width)) + i].color.literal = 0;
		}
		printf("\n");
	}
	return 0;
}

int main() {
	frameBuffer* fb = createFrameBuffer(25, 25);
	renderContext rc = {.frame_buffer = fb, .render_mode = WIREFRAME };
	while (1) {

		for (int j=0;j<=25;j++){
			for (int i=0;i<=20;i++){
				renderLine(&rc, (Point2){.x=i, .y=j}, (Point2){.x=i+5,.y=j});
				renderLine(&rc, (Point2){.x=j, .y=i}, (Point2){.x=j,.y=i+5});
				format_frame_buffer(fb);
				usleep(1e5);
				flushPixelBuffer(fb->buffer,fb->width,fb->height);
			}
		}
	}

	destroyFrameBuffer(fb);
	return 0;
}
