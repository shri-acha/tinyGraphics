#include "../_graphics.h"

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

