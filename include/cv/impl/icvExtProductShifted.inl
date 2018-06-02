
{
  int x, y, dstsize = size.w * size.h;

  for (y = 0; y < size.h; y++, vec += vecstep, avg += avgstep) {
    for (x = 0; x < size.w; x++) {
      *tempbuf++ = load_macro(vec[x]) - avg[x];
    }
  }

  tempbuf -= dstsize;

  for (y = 0; y < dstsize; y++, dst += dststep) {
    double ty = tempbuf[y];

    for (x = 0; x <= y - 3; x += 4) {
      double t0 = dst[x] + ty * tempbuf[x];
      double t1 = dst[x + 1] + ty * tempbuf[x + 1];
      dst[x] = (avgtype)t0;
      dst[x + 1] = (avgtype)t1;
      t0 = dst[x + 2] + ty * tempbuf[x + 2];
      t1 = dst[x + 3] + ty * tempbuf[x + 3];
      dst[x + 2] = (avgtype)t0;
      dst[x + 3] = (avgtype)t1;
    }

    for (; x <= y; x++) {
      dst[x] = (avgtype)(dst[x] + ty * tempbuf[x]);
    }
  }
}
