#include "../graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>


int main() {
    frameBuffer* fb = createFrameBuffer(1600, 1600);
    renderContext rc = {
        .frame_buffer = fb,
        .render_mode = FILLED,
        .origin = (Index){ .x = 800, .y = 800, .z = 0 },
        .scene_context = newSceneContext(),
        .camera_position = (Point3){ .x = 0, .y = 150, .z = 300 },
		.shading_mode = SHADE_GOURAUD,
        .projection = PERSPECTIVE,
        .focal_length = 200.0f,
    };

    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "3D Cube Orbit - tinyGraphics", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLuint textureID;
    TINY_GL_INIT_TEXTURE(fb, textureID);

    float theta = 0.0f;
    float orbit_radius = 350.0f;
    float cam_height = 150.0f;
    Point3 target = { .x = 0, .y = 0, .z = 0 };
    Vector4 world_up = (Vector4){ .inner = { 0.0f, 1.0f, 0.0f, 0.0f } };

    while (!glfwWindowShouldClose(window)) {
        rc.camera_position.x = (int)roundf(orbit_radius * sinf(theta));
        rc.camera_position.y = (int)roundf(cam_height);
        rc.camera_position.z = (int)roundf(orbit_radius * cosf(theta));

        rc.camera_direction = lookAt(rc.camera_position, target, world_up);

        Color grid_color = { .literal = (0x10 << 11) | (0x20 << 5) | 0x10 };

		  renderCircle3D(&rc,(Point3){0,0,0},100,(Color) {.literal = 0b1111100000000000});
        renderLine3D(&rc, (Point3){ .x = -400, .y = 0, .z = 0 }, (Point3){ .x = 400, .y = 0, .z = 0 }, grid_color);
        renderLine3D(&rc, (Point3){ .x = 0, .y = -400, .z = 0 }, (Point3){ .x = 0, .y = 400, .z = 0 }, grid_color);
        renderLine3D(&rc, (Point3){ .x = 0, .y = 0, .z = -400 }, (Point3){ .x = 0, .y = 0, .z = 400 }, grid_color);

        TINY_GL_PRESENT(window, fb, &rc, textureID);
        theta += M_PI / 180.0f;
    }

    destroyFrameBuffer(fb);
    destroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
