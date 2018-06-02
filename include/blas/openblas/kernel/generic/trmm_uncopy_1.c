



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG posX, BLASLONG posY, FLOAT *b){

  BLASLONG i, X;

  FLOAT data01;
  FLOAT *ao1;

  while (n > 0) {
    X = posX;

    if (posX <= posY) {
      ao1 = a + posX + (posY + 0) * lda;
    } else {
      ao1 = a + posY + (posX + 0) * lda;
    }

    i = m;
    if (m > 0) {
      do {

	if (X < posY) {
	  data01 = *(ao1 + 0);
	  b[ 0] = data01;
	  ao1 += 1;
	  b += 1;
	} else
	  if (X > posY) {
	    ao1 += lda;
	    b += 1;
	  } else {
#ifdef UNIT
	    b[ 0] = ONE;
#else
	    data01 = *(ao1 + 0);
	    b[ 0] = data01;
#endif
	    b += 1;
	    ao1 += lda;
	  }

	X += 1;
	i --;
      } while (i > 0);
    }
    posY ++;
    n --;
  }

  return 0;
}
