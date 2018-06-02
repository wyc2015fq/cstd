
#ifndef _MAT3_INL_
#define _MAT3_INL_

#include "math/math_def.txt"
#define EYE3DEF(x)    x, 0, 0, 0, x, 0, 0, 0, x

/*
* NOTE: All computations are done COLUMN MAJOR!
*/

#if defined(MATLAB_FMT)
#define printmat3_fmt(A)        #A " = [ %g %g %g; %g %g %g; %g %g %g ]\n"
#define readmat3_fmt(A)         "%lf %lf %lf %lf %lf %lf %lf %lf %lf"
#define printcol3_fmt(A,j)      #A " = [ %g %g %g ]'\n"
#define printcolp3_fmt(A,P,j)   #P #A " = [ %g %g %g ]'\n"
#define printrow3_fmt(A,i)      #A " = [ %g %g %g ]\n"
#define printL3_fmt(LDU,P)      "L = [ 1 0 0; %g 1 0; %g %g 1 ]\n"
#define printD3_fmt(LDU,P)      "D = [ %g 0 0; 0 %g 0; 0 0 %g ]\n"
#define printU3_fmt(LDU,P)      "U = [ 1 %g %g; 0 1 %g; 0 0 1 ]\n"
#define printP3_fmt(P)          #P " = [ %d %d %d; %d %d %d; %d %d %d ]\n"
#define printmatp3_fmt(A,P)     #A #P " = [ %g %g %g; %g %g %g; %g %g %g ]\n"
#else /* MATLAB_FMT */
#define printmat3_fmt(A)        #A " =\n%g %g %g\n%g %g %g\n%g %g %g\n"
#define readmat3_fmt(A)         "%lf %lf %lf %lf %lf %lf %lf %lf %lf"
#define printcol3_fmt(A,j)      #A " =\n%g\n%g\n%g\n"
#define printcolp3_fmt(A,P,j)   #P #A " =\n%g\n%g\n%g\n"
#define printrow3_fmt(A,i)      #A " =\n%g %g %g\n"
#define printL3_fmt(LDU,P)      "L =\n1 0 0\n%g 1 0\n%g %g 1\n"
#define printD3_fmt(LDU,P)      "D =\n%g 0 0\n0 %g 0\n0 0 %g\n"
#define printU3_fmt(LDU,P)      "U =\n1 %g %g\n0 1 %g\n0 0 1\n"
#define printP3_fmt(P)          #P " =\n%d %d %d\n%d %d %d\n%d %d %d\n"
#define printmatp3_fmt(A,P)     #A #P " =\n%g %g %g\n%g %g %g\n%g %g %g\n"
#endif /* !MATLAB_FMT */

#define printmat3(A)    ( printf(printmat3_fmt(A),                                \
  ((double *)(A))[3*0+0], ((double *)(A))[3*0+1], ((double *)(A))[3*0+2],         \
  ((double *)(A))[3*1+0], ((double *)(A))[3*1+1], ((double *)(A))[3*1+2],         \
  ((double *)(A))[3*2+0], ((double *)(A))[3*2+1], ((double *)(A))[3*2+2]) )

#define readmat3(A)     ( scanf(readmat3_fmt(A),                          \
  (double *)(A)+(3*0+0]), (double *)(A)+(3*0+1), (double *)(A)+(3*0+2),    \
  (double *)(A)+(3*1+0]), (double *)(A)+(3*1+1), (double *)(A)+(3*1+2),    \
  (double *)(A)+(3*2+0]), (double *)(A)+(3*2+1), (double *)(A)+(3*2+2)) )

#define printcol3(A,j)  ( printf(printcol3_fmt(A,j),    \
  ((double *)(A))[3*(j)+0],                             \
  ((double *)(A))[3*(j)+1],                             \
((double *)(A))[3*(j)+2]) )

#define printcolp3(A,P,j)  ( printf(printcolp3_fmt(A,P,j),  \
  ((double *)(A))[3*(j)+(P)[0]],                            \
  ((double *)(A))[3*(j)+(P)[1]],                            \
((double *)(A))[3*(j)+(P)[2]]) )

#define printrow3(A,i)  ( printf(printrow3_fmt(A,i)                                     \
((double *)(A))[3*0+(i)], ((double *)(A))[3*1+(i)], ((double *)(A))[3*2+(i)]) )

#define printL3(LDU,P)  ( printf(printL3_fmt(LDU,P),                    \
  ((double *)(LDU))[3*(P)[0]+1],                                  \
((double *)(LDU))[3*(P)[0]+2], ((double *)(LDU))[3*(P)[1]+2]) )

#define printD3(LDU,P)  ( printf(printD3_fmt(LDU,P),    \
  ((double *)(LDU))[3*(P)[0]+0],                  \
  ((double *)(LDU))[3*(P)[1]+1],                  \
((double *)(LDU))[3*(P)[2]+2]) )

#define printU3(LDU,P)  ( printf(printU3_fmt(LDU,P),                    \
  ((double *)(LDU))[3*(P)[1]+0], ((double *)(LDU))[3*(P)[2]+0],   \
((double *)(LDU))[3*(P)[2]+1]) )

#define printP3(P)      ( printf(printP3_fmt(P),        \
  (P)[0]==0, (P)[1]==0, (P)[2]==0,                \
  (P)[0]==1, (P)[1]==1, (P)[2]==1,                \
