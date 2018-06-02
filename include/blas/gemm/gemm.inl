//#include "commons.h"

typedef enum CBLAS_ORDER     {CblasRowMajor=101, CblasColMajor=102} CBLAS_ORDER;
typedef enum CBLAS_TRANSPOSE {CblasNoTrans=111, CblasTrans=112, CblasConjTrans=113, CblasConjNoTrans=114} CBLAS_TRANSPOSE;
typedef enum CBLAS_UPLO      {CblasUpper=121, CblasLower=122} CBLAS_UPLO;
typedef enum CBLAS_DIAG      {CblasNonUnit=131, CblasUnit=132} CBLAS_DIAG;
typedef enum CBLAS_SIDE      {CblasLeft=141, CblasRight=142} CBLAS_SIDE;

#define MC  384
#define KC  384
#define NC  40960
#define MR  4
#define NR  4

struct Gemmer {
  float A_[MC*KC];
  float B_[KC*NC];
  float C_[MR*NR];
  float AB_[MR*NR];
};

#if 1
#define equal_f32(a, b)  ((a)==(b))
#else
bool equal_f32(float a, float b) {
  const float EPSILON = 1e-5;
  if (fabsf(a - b) < EPSILON) {
    return true;
  }
  return false;
}
#endif

void pack_MRxk(int k, const float *A, int al, int ai, float *buffer) {
  int j, a2 = al, a3 = 2*al, a4 = 3*al;
  for (j = 0; j < k; ++j) {
    // for (int i = 0; i < MR; ++i) {
    //     buffer[i] = A[i*al];
    // }
    buffer[0] = A[0];
    buffer[1] = A[a2];
    buffer[2] = A[a3];
    buffer[3] = A[a4];
    A += ai;
    buffer += MR;
  }
}

void pack_A(int mc, int kc, const float *A, int al, int ai, float *buffer) {
  int mp = mc / MR;
  int _mr = mc % MR;
  int tmp1 = kc*MR;
  int tmp2 = MR*al;
  int i, j;
  
  for (i = 0; i < mp; ++i) {
    pack_MRxk(kc, A, al, ai, buffer);
    buffer += tmp1;
    A += tmp2;
    // buffer += kc*MR;
    // A += MR*al;
  }
  if (_mr > 0) {
    for (j = 0; j < kc; ++j) {
      for (i = 0; i < _mr; ++i) {
        buffer[i] = A[i*al];
      }
      for (i = _mr; i < MR; ++i) {
        buffer[i] = 0.0;
      }
      A += 1;
      buffer += MR;
    }
  }
}

void pack_kxNR(int k, const float *B, int bl, int bi, float *buffer) {
  int i, j;
  for (i = 0; i < k; ++i) {
    for (j = 0; j < NR; ++j) {
      buffer[j] = B[j*bi];
    }
    // float32x4_t bv = vld1q_f32(B);
    // vst1q_f32(buffer, bv);
    B += bl;
    buffer += NR;
  }
}

void pack_B(int kc, int nc, const float *B, int bl, int bi, float *buffer) {
  int np = nc / NR;
  int _nr = nc % NR;
  int tmp1 = kc*NR;
  int i, j;
  
  for (j = 0; j < np; ++j) {
    pack_kxNR(kc, B, bl, bi, buffer);
    B += NR;
    buffer += tmp1;
  }
  if (_nr > 0) {
    for (i = 0; i < kc; ++i) {
      for (j = 0; j < _nr; ++j) {
        buffer[j] = B[j*bi];
      }
      for (j = _nr; j < NR; ++j) {
        buffer[j] = 0.0;
      }
      buffer += NR;
      B += bl;
    }
  }
}
#if defined(MDL_V7)

