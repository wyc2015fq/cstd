
typedef struct CvResizeAlpha {
  int idx;
  worktype alpha;
} CvResizeAlpha;

{
  // true "area" method for the cases (scale_x > 1 & scale_y < 1) and
  // (scale_x < 1 & scale_y > 1) is not implemented.
  // instead, it is emulated via some variant of bilinear interpolation.
  int method = 0;//CC_INTER_AREA;
  void* temp_buf = 0;
  float scale_x = (float)ssize.w / dsize.w;
  float scale_y = (float)ssize.h / dsize.h;
  int iscale_x = cRound(scale_x);
  int iscale_y = cRound(scale_y);
  float inv_scale_x = (float)dsize.w / ssize.w;
  float inv_scale_y = (float)dsize.h / ssize.h;
  int xmax = dsize.w, width = dsize.w * cn, buf_size;
  worktype* buf0, *buf1;
  CvResizeAlpha* xofs, *yofs;
  int area_mode = method == CC_INTER_AREA;
  float fx, fy;
  int prev_sy0 = -1, prev_sy1 = -1;
  int k, sx, sy, dx, dy;

  buf_size = width * 2 * sizeof(float) + (width + dsize.h) * sizeof(CvResizeAlpha);
  temp_buf = buf0 = (worktype*)cAlloc(buf_size);

  buf1 = buf0 + width;
  xofs = (CvResizeAlpha*)(buf1 + width);
  yofs = xofs + width;

  for (dx = 0; dx < dsize.w; dx++) {
    if (!area_mode) {
      fx = (float)((dx + 0.5) * scale_x - 0.5);
      sx = cFloor(fx);
      fx -= sx;
    }
    else {
      sx = cFloor(dx * scale_x);
      fx = (dx + 1) - (sx + 1) * inv_scale_x;
      fx = fx <= 0 ? 0.f : fx - cFloor(fx);
    }

    if (sx < 0) {
      fx = 0, sx = 0;
    }

    if (sx >= ssize.w - 1) {
      fx = 0, sx = ssize.w - 1;

      if (xmax >= dsize.w) {
        xmax = dx;
      }
    }

    for (k = 0, sx *= cn; k < cn; k++) {
      xofs[dx * cn + k].idx = sx + k;
      xofs[dx * cn + k].alpha = div_one_macro(fx);
    }
  }

  for (dy = 0; dy < dsize.h; dy++) {
    if (!area_mode) {
      fy = (float)((dy + 0.5) * scale_y - 0.5);
      sy = cFloor(fy);
      fy -= sy;

      if (sy < 0) {
        sy = 0, fy = 0;
      }
    }
    else {
      sy = cFloor(dy * scale_y);
      fy = (dy + 1) - (sy + 1) * inv_scale_y;
      fy = fy <= 0 ? 0.f : fy - cFloor(fy);
    }

    yofs[dy].idx = sy;
    yofs[dy].alpha = div_one_macro(fy);
  }

  dsize.w *= cn;
  xmax *= cn;

  for (dy = 0; dy < dsize.h; dy++, dst += dststep) {
    worktype fy = yofs[dy].alpha, *swap_t;
    int sy0 = yofs[dy].idx, sy1 = sy0 + (fy > 0 && sy0 < ssize.h - 1);

    if (sy0 == prev_sy0 && sy1 == prev_sy1) {
      k = 2;
    }
    else if (sy0 == prev_sy1) {
      CC_SWAP(buf0, buf1, swap_t);
      k = 1;
    }
    else {
      k = 0;
    }

    for (; k < 2; k++) {
      worktype* _buf = k == 0 ? buf0 : buf1;
      const arrtype* _src;
      int sy = k == 0 ? sy0 : sy1;

      if (k == 1 && sy1 == sy0) {
        memcpy(buf1, buf0, dsize.w * sizeof(buf0[0]));
        continue;
      }

      _src = src + sy * srcstep;

      for (dx = 0; dx < xmax; dx++) {
        int sx = xofs[dx].idx;
        worktype fx = xofs[dx].alpha;
        worktype t = _src[sx];
        _buf[dx] = mul_one_macro(t) + fx * (_src[sx + cn] - t);
      }

      for (; dx < dsize.w; dx++) {
        _buf[dx] = mul_one_macro(_src[xofs[dx].idx]);
      }
    }

    prev_sy0 = sy0;
    prev_sy1 = sy1;

    if (sy0 == sy1) {
      for (dx = 0; dx < dsize.w; dx++) {
        dst[dx] = (arrtype)descale_macro(mul_one_macro(buf0[dx]));
      }
    }
    else {
      for (dx = 0; dx < dsize.w; dx++) {
        dst[dx] = (arrtype)descale_macro(mul_one_macro(buf0[dx]) + fy * (buf1[dx] - buf0[dx]));
      }
    }
  }

  FREE(temp_buf);
}

#undef div_one_macro
#undef mul_one_macro
#undef descale_macro