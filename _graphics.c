#include "tiny_math.h"
#include "types.h"
#include <stdlib.h>
#include "_graphics.h"
#include "utils.h"
#include <stdio.h>


Point2 _rotatePoint2D(renderContext* rc, int x, int y, float theta, axis ax) {
  tinyVec v = point2ToVec((Point2){.x = x, .y = y});
  tinyMatrix rotM = getRotationMatrix2D(theta, ax);
  tinyVec res = matrixVecMul(rotM, v);
  return vecToPoint2(res);
}

Point3 _rotatePoint3D(renderContext* rc, int x, int y, int z, float theta, axis ax) {
  tinyVec v = point3ToVec((Point3){.x = x, .y = y, .z = z});
  tinyMatrix rotM = getRotationMatrix3D(theta, ax);
  tinyVec res = matrixVecMul(rotM, v);
  return vecToPoint3(res);
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

void _renderPoint2D(renderContext *rc, Point2 p, Color color) {
  tinyVec v = point2ToVec(p);
  tinyMatrix transM = getTranslationMatrix((float)rc->origin.x, (float)rc->origin.y, 0.0f);
  tinyVec v_trans = matrixVecMul(transM, v);
  Point2 p_screen = vecToPoint2(v_trans);
  int x = p_screen.x, y = p_screen.y;
  int width = rc->frame_buffer->width;
  int height = rc->frame_buffer->height;

  if (x >= width || y >= height || x < 0 || y < 0) {
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

Point2 _project3D(renderContext* rc, Point3 p,int* depth) {
    float px = (float)p.x - (float)rc->camera_position.x;
    float py = (float)p.y - (float)rc->camera_position.y;
    float pz = (float)p.z - (float)rc->camera_position.z;

    float* r = rc->camera_direction.right.inner;
    float* u = rc->camera_direction.up.inner;
    float* f = rc->camera_direction.forward.inner;

    float x_cam = r[0] * px + r[1] * py + r[2] * pz;
    float y_cam = u[0] * px + u[1] * py + u[2] * pz;
    float z_cam = f[0] * px + f[1] * py + f[2] * pz;

    if (depth != NULL) {
        *depth = (int)roundf(z_cam);
    }

    if (rc->projection == PERSPECTIVE) {
        if (z_cam <= 0.1f) {
            return (Point2){ .x = -9999, .y = -9999 };
        }

        float x_proj = (x_cam * rc->focal_length) / z_cam;
        float y_proj = (y_cam * rc->focal_length) / z_cam;

        return (Point2){
            .x = (int)roundf(x_proj + (float)rc->origin.x),
            .y = (int)roundf((float)rc->origin.y - y_proj)
        };
    } else {
        return (Point2){
            .x = (int)roundf(x_cam + (float)rc->origin.x),
            .y = (int)roundf((float)rc->origin.y - y_cam)
        };
    }
}


void _renderLine3D(renderContext* rc, Point3 p1, Point3 p2, Color color) {
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int dz = abs(p2.z - p1.z);

    int steps = dx;
    if (dy > steps) steps = dy;
    if (dz > steps) steps = dz;

    if (steps == 0) {
        _renderPoint3D(rc, p1, color);
        return;
    }

    float x_inc = (float)(p2.x - p1.x) / (float)steps;
    float y_inc = (float)(p2.y - p1.y) / (float)steps;
    float z_inc = (float)(p2.z - p1.z) / (float)steps;

    float x = (float)p1.x;
    float y = (float)p1.y;
    float z = (float)p1.z;

    for (int i = 0; i <= steps; i++) {
        Point3 p = {
            .x = (int)roundf(x),
            .y = (int)roundf(y),
            .z = (int)roundf(z)
        };
        _renderPoint3D(rc, p, color);
        x += x_inc;
        y += y_inc;
        z += z_inc;
    }
}

int _drawPixel(renderContext *rc, int x, int y, int z, Color color) {
  int width = rc->frame_buffer->width;
  int height = rc->frame_buffer->height;

  if (x >= width || y >= height || x < 0 || y < 0) {
    return -1;
  }

  int idx = get_index(rc->frame_buffer, x, y);
  if (rc->frame_buffer->depth_buffer == NULL || rc->frame_buffer->depth_buffer[idx] > z) {
      rc->frame_buffer->buffer[idx].color = color;
      if (rc->frame_buffer->depth_buffer) {
          rc->frame_buffer->depth_buffer[idx] = z;
      }
  }
  return 0;
}

static void _drawHorizontalLineScreen(renderContext *rc, int x1, int x2,float z1,float z2, int y, Color color)  {
	if (x1 > x2) {
		int tmpx = x1; x1 = x2; x2 = tmpx;
    float tmpz = z1; z1 = z2; z2 = tmpz;
  }
  int steps = x2 - x1;
  float z_inc = (steps != 0) ? (z2 - z1) / (float)steps : 0.0f;
  float z = z1;
  for (int i = x1; i <= x2; i++) {
    _drawPixel(rc, i, y, (int)roundf(z), color);
    z += z_inc;
  }
}

void _renderPoint3D(renderContext *rc, Point3 p,Color color) {
	int depth;
	Point2 projected = _project3D(rc,p,&depth);
	_drawPixel(rc, projected.x, projected.y, depth, color);
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
		 int depth_buff[3];
		 for (int i=0;i<3;i++) {
				tmp_buff[i] = _project3D(rc, *points[i], &depth_buff[i]);
				points_2[i] = &tmp_buff[i];
		 }

		 sort_point2(points_2, 3);

		 int idx0 = (int)(points_2[0] - tmp_buff);
		 int idx1 = (int)(points_2[1] - tmp_buff);
		 int idx2 = (int)(points_2[2] - tmp_buff);

		 int x1 = points_2[0]->x, y1 = points_2[0]->y, z1 = depth_buff[idx0];
		 int x2 = points_2[1]->x, y2 = points_2[1]->y, z2 = depth_buff[idx1];
		 int x3 = points_2[2]->x, y3 = points_2[2]->y, z3 = depth_buff[idx2];

		 int y4 = y2;
		 int x4 = (y3-y1 != 0)?( x1 + (int)((float)(y4 - y1) * ((float)(x3 - x1) / (float)(y3 - y1))) ):0;
		 int z4 = (y3-y1 != 0)?( z1 + (int)((float)(y4 - y1) * ((float)(z3 - z1) / (float)(y3 - y1))) ):z1;

		 {
			// TOP HALF
			  float x_left = (float)x1;
			  float x_right = (float)x1;
			  float z_left = (float)z1;
			  float z_right = (float)z1;

			  if ( y2 != y1) {
			  float change_x_left = (y2 - y1 != 0)?((float)(x2 - x1) / (float)(y2 - y1)):((float)(x4 - x1) / (float)(y1 - y2));
			  float change_x_right = (y2 - y1 != 0)?((float)(x4 - x1) / (float)(y2 - y1)):((float)(x4 - x1) / (float)(y1 - y2));
			  float change_z_left = (y2 - y1 != 0)?((float)(z2 - z1) / (float)(y2 - y1)):((float)(z4 - z1) / (float)(y1 - y2));
			  float change_z_right = (y2 - y1 != 0)?((float)(z4 - z1) / (float)(y2 - y1)):((float)(z4 - z1) / (float)(y1 - y2));

					for (int y_scan = y1; y_scan < y2; y_scan++) {
						  if (x_left >= x_right) {
								 _drawHorizontalLineScreen(rc, (int)x_right, (int)x_left, z_right, z_left, y_scan,color);
						  } else {
								 _drawHorizontalLineScreen(rc, (int)x_left, (int)x_right, z_left, z_right, y_scan,color);
						  }
						  x_left += change_x_left;
						  x_right += change_x_right;
						  z_left += change_z_left;
						  z_right += change_z_right;
					}
			  }
		 }

		 {
			// BOTTOM HALF
			  float x_left = (float)x2;
			  float x_right = (float)x4;
			  float z_left = (float)z2;
			  float z_right = (float)z4;

			  if ( y3 != y2) {
					float change_x_left = (y3-y2 > 0)?((float)(x3 - x2) / (float)(y3 - y2)):((float)(x3 - x2) / (float)(y2 - y3));
					float change_x_right = (y3-y2 > 0)?((float)(x3 - x4) / (float)(y3 - y2)):((float)(x3 - x2) / (float)(y2 - y3));
					float change_z_left = (y3-y2 > 0)?((float)(z3 - z2) / (float)(y3 - y2)):((float)(z3 - z2) / (float)(y2 - y3));
					float change_z_right = (y3-y2 > 0)?((float)(z3 - z4) / (float)(y3 - y2)):((float)(z3 - z2) / (float)(y2 - y3));

					for (int y_scan = y2; y_scan < y3; y_scan++) {
						 if (x_left >= x_right) {
							  _drawHorizontalLineScreen(rc, (int)x_right, (int)x_left, z_right, z_left, y_scan,color);
						 } else {
							  _drawHorizontalLineScreen(rc, (int)x_left, (int)x_right, z_left, z_right, y_scan,color);
						 }
						 x_left += change_x_left;
						 x_right += change_x_right;
						 z_left += change_z_left;
						 z_right += change_z_right;
					}
			  }
		 }
		 break;
	}
    }
}

