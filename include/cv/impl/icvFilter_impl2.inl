
{
  int width = size.w;
  int height = size.h;
  int y, x;
  int nxoff = 0, nyoff = 0;
  srctype* tmp_src = 0;

  nxoff = (size.w + ksize.w) * cn;
  nyoff = size.h + ksize.h;
  width *= cn;

  borderType = BD_CONSTANT == borderType ? BD_REPLICATE : borderType;
  tmp_src = MALLOC(srctype, nxoff * nyoff);
  mem_fill_border(size.h, size.w, src, srcstep, nyoff, nxoff / cn, tmp_src, nxoff, cn, anchor.x, anchor.y, 0, borderType);
  srcstep = nxoff;
  src = tmp_src + anchor.y * nxoff + anchor.x * cn;

  for (y = 0; y < size.h; ++y, dst += dststep, src += srcstep) {
    const srctype* psrc = src;

    for (x = 0; x < width - 3; x += 4) {
      update_macro((dst + x + 0), (psrc + x + 0), srcstep, cn);
      update_macro((dst + x + 1), (psrc + x + 1), srcstep, cn);
      update_macro((dst + x + 2), (psrc + x + 2), srcstep, cn);
      update_macro((dst + x + 3), (psrc + x + 3), srcstep, cn);
    }

    for (; x < width; x++) {
      update_macro((dst + x), (psrc + x), srcstep, cn);
    }
  }

  FREE(tmp_src);
}

#undef update_extr_macro
#undef toggle_macro
