




int CNAME(BLASLONG m, BLASLONG n, BLASLONG dummy1, FLOAT alpha_r, FLOAT alpha_i,
	 FLOAT *x, BLASLONG incx,
	 FLOAT *y, BLASLONG incy,
	 FLOAT *a, BLASLONG lda, FLOAT *buffer){

  FLOAT *X    = x;

  if (incx != 1) {
    X = buffer;
    COPY_K(m, x, incx, X, 1);
  }

  lda  *= 2;
  incy *= 2;

  while (n > 0) {
    FLOAT beta_r = y[0];
    FLOAT beta_i = y[1];

#ifndef XCONJ
    AXPYU_K
#else
    AXPYC_K
#endif
      (m, 0, 0,
#ifndef CONJ
       alpha_r * beta_r - alpha_i * beta_i,
       alpha_r * beta_i + alpha_i * beta_r,
#else
       alpha_r * beta_r + alpha_i * beta_i,
       -alpha_r * beta_i + alpha_i * beta_r,
#endif
       X, 1, a, 1, NULL, 0);

    a += lda;
    y += incy;
    n --;
  }

  return 0;
}

