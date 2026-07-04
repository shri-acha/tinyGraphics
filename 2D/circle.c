#include "../_graphics.h"
#include "../graphics.h"
#include "../types.h"

void renderCircle(renderContext *rc, Point2 p, int r, Color color) {
  renderCircleDetached(rc,p,r,color);
  loadRenderedObjectToContext(rc, newCircleObject(r, 0, p, X));
  return;
}

void renderAngledCircle(renderContext *rc, Point2 p, int r, float theta, axis ax, Color color) {
  renderAngledCircleDetached(rc,p,r,theta,ax,color);
  loadRenderedObjectToContext(rc, newCircleObject(r, theta, p, ax));
}
