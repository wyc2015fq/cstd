{
  int s0 = 0;
  int len = size.w;
  enum {_cn = 1};
  const checktype* isrc = (const checktype*)src;

  for (; size.h--; isrc += step) {
    int x = 0;

    for (; x <= (len) - 4 * (_cn); x += 4 * (_cn)) {
      s0 += __op__(isrc[x]) + __op__(isrc[x + (_cn)]) + __op__(isrc[x + (_cn) * 2]) + __op__(isrc[x + (_cn) * 3]);
    }

    for (; x < (len); x += (_cn)) {
      s0 += __op__(isrc[x]);
    }
  }

  count = s0;
}
#undef __op__
