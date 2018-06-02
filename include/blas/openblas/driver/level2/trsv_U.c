

#include <ctype.h>


const static FLOAT dm1 = -1.;

int CNAME(BLASLONG m, FLOAT *a, BLASLONG lda, FLOAT *b, BLASLONG incb, void *buffer){

  BLASLONG i, is, min_i;
  FLOAT *gemvbuffer = (FLOAT *)buffer;
  FLOAT *B = b;

  if (incb != 1) {
    B = buffer;
    gemvbuffer = (FLOAT *)(((BLASLONG)buffer + m * sizeof(FLOAT) + 4095) & ~4095);
    COPY_K(m, b, incb, buffer, 1);
  }

  for (is = m; is > 0; is -= DTB_ENTRIES){

    min_i = MIN(is, DTB_ENTRIES);

#ifdef TRANSA
    if (m - is > 0){
      GEMV_T(m - is, min_i, 0, dm1,
	     a + is + (is - min_i) * lda, lda,
	     B + is, 1,
	     B + is - min_i, 1, gemvbuffer);
    }
#endif

    for (i = 0; i < min_i; i++) {
      FLOAT *AA = a + (is - i - 1) + (is - i - 1) * lda;
      FLOAT *BB = B + (is - i - 1);

#ifdef TRANSA
      if (i > 0) BB[0] -= DOTU_K(i, AA + 1, 1, BB + 1, 1);
#endif

#ifndef UNIT
      BB[0] /= AA[0];
#endif

#ifndef TRANSA
      if (i < min_i - 1) AXPYU_K(min_i - i - 1, 0, 0, -BB[0], AA - (min_i - i - 1), 1, BB - (min_i - i - 1), 1, NULL, 0);
#endif
    }

#ifndef TRANSA
    if (is - min_i > 0){
      GEMV_N(is - min_i, min_i, 0, dm1,
	     a +  (is - min_i) * lda, lda,
	     B + is - min_i, 1,
	     B,              1, gemvbuffer);
    }
#endif

  }

  if (incb != 1) {
    COPY_K(m, buffer, 1, b, incb);
  }

  return 0;
}

