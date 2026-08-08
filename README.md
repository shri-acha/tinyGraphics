# tinyGraphics

A tiny, dependency-light software rendering library written in C. It rasterizes
2D and 3D primitives (points, lines, circles, triangles) into an in-memory frame
buffer and can render the result directly to the terminal using ANSI truecolor
escape codes. The algorithms are chosen to minimize operations, making the
library suitable for low-performance systems.

## Features

- **Frame buffer rendering** into a flat pixel buffer with a 16-bit `5R6G5B`
  color layout.
- **2D primitives**:
  - Points
  - Lines (Bresenham) and fast horizontal lines
  - Circles (Jesko's method to reduce per-pixel operations)
  - Triangles (scanline/DDA fill)
- **3D primitives**: lines and triangles, projected onto the frame buffer.
- **Projection modes**: `ORTHOGRAPHIC` and `PERSPECTIVE` (the latter uses the
  context's `focal_length`).
- **Render modes**: `WIREFRAME` and `FILLED`.
- **Rotation**: render circles and triangles rotated by an angle `theta`
  about the `X`, `Y`, or `Z` axis.
- **Configurable origin** within the render context.
- **Scene context** (work in progress) that keeps track of the objects
  registered in a scene.
- **Terminal output** via `formatBuffer`, which prints pixels using ANSI
  truecolor escape sequences.

## Core types

Defined in `types.h`:

- `Point2` — world-coordinate point `{ int x; int y; }` relative to the
  context origin.
- `Point3` — world-coordinate point `{ int x; int y; int z; }`.
- `Index` — `{ int x; int y; int z; }`, used for the context origin.
- `axis` — `X` (default), `Y`, `Z` for rotation.
- `Color` — a 16-bit `5R6G5B` color (`uint16_t literal`).
- `pixelBuffer` — a packed struct holding a single `Color`.
- `frameBuffer` — `{ int width; int height; pixelBuffer* buffer; }`.
- `renderMode` — `WIREFRAME` or `FILLED`.
- `projectionMode` — `ORTHOGRAPHIC` or `PERSPECTIVE`.
- `cameraMode` — `TW` (2D) or `TH` (3D).
- Scene types:
  - `objectType` — `LINE`, `CIRCLE`, `TRIANGLE`.
  - `Line` — `{ Point2 start; Point2 end; }`.
  - `Circle` — `{ Point2 center; int radius; int theta; axis ax; }`.
  - `Triangle` — `{ Point2* points[3]; int theta; axis ax; }`.
  - `renderedObject` — tagged union of `Line` / `Circle` / `Triangle` plus its
    `objectType`.
  - `sceneContext` — `{ renderedObject* objs; int no_of_objs; }`.
- `renderContext` — holds the `frame_buffer`, `render_mode`, `projection`,
  `focal_length`, `Index origin`, `sceneContext* scene_context`, and
  `Point3 camera_position`. It is passed to every rendering call.

## Public API

Declared in `graphics.h`:

```c
// Frame buffer lifecycle
frameBuffer *createFrameBuffer(int width, int height);
int          destroyFrameBuffer(frameBuffer *fb);
int          destroyContext(renderContext* rc);
int          flushPixelBuffer(pixelBuffer *pb, int width, int height);
int          flushSceneContext(sceneContext* sc);

// Scene context
sceneContext* newSceneContext();

// Pixel access
int         get_index(frameBuffer *fb, int x, int y);
pixelBuffer get_pixel(frameBuffer *fb, int x, int y);

// Context
void set_origin(renderContext* rc, Point2 origin);

// 2D primitives
void renderPoint2D(renderContext *rc, Point2 p, Color color);
void renderLine2D(renderContext *rc, Point2 p1, Point2 p2, Color color);
void renderHorizontalLine2D(renderContext *rc, int x1, int x2, int y, Color color);
void renderCircle2D(renderContext *rc, Point2 p, int r, Color color);
void renderAngledCircle2D(renderContext *rc, Point2 p, int r, float theta,
                          axis ax, Color color);
void renderTriangle2D(renderContext *rc, Point2* points[3], Color color);
void renderAngledTriangle2D(renderContext *rc, Point2 *points[3], float theta,
                            axis ax, Color color);

// 3D primitives
void renderLine3D(renderContext *rc, Point3 p1, Point3 p2, Color color);
void renderTriangle3D(renderContext *rc, Point3* points[3], Color color);
void renderAngledTriangle3D(renderContext *rc, Point3 *points[3], float theta,
                            axis ax, Color color);

// Rotation helper
Point2 rotatePoint2D(renderContext* rc, int x, int y, float theta, axis ax);

// Output
void formatBuffer(frameBuffer *fb);
```

## Usage example

```c
#include "graphics.h"

int main(void) {
    frameBuffer *fb = createFrameBuffer(64, 32);
    renderContext rc = {
        .frame_buffer = fb,
        .render_mode  = WIREFRAME,
        .projection   = ORTHOGRAPHIC,
        .origin       = { .x = 0, .y = 0, .z = 0 },
        .scene_context = NULL,
    };
    Color white = { .literal = 0xFFFF };
    renderCircle2D(&rc, (Point2){ .x = 20, .y = 16 }, 10, white);
    renderLine2D(&rc, (Point2){0,0}, (Point2){60,30}, white);
    formatBuffer(fb);
    destroyFrameBuffer(fb);
    return 0;
}
```

`createFrameBuffer` takes only the width and height (the buffer is zeroed), and
every primitive takes a trailing `Color`.

## Building

The project is plain C with a dependency on the math library. Compile the
source files together with your program, for example:

```sh
cc main.c graphics.c scene_context.c utils.c _graphics.c \
   2D/point.c 2D/line.c 2D/circle.c 2D/triangle.c \
   3D/point.c 3D/line.c 3D/triangle.c -lm -o demo
```

`utils.h` includes `<unitypes.h>`, so the libunistring headers must be
available (`apt install libunistring-dev` on Debian/Ubuntu).

## Project structure

| File | Purpose |
|------|---------|
| `types.h` | Core data structures (`Point2`, `Point3`, `Color`, `frameBuffer`, `renderContext`, scene types, etc.) |
| `graphics.h` / `graphics.c` | Public API, frame buffer and context lifecycle, terminal output |
| `_graphics.h` / `_graphics.c` | Internal rasterization, projection and rotation routines |
| `2D/` | 2D primitives: `point.c`, `line.c`, `circle.c`, `triangle.c` |
| `3D/` | 3D primitives: `point.c`, `line.c`, `triangle.c` (`circle.c` is an empty placeholder) |
| `utils.h` / `utils.c` | Pixel helpers, index math, point sorting |
| `scene_context.c` | Scene/object context management (work in progress) |
| `examples/` | Terminal and OpenGL demos |

## Notes

- `formatBuffer` decodes the `5R6G5B` color and emits ANSI truecolor escape
  codes, then clears the screen — it is intended for terminal-based previews.
- The scene context API (`scene_context.c`, `_graphics.h`) is incomplete and
  under development.

## Example

### Rotating Prism
<img width="400" height="225" alt="2026-07-18 05-22-58" src="https://github.com/user-attachments/assets/95aab68d-129f-4bb7-b20d-7a9e67c3f366" />

### Rotating Triangle
<img width="400" height="225" alt="2026-07-18 05-33-54" src="https://github.com/user-attachments/assets/5ff6ea67-d1d1-4604-a21c-22bf792383ef" />
