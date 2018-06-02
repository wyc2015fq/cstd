

#include <ctype.h>


const static FLOAT dp1 = 1.;

int CNAME(BLASLONG m, FLOAT *a, BLASLONG lda, FLOAT *b, BLASLONG incb, FLOAT *buffer){

  BLASLONG i, is, min_i;
  FLOAT *gemvbuffer = (FLOAT *)buffer;
  FLOAT *B = b;

  if (incb != 1) {
    B = buffer;
    gemvbuffer = (FLOAT *)(((BLASLONG)buffer + m * sizeof(FLOAT) + 4095) & ~4095);
    COPY_K(m, b, incb, buffer, 1);
  }

  for (is = 0; is < m; is += DTB_ENTRIES){

    min_i = MIN(m - is, DTB_ENTRIES);

#ifndef TRANSA
    if (is > 0){
      GEMV_N(is, min_i, 0, dp1,
	     a + is * lda,  lda,
	     B + is, 1,
	     B,      1, gemvbuffer);
    }
#endif

    for (i = 0; i < min_i; i++) {
      FLOAT *AA = a + is + (i + is) * lda;
      FLOAT *BB = B + is;

#ifndef TRANSA
      if (i > 0) AXPYU_K(i, 0, 0, BB[i], AA, 1, BB, 1, NULL, 0);
#endif

#ifndef UNIT
      BB[i] *= AA[i];
#endif

#ifdef TRANSA
      if (i < min_i - 1) BB[i] += DOTU_K(min_i - i - 1, AA + i + 1, 1, BB + i + 1, 1);
#endif
    }

#ifdef TRANSA
    if (m - is > min_i){
      GEMV_T(m - is - min_i, min_i, 0, dp1,
	     a + is + min_i + is * lda,  lda,
	     B + is + min_i, 1,
	     B + is,         1, gemvbuffer);
  }
#endif

  }

  if (incb != 1) {
    COPY_K(m, buffer, 1, b, incb);
  }

  return 0;
}

