
{
  int x, y, k;
  double  A12 = matrix[1], b1 = matrix[2];
  double  A22 = matrix[4], b2 = matrix[5];

  for (y = 0; y < dsize.h; y++, dst += dststep) {
    int xs = CC_FLT_TO_FIX(A12 * y + b1, ICC_WARP_SHIFT);
    int ys = CC_FLT_TO_FIX(A22 * y + b2, ICC_WARP_SHIFT);

    for (x = 0; x < dsize.w; x++) {
      int ixs = xs + ofs[x * 2];
      int iys = ys + ofs[x * 2 + 1];
      worktype a = scale_alpha_macro(ixs & ICC_WARP_MASK);
      worktype b = scale_alpha_macro(iys & ICC_WARP_MASK);
      worktype p0, p1;
      ixs >>= ICC_WARP_SHIFT;
      iys >>= ICC_WARP_SHIFT;

      if ((unsigned)ixs < (unsigned)(ssize.w - 1) && (unsigned)iys < (unsigned)(ssize.h - 1)) {
        const arrtype* ptr = src + srcstep * iys + ixs * cn;

        for (k = 0; k < cn; k++) {
          p0 = mul_one_macro(ptr[k]) +
          a * (ptr[k + cn] - ptr[k]);
          p1 = mul_one_macro(ptr[k + srcstep]) +
          a * (ptr[k + cn + srcstep] - ptr[k + srcstep]);
          p0 = descale_macro(mul_one_macro(p0) + b * (p1 - p0));
          dst[x * cn + k] = (arrtype)cast_macro(p0);
        }
      }
      else if ((unsigned)(ixs + 1) < (unsigned)(ssize.w + 1) && (unsigned)(iys + 1) < (unsigned)(ssize.h + 1)) {
        int x0 = ICC_WARP_CLIP_X(ixs);
        int y0 = ICC_WARP_CLIP_Y(iys);
        int x1 = ICC_WARP_CLIP_X(ixs + 1);
        int y1 = ICC_WARP_CLIP_Y(iys + 1);
        const arrtype* ptr0, *ptr1, *ptr2, *ptr3;

        ptr0 = src + y0 * srcstep + x0 * cn;
        ptr1 = src + y0 * srcstep + x1 * cn;
        ptr2 = src + y1 * srcstep + x0 * cn;
        ptr3 = src + y1 * srcstep + x1 * cn;

        for (k = 0; k < cn; k++) {
          p0 = mul_one_macro(ptr0[k]) + a * (ptr1[k] - ptr0[k]);
          p1 = mul_one_macro(ptr2[k]) + a * (ptr3[k] - ptr2[k]);
          p0 = descale_macro(mul_one_macro(p0) + b * (p1 - p0));
          dst[x * cn + k] = (arrtype)cast_macro(p0);
        }
      }
      else if (fillval) {
        for (k = 0; k < cn; k++) {
          dst[x * cn + k] = fillval[k];
        }
      }
    }
  }

}

#undef scale_alpha_macro
#undef mul_one_macro
#undef descale_macro
#undef cast_macro
