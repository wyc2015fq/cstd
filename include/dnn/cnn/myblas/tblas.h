static void TBLAS(set)(int N, const FTYPE alpha, FTYPE* Y) {
  int i = 0;
  if (alpha == 0) {
    memset(Y, 0, sizeof(FTYPE) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (; i < N-3; i+=4) {
    Y[i+0] = alpha;
    Y[i+1] = alpha;
    Y[i+2] = alpha;
    Y[i+3] = alpha;
  }
  for (; i < N; ++i) {
    Y[i] = alpha;
  }
}
static void TBLAS(pow)(int n, const FTYPE* a, const FTYPE b, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { pow(a[i], b); }
}
static void TBLAS(sqr)(int n, const FTYPE* a, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] * a[i]; }
}
static void TBLAS(sqrt)(int n, const FTYPE* a, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = sqrt(a[i]); }
}
static void TBLAS(exp)(int n, const FTYPE* a, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = exp(a[i]); }
}
static void TBLAS(log)(int n, const FTYPE* a, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = log(a[i]); }
}
static void TBLAS(abs)(int n, const FTYPE* a, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = fabs(a[i]); }
}
static void TBLAS(add)(int n, const FTYPE* a, const FTYPE* b, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] + b[i]; }
}
static void TBLAS(sub)(int n, const FTYPE* a, const FTYPE* b, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] - b[i]; }
}
static void TBLAS(mul)(int n, const FTYPE* a, const FTYPE* b, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] * b[i]; }
}
static void TBLAS(div)(int n, const FTYPE* a, const FTYPE* b, FTYPE* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] / b[i]; }
}
static FTYPE TBLAS(dot)(int n, const FTYPE *x, int incx, const FTYPE *y, int incy) {
  FTYPE sum=0;
  int i=0;
  for (; i<n; ++i) {
    sum += x[i] * y[i];
  }
  return sum;
}
static FTYPE TBLAS(asum)(int n, const FTYPE *x, int incx) {
  FTYPE sum=0;
  int i=0;
  for (; i<n; ++i) {
    sum += x[i];
  }
  return sum;
}
static double TBLAS(euclidean)(int n, const FTYPE* a, const FTYPE* b) {
  double sum = 0;
  int i;
  for (i=0; i<n; ++i) {
    FTYPE t = a[i] - b[i];
    sum += t*t;
  }
  return sum;
}
static FTYPE TBLAS(min)(int n, const FTYPE* a) {
  int i;
  FTYPE _min;
  if (n<=0) return 0;
  _min = a[0];
  for (i=1; i<n; ++i) {
    if (a[i]<_min) {
      _min = a[i];
    }
  }
  return _min;
}
static int TBLAS(argmin)(int n, const FTYPE* a) {
  int i;
  int _min;
  if (n<=0) return -1;
  _min = 0;
  for (i=1; i<n; ++i) {
    if (a[i]<a[_min]) {
      _min = i;
    }
  }
  return _min;
}
static FTYPE TBLAS(max)(int n, const FTYPE* a) {
  int i;
  FTYPE _max;
  if (n<=0) return 0;
  _max = a[0];
  for (i=1; i<n; ++i) {
    if (a[i]>_max) {
      _max = a[i];
    }
  }
  return _max;
}
static int TBLAS(argmax)(int n, const FTYPE* a) {
  int i;
  int _max;
  if (n<=0) return -1;
  _max = 0;
  for (i=1; i<n; ++i) {
    if (a[i]>a[_max]) {
      _max = i;
    }
  }
  return _max;
}
static void TBLAS(minmax)(int n, const FTYPE* a, FTYPE* _min, FTYPE* _max) {
  int i;
  if (n>0) {
    *_min = *_max = *a;
    for (i=1; i<n; ++i) {
      if (a[i]<*_min) {
        *_min = a[i];
      } else if (a[i]>*_max) {
        *_max = a[i];
      }
    }
  }
  return ;
}
static void TBLAS(scal)(int N, FTYPE alpha, FTYPE *X, int incX) {
  int i = 0;
  for (; i<N; ++i) {
    X[i*incX] *= alpha;
  }
}
static void TBLAS(axpy)(int n, FTYPE alpha, const FTYPE *x, int incx, FTYPE *y, int incy) {
  int i = 0;
#pragma omp parallel for
  for (; i<n-3; i+=4) {
    y[(i+0)*incy] += alpha * x[(i+0)*incx];
    y[(i+1)*incy] += alpha * x[(i+1)*incx];
    y[(i+2)*incy] += alpha * x[(i+2)*incx];
    y[(i+3)*incy] += alpha * x[(i+3)*incx];
  }
  for (; i<n; ++i) {
    y[i*incy] += alpha * x[i*incx];
  }
}
static void TBLAS(axpby)(int N, FTYPE alpha, const FTYPE* X, int incX, FTYPE beta, FTYPE* Y, int incY) {
  if (0) {
    TBLAS(scal)(N, beta, Y, incY);
    TBLAS(axpy)(N, alpha, X, incX, Y, incY);
  } else {
    int i=0;
#pragma omp parallel for
    for (; i<N-3; i+=4) {
      Y[(i+0)*incY] = alpha * X[(i+0)*incX] + beta * Y[(i+0)*incY];
      Y[(i+1)*incY] = alpha * X[(i+1)*incX] + beta * Y[(i+1)*incY];
      Y[(i+2)*incY] = alpha * X[(i+2)*incX] + beta * Y[(i+2)*incY];
      Y[(i+3)*incY] = alpha * X[(i+3)*incX] + beta * Y[(i+3)*incY];
    }
    for (; i<N; ++i) {
      Y[i*incY] = alpha * X[i*incX] + beta * Y[i*incY];
    }
  }
}
// y = alpha * A * x + beta
static void TBLAS(gemv)(enum CBLAS_ORDER order, enum CBLAS_TRANSPOSE trans, int M, int N,
		 FTYPE alpha, const FTYPE *A, int lda, const FTYPE *X, int incx, FTYPE beta, FTYPE *Y, int incy) {
  int i, j;
  if (CblasColMajor==order) {
    ASSERT(0);
  }
  if (CblasTrans==trans) {
    for (j=0; j<N; ++j) {
      Y[j*incy] *= beta;
    }
    for (i=0; i<M; ++i) {
      FTYPE x = alpha*X[i*incx];
      for (j=0; j<N; ++j) {
        Y[j*incy] += A[i*lda+j]*x;
      }
    }
  } else {
    for (i=0; i<M; ++i) {
      FTYPE sum = Y[i*incy]*beta;
      for (j=0; j<N; ++j) {
        sum += A[i*lda+j]*X[j*incx];
      }
      Y[i*incy] = sum;
    }
  }
  return ;
}

