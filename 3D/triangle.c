#include "../_graphics.h"
#include "../types.h"


//void renderTriangle3D(renderContext *rc, Point3 *points[3],Color color) {
	// loadRenderedObjectToContext(rc, newTriangleObject(points, 0, X));
	// return _renderTriangle3D(rc, points,color);
//}

//void renderAngledTriangle3D(renderContext *rc,Point2 *points[3], float theta, axis ax,Color color){
//  	Point2 p1 = (*points[0]);
//  	Point2 p2 = (*points[1]);
//  	Point2 p3 = (*points[2]);
//     Point2 rt_points_0 = _rotatePoint2D(rc,p1.x, p1.y, theta, ax);
//     Point2 rt_points_1 = _rotatePoint2D(rc,p2.x, p2.y, theta, ax);
//     Point2 rt_points_2 = _rotatePoint2D(rc,p3.x, p3.y, theta, ax);
//
//  	Point2* rt_points[3] = {&rt_points_0,&rt_points_1,&rt_points_2};
//
//  	loadRenderedObjectToContext(rc, newTriangleObject(points, theta, ax));
//  	//return _renderTriangle3D(rc, rt_points, color);
// }
