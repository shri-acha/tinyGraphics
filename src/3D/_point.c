#include "../_graphics.h"

Point3 _rotatePoint3D(renderContext* rc, int x, int y, int z, float theta, axis ax) {
  tinyVec v = point3ToVec((Point3){.x = x, .y = y, .z = z});
  tinyMatrix rotM = getRotationMatrix3D(theta, ax);
  tinyVec res = matrixVecMul(rotM, v);
  return vecToPoint3(res);
}

void _renderPoint3D(renderContext *rc, Point3 p,Color color) {
	int depth;
	Point2 projected = _project3D(rc,p,&depth);
	_drawPixel(rc, projected.x, projected.y, depth, color);
	return;
}
