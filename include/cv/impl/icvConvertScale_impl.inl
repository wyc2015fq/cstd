{
  int i;

  for (; size.h--; src += srcstep, dst += dststep) {
    for (i = 0; i <= size.w - 4; i += 4) {
      dst[i] = cast_macro(src[i]);
      dst[i + 1] = cast_macro(src[i + 1]);
      dst[i + 2] = cast_macro(src[i + 2]);
      dst[i + 3] = cast_macro(src[i + 3]);
    }

    for (; i < size.w; i++) {
      dst[i] = cast_macro(src[i]);
    }
  }
}
#undef cast_macro
