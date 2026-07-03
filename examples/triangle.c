#include "../graphics.h"
#include <unistd.h>

int main() {
  frameBuffer* fb = createFrameBuffer(50,50,(uint16_t) 0b0000011111100000);
  renderContext rc = {.frame_buffer = fb, .render_mode = FILLED,.origin = (Index) {.x=0,.y=0}};
  
  Point2 p1 = {.x = 25, .y = 0};
  Point2 p2 = {.x = 12, .y = 25};
  Point2 p3 = {.x = 50, .y = 50}; 
  
  Point2 *points[3] = { &p1, &p2, &p3 };
  
  while (1) {
	 flushPixelBuffer(fb->buffer, fb->width, fb->height);
    renderTriangle(&rc, points);
    formatBuffer(fb);
    usleep(100000);
  }
  
	destroyFrameBuffer(fb);
	destroyContext(NULL);
  return 0;
}
