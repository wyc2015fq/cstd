
for (i = 0; i < size.w; i++)
{
  if (mask[i]) {
    type t0 = src[i * 4];
    type t1 = src[i * 4 + 1];
    dst[i * 4] = t0;
    dst[i * 4 + 1] = t1;

    t0 = src[i * 4 + 2];
    t1 = src[i * 4 + 3];
    dst[i * 4 + 2] = t0;
    dst[i * 4 + 3] = t1;
  }
}

