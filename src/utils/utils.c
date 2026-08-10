#include "../utils.h"
#include <stdlib.h>

/*Gets the value of the pixel for the pixelBuffer at (x,y) */
pixelBuffer get_pixel(frameBuffer *fb, int x, int y) {
  return fb->buffer[get_index(fb, x, y)];
}

/* General comparison operations*/
int compare_value(Operator op, int p1, int p2) {
  switch (op) {
  case EQ:
    if (p1 == p2) {
      return 0;
    }
    break;
  case GT:
    if (p1 > p2) {
      return 0;
    }
    break;
  case LT:
    if (p1 < p2) {
      return 0;
    }
    break;
  default:
    return 1;
  }
  return 1;
}

int get_index(frameBuffer *fb, int x, int y) {
  return y * fb->width + x;
}

int sort_point2(Point2 **points, size_t points_len) {
  for (size_t i = 0; i < points_len - 1; ++i) {
    for (size_t j = 0; j < points_len - 1 - i; ++j) {
      if (!compare_value(GT, points[j]->y, points[j + 1]->y)) {
        Point2 *temp = points[j];
        points[j] = points[j + 1];
        points[j + 1] = temp;
      }
    }
  }
  return 0;
}

Vector4 point_to_vector(Point3 p, int w) {
	return (Vector4) {
		.inner = { (float)p.x, (float)p.y, (float)p.z, (float)w }
	};
} 

DirectionVector lookAt(Point3 camera_pos, Point3 target, Vector4 world_up) {
    float fx = (float)target.x - (float)camera_pos.x;
    float fy = (float)target.y - (float)camera_pos.y;
    float fz = (float)target.z - (float)camera_pos.z;
    float f_len = sqrtf(fx*fx + fy*fy + fz*fz);
    if (f_len > 0.0001f) { fx /= f_len; fy /= f_len; fz /= f_len; }

    float rx = fy * world_up.inner[2] - fz * world_up.inner[1];
    float ry = fz * world_up.inner[0] - fx * world_up.inner[2];
    float rz = fx * world_up.inner[1] - fy * world_up.inner[0];
    float r_len = sqrtf(rx*rx + ry*ry + rz*rz);
    if (r_len > 0.0001f) { rx /= r_len; ry /= r_len; rz /= r_len; }

    float ux = ry * fz - rz * fy;
    float uy = rz * fx - rx * fz;
    float uz = rx * fy - ry * fx;

    return (DirectionVector){
        .right   = (tinyVec){ .inner = { rx, ry, rz, 0.0f } },
        .up      = (tinyVec){ .inner = { ux, uy, uz, 0.0f } },
        .forward = (tinyVec){ .inner = { fx, fy, fz, 0.0f } }
    };
}


float clamp01(float v) {
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

Color kd_to_color(const float kd[3]) {
    uint16_t r5 = (uint16_t)(clamp01(kd[0]) * 31.0f);
    uint16_t g6 = (uint16_t)(clamp01(kd[1]) * 63.0f);
    uint16_t b5 = (uint16_t)(clamp01(kd[2]) * 31.0f);
    return (Color){ .literal = (uint16_t)((r5 << 11) | (g6 << 5) | b5) };
}

Color colorScale(Color c, float intensity) {
    if (intensity < 0.0f) intensity = 0.0f;
    if (intensity > 1.0f) intensity = 1.0f;
    uint16_t r5 = (c.literal >> 11) & 0x1F;
    uint16_t g6 = (c.literal >>  5) & 0x3F;
    uint16_t b5 =  c.literal        & 0x1F;
    r5 = (uint16_t)(r5 * intensity);
    g6 = (uint16_t)(g6 * intensity);
    b5 = (uint16_t)(b5 * intensity);
    return (Color){ .literal = (uint16_t)((r5 << 11) | (g6 << 5) | b5) };
}
