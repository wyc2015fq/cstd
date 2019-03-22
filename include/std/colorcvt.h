#ifndef _COLOR_INL_
#define _COLOR_INL_
//#include "cstd.h"
//#include "precomp.hpp"

#include <math.h>
#include <float.h>

#define ChannelBlend_Normal(A,B)       ((uint8)(A))
#define ChannelBlend_Lighten(A,B)      ((uint8)((B > A) ? B:A))
#define ChannelBlend_Darken(A,B)       ((uint8)((B > A) ? A:B))
#define ChannelBlend_Multiply(A,B)     ((uint8)((A * B) / 255))
#define ChannelBlend_Average(A,B)      ((uint8)((A + B) / 2))
#define ChannelBlend_Add(A,B)          ((uint8)(min(255, (A + B))))
#define ChannelBlend_Subtract(A,B)     ((uint8)((A + B < 255) ? 0:(A + B - 255)))
#define ChannelBlend_Difference(A,B)   ((uint8)(abs(A - B)))
#define ChannelBlend_Negation(A,B)     ((uint8)(255 - abs(255 - A - B)))
#define ChannelBlend_Screen(A,B)       ((uint8)(255 - (((255 - A) * (255 - B))>> 8)))
#define ChannelBlend_Exclusion(A,B)    ((uint8)(A + B - 2 * A * B / 255))
#define ChannelBlend_Overlay(A,B)      ((uint8)((B < 128) ? (2 * A * B / 255):(255 - 2 * (255 - A) * (255 - B) / 255)))
#define ChannelBlend_SoftLight(A,B)    ((uint8)((B < 128)?(2*((A>>1)+64))*((float)B/255):(255-(2*(255-((A>>1)+64))*(float)(255-B)/255))))
#define ChannelBlend_HardLight(A,B)    (ChannelBlend_Overlay(B,A))
#define ChannelBlend_ColorDodge(A,B)   ((uint8)((B == 255) ? B:min(255, ((A << 8 ) / (255 - B)))))
#define ChannelBlend_ColorBurn(A,B)    ((uint8)((B == 0) ? B:max(0, (255 - ((255 - A) << 8 ) / B))))
#define ChannelBlend_LinearDodge(A,B)  (ChannelBlend_Add(A,B))
#define ChannelBlend_LinearBurn(A,B)   (ChannelBlend_Subtract(A,B))
#define ChannelBlend_LinearLight(A,B)  ((uint8)(B < 128)?ChannelBlend_LinearBurn(A,(2 * B)):ChannelBlend_LinearDodge(A,(2 * (B - 128))))
#define ChannelBlend_VividLight(A,B)   ((uint8)(B < 128)?ChannelBlend_ColorBurn(A,(2 * B)):ChannelBlend_ColorDodge(A,(2 * (B - 128))))
#define ChannelBlend_PinLight(A,B)     ((uint8)(B < 128)?ChannelBlend_Darken(A,(2 * B)):ChannelBlend_Lighten(A,(2 * (B - 128))))
#define ChannelBlend_HardMix(A,B)      ((uint8)((ChannelBlend_VividLight(A,B) < 128) ? 0:255))
#define ChannelBlend_Reflect(A,B)      ((uint8)((B == 255) ? B:min(255, (A * A / (255 - B)))))
#define ChannelBlend_Glow(A,B)         (ChannelBlend_Reflect(B,A))
#define ChannelBlend_Phoenix(A,B)      ((uint8)(min(A,B) - max(A,B) + 255))
#define ChannelBlend_Alpha(A,B,O)      ((uint8)(O * A + (1 - O) * B))
#define ChannelBlend_AlphaF(A,B,F,O)   (ChannelBlend_Alpha(F(A,B),A,O))

typedef unsigned short COLOR16;

typedef struct _TRIVER_TEX {
  int x, y;
  unsigned short r, g, b, a;
} TRIVER_TEX;


// FUNCTION : PixelAlpha ()
// * Description : Shades a color value with a specified percentage
// * Author : [Guillaume Nodet]
// * Returns : [COLOR] - The result pixel
// * Function parameters :
// [srcPixel] - The source pixel
// [dstPixel] - The destination pixel
// [percent] - Percentage (amount of shadow)
CC_INLINE COLOR PixelAlpha3(COLOR srcPixel, COLOR dstPixel, int percent)
{
  int ipercent = 100 - percent;
  COLOR clrFinal = _RGB((GetRV(srcPixel) * percent + GetRV(dstPixel) * ipercent) / 100,
                        (GetGV(srcPixel) * percent + GetGV(dstPixel) * ipercent) / 100,
                        (GetBV(srcPixel) * percent + GetBV(dstPixel) * ipercent) / 100);
  return (clrFinal);
}
// Conversion between the HSL (Hue, Saturation, and LuMINosity)
// and RBG color model.
// The conversion algorithms presented here come from the book by
// Fundamentals of Interactive Computer Graphics by Foley and van Dam.
// In the example code, HSL values are represented as floating point
// number in the range 0 to 1. _RGB tridrants use the Windows convention
// of 0 to 255 of each element.
CC_INLINE double HuetoRGB(double m1, double m2, double h)
{
  if (h < 0) {
    h += 1.0;
  }
  if (h > 1) {
    h -= 1.0;
  }
  if (6.0 * h < 1) {
    return (m1 + (m2 - m1) * h * 6.0);
  }
  if (2.0 * h < 1) {
    return m2;
  }
  if (3.0 * h < 2.0) {
    return (m1 + (m2 - m1) * ((2.0 / 3.0) - h) * 6.0);
  }
  return m1;
}
CC_INLINE BYTE HueToRGB(float rm1, float rm2, float rh)
{
  if (rh > 360.0f) {
    rh -= 360.0f;
  } else if (rh < 0.0f) {
    rh += 360.0f;
  }
  if (rh < 60.0f) {
    rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;
  } else if (rh < 180.0f) {
    rm1 = rm2;
  } else if (rh < 240.0f) {
    rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;
  }
  return (BYTE)(rm1 * 255);
}
CC_INLINE COLOR HLStoRGB_ONE(double H, double L, double S)
{
  double r, g, b;
  double m1, m2;
  if (S == 0) {
    r = g = b = L;
  } else {
    if (L <= 0.5) {
      m2 = L * (1.0 + S);
    } else {
      m2 = L + S - L * S;
    }
    m1 = 2.0 * L - m2;
    r = HuetoRGB(m1, m2, H + 1.0 / 3.0);
    g = HuetoRGB(m1, m2, H);
    b = HuetoRGB(m1, m2, H - 1.0 / 3.0);
  }
  return _RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}
CC_INLINE COLOR HLStoRGB_TWO(double H, double L, double S)
{
  WORD R, G, B; // _RGB component values
  if (S == 0.0) {
    R = G = B = (unsigned char)(L * 255.0);
  } else {
    float rm1, rm2;
    if (L <= 0.5f) {
      rm2 = (float)(L + L * S);
    } else {
      rm2 = (float)(L + S - L * S);
    }
    rm1 = (float)(2.0f * L - rm2);
    R = HueToRGB(rm1, rm2, (float)(H + 120.0f));
    G = HueToRGB(rm1, rm2, (float)(H));
    B = HueToRGB(rm1, rm2, (float)(H - 120.0f));
  }
  return _RGB(R, G, B);
}
CC_INLINE void RGBtoHSL(COLOR rgb, double* H, double* S, double* L)
{
  double delta;
  double r = (double)GetRV(rgb) / 255;
  double g = (double)GetGV(rgb) / 255;
  double b = (double)GetBV(rgb) / 255;
  double cmax = MAX(r, MAX(g, b));
  double cMIN = MIN(r, MIN(g, b));
  *L = (cmax + cMIN) / 2.0;
  if (cmax == cMIN) {
    *S = 0;
    *H = 0; // it's really undefined
  } else {
    if (*L < 0.5) {
      *S = (cmax - cMIN) / (cmax + cMIN);
    } else {
      *S = (cmax - cMIN) / (2.0 - cmax - cMIN);
    }
    delta = cmax - cMIN;
    if (r == cmax) {
      *H = (g - b) / delta;
    } else if (g == cmax) {
      *H = 2.0 + (b - r) / delta;
    } else {
      *H = 4.0 + (r - g) / delta;
    }
    *H /= 6.0;
    if (*H < 0.0) {
      *H += 1;
    }
  }
}

// Algoritm by A. R. Smith
CC_INLINE void RGBtoHSV(COLOR rgb, double* H, double* S, double* V)
{
  double r = (double) GetRV(rgb) / 255;
  double g = (double) GetGV(rgb) / 255;
  double b = (double) GetBV(rgb) / 255;
  double dblMin = MIN(r, MIN(g, b));
  double dblMax = MAX(r, MAX(g, b));
  double delta = dblMax - dblMin;
  *V = dblMax; // v
  if (dblMax != 0) {
    *S = delta / dblMax; // s
  } else {
    // r = g = b = 0 // s = 0, v is undefined
    *S = 0;
    *H = -1;
    return;
  }
  if (delta == 0.) {
    *H = 1;
  } else {
    if (r == dblMax) {
      *H = (g - b) / delta; // between yellow & magenta
    } else if (g == dblMax) {
      *H = 2 + (b - r) / delta; // between cyan & yellow
    } else {
      *H = 4 + (r - g) / delta; // between magenta & cyan
    }
  }
  *H *= 60; // degrees
  if (*H < 0) {
    *H += 360;
  }
}
// Algoritm by A. R. Smith
CC_INLINE COLOR HSVtoRGB(double h, double s, double v)
{
  int i;
  double f, p, q, t;
  double r, g, b;
  if (s == 0) {
    // achromatic (grey)
    r = g = b = v;
  } else {
    h /= 60; // sector 0 to 5
    i = (int) floor(h);
    f = h - i; // factorial part of h
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch (i) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default: // case 5:
      r = v;
      g = p;
      b = q;
      break;
    }
  }
  return _RGB(
           (int)(.5 + r * 255),
           (int)(.5 + g * 255),
           (int)(.5 + b * 255));
}
CC_INLINE COLOR SmartMixColors(COLOR color1, COLOR color2, double dblLumRatio, int k1, int k2)
{
  COLOR color;
  double h1, s1, v1;
  double h2, s2, v2;
  ASSERT(k1 >= 0);
  ASSERT(k2 >= 0);
  if (k1 + k2 == 0) {
    ASSERT(FALSE);
    return _RGB(0, 0, 0);
  }
  color = _RGB(
            (GetRV(color1) * k1 + GetRV(color2) * k2) / (k1 + k2),
            (GetGV(color1) * k1 + GetGV(color2) * k2) / (k1 + k2),
            (GetBV(color1) * k1 + GetBV(color2) * k2) / (k1 + k2));
  RGBtoHSV(color, &h1, &s1, &v1);
  RGBtoHSV(color2, &h2, &s2, &v2);
  v1 = v2;
  s1 = (s1 * k1 + s2 * k2) / (k1 + k2);
  color = HSVtoRGB(h1, s1, v1);
  if (dblLumRatio != 1.) {
    double H, S, L;
    RGBtoHSL(color, &H, &S, &L);
    color = HLStoRGB_ONE(H, MIN(1., L * dblLumRatio), S);
  }
  return color;
}
CC_INLINE COLOR MixColors(COLOR clr1, COLOR clr2, double dblRatio)
{
  ASSERT(dblRatio > 0.0f && dblRatio <= 1.0f);
  return _RGB(
           GetRV(clr1) + dblRatio * (GetRV(clr2) - GetRV(clr1)),
           GetGV(clr1) + dblRatio * (GetGV(clr2) - GetGV(clr1)),
           GetBV(clr1) + dblRatio * (GetBV(clr2) - GetBV(clr1))
         );
}
CC_INLINE int AdjustChannel(int nValue, double nPercent)
{
  int nNewValue = (int)(.5 + nPercent * nValue);
  if (nValue == 0 && nPercent > 1.) {
    nNewValue = (int)(.5 + (nPercent - 1.) * 255);
  }
  return MIN(nNewValue, 255);
}
CC_INLINE COLOR PixelAlpha4(COLOR srcPixel, double percentR, double percentG, double percentB)
{
  COLOR clrFinal = _RGB(AdjustChannel(GetRV(srcPixel), percentR), AdjustChannel(GetGV(srcPixel), percentG), AdjustChannel(GetBV(srcPixel), percentB));
  return (clrFinal);
}

