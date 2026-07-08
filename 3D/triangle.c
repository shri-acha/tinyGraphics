#include "../_graphics.h"
#include "../types.h"


void renderTriangle3D(renderContext *rc, Point3 *points[3],Color color) {
	 //loadRenderedObjectToContext(rc, newTriangleObject(points, 0, X));
	 return _renderTriangle3D(rc, points,color);
}

void renderAngledTriangle3D(renderContext *rc,Point3 *points[3], float theta, axis ax,Color color){
		Point3 p1 = (*points[0]);
		Point3 p2 = (*points[1]);
		Point3 p3 = (*points[2]);
      Point3 rt_points_0 = _rotatePoint3D(rc,p1.x, p1.y,p1.z, theta, ax);
      Point3 rt_points_1 = _rotatePoint3D(rc,p2.x, p2.y,p2.z, theta, ax);
      Point3 rt_points_2 = _rotatePoint3D(rc,p3.x, p3.y,p3.z, theta, ax);

  	  Point3* rt_points[3] = {&rt_points_0,&rt_points_1,&rt_points_2};

  	//loadRenderedObjectToContext(rc, newTriangleObject(points, theta, ax));
  	return _renderTriangle3D(rc, rt_points, color);
 }
