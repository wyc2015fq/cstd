
#ifndef _ML_H_
#define _ML_H_

#include "xtypes.h"

#define cvAlloc malloc

#undef cvFree
#define cvFree(ptr) free(*(ptr))


typedef struct CvSparseVecElem32f {
  int idx;
  real val;
}
CvSparseVecElem32f;

/* log(2*PI) */
#define CV_LOG2PI (1.8378770664093454835606594728112)

/* columns of <trainData> matrix are training samples */
#define CV_COL_SAMPLE 0

/* rows of <trainData> matrix are training samples */
#define CV_ROW_SAMPLE 1

#define CV_IS_ROW_SAMPLE(flags) ((flags) & CV_ROW_SAMPLE)

typedef struct CvVectors {
  int dims, count;
  real* data;
  struct CvVectors* next;
}
CvVectors;

typedef struct tagNEAREST {
  real dist;
  real* data;
}
NEAREST, *PNEAREST;



typedef struct tagVECTOR {
  int size;
  int used;
  int dims;
  unsigned char* data;
}
VECTOR, *PVECTOR;

typedef VECTOR KNEAREST;
typedef PVECTOR PKNEAREST;

PVECTOR VECTOR_NEW(int size, int dims);
void VECTOR_DEL(PVECTOR vec);
void* VECTOR_PUSH_BACK(PVECTOR vec, int num, void* ptr, int len, int step);
int find_nearest(PVECTOR vec, void* sample, PNEAREST out, int len,
    real(*distor)(const void*, const void*, const void*), const void* userdata);

real chi_square(const void* a, const void* b, const void* userdata);

#endif // _ML_H_
