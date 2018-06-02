
{
  int width = size.w;
  int height = size.h;
  int k, y, x;
  int nxoff = 0, nyoff = 0;
  int* xoff = 0;
  int* yoff = 0;
  int* xoff0 = 0;
  int* yoff0 = 0;
  const arrtype** el_ptr = 0;
  borderType = BD_CONSTANT == borderType ? BD_REPLICATE : borderType;

  if (CC_MORPH_RECT == element_shape) {
    uchar* row_buf = 0;
    nxoff = (size.w + ksize.w) * cn;
    nyoff = size.h + ksize.h;
    xoff0 = xoff = MALLOC(int, nxoff);
    yoff0 = yoff = MALLOC(int, nyoff);
    width *= cn;

    for (x = 0; x < nxoff; ++x) {
      xoff[x] = borderInterpolate(x / cn - anchor.x, size.w, borderType) * cn + x % cn;
    }

    for (y = 0; y < nyoff; ++y) {
      yoff[y] = borderInterpolate(y - anchor.y, size.h, borderType) * srcstep;
    }

    el_ptr = MALLOC(arrtype*, ksize.h);
    row_buf = MALLOC(arrtype, width);

    for (y = 0; y < size.h; ++y, dst += dststep) {
      for (k = 0; k < ksize.h; ++k) {
        el_ptr[k] = src + yoff[y + k];
      }

#if 0

      for (x = 0; x < width; x++) {
        const arrtype* sptr = el_ptr[0];
        worktype s0 = sptr[x], t0;

        for (k = 1; k < ksize.h; ++k) {
          update_extr_macro(s0, el_ptr[k][x]);
        }

        row_buf[x] = (arrtype)toggle_macro(s0);
      }

#else
      MEMCPY(row_buf, el_ptr[0], width);

      for (k = 1; k < ksize.h; ++k) {
        const arrtype* sptr = el_ptr[k];

        for (x = 0; x < width; x++) {
          update_extr_macro(row_buf[x], sptr[x]);
        }
      }

#endif

      for (x = 0; x < width; x++) {
        int* xoffx = xoff + x;
        worktype s0 = row_buf[xoffx[0]], t0;

        for (k = cn; k < ksize.w * cn; k += cn) {
          t0 = row_buf[xoffx[k]];
          update_extr_macro(s0, t0);
        }

        dst[x] = s0;
      }
    }

    FREE(el_ptr);
    FREE(xoff0);
    FREE(yoff0);
    FREE(row_buf);
  }
  else {
    IRECT bound;
    IPOINT* el_sparse = 0;
    int el_count = 0;
    el_sparse = MALLOC(IPOINT, ksize.w * ksize.h);
    el_count = init_binary_element(ksize.w, ksize.h, element_shape, anchor, 0, 0, el_sparse);
    bound = iRECT(el_sparse[0].x, el_sparse[0].y, el_sparse[0].x, el_sparse[0].y);

    for (k = 1; k < el_count; k++) {
      bound.l = MIN(bound.l, el_sparse[k].x);
      bound.r = MAX(bound.r, el_sparse[k].x);
      bound.t = MIN(bound.t, el_sparse[k].y);
      bound.b = MAX(bound.b, el_sparse[k].y);
    }

    nxoff = (size.w + bound.r - bound.l + 1) * cn;
    nyoff = size.h + bound.b - bound.t + 1;
    xoff0 = xoff = MALLOC(int, nxoff);
    yoff0 = yoff = MALLOC(int, nyoff);
    width *= cn;

    for (x = 0; x < nxoff / cn; ++x) {
      xoff[x * cn] = borderInterpolate(x + bound.l, size.w, borderType) * cn;

      for (k = 1; k < cn; ++k) {
        xoff[x * cn + k] = xoff[x * cn] + k;
      }
    }

    xoff -= bound.l * cn;

    for (y = 0; y < nyoff; ++y) {
      yoff[y] = borderInterpolate(y + bound.t, size.h, borderType) * srcstep;
    }

    yoff -= bound.t;

    el_ptr = MALLOC(arrtype*, el_count);

    for (k = 0; k < el_count; ++k) {
      el_sparse[k].x *= cn;
    }

    for (y = 0; y < size.h; ++y, dst += dststep) {
      for (k = 0; k < el_count; ++k) {
        el_ptr[k] = src + yoff[y + el_sparse[k].y] + xoff[el_sparse[k].x];
      }

      x = 0;

      for (; x < -bound.l * cn; x++) {
        int* xoffx = xoff + x;
        const arrtype* sptr = el_ptr[0] + xoffx[el_sparse[0].x];
        worktype s0 = sptr[0], t0;

        for (k = 1; k < el_count; k++) {
          t0 = el_ptr[k][xoffx[el_sparse[k].x]];
          update_extr_macro(s0, t0);
        }

        dst[x] = (arrtype)toggle_macro(s0);
      }

      for (k = 0; k < el_count; ++k) {
        el_ptr[k] = src + yoff[y + el_sparse[k].y] + xoff[el_sparse[k].x];
      }

      for (; x <= width - 4 - bound.r * cn; x += 4) {
        int* xoffx = xoff + x;
        const arrtype* sptr = el_ptr[0];
        worktype s0 = sptr[x], s1 = sptr[x + 1], s2 = sptr[x + 2], s3 = sptr[x + 3], t;
#if 0

        for (k = 1; k < el_count; k++) {
          sptr = el_ptr[k];
          t = sptr[xoffx[el_sparse[k].x]];
          update_extr_macro(s0, t);
          t = sptr[xoffx[el_sparse[k].x + 1]];
          update_extr_macro(s1, t);
          t = sptr[xoffx[el_sparse[k].x + 2]];
          update_extr_macro(s2, t);
          t = sptr[xoffx[el_sparse[k].x + 3]];
          update_extr_macro(s3, t);
        }

#else

        for (k = 1; k < el_count; k++) {
          sptr = el_ptr[k];
          t = sptr[x];
          update_extr_macro(s0, t);
          t = sptr[x + 1];
          update_extr_macro(s1, t);
          t = sptr[x + 2];
          update_extr_macro(s2, t);
          t = sptr[x + 3];
          update_extr_macro(s3, t);
        }

#endif

        dst[x] = (arrtype)toggle_macro(s0);
        dst[x + 1] = (arrtype)toggle_macro(s1);
        dst[x + 2] = (arrtype)toggle_macro(s2);
        dst[x + 3] = (arrtype)toggle_macro(s3);
      }

      for (; x < width; x++) {
        int* xoffx = xoff + x;
        const arrtype* sptr = el_ptr[0] + xoffx[el_sparse[0].x];
        worktype s0 = sptr[0], t0;

        for (k = 1; k < el_count; k++) {
          t0 = el_ptr[k][xoffx[el_sparse[k].x]];
          update_extr_macro(s0, t0);
        }

        dst[x] = (arrtype)toggle_macro(s0);
      }
    }

    FREE(el_sparse);
    FREE(xoff0);
    FREE(yoff0);
    FREE(el_ptr);
  }
}

#undef update_extr_macro
#undef toggle_macro