CC_INLINE int dv_toint(int n, const double* a, int* b, int shi)
{
  int i;
  for (i = 0; i < 5; ++i) {
    b[i] = (int)(a[i] * shi);
  }
  return 0;
}

#if 0
#ifdef _WIN

#if(WINVER <= 0x0400)
#define COLOR_3DDKSHADOW        21
#define COLOR_3DLIGHT           22
#define COLOR_INFOTEXT          23
#define COLOR_INFOBK            24
#define COLOR_HOTLIGHT                  26
#define COLOR_GRADIENTACTIVECAPTION     27
#define COLOR_GRADIENTINACTIVECAPTION   28
#endif

int make_syscolor()
{
#define _GetSysColor(_COL)  SWAPRB(GetSysColor(_COL))
  const char* fname = "syscolordef.txt";
  FILE* pf = fopen(fname, "wb");
#define PRINTCOLOR(NAME)  fprintf(pf, "SYSCOLORDEF(%s, 0x%X)\r\n", #NAME , _GetSysColor(NAME));
#include "printcolor.txt"
#undef PRINTCOLOR
  fclose(pf);
  return 0;
}
#else
#endif
#endif


#if 0

typedef struct ColTable {
  const char* szName;
  COLOR colVal;
} ColTable;
static ColTable g_ColTable[] = {
#define COLORTABLEDEF(a, b)  {#a, b},
  COLORTABLEDEF_DEF(COLORTABLEDEF)
#undef COLORTABLEDEF
  {NULL, 0}
};
#endif


CC_INLINE int colorcn(ColorSpace type)
{
  static int inited = 0;
  static int _any_code_cn[ T_MaxColorSpace ];
  if (!inited) {
    inited = 1;
#define COLOR_TYPE_DEF(a, b, c) _any_code_cn[a] = b;
    COLOR_TYPE_DEF_DEF(COLOR_TYPE_DEF)
#undef COLOR_TYPE_DEF
  }
  return (_any_code_cn[BOUND(type, 0, T_MaxColorSpace - 1)]);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
// Fast cube root by Ken Turkowski (http://www.worldserver.com/turk/computergraphics/papers.html)
static float scbrt(float value)
{
  suf32_t fr;
  suf32_t fv;
  int s, ex, shx, ix;
  fv.f = value;
  ix = fv.i;
  s = ix & 0x80000000;
  ix &= 0x7fffffff;
  ex = (ix >> 23) - 127;
  shx = ex % 3;
  shx -= shx >= 0 ? 3 : 0;
  ex = (ex - shx) / 3;   /* exponent of cube root */
  fr.i = (ix & ((1 << 23) - 1)) | ((shx + 127) << 23);
  // 0.125 <= fr < 1.0
  /* Use quartic rational polynomial with error < 2^(-24) */
  fr.f = (float)(((((45.2548339756803022511987494 * fr.f +
                     192.2798368355061050458134625) * fr.f +
                    119.1654824285581628956914143) * fr.f +
                   13.43250139086239872172837314) * fr.f +
                  0.1636161226585754240958355063) /
                 ((((14.80884093219134573786480845 * fr.f +
                     151.9714051044435648658557668) * fr.f +
                    168.5254414101568283957668343) * fr.f +
                   33.9905941350215598754191872) * fr.f +
                  1.0));
  // fr *= 2^ex * sign
  fr.i = (fr.i + (ex << 23) + s) & (fv.i * 2 != 0 ? -1 : 0);
  return fr.f;
}

// Various 3/4-channel to 3/4-channel RGB transformations
#define COLOR_FUN_ARGDEF       int height, int width, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int blue_idx
#define COLOR_FUN_ARGDEF_32F   int height, int width, const float* src, int srcstep, int srccn, float* dst, int dststep, int dstcn, int blue_idx
#define COLOR_FUN_ARG    height, width, src, srcstep, srccn, dst, dststep, dstcn, blue_idx
//typedef int (* color_cvt_f)(COLOR_FUN_ARGDEF);
typedef CStatus (* color_cvt_32f_f)(COLOR_FUN_ARGDEF_32F);

static CStatus icvBGRA2BGR_8u(COLOR_FUN_ARGDEF)
{
  int i;
  srcstep -= width * srccn;
  dststep -= width * dstcn;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
      uchar t0 = src[blue_idx], t1 = src[1], t2 = src[2 ^ blue_idx];
      dst[0] = t0;
      dst[1] = t1;
      dst[2] = t2;
    }
  }
  return CC_OK;
}

static CStatus icvBGRA2RGB_8u(COLOR_FUN_ARGDEF)
{
  int i;
  srcstep -= width * srccn;
  dststep -= width * dstcn;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
      uchar t0 = src[blue_idx], t1 = src[1], t2 = src[2 ^ blue_idx];
      dst[2] = t0;
      dst[1] = t1;
      dst[0] = t2;
    }
  }
  return CC_OK;
}

#define icvRGB2BGR_8u icvRGB2BGRA_8u
static CStatus icvRGB2BGRA_8u(COLOR_FUN_ARGDEF)
{
  int i;
  srcstep -= width * srccn;
  dststep -= width * dstcn;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
      uchar t0 = src[0], t1 = src[1], t2 = src[2];
      dst[2 ^ blue_idx] = t0;
      dst[1] = t1;
      dst[blue_idx] = t2;
    }
  }
  return CC_OK;
}

static CStatus icvBGR2BGRA_8u(COLOR_FUN_ARGDEF)
{
  int i;
  srcstep -= width * srccn;
  dststep -= width * dstcn;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; ++i, src += srccn, dst += dstcn) {
      uchar t0 = src[0], t1 = src[1], t2 = src[2];
      dst[0] = t0;
      dst[1] = t1;
      dst[2] = t2;
      dst[3] = 0;
    }
  }
  return CC_OK;
}

static CStatus icvBGRA2RGBA_8u_C4R_(COLOR_FUN_ARGDEF)
{
  int i;
  width *= 4;
  for (; height--; src += srcstep, dst += dststep) {
    const uchar* s = (src);
    uchar* d = (dst);
    for (i = 0; i < width; s += srccn, d += dstcn) {
      uchar t0 = s[2], t1 = s[1], t2 = s[0], t3 = s[3];
      d[0] = t0;
      d[1] = t1;
      d[2] = t2;
      d[3] = t3;
    }
  }
  return CC_OK;
}


/****************************************************************************************\
*           Transforming 16-bit (565 or 555) RGB to/from 24/32-bit (888[8]) RGB          *
\****************************************************************************************/

static CStatus icvBGR5652BGRA_8u(COLOR_FUN_ARGDEF)
{
  int i;
  dststep -= width * dstcn;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i++, dst += dstcn) {
      unsigned t = ((const ushort*) src) [ i ];
      dst[ 0 ] = (uchar)(t << 3);
      dst[ 1 ] = (uchar)((t >> 3) & ~3);
      dst[ 2 ] = (uchar)((t >> 8) & ~7);
      // dst[ 3 ] = 0;
    }
  }
  return CC_OK;
}

static CStatus icvBGR5552BGRA_8u(COLOR_FUN_ARGDEF)
{
  int i;
  dststep -= width * dstcn;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i++, dst += dstcn) {
      unsigned t = ((const ushort*) src) [ i ];
      dst[ 0 ] = (uchar)(t << 3);
      dst[ 1 ] = (uchar)((t >> 2) & ~7);
      dst[ 2 ] = (uchar)((t >> 7) & ~7);
      // dst[ 3 ] = 0;
    }
  }
  return CC_OK;
}


static CStatus icvBGRA2BGR565_8u(COLOR_FUN_ARGDEF)
{
  int i;
  srcstep -= width * srccn;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i++, src += srccn) {
      int t = (src[ 0 ] >> 3) | ((src[ 1 ] & ~3) << 3) | ((src[ 2 ] & ~7) << 8);
      ((ushort*) dst) [ i ] = (ushort) t;
    }
  }
  return CC_OK;
}



static CStatus icvBGRA2BGR555_8u(COLOR_FUN_ARGDEF)
{
  int i;
  srcstep -= width * srccn;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i++, src += srccn) {
      int t = (src[ 0 ] >> 3) | ((src[ 1 ] & ~7) << 2) | ((src[ 2 ] & ~7) << 7);
      ((ushort*) dst) [ i ] = (ushort) t;
    }
  }
  return CC_OK;
}

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
static CStatus icvGRAY2BGRA_8u(COLOR_FUN_ARGDEF)
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

#define icvBGR2GRAY_8u  icvBGRA2GRAY_8u
#define icvRGB2GRAY_8u  icvBGRA2GRAY_8u
static CStatus icvBGRA2GRAY_8u(COLOR_FUN_ARGDEF)
{
  int i;
  srcstep -= width * srccn;
  if (width * height >= 1024) {
    int tab[ 256 * 3 ]; // = ( int* ) cvStackAlloc( 256 * 3 * sizeof( tab[ 0 ] ) );
    int r = 0, g = 0, b = (1 << (csc_shift - 1));
    for (i = 0; i < 256; i++) {
      tab[ i ] = b;
      tab[ i + 256 ] = g;
      tab[ i + 512 ] = r;
      g += cscGg;
      if (!blue_idx) {
        b += cscGb, r += cscGr;
      } else {
        b += cscGr, r += cscGb;
      }
    }
    for (; height--; src += srcstep, dst += dststep) {
      for (i = 0; i < width; i++, src += srccn) {
        int t0 = tab[ src[ 0 ] ] + tab[ src[ 1 ] + 256 ] + tab[ src[ 2 ] + 512 ];
        dst[ i ] = (uchar)(t0 >> csc_shift);
      }
    }
  } else {
    for (; height--; src += srcstep, dst += dststep) {
      for (i = 0; i < width; i++, src += srccn) {
        int t0 = src[ blue_idx ] * cscGb + src[ 1 ] * cscGg + src[ 2 ^ blue_idx ] * cscGr;
        dst[ i ] = (uchar) CC_DESCALE(t0, csc_shift);
      }
    }
  }
  return CC_OK;
}

static CStatus icvBGRA2GRAY_16u(COLOR_FUN_ARGDEF)
{
  int i;
  srcstep -= width * srccn;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i++, src += srccn) {
      dst[ i ] = (uchar) CC_DESCALE((unsigned)(src[ 0 ] * cscGb + src[ 1 ] * cscGg + src[ 2 ] * cscGr), csc_shift);
    }
  }
  return CC_OK;
}

/****************************************************************************************\
*                                     RGB <-> YCrCb                                      *
\****************************************************************************************/

/* BGR/RGB -> YCrCb */
#define yuvYr_32f cscGr_32f
#define yuvYg_32f cscGg_32f
#define yuvYb_32f cscGb_32f
#define yuvCr_32f 0.713f
#define yuvCb_32f 0.564f

#define yuv_shift 14
#define yuvYr  fix(yuvYr_32f,yuv_shift)
#define yuvYg  fix(yuvYg_32f,yuv_shift)
#define yuvYb  fix(yuvYb_32f,yuv_shift)
#define yuvCr  fix(yuvCr_32f,yuv_shift)
#define yuvCb  fix(yuvCb_32f,yuv_shift)

#define yuv_descale(x)  CC_DESCALE((x), yuv_shift)
#define yuv_prescale(x) ((x) << yuv_shift)