(P)[0]==2, (P)[1]==2, (P)[2]==2) )

#define printmatp3(A,P) ( printf(printmatp3_fmt(A,P),                                           \
  ((double *)(A))[3*(P)[0]+0], ((double *)(A))[3*(P)[1]+0], ((double *)(A))[3*(P)[2]+0],  \
  ((double *)(A))[3*(P)[0]+1], ((double *)(A))[3*(P)[1]+1], ((double *)(A))[3*(P)[2]+1],  \
((double *)(A))[3*(P)[0]+2], ((double *)(A))[3*(P)[1]+2], ((double *)(A))[3*(P)[2]+2]) )


/* Computes cross product of 3D vectors x, y and stores the result in z */
CC_INLINE void cross(double* z, const double* x, const double* y) {
  z[0] = x[1]*y[2]-x[2]*y[1];
  z[1] = -(x[0]*y[2]-x[2]*y[0]);
  z[2] = x[0]*y[1]-x[1]*y[0];
}

/* Sorts 3 elements */
CC_INLINE void sort3(double* x) {
  double tmp;
  
  if (x[0] < x[1]) {
    tmp = x[0];
    x[0] = x[1];
    x[1] = tmp;
  }
  if (x[1] < x[2]) {
    if (x[0] < x[2]) {
      tmp = x[2];
      x[2] = x[1];
      x[1] = x[0];
      x[0] = tmp;
    }
    else {
      tmp = x[1];
      x[1] = x[2];
      x[2] = tmp;
    }
  }
}

/* Normalizes a 3D vector (with respect to L2) */
CC_INLINE double unit3(double* x) {
  double tmp = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
  x[0] /= tmp;
  x[1] /= tmp;
  x[2] /= tmp;
  return tmp;
}

/* Does the backward-solve step, or U*x = y */
CC_INLINE void ldubsolve3(double* x, const double* y, const double* LDU, const int* P) {
  x[P[2]] = y[2];
  x[P[1]] = y[1] - LDU[3*P[2]+1]*x[P[2]];
  x[P[0]] = y[0] - LDU[3*P[2]+0]*x[P[2]] - LDU[3*P[1]+0]*x[P[1]];
  
#ifdef DEBUG
  puts("\nbsolve");
  printcol3(y,0);
  putchar('\n');
  printcolp3(x,P,0);
  putchar('\n');
#endif
}

CC_INLINE void trans2(double* A_) {
  double2* A = (double2*)A_;
  double tmp;
  CC_SWAP(A[0][1], A[1][0], tmp);
}
/* Computes the matrix transpose of A */
CC_INLINE void trans3(double* A_) {
  double3* A = (double3*)A_;
  double tmp;
  CC_SWAP(A[0][1], A[1][0], tmp);
  CC_SWAP(A[0][2], A[2][0], tmp);
  CC_SWAP(A[1][2], A[2][1], tmp);
}

/* Computes the matrix multiplication C = A*B */
CC_INLINE void _matmul2(double* C_, const double* A_, const double* B_) {
  double2* A = (double2*)A_;
  double2* B = (double2*)B_;
  double2* C = (double2*)C_;
  C[0][0] = A[0][0]*B[0][0] + A[1][0]*B[0][1];
  C[1][0] = A[0][0]*B[1][0] + A[1][0]*B[1][1];
  
  C[0][1] = A[0][1]*B[0][0] + A[1][1]*B[0][1];
  C[1][1] = A[0][1]*B[1][0] + A[1][1]*B[1][1];
}
CC_INLINE void matmul2(double* C, const double* A, const double* B) {
  double4 A_, B_;
  COPY4(A_, A);
  COPY4(B_, B);
  _matmul2(C, A_, B_);
}
CC_INLINE void matmulvec3Impl(double3 C_, const double9 A_, double b0, double b1, double b2, bool transA CC_DEFAULT(false) ) {
  double3* A = (double3*)A_;
  if (transA) {
    C_[0] = A[0][0]*b0 + A[0][1]*b1 + A[0][2]*b2;
    C_[1] = A[1][0]*b0 + A[1][1]*b1 + A[1][2]*b2;
    C_[2] = A[2][0]*b0 + A[2][1]*b1 + A[2][2]*b2;
  } else {
    C_[0] = A[0][0]*b0 + A[1][0]*b1 + A[2][0]*b2;
    C_[1] = A[0][1]*b0 + A[1][1]*b1 + A[2][1]*b2;
    C_[2] = A[0][2]*b0 + A[1][2]*b1 + A[2][2]*b2;
  }
}
CC_INLINE void matmulvec3(double3 C_, const double9 A_, const double3 B_, bool transA CC_DEFAULT(false) ) {
  matmulvec3Impl(C_, A_, B_[0], B_[1], B_[2], transA);
  return ;
}
CC_INLINE void matmul3_(double* C_, const double* A_, const double* B_) {
  double3* A = (double3*)A_;
  double3* B = (double3*)B_;
  double3* C = (double3*)C_;
  C[0][0] = A[0][0]*B[0][0] + A[1][0]*B[0][1] + A[2][0]*B[0][2];
  C[1][0] = A[0][0]*B[1][0] + A[1][0]*B[1][1] + A[2][0]*B[1][2];
  C[2][0] = A[0][0]*B[2][0] + A[1][0]*B[2][1] + A[2][0]*B[2][2];
  
  C[0][1] = A[0][1]*B[0][0] + A[1][1]*B[0][1] + A[2][1]*B[0][2];
  C[1][1] = A[0][1]*B[1][0] + A[1][1]*B[1][1] + A[2][1]*B[1][2];
  C[2][1] = A[0][1]*B[2][0] + A[1][1]*B[2][1] + A[2][1]*B[2][2];
  
  C[0][2] = A[0][2]*B[0][0] + A[1][2]*B[0][1] + A[2][2]*B[0][2];
  C[1][2] = A[0][2]*B[1][0] + A[1][2]*B[1][1] + A[2][2]*B[1][2];
  C[2][2] = A[0][2]*B[2][0] + A[1][2]*B[2][1] + A[2][2]*B[2][2];
}
CC_INLINE void matmul3(double* C, const double* A, const double* B) {
  double9 A_, B_;
  COPY9(A_, A);
  COPY9(B_, B);
  matmul3_(C, A_, B_);
}

