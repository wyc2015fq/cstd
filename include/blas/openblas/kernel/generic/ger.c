



int CNAME(BLASLONG m, BLASLONG n, BLASLONG dummy1, FLOAT alpha,
	 FLOAT *x, BLASLONG incx,
	 FLOAT *y, BLASLONG incy,
	 FLOAT *a, BLASLONG lda, FLOAT *buffer){

  FLOAT *X    = x;

  if (incx != 1) {
    X = buffer;
    COPY_K(m, x, incx, X, 1);
  }

  while (n > 0) {
    AXPYU_K(m, 0, 0, alpha * *y, X, 1, a, 1, NULL, 0);
    a += lda;
    y += incy;
    n --;
  }

  return 0;
}

