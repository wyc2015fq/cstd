
static Dtype* FUN(pack)(int mc, int kc, const Dtype *A, int incRowA, int incColA, Dtype *buffer, int MR) {
  int mp = mc / MR;
  int _mr = mc % MR;
  int i, j;
  int a1 = incRowA, a2 = 2 * incRowA, a3 = 3 * incRowA;

  for (i = 0; i < mp; ++i) {
    const Dtype* A0 = A;
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

static void FUN(gemm_kernel_4x4)(int kc, const Dtype* alpha, const Dtype *A, const Dtype *B, Dtype beta, Dtype *C, int incRowC, int incColC) {
  int i, j, l = 0;
  enum { MR = 4, NR = 4};
  Dtype a = *alpha;
  Dtype AB_[MR * NR] = {0};
#if defined(__ARM_V8) && !ISDOUBLE
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
  l = 1;
#endif

#if defined(__ARM_V7) && !ISDOUBLE
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
  l = 1;
#endif


#if defined(__SSE2) && !ISDOUBLE

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
  l = 1;
#endif

  if (l==0) {
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

  if (!fequal(a, 1.0)) {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] += a * AB_[i + j * MR];
      }
    }
  } else {
    for (j = 0; j < NR; ++j) {
      for (i = 0; i < MR; ++i) {
        C[i * incRowC + j * incColC] += AB_[i + j * MR];
      }
    }
  }
}

static int FUN(gemm_nn)(int m, int n, int k, const Dtype* alpha, const Dtype *A, int incRowA, int incColA,
              const Dtype *B, int incRowB, int incColB, const Dtype* beta, Dtype *C, int incRowC, int incColC, int MC, int NC, int KC, Dtype* A_, Dtype* B_) {
	Dtype a = *alpha;
	Dtype b = *beta;
  int mb = (m + MC - 1) / MC;
  int nb = (n + NC - 1) / NC;
  int kb = (k + KC - 1) / KC;

  int _mc = m % MC;
  int _nc = n % NC;
  int _kc = k % KC;

  int i, j, l;
  enum { MR = 4, NR = 4};
  Dtype C_[MR * NR];
  Dtype _beta;
  Dtype alpha1 = 1.0;

  if (fequal(a, 0.0) ||  k == 0) {
    FUN(gescal)(m, n, beta, C, incRowC, incColC);
    return 0;
  }

  for (j = 0; j < nb; ++j) {
    int nc = (j != nb - 1 || _nc == 0) ? NC : _nc;

    for (l = 0; l < kb; ++l) {
      int kc = (l != kb - 1 || _kc == 0) ? KC : _kc;
      _beta = (Dtype)((l == 0) ? b : 1.0);

      FUN(pack)(nc, kc, &B[l * KC * incRowB + j * NC * incColB], incColB, incRowB, B_, NR);

      for (i = 0; i < mb; ++i) {
        int mc = (i != mb - 1 || _mc == 0) ? MC : _mc;
        Dtype* C0 = &C[i * MC * incRowC + j * NC * incColC];
        int mm, nn;
        int mp = (mc + MR - 1) / MR;
        int np = (nc + NR - 1) / NR;
        int _mr = mc % MR;
        int _nr = nc % NR;
        FUN(pack)(mc, kc, &A[i * MC * incRowA + l * KC * incColA], incRowA, incColA, A_, MR);

        for (nn = 0; nn < np; ++nn) {
          int nr = (nn != np - 1 || _nr == 0) ? NR : _nr;

          for (mm = 0; mm < mp; ++mm) {
            int mr = (mm != mp - 1 || _mr == 0) ? MR : _mr;

            if (mr == MR && nr == NR) {
              FUN(gemm_kernel_4x4)(kc, alpha, &A_[mm * kc * MR], &B_[nn * kc * NR], _beta, &C0[mm * MR * incRowC + nn * NR * incColC], incRowC, incColC);
            } else {
              FUN(gemm_kernel_4x4)(kc, alpha, &A_[mm * kc * MR], &B_[nn * kc * NR], 0.0, C_, 1, MR);
              FUN(gescal)(mr, nr, &_beta, &C0[mm * MR * incRowC + nn * NR * incColC], incRowC, incColC);
              FUN(geaxpy)(mr, nr, &alpha1, C_, 1, MR, &C0[mm * MR * incRowC + nn * NR * incColC], incRowC, incColC);
            }
          }
        }
      }
    }
  }
  return 0;
}

static void FUN(gemm)(CBLAS_ORDER order, const CBLAS_TRANSPOSE TransA, const CBLAS_TRANSPOSE TransB, int M, int N, int K,
                       Dtype alpha, const Dtype* A, int lda, const Dtype* B, int ldb, Dtype beta, Dtype* C, int ldc) {
  int MC = 384, KC = 384, NC = 4096;
  Dtype* fast_sgemm_buf = NULL;
  Dtype *A_, *B_;
  int ida=1, idb=1, idc=1;
  if (CblasTrans==TransA) {
    T_SWAP(int, lda, ida);
  }
  if (CblasTrans==TransB) {
    T_SWAP(int, ldb, idb);
  }
  if (CblasColMajor==order) {
    T_SWAP(int, ldc, idc);
  }

  if (NULL==fast_sgemm_buf) {
    int size = MC * KC + KC * NC;
	Dtype zero = 0;
	fast_sgemm_buf = (Dtype*)malloc(size*sizeof(Dtype));
    FUN(set)(size, &zero, fast_sgemm_buf);
  }

  A_ = fast_sgemm_buf;
  B_ = fast_sgemm_buf + MC * KC;
  FUN(gemm_nn)(M, N, K, &alpha, A, lda, ida, B, ldb, idb, &beta, C, ldc, idc, MC, NC, KC, A_, B_);
  if (fast_sgemm_buf) {
	  free(fast_sgemm_buf);
  }
}

static void FUN(gemm1)(CBLAS_ORDER order, const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, int M, int N, int K,
    Dtype alpha, const Dtype* A, int lda, const Dtype* B, int ldb, Dtype beta,
    Dtype* C, int ldc) {
  int i, j, k, ida=1, idb=1, idc=1;
  if (CblasTrans==TransA) {
    T_SWAP(int, lda, ida);
  }
  if (CblasTrans==TransB) {
    T_SWAP(int, ldb, idb);
  }
  if (CblasColMajor==order) {
    T_SWAP(int, ldc, idc);
  }
  for (i=0; i<M; ++i) {
    for (j=0; j<N; ++j) {
      Dtype sum = 0;
      for (k=0; k<K; ++k) {
        sum += A[i*lda+k*ida]*B[k*ldb+j*idb];
      }
      C[i*ldc+j*idc] = alpha*sum + beta*C[i*ldc+j*idc];
    }
  }
  return ;
}

