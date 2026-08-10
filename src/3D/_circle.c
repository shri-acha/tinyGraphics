#include "../_graphics.h"

void _renderCircle3D(renderContext *rc, Point3 p, int r,Color color) {
  int x1 = r;
  int y1 = 0;
  int x = p.x;
  int y = p.y;
  int z = p.z;
  float t1 = r >> 4;
  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {
 
     _renderPoint3D(rc,(Point3) {.x= x + x1,.y= y + y1, .z= z},color);
     _renderPoint3D(rc,(Point3) {.x= x - x1,.y= y + y1, .z= z},color);
     _renderPoint3D(rc,(Point3) {.x= x + x1,.y= y - y1, .z= z},color);
     _renderPoint3D(rc,(Point3) {.x= x - x1,.y= y - y1, .z= z},color);
     _renderPoint3D(rc,(Point3) {.x= x + y1,.y= y + x1, .z= z},color);
     _renderPoint3D(rc,(Point3) {.x= x + y1,.y= y - x1, .z= z},color);
     _renderPoint3D(rc,(Point3) {.x= x - y1,.y= y + x1, .z= z},color);
     _renderPoint3D(rc,(Point3) {.x= x - y1,.y= y - x1, .z= z},color);
                              
      y1++;                   
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
    break;
  case FILLED:
    while (x1 >= y1) {

     _renderLine3D(rc, (Point3){.x = x - x1, .y = y + y1, .z = z}, (Point3){.x = x + x1, .y = y + y1, .z = z}, color);
     _renderLine3D(rc, (Point3){.x = x - x1, .y = y - y1, .z = z}, (Point3){.x = x + x1, .y = y - y1, .z = z}, color);
     _renderLine3D(rc, (Point3){.x = x - y1, .y = y + x1, .z = z}, (Point3){.x = x + y1, .y = y + x1, .z = z}, color);
     _renderLine3D(rc, (Point3){.x = x - y1, .y = y - x1, .z = z}, (Point3){.x = x + y1, .y = y - x1, .z = z}, color);

      y1++;
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
    break;
  }
  return;
}

void _renderAngledCircle3D(renderContext *rc, Point3 p, int r, float theta, axis ax,Color color) {
  int x1 = r;
  int y1 = 0;
  int x = p.x;
  int y = p.y;
  int z = p.z;
  float t1 = r >> 4;
  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {
 
     _renderPoint3D(rc,_rotatePoint3D(rc,x + x1,y + y1,z,theta,ax),color);
     _renderPoint3D(rc,_rotatePoint3D(rc, x - x1, y + y1, z,theta,ax),color);
     _renderPoint3D(rc,_rotatePoint3D(rc, x + x1, y - y1, z,theta,ax),color);
     _renderPoint3D(rc,_rotatePoint3D(rc,x - x1,y - y1,z,theta,ax),color);
     _renderPoint3D(rc,_rotatePoint3D(rc,x + y1,y + x1, z,theta,ax),color);
     _renderPoint3D(rc,_rotatePoint3D(rc,x + y1,y - x1,z,theta,ax),color);
     _renderPoint3D(rc,_rotatePoint3D(rc,x - y1,y + x1,z,theta,ax),color);
     _renderPoint3D(rc,_rotatePoint3D(rc,x - y1,y - x1,z,theta,ax),color);

                              
      y1++;                   
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
    break;
  case FILLED:
    while (x1 >= y1) {

		Point3 rt_points_0 =_rotatePoint3D(rc,x+x1, y+y1,z, theta, ax);
		Point3 rt_points_1 =_rotatePoint3D(rc,x-x1, y+y1,z, theta, ax);
		Point3 rt_points_2 =_rotatePoint3D(rc,x+x1, y-y1,z, theta, ax);
		Point3 rt_points_3 =_rotatePoint3D(rc,x-x1, y-y1,z, theta, ax);
		Point3 rt_points_4 =_rotatePoint3D(rc,x+y1, y+x1,z, theta, ax);
		Point3 rt_points_5 =_rotatePoint3D(rc,x+y1, y-x1,z, theta, ax);
		Point3 rt_points_6 =_rotatePoint3D(rc,x-y1, y+x1,z, theta, ax);
		Point3 rt_points_7 =_rotatePoint3D(rc,x-y1, y-x1,z, theta, ax);

		_renderLine3D(rc, rt_points_1, rt_points_0,color); // top-bottom 
		_renderLine3D(rc, rt_points_3, rt_points_2,color); // bottom-top pair
		_renderLine3D(rc, rt_points_6, rt_points_4,color); // pair 1
		_renderLine3D(rc, rt_points_7, rt_points_5,color); // pair 2

      y1++;
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
    break;
  }
  return;
}

