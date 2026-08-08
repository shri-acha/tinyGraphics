#include "tiny_math.h"


/*Basic matrix operations*/
tinyMatrix matrixAdd(tinyMatrix a,tinyMatrix b){
	return (tinyMatrix) {
		.inner = {
			{a.inner[0][0]+b.inner[0][0],a.inner[0][1]+b.inner[0][1],a.inner[0][2]+b.inner[0][2],a.inner[0][3]+b.inner[0][3]},
			{a.inner[1][0]+b.inner[1][0],a.inner[1][1]+b.inner[1][1],a.inner[1][2]+b.inner[1][2],a.inner[1][3]+b.inner[1][3]},
			{a.inner[2][0]+b.inner[2][0],a.inner[2][1]+b.inner[2][1],a.inner[2][2]+b.inner[2][2],a.inner[2][3]+b.inner[2][3]},
			{a.inner[3][0]+b.inner[3][0],a.inner[3][1]+b.inner[3][1],a.inner[3][2]+b.inner[3][2],a.inner[3][3]+b.inner[3][3]},
		}
	};
}

tinyMatrix matrixNeg(tinyMatrix a){
	tinyMatrix b;
	for (int i=0;i<4;i++) {
		for (int j=0;j<4;j++) {
			b.inner[i][j] = -a.inner[i][j];
		}
	}
	return b;
}

tinyMatrix matrixMul(tinyMatrix a,tinyMatrix b){
	return (tinyMatrix) {
		 .inner = {
			  {
					a.inner[0][0]*b.inner[0][0] + a.inner[0][1]*b.inner[1][0] + a.inner[0][2]*b.inner[2][0] + a.inner[0][3]*b.inner[3][0],
					a.inner[0][0]*b.inner[0][1] + a.inner[0][1]*b.inner[1][1] + a.inner[0][2]*b.inner[2][1] + a.inner[0][3]*b.inner[3][1],
					a.inner[0][0]*b.inner[0][2] + a.inner[0][1]*b.inner[1][2] + a.inner[0][2]*b.inner[2][2] + a.inner[0][3]*b.inner[3][2],
					a.inner[0][0]*b.inner[0][3] + a.inner[0][1]*b.inner[1][3] + a.inner[0][2]*b.inner[2][3] + a.inner[0][3]*b.inner[3][3]
			  },
			  {
					a.inner[1][0]*b.inner[0][0] + a.inner[1][1]*b.inner[1][0] + a.inner[1][2]*b.inner[2][0] + a.inner[1][3]*b.inner[3][0],
					a.inner[1][0]*b.inner[0][1] + a.inner[1][1]*b.inner[1][1] + a.inner[1][2]*b.inner[2][1] + a.inner[1][3]*b.inner[3][1],
					a.inner[1][0]*b.inner[0][2] + a.inner[1][1]*b.inner[1][2] + a.inner[1][2]*b.inner[2][2] + a.inner[1][3]*b.inner[3][2],
					a.inner[1][0]*b.inner[0][3] + a.inner[1][1]*b.inner[1][3] + a.inner[1][2]*b.inner[2][3] + a.inner[1][3]*b.inner[3][3]
			  },
			  {
					a.inner[2][0]*b.inner[0][0] + a.inner[2][1]*b.inner[1][0] + a.inner[2][2]*b.inner[2][0] + a.inner[2][3]*b.inner[3][0],
					a.inner[2][0]*b.inner[0][1] + a.inner[2][1]*b.inner[1][1] + a.inner[2][2]*b.inner[2][1] + a.inner[2][3]*b.inner[3][1],
					a.inner[2][0]*b.inner[0][2] + a.inner[2][1]*b.inner[1][2] + a.inner[2][2]*b.inner[2][2] + a.inner[2][3]*b.inner[3][2],
					a.inner[2][0]*b.inner[0][3] + a.inner[2][1]*b.inner[1][3] + a.inner[2][2]*b.inner[2][3] + a.inner[2][3]*b.inner[3][3]
			  },
			  {
					a.inner[3][0]*b.inner[0][0] + a.inner[3][1]*b.inner[1][0] + a.inner[3][2]*b.inner[2][0] + a.inner[3][3]*b.inner[3][0],
					a.inner[3][0]*b.inner[0][1] + a.inner[3][1]*b.inner[1][1] + a.inner[3][2]*b.inner[2][1] + a.inner[3][3]*b.inner[3][1],
					a.inner[3][0]*b.inner[0][2] + a.inner[3][1]*b.inner[1][2] + a.inner[3][2]*b.inner[2][2] + a.inner[3][3]*b.inner[3][2],
					a.inner[3][0]*b.inner[0][3] + a.inner[3][1]*b.inner[1][3] + a.inner[3][2]*b.inner[2][3] + a.inner[3][3]*b.inner[3][3]
			  }
		 }
	};

}

/*Basic matrix operations with vectors 4x1 and 4x4*/
tinyVec matrixVecMul(tinyMatrix a, tinyVec b) {
    return (tinyVec) {
        .inner = {
            a.inner[0][0] * b.inner[0] +
            a.inner[0][1] * b.inner[1] +
            a.inner[0][2] * b.inner[2] +
            a.inner[0][3] * b.inner[3],

            a.inner[1][0] * b.inner[0] +
            a.inner[1][1] * b.inner[1] +
            a.inner[1][2] * b.inner[2] +
            a.inner[1][3] * b.inner[3],

            a.inner[2][0] * b.inner[0] +
            a.inner[2][1] * b.inner[1] +
            a.inner[2][2] * b.inner[2] +
            a.inner[2][3] * b.inner[3],

            a.inner[3][0] * b.inner[0] +
            a.inner[3][1] * b.inner[1] +
            a.inner[3][2] * b.inner[2] +
            a.inner[3][3] * b.inner[3]
        }
    };
}
