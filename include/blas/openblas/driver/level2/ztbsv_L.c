

#include <ctype.h>


const static FLOAT dp1 = 1.;

int CNAME(BLASLONG n, BLASLONG k, FLOAT *a, BLASLONG lda, FLOAT *b, BLASLONG incb, void *buffer){

  BLASLONG i;
  FLOAT *B = b;
  BLASLONG length;
#if (TRANSA == 2) || (TRANSA == 4)
  OPENBLAS_COMPLEX_FLOAT temp;
#endif
#ifndef UNIT
  FLOAT ar, ai, br, bi, ratio, den;
#endif

  if (incb != 1) {
    B = buffer;
    COPY_K(n, b, incb, buffer, 1);
  }

  for (i = 0; i < n; i++) {

#if (TRANSA == 2) || (TRANSA == 4)
    length  = i;
    if (length > k) length = k;

    if (length > 0) {
#if TRANSA == 2
      temp = DOTU_K(length, a + (k - length) * COMPSIZE, 1, B + (i - length) * COMPSIZE, 1);
#else
      temp = DOTC_K(length, a + (k - length) * COMPSIZE, 1, B + (i - length) * COMPSIZE, 1);
#endif

      B[i * 2 + 0] -= CREAL(temp);
      B[i * 2 + 1] -= CIMAG(temp);
    }
#endif

#ifndef UNIT
#if (TRANSA == 1) || (TRANSA == 3)
    ar = a[0];
    ai = a[1];
#else
    ar = a[k * 2 + 0];
    ai = a[k * 2 + 1];
#endif

    if (fabs(ar) >= fabs(ai)){
      ratio = ai / ar;
      den = 1./(ar * ( 1 + ratio * ratio));

      ar =  den;
#if TRANSA < 3
      ai = -ratio * den;
#else
      ai =  ratio * den;
#endif
    } else {
      ratio = ar / ai;
      den = 1./(ai * ( 1 + ratio * ratio));
      ar =  ratio * den;
#if TRANSA < 3
      ai = -den;
#else
      ai =  den;
#endif
    }

    br = B[i * 2 + 0];
    bi = B[i * 2 + 1];

    B[i * 2 + 0] = ar*br - ai*bi;
    B[i * 2 + 1] = ar*bi + ai*br;
#endif

#if (TRANSA == 1) || (TRANSA == 3)
    length  = n - i - 1;
    if (length > k) length = k;

    if (length > 0) {
#if   TRANSA == 1
      AXPYU_K(length, 0, 0,
	      -B[i * 2 + 0],  -B[i * 2 + 1],
	      a + COMPSIZE, 1, B + (i + 1) * COMPSIZE, 1, NULL, 0);
#else
      AXPYC_K(length, 0, 0,
	      -B[i * 2 + 0],  -B[i * 2 + 1],
	      a + COMPSIZE, 1, B + (i + 1) * COMPSIZE, 1, NULL, 0);
#endif
    }
#endif

    a += lda * COMPSIZE;
  }

  if (incb != 1) {
    COPY_K(n, buffer, 1, b, incb);
  }

  return 0;
}

