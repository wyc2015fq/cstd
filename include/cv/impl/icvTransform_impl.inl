{
  int i, j;
  int diag_transform = 0;
  double buffer[CC_CN_MAX * (CC_CN_MAX + 1)];
  double* mat = buffer;

  if (cn > 4 || dst_cn > 4) {
    CC_ERROR(CC_StsOutOfRange, "Both input and output array must have at most 4 channels");
  }

  if (rot_cols == cn + 1 || rot_cols == cn) {
    for (i = 0; i < dst_cn; i++) {
      mat[i * (cn + 1) + cn] = 0;

      for (j = 0; j < rot_cols; j++) {
        mat[i * (cn + 1) + j] = (rot + rotstep * i)[j];
      }
    }
  }
  else {
    CC_ERROR(CC_StsUnmatchedSizes, "If the source array has <cn> channels, the transformation matrix must have <cn> x <cn>+1 or <cn> x <cn> size");
  }

  if (shift) {
    for (i = 0; i < dst_cn; i++) {
      mat[i * (cn + 1) + cn] += shift[i];
    }
  }

  if (dst_cn == cn) {
    diag_transform = 1;

    for (i = 0; i < dst_cn; i++) {
      for (j = 0; j < cn; j++) {
        if (i != j && fabs(mat[i * (cn + 1) + j]) > DBL_EPSILON) {
          diag_transform = 0;
          break;
        }
      }
    }

  }

  if (diag_transform) {
#if 0
    uchar lut[256];

    for (i = 0; i < cn; i++) {
      double a = mat[i * (cn + 1) + i], b = mat[i * (cn + 1) + cn];
      uchar* ltab = lut;

      for (j = 0; j < 256; j++) {
        int t = cRound(a * j + b);
        ltab[j * cn + i] = CC_CAST_8U(t);
      }
    }


#endif

    int i;

    switch (cn) {
    case 1: {
      for (; size.h--; src += srcstep, dst += dststep) {
        for (i = 0; i < size.w; i++) {
          double ft0;
          worktype t0;
          ft0 = mat[0] * _ld_(src[i]) + mat[1];
          t0 = _cast_macro1_(ft0);
          dst[i] = _cast_macro2_(t0);
        }
      }
    }
    break;

    case 2: {
      for (; size.h--; src += srcstep, dst += dststep) {
        for (i = 0; i < size.w * 2; i += 2) {
          double ft0, ft1;
          worktype t0, t1;
          ft0 = mat[0] * _ld_(src[i]) + mat[2];
          ft1 = mat[4] * _ld_(src[i + 1]) + mat[5];
          t0 = _cast_macro1_(ft0);
          t1 = _cast_macro1_(ft1);
          dst[i] = _cast_macro2_(t0);
          dst[i + 1] = _cast_macro2_(t1);
        }
      }
    }
    break;

    case 3: {
      for (; size.h--; src += srcstep, dst += dststep) {
        for (i = 0; i < size.w * 3; i += 3) {
          double ft0, ft1, ft2;
          worktype t0, t1, t2;
          ft0 = mat[0] * _ld_(src[i]) + mat[3];
          ft1 = mat[5] * _ld_(src[i + 1]) + mat[7];
          ft2 = mat[10] * _ld_(src[i + 2]) + mat[11];
          t0 = _cast_macro1_(ft0);
          t1 = _cast_macro1_(ft1);
          t2 = _cast_macro1_(ft2);
          dst[i] = _cast_macro2_(t0);
          dst[i + 1] = _cast_macro2_(t1);
          dst[i + 2] = _cast_macro2_(t2);
        }
      }
    }
    break;

    case 4: {
      for (; size.h--; src += srcstep, dst += dststep) {
        for (i = 0; i < size.w * 4; i += 4) {
          double ft0, ft1;
          worktype t0, t1;
          ft0 = mat[0] * _ld_(src[i]) + mat[4];
          ft1 = mat[6] * _ld_(src[i + 1]) + mat[9];
          t0 = _cast_macro1_(ft0);
          t1 = _cast_macro1_(ft1);
          dst[i] = _cast_macro2_(t0);
          dst[i + 1] = _cast_macro2_(t1);
          ft0 = mat[12] * _ld_(src[i + 2]) + mat[14];
          ft1 = mat[18] * _ld_(src[i + 3]) + mat[19];
          t0 = _cast_macro1_(ft0);
          t1 = _cast_macro1_(ft1);
          dst[i + 2] = _cast_macro2_(t0);
          dst[i + 3] = _cast_macro2_(t1);
        }
      }
    }
    break;

    default:
      ASSERT(0);
      break;
    }
  }
  else {

    int i, k;
    srcstep = srcstep - size.w * cn;
    dststep = dststep - size.w * dst_cn;

    switch (cn) {
    case 1: {
      for (; size.h--; src += srcstep, dst += dststep) {
        for (i = 0; i < size.w; i++, dst += dst_cn) {
          const double* _mat = mat;
          double v0 = _ld_(src[i]);

          for (k = 0; k < dst_cn; k++, _mat += 2) {
            worktype t0 = _cast_macro1_(_mat[0] * v0 + _mat[1]);
            dst[k] = _cast_macro2_(t0);
          }
        }

        src += size.w;
      }
    }
    break;

    case 2: {
      if (dst_cn == 2) {
        for (; size.h--; src += srcstep, dst += dststep) {
          for (i = 0; i < size.w * 2; i += 2) {
            double ft0, ft1;
            worktype t0, t1;
            ft0 = mat[0] * _ld_(src[i]) + mat[1] * _ld_(src[i + 1]) + mat[2];
            ft1 = mat[3] * _ld_(src[i]) + mat[4] * _ld_(src[i + 1]) + mat[5];
            t0 = _cast_macro1_(ft0);
            t1 = _cast_macro1_(ft1);
            dst[i] = _cast_macro2_(t0);
            dst[i + 1] = _cast_macro2_(t1);
          }

          src += size.w * 2;
          dst += size.w * 2;
        }
      }
      else {
        for (; size.h--; src += srcstep, dst += dststep) {
          for (i = 0; i < size.w; i++, src += 2, dst += dst_cn) {
            const double* _mat = mat;
            double v0 = _ld_(src[0]), v1 = src[1];

            for (k = 0; k < dst_cn; k++, _mat += 3) {
              worktype t0 = _cast_macro1_(_mat[0] * v0 + _mat[1] * v1 + _mat[2]);
              dst[k] = _cast_macro2_(t0);
            }
          }
        }
      }
    }
    break;

    case 3: {
      if (dst_cn == 3) {
        for (; size.h--; src += srcstep, dst += dststep) {
          for (i = 0; i < size.w * 3; i += 3) {
            double ft0, ft1, ft2;
            worktype t0, t1, t2;
            ft0 = mat[0] * _ld_(src[i]) + mat[1] * _ld_(src[i + 1]) + mat[2] * _ld_(src[i + 2]) + mat[3];
            ft1 = mat[4] * _ld_(src[i]) + mat[5] * _ld_(src[i + 1]) + mat[6] * _ld_(src[i + 2]) + mat[7];
            ft2 = mat[8] * _ld_(src[i]) + mat[9] * _ld_(src[i + 1]) + mat[10] * _ld_(src[i + 2]) + mat[11];
            t0 = _cast_macro1_(ft0);
            t1 = _cast_macro1_(ft1);
            t2 = _cast_macro1_(ft2);
            dst[i] = _cast_macro2_(t0);
            dst[i + 1] = _cast_macro2_(t1);
            dst[i + 2] = _cast_macro2_(t2);
          }

          src += size.w * 3;
          dst += size.w * 3;
        }
      }
      else if (dst_cn == 1) {
        for (; size.h--; src += srcstep, dst += dststep) {
          for (i = 0; i < size.w; i++, src += 3) {
            worktype t0 = _cast_macro1_(mat[0] * _ld_(src[0]) + mat[1] * _ld_(src[1]) + mat[2] * _ld_(src[2]) + mat[3]);
            dst[i] = _cast_macro2_(t0);
          }

          dst += size.w;
        }
      }
      else {
        for (; size.h--; src += srcstep, dst += dststep) {
          for (i = 0; i < size.w; i++, src += 3, dst += dst_cn) {
            const double* _mat = mat;
            double v0 = _ld_(src[0]), v1 = _ld_(src[1]), v2 = _ld_(src[2]);

            for (k = 0; k < dst_cn; k++, _mat += 4) {
              worktype t0 = _cast_macro1_(_mat[0] * v0 + _mat[1] * v1 + _mat[2] * v2 + _mat[3]);
              dst[k] = _cast_macro2_(t0);
            }
          }
        }
      }
    }
    break;

    case 4: {
      for (; size.h--; src += srcstep, dst += dststep) {
        for (i = 0; i < size.w; i++, src += 4, dst += dst_cn) {
          const double* _mat = mat;
          double v0 = _ld_(src[0]), v1 = _ld_(src[1]),
                 v2 = _ld_(src[2]), v3 = _ld_(src[3]);

          for (k = 0; k < dst_cn; k++, _mat += 5) {
            worktype t0 = _cast_macro1_(_mat[0] * v0 + _mat[1] * v1 + _mat[2] * v2 + _mat[3] * v3 + _mat[4]);
            dst[k] = _cast_macro2_(t0);
          }
        }
      }
    }
    break;

    default:
      ASSERT(0);
      break;
    }
  }
}

#undef _ld_
#undef _cast_macro1_
#undef _cast_macro2_
