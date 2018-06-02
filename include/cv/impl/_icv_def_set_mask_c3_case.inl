
{
  for (j = 0; j < size.h; ++j) {
    arrtype* dst = (arrtype*)(((uchar*)dst0) + j * dststep);

    for (i = 0; i < size.w; i++) {
      if (mask[i]) {
        dst[i * 3 + 0] = s0;
        dst[i * 3 + 1] = s1;
        dst[i * 3 + 2] = s2;
      }
    }

    mask += maskstep;
  }
}
