{
#define cast_get_bgr_macro(dst, _B, _G, _R)    (_B = src[0]*0.0039215686274509803f, _G = src[1]*0.0039215686274509803f, _R = src[2]*0.0039215686274509803f)
#define cast_set_luv_macro(dst, _L, _U, _V)    { int _l=cvRound(_L*2.55f), _u=cvRound(_U*0.72033898305084743f+96.525423728813564f), _v=cvRound(_V*0.99609375f+139.453125f);  dst[ 0 ] = CV_CAST_8U(_l), dst[ 1 ] = CV_CAST_8U(_u), dst[ 2 ] = CV_CAST_8U(_v); }

  float b, g, r;
  float x, y, z;
  float L, u, v, t;

  cast_get_bgr_macro(src, b, g, r);

  x = b * xyzXb_32f + g * xyzXg_32f + r * xyzXr_32f;
  y = b * xyzYb_32f + g * xyzYg_32f + r * xyzYr_32f;
  z = b * xyzZb_32f + g * xyzZg_32f + r * xyzZr_32f;

  if (!x && !y && !z) {
    L = u = v = 0.f;
  }
  else {
    if (y > labT_32f) {
      L = labLScale_32f * cvCbrt1(y) - labLShift_32f;
    }
    else {
      L = labLScale2_32f * y;
    }

    t = 1.f / (x + 15 * y + 3 * z);
    u = 4.0f * x * t;
    v = 9.0f * y * t;

    u = 13 * L * (u - luvUn_32f);
    v = 13 * L * (v - luvVn_32f);
  }

  cast_set_luv_macro(dst, L, u, v);
#undef cast_get_bgr_macro
#undef cast_set_luv_macro
}
