

#include <ctype.h>


const static FLOAT dp1 = 1.;

int CNAME(BLASLONG m, FLOAT *a, FLOAT *b, BLASLONG incb, void *buffer){

  BLASLONG i;
  FLOAT *B = b;

  if (incb != 1) {
    B = buffer;
    COPY_K(m, b, incb, buffer, 1);
  }

  a += (m + 1) * m / 2 - 1;

  for (i = 0; i < m; i++) {
#ifndef TRANSA
    if (i > 0) AXPYU_K(i, 0, 0, B[m - i - 1], a + 1, 1, B + m - i, 1, NULL, 0);
#endif

#ifndef UNIT
    B[m - i - 1] *= a[0];
#endif

#ifdef TRANSA
    if (i < m - 1) B[m - i - 1] += DOTU_K(m - i - 1, a - (m - i - 1), 1, B, 1);
#endif

#ifndef TRANSA
    a -= (i + 2);
#else
    a -= (m - i);
#endif
  }

  if (incb != 1) {
    COPY_K(m, buffer, 1, b, incb);
  }

  return 0;
}

