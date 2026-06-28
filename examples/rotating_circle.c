#include "../graphics.h"
#include <unistd.h>

int main() {
  frameBuffer *fb = createFrameBuffer(50, 50);
  renderContext rc = {.frame_buffer = fb, .render_mode = FILLED };
  float theta = 0.0;
  // renderLine(fb, 2, 2, 11, 12);
  while (1) {
    renderAngledCircle(&rc, (Point2){.x = 25,.y=25}, 10, theta, 2);
    formatBuffer(fb);
    usleep(1e5);
    flushPixelBuffer(fb->buffer, fb->width, fb->height);
    theta += M_PI / 12;
  }
  destroyFrameBuffer(fb);
  return 0;
}
