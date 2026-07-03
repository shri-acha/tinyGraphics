#include "types.h"
#include <stdlib.h>
#include "_graphics.h"

renderedObject* newCircleObject(int radius,int theta, Point2 center, axis ax) {
	renderedObject* ro = malloc(sizeof(renderedObject));
	if (ro == NULL) return NULL;
	ro->inner.circle = (Circle) {
		.center = center,
		.ax = ax,
		.radius = radius,
		.theta = theta
	};
	ro->type = CIRCLE;
	return ro;
}

renderedObject* newTriangleObject(Point2* points[3],int theta, axis ax) {
	renderedObject* ro = malloc(sizeof(renderedObject));
	if (ro == NULL) return NULL;
	ro->inner.triangle = (Triangle) {
		.ax = ax,
		.theta = theta,
		.points = (Point2*) points,
	};
	ro->type = TRIANGLE;

	return ro;
}

renderedObject* newLineObject(Point2 start, Point2 end) {
    renderedObject* ro = malloc(sizeof(renderedObject));
	if (ro == NULL) return NULL;
    ro->inner.line = (Line){ .start = start, .end = end };
    ro->type = LINE;

    return ro;
}
