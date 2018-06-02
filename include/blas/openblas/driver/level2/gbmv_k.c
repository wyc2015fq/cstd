

#include <ctype.h>


#ifndef TRANS
#define M m
#define N n
#else
#define N m
#define M n
#endif

void CNAME(BLASLONG m, BLASLONG n, BLASLONG ku, BLASLONG kl, FLOAT alpha,
	  FLOAT *a, BLASLONG lda,
	  FLOAT *x, BLASLONG incx, FLOAT *y, BLASLONG incy, void *buffer){

  BLASLONG i, offset_u, offset_l, start, end, length;
  FLOAT *X = x;
  FLOAT *Y = y;
  FLOAT *gemvbuffer = (FLOAT *)buffer;
  FLOAT *bufferY    = gemvbuffer;
  FLOAT *bufferX    = gemvbuffer;

  if (incy != 1) {
    Y = bufferY;
    bufferX    = (FLOAT *)(((BLASLONG)bufferY + M * sizeof(FLOAT) + 4095) & ~4095);
    gemvbuffer = bufferX;
    COPY_K(M, y, incy, Y, 1);
  }

  if (incx != 1) {
    X = bufferX;
    gemvbuffer = (FLOAT *)(((BLASLONG)bufferX + N * sizeof(FLOAT) + 4095) & ~4095);
    COPY_K(N, x, incx, X, 1);
  }

  offset_u = ku;
  offset_l = ku + m;

  for (i = 0; i < MIN(n, m + ku); i++) {

    start = MAX(offset_u, 0);
    end   = MIN(offset_l, ku + kl + 1);

    length  = end - start;

#ifndef TRANS
    AXPYU_K(length, 0, 0,
	   alpha * X[i],
	   a + start, 1, Y + start - offset_u, 1, NULL, 0);
#else
    Y[i] += alpha * DOTU_K(length, a + start, 1, X + start - offset_u, 1);
#endif

    offset_u --;
    offset_l --;

    a += lda;
  }

  if (incy != 1) {
    COPY_K(M, Y, 1, y, incy);
  }

  return;
}

