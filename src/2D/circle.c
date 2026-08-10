#include "../_graphics.h"

void renderCircle2D(renderContext *rc, Point2 p, int r, Color color) {
  _renderCircle2D(rc,p,r,color);
  loadRenderedObjectToContext(rc, newCircleObject(r, 0, p, X));
  return;
}

void renderAngledCircle2D(renderContext *rc, Point2 p, int r, float theta, axis ax, Color color) {
	_renderAngledCircle2D(rc,p,r,theta,ax,color);
  loadRenderedObjectToContext(rc, newCircleObject(r, theta, p, ax));
}
