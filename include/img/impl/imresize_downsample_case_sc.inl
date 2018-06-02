{
  int x, y;

  for (y = 0; y < bh; ++y) {
    const uchar* a0 = A + (y * (1 << __cch)) * al;
    uchar* b0 = B + y * bl;
    ASSERT(__cn == ai && __cn == bi);

    for (x = 0; x < bw; ++x, b0 += __cn, a0 += (1 << __ccw) * __cn) {
#define _OP(_i)  (b0[_i] = (ADDY(_i))>>(__ccw+__cch))
      OPTREPT(0, _OP);
#undef _OP
    }
  }
}

#undef ADDY
#undef __cch



