

#include <ctype.h>


int CNAME(BLASLONG m, FLOAT *a, FLOAT *b, BLASLONG incb, void *buffer){

  BLASLONG i;
  FLOAT *B = b;

  if (incb != 1) {
    B = buffer;
    COPY_K(m, b, incb, buffer, 1);
  }

  a += (m + 1) * m / 2 - 1;

  for (i = 0; i < m; i++) {
#ifdef TRANSA
    if (i > 0) B[m - i - 1] -= DOTU_K(i, a + 1, 1, B + m - i, 1);
#endif

#ifndef UNIT
    B[m - i - 1] /= a[0];
#endif

#ifndef TRANSA
    if (i < m - 1) AXPYU_K(m - i - 1, 0, 0, -B[m - i - 1], a - (m - i - 1), 1, B, 1, NULL, 0);
#endif

#ifndef TRANSA
    a -= (m - i);
#else
    a -= (i + 2);
#endif
  }

  if (incb != 1) {
    COPY_K(m, buffer, 1, b, incb);
  }

  return 0;
}

