
{
  int i;
  float buffer[RAND_BUF_SIZE];
  worktype param[24];

  for (i = 0; i < channels; i++) {
    param[i] = param1[i];
    param[12 + i] = param2[i];
  }

  for (; i < 12; i++) {
    param[i] = param[i - channels];
    param[12 + i] = param[12 + i - channels];
  }

  for (; size.h--; arr += step) {
    int i, j, len = RAND_BUF_SIZE;

    for (i = 0; i < size.w; i += RAND_BUF_SIZE) {
      int k = 3;
      const worktype* p = param;

      if (i + len > size.w) {
        len = size.w - i;
      }

      icvRandn_0_1_32f_C1R(buffer, len, rng);

      for (j = 0; j <= len - 4; j += 4) {
        worktype f0, f1;

        f0 = cast_macro1(buffer[j] * p[j + 12] + p[j]);
        f1 = cast_macro1(buffer[j + 1] * p[j + 13] + p[j + 1]);
        arr[i + j] = cast_macro2(f0);
        arr[i + j + 1] = cast_macro2(f1);

        f0 = cast_macro1(buffer[j + 2] * p[j + 14] + p[j + 2]);
        f1 = cast_macro1(buffer[j + 3] * p[j + 15] + p[j + 3]);
        arr[i + j + 2] = cast_macro2(f0);
        arr[i + j + 3] = cast_macro2(f1);

        if (--k == 0) {
          k = 3;
          p -= 12;
        }
      }

      for (; j < len; j++) {
        worktype f0 = cast_macro1(buffer[j] * p[j + 12] + p[j]);
        arr[i + j] = cast_macro2(f0);
      }
    }
  }

}

#undef cast_macro1
#undef cast_macro2