CC_INLINE void scale2(double* B, const double* A, double alpha) {
  B[0] = A[0] * alpha;
  B[1] = A[1] * alpha;
}
CC_INLINE void scale3(double* B, const double* A, double alpha) {
  B[0] = A[0] * alpha;
  B[1] = A[1] * alpha;
  B[2] = A[2] * alpha;
}
CC_INLINE void scale4(double* B, const double* A, double alpha) {
  B[0] = A[0] * alpha;
  B[1] = A[1] * alpha;
  B[2] = A[2] * alpha;
  B[3] = A[3] * alpha;
}
CC_INLINE void scale9(double* B, const double* A, double alpha) {
  B[0] = A[0] * alpha;
  B[1] = A[1] * alpha;
  B[2] = A[2] * alpha;
  B[3] = A[3] * alpha;
  B[4] = A[4] * alpha;
  B[5] = A[5] * alpha;
  B[6] = A[6] * alpha;
  B[7] = A[7] * alpha;
  B[8] = A[8] * alpha;
}
// D = alpha*A*B + beta*C + gama
CC_INLINE void _gemm2(const double* A_, const double* B_, double alpha, const double* C_, double beta, double gama, double* D_) {
  double2* A = (double2*)A_;
  double2* B = (double2*)B_;
  double2* C = (double2*)C_;
  double2* D = (double2*)D_;
  if (alpha!=0) {
    if (NULL==C || beta==0) {
      D[0][0] = (A[0][0]*B[0][0] + A[1][0]*B[0][1]) * alpha + gama;
      D[1][0] = (A[0][0]*B[1][0] + A[1][0]*B[1][1]) * alpha + gama;
      
      D[0][1] = (A[0][1]*B[0][0] + A[1][1]*B[0][1]) * alpha + gama;
      D[1][1] = (A[0][1]*B[1][0] + A[1][1]*B[1][1]) * alpha + gama;
    } else {
      D[0][0] = (A[0][0]*B[0][0] + A[1][0]*B[0][1]) * alpha + C[0][0] * beta + gama;
      D[1][0] = (A[0][0]*B[1][0] + A[1][0]*B[1][1]) * alpha + C[1][0] * beta + gama;
      
      D[0][1] = (A[0][1]*B[0][0] + A[1][1]*B[0][1]) * alpha + C[0][1] * beta + gama;
      D[1][1] = (A[0][1]*B[1][0] + A[1][1]*B[1][1]) * alpha + C[1][1] * beta + gama;
    }
  } else {
    if (NULL==C || beta==0) {
      D[0][0] = gama;
      D[1][0] = gama;
      
      D[0][1] = gama;
      D[1][1] = gama;
    } else {
      D[0][0] = C[0][0] * beta + gama;
      D[1][0] = C[1][0] * beta + gama;
      
      D[0][1] = C[0][1] * beta + gama;
      D[1][1] = C[1][1] * beta + gama;
    }
  }
}
// D = alpha*A*B + beta*C
CC_INLINE void gemm2(const double* A_, const double* B_, double alpha, const double* C_, double beta, double gama, double* D, int flags) {
  double4 A, B, C;
  COPY4(A, A_);
  COPY4(B, B_);
  if (flags&CC_GEMM_A_T) {
    trans2(A);
  }
  if (flags&CC_GEMM_B_T) {
    trans2(B);
  }
  if (C_) {
    COPY4(C, C_);
    if (flags&CC_GEMM_C_T) {
      trans2(C);
    }
  }
  _gemm2(A, B, alpha, C, beta, gama, D);
  if (flags&CC_GEMM_D_T) {
    trans2(D);
  }
}
CC_INLINE void matmul2(double* C, const double* A, const double* B, int flags) {
  gemm2(A, B, 1, NULL, 0, 0, C, flags);
}
CC_INLINE void _gemm3(const double* A_, const double* B_, double alpha, const double* C_, double beta, double gama, double* D_) {
  double3* A = (double3*)A_;
  double3* B = (double3*)B_;
  double3* C = (double3*)C_;
  double3* D = (double3*)D_;
  if (alpha!=0) {
    if (NULL==C || beta==0) {
      D[0][0] = (A[0][0]*B[0][0] + A[1][0]*B[0][1] + A[2][0]*B[0][2]) * alpha + gama;
      D[1][0] = (A[0][0]*B[1][0] + A[1][0]*B[1][1] + A[2][0]*B[1][2]) * alpha + gama;
      D[2][0] = (A[0][0]*B[2][0] + A[1][0]*B[2][1] + A[2][0]*B[2][2]) * alpha + gama;
      
      D[0][1] = (A[0][1]*B[0][0] + A[1][1]*B[0][1] + A[2][1]*B[0][2]) * alpha + gama;
      D[1][1] = (A[0][1]*B[1][0] + A[1][1]*B[1][1] + A[2][1]*B[1][2]) * alpha + gama;
      D[2][1] = (A[0][1]*B[2][0] + A[1][1]*B[2][1] + A[2][1]*B[2][2]) * alpha + gama;
      
      D[0][2] = (A[0][2]*B[0][0] + A[1][2]*B[0][1] + A[2][2]*B[0][2]) * alpha + gama;
      D[1][2] = (A[0][2]*B[1][0] + A[1][2]*B[1][1] + A[2][2]*B[1][2]) * alpha + gama;
      D[2][2] = (A[0][2]*B[2][0] + A[1][2]*B[2][1] + A[2][2]*B[2][2]) * alpha + gama;
    } else {
      D[0][0] = (A[0][0]*B[0][0] + A[1][0]*B[0][1] + A[2][0]*B[0][2]) * alpha + C[0][0] * beta + gama;
      D[1][0] = (A[0][0]*B[1][0] + A[1][0]*B[1][1] + A[2][0]*B[1][2]) * alpha + C[1][0] * beta + gama;
      D[2][0] = (A[0][0]*B[2][0] + A[1][0]*B[2][1] + A[2][0]*B[2][2]) * alpha + C[2][0] * beta + gama;
      
      D[0][1] = (A[0][1]*B[0][0] + A[1][1]*B[0][1] + A[2][1]*B[0][2]) * alpha + C[0][1] * beta + gama;
      D[1][1] = (A[0][1]*B[1][0] + A[1][1]*B[1][1] + A[2][1]*B[1][2]) * alpha + C[1][1] * beta + gama;
      D[2][1] = (A[0][1]*B[2][0] + A[1][1]*B[2][1] + A[2][1]*B[2][2]) * alpha + C[2][1] * beta + gama;
      
      D[0][2] = (A[0][2]*B[0][0] + A[1][2]*B[0][1] + A[2][2]*B[0][2]) * alpha + C[0][2] * beta + gama;
      D[1][2] = (A[0][2]*B[1][0] + A[1][2]*B[1][1] + A[2][2]*B[1][2]) * alpha + C[1][2] * beta + gama;
      D[2][2] = (A[0][2]*B[2][0] + A[1][2]*B[2][1] + A[2][2]*B[2][2]) * alpha + C[2][2] * beta + gama;
    }
  } else {
    if (NULL==C || beta==0) {
      D[0][0] = gama;
      D[1][0] = gama;
      D[2][0] = gama;
      
      D[0][1] = gama;
      D[1][1] = gama;
      D[2][1] = gama;
      
      D[0][2] = gama;
      D[1][2] = gama;
      D[2][2] = gama;
    } else {
      D[0][0] = C[0][0] * beta + gama;
      D[1][0] = C[1][0] * beta + gama;
      D[2][0] = C[2][0] * beta + gama;
      
      D[0][1] = C[0][1] * beta + gama;
      D[1][1] = C[1][1] * beta + gama;
      D[2][1] = C[2][1] * beta + gama;
      
      D[0][2] = C[0][2] * beta + gama;
      D[1][2] = C[1][2] * beta + gama;
      D[2][2] = C[2][2] * beta + gama;
    }
  }
}
// D = alpha*A*B + beta*C
CC_INLINE void gemm3(const double* A_, const double* B_, double alpha, const double* C_, double beta, double gama, double* D, int flags) {
  double9 A, B, C;
  COPY9(A, A_);
  COPY9(B, B_);
  if (flags&CC_GEMM_A_T) {
    trans3(A);
  }
  if (flags&CC_GEMM_B_T) {
    trans3(B);
  }
  if (C) {
    COPY9(C, C_);
    if (flags&CC_GEMM_C_T) {
      trans3(C);
    }
  }
  _gemm3(A, B, alpha, C, beta, gama, D);
  if (flags&CC_GEMM_D_T) {
    trans3(D);
  }
}
// D=A*B*C
CC_INLINE void matmul3_3(double* D, const double* A, const double* B, const double* C) {
  matmul3(D, A, B);
  matmul3(D, D, C);
}
/* Computes the matrix multiplication y = A*x */
CC_INLINE void matvec2(double* y, const double* A_, double x0, double x1) {
  double2* A = (double2*)A_;
  y[0] = A[0][0]*x0 + A[1][0]*x1;
  y[1] = A[0][1]*x0 + A[1][1]*x1;
}
CC_INLINE void matvec2(double* y, const double* A_, const double* x) {
  double2* A = (double2*)A_;
  double x0 = x[0];
  double x1 = x[1];
  y[0] = A[0][0]*x0 + A[1][0]*x1;
  y[1] = A[0][1]*x0 + A[1][1]*x1;
}
CC_INLINE void matvec3(double* y, const double* A_, double x0, double x1, double x2) {
  double2* A = (double2*)A_;
  y[0] = A[0][0]*x0 + A[1][0]*x1 + A[2][0]*x2;
  y[1] = A[0][1]*x0 + A[1][1]*x1 + A[2][1]*x2;
  y[2] = A[0][2]*x0 + A[1][2]*x1 + A[2][2]*x2;
}
CC_INLINE void matvec3(double* y, const double* A_, const double* x) {
  double2* A = (double2*)A_;
  double x0 = x[0];
  double x1 = x[1];
  double x2 = x[2];
  y[0] = A[0][0]*x0 + A[1][0]*x1 + A[2][0]*x2;
  y[1] = A[0][1]*x0 + A[1][1]*x1 + A[2][1]*x2;
  y[2] = A[0][2]*x0 + A[1][2]*x1 + A[2][2]*x2;
}
CC_INLINE void matvec3t(double* y, const double* A_, const double* x) {
  double2* A = (double2*)A_;
  double x0 = x[0];
  double x1 = x[1];
  double x2 = x[2];
  y[0] = A[0][0]*x0 + A[0][1]*x1 + A[0][2]*x2;
  y[1] = A[1][0]*x0 + A[1][1]*x1 + A[1][2]*x2;
  y[2] = A[1][0]*x0 + A[2][1]*x1 + A[2][2]*x2;
}

