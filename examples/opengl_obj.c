#include "../graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

int main() {
    fastObjMesh* chicken_mesh = fast_obj_read("./models/Chicken_01.obj");
    fastObjMesh* couch_mesh = fast_obj_read("./models/77.obj");
    if (!chicken_mesh || !couch_mesh) {
        fprintf(stderr, "failed to load\n");
        return -1;
    }

    frameBuffer* fb = createFrameBuffer(1600, 1600);
    renderContext rc = {
        .frame_buffer = fb,
        .render_mode = FILLED,
        .origin = (Index){ .x = 800, .y = 800, .z = 0 },
        .scene_context = newSceneContext(),
        .camera_position = (Point3){ .x = 0, .y = 150, .z = 300 },
        .projection = PERSPECTIVE,
        .focal_length = 200.0f,
        .shading_mode = SHADE_GOURAUD,
        .light = {
            .direction = (tinyVec){ .inner = { 1.0f, 1.0f, 1.0f, 0.0f } },
            .ambient = 0.2f,
            .diffuse = 0.8f
        },
    };

    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "3D Mesh Orbit - tinyGraphics", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    GLuint textureID;
    TINY_GL_INIT_TEXTURE(fb, textureID);

    Color mesh_color = { .literal = (0x1F << 11) | (0x3F << 5) | 0x00 };
    Point3 chicken_mesh_offset = { .x = 0, .y = 50, .z = 0 };
    Point3 couch_mesh_offset = { .x = -250, .y = 0, .z = 500 };
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
        renderLine3D(&rc, (Point3){ .x = -400, .y = 0, .z = 0 }, (Point3){ .x = 400, .y = 0, .z = 0 }, grid_color);
        renderLine3D(&rc, (Point3){ .x = 0, .y = -400, .z = 0 }, (Point3){ .x = 0, .y = 400, .z = 0 }, grid_color);
        renderLine3D(&rc, (Point3){ .x = 0, .y = 0, .z = -400 }, (Point3){ .x = 0, .y = 0, .z = 400 }, grid_color);

        renderMesh3D(&rc, chicken_mesh, chicken_mesh_offset, mesh_color, 0.75f);
        renderMesh3D(&rc, couch_mesh, couch_mesh_offset, mesh_color, 1.5f);

        TINY_GL_PRESENT(window, fb, &rc, textureID);
        theta += M_PI / 180.0f;
    }

    fast_obj_destroy(chicken_mesh);
    fast_obj_destroy(couch_mesh);
    destroyFrameBuffer(fb);
    destroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
