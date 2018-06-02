
//#define icv_def_un_ari_op_2d(__op__, name, arrtype, worktype, cast_macro)
//static CStatus name(const arrtype* src, int srcstep, arrtype* dst, int dststep,CSize size, const worktype* scalar)
{
  int i, len;

  if (!mask && size.w == srcstep && size.w == dststep) {
    len = size.h * size.w;

    for (i = 0; i < len; ++i) {
      worktype t0 = __op__(*(scalar), (src)[i]);
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

    for (; size.h--; src += srcstep, dst += dststep) {
      const arrtype* tsrc = src;
      arrtype* tdst = dst;

      for (; ((len) -= 12) >= 0; (dst) += 12, (tsrc) += 12) {
        worktype t0 = __op__((scalar)[0], (tsrc)[0]);
        worktype t1 = __op__((scalar)[1], (tsrc)[1]);

        (dst)[0] = cast_macro(t0);
        (dst)[1] = cast_macro(t1);

        t0 = __op__((scalar)[2], (tsrc)[2]);
        t1 = __op__((scalar)[3], (tsrc)[3]);

        (dst)[2] = cast_macro(t0);
        (dst)[3] = cast_macro(t1);

        t0 = __op__((scalar)[4], (tsrc)[4]);
        t1 = __op__((scalar)[5], (tsrc)[5]);

        (dst)[4] = cast_macro(t0);
        (dst)[5] = cast_macro(t1);

        t0 = __op__((scalar)[6], (tsrc)[6]);
        t1 = __op__((scalar)[7], (tsrc)[7]);

        (dst)[6] = cast_macro(t0);
        (dst)[7] = cast_macro(t1);

        t0 = __op__((scalar)[8], (tsrc)[8]);
        t1 = __op__((scalar)[9], (tsrc)[9]);

        (dst)[8] = cast_macro(t0);
        (dst)[9] = cast_macro(t1);

        t0 = __op__((scalar)[10], (tsrc)[10]);
        t1 = __op__((scalar)[11], (tsrc)[11]);

        (dst)[10] = cast_macro(t0);
        (dst)[11] = cast_macro(t1);
      }

      for ((len) += 12, i = 0; i < (len); i++) {
        worktype t0 = __op__((scalar)[i], (tsrc)[i]);
        (dst)[i] = cast_macro(t0);
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