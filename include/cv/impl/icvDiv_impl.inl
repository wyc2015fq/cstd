{
  int i;
  worktype z0, z1, z2, z3;
  const checktype* isrc = (const checktype*)src2;

  if (src1) {
    for (i = 0; i <= len - 4; i += 4) {
      if (_check_macro_(isrc[i]) && _check_macro_(isrc[i + 1]) &&
        _check_macro_(isrc[i + 2]) && _check_macro_(isrc[i + 3])) {
        double a = (double)_cvt_macro_(src2[i]) * _cvt_macro_(src2[i + 1]);
        double b = (double)_cvt_macro_(src2[i + 2]) * _cvt_macro_(src2[i + 3]);
        double d = scale / (a * b);
        
        b *= d;
        a *= d;
        
        z0 = _cast_macro1_(src2[i + 1] * _cvt_macro_(src1[i]) * b);
        z1 = _cast_macro1_(src2[i] * _cvt_macro_(src1[i + 1]) * b);
        z2 = _cast_macro1_(src2[i + 3] * _cvt_macro_(src1[i + 2]) * a);
        z3 = _cast_macro1_(src2[i + 2] * _cvt_macro_(src1[i + 3]) * a);
        
        dst[i] = _cast_macro2_(z0);
        dst[i + 1] = _cast_macro2_(z1);
        dst[i + 2] = _cast_macro2_(z2);
        dst[i + 3] = _cast_macro2_(z3);
      }
      else {
        z0 = _check_macro_(isrc[i]) ? _cast_macro1_(_cvt_macro_(src1[i]) * scale / _cvt_macro_(src2[i])) : 0;
        z1 = _check_macro_(isrc[i + 1]) ? _cast_macro1_(_cvt_macro_(src1[i + 1]) * scale / _cvt_macro_(src2[i + 1])) : 0;
        z2 = _check_macro_(isrc[i + 2]) ? _cast_macro1_(_cvt_macro_(src1[i + 2]) * scale / _cvt_macro_(src2[i + 2])) : 0;
        z3 = _check_macro_(isrc[i + 3]) ? _cast_macro1_(_cvt_macro_(src1[i + 3]) * scale / _cvt_macro_(src2[i + 3])) : 0;
        
        dst[i] = _cast_macro2_(z0);
        dst[i + 1] = _cast_macro2_(z1);
        dst[i + 2] = _cast_macro2_(z2);
        dst[i + 3] = _cast_macro2_(z3);
      }
    }
    
    for (; i < len; i++) {
      z0 = _check_macro_(isrc[i]) ? _cast_macro1_(_cvt_macro_(src1[i]) * scale / _cvt_macro_(src2[i])) : 0;
      dst[i] = _cast_macro2_(z0);
    }
  } else {
    for (i = 0; i <= len - 4; i += 4) {
      if (_check_macro_(isrc[i]) && _check_macro_(isrc[i + 1]) &&
        _check_macro_(isrc[i + 2]) && _check_macro_(isrc[i + 3])) {
        double a = (double)_cvt_macro_(src2[i]) * _cvt_macro_(src2[i + 1]);
        double b = (double)_cvt_macro_(src2[i + 2]) * _cvt_macro_(src2[i + 3]);
        double d = scale / (a * b);
        b *= d;
        a *= d;
        z0 = _cast_macro1_(src2[i + 1] * b);
        z1 = _cast_macro1_(src2[i] * b);
        z2 = _cast_macro1_(src2[i + 3] * a);
        z3 = _cast_macro1_(src2[i + 2] * a);
        dst[i] = _cast_macro2_(z0);
        dst[i + 1] = _cast_macro2_(z1);
        dst[i + 2] = _cast_macro2_(z2);
        dst[i + 3] = _cast_macro2_(z3);
      }
      else {
        z0 = _check_macro_(isrc[i]) ? _cast_macro1_(scale / _cvt_macro_(src2[i])) : 0;
        z1 = _check_macro_(isrc[i + 1]) ? _cast_macro1_(scale / _cvt_macro_(src2[i + 1])) : 0;
        z2 = _check_macro_(isrc[i + 2]) ? _cast_macro1_(scale / _cvt_macro_(src2[i + 2])) : 0;
        z3 = _check_macro_(isrc[i + 3]) ? _cast_macro1_(scale / _cvt_macro_(src2[i + 3])) : 0;
        dst[i] = _cast_macro2_(z0);
        dst[i + 1] = _cast_macro2_(z1);
        dst[i + 2] = _cast_macro2_(z2);
        dst[i + 3] = _cast_macro2_(z3);
      }
    }
    for (; i < len; i++) {
      z0 = _check_macro_(isrc[i]) ? _cast_macro1_(scale / _cvt_macro_(src2[i])) : 0;
      dst[i] = _cast_macro2_(z0);
    }
  }
}

#undef _cast_macro1_
#undef _cast_macro2_
#undef _cvt_macro_
#undef _check_macro_


