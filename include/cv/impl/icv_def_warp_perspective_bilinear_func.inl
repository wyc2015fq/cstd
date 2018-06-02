
{
  int x, y, k;
  float A11 = (float)matrix[0], A12 = (float)matrix[1], A13 = (float)matrix[2];
  float A21 = (float)matrix[3], A22 = (float)matrix[4], A23 = (float)matrix[5];
  float A31 = (float)matrix[6], A32 = (float)matrix[7], A33 = (float)matrix[8];

  for (y = 0; y < dsize.h; y++, dst += dststep) {
    float xs0 = A12 * y + A13;
    float ys0 = A22 * y + A23;
    float ws = A32 * y + A33;

    for (x = 0; x < dsize.w; x++, xs0 += A11, ys0 += A21, ws += A31) {
      float inv_ws = 1.f / ws;
      float xs = xs0 * inv_ws;
      float ys = ys0 * inv_ws;
      int ixs = cFloor(xs);
      int iys = cFloor(ys);
      float a = xs - ixs;
      float b = ys - iys;
      float p0, p1;

      if ((unsigned)ixs < (unsigned)(ssize.w - 1) && (unsigned)iys < (unsigned)(ssize.h - 1)) {
        const arrtype* ptr = src + srcstep * iys + ixs * cn;

        for (k = 0; k < cn; k++) {
          p0 = load_macro(ptr[k]) + a * (load_macro(ptr[k + cn]) - load_macro(ptr[k]));
          p1 = load_macro(ptr[k + srcstep]) + a * (load_macro(ptr[k + cn + srcstep]) - load_macro(ptr[k + srcstep]));
          dst[x * cn + k] = (arrtype)cast_macro(p0 + b * (p1 - p0));
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
          p0 = load_macro(ptr0[k]) + a * (load_macro(ptr1[k]) - load_macro(ptr0[k]));
          p1 = load_macro(ptr2[k]) + a * (load_macro(ptr3[k]) - load_macro(ptr2[k]));
          dst[x * cn + k] = (arrtype)cast_macro(p0 + b * (p1 - p0));
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
#undef load_macro
#undef cast_macro
