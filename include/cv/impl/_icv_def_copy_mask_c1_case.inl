{
  for (j = 0; j < size.h; ++j) {
    const arrtype* src = (const arrtype*)(((const uchar*)src0) + j * srcstep);
    arrtype* dst = (arrtype*)(((uchar*)dst0) + j * dststep);

    for (i = 0; i <= size.w - 2; i += 2) {
      if (mask[i]) {
        dst[i] = src[i];
      }

      if (mask[i + 1]) {
        dst[i + 1] = src[i + 1];
      }
    }

    for (; i < size.w; i++) {
      if (mask[i]) {
        dst[i] = src[i];
      }
    }

    mask += maskstep;
  }
}