static void TBLAS(geaxpy)(int m, int n, FTYPE alpha, const FTYPE *X, int incRowX, int incColX, FTYPE *Y, int incRowY, int incColY) {
  int i, j;
  if ((incRowX*incRowY)>(incColX*incColY)) {
    T_SWAP(int, m, n);
    T_SWAP(int, incRowX, incColX);
    T_SWAP(int, incRowY, incColY);
  }
  if (!fequal(alpha, 1.0)) {
    for (j = 0; j < n; ++j) {
      for (i = 0; i < m; ++i) {
        Y[i * incRowY + j * incColY] += alpha * X[i * incRowX + j * incColX];
      }
    }
  } else {
    for (j = 0; j < n; ++j) {
      for (i = 0; i < m; ++i) {
        Y[i * incRowY + j * incColY] += X[i * incRowX + j * incColX];
      }
    }
  }
}
static void TBLAS(gescal)(int m, int n, FTYPE alpha, FTYPE *X, int incRowX, int incColX) {
  int i, j;
  if (incRowX>incColX) {
    T_SWAP(int, m, n);
    T_SWAP(int, incRowX, incColX);
  }
  if (!fequal(alpha, 0.0f)) {
    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        X[i * incRowX + j * incColX] *= alpha;
      }
    }
  } else {
    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        X[i * incRowX + j * incColX] = 0.0;
      }
    }
  }
}

static FTYPE* TBLAS(pack)(int mc, int kc, const FTYPE *A, int incRowA, int incColA, FTYPE *buffer, int MR) {
  int mp = mc / MR;
  int _mr = mc % MR;
  int i, j;
  int a1 = incRowA, a2 = 2 * incRowA, a3 = 3 * incRowA;
  
  for (i = 0; i < mp; ++i) {
    const FTYPE* A0 = A;
    for (j = 0; j < kc; ++j) {
      buffer[0] = A0[0];
      buffer[1] = A0[a1];
      buffer[2] = A0[a2];
      buffer[3] = A0[a3];
      A0 += incColA;
      buffer += MR;
    }
    A += MR * incRowA;
  }
  if (_mr > 0) {
    for (j = 0; j < kc; ++j) {
      for (i = 0; i < _mr; ++i) {
        buffer[i] = A[i * incRowA];
      }
      for (; i < MR; ++i) {
        buffer[i] = 0.0;
      }
      A += incColA;
      buffer += MR;
    }
  }
  return buffer;
}

