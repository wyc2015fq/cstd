



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG posX, BLASLONG posY, FLOAT *b){

  BLASLONG i, js, offset;

  FLOAT data01, data02;
  FLOAT *ao1, *ao2;

  js = (n >> 1);
  while (js > 0){

    offset = posX - posY;

    if (offset >  0) ao1 = a + posX + 0 + posY * lda; else ao1 = a + posY + (posX + 0) * lda;
    if (offset > -1) ao2 = a + posX + 1 + posY * lda; else ao2 = a + posY + (posX + 1) * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao2 + 0);

      if (offset >   0) ao1 += lda; else ao1 ++;
      if (offset >  -1) ao2 += lda; else ao2 ++;

      b[ 0] = data01;
      b[ 1] = data02;

      b += 2;

      offset --;
      i --;
    }

    posX += 2;
    js --;
  }

  if (n & 1) {

    offset = posX - posY;

    if (offset >  0) ao1 = a + posX + 0 + posY * lda; else ao1 = a + posY + (posX + 0) * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);

      if (offset >   0) ao1 += lda; else ao1 ++;

      b[ 0] = data01;

      b ++;

      offset --;
      i --;
    }
  }

  return 0;
}
