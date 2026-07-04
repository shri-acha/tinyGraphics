#include "../graphics.h"
#include <unistd.h>

int main() {
	frameBuffer* fb = createFrameBuffer(100,100,(uint16_t) 0b0000011111100000);
  renderContext rc = {.frame_buffer = fb, .render_mode = WIREFRAME,.origin = (Index) {.x=50,.y=50}, .scene_context=newSceneContext() };
  float theta = 0.0;

  Point2 p1 = {.x = 25, .y = 0};
  Point2 p2 = {.x = 12, .y = 25};
  Point2 p3 = {.x = 50, .y = 50}; 
  
  Point2 *points[3] = { &p1, &p2, &p3 };

  int windowShouldClose = 0;
  while (!windowShouldClose) {
    renderAngledTriangle(&rc, points, theta, Y);
	 printf("No of objects in the scene:\t %d\n",rc.scene_context->no_of_objs);
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
