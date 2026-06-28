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

      renderPoint(rc,(Point2) {.x=x + x1,.y= y + y1});
      renderPoint(rc,(Point2) {.x=x - x1,.y= y + y1});
      renderPoint(rc,(Point2) {.x=x + x1,.y= y - y1});
      renderPoint(rc,(Point2) {.x=x - x1,.y= y - y1});
      renderPoint(rc,(Point2) {.x=y + y1,.y= x + x1});
      renderPoint(rc,(Point2) {.x=y + y1,.y= x - x1});
      renderPoint(rc,(Point2){.x=y - y1,.y= x + x1});
      renderPoint(rc,(Point2){.x=y - y1,.y= x - x1});
                              
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

int renderAngledCircle(renderContext *rc, Point2 p, int r, float theta,
                       axis ax) {
  int x1 = r;
  int y1 = 0;
  float t1 = r >> 4;
  int x = p.x;
  int y = p.y;

  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {

      Point2 rt_points_0 = rotate_point(x1, y1, theta, ax);
      Point2 rt_points_1 = rotate_point(-x1, y1, theta, ax);
      Point2 rt_points_2 = rotate_point(x1, -y1, theta, ax);
      Point2 rt_points_3 = rotate_point(-x1, -y1, theta, ax);
      Point2 rt_points_4 = rotate_point(y1, x1, theta, ax);
      Point2 rt_points_5 = rotate_point(y1, -x1, theta, ax);
      Point2 rt_points_6 = rotate_point(-y1, x1, theta, ax);
      Point2 rt_points_7 = rotate_point(-y1, -x1, theta, ax);

      renderPoint(rc,(Point2){.x= x + rt_points_0.x, y + rt_points_0.y});
      renderPoint(rc,(Point2){.x=x + rt_points_1.x,.y= y + rt_points_1.y});
      renderPoint(rc,(Point2){.x=x + rt_points_2.x,.y=y + rt_points_2.y});
      renderPoint(rc,(Point2){.x=x + rt_points_3.x,.y=y + rt_points_3.y});
      renderPoint(rc,(Point2){.x=y + rt_points_4.x,.y=x + rt_points_4.y});
      renderPoint(rc,(Point2){.x=y + rt_points_5.x,.y=x + rt_points_5.y});
      renderPoint(rc,(Point2){.x=y + rt_points_6.x,.y=x + rt_points_6.y});
      renderPoint(rc,(Point2){.x=y + rt_points_7.x,.y=x + rt_points_7.y});

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

      Point2 rt_points_0 = rotate_point(x1, y1, theta, ax);
      Point2 rt_points_1 = rotate_point(-x1, y1, theta, ax);

      Point2 rt_points_2 = rotate_point(x1, -y1, theta, ax);
      Point2 rt_points_3 = rotate_point(-x1, -y1, theta, ax);

      Point2 rt_points_4 = rotate_point(y1, x1, theta, ax);
      Point2 rt_points_5 = rotate_point(y1, -x1, theta, ax);

      Point2 rt_points_6 = rotate_point(-y1, x1, theta, ax);
      Point2 rt_points_7 = rotate_point(-y1, -x1, theta, ax);

      renderHorizontalLine(rc, x + rt_points_1.x, x + rt_points_0.x,
                           y + rt_points_0.y);
      renderHorizontalLine(rc, x + rt_points_1.x, x + rt_points_0.x,
                           y + rt_points_2.y);
      renderHorizontalLine(rc, x + rt_points_6.x, x + rt_points_4.x,
                           y + rt_points_4.y);
      renderHorizontalLine(rc, x + rt_points_7.x, x + rt_points_5.x,
                           y + rt_points_5.y);

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
