#ifndef _CVTCOLOR_INL_
#define _CVTCOLOR_INL_

//  * Turn a YUV4:2:0 block into an RGB block
//  *
//  * Video4Linux seems to use the blue, green, red channel
//  * order convention-- rgb[0] is blue, rgb[1] is green, rgb[2] is red.
//  *
//  * Color space conversion coefficients taken from the excellent
//  * http://www.inforamp.net/~poynton/ColorFAQ.html
//  * In his terminology, this is a CCIR 601.1 YCbCr -> RGB.
//  * Y values are given for all 4 pixels, but the U (Pb)
//  * and V (Pr) are assumed constant over the 2x2 block.
//  *
//  * To avoid floating point arithmetic, the color conversion
//  * coefficients are scaled into 16.16 fixed-point integers.
//  * They were determined as follows:
//  *
//  * double brightness = 1.0;  (0->black; 1->full scale)
//  * double saturation = 1.0;  (0->greyscale; 1->full color)
//  * double fixScale = brightness * 256 * 256;
//  * int rvScale = (int)(1.402 * saturation * fixScale);
//  * int guScale = (int)(-0.344136 * saturation * fixScale);
//  * int gvScale = (int)(-0.714136 * saturation * fixScale);
//  * int buScale = (int)(1.772 * saturation * fixScale);
//  * int yScale = (int)(fixScale);


/* LIMIT: convert a 16.16 fixed-point value to a byte, with clipping. */
#define LIMIT(x) ((x)>0xffffff?0xff: ((x)<=0xffff?0:((x)>>16)))

CC_INLINE int move_420_block1(int yTL, int yTR, int yBL, int yBR, int u, int v, uchar* rgb0, uchar* rgb1, uchar* rgb2, uchar* rgb3)
{
  const int rvScale = 91881;
  const int guScale = -22553;
  const int gvScale = -46801;
  const int buScale = 116129;
  const int yScale = 65536;
  int r, g, b;

  g = guScale * u + gvScale * v;
  //  if (force_rgb) {
  //      r = buScale * u;
  //      b = rvScale * v;
  //  } else {
  r = rvScale * v;
  b = buScale * u;
  //  }

  yTL *= yScale;
  yTR *= yScale;
  yBL *= yScale;
  yBR *= yScale;

  /* Write out top two pixels */
  rgb0[ 0 ] = LIMIT(b + yTL);
  rgb0[ 1 ] = LIMIT(g + yTL);
  rgb0[ 2 ] = LIMIT(r + yTL);

  rgb1[ 0 ] = LIMIT(b + yTR);
  rgb1[ 1 ] = LIMIT(g + yTR);
  rgb1[ 2 ] = LIMIT(r + yTR);

  /* Skip down to next line to write out bottom two pixels */
  rgb2[ 0 ] = LIMIT(b + yBL);
  rgb2[ 1 ] = LIMIT(g + yBL);
  rgb2[ 2 ] = LIMIT(r + yBL);

  rgb3[ 0 ] = LIMIT(b + yBR);
  rgb3[ 1 ] = LIMIT(g + yBR);
  rgb3[ 2 ] = LIMIT(r + yBR);
  return 0;
}

CC_INLINE int move_411_block1(int yTL, int yTR, int yBL, int yBR, int u, int v, uchar* rgb0, uchar* rgb1, uchar* rgb2, uchar* rgb3)
{
  const int rvScale = 91881;
  const int guScale = -22553;
  const int gvScale = -46801;
  const int buScale = 116129;
  const int yScale = 65536;
  int r, g, b;

  g = guScale * u + gvScale * v;
  //  if (force_rgb) {
  //      r = buScale * u;
  //      b = rvScale * v;
  //  } else {
  r = rvScale * v;
  b = buScale * u;
  //  }

  yTL *= yScale;
  yTR *= yScale;
  yBL *= yScale;
  yBR *= yScale;

  /* Write out top two first pixels */
  rgb0[ 0 ] = LIMIT(b + yTL);
  rgb0[ 1 ] = LIMIT(g + yTL);
  rgb0[ 2 ] = LIMIT(r + yTL);

  rgb1[ 0 ] = LIMIT(b + yTR);
  rgb1[ 1 ] = LIMIT(g + yTR);
  rgb1[ 2 ] = LIMIT(r + yTR);

  /* Write out top two last pixels */
  rgb2[ 0 ] = LIMIT(b + yBL);
  rgb2[ 1 ] = LIMIT(g + yBL);
  rgb2[ 2 ] = LIMIT(r + yBL);

  rgb3[ 0 ] = LIMIT(b + yBR);
  rgb3[ 1 ] = LIMIT(g + yBR);
  rgb3[ 2 ] = LIMIT(r + yBR);
  return 0;
}


static const uchar color_cn[] = {
#define COLOR_TYPE_DEF(a, b)  a,
#include "color_type_def.txt"
};

#define GETPIXRGB(pix)   (r = (pix)[0], g = (pix)[1], b = (pix)[2])
#define SETPIXRGB(pix)   ((pix)[0] = r, (pix)[1] = g, (pix)[2] = b)

int cvtcolor(int height, int width, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int srctype, int dsttype, int istorgb)
{
  int type1 = istorgb ? dsttype : srctype;
  int type2 = istorgb ? srctype : dsttype;
  uchar r, g, b;
  uchar rgb0[4] = {0}, rgb1[4] = {0}, rgb2[4] = {0}, rgb3[4] = {0};
  uchar* dst0 = 0, * dst1 = 0, * dst2 = 0, * dst3 = 0;
  //int ir, ig, ib;
  typedef int worktype;
  typedef uchar arrtype;
  ASSERT(srctype < countof(color_cn) && dsttype < countof(color_cn));
  ASSERT(color_cn[srctype] == srccn && color_cn[dsttype] == dstcn);

  switch (type1) {
  case T_RGB: {
    enum {};
#define GETRGB(pix, r, g, b)   (r = (pix)[0], g = (pix)[1], b = (pix)[2])
#define SETRGB(pix, r, g, b)   ((pix)[0] = r, (pix)[1] = g, (pix)[2] = b)
#include "impl/cvtcolor_impl.inl"
  }
  break;

  default:
    ASSERT(0);
    break;
  }

  return 0;
}

#if 0

/****************************************************************************************\
*           Transforming 16-bit (565 or 555) RGB to/from 24/32-bit (888[8]) RGB          *
\****************************************************************************************/

/****************************************************************************************\
*                                 Color to/from grayscale                                *
\****************************************************************************************/

#define fix(x,n)      (int)((x)*(1 << (n)) + 0.5)
#undef descale
#define descale       CC_DESCALE

#define cscGr_32f  0.299f
#define cscGg_32f  0.587f
#define cscGb_32f  0.114f

/* BGR/RGB -> GRAY */
#define csc_shift  14
#define cscGr  fix(cscGr_32f,csc_shift)
#define cscGg  fix(cscGg_32f,csc_shift)
#define cscGb  /*fix(cscGb_32f,csc_shift)*/ ((1 << csc_shift) - cscGr - cscGg)

