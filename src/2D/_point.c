#include "../_graphics.h"
#include "../utils.h"

Point2 _rotatePoint2D(renderContext* rc, int x, int y, float theta, axis ax) {
  tinyVec v = point2ToVec((Point2){.x = x, .y = y});
  tinyMatrix rotM = getRotationMatrix2D(theta, ax);
  tinyVec res = matrixVecMul(rotM, v);
  return vecToPoint2(res);
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
