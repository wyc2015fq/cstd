

#include <ctype.h>


int CNAME(BLASLONG n, BLASLONG k, FLOAT alpha,
	  FLOAT *a, BLASLONG lda,
	  FLOAT *x, BLASLONG incx, FLOAT *y, BLASLONG incy, void *buffer){

  BLASLONG i, length;

  FLOAT *X = x;
  FLOAT *Y = y;
  FLOAT *sbmvbuffer = (FLOAT *)buffer;
  FLOAT *bufferY    = sbmvbuffer;
  FLOAT *bufferX    = sbmvbuffer;

  if (incy != 1) {
    Y = bufferY;
    bufferX    = (FLOAT *)(((BLASLONG)bufferY + n * sizeof(FLOAT) + 4095) & ~4095);
    sbmvbuffer = bufferX;
    COPY_K(n, y, incy, Y, 1);
  }

  if (incx != 1) {
    X = bufferX;
    sbmvbuffer = (FLOAT *)(((BLASLONG)bufferX + n * sizeof(FLOAT) + 4095) & ~4095);
    COPY_K(n, x, incx, X, 1);
  }

  for (i = 0; i < n; i++) {

#ifndef LOWER
    length  = i;
    if (length > k) length = k;

    AXPYU_K(length + 1, 0, 0,
	   alpha * X[i],
	   a + k - length, 1, Y + i - length, 1, NULL, 0);
    Y[i] += alpha * DOTU_K(length, a + k - length, 1, X + i - length, 1);
#else
    length  = k;
    if (n - i - 1 < k) length = n - i - 1;

    AXPYU_K(length + 1, 0, 0,
	   alpha * X[i],
	   a, 1, Y + i, 1, NULL, 0);
    Y[i] += alpha * DOTU_K(length, a + 1, 1, X + i + 1, 1);
#endif

    a += lda;
  }

  if (incy != 1) {
    COPY_K(n, Y, 1, y, incy);
  }

  return 0;
}

