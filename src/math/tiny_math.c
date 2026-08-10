#include "../tiny_math.h"

tinyMatrix matrixIdentity(void) {
	return (tinyMatrix){
		.inner = {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		}
	};
}

tinyMatrix matrixZero(void) {
	return (tinyMatrix){
		.inner = {
			{0.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 0.0f}
		}
	};
}

tinyMatrix matrixAdd(tinyMatrix a, tinyMatrix b) {
	tinyMatrix res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.inner[i][j] = a.inner[i][j] + b.inner[i][j];
		}
	}
	return res;
}

tinyMatrix matrixNeg(tinyMatrix a) {
	tinyMatrix res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.inner[i][j] = -a.inner[i][j];
		}
	}
	return res;
}

tinyMatrix matrixMul(tinyMatrix a, tinyMatrix b) {
	tinyMatrix res = matrixZero();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				res.inner[i][j] += a.inner[i][k] * b.inner[k][j];
			}
		}
	}
	return res;
}

tinyVec matrixVecMul(tinyMatrix a, tinyVec b) {
	tinyVec res;
	for (int i = 0; i < 4; i++) {
		res.inner[i] = 0.0f;
		for (int j = 0; j < 4; j++) {
			res.inner[i] += a.inner[i][j] * b.inner[j];
		}
	}
	return res;
}

tinyVec point2ToVec(Point2 p) {
	return (tinyVec){ .inner = { (float)p.x, (float)p.y, 0.0f, 1.0f } };
}

tinyVec point3ToVec(Point3 p) {
	return (tinyVec){ .inner = { (float)p.x, (float)p.y, (float)p.z, 1.0f } };
}

Point2 vecToPoint2(tinyVec v) {
	return (Point2){ .x = (int)roundf(v.inner[0]), .y = (int)roundf(v.inner[1]) };
}

Point3 vecToPoint3(tinyVec v) {
	return (Point3){ .x = (int)roundf(v.inner[0]), .y = (int)roundf(v.inner[1]), .z = (int)roundf(v.inner[2]) };
}

tinyMatrix getTranslationMatrix(float tx, float ty, float tz) {
	tinyMatrix m = matrixIdentity();
	m.inner[0][3] = tx;
	m.inner[1][3] = ty;
	m.inner[2][3] = tz;
	return m;
}

tinyMatrix getScaleMatrix(float sx, float sy, float sz) {
	tinyMatrix m = matrixIdentity();
	m.inner[0][0] = sx;
	m.inner[1][1] = sy;
	m.inner[2][2] = sz;
	return m;
}

tinyMatrix getRotationMatrix2D(float theta, axis ax) {
	tinyMatrix m = matrixIdentity();
	float c = cosf(theta);
	float s = sinf(theta);
	switch (ax) {
	case Z:
		m.inner[0][0] = c;
		m.inner[0][1] = -s;
		m.inner[1][0] = s;
		m.inner[1][1] = c;
		break;
	case X:
		m.inner[1][1] = c;
		break;
	case Y:
		m.inner[0][0] = c;
		break;
	default:
		break;
	}
	return m;
}

tinyMatrix getRotationMatrix3D(float theta, axis ax) {
	tinyMatrix m = matrixIdentity();
	float c = cosf(theta);
	float s = sinf(theta);
	switch (ax) {
	case Z:
		m.inner[0][0] = c;
		m.inner[0][1] = -s;
		m.inner[1][0] = s;
		m.inner[1][1] = c;
		break;
	case X:
		m.inner[1][1] = c;
		m.inner[1][2] = s;
		m.inner[2][1] = -s;
		m.inner[2][2] = c;
		break;
	case Y:
		m.inner[0][0] = c;
		m.inner[0][2] = -s;
		m.inner[2][0] = -s;
		m.inner[2][2] = c;
		break;
	default:
		break;
	}
	return m;
}

float vecDot3(tinyVec a, tinyVec b) {
	return a.inner[0] * b.inner[0] +
	       a.inner[1] * b.inner[1] +
	       a.inner[2] * b.inner[2];
}

tinyVec vecCross(tinyVec a, tinyVec b) {
	return (tinyVec){ .inner = {
		a.inner[1] * b.inner[2] - a.inner[2] * b.inner[1],
		a.inner[2] * b.inner[0] - a.inner[0] * b.inner[2],
		a.inner[0] * b.inner[1] - a.inner[1] * b.inner[0],
		0.0f
	}};
}

tinyVec vecSub(tinyVec a, tinyVec b) {
	return (tinyVec){ .inner = {
		a.inner[0] - b.inner[0],
		a.inner[1] - b.inner[1],
		a.inner[2] - b.inner[2],
		0.0f
	}};
}

tinyVec vecNormalize(tinyVec v) {
	float len = sqrtf(v.inner[0] * v.inner[0] +
	                  v.inner[1] * v.inner[1] +
	                  v.inner[2] * v.inner[2]);
	if (len < 0.0001f) return (tinyVec){ .inner = { 0, 0, 0, 0 } };
	return (tinyVec){ .inner = {
		v.inner[0] / len,
		v.inner[1] / len,
		v.inner[2] / len,
		0.0f
	}};
}
