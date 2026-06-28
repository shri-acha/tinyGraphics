#include "../graphics.h"
#include <unistd.h>

int main() {
  frameBuffer* fb = createFrameBuffer(50, 50);
  renderContext rc = {.frame_buffer = fb, .render_mode = FILLED };

  renderCircle(&rc,(Point2){.x = 25,.y=25},10);
  formatBuffer(fb);
  destroyFrameBuffer(fb);
  return 0;
}
