
{
  for (; size.h--; src += srcstep, dst += dststep) {
    int i;

    for (i = 0; i <= size.w - 4; i += 4) {
      worktype t0 = scale_macro((a) * src[i] + (b));
      worktype t1 = scale_macro((a) * src[i + 1] + (b));

      t0 = (worktype)abs_macro(t0);
      t1 = (worktype)abs_macro(t1);

      dst[i] = cast_macro(t0);
      dst[i + 1] = cast_macro(t1);

      t0 = scale_macro((a) * src[i + 2] + (b));
      t1 = scale_macro((a) * src[i + 3] + (b));

      t0 = (worktype)abs_macro(t0);
      t1 = (worktype)abs_macro(t1);

      dst[i + 2] = cast_macro(t0);
      dst[i + 3] = cast_macro(t1);
    }

    for (; i < size.w; i++) {
      worktype t0 = scale_macro((a) * src[i] + (b));
      t0 = (worktype)abs_macro(t0);
      dst[i] = cast_macro(t0);
    }
  }
}

#undef scale_macro
#undef abs_macro
#undef cast_macro
