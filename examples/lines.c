#include "../graphics.h"
#include <stdio.h>
#include <unistd.h>

int main() {
	frameBuffer* fb = createFrameBuffer(25, 25,(uint16_t) 0b0000011111100000);
	renderContext rc = {.frame_buffer = fb, .render_mode = WIREFRAME, .origin = (Index) {.x=12,.y=12} ,.scene_context=newSceneContext()};
	while (1) {

		for (int j=0;j<=25;j++){
			for (int i=0;i<=20;i++){
				renderLine(&rc, (Point2){.x=i, .y=j}, (Point2){.x=i+5,.y=j});
				renderLine(&rc, (Point2){.x=j, .y=i}, (Point2){.x=j,.y=i+5});
				formatBuffer(fb);
				printf("%d",rc.scene_context->no_of_objs);
				usleep(1e6);
				flushPixelBuffer(fb->buffer,fb->width,fb->height);
				flushSceneContext(rc.scene_context);
			}
		}
	}

	destroyFrameBuffer(fb);
	destroyContext(NULL);
	return 0;
}
