

#include <ctype.h>


int CNAME(BLASLONG m, FLOAT *a, FLOAT *b, BLASLONG incb, void *buffer){

  BLASLONG i;
#if (TRANSA == 2) || (TRANSA == 4)
  OPENBLAS_COMPLEX_FLOAT temp;
#endif
#ifndef UNIT
  FLOAT atemp1, atemp2, btemp1, btemp2;
#endif
  FLOAT *B = b;

  if (incb != 1) {
    B = buffer;
    COPY_K(m, b, incb, buffer, 1);
  }

  a += (m + 1) * m - 2;

  for (i = 0; i < m; i++) {

#if (TRANSA == 1) || (TRANSA == 3)
#if TRANSA == 1
      if (i > 0) AXPYU_K (i, 0, 0,
			  B[(m - i - 1) * 2 + 0], B[(m - i - 1) * 2 + 1],
			  a + 2, 1, B + (m - i) * 2, 1, NULL, 0);
#else
      if (i > 0) AXPYC_K(i, 0, 0,
			  B[(m - i - 1) * 2 + 0], B[(m - i - 1) * 2 + 1],
			  a + 2, 1, B + (m - i) * 2, 1, NULL, 0);
#endif
#endif

#ifndef UNIT
      atemp1 = a[0];
      atemp2 = a[1];

      btemp1 = B[(m - i - 1) * 2 + 0];
      btemp2 = B[(m - i - 1) * 2 + 1];

#if (TRANSA == 1) || (TRANSA == 2)
      B[(m - i - 1) * 2 + 0] = atemp1 * btemp1 - atemp2 * btemp2;
      B[(m - i - 1) * 2 + 1] = atemp1 * btemp2 + atemp2 * btemp1;
#else
      B[(m - i - 1) * 2 + 0] = atemp1 * btemp1 + atemp2 * btemp2;
      B[(m - i - 1) * 2 + 1] = atemp1 * btemp2 - atemp2 * btemp1;
#endif
#endif

#if (TRANSA == 2) || (TRANSA == 4)
      if (i < m - 1) {
#if   TRANSA == 2
	temp = DOTU_K(m - i - 1, a - (m - i - 1) * 2, 1, B, 1);
#else
	temp = DOTC_K(m - i - 1, a - (m - i - 1) * 2, 1, B, 1);
#endif

	B[(m - i - 1) * 2 + 0] += CREAL(temp);
	B[(m - i - 1) * 2 + 1] += CIMAG(temp);
      }
#endif

#if (TRANSA == 1) || (TRANSA == 3)
    a -= (i + 2) * 2;
#else
    a -= (m - i) * 2;
#endif

    }


  if (incb != 1) {
    COPY_K(m, buffer, 1, b, incb);
  }

  return 0;
}
