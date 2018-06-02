

#include <ctype.h>


const static FLOAT dp1 = 1.;

int CNAME(BLASLONG n, BLASLONG k, FLOAT *a, BLASLONG lda, FLOAT *b, BLASLONG incb, void *buffer){

  BLASLONG i;
  FLOAT *B = b;
  BLASLONG length;

  if (incb != 1) {
    B = buffer;
    COPY_K(n, b, incb, buffer, 1);
  }

  a += (n - 1) * lda;

  for (i = n - 1; i >= 0; i--) {

#ifdef TRANSA
    length  = n - i - 1;
    if (length > k) length = k;

    if (length > 0) {
      B[i] -= DOTU_K(length, a + 1, 1, B + i + 1, 1);
    }
#endif

#ifndef UNIT
#ifdef TRANSA
    B[i] /= a[0];
#else
    B[i] /= a[k];
#endif
#endif

#ifndef TRANSA
    length  = i;
    if (length > k) length = k;

    if (length > 0) {
      AXPYU_K(length, 0, 0,
	     - B[i],
	     a + k - length, 1, B + i - length, 1, NULL, 0);
    }
#endif

    a -= lda;
  }

  if (incb != 1) {
    COPY_K(n, buffer, 1, b, incb);
  }

  return 0;
}