#define icvGRAY2BGR_8u icvGRAY2BGRA_8u
#define icvGRAY2RGB_8u icvGRAY2BGRA_8u
static int icvGRAY2BGRA_8u(COLOR_FUN_ARGDEF)
{
  int i;
  dststep -= width * dstcn;

  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i++, dst += dstcn) {
      dst[0] = dst[1] = dst[2] = src[i];
    }
  }

  return CC_OK;
}

/****************************************************************************************\
*                                      RGB <-> XYZ                                       *
\****************************************************************************************/

/****************************************************************************************\
*                          Non-linear Color Space Transformations                        *
\****************************************************************************************/

// driver color space conversion function for 8u arrays that uses 32f function
// with appropriate pre- and post-scaling.
static int icvABC2BGRA_8u(COLOR_FUN_ARGDEF, color_cvt_f cvtfunc_32f, const float* pre_coeffs, int postscale)
{
  int block_size = MIN(1 << 8, width);
  float buffer[(1 << 8) * 3 ];    // = ( float* ) cvStackAlloc( block_size * 3 * sizeof( buffer[ 0 ] ) );
  int i, di, k;
  int status = CC_OK;

  dststep -= width * dstcn;

  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i += block_size) {
      const uchar* src1 = src + i * 3;
      di = MIN(block_size, width - i);

      for (k = 0; k < di * 3; k += 3) {
        float a = CC_8TO32F(src1[ k ]) * pre_coeffs[ 0 ] + pre_coeffs[ 1 ];
        float b = CC_8TO32F(src1[ k + 1 ]) * pre_coeffs[ 2 ] + pre_coeffs[ 3 ];
        float c = CC_8TO32F(src1[ k + 2 ]) * pre_coeffs[ 4 ] + pre_coeffs[ 5 ];
        buffer[ k ] = a;
        buffer[ k + 1 ] = b;
        buffer[ k + 2 ] = c;
      }

      status = cvtfunc_32f(1, di, (const uchar*)buffer, srcstep, srccn, (uchar*)buffer, dststep, dstcn, blue_idx);

      if (status < 0) {
        return status;
      }

      if (postscale) {
        for (k = 0; k < di * 3; k += 3, dst += dstcn) {
          int b = cvRound(buffer[ k ] * 255.);
          int g = cvRound(buffer[ k + 1 ] * 255.);
          int r = cvRound(buffer[ k + 2 ] * 255.);

          dst[ 0 ] = CC_CAST_8U(b);
          dst[ 1 ] = CC_CAST_8U(g);
          dst[ 2 ] = CC_CAST_8U(r);

          if (dstcn == 4) {
            dst[ 3 ] = 0;
          }
        }
      }
      else {
        for (k = 0; k < di * 3; k += 3, dst += dstcn) {
          int b = cvRound(buffer[ k ]);
          int g = cvRound(buffer[ k + 1 ]);
          int r = cvRound(buffer[ k + 2 ]);

          dst[ 0 ] = CC_CAST_8U(b);
          dst[ 1 ] = CC_CAST_8U(g);
          dst[ 2 ] = CC_CAST_8U(r);

          if (dstcn == 4) {
            dst[ 3 ] = 0;
          }
        }
      }
    }
  }

  return CC_OK;
}


// driver color space conversion function for 8u arrays that uses 32f function
// with appropriate pre- and post-scaling.
static int icvBGRA2ABC_8u(COLOR_FUN_ARGDEF, color_cvt_f cvtfunc_32f, int prescale, const float* post_coeffs)
{
  int block_size = MIN(1 << 8, width);
  float buffer[(1 << 8) * 3 ];    // = ( float* ) cvStackAlloc( block_size * 3 * sizeof( buffer[ 0 ] ) );
  int i, di, k;
  int status = CC_OK;

  srcstep -= width * srccn;

  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i += block_size) {
      uchar* dst1 = dst + i * 3;
      di = MIN(block_size, width - i);

      if (prescale) {
        for (k = 0; k < di * 3; k += 3, src += srccn) {
          float b = CC_8TO32F(src[ 0 ]) * 0.0039215686274509803f;
          float g = CC_8TO32F(src[ 1 ]) * 0.0039215686274509803f;
          float r = CC_8TO32F(src[ 2 ]) * 0.0039215686274509803f;

          buffer[ k ] = b;
          buffer[ k + 1 ] = g;
          buffer[ k + 2 ] = r;
        }
      }
      else {
        for (k = 0; k < di * 3; k += 3, src += srccn) {
          float b = CC_8TO32F(src[ 0 ]);
          float g = CC_8TO32F(src[ 1 ]);
          float r = CC_8TO32F(src[ 2 ]);

          buffer[ k ] = b;
          buffer[ k + 1 ] = g;
          buffer[ k + 2 ] = r;
        }
      }

      status = cvtfunc_32f(1, di, (const uchar*)buffer, srcstep, srccn, (uchar*)buffer, dststep, dstcn, blue_idx);

      if (status < 0) {
        return status;
      }

      for (k = 0; k < di * 3; k += 3) {
        int a = cvRound(buffer[ k ] * post_coeffs[ 0 ] + post_coeffs[ 1 ]);
        int b = cvRound(buffer[ k + 1 ] * post_coeffs[ 2 ] + post_coeffs[ 3 ]);
        int c = cvRound(buffer[ k + 2 ] * post_coeffs[ 4 ] + post_coeffs[ 5 ]);
        dst1[ k ] = CC_CAST_8U(a);
        dst1[ k + 1 ] = CC_CAST_8U(b);
        dst1[ k + 2 ] = CC_CAST_8U(c);
      }
    }
  }

  return CC_OK;
}

/****************************************************************************************\
*                                      RGB <-> HSV                                       *
\****************************************************************************************/


static int icvBGRA2HSV_8u(COLOR_FUN_ARGDEF)
{
  int i;

  return CC_OK;
}


static int icvBGRA2HSV_32f(COLOR_FUN_ARGDEF)
{
  int i;

  return CC_OK;
}

static int icvHSV2BGRA_32f(COLOR_FUN_ARGDEF)
{
  int i;
  return CC_OK;
}

static int icvHSV2BGRA_8u(COLOR_FUN_ARGDEF)
{
  static const float pre_coeffs[] = {2.f, 0.f, 0.0039215686274509803f, 0.f, 1.f, 0.f };
  return icvABC2BGRA_8u(COLOR_FUN_ARG, icvHSV2BGRA_32f, pre_coeffs, 0);
}


/****************************************************************************************\
*                                     RGB <-> HLS                                        *
\****************************************************************************************/

