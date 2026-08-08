#include "../graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>

int main() {

    frameBuffer* fb = createFrameBuffer(1600, 1600);
    renderContext rc = {
        .frame_buffer = fb, 
        .render_mode = WIREFRAME, 
        .origin = (Index){.x = 800, .y = 800, .z= 800}, 
        .scene_context = newSceneContext(),
        .camera_position= (Point3) {.x=0,.y=0,.z=200},
        .projection = PERSPECTIVE, 
        .focal_length = 400.0f, 
    };
    float theta = M_PI/2;

    if (!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "My Custom 3D Engine", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLuint textureID;
    TINY_GL_INIT_TEXTURE(fb, textureID);

    while (!glfwWindowShouldClose(window)) {

        Color grid_color;
        uint16_t grid_r = (uint16_t)(10 + 8 ) & 0x1F;
        uint16_t grid_b = (uint16_t)(20 + 11) & 0x1F;
        grid_color.literal = (grid_r << 11) | (0 << 5) | grid_b;


		  Point3 p1 = {.x = 50, .y = 0, .z=0};
		  Point3 p2 = {.x = 24, .y = 50,.z=0};
		  Point3 p3 = {.x = 100, .y = 100,.z=0}; 
		  
		  Point3 *points[3] = { &p1, &p2, &p3 };

		 renderAngledTriangle3D(&rc, points, theta, Z, grid_color);

        TINY_GL_PRESENT(window, fb, &rc, textureID);
    }

    destroyFrameBuffer(fb);
    destroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
