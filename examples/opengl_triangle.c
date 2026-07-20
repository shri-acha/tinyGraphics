#include "../graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>

int main() {

    frameBuffer* fb = createFrameBuffer(1600, 1600);
    renderContext rc = {
        .frame_buffer = fb, 
        .render_mode = FILLED,
        .origin = (Index){.x = 800, .y = 800, .z= 800}, 
        .scene_context = newSceneContext(),
        .camera_position= (Point3) {.x=0,.y=0,.z=200},
        .projection = PERSPECTIVE, 
        .focal_length = 400.0f, 
    };
    float theta = 0.0;

    if (!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "My Custom 3D Engine", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb->width, fb->height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

    while (!glfwWindowShouldClose(window)) {

        Color grid_color;
        uint16_t grid_r = (uint16_t)(10 + 8 ) & 0x1F;
        uint16_t grid_b = (uint16_t)(20 + 11) & 0x1F;
        grid_color.literal = (grid_r << 11) | (0 << 5) | grid_b;


		  Point3 p1 = {.x = 50, .y = 0, .z=0};
		  Point3 p2 = {.x = 24, .y = 50,.z=0};
		  Point3 p3 = {.x = 100, .y = 100,.z=0}; 
		  
		  Point3 *points[3] = { &p1, &p2, &p3 };

		 renderAngledTriangle3D(&rc, points, theta, Y, grid_color);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, fb->width, fb->height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, fb->buffer);

        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f); // bl 
            glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f); // br 
            glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f); // tr 
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f); // tl
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();

        flushPixelBuffer(fb->buffer, fb->width, fb->height);
        flushSceneContext(rc.scene_context);
        
        // Speed control for the rotation
        theta += M_PI / 160.0f;
    }

    destroyFrameBuffer(fb);
    destroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
