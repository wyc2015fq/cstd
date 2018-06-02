
{
  // "area" method for (scale_x > 1 & scale_y > 1)
  float scale_x = (float)ssize.w / dsize.w;
  float scale_y = (float)ssize.h / dsize.h;
  int iscale_x = cRound(scale_x);
  int iscale_y = cRound(scale_y);

  if (fabs(scale_x - iscale_x) < DBL_EPSILON && fabs(scale_y - iscale_y) < DBL_EPSILON) {
    int sx, sy, dy, dx, k = 0;
    int area = iscale_x * iscale_y;
    float scale = 1.f / (iscale_x * iscale_y);
    int* ofs = (int*)cvStackAlloc((area + dsize.w * cn) * sizeof(int));
    int* xofs = ofs + area;

    //srcstep = srcstep / cn;

    for (sy = 0, k = 0; sy < iscale_y; sy++) {
      for (sx = 0; sx < iscale_x; sx++) {
        ofs[k++] = sy * srcstep + sx * cn;
      }
    }

    for (dx = 0; dx < dsize.w; dx++) {
      sx = dx * iscale_x * cn;

      for (k = 0; k < cn; k++) {
        xofs[dx * cn + k] = sx + k;
      }
    }

    dsize.w *= cn;

    for (dy = 0; dy < dsize.h; dy++, dst += dststep) {
      for (dx = 0; dx < dsize.w; dx++) {
        const arrtype* _src = src + dy * iscale_y * srcstep + xofs[dx];
        worktype sum = 0;

        for (k = 0; k <= area - 4; k += 4)
          sum += _src[ofs[k]] + _src[ofs[k + 1]] +
              _src[ofs[k + 2]] + _src[ofs[k + 3]];

        for (; k < area; k++) {
          sum += _src[ofs[k]];
        }

        dst[dx] = (arrtype)cast_macro(sum * scale);
      }
    }
  }
  else
  {
    int k, sx, sy, dx, cur_dy = 0;
    int buf_len = dsize.w * cn + 4, buf_size, xofs_count = 0;
    float scale = 1.f / (scale_x * scale_y);
    float * buf, *sum;
    CvDecimateAlpha* xofs;

    buf_size = buf_len * 2 * sizeof(float) + ssize.w * 2 * sizeof(CvDecimateAlpha);
    buf = (float*)MALLOC(uchar, buf_size);

    sum = buf + buf_len;
    xofs = (CvDecimateAlpha*)(sum + buf_len);

    for (dx = 0, k = 0; dx < dsize.w; dx++) {
      float fsx1 = dx * scale_x, fsx2 = fsx1 + scale_x;
      int sx1 = cCeil(fsx1), sx2 = cFloor(fsx2);

      assert((unsigned)sx1 < (unsigned)ssize.w);

      if (sx1 > fsx1) {
        assert(k < ssize.w * 2);
        xofs[k].di = dx * cn;
        xofs[k].si = (sx1 - 1) * cn;
        xofs[k++].alpha = (sx1 - fsx1) * scale;
      }

      for (sx = sx1; sx < sx2; sx++) {
        assert(k < ssize.w * 2);
        xofs[k].di = dx * cn;
        xofs[k].si = sx * cn;
        xofs[k++].alpha = scale;
      }

      if (fsx2 - sx2 > 1e-3) {
        assert(k < ssize.w * 2);
        assert((unsigned)sx2 < (unsigned)ssize.w);
        xofs[k].di = dx * cn;
        xofs[k].si = sx2 * cn;
        xofs[k++].alpha = (fsx2 - sx2) * scale;
      }
    }

    xofs_count = k;
    memset(sum, 0, buf_len * sizeof(float));
    memset(buf, 0, buf_len * sizeof(float));


    srcstep /= sizeof(src[0]);
    dststep /= sizeof(dst[0]);
    dsize.w *= cn;

    for (sy = 0; sy < ssize.h; sy++, src += srcstep) {
      if (cn == 1) {
        for (k = 0; k < xofs_count; k++) {
          int dxn = xofs[k].di;
          float alpha = xofs[k].alpha;
          buf[dxn] = buf[dxn] + load_macro(src[xofs[k].si]) * alpha;
        }
      }
      else if (cn == 2) {
        for (k = 0; k < xofs_count; k++) {
          int sxn = xofs[k].si;
          int dxn = xofs[k].di;
          float alpha = xofs[k].alpha;
          float t0 = buf[dxn] + load_macro(src[sxn]) * alpha;
          float t1 = buf[dxn + 1] + load_macro(src[sxn + 1]) * alpha;
          buf[dxn] = t0;
          buf[dxn + 1] = t1;
        }
      }
      else if (cn == 3) {
        for (k = 0; k < xofs_count; k++) {
          int sxn = xofs[k].si;
          int dxn = xofs[k].di;
          float alpha = xofs[k].alpha;
          float t0 = buf[dxn] + load_macro(src[sxn]) * alpha;
          float t1 = buf[dxn + 1] + load_macro(src[sxn + 1]) * alpha;
          float t2 = buf[dxn + 2] + load_macro(src[sxn + 2]) * alpha;
          buf[dxn] = t0;
          buf[dxn + 1] = t1;
          buf[dxn + 2] = t2;
        }
      }
      else {
        for (k = 0; k < xofs_count; k++) {
          int sxn = xofs[k].si;
          int dxn = xofs[k].di;
          float alpha = xofs[k].alpha;
          float t0 = buf[dxn] + load_macro(src[sxn]) * alpha;
          float t1 = buf[dxn + 1] + load_macro(src[sxn + 1]) * alpha;
          buf[dxn] = t0;
          buf[dxn + 1] = t1;
          t0 = buf[dxn + 2] + load_macro(src[sxn + 2]) * alpha;
          t1 = buf[dxn + 3] + load_macro(src[sxn + 3]) * alpha;
          buf[dxn + 2] = t0;
          buf[dxn + 3] = t1;
        }
      }

      if ((cur_dy + 1)*scale_y <= sy + 1 || sy == ssize.h - 1) {
        float beta = sy + 1 - (cur_dy + 1) * scale_y, beta1;
        beta = MAX(beta, 0);
        beta1 = 1 - beta;

        if (fabs(beta) < 1e-3) {
          for (dx = 0; dx < dsize.w; dx++) {
            dst[dx] = (arrtype)cast_macro(sum[dx] + buf[dx]);
            sum[dx] = buf[dx] = 0;
          }
        }
        else {
          for (dx = 0; dx < dsize.w; dx++) {
            dst[dx] = (arrtype)cast_macro(sum[dx] + buf[dx] * beta1);
            sum[dx] = buf[dx] * beta;
            buf[dx] = 0;
          }
        }

        dst += dststep;
        cur_dy++;
      }
      else {
        for (dx = 0; dx < dsize.w; dx += 2) {
          float t0 = sum[dx] + buf[dx];
          float t1 = sum[dx + 1] + buf[dx + 1];
          sum[dx] = t0;
          sum[dx + 1] = t1;
          buf[dx] = buf[dx + 1] = 0;
        }
      }
    }

    FREE(buf);
  }
}
#undef load_macro
#undef cast_macro

