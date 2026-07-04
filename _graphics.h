#pragma once
#include "types.h"
#include "stdlib.h"

/* Loads a rendered object into the context */
int loadRenderedObjectToContext(renderContext* rc, renderedObject* ro);

/* Initializes a new Geometrical objects and used for adding renderedObject into sceneContext*/
renderedObject* newCircleObject(int radius,int theta, Point2 center, axis ax);
renderedObject* newTriangleObject(Point2* points[3],int theta, axis ax);
renderedObject* newLineObject(Point2 start,Point2 end); 

Point2 rotatePointDetached(renderContext* rc,int x, int y, float theta, axis ax);

void renderTriangleDetached(renderContext* rc, Point2 *points[3],Color color);
void renderLineDetached(renderContext *rc,Point2 p1, Point2 p2,Color color);
void renderHorizontalLineDetached(renderContext *rc, int x1, int x2, int y,Color color);
int renderPointDetached(renderContext *rc, Point2 p,Color color);
void renderCircleDetached(renderContext *rc, Point2 p, int r, Color color);
void renderAngledCircleDetached(renderContext *rc, Point2 p, int r, float theta, axis ax, Color color);
void renderAngledTriangleDetached(renderContext *rc,Point2 *points[3], float theta, axis ax, Color color);