#define  yuvRCr_32f   1.403f
#define  yuvGCr_32f   (-0.714f)
#define  yuvGCb_32f   (-0.344f)
#define  yuvBCb_32f   1.773f

#define  yuvRCr   fix(yuvRCr_32f,yuv_shift)
#define  yuvGCr   (-fix(-yuvGCr_32f,yuv_shift))
#define  yuvGCb   (-fix(-yuvGCb_32f,yuv_shift))
#define  yuvBCb   fix(yuvBCb_32f,yuv_shift)

#define CC_IMPL_BGRA2YCrCb( flavor, arrtype, worktype, scale_macro, cast_macro,     \
    YUV_YB, YUV_YG, YUV_YR, YUV_CR, YUV_CB, YUV_Cx_BIAS )   \
static int                                             \
icvBGRA2YCrCb_##flavor(COLOR_FUN_ARGDEF)                           \
{                                                                     \
  int i;                                            \
  srcstep -= width*srccn;                                       \
  width *= 3;                                                    \
  for( ; height--; src += srcstep, dst += dststep )              \
  {                                                                   \
    for( i = 0; i < width; i += 3, src += srccn )               \
    {                                                                 \
      worktype b = src[0], r = src[2^0], y;             \
      y = scale_macro(b*YUV_YB + src[1]*YUV_YG + r*YUV_YR);           \
      r = scale_macro((r - y)*YUV_CR) + YUV_Cx_BIAS;                  \
      b = scale_macro((b - y)*YUV_CB) + YUV_Cx_BIAS;                  \
      dst[i] = cast_macro(y);                                         \
      dst[i+1] = cast_macro(r);                                       \
      dst[i+2] = cast_macro(b);                                       \
    }                                                                 \
  }                                                                   \
  return CC_OK;                                                       \
}


CC_IMPL_BGRA2YCrCb(8u, uchar, int, yuv_descale, CC_CAST_8U, yuvYb, yuvYg, yuvYr, yuvCr, yuvCb, 128)
//CC_IMPL_BGRA2YCrCb(16u, ushort, int, yuv_descale, CC_CAST_16U, yuvYb, yuvYg, yuvYr, yuvCr, yuvCb, 32768)
//CC_IMPL_BGRA2YCrCb(32f, float, float, CC_NOP, CC_NOP, yuvYb_32f, yuvYg_32f, yuvYr_32f, yuvCr_32f, yuvCb_32f, 0.5f)


#define CC_IMPL_YCrCb2BGRA( flavor, arrtype, worktype, prescale_macro,      \
    scale_macro, cast_macro, YUV_BCb, YUV_GCr, YUV_GCb, YUV_RCr, YUV_Cx_BIAS)\
static CStatus icvYCrCb2BGRA_##flavor(COLOR_FUN_ARGDEF)                           \
{                                                                     \
  int i;                                             \
  dststep -= width*dstcn;                                       \
  width *= 3;                                                    \
  for( ; height--; src += srcstep, dst += dststep )              \
  {                                                                   \
    for( i = 0; i < width; i += 3, dst += dstcn )               \
    {                                                                 \
      worktype Y = prescale_macro(src[i]),                            \
          Cr = src[i+1] - YUV_Cx_BIAS,                       \
              Cb = src[i+2] - YUV_Cx_BIAS;                  \
      worktype b, g, r;                                               \
      b = scale_macro( Y + YUV_BCb*Cb );                              \
      g = scale_macro( Y + YUV_GCr*Cr + YUV_GCb*Cb );                 \
      r = scale_macro( Y + YUV_RCr*Cr );                              \
      dst[0] = cast_macro(b);                                  \
      dst[1] = cast_macro(g);                                         \
      dst[0^2] = cast_macro(r);                                \
      if( dstcn == 4 )                                               \
        dst[3] = 0;                                                   \
    }                                                                 \
  }                                                                   \
  return CC_OK;                                                       \
}

#define icvBGRA2YCrYCb_8u (0)

CC_IMPL_YCrCb2BGRA(8u, uchar, int, yuv_prescale, yuv_descale, CC_CAST_8U, yuvBCb, yuvGCr, yuvGCb, yuvRCr, 128)
//CC_IMPL_YCrCb2BGRA(16u, ushort, int, yuv_prescale, yuv_descale, CC_CAST_16U, yuvBCb, yuvGCr, yuvGCb, yuvRCr, 32768)
//CC_IMPL_YCrCb2BGRA(32f, float, float, CC_NOP, CC_NOP, CC_NOP, yuvBCb_32f, yuvGCr_32f, yuvGCb_32f, yuvRCr_32f, 0.5f)


#define CC_IMPL_YCrYCb2BGRA( flavor, arrtype, worktype, prescale_macro,      \
    scale_macro, cast_macro, YUV_BCb, YUV_GCr, YUV_GCb, YUV_RCr, YUV_Cx_BIAS)\
static CStatus icvYCrYCb2BGRA_##flavor(COLOR_FUN_ARGDEF)                           \
{                                                                     \
  int i; worktype s[4];                                             \
  dststep -= width*dstcn;                                       \
  width *= 3;                                                    \
  for( ; height--; src += srcstep, dst += dststep )              \
  {                                                                   \
    for( i = 0; i < width; i += 2, dst += dstcn )               \
    {                                                                 \
      worktype b, g, r, Y, Cb, Cr;                                               \
      Y = (src[i]);                            \
      s[1+(i^1)] = src[i+1] - YUV_Cx_BIAS;                       \
      Y=s[0], Cb=s[1], Cr=s[2];                                               \
      b = ( Y + YUV_BCb*Cb );                              \
      g = ( Y + YUV_GCr*Cr + YUV_GCb*Cb );                 \
      r = ( Y + YUV_RCr*Cr );                              \
      dst[0] = (b);                                  \
      dst[1] = (g);                                         \
      dst[0^2] = (r);                                \
      if( dstcn == 4 )                                               \
        dst[3] = 0;                                                   \
    }                                                                 \
  }                                                                   \
  return CC_OK;                                                       \
}

CC_IMPL_YCrYCb2BGRA(8u, uchar, int, yuv_prescale, yuv_descale, CC_CAST_8U,
                    yuvBCb, yuvGCr, yuvGCb, yuvRCr, 128)


/****************************************************************************************\
*                                      RGB <-> XYZ                                       *
\****************************************************************************************/

#define xyzXr_32f  0.412453f
#define xyzXg_32f  0.357580f
#define xyzXb_32f  0.180423f

#define xyzYr_32f  0.212671f
#define xyzYg_32f  0.715160f
#define xyzYb_32f  0.072169f

#define xyzZr_32f  0.019334f
#define xyzZg_32f  0.119193f
#define xyzZb_32f  0.950227f

#define xyzRx_32f  3.240479f
#define xyzRy_32f  (-1.53715f)
#define xyzRz_32f  (-0.498535f)

#define xyzGx_32f  (-0.969256f)
#define xyzGy_32f  1.875991f
#define xyzGz_32f  0.041556f

#define xyzBx_32f  0.055648f
#define xyzBy_32f  (-0.204043f)
#define xyzBz_32f  1.057311f

#define xyz_shift  10
#define xyzXr_32s  fix(xyzXr_32f, xyz_shift )
#define xyzXg_32s  fix(xyzXg_32f, xyz_shift )
#define xyzXb_32s  fix(xyzXb_32f, xyz_shift )

#define xyzYr_32s  fix(xyzYr_32f, xyz_shift )
#define xyzYg_32s  fix(xyzYg_32f, xyz_shift )
#define xyzYb_32s  fix(xyzYb_32f, xyz_shift )

#define xyzZr_32s  fix(xyzZr_32f, xyz_shift )
#define xyzZg_32s  fix(xyzZg_32f, xyz_shift )
#define xyzZb_32s  fix(xyzZb_32f, xyz_shift )

#define xyzRx_32s  fix(3.240479f, xyz_shift )
#define xyzRy_32s  -fix(1.53715f, xyz_shift )
#define xyzRz_32s  -fix(0.498535f, xyz_shift )

#define xyzGx_32s  -fix(0.969256f, xyz_shift )
#define xyzGy_32s  fix(1.875991f, xyz_shift )
#define xyzGz_32s  fix(0.041556f, xyz_shift )

#define xyzBx_32s  fix(0.055648f, xyz_shift )
#define xyzBy_32s  -fix(0.204043f, xyz_shift )
#define xyzBz_32s  fix(1.057311f, xyz_shift )

#define xyz_descale(x) CC_DESCALE((x),xyz_shift)

#define CC_IMPL_BGRA2XYZ( flavor, arrtype, worktype, scale_macro, cast_macro, suffix ) \
  static CStatus icvBGRA2XYZ_##flavor(COLOR_FUN_ARGDEF)                                   \
  {                                                                           \
    int i;                                                   \
    worktype t, _m[] =                                                        \
        {                                                                         \
            xyzXb##suffix, xyzXg##suffix, xyzXr##suffix,                          \
            xyzYb##suffix, xyzYg##suffix, xyzYr##suffix,                          \
            xyzZb##suffix, xyzZg##suffix, xyzZr##suffix                           \
        };                                                                        \
    srcstep -= width*srccn;                                             \
    width *= 3;                                                          \
    if( 0 )                                                            \
    {                                                                         \
      CC_SWAP( _m[0], _m[2], t );                                             \
      CC_SWAP( _m[3], _m[5], t );                                             \
      CC_SWAP( _m[6], _m[8], t );                                             \
    }                                                                         \
    for( ; height--; src += srcstep, dst += dststep )                         \
    {                                                                         \
      for( i = 0; i < width; i += 3, src += srccn )                           \
      {                                                                       \
        worktype x = scale_macro(src[0]*_m[0] + src[1]*_m[1] + src[2]*_m[2]); \
        worktype y = scale_macro(src[0]*_m[3] + src[1]*_m[4] + src[2]*_m[5]); \
        worktype z = scale_macro(src[0]*_m[6] + src[1]*_m[7] + src[2]*_m[8]); \
        dst[i] = (arrtype)(x);                                                \
        dst[i+1] = (arrtype)(y);                                              \
        dst[i+2] = cast_macro(z); /*sum of weights for z > 1*/                \
      }                                                                       \
    }                                                                         \
    return CC_OK;                                                             \
  }


CC_IMPL_BGRA2XYZ(8u, uchar, int, xyz_descale, CC_CAST_8U, _32s)
//CC_IMPL_BGRA2XYZ(16u, ushort, int, xyz_descale, CC_CAST_16U, _32s)
//CC_IMPL_BGRA2XYZ(32f, float, float, CC_NOP, CC_NOP, _32f)


#define CC_IMPL_XYZ2BGRA( flavor, arrtype, worktype, scale_macro, cast_macro, suffix )                              \
  static int                                                   \
  icvXYZ2BGRA_##flavor(COLOR_FUN_ARGDEF)                                   \
  {                                                                           \
    int i;                                                   \
    worktype t, _m[] =                                                        \
        {                                                                     \
            xyzBx##suffix, xyzBy##suffix, xyzBz##suffix,                      \
            xyzGx##suffix, xyzGy##suffix, xyzGz##suffix,                      \
            xyzRx##suffix, xyzRy##suffix, xyzRz##suffix                       \
        };                                                                    \
    dststep -= width*dstcn;                                             \
    width *= 3;                                                          \
    if( 0 )                                                            \
    {                                                                         \
      CC_SWAP( _m[0], _m[6], t );                                             \
      CC_SWAP( _m[1], _m[7], t );                                             \
      CC_SWAP( _m[2], _m[8], t );                                             \
    }                                                                         \
    for( ; height--; src += srcstep, dst += dststep )                    \
    {                                                                         \
      for( i = 0; i < width; i += 3, dst += dstcn )                     \
      {                                                                       \
        worktype b = scale_macro(src[i]*_m[0] + src[i+1]*_m[1] + src[i+2]*_m[2]); \
        worktype g = scale_macro(src[i]*_m[3] + src[i+1]*_m[4] + src[i+2]*_m[5]); \
        worktype r = scale_macro(src[i]*_m[6] + src[i+1]*_m[7] + src[i+2]*_m[8]); \
        dst[0] = cast_macro(b);                                               \
        dst[1] = cast_macro(g);                                               \
        dst[2] = cast_macro(r);                                               \
        if( dstcn == 4 )                                                     \
          dst[3] = 0;                                                         \
      }                                                                       \
    }                                                                         \
    return CC_OK;                                                             \
  }

