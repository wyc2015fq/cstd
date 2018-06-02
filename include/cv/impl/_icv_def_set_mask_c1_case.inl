
{
  for (j = 0; j < size.h; ++j) {
    arrtype* dst = (arrtype*)(((uchar*)dst0) + j * dststep);

    for (i = 0; i <= size.w - 2; i += 2) {
      if (mask[i]) {
        dst[i] = s0;
      }

      if (mask[i + 1]) {
        dst[i + 1] = s0;
      }
    }

    for (; i < size.w; i++) {
      if (mask[i]) {
        dst[i] = s0;
      }
    }

    mask += maskstep;
  }
}