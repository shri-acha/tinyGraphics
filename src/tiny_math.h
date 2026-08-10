#pragma once
#include <math.h>

typedef enum {
	X, // DEFAULT AXIS OF ROTATION
	Y,
	Z,
} axis;


/// Point2 represents the world coordinates points, i.e. coordinates w.r.t. to the origin defined by the 
/// RenderingContext
typedef struct {
  int x;
  int y;
}Point2;

/// Point3 represents the world coordinates points, i.e. coordinates w.r.t. to the origin defined by the 
/// RenderingContext
typedef struct {
  int x;
  int y;
  int z;
}Point3;

/* Internal Vector implementation (4-element vector in homogeneous coordinates) */
typedef struct {
	float inner[4];
} tinyVec;

/// Vector type for direction
typedef struct{
	tinyVec up;
	tinyVec right;
	tinyVec forward;
}DirectionVector;

/* Internal Matrix implementation (4x4 transformation matrix) */
typedef struct {
	float inner[4][4];
} tinyMatrix;

/* Basic matrix operations */
tinyMatrix matrixIdentity(void);
tinyMatrix matrixZero(void);
tinyMatrix matrixAdd(tinyMatrix a, tinyMatrix b);
tinyMatrix matrixNeg(tinyMatrix a);
tinyMatrix matrixMul(tinyMatrix a, tinyMatrix b);

/* Vector operations */
tinyVec matrixVecMul(tinyMatrix a, tinyVec b);

/* Conversion helper functions */
tinyVec point2ToVec(Point2 p);
tinyVec point3ToVec(Point3 p);
Point2 vecToPoint2(tinyVec v);
Point3 vecToPoint3(tinyVec v);

/* Transformation matrix generators */
tinyMatrix getTranslationMatrix(float tx, float ty, float tz);
tinyMatrix getScaleMatrix(float sx, float sy, float sz);
tinyMatrix getRotationMatrix2D(float theta, axis ax);
tinyMatrix getRotationMatrix3D(float theta, axis ax);

/* 3-component vector operations (ignores w) */
float vecDot3(tinyVec a, tinyVec b);
tinyVec vecCross(tinyVec a, tinyVec b);
tinyVec vecSub(tinyVec a, tinyVec b);
tinyVec vecNormalize(tinyVec v);
