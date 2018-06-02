{
  int i, fast_int_mode = 0;
  uint64 temp = *rng;
  worktype param[24] = {0};
  int iparam[24] = {0};

  if (is_int) {
    int t0, t1, diff;
    int iparam[24] = {0};

    for (i = 0, fast_int_mode = 1; i < channels; i++) {
      t0 = iparam[i] = (int)(param1[i]);
      t1 = iparam[12 + i] = (int)(param2[i]) - t0;
      diff = (int)param1[i] - (int)param2[i];

      fast_int_mode &= (INT_MIN <= diff) && (diff <= INT_MAX) && ((t1 & (t1 - 1)) == 0);
    }
  }

  if (fast_int_mode) {
    int small_flag = (iparam[12] | iparam[13] | iparam[14] | iparam[15]) <= 255;

    for (i = 0; i < channels; i++) {
      iparam[12 + i]--;
    }

    for (; i < 12; i++) {
      int t0 = iparam[i - channels];
      int t1 = iparam[12 + i - channels];

      iparam[i] = t0;
      iparam[12 + i] = t1;
    }

    for (; size.h--; arr += step) {
      int i, k = 3;
      const int* p = iparam;

      if (!small_flag) {
        for (i = 0; i <= size.w - 4; i += 4) {
          unsigned t0, t1;

          temp = ICC_RNG_NEXT(temp);
          t0 = ((unsigned)temp & p[i + 12]) + p[i];
          temp = ICC_RNG_NEXT(temp);
          t1 = ((unsigned)temp & p[i + 13]) + p[i + 1];
          arr[i] = cast_macro2((int)t0);
          arr[i + 1] = cast_macro2((int)t1);

          temp = ICC_RNG_NEXT(temp);
          t0 = ((unsigned)temp & p[i + 14]) + p[i + 2];
          temp = ICC_RNG_NEXT(temp);
          t1 = ((unsigned)temp & p[i + 15]) + p[i + 3];
          arr[i + 2] = cast_macro2((int)t0);
          arr[i + 3] = cast_macro2((int)t1);

          if (!--k) {
            k = 3;
            p -= 12;
          }
        }
      }
      else {
        for (i = 0; i <= size.w - 4; i += 4) {
          unsigned t0, t1, t;

          temp = ICC_RNG_NEXT(temp);
          t = (unsigned)temp;
          t0 = (t & p[i + 12]) + p[i];
          t1 = ((t >> 8) & p[i + 13]) + p[i + 1];
          arr[i] = cast_macro2((int)t0);
          arr[i + 1] = cast_macro2((int)t1);

          t0 = ((t >> 16) & p[i + 14]) + p[i + 2];
          t1 = ((t >> 24) & p[i + 15]) + p[i + 3];
          arr[i + 2] = cast_macro2((int)t0);
          arr[i + 3] = cast_macro2((int)t1);

          if (!--k) {
            k = 3;
            p -= 12;
          }
        }
      }

      for (; i < size.w; i++) {
        unsigned t0;
        temp = ICC_RNG_NEXT(temp);

        t0 = ((unsigned)temp & p[i + 12]) + p[i];
        arr[i] = cast_macro2((int)t0);
      }
    }

    *rng = temp;
  }
  else {
    for (i = 0; i < channels; i++) {
      worktype t0 = param1[i];
      worktype t1 = param2[i];

      param[i] = t0 - (t1 - t0);
      param[12 + i] = t1 - t0;
    }

    for (; size.h--; arr += step) {
      int i, k = 3;
      const worktype* p = param;

      for (i = 0; i <= size.w - 4; i += 4) {
        worktype f0, f1;
        suf32_t t0, t1;

        temp = ICC_RNG_NEXT(temp);
        t0.u = ICC_CVT_FLT(temp);
        temp = ICC_RNG_NEXT(temp);
        t1.u = ICC_CVT_FLT(temp);
        f0 = cast_macro1(t0.f * p[i + 12] + p[i]);
        f1 = cast_macro1(t1.f * p[i + 13] + p[i + 1]);
        arr[i] = cast_macro2(f0);
        arr[i + 1] = cast_macro2(f1);

        temp = ICC_RNG_NEXT(temp);
        t0.u = ICC_CVT_FLT(temp);
        temp = ICC_RNG_NEXT(temp);
        t1.u = ICC_CVT_FLT(temp);
        f0 = cast_macro1(t0.f * p[i + 14] + p[i + 2]);
        f1 = cast_macro1(t1.f * p[i + 15] + p[i + 3]);
        arr[i + 2] = cast_macro2(f0);
        arr[i + 3] = cast_macro2(f1);

        if (!--k) {
          k = 3;
          p -= 12;
        }
      }

      for (; i < size.w; i++) {
        worktype f0;
        suf32_t t0;

        temp = ICC_RNG_NEXT(temp);
        t0.u = ICC_CVT_FLT(temp);
        f0 = cast_macro1(t0.f * p[i + 12] + p[i]);
        arr[i] = cast_macro2(f0);
      }
    }

    *rng = temp;
  }
}
#undef cast_macro1
#undef cast_macro2