static int icvBGRA2HLS_32f(COLOR_FUN_ARGDEF)
{
  int i;
  srcstep -= width * srccn;
  width *= 3;

  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i += 3, src += srccn) {
      float b = src[ 0 ], g = src[ 1 ], r = src[ 2 ^ 0 ];
      float h = 0.f, s = 0.f, l;
      float vmin, vmax, diff;

      vmax = vmin = r;

      if (vmax < g) {
        vmax = g;
      }

      if (vmax < b) {
        vmax = b;
      }

      if (vmin > g) {
        vmin = g;
      }

      if (vmin > b) {
        vmin = b;
      }

      diff = vmax - vmin;
      l = (vmax + vmin) * 0.5f;

      if (diff > FLT_EPSILON) {
        s = l < 0.5f ? diff / (vmax + vmin) : diff / (2 - vmax - vmin);
        diff = 60.f / diff;

        if (vmax == r) {
          h = (g - b) * diff;
        }
        else if (vmax == g) {
          h = (b - r) * diff + 120.f;
        }
        else {
          h = (r - g) * diff + 240.f;
        }

        if (h < 0.f) {
          h += 360.f;
        }
      }

      dst[ i ] = (uchar)h;
      dst[ i + 1 ] = (uchar)l;
      dst[ i + 2 ] = (uchar)s;
    }
  }

  return CC_OK;
}


static int icvHLS2BGRA_32f(COLOR_FUN_ARGDEF)
{
  int i;
  dststep -= width * dstcn;
  width *= 3;

  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i += 3, dst += dstcn) {
      float h = src[ i ], l = src[ i + 1 ], s = src[ i + 2 ];
      float b, g, r;

      if (s == 0) {
        b = g = r = l;
      }
      else {
        static const int sector_data[][ 3 ] = { {
            1, 3, 0
          }
          , {1, 0, 2}, {3, 0, 1}, {0, 2, 1}, {0, 1, 3}, {2, 1, 0}
        };
        float tab[ 4 ];
        int sector;

        float p2 = l <= 0.5f ? l * (1 + s) : l + s - l * s;
        float p1 = 2 * l - p2;

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

        assert(0 <= h && h < 6);
        sector = cvFloor(h);
        h -= sector;

        tab[ 0 ] = p2;
        tab[ 1 ] = p1;
        tab[ 2 ] = p1 + (p2 - p1) * (1 - h);
        tab[ 3 ] = p1 + (p2 - p1) * h;

        b = tab[ sector_data[ sector ][ 0 ] ];
        g = tab[ sector_data[ sector ][ 1 ] ];
        r = tab[ sector_data[ sector ][ 2 ] ];
      }

      dst[ 0 ] = (uchar)b;
      dst[ 1 ] = (uchar)g;
      dst[ 2 ] = (uchar)r;

      if (dstcn == 4) {
        dst[ 3 ] = 0;
      }
    }
  }

  return CC_OK;
}

static int icvBGRA2HLS_8u(COLOR_FUN_ARGDEF)
{
  static const float post_coeffs[] = {
    0.5f, 0.f, 255.f, 0.f, 255.f, 0.f
  };

  return icvBGRA2ABC_8u(COLOR_FUN_ARG, icvBGRA2HLS_32f, 1, post_coeffs);
}


static int icvHLS2BGRA_8u(COLOR_FUN_ARGDEF)
{
  static const float pre_coeffs[] = {
    2.f, 0.f, 0.0039215686274509803f, 0.f,
    0.0039215686274509803f, 0.f
  };

  return icvABC2BGRA_8u(COLOR_FUN_ARG, icvHLS2BGRA_32f, pre_coeffs, 1);
}


/****************************************************************************************\
*                                     RGB <-> L*a*b*                                     *
\****************************************************************************************/

#define labT   fix(labT_32f*255,lab_shift)

#undef lab_shift
#define lab_shift 10
#define labXr  fix(labXr_32f,lab_shift)
#define labXg  fix(labXg_32f,lab_shift)
#define labXb  fix(labXb_32f,lab_shift)

#define labYr  fix(labYr_32f,lab_shift)
#define labYg  fix(labYg_32f,lab_shift)
#define labYb  fix(labYb_32f,lab_shift)

#define labZr  fix(labZr_32f,lab_shift)
#define labZg  fix(labZg_32f,lab_shift)
#define labZb  fix(labZb_32f,lab_shift)



static int icvBGRA2Lab_8u(COLOR_FUN_ARGDEF)
{
  int i;
  return CC_OK;
}

static int icvBGRA2Lab_32f(COLOR_FUN_ARGDEF)
{
  int i;

  srcstep -= width * srccn;
  width *= 3;

  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i += 3, src += srccn) {
      float b = src[ 0 ], g = src[ 1 ], r = src[ 2 ^ 0 ];
      float x, y, z;
      float L, a;

      x = b * labXb_32f + g * labXg_32f + r * labXr_32f;
      y = b * labYb_32f + g * labYg_32f + r * labYr_32f;
      z = b * labZb_32f + g * labZg_32f + r * labZr_32f;

      if (x > labT_32f) {
        x = cvCbrt(x);
      }
      else {
        x = x * labSmallScale_32f + labSmallShift_32f;
      }

      if (z > labT_32f) {
        z = cvCbrt(z);
      }
      else {
        z = z * labSmallScale_32f + labSmallShift_32f;
      }

      if (y > labT_32f) {
        y = cvCbrt(y);
        L = y * labLScale_32f - labLShift_32f;
      }
      else {
        L = y * labLScale2_32f;
        y = y * labSmallScale_32f + labSmallShift_32f;
      }

      a = 500.f * (x - y);
      b = 200.f * (y - z);

      dst[ i ] = (uchar)L;
      dst[ i + 1 ] = (uchar)a;
      dst[ i + 2 ] = (uchar)b;
    }
  }

  return CC_OK;
}


static int icvLab2BGRA_32f(COLOR_FUN_ARGDEF)
{
  int i;

  dststep -= width * dstcn;
  width *= 3;

  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i += 3, dst += dstcn) {
      float L = src[ i ], a = src[ i + 1 ], b = src[ i + 2 ];
      float x, y, z;
      float g, r;

      L = (L + labLShift_32f) * (1.f / labLScale_32f);
      x = (L + a * 0.002f);
      z = (L - b * 0.005f);
      y = L * L * L;
      x = x * x * x;
      z = z * z * z;

      b = x * labBx_32f + y * labBy_32f + z * labBz_32f;
      g = x * labGx_32f + y * labGy_32f + z * labGz_32f;
      r = x * labRx_32f + y * labRy_32f + z * labRz_32f;

      dst[ 0 ] = (uchar)b;
      dst[ 1 ] = (uchar)g;
      dst[ 2 ] = (uchar)r;

      if (dstcn == 4) {
        dst[ 3 ] = 0;
      }
    }
  }

  return CC_OK;
}

static int icvLab2BGRA_8u(COLOR_FUN_ARGDEF)
{
  // L: [0..255] -> [0..100]
  // a: [0..255] -> [-128..127]
  // b: [0..255] -> [-128..127]
  static const float pre_coeffs[] = { 0.39215686274509809f, 0.f, 1.f, -128.f, 1.f, -128.f };
  return icvABC2BGRA_8u(COLOR_FUN_ARG, icvLab2BGRA_32f, pre_coeffs, 1);
}


/****************************************************************************************\
*                                     RGB <-> L*u*v*                                     *
\****************************************************************************************/

