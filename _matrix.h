#pragma once
#include <math.h>
#include "types.h"
#include "tiny_math.h"

static inline Matrix3 addMatrix3(Matrix3 a, Matrix3 b) {
	return matrixAdd(a, b);
}

static inline Matrix3 multiplyMatrix3(Matrix3 a, Matrix3 b) {
	return matrixMul(a, b);
}

static inline void negateMatrix3(Matrix3* a) {
	*a = matrixNeg(*a);
}

static inline Vector4 multiplyMatrixWithVector(Matrix3 a, Vector4 p) {
	return matrixVecMul(a, p);
}

