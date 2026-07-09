#include "types.h"
#include <math.h>
#include <stdlib.h>
#include "_graphics.h"
#include "utils.h"
#include <stdio.h>

Point2 _rotatePoint2D(renderContext* rc,int x, int y, float theta, axis ax) {
  Point2 rt_pair;
  switch (ax) {
  case Z:
    rt_pair.x = -(y) * sin(theta) + (x) * cos(theta);
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

Point3 _rotatePoint3D(renderContext* rc,int x, int y, int z, float theta, axis ax) {
  Point3 rt_pair;
  switch (ax) {
  case Z:
    rt_pair.x = -(y) * sin(theta) + (x) * cos(theta);
    rt_pair.y = (x) * sin(theta) + (y) * cos(theta);
    rt_pair.z = z;
    break;
  case X:
    rt_pair.x = x;
    rt_pair.y = (y)* cos(theta) + z*sin(theta);
    rt_pair.z = -(y)*sin(theta) + z*cos(theta);
    break;
  case Y:
    rt_pair.x = (x) * cos(theta) - (z) * sin(theta);
    rt_pair.y = y;
    rt_pair.z = -(x)*sin(theta) + (z)*cos(theta);
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
     _renderPoint2D(rc,(Point2) {.x= y + y1,.y= x + x1},color);
     _renderPoint2D(rc,(Point2) {.x= y + y1,.y= x - x1},color);
     _renderPoint2D(rc,(Point2) {.x= y - y1,.y= x + x1},color);
     _renderPoint2D(rc,(Point2) {.x= y - y1,.y= x - x1},color);
                              
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

void _renderLine2D(renderContext *rc,Point2 p1, Point2 p2,Color color){
  int x1 = p1.x,y1 = p1.y;
  int x2 = p2.x,y2 = p2.y;

  int dx = abs(x2 - x1);
  int dy = -abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;

  while (1) {
    _renderPoint2D(rc,(Point2){.x=x1,.y=y1},color);
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
void _renderHorizontalLine2D(renderContext *rc, int x1, int x2, int y,Color color) {
  if (x1 > x2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
  }

  for (int i = x1; i <= x2; i++) {
    _renderPoint2D(rc,(Point2){.x=i,.y=y},color);
  }
}

/* This is an internal purely rendering function that doesn't affect the context*/
void _renderTriangle2D(renderContext* rc, Point2 *points[3],Color color){
    switch (rc->render_mode) {
    case WIREFRAME: {
        Point2 p1 = *(points[0]);
        Point2 p2 = *(points[1]);
        Point2 p3 = *(points[2]);
        _renderLine2D(rc, p1, p2,color);
        _renderLine2D(rc, p2, p3,color);
        _renderLine2D(rc, p3, p1,color);
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

				float change_x_left = (y2 - y1 != 0) ? (float)(x2 - x1) / (float)(y2 - y1) : 0.0f;
				float change_x_right = (y2 - y1 != 0) ? (float)(x4 - x1) / (float)(y2 - y1) : 0.0f;

            for (int y_scan = y1; y_scan <= y2; y_scan++) {
                if (x_left >= x_right) {
                    _renderHorizontalLine2D(rc, (int)x_right, (int)x_left, y_scan,color);
                } else {
                    _renderHorizontalLine2D(rc, (int)x_left, (int)x_right, y_scan,color);
                }
                x_left += change_x_left;
                x_right += change_x_right;
            }
        }

        {

			  // BOTTOM HALF
            float x_left = (float)x2;
            float x_right = (float)x4;
            
            float change_x_left = (y3-y2 != 0)?((float)(x3 - x2) / (float)(y3 - y2)):0.0f;
            float change_x_right = (y3-y2 != 0)?((float)(x3 - x4) / (float)(y3 - y2)):0.0f;

            for (int y_scan = y2; y_scan < y3; y_scan++) {
                if (x_left >= x_right) {
                    _renderHorizontalLine2D(rc, (int)x_right, (int)x_left, y_scan,color);
                } else {
                    _renderHorizontalLine2D(rc, (int)x_left, (int)x_right, y_scan,color);
                }
                x_left += change_x_left;
                x_right += change_x_right;
            }
        }
        break;
    }
    }
}

void _renderPoint2D(renderContext *rc, Point2 p,Color color) {
  int x = p.x + rc->origin.x,y = p.y + rc->origin.y;
  int width = rc->frame_buffer->width;
  int height = rc->frame_buffer->height;

  if (x > width || y > height || x < 0 || y < 0) {
    return;
  }
  rc->frame_buffer->buffer[get_index(rc->frame_buffer, x, y)].color = color;
  return;
}

void _renderAngledTriangle2D(renderContext* rc, Point2* points[3],float theta, axis ax,Color color) {
		Point2 p1 = (*points[0]);
		Point2 p2 = (*points[1]);
		Point2 p3 = (*points[2]);
      Point2 rt_points_0 = _rotatePoint2D(rc,p1.x, p1.y, theta, ax);
      Point2 rt_points_1 = _rotatePoint2D(rc,p2.x, p2.y, theta, ax);
      Point2 rt_points_2 = _rotatePoint2D(rc,p3.x, p3.y, theta, ax);

		Point2* rt_points[3] = {&rt_points_0,&rt_points_1,&rt_points_2};

		loadRenderedObjectToContext(rc, newTriangleObject(points, theta, ax));
		return _renderTriangle2D(rc, rt_points,color);

}

Point2 _project3D(renderContext* rc, Point3 p) {
    int sx1, sy1;
    if (rc->projection == PERSPECTIVE) {
			 sx1 = (int)(((float)p.x * (float)rc->focal_length) / ((float)p.z + rc->camera_position.z)  - rc->camera_position.x);
			 sy1 = (int)(((float)p.y * (float)rc->focal_length) / ((float)p.z + rc->camera_position.z)  - rc->camera_position.y); 
    } else {
        sx1 = (int)p.x + rc->origin.x;
        sy1 = (int)p.y + rc->origin.y;
    }
	 return (Point2) {.x = sx1,.y=sy1};
}

void _renderLine3D(renderContext* rc, Point3 p1, Point3 p2, Color color) {

	 int sx1 = p1.x,sx2=p2.x,sy1=p1.y,sy2=p2.y;

    int dx = abs(sx2 - sx1);
    int dy = -abs(sy2 - sy1);

    int sx = sx1 < sx2 ? 1 : -1;
    int sy = sy1 < sy2 ? 1 : -1;
    int err = dx + dy;

    int total_steps = (dx > -dy) ? dx : -dy;
    float current_z = p1.z;
    float z_step = (total_steps == 0) ? 0 : (p2.z - p1.z) / (float)total_steps;

    while (1) {
			 Point3 screen_point = {
				 .x = sx1 ,
				 .y = sy1 ,
				 .z = current_z,
			 };
			 _renderPoint3D(rc, screen_point, color);

        if (sx1 == sx2 && sy1 == sy2)
            break;
            
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            sx1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            sy1 += sy;
        }
        
        current_z += z_step;
    }
}

int _drawPixel(renderContext *rc, int x, int y,Color color) {
  int width = rc->frame_buffer->width;
  int height = rc->frame_buffer->height;

  if (x >= width || y >= height || x < 0 || y < 0) {
    return -1;
  }

  rc->frame_buffer->buffer[get_index(rc->frame_buffer, x, y)].color = color;
  return 0;
}

void _renderPoint3D(renderContext *rc, Point3 p,Color color) {
	int screenX,screenY;
	Point2 projected = _project3D(rc,p);
	_drawPixel(rc, projected.x + rc->origin.x, projected.y + rc->origin.y, color);
	return;
}

void _renderTriangle3D(renderContext* rc, Point3 *points[3],Color color){
	 Point2* points_2[3];
	 Point2 tmp_buff[3];
    switch (rc->render_mode) {
    case WIREFRAME: {
        Point3 p1 = *(points[0]);
        Point3 p2 = *(points[1]);
        Point3 p3 = *(points[2]);
        _renderLine3D(rc, p1, p2,color);
        _renderLine3D(rc, p2, p3,color);
        _renderLine3D(rc, p3, p1,color);
        break;
    }
    case FILLED: {
			for (int i=0;i<3;i++) {
				 tmp_buff[i] = _project3D(rc, *points[i]);
				 points_2[i] = &tmp_buff[i];
			}

        sort_point2(points_2, 3); 

        int x1 = points_2[0]->x, y1 = points_2[0]->y;
        int x2 = points_2[1]->x, y2 = points_2[1]->y;
        int x3 = points_2[2]->x, y3 = points_2[2]->y;

		// Since on sorting, points[0] & points[2] are the furthest away from each other, hence, point[1] is the seperating line for rendering the triangle's
		// top and bottom half.
		  int y4 = y2; // USING FOR MY SANITY'S SAKE
        int x4 = (y3-y1 != 0)?( x1 + (int)((float)(y4 - y1) * ((float)(x3 - x1) / (float)(y3 - y1))) ):0;

		  // USES THE DDA TO RENDER EACH LINE 
				{
			  // TOP HALF
            float x_left = (float)x1;
            float x_right = (float)x1;

				if ( y2 != y1) {
				float change_x_left = (y2 - y1 != 0)?((float)(x2 - x1) / (float)(y2 - y1)):((float)(x4 - x1) / (float)(y1 - y2));
				float change_x_right = (y2 - y1 != 0)?((float)(x4 - x1) / (float)(y2 - y1)):((float)(x4 - x1) / (float)(y1 - y2));

					for (int y_scan = y1; y_scan < y2; y_scan++) {
						 if (x_left >= x_right) {
							  _renderHorizontalLine2D(rc, (int)x_right, (int)x_left, y_scan,color);
						 } else {
							  _renderHorizontalLine2D(rc, (int)x_left, (int)x_right, y_scan,color);
						 }
						 x_left += change_x_left;
						 x_right += change_x_right;

						 printf("(%f,%d) (%f,%d)\n",x_left,y1,x_right,y2);
					}
				}
        }

        {

			  // BOTTOM HALF
            float x_left = (float)x2;
				float x_right = (float)x4;
				if ( y3 != y2) {
					float change_x_left = (y3-y2 > 0)?((float)(x3 - x2) / (float)(y3 - y2)):((float)(x3 - x2) / (float)(y2 - y3));
					float change_x_right = (y3-y2 > 0)?((float)(x3 - x4) / (float)(y3 - y2)):((float)(x3 - x2) / (float)(y2 - y3));

					for (int y_scan = y2; y_scan < y3; y_scan++) {
						if (x_left >= x_right) {
							_renderHorizontalLine2D(rc, (int)x_right, (int)x_left, y_scan,color);
						} else {
							_renderHorizontalLine2D(rc, (int)x_left, (int)x_right, y_scan,color);
						}
						x_left += change_x_left;
						x_right += change_x_right;
					}
				}

        }
        break;
    }
    }
}