#define luvUn_32f  0.19793943f
#define luvVn_32f  0.46831096f
#define luvYmin_32f  0.05882353f /* 15/255 */

static int icvBGRA2Luv_32f(COLOR_FUN_ARGDEF)
{
  int i;

  srcstep -= width * srccn;
  width *= 3;

  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i += 3, src += srccn) {
      float b = src[ 0 ], g = src[ 1 ], r = src[ 2 ^ 0 ];
      float x, y, z;
      float L, u, v, t;

      x = b * xyzXb_32f + g * xyzXg_32f + r * xyzXr_32f;
      y = b * xyzYb_32f + g * xyzYg_32f + r * xyzYr_32f;
      z = b * xyzZb_32f + g * xyzZg_32f + r * xyzZr_32f;

      if (!x && !y && !z) {
        L = u = v = 0.f;
      }
      else {
        if (y > labT_32f) {
          L = labLScale_32f * cvCbrt(y) - labLShift_32f;
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

      dst[ i ] = (uchar)L;
      dst[ i + 1 ] = (uchar)u;
      dst[ i + 2 ] = (uchar)v;
    }
  }

  return CC_OK;
}


static int icvLuv2BGRA_32f(COLOR_FUN_ARGDEF)
{
  int i;

  dststep -= width * dstcn;
  width *= 3;

  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i += 3, dst += dstcn) {
      float L = src[ i ], u = src[ i + 1 ], v = src[ i + 2 ];
      float x, y, z, t, u1, v1, b, g, r;

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

      dst[ 0 ] = (uchar)b;
      dst[ 1 ] = (uchar)g;
      dst[ 2 ] = (uchar)r;

      if (dstcn == 4) {
        dst[ 3 ] = 0;
      }
    }
  }

  return CC_OK;
}


static int icvBGRA2Luv_8u(COLOR_FUN_ARGDEF)
{
  // L: [0..100] -> [0..255]
  // u: [-134..220] -> [0..255]
  // v: [-140..122] -> [0..255]
  //static const float post_coeffs[] = { 2.55f, 0.f, 1.f, 83.f, 1.f, 140.f };
  static const float post_coeffs[] = {
    2.55f, 0.f, 0.72033898305084743f, 96.525423728813564f,
    0.99609375f, 139.453125f
  };
  return icvBGRA2ABC_8u(COLOR_FUN_ARG, icvBGRA2Luv_32f, 1, post_coeffs);
}

static int icvLuv2BGRA_8u(COLOR_FUN_ARGDEF)
{
  // L: [0..255] -> [0..100]
  // u: [0..255] -> [-134..220]
  // v: [0..255] -> [-140..122]
  static const float pre_coeffs[] = {
    0.39215686274509809f, 0.f, 1.388235294117647f, -134.f,
    1.003921568627451f, -140.f
  };

  return icvABC2BGRA_8u(COLOR_FUN_ARG, icvLuv2BGRA_32f, pre_coeffs, 1);
}

CC_INLINE int icvGRAY2UYVY_8u(COLOR_FUN_ARGDEF)
{


  return CC_OK;
}

// BAYER2RGB24 ROUTINE TAKEN FROM:
// Sonix SN9C10x based webcam basic I/F routines
// Takafumi Mizuno <taka-qce@ls-a.jp>
CC_INLINE int bayer2rgb24(COLOR_FUN_ARGDEF)
{
  long int i;
  const uchar* rawpt;
  uchar* scanpt;
  long int size;

  rawpt = src;
  scanpt = dst;
  size = width * height;

  for (i = 0; i < size; i++) {
    if ((i / width) % 2 == 0) {
      if ((i % 2) == 0) {
        /* B */
        if ((i > width) && ((i % width) > 0)) {
          *scanpt++ = (*(rawpt - width - 1) + * (rawpt - width + 1) + * (rawpt + width - 1) + * (rawpt + width + 1)) / 4;        /* R */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1) + * (rawpt + width) + * (rawpt - width)) / 4;                /* G */
          *scanpt++ = *rawpt;                             /* B */
        }
        else {
          /* first line or left column */
          *scanpt++ = *(rawpt + width + 1);                     /* R */
          *scanpt++ = (*(rawpt + 1) + * (rawpt + width)) / 2;           /* G */
          *scanpt++ = *rawpt;                             /* B */
        }
      }
      else {
        /* (B)G */
        if ((i > width) && ((i % width) < (width - 1))) {
          *scanpt++ = (*(rawpt + width) + * (rawpt - width)) / 2;       /* R */
          *scanpt++ = *rawpt;                             /* G */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1)) / 2;               /* B */
        }
        else {
          /* first line or right column */
          *scanpt++ = *(rawpt + width);       /* R */
          *scanpt++ = *rawpt;             /* G */
          *scanpt++ = *(rawpt - 1);           /* B */
        }
      }
    }
    else {
      if ((i % 2) == 0) {
        /* G(R) */
        if ((i < (width * (height - 1))) && ((i % width) > 0)) {
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1)) / 2;               /* R */
          *scanpt++ = *rawpt;                                     /* G */
          *scanpt++ = (*(rawpt + width) + * (rawpt - width)) / 2;       /* B */
        }
        else {
          /* bottom line or left column */
          *scanpt++ = *(rawpt + 1);           /* R */
          *scanpt++ = *rawpt;                     /* G */
          *scanpt++ = *(rawpt - width);               /* B */
        }
      }
      else {
        /* R */
        if (i < (width * (height - 1)) && ((i % width) < (width - 1))) {
          *scanpt++ = *rawpt;                                     /* R */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1) +
              * (rawpt - width) + * (rawpt + width)) / 4;         /* G */
          *scanpt++ = (*(rawpt - width - 1) + * (rawpt - width + 1) +
              * (rawpt + width - 1) + * (rawpt + width + 1)) / 4;     /* B */
        }
        else {
          /* bottom line or right column */
          *scanpt++ = *rawpt;                             /* R */
          *scanpt++ = (*(rawpt - 1) + * (rawpt - width)) / 2;           /* G */
          *scanpt++ = *(rawpt - width - 1);             /* B */
        }
      }
    }

    rawpt++;
  }

  return CC_OK;
}

CC_INLINE int icvSGBRG2BGRA_8u(COLOR_FUN_ARGDEF)
{
  return CC_OK;
}

typedef struct code_table_t {
  int is_abs;
  int len;
  int val;
} code_table_t;

/* local storage */
static code_table_t table[ 256 ];
static int init_done = 0;


