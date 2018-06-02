
int i;

srcstep -= width * srccn;
dststep -= width * dstcn;

for (; height--; src += srcstep, dst += dststep)
{
  for (i = 0; i < width; src += srccn, dst += dstcn) {
    float L, u, v;
    float x, y, z, t, u1, v1, b, g, r;

    cast_get_luv_macro(src, L, u, v);

    if (L >= 8) {
      t = (L + labLShift_32f) * (1.f / labLScale_32f);
      y = t * t * t;
    }
    else {
      y = L * (1.f / labLScale2_32f);
      L = MAX(L, 0.001f);
    }

    t = 1.f / (13.f * L);
    u1 = u * t + luvUn_32f;
    v1 = v * t + luvVn_32f;
    x = 2.25f * u1 * y / v1 ;
    z = (12 - 3 * u1 - 20 * v1) * y / (4 * v1);

    b = xyzBx_32f * x + xyzBy_32f * y + xyzBz_32f * z;
    g = xyzGx_32f * x + xyzGy_32f * y + xyzGz_32f * z;
    r = xyzRx_32f * x + xyzRy_32f * y + xyzRz_32f * z;

    cast_set_bgr_macro(dst, b, g, r);
  }
}

#undef cast_set_bgr_macro
#undef cast_get_luv_macro
