#include "_graphics.h"
#include "graphics.h"
#include "types.h"

void renderCircle(renderContext *rc, Point2 p, int r) {
  renderCircleDetached(rc,p,r);
  loadRenderedObjectToContext(rc, newCircleObject(r, 0, p, X));
  return;
}

void renderAngledCircle(renderContext *rc, Point2 p, int r, float theta, axis ax) {
  renderAngledCircleDetached(rc,p,r,theta,ax);
  loadRenderedObjectToContext(rc, newCircleObject(r, theta, p, ax));
}
