{
  int x, y;

  for (y = 0; y < bh; ++y) {
    const uchar* a0 = A + (y * __rows) * al;
    uchar* b0 = B + y * bl;

    for (x = 0; x < bw; ++x, b0 += bi, a0 += __cols * ai) {
    }
  }
}