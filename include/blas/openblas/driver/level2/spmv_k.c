

#include <ctype.h>


int CNAME(BLASLONG m, FLOAT alpha, FLOAT *a,
	  FLOAT *x, BLASLONG incx, FLOAT *y, BLASLONG incy, void *buffer){

  BLASLONG i;
  FLOAT *X = x;
  FLOAT *Y = y;
  FLOAT *gemvbuffer = (FLOAT *)buffer;
  FLOAT *bufferY    = gemvbuffer;
  FLOAT *bufferX    = gemvbuffer;

  if (incy != 1) {
    Y = bufferY;
    bufferX    = (FLOAT *)(((BLASLONG)bufferY + m * sizeof(FLOAT) + 4095) & ~4095);
    gemvbuffer = bufferX;
    COPY_K(m, y, incy, Y, 1);
  }

  if (incx != 1) {
    X = bufferX;
    gemvbuffer = (FLOAT *)(((BLASLONG)bufferX + m * sizeof(FLOAT) + 4095) & ~4095);
    COPY_K(m, x, incx, X, 1);
  }

  for (i = 0; i < m; i++) {
#ifndef LOWER
    if (i > 0) Y[i] += alpha * DOTU_K(i, a, 1, X, 1);
    AXPYU_K(i + 1, 0, 0, alpha * X[i], a, 1, Y, 1, NULL, 0);
    a += i + 1;

#else
    Y[i] += alpha * DOTU_K(m - i, a + i, 1, X + i, 1);
    if (m - i > 1) AXPYU_K(m - i - 1, 0, 0, alpha * X[i],
			  a + i + 1, 1, Y + i + 1, 1, NULL, 0);
    a += m - i - 1;
#endif
  }

  if (incy != 1) {
    COPY_K(m, Y, 1, y, incy);
  }

  return 0;
}

