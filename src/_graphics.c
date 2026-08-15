#include "tiny_math.h"
#include "_graphics.h"
#include <stdlib.h>
#include "utils.h"
#include <stdio.h>


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

Point2 _project3D(renderContext* rc, Point3 p,int* depth) {
    float px = (float)p.x - (float)rc->camera_position.x;
    float py = (float)p.y - (float)rc->camera_position.y;
    float pz = (float)p.z - (float)rc->camera_position.z;

    static const float default_r[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
    static const float default_u[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
    static const float default_f[4] = { 0.0f, 0.0f, -1.0f, 0.0f };

    const float* r = rc->camera_direction.right.inner;
    const float* u = rc->camera_direction.up.inner;
    const float* f = rc->camera_direction.forward.inner;

    if (f[0] == 0.0f && f[1] == 0.0f && f[2] == 0.0f &&
        r[0] == 0.0f && r[1] == 0.0f && r[2] == 0.0f) {
        r = default_r;
        u = default_u;
        f = default_f;
    }

    float x_cam = r[0] * px + r[1] * py + r[2] * pz;
    float y_cam = u[0] * px + u[1] * py + u[2] * pz;
    float z_cam = f[0] * px + f[1] * py + f[2] * pz;

    if (depth != NULL) {
        *depth = (int)roundf(z_cam);
    }

    if (rc->projection == PERSPECTIVE) {
        if (z_cam <= 0.1f) {
            return (Point2){ .x = 0, .y = 0 };
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

void _drawHorizontalLineScreen(renderContext *rc, int x1, int x2, float z1, float z2, int y, Color color) {
  if (y < 0 || y >= rc->frame_buffer->height) return;
  if (x1 > x2) {
    int tmpx = x1; x1 = x2; x2 = tmpx;
    float tmpz = z1; z1 = z2; z2 = tmpz;
  }
  int width = rc->frame_buffer->width;
  if (x2 < 0 || x1 >= width) return;

  int steps = x2 - x1;
  float z_inc = (steps != 0) ? (z2 - z1) / (float)steps : 0.0f;
  float z = z1;
  int start_x = x1;
  int end_x = x2;

  if (start_x < 0) {
    z += (float)(0 - start_x) * z_inc;
    start_x = 0;
  }
  if (end_x >= width) {
    end_x = width - 1;
  }

  for (int i = start_x; i <= end_x; i++) {
    _drawPixel(rc, i, y, (int)roundf(z), color);
    z += z_inc;
  }
}

void _drawHorizontalLineScreenGouraud(renderContext *rc, int x1, int x2,
                                       float z1, float z2,
                                       float i1, float i2,
                                       int y, Color color) {
  if (y < 0 || y >= rc->frame_buffer->height) return;
  if (x1 > x2) {
    int tmpx = x1; x1 = x2; x2 = tmpx;
    float tmpz = z1; z1 = z2; z2 = tmpz;
    float tmpi = i1; i1 = i2; i2 = tmpi;
  }
  int width = rc->frame_buffer->width;
  if (x2 < 0 || x1 >= width) return;

  int steps = x2 - x1;
  float z_inc = (steps != 0) ? (z2 - z1) / (float)steps : 0.0f;
  float i_inc = (steps != 0) ? (i2 - i1) / (float)steps : 0.0f;
  float z = z1;
  float intensity = i1;
  int start_x = x1;
  int end_x = x2;

  if (start_x < 0) {
    float skip = (float)(0 - start_x);
    z += skip * z_inc;
    intensity += skip * i_inc;
    start_x = 0;
  }
  if (end_x >= width) {
    end_x = width - 1;
  }

  for (int px = start_x; px <= end_x; px++) {
    _drawPixel(rc, px, y, (int)roundf(z), colorScale(color, intensity));
    z += z_inc;
    intensity += i_inc;
  }
}
