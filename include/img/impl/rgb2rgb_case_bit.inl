{
  typedef uchar b_type;
  enum {__bcn = 1 << __bit, __mask = __bcn - 1, __bb = 8 / __bcn, __mask2 = (1 << __bb) - 1};
  int y, x;

  for (y = 0; y < h; ++y) {
    const a_type* A = (a_type*)(A0 + y * al);
    b_type* B = (b_type*)(B0 + y * bl);

    for (x = 0; x < w - __mask; x += __bcn, A += __bcn) {
      B[x >> __bit] = (b_type)PUT_IDX();
    }

    for (; x < w; ++x, ++A) {
      B[x >> __bit] |= GET_IDX(0) << (__mask - (x & __mask));
    }
  }
}
#undef PUT_IDX

