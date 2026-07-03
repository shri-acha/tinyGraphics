#include "../graphics.h"
#include <unistd.h>

int main() {
	frameBuffer* fb = createFrameBuffer(50,50,(uint16_t) 0b0000011111100000);
  renderContext rc = {.frame_buffer = fb, .render_mode = FILLED,.origin = (Index) {.x=0,.y=0}, .scene_context=newSceneContext()};

   renderCircle(&rc,(Point2){.x = 25,.y=25},10);
	formatBuffer(fb);
	destroyFrameBuffer(fb);
	destroyContext(NULL);
  return 0;
}
