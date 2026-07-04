#include "../graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec2 aPos;\n"
                                 "layout (location = 1) in vec2 aTexCoord;\n"
                                 "out vec2 TexCoord;\n"
                                 "void main() {\n"
                                 "gl_Position = vec4(aPos, 0.0, 1.0);\n"
                                 "TexCoord = aTexCoord;\n"
                                 "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec2 TexCoord;\n"
                                   "uniform sampler2D ourTexture;\n"
                                   "void main() {\n"
                                   "	FragColor = texture(ourTexture, TexCoord);\n"
                                   "}\0";

int main() {

    frameBuffer* fb = createFrameBuffer(800, 800, (uint16_t)0b0000011111100000);
    renderContext rc = {
        .frame_buffer = fb, 
        .render_mode = FILLED,
        .origin = (Index){.x = 400, .y = 400}, 
        .scene_context = newSceneContext()
    };
    float theta = 0.0;

    Point2 p1 = {.x = 200, .y = 0};
    Point2 p2 = {.x = 100, .y = 200};
    Point2 p3 = {.x = 400, .y = 400}; 
    Point2 *points[3] = { &p1, &p2, &p3 };

    if (!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 800, "My Custom 2D Engine", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

	 // have to enable legacy OpenGL
    glEnable(GL_TEXTURE_2D);
	 // texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified. 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified. 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// initializing the texture to null
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb->width, fb->height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

    while (!glfwWindowShouldClose(window)) {
		  // library function
        renderAngledTriangle(&rc, points, theta, Y);
        
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
        
        theta += M_PI / 12;
    }

    destroyFrameBuffer(fb);
    destroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
