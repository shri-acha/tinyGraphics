#include "graphics.h"
#include "_graphics.h"
#include "types.h"

void renderLine(renderContext *rc, Point2 p1, Point2 p2) {
  renderLineDetached(rc, p1,p2);
  loadRenderedObjectToContext(rc, newLineObject(p1, p2)); 
  return;
}

void renderHorizontalLine(renderContext *rc, int x1, int x2, int y) {
  renderHorizontalLineDetached(rc, x1,x2,y);
  Point2 p1 = (Point2) {.x = x1,.y = y}, p2 = (Point2) {.x = x2,.y=y};
  loadRenderedObjectToContext(rc, newLineObject(p1, p2)); 
}
