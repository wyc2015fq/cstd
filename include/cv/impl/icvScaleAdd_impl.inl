{
  int i, len = size.w * cn;

  if (srcstep1 == size.w && srcstep2 == size.w && dststep == size.w && 1 == cn && size.w <= CC_MAX_INLINE_MAT_OP_SIZE) {
    const double* mA = src1;
    const double* mB = src2;
    double* mC = dst;
    size.w *= size.h;

    do {
      mC[size.w - 1] = mA[size.w - 1] * scalar[0] + mB[size.w - 1];
    }
    while (--size.w);
  }
  else {
    switch (cn) {
    case 1: {
      worktype s0 = scalar[0];

      for (; size.h--; src1 += srcstep1, src2 += srcstep2, dst += dststep) {
        for (i = 0; i <= (len) - 4; i += 4) {
          worktype t0 = (src1)[i] * s0 + (src2)[i];
          worktype t1 = (src1)[i + 1] * s0 + (src2)[i + 1];

          (dst)[i] = (arrtype)t0;
          (dst)[i + 1] = (arrtype)t1;

          t0 = (src1)[i + 2] * s0 + (src2)[i + 2];
          t1 = (src1)[i + 3] * s0 + (src2)[i + 3];

          (dst)[i + 2] = (arrtype)t0;
          (dst)[i + 3] = (arrtype)t1;
        }

        for (; i < (len); i++) {
          worktype t0 = (src1)[i] * s0 + (src2)[i];
          (dst)[i] = (arrtype)t0;
        }
      }
    }
    break;

    case 2: {
      worktype s0 = scalar[0], s1 = scalar[1];

      for (; size.h--; src1 += srcstep1, src2 += srcstep2, dst += dststep) {
        for (i = 0; i <= (len) - 4; i += 4) {
          worktype t0 = (src1)[i] * s0 - (src1)[i + 1] * s1 + (src2)[i];
          worktype t1 = (src1)[i] * s1 + (src1)[i + 1] * s0 + (src2)[i + 1];

          (dst)[i] = (arrtype)t0;
          (dst)[i + 1] = (arrtype)t1;

          t0 = (src1)[i + 2] * s0 - (src1)[i + 3] * s1 + (src2)[i + 2];
          t1 = (src1)[i + 2] * s1 + (src1)[i + 3] * s0 + (src2)[i + 3];

          (dst)[i + 2] = (arrtype)t0;
          (dst)[i + 3] = (arrtype)t1;
        }

        for (; i < (len); i += 2) {
          worktype t0 = (src1)[i] * s0 - (src1)[i + 1] * s1 + (src2)[i];
          worktype t1 = (src1)[i] * s1 + (src1)[i + 1] * s0 + (src2)[i + 1];

          (dst)[i] = (arrtype)t0;
          (dst)[i + 1] = (arrtype)t1;
        }
      }

    }
    break;

    case 3: {
    }
    break;

    case 4: {
    }
    break;

    default:
      ASSERT(0);
      break;
    }

  }
  CC_CHECK_NANS(dst);
}