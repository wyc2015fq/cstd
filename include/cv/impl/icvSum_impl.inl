
{
  int len = size.w * cn;
  worktype s0 = 0, s1 = 0, s2 = 0, s3 = 0;

  switch (cn) {
  case 1: {
    enum {_cn = 1};

    for (; size.h--; src += step) {
      int x = 0;

      for (; x <= (len) - 4 * (_cn); x += 4 * (_cn)) {
        s0 += __op__(src[x]) + __op__(src[x + (_cn)]) + __op__(src[x + (_cn) * 2]) + __op__(src[x + (_cn) * 3]);
      }

      for (; x < (len); x += (_cn)) {
        s0 += __op__(src[x]);
      }
    }

    sum[0] = s0;
  }
  break;

  case 2: {
    for (; size.h--; src += step) {
      int x = 0;

      for (; x <= (len) - 8; x += 8) {
        s0 += __op__(src[x]) + __op__(src[x + 2]) + __op__(src[x + 4]) + __op__(src[x + 6]);
        s1 += __op__(src[x + 1]) + __op__(src[x + 3]) + __op__(src[x + 5]) + __op__(src[x + 7]);
      }

      for (; x < (len); x += 2) {
        s0 += __op__(src[x]);
        s1 += __op__(src[x + 1]);
      }
    }

    sum[0] = s0, sum[1] = s1;
  }
  break;

  case 3: {
    for (; size.h--; src += step) {
      int x = 0;

      for (; x <= (len) - 12; x += 12) {
        s0 += __op__(src[x]) + __op__(src[x + 3]) + __op__(src[x + 6]) + __op__(src[x + 9]);
        s1 += __op__(src[x + 1]) + __op__(src[x + 4]) + __op__(src[x + 7]) + __op__(src[x + 10]);
        s2 += __op__(src[x + 2]) + __op__(src[x + 5]) + __op__(src[x + 8]) + __op__(src[x + 11]);
      }

      for (; x < (len); x += 3) {
        s0 += __op__(src[x]);
        s1 += __op__(src[x + 1]);
        s2 += __op__(src[x + 2]);
      }
    }

    sum[0] = s0, sum[1] = s1, sum[2] = s2;
  }
  break;

  case 4: {
    for (; size.h--; src += step) {
      int x = 0;

      for (; x <= (len) - 16; x += 16) {
        s0 += __op__(src[x]) + __op__(src[x + 4]) + __op__(src[x + 8]) + __op__(src[x + 12]);
        s1 += __op__(src[x + 1]) + __op__(src[x + 5]) + __op__(src[x + 9]) + __op__(src[x + 13]);
        s2 += __op__(src[x + 2]) + __op__(src[x + 6]) + __op__(src[x + 10]) + __op__(src[x + 14]);
        s3 += __op__(src[x + 3]) + __op__(src[x + 7]) + __op__(src[x + 11]) + __op__(src[x + 15]);
      }

      for (; x < (len); x += 4) {
        s0 += __op__(src[x]);
        s1 += __op__(src[x + 1]);
        s2 += __op__(src[x + 2]);
        s3 += __op__(src[x + 3]);
      }
    }

    sum[0] = s0, sum[1] = s1, sum[2] = s2, sum[3] = s3;
  }
  break;

  default:
    ASSERT(0);
    break;
  }
}
#undef __op__
