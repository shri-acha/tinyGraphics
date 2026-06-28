#include "graphics.h"

int renderTriangle(renderContext *rc, Point2 *points[3]) {
    switch (rc->render_mode) {
    case WIREFRAME: {
        Point2 p1 = *(points[0]);
        Point2 p2 = *(points[1]);
        Point2 p3 = *(points[2]);
        renderLine(rc, p1, p2);
        renderLine(rc, p2, p3);
        renderLine(rc, p3, p1);
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
                    renderHorizontalLine(rc, (int)x_right, (int)x_left, y_scan);
                } else {
                    renderHorizontalLine(rc, (int)x_left, (int)x_right, y_scan);
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
                    renderHorizontalLine(rc, (int)x_right, (int)x_left, y_scan);
                } else {
                    renderHorizontalLine(rc, (int)x_left, (int)x_right, y_scan);
                }
                x_left += change_x_left;
                x_right += change_x_right;
            }
        }
        break;
    }
    }
    return 0;
}
int renderAngledTriangle(renderContext *rc,Point2 *points[3], float theta, int axis){
		Point2 p1 = (*points[0]);
		Point2 p2 = (*points[1]);
		Point2 p3 = (*points[2]);

      Point2 rt_points_0 = rotate_point(p1.x, p1.y, theta, axis);
      Point2 rt_points_1 = rotate_point(p2.x, p2.y, theta, axis);
      Point2 rt_points_2 = rotate_point(p3.x, p3.y, theta, axis);

		Point2* rt_points[3] = {&rt_points_0,&rt_points_1,&rt_points_2};

		return renderTriangle(rc, rt_points);

}
