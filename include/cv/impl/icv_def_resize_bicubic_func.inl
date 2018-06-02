
typedef struct CvResizeAlpha {
  int idx;
  int ialpha;
} CvResizeAlpha;

{
  void* temp_buf = 0;
  float scale_x = (float)ssize.w / dsize.w;
  float scale_y = (float)ssize.h / dsize.h;
  int iscale_x = cRound(scale_x);
  int iscale_y = cRound(scale_y);

  int k, dx, dy, sx, sy, sy2, ify;
  int prev_sy2 = -2;

  int width = dsize.w * cn, buf_size;
  int ifx, xmin = dsize.w, xmax = -1;
  CvResizeAlpha* xofs;
  float* buf[4];

  icvInitCubicCoeffTab();
  buf_size = width * (4 * sizeof(float) + sizeof(xofs[0]));
  temp_buf = buf[0] = (float*)MALLOC(uchar, buf_size);

  for (k = 1; k < 4; k++) {
    buf[k] = buf[k - 1] + width;
  }

  xofs = (CvResizeAlpha*)(buf[3] + width);

  for (dx = 0; dx < dsize.w; dx++) {
    float fx = dx * scale_x;
    sx = cFloor(fx);
    fx -= sx;
    ifx = cRound(fx * ICC_CUBIC_TAB_SIZE);

    if (sx - 1 >= 0 && xmin > dx) {
      xmin = dx;
    }

    if (sx + 2 < ssize.w) {
      xmax = dx + 1;
    }

    // at least one of 4 points should be within the image - to
    // be able to set other points to the same value. see the loops
    // for(dx = 0; dx < xmin; dx++) ... and for(; dx < width; dx++) ...
    if (sx < -2) {
      sx = -2;
    }
    else if (sx > ssize.w) {
      sx = ssize.w;
    }

    for (k = 0; k < cn; k++) {
      xofs[dx * cn + k].idx = sx * cn + k;
      xofs[dx * cn + k].ialpha = ifx;
    }
  }


  xmin *= cn;
  xmax *= cn;
  dsize.w *= cn;
  ssize.w *= cn;
  srcstep /= sizeof(src[0]);
  dststep /= sizeof(dst[0]);

  for (dy = 0; dy < dsize.h; dy++, dst += dststep) {
    float w0, w1, w2, w3;
    float fy, x, sum;
    float* row, *row0, *row1, *row2, *row3;
    int k1, k = 4;

    fy = dy * scale_y;
    sy = cFloor(fy);
    fy -= sy;
    ify = cRound(fy * ICC_CUBIC_TAB_SIZE);
    sy2 = sy + 2;

    if (sy2 > prev_sy2) {
      int delta = prev_sy2 - sy + 2;

      for (k = 0; k < delta; k++) {
        CC_SWAP(buf[k], buf[k + 4 - delta], row);
      }
    }

    for (sy += k - 1; k < 4; k++, sy++) {
      const arrtype* _src = src + sy * srcstep;

      row = buf[k];

      if (sy < 0) {
        continue;
      }

      if (sy >= ssize.h) {
        assert(k > 0);
        memcpy(row, buf[k - 1], dsize.w * sizeof(row[0]));
        continue;
      }

      for (dx = 0; dx < xmin; dx++) {
        int ifx = xofs[dx].ialpha, sx0 = xofs[dx].idx;
        sx = sx0 + cn * 2;

        while (sx >= ssize.w) {
          sx -= cn;
        }

        x = load_macro(_src[sx]);
        sum = x * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2 + 1];

        if ((unsigned)(sx = sx0 + cn) < (unsigned)ssize.w) {
          x = load_macro(_src[sx]);
        }

        sum += x * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2];

        if ((unsigned)(sx = sx0) < (unsigned)ssize.w) {
          x = load_macro(_src[sx]);
        }

        sum += x * icvCubicCoeffs[ifx * 2];

        if ((unsigned)(sx = sx0 - cn) < (unsigned)ssize.w) {
          x = load_macro(_src[sx]);
        }

        row[dx] = sum + x * icvCubicCoeffs[ifx * 2 + 1];
      }

      for (; dx < xmax; dx++) {
        int ifx = xofs[dx].ialpha;
        int sx0 = xofs[dx].idx;
        row[dx] = _src[sx0 - cn] * icvCubicCoeffs[ifx * 2 + 1] +
            _src[sx0] * icvCubicCoeffs[ifx * 2] +
            _src[sx0 + cn] * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2] +
            _src[sx0 + cn * 2] * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2 + 1];
      }

      for (; dx < dsize.w; dx++) {
        int ifx = xofs[dx].ialpha, sx0 = xofs[dx].idx;
        x = load_macro(_src[sx0 - cn]);
        sum = x * icvCubicCoeffs[ifx * 2 + 1];

        if ((unsigned)(sx = sx0) < (unsigned)ssize.w) {
          x = load_macro(_src[sx]);
        }

        sum += x * icvCubicCoeffs[ifx * 2];

        if ((unsigned)(sx = sx0 + cn) < (unsigned)ssize.w) {
          x = load_macro(_src[sx]);
        }

        sum += x * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2];

        if ((unsigned)(sx = sx0 + cn * 2) < (unsigned)ssize.w) {
          x = load_macro(_src[sx]);
        }

        row[dx] = sum + x * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2 + 1];
      }

      if (sy == 0) {
        for (k1 = 0; k1 < k; k1++) {
          memcpy(buf[k1], row, dsize.w * sizeof(row[0]));
        }
      }
    }

    prev_sy2 = sy2;

    row0 = buf[0];
    row1 = buf[1];
    row2 = buf[2];
    row3 = buf[3];

    w0 = icvCubicCoeffs[ify * 2 + 1];
    w1 = icvCubicCoeffs[ify * 2];
    w2 = icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ify) * 2];
    w3 = icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ify) * 2 + 1];

    for (dx = 0; dx < dsize.w; dx++) {
      worktype val = cast_macro1(row0[dx] * w0 + row1[dx] * w1 + row2[dx] * w2 + row3[dx] * w3);
      dst[dx] = cast_macro2(val);
    }
  }

  FREE(temp_buf);
}

#undef load_macro
#undef cast_macro1
#undef cast_macro2