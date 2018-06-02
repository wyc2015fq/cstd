
{
  int k, y, x, sum_count, width, height, buf_size, sumstep;
  uchar* tmp_buf = 0;
  int* xoff = 0;
  int modh = 0;
  worktype* sum_buf = 0;

  borderType = BD_CONSTANT == borderType ? BD_REPLICATE : borderType;

  ksize.h = MAX(1, ksize.h);
  ksize.w = MAX(1, ksize.w);

  modh = ksize.h + 2;
  sum_count = ksize.h * ksize.w;
  width = size.w + ksize.w;
  height = size.h + ksize.h;

  sumstep = (width + 1) * cn;
  buf_size = (ksize.h + 2) * sumstep * cn * sizeof(worktype) + width * sizeof(int);
  tmp_buf = MALLOC(uchar, buf_size);
  xoff = (int*)tmp_buf;
  sum_buf = (worktype*)(xoff + width);

  for (x = 0; x < width; ++x) {
    xoff[x] = borderInterpolate(x - anchor.x, size.w, borderType) * cn;
  }

  for (x = 0; x < sumstep; ++x) {
    sum_buf[x] = 0;
  }

  for (y = 0; y < height; ++y) {
    worktype* sum0 = sum_buf + (y % modh) * sumstep + cn;
    worktype* sum1 = sum_buf + ((y + 1) % modh) * sumstep + cn;
    int ty = borderInterpolate(y - anchor.y, size.h, borderType);
    const arrtype* src0 = src + srcstep * ty;

    for (k = 0; k < cn; k++, src0++, sum1++, sum0++) {
      worktype s = 0;
      sum1[-cn] = 0;

      for (x = 0; x < width; ++x) {
        worktype t = (src0[xoff[x]]);
        s += t;
        sum1[x * cn] = sum0[x * cn] + s;
      }
    }

    if (y >= ksize.h) {
      worktype* sum0 = sum_buf + ((y - ksize.h) % modh) * sumstep;
      worktype* sum1 = sum_buf + (y % modh) * sumstep;
      int p1 = ksize.w * cn;

      for (x = 0; x < size.w * cn; ++x, ++sum0, ++sum1) {
        dst[x] = (sum1[p1] - sum1[0] - sum0[p1] + sum0[0]) / sum_count;
      }

      dst += dststep;
    }
  }

  FREE(tmp_buf);
}
