#include "../graphics.h"
#include <unistd.h>

int main() {
	frameBuffer* fb = createFrameBuffer(25, 25);
	renderContext rc = {.frame_buffer = fb, .render_mode = WIREFRAME };
	while (1) {

		for (int j=0;j<=25;j++){
			for (int i=0;i<=20;i++){
				renderLine(&rc, (Point2){.x=i, .y=j}, (Point2){.x=i+5,.y=j});
				renderLine(&rc, (Point2){.x=j, .y=i}, (Point2){.x=j,.y=i+5});
				formatBuffer(fb);
				usleep(1e5);
				flushPixelBuffer(fb->buffer,fb->width,fb->height);
			}
		}
	}

	destroyFrameBuffer(fb);
	return 0;
}
