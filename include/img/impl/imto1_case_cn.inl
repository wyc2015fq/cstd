{
  int i, j;
  imsetsize(im1, im->h, (im->w + 7) / 8, 1, im->f);
  memset(im1->tt.data, 0, im1->h * im1->s);

  for (i = 0; i < im->h * im->f; ++i) {
    const uchar* src = im->tt.data + i * im->s;
    uchar* dst = im1->tt.data + i * im1->s;
    j = 0;

    for (; j < im->w - 7; j += 8, src += 8 * im->c) {
      //dst[j] = GETBIT1(0) << (7);
    }

    for (; j < im->w; ++j, src += im->c) {
      dst[j >> 3] |= GETBIT1(0) << (7 - (j & 7));
    }
  }
}

#undef GETBIT1