#pragma once
#include <math.h>

typedef enum {
	X, // DEFAULT AXIS OF ROTATION
	Y,
	Z,
} axis;

typedef struct Point2 Point2;
typedef struct Point3 Point3;

/* Internal Vector implementation (4-element vector in homogeneous coordinates) */
typedef struct {
	float inner[4];
} tinyVec;

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