CC_IMPL_XYZ2BGRA(8u, uchar, int, xyz_descale, CC_CAST_8U, _32s)
//CC_IMPL_XYZ2BGRA(16u, ushort, int, xyz_descale, CC_CAST_16U, _32s)
//CC_IMPL_XYZ2BGRA(32f, float, float, CC_NOP, CC_NOP, _32f)


/****************************************************************************************\
*                          Non-linear Color Space Transformations                        *
\****************************************************************************************/

// driver color space conversion function for 8u arrays that uses 32f function
// with appropriate pre- and post-scaling.
static CStatus icvABC2BGRA_8u(COLOR_FUN_ARGDEF, color_cvt_32f_f cvtfunc_32f, const float* pre_coeffs, int postscale)
{
  int block_size = MIN(1 << 8, width);
  float buffer[(1 << 8) * 3 ];    // = ( float* ) cvStackAlloc( block_size * 3 * sizeof( buffer[ 0 ] ) );
  int i, di, k;
  CStatus status = CC_OK;
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
      status = cvtfunc_32f(1, di, buffer, srcstep, srccn, buffer, dststep, dstcn, blue_idx);
      if (status < 0) {
        return status;
      }
      if (postscale) {
        for (k = 0; k < di * 3; k += 3, dst += dstcn) {
          int b = ROUND(buffer[ k ] * 255.);
          int g = ROUND(buffer[ k + 1 ] * 255.);
          int r = ROUND(buffer[ k + 2 ] * 255.);
          dst[ 0 ] = CC_CAST_8U(b);
          dst[ 1 ] = CC_CAST_8U(g);
          dst[ 2 ] = CC_CAST_8U(r);
          if (dstcn == 4) {
            dst[ 3 ] = 0;
          }
        }
      } else {
        for (k = 0; k < di * 3; k += 3, dst += dstcn) {
          int b = ROUND(buffer[ k ]);
          int g = ROUND(buffer[ k + 1 ]);
          int r = ROUND(buffer[ k + 2 ]);
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
static CStatus icvBGRA2ABC_8u(COLOR_FUN_ARGDEF, color_cvt_32f_f cvtfunc_32f, int prescale, const float* post_coeffs)
{
  int block_size = MIN(1 << 8, width);
  float buffer[(1 << 8) * 3 ];    // = ( float* ) cvStackAlloc( block_size * 3 * sizeof( buffer[ 0 ] ) );
  int i, di, k;
  CStatus status = CC_OK;
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
      } else {
        for (k = 0; k < di * 3; k += 3, src += srccn) {
          float b = CC_8TO32F(src[ 0 ]);
          float g = CC_8TO32F(src[ 1 ]);
          float r = CC_8TO32F(src[ 2 ]);
          buffer[ k ] = b;
          buffer[ k + 1 ] = g;
          buffer[ k + 2 ] = r;
        }
      }
      status = cvtfunc_32f(1, di, buffer, srcstep, srccn, buffer, dststep, dstcn, blue_idx);
      if (status < 0) {
        return status;
      }
      for (k = 0; k < di * 3; k += 3) {
        int a = ROUND(buffer[ k ] * post_coeffs[ 0 ] + post_coeffs[ 1 ]);
        int b = ROUND(buffer[ k + 1 ] * post_coeffs[ 2 ] + post_coeffs[ 3 ]);
        int c = ROUND(buffer[ k + 2 ] * post_coeffs[ 4 ] + post_coeffs[ 5 ]);
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

static const uchar icHue255To180[] = { //
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


static const uchar icHue180To255[] = { //
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


static CStatus icvBGRA2HSV_8u(COLOR_FUN_ARGDEF)
{
  int i;
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
      int b = (src) [ 0 ], g = (src) [ 1 ], r = (src) [ 2 ^ 0 ];
      int h, s, v = b;
      int vmin = b, diff;
      int vr, vg;
      CC_CALC_MAX_8U(v, g);
      CC_CALC_MAX_8U(v, r);
      CC_CALC_MIN_8U(vmin, g);
      CC_CALC_MIN_8U(vmin, r);
      diff = v - vmin;
      vr = v == r ? -1 : 0;
      vg = v == g ? -1 : 0;
      s = diff * div_table[ v ] >> hsv_shift;
      h = (vr & (g - b)) +
          (~vr & ((vg & (b - r + 2 * diff)) + ((~vg) & (r - g + 4 * diff))));
      h = ((h * div_table[ diff ] * 15 + (1 << (hsv_shift + 6))) >> (7 + hsv_shift)) \
          + (h < 0 ? 30 * 6 : 0);
      dst[ i ] = (uchar) h;
      dst[ i + 1 ] = (uchar) s;
      dst[ i + 2 ] = (uchar) v;
    }
  }
  return CC_OK;
}


static CStatus icvBGRA2HSV_32f(COLOR_FUN_ARGDEF_32F)
{
  int i;
  float* dstf = (float*)(dst);
  dststep /= dstcn;
  srcstep -= width * srccn;
  width *= 3;
  for (; height--; src += srcstep, dst += dststep) {
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
      } else if (v == g) {
        h = (b - r) * diff + 120.f;
      } else {
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
  return CC_OK;
}

static CStatus icvHSV2BGRA_32f(COLOR_FUN_ARGDEF_32F)
{
  int i;
  dststep -= width * dstcn;
  width *= 3;
  for (; height--; src += srcstep, dst += dststep) {
    for (i = 0; i < width; i += 3, dst += dstcn) {
      float h = src[ i ], s = src[ i + 1 ], v = src[ i + 2 ];
      float b, g, r;
      if (s == 0) {
        b = g = r = v;
      } else {
        static const int sector_data[][ 3 ] = { {1, 3, 0}, {1, 0, 2}, {3, 0, 1}, {0, 2, 1}, {0, 1, 3}, {2, 1, 0} };
        float tab[ 4 ];
        int sector;
        h *= 0.016666666666666666f; // h /= 60;
        if (h < 0)
          do {
            h += 6;
          } while (h < 0);
        else if (h >= 6)
          do {
            h -= 6;
          } while (h >= 6);
        sector = FLOOR(h);
        h -= sector;
        tab[ 0 ] = v;
        tab[ 1 ] = v * (1.f - s);
        tab[ 2 ] = v * (1.f - s * h);
        tab[ 3 ] = v * (1.f - s * (1.f - h));
        b = tab[ sector_data[ sector ][ 0 ] ];
        g = tab[ sector_data[ sector ][ 1 ] ];
        r = tab[ sector_data[ sector ][ 2 ] ];
      }
      dst[ 0 ] = b;
      dst[ 1 ] = g;
      dst[ 2 ] = r;
    }
  }
  return CC_OK;
}

static CStatus icvHSV2BGRA_8u(COLOR_FUN_ARGDEF)
{
  static const float pre_coeffs[] = {2.f, 0.f, 0.0039215686274509803f, 0.f, 1.f, 0.f };
  return icvABC2BGRA_8u(COLOR_FUN_ARG, icvHSV2BGRA_32f, pre_coeffs, 0);
}


/****************************************************************************************\
*                                     RGB <-> HLS                                        *
\****************************************************************************************/

static CStatus icvBGRA2HLS_32f(COLOR_FUN_ARGDEF_32F)
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
        } else if (vmax == g) {
          h = (b - r) * diff + 120.f;
        } else {
          h = (r - g) * diff + 240.f;
        }
        if (h < 0.f) {
          h += 360.f;
        }
      }
      dst[ i ] = h;
      dst[ i + 1 ] = l;
      dst[ i + 2 ] = s;
    }
  }
  return CC_OK;
}


static CStatus icvHLS2BGRA_32f(COLOR_FUN_ARGDEF_32F)
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
      } else {
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
          } while (h < 0);
        else if (h >= 6)
          do {
            h -= 6;
          } while (h >= 6);
        assert(0 <= h && h < 6);
        sector = FLOOR(h);
        h -= sector;
        tab[ 0 ] = p2;
        tab[ 1 ] = p1;
        tab[ 2 ] = p1 + (p2 - p1) * (1 - h);
        tab[ 3 ] = p1 + (p2 - p1) * h;
        b = tab[ sector_data[ sector ][ 0 ] ];
        g = tab[ sector_data[ sector ][ 1 ] ];
        r = tab[ sector_data[ sector ][ 2 ] ];
      }
      dst[ 0 ] = b;
      dst[ 1 ] = g;
      dst[ 2 ] = r;
      if (dstcn == 4) {
        dst[ 3 ] = 0;
      }
    }
  }
  return CC_OK;
}

static CStatus icvBGRA2HLS_8u(COLOR_FUN_ARGDEF)
{
  static const float post_coeffs[] = {
    0.5f, 0.f, 255.f, 0.f, 255.f, 0.f
  };
  return icvBGRA2ABC_8u(COLOR_FUN_ARG, icvBGRA2HLS_32f, 1, post_coeffs);
}


static CStatus icvHLS2BGRA_8u(COLOR_FUN_ARGDEF)
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

#define  labXr_32f  0.433953f /* = xyzXr_32f / 0.950456 */
#define  labXg_32f  0.376219f /* = xyzXg_32f / 0.950456 */
#define  labXb_32f  0.189828f /* = xyzXb_32f / 0.950456 */

#define  labYr_32f  0.212671f /* = xyzYr_32f */
#define  labYg_32f  0.715160f /* = xyzYg_32f */
#define  labYb_32f  0.072169f /* = xyzYb_32f */

#define  labZr_32f  0.017758f /* = xyzZr_32f / 1.088754 */
#define  labZg_32f  0.109477f /* = xyzZg_32f / 1.088754 */
#define  labZb_32f  0.872766f /* = xyzZb_32f / 1.088754 */

#define  labRx_32f  3.0799327f  /* = xyzRx_32f * 0.950456 */
#define  labRy_32f  (-1.53715f) /* = xyzRy_32f */
#define  labRz_32f  (-0.542782f)/* = xyzRz_32f * 1.088754 */

#define  labGx_32f  (-0.921235f)/* = xyzGx_32f * 0.950456 */
#define  labGy_32f  1.875991f   /* = xyzGy_32f */
#define  labGz_32f  0.04524426f /* = xyzGz_32f * 1.088754 */

#define  labBx_32f  0.0528909755f /* = xyzBx_32f * 0.950456 */
#define  labBy_32f  (-0.204043f)  /* = xyzBy_32f */
#define  labBz_32f  1.15115158f   /* = xyzBz_32f * 1.088754 */

#define  labT_32f   0.008856f

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

#define labSmallScale_32f  7.787f
#define labSmallShift_32f  0.13793103448275862f  /* 16/116 */
#define labLScale_32f      116.f
#define labLShift_32f      16.f
#define labLScale2_32f     903.3f

#define labSmallScale fix(31.27 /* labSmallScale_32f*(1<<lab_shift)/255 */,lab_shift)
#define labSmallShift fix(141.24138 /* labSmallScale_32f*(1<<lab) */,lab_shift)
#define labLScale fix(295.8 /* labLScale_32f*255/100 */,lab_shift)
#define labLShift fix(41779.2 /* labLShift_32f*1024*255/100 */,lab_shift)
#define labLScale2 fix(labLScale2_32f*0.01,lab_shift)