static void TBLAS(gemm_kernel_4x4)(int kc, FTYPE alpha, const FTYPE *A, const FTYPE *B, FTYPE beta, FTYPE *C, int incRowC, int incColC) {
  int i, j, l = 0;
  enum { MR = 4, NR = 4};
  FTYPE AB_[MR * NR];
#if defined(MDL_V7) && !ISDOUBLE
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
  
  if (fequal(beta, 0.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] = 0.0;
      }
    }
  } else if (!fequal(beta, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] *= beta;
      }
    }
  }
  
  if (!fequal(alpha, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] += alpha * AB_[i + j * MR];
      }
    }
  } else {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] += AB_[i + j * MR];
      }
    }
  }
  
#elif defined(MDL_V8) && !ISDOUBLE
  
  
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
        C[i * incRowC + j * incColC] = 0.0;
      }
    }
  } else if (beta != 1.0) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] *= beta;
      }
    }
  }
  
  for (j = 0; j < NR; ++j) {
    for (i = 0; i < MR; ++i) {
      C[i * incRowC + j * incColC] += AB_[i + j * MR];
    }
  }
  
  
#elif CC_SSE2 && !ISDOUBLE
  
  __m128 abv0 = _mm_set_ps1(0);
  __m128 abv1 = _mm_set_ps1(0);
  __m128 abv2 = _mm_set_ps1(0);
  __m128 abv3 = _mm_set_ps1(0);
  __m128 av;
  
  for (; l < kc; ++l) {
    av = _mm_loadu_ps(A);
    _mm_muladd_ps(abv0, av, _mm_set_ps1(B[0]));
    _mm_muladd_ps(abv1, av, _mm_set_ps1(B[1]));
    _mm_muladd_ps(abv2, av, _mm_set_ps1(B[2]));
    _mm_muladd_ps(abv3, av, _mm_set_ps1(B[3]));
    A += MR;
    B += NR;
  }
  
  av = _mm_set_ps1(alpha);
  _mm_storeu_ps(AB_ +  0, _mm_mul_ps(abv0, av));
  _mm_storeu_ps(AB_ +  4, _mm_mul_ps(abv1, av));
  _mm_storeu_ps(AB_ +  8, _mm_mul_ps(abv2, av));
  _mm_storeu_ps(AB_ + 12, _mm_mul_ps(abv3, av));

  if (fequal(beta, 0.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] = AB_[i + j * MR];
      }
    }
  } else if (!fequal(beta, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] = C[i * incRowC + j * incColC] * beta + AB_[i + j * MR];
      }
    }
  } else {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] += AB_[i + j * MR];
      }
    }
  }
#else
  
  for (l = 0; l < MR * NR; ++l) {
    AB_[l] = 0;
  }
  for (l = 0; l < kc; ++l) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        AB_[i + j * MR] += A[i] * B[j];
      }
    }
    A += MR;
    B += NR;
  }
  if (fequal(beta, 0.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] = 0.0;
      }
    }
  } else if (!fequal(beta, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] *= beta;
      }
    }
  }
  
  if (!fequal(alpha, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] += alpha * AB_[i + j * MR];
      }
    }
  } else {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] += AB_[i + j * MR];
      }
    }
  }
#endif
}

