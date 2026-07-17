# tinyGraphics  
  
A tiny, dependency-light software rendering library written in C. It rasterizes  
basic 2D primitives (points, lines, circles, triangles) into an in-memory frame  
buffer and can render the result directly to the terminal using ANSI truecolor  
escape codes. The algorithms are chosen to minimize operations, making the  
library suitable for low-performance systems.  
  
## Features  
  
- **Frame buffer rendering** into a flat pixel buffer with a 16-bit `5R6G5B`  
  color layout.  
- **Primitives**:  
  - Points  
  - Lines (Bresenham) and fast horizontal lines  
  - Circles (Jesko's method to reduce per-pixel operations)  
  - Triangles (scanline/DDA fill)  
- **Render modes**: `WIREFRAME` and `FILLED`.  
- **Rotation**: render circles and triangles rotated by an angle `theta`  
  about the `X`, `Y`, or `Z` axis.  
- **Configurable origin** within the render context.  
- **Terminal output** via `formatBuffer`, which prints pixels using ANSI  
  truecolor escape sequences.  
  
## Core types  
  
Defined in `types.h`:  
  
- `Point2` ; world-coordinate point `{ int x; int y; }` relative to the  
  context origin.  
- `Color` ; a 16-bit `5R6G5B` color (`uint16_t literal`).  
- `pixelBuffer` ; a packed struct holding a single `Color`.  
- `frameBuffer` ; `{ int width; int height; pixelBuffer* buffer; }`.  
- `renderMode` ; `WIREFRAME` or `FILLED`.  
- `axis` ; `X`, `Y`, `Z` for rotation.  
- `renderContext` ; holds the `frame_buffer`, `render_mode`, `origin`, and a  
  `scene_context`. It is passed to every rendering call.  
  
## Public API  
  
Declared in `graphics.h`:  
  
```c  
// Frame buffer lifecycle  
frameBuffer *createFrameBuffer(int width, int height, uint16_t color);  
int          destroyFrameBuffer(frameBuffer *fb);  
int          flushPixelBuffer(pixelBuffer *pb, int width, int height);  
  
// Pixel access  
int        get_index(frameBuffer *fb, int x, int y);  
void       set_pixel(frameBuffer *fb, pixelBuffer pb, int x, int y);  
pixelBuffer get_pixel(frameBuffer *fb, int x, int y);  
  
// Context  
void set_origin(renderContext *rc, Point2 origin);  
  
// Primitives  
int  renderPoint(renderContext *rc, Point2 p);  
void renderLine(renderContext *rc, Point2 p1, Point2 p2);  
void renderHorizontalLine(renderContext *rc, int x1, int x2, int y);  
int  renderCircle(renderContext *rc, Point2 p, int r);  
int  renderAngledCircle(renderContext *rc, Point2 p, int r, float theta, axis ax);  
int  renderTriangle(renderContext *rc, Point2 *points[3]);  
int  renderAngledTriangle(renderContext *rc, Point2 *points[3], float theta, axis ax);  
  
// Rotation helper  
Point2 rotatePoint(renderContext *rc, int x, int y, float theta, axis ax);  
  
// Output  
void formatBuffer(frameBuffer *fb);  
```  
  
## Building  
  
The project is plain C with a dependency on the math library. Compile the  
source files together with your program, for example:  
  
```sh  
cc main.c graphics.c point.c line.c circle.c triangle.c utils.c scene_context.c -lm -o demo  
```  
  
## Usage example  
  
```c  
#include "graphics.h"  
  
int main(void) {  
    frameBuffer *fb = createFrameBuffer(64, 32, 0xFFFF);  
  
    renderContext rc = {  
        .frame_buffer = fb,  
        .render_mode  = WIREFRAME,  
        .origin       = { .x = 0, .y = 0 },  
    };  
  
    // Draw a circle and a line  
    renderCircle(&rc, (Point2){ .x = 20, .y = 16 }, 10);  
    renderLine(&rc, (Point2){ .x = 0, .y = 0 }, (Point2){ .x = 60, .y = 30 });  
  
    // Print to terminal  
    formatBuffer(fb);  
  
    destroyFrameBuffer(fb);  
    return 0;  
}  
```  
  
## Project structure  
  
| File | Purpose |  
|------|---------|  
| `types.h` | Core data structures (`Point2`, `Color`, `frameBuffer`, `renderContext`, etc.) |  
| `graphics.h` / `graphics.c` | Public API, frame buffer management, terminal output, rotation |  
| `point.c` | `renderPoint` |  
| `line.c` | `renderLine` (Bresenham) and `renderHorizontalLine` |  
| `circle.c` | `renderCircle` / `renderAngledCircle` (Jesko's method) |  
| `triangle.c` | `renderTriangle` / `renderAngledTriangle` (scanline fill) |  
| `utils.h` / `utils.c` | Pixel helpers, index math, point sorting |  
| `_graphics.h` / `scene_context.c` | Scene/object context management (work in progress) |  
  
## Notes  
  
- `formatBuffer` decodes the `5R6G5B` color and emits ANSI truecolor escape  
  codes, then clears the screen ; it is intended for terminal-based previews.  
- The scene context API (`scene_context.c`, `_graphics.h`) is incomplete and  
  under development.

## Example

### Rotating Prism
<img width="400" height="225" alt="2026-07-18 05-22-58" src="https://github.com/user-attachments/assets/95aab68d-129f-4bb7-b20d-7a9e67c3f366" />

### Rotating Triangle
<img width="400" height="225" alt="2026-07-18 05-33-54" src="https://github.com/user-attachments/assets/5ff6ea67-d1d1-4604-a21c-22bf792383ef" />

