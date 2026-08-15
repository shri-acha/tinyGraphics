CFLAGS = -I src/vendor/fast_obj/
LIBS = -lm -lglfw -lGL
BUILD_DIR = build/

opengl_prism: opengl_prism.c ../src/3D/*.c ../src/2D/*.c ../src/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../*.c 
	@mkdir -p $(BUILD_DIR)
	gcc opengl_prism.c ../src/2D/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../src/3D/*.c ../src/*.c ../*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_prism 

opengl_triangle: opengl_triangle.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../src/3D/*.c ../src/2D/*.c ../src/*.c ../*.c 
	@mkdir -p $(BUILD_DIR)
	gcc opengl_triangle.c ../src/2D/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../src/3D/*.c ../src/*.c ../*.c  $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_triangle 

opengl_circle: opengl_circle.c ../src/3D/*.c ../src/2D/*.c ../src/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../*.c 
	@mkdir -p $(BUILD_DIR)
	gcc opengl_circle.c ../src/2D/*.c ../src/3D/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../src/*.c  ../*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_circle 

opengl_cube: opengl_cube.c ../src/3D/*.c ../src/2D/*.c ../src/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../*.c 
	@mkdir -p $(BUILD_DIR)
	gcc opengl_cube.c ../src/2D/*.c ../src/3D/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../src/*.c  ../*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_cube


opengl_obj: opengl_obj.c ../src/3D/*.c ../src/2D/*.c ../src/*.c ../*.c 
	@mkdir -p $(BUILD_DIR)
	gcc opengl_obj.c ../src/2D/*.c ../src/3D/*.c ../src/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../src/vendor/fast_obj/*.c ../*.c  $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_obj

opengl_events: opengl_events.c ../src/3D/*.c ../src/2D/*.c ../src/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../*.c
	@mkdir -p $(BUILD_DIR)
	gcc opengl_events.c ../src/2D/*.c ../src/3D/*.c ../src/*.c ../src/events/*.c ../src/math/*.c ../src/utils/*.c ../*.c $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/ogl_events

