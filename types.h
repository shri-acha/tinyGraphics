#pragma once
#include <stdint.h>

/// Point2 represents the world coordinates points, i.e. coordinates w.r.t. to the origin defined by the 
/// RenderingContext
typedef struct {
  int x;
  int y;
} Point2;

typedef struct {
	int x;
	int y;
} Index;

typedef enum {
	X, //DEFAULT AXIS OF ROTATION
	Y,
	Z,
}axis;

// 00000(R) 000000(G) 00000(B)
typedef struct {
  uint16_t literal;
} Color; 

typedef struct __attribute__((__packed__)) {
  Color color;
} pixelBuffer;

typedef struct {
  int width;
  int height;
  pixelBuffer *buffer;
} frameBuffer;

typedef enum {
  WIREFRAME,
  FILLED,
} renderMode;

typedef enum {
	LINE,
	CIRCLE,
	TRIANGLE,
}objectType;

typedef struct {
	Point2 start;
	Point2 end;
}Line;

typedef struct {
	Point2 center;
	int radius;
	int theta;
	axis ax;
}Circle;

typedef struct {
	Point2* points[3];
	int theta;
	axis ax;
}Triangle;

typedef struct {
	union {
	Line line;
	Circle circle; 
	Triangle triangle;
	}inner;
	objectType type;
}renderedObject;

typedef struct sceneContext{
	renderedObject* objs; // array of all objects being rendered
	int no_of_objs; // number of objects registered and currently in scene
}sceneContext;

/// Render context holds relevant data and contexts required internally or by the user, like sceneContext, that can be passed to 
/// obtain relevant information about the scene
typedef struct {
  frameBuffer *frame_buffer;
  renderMode render_mode;
  Index origin;
  sceneContext* scene_context;
} renderContext;