void dgemm_micro_kernel(int kc, float alpha, const float *A, const float *B, float beta, float *C, int cl,
                        int ci) {
  int i, j, l;
  float32x4_t abv0 = vdupq_n_f32(0);
  float32x4_t abv1 = vdupq_n_f32(0);
  float32x4_t abv2 = vdupq_n_f32(0);
  float32x4_t abv3 = vdupq_n_f32(0);
  
  float32x4_t av;
  float32x4_t bv;
  
  float32x2_t bv01;
  float32x2_t bv23;
  
  for (l = 0; l < kc; ++l) {
    av = vld1q_f32(A);
    bv = vld1q_f32(B);
    bv01 = vget_low_f32(bv);
    abv0 = vmlaq_lane_f32(abv0, av, bv01, 0);
    abv1 = vmlaq_lane_f32(abv1, av, bv01, 1);
    bv23 = vget_high_f32(bv);
    abv2 = vmlaq_lane_f32(abv2, av, bv23, 0);
    abv3 = vmlaq_lane_f32(abv3, av, bv23, 1);
    A += MR;
    B += NR;
  }
  
  vst1q_f32(AB_ + 0, abv0);
  vst1q_f32(AB_ + 4, abv1);
  vst1q_f32(AB_ + 8, abv2);
  vst1q_f32(AB_ + 12, abv3);
  
  if (equal_f32(beta, 0.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] = 0.0;
      }
    }
  } else if (!equal_f32(beta, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] *= beta;
      }
    }
  }
  
  if (!equal_f32(alpha, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] += alpha*s->AB_[i + j*MR];
      }
    }
  } else {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] += s->AB_[i + j*MR];
      }
    }
  }
}
#elif defined(MDL_V8)
void dgemm_micro_kernel(int kc, float alpha, const float *A, const float *B, float beta, float *C, int cl, int ci) {
  int i, j, l;
  
  float32x4_t abv0 = vdupq_n_f32(0);
  float32x4_t abv1 = vdupq_n_f32(0);
  float32x4_t abv2 = vdupq_n_f32(0);
  float32x4_t abv3 = vdupq_n_f32(0);
  
  float32x4_t av;
  float32x4_t bv;
  
  int kc1 = kc / 4, kc2 = kc % 4;
  asm volatile (
    "subs %[kc1], %[kc1], #1\n\t"
    "blt end1\n\t"
    "loop1: \n\t"
    "ld1 {%[av].4S}, [%[A]], #16\n\t"
    "ld1 {%[bv].4S}, [%[B]], #16\n\t"
    "fmla %[abv0].4S, %[av].4S, %[bv].4S[0]\n\t"
    "fmla %[abv1].4S, %[av].4S, %[bv].4S[1]\n\t"
    "fmla %[abv2].4S, %[av].4S, %[bv].4S[2]\n\t"
    "fmla %[abv3].4S, %[av].4S, %[bv].4S[3]\n\t"
    // "add %[A], %[A], #16\n\t"
    // "add %[B], %[B], #16\n\t"
    "ld1 {%[av].4S}, [%[A]], #16\n\t"
    "ld1 {%[bv].4S}, [%[B]], #16\n\t"
    "fmla %[abv0].4S, %[av].4S, %[bv].4S[0]\n\t"
    "fmla %[abv1].4S, %[av].4S, %[bv].4S[1]\n\t"
    "fmla %[abv2].4S, %[av].4S, %[bv].4S[2]\n\t"
    "fmla %[abv3].4S, %[av].4S, %[bv].4S[3]\n\t"
    // "add %[A], %[A], #16\n\t"
    // "add %[B], %[B], #16\n\t"
    "ld1 {%[av].4S}, [%[A]], #16\n\t"
    "ld1 {%[bv].4S}, [%[B]], #16\n\t"
    "fmla %[abv0].4S, %[av].4S, %[bv].4S[0]\n\t"
    "fmla %[abv1].4S, %[av].4S, %[bv].4S[1]\n\t"
    "fmla %[abv2].4S, %[av].4S, %[bv].4S[2]\n\t"
    "fmla %[abv3].4S, %[av].4S, %[bv].4S[3]\n\t"
    // "add %[A], %[A], #16\n\t"
    // "add %[B], %[B], #16\n\t"
    "ld1 {%[av].4S}, [%[A]], #16\n\t"
    "ld1 {%[bv].4S}, [%[B]], #16\n\t"
    "fmla %[abv0].4S, %[av].4S, %[bv].4S[0]\n\t"
    "fmla %[abv1].4S, %[av].4S, %[bv].4S[1]\n\t"
    "fmla %[abv2].4S, %[av].4S, %[bv].4S[2]\n\t"
    "fmla %[abv3].4S, %[av].4S, %[bv].4S[3]\n\t"
    // "add %[A], %[A], #16\n\t"
    // "add %[B], %[B], #16\n\t"
    "subs %[kc1], %[kc1], #1\n\t"
    "bge loop1\n\t"
    "end1:\n\t"
    "subs %[kc2], %[kc2], #1\n\t"
    "blt end2\n\t"
    "loop2: \n\t"
    "ld1 {%[av].4S}, [%[A]]\n\t"
    "ld1 {%[bv].4S}, [%[B]]\n\t"
    "fmla %[abv0].4S, %[av].4S, %[bv].4S[0]\n\t"
    "fmla %[abv1].4S, %[av].4S, %[bv].4S[1]\n\t"
    "fmla %[abv2].4S, %[av].4S, %[bv].4S[2]\n\t"
    "fmla %[abv3].4S, %[av].4S, %[bv].4S[3]\n\t"
    "add %[A], %[A], #16\n\t"
    "add %[B], %[B], #16\n\t"
    "subs %[kc2], %[kc2], #1\n\t"
    "bge loop2\n\t"
    "end2:\n\t"
    : [A]"=r"(A), [B]"=r"(B), [av]"=w"(av), [bv]"=w"(bv),
    [abv0]"=w"(abv0), [abv1]"=w"(abv1), [abv2]"=w"(abv2), [abv3]"=w"(abv3),
    [kc1]"=r"(kc1), [kc2]"=r"(kc2)
    : "[A]"(A), "[B]"(B), "[av]"(av), "[bv]"(bv),
    "[abv0]"(abv0), "[abv1]"(abv1), "[abv2]"(abv2), "[abv3]"(abv3),
    "[kc1]"(kc1), "[kc2]"(kc2)
    );
  
  vst1q_f32(AB_ + 0, abv0);
  vst1q_f32(AB_ + 4, abv1);
  vst1q_f32(AB_ + 8, abv2);
  vst1q_f32(AB_ + 12, abv3);
  
  if (beta == 0.0) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] = 0.0;
      }
    }
  } else if (beta != 1.0) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] *= beta;
      }
    }
  }
  
  for (j = 0; j < NR; ++j) {
    for (i = 0; i < MR; ++i) {
      C[i*cl + j*ci] += s->AB_[i + j*MR];
    }
  }
}


