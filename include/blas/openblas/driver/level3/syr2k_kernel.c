



int CNAME(BLASLONG m, BLASLONG n, BLASLONG k, FLOAT alpha_r,
#ifdef COMPLEX
	   FLOAT alpha_i,
#endif
	   FLOAT *a, FLOAT *b, FLOAT *c, BLASLONG ldc, BLASLONG offset, int flag){

  BLASLONG i, j;
  BLASLONG loop;
  FLOAT subbuffer[GEMM_UNROLL_MN * GEMM_UNROLL_MN * COMPSIZE];

  if (m + offset < 0) {
#ifndef LOWER
    GEMM_KERNEL_N(m, n, k,
		alpha_r,
#ifdef COMPLEX
		alpha_i,
#endif
		a, b, c, ldc);
#endif
    return 0;
  }

  if (n < offset) {
#ifdef LOWER
    GEMM_KERNEL_N(m, n, k,
		alpha_r,
#ifdef COMPLEX
		alpha_i,
#endif
		a, b, c, ldc);
#endif
    return 0;
  }


  if (offset > 0) {
#ifdef LOWER
    GEMM_KERNEL_N(m, offset, k,
		alpha_r,
#ifdef COMPLEX
		alpha_i,
#endif
		a, b, c, ldc);
#endif
    b += offset * k   * COMPSIZE;
    c += offset * ldc * COMPSIZE;
    n -= offset;
    offset = 0;

    if (n <= 0) return 0;
  }

  if (n > m + offset) {
#ifndef LOWER
      GEMM_KERNEL_N(m, n - m - offset, k,
		  alpha_r,
#ifdef COMPLEX
		  alpha_i,
#endif
		  a,
		  b + (m + offset) * k   * COMPSIZE,
		  c + (m + offset) * ldc * COMPSIZE, ldc);
#endif

    n = m + offset;
    if (n <= 0) return 0;
  }


  if (offset < 0) {
#ifndef LOWER
    GEMM_KERNEL_N(-offset, n, k,
		alpha_r,
#ifdef COMPLEX
		alpha_i,
#endif
		a, b, c, ldc);
#endif
    a -= offset * k   * COMPSIZE;
    c -= offset       * COMPSIZE;
    m += offset;
    offset = 0;

  if (m <= 0) return 0;
  }

  if (m > n - offset) {
#ifdef LOWER
    GEMM_KERNEL_N(m - n + offset, n, k,
		alpha_r,
#ifdef COMPLEX
		alpha_i,
#endif
		a + (n - offset) * k * COMPSIZE,
		b,
		c + (n - offset)     * COMPSIZE, ldc);
#endif
    m = n + offset;
  if (m <= 0) return 0;
  }

  for (loop = 0; loop < n; loop += GEMM_UNROLL_MN) {

    int mm, nn;

    mm = (loop & ~(GEMM_UNROLL_MN - 1));
    nn = MIN(GEMM_UNROLL_MN, n - loop);

#ifndef LOWER
    GEMM_KERNEL_N(mm, nn, k,
		  alpha_r,
#ifdef COMPLEX
		  alpha_i,
#endif
		  a, b + loop * k * COMPSIZE, c + loop * ldc * COMPSIZE, ldc);
#endif

    if (flag) {
      GEMM_BETA(nn, nn, 0, ZERO,
#ifdef COMPLEX
		ZERO,
#endif
		NULL, 0, NULL, 0, subbuffer, nn);

      GEMM_KERNEL_N(nn, nn, k,
		    alpha_r,
#ifdef COMPLEX
		    alpha_i,
#endif
		    a + loop * k * COMPSIZE, b + loop * k * COMPSIZE, subbuffer, nn);

#ifndef LOWER

      for (j = 0; j < nn; j ++) {
	for (i = 0; i <= j; i ++) {
#ifndef COMPLEX
	  c[i + loop + (j + loop) * ldc] +=
	    subbuffer[i + j * nn] + subbuffer[j + i * nn];
#else
	  c[(i + loop + (j + loop) * ldc) * 2 + 0] +=
	    subbuffer[(i + j * nn) * 2 + 0] + subbuffer[(j + i * nn) * 2 + 0];
	  c[(i + loop + (j + loop) * ldc) * 2 + 1] +=
	    subbuffer[(i + j * nn) * 2 + 1] + subbuffer[(j + i * nn) * 2 + 1];
#endif
	}
      }
#else
      for (j = 0; j < nn; j ++) {
	for (i = j; i < nn; i ++) {
#ifndef COMPLEX
	  c[i + loop + (j + loop) * ldc] +=
	    subbuffer[i + j * nn] + subbuffer[j + i * nn];
#else
	  c[(i + loop + (j + loop) * ldc) * 2 + 0] +=
	    subbuffer[(i + j * nn) * 2 + 0] + subbuffer[(j + i * nn) * 2 + 0];
	  c[(i + loop + (j + loop) * ldc) * 2 + 1] +=
	    subbuffer[(i + j * nn) * 2 + 1] + subbuffer[(j + i * nn) * 2 + 1];
#endif
	  }
      }
#endif
    }

#ifdef LOWER
    GEMM_KERNEL_N(m - mm - nn, nn, k,
		  alpha_r,
#ifdef COMPLEX
		  alpha_i,
#endif
		  a + (mm + nn) * k * COMPSIZE, b + loop * k * COMPSIZE,
		  c + (mm + nn + loop * ldc) * COMPSIZE, ldc);
#endif
  }

  return 0;
}
