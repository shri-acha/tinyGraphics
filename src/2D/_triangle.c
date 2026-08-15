#include "../_graphics.h"
#include "../utils.h"

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

        int fb_w = rc->frame_buffer->width;
        int fb_h = rc->frame_buffer->height;
        if (y3 < 0 || y1 >= fb_h) break;
        int min_x = x1; if (x2 < min_x) min_x = x2; if (x3 < min_x) min_x = x3; if (x4 < min_x) min_x = x4;
        int max_x = x1; if (x2 > max_x) max_x = x2; if (x3 > max_x) max_x = x3; if (x4 > max_x) max_x = x4;
        if (max_x < 0 || min_x >= fb_w) break;

		  // USES THE DDA TO RENDER EACH LINE 
				{
			  // TOP HALF
            float x_left = (float)x1;
            float x_right = (float)x1;

				float change_x_left = (y2 - y1 != 0) ? (float)(x2 - x1) / (float)(y2 - y1) : 0.0f;
				float change_x_right = (y2 - y1 != 0) ? (float)(x4 - x1) / (float)(y2 - y1) : 0.0f;

            int start_y = y1;
            int end_y = y2;
            if (start_y < 0) {
                float skip = (float)(0 - start_y);
                x_left += change_x_left * skip;
                x_right += change_x_right * skip;
                start_y = 0;
            }
            if (end_y > fb_h) end_y = fb_h;

            for (int y_scan = start_y; y_scan <= end_y; y_scan++) {
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

            int start_y = y2;
            int end_y = y3;
            if (start_y < 0) {
                float skip = (float)(0 - start_y);
                x_left += change_x_left * skip;
                x_right += change_x_right * skip;
                start_y = 0;
            }
            if (end_y > fb_h) end_y = fb_h;

            for (int y_scan = start_y; y_scan < end_y; y_scan++) {
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