#else
void dgemm_micro_kernel(Gemmer* s, int kc, float alpha, const float *A, const float *B, float beta, float *C, int cl,
                        int ci) {
  int i = 0;
  int j = 0;
  int l = 0;
  for (l = 0; l < MR*NR; ++l) {
    s->AB_[l] = 0;
  }
  for (l = 0; l < kc; ++l) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        s->AB_[i + j*MR] += A[i]*B[j];
      }
    }
    A += MR;
    B += NR;
  }
  
  if (equal_f32(beta, 0.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] = 0.0;
      }
    }
  } else if (!equal_f32(beta, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] *= beta;
      }
    }
  }
  
  if (!equal_f32(alpha, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] += alpha*s->AB_[i + j*MR];
      }
    }
  } else {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i*cl + j*ci] += s->AB_[i + j*MR];
      }
    }
  }
}
#endif


void dgeaxpy(int m, int n, float alpha, const float *X, int xl, int xi, float *Y, int yl,
             int yi) {
  int i, j;
  if (!equal_f32(alpha, 1.0)) {
    for (j = 0; j < n; ++j) {
      for (i = 0; i < m; ++i) {
        Y[i*yl + j*yi] += alpha*X[i*xl + j*xi];
      }
    }
  } else {
    for (j = 0; j < n; ++j) {
      for (i = 0; i < m; ++i) {
        Y[i*yl + j*yi] += X[i*xl + j*xi];
      }
    }
  }
}

void dgescal(int m, int n, float alpha, float *X, int xl, int xi) {
  int i, j;
  if (!equal_f32(alpha, 0.0)) {
    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        X[i*xl + j*xi] *= alpha;
      }
    }
  } else {
    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        X[i*xl + j*xi] = 0.0;
      }
    }
  }
}

