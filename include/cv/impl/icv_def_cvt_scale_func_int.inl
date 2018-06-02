
#define  icv_def_cvt_scale_func_int(flavor, dsttype, cast_macro)
static  CStatus icvCvtScaleTo_##flavor##_C1R(const uchar* src, int srcstep,
    dsttype* dst, int dststep, CSize size,
    double scale, double shift, int param)
{
  int i, srctype = param;
  dsttype lut[256];
  dststep /= sizeof(dst[0]);

  switch (srctype) {
  case  CC_8U:
    if (size.w * size.h >= 256) {
      double val = shift;

      for (i = 0; i < 256; i++, val += scale) {
        int t = cRound(val);
        lut[i] = cast_macro(t);
      }

      icvLUT_Transform8u_##flavor##_C1R(src, srcstep, dst,
          dststep* sizeof(dst[0]), size, lut);
    }
    else if (fabs(scale) <= 128. &&
        fabs(shift) <= (INT_MAX * 0.5) / (1 << ICC_FIX_SHIFT)) {
      int iscale = cRound(scale * (1 << ICC_FIX_SHIFT));
      int ishift = cRound(shift * (1 << ICC_FIX_SHIFT));

      ICC_DEF_CVT_SCALE_CASE(uchar, int, ICC_SCALE, cast_macro, iscale, ishift);
    }
    else {
      ICC_DEF_CVT_SCALE_CASE(uchar, int, cRound, cast_macro, scale, shift);
    }

    break;

  case  CC_8S:
    if (size.w * size.h >= 256) {
      for (i = 0; i < 256; i++) {
        int t = cRound((char)i * scale + shift);
        lut[i] = cast_macro(t);
      }

      icvLUT_Transform8u_##flavor##_C1R(src, srcstep, dst, dststep* sizeof(dst[0]), size, lut);
    }
    else if (fabs(scale) <= 128. &&
        fabs(shift) <= (INT_MAX * 0.5) / (1 << ICC_FIX_SHIFT)) {
      int iscale = cRound(scale * (1 << ICC_FIX_SHIFT));
      int ishift = cRound(shift * (1 << ICC_FIX_SHIFT));

      ICC_DEF_CVT_SCALE_CASE(char, int, ICC_SCALE,
          cast_macro, iscale, ishift);
    }
    else {
      ICC_DEF_CVT_SCALE_CASE(char, int, cRound, cast_macro, scale, shift);
    }

    break;

  case  CC_16U:
    if (fabs(scale) <= 1. && fabs(shift) < DBL_EPSILON) {
      int iscale = cRound(scale * (1 << ICC_FIX_SHIFT));

      ICC_DEF_CVT_SCALE_CASE(ushort, int, ICC_SCALE, cast_macro, iscale, 0);
    }
    else {
      ICC_DEF_CVT_SCALE_CASE(ushort, int, cRound, cast_macro, scale, shift);
    }

    break;

  case  CC_16S:
    if (fabs(scale) <= 1. &&
        fabs(shift) <= (INT_MAX * 0.5) / (1 << ICC_FIX_SHIFT)) {
      int iscale = cRound(scale * (1 << ICC_FIX_SHIFT));
      int ishift = cRound(shift * (1 << ICC_FIX_SHIFT));

      ICC_DEF_CVT_SCALE_CASE(short, int, ICC_SCALE, cast_macro, iscale, ishift);
    }
    else {
      ICC_DEF_CVT_SCALE_CASE(short, int, cRound, cast_macro, scale, shift);
    }

    break;

  case  CC_32S:
    ICC_DEF_CVT_SCALE_CASE(int, int, cRound, cast_macro, scale, shift);
    break;

  case  CC_32F:
    ICC_DEF_CVT_SCALE_CASE(float, int, cRound, cast_macro, scale, shift);
    break;

  case  CC_64F:
    ICC_DEF_CVT_SCALE_CASE(double, int, cRound, cast_macro, scale, shift);
    break;

  default:
    assert(0);
    return CC_BADFLAG_ERR;
  }

  return  CC_OK;
}
