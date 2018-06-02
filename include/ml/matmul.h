
#ifndef _MATMUL_H_
#define _MATMUL_H_

#include "xtypes.h"

#define CV_GEMM_A_T 1
#define CV_GEMM_B_T 2
#define CV_GEMM_C_T 4

/* maximal size of vector to run matrix operations on it inline (i.e. w/o ipp calls) */
#define  CV_MAX_INLINE_MAT_OP_SIZE  10

/* maximal linear size of matrix to allocate it on stack. */
#define  CV_MAX_LOCAL_MAT_SIZE  32

/* maximal size of local memory storage */
#define  CV_MAX_LOCAL_SIZE  \
  (CV_MAX_LOCAL_MAT_SIZE*CV_MAX_LOCAL_MAT_SIZE*(int)sizeof(double))

void
cvGEMM(int a_height, int a_width, int d_width, const real* A, int A_step, const real* B, int B_step,
    real alpha, const real* C, int C_step, real beta, real* D, int D_step, int flags);

// cvGEMM( a_height, a_width, d_width, A, A_step, B, B_step, alpha, C, C_step, beta, D, D_step, flags )
#define cvMatMulAdd( a_height, a_width, d_width, A, A_step, B, B_step, C, C_step, D, D_step ) \
  cvGEMM( a_height, a_width, d_width, A, A_step, B, B_step, 1, C, C_step, 1, D, D_step, 0 )

#define cvMatMul( a_height, a_width, d_width, A, A_step, B, B_step, D, D_step ) \
  cvMatMulAdd( a_height, a_width, d_width, A, A_step, B, B_step, 0, 0, D, D_step )

#define cvMatMul_AxAT(a_height, a_width, A, A_step, D, D_step)\
  cvGEMM( a_height, a_width, a_height, A, A_step, A, A_step, 1, 0, 0, 1, D, D_step, CV_GEMM_B_T )

#define cvMatMul_ATxA(a_height, a_width, A, A_step, D, D_step)\
  cvGEMM( a_height, a_width, a_height, A, A_step, A, A_step, 1, 0, 0, 1, D, D_step, CV_GEMM_A_T )

#define CV_SVD_MODIFY_A   1
#define CV_SVD_U_T        2
#define CV_SVD_V_T        4

#define CV_LU  0
#define CV_SVD 1
#define CV_SVD_SYM 2

void
cvSVD(int arows, int acols, real* aarr, int astep,
    real* warr, int wstep, int u_cols, real* uarr, int ustep,
    real* varr, int vstep, int flags);

void
cvSVBkSb(const real* warr, int wstep,
    const real* uarr, int ustep,
    const real* varr, int vstep,
    int bcols, const real* barr, int bstep,
    int xrows, int xcols, real* xarr, int xstep, int flags);

//void
//cvCalcCovarMatrix( int vecheight, int vecwidth, const real* vec, int vecstep,
//                   real* cov, int covstep, real* avg, int flags );

//real
//cvInvert( int srcrows, int srccols, const real* srcarr, int srcstep,
//          real* dstarr, int dststep, int method );

int
cvSolve(int srcrows, int srccols, const real* A, int srcstep,
    int bcols, const real* b, int bstep, real* x, int xstep, int method);

void cvDFT(int src_rows, int src_cols, const real* src, int src_step, int srccn,
    int dst_rows, int dst_cols, real* dst, int dst_step, int dstcn,
    int flags, int nonzero_rows);

#endif // _MATMUL_H_
