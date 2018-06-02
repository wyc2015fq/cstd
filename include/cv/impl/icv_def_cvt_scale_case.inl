
#define icv_def_cvt_scale_case(srctype, worktype, scale_macro, cast_macro, a, b)

{
  const srctype* _src = (const srctype*)src;
  srcstep /= sizeof(_src[0]);

  for (; size.h--; _src += srcstep, dst += dststep) {
    for (i = 0; i <= size.w - 4; i += 4) {
      worktype t0 = scale_macro((a) * _src[i] + (b));
      worktype t1 = scale_macro((a) * _src[i + 1] + (b));

      dst[i] = cast_macro(t0);
      dst[i + 1] = cast_macro(t1);

      t0 = scale_macro((a) * _src[i + 2] + (b));
      t1 = scale_macro((a) * _src[i + 3] + (b));

      dst[i + 2] = cast_macro(t0);
      dst[i + 3] = cast_macro(t1);
    }

    for (; i < size.w; i++) {
      worktype t0 = scale_macro((a) * _src[i] + (b));
      dst[i] = cast_macro(t0);
    }
  }
}