/*
  sonix_decompress_init
  =====================
    pre-calculates a locally stored table for efficient huffman-decoding.

  Each entry at index x in the table represents the codeword
  present at the MSB of byte x.

*/
CC_INLINE void sonix_decompress_init(void)
{
  int i;
  int is_abs, val, len;

  for (i = 0; i < 256; i++) {
    is_abs = 0;
    val = 0;
    len = 0;

    if ((i & 0x80) == 0) {
      /* code 0 */
      val = 0;
      len = 1;
    }
    else if ((i & 0xE0) == 0x80) {
      /* code 100 */
      val = + 4;
      len = 3;
    }
    else if ((i & 0xE0) == 0xA0) {
      /* code 101 */
      val = -4;
      len = 3;
    }
    else if ((i & 0xF0) == 0xD0) {
      /* code 1101 */
      val = + 11;
      len = 4;
    }
    else if ((i & 0xF0) == 0xF0) {
      /* code 1111 */
      val = -11;
      len = 4;
    }
    else if ((i & 0xF8) == 0xC8) {
      /* code 11001 */
      val = + 20;
      len = 5;
    }
    else if ((i & 0xFC) == 0xC0) {
      /* code 110000 */
      val = -20;
      len = 6;
    }
    else if ((i & 0xFC) == 0xC4) {
      /* code 110001xx: unknown */
      val = 0;
      len = 8;
    }
    else if ((i & 0xF0) == 0xE0) {
      /* code 1110xxxx */
      is_abs = 1;
      val = (i & 0x0F) << 4;
      len = 8;
    }

    table[ i ].is_abs = is_abs;
    table[ i ].val = val;
    table[ i ].len = len;
  }

  init_done = 1;
}

/*
  sonix_decompress
  ================
    decompresses an image encoded by a SN9C101 camera controller chip.

  IN    width
    height
    inp         pointer to compressed frame (with header already stripped)
  OUT   outp    pointer to decompressed frame

  Returns 0 if the operation was successful.
  Returns <0 if operation failed.

*/
CC_INLINE int sonix_decompress(COLOR_FUN_ARGDEF)
{

  int row, col;
  int val;
  int bitpos;
  uchar code;
  const uchar* addr;
  const uchar* inp = src;
  uchar* outp = dst;

  if (!init_done) {
    /* do sonix_decompress_init first! */
    return -1;
  }

  bitpos = 0;

  for (row = 0; row < height; row++) {

    col = 0;

    /* first two pixels in first two rows are stored as raw 8-bit */
    if (row < 2) {
      addr = inp + (bitpos >> 3);
      code = (addr[ 0 ] << (bitpos & 7)) | (addr[ 1 ] >> (8 - (bitpos & 7)));
      bitpos += 8;
      *outp++ = code;

      addr = inp + (bitpos >> 3);
      code = (addr[ 0 ] << (bitpos & 7)) | (addr[ 1 ] >> (8 - (bitpos & 7)));
      bitpos += 8;
      *outp++ = code;

      col += 2;
    }

    while (col < width) {
      /* get bitcode from bitstream */
      addr = inp + (bitpos >> 3);
      code = (addr[ 0 ] << (bitpos & 7)) | (addr[ 1 ] >> (8 - (bitpos & 7)));

      /* update bit position */
      bitpos += table[ code ].len;

      /* calculate pixel value */
      val = table[ code ].val;

      if (!table[ code ].is_abs) {
        /* value is relative to top and left pixel */
        if (col < 2) {
          /* left column: relative to top pixel */
          val += outp[ -2 * width ];
        }
        else if (row < 2) {
          /* top row: relative to left pixel */
          val += outp[ -2 ];
        }
        else {
          /* main area: average of left pixel and top pixel */
          val += (outp[ -2 ] + outp[ -2 * width ]) / 2;
        }
      }

      /* store pixel */
      *outp++ = CLAMP255(val);
      col++;
    }
  }

  return CC_OK;
}

CC_INLINE int icvSN9C10X2BGRA_8u(COLOR_FUN_ARGDEF)
{
  sonix_decompress_init();
  sonix_decompress(COLOR_FUN_ARG);
  return bayer2rgb24(COLOR_FUN_ARG);
}

// Conversion from RGB to YUV420
static int RGB2YUV_YR[ 256 ], RGB2YUV_YG[ 256 ], RGB2YUV_YB[ 256 ];
static int RGB2YUV_UR[ 256 ], RGB2YUV_UG[ 256 ], RGB2YUV_UBVR[ 256 ];
static int RGB2YUV_VG[ 256 ], RGB2YUV_VB[ 256 ];

// Conversion from YUV420 to RGB24
static uchar YUV2RGB_CLP[ 1024 ];   //for clip in CCIR601
static int YUV2RGB_RV[ 256 ], YUV2RGB_BU[ 256 ], YUV2RGB_GU[ 256 ], YUV2RGB_GV[ 256 ], YUV2RGB_76309[ 256 ];

// YUV2BGR
static int YUV2RGB_YY[ 256 ], YUV2RGB_UB[ 256 ], YUV2RGB_UG[ 256 ], YUV2RGB_VG[ 256 ], YUV2RGB_VR[ 256 ];

//Initialize conversion table for YUV420 to RGB
CC_INLINE int icvYUVInitTable()
{
  int crv, cbu, cgu, cgv;
  int i, ind;

  crv = 104597;
  cbu = 132201;  /* fra matrise i global.h */
  cgu = 25675;
  cgv = 53279;

  for (i = 0; i < 256; i++) {
    YUV2RGB_RV[ i ] = (i - 128) * crv;
    YUV2RGB_BU[ i ] = (i - 128) * cbu;
    YUV2RGB_GU[ i ] = (i - 128) * cgu;
    YUV2RGB_GV[ i ] = (i - 128) * cgv;
    YUV2RGB_76309[ i ] = 76309 * (i - 16);
  }

  for (i = 0; i < 384; i++) {
    YUV2RGB_CLP[ i ] = 0;
  }

  ind = 384;

  for (i = 0; i < 256; i++) {
    YUV2RGB_CLP[ ind++ ] = i;
  }

  ind = 640;

  for (i = 0; i < 384; i++) {
    YUV2RGB_CLP[ ind++ ] = 255;
  }

  {
    for (i = 0; i < 256; ++i) {
      YUV2RGB_YY[ i ] = (4767 * i) >> 12;
      YUV2RGB_UB[ i ] = (8265 * (i - 128)) >> 12;
      YUV2RGB_UG[ i ] = (1556 * (i - 128)) >> 12;
      YUV2RGB_VG[ i ] = (3330 * (i - 128)) >> 12;
      YUV2RGB_VR[ i ] = (4747 * (i - 128)) >> 12;
    }
  }

  // Table used for RGB to YUV420 conversion
  for (i = 0; i < 256; i++) {
    RGB2YUV_YR[ i ] = (int)((float) 65.481 * (i << 8));
    RGB2YUV_YG[ i ] = (int)((float) 128.553 * (i << 8));
    RGB2YUV_YB[ i ] = (int)((float) 24.966 * (i << 8));
    RGB2YUV_UR[ i ] = (int)((float) 37.797 * (i << 8));
    RGB2YUV_UG[ i ] = (int)((float) 74.203 * (i << 8));
    RGB2YUV_VG[ i ] = (int)((float) 93.786 * (i << 8));
    RGB2YUV_VB[ i ] = (int)((float) 18.214 * (i << 8));
    RGB2YUV_UBVR[ i ] = (int)((float) 112 * (i << 8));
  }

  return CC_OK;
}

