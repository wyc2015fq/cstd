



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG posX, BLASLONG posY, FLOAT *b){

  BLASLONG i, js, offset;

  FLOAT data01, data02, data03, data04;
  FLOAT *ao1, *ao2;

  lda *= 2;

  js = (n >> 1);

  while (js > 0){

    offset = posX - posY;

    if (offset >  0) ao1 = a + (posX + 0) * 2 + posY * lda; else ao1 = a + posY * 2 + (posX + 0) * lda;
    if (offset > -1) ao2 = a + (posX + 1) * 2 + posY * lda; else ao2 = a + posY * 2 + (posX + 1) * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao1 + 1);
      data03 = *(ao2 + 0);
      data04 = *(ao2 + 1);

      if (offset >   0) ao1 += lda; else ao1 += 2;
      if (offset >  -1) ao2 += lda; else ao2 += 2;

      b[ 0] = data01;
      b[ 1] = data02;
      b[ 2] = data03;
      b[ 3] = data04;

      b += 4;

      offset --;
      i --;
    }

    posX += 2;
    js --;
  }

  if (n & 1) {

    offset = posX - posY;

    if (offset >  0) ao1 = a + (posX + 0) * 2 + posY * lda; else ao1 = a + posY * 2 + (posX + 0) * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao1 + 1);

      if (offset >   0) ao1 += lda; else ao1 += 2;

      b[ 0] = data01;
      b[ 1] = data02;

      b += 2;

      offset --;
      i --;
    }

  }

  return 0;
}
