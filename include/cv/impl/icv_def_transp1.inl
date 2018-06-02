{
  const arrtype* src = (const arrtype*)src0;
  arrtype* dst = (arrtype*)dst0;
  const arrtype* src1;
  arrtype* dst1, t0, t1;
  int x, y;
  srcstep /= sizeof(src[0]);
  dststep /= sizeof(dst[0]);
  for (y = 0; y <= height - 2; y += 2, src += 2 * srcstep, dst += 2) {
    src1 = src + srcstep;
    dst1 = dst;
    for (x = 0; x <= width - 2; x += 2, dst1 += dststep) {
      t0 = src[x], t1 = src1[x];
      dst1[0] = t0, dst1[1] = t1;
      dst1 += dststep;
      t0 = src[x + 1], t1 = src1[x + 1];
      dst1[0] = t0, dst1[1] = t1;
    }
    if (x < width) {
      t0 = src[x], t1 = src1[x];
      dst1[0] = t0, dst1[1] = t1;
    }
  }
  if (y < height) {
    dst1 = dst;
    for (x = 0; x <= width - 2; x += 2, dst1 += 2 * dststep) {
      t0 = src[x], t1 = src[x + 1];
      dst1[0] = t0, dst1[dststep] = t1;
    }
    if (x < width) {
      t0 = src[x], dst1[0] = t0;
    }
  }
}