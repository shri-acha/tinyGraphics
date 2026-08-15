CFLAGS = -I src/vendor/fast_obj/
LIBS = -lm -lglfw -lGL
BUILD_DIR = build/

SRC_FILES = ../src/2D/*.c ../src/3D/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../src/window/*.c ../src/*.c ../*.c

opengl_prism: opengl_prism.c $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	gcc opengl_prism.c $(SRC_FILES) $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_prism 

opengl_triangle: opengl_triangle.c $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	gcc opengl_triangle.c $(SRC_FILES) $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_triangle 

opengl_circle: opengl_circle.c $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	gcc opengl_circle.c $(SRC_FILES) $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_circle 

opengl_cube: opengl_cube.c $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	gcc opengl_cube.c $(SRC_FILES) $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_cube

opengl_obj: opengl_obj.c $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	gcc -O3 opengl_obj.c $(SRC_FILES) ../src/vendor/fast_obj/*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_obj

