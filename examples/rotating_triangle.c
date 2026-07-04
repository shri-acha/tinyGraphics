#include "../graphics.h"
#include <unistd.h>

int main() {
	frameBuffer* fb = createFrameBuffer(50,50,(uint16_t) 0b0000011111100000);
  renderContext rc = {.frame_buffer = fb, .render_mode = WIREFRAME,.origin = (Index) {.x=25,.y=25}, .scene_context=newSceneContext() };
  float theta = 0.0;

  Point2 p1 = {.x = 25, .y = 0};
  Point2 p2 = {.x = 12, .y = 25};
  Point2 p3 = {.x = 50, .y = 50}; 
  
  Point2 *points[3] = { &p1, &p2, &p3 };
  while (1) {
    renderAngledTriangle(&rc, points, theta, 2);
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
