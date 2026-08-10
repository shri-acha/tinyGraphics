#include "../_graphics.h"
#include "../utils.h"

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

		 /* Compute per-vertex Gouraud intensities (or 1.0 when shading is off) */
		 float vert_intensity[3] = { 1.0f, 1.0f, 1.0f };

		 if (rc->shading_mode == SHADE_GOURAUD) {
			 tinyVec v0 = point3ToVec(*points[0]);
			 tinyVec v1 = point3ToVec(*points[1]);
			 tinyVec v2 = point3ToVec(*points[2]);
			 tinyVec edge1 = vecSub(v1, v0);
			 tinyVec edge2 = vecSub(v2, v0);
			 tinyVec face_normal = vecNormalize(vecCross(edge1, edge2));

			 tinyVec light_dir = vecNormalize(rc->light.direction);
			 float ambient = rc->light.ambient;
			 float diffuse = rc->light.diffuse;

			 /* Flat-per-vertex: same face normal for all 3 vertices */
			 float ndotl = vecDot3(face_normal, light_dir);
			 if (ndotl < 0.0f) ndotl = 0.0f;
			 float intensity = ambient + diffuse * ndotl;
			 if (intensity > 1.0f) intensity = 1.0f;
			 vert_intensity[0] = intensity;
			 vert_intensity[1] = intensity;
			 vert_intensity[2] = intensity;
		 }

		 /* Reorder intensities to match sorted vertex order */
		 sort_point2(points_2, 3);

		 int idx0 = (int)(points_2[0] - tmp_buff);
		 int idx1 = (int)(points_2[1] - tmp_buff);
		 int idx2 = (int)(points_2[2] - tmp_buff);

		 int x1 = points_2[0]->x, y1 = points_2[0]->y, z1 = depth_buff[idx0];
		 int x2 = points_2[1]->x, y2 = points_2[1]->y, z2 = depth_buff[idx1];
		 int x3 = points_2[2]->x, y3 = points_2[2]->y, z3 = depth_buff[idx2];
		 float i1 = vert_intensity[idx0];
		 float i2 = vert_intensity[idx1];
		 float i3 = vert_intensity[idx2];

		 int y4 = y2;
		 int x4 = (y3-y1 != 0)?( x1 + (int)((float)(y4 - y1) * ((float)(x3 - x1) / (float)(y3 - y1))) ):0;
		 int z4 = (y3-y1 != 0)?( z1 + (int)((float)(y4 - y1) * ((float)(z3 - z1) / (float)(y3 - y1))) ):z1;
		 float i4 = (y3-y1 != 0)?( i1 + (float)(y4 - y1) * ((i3 - i1) / (float)(y3 - y1)) ):i1;

		 if (rc->shading_mode == SHADE_GOURAUD) {
			 // ---- GOURAUD SCANLINE RASTERIZATION ----
			 {
				// TOP HALF
				  float x_left = (float)x1;
				  float x_right = (float)x1;
				  float z_left = (float)z1;
				  float z_right = (float)z1;
				  float i_left = i1;
				  float i_right = i1;

				  if ( y2 != y1) {
				  float dy_top = (float)(y2 - y1);
				  float change_x_left = (float)(x2 - x1) / dy_top;
				  float change_x_right = (float)(x4 - x1) / dy_top;
				  float change_z_left = (float)(z2 - z1) / dy_top;
				  float change_z_right = (float)(z4 - z1) / dy_top;
				  float change_i_left = (i2 - i1) / dy_top;
				  float change_i_right = (i4 - i1) / dy_top;

						for (int y_scan = y1; y_scan < y2; y_scan++) {
							  if (x_left >= x_right) {
									 _drawHorizontalLineScreenGouraud(rc, (int)x_right, (int)x_left, z_right, z_left, i_right, i_left, y_scan, color);
							  } else {
									 _drawHorizontalLineScreenGouraud(rc, (int)x_left, (int)x_right, z_left, z_right, i_left, i_right, y_scan, color);
							  }
							  x_left += change_x_left;
							  x_right += change_x_right;
							  z_left += change_z_left;
							  z_right += change_z_right;
							  i_left += change_i_left;
							  i_right += change_i_right;
						}
				  }
			 }

			 {
				// BOTTOM HALF
				  float x_left = (float)x2;
				  float x_right = (float)x4;
				  float z_left = (float)z2;
				  float z_right = (float)z4;
				  float i_left = i2;
				  float i_right = i4;

				  if ( y3 != y2) {
						float dy_bot = (float)(y3 - y2);
						float change_x_left = (float)(x3 - x2) / dy_bot;
						float change_x_right = (float)(x3 - x4) / dy_bot;
						float change_z_left = (float)(z3 - z2) / dy_bot;
						float change_z_right = (float)(z3 - z4) / dy_bot;
						float change_i_left = (i3 - i2) / dy_bot;
						float change_i_right = (i3 - i4) / dy_bot;

						for (int y_scan = y2; y_scan < y3; y_scan++) {
							 if (x_left >= x_right) {
								  _drawHorizontalLineScreenGouraud(rc, (int)x_right, (int)x_left, z_right, z_left, i_right, i_left, y_scan, color);
							 } else {
								  _drawHorizontalLineScreenGouraud(rc, (int)x_left, (int)x_right, z_left, z_right, i_left, i_right, y_scan, color);
							 }
							 x_left += change_x_left;
							 x_right += change_x_right;
							 z_left += change_z_left;
							 z_right += change_z_right;
							 i_left += change_i_left;
							 i_right += change_i_right;
						}
				  }
			 }
		 } else {
			 // ---- ORIGINAL FLAT SCANLINE RASTERIZATION ----
			 {
				// TOP HALF
				  float x_left = (float)x1;
				  float x_right = (float)x1;
				  float z_left = (float)z1;
				  float z_right = (float)z1;

				  if ( y2 != y1) {
				  float change_x_left = (y2 - y1 > 0)?((float)(x2 - x1) / (float)(y2 - y1)):((float)(x4 - x1) / (float)(y1 - y2));
				  float change_x_right = (y2 - y1 > 0)?((float)(x4 - x1) / (float)(y2 - y1)):((float)(x4 - x1) / (float)(y1 - y2));
				  float change_z_left = (y2 - y1 > 0)?((float)(z2 - z1) / (float)(y2 - y1)):((float)(z4 - z1) / (float)(y1 - y2));
				  float change_z_right = (y2 - y1 > 0)?((float)(z4 - z1) / (float)(y2 - y1)):((float)(z4 - z1) / (float)(y1 - y2));

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
		 }
		 break;
	}
    }
}