static void TBLAS(gemm_nn)(int m, int n, int k, FTYPE alpha, const FTYPE *A, int incRowA, int incColA,
              const FTYPE *B, int incRowB, int incColB, FTYPE beta, FTYPE *C, int incRowC, int incColC, int MC, int NC, int KC, FTYPE* A_, FTYPE* B_) {
  int mb = (m + MC - 1) / MC;
  int nb = (n + NC - 1) / NC;
  int kb = (k + KC - 1) / KC;
  
  int _mc = m % MC;
  int _nc = n % NC;
  int _kc = k % KC;
  
  int i, j, l;
  enum { MR = 4, NR = 4};
  FTYPE C_[MR * NR];
  FTYPE _beta;
  
  if (fequal(alpha, 0.0) ||  k == 0) {
    TBLAS(gescal)(m, n, beta, C, incRowC, incColC);
    return;
  }
  
  for (j = 0; j < nb; ++j) {
    int nc = (j != nb - 1 || _nc == 0) ? NC : _nc;
    
    for (l = 0; l < kb; ++l) {
      int kc = (l != kb - 1 || _kc == 0) ? KC : _kc;
      _beta = (l == 0) ? beta : 1.0;
      
      TBLAS(pack)(nc, kc, &B[l * KC * incRowB + j * NC * incColB], incColB, incRowB, B_, NR);
      
      for (i = 0; i < mb; ++i) {
        int mc = (i != mb - 1 || _mc == 0) ? MC : _mc;
        FTYPE* C0 = &C[i * MC * incRowC + j * NC * incColC];
        int mm, nn;
        int mp = (mc + MR - 1) / MR;
        int np = (nc + NR - 1) / NR;
        int _mr = mc % MR;
        int _nr = nc % NR;
        TBLAS(pack)(mc, kc, &A[i * MC * incRowA + l * KC * incColA], incRowA, incColA, A_, MR);
        
        for (nn = 0; nn < np; ++nn) {
          int nr = (nn != np - 1 || _nr == 0) ? NR : _nr;
          
          for (mm = 0; mm < mp; ++mm) {
            int mr = (mm != mp - 1 || _mr == 0) ? MR : _mr;
            
            if (mr == MR && nr == NR) {
              TBLAS(gemm_kernel_4x4)(kc, alpha, &A_[mm * kc * MR], &B_[nn * kc * NR], _beta, &C0[mm * MR * incRowC + nn * NR * incColC], incRowC, incColC);
            } else {
              TBLAS(gemm_kernel_4x4)(kc, alpha, &A_[mm * kc * MR], &B_[nn * kc * NR], 0.0, C_, 1, MR);
              TBLAS(gescal)(mr, nr, _beta, &C0[mm * MR * incRowC + nn * NR * incColC], incRowC, incColC);
              TBLAS(geaxpy)(mr, nr, 1.0, C_, 1, MR, &C0[mm * MR * incRowC + nn * NR * incColC], incRowC, incColC);
            }
          }
        }
      }
    }
  }
}

static void TBLAS(gemm)(CBLAS_ORDER order, const CBLAS_TRANSPOSE TransA, const CBLAS_TRANSPOSE TransB, int M, int N, int K,
                       FTYPE alpha, const FTYPE* A, int lda, const FTYPE* B, int ldb, FTYPE beta, FTYPE* C, int ldc) {
  int MC = 384, KC = 384, NC = 4096;
  FTYPE* fast_sgemm_buf = NULL;
  FTYPE *A_, *B_;
  int ida=1, idb=1, idc=1;
  if (CblasTrans==TransA) {
    SWAP_T(lda, ida, int);
  }
  if (CblasTrans==TransB) {
    SWAP_T(ldb, idb, int);
  }
  if (CblasColMajor==order) {
    SWAP_T(ldc, idc, int);
  }
  
  if (NULL==fast_sgemm_buf) {
    int size = MC * KC + KC * NC;
    MYREALLOC(fast_sgemm_buf, MC * KC + KC * NC);
  }
  
  A_ = fast_sgemm_buf;
  B_ = fast_sgemm_buf + MC * KC;
  TBLAS(gemm_nn)(M, N, K, alpha, A, lda, ida, B, ldb, idb, beta, C, ldc, idc, MC, NC, KC, A_, B_);
  FREE(fast_sgemm_buf);
}

static void TBLAS(gemm1)(CBLAS_ORDER order, const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, int M, int N, int K,
    FTYPE alpha, const FTYPE* A, int lda, const FTYPE* B, int ldb, FTYPE beta,
    FTYPE* C, int ldc) {
  int i, j, k, ida=1, idb=1, idc=1;
  if (CblasTrans==TransA) {
    SWAP_T(lda, ida, int);
  }
  if (CblasTrans==TransB) {
    SWAP_T(ldb, idb, int);
  }
  if (CblasColMajor==order) {
    SWAP_T(ldc, idc, int);
  }
  for (i=0; i<M; ++i) {
    for (j=0; j<N; ++j) {
      FTYPE sum = 0;
      for (k=0; k<K; ++k) {
        sum += A[i*lda+k*ida]*B[k*ldb+j*idb];
      }
      C[i*ldc+j*idc] = alpha*sum + beta*C[i*ldc+j*idc];
    }
  }
  return ;
}
#undef FTYPE
#undef TBLAS
#undef ISDOUBLE

