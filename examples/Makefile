CFLAGS = -I vendor/fast_obj/
LIBS = -lm -lglfw -lGL
BUILD_DIR = build/

opengl_prism: opengl_prism.c ../3D/*.c ../2D/*.c ../*.c
	@mkdir -p $(BUILD_DIR)
	gcc opengl_prism.c ../2D/*.c ../3D/*.c ../*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_prism 

opengl_triangle: opengl_triangle.c ../3D/*.c ../2D/*.c ../*.c
	@mkdir -p $(BUILD_DIR)
	gcc opengl_triangle.c ../2D/*.c ../3D/*.c ../*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_triangle 

opengl_circle: opengl_circle.c ../3D/*.c ../2D/*.c ../*.c
	@mkdir -p $(BUILD_DIR)
	gcc opengl_circle.c ../2D/*.c ../3D/*.c ../*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_circle 

opengl_cube: opengl_cube.c ../3D/*.c ../2D/*.c ../*.c
	@mkdir -p $(BUILD_DIR)
	gcc opengl_cube.c ../2D/*.c ../3D/*.c ../*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_cube

opengl_gouraud_cube: opengl_gouraud_cube.c ../3D/*.c ../2D/*.c ../*.c
	@mkdir -p $(BUILD_DIR)
	gcc opengl_gouraud_cube.c ../2D/*.c ../3D/*.c ../*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_gouraud_cube


opengl_obj: opengl_obj.c ../3D/*.c ../2D/*.c ../*.c
	@mkdir -p $(BUILD_DIR)
	gcc opengl_obj.c ../2D/*.c ../3D/*.c ../*.c ../vendor/fast_obj/*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_obj
