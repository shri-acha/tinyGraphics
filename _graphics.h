#pragma once
#include "types.h"
#include "stdlib.h"

/* Loads a rendered object into the context */
int loadRenderedObjectToContext(renderContext* rc, renderedObject* ro);

/* Initializes a new Geometrical objects and used for adding renderedObject into sceneContext*/
renderedObject* newCircleObject(int radius,int theta, Point2 center, axis ax);
renderedObject* newTriangleObject(Point2* points[3],int theta, axis ax);
renderedObject* newLineObject(Point2 start,Point2 end); 
