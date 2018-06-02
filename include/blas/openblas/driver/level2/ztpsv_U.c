

#include <ctype.h>


int CNAME(BLASLONG m, FLOAT *a, FLOAT *b, BLASLONG incb, void *buffer){

  BLASLONG i;
#if (TRANSA == 2) || (TRANSA == 4)
  OPENBLAS_COMPLEX_FLOAT result;
#endif
#ifndef UNIT
  FLOAT ar, ai, br, bi, ratio, den;
#endif
  FLOAT *B = b;

  if (incb != 1) {
    B = buffer;
    COPY_K(m, b, incb, buffer, 1);
  }

  a += (m + 1) * m - 2;

  for (i = 0; i < m; i++) {

#if (TRANSA == 2) || (TRANSA == 4)
    if (i > 0) {
#if TRANSA == 2
      result = DOTU_K(i, a + 2, 1, B + (m - i) * 2, 1);
#else
      result = DOTC_K(i, a + 2, 1, B + (m - i) * 2, 1);
#endif

      B[(m - i - 1) * 2 + 0] -= CREAL(result);
      B[(m - i - 1) * 2 + 1] -= CIMAG(result);
    }
#endif

#ifndef UNIT
    ar = a[0];
    ai = a[1];

    if (fabs(ar) >= fabs(ai)){
      ratio = ai / ar;
      den = 1./(ar * ( 1 + ratio * ratio));

      ar =  den;
#if (TRANSA == 1) || (TRANSA == 2)
      ai = -ratio * den;
#else
      ai =  ratio * den;
#endif
    } else {
      ratio = ar / ai;
      den = 1./(ai * ( 1 + ratio * ratio));
      ar =  ratio * den;
#if (TRANSA == 1) || (TRANSA == 2)
      ai = -den;
#else
      ai =  den;
#endif
    }

    br = B[(m - i - 1) * 2 + 0];
    bi = B[(m - i - 1) * 2 + 1];

    B[(m - i - 1) * 2 + 0] = ar*br - ai*bi;
    B[(m - i - 1) * 2 + 1] = ar*bi + ai*br;
#endif

#if (TRANSA == 1) || (TRANSA == 3)
    if (i < m - 1) {
#if TRANSA == 1
      AXPYU_K (m - i - 1, 0, 0, - B[(m - i - 1) * 2 + 0], -B[(m - i - 1) * 2 + 1],
	       a - (m - i - 1) * COMPSIZE, 1, B, 1, NULL, 0);
#else
      AXPYC_K (m - i - 1, 0, 0, - B[(m - i - 1) * 2 + 0], -B[(m - i - 1) * 2 + 1],
	       a - (m - i - 1) * COMPSIZE, 1, B, 1, NULL, 0);
#endif
    }
#endif

#if (TRANSA == 1) || (TRANSA == 3)
    a -= (m - i) * 2;
#else
    a -= (i + 2) * 2;
#endif
  }

  if (incb != 1) {
    COPY_K(m, buffer, 1, b, incb);
  }

  return 0;
}

