
switch (flags)
{
case CC_INTER_LINEAR: {
  int i, j, k;
  ssize.w--;
  ssize.h--;

  for (i = 0; i < dsize.h; i++, dst += dststep, mapx += mxstep, mapy += mystep) {
    for (j = 0; j < dsize.w; j++) {
      int ix = cRound(mapx[j] * (1 << ICC_WARP_SHIFT));
      int iy = cRound(mapy[j] * (1 << ICC_WARP_SHIFT));
      int ifx = ix & ICC_WARP_MASK;
      int ify = iy & ICC_WARP_MASK;
      float x0, x1, y0, y1;
      ix >>= ICC_WARP_SHIFT;
      iy >>= ICC_WARP_SHIFT;

      x0 = icvLinearCoeffs[ifx * 2];
      x1 = icvLinearCoeffs[ifx * 2 + 1];
      y0 = icvLinearCoeffs[ify * 2];
      y1 = icvLinearCoeffs[ify * 2 + 1];

      if ((unsigned)ix < (unsigned)ssize.w && (unsigned)iy < (unsigned)ssize.h) {
        const arrtype* s = src + iy * srcstep + ix * cn;

        for (k = 0; k < cn; k++, s++) {
          float t0 = x1 * load_macro(s[0]) + x0 * load_macro(s[cn]);
          float t1 = x1 * load_macro(s[srcstep]) + x0 * load_macro(s[srcstep + cn]);
          dst[j * cn + k] = (arrtype)cast_macro1(y1 * t0 + y0 * t1);
        }
      }
      else if (fillval) {
        for (k = 0; k < cn; k++) {
          dst[j * cn + k] = fillval[k];
        }
      }
    }
  }
}
break;

case CC_INTER_CUBIC: {
  int i, j, k;
  ssize.w = MAX(ssize.w - 3, 0);
  ssize.h = MAX(ssize.h - 3, 0);

  for (i = 0; i < dsize.h; i++, dst += dststep,
      mapx += mxstep, mapy += mystep) {
    for (j = 0; j < dsize.w; j++) {
      int ix = cRound(mapx[j] * (1 << ICC_WARP_SHIFT));
      int iy = cRound(mapy[j] * (1 << ICC_WARP_SHIFT));
      int ifx = ix & ICC_WARP_MASK;
      int ify = iy & ICC_WARP_MASK;
      ix >>= ICC_WARP_SHIFT;
      iy >>= ICC_WARP_SHIFT;

      if ((unsigned)(ix - 1) < (unsigned)ssize.w && (unsigned)(iy - 1) < (unsigned)ssize.h) {
        for (k = 0; k < cn; k++) {
          const arrtype* s = src + (iy - 1) * srcstep + ix * cn + k;
          float t0, t1, t2, t3;
          worktype t;
          t0 = load_macro(s[-cn]) * icvCubicCoeffs[ifx * 2 + 1] + load_macro(s[0]) * icvCubicCoeffs[ifx * 2] + load_macro(s[cn]) * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2] + load_macro(s[cn * 2]) * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2 + 1];
          s += srcstep;
          t1 = load_macro(s[-cn]) * icvCubicCoeffs[ifx * 2 + 1] + load_macro(s[0]) * icvCubicCoeffs[ifx * 2] + load_macro(s[cn]) * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2] + load_macro(s[cn * 2]) * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2 + 1];
          s += srcstep;
          t2 = load_macro(s[-cn]) * icvCubicCoeffs[ifx * 2 + 1] + load_macro(s[0]) * icvCubicCoeffs[ifx * 2] + load_macro(s[cn]) * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2] + load_macro(s[cn * 2]) * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2 + 1];
          s += srcstep;
          t3 = load_macro(s[-cn]) * icvCubicCoeffs[ifx * 2 + 1] + load_macro(s[0]) * icvCubicCoeffs[ifx * 2] + load_macro(s[cn]) * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2] + load_macro(s[cn * 2]) * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx) * 2 + 1];
          t = cast_macro1(t0 * icvCubicCoeffs[ify * 2 + 1] +
              t1 * icvCubicCoeffs[ify * 2] +
              t2 * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ify) * 2] +
              t3 * icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ify) * 2 + 1]);

          dst[j * cn + k] = cast_macro2(t);
        }
      }
      else if (fillval)
        for (k = 0; k < cn; k++) {
          dst[j * cn + k] = fillval[k];
        }
    }
  }
}
break;

default:
  ASSERT(0);
  break;
}

#undef load_macro
#undef cast_macro1
#undef cast_macro2
