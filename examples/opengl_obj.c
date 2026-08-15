#include "../graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

static float g_theta = 0.0f;
static float g_orbit_radius = 350.0f;
static float g_cam_height = 150.0f;
static int g_auto_rotate = 1;
static Point2 g_last_mouse_pos = { .x = 0, .y = 0 };
static int g_mouse_down = 0;

void handleEvent(Event e) {
    if (e.ev_typ == KEYBOARD) {
        if (e.ke.state == DOWN) {
            switch (e.ke.keycode) {
                case GLFW_KEY_A:
                case GLFW_KEY_LEFT:
                    g_theta += 0.1f;
                    g_auto_rotate = 0;
                    break;
                case GLFW_KEY_D:
                case GLFW_KEY_RIGHT:
                    g_theta -= 0.1f;
                    g_auto_rotate = 0;
                    break;
                case GLFW_KEY_W:
                case GLFW_KEY_UP:
                    g_orbit_radius -= 25.0f;
                    if (g_orbit_radius < 50.0f) g_orbit_radius = 50.0f;
                    break;
                case GLFW_KEY_S:
                case GLFW_KEY_DOWN:
                    g_orbit_radius += 25.0f;
                    if (g_orbit_radius > 1200.0f) g_orbit_radius = 1200.0f;
                    break;
                case GLFW_KEY_E:
                    g_cam_height += 25.0f;
                    break;
                case GLFW_KEY_Q:
                    g_cam_height -= 25.0f;
                    break;
                case GLFW_KEY_SPACE:
                    g_auto_rotate = !g_auto_rotate;
                    printf("Auto-rotate: %s\n", g_auto_rotate ? "ON" : "OFF");
                    break;
                case GLFW_KEY_R:
                    g_theta = 0.0f;
                    g_orbit_radius = 350.0f;
                    g_cam_height = 150.0f;
                    g_auto_rotate = 1;
                    printf("Camera reset\n");
                    break;
                default:
                    break;
            }
        }
    } else if (e.ev_typ == MOUSE) {
        if (e.me.state == DOWN) {
            g_mouse_down = 1;
            g_last_mouse_pos = e.me.pos;
            if (e.me.btn == LEFT) {
                /* Drag or click to change angle */
                g_theta += (float)(e.me.pos.x - 960) * 0.002f;
                g_cam_height += (float)(540 - e.me.pos.y) * 0.5f;
                g_auto_rotate = 0;
            } else if (e.me.btn == RIGHT) {
                g_orbit_radius += 50.0f;
                if (g_orbit_radius > 1200.0f) g_orbit_radius = 150.0f;
            }
        } else {
            g_mouse_down = 0;
        }
    }
}

int main() {
    fastObjMesh* chicken_mesh = fast_obj_read("./models/Chicken_01.obj");
    // fastObjMesh* couch_mesh = fast_obj_read("./models/77.obj");
    if (!chicken_mesh) {
        fprintf(stderr, "failed to load ./models/Chicken_01.obj\n");
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
        .focal_length = 150.0f,
        .shading_mode = SHADE_GOURAUD,
        .light = {
            .direction = (tinyVec){ .inner = { 0.0f, 1.0f, 1.0f, 0.0f } },
            .ambient = 0.2f,
            .diffuse = 0.8f
        },
    };

    registerEventHandler(&rc, handleEvent);

    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "3D Mesh Orbit - tinyGraphics", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /* Register GLFW callbacks to push events into renderContext ring buffer */
    TINY_GL_REGISTER_EVENTS(window, &rc);

    GLuint textureID;
    TINY_GL_INIT_TEXTURE(fb, textureID);

    Color mesh_color = { .literal = (0x1F << 11) | (0x3F << 5) | 0x00 };
    Point3 chicken_mesh_offset = { .x = 0, .y = 50, .z = 0 };
    Point3 couch_mesh_offset = { .x = -250, .y = 0, .z = 500 };
    Point3 target = { .x = 0, .y = 0, .z = 0 };
    Vector4 world_up = (Vector4){ .inner = { 0.0f, 1.0f, 0.0f, 0.0f } };

    printf("=== tinyGraphics Interactive 3D Model Viewer ===\n");
    printf("Controls:\n");
    printf("  A / D or Left / Right : Orbit Camera\n");
    printf("  W / S or Up / Down    : Zoom In / Zoom Out\n");
    printf("  E / Q                 : Camera Height Up / Down\n");
    printf("  Space                 : Toggle Auto-Rotation\n");
    printf("  R                     : Reset Camera\n");
    printf("  Mouse Left Click      : Orbit to Click Position\n");
    printf("  Mouse Right Click     : Cycle Zoom Distance\n");

    while (!glfwWindowShouldClose(window)) {
        if (g_auto_rotate) {
            g_theta += M_PI / 120.0f;
        }

        rc.camera_position.x = (int)roundf(g_orbit_radius * sinf(g_theta));
        rc.camera_position.y = (int)roundf(g_cam_height);
        rc.camera_position.z = (int)roundf(g_orbit_radius * cosf(g_theta));
        rc.camera_direction = lookAt(rc.camera_position, target, world_up);

        Color grid_color = { .literal = (0x10 << 11) | (0x20 << 5) | 0x10 };
        renderLine3D(&rc, (Point3){ .x = -400, .y = 0, .z = 0 }, (Point3){ .x = 400, .y = 0, .z = 0 }, grid_color);
        renderLine3D(&rc, (Point3){ .x = 0, .y = -400, .z = 0 }, (Point3){ .x = 0, .y = 400, .z = 0 }, grid_color);
        renderLine3D(&rc, (Point3){ .x = 0, .y = 0, .z = -400 }, (Point3){ .x = 0, .y = 0, .z = 400 }, grid_color);

		for (int i=0;i<10;i++){
			renderMesh3D(&rc, chicken_mesh, (Point3) {.x = chicken_mesh_offset.x +50*i,.y= chicken_mesh_offset.y+ 50*i, .z=chicken_mesh_offset.z + 50*i }, mesh_color, 1.0f);
		}
        // renderMesh3D(&rc, couch_mesh, couch_mesh_offset, mesh_color, 1.5f);

        TINY_GL_PRESENT(window, fb, &rc, textureID);
    }

    fast_obj_destroy(chicken_mesh);
    // fast_obj_destroy(couch_mesh);
    destroyFrameBuffer(fb);
    destroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

