{
  double dparam[2][12];
  int iparam[2][12];
  void* param = dparam;
  int i, fast_int_mode = 0;
  int arr_step = 0;

  if (!rng) {
    CC_ERROR(CC_StsNullPtr, "Null pointer to RNG state");
  }

  size.w *= channels;

  if (disttype == CC_RAND_UNI) {
    if (depth <= CC_32S) {
      for (i = 0, fast_int_mode = 1; i < channels; i++) {
        int t0 = iparam[0][i] = cCeil(param1.val[i]);
        int t1 = iparam[1][i] = cFloor(param2.val[i]) - t0;
        double diff = param1.val[i] - param2.val[i];

        fast_int_mode &= INT_MIN <= diff && diff <= INT_MAX && (t1 & (t1 - 1)) == 0;
      }
    }

    if (fast_int_mode) {

      CC_GET_FUNC_PTR(func, (CvFunc2D_1A2P)(fastrng_tab[type]));
      param = iparam;
    }
    else {

      CC_GET_FUNC_PTR(func, (CvFunc2D_1A2P)(rng_tab[0][type]));
    }
  }
  else if (disttype == CC_RAND_NORMAL) {
    for (i = 0; i < channels; i++) {
      double t0 = param1.val[i];
      double t1 = param2.val[i];

      dparam[0][i] = t0;
      dparam[1][i] = t1;
    }

    CC_GET_FUNC_PTR(func, (CvFunc2D_1A2P)(rng_tab[1][type]));
  }
  else
  {
    CC_ERROR(CC_StsBadArg, "Unknown distribution type");
  }

  if (!fast_int_mode) {
    for (i = channels; i < 12; i++) {
      double t0 = dparam[0][i - channels];
      double t1 = dparam[1][i - channels];

      dparam[0][i] = t0;
      dparam[1][i] = t1;
    }
  }

  IPPI_CALL(func(arr, arr_step, size, rng, param));

  return CC_OK;
}