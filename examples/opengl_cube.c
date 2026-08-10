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

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "3D Cube Orbit - tinyGraphics", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLuint textureID;
    TINY_GL_INIT_TEXTURE(fb, textureID);

    int s = 60; 
    Point3 v0 = { .x = -s, .y = -s, .z = -s };
    Point3 v1 = { .x =  s, .y = -s, .z = -s };
    Point3 v2 = { .x =  s, .y =  s, .z = -s };
    Point3 v3 = { .x = -s, .y =  s, .z = -s };
    Point3 v4 = { .x = -s, .y = -s, .z =  s };
    Point3 v5 = { .x =  s, .y = -s, .z =  s };
    Point3 v6 = { .x =  s, .y =  s, .z =  s };
    Point3 v7 = { .x = -s, .y =  s, .z =  s };

    Point3* t1[3]  = { &v4, &v5, &v6 }; 
    Point3* t2[3]  = { &v4, &v6, &v7 }; 
    Point3* t3[3]  = { &v1, &v0, &v3 }; 
    Point3* t4[3]  = { &v1, &v3, &v2 };
    Point3* t5[3]  = { &v3, &v2, &v6 };
    Point3* t6[3]  = { &v3, &v6, &v7 };
    Point3* t7[3]  = { &v4, &v5, &v1 }; 
    Point3* t8[3]  = { &v4, &v1, &v0 }; 
    Point3* t9[3]  = { &v5, &v1, &v2 }; 
    Point3* t10[3] = { &v5, &v2, &v6 };
    Point3* t11[3] = { &v0, &v4, &v7 };
    Point3* t12[3] = { &v0, &v7, &v3 }; 

    Point3** faces[12] = { t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12 };

    // Distinct colors for each face pair
    Color face_colors[6] = {
        { .literal = (0x1F << 11) | (0x00 << 5) | 0x00 }, 
        { .literal = (0x00 << 11) | (0x3F << 5) | 0x00 },
        { .literal = (0x00 << 11) | (0x00 << 5) | 0x1F }, 
        { .literal = (0x1F << 11) | (0x3F << 5) | 0x00 }, 
        { .literal = (0x1F << 11) | (0x00 << 5) | 0x1F }, 
        { .literal = (0x00 << 11) | (0x3F << 5) | 0x1F }
    };

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

        for (int i = 0; i < 12; i++) {
            Color color = face_colors[i / 2];
            renderTriangle3D(&rc, faces[i], color);
        }

        TINY_GL_PRESENT(window, fb, &rc, textureID);
        theta += M_PI / 180.0f;
    }

    destroyFrameBuffer(fb);
    destroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
