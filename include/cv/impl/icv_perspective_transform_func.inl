{
  int i;
  size.w *= cn;

  switch (cn) {
  case 2: {
    for (; size.h--; src += srcstep, dst += dststep) {
      for (i = 0; i < size.w; i += 2) {
        arrtype x = src[i], y = src[i + 1];
        double w = x * mat[6] + y * mat[7] + mat[8];

        if (fabs(w) > FLT_EPSILON) {
          w = 1. / w;
          dst[i] = (arrtype)((x * mat[0] + y * mat[1] + mat[2]) * w);
          dst[i + 1] = (arrtype)((x * mat[3] + y * mat[4] + mat[5]) * w);
        }
        else {
          dst[i] = (arrtype)0;
          dst[i + 1] = (arrtype)0;
        }
      }
    }
  }
  break;

  case 3: {
    for (; size.h--; src += srcstep, dst += dststep) {
      for (i = 0; i < size.w; i += 3) {
        arrtype x = src[i], y = src[i + 1], z = src[i + 2];
        double w = x * mat[12] + y * mat[13] + z * mat[14] + mat[15];

        if (fabs(w) > FLT_EPSILON) {
          w = 1. / w;
          dst[i] = (arrtype)((x * mat[0] + y * mat[1] + z * mat[2] + mat[3]) * w);
          dst[i + 1] = (arrtype)((x * mat[4] + y * mat[5] + z * mat[6] + mat[7]) * w);
          dst[i + 2] = (arrtype)((x * mat[8] + y * mat[9] + z * mat[10] + mat[11]) * w);
        }
        else {
          dst[i] = (arrtype)0;
          dst[i + 1] = (arrtype)0;
          dst[i + 2] = (arrtype)0;
        }
      }
    }
  }
  break;

  default:
    CC_ERROR(CC_BadNumChannels, cvUnsupportedFormat);
    break;
  }

  CC_CHECK_NANS(dst);
}