/* Computes the matrix multiplication AA = A^T*A */
CC_INLINE void ata2(double* _AA, const double* A_) {
  double2* AA = (double2*)_AA;
  double22 A;
  COPY4(A[0], A_);
  AA[0][0] = A[0][0]*A[0][0] + A[0][1]*A[0][1];
  AA[1][0] = A[0][0]*A[1][0] + A[0][1]*A[1][1];
  
  AA[0][1] = AA[1][0];
  AA[1][1] = A[1][0]*A[1][0] + A[1][1]*A[1][1];
  
}
/* Computes the matrix multiplication AA = A^T*A */
CC_INLINE void ata3(double* _AA, const double* A_) {
  double3* AA = (double3*)_AA;
  double33 A;
  COPY9(A[0], A_);
  AA[0][0] = A[0][0]*A[0][0] + A[0][1]*A[0][1] + A[0][2]*A[0][2];
  AA[1][0] = A[0][0]*A[1][0] + A[0][1]*A[1][1] + A[0][2]*A[1][2];
  AA[2][0] = A[0][0]*A[2][0] + A[0][1]*A[2][1] + A[0][2]*A[2][2];
  
  AA[0][1] = AA[1][0];
  AA[1][1] = A[1][0]*A[1][0] + A[1][1]*A[1][1] + A[1][2]*A[1][2];
  AA[2][1] = A[1][0]*A[2][0] + A[1][1]*A[2][1] + A[1][2]*A[2][2];
  
  AA[0][2] = AA[2][0];
  AA[1][2] = AA[2][1];
  AA[2][2] = A[2][0]*A[2][0] + A[2][1]*A[2][1] + A[2][2]*A[2][2];
}

