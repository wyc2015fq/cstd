{
  int i;
#define scale_macro(x)  CC_DESCALE((x), xshift)
#define cast_macro      CC_CAST_8U
  enum {xshift = 8};
  enum {
#define CVTCOLOR_COEFFICIENT_DEF(a, b)  a=(int)((b)*(1<<xshift)),
#include "cvtcolor_coefficient_def.txt"
  };

#if 0
  static uchar gr_table[256] = {0};
  static uchar gg_table[256] = {0};
  static uchar gb_table[256] = {0};
  static gray_table_inited = 0;

  if (gray_table_inited) {
    gray_table_inited = 1;

    for (i = 0; i < 256; ++i) {
      gr_table[i] = (i * csc_gr);
      gg_table[i] = (i * csc_gg);
      gb_table[i] = (i * csc_gb);
    }
  }

#endif

  switch (type2) {
  case T_GRAY: {
    srcstep -= width * srccn;
    dststep -= width * dstcn;

    if (istorgb) {
      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
          SETRGB(dst, src[0], src[0], src[0]);
        }
      }
    }
    else {
      // Gray = R*0.299 + G*0.587  + B*0.114
      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
          GETRGB(src, r, g, b);
          //dst[0] = gr_table[r] + gg_table[g] + gb_table[b];
          dst[0] = (uchar)scale_macro((r * csc_gr + g * csc_gg + b * csc_gb));
        }
      }
    }
  }
  break;

  case T_RGB: {
    srcstep -= width * srccn;
    dststep -= width * dstcn;

    if (istorgb) {
      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
          SETRGB(dst, src[0], src[1], src[2]);
        }
      }
    }
    else {
      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
          GETRGB(src, r, g, b);
          dst[0] = r, dst[1] = g, dst[2] = b;
        }
      }
    }
  }
  break;

  case T_BGR: {
    int i;
    srcstep -= width * srccn;
    dststep -= width * dstcn;

    if (istorgb) {
      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
          SETRGB(dst, src[2], src[1], src[0]);
        }
      }
    }
    else {
      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
          GETRGB(src, r, g, b);
          dst[0] = b, dst[1] = g, dst[2] = r;
        }
      }
    }
  }
  break;

  case T_BGRA: {
    int i;
    srcstep -= width * srccn;
    dststep -= width * dstcn;

    if (istorgb) {
      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
          SETRGB(dst, src[2], src[1], src[0]);
        }
      }
    }
    else {
      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
          GETRGB(src, r, g, b);
          dst[0] = b, dst[1] = g, dst[2] = r;
        }
      }
    }
  }
  break;

  case T_BGR555: {
    if (istorgb) {
      dststep -= width * dstcn;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, dst += dstcn) {
          unsigned t = ((const ushort*) src) [ i ];
          b = (uchar)(t << 3), g = (uchar)((t >> 2) & ~7), r = (uchar)((t >> 7) & ~7);
          SETRGB(dst, r, g, b);
        }
      }
    }
    else {
      srcstep -= width * srccn;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn) {
          int t;
          GETRGB(src, r, g, b);
          t = (b >> 3) | ((g & ~7) << 2) | ((r & ~7) << 7);
          ((ushort*) dst) [ i ] = (ushort) t;
        }
      }
    }
  }
  break;

  case T_BGR565: {
    if (istorgb) {
      dststep -= width * dstcn;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, dst += dstcn) {
          unsigned t = ((const ushort*) src) [ i ];
          b = (uchar)(t << 3), g = (uchar)((t >> 3) & ~3), r = (uchar)((t >> 8) & ~7);
          SETRGB(dst, r, g, b);
        }
      }
    }
    else {
      srcstep -= width * srccn;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn) {
          int t;
          GETRGB(src, r, g, b);
          t = (b >> 3) | ((g & ~3) << 3) | ((r & ~7) << 8);
          ((ushort*) dst) [ i ] = (ushort) t;
        }
      }
    }
  }
  break;

  case T_YCrCb: {
    worktype y;

    if (istorgb) {

      dststep -= width * dstcn;
      width *= 3;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i += 3, src += srccn, dst += dstcn) {
          int Y = src[0], Cr = src[1] - yuv_cx_bias, Cb = src[2] - yuv_cx_bias;
          int ib = (Y + yuv_bcb * Cb);
          int ig = (Y + yuv_gcr * Cr + yuv_gcb * Cb);
          int ir = (Y + yuv_rcr * Cr);
          SETRGB(dst, CC_CAST_8U(ir), CC_CAST_8U(ig), CC_CAST_8U(ib));
        }
      }
    }
    else {
      srcstep -= width * srccn;
      width *= 3;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i += 3, src += srccn) {
          GETRGB(src, r, g, b);
          y = scale_macro(b * yuv_yb + g * yuv_yg + r * yuv_yr);
          r = scale_macro((r - y) * yuv_cr) + yuv_cx_bias;
          b = scale_macro((b - y) * yuv_cb) + yuv_cx_bias;
          dst[i + 0] = cast_macro(y);
          dst[i + 1] = cast_macro(r);
          dst[i + 2] = cast_macro(b);
        }
      }
    }
  }
  break;

  case T_YCrYCb: {
    if (istorgb) {
      typedef int worktype;
      int i;
      int ir, ig, ib;
      worktype s[4];
      dststep -= width * dstcn;
      width *= 3;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i += 2, dst += dstcn) {
          worktype Y, Cb, Cr;
          Y = (src[i]);
          s[1 + i ^ 1] = src[i + 1] - yuv_cx_bias;
          Y = s[0], Cb = s[1], Cr = s[2];
          ib = (Y + yuv_bcb * Cb);
          ig = (Y + yuv_gcr * Cr + yuv_gcb * Cb);
          ir = (Y + yuv_rcr * Cr);
          SETRGB(dst, CC_CAST_8U(ir), CC_CAST_8U(ig), CC_CAST_8U(ib));
        }
      }
    }
    else {}
  } break;

  case T_XYZ: {
    if (istorgb) {
      worktype t, _m[] = {
        xyz_bx, xyz_by, xyz_bz,
        xyz_gx, xyz_gy, xyz_gz,
        xyz_rx, xyz_ry, xyz_rz
      };
      dststep -= width * dstcn;
      width *= 3;

      if (0) {
        CC_SWAP(_m[0], _m[6], t);
        CC_SWAP(_m[1], _m[7], t);
        CC_SWAP(_m[2], _m[8], t);
      }

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i += 3, dst += dstcn) {
          worktype ib = scale_macro(src[i] * _m[0] + src[i + 1] * _m[1] + src[i + 2] * _m[2]);
          worktype ig = scale_macro(src[i] * _m[3] + src[i + 1] * _m[4] + src[i + 2] * _m[5]);
          worktype ir = scale_macro(src[i] * _m[6] + src[i + 1] * _m[7] + src[i + 2] * _m[8]);
          SETRGB(dst, cast_macro(ir), cast_macro(ig), cast_macro(ib));
        }
      }
    }
    else {
      worktype t, _m[] = {
        xyz_xb, xyz_xg, xyz_xr,
        xyz_yb, xyz_yg, xyz_yr,
        xyz_zb, xyz_zg, xyz_zr
      };
      srcstep -= width * srccn;
      width *= 3;

      if (0) {
        CC_SWAP(_m[0], _m[2], t);
        CC_SWAP(_m[3], _m[5], t);
        CC_SWAP(_m[6], _m[8], t);
      }

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i += 3, src += srccn) {
          worktype x = scale_macro(src[0] * _m[0] + src[1] * _m[1] + src[2] * _m[2]);
          worktype y = scale_macro(src[0] * _m[3] + src[1] * _m[4] + src[2] * _m[5]);
          worktype z = scale_macro(src[0] * _m[6] + src[1] * _m[7] + src[2] * _m[8]);
          dst[i + 0] = (arrtype)(x);
          dst[i + 1] = (arrtype)(y);
          dst[i + 2] = cast_macro(z); /*sum of weights for z > 1*/
        }
      }
    }
  }
  break;

  case T_BayerBG:
  case T_BayerGB:
  case T_BayerGR: {
    if (istorgb) {
      int src_type = srctype;
      const uchar* bayer0 = src;
      uchar* dst0 = dst;
      int bayer_step = srcstep;
      int blue = src_type == T_BayerBG || src_type == T_BayerGB ? -1 : 1;
      int start_with_green = src_type == T_BayerGB || src_type == T_BayerGR;

      memset(dst0, 0, width * 3 * sizeof(dst0[ 0 ]));
      memset(dst0 + (height - 1) * dststep, 0, width * 3 * sizeof(dst0[ 0 ]));
      dst0 += dststep + 3 + 1;
      height -= 2;
      width -= 2;

      for (; height-- > 0; bayer0 += bayer_step, dst0 += dststep) {
        int t0, t1;
        const uchar* bayer = bayer0;
        uchar* dst = dst0;
        const uchar* bayer_end = bayer + width;

        dst[ -4 ] = dst[ -3 ] = dst[ -2 ] = dst[ width * 3 - 1 ] = dst[ width * 3 ] = dst[ width * 3 + 1 ] = 0;

        if (width <= 0) {
          continue;
        }

        if (start_with_green) {
          t0 = (bayer[ 1 ] + bayer[ bayer_step * 2 + 1 ] + 1) >> 1;
          t1 = (bayer[ bayer_step ] + bayer[ bayer_step + 2 ] + 1) >> 1;
          dst[ -blue ] = (uchar) t0;
          dst[ 0 ] = bayer[ bayer_step + 1 ];
          dst[ blue ] = (uchar) t1;
          bayer++;
          dst += 3;
        }

        if (blue > 0) {
          for (; bayer <= bayer_end - 2; bayer += 2, dst += 6) {
            t0 = (bayer[ 0 ] + bayer[ 2 ] + bayer[ bayer_step * 2 ] + bayer[ bayer_step * 2 + 2 ] + 2) >> 2;
            t1 = (bayer[ 1 ] + bayer[ bayer_step ] + bayer[ bayer_step + 2 ] + bayer[ bayer_step * 2 + 1 ] + 2) >> 2;
            dst[ -1 ] = (uchar) t0;
            dst[ 0 ] = (uchar) t1;
            dst[ 1 ] = bayer[ bayer_step + 1 ];

            t0 = (bayer[ 2 ] + bayer[ bayer_step * 2 + 2 ] + 1) >> 1;
            t1 = (bayer[ bayer_step + 1 ] + bayer[ bayer_step + 3 ] + 1) >> 1;
            dst[ 2 ] = (uchar) t0;
            dst[ 3 ] = bayer[ bayer_step + 2 ];
            dst[ 4 ] = (uchar) t1;
          }
        }
        else {
          for (; bayer <= bayer_end - 2; bayer += 2, dst += 6) {
            t0 = (bayer[ 0 ] + bayer[ 2 ] + bayer[ bayer_step * 2 ] + bayer[ bayer_step * 2 + 2 ] + 2) >> 2;
            t1 = (bayer[ 1 ] + bayer[ bayer_step ] + bayer[ bayer_step + 2 ] + bayer[ bayer_step * 2 + 1 ] + 2) >> 2;
            dst[ 1 ] = (uchar) t0;
            dst[ 0 ] = (uchar) t1;
            dst[ -1 ] = bayer[ bayer_step + 1 ];

            t0 = (bayer[ 2 ] + bayer[ bayer_step * 2 + 2 ] + 1) >> 1;
            t1 = (bayer[ bayer_step + 1 ] + bayer[ bayer_step + 3 ] + 1) >> 1;
            dst[ 4 ] = (uchar) t0;
            dst[ 3 ] = bayer[ bayer_step + 2 ];
            dst[ 2 ] = (uchar) t1;
          }
        }

        if (bayer < bayer_end) {
          t0 = (bayer[ 0 ] + bayer[ 2 ] + bayer[ bayer_step * 2 ] + bayer[ bayer_step * 2 + 2 ] + 2) >> 2;
          t1 = (bayer[ 1 ] + bayer[ bayer_step ] + bayer[ bayer_step + 2 ] + bayer[ bayer_step * 2 + 1 ] + 2) >> 2;
          dst[ -blue ] = (uchar) t0;
          dst[ 0 ] = (uchar) t1;
          dst[ blue ] = bayer[ bayer_step + 1 ];
          bayer++;
          dst += 3;
        }

        blue = -blue;
        start_with_green = !start_with_green;
      }

    }
    else {}
  } break;

  case T_HSV32F: {

    static const uchar icvHue255To180[] = { //
      0, 1, 1, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11,
      11, 12, 13, 13, 14, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 22,
      23, 23, 24, 25, 25, 26, 27, 28, 28, 29, 30, 30, 31, 32, 32, 33,
      34, 35, 35, 36, 37, 37, 38, 39, 40, 40, 41, 42, 42, 43, 44, 44,
      45, 46, 47, 47, 48, 49, 49, 50, 51, 52, 52, 53, 54, 54, 55, 56,
      56, 57, 58, 59, 59, 60, 61, 61, 62, 63, 64, 64, 65, 66, 66, 67,
      68, 68, 69, 70, 71, 71, 72, 73, 73, 74, 75, 76, 76, 77, 78, 78,
      79, 80, 80, 81, 82, 83, 83, 84, 85, 85, 86, 87, 88, 88, 89, 90,
      90, 91, 92, 92, 93, 94, 95, 95, 96, 97, 97, 98, 99, 100, 100, 101,
      102, 102, 103, 104, 104, 105, 106, 107, 107, 108, 109, 109, 110, 111, 112, 112,
      113, 114, 114, 115, 116, 116, 117, 118, 119, 119, 120, 121, 121, 122, 123, 124,
      124, 125, 126, 126, 127, 128, 128, 129, 130, 131, 131, 132, 133, 133, 134, 135,
      136, 136, 137, 138, 138, 139, 140, 140, 141, 142, 143, 143, 144, 145, 145, 146,
      147, 148, 148, 149, 150, 150, 151, 152, 152, 153, 154, 155, 155, 156, 157, 157,
      158, 159, 160, 160, 161, 162, 162, 163, 164, 164, 165, 166, 167, 167, 168, 169,
      169, 170, 171, 172, 172, 173, 174, 174, 175, 176, 176, 177, 178, 179, 179, 180
    };


    static const uchar icvHue180To255[] = { //
      0, 1, 3, 4, 6, 7, 9, 10, 11, 13, 14, 16, 17, 18, 20, 21,
      23, 24, 26, 27, 28, 30, 31, 33, 34, 35, 37, 38, 40, 41, 43, 44,
      45, 47, 48, 50, 51, 52, 54, 55, 57, 58, 60, 61, 62, 64, 65, 67,
      68, 69, 71, 72, 74, 75, 77, 78, 79, 81, 82, 84, 85, 86, 88, 89,
      91, 92, 94, 95, 96, 98, 99, 101, 102, 103, 105, 106, 108, 109, 111, 112,
      113, 115, 116, 118, 119, 120, 122, 123, 125, 126, 128, 129, 130, 132, 133, 135,
      136, 137, 139, 140, 142, 143, 145, 146, 147, 149, 150, 152, 153, 154, 156, 157,
      159, 160, 162, 163, 164, 166, 167, 169, 170, 171, 173, 174, 176, 177, 179, 180,
      181, 183, 184, 186, 187, 188, 190, 191, 193, 194, 196, 197, 198, 200, 201, 203,
      204, 205, 207, 208, 210, 211, 213, 214, 215, 217, 218, 220, 221, 222, 224, 225,
      227, 228, 230, 231, 232, 234, 235, 237, 238, 239, 241, 242, 244, 245, 247, 248,
      249, 251, 252, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    };

    if (istorgb) {
      dststep -= width * dstcn;
      width *= 3;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i += 3, dst += dstcn) {
          float h = src[ i ], s = src[ i + 1 ], v = src[ i + 2 ];
          float b, g, r;

          if (s == 0) {
            b = g = r = v;
          }
          else {
            static const int sector_data[][ 3 ] = { {1, 3, 0}, {1, 0, 2}, {3, 0, 1}, {0, 2, 1}, {0, 1, 3}, {2, 1, 0} };
            float tab[ 4 ];
            int sector;
            h *= 0.016666666666666666f; // h /= 60;

            if (h < 0)
              do {
                h += 6;
              }
              while (h < 0);
            else if (h >= 6)
              do {
                h -= 6;
              }
              while (h >= 6);

            sector = cvFloor(h);
            h -= sector;

            tab[ 0 ] = v;
            tab[ 1 ] = v * (1.f - s);
            tab[ 2 ] = v * (1.f - s * h);
            tab[ 3 ] = v * (1.f - s * (1.f - h));

            b = tab[ sector_data[ sector ][ 0 ] ];
            g = tab[ sector_data[ sector ][ 1 ] ];
            r = tab[ sector_data[ sector ][ 2 ] ];
          }

          dst[ 0 ] = (uchar)b;
          dst[ 1 ] = (uchar)g;
          dst[ 2 ] = (uchar)r;
        }
      }

    }
    else {
      float* dstf = (float*)dst;
      dststep /= dstcn;
      srcstep -= width * srccn;
      width *= 3;

      for (; height--; src += srcstep, dstf += dststep) {
        for (i = 0; i < width; i += 3, src += srccn) {
          float b = src[ 0 ], g = src[ 1 ], r = src[ 2 ^ 0 ];
          float h, s, v;

          float vmin, diff;

          v = vmin = r;

          if (v < g) {
            v = g;
          }

          if (v < b) {
            v = b;
          }

          if (vmin > g) {
            vmin = g;
          }

          if (vmin > b) {
            vmin = b;
          }

          diff = v - vmin;
          s = diff / (float)(fabs(v) + FLT_EPSILON);
          diff = (float)(60. / (diff + FLT_EPSILON));

          if (v == r) {
            h = (g - b) * diff;
          }
          else if (v == g) {
            h = (b - r) * diff + 120.f;
          }
          else {
            h = (r - g) * diff + 240.f;
          }

          if (h < 0) {
            h += 360.f;
          }

          dstf[ i ] = h;
          dstf[ i + 1 ] = s;
          dstf[ i + 2 ] = v;
        }
      }
    }
  }
  break;

  case T_HSV: {
    if (istorgb) {}
    else {
      const int hsv_shift = 12;

      static const int div_table[] = { //
        0, 1044480, 522240, 348160, 261120, 208896, 174080, 149211,
        130560, 116053, 104448, 94953, 87040, 80345, 74606, 69632,
        65280, 61440, 58027, 54973, 52224, 49737, 47476, 45412,
        43520, 41779, 40172, 38684, 37303, 36017, 34816, 33693,
        32640, 31651, 30720, 29842, 29013, 28229, 27486, 26782,
        26112, 25475, 24869, 24290, 23738, 23211, 22706, 22223,
        21760, 21316, 20890, 20480, 20086, 19707, 19342, 18991,
        18651, 18324, 18008, 17703, 17408, 17123, 16846, 16579,
        16320, 16069, 15825, 15589, 15360, 15137, 14921, 14711,
        14507, 14308, 14115, 13926, 13743, 13565, 13391, 13221,
        13056, 12895, 12738, 12584, 12434, 12288, 12145, 12006,
        11869, 11736, 11605, 11478, 11353, 11231, 11111, 10995,
        10880, 10768, 10658, 10550, 10445, 10341, 10240, 10141,
        10043, 9947, 9854, 9761, 9671, 9582, 9495, 9410,
        9326, 9243, 9162, 9082, 9004, 8927, 8852, 8777,
        8704, 8632, 8561, 8492, 8423, 8356, 8290, 8224,
        8160, 8097, 8034, 7973, 7913, 7853, 7795, 7737,
        7680, 7624, 7569, 7514, 7461, 7408, 7355, 7304,
        7253, 7203, 7154, 7105, 7057, 7010, 6963, 6917,
        6872, 6827, 6782, 6739, 6695, 6653, 6611, 6569,
        6528, 6487, 6447, 6408, 6369, 6330, 6292, 6254,
        6217, 6180, 6144, 6108, 6073, 6037, 6003, 5968,
        5935, 5901, 5868, 5835, 5803, 5771, 5739, 5708,
        5677, 5646, 5615, 5585, 5556, 5526, 5497, 5468,
        5440, 5412, 5384, 5356, 5329, 5302, 5275, 5249,
        5222, 5196, 5171, 5145, 5120, 5095, 5070, 5046,
        5022, 4998, 4974, 4950, 4927, 4904, 4881, 4858,
        4836, 4813, 4791, 4769, 4748, 4726, 4705, 4684,
        4663, 4642, 4622, 4601, 4581, 4561, 4541, 4522,
        4502, 4483, 4464, 4445, 4426, 4407, 4389, 4370,
        4352, 4334, 4316, 4298, 4281, 4263, 4246, 4229,
        4212, 4195, 4178, 4161, 4145, 4128, 4112, 4096
      };

      srcstep -= width * srccn;
      width *= 3;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i += 3, src += srccn) {
          int h, s, v, vmin, diff, vr, vg;
          GETRGB(src, r, g, b);
          v = b, vmin = b;

          CC_CALC_MAX_8U(v, g);
          CC_CALC_MAX_8U(v, r);
          CC_CALC_MIN_8U(vmin, g);
          CC_CALC_MIN_8U(vmin, r);

          diff = v - vmin;
          vr = v == r ? -1 : 0;
          vg = v == g ? -1 : 0;

          s = diff * div_table[ v ] >> hsv_shift;
          h = (vr & (g - b)) + (~vr & ((vg & (b - r + 2 * diff)) + ((~vg) & (r - g + 4 * diff))));
          h = ((h * div_table[ diff ] * 15 + (1 << (hsv_shift + 6))) >> (7 + hsv_shift)) + (h < 0 ? 30 * 6 : 0);

          dst[ i + 0 ] = (uchar) h;
          dst[ i + 1 ] = (uchar) s;
          dst[ i + 2 ] = (uchar) v;
        }
      }
    }
  }
  break;

  case T_HLS: {
    if (istorgb) {}
    else {}
  } break;

  case T_Lab: {

    /* 1024*(([0..511]./255)**(1./3)) */
    static ushort icvLabCubeRootTab[] = { //
      0, 161, 203, 232, 256, 276, 293, 308, 322, 335, 347, 359, 369, 379, 389, 398,
      406, 415, 423, 430, 438, 445, 452, 459, 465, 472, 478, 484, 490, 496, 501, 507,
      512, 517, 523, 528, 533, 538, 542, 547, 552, 556, 561, 565, 570, 574, 578, 582,
      586, 590, 594, 598, 602, 606, 610, 614, 617, 621, 625, 628, 632, 635, 639, 642,
      645, 649, 652, 655, 659, 662, 665, 668, 671, 674, 677, 680, 684, 686, 689, 692,
      695, 698, 701, 704, 707, 710, 712, 715, 718, 720, 723, 726, 728, 731, 734, 736,
      739, 741, 744, 747, 749, 752, 754, 756, 759, 761, 764, 766, 769, 771, 773, 776,
      778, 780, 782, 785, 787, 789, 792, 794, 796, 798, 800, 803, 805, 807, 809, 811,
      813, 815, 818, 820, 822, 824, 826, 828, 830, 832, 834, 836, 838, 840, 842, 844,
      846, 848, 850, 852, 854, 856, 857, 859, 861, 863, 865, 867, 869, 871, 872, 874,
      876, 878, 880, 882, 883, 885, 887, 889, 891, 892, 894, 896, 898, 899, 901, 903,
      904, 906, 908, 910, 911, 913, 915, 916, 918, 920, 921, 923, 925, 926, 928, 929,
      931, 933, 934, 936, 938, 939, 941, 942, 944, 945, 947, 949, 950, 952, 953, 955,
      956, 958, 959, 961, 962, 964, 965, 967, 968, 970, 971, 973, 974, 976, 977, 979,
      980, 982, 983, 985, 986, 987, 989, 990, 992, 993, 995, 996, 997, 999, 1000, 1002,
      1003, 1004, 1006, 1007, 1009, 1010, 1011, 1013, 1014, 1015, 1017, 1018, 1019, 1021, 1022, 1024,
      1025, 1026, 1028, 1029, 1030, 1031, 1033, 1034, 1035, 1037, 1038, 1039, 1041, 1042, 1043, 1044,
      1046, 1047, 1048, 1050, 1051, 1052, 1053, 1055, 1056, 1057, 1058, 1060, 1061, 1062, 1063, 1065,
      1066, 1067, 1068, 1070, 1071, 1072, 1073, 1074, 1076, 1077, 1078, 1079, 1081, 1082, 1083, 1084,
      1085, 1086, 1088, 1089, 1090, 1091, 1092, 1094, 1095, 1096, 1097, 1098, 1099, 1101, 1102, 1103,
      1104, 1105, 1106, 1107, 1109, 1110, 1111, 1112, 1113, 1114, 1115, 1117, 1118, 1119, 1120, 1121,
      1122, 1123, 1124, 1125, 1127, 1128, 1129, 1130, 1131, 1132, 1133, 1134, 1135, 1136, 1138, 1139,
      1140, 1141, 1142, 1143, 1144, 1145, 1146, 1147, 1148, 1149, 1150, 1151, 1152, 1154, 1155, 1156,
      1157, 1158, 1159, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172,
      1173, 1174, 1175, 1176, 1177, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1187, 1188,
      1189, 1190, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204,
      1205, 1206, 1207, 1208, 1209, 1210, 1211, 1212, 1213, 1214, 1215, 1215, 1216, 1217, 1218, 1219,
      1220, 1221, 1222, 1223, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1230, 1231, 1232, 1233, 1234,
      1235, 1236, 1237, 1238, 1239, 1240, 1241, 1242, 1242, 1243, 1244, 1245, 1246, 1247, 1248, 1249,
      1250, 1251, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1259, 1260, 1261, 1262, 1263,
      1264, 1265, 1266, 1266, 1267, 1268, 1269, 1270, 1271, 1272, 1273, 1273, 1274, 1275, 1276, 1277,
      1278, 1279, 1279, 1280, 1281, 1282, 1283, 1284, 1285, 1285, 1286, 1287, 1288, 1289, 1290, 1291
    };

    if (istorgb) {}
    else {

      srcstep -= width * srccn;
      width *= 3;

      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i += 3, src += srccn) {
          int b = src[ 0 ], g = src[ 1 ], r = src[ 2 ^ 0 ];
          int x, y, z, f;
          int L, a;

          x = b * labXb + g * labXg + r * labXr;
          y = b * labYb + g * labYg + r * labYr;
          z = b * labZb + g * labZg + r * labZr;

          f = x > labT;
          x = CC_DESCALE(x, lab_shift);

          if (f) {
            assert((unsigned) x < 512), x = icvLabCubeRootTab[ x ];
          }
          else {
            x = CC_DESCALE(x * labSmallScale + labSmallShift, lab_shift);
          }

          f = z > labT;
          z = CC_DESCALE(z, lab_shift);

          if (f) {
            assert((unsigned) z < 512), z = icvLabCubeRootTab[ z ];
          }
          else {
            z = CC_DESCALE(z * labSmallScale + labSmallShift, lab_shift);
          }

          f = y > labT;
          y = CC_DESCALE(y, lab_shift);

          if (f) {
            assert((unsigned) y < 512), y = icvLabCubeRootTab[ y ];
            L = CC_DESCALE(y * labLScale - labLShift, 2 * lab_shift);
          }
          else {
            L = CC_DESCALE(y * labLScale2, lab_shift);
            y = CC_DESCALE(y * labSmallScale + labSmallShift, lab_shift);
          }

          a = CC_DESCALE(500 * (x - y), lab_shift) + 128;
          b = CC_DESCALE(200 * (y - z), lab_shift) + 128;

          dst[ i + 0 ] = CC_CAST_8U(L);
          dst[ i + 1 ] = CC_CAST_8U(a);
          dst[ i + 2 ] = CC_CAST_8U(b);
        }
      }

    }
  }
  break;

  case T_Luv: {
    if (istorgb) {}
    else {}
  } break;

  case T_YUV420P: {
    if (istorgb) {
      // Consider a YUV420P image of 8x2 pixels.
      //
      // A plane of Y values    A B C D E F G H
      //                        I J K L M N O P
      //
      // A plane of U values    1   2   3   4
      // A plane of V values    1   2   3   4 ....
      //
      // The U1/V1 samples correspond to the ABIJ pixels.
      //     U2/V2 samples correspond to the CDKL pixels.
      //
      // Converts from planar YUV420P to RGB24.
      const int numpix = width * height;
      int i, j, y00, y01, y10, y11, u, v;
      const uchar* pY = src;
      const uchar* pU = pY + numpix;
      const uchar* pV = pU + numpix / 4;
      dststep = dststep - width * dstcn;

      for (j = 0; j <= height - 2; j += 2) {
        dst0 = dst + j * dststep;

        for (i = 0; i <= width - 2; i += 2) {
          y00 = *pY;
          y01 = *(pY + 1);
          y10 = *(pY + width);
          y11 = *(pY + width + 1);
          u = (*pU++) - 128;
          v = (*pV++) - 128;

          dst1 = dst0 + dstcn;
          dst2 = dst0 + dststep;
          dst3 = dst0 + dststep + dstcn;
          move_420_block1(y00, y01, y10, y11, u, v, rgb0, rgb1, rgb2, rgb3);
          SETRGB(dst, rgb0[2], rgb0[1], rgb0[0]);
          SETRGB(dst + dstcn, rgb1[2], rgb1[1], rgb1[0]);
          SETRGB(dst + dststep, rgb2[2], rgb2[1], rgb2[0]);
          SETRGB(dst + dststep + dstcn, rgb3[2], rgb3[1], rgb3[0]);
          pY += 2;
          dst0 += 2 * dstcn;
        }

        pY += width;
      }

    }
    else {}
  } break;

  case T_YUV420: {
    if (istorgb) {
      // Consider a YUV420 image of 6x2 pixels.
      //
      // A B C D U1 U2
      // I J K L V1 V2
      //
      // The U1/V1 samples correspond to the ABIJ pixels.
      //     U2/V2 samples correspond to the CDKL pixels.
      //
      /* Converts from interlaced YUV420 to RGB24. */
      /* [FD] untested... */
      int i, j, y00, y01, y10, y11, u, v;
      const uchar* pY = src;
      const uchar* pU = pY + 4;
      const uchar* pV = pU + width;

      for (j = 0; j <= height - 2; j += 2) {
        dst0 = dst + j * dststep;

        for (i = 0; i <= width - 4; i += 4) {
          y00 = *pY;
          y01 = *(pY + 1);
          y10 = *(pY + width);
          y11 = *(pY + width + 1);
          u = (*pU++) - 128;
          v = (*pV++) - 128;
          dst1 = dst0 + dstcn;
          dst2 = dst0 + dststep;
          dst3 = dst0 + dststep + dstcn;
          move_420_block1(y00, y01, y10, y11, u, v, rgb0, rgb1, rgb2, rgb3);
          SETRGB(dst0, rgb0[2], rgb0[1], rgb0[0]);
          SETRGB(dst1, rgb1[2], rgb1[1], rgb1[0]);
          SETRGB(dst2, rgb2[2], rgb2[1], rgb2[0]);
          SETRGB(dst3, rgb3[2], rgb3[1], rgb3[0]);

          pY += 2;
          dst0 += 2 * dstcn;

          y00 = *pY;
          y01 = *(pY + 1);
          y10 = *(pY + width);
          y11 = *(pY + width + 1);
          u = (*pU++) - 128;
          v = (*pV++) - 128;

          dst1 = dst0 + dstcn;
          dst2 = dst0 + dststep;
          dst3 = dst0 + dststep + dstcn;
          move_420_block1(y00, y01, y10, y11, u, v, rgb0, rgb1, rgb2, rgb3);
          SETRGB(dst, rgb0[2], rgb0[1], rgb0[0]);
          SETRGB(dst + dstcn, rgb1[2], rgb1[1], rgb1[0]);
          SETRGB(dst + dststep, rgb2[2], rgb2[1], rgb2[0]);
          SETRGB(dst + dststep + dstcn, rgb3[2], rgb3[1], rgb3[0]);

          pY += 4; // skip UV
          dst0 += 2 * dstcn;
        }

        pY += width;
      }
    }
    else {}
  } break;

  case T_YUV411P: {
    if (istorgb) {
      // Consider a YUV411P image of 8x2 pixels.
      //
      // A plane of Y values as before.
      //
      // A plane of U values    1       2
      //                        3       4
      //
      // A plane of V values    1       2
      //                        3       4
      //
      // The U1/V1 samples correspond to the ABCD pixels.
      //     U2/V2 samples correspond to the EFGH pixels.
      //
      /* Converts from planar YUV411P to RGB24. */
      /* [FD] untested... */
      const int numpix = width * height;
      int i, j, y00, y01, y10, y11, u, v;
      uchar* dst0 = dst;
      uchar* dst1 = dst;
      uchar* dst2 = dst;
      uchar* dst3 = dst;
      const uchar* pY = src;
      const uchar* pU = pY + numpix;
      const uchar* pV = pU + numpix / 4;
      uchar* pOut = dst;

      for (j = 0; j < height; j++) {
        dst0 = dst + j * dststep;

        for (i = 0; i < width - 4; i += 4) {
          y00 = *pY;
          y01 = *(pY + 1);
          y10 = *(pY + 2);
          y11 = *(pY + 3);
          u = (*pU++) - 128;
          v = (*pV++) - 128;

          move_411_block1(y00, y01, y10, y11, u, v, rgb0, rgb1, rgb2, rgb3);
          dst1 = (dst0 + 1 * dstcn);
          dst2 = (dst0 + 2 * dstcn);
          dst3 = (dst0 + 3 * dstcn);

          SETRGB(dst0, rgb0[2], rgb0[1], rgb0[0]);
          SETRGB(dst1, rgb1[2], rgb1[1], rgb1[0]);
          SETRGB(dst2, rgb2[2], rgb2[1], rgb2[0]);
          SETRGB(dst3, rgb3[2], rgb3[1], rgb3[0]);
          pY += 4;
          dst += 4 * dstcn;
        }
      }
    }
    else {}
  } break;

  case T_YUYV: {
    if (istorgb) {
      // convert from 4:2:2 YUYV interlaced to RGB24
      // based on ccvt_yuyv_bgr32() from camstream
      int i, c, r, g, b, cr, cg, cb, y1, y2;

      if (T_GRAY != dsttype) {
        for (i = 0; i < height; ++i) {
          const uchar* s = src + i * srcstep;
          uchar* d = dst + i * dststep;
          c = width >> 1;

          while (c--) {
            y1 = *s++;
            cb = ((*s - 128) * 454) >> 8;
            cg = (*s++ - 128) * 88;
            y2 = *s++;
            cr = ((*s - 128) * 359) >> 8;
            cg = (cg + (*s++ - 128) * 183) >> 8;

            r = y1 + cr, b = y1 + cb, g = y1 - cg;
            SETRGB(d, BOUND(r, 0, 255), BOUND(g, 0, 255), BOUND(b, 0, 255));
            d += dstcn;

            r = y2 + cr, b = y2 + cb, g = y2 - cg;
            SETRGB(d, BOUND(r, 0, 255), BOUND(g, 0, 255), BOUND(b, 0, 255));
            d += dstcn;
          }
        }
      }
      else {
        const uchar* s;
        uchar* d;
        int l, c, y1, y2;

        for (l = 0; l < height; ++l) {
          s = src + l * srcstep;
          d = dst + l * dststep;

          for (c = 0; c < width; c += 4) {
            y1 = s[0];
            y2 = s[2];

            d[0] = y1;
            d[1] = y2;

            y1 = s[4];
            y2 = s[6];

            d[2] = y1;
            d[3] = y2;

            s += 8;
            d += 4;
          }
        }
      }
    }
    else {}
  } break;

  case T_UYVY: {
    if (istorgb) {
      int i, c, r, g, b, cr, cg, cb, y1, y2;

      if (T_GRAY != dsttype) {
        for (i = 0; i < height; ++i) {
          const uchar* s = src + i * srcstep;
          uchar* d = dst + i * dststep;
          c = width >> 1;

          while (c--) {
            cb = ((*s - 128) * 454) >> 8;
            cg = (*s++ - 128) * 88;
            y1 = *s++;
            cr = ((*s - 128) * 359) >> 8;
            cg = (cg + (*s++ - 128) * 183) >> 8;
            y2 = *s++;

            r = y1 + cr, b = y1 + cb, g = y1 - cg;
            SETRGB(d, BOUND(r, 0, 255), BOUND(g, 0, 255), BOUND(b, 0, 255));
            d += dstcn;

            r = y2 + cr, b = y2 + cb, g = y2 - cg;
            SETRGB(d, BOUND(r, 0, 255), BOUND(g, 0, 255), BOUND(b, 0, 255));
            d += dstcn;
          }
        }
      }
      else {
        const uchar* s;
        uchar* d;
        int l, c, y1, y2;

        for (l = 0; l < height; ++l) {
          s = src + l * srcstep;
          d = dst + l * dststep;

          for (c = 0; c < width; c += 4) {
            y1 = s[1];
            y2 = s[3];

            d[0] = y1;
            d[1] = y2;

            y1 = s[5];
            y2 = s[7];

            d[2] = y1;
            d[3] = y2;

            s += 8;
            d += 4;
          }
        }

      }
    }
    else {
      if (T_GRAY == srctype) {
        const uchar* s;
        uchar* d;
        int l, c, y1, y2;

        for (l = 0; l < height; ++l) {
          s = src + l * srcstep;
          d = dst + l * dststep;

          for (c = 0; c < width; c += 4) {
            y1 = s[0];
            y2 = s[1];

            d[1] = y1;
            d[3] = y2;

            y1 = s[2];
            y2 = s[3];

            d[5] = y1;
            d[7] = y2;

            s += 4;
            d += 8;
          }
        }
      }
    }
  }
  break;

  case T_SN9C10X: {
    if (istorgb) {}
    else {}
  } break;

  case T_SGBRG: {
    if (istorgb) {
      // SGBRG to RGB24
      // for some reason, red and blue needs to be swapped
      // at least for  046d:092f Logitech, Inc. QuickCam Express Plus to work
      //see: http://www.siliconimaging.com/RGB%20Bayer.htm
      //and 4.6 at http://tldp.org/HOWTO/html_single/libdc1394-HOWTO/
      long int i;
      const uchar* rawpt;
      uchar* scanpt;
      long int size;

      rawpt = src;
      scanpt = dst;
      size = width * height;

      for (i = 0; i < size; i++) {
        if ((i / width) % 2 == 0) {    //even row
          if ((i % 2) == 0) {    //even pixel
            if ((i > width) && ((i % width) > 0)) {
              *scanpt++ = (*(rawpt - 1) + * (rawpt + 1)) / 2;            /* R */
              *scanpt++ = *(rawpt);                          /* G */
              *scanpt++ = (*(rawpt - width) + * (rawpt + width)) / 2;           /* B */
            }
            else {
              /* first line or left column */
              *scanpt++ = *(rawpt + 1);            /* R */
              *scanpt++ = *(rawpt);                /* G */
              *scanpt++ = *(rawpt + width);        /* B */
            }
          }
          else { //odd pixel
            if ((i > width) && ((i % width) < (width - 1))) {
              *scanpt++ = *(rawpt);         /* R */
              *scanpt++ = (*(rawpt - 1) + * (rawpt + 1) + * (rawpt - width) + * (rawpt + width)) / 4;        /* G */
              *scanpt++ = (*(rawpt - width - 1) + * (rawpt - width + 1) + * (rawpt + width - 1) + * (rawpt + width + 1)) / 4;             /* B */
            }
            else {
              /* first line or right column */

              *scanpt++ = *(rawpt);         /* R */
              *scanpt++ = (*(rawpt - 1) + * (rawpt + width)) / 2;      /* G */
              *scanpt++ = *(rawpt + width - 1);        /* B */
            }
          }
        }
        else {   //odd row
          if ((i % 2) == 0) {    //even pixel
            if ((i < (width * (height - 1))) && ((i % width) > 0)) {
              *scanpt++ = (*(rawpt - width - 1) + * (rawpt - width + 1) + * (rawpt + width - 1) + * (rawpt + width + 1)) / 4;                 /* R */
              *scanpt++ = (*(rawpt - 1) + * (rawpt + 1) + * (rawpt - width) + * (rawpt + width)) / 4;             /* G */
              *scanpt++ = *(rawpt);   /* B */
            }
            else {
              /* bottom line or left column */

              *scanpt++ = *(rawpt - width + 1);            /* R */
              *scanpt++ = (*(rawpt + 1) + * (rawpt - width)) / 2;           /* G */
              *scanpt++ = *(rawpt);   /* B */
            }
          }
          else {   //odd pixel
            if (i < (width * (height - 1)) && ((i % width) < (width - 1))) {
              *scanpt++ = (*(rawpt - width) + * (rawpt + width)) / 2;       /* R */
              *scanpt++ = *(rawpt);        /* G */
              *scanpt++ = (*(rawpt - 1) + * (rawpt + 1)) / 2;      /* B */
            }
            else {
              /* bottom line or right column */

              *scanpt++ = (*(rawpt - width));      /* R */
              *scanpt++ = *(rawpt);        /* G */
              *scanpt++ = (*(rawpt - 1));     /* B */
            }
          }
        }

        rawpt++;
      }

    }
    else {}
  } break;

  default:
    ASSERT(0);
    break;
  }
}
#undef GETRGB
#undef SETRGB
#undef scale_macro
#undef cast_macro

