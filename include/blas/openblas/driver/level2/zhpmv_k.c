

#include <ctype.h>


int CNAME(BLASLONG m, FLOAT alpha_r, FLOAT alpha_i,
	 FLOAT *a, FLOAT *x, BLASLONG incx, FLOAT *y, BLASLONG incy, void *buffer){

  BLASLONG i;
  FLOAT *X = x;
  FLOAT *Y = y;
  FLOAT *gemvbuffer = (FLOAT *)buffer;
  FLOAT *bufferY    = gemvbuffer;
  FLOAT *bufferX    = gemvbuffer;
  FLOAT temp[2];

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

#ifndef HEMVREV
#ifndef LOWER
    if (i > 0) {
      result = DOTC_K(i, a, 1, X, 1);

      Y[i * 2 + 0] += alpha_r * CREAL(result) - alpha_i * CIMAG(result);
      Y[i * 2 + 1] += alpha_r * CIMAG(result) + alpha_i * CREAL(result);
    }

    temp[0] = a[i * 2 + 0] * X[i * 2 + 0];
    temp[1] = a[i * 2 + 0] * X[i * 2 + 1];

    Y[i * 2 + 0] +=  alpha_r * temp[0] - alpha_i * temp[1];
    Y[i * 2 + 1] +=  alpha_r * temp[1] + alpha_i * temp[0];

    if (i > 0) {
      AXPYU_K(i, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	      a, 1, Y, 1, NULL, 0);
    }

    a += (i + 1) * 2;

#else

    if (m - i > 1) {
      result = DOTC_K(m - i - 1, a + (i + 1) * 2, 1, X + (i + 1) * 2, 1);

      Y[i * 2 + 0] += alpha_r * CREAL(result) - alpha_i * CIMAG(result);
      Y[i * 2 + 1] += alpha_r * CIMAG(result) + alpha_i * CREAL(result);
    }

    temp[0] = a[i * 2 + 0] * X[i * 2 + 0];
    temp[1] = a[i * 2 + 0] * X[i * 2 + 1];

    Y[i * 2 + 0] +=  alpha_r * temp[0] - alpha_i * temp[1];
    Y[i * 2 + 1] +=  alpha_r * temp[1] + alpha_i * temp[0];

    if (m - i > 1) {
      AXPYU_K(m - i - 1, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	      a + (i + 1) * 2, 1, Y + (i + 1) * 2, 1, NULL, 0);
    }

    a += (m - i - 1) * 2;

#endif
#else
#ifndef LOWER
    if (i > 0) {
      result = DOTU_K(i, a, 1, X, 1);

      Y[i * 2 + 0] += alpha_r * CREAL(result) - alpha_i * CIMAG(result);
      Y[i * 2 + 1] += alpha_r * CIMAG(result) + alpha_i * CREAL(result);
    }

    temp[0] = a[i * 2 + 0] * X[i * 2 + 0];
    temp[1] = a[i * 2 + 0] * X[i * 2 + 1];

    Y[i * 2 + 0] +=  alpha_r * temp[0] - alpha_i * temp[1];
    Y[i * 2 + 1] +=  alpha_r * temp[1] + alpha_i * temp[0];

    if (i > 0) {
      AXPYC_K(i, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	      a, 1, Y, 1, NULL, 0);
    }

    a += (i + 1) * 2;

#else

    if (m - i > 1) {
      result = DOTU_K(m - i - 1, a + (i + 1) * 2, 1, X + (i + 1) * 2, 1);

      Y[i * 2 + 0] += alpha_r * CREAL(result) - alpha_i * CIMAG(result);
      Y[i * 2 + 1] += alpha_r * CIMAG(result) + alpha_i * CREAL(result);
    }

    temp[0] = a[i * 2 + 0] * X[i * 2 + 0];
    temp[1] = a[i * 2 + 0] * X[i * 2 + 1];

    Y[i * 2 + 0] +=  alpha_r * temp[0] - alpha_i * temp[1];
    Y[i * 2 + 1] +=  alpha_r * temp[1] + alpha_i * temp[0];

    if (m - i > 1) {
      AXPYC_K(m - i - 1, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	      a + (i + 1) * 2, 1, Y + (i + 1) * 2, 1, NULL, 0);
    }

    a += (m - i - 1) * 2;

#endif
#endif

  }

  if (incy != 1) {
    COPY_K(m, Y, 1, y, incy);
  }

  return 0;
}

