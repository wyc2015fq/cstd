



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, FLOAT *b){
  BLASLONG i, j;
  FLOAT *a_offset, *a_offset1;
  FLOAT *b_offset;

  a_offset = a;
  b_offset = b;

  j = n;

  if (j > 0){
    do {
      a_offset1 = a_offset;
      a_offset += lda;

      i = (m >> 3);

      if (i > 0){
	do {
	  *(b_offset + 0) = *(a_offset1 + 0);
	  *(b_offset + 1) = *(a_offset1 + 1);
	  *(b_offset + 2) = *(a_offset1 + 2);
	  *(b_offset + 3) = *(a_offset1 + 3);
	  *(b_offset + 4) = *(a_offset1 + 4);
	  *(b_offset + 5) = *(a_offset1 + 5);
	  *(b_offset + 6) = *(a_offset1 + 6);
	  *(b_offset + 7) = *(a_offset1 + 7);
	  a_offset1 += 8;
	  b_offset  += 8;
	  i --;
	} while (i > 0);
      }

      i = (m & 7);

      if (i > 0){
	do {
	  *(b_offset + 0) = *(a_offset1 + 0);
	  a_offset1 ++;
	  b_offset  ++;
	  i --;
	} while (i > 0);
      }
      j --;
    } while (j > 0);
  }

  return 0;
}
