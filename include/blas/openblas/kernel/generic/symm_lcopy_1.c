



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG posX, BLASLONG posY, FLOAT *b){

  BLASLONG i, js, offset;

  FLOAT data01;
  FLOAT *ao1;

  js = n;
  while (js > 0){

    offset = posX - posY;

    if (offset >  0) ao1 = a + posX + 0 + posY * lda; else ao1 = a + posY + (posX + 0) * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);

      if (offset >   0) ao1 += lda; else ao1 ++;

      b[ 0] = data01;

      b += 1;

      offset --;
      i --;
    }

    posX += 1;
    js --;
  }

  return 0;
}