// RGB => YUV420
#define RGB2YUV420(R, G, B, Y, U, V)                                          \
  do {                                                                          \
    (Y)=( RGB2YUV_YR[R]  +RGB2YUV_YG[G]+RGB2YUV_YB[B]+1048576)>>16;             \
    (U)=(-RGB2YUV_UR[R]  -RGB2YUV_UG[G]+RGB2YUV_UBVR[B]+8388608)>>16;           \
    (V)=( RGB2YUV_UBVR[R]-RGB2YUV_VG[G]-RGB2YUV_VB[B]+8388608)>>16;             \
  } while(0)

#define CLP(_X) YUV2RGB_CLP[(_X)+384]

// YUV转换为RGB
// R = Y + 1.4075 *（V-128）
// G = Y –0.3455 *（U –128）–0.7169 *（V –128）
// B = Y + 1.779 *（U – 128）
#define YUV2RGB(Y, U, V, R, G, B)                                             \
  do {                                                                          \
    int y, u, v;                                                                \
    y = (Y);                                                                    \
    u = (U) - 128;                                                              \
    v = (V) - 128;                                                              \
    (R) = CLP(y + ((v * 360) >> 8)         );                                   \
    (G) = CLP(y - ((u * 88 + v * 184) >> 8));                                   \
    (B) = CLP(y + ((u * 455) >> 8)         );                                   \
  } while(0)

#define YUV2RGB2(Y, U, V, R, G, B)                                            \
  do {                                                                          \
    int y, u, v, yy;                                                            \
    y = (Y);                                                                    \
    u = (U);                                                                    \
    v = (V);                                                                    \
    yy = y-16;                                                                  \
    (R) = CLP(YUV2RGB_YY[yy] + YUV2RGB_VR[v]                );                  \
    (G) = CLP(YUV2RGB_YY[yy] - YUV2RGB_UG[u] - YUV2RGB_VG[v]);                  \
    (B) = CLP(YUV2RGB_YY[yy] + YUV2RGB_UB[u]                );                  \
  } while(0)

CC_INLINE int icvYUYV2BGRA_8u_table(COLOR_FUN_ARGDEF)
{
  int i, j;
  static int inityuvtab = 0;

  if (inityuvtab == 0) {
    icvYUVInitTable();
    inityuvtab = 1;
  }

  //printf("%d\n", inittab);
  for (i = 0; i < height; ++i) {
    unsigned int yuv_uv[ 2 ] = {0, 0}; // {u0, v1};
    const uchar* src0 = src + i * srcstep;
    uchar* dst0 = dst + i * dststep;

    for (j = 0; j < width; ++j, src0 += srccn, dst0 += dstcn) {
      // UYVY标准  [U0 Y0 V0 Y1] [U1 Y2 V1 Y3] [U2 Y4 V2 Y5] 每像素点两个字节，[内]为四个字节
      yuv_uv[(j & 1) ] = src0[ 1 ];
      YUV2RGB(src0[ 0 ], yuv_uv[ 0 ], yuv_uv[ 1 ], dst0[ 2 ], dst0[ 1 ], dst0[ 0 ]);
    }
  }

  return CC_OK;
}

//#include "icvYUYV2BGR_8u_C2C3R_mms.inl"

CC_INLINE int icvPYUV4222BGR_8u(COLOR_FUN_ARGDEF)
{

  int i, j, offset1, offset2, index;
  int y, u, v, yy, b, g, r;
  static int inityuvtab = 0;

  if (inityuvtab == 0) {
    icvYUVInitTable();
    inityuvtab = 1;
  }

  offset1 = width * height;
  offset2 = offset1 + (width >> 1) * height;

  for (i = 0; i < height; i++) {
    int widthOffset1 = i * width;
    int index1 = widthOffset1 * dstcn;

    for (j = 0; j < width; j += 2) {
      y = src[ widthOffset1 + j ];
      u = src[ offset1 + ((widthOffset1 + j) >> 1) ];
      v = src[ offset2 + ((widthOffset1 + j) >> 1) ];
      yy = y - 16;
      b = YUV2RGB_YY[ yy ] + YUV2RGB_UB[ u ];
      g = YUV2RGB_YY[ yy ] - YUV2RGB_UG[ u ] - YUV2RGB_VG[ v ];
      r = YUV2RGB_YY[ yy ] + YUV2RGB_VR[ v ];

      index = index1 + j * dstcn;
      dst[ index ]     = CC_CAST_8U(b);
      dst[ index + 1 ] = CC_CAST_8U(g);
      dst[ index + 2 ] = CC_CAST_8U(r);

      y = src[ widthOffset1 + (j + 1) ];
      u = src[ offset1 + ((widthOffset1 + (j + 1)) >> 1) ];
      v = src[ offset2 + ((widthOffset1 + (j + 1)) >> 1) ];
      yy = y - 16;
      b = YUV2RGB_YY[ yy ] + YUV2RGB_UB[ u ];
      g = YUV2RGB_YY[ yy ] - YUV2RGB_UG[ u ] - YUV2RGB_VG[ v ];
      r = YUV2RGB_YY[ yy ] + YUV2RGB_VR[ v ];

      index = index1 + (j + 1) * dstcn;
      dst[ index ]     = CC_CAST_8U(b);
      dst[ index + 1 ] = CC_CAST_8U(g);
      dst[ index + 2 ] = CC_CAST_8U(r);
    }
  }

  return CC_OK;
}

//  Convert from YUV420 to RGB24
CC_INLINE int icvYUV420P2BGR_8u_C2C3R(COLOR_FUN_ARGDEF)
{
  int y1, y2, u, v;
  int i, j, c1, c2, c3, c4;
  const uchar* py1, *py2;
  const uchar* src0, *src1, *src2;
  uchar* d1, *d2;

  src0 = src;
  src1 = src + height * width;
  src2 = src1 + (width >> 1) * height;
  py1 = src0;
  py2 = py1 + width;
  d1 = dst;
  d2 = d1 + dststep;

  for (j = 0; j < height; j += 2) {
    for (i = 0; i < width; i += 2) {
      u = *src1++;
      v = *src2++;
      c1 = YUV2RGB_RV[ v ];
      c2 = YUV2RGB_GU[ u ];
      c3 = YUV2RGB_GV[ v ];
      c4 = YUV2RGB_BU[ u ];

      //up-left
      y1 = YUV2RGB_76309[ *py1++ ];
      *d1++ = YUV2RGB_CLP[ 384 + ((y1 + c4) >> 16) ];
      *d1++ = YUV2RGB_CLP[ 384 + ((y1 - c2 - c3) >> 16) ];
      *d1++ = YUV2RGB_CLP[ 384 + ((y1 + c1) >> 16) ];

      //down-left
      y2 = YUV2RGB_76309[ *py2++ ];
      *d2++ = YUV2RGB_CLP[ 384 + ((y2 + c4) >> 16) ];
      *d2++ = YUV2RGB_CLP[ 384 + ((y2 - c2 - c3) >> 16) ];
      *d2++ = YUV2RGB_CLP[ 384 + ((y2 + c1) >> 16) ];

      //up-right
      y1 = YUV2RGB_76309[ *py1++ ];
      *d1++ = YUV2RGB_CLP[ 384 + ((y1 + c4) >> 16) ];
      *d1++ = YUV2RGB_CLP[ 384 + ((y1 - c2 - c3) >> 16) ];
      *d1++ = YUV2RGB_CLP[ 384 + ((y1 + c1) >> 16) ];

      //down-right
      y2 = YUV2RGB_76309[ *py2++ ];
      *d2++ = YUV2RGB_CLP[ 384 + ((y2 + c4) >> 16) ];
      *d2++ = YUV2RGB_CLP[ 384 + ((y2 - c2 - c3) >> 16) ];
      *d2++ = YUV2RGB_CLP[ 384 + ((y2 + c1) >> 16) ];
    }

    d1 += dststep;
    d2 += dststep;
    py1 += width;
    py2 += width;
  }

  return CC_OK;
}

