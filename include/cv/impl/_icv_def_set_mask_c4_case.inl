
{
  for (j = 0; j < size.h; ++j) {
    arrtype* dst = (arrtype*)(((uchar*)dst0) + j * dststep);

    for (i = 0; i < size.w; i++) {
      if (mask[i]) {
        dst[i * 4] = s0;
        dst[i * 4 + 1] = s1;
        dst[i * 4 + 2] = s2;
        dst[i * 4 + 3] = s3;
      }
    }

    mask += maskstep;
  }
}