/* 1024*(([0..511]./255)**(1./3)) */
static ushort icLabCubeRootTab[] = { //
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

static CStatus icvBGRA2Lab_8u(COLOR_FUN_ARGDEF)
{
  int i;
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
        assert((unsigned) x < 512), x = icLabCubeRootTab[ x ];
      } else {
        x = CC_DESCALE(x * labSmallScale + labSmallShift, lab_shift);
      }
      f = z > labT;
      z = CC_DESCALE(z, lab_shift);
      if (f) {
        assert((unsigned) z < 512), z = icLabCubeRootTab[ z ];
      } else {
        z = CC_DESCALE(z * labSmallScale + labSmallShift, lab_shift);
      }
      f = y > labT;
      y = CC_DESCALE(y, lab_shift);
      if (f) {
        assert((unsigned) y < 512), y = icLabCubeRootTab[ y ];
        L = CC_DESCALE(y * labLScale - labLShift, 2 * lab_shift);
      } else {
        L = CC_DESCALE(y * labLScale2, lab_shift);
        y = CC_DESCALE(y * labSmallScale + labSmallShift, lab_shift);
      }
      a = CC_DESCALE(500 * (x - y), lab_shift) + 128;
      b = CC_DESCALE(200 * (y - z), lab_shift) + 128;
      dst[ i ] = CC_CAST_8U(L);
      dst[ i + 1 ] = CC_CAST_8U(a);
      dst[ i + 2 ] = CC_CAST_8U(b);
    }
  }
  return CC_OK;
}

static CStatus icvBGRA2Lab_32f(COLOR_FUN_ARGDEF_32F)
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
        x = scbrt(x);
      } else {
        x = x * labSmallScale_32f + labSmallShift_32f;
      }
      if (z > labT_32f) {
        z = scbrt(z);
      } else {
        z = z * labSmallScale_32f + labSmallShift_32f;
      }
      if (y > labT_32f) {
        y = scbrt(y);
        L = y * labLScale_32f - labLShift_32f;
      } else {
        L = y * labLScale2_32f;
        y = y * labSmallScale_32f + labSmallShift_32f;
      }
      a = 500.f * (x - y);
      b = 200.f * (y - z);
      dst[ i ] = L;
      dst[ i + 1 ] = a;
      dst[ i + 2 ] = b;
    }
  }
  return CC_OK;
}


static CStatus icvLab2BGRA_32f(COLOR_FUN_ARGDEF_32F)
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
      dst[ 0 ] = b;
      dst[ 1 ] = g;
      dst[ 2 ] = r;
      if (dstcn == 4) {
        dst[ 3 ] = 0;
      }
    }
  }
  return CC_OK;
}

static CStatus icvLab2BGRA_8u(COLOR_FUN_ARGDEF)
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

#if 0
static CStatus icvBGRA2Luv_32f(COLOR_FUN_ARGDEF_32F)
{
#define cast_get_bgr_macro(dst, _B, _G, _R)    (_B = src[0], _G = src[1], _R = src[2])
#define cast_set_luv_macro(dst, _L, _U, _V)    (dst[ 0 ] = _L, dst[ 1 ] = _U, dst[ 2 ] = _V)
#include "impl/icvBGRA2Luv_32f.inl"
  return CC_OK;
}


static CStatus icvLuv2BGRA_32f(COLOR_FUN_ARGDEF_32F)
{
#define cast_set_bgr_macro(dst, _B, _G, _R)    (dst[0]=_B, dst[1]=_G, dst[2]=_R)
#define cast_get_luv_macro(src, _L, _U, _V)    (_L = src[0], _U = src[1], _V = src[2])
#include "impl/icvLuv2BGRA_32f.inl"
  return CC_OK;
}

static CStatus icvBGRA2Luv_8u(COLOR_FUN_ARGDEF)
{
  // L: [   0..100] -> [0..255]
  // u: [-134..220] -> [0..255]
  // v: [-140..122] -> [0..255]
  //static const float post_coeffs[] = { 2.55f, 0.f, 1.f, 83.f, 1.f, 140.f };
  static const float post_coeffs[] = { 2.55f, 0.f, 0.72033898305084743f, 96.525423728813564f, 0.99609375f, 139.453125f};
  return icvBGRA2ABC_8u(COLOR_FUN_ARG, icvBGRA2Luv_32f, 1, post_coeffs);
}
static CStatus icvLuv2BGRA_8u(COLOR_FUN_ARGDEF)
{
  // L: [0..255] -> [0..100]
  // u: [0..255] -> [-134..220]
  // v: [0..255] -> [-140..122]
  static const float pre_coeffs[] = { 0.39215686274509809f, 0.f, 1.388235294117647f, -134.f, 1.003921568627451f, -140.f };
  return icvABC2BGRA_8u(COLOR_FUN_ARG, icvLuv2BGRA_32f, pre_coeffs, 1);
}

static CStatus icvBGRA2Luv_8u(COLOR_FUN_ARGDEF)
{
  // L: [   0..100] -> [0..255]
  // u: [-134..220] -> [0..255]
  // v: [-140..122] -> [0..255]
  //static const float post_coeffs[] = { 2.55f, 0.f, 1.f, 83.f, 1.f, 140.f };
#define cast_get_bgr_macro(dst, _B, _G, _R)    (_B = src[0]*0.0039215686274509803f, _G = src[1]*0.0039215686274509803f, _R = src[2]*0.0039215686274509803f)
#define cast_set_luv_macro(dst, _L, _U, _V)    { int _l=ROUND(_L*2.55f), _u=ROUND(_U*0.72033898305084743f+96.525423728813564f), _v=ROUND(_V*0.99609375f+139.453125f);  dst[ 0 ] = CC_CAST_8U(_l), dst[ 1 ] = CC_CAST_8U(_u), dst[ 2 ] = CC_CAST_8U(_v); }
#include "impl/icvBGRA2Luv_32f.inl"
  return CC_OK;
}

static CStatus icvLuv2BGRA_8u(COLOR_FUN_ARGDEF)
{
  // L: [0..255] -> [   0..100]
  // u: [0..255] -> [-134..220]
  // v: [0..255] -> [-140..122]
  //static const float pre_coeffs[] = { 0.39215686274509809f, 0.f, 1.388235294117647f, -134.f, 1.003921568627451f, -140.f };
#define cast_set_bgr_macro(dst, _B, _G, _R)    { int _b=ROUND(_B*255), _g=ROUND(_G*255), _r=ROUND(_R*255); dst[0]=CC_CAST_8U(_b), dst[1]=CC_CAST_8U(_g), dst[2]=CC_CAST_8U(_r);}
#define cast_get_luv_macro(src, _L, _U, _V)    (_L = src[0]*0.39215686274509809f, _U = src[1]*1.388235294117647f-134.f, _V = src[2]*1.003921568627451f-140.f)
#include "impl/icvLuv2BGRA_32f.inl"
  return CC_OK;
}
#endif
/****************************************************************************************\
*                            Bayer Pattern -> RGB conversion                             *
\****************************************************************************************/

static CStatus icvBayer2BGRA_8u_impl(COLOR_FUN_ARGDEF, int src_type)
{
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
    } else {
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
  return CC_OK;
}

static CStatus icvBayerGR2BGRA_8u(COLOR_FUN_ARGDEF)
{
  return icvBayer2BGRA_8u_impl(COLOR_FUN_ARG, T_BayerGR);
}

static CStatus icvBayerGB2BGRA_8u(COLOR_FUN_ARGDEF)
{
  return icvBayer2BGRA_8u_impl(COLOR_FUN_ARG, T_BayerGB);
}

static CStatus icvBayerBG2BGRA_8u(COLOR_FUN_ARGDEF)
{
  return icvBayer2BGRA_8u_impl(COLOR_FUN_ARG, T_BayerBG);
}

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
//

/* LIMIT: convert a 16.16 fixed-point value to a byte, with clipping. */
#define LIMIT(x) ((x)>0xffffff?0xff: ((x)<=0xffff?0:((x)>>16)))

#ifndef CC_INLINE
#define CC_INLINE static
#endif // CC_INLINE

CC_INLINE void move_420_block(int yTL, int yTR, int yBL, int yBR, int u, int v, int dststep, int dstcn, int blue_idx, unsigned char* rgb)
{
  const int rvScale = 91881;
  const int guScale = -22553;
  const int gvScale = -46801;
  const int buScale = 116129;
  const int yScale = 65536;
  int r, g, b;
  unsigned char* rgb0 = rgb;
  unsigned char* rgb1 = rgb + dstcn;
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
  rgb0[ blue_idx ] = LIMIT(b + yTL);
  rgb0[ 1 ] = LIMIT(g + yTL);
  rgb0[ 2 ^ blue_idx ] = LIMIT(r + yTL);
  rgb1[ blue_idx ] = LIMIT(b + yTR);
  rgb1[ 1 ] = LIMIT(g + yTR);
  rgb1[ 2 ^ blue_idx ] = LIMIT(r + yTR);
  /* Skip down to next line to write out bottom two pixels */
  rgb0 += dststep;
  rgb0[ blue_idx ] = LIMIT(b + yBL);
  rgb0[ 1 ] = LIMIT(g + yBL);
  rgb0[ 2 ^ blue_idx ] = LIMIT(r + yBL);
  rgb1[ blue_idx ] = LIMIT(b + yBR);
  rgb1[ 1 ] = LIMIT(g + yBR);
  rgb1[ 2 ^ blue_idx ] = LIMIT(r + yBR);
}

CC_INLINE void move_411_block(int yTL, int yTR, int yBL, int yBR, int u, int v, int dststep, int dstcn, int blue_idx, unsigned char* rgb)
{
  const int rvScale = 91881;
  const int guScale = -22553;
  const int gvScale = -46801;
  const int buScale = 116129;
  const int yScale = 65536;
  int r, g, b;
  unsigned char* rgb0 = rgb;
  unsigned char* rgb1 = rgb + dstcn;
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
  rgb0[ blue_idx ] = LIMIT(b + yTL);
  rgb0[ 1 ] = LIMIT(g + yTL);
  rgb0[ 2 ^ blue_idx ] = LIMIT(r + yTL);
  rgb1[ blue_idx ] = LIMIT(b + yTR);
  rgb1[ 1 ] = LIMIT(g + yTR);
  rgb1[ 2 ^ blue_idx ] = LIMIT(r + yTR);
  /* Write out top two last pixels */
  rgb += 6;
  rgb0[ blue_idx ] = LIMIT(b + yBL);
  rgb0[ 1 ] = LIMIT(g + yBL);
  rgb0[ 2 ^ blue_idx ] = LIMIT(r + yBL);
  rgb1[ blue_idx ] = LIMIT(b + yBR);
  rgb1[ 1 ] = LIMIT(g + yBR);
  rgb1[ 2 ^ blue_idx ] = LIMIT(r + yBR);
}

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
/* Converts from planar YUV420P to RGB24. */
CC_INLINE int icvYUV420P2BGRA_8u_(COLOR_FUN_ARGDEF)
{
  const int numpix = width * height;
  int i, j, y00, y01, y10, y11, u, v;
  const unsigned char* pY = src;
  const unsigned char* pU = pY + numpix;
  const unsigned char* pV = pU + numpix / 4;
  unsigned char* pOut = dst;
  for (j = 0; j <= height - 2; j += 2) {
    for (i = 0; i <= width - 2; i += 2) {
      y00 = *pY;
      y01 = *(pY + 1);
      y10 = *(pY + width);
      y11 = *(pY + width + 1);
      u = (*pU++) - 128;
      v = (*pV++) - 128;
      move_420_block(y00, y01, y10, y11, u, v, dststep, dstcn, blue_idx, pOut);
      pY += 2;
      pOut += 2 * dstcn;
    }
    pY += width;
    pOut += dststep;
  }
  return CC_OK;
}

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
CC_INLINE int icvYUV4202BGRA_8u(COLOR_FUN_ARGDEF)
{
  const int bytes = 24 >> 3;
  int i, j, y00, y01, y10, y11, u, v;
  const unsigned char* pY = src;
  const unsigned char* pU = pY + 4;
  const unsigned char* pV = pU + width;
  unsigned char* pOut = dst;
  for (j = 0; j <= height - 2; j += 2) {
    for (i = 0; i <= width - 4; i += 4) {
      y00 = *pY;
      y01 = *(pY + 1);
      y10 = *(pY + width);
      y11 = *(pY + width + 1);
      u = (*pU++) - 128;
      v = (*pV++) - 128;
      move_420_block(y00, y01, y10, y11, u, v, dststep, dstcn, blue_idx, pOut);
      pY += 2;
      pOut += 2 * bytes;
      y00 = *pY;
      y01 = *(pY + 1);
      y10 = *(pY + width);
      y11 = *(pY + width + 1);
      u = (*pU++) - 128;
      v = (*pV++) - 128;
      move_420_block(y00, y01, y10, y11, u, v, dststep, dstcn, blue_idx, pOut);
      pY += 4; // skip UV
      pOut += 2 * bytes;
    }
    pY += width;
    pOut += width * bytes;
  }
  return CC_OK;
}

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
CC_INLINE int icvYUV411P2BGRA_8u(COLOR_FUN_ARGDEF)
{
  const int numpix = width * height;
  const int bytes = 24 >> 3;
  int i, j, y00, y01, y10, y11, u, v;
  const unsigned char* pY = src;
  const unsigned char* pU = pY + numpix;
  const unsigned char* pV = pU + numpix / 4;
  unsigned char* pOut = dst;
  for (j = 0; j < height; j++) {
    for (i = 0; i < width - 4; i += 4) {
      y00 = *pY;
      y01 = *(pY + 1);
      y10 = *(pY + 2);
      y11 = *(pY + 3);
      u = (*pU++) - 128;
      v = (*pV++) - 128;
      move_411_block(y00, y01, y10, y11, u, v, dststep, dstcn, blue_idx, pOut);
      pY += 4;
      pOut += 4 * bytes;
    }
  }
  return CC_OK;
}

