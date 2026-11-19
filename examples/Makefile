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


# ---------------------------------------------------------------------------
# WebAssembly (requires emsdk: `source /path/to/emsdk/emsdk_env.sh`)
# ---------------------------------------------------------------------------
EMCC ?= emcc
WASM_FLAGS = -O3 -DTINY_NO_FAST_OBJ -sALLOW_MEMORY_GROWTH=1 -sEXPORTED_RUNTIME_METHODS=HEAPU8 -lm

wasm_fishes: wasm_fishes.c web/index.html $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)/web
	$(EMCC) wasm_fishes.c $(SRC_FILES) $(WASM_FLAGS) -o $(BUILD_DIR)/web/fishes.js
	cp web/index.html $(BUILD_DIR)/web/index.html
	@echo "Serve with: python3 -m http.server -d $(BUILD_DIR)/web 8000"

# Same demo, native window (GLFW)
native_fishes: wasm_fishes.c $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	gcc -O3 wasm_fishes.c $(SRC_FILES) -DTINY_NO_FAST_OBJ $(LIBS) -o $(BUILD_DIR)/fishes

.PHONY: wasm_fishes native_fishes
