#include "../graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>

int main() {

    frameBuffer* fb = createFrameBuffer(800, 800);
    renderContext rc = {
        .frame_buffer = fb, 
        .render_mode = WIREFRAME,
        .origin = (Index){.x = 400, .y = 400, .z= 400}, 
        .scene_context = newSceneContext(),
		  .camera_position= (Point3) {.x=0,.y=0,.z=200},
        .projection = PERSPECTIVE, 
        
        .focal_length = 100.0f, 
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

    // have to enable legacy opengl
    glEnable(GL_TEXTURE_2D);
    // texture minifying function
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // texture magnification function
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // initializing the texture to null
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb->width, fb->height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

	 Point3 p1 = (Point3){.x=-50, .y=-50,.z =0};
	 Point3 p2 = (Point3) { .x=50, .y=-50, .z=0};
	 Point3 p3 = (Point3) { .x=50,  .y=50, .z=0};

	 Point3* triangle_points[3] = { &p1, &p2, &p3 };

    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // bf edges
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // ff edges 
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // side bridge edges 
    };

    while (!glfwWindowShouldClose(window)) {
			Color depth_color;
			 uint16_t red   = 0;                                 
			 uint16_t green = (uint16_t)(63) & 0x3F;         
			 uint16_t blue  = (uint16_t)(31) & 0x1F;         	 
			 depth_color.literal = (red << 11) | (green << 5) | blue;

			renderLine3D(&rc,(Point3){.x=-400,.y=0,.z=0},(Point3){.x=400,.y=0,.z=0},depth_color);
			renderLine3D(&rc,(Point3){.x=0,.y=-400,.z=0},(Point3){.x=0,.y=400,.z=0},depth_color);
			renderAngledTriangle3D(&rc,triangle_points,theta,X,depth_color);

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
         
         theta += M_PI / 100;
    }

    destroyFrameBuffer(fb);
    destroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
