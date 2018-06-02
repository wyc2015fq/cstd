

#include <ctype.h>


int CNAME(BLASLONG n, BLASLONG k, FLOAT alpha_r, FLOAT alpha_i,
	  FLOAT *a, BLASLONG lda,
	  FLOAT *x, BLASLONG incx, FLOAT *y, BLASLONG incy, void *buffer){

  BLASLONG i, length;
#ifndef LOWER
  BLASLONG offset;
#endif

  FLOAT *X = x;
  FLOAT *Y = y;
  FLOAT *sbmvbuffer = (FLOAT *)buffer;
  FLOAT *bufferY    = sbmvbuffer;
  FLOAT *bufferX    = sbmvbuffer;
  FLOAT temp[2];

  OPENBLAS_COMPLEX_FLOAT result;

  if (incy != 1) {
    Y = bufferY;
    bufferX    = (FLOAT *)(((BLASLONG)bufferY + n * sizeof(FLOAT) * COMPSIZE + 4095) & ~4095);
    sbmvbuffer = bufferX;
    COPY_K(n, y, incy, Y, 1);
  }

  if (incx != 1) {
    X = bufferX;
    sbmvbuffer = (FLOAT *)(((BLASLONG)bufferX + n * sizeof(FLOAT) * COMPSIZE + 4095) & ~4095);
    COPY_K(n, x, incx, X, 1);
  }

#ifndef LOWER
  offset = k;
#endif

  for (i = 0; i < n; i++) {

#ifndef HEMVREV
#ifndef LOWER
    length  = k - offset;

    if (length > 0) {
      AXPYU_K(length, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	      a + offset * COMPSIZE, 1, Y + (i - length) * COMPSIZE, 1, NULL, 0);
    }

    temp[0] = a[k * 2 + 0] * X[i * 2 + 0];
    temp[1] = a[k * 2 + 0] * X[i * 2 + 1];

    Y[i * 2 + 0] +=  alpha_r * temp[0] - alpha_i * temp[1];
    Y[i * 2 + 1] +=  alpha_r * temp[1] + alpha_i * temp[0];

    if (length > 0) {
      result = DOTC_K(length, a + offset * COMPSIZE, 1, X + (i - length) * COMPSIZE, 1);

      Y[i * 2 + 0] += alpha_r * CREAL(result) - alpha_i * CIMAG(result);
      Y[i * 2 + 1] += alpha_r * CIMAG(result) + alpha_i * CREAL(result);
      }

    if (offset > 0) offset --;
#else
    length  = k;
    if (n - i - 1 < k) length = n - i - 1;

    if (length > 0) {
      AXPYU_K(length, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	      a + COMPSIZE, 1, Y + (i + 1) * COMPSIZE, 1, NULL, 0);
    }

    temp[0] = a[0] * X[i * 2 + 0];
    temp[1] = a[0] * X[i * 2 + 1];

    Y[i * 2 + 0] +=  alpha_r * temp[0] - alpha_i * temp[1];
    Y[i * 2 + 1] +=  alpha_r * temp[1] + alpha_i * temp[0];

    if (length > 0) {
      result = DOTC_K(length, a + COMPSIZE, 1, X + (i + 1) * COMPSIZE, 1);

      Y[i * 2 + 0] += alpha_r * CREAL(result) - alpha_i * CIMAG(result);
      Y[i * 2 + 1] += alpha_r * CIMAG(result) + alpha_i * CREAL(result);
      }
#endif
#else
#ifndef LOWER

    length  = k - offset;

    if (length > 0) {
      AXPYC_K(length, 0, 0,
	       alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	       alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	      a + offset * COMPSIZE, 1, Y + (i - length) * COMPSIZE, 1, NULL, 0);
    }

    temp[0] = a[k * 2 + 0] * X[i * 2 + 0];
    temp[1] = a[k * 2 + 0] * X[i * 2 + 1];

    Y[i * 2 + 0] +=  alpha_r * temp[0] - alpha_i * temp[1];
    Y[i * 2 + 1] +=  alpha_r * temp[1] + alpha_i * temp[0];

    if (length > 0) {
      result = DOTU_K(length, a + offset * COMPSIZE, 1, X + (i - length) * COMPSIZE, 1);

      Y[i * 2 + 0] +=   alpha_r * CREAL(result) - alpha_i * CIMAG(result);
      Y[i * 2 + 1] +=  alpha_r * CIMAG(result) + alpha_i * CREAL(result);
      }

    if (offset > 0) offset --;
#else
    length  = k;
    if (n - i - 1 < k) length = n - i - 1;

    if (length > 0) {
      AXPYC_K(length, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_r * X[i * 2 + 1] + alpha_i * X[i * 2 + 0],
	      a + COMPSIZE, 1, Y + (i + 1) * COMPSIZE, 1, NULL, 0);
    }

    temp[0] = a[0] * X[i * 2 + 0];
    temp[1] = a[0] * X[i * 2 + 1];

    Y[i * 2 + 0] +=  alpha_r * temp[0] - alpha_i * temp[1];
    Y[i * 2 + 1] +=  alpha_r * temp[1] + alpha_i * temp[0];

    if (length > 0) {
      result = DOTU_K(length, a + COMPSIZE, 1, X + (i + 1) * COMPSIZE, 1);

      Y[i * 2 + 0] += alpha_r * CREAL(result) - alpha_i * CIMAG(result);
      Y[i * 2 + 1] += alpha_r * CIMAG(result) + alpha_i * CREAL(result);
      }
#endif
#endif


    a += lda * 2;
  }

  if (incy != 1) {
    COPY_K(n, Y, 1, y, incy);
  }

  return 0;
}

