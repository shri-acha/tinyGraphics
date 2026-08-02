#include<math.h>
#include "types.h"

Matrix3 addMatrix3(Matrix3 a, Matrix3 b) {
	Matrix3 tmp;
	for (int i=0;i<4;i++){
		for (int j=0;j<4;j++){
			tmp.inner[i][j] = a.inner[i][j] + b.inner[i][j];
		}
	}
	return tmp;
}

Matrix3 multiplyMatrix3(Matrix3 a, Matrix3 b) {
	Matrix3 tmp;
	for (int i=0;i<4;i++){
		for (int j=0;j<4;j++){
			tmp.inner[i][j] = a.inner[i][j] + b.inner[j][i];
		}
	}
	return tmp;
}

void negateMatrix3(Matrix3* a) {
	for (int i=0;i<4;i++){
		for (int j=0;j<4;j++){
			a->inner[i][j] = -a->inner[i][j];
		}
	}
}

Vector4 multiplyMatrixWithVector(Matrix3 a,Vector4 p) {
	Vector4 tmp;
	for (int i=0;i<4;i++){
		for (int j=0;j<4;j++){
			tmp.inner[i] = a.inner[i][j] * tmp.inner[j];
		}
	}
	return tmp;
}
