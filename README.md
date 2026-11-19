# tinyGraphics

A lightweight, dependency-light 2D/3D software rendering engine written in C.

## Overview

**tinyGraphics** is designed as a portable, pure CPU software rasterization library. Its primary goal is to bring real-time 2D and 3D rendering capabilities to low-performance CPUs, embedded systems, and resource-constrained microcontrollers without relying on hardware GPU acceleration.

### Key Architectural Highlights

- **Pure Software Pipeline**: Custom implementation of 2D/3D math, perspective and orthographic projections, $Z$-depth buffering, wireframe/filled rasterization, and Gouraud shading.
- **Performance-Conscious Rasterization**: Utilizes lightweight scanline DDA algorithms, Jesko's fast circle rendering, and aggressive viewport bounds culling to eliminate wasted CPU operations on off-screen geometry.
- **Pluggable Windowing API**: A generic backend abstraction layer (`tinyWindow`) that decouples the graphics engine from platform window managers. Includes out-of-the-box support for GLFW, ASCII terminal output, and custom backends (SDL, SFML, GLUT, or raw micro-display framebuffers).
- **Minimal Footprint**: Compact 16-bit RGB (5-6-5) pixel buffers and cache-friendly event queues optimized for constrained execution environments.

## Code Example

```c
#include "graphics.h"

int main(void) {
    frameBuffer* fb = createFrameBuffer(800, 600);
    renderContext rc = {
        .frame_buffer = fb,
        .render_mode = FILLED,
        .origin = (Index){ .x = 400, .y = 300, .z = 0 },
        .projection = PERSPECTIVE,
        .focal_length = 150.0f,
    };

    tinyWindow* window = tinyCreateWindow(tinyGetGLFWBackend(), 800, 600, "tinyGraphics Example", &rc, NULL);

    while (!tinyWindowShouldClose(window)) {
        Color color = { .literal = (0x1F << 11) | (0x1F << 5) | 0x1F };
        Point3 p1 = { -100, -100, 200 }, p2 = { 100, -100, 200 }, p3 = { 0, 100, 200 };
        Point3* tri[3] = { &p1, &p2, &p3 };

        renderTriangle3D(&rc, tri, color);
        tinyWindowPresent(window);
    }

    tinyDestroyWindow(window);
    destroyFrameBuffer(fb);
    return 0;
}
```
## WebAssembly

tinyGraphics runs in the browser through Emscripten. All rasterization still happens on the CPU inside the WASM module. The `tinyGetWasmBackend()` window backend only converts the RGB565 framebuffer to RGBA and blits it onto `<canvas id="canvas">`. Keyboard and mouse events are forwarded into the regular event queue.

Browsers can't run a blocking `while` loop, so use the portable `tinyWindowRunLoop()`. Natively it is a plain loop. On the web it maps to `requestAnimationFrame`:

```c
static void frame(tinyWindow* win, void* arg) {
    renderContext* rc = arg;
    /* draw with renderTriangle2D / renderCircle2D / ... */
}

int main(void) {
    /* ... create frameBuffer + renderContext rc ... */
    tinyWindow* win = tinyCreateWindow(NULL, 480, 270, "demo", &rc, NULL); /* NULL -> WASM backend on the web */
    tinyWindowRunLoop(win, frame, &rc);
}
```

Try the low-poly aquarium demo (`examples/wasm_fishes.c`). It is live at **https://shri-acha.github.io/tinyGraphics/**, and you can build it locally with:

```sh
source /path/to/emsdk/emsdk_env.sh      # or: nix-shell -p emscripten
cd examples
make wasm_fishes                        # -> build/web/{index.html,fishes.js,fishes.wasm}
python3 -m http.server -d build/web 8000
# open http://localhost:8000
```

`make native_fishes` builds the same source against GLFW. Pass `-DTINY_NO_FAST_OBJ` to build without the optional `fast_obj` OBJ loader (and `renderMesh3D`).

## Use of AI and LLMs

The initial draft and development was purely out of textbook knowledge. Now that I've decided to add more features and make it more approachable, I've decided to use
LLMs in generating docs, refactoring, code review and suggestions.  

## Preview

### Rotating Prism
<img width="400" height="225" alt="Rotating Prism" src="https://github.com/user-attachments/assets/95aab68d-129f-4bb7-b20d-7a9e67c3f366" />

### Rotating Triangle
<img width="400" height="225" alt="Rotating Triangle" src="https://github.com/user-attachments/assets/5ff6ea67-d1d1-4604-a21c-22bf792383ef" />

### Detachable Camera
<img width="1280" height="720" alt="Detachable Camera" src="https://github.com/user-attachments/assets/8ef4a85d-1971-48e5-be71-2b4c47122944" />