/* Computes the matrix multiplication AA = A*A^T */
CC_INLINE void aat3(double* _AA, const double* A_) {
  double3* A = (double3*)A_;
  double3* AA = (double3*)_AA;
  AA[0][0] = A[0][0]*A[0][0] + A[1][0]*A[1][0] + A[2][0]*A[2][0];
  AA[1][0] = A[0][0]*A[0][1] + A[1][0]*A[1][1] + A[2][0]*A[2][1];
  AA[2][0] = A[0][0]*A[0][2] + A[1][0]*A[1][2] + A[2][0]*A[2][2];
  
  AA[0][1] = AA[1][0];
  AA[1][1] = A[0][1]*A[0][1] + A[1][1]*A[1][1] + A[2][1]*A[2][1];
  AA[2][1] = A[0][1]*A[0][2] + A[1][1]*A[1][2] + A[2][1]*A[2][2];
  
  AA[0][2] = AA[2][0];
  AA[1][2] = AA[2][1];
  AA[2][2] = A[0][2]*A[0][2] + A[1][2]*A[1][2] + A[2][2]*A[2][2];
}
#define CC_DET2(m)   (m(0,0)*m(1,1) - m(0,1)*m(1,0))
#define CC_DET3(m)   (m(0,0)*(m(1,1)*m(2,2) - m(1,2)*m(2,1)) -  \
  m(0,1)*(m(1,0)*m(2,2) - m(1,2)*m(2,0)) +  \
m(0,2)*(m(1,0)*m(2,1) - m(1,1)*m(2,0)))

static double det2(const double4 A) {
#define _Md(y, x)  A[y*2+x]
  return CC_DET2(_Md);
#undef _Md
}
static double det3(const double9 A) {
#define _Md(y, x)  A[y*3+x]
  return CC_DET3(_Md);
#undef _Md
}
#undef CC_DET2
#undef CC_DET3
static void eye2(double4 A) {
  M2ID(A);
}
static void eye3(double9 A) {
  M3ID(A);
}

