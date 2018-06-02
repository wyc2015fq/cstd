
{
  int width = size.w;
  int height = size.h;
  int k, y, x;
  int nxoff = 0, nyoff = 0;
  arrtype* tmp_src = 0;
  borderType = BD_CONSTANT == borderType ? BD_REPLICATE : borderType;

  if (CC_MORPH_RECT == element_shape) {
    arrtype* row_buf = 0;
    nxoff = (size.w + ksize.w) * cn;
    nyoff = size.h + ksize.h;
    width *= cn;

    row_buf = MALLOC(arrtype, nxoff);
    tmp_src = MALLOC(arrtype, nxoff * nyoff);
    mem_fill_border(size.h, size.w, src, srcstep, nyoff, nxoff / cn, tmp_src, nxoff, cn, anchor.x, anchor.y, 0, borderType);
    srcstep = nxoff;
    src = tmp_src + anchor.y * nxoff + anchor.x * cn;


    for (y = 0; y < size.h; ++y, dst += dststep) {
      const arrtype* psrc = tmp_src + y * srcstep;

      MEMCPY(row_buf, psrc, nxoff);

      for (k = 1; k < ksize.h; ++k) {
        const arrtype* sptr = psrc + k * srcstep;

        for (x = 0; x < nxoff - 3; x += 4) {
          update_extr_macro(row_buf[x + 0], sptr[x + 0]);
          update_extr_macro(row_buf[x + 1], sptr[x + 1]);
          update_extr_macro(row_buf[x + 2], sptr[x + 2]);
          update_extr_macro(row_buf[x + 3], sptr[x + 3]);
        }

        for (; x < nxoff; x++) {
          update_extr_macro(row_buf[x], sptr[x]);
        }
      }

      MEMCPY(dst, row_buf, width);

      for (k = cn; k < ksize.w * cn; k += cn) {
        const arrtype* sptr = row_buf + k;

        for (x = 0; x < width - 3; x += 4) {
          update_extr_macro(dst[x + 0], sptr[x + 0]);
          update_extr_macro(dst[x + 1], sptr[x + 1]);
          update_extr_macro(dst[x + 2], sptr[x + 2]);
          update_extr_macro(dst[x + 3], sptr[x + 3]);
        }

        for (; x < width; x++) {
          update_extr_macro(dst[x], sptr[x]);
        }
      }
    }

    FREE(row_buf);
    FREE(tmp_src);
  }
  else {
    IRECT bound;
    IPOINT* el_sparse = 0;
    int el_count = 0;
    el_sparse = MALLOC(IPOINT, ksize.w * ksize.h);
    el_count = morph_element_init(ksize.w, ksize.h, element_shape, anchor, 0, 0, el_sparse);
    bound = iRECT(el_sparse[0].x, el_sparse[0].y, el_sparse[0].x, el_sparse[0].y);

    for (k = 1; k < el_count; k++) {
      bound.l = MIN(bound.l, el_sparse[k].x);
      bound.r = MAX(bound.r, el_sparse[k].x);
      bound.t = MIN(bound.t, el_sparse[k].y);
      bound.b = MAX(bound.b, el_sparse[k].y);
    }

    nxoff = (size.w + bound.r - bound.l + 1) * cn;
    nyoff = size.h + bound.b - bound.t + 1;
    tmp_src = MALLOC(uchar, nxoff * nyoff);
    mem_fill_border(size.h, size.w, src, srcstep, nyoff, nxoff / cn, tmp_src, nxoff, cn, -bound.l, -bound.t, 0, borderType);
    srcstep = nxoff;
    src = tmp_src - bound.t * nxoff - bound.l * cn;
    width *= cn;

    for (k = 0; k < el_count; ++k) {
      el_sparse[k].x *= cn;
    }

    for (y = 0; y < size.h; ++y, dst += dststep) {
      const arrtype* sptr;
      k = 0;
      sptr = src + (y + el_sparse[k].y) * srcstep + el_sparse[k].x;
      MEMCPY(dst, sptr, width);

      for (k = 1; k < el_count; k++) {
        sptr = src + (y + el_sparse[k].y) * srcstep + el_sparse[k].x;

        for (x = 0; x < width - 3; x += 4) {
          update_extr_macro(dst[x + 0], sptr[x + 0]);
          update_extr_macro(dst[x + 1], sptr[x + 1]);
          update_extr_macro(dst[x + 2], sptr[x + 2]);
          update_extr_macro(dst[x + 3], sptr[x + 3]);
        }

        for (; x < width; x++) {
          update_extr_macro(dst[x], sptr[x]);
        }
      }
    }

    FREE(el_sparse);
    FREE(tmp_src);
  }
}

#undef update_extr_macro
#undef toggle_macro
