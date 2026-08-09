#include "../_graphics.h"

int renderPoint3D(renderContext *rc, Point3 p, int* depth, Color color) {
	Point2 projected = _project3D(rc, p, depth);
	return _drawPixel(rc, projected.x, projected.y, p.z, color);
}


Point3 rotatePoint3D(renderContext* rc, int x,int y, int z, float theta, axis ax) {
	return _rotatePoint3D(rc,x,y,z,theta,ax);
}
