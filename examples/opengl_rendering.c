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
        
        .projection = ORTHOGRAPHIC, 
        
        .focal_length = 10.0f, 
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
    
    Point3 local_vertices[8] = {
        {-50, -50, -50}, { 50, -50, -50}, { 50,  50, -50}, {-50,  50, -50}, // bf 
        {-50, -50,  50}, { 50, -50,  50}, { 50,  50,  50}, {-50,  50,  50}  // ff 
    };
	 Point3 p1 = (Point3){.x=-50, .y=-50,.z =-50};
	 Point3 p2 = (Point3) { .x=50, .y=-50, .z=-50};
	 Point3 p3 = (Point3) { .x=50,  .y=50, .z=-50};

	 Point3* triangle_points[3] = { &p1, &p2, &p3 };

    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // bf edges
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // ff edges 
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // side bridge edges 
    };

    while (!glfwWindowShouldClose(window)) {
			Point3 transformed_vertices[8];

			float dynamic_z_offset = 250.0f + sinf(theta * 0.5f) * 100.0f;

			for (int i = 0; i < 8; i++) {
				 float x1 = local_vertices[i].x * cosf(theta) - local_vertices[i].z * sinf(theta);
				 float z1 = local_vertices[i].x * sinf(theta) + local_vertices[i].z * cosf(theta);

				 float y2 = local_vertices[i].y * cosf(theta * 0.5f) - z1 * sinf(theta * 0.5f);
				 float z2 = local_vertices[i].y * sinf(theta * 0.5f) + z1 * cosf(theta * 0.5f);

				 transformed_vertices[i] = (Point3){
					  .x = x1,
					  .y = y2,
					  .z = z2 + dynamic_z_offset 
				 };
			}

			for (int i = 0; i < 12; i++) {
				 Point3 p1 = transformed_vertices[edges[i][0]];
				 Point3 p2 = transformed_vertices[edges[i][1]];

				 float avg_z = (p1.z + p2.z) / 2.0f;

				 float t = (430.0f - avg_z) / (430.0f - 120.0f);
				 if (t > 1.0f) t = 1.0f;
				 if (t < 0.1f) t = 0.1f; 

				 uint16_t red   = 0;                                 // 5 bits
				 uint16_t green = (uint16_t)(63 * t) & 0x3F;         // 6 bits
				 uint16_t blue  = (uint16_t)(31 * t) & 0x1F;         // 5 bits
				 
				 Color depth_color;
				 depth_color.literal = (red << 11) | (green << 5) | blue;

				 renderLine3D(&rc, p1, p2, depth_color);
				 renderAngledTriangle3D(&rc, triangle_points,theta,Y,depth_color);
			}

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
