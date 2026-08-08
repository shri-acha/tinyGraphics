#include "../graphics.h"
#include <GLFW/glfw3.h>
#include <math.h>

int main() {

    frameBuffer* fb = createFrameBuffer(1600, 1600);
    renderContext rc = {
        .frame_buffer = fb, 
        .render_mode = WIREFRAME,
        .origin = (Index){.x = 600, .y = 600, .z= 800}, 
        .scene_context = newSceneContext(),
        .camera_position= (Point3) {.x=0,.y=0,.z=200},
        .projection = PERSPECTIVE, 
        .focal_length = 150.0f, 
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
    TINY_GL_INIT_TEXTURE(fb, textureID);

    Point3 apex       = (Point3){.x = 0,   .y = 100,  .z = 0};   // Top point
    Point3 front_left = (Point3){.x = -55, .y = -40, .z = -55}; // Base corner 1
    Point3 front_right= (Point3){.x = 55,  .y = -40, .z = -55}; // Base corner 2
    Point3 back_right = (Point3){.x = 55,  .y = -40, .z = 55};  // Base corner 3
    Point3 back_left  = (Point3){.x = -55, .y = -40, .z = 55};  // Base corner 4

    // --- Define the 6 Triangular Faces (4 Sides + 2 for the Square Base) ---
    Point3* t1[3] = { &apex, &front_left, &front_right }; // Front Face
    Point3* t2[3] = { &apex, &front_right, &back_right }; // Right Face
    Point3* t3[3] = { &apex, &back_right, &back_left };   // Back Face
    Point3* t4[3] = { &apex, &back_left, &front_left };   // Left Face
    Point3* t5[3] = { &front_left, &back_left, &front_right }; // Base Split 1
    Point3* t6[3] = { &back_right, &front_right, &back_left }; // Base Split 2

    while (!glfwWindowShouldClose(window)) {

        Color grid_color;
        uint16_t grid_r = (uint16_t)(10 + 8 ) & 0x1F;
        uint16_t grid_b = (uint16_t)(20 + 11) & 0x1F;
        grid_color.literal = (grid_r << 11) | (0 << 5) | grid_b;

        renderLine3D(&rc, (Point3){.x=-400,.y=0,.z=0}, (Point3){.x=400,.y=0,.z=0}, grid_color);
        renderLine3D(&rc, (Point3){.x=0,.y=-400,.z=0}, (Point3){.x=0,.y=400,.z=0}, grid_color);

        Point3** faces[6] = { t1, t2, t3, t4, t5, t6 };
        
        for (int i = 0; i < 6; i++) {
            float face_offset = theta + (i * (M_PI / 3.0f));
            
            uint16_t red   = (uint16_t)(15.5f + 15.5f ) & 0x1F;
            uint16_t green = (uint16_t)(31.5f + 31.5f ) & 0x3F;
            uint16_t blue  = (uint16_t)(15.5f + 15.5f ) & 0x1F;
            
            Color face_color;
            face_color.literal = (red << 11) | (green << 5) | blue;

            renderAngledTriangle3D(&rc, faces[i], theta, X, face_color);
        }

        // 3. OpenGL Display Pipeline
        TINY_GL_PRESENT(window, fb, &rc, textureID);
        
        // Speed control for the rotation
        theta += M_PI / 160.0f;
    }

    destroyFrameBuffer(fb);
    destroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