/* convert from 4:2:2 YUYV interlaced to RGB24 */
/* based on ccvt_yuyv_bgr32() from camstream */
#define SAT(c)   if (c & (~255)) { if (c < 0) c = 0; else c = 255; }

CC_INLINE int icvYUYV2BGRA_8u(COLOR_FUN_ARGDEF)
{
  int i, c, r, g, b, cr, cg, cb, y1, y2;
  for (i = 0; i < height; ++i) {
    const unsigned char* s = src + i * srcstep;
    unsigned char* d = dst + i * dststep;
    c = width >> 1;
    while (c--) {
      y1 = *s++;
      cb = ((*s - 128) * 454) >> 8;
      cg = (*s++ - 128) * 88;
      y2 = *s++;
      cr = ((*s - 128) * 359) >> 8;
      cg = (cg + (*s++ - 128) * 183) >> 8;
      r = y1 + cr;
      b = y1 + cb;
      g = y1 - cg;
      SAT(r);
      SAT(g);
      SAT(b);
      d[blue_idx] = b;
      d[1] = g;
      d[2 ^ blue_idx] = r;
      d += dstcn;
      r = y2 + cr;
      b = y2 + cb;
      g = y2 - cg;
      SAT(r);
      SAT(g);
      SAT(b);
      d[blue_idx] = b;
      d[1] = g;
      d[2 ^ blue_idx] = r;
      d += dstcn;
    }
  }
  return CC_OK;
}

CC_INLINE int icvUYVY2BGRA_8u(COLOR_FUN_ARGDEF)
{
  int i, c, r, g, b, cr, cg, cb, y1, y2;
  for (i = 0; i < height; ++i) {
    const uchar* s = src + i * srcstep;
    unsigned char* d = dst + i * dststep;
    c = width >> 1;
    while (c--) {
      cb = ((*s - 128) * 454) >> 8;
      cg = (*s++ - 128) * 88;
      y1 = *s++;
      cr = ((*s - 128) * 359) >> 8;
      cg = (cg + (*s++ - 128) * 183) >> 8;
      y2 = *s++;
      r = y1 + cr;
      b = y1 + cb;
      g = y1 - cg;
      SAT(r);
      SAT(g);
      SAT(b);
      d[blue_idx] = b;
      d[1] = g;
      d[2 ^ blue_idx] = r;
      d += dstcn;
      r = y2 + cr;
      b = y2 + cb;
      g = y2 - cg;
      SAT(r);
      SAT(g);
      SAT(b);
      d[blue_idx] = b;
      d[1] = g;
      d[2 ^ blue_idx] = r;
      d += dstcn;
    }
  }
  return CC_OK;
}

CC_INLINE int icvYUYV2GRAY_8u(COLOR_FUN_ARGDEF)
{
  const unsigned char* s;
  unsigned char* d;
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
  return CC_OK;
}

CC_INLINE int icvUYVY2GRAY_8u(COLOR_FUN_ARGDEF)
{
  const unsigned char* s;
  unsigned char* d;
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
  return CC_OK;
}

CC_INLINE int icvGRAY2UYVY_8u(COLOR_FUN_ARGDEF)
{
  const unsigned char* s;
  unsigned char* d;
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
  return CC_OK;
}

CC_INLINE int icvGRAY2YUYV_8u(COLOR_FUN_ARGDEF)
{
  const unsigned char* s;
  unsigned char* d;
  int l, c, y1, y2;
  for (l = 0; l < height; ++l) {
    s = src + l * srcstep;
    d = dst + l * dststep;
    for (c = 0; c < width; c += 4) {
      y1 = s[0];
      y2 = s[1];
      d[0] = y1;
      d[2] = y2;
      y1 = s[2];
      y2 = s[3];
      d[4] = y1;
      d[6] = y2;
      s += 4;
      d += 8;
    }
  }
  return CC_OK;
}

