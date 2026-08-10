#include "../_graphics.h"

void renderLine2D(renderContext *rc, Point2 p1, Point2 p2,Color color) {
  _renderLine2D(rc, p1,p2,color);
  loadRenderedObjectToContext(rc, newLineObject(p1, p2)); 
  return;
}

void renderHorizontalLine2D(renderContext *rc, int x1, int x2, int y,Color color) {
  _renderHorizontalLine2D(rc, x1,x2,y,color);
  Point2 p1 = (Point2) {.x = x1,.y = y}, p2 = (Point2) {.x = x2,.y=y};
  loadRenderedObjectToContext(rc, newLineObject(p1, p2)); 
}
