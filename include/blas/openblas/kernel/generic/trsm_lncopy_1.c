



#ifndef UNIT
#define INV(a) (ONE / (a))
#else
#define INV(a) (ONE)
#endif

int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG offset, FLOAT *b){

  BLASLONG i, ii, j, jj;

#ifndef UNIT
  FLOAT data01;
#endif
  FLOAT *a1;

  jj = offset;

  j = n;
  while (j > 0){

    a1 = a + 0 * lda;

    i = m;
    ii = 0;
    while (i > 0) {

      if (ii == jj) {
#ifndef UNIT
	data01 = *(a1 + 0);
#endif
	*(b +  0) = INV(data01);
      }

      if (ii > jj) *(b +  0) = *(a1 + 0);

      a1 ++;
      b ++;

      i  --;
      ii ++;
    }

    a += lda;
    jj ++;
    j  --;
  }

  return 0;
}