static double inv2(double4 B_, const double4 A_) {
  double d = det2(A_);
  if(d != 0.)
  {
    double2* A = (double2*)A_;
    double2* B = (double2*)B_;
    double Sf00 = A[0][0];
    double Sf01 = A[0][1];
    double Sf10 = A[1][0];
    double Sf11 = A[1][1];
    double t0, t1;
    d = 1./d;
    t0 = Sf00*d;
    t1 = Sf11*d;
    B[1][1] = t0;
    B[0][0] = t1;
    t0 = -Sf01*d;
    t1 = -Sf10*d;
    B[0][1] = t0;
    B[1][0] = t1;
  }
  return d;
}
/*
* Solves for the roots of a monic cubic polynomial with 3 coefficients
* ordered by degree that is assumed to have 3 real roots (D <= 0)
*/
static void solvecubic(double* c) {
  const double sq3d2 = 0.86602540378443864676, c2d3 = c[2]/3,
    c2sq = c[2]*c[2], Q = (3*c[1]-c2sq)/9,
    R = (c[2]*(9*c[1]-2*c2sq)-27*c[0])/54;
  double tmp, t, sint, cost;
  
  if (Q < 0) {
  /*
  * Instead of computing
  * c_0 = A cos(t) - B
  * c_1 = A cos(t + 2 pi/3) - B
  * c_2 = A cos(t + 4 pi/3) - B
  * Use cos(a+b) = cos(a) cos(b) - sin(a) sin(b)
  * Keeps t small and eliminates 1 function call.
  * cos(2 pi/3) = cos(4 pi/3) = -0.5
  * sin(2 pi/3) = sqrt(3)/2
  * sin(4 pi/3) = -sqrt(3)/2
    */
    
    tmp = 2*sqrt(-Q);
    t = acos(R/sqrt(-Q*Q*Q))/3;
    cost = tmp*cos(t);
    sint = tmp*sin(t);
    
    c[0] = cost - c2d3;
    
    cost = -0.5*cost - c2d3;
    sint = sq3d2*sint;
    
    c[1] = cost - sint;
    c[2] = cost + sint;
  }
  else {
    tmp = cCbrt(R);
    c[0] = -c2d3 + 2*tmp;
    c[1] = c[2] = -c2d3 - tmp;
  }
}

/* Computes the LDUP decomposition in-place */
static void ldu3(double* A, int* P) {
  int tmp;
  
#ifdef DEBUG
  printmat3(A);
  putchar('\n');
#endif
  
  P[1] = 1;
  P[2] = 2;
  
  P[0] = fabs(A[3*1+0]) > fabs(A[3*0+0]) ?
    (fabs(A[3*2+0]) > fabs(A[3*1+0]) ? 2 : 1) :
  (fabs(A[3*2+0]) > fabs(A[3*0+0]) ? 2 : 0);
  P[P[0]] = 0;
  
  if (fabs(A[3*P[2]+1]) > fabs(A[3*P[1]+1])) {
    tmp = P[1];
    P[1] = P[2];
    P[2] = tmp;
  }
  
#ifdef DEBUG
  printmatp3(A,P);
  putchar('\n');
#endif
  
  if (A[3*P[0]+0] != 0) {
    A[3*P[1]+0] = A[3*P[1]+0]/A[3*P[0]+0];
    A[3*P[2]+0] = A[3*P[2]+0]/A[3*P[0]+0];
    A[3*P[0]+1] = A[3*P[0]+1]/A[3*P[0]+0];
    A[3*P[0]+2] = A[3*P[0]+2]/A[3*P[0]+0];
  }
  
  A[3*P[1]+1] = A[3*P[1]+1] - A[3*P[0]+1]*A[3*P[1]+0]*A[3*P[0]+0];
  
  if (A[3*P[1]+1] != 0) {
    A[3*P[2]+1] = (A[3*P[2]+1] - A[3*P[0]+1]*A[3*P[2]+0]*A[3*P[0]+0])/A[3*P[1]+1];
    A[3*P[1]+2] = (A[3*P[1]+2] - A[3*P[0]+2]*A[3*P[1]+0]*A[3*P[0]+0])/A[3*P[1]+1];
  }
  
  A[3*P[2]+2] = A[3*P[2]+2] - A[3*P[0]+2]*A[3*P[2]+0]*A[3*P[0]+0] - A[3*P[1]+2]*A[3*P[2]+1]*A[3*P[1]+1];
  
#ifdef DEBUG
  printL3(A,P);
  putchar('\n');
  printD3(A,P);
  putchar('\n');
  printU3(A,P);
  putchar('\n');
  printP3(P);
#endif
}
static double dot3(const double4 a, const double4 b) {
  return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}
static double dot4(const double3 a, const double3 b) {
  return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3];
}

#if 0
static void svd2_AbsScaleOnly(const double4 matrixIn, double2 scale)
{
  double* vectorized = matrixIn;
  double Q = dot4(vectorized, vectorized);
  double R = det2(matrixIn); //ad-bc
  double discriminant = sqrt(Q*Q-4*R*R);
  
  scale[0] = sqrt(Q+discriminant) / 2;
  scale[0] = sqrt(Q-discriminant) / 2;
}
static double22 svd2_EigenvectorGivenEigenvalue(const double4 matrixIn, double2 eigenvalue)
{
  eigenvalue = eigenvalue * eigenvalue;
  
  double M01 = dot(matrixIn[0], matrixIn[1]);
  double M00 = dot(matrixIn[0], matrixIn[0]);
  
  double2 eigenVectorNumerator = double2(M01, M01);
  double2 eigenVectorDenominator = eigenvalue - double2(M00, M00);
  
  double2 eigenFraction = eigenVectorNumerator / eigenVectorDenominator;
  
  double22 eigenMatrix = {
    normalize(double2(eigenFraction.x, 1)),
      normalize(double2(eigenFraction.y, 1)),
  };
  
  eigenMatrix = transpose(eigenMatrix);
  
  return eigenMatrix;
}
static void svd2_PartialDecompose(double22 matrixIn, out double22 U, out double2 scale)
{
  scale = svd2_AbsScaleOnly(matrixIn);
  U = svd2_EigenvectorGivenEigenvalue(matrixIn, scale);
}

