
{
  int width = size.w;
  int height = size.h;
  int k, y, x;
  int nxoff = 0, nyoff = 0;
  srctype* tmp_src = 0;

  worktype* row_buf = 0;
  worktype* col_buf = 0;
  nxoff = (size.w + ksize.w) * cn;
  nyoff = size.h + ksize.h;
  width *= cn;

  borderType = BD_CONSTANT == borderType ? BD_REPLICATE : borderType;

  row_buf = MALLOC(worktype, nxoff);
  col_buf = MALLOC(worktype, nxoff);
  tmp_src = MALLOC(srctype, nxoff * nyoff);
  mem_fill_border(size.h, size.w, src, srcstep, nyoff, nxoff / cn, tmp_src, nxoff, cn, anchor.x, anchor.y, 0, borderType);
  srcstep = nxoff;
  src = tmp_src + anchor.y * nxoff + anchor.x * cn;

  for (y = 0; y < size.h; ++y, dst += dststep) {
    const srctype* psrc = tmp_src + y * srcstep;
    int t0 = ky[0];
#define update_extr_macro(a, b)  a += t0*(b)

    if (t0) {
      for (x = 0; x < nxoff; x++) {
        row_buf[x] = t0 * psrc[x];
      }
    }
    else {
      for (x = 0; x < nxoff; x++) {
        row_buf[x] = 0;
      }
    }

    for (k = 1; k < ksize.h; ++k) {
      const srctype* sptr = psrc + k * srcstep;
      int t0 = ky[k];

      if (t0) {
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
    }

    t0 = kx[0];

    if (t0) {
      for (x = 0; x < width; x++) {
        col_buf[x] = t0 * row_buf[x];
      }
    }
    else {
      for (x = 0; x < width; x++) {
        col_buf[x] = 0;
      }
    }

    for (k = 1; k < ksize.w; k++) {
      const worktype* sptr = row_buf + k * cn;
      int t0 = kx[k];

      if (t0) {
        for (x = 0; x < width - 3; x += 4) {
          update_extr_macro(col_buf[x + 0], sptr[x + 0]);
          update_extr_macro(col_buf[x + 1], sptr[x + 1]);
          update_extr_macro(col_buf[x + 2], sptr[x + 2]);
          update_extr_macro(col_buf[x + 3], sptr[x + 3]);
        }

        for (; x < width; x++) {
          update_extr_macro(col_buf[x], sptr[x]);
        }
      }
    }

    if (shift) {
      for (x = 0; x < width; x++) {
        dst[x] = col_buf[x] >> shift;
      }
    }
    else {
      for (x = 0; x < width; x++) {
        dst[x] = col_buf[x];
      }
    }
  }

  FREE(tmp_src);
  FREE(row_buf);
  FREE(col_buf);
}

#undef update_extr_macro
#undef toggle_macro
