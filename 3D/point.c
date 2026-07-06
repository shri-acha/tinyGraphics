#include "../graphics.h"
#include "../_graphics.h"

int renderPoint3D(renderContext *rc, Point3 p,Color color) {
	int screenX,screenY,depth = p.z;

	switch (rc->projection) {
		case PERSPECTIVE:
			screenX = (int) ((p.x*rc->focal_length) / p.z) + rc->origin.x;
			screenY = (int) ((p.y*rc->focal_length) / p.z) + rc->origin.y;
			break;
		case ORTHOGRAPHIC: 
			screenX = (int) p.x + rc->origin.x;
			screenY = (int) p.y + rc->origin.y;
			break;
		default:
			fprintf(stderr,"[ERROR] Projection type not supported!");
	}
	return _drawPixel(rc, screenX, screenY, color);
}

Point3 rotatePoint3D(renderContext* rc, int x,int y, int z, float theta, axis ax) {
	return _rotatePoint3D(rc,x,y,z,theta,ax);
}
