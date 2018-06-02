
int i;

srcstep -= width * srccn;
srcstep -= width * srccn;

for (; height--; src += srcstep, dst += dststep)
{
  for (i = 0; i < width; src += srccn, dst += dstcn) {
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
        L = labLScale_32f * scbrt(y) - labLShift_32f;
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
  }
}

#undef cast_set_luv_macro
#undef cast_get_bgr_macro
