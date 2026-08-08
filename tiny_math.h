#include <math.h>

/* Internal Vector implementation */
typedef struct {
	int inner[4];
}tinyVec;

/* Internal Matrix implementation */
typedef struct {
	int inner[4][4];
}tinyMatrix;


/*Basic matrix operations*/
tinyMatrix matrixAdd(tinyMatrix a,tinyMatrix b);
tinyMatrix matrixNeg(tinyMatrix a);
tinyMatrix matrixMul(tinyMatrix a,tinyMatrix b);

/*Basic matrix operations with vectors*/
tinyVec matrixVecMul(tinyMatrix b, tinyVec a);
