



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG posX, BLASLONG posY, FLOAT *b){

  BLASLONG i, js, offset;

  FLOAT data01, data02, data03, data04;
  FLOAT *ao1, *ao2, *ao3, *ao4;

  js = (n >> 2);
  while (js > 0){

    offset = posX - posY;

    if (offset >  0) ao1 = a + posY + (posX + 0) * lda; else ao1 = a + posX + 0 + posY * lda;
    if (offset > -1) ao2 = a + posY + (posX + 1) * lda; else ao2 = a + posX + 1 + posY * lda;
    if (offset > -2) ao3 = a + posY + (posX + 2) * lda; else ao3 = a + posX + 2 + posY * lda;
    if (offset > -3) ao4 = a + posY + (posX + 3) * lda; else ao4 = a + posX + 3 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao2 + 0);
      data03 = *(ao3 + 0);
      data04 = *(ao4 + 0);

      if (offset >   0) ao1 ++; else ao1 += lda;
      if (offset >  -1) ao2 ++; else ao2 += lda;
      if (offset >  -2) ao3 ++; else ao3 += lda;
      if (offset >  -3) ao4 ++; else ao4 += lda;

      b[ 0] = data01;
      b[ 1] = data02;
      b[ 2] = data03;
      b[ 3] = data04;

      b += 4;

      offset --;
      i --;
    }

    posX += 4;
    js --;
  }

  if (n & 2) {
    offset = posX - posY;

    if (offset >  0) ao1 = a + posY + (posX + 0) * lda; else ao1 = a + posX + 0 + posY * lda;
    if (offset > -1) ao2 = a + posY + (posX + 1) * lda; else ao2 = a + posX + 1 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao2 + 0);

      if (offset >   0) ao1 ++; else ao1 += lda;
      if (offset >  -1) ao2 ++; else ao2 += lda;

      b[ 0] = data01;
      b[ 1] = data02;

      b += 2;

      offset --;
      i --;
    }

    posX += 2;
  }

  if (n & 1) {
    offset = posX - posY;

    if (offset >  0) ao1 = a + posY + (posX + 0) * lda; else ao1 = a + posX + 0 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);

      if (offset >   0) ao1 ++; else ao1 += lda;

      b[ 0] = data01;

      b ++;

      offset --;
      i --;
    }
  }

  return 0;
}
