#pragma once
#include "types.h"
#include "stdlib.h"
/*Draws a pixel at the location x,y with z  */
int _drawPixel(renderContext *rc, int x, int y,Color color);

/* Loads a rendered object into the context */
int loadRenderedObjectToContext(renderContext* rc, renderedObject* ro);

/* Initializes a new Geometrical objects and used for adding renderedObject into sceneContext*/
renderedObject* newCircleObject(int radius,int theta, Point2 center, axis ax);
renderedObject* newTriangleObject(Point2* points[3],int theta, axis ax);
renderedObject* newLineObject(Point2 start,Point2 end); 

Point2 _rotatePoint2D(renderContext* rc,int x, int y, float theta, axis ax);
Point3 _rotatePoint3D(renderContext* rc,int x, int y, int z, float theta, axis ax);
Point2 _project3D(renderContext* rc, Point3 p);


void _renderTriangle2D(renderContext* rc, Point2 *points[3],Color color);
void _renderLine2D(renderContext *rc,Point2 p1, Point2 p2,Color color);
void _renderHorizontalLine2D(renderContext *rc, int x1, int x2, int y,Color color);
void _renderPoint2D(renderContext *rc, Point2 p,Color color);
void _renderCircle2D(renderContext *rc, Point2 p, int r, Color color);
void _renderAngledCircle2D(renderContext *rc, Point2 p, int r, float theta, axis ax, Color color);
void _renderAngledTriangle2D(renderContext *rc,Point2 *points[3], float theta, axis ax, Color color);

void _renderPoint3D(renderContext *rc, Point3 p,Color color);
void _renderLine3D(renderContext *rc,Point3 p1, Point3 p2,Color color);
void _renderTriangle3D(renderContext *rc, Point3* points[3],Color color);
void _renderCircle3D(renderContext *rc, Point3 c,int r,Color color);
