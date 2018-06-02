{
  uchar tmpByte;

  switch (abpp) {
#if 1

  case 1: {
    for (y = 0; y < h; ++y) {
      const a_type* A = (a_type*)(A0 + y * al);
      b_type* B = (b_type*)(B0 + y * bl);

      for (x = 0; x < (w >> 3); ++x, B += 8 * bi) {
        uchar tmpByte = A[x];
        uchar i0 = (tmpByte >> 7);
        uchar i1 = (tmpByte >> 6) & 1;
        uchar i2 = (tmpByte >> 5) & 1;
        uchar i3 = (tmpByte >> 4) & 1;
        uchar i4 = (tmpByte >> 3) & 1;
        uchar i5 = (tmpByte >> 2) & 1;
        uchar i6 = (tmpByte >> 1) & 1;
        uchar i7 = tmpByte & 1;
        PUT_RGB(0, GET_RGB(i0, 0), GET_RGB(i0, 1), GET_RGB(i0, 2));
        PUT_RGB(1, GET_RGB(i1, 0), GET_RGB(i1, 1), GET_RGB(i1, 2));
        PUT_RGB(2, GET_RGB(i2, 0), GET_RGB(i2, 1), GET_RGB(i2, 2));
        PUT_RGB(3, GET_RGB(i3, 0), GET_RGB(i3, 1), GET_RGB(i3, 2));
        PUT_RGB(4, GET_RGB(i4, 0), GET_RGB(i4, 1), GET_RGB(i4, 2));
        PUT_RGB(5, GET_RGB(i5, 0), GET_RGB(i5, 1), GET_RGB(i5, 2));
        PUT_RGB(6, GET_RGB(i6, 0), GET_RGB(i6, 1), GET_RGB(i6, 2));
        PUT_RGB(7, GET_RGB(i7, 0), GET_RGB(i7, 1), GET_RGB(i7, 2));
      }

      if (w & 7) {
        tmpByte = A[w >> 3];

        for (x = 0; x < (w & 7); ++x, B += 8 * bi) {
          uchar i0 = (tmpByte >> (7 - x)) & 1;
          PUT_RGB(0, GET_RGB(i0, 0), GET_RGB(i0, 1), GET_RGB(i0, 2));
        }
      }
    }
  }
  break;
#else

  case 8: {
    for (y = 0; y < h; ++y) {
      const a_type* A = (a_type*)(A0 + y * al);
      b_type* B = (b_type*)(B0 + y * bl);

      for (x = 0; x < w; ++x, B += bi) {
        uchar i = A[x];
        PUT_RGB(0, GET_RGB(i, 0), GET_RGB(i, 1), GET_RGB(i, 2));
      }
    }
  }
  break;

  case 4: {
    for (y = 0; y < h; ++y) {
      const a_type* A = (a_type*)(A0 + y * al);
      b_type* B = (b_type*)(B0 + y * bl);

      for (x = 0; x < w - 1; x += 2, B += 2 * bi) {
        uchar i0 = A[x] >> 4;
        uchar i1 = A[x] & 0xf;
        PUT_RGB(0, GET_RGB(i0, 0), GET_RGB(i0, 1), GET_RGB(i0, 2));
        PUT_RGB(1, GET_RGB(i1, 0), GET_RGB(i1, 1), GET_RGB(i1, 2));
      }

      if ((w & 1) != 0) {
        uchar i0 = A0[w >> 1] >> 4; // is safe
        PUT_RGB(0, GET_RGB(i0, 0), GET_RGB(i0, 1), GET_RGB(i0, 2));
      }
    }
  }
  break;

  case 1: {
    for (y = 0; y < h; ++y) {
      const a_type* A = (a_type*)(A0 + y * al);
      b_type* B = (b_type*)(B0 + y * bl);

      for (x = 0; x < w - 7; x += 8, B += bi) {
        uchar tmpByte = A[x >> 3];
        uchar i0 = (tmpByte >> 7);
        uchar i1 = (tmpByte >> 6) & 1;
        uchar i2 = (tmpByte >> 5) & 1;
        uchar i3 = (tmpByte >> 4) & 1;
        uchar i4 = (tmpByte >> 3) & 1;
        uchar i5 = (tmpByte >> 2) & 1;
        uchar i6 = (tmpByte >> 1) & 1;
        uchar i7 = tmpByte & 1;
        PUT_RGB(0, GET_RGB(i0, 0), GET_RGB(i0, 1), GET_RGB(i0, 2));
        PUT_RGB(1, GET_RGB(i1, 0), GET_RGB(i1, 1), GET_RGB(i1, 2));
        PUT_RGB(2, GET_RGB(i2, 0), GET_RGB(i2, 1), GET_RGB(i2, 2));
        PUT_RGB(3, GET_RGB(i3, 0), GET_RGB(i3, 1), GET_RGB(i3, 2));
        PUT_RGB(4, GET_RGB(i4, 0), GET_RGB(i4, 1), GET_RGB(i4, 2));
        PUT_RGB(5, GET_RGB(i5, 0), GET_RGB(i5, 1), GET_RGB(i5, 2));
        PUT_RGB(6, GET_RGB(i6, 0), GET_RGB(i6, 1), GET_RGB(i6, 2));
        PUT_RGB(7, GET_RGB(i7, 0), GET_RGB(i7, 1), GET_RGB(i7, 2));
      }

      for (; x < w; ++x, B += bi) {
        uchar tmpByte = A[x >> 3];
        uchar i0 = (tmpByte >> (7 - (x & 7))) & 1;
        PUT_RGB(0, GET_RGB(i0, 0), GET_RGB(i0, 1), GET_RGB(i0, 2));
      }
    }
  }
  break;
#endif

  default:
    ASSERT(0);
    break;
  }

}
#undef GET_RGB
#undef PUT_RGB
