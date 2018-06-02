

#include <ctype.h>


int CNAME(BLASLONG m, FLOAT alpha_r, FLOAT *x, BLASLONG incx,
		       FLOAT *y, BLASLONG incy, FLOAT *a, BLASLONG lda, FLOAT *buffer){

  BLASLONG i;
  FLOAT *X, *Y;

  X = x;
  Y = y;

  if (incx != 1) {
    COPY_K(m, x, incx, buffer, 1);
    X = buffer;
  }

  if (incy != 1) {
    COPY_K(m, y, incy, (FLOAT *)((BLASLONG)buffer + (BUFFER_SIZE / 2)), 1);
    Y = (FLOAT *)((BLASLONG)buffer + (BUFFER_SIZE / 2));
  }

  for (i = 0; i < m; i++){
#ifndef LOWER
    AXPYU_K(i + 1, 0, 0, alpha_r * X[i], Y,     1, a, 1, NULL, 0);
    AXPYU_K(i + 1, 0, 0, alpha_r * Y[i], X,     1, a, 1, NULL, 0);
    a += lda;
#else
    AXPYU_K(m - i, 0, 0, alpha_r * X[i], Y + i, 1, a, 1, NULL, 0);
    AXPYU_K(m - i, 0, 0, alpha_r * Y[i], X + i, 1, a, 1, NULL, 0);
    a += 1 + lda;
#endif
    }

  return 0;
}
