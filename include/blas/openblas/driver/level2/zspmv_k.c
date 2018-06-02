

#include <ctype.h>


int CNAME(BLASLONG m, FLOAT alpha_r, FLOAT alpha_i,
	  FLOAT *a, FLOAT *x, BLASLONG incx, FLOAT *y, BLASLONG incy, void *buffer){

  BLASLONG i;
  FLOAT *X = x;
  FLOAT *Y = y;
  FLOAT *gemvbuffer = (FLOAT *)buffer;
  FLOAT *bufferY    = gemvbuffer;
  FLOAT *bufferX    = gemvbuffer;

  OPENBLAS_COMPLEX_FLOAT result;

  if (incy != 1) {
    Y = bufferY;
    bufferX    = (FLOAT *)(((BLASLONG)bufferY + m * sizeof(FLOAT) * 2 + 4095) & ~4095);
    gemvbuffer = bufferX;
    COPY_K(m, y, incy, Y, 1);
  }

  if (incx != 1) {
    X = bufferX;
    gemvbuffer = (FLOAT *)(((BLASLONG)bufferX + m * sizeof(FLOAT) * 2 + 4095) & ~4095);
    COPY_K(m, x, incx, X, 1);
  }

  for (i = 0; i < m; i++) {
#ifndef LOWER

    if (i > 0) {
      result = DOTU_K(i, a, 1, X, 1);

      Y[i * 2 + 0] += alpha_r * CREAL(result) - alpha_i * CIMAG(result);
      Y[i * 2 + 1] += alpha_r * CIMAG(result) + alpha_i * CREAL(result);
    }

    AXPYU_K(i + 1, 0, 0,
	   alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	   alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	   a, 1, Y, 1, NULL, 0);

    a += (i + 1) * 2;

#else

    result = DOTU_K(m - i, a + i * 2, 1, X + i * 2, 1);

    Y[i * 2 + 0] += alpha_r * CREAL(result) - alpha_i * CIMAG(result);
    Y[i * 2 + 1] += alpha_r * CIMAG(result) + alpha_i * CREAL(result);

    if (m - i > 1)
      AXPYU_K(m - i - 1, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	      a + (i + 1) * 2, 1, Y + (i + 1) * 2, 1, NULL, 0);

    a += (m - i - 1) * 2;

#endif
  }

  if (incy != 1) {
    COPY_K(m, Y, 1, y, incy);
  }

  return 0;
}

