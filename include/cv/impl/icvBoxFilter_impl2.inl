
{
  int i, k, y, x, sum_count, width, height, buf_size;
  uchar* tmp_buf = 0;
  int* xoff = 0;
  worktype* sum_buf = 0;

  borderType = BD_CONSTANT == borderType ? BD_REPLICATE : borderType;

  ksize.h = MAX(1, ksize.h);
  ksize.w = MAX(1, ksize.w);

  sum_count = ksize.h * ksize.w;
  width = size.w + ksize.w;
  height = size.h + ksize.h;
  //srcstep /= sizeof(src[0]);
  //dststep /= sizeof(dst[0]);

  buf_size = 2 * size.w * cn * sizeof(worktype) + width * sizeof(int);
  tmp_buf = MALLOC(uchar, buf_size);
  xoff = (int*)tmp_buf;
  sum_buf = (worktype*)(xoff + width);

  for (x = 0; x < width; ++x) {
    xoff[x] = borderInterpolate(x - anchor.x, size.w, borderType) * cn;
  }

  for (x = 0; x < 2 * size.w* cn; ++x) {
    sum_buf[x] = 0;
  }

  for (y = 0; y < height; ++y) {
    for (i = 0; i < 2; ++i) {
      if ((0 == i && y >= ksize.h) || i) {
        worktype* sum0 = sum_buf + i * size.w * cn;
        int ay = i ? y - anchor.y : y - anchor.y - ksize.h;
        int ty = borderInterpolate(ay, size.h, borderType);
        const arrtype* src0 = src + srcstep * ty;

        for (k = 0; k < cn; k++, src0++, sum0++) {
          worktype s = 0;
          worktype* sum = sum0;

          for (x = 0; x < ksize.w; ++x) {
            s += src0[xoff[x]];
          }

          sum[0] += s;

          for (sum += cn; x < width - 1; ++x, sum += cn) {
            s += (worktype)src0[xoff[x]] - src0[xoff[x - ksize.w]];
            sum[0] += s;
            ASSERT(sum[0] > 0);
          }
        }
      }
    }

    if (y >= ksize.h) {
      worktype* sum0 = sum_buf;
      worktype* sum1 = sum_buf + size.w * cn;
      x = 0;

      for (; x < size.w * cn; ++x) {
        dst[x] = (sum1[x] - sum0[x]) / sum_count;
      }

      dst += dststep;
    }
  }

  FREE(tmp_buf);
}