

#include <ctype.h>


int CNAME(BLASLONG dummy1, BLASLONG n,  BLASLONG dummy2, FLOAT alpha_r, FLOAT alpha_i,
	  FLOAT *dummy3, BLASLONG dummy4, FLOAT *dummy5, BLASLONG dummy6,
	  FLOAT *c, BLASLONG ldc,
	  FLOAT *dummy7, FLOAT *dummy8, BLASLONG from, BLASLONG to){

  BLASLONG i;

#ifndef LOWER
  for (i = from; i < to; i++){
    ZSCAL_K(i + 1, 0, 0, alpha_r, alpha_i, c + i * ldc * 2, 1, NULL, 0, NULL, 0);
  }
#else
  for (i = from; i < to; i++){
	ZSCAL_K(n - i, 0, 0, alpha_r, alpha_i, c + i * (ldc + 1) * 2, 1, NULL, 0, NULL, 0);
    }
#endif
  return 0;
}
