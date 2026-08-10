#include "../_graphics.h"

void _renderCircle2D(renderContext *rc, Point2 p, int r,Color color) {
  int x1 = r;
  int y1 = 0;
  int x = p.x;
  int y = p.y;
  float t1 = r >> 4;
  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {
 
     _renderPoint2D(rc,(Point2) {.x= x + x1,.y= y + y1},color);
     _renderPoint2D(rc,(Point2) {.x= x - x1,.y= y + y1},color);
     _renderPoint2D(rc,(Point2) {.x= x + x1,.y= y - y1},color);
     _renderPoint2D(rc,(Point2) {.x= x - x1,.y= y - y1},color);
     _renderPoint2D(rc,(Point2) {.x= x + y1,.y= y + x1},color);
     _renderPoint2D(rc,(Point2) {.x= x + y1,.y= y - x1},color);
     _renderPoint2D(rc,(Point2) {.x= x - y1,.y= y + x1},color);
     _renderPoint2D(rc,(Point2) {.x= x - y1,.y= y - x1},color);
                              
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

     _renderHorizontalLine2D(rc, x - x1, x + x1, y + y1,color);
     _renderHorizontalLine2D(rc, x - x1, x + x1, y - y1,color);
     _renderHorizontalLine2D(rc, x - y1, x + y1, y + x1,color);
     _renderHorizontalLine2D(rc, x - y1, x + y1, y - x1,color);

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
  loadRenderedObjectToContext(rc, newCircleObject(r, 0, p, X));
  return;
}

void _renderAngledCircle2D(renderContext *rc, Point2 p, int r, float theta, axis ax,Color color) {
  int x1 = r;
  int y1 = 0;
  float t1 = r >> 4;
  int x = p.x;
  int y = p.y;

  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {

      _renderPoint2D(rc,_rotatePoint2D(rc,x+x1, y+y1, theta, ax),color);
      _renderPoint2D(rc,_rotatePoint2D(rc,x-x1, y+y1, theta, ax),color);
      _renderPoint2D(rc,_rotatePoint2D(rc,x+x1, y-y1, theta, ax),color);
      _renderPoint2D(rc,_rotatePoint2D(rc,x-x1, y-y1, theta, ax),color);
      _renderPoint2D(rc,_rotatePoint2D(rc,x+y1, y+x1, theta, ax),color);
      _renderPoint2D(rc,_rotatePoint2D(rc,x+y1, y-x1, theta, ax),color);
      _renderPoint2D(rc,_rotatePoint2D(rc,x-y1, y+x1, theta, ax),color);
      _renderPoint2D(rc,_rotatePoint2D(rc,x-y1, y-x1, theta, ax),color);

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

 
		Point2 rt_points_0 =_rotatePoint2D(rc,x+x1, y+y1, theta, ax);
		Point2 rt_points_1 =_rotatePoint2D(rc,x-x1, y+y1, theta, ax);
		Point2 rt_points_2 =_rotatePoint2D(rc,x+x1, y-y1, theta, ax);
		Point2 rt_points_3 =_rotatePoint2D(rc,x-x1, y-y1, theta, ax);
		Point2 rt_points_4 =_rotatePoint2D(rc,x+y1, y+x1, theta, ax);
		Point2 rt_points_5 =_rotatePoint2D(rc,x+y1, y-x1, theta, ax);
		Point2 rt_points_6 =_rotatePoint2D(rc,x-y1, y+x1, theta, ax);
		Point2 rt_points_7 =_rotatePoint2D(rc,x-y1, y-x1, theta, ax);

		_renderLine2D(rc, rt_points_1, rt_points_0,color); // top-bottom 
		_renderLine2D(rc, rt_points_3, rt_points_2,color); // bottom-top pair
		_renderLine2D(rc, rt_points_6, rt_points_4,color); // pair 1
		_renderLine2D(rc, rt_points_7, rt_points_5,color); // pair 2

      y1++;
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
  }
  loadRenderedObjectToContext(rc, newCircleObject(r, theta, p, ax));
}
