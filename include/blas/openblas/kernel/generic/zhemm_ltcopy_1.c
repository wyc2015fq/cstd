



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG posX, BLASLONG posY, FLOAT *b){

  BLASLONG i, js, offset;

  FLOAT data01, data02;
  FLOAT *ao1;

  lda *= 2;

  js = n;

  while (js > 0){

    offset = posX - posY;

    if (offset >  0) ao1 = a + (posX + 0) * 2 + posY * lda; else ao1 = a + posY * 2 + (posX + 0) * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao1 + 1);

      if (offset >   0) ao1 += lda; else ao1 += 2;

      if (offset > 0) {
	b[ 0] = data01;
	b[ 1] = data02;
      } else
	if (offset < 0) {
	  b[ 0] = data01;
	  b[ 1] = -data02;
	} else {
	    b[ 0] = data01;
	    b[ 1] = ZERO;
	}

      b += 2;

      offset --;
      i --;
    }
    posX ++;
    js --;

  }

  return 0;
}
