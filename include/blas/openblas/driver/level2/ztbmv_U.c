

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
  FLOAT atemp1, atemp2, btemp1, btemp2;
#endif

  if (incb != 1) {
    B = buffer;
    COPY_K(n, b, incb, buffer, 1);
  }

  for (i = 0; i < n; i++) {

#if (TRANSA == 1) || (TRANSA == 3)
    length  = i;
    if (length > k) length = k;

    if (length > 0) {
#if   TRANSA == 1
      AXPYU_K(length, 0, 0,
	      B[i * 2 + 0],  B[i * 2 + 1],
	      a + (k - length) * COMPSIZE, 1, B + (i - length) * COMPSIZE, 1, NULL, 0);
#else
      AXPYC_K(length, 0, 0,
	      B[i * 2 + 0],  B[i * 2 + 1],
	      a + (k - length) * COMPSIZE, 1, B + (i - length) * COMPSIZE, 1, NULL, 0);
#endif

    }
#endif

#ifndef UNIT
#if (TRANSA == 1) || (TRANSA == 3)
      atemp1 = a[k * 2 + 0];
      atemp2 = a[k * 2 + 1];
#else
      atemp1 = a[0];
      atemp2 = a[1];
#endif

      btemp1 = B[i * 2 + 0];
      btemp2 = B[i * 2 + 1];

#if (TRANSA == 1) || (TRANSA == 2)
      B[i * 2 + 0] = atemp1 * btemp1 - atemp2 * btemp2;
      B[i * 2 + 1] = atemp1 * btemp2 + atemp2 * btemp1;
#else
      B[i * 2 + 0] = atemp1 * btemp1 + atemp2 * btemp2;
      B[i * 2 + 1] = atemp1 * btemp2 - atemp2 * btemp1;
#endif
#endif

#if (TRANSA == 2) || (TRANSA == 4)
    length  = n - i - 1;
    if (length > k) length = k;

    if (length > 0) {
#if TRANSA == 2
      temp = DOTU_K(length, a + COMPSIZE, 1, B + (i + 1) * COMPSIZE, 1);
#else
      temp = DOTC_K(length, a + COMPSIZE, 1, B + (i + 1) * COMPSIZE, 1);
#endif

      B[i * 2 + 0] += CREAL(temp);
      B[i * 2 + 1] += CIMAG(temp);
    }
#endif

    a += lda * COMPSIZE;
  }

  if (incb != 1) {
    COPY_K(n, buffer, 1, b, incb);
  }

  return 0;
}

