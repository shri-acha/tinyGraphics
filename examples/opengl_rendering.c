#include "../graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>

int main() {

    frameBuffer* fb = createFrameBuffer(800, 800);
    renderContext rc = {
        .frame_buffer = fb, 
        .render_mode = FILLED,
        .origin = (Index){.x = 400, .y = 400}, 
        .scene_context = newSceneContext(),
    };
    float theta = 0.0;

    if (!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "My Custom 2D Engine", NULL, NULL);
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


	 // have to enable legacy OpenGL
    glEnable(GL_TEXTURE_2D);
	 // texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified. 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified. 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// initializing the texture to null
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb->width, fb->height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    while (!glfwWindowShouldClose(window)) {
		 // library functions
		 renderLine(&rc,(Point2) {.x = 0,.y = -400}, (Point2) {.x = 0,.y = 400},(Color) {.literal = 0b0000011111100000});
		 renderLine(&rc,(Point2) {.x = -400,.y = 0},(Point2) {.x = 400,.y = 0},(Color) {.literal = 0b0000011111100000});
		 renderAngledCircle(&rc, (Point2){.x = 100,.y=100}, 100, theta, Z,(Color) {.literal = 0b0000011111100000});
		 renderAngledCircle(&rc, (Point2){.x = 100,.y=100}, 50, theta, Z,(Color) {.literal = 0xFFFF});
		  // binds GL_TEXTURE_2D into textureID
        glBindTexture(GL_TEXTURE_2D, textureID);
		  // initializing the texture to the framebuffer 
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, fb->width, fb->height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, fb->buffer);

		  // binds GL_TEXTURE_2D into textureID
			glClear(GL_COLOR_BUFFER_BIT);

		  // maps texture onto the quads
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
