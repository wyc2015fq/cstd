
{
  int i, len;

  if (!mask && size.w == src1step && size.w == src2step && size.w == dststep) {
    len = size.h * size.w;

    for (i = 0; i < len; ++i) {
      worktype t0 = __op__((src1)[i], (src2)[i]);
      (dst)[i] = cast_macro(t0);
    }
  }
  else {
    arrtype* tmp_buf = 0;
    arrtype* tdst = dst;
    CSize dsize = cSize(size.w, 1);
    len = size.w;

    if (mask) {
      tdst = tmp_buf = MALLOC(arrtype, len);
    }

    for (; size.h--; src1 += src1step, src2 += src2step, dst += dststep) {
      for (i = 0; i <= (len) - 4; i += 4) {
        worktype t0 = __op__((src1)[i], (src2)[i]);
        worktype t1 = __op__((src1)[i + 1], (src2)[i + 1]);

        (tdst)[i] = cast_macro(t0);
        (tdst)[i + 1] = cast_macro(t1);

        t0 = __op__((src1)[i + 2], (src2)[i + 2]);
        t1 = __op__((src1)[i + 3], (src2)[i + 3]);

        (tdst)[i + 2] = cast_macro(t0);
        (tdst)[i + 3] = cast_macro(t1);
      }

      for (; i < (len); i++) {
        worktype t0 = __op__((src1)[i], (src2)[i]);
        (tdst)[i] = cast_macro(t0);
      }

      if (mask) {
        cvCopyMask(tdst, 1, dst, 1, mask, 1, dsize, cn * sizeof(arrtype));
        mask += maskstep;
      }
    }

    if (mask) {
      FREE(tmp_buf);
    }
  }
}
#undef __op__
#undef cast_macro
