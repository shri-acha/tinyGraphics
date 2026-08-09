#pragma once
#include <stdint.h>
#include "tiny_math.h"

/// Point2 represents the world coordinates points, i.e. coordinates w.r.t. to the origin defined by the 
/// RenderingContext
struct Point2 {
  int x;
  int y;
};

/// Point3 represents the world coordinates points, i.e. coordinates w.r.t. to the origin defined by the 
/// RenderingContext
struct Point3 {
  int x;
  int y;
  int z;
};

/// Vector type for direction
typedef struct{
	tinyVec up;
	tinyVec right;
	tinyVec forward;
}DirectionVector;

/// Type of event handled.
typedef enum {
	MOUSE,
	KEYBOARD,
}EventType;

/// Defines the state of the event.
/// currently, it only supports keyboard and mouse,(tap and release) event, TODO: scroll and control signals?
typedef enum {
	DOWN,
	UP,
}KeyState;

/// Defines the what side of the mouse it is being clicked
typedef enum {
	LEFT,
	RIGHT,
}MouseButton;

/// Defines the Event that is handled by a Mouse 
typedef struct {
	KeyState state;
	MouseButton btn;
	Point2 pos;
}MouseEvent;

/// Defines the Event that is handled by a Keyboard 
typedef struct {
	int keycode;
	KeyState state;
}KeyboardEvent;

/// Generic event type
typedef struct {
	EventType ev_typ;
	union {
		MouseEvent me;
		KeyboardEvent ke;
	};
}Event;

/// Event handler wrapper
typedef struct {
	void (*f) (Event e);
}EventHandler;

/// Vector4 is an internal struct representation used for storing 3D
/// point with scale value 'w'.
typedef tinyVec Vector4;

/// Matrix3 is a 4x4 matrix as opposed to its name Matrix3, 
/// as 3 mentions the 3-dimensional requirement but 4x4 matrix
/// satisfies the 
typedef tinyMatrix Matrix3;

/// Index just the same type as a Point3
typedef struct {
	int x;
	int y;
	int z;
} Index;

// axis enum is defined in tiny_math.h


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

/* Is used by the engine to represent the objects within the scene */
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

/* Defines the projection type used, while in the (3d projection mode) */
typedef enum {
	ORTHOGRAPHIC,
	PERSPECTIVE,
}projectionMode;

/* Defines the coordinate system used when rendering */
typedef enum {
	TW, // 2D 
	TH, // 3D
}cameraMode;

/* Render context holds relevant data and contexts required internally or by the user, like sceneContext, that can be passed to 
 obtain relevant information about the scene */
typedef struct {
  frameBuffer *frame_buffer;
  renderMode render_mode;
  projectionMode projection;
  float focal_length;
  Index origin;
  sceneContext* scene_context;
  DirectionVector camera_direction;
  Point3 camera_position;
  EventHandler event_handler;
} renderContext;