static void svd2_FullDecompose(const double4 matrixIn, double4 U, double4 scale, double4 V)
{
  svd2_PartialDecompose(matrixIn, U, scale);
  
  // A = U * E * V
  // U^T * A = E * V
  // E^-1 * U^T * A = V
  
  double2 inverseScale = 1.0 / scale;
  
  double22 E = {
    inverseScale.x, 0,
      0, inverseScale.y,
  };
  
  V = mul(E, mul(transpose(U), matrixIn));
}
#endif
static void svd3(double* U, double* S, double* V, const double* A_) {
  double9 A;
  //double9 buffer;
  COPY9(A, A_);
  img_t Amat[1] = {0};
  img_t Umat[1] = {0};
  img_t Smat[1] = {0};
  img_t Vmat[1] = {0};
  cvMat(Amat, 3, 3, CC_64F, 1, A);
  cvMat(Umat, 3, 3, CC_64F, 1, U);
  cvMat(Vmat, 3, 3, CC_64F, 1, V);
  cvMat(Smat, 3, 1, CC_64F, 1, S);
  cvSVD(Amat, Smat, Umat, Vmat, CC_SVD_MODIFY_A);
  //icvSVD_64f(A, 3, 3, 3, S, U, 3, 3, V, 3, buffer);
}
static void ssvd2(float* U, float* S, float* V, const float* A_) {
  float9 A;
  //double9 buffer;
  COPY9(A, A_);
  img_t Amat[1] = {0};
  img_t Umat[1] = {0};
  img_t Smat[1] = {0};
  img_t Vmat[1] = {0};
  cvMat(Amat, 2, 2, CC_32F, 1, A);
  cvMat(Umat, 2, 2, CC_32F, 1, U);
  cvMat(Vmat, 2, 2, CC_32F, 1, V);
  cvMat(Smat, 2, 1, CC_32F, 1, S);
  cvSVD(Amat, Smat, Umat, Vmat, CC_SVD_MODIFY_A);
  //icvSVD_64f(A, 3, 3, 3, S, U, 3, 3, V, 3, buffer);
}
/* Explicitly computes the SVD of a 3x3 matrix */
static void svd3a(double* U, double* S, double* V, const double* A) {
  const double thr = 1e-10;
  int P[3], k;
  double y[3], AA[3][3], LDU[3][3];
  
  // Steps:
  // 1) Use eigendecomposition on A^T A to compute V.
  // Since A = U S V^T then A^T A = V S^T S V^T with D = S^T S and V the
  // eigenvalues and eigenvectors respectively (V is orthogonal).
  // 2) Compute U from A and V.
  // 3) Normalize columns of U and V and root the eigenvalues to obtain
  // the singular values.
  
  
  /* Compute AA = A^T A */
  ata3((double *)AA, A);
  
  /* Form the monic characteristic polynomial */
  S[2] = -AA[0][0] - AA[1][1] - AA[2][2];
  S[1] = AA[0][0]*AA[1][1] + AA[2][2]*AA[0][0] + AA[2][2]*AA[1][1] -
    AA[2][1]*AA[1][2] - AA[2][0]*AA[0][2] - AA[1][0]*AA[0][1];
  S[0] = AA[2][1]*AA[1][2]*AA[0][0] + AA[2][0]*AA[0][2]*AA[1][1] + AA[1][0]*AA[0][1]*AA[2][2] -
    AA[0][0]*AA[1][1]*AA[2][2] - AA[1][0]*AA[2][1]*AA[0][2] - AA[2][0]*AA[0][1]*AA[1][2];
  
  /* Solve the cubic equation. */
  solvecubic(S);
  
  /* All roots should be positive */
  if (S[0] < 0)
    S[0] = 0;
  if (S[1] < 0)
    S[1] = 0;
  if (S[2] < 0)
    S[2] = 0;
  
  /* Sort from greatest to least */
  sort3(S);
  
  /* Form the eigenvector system for the first (largest) eigenvalue */
  memcpy(LDU,AA,sizeof(LDU));
  LDU[0][0] -= S[0];
  LDU[1][1] -= S[0];
  LDU[2][2] -= S[0];
  
  /* Perform LDUP decomposition */
  ldu3((double *)LDU, P);
  
  // Write LDU = AA-I*lambda.  Then an eigenvector can be
  // found by solving LDU x = LD y = L z = 0
  // L is invertible, so L z = 0 implies z = 0
  // D is singular since det(AA-I*lambda) = 0 and so
  // D y = z = 0 has a non-unique solution.
  // Pick k so that D_kk = 0 and set y = e_k, the k'th column
  // of the identity matrix.
  // U is invertible so U x = y has a unique solution for a given y.
  // The solution for U x = y is an eigenvector.
  
  
  /* Pick the component of D nearest to 0 */
  y[0] = y[1] = y[2] = 0;
  k = fabs(LDU[P[1]][1]) < fabs(LDU[P[0]][0]) ?
    (fabs(LDU[P[2]][2]) < fabs(LDU[P[1]][1]) ? 2 : 1) :
  (fabs(LDU[P[2]][2]) < fabs(LDU[P[0]][0]) ? 2 : 0);
  y[k] = 1;
  
  /* Do a backward solve for the eigenvector */
  ldubsolve3(V+(3*0+0), y, (double *)LDU, P);
  
  /* Form the eigenvector system for the last (smallest) eigenvalue */
  memcpy(LDU,AA,sizeof(LDU));
  LDU[0][0] -= S[2];
  LDU[1][1] -= S[2];
  LDU[2][2] -= S[2];
  
  /* Perform LDUP decomposition */
  ldu3((double *)LDU, P);
  
  // NOTE: The arrangement of the ternary operator output is IMPORTANT!
  // It ensures a different system is solved if there are 3 repeat eigenvalues.
  
  /* Pick the component of D nearest to 0 */
  y[0] = y[1] = y[2] = 0;
  k = fabs(LDU[P[0]][0]) < fabs(LDU[P[2]][2]) ?
    (fabs(LDU[P[0]][0]) < fabs(LDU[P[1]][1]) ? 0 : 1) :
  (fabs(LDU[P[1]][1]) < fabs(LDU[P[2]][2]) ? 1 : 2);
  y[k] = 1;
  
  /* Do a backward solve for the eigenvector */
  ldubsolve3(V+(3*2+0), y, (double *)LDU, P);
  
  /* The remaining column must be orthogonal (AA is symmetric) */
  cross(V+(3*1+0), V+(3*2+0), V+(3*0+0));
  
  /* Count the rank */
  k = (S[0] > thr) + (S[1] > thr) + (S[2] > thr);
  
  switch (k) {
  case 0:
    // Zero matrix.
    // Since V is already orthogonal, just copy it into U.
    memcpy(U,V,9*sizeof(double));
    break;
  case 1:
    // The first singular value is non-zero.
    // Since A = U S V^T, then A V = U S.
    // A V_1 = S_11 U_1 is non-zero. Here V_1 and U_1 are
    // column vectors. Since V_1 is known, we may compute
    // U_1 = A V_1.  The S_11 factor is not important as
    // U_1 will be normalized later.
    matvec3(U+(3*0+0), A, V+(3*0+0));
    
    // The other columns of U do not contribute to the expansion
    // and we may arbitrarily choose them (but they do need to be
    // orthogonal). To ensure the first cross product does not fail,
    // pick k so that U_k1 is nearest 0 and then cross with e_k to
    // obtain an orthogonal vector to U_1.
    y[0] = y[1] = y[2] = 0;
    k = fabs(U[3*0+0]) < fabs(U[3*0+2]) ?
      (fabs(U[3*0+0]) < fabs(U[3*0+1]) ? 0 : 1) :
    (fabs(U[3*0+1]) < fabs(U[3*0+2]) ? 1 : 2);
    y[k] = 1;
    
    cross(U+(3*1+0), y, U+(3*0+0));
    
    /* Cross the first two to obtain the remaining column */
    cross(U+(3*2+0), U+(3*0+0), U+(3*1+0));
    break;
  case 2:
    // The first two singular values are non-zero.
    // Compute U_1 = A V_1 and U_2 = A V_2. See case 1
    // for more information.
    matvec3(U+(3*0+0), A, V+(3*0+0));
    matvec3(U+(3*1+0), A, V+(3*1+0));
    /* Cross the first two to obtain the remaining column */
    cross(U+(3*2+0), U+(3*0+0), U+(3*1+0));
    break;
  case 3:
    // All singular values are non-zero.
    // We may compute U = A V. See case 1 for more information.
    matmul3(U, A, V);
    break;
  }
  
  /* Normalize the columns of U and V */
  unit3(V+(3*0+0));
  unit3(V+(3*1+0));
  unit3(V+(3*2+0));
  
  unit3(U+(3*0+0));
  unit3(U+(3*1+0));
  unit3(U+(3*2+0));
  
  /* S was initially the eigenvalues of A^T A = V S^T S V^T which are squared. */
  S[0] = sqrt(S[0]);
  S[1] = sqrt(S[1]);
  S[2] = sqrt(S[2]);
}

static int test_mat3() {
  double3 s;
  double33 U, S, V, USVT, US, UTU, VTV;
  memset(S,0,sizeof(S));
  double33 A = {1,2,3, 4,5,6, 7,8,9};
  svd3((double *)U, s, (double *)V, (double *)A);
  if (0) {
    //e = readmat3(A);
    putchar('\n');
    
    printmat3(A);
    
    puts("\nComputing SVD ...\n");
    
    svd3((double *)U, s, (double *)V, (double *)A);
    
    S[0][0] = s[0];
    S[1][1] = s[1];
    S[2][2] = s[2];
    
    printmat3(U);
    putchar('\n');
    printmat3(S);
    putchar('\n');
    printmat3(V);
    putchar('\n');
    
    ata3((double *)UTU, (double *)U);
    printmat3(UTU);
    putchar('\n');
    
    ata3((double *)VTV, (double *)V);
    printmat3(VTV);
    putchar('\n');
    
    matmul3((double *)US, (double *)U, (double *)S);
    trans3((double *)V);
    matmul3((double *)USVT, (double *)US, (double *)V);
    
    printmat3(USVT);
    
    //fflush(stdin);
  }
  
  return 0;
}

#endif // _MAT3_INL_

