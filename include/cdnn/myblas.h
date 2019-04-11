
//#define USE_CBLAS
#ifdef USE_CBLAS

#ifdef USE_MKL
#include <mkl.h>
#else  // If use MKL, simply include the MKL header
extern "C" {
#include <cblas.h>
}
#include <math.h>
#endif  // USE_MKL

#define CBLASFUN(NAME)  cblas_s##NAME
#else
//#define CBLASFUN(NAME)  cblas_s##NAME
#define ASSERT assert
#define CBLASFUN(NAME)  my_s##NAME


typedef enum CBLAS_ORDER { CblasRowMajor = 101, CblasColMajor = 102 } CBLAS_ORDER;
typedef enum CBLAS_TRANSPOSE { CblasNoTrans = 111, CblasTrans = 112, CblasConjTrans = 113, CblasConjNoTrans = 114 } CBLAS_TRANSPOSE;
typedef enum CBLAS_UPLO { CblasUpper = 121, CblasLower = 122 } CBLAS_UPLO;
typedef enum CBLAS_DIAG { CblasNonUnit = 131, CblasUnit = 132 } CBLAS_DIAG;
typedef enum CBLAS_SIDE { CblasLeft = 141, CblasRight = 142 } CBLAS_SIDE;
#endif // USE_CBLAS

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <assert.h>
//#include "../types.h"

#define fequal(a, b)   ((a)==(b))

#define Dtype float
#define Stype double
#define ISDOUBLE 0
#define FUN(NAME) my_s##NAME
//#undef CBLASFUN 
//typedef long BLASLONG;
#include "level1.h"
//#include "gemv.h"
#include "gemm.h"
//#include "conv.h"


#undef Dtype
#undef Stype
#undef FUN
#undef ISDOUBLE

#undef fequal

#if 0
// 44.857143
#define cblas1_sgemm my_sgemm
#define cblas1_sgemv my_sgemv
#define cblas1_saxpy my_saxpy
#define cblas1_sscal my_sscal
#define cblas1_sdot  my_sdot
#define cblas1_sasum my_sasum
#define cblas1_scopy cblas_scopy
#endif