#define YUV420_3x2_BLOCK()                                                                \
  rgb00 = src;                                                                            \
  rgb01 = src + srccn;                                                                   \
  rgb10 = src + srcstep;                                                                  \
  rgb11 = src + srcstep + srccn;                                                         \
  \
  b00 = rgb00[ blue_idx ], g00 = rgb00[ 1 ], r00 = rgb00[ 2 ^ blue_idx ];                 \
  b01 = rgb01[ blue_idx ], g01 = rgb01[ 1 ], r01 = rgb01[ 2 ^ blue_idx ];                 \
  b10 = rgb10[ blue_idx ], g10 = rgb10[ 1 ], r10 = rgb10[ 2 ^ blue_idx ];                 \
  b11 = rgb11[ blue_idx ], g11 = rgb11[ 1 ], r11 = rgb11[ 2 ^ blue_idx ];                 \
  \
  y00 = ( RGB2YUV_YR[ r00 ] + RGB2YUV_YG[ g00 ] + RGB2YUV_YB[ b00 ] + 1048576 ) >> 16;    \
  y01 = ( RGB2YUV_YR[ r01 ] + RGB2YUV_YG[ g01 ] + RGB2YUV_YB[ b01 ] + 1048576 ) >> 16;    \
  y10 = ( RGB2YUV_YR[ r10 ] + RGB2YUV_YG[ g10 ] + RGB2YUV_YB[ b10 ] + 1048576 ) >> 16;    \
  y11 = ( RGB2YUV_YR[ r11 ] + RGB2YUV_YG[ g11 ] + RGB2YUV_YB[ b11 ] + 1048576 ) >> 16;    \
  \
  u00 = ( -RGB2YUV_UR[ r00 ] - RGB2YUV_UG[ g00 ] + RGB2YUV_UBVR[ b00 ] + 8388608 ) >> 16; \
  u01 = ( -RGB2YUV_UR[ r01 ] - RGB2YUV_UG[ g01 ] + RGB2YUV_UBVR[ b01 ] + 8388608 ) >> 16; \
  u10 = ( -RGB2YUV_UR[ r10 ] - RGB2YUV_UG[ g10 ] + RGB2YUV_UBVR[ b10 ] + 8388608 ) >> 16; \
  u11 = ( -RGB2YUV_UR[ r11 ] - RGB2YUV_UG[ g11 ] + RGB2YUV_UBVR[ b11 ] + 8388608 ) >> 16; \
  \
  v00 = ( RGB2YUV_UBVR[ r00 ] - RGB2YUV_VG[ g00 ] - RGB2YUV_VB[ b00 ] + 8388608 ) >> 16;  \
  v01 = ( RGB2YUV_UBVR[ r01 ] - RGB2YUV_VG[ g01 ] - RGB2YUV_VB[ b01 ] + 8388608 ) >> 16;  \
  v10 = ( RGB2YUV_UBVR[ r10 ] - RGB2YUV_VG[ g10 ] - RGB2YUV_VB[ b10 ] + 8388608 ) >> 16;  \
  v11 = ( RGB2YUV_UBVR[ r11 ] - RGB2YUV_VG[ g11 ] - RGB2YUV_VB[ b11 ] + 8388608 ) >> 16;  \
  pY[j] = y00, pY[j+1] = y01, pY[j+dststep] = y01, pY[j+dststep+1] = y11;                 \
  *pU++ = (u00+u01+u10+u11)>>2; *pV++ = (v00+v01+v10+v11)>>2; src+=2, j+=2;

//  Convert from  RGB24 to YUV420
CC_INLINE int icvRGBA2YUV420P_8u(COLOR_FUN_ARGDEF)
{
  int i, j;
  const uchar* rgb00, *rgb01, *rgb10, *rgb11;
  uchar* pY, *pU, *pV;
  int r00, r01, r10, r11, y00, y01, y10, y11;
  int g00, g01, g10, g11, u00, u01, u10, u11;
  int b00, b01, b10, b11, v00, v01, v10, v11;
  const int numpix = width * height;

  srcstep -= width * srccn;

  // planar YUV420P
  pY = dst, pU = pY + numpix, pV = pU + numpix / 4;

  //Get YUV values for rgb values...
  for (i = 0; i < height; i += 2, src += srcstep) {
    for (j = 0; j < width;) {
      YUV420_3x2_BLOCK();
    }
  }


  return CC_OK;
}

CC_INLINE int icvRGB2YUV420_8u(COLOR_FUN_ARGDEF)
{
  int i, j;
  const uchar* rgb00, *rgb01, *rgb10, *rgb11;
  uchar* pY, *pU, *pV;
  int r00, r01, r10, r11, y00, y01, y10, y11;
  int g00, g01, g10, g11, u00, u01, u10, u11;
  int b00, b01, b10, b11, v00, v01, v10, v11;

  srcstep -= width * srccn;

  pY = dst, pU = pY + 4, pV = pU + dststep;

  //Get YUV values for rgb values...
  for (i = 0; i < height; i += 2, src += srcstep) {
    for (j = 0; j < width;) {
      YUV420_3x2_BLOCK();
      YUV420_3x2_BLOCK();
      pY += 2, pU += 4, pV += 4, j += 2;
    }
  }


  return CC_OK;
}

