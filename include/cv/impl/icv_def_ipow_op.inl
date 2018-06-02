{
  int i;
  for (i = 0; i < len; i++) {
    worktype a = 1, b = src[i];
    int p = power;

    while (p > 1) {
      if (p & 1) {
        a *= b;
      }

      b *= b;
      p >>= 1;
    }

    a *= b;
    dst[i] = cast_macro(a);
  }
}
#undef cast_macro
