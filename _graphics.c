#include "types.h"
#include <math.h>
#include <stdlib.h>
#include "_graphics.h"
#include "utils.h"

Point2 rotatePointDetached(renderContext* rc,int x, int y, float theta, axis ax) {
  Point2 rt_pair;
  switch (ax) {
  case Z:
    rt_pair.x = (y) * sin(theta) + (-x) * cos(theta);
    rt_pair.y = (x) * sin(theta) + (y) * cos(theta);
    break;
  case X:
    rt_pair.x = x;
    rt_pair.y = (y)* cos(theta);
    break;
  case Y:
    rt_pair.x = (x) * cos(theta);
    rt_pair.y = y;
    break;
  default:
	 break;
  }
  return rt_pair;
}

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

void renderCircleDetached(renderContext *rc, Point2 p, int r) {
  int x1 = r;
  int y1 = 0;
  int x = p.x;
  int y = p.y;
  float t1 = r >> 4;
  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {
 
      renderPointDetached(rc,(Point2) {.x= x + x1,.y= y + y1});
      renderPointDetached(rc,(Point2) {.x= x - x1,.y= y + y1});
      renderPointDetached(rc,(Point2) {.x= x + x1,.y= y - y1});
      renderPointDetached(rc,(Point2) {.x= x - x1,.y= y - y1});
      renderPointDetached(rc,(Point2) {.x= y + y1,.y= x + x1});
      renderPointDetached(rc,(Point2) {.x= y + y1,.y= x - x1});
      renderPointDetached(rc,(Point2) {.x= y - y1,.y= x + x1});
      renderPointDetached(rc,(Point2) {.x= y - y1,.y= x - x1});
                              
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

      renderHorizontalLineDetached(rc, x - x1, x + x1, y + y1);
      renderHorizontalLineDetached(rc, x - x1, x + x1, y - y1);
      renderHorizontalLineDetached(rc, x - y1, x + y1, y + x1);
      renderHorizontalLineDetached(rc, x - y1, x + y1, y - x1);

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

void renderAngledCircleDetached(renderContext *rc, Point2 p, int r, float theta, axis ax) {
  int x1 = r;
  int y1 = 0;
  float t1 = r >> 4;
  int x = p.x;
  int y = p.y;

  switch (rc->render_mode) {
  case WIREFRAME:
    while (x1 >= y1) {

      renderPointDetached(rc,rotatePointDetached(rc,x+x1, y+y1, theta, ax));
      renderPointDetached(rc,rotatePointDetached(rc,x-x1, y+y1, theta, ax) );
      renderPointDetached(rc,rotatePointDetached(rc,x+x1, y-y1, theta, ax) );
      renderPointDetached(rc,rotatePointDetached(rc,x-x1, y-y1, theta, ax));
      renderPointDetached(rc,rotatePointDetached(rc,x+y1, y+x1, theta, ax)  );
      renderPointDetached(rc,rotatePointDetached(rc,x+y1, y-x1, theta, ax));
      renderPointDetached(rc,rotatePointDetached(rc,x-y1, y+x1, theta, ax) );
      renderPointDetached(rc,rotatePointDetached(rc,x-y1, y-x1, theta, ax));

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

 
   Point2 rt_points_0 = rotatePointDetached(rc,x+x1, y+y1, theta, ax);
   Point2 rt_points_1 = rotatePointDetached(rc,x-x1, y+y1, theta, ax);
   Point2 rt_points_2 = rotatePointDetached(rc,x+x1, y-y1, theta, ax);
   Point2 rt_points_3 = rotatePointDetached(rc,x-x1, y-y1, theta, ax);
   Point2 rt_points_4 = rotatePointDetached(rc,x+y1, y+x1, theta, ax);
   Point2 rt_points_5 = rotatePointDetached(rc,x+y1, y-x1, theta, ax);
   Point2 rt_points_6 = rotatePointDetached(rc,x-y1, y+x1, theta, ax);
   Point2 rt_points_7 = rotatePointDetached(rc,x-y1, y-x1, theta, ax);

      renderHorizontalLineDetached(rc, rt_points_1.x, rt_points_0.x, rt_points_0.y);
      renderHorizontalLineDetached(rc, rt_points_1.x, rt_points_0.x, rt_points_2.y);
      renderHorizontalLineDetached(rc,rt_points_6.x,rt_points_4.x,rt_points_4.y);
      renderHorizontalLineDetached(rc, rt_points_7.x,rt_points_5.x,rt_points_5.y);

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

void renderLineDetached(renderContext *rc,Point2 p1, Point2 p2){
  int x1 = p1.x,y1 = p1.y;
  int x2 = p2.x,y2 = p2.y;

  int dx = abs(x2 - x1);
  int dy = -abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;

  while (1) {
    renderPointDetached(rc,(Point2){.x=x1,.y=y1});
    if (x1 == x2 && y1 == y2)
      break;
    int e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x1 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y1 += sy;
    }
  }
}
void renderHorizontalLineDetached(renderContext *rc, int x1, int x2, int y) {
  if (x1 > x2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
  }

  for (int i = x1; i <= x2; i++) {
    renderPointDetached(rc,(Point2){.x=i,.y=y});
  }
}

/* This is an internal purely rendering function that doesn't affect the context*/
void renderTriangleDetached(renderContext* rc, Point2 *points[3]){
    switch (rc->render_mode) {
    case WIREFRAME: {
        Point2 p1 = *(points[0]);
        Point2 p2 = *(points[1]);
        Point2 p3 = *(points[2]);
        renderLineDetached(rc, p1, p2);
        renderLineDetached(rc, p2, p3);
        renderLineDetached(rc, p3, p1);
        break;
    }
    case FILLED: {
        sort_point2(points, 3); 

        int x1 = points[0]->x, y1 = points[0]->y;
        int x2 = points[1]->x, y2 = points[1]->y;
        int x3 = points[2]->x, y3 = points[2]->y;

        if (y1 == y3) break; 		  
		// Since on sorting, points[0] & points[2] are the furthest away from each other, hence, point[1] is the seperating line for rendering the triangle's
		// top and bottom half.
		  int y4 = y2; // USING FOR MY SANITY'S SAKE
        int x4 = x1 + (int)((float)(y4 - y1) * ((float)(x3 - x1) / (float)(y3 - y1)));

		  // USES THE DDA TO RENDER EACH LINE 
				{
			  // TOP HALF
            float x_left = (float)x1;
            float x_right = (float)x1;
            float change_x_left = (float)(x2 - x1) / (float)(y2 - y1);
            float change_x_right = (float)(x4 - x1) / (float)(y2 - y1);

            for (int y_scan = y1; y_scan < y2; y_scan++) {
                if (x_left >= x_right) {
                    renderHorizontalLineDetached(rc, (int)x_right, (int)x_left, y_scan);
                } else {
                    renderHorizontalLineDetached(rc, (int)x_left, (int)x_right, y_scan);
                }
                x_left += change_x_left;
                x_right += change_x_right;
            }
        }

        {

			  // BOTTOM HALF
            float x_left = (float)x2;
            float x_right = (float)x4;
            
            float change_x_left = (float)(x3 - x2) / (float)(y3 - y2);
            float change_x_right = (float)(x3 - x4) / (float)(y3 - y2);

            for (int y_scan = y2; y_scan < y3; y_scan++) {
                if (x_left >= x_right) {
                    renderHorizontalLineDetached(rc, (int)x_right, (int)x_left, y_scan);
                } else {
                    renderHorizontalLineDetached(rc, (int)x_left, (int)x_right, y_scan);
                }
                x_left += change_x_left;
                x_right += change_x_right;
            }
        }
        break;
    }
    }
}

int renderPointDetached(renderContext *rc, Point2 p) {
  int x = p.x+ rc->origin.x ,y = p.y + rc->origin.y;
  int width = rc->frame_buffer->width;
  int height = rc->frame_buffer->height;

  if (x > width || y > height || x < 0 || y < 0) {
    return -1;
  }
  rc->frame_buffer->buffer[get_index(rc->frame_buffer, x, y)].color.literal = (uint16_t)0xFFFF;
  return 0;
}

void renderAngledTriangleDetached(renderContext* rc, Point2* points[3],float theta, axis ax) {
		Point2 p1 = (*points[0]);
		Point2 p2 = (*points[1]);
		Point2 p3 = (*points[2]);
      Point2 rt_points_0 = rotatePointDetached(rc,p1.x, p1.y, theta, ax);
      Point2 rt_points_1 = rotatePointDetached(rc,p2.x, p2.y, theta, ax);
      Point2 rt_points_2 = rotatePointDetached(rc,p3.x, p3.y, theta, ax);

		Point2* rt_points[3] = {&rt_points_0,&rt_points_1,&rt_points_2};

		loadRenderedObjectToContext(rc, newTriangleObject(points, theta, ax));
		return renderTriangleDetached(rc, rt_points);

}
