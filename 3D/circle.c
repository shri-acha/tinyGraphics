#include "../types.h"
#include "../graphics.h"
#include "../_graphics.h"

void renderCircle3D(renderContext *rc, Point3 p, int r, Color color){
	_renderCircle3D(rc,p,r,color);
}

void renderAngledCircle3D(renderContext *rc, Point3 p, int r, float theta, axis ax,Color color){
	_renderAngledCircle3D(rc,p,r,theta,ax,color);
}

