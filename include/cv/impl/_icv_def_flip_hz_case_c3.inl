
for (j = 0; j < size.h; ++j)
{
  const arrtype* src = (const arrtype*)(((const uchar*)src0) + j * srcstep);
  arrtype* dst = (arrtype*)(((uchar*)dst0) + j * dststep);

  for (i = 0; i < (len + 1) / 2; i++) {
    arrtype t0 = src[i * 3];
    arrtype t1 = src[(len - i) * 3 - 3];
    dst[i * 3] = t1;
    dst[(len - i) * 3 - 3] = t0;
    t0 = src[i * 3 + 1];
    t1 = src[(len - i) * 3 - 2];
    dst[i * 3 + 1] = t1;
    dst[(len - i) * 3 - 2] = t0;
    t0 = src[i * 3 + 2];
    t1 = src[(len - i) * 3 - 1];
    dst[i * 3 + 2] = t1;
    dst[(len - i) * 3 - 1] = t0;
  }
}
