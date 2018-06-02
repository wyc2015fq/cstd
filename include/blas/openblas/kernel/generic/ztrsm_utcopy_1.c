



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG offset, FLOAT *b){

  BLASLONG i, ii, j, jj;

  FLOAT data01, data02;
  FLOAT *a1;

  lda *= 2;

  jj = offset;

  j = (n);
  while (j > 0){

    a1 = a + 0 * lda;

    i = m;
    ii = 0;
    while (i > 0) {

      if (ii == jj) {
#ifndef UNIT
	data01 = *(a1 + 0);
	data02 = *(a1 + 1);
#endif
	compinv(b + 0, data01, data02);
      }

      if (ii > jj) {
	data01 = *(a1 + 0);
	data02 = *(a1 + 1);
	*(b +  0) = data01;
	*(b +  1) = data02;
      }

      a1 += lda;
      b += 2;

      i  --;
      ii ++;
    }

    a += 2;
    jj ++;
    j  --;
  }

  return 0;
}
