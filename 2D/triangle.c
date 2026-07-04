#include "../_graphics.h"
#include "../graphics.h"
#include "../types.h"


void renderTriangle(renderContext *rc, Point2 *points[3],Color color) {
	loadRenderedObjectToContext(rc, newTriangleObject(points, 0, X));
	return renderTriangleDetached(rc, points,color);
}

 void renderAngledTriangle(renderContext *rc,Point2 *points[3], float theta, axis ax,Color color){
		Point2 p1 = (*points[0]);
		Point2 p2 = (*points[1]);
		Point2 p3 = (*points[2]);
      Point2 rt_points_0 = rotatePoint(rc,p1.x, p1.y, theta, ax);
      Point2 rt_points_1 = rotatePoint(rc,p2.x, p2.y, theta, ax);
      Point2 rt_points_2 = rotatePoint(rc,p3.x, p3.y, theta, ax);

		Point2* rt_points[3] = {&rt_points_0,&rt_points_1,&rt_points_2};

		loadRenderedObjectToContext(rc, newTriangleObject(points, theta, ax));
		return renderTriangleDetached(rc, rt_points, color);
}
