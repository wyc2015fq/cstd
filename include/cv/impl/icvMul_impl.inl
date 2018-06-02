
//#define icv_def_mul_op_case(flavor, arrtype, worktype, _cast_macro1_, _cast_macro2_, _cvt_macro_)
//static CStatus icvMul_##flavor##_C1R(const arrtype* src1, int src1step,const arrtype* src2, int src2step,arrtype* dst, int dststep,CSize size, double scale)
{
  if (fabs(scale - 1.) < DBL_EPSILON) {
    for (; size.h--; src1 += src1step, src2 += src2step, dst += dststep) {
      int i;

      for (i = 0; i <= size.w - 4; i += 4) {
        worktype t0 = src1[i] * src2[i];
        worktype t1 = src1[i + 1] * src2[i + 1];

        dst[i] = _cast_macro2_(t0);
        dst[i + 1] = _cast_macro2_(t1);

        t0 = src1[i + 2] * src2[i + 2];
        t1 = src1[i + 3] * src2[i + 3];

        dst[i + 2] = _cast_macro2_(t0);
        dst[i + 3] = _cast_macro2_(t1);
      }

      for (; i < size.w; i++) {
        worktype t0 = src1[i] * src2[i];
        dst[i] = _cast_macro2_(t0);
      }
    }
  }
  else {
    for (; size.h--; src1 += src1step, src2 += src2step, dst += dststep) {
      int i;

      for (i = 0; i <= size.w - 4; i += 4) {
        double ft0 = scale * _cvt_macro_(src1[i]) * _cvt_macro_(src2[i]);
        double ft1 = scale * _cvt_macro_(src1[i + 1]) * _cvt_macro_(src2[i + 1]);
        worktype t0 = _cast_macro1_(ft0);
        worktype t1 = _cast_macro1_(ft1);

        dst[i] = _cast_macro2_(t0);
        dst[i + 1] = _cast_macro2_(t1);

        ft0 = scale * _cvt_macro_(src1[i + 2]) * _cvt_macro_(src2[i + 2]);
        ft1 = scale * _cvt_macro_(src1[i + 3]) * _cvt_macro_(src2[i + 3]);
        t0 = _cast_macro1_(ft0);
        t1 = _cast_macro1_(ft1);

        dst[i + 2] = _cast_macro2_(t0);
        dst[i + 3] = _cast_macro2_(t1);
      }

      for (; i < size.w; i++) {
        worktype t0;
        t0 = _cast_macro1_(scale * _cvt_macro_(src1[i]) * _cvt_macro_(src2[i]));
        dst[i] = _cast_macro2_(t0);
      }
    }
  }
}
#undef _cast_macro1_
#undef _cast_macro2_
#undef _cvt_macro_

