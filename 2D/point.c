#include "../graphics.h"
#include "../_graphics.h"

void renderPoint2D(renderContext *rc, Point2 p,Color color) {
	return _renderPoint2D(rc,p,color);
}

Point2 rotatePoint2D(renderContext* rc,int x, int y, float theta, axis ax) {
	return _rotatePoint2D(rc,x,y,theta,ax);
}
