



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, FLOAT *b){
  BLASLONG i, j;

  FLOAT *a_offset, *a_offset1;
  FLOAT *b_offset, *b_offset1;

  a_offset = a;
  b_offset = b;

  i = m;

  if (i > 0) {
    do {
      a_offset1 = a_offset;
      a_offset += lda;

      b_offset1 = b_offset;
      b_offset ++;

      j = n;
      if (j > 0) {
	do {
	  *(b_offset1 + 0) = *(a_offset1 + 0);
	  a_offset1 ++;
	  b_offset1 += m;
	  j --;
	} while (j > 0);
      }
      i --;
    } while (i > 0);
  }

  return 0;
}