// BAYER2RGB24 ROUTINE TAKEN FROM:
// Sonix SN9C10x based webcam basic I/F routines
// Takafumi Mizuno <taka-qce@ls-a.jp>
CC_INLINE int bayer2rgb24(COLOR_FUN_ARGDEF)
{
  long int i;
  const unsigned char* rawpt;
  unsigned char* scanpt;
  long int size;
  rawpt = src;
  scanpt = dst;
  size = width * height;
  for (i = 0; i < size; i++) {
    if ((i / width) % 2 == 0) {
      if ((i % 2) == 0) {
        /* B */
        if ((i > width) && ((i % width) > 0)) {
          *scanpt++ = (*(rawpt - width - 1) + * (rawpt - width + 1) +
                       * (rawpt + width - 1) + * (rawpt + width + 1)) / 4;             /* R */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1) +
                       * (rawpt + width) + * (rawpt - width)) / 4;                 /* G */
          *scanpt++ = *rawpt;                             /* B */
        } else {
          /* first line or left column */
          *scanpt++ = *(rawpt + width + 1);                     /* R */
          *scanpt++ = (*(rawpt + 1) + * (rawpt + width)) / 2;           /* G */
          *scanpt++ = *rawpt;                             /* B */
        }
      } else {
        /* (B)G */
        if ((i > width) && ((i % width) < (width - 1))) {
          *scanpt++ = (*(rawpt + width) + * (rawpt - width)) / 2;       /* R */
          *scanpt++ = *rawpt;                             /* G */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1)) / 2;               /* B */
        } else {
          /* first line or right column */
          *scanpt++ = *(rawpt + width);       /* R */
          *scanpt++ = *rawpt;             /* G */
          *scanpt++ = *(rawpt - 1);           /* B */
        }
      }
    } else {
      if ((i % 2) == 0) {
        /* G(R) */
        if ((i < (width * (height - 1))) && ((i % width) > 0)) {
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1)) / 2;               /* R */
          *scanpt++ = *rawpt;                                     /* G */
          *scanpt++ = (*(rawpt + width) + * (rawpt - width)) / 2;       /* B */
        } else {
          /* bottom line or left column */
          *scanpt++ = *(rawpt + 1);           /* R */
          *scanpt++ = *rawpt;                     /* G */
          *scanpt++ = *(rawpt - width);               /* B */
        }
      } else {
        /* R */
        if (i < (width * (height - 1)) && ((i % width) < (width - 1))) {
          *scanpt++ = *rawpt;                                     /* R */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1) +
                       * (rawpt - width) + * (rawpt + width)) / 4;         /* G */
          *scanpt++ = (*(rawpt - width - 1) + * (rawpt - width + 1) +
                       * (rawpt + width - 1) + * (rawpt + width + 1)) / 4;     /* B */
        } else {
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

// SGBRG to RGB24
// for some reason, red and blue needs to be swapped
// at least for  046d:092f Logitech, Inc. QuickCam Express Plus to work
//see: http://www.siliconimaging.com/RGB%20Bayer.htm
//and 4.6 at http://tldp.org/HOWTO/html_single/libdc1394-HOWTO/
CC_INLINE int icvSGBRG2BGRA_8u(COLOR_FUN_ARGDEF)
{
  long int i;
  const unsigned char* rawpt;
  unsigned char* scanpt;
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
        } else {
          /* first line or left column */
          *scanpt++ = *(rawpt + 1);            /* R */
          *scanpt++ = *(rawpt);                /* G */
          *scanpt++ = *(rawpt + width);        /* B */
        }
      } else { //odd pixel
        if ((i > width) && ((i % width) < (width - 1))) {
          *scanpt++ = *(rawpt);         /* R */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1) + * (rawpt - width) + * (rawpt + width)) / 4;        /* G */
          *scanpt++ = (*(rawpt - width - 1) + * (rawpt - width + 1) + * (rawpt + width - 1) + * (rawpt + width + 1)) / 4;             /* B */
        } else {
          /* first line or right column */
          *scanpt++ = *(rawpt);         /* R */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + width)) / 2;      /* G */
          *scanpt++ = *(rawpt + width - 1);        /* B */
        }
      }
    } else { //odd row
      if ((i % 2) == 0) {    //even pixel
        if ((i < (width * (height - 1))) && ((i % width) > 0)) {
          *scanpt++ = (*(rawpt - width - 1) + * (rawpt - width + 1) + * (rawpt + width - 1) + * (rawpt + width + 1)) / 4;                 /* R */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1) + * (rawpt - width) + * (rawpt + width)) / 4;             /* G */
          *scanpt++ = *(rawpt);   /* B */
        } else {
          /* bottom line or left column */
          *scanpt++ = *(rawpt - width + 1);            /* R */
          *scanpt++ = (*(rawpt + 1) + * (rawpt - width)) / 2;           /* G */
          *scanpt++ = *(rawpt);   /* B */
        }
      } else { //odd pixel
        if (i < (width * (height - 1)) && ((i % width) < (width - 1))) {
          *scanpt++ = (*(rawpt - width) + * (rawpt + width)) / 2;       /* R */
          *scanpt++ = *(rawpt);        /* G */
          *scanpt++ = (*(rawpt - 1) + * (rawpt + 1)) / 2;      /* B */
        } else {
          /* bottom line or right column */
          *scanpt++ = (*(rawpt - width));      /* R */
          *scanpt++ = *(rawpt);        /* G */
          *scanpt++ = (*(rawpt - 1));     /* B */
        }
      }
    }
    rawpt++;
  }
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
    } else if ((i & 0xE0) == 0x80) {
      /* code 100 */
      val = + 4;
      len = 3;
    } else if ((i & 0xE0) == 0xA0) {
      /* code 101 */
      val = -4;
      len = 3;
    } else if ((i & 0xF0) == 0xD0) {
      /* code 1101 */
      val = + 11;
      len = 4;
    } else if ((i & 0xF0) == 0xF0) {
      /* code 1111 */
      val = -11;
      len = 4;
    } else if ((i & 0xF8) == 0xC8) {
      /* code 11001 */
      val = + 20;
      len = 5;
    } else if ((i & 0xFC) == 0xC0) {
      /* code 110000 */
      val = -20;
      len = 6;
    } else if ((i & 0xFC) == 0xC4) {
      /* code 110001xx: unknown */
      val = 0;
      len = 8;
    } else if ((i & 0xF0) == 0xE0) {
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
  unsigned char code;
  const unsigned char* addr;
  const unsigned char* inp = src;
  unsigned char* outp = dst;
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
        } else if (row < 2) {
          /* top row: relative to left pixel */
          val += outp[ -2 ];
        } else {
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
static unsigned char YUV2RGB_CLP[ 1024 ];   //for clip in CCIR601
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
    const unsigned char* src0 = src + i * srcstep;
    unsigned char* dst0 = dst + i * dststep;
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
CC_INLINE int icvYUV420P2BGR_8u(COLOR_FUN_ARGDEF)
{
  int y1, y2, u, v;
  int i, j, c1, c2, c3, c4;
  const unsigned char* py1, *py2;
  const unsigned char* src0, *src1, *src2;
  unsigned char* d1, *d2;
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
  y00 = ( RGB2YUV_YR[ r00 ] + RGB2YUV_YG[ g00 ] + RGB2YUV_YB[ b00 ] + 1048576 ) >> 16;    \
  y01 = ( RGB2YUV_YR[ r01 ] + RGB2YUV_YG[ g01 ] + RGB2YUV_YB[ b01 ] + 1048576 ) >> 16;    \
  y10 = ( RGB2YUV_YR[ r10 ] + RGB2YUV_YG[ g10 ] + RGB2YUV_YB[ b10 ] + 1048576 ) >> 16;    \
  y11 = ( RGB2YUV_YR[ r11 ] + RGB2YUV_YG[ g11 ] + RGB2YUV_YB[ b11 ] + 1048576 ) >> 16;    \
  u00 = ( -RGB2YUV_UR[ r00 ] - RGB2YUV_UG[ g00 ] + RGB2YUV_UBVR[ b00 ] + 8388608 ) >> 16; \
  u01 = ( -RGB2YUV_UR[ r01 ] - RGB2YUV_UG[ g01 ] + RGB2YUV_UBVR[ b01 ] + 8388608 ) >> 16; \
  u10 = ( -RGB2YUV_UR[ r10 ] - RGB2YUV_UG[ g10 ] + RGB2YUV_UBVR[ b10 ] + 8388608 ) >> 16; \
  u11 = ( -RGB2YUV_UR[ r11 ] - RGB2YUV_UG[ g11 ] + RGB2YUV_UBVR[ b11 ] + 8388608 ) >> 16; \
  v00 = ( RGB2YUV_UBVR[ r00 ] - RGB2YUV_VG[ g00 ] - RGB2YUV_VB[ b00 ] + 8388608 ) >> 16;  \
  v01 = ( RGB2YUV_UBVR[ r01 ] - RGB2YUV_VG[ g01 ] - RGB2YUV_VB[ b01 ] + 8388608 ) >> 16;  \
  v10 = ( RGB2YUV_UBVR[ r10 ] - RGB2YUV_VG[ g10 ] - RGB2YUV_VB[ b10 ] + 8388608 ) >> 16;  \
  v11 = ( RGB2YUV_UBVR[ r11 ] - RGB2YUV_VG[ g11 ] - RGB2YUV_VB[ b11 ] + 8388608 ) >> 16;  \
  pY[j] = y00, pY[j+1] = y01, pY[j+dststep] = y01, pY[j+dststep+1] = y11;                 \
  *pU++ = (u00+u01+u10+u11)>>2; *pV++ = (v00+v01+v10+v11)>>2; src+=2, j+=2;

//  Convert from  RGB24 to YUV420
CC_INLINE int icvBGRA2YUV420P_8u_(COLOR_FUN_ARGDEF)
{
  int i, j;
  const unsigned char* rgb00, *rgb01, *rgb10, *rgb11;
  unsigned char* pY, *pU, *pV;
  int r00, r01, r10, r11, y00, y01, y10, y11;
  int g00, g01, g10, g11, u00, u01, u10, u11;
  int b00, b01, b10, b11, v00, v01, v10, v11;
  const int numpix = width * height;
  UNUSED(y10);
  icvYUVInitTable();
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

CC_INLINE int icvBGRA2YUV420_8u(COLOR_FUN_ARGDEF)
{
  int i, j;
  const unsigned char* rgb00, *rgb01, *rgb10, *rgb11;
  unsigned char* pY, *pU, *pV;
  int r00, r01, r10, r11, y00, y01, y10, y11;
  int g00, g01, g10, g11, u00, u01, u10, u11;
  int b00, b01, b10, b11, v00, v01, v10, v11;
  UNUSED(y10);
  icvYUVInitTable();
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

enum {
  ITUR_BT_601_CY = 1220542,
  ITUR_BT_601_CUB = 2116026,
  ITUR_BT_601_CUG = -409993,
  ITUR_BT_601_CVG = -852492,
  ITUR_BT_601_CVR = 1673527,
  ITUR_BT_601_SHIFT = 20,

  // Coefficients for RGB to YUV420p conversion
  ITUR_BT_601_CRY =  269484,
  ITUR_BT_601_CGY =  528482,
  ITUR_BT_601_CBY =  102760,
  ITUR_BT_601_CRU = -155188,
  ITUR_BT_601_CGU = -305135,
  ITUR_BT_601_CBU =  460324,
  ITUR_BT_601_CGV = -385875,
  ITUR_BT_601_CBV = -74448,
};

CC_INLINE int icvYUV420P2BGRA_8u(COLOR_FUN_ARGDEF)
{
  int stride = width, stride2 = stride / 2;
  int i, j, sz = height * width;
  const uchar* y1 = src;
  const uchar* u1 = src + sz;
  const uchar* v1 = src + sz + sz / 4;
  const int bIdx = blue_idx;
  for (j = 0; j < height - 1; j += 2, y1 += 2 * stride, u1 += stride2, v1 += stride2) {
    uchar* row1 = dst + (j) * dststep;
    uchar* row2 = dst + (j + 1) * dststep;
    const uchar* y2 = y1 + stride;
    for (i = 0; i < width / 2; i += 1, row1 += dstcn * 2, row2 += dstcn * 2) {
      int u = (u1[i]) - 128;
      int v = (v1[i]) - 128;
      int ruv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CVR * v;
      int guv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CVG * v + ITUR_BT_601_CUG * u;
      int buv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CUB * u;
      uchar* row10 = row1;
      uchar* row20 = row2;
      uchar* row11 = row1 + dstcn;
      uchar* row21 = row2 + dstcn;
      int y00, y01, y10, y11;
#define CLIP255(x)   ((uchar)BOUND(x, 0, 255))
      y00 = (int)(y1[2 * i]) - 16;
      y00 = MAX(0, y00) * ITUR_BT_601_CY;
      row10[2 - bIdx] = CLIP255((y00 + ruv) >> ITUR_BT_601_SHIFT);
      row10[1]      = CLIP255((y00 + guv) >> ITUR_BT_601_SHIFT);
      row10[bIdx]   = CLIP255((y00 + buv) >> ITUR_BT_601_SHIFT);
      y01 = (int)(y1[2 * i + 1]) - 16;
      y01 = MAX(0, y01) * ITUR_BT_601_CY;
      row11[2 - bIdx] = CLIP255((y01 + ruv) >> ITUR_BT_601_SHIFT);
      row11[1]      = CLIP255((y01 + guv) >> ITUR_BT_601_SHIFT);
      row11[bIdx]   = CLIP255((y01 + buv) >> ITUR_BT_601_SHIFT);
      y10 = (int)(y2[2 * i]) - 16;
      y10 = MAX(0, y10) * ITUR_BT_601_CY;
      row20[2 - bIdx] = CLIP255((y10 + ruv) >> ITUR_BT_601_SHIFT);
      row20[1]      = CLIP255((y10 + guv) >> ITUR_BT_601_SHIFT);
      row20[bIdx]   = CLIP255((y10 + buv) >> ITUR_BT_601_SHIFT);
      y11 = (int)(y2[2 * i + 1]) - 16;
      y11 = MAX(0, y11) * ITUR_BT_601_CY;
      row21[2 - bIdx] = CLIP255((y11 + ruv) >> ITUR_BT_601_SHIFT);
      row21[1]      = CLIP255((y11 + guv) >> ITUR_BT_601_SHIFT);
      row21[bIdx]   = CLIP255((y11 + buv) >> ITUR_BT_601_SHIFT);
    }
  }
  return 0;
}

CC_INLINE int icvBGRA2YUV420P_8u(COLOR_FUN_ARGDEF)
{
  const int uIdx = 1;
  const int bIdx = blue_idx;
  const int cn = srccn;
  int sz = height * width;
  int i, j, k;
  int stride = width, stride2 = stride / 2;
  uchar* y = dst;
  uchar* u = dst + sz;
  uchar* v = dst + sz + sz / 4;
  enum { shifted16 = (16 << ITUR_BT_601_SHIFT), halfShift = (1 << (ITUR_BT_601_SHIFT - 1)), shifted128 = (128 << ITUR_BT_601_SHIFT) };
  for (i = 0; i < height / 2; i++, y += 2 * stride, u += stride2, v += stride2) {
    const uchar* row0 = src + (2 * i) * srcstep;
    const uchar* row1 = src + (2 * i + 1) * srcstep;
    if (uIdx == 2) {
      T_SWAP(uchar*, u, v);
    }
    for (j = 0, k = 0; j < width * cn; j += 2 * cn, k++) {
      int r00 = row0[2 - bIdx + j];
      int g00 = row0[1 + j];
      int b00 = row0[bIdx + j];
      int r01 = row0[2 - bIdx + cn + j];
      int g01 = row0[1 + cn + j];
      int b01 = row0[bIdx + cn + j];
      int r10 = row1[2 - bIdx + j];
      int g10 = row1[1 + j];
      int b10 = row1[bIdx + j];
      int r11 = row1[2 - bIdx + cn + j];
      int g11 = row1[1 + cn + j];
      int b11 = row1[bIdx + cn + j];
      int y00 = ITUR_BT_601_CRY * r00 + ITUR_BT_601_CGY * g00 + ITUR_BT_601_CBY * b00 + halfShift + shifted16;
      int y01 = ITUR_BT_601_CRY * r01 + ITUR_BT_601_CGY * g01 + ITUR_BT_601_CBY * b01 + halfShift + shifted16;
      int y10 = ITUR_BT_601_CRY * r10 + ITUR_BT_601_CGY * g10 + ITUR_BT_601_CBY * b10 + halfShift + shifted16;
      int y11 = ITUR_BT_601_CRY * r11 + ITUR_BT_601_CGY * g11 + ITUR_BT_601_CBY * b11 + halfShift + shifted16;
      int u00, v00;
      y[2 * k + 0]         = CLIP255(y00 >> ITUR_BT_601_SHIFT);
      y[2 * k + 1]         = CLIP255(y01 >> ITUR_BT_601_SHIFT);
      y[2 * k + width + 0] = CLIP255(y10 >> ITUR_BT_601_SHIFT);
      y[2 * k + width + 1] = CLIP255(y11 >> ITUR_BT_601_SHIFT);
      u00 = ITUR_BT_601_CRU * r00 + ITUR_BT_601_CGU * g00 + ITUR_BT_601_CBU * b00 + halfShift + shifted128;
      v00 = ITUR_BT_601_CBU * r00 + ITUR_BT_601_CGV * g00 + ITUR_BT_601_CBV * b00 + halfShift + shifted128;
      u[k] = CLIP255(u00 >> ITUR_BT_601_SHIFT);
      v[k] = CLIP255(v00 >> ITUR_BT_601_SHIFT);
    }
  }
  return 0;
}


#if 0

CC_INLINE int icvRGBA2YUV420P_8u(COLOR_FUN_ARGDEF)
{
  int i, j;
  const unsigned char* rgb00, *rgb01, *rgb10, *rgb11;
  unsigned char* pY, *pU, *pV;
  int r00, r01, r10, r11, y00, y01, y10, y11;
  int g00, g01, g10, g11, u00, u01, u10, u11;
  int b00, b01, b10, b11, v00, v01, v10, v11;
  srcstep -= width * srccn;
  pY = dst, pU = pY + width * height, pV = pU + width * height /;
  //Get YUV values for rgb values...
  for (i = 0; i < height; i += 2, src += srcstep) {
    for (j = 0; j < width; j += 2) {
      YUV420_3x2_BLOCK();
      YUV420_3x2_BLOCK();
      pY += 2, pU += 4, pV += 4;
    }
  }
  return CC_OK;
}
#endif

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
      Y = (unsigned char)((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
      U = (unsigned char)((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
      V = (unsigned char)((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
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
      Y = (unsigned char)((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
      YUV[ 0 ] = CC_CAST_8U(Y);
      if (j & 1) {
        V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
        YUV[ 1 ] = CC_CAST_8U(V);
      } else {
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
      Y = (unsigned char)((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
      uyvy[ 1 ] = CC_CAST_8U(Y);
      if (j & 1) {
        V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
        uyvy[ 0 ] = CC_CAST_8U(V);
      } else {
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
  const unsigned char* yuyv = (uchar*)src;
  unsigned char* uyvy = dst, t0, t1;
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
  const unsigned char* pY = src;
  unsigned char* poutY = dst;
  unsigned char* poutU = poutY + numpix;
  unsigned char* poutV = poutU + numpix / 4;
  //unsigned char* pOut = dst;
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

static int get_blue_idx(ColorSpace c)
{
  int idx = -1;
  switch (c) {
  case T_RGB:
    idx = 2;
    break;
  case T_BGRA:
  case T_BGR:
    idx = 0;
    break;
  default:
    idx = -1;
    break;
  }
  return idx;
}

// The main function
static int colorcvt(int height, int width, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, ColorSpace srctype, ColorSpace dsttype)
{
  int src_blue_idx = get_blue_idx(srctype);
  int dst_blue_idx = get_blue_idx(dsttype);
  int i, blue_idx = src_blue_idx >= 0 ? src_blue_idx : dst_blue_idx;
  int scn = colorcn(srctype);
  int dcn = colorcn(dsttype);
  srccn = MAX(srccn, scn);
  dstcn = MAX(dstcn, dcn);
  srctype = src_blue_idx < 0 ? srctype : T_BGRA;
  dsttype = dst_blue_idx < 0 ? dsttype : T_BGRA;
  srcstep = MAX(srcstep, srccn * width);
  dststep = MAX(dststep, dstcn * width);
  if (src_blue_idx >= 0 && dst_blue_idx >= 0) {
    if (src_blue_idx == dst_blue_idx) {
      if (srccn == dstcn) {
        matcpy(dst, dststep, src, srcstep, height, width * srccn);
      } else {
        srcstep -= width * srccn;
        dststep -= width * dstcn;
        for (; height--; src += srcstep, dst += dststep) {
          for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
            uchar t0 = src[0], t1 = src[1], t2 = src[2];
            dst[0] = t0;
            dst[1] = t1;
            dst[2] = t2;
          }
        }
      }
    } else {
      srcstep -= width * srccn;
      dststep -= width * dstcn;
      for (; height--; src += srcstep, dst += dststep) {
        for (i = 0; i < width; i++, src += srccn, dst += dstcn) {
          uchar t0 = src[2], t1 = src[1], t2 = src[0];
          dst[0] = t0;
          dst[1] = t1;
          dst[2] = t2;
        }
      }
    }
    return 1;
  }
#define COLORCVT_CASE(A, B)  case ((T_##A << 8) | T_##B): { icv##A##2##B##_8u(COLOR_FUN_ARG); } break
#define COLORCVT_CASE2(A, B)  COLORCVT_CASE(A, B);COLORCVT_CASE(B, A)
  switch ((srctype << 8) | dsttype) {
    COLORCVT_CASE(RGB, BGR);
    COLORCVT_CASE(RGB, BGRA);
    COLORCVT_CASE(BayerGR, BGRA);
    COLORCVT_CASE(BayerGB, BGRA);
    COLORCVT_CASE(BayerBG, BGRA);
    COLORCVT_CASE(SGBRG, BGRA);
    COLORCVT_CASE(SN9C10X, BGRA);
    COLORCVT_CASE(YUV411P, BGRA);
    COLORCVT_CASE2(BGRA, GRAY);
    COLORCVT_CASE2(BGRA, BGR555);
    COLORCVT_CASE2(BGRA, BGR565);
    COLORCVT_CASE2(BGRA, Lab);
    //COLORCVT_CASE2(BGRA, Luv);
    COLORCVT_CASE2(BGRA, HSV);
    COLORCVT_CASE2(BGRA, YCrCb);
    COLORCVT_CASE2(BGRA, XYZ);
    COLORCVT_CASE2(BGRA, HLS);
    COLORCVT_CASE2(BGRA, YUV420P);
    COLORCVT_CASE2(BGRA, YUV420);
    COLORCVT_CASE2(BGRA, YUYV);
    COLORCVT_CASE2(BGRA, UYVY);
    COLORCVT_CASE2(GRAY, UYVY);
    COLORCVT_CASE2(GRAY, YUYV);
  default:
    printf("colorcvt not suppot %d type to %d type\n", srctype, dsttype);
    break;
  }
  return dstcn;
}

static int imcolorcvt(const img_t* ims, img_t* imd, ColorSpace _SC, ColorSpace _DC)
{
  int dstcn = colorcn(_DC);
  img_t im1[1] _INIT0;
  if (ims == imd) {
    imclone2(ims, im1);
    ims = im1;
  }
  imsetsize(imd, ims->h, ims->w, dstcn, ims->f);
  colorcvt(ims->f * ims->h, ims->w, ims->tt.data, ims->s, ims->c, imd->tt.data, imd->s, dstcn, _SC, _DC);
  imfree(im1);
  return 0;
}

#undef ChannelBlend_Normal
#undef ChannelBlend_Lighten
#undef ChannelBlend_Darken
#undef ChannelBlend_Multiply
#undef ChannelBlend_Average
#undef ChannelBlend_Add
#undef ChannelBlend_Subtract
#undef ChannelBlend_Difference
#undef ChannelBlend_Negation
#undef ChannelBlend_Screen
#undef ChannelBlend_Exclusion
#undef ChannelBlend_Overlay
#undef ChannelBlend_SoftLight
#undef ChannelBlend_HardLight
#undef ChannelBlend_ColorDodge
#undef ChannelBlend_ColorBurn
#undef ChannelBlend_LinearDodge
#undef ChannelBlend_LinearBurn
#undef ChannelBlend_LinearLight
#undef ChannelBlend_VividLight
#undef ChannelBlend_PinLight
#undef ChannelBlend_HardMix
#undef ChannelBlend_Reflect
#undef ChannelBlend_Glow
#undef ChannelBlend_Phoenix
#undef ChannelBlend_Alpha
#undef ChannelBlend_AlphaF
#undef COLOR_3DDKSHADOW
#undef COLOR_3DLIGHT
#undef COLOR_INFOTEXT
#undef COLOR_INFOBK
#undef COLOR_HOTLIGHT
#undef COLOR_GRADIENTACTIVECAPTION
#undef COLOR_GRADIENTINACTIVECAPTION
#undef _GetSysColor
#undef PRINTCOLOR
#undef COLORTABLEDEF
#undef COLOR_TYPE_DEF
#undef COLOR_FUN_ARGDEF
#undef COLOR_FUN_ARGDEF_32F
#undef COLOR_FUN_ARG
#undef icvRGB2BGR_8u
#undef fix
#undef descale
#undef cscGr_32f
#undef cscGg_32f
#undef cscGb_32f
#undef csc_shift
#undef cscGr
#undef cscGg
#undef cscGb
#undef icvGRAY2BGR_8u
#undef icvGRAY2RGB_8u
#undef icvBGR2GRAY_8u
#undef icvRGB2GRAY_8u
#undef yuvYr_32f
#undef yuvYg_32f
#undef yuvYb_32f
#undef yuvCr_32f
#undef yuvCb_32f
#undef yuv_shift
#undef yuvYr
#undef yuvYg
#undef yuvYb
#undef yuvCr
#undef yuvCb
#undef yuv_descale
#undef yuv_prescale
#undef yuvRCr_32f
#undef yuvGCr_32f
#undef yuvGCb_32f
#undef yuvBCb_32f
#undef yuvRCr
#undef yuvGCr
#undef yuvGCb
#undef yuvBCb
#undef CC_IMPL_BGRA2YCrCb
#undef CC_IMPL_YCrCb2BGRA
#undef icvBGRA2YCrYCb_8u
#undef CC_IMPL_YCrYCb2BGRA
#undef xyzXr_32f
#undef xyzXg_32f
#undef xyzXb_32f
#undef xyzYr_32f
#undef xyzYg_32f
#undef xyzYb_32f
#undef xyzZr_32f
#undef xyzZg_32f
#undef xyzZb_32f
#undef xyzRx_32f
#undef xyzRy_32f
#undef xyzRz_32f
#undef xyzGx_32f
#undef xyzGy_32f
#undef xyzGz_32f
#undef xyzBx_32f
#undef xyzBy_32f
#undef xyzBz_32f
#undef xyz_shift
#undef xyzXr_32s
#undef xyzXg_32s
#undef xyzXb_32s
#undef xyzYr_32s
#undef xyzYg_32s
#undef xyzYb_32s
#undef xyzZr_32s
#undef xyzZg_32s
#undef xyzZb_32s
#undef xyzRx_32s
#undef xyzRy_32s
#undef xyzRz_32s
#undef xyzGx_32s
#undef xyzGy_32s
#undef xyzGz_32s
#undef xyzBx_32s
#undef xyzBy_32s
#undef xyzBz_32s
#undef xyz_descale
#undef CC_IMPL_BGRA2XYZ
#undef CC_IMPL_XYZ2BGRA
#undef labXr_32f
#undef labXg_32f
#undef labXb_32f
#undef labYr_32f
#undef labYg_32f
#undef labYb_32f
#undef labZr_32f
#undef labZg_32f
#undef labZb_32f
#undef labRx_32f
#undef labRy_32f
#undef labRz_32f
#undef labGx_32f
#undef labGy_32f
#undef labGz_32f
#undef labBx_32f
#undef labBy_32f
#undef labBz_32f
#undef labT_32f
#undef labT
#undef lab_shift
#undef labXr
#undef labXg
#undef labXb
#undef labYr
#undef labYg
#undef labYb
#undef labZr
#undef labZg
#undef labZb
#undef labSmallScale_32f
#undef labSmallShift_32f
#undef labLScale_32f
#undef labLShift_32f
#undef labLScale2_32f
#undef labSmallScale
#undef labSmallShift
#undef labLScale
#undef labLShift
#undef labLScale2
#undef luvUn_32f
#undef luvVn_32f
#undef luvYmin_32f
#undef cast_get_bgr_macro
#undef cast_set_luv_macro
#undef cast_set_bgr_macro
#undef cast_get_luv_macro
#undef cast_get_bgr_macro
#undef cast_set_luv_macro
#undef cast_set_bgr_macro
#undef cast_get_luv_macro
#undef LIMIT
#undef SAT
#undef RGB2YUV420
#undef CLP
#undef YUV2RGB
#undef YUV2RGB2
#undef YUV420_3x2_BLOCK
#undef CLIP255
#undef iSourLinePitch
#undef COLORCVT_CASE
#undef COLORCVT_CASE2

#endif // _COLOR_INL_
