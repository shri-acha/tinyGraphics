#include "graphics.h"

int renderCircle(renderContext *rc, Point2 p, int r) {
  int x1 = r;
  int y1 = 0;
  int x = p.x;
  int y = p.y;
  float t1 = r >> 4;
  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {
 
      renderPoint(rc,(Point2) {.x= x + x1,.y= y + y1});
      renderPoint(rc,(Point2) {.x= x - x1,.y= y + y1});
      renderPoint(rc,(Point2) {.x= x + x1,.y= y - y1});
      renderPoint(rc,(Point2) {.x= x - x1,.y= y - y1});
      renderPoint(rc,(Point2) {.x= y + y1,.y= x + x1});
      renderPoint(rc,(Point2) {.x= y + y1,.y= x - x1});
      renderPoint(rc,(Point2) {.x= y - y1,.y= x + x1});
      renderPoint(rc,(Point2) {.x= y - y1,.y= x - x1});
                              
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

      renderHorizontalLine(rc, x - x1, x + x1, y + y1);
      renderHorizontalLine(rc, x - x1, x + x1, y - y1);
      renderHorizontalLine(rc, x - y1, x + y1, y + x1);
      renderHorizontalLine(rc, x - y1, x + y1, y - x1);

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
  return 0;
}

int renderAngledCircle(renderContext *rc, Point2 p, int r, float theta, axis ax) {
  int x1 = r;
  int y1 = 0;
  float t1 = r >> 4;
  int x = p.x;
  int y = p.y;

  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {

      renderPoint(rc,rotatePoint(rc,x+x1, y+y1, theta, ax));
      renderPoint(rc,rotatePoint(rc,x-x1, y+y1, theta, ax) );
      renderPoint(rc,rotatePoint(rc,x+x1, y-y1, theta, ax) );
      renderPoint(rc,rotatePoint(rc,x-x1, y-y1, theta, ax));
      renderPoint(rc,rotatePoint(rc,x+y1, y+x1, theta, ax)  );
      renderPoint(rc,rotatePoint(rc,x+y1, y-x1, theta, ax));
      renderPoint(rc,rotatePoint(rc,x-y1, y+x1, theta, ax) );
      renderPoint(rc,rotatePoint(rc,x-y1, y-x1, theta, ax));

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

 
   Point2 rt_points_0 = rotatePoint(rc,x+x1, y+y1, theta, ax);
   Point2 rt_points_1 = rotatePoint(rc,x-x1, y+y1, theta, ax);
   Point2 rt_points_2 = rotatePoint(rc,x+x1, y-y1, theta, ax);
   Point2 rt_points_3 = rotatePoint(rc,x-x1, y-y1, theta, ax);
   Point2 rt_points_4 = rotatePoint(rc,x+y1, y+x1, theta, ax);
   Point2 rt_points_5 = rotatePoint(rc,x+y1, y-x1, theta, ax);
   Point2 rt_points_6 = rotatePoint(rc,x-y1, y+x1, theta, ax);
   Point2 rt_points_7 = rotatePoint(rc,x-y1, y-x1, theta, ax);

      renderHorizontalLine(rc, rt_points_1.x, rt_points_0.x, rt_points_0.y);
      renderHorizontalLine(rc, rt_points_1.x, rt_points_0.x, rt_points_2.y);
      renderHorizontalLine(rc,rt_points_6.x,rt_points_4.x,rt_points_4.y);
      renderHorizontalLine(rc, rt_points_7.x,rt_points_5.x,rt_points_5.y);

      y1++;
      t1 += y1;
      float t2 = t1 - x1;
      if (t2 >= 0) {
        t1 = t2;
        x1--;
      }
    }
  }

  return 0;
}
