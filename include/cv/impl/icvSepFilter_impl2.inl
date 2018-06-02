
{
  int width = size.w;
  int height = size.h;
  int y, x;
  int nxoff = 0, nyoff = 0;
  srctype* tmp_src = 0;

  worktype* row_buf = 0;
  nxoff = (size.w + ksize.w) * cn;
  nyoff = size.h + ksize.h;
  width *= cn;

  borderType = BD_CONSTANT == borderType ? BD_REPLICATE : borderType;
  row_buf = MALLOC(worktype, nxoff);
  tmp_src = MALLOC(srctype, nxoff * nyoff);
  mem_fill_border(size.h, size.w, src, srcstep, nyoff, nxoff / cn, tmp_src, nxoff, cn, anchor.x, anchor.y, 0, borderType);
  srcstep = nxoff;
  src = tmp_src + anchor.y * nxoff + anchor.x * cn;

  for (y = 0; y < size.h; ++y, dst += dststep) {
    const srctype* psrc = tmp_src + (y + anchor.y) * srcstep;
    worktype* prow = row_buf;

    for (x = 0; x < nxoff - 3; x += 4) {
      update_col_macro((prow + x + 0), (psrc + x + 0), srcstep);
      update_col_macro((prow + x + 1), (psrc + x + 1), srcstep);
      update_col_macro((prow + x + 2), (psrc + x + 2), srcstep);
      update_col_macro((prow + x + 3), (psrc + x + 3), srcstep);
    }

    for (; x < nxoff; x++) {
      update_col_macro((prow + x), (psrc + x), srcstep);
    }

    prow += anchor.x * cn;

    for (x = 0; x < width - 3; x += 4) {
      update_row_macro((dst + x + 0), (prow + x + 0), cn);
      update_row_macro((dst + x + 1), (prow + x + 1), cn);
      update_row_macro((dst + x + 2), (prow + x + 2), cn);
      update_row_macro((dst + x + 3), (prow + x + 3), cn);
    }

    for (; x < width; x++) {
      update_row_macro((dst + x), (prow + x), cn);
    }
  }

  FREE(tmp_src);
  FREE(row_buf);
}

#undef update_extr_macro
#undef toggle_macro
