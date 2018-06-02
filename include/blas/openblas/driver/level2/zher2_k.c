

#include <ctype.h>


int CNAME(BLASLONG m, FLOAT alpha_r, FLOAT alpha_i,
	 FLOAT *x, BLASLONG incx,
	 FLOAT *y, BLASLONG incy, FLOAT *a, BLASLONG lda, FLOAT *buffer){

  BLASLONG i;
  FLOAT *X, *Y;

  X = x;
  Y = y;

  lda *= 2;

  if (incx != 1) {
    COPY_K(m, x, incx, buffer, 1);
    X = buffer;
  }

  if (incy != 1) {
    COPY_K(m, y, incy, (FLOAT *)((BLASLONG)buffer + (BUFFER_SIZE / 2)), 1);
    Y = (FLOAT *)((BLASLONG)buffer + (BUFFER_SIZE / 2));
  }

  for (i = 0; i < m; i++){
#ifndef HEMVREV
#ifndef LOWER
    AXPYU_K(i + 1, 0, 0,
	    alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	  - alpha_i * X[i * 2 + 0] - alpha_r * X[i * 2 + 1],
	    Y,     1, a, 1, NULL, 0);
    AXPYU_K(i + 1, 0, 0,
	    alpha_r * Y[i * 2 + 0] + alpha_i * Y[i * 2 + 1],
	    alpha_i * Y[i * 2 + 0] - alpha_r * Y[i * 2 + 1],
	    X,     1, a, 1, NULL, 0);
    a[i * 2 + 1] = ZERO;
    a += lda;
#else
    AXPYU_K(m - i, 0, 0,
	    alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	  - alpha_i * X[i * 2 + 0] - alpha_r * X[i * 2 + 1],
	    Y + i * 2, 1, a, 1, NULL, 0);
    AXPYU_K(m - i, 0, 0,
	    alpha_r * Y[i * 2 + 0] + alpha_i * Y[i * 2 + 1],
	    alpha_i * Y[i * 2 + 0] - alpha_r * Y[i * 2 + 1],
	    X + i * 2, 1, a, 1, NULL, 0);
    a[1] = ZERO;
    a += 2 + lda;
#endif
#else
#ifndef LOWER
    AXPYC_K(i + 1, 0, 0,
	    alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	    alpha_i * X[i * 2 + 0] + alpha_r * X[i * 2 + 1],
	    Y,     1, a, 1, NULL, 0);
    AXPYC_K(i + 1, 0, 0,
	    alpha_r * Y[i * 2 + 0] + alpha_i * Y[i * 2 + 1],
	  - alpha_i * Y[i * 2 + 0] + alpha_r * Y[i * 2 + 1],
	    X,     1, a, 1, NULL, 0);
    a[i * 2 + 1] = ZERO;
    a += lda;
#else
    AXPYC_K(m - i, 0, 0,
	    alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	    alpha_i * X[i * 2 + 0] + alpha_r * X[i * 2 + 1],
	    Y + i * 2, 1, a, 1, NULL, 0);
    AXPYC_K(m - i, 0, 0,
	    alpha_r * Y[i * 2 + 0] + alpha_i * Y[i * 2 + 1],
	  - alpha_i * Y[i * 2 + 0] + alpha_r * Y[i * 2 + 1],
	    X + i * 2, 1, a, 1, NULL, 0);
    a[1] = ZERO;
    a += 2 + lda;
#endif
#endif

    }


  return 0;
}