// RGB转YUV的公式
// From RGB to YUV
// Y = 0.299R + 0.587G + 0.114B
// U = 0.492 (B-Y)
// V = 0.877 (R-Y)
// It can also be represented as:
// Y =  0.299R + 0.587G + 0.114B
// U = -0.147R - 0.289G + 0.436B
// V =  0.615R - 0.515G - 0.100B
// From YUV to RGB
// R = Y + 1.140V
// G = Y - 0.395U - 0.581V
// B = Y + 2.032U
CC_INLINE int icvBGRA2YUV_8u(COLOR_FUN_ARGDEF)
{
  int i, j;
  int Y, U, V;

  // [Y U V]
  for (i = 0; i < height; ++i, src += srcstep, dst += dststep) {
    for (j = 0; j < width; ++j, src += srccn, dst += dstcn) {
      int B = (src) [ blue_idx ], G = (src) [ 1 ], R = (src) [ 2 ^ blue_idx ];

      Y = (uchar)((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
      U = (uchar)((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
      V = (uchar)((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

      dst[ 0 ] = CC_CAST_8U(Y);
      dst[ 1 ] = CC_CAST_8U(V);
      dst[ 2 ] = CC_CAST_8U(U);
    }
  }

  return CC_OK;
}

CC_INLINE int icvBGRA2YUYV_8u(COLOR_FUN_ARGDEF)
{
  int i, j;
  int Y, U, V;
  uchar* YUV = dst;
  // [Y U Y V]
  srcstep -= width * srccn;
  dststep -= width * dstcn;

  for (i = 0; i < height; ++i, src += srcstep, YUV += dststep) {
    for (j = 0; j < width; ++j, src += srccn, YUV += dstcn) {
      int B = (src) [ blue_idx ], G = (src) [ 1 ], R = (src) [ 2 ^ blue_idx ];
      Y = (uchar)((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
      YUV[ 0 ] = CC_CAST_8U(Y);

      if (j & 1) {
        V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
        YUV[ 1 ] = CC_CAST_8U(V);
      }
      else {
        U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
        YUV[ 1 ] = CC_CAST_8U(U);
      }
    }
  }

  return CC_OK;
}

CC_INLINE int icvBGRA2UYVY_8u(COLOR_FUN_ARGDEF)
{
  int i, j;
  int Y, U, V;
  uchar* uyvy = dst;
  // [U Y V Y]
  srcstep -= width * srccn;
  dststep -= width * dstcn;

  for (i = 0; i < height; ++i, src += srcstep, uyvy += dststep) {
    for (j = 0; j < width; ++j, src += srccn, uyvy += dstcn) {
      int B = (src) [ blue_idx ], G = (src) [ 1 ], R = (src) [ 2 ^ blue_idx ];
      Y = (uchar)((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
      uyvy[ 1 ] = CC_CAST_8U(Y);

      if (j & 1) {
        V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
        uyvy[ 0 ] = CC_CAST_8U(V);
      }
      else {
        U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
        uyvy[ 0 ] = CC_CAST_8U(U);
      }
    }
  }

  return CC_OK;
}

CC_INLINE int yuv4222yuv420a(COLOR_FUN_ARGDEF)
{
  //#define iSourLinePitch  720
  int iSourLoc = 0;
  int iDstLoc = 0;

  //  while()
  int ilineNum = 0;
  int iRow = 0;
  iSourLoc = 0;
  iDstLoc = 0;

  iSourLoc += srcstep * height;
  iDstLoc += srcstep * height;

  for (ilineNum = 0; ilineNum < height / 2; ilineNum++) {
    for (iRow = 0; iRow < width / 2; iRow++) {
      *(dst + iDstLoc + ilineNum * dststep / 2 + iRow) = (
          *(src + iSourLoc + (ilineNum * 2 + 0) * srcstep / 2 + iRow * 2 + 0) +
          * (src + iSourLoc + (ilineNum * 2 + 0) * srcstep / 2 + iRow * 2 + 1) +
          * (src + iSourLoc + (ilineNum * 2 + 1) * srcstep / 2 + iRow * 2 + 0) +
          * (src + iSourLoc + (ilineNum * 2 + 1) * srcstep / 2 + iRow * 2 + 1)
          ) >> 2;
    }
  }

  iSourLoc += srcstep / 2 * height;
  iDstLoc += width * height / 4;

  for (ilineNum = 0; ilineNum < height / 2; ilineNum++) {
    for (iRow = 0; iRow < width / 2; iRow++) {
      *(dst + iDstLoc + ilineNum * dststep / 2 + iRow) = (
          *(src + iSourLoc + (ilineNum * 2 + 0) * srcstep / 2 + iRow * 2 + 0) +
          * (src + iSourLoc + (ilineNum * 2 + 0) * srcstep / 2 + iRow * 2 + 1) +
          * (src + iSourLoc + (ilineNum * 2 + 1) * srcstep / 2 + iRow * 2 + 0) +
          * (src + iSourLoc + (ilineNum * 2 + 1) * srcstep / 2 + iRow * 2 + 1)
          ) >> 2;
    }
  }

  return 1;
}

CC_INLINE int yuyv2uyvy(COLOR_FUN_ARGDEF)
{
  int i, numpix = width * height;
  const uchar* yuyv = (uchar*)src;
  uchar* uyvy = dst, t0, t1;

  for (i = 0; i < numpix * 2; i += 2) {
    t0 = yuyv[i];
    t1 = yuyv[i + 1];
    uyvy[i + 1] = t0;
    uyvy[i] = t1;
  }

  return 0;
}

CC_INLINE int yuv4222yuv420(COLOR_FUN_ARGDEF)
{
  const int numpix = width * height;
  int i, j, y00, y01, y10, y11, u0, v0, u1, v1;
  const uchar* pY = src;
  uchar* poutY = dst;
  uchar* poutU = poutY + numpix;
  uchar* poutV = poutU + numpix / 4;
  //uchar* pOut = dst;

  for (j = 0; j <= height - 2; j += 2) {
    for (i = 0; i <= width - 2; i += 2) {
      y00 = *pY;
      u0 = *(pY + 1);
      y01 = *(pY + 2);
      v0 = *(pY + 3);
      y10 = *(pY + width);
      u1 = *(pY + width + 1);
      y11 = *(pY + width + 2);
      v1 = *(pY + width + 3);
      *(poutY) = y00;
      *(poutY + 1) = y01;
      *(poutY + width) = y10;
      *(poutY + width + 1) = y11;
      *poutU++ = (u0 + u1) >> 1;
      *poutV++ = (v0 + v1) >> 1;
      pY += 4;
      poutY += 2;
    }

    pY += 2 * width;
    poutY += width;
  }

  return 0;
}




// The main function

#define icvBGRA2YUV420P_8u 0
#define icvBGRA2YUV420_8u 0
#define icvBGRA2YVU420_8u 0
#define icvBGRA2YUV411P_8u 0

#define icvYUYV2BGRA_8u icvYUYV2BGRA_8u_table
//#define icvBGRA2YUYV_8u 0
//#define icvBGRA2UYVY_8u 0

#define icvBGRA2SBGGR8_8u 0

#define icvBGRA2SN9C10X_8u 0

#define icvBGRA2SGBRG_8u 0
#define icvBGRA2BayerGR_8u 0
#define icvBGRA2BayerGB_8u 0
#define icvBGRA2BayerBG_8u 0
#define icvBGRA2BGRA_8u 0


#define icvGRAY2GRAY_8u 0
#define icvBayerGR2GRAY_8u 0
#define icvGRAY2BayerGR_8u 0
#define icvBayerGB2GRAY_8u 0
#define icvGRAY2BayerGB_8u 0
#define icvBayerBG2GRAY_8u 0
#define icvGRAY2BayerBG_8u 0

enum {T_MAXNUM = 30};
static int _any_code_cn[ T_MAXNUM ] = {
#define COLOR_TYPE_DEF(_N, _T) _N,
#include "../color_type_def.txt"
#undef COLOR_TYPE_DEF
};

#endif

#endif // _CVTCOLOR_INL_
