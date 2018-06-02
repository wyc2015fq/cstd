
#define icv_def_cvt_scale_func_flt(flavor, dsttype, cast_macro)
static CStatus C_STDCALL
icvCvtScaleTo_##flavor##_C1R(const uchar* src, int srcstep,
    dsttype* dst, int dststep, CSize size,
    double scale, double shift, int param)
{
  int i, srctype = param;
  dsttype lut[256];
  dststep /= sizeof(dst[0]);

  switch (srctype) {
  case CC_8U:
    if (size.w * size.h >= 256) {
      double val = shift;

      for (i = 0; i < 256; i++, val += scale) {
        lut[i] = (dsttype)val;
      }

      icvLUT_Transform8u_##flavor##_C1R(src, srcstep, dst, dststep* sizeof(dst[0]), size, lut);
    }
    else {
      ICC_DEF_CVT_SCALE_CASE(uchar, double, CC_NOP, cast_macro, scale, shift);
    }

    break;

  case CC_8S:
    if (size.w * size.h >= 256) {
      for (i = 0; i < 256; i++) {
        lut[i] = (dsttype)((char)i * scale + shift);
      }

      icvLUT_Transform8u_##flavor##_C1R(src, srcstep, dst, dststep* sizeof(dst[0]), size, lut);
    }
    else {
      ICC_DEF_CVT_SCALE_CASE(char, double, CC_NOP, cast_macro, scale, shift);
    }

    break;

  case CC_16U:
    ICC_DEF_CVT_SCALE_CASE(ushort, double, CC_NOP, cast_macro, scale, shift);
    break;

  case CC_16S:
    ICC_DEF_CVT_SCALE_CASE(short, double, CC_NOP, cast_macro, scale, shift);
    break;

  case CC_32S:
    ICC_DEF_CVT_SCALE_CASE(int, double, CC_NOP, cast_macro, scale, shift);
    break;

  case CC_32F:
    ICC_DEF_CVT_SCALE_CASE(float, double, CC_NOP, cast_macro, scale, shift);
    break;

  case CC_64F:
    ICC_DEF_CVT_SCALE_CASE(double, double, CC_NOP, cast_macro, scale, shift);
    break;

  default:
    assert(0);
    return CC_BADFLAG_ERR;
  }

  return CC_OK;
}