void dgemm_macro_kernel(Gemmer* s, int mc, int nc, int kc, float alpha, float beta, float *C, int cl, int ci) {
  int mp = (mc + MR - 1) / MR;
  int np = (nc + NR - 1) / NR;
  
  int _mr = mc % MR;
  int _nr = nc % NR;
  
  int i, j;
  
  for (j = 0; j < np; ++j) {
    int nr = (j != np - 1 || _nr == 0) ? NR : _nr;
    
    for (i = 0; i < mp; ++i) {
      int mr = (i != mp - 1 || _mr == 0) ? MR : _mr;
      
      if ((C+4)==(&C[i*MR*cl + j*NR*ci])) {
        int asdf=0;
      }
      if (mr == MR && nr == NR) {
        dgemm_micro_kernel(s, kc, alpha, &s->A_[i*kc*MR], &s->B_[j*kc*NR], beta, &C[i*MR*cl + j*NR*ci], cl, ci);
      } else {
        dgemm_micro_kernel(s, kc, alpha, &s->A_[i*kc*MR], &s->B_[j*kc*NR], 0.0, s->C_, 1, MR);
        dgescal(mr, nr, beta, &C[i*MR*cl + j*NR], cl, ci);
        dgeaxpy(mr, nr, 1.0, s->C_, 1, MR, &C[i*MR*cl + j*NR*ci], cl, ci);
      }
    }
  }
}

void dgemm_nn(Gemmer* s, int m, int n, int k, float alpha, const float *A, int al, int ai, const float *B, int bl, int bi, float beta, float *C, int cl, int ci) {
  int mb = (m + MC - 1) / MC;
  int nb = (n + NC - 1) / NC;
  int kb = (k + KC - 1) / KC;
  
  int _mc = m % MC;
  int _nc = n % NC;
  int _kc = k % KC;
  
  int mc, nc, kc;
  int i, j, l;
  
  float _beta;
  
  if (equal_f32(alpha, 0.0) ||  k == 0) {
    dgescal(m, n, beta, C, cl, ci);
    return;
  }
  
  for (j = 0; j < nb; ++j) {
    nc = (j != nb - 1 || _nc == 0) ? NC : _nc;
    
    for (l = 0; l < kb; ++l) {
      kc = (l != kb - 1 || _kc == 0) ? KC : _kc;
      _beta = (l == 0) ? beta : 1.0;
      
      pack_B(kc, nc, &B[l*KC*bl + j*NC*bi], bl, bi, s->B_);
      
      for (i = 0; i < mb; ++i) {
        mc = (i != mb - 1 || _mc == 0) ? MC : _mc;
        
        pack_A(mc, kc, &A[i*MC*al + l*KC*ai], al, ai, s->A_);
        dgemm_macro_kernel(s, mc, nc, kc, alpha, _beta, &C[i*MC*cl + j*NC*ci], cl, ci);
      }
    }
  }
}

float fdist(int n, const float* a, const float* b) {
  float sum = 0;
  for (int i=0; i<n; ++i) {
    float t = fabs(a[i] - b[i]);
    if (t>0.0001) {
      int asdf=0;
    }
    sum += t;
  }
  return sum;
}
Gemmer g_gemmer[1] = {0};
void cc_sgemm(CBLAS_ORDER Order, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, int M, int N, int K,
              float alpha, const float *A, int lda, const float *B, int ldb, float beta, float *C, int ldc) {
  int al=lda, ai=1;
  int bl=ldb, bi=1;
  int cl=ldc, ci=1;
  if (CblasTrans==TransA) { T_SWAP(int, al, ai); }
  if (CblasTrans==TransB) { T_SWAP(int, bl, bi); }
  //float c0 = C[0], c1 = C[1];
  float* C0 = (float*)memdup(C, M*N*sizeof(float));
  dgemm_nn(g_gemmer, M, N, K, alpha, A, al, ai, B, bl, bi, beta, C, cl, ci);
  //ASSERT(c0 == C[0] && c1 == C[1]);
  float dd = fdist(M*N, C0, C);
  if (dd>0.0001) {
    int adsf=0;
  }
  free(C0);
  //int adsf=0;
  return ;
}

#undef MC  384
#undef KC  384
#undef NC  4096
#undef MR  4
#undef NR  4

