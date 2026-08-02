#include "../graphics.h"
#include <unistd.h>

int main() {
	frameBuffer* fb = createFrameBuffer(100,100);
   renderContext rc = {.frame_buffer = fb, .render_mode = FILLED ,.origin = (Index) {.x=50,.y=50},.scene_context = newSceneContext()};
   float theta = 0.0;
  while (1) {
	 renderLine2D(&rc,(Point2){.x=0,.y=-25},(Point2){.x=0,.y=25},(Color){.literal=(uint16_t) 0b0000011111100000});
    renderAngledCircle2D(&rc, (Point2){.x = 12,.y=13}, 5, theta, Y,(Color){.literal=(uint16_t) 0b0000011111100000});
	 printf("%d\n",rc.scene_context->no_of_objs);
    formatBuffer(fb);
    usleep(1e5);
    flushPixelBuffer(fb->buffer, fb->width, fb->height);
	 flushSceneContext(rc.scene_context);
    theta += M_PI / 12;
  }
	destroyFrameBuffer(fb);
	destroyContext(NULL);
  return 0;
}
