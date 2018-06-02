
{
  for (; size.h--; src1 += step1, src2 += step2, dst += step) {
    int i;

    for (i = 0; i <= size.w - 4; i += 4) {
      worktype t0 = cast_macro1(load_macro((src1)[i]) * alpha + load_macro((src2)[i]) * beta + gamma);
      worktype t1 = cast_macro1(load_macro((src1)[i + 1]) * alpha + load_macro((src2)[i + 1]) * beta + gamma);

      (dst)[i] = cast_macro2(t0);
      (dst)[i + 1] = cast_macro2(t1);

      t0 = cast_macro1(load_macro((src1)[i + 2]) * alpha + load_macro((src2)[i + 2]) * beta + gamma);
      t1 = cast_macro1(load_macro((src1)[i + 3]) * alpha + load_macro((src2)[i + 3]) * beta + gamma);

      (dst)[i + 2] = cast_macro2(t0);
      (dst)[i + 3] = cast_macro2(t1);
    }

    for (; i < size.w; i++) {
      worktype t0 = cast_macro1(load_macro((src1)[i]) * alpha + load_macro((src2)[i]) * beta + gamma);
      (dst)[i] = cast_macro2(t0);
    }
  }

}

#undef load_macro
#undef cast_macro1
#undef cast_macro2


