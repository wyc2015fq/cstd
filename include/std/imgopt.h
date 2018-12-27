#ifndef _IMOPT_INL_
#define _IMOPT_INL_

#include "img_c.h"
#include "geo_c.h"
#include "types_c.h"
#include "error_c.h"
#include <math.h>

/////////////////////////////////////////////
/* Sub-pixel interpolation methods */
enum InterpolationFlags {
  CC_INTER_NN = 0,
  CC_INTER_NEAREST = 0,
  CC_INTER_LINEAR = 1,
  CC_INTER_CUBIC = 2,
  CC_INTER_AREA = 3,
  CC_INTER_LANCZOS4 = 4,
  CC_INTER_DOWN,
  CC_INTER_MAX = 7,
  CC_WARP_FILL_OUTLIERS = 8,
  CC_WARP_INVERSE_MAP = 16,
};
enum InterpolationMasks {
  CC_INTER_BITS = 5,
  CC_INTER_BITS2 = CC_INTER_BITS * 2,
  CC_INTER_TAB_SIZE = 1 << CC_INTER_BITS,
  CC_INTER_TAB_SIZE2 = CC_INTER_TAB_SIZE * CC_INTER_TAB_SIZE
};

#define INTER_NEARSAMPLE(in, x1, y1, wl, bi, B) \
  do { \
    int pos = ((y1>>_SHIFT) * wl + (x1>>_SHIFT) * bi), bii; \
    for (bii=0; bii<bi; ++pos, ++bii) \
      (B)[bii] = in[pos]; \
  } while(0)
#define INTER_NEARSAMPLERGB(in, x1, y1, wl, bi, B) \
  do { \
    int pos = ((y1>>_SHIFT) * wl + (x1>>_SHIFT) * bi), bii; \
    (B)[bii+0] = in[pos+0], (B)[bii+1] = in[pos+1], (B)[bii+2] = in[pos+2]; \
  } while(0)
// ?????????? Bilinear Bicubic
#define INTER_BILINEARITY(in, x0, y0, wl, bi, B) \
  do { \
    int x1=(x0)& _SHIFT_MARK, y1=(y0)& _SHIFT_MARK, bii; \
    int pos = (((y0)>>_SHIFT) * wl + ((x0)>>_SHIFT) * bi); \
    int pm3 = _SHIFT_MUL(x1, y1); \
    int pm2 = y1-pm3, pm1 = x1-pm3, pm0 = _SHIFT1-y1-x1+pm3; \
    for (bii=0; bii<bi; ++pos, ++bii) \
      *(B+bii) = (int)(pm0 * in[pos] + pm1 * in[pos + bi] \
          + pm2 * in[pos + wl] + pm3 * in[pos + wl + bi] + (_SHIFT1-1))>>_SHIFT;\
  } while(0)
#define INTER_BILINEARITY_GRAY(in, x0, y0, wl, bi, out) \
  do { \
    int R, G, B; \
    int x1=(x0)& _SHIFT_MARK, y1=(y0)& _SHIFT_MARK; \
    int pos = (((y0)>>_SHIFT) * (wl) + ((x0)>>_SHIFT) * (bi)); \
    int pm3 = _SHIFT_MUL(x1, y1); \
    int pm2 = y1-pm3, pm1 = x1-pm3, pm0 = _SHIFT1-y1-x1+pm3; \
    B = (pm0 * in[pos] + pm1 * in[pos + bi] + pm2 * in[pos + wl] \
        + pm3 * in[pos + wl + bi] + (_SHIFT1-1))>>_SHIFT;++pos; \
    G = (pm0 * in[pos] + pm1 * in[pos + bi] + pm2 * in[pos + wl] \
        + pm3 * in[pos + wl + bi] + (_SHIFT1-1))>>_SHIFT;++pos; \
    R = (pm0 * in[pos] + pm1 * in[pos + bi] + pm2 * in[pos + wl] \
        + pm3 * in[pos + wl + bi] + (_SHIFT1-1))>>_SHIFT;++pos; \
    *(out) = (int)GREYSCALIZE(R, G, B); \
  } while(0)


//printf("%d, %d, %f, %f, %f, %f\n", pos, *out, _SHIFT_TOFLOAT(pm0), _SHIFT_TOFLOAT(pm1), _SHIFT_TOFLOAT(pm2), _SHIFT_TOFLOAT(pm3));
// ????????��??????????
// INTER - ?????
// IMRESIZE(h, w, A, al, ai, h*sc, w*sc, A0, w*sc, 1, INTER_BILINEARITY);
#define IMRESIZE(ah, aw, A, al, ai, bh, bw, B, bl, bi, _INTER) \
  do { \
    if ((ah)==(bh) && (aw)==(bw) && (ai)==(bi)) { \
      SKIP_COPY(ah, aw*ai, A, al, 1, B, bl, 1); \
    } else { \
      int _I, _J, _bh=(int)(bh); \
      int x = (int)_SHIFT_DIV(aw-1, bw); \
      int y = (int)_SHIFT_DIV(ah-1, bh); \
      int bwi = (int)(bw * bi); \
      int bll = (int)((int)(bl) - bwi); \
      int k = 0; \
      int Be; \
      for (_I=0; _bh--; _I+=y, k+=bll) { \
        for (_J=0, Be=k+bwi; k!=Be; _J+=x, k+=bi) { \
          _INTER((A), _J, _I, al, ai, (B+k)); \
        } \
      } \
    } \
  } while(0)
#define IMRESIZE_BASE(ah, aw, A, al, ai, bh, bw, B, bl, bi, _INTER, _IX, _IY) \
  do { \
    int _I, _J, Be, k = 0, _bh=(int)(bh); \
    int x = (int)_SHIFT_DIV(aw, bw); \
    int y = (int)_SHIFT_DIV(ah, bh); \
    int bwi = (int)(bw * bi); \
    int IXi = (int)(_IX * bi); \
    int bll = (int)((int)(bl) - bwi); \
    for (_I=0; _bh>(_IY); --_bh, _I+=y, k+=bll) { \
      for (_J=0, Be=k+bwi-IXi; k!=Be; _J+=x, k+=bi) { \
        _INTER((A), _J, _I, al, ai, (B+k)); \
      } \
      for (Be=k+IXi; k!=Be; _J+=x, k+=bi) { \
        INTER_NEARSAMPLE((A), _J, _I, al, ai, (B+k)); \
      } \
    } \
    for (; _bh--; _I+=y, k+=bll) { \
      for (_J=0, Be=k+bwi; k!=Be; _J+=x, k+=bi) { \
        INTER_NEARSAMPLE((A), _J, _I, al, ai, (B+k)); \
      } \
    } \
  } while(0)
#define IMRESIZE_AREA(SRCH, SRCW, SRC, SRCSTEP, DSTH, DSTW, DST, DSTSTEP, CN) \
  do { \
    int _I, _YS, _YD, _XS_, _XD_; \
    int x = (SRCW)/(DSTW); \
    int y = (SRCH)/(DSTH); \
    int _AREA = x*y; \
    if (_AREA>1) { \
      int _X2 = ((SRCW)<<14)/(DSTW); \
      int _Y2 = ((SRCH)<<14)/(DSTH); \
      int DSTWCN = (DSTW)*(CN); \
      int* OFS = (int*)malloc(_AREA*sizeof(int)); \
      for (_YD = 0; _YD < y; ++_YD) { \
        for (_XD_ = 0; _XD_ < x; ++_XD_) { \
          OFS[_YD*x+_XD_] = _YD*(SRCSTEP)+_XD_*(CN); \
        } \
      } \
      for (_YS=0,_YD=0;_YD<(DSTH)*(DSTSTEP);_YD+=(DSTSTEP),_YS+=_Y2) { \
        for (_XS_=0,_XD_=_YD;_XD_<(_YD+(DSTWCN));_XD_+=(CN),_XS_+=_X2){ \
          for (_I=0; _I<(CN); ++_I) { \
            int _K=0, _SUM=0, _XS2I=(_YS>>14)*(SRCSTEP)+(_XS_>>14)*(CN)+_I; \
            for(; _K<_AREA-3; _K+=4) { \
              int OFT = OFS[_K], OFT1 = OFS[_K+1]; \
              int OFT2 = OFS[_K+2], OFT3 = OFS[_K+3]; \
              _SUM += *(SRC+_XS2I+OFT)+*(SRC+_XS2I+OFT1) \
                  +*(SRC+_XS2I+OFT2)+*(SRC+_XS2I+OFT3); \
            } \
            for(; _K<_AREA; ++_K) { \
              int OFT = OFS[_K]; \
              _SUM += *(SRC+_XS2I+OFT); \
            } \
            *(DST+_XD_+_I) = (unsigned char)(_SUM/_AREA); \
          } \
        } \
      } \
      free(OFS); \
    } else { \
      IMRESIZE(SRCH, SRCW, SRC, SRCSTEP, CN, DSTH, DSTW, DST, DSTSTEP, CN, INTER_BILINEARITY);\
    } \
  } while(0)
#define IMRESIZE_BILIN(SRCH, SRCW, SRC, SRCSTEP, DSTH, DSTW, DST, DSTSTEP, CN) \
  IMRESIZE(SRCH, SRCW, SRC, SRCSTEP, CN, DSTH, DSTW, DST, DSTSTEP, CN, INTER_BILINEARITY)
// ???��???��
// co[] ????��???
#define __IMAFFINE(ah, aw, A, al, ai, bh, bw, B, bl, bi, _MUL3X2, co, _INTER, _BOUND) \
  do { \
    int _bh=(int)(bh), _bw=(int)(bw); \
    int _I, _J, x=0, y=0; \
    int bwi = (int)(bw * bi); \
    int bll = (int)((int)(bl) - bwi); \
    int k = 0; \
    int iah = _SHIFT_TOINT(ah-1), iaw = _SHIFT_TOINT(aw-1); \
    for (_I=0; _I!=_bh; ++_I, k+=bll) { \
      for (_J=0; _J!=_bw; ++_J, k+=bi) { \
        _MUL3X2(co, _J, _I, x, y); \
        _BOUND(x, 0, iaw); \
        _BOUND(y, 0, iah); \
        if (x>=0 && x<iaw && y>=0 && y<iah) { \
          _INTER(A, x, y, al, ai, (B+k)); \
        } else if (0) { _INTER(A, 0, 0, al, ai, (B+k)); } \
      } \
    } \
  } while(0)
#define IMAFFINE(ah, aw, A, al, ai, bh, bw, B, bl, bi, m, INTER) \
  __IMAFFINE(ah, aw, A, al, ai, bh, bw, B, bl, bi, MATRIX3X2_MUL_POINT, m, INTER, BOUND_CLIP)
#define IMAFFINE2(ah, aw, A, al, ai, bh, bw, B, bl, bi, m, INTER) \
  __IMAFFINE(ah, aw, A, al, ai, bh, bw, B, bl, bi, MATRIX3X2_MUL_POINT, m, INTER, BOUND_NUL)
#define IMROTATE(_AH, _AW, _A, _AL, _AI, _BH, _BW, k, _BL, _BI, _ANG, _CX, _CY, _BOUND) do {\
    MATRIX3X2 _M; MATRIX3X2_INIT(_M); MATRIX3X2_MOVE(_M, _CX, _CY); MATRIX3X2_ROTATE(_M, _ANG); \
    MATRIX3X2_MOVE(_M, -(_CX), -(_CY)); __IMAFFINE( _AH, _AW, _A, _AL, _AI, _BH, _BW, k, _BL, _BI, \
        MATRIX3X2_MUL_POINT, _M, INTER_BILINEARITY, _BOUND ); } while(0)

/////////////////////////////////////////////

static int mat_add_row(int row_a, int col_a, const double* A, int al, const double* dst, double* C, int cl) {
  int i, j;
  
  for (i = 0; i < row_a; i++) {
    for (j = 0; j < col_a; j++) {
      C[i * cl + j] = A[i * al + j] + dst[i];
    }
  }
  return 0;
}
static int mat_add_col(int row_a, int col_a, const double* A, int al, const double* dst, double* C, int cl) {
  int i, j;
  
  for (i = 0; i < row_a; i++) {
    for (j = 0; j < col_a; j++) {
      C[i * cl + j] = A[i * al + j] + dst[j];
    }
  }
  return 0;
}
// ¾ØÕó³Ë·¨ÔËËãº¯Êý
static int mat_mul_AB(int row_c, int col_c, int col_a, const double* A, int al, const double* dst, int bl, double* C, int cl)
{
  int i, j, k;
  
  for (i = 0; i < row_c; i++) {
    for (k = 0; k < col_a; k++) {
      for (j = 0; j < col_c; j++) {
        C[i * cl + j] += A[i * al + k] * dst[k * bl + j];
      }
    }
  }
  
  return 0;
}
// ¾ØÕó³Ë·¨ÔËËãº¯Êý
static int mat_mul_ATBT(int row_c, int col_c, int len, const double* A, int al, const double* dst, int bl, double* C, int cl)
{
  int i, j, k;
  
  for (j = 0; j < col_c; j++) {
    for (k = 0; k < len; k++) {
      for (i = 0; i < row_c; i++) {
        C[i * cl + j] += A[j * al + k] * dst[k * bl + i];
      }
    }
  }
  
  return 0;
}
#define mat_mul_ATA(row_a, col_a, A, al, C, cl)  mat_mul_ATB(col_a, col_a, row_a, A, al, A, al, C, cl)
static int mat_mul_ATB(int row_c, int col_c, int n, const double* A, int al, const double* dst, int bl, double* C, int cl)
{
  int i, j, k;
  for (i = 0; i < row_c; i++) {
    for (j = 0; j < col_c; j++) {
      C[i * cl + j] = 0;
    }
  }
  
  for (k = 0; k < n; k++) {
    for (i = 0; i < row_c; i++) {
      for (j = 0; j < col_c; j++) {
        C[i * cl + j] += A[k * al + i] * dst[k * bl + j];
      }
    }
  }
  
  return 0;
}
static int mat_mul_ABT(int row_c, int col_c, int n, const double* A, int al, const double* dst, int bl, double* C, int cl)
{
  int i, j, k;
  for (i = 0; i < row_c; i++) {
    for (j = 0; j < col_c; j++) {
      for (k = 0; k < n; k++) {
        C[i * cl + j] += A[i * al + k] * dst[j * bl + k];
      }
    }
  }
  
  return 0;
}
static int mat_mul_Ab(int row_a, int col_a, const double* A, int al, const double* dst, double* C)
{
  int i, j;
  for (i = 0; i < row_a; i++) {
    C[i] = 0;
    for (j = 0; j < col_a; j++) {
      C[i] += A[i * al + j] * dst[j];
    }
  }
  
  return 0;
}
static int mat_dot_Ab(int row_c, int col_c, const double* A, int al, const double* dst, double* C, int cl)
{
  int i, j;
  for (i = 0; i < row_c; i++) {
    for (j = 0; j < col_c; j++) {
      C[i * cl + j] = A[i * al + j] * dst[j];
    }
  }
  
  return 0;
}
static int immul_ABT_f8(const img_t* A, const img_t* dst, img_t* C) {
  img_t C1[1] _INIT0, *C0 = C;
  ASSERT(A->c==sizeof(double) && dst->c==A->c);
  if (A==C0 || dst==C0) {
    C = C1;
  }
  imsetsize(C, A->h, dst->h, A->c, 1);
  mat_mul_ABT(C->h, C->w, A->w, A->tt.f64, A->w, dst->tt.f64, dst->w, C->tt.f64, C->w);
  if (A==C0 || dst==C0) {
    imclone2(C1, C0);
  }
  imfree(C1);
  return 0;
}
#define mat_mul_AATw(row_a, col_a, A, al, W, C, cl)  mat_mul_ABTw(row_a, row_a, col_a, A, al, A, al, W, C, cl)
static int mat_mul_ABTw(int row_c, int col_c, int n, const double* A, int al, const double* dst, int bl, const double* W, double* C, int cl)
{
  int i, j, k;
  for (i = 0; i < row_c; i++) {
    for (j = 0; j < col_c; j++) {
      for (k = 0; k < n; k++) {
        C[i * cl + j] += A[i * al + k] * dst[j * bl + k] * W[k];
      }
    }
  }
  
  return 0;
}
static int immul_f8(const img_t* A, const img_t* dst, img_t* C) {
  //CC_GEMM_A_T
  return 0;
}
static int mat_scale(int row_c, int col_c, const double* A, int al, double bb, double dd, double* C, int cl)
{
  int i, j;
  
  for (i = 0; i < row_c; i++) {
    for (j = 0; j < col_c; j++) {
      C[i * cl + j] = A[i * al + j] * bb + dd;
    }
  }
  
  return 0;
}
static int mat_scale2(int row_c, int col_c, const double* A, int al, double sa, const double* dst, int bl, double sb, double dd, double* C, int cl)
{
  int i, j;
  
  for (i = 0; i < row_c; i++) {
    for (j = 0; j < col_c; j++) {
      C[i * cl + j] = A[i * al + j] * sa + dst[i * al + j] * sb + dd;
    }
  }
  
  return 0;
}
static int imscale2(const img_t* A, double sa, const img_t* dst, double sb, double dd, img_t* C) {
  imsetsize(C, A->h, A->w, A->c, 1);
  mat_scale2(A->h, A->w*A->c/sizeof(double), A->tt.f64, A->s/sizeof(double), sa, dst->tt.f64, dst->s/sizeof(double), sb, dd, C->tt.f64, C->s/sizeof(double));
  return 0;
}

static int mat_dotmul(int row_c, int col_c, const double* A, int al, const double* dst, int bl, double ss, double dd, double* C, int cl)
{
  int i, j;
  
  for (i = 0; i < row_c; i++) {
    for (j = 0; j < col_c; j++) {
      C[i * cl + j] = A[i * al + j] * dst[i * al + j] * ss + dd;
    }
  }
  
  return 0;
}
static int imdotmul(const img_t* A, const img_t* dst, double ss, double dd, img_t* C) {
  imsetsize(C, A->h, A->w, sizeof(double), 1);
  mat_dotmul(A->h, A->w, A->tt.f64, A->s/sizeof(double), dst->tt.f64, dst->s/sizeof(double), ss, dd, C->tt.f64, C->s/sizeof(double));
  return 0;
}
static int swap_rb(uchar* data, int step, int cn, int w, int h)
{
  int i, j;

  for (i = 0; i < h; ++i) {
    uchar* prgb = data + i * step;

    for (j = 0; j < w; ++j, prgb += cn) {
      uchar t = prgb[0];
      prgb[0] = prgb[2];
      prgb[2] = t;
    }
  }

  return 0;
}
static int imswap_rb(img_t* im) {
  return swap_rb(im->tt.data, im->s, im->c, im->w, im->h);
}
static int palette_is_color(int n, const uchar* palette)
{
  int i;

  for (i = 0; i < n; ++i) {
    const uchar* p = palette + i * 4;

    if (p[0] != p[1] || p[2] != p[1]) {
      return TRUE;
    }
  }

  return FALSE;
}
static int palette_fillgray(int n, uchar* palette, int negative)
{
  int i;
  int xor_mask = negative ? 255 : 0;

  for (i = 0; i < n; i++) {
    uchar* p = palette + i * 4;
    int val = (i * 255 / (n - 1)) ^ xor_mask;
    p[0] = p[1] = p[2] = (unsigned char) val;
    palette[3] = 0;
  }

  return 0;
}

static unsigned char* fill_unicolor(unsigned char* data, unsigned char** line_end, int step, int width3, int* y, int height, int count3, PaletteEntry clr)
{
  do {
    unsigned char* end = data + count3;

    if (end > *line_end) {
      end = *line_end;
    }

    count3 -= (int)(end - data);

    for (; data < end; data += 3) {
      *(data + 0) = clr.b;
      *(data + 1) = clr.g;
      *(data + 2) = clr.r;
    }

    if (data >= *line_end) {
      *line_end += step;
      data = *line_end - width3;

      if (++(*y) >= height) {
        break;
      }
    }
  }
  while (count3 > 0);

  return data;
}

static unsigned char* fill_unigray(unsigned char* data, unsigned char** line_end, int step, int width,
    int* y, int height, int count, unsigned char clr)
{
  do {
    unsigned char* end = data + count;

    if (end > *line_end) {
      end = *line_end;
    }

    count -= (int)(end - data);

    for (; data < end; data++) {
      *data = clr;
    }

    if (data >= *line_end) {
      *line_end += step;
      data = *line_end - width;

      if (++(*y) >= height) {
        break;
      }
    }
  }
  while (count > 0);

  return data;
}

#define PALETTE_FILLROW_IMPL(CN, PUTPIX) \
  switch (bpp) { \
  case 1: \
    for (i=0; i<len/8; ++i, data +=8*CN) { \
      idx = indices[i]; \
      PUTPIX(0,(idx & 128) != 0); \
      PUTPIX(1,(idx & 64) != 0 ); \
      PUTPIX(2,(idx & 32) != 0 ); \
      PUTPIX(3,(idx & 16) != 0 ); \
      PUTPIX(4,(idx & 8) != 0 ); \
      PUTPIX(5,(idx & 4) != 0 ); \
      PUTPIX(6,(idx & 2) != 0 ); \
      PUTPIX(7,(idx & 1)); \
    } \
    idx = indices[i]; \
    for (i=0; i<(len&7); ++i) { \
      PUTPIX(i, (idx>>(7-i))&1); \
    } \
    break; \
  case 2: \
    for (i=0; i<len/4; ++i, data +=4*CN) { \
      idx = indices[i]; \
      PUTPIX(0,(idx>>6)&3); \
      PUTPIX(1,(idx>>4)&3); \
      PUTPIX(2,(idx>>2)&3); \
      PUTPIX(3,idx & 3); \
    } \
    idx = indices[i]; \
    for (i=0; i<(len&3); ++i) { \
      PUTPIX(i, (idx>>(6-i*2))&3); \
    } \
    break; \
  case 4: \
    for (i=0; i<len/2; ++i, data +=2*CN) { \
      idx = indices[i]; \
      PUTPIX(0,idx>>4); \
      PUTPIX(1,idx&15); \
    } \
    if (len&1) { \
      idx = indices[i]; \
      PUTPIX(0, idx>>4); \
    } \
    break; \
  case 8: \
    for (i=0; i<len; ++i, data +=CN) { \
      idx = indices[i]; \
      PUTPIX(0, idx); \
    } \
    break; \
  }

static int palette_fillrow(int len, uchar* data, int cn, const uchar* src, int bpp, const uchar* palette)
{
  int i, idx;
  uchar mm4[] = {6, 4, 2, 1};
  
  if (bpp<=8) {
    const uchar* indices = src;
    if (1 == cn) {
#define PUTPIX(_I, _J)   data[_I] = palette[_J]
      PALETTE_FILLROW_IMPL(1, PUTPIX);
#undef PUTPIX
    }
    else {
#define PUTPIX(_I, _J)   memcpy(data+(_I)*cn, palette+(_J)*4, 3)
      PALETTE_FILLROW_IMPL(cn, PUTPIX);
#undef PUTPIX
    }
  } else {
    if (1 == cn) {
      switch (bpp) {
      case 15:
        IMTRANS1D(len, src, 2, data, 1, BGR5552GRAY);
        break;
        
      case 16:
        IMTRANS1D(len, src, 2, data, 1, BGR5652GRAY);
        break;
        
      case 24:
        IMTRANS1D(len, src, 3, data, 1, BGR2GRAY);
        break;
        
      case 32:
        IMTRANS1D(len, src, 4, data, 1, BGRA2GRAY);
        break;
      }
    }
    else {
      switch (bpp) {
      case 15:
        IMTRANS1D(len, src, 2, data, cn, BGR5552BGR);
        break;
        
      case 16:
        IMTRANS1D(len, src, 2, data, cn, BGR5652BGR);
        break;
        
      case 24:
        IMTRANS1D(len, src, 3, data, cn, BGRA2BGR);
        break;
        
      case 32:
        IMTRANS1D(len, src, 4, data, cn, BGRA2BGR);
        break;
      }
    }
  }
  
  return 0;
}

#undef PALETTE_FILLROW_IMPL

static int imgsetbitmap(int m_height, int m_width, uchar* data, int step, int cn_req, const uchar* src, int src_pitch, int m_bpp, const uchar* m_palette)
{
  int y;

  if (src_pitch <= 0) {
    src_pitch = (m_width * m_bpp + 7) >> 3;
    src_pitch = (src_pitch + 3) & (~3);
  }

  if (m_bpp <= 8) {
    uchar palette[1024];

    if (NULL == m_palette) {
      palette_fillgray(256, palette, 0);
      m_palette = palette;
    }

    if (1 == cn_req) {
      int clr_used = 1 << m_bpp;
      IMTRANS1D(clr_used, m_palette, 4, palette, 1, BGR2GRAY);
      m_palette = palette;
    }
  }
  for (y = 0; y < m_height; y++, data += step) {
    const uchar* src_line = src + y * src_pitch;
    palette_fillrow(m_width, data, cn_req, src_line, m_bpp, m_palette);
  }

  return 0;
}
static BOOL imsetbitmap(img_t* im, int nWidth, int nHeight, int nPlanes, int nBitcount, const void* lpBits, const void* bmiColors)
{
  int cn = (nBitcount > 8 || palette_is_color(1 << nBitcount, (uchar*)bmiColors)) ? 4 : 1;
  imsetsize(im, nWidth, nHeight, cn, 1);
  imgsetbitmap(im->h, im->w, im->tt.data, im->s, im->c, (const uchar*)lpBits, -1, nBitcount, (const uchar*)bmiColors);
  return 1;
}
static int imprint(const img_t* im, int maxh, int maxw)
{
  int i, j;
  printf("[%3d %3d]\n", im->h, im->w);

  for (i = 0; i < MIN(maxh, im->h); ++i) {
    for (j = 0; j < MIN(maxw, im->s); ++j) {
      printf("%3d,", im->tt.data[i * im->s + j]);
    }

    printf("\n");
  }

  return 0;
}

static int trans_copy_cn3(int h, int w, const unsigned char* src, int srcstep, unsigned char* dst, int dststep)
{
  int i, j = 0;

  for (j = 0; j < h; ++j) {
    for (i = 0; i < w; i += 2) {
      dst[(i + 0) * dststep + j * 3 + 0] = src[j * srcstep + (w - 1 - (i + 0)) * 3 + 0];
      dst[(i + 0) * dststep + j * 3 + 1] = src[j * srcstep + (w - 1 - (i + 0)) * 3 + 1];
      dst[(i + 0) * dststep + j * 3 + 2] = src[j * srcstep + (w - 1 - (i + 0)) * 3 + 2];

      dst[(i + 1) * dststep + j * 3 + 0] = src[j * srcstep + (w - 1 - (i + 1)) * 3 + 0];
      dst[(i + 1) * dststep + j * 3 + 1] = src[j * srcstep + (w - 1 - (i + 1)) * 3 + 1];
      dst[(i + 1) * dststep + j * 3 + 2] = src[j * srcstep + (w - 1 - (i + 1)) * 3 + 2];
    }
  }

  return 0;
}


#define TRANS_COPY(TCOPY) \
{\
  int i, j;\
  if (istrans) {\
    for (j=0; j<h; ++j) {\
      const uchar* s0 = (const uchar*)(src + j * srcstep);\
      uchar* d0 = (uchar*)(dst + (ismirry ? (h-1-j) : j)*dstcn);\
      if (ismirrx) {\
        for (i=0; i<w; ++i) {\
          TCOPY(d0+(w-1-i)*dststep, s0 + i*srccn);\
        }\
      } else {\
        for (i=0; i<w; ++i) {\
          TCOPY(d0+i*dststep, s0 + i*srccn);\
        }\
      }\
    }\
  } else {\
    for (j=0; j<h; ++j) {\
      const uchar* s0 = (const uchar*)(src + j * srcstep);\
      uchar* d0 = (uchar*)(dst + (ismirry ? (h-1-j) : j) * dststep);\
      if (ismirrx) {\
        for (i=0; i<w; ++i) {\
          TCOPY(d0+(w-1-i)*dstcn, s0+i*srccn);\
        }\
      } else {\
        for (i=0; i<w; ++i) {\
          TCOPY(d0+i*dstcn, s0+i*srccn);\
        }\
      }\
    }\
  }\
}

static int trans_copy(int h, int w, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int cn, int istrans, int ismirrx, int ismirry)
{
//#include "impl/trans_copy.inl"
  switch (cn) {
#define TCOPY(dd, ss)   (*(T*)(dd) = *(const T*)(ss))
#define IMTRANSCASE(N)  case N: {typedef struct {uchar t[N];} T; TRANS_COPY(TCOPY); } break
    IMTRANSCASE(1);
    IMTRANSCASE(2);
    IMTRANSCASE(3);
    IMTRANSCASE(4);
    IMTRANSCASE(5);
    IMTRANSCASE(6);
    IMTRANSCASE(7);
    IMTRANSCASE(8);
    IMTRANSCASE(12);
    IMTRANSCASE(16);
    IMTRANSCASE(20);
    IMTRANSCASE(24);
    IMTRANSCASE(28);
    IMTRANSCASE(32);
  default:
#define TCOPY2(dd, ss)   memcpy(dd, ss, cn)
    TRANS_COPY(TCOPY2);
    break;
  }
#undef IMTRANSCASE
#undef TCOPY2
#undef TCOPY
  return 0;
}


static int imtrans(const img_t* im, img_t* im2, int istrans, int ismirrx, int ismirry) {
  img_t im1[1] _INIT0;
  if (im==im2) {
    imclone2(im, im1);
    im = im1;
  }
  imsetsize(im2, im->w, im->h, im->c, im->f);
  trans_copy(im->h, im->w, im->tt.data, im->s, im->c, im2->tt.data, im2->s, im->c, im->c, istrans, ismirrx, ismirry);
  imfree(im1);
  return 0;
}


// flag 1-90 2-180 3-270
static img_t* rotate90(const img_t* im, img_t* dst, int flag)
{
  enum {
    ISMIRRX = 1,
    ISMIRRY = 2,
    ISTRANS = 4,
    ISROT1 = (ISTRANS | ISMIRRX),
    ISROT2 = (ISMIRRY | ISMIRRX),
    ISROT3 = (ISTRANS | ISMIRRY)
  };

  int hw[] = {im->h, im->w};
  img_t im2[1] _INIT0;
  int istrans = !!(flag & ISTRANS);
  int ismirrx = !!(flag & ISMIRRX);
  int ismirry = !!(flag & ISMIRRY);

  if (im == dst) {
    im = imclone2(im, im2);
  }

  imsetsize(dst, hw[!!istrans], hw[!istrans], im->c, 1);
  trans_copy(im->h, im->w, im->tt.data, im->s, im->c, dst->tt.data, dst->s, dst->c, im->c, istrans, ismirrx, ismirry);
  imfree(im2);
  return dst;
}

#define MAT_TRANS_CASE_IMPL(T) \
for (y=0; y<h-1; y+=2) { \
  uchar* dst0 = dst + y * incy; \
  uchar* dst1 = dst + (y+1) * incy; \
  const uchar* src0 = src + y * al; \
  const uchar* src1 = src + (y+1) * al; \
  for (x = 0; x<w-1; x+=2) { \
    *(T*)(dst0) = *(const T*)(src0); \
    *(T*)(dst0+incx) = *(const T*)(src0+ai); \
    *(T*)(dst1) = *(const T*)(src1); \
    *(T*)(dst1+incx) = *(const T*)(src1+ai); \
    src0 += 2*ai; \
    src1 += 2*ai; \
    dst0 += 2*incx; \
    dst1 += 2*incx; \
  } \
  if (x<w) { \
    *(T*)(dst0) = *(const T*)(src0); \
    *(T*)(dst1) = *(const T*)(src1); \
  } \
} \
if (y<h) { \
  uchar* dst0 = dst + y * incy; \
  const uchar* src0 = src + y * al; \
  for (x = 0; x<w; ++x) { \
    *(T*)(dst0) = *(const T*)(src0); \
    src0 += ai; \
    dst0 += incx; \
  } \
}

#define MAT_TRANS_CASE(cn) case cn: {typedef struct __ucn {uchar x[cn];} __ucn; MAT_TRANS_CASE_IMPL(__ucn); } break

// Í¼Æ¬Ðý×ªº¯ÊýÖ§³Ö90µÄ±¶Êý
// ÊäÈëÍ¼Æ¬Êý¾Ýimg  ¸ßh ¿íw Ã¿ÐÐÏñËØÕ¼µÄ×Ö½ÚÊýal Í¨µÀÊýai
// Êä³öÍ¼Æ¬Êý¾Ýrgb2 ¸ßph ¿ípw Ã¿ÐÐÏñËØÕ¼µÄ×Ö½ÚÊýpal Í¨µÀÊýÓëÊäÈëÍ¼Æ¬ÏàÍ¬
// istrans=1 Ðý×ª90¶È istrans=2 Ðý×ª180¶È istrans=3 Ðý×ª270¶È
static int mat_trans(int h, int w, const uchar* src, int al, int ai, int* ph, int* pw, uchar* dst, int* pal, int transopt, int isflop) {
  int x, y, incx, incy;
  int beg_tab[] = {0, h-1, h*w-1, (w-1)*h, w-1, 0, (h-1)*w, h*w-1};
  int incx_tab[] = {1, h, -1, -h, -1, h, 1, -h};
  int incy_tab[] = {w, -1, -w, 1, w, 1, -w, -1};
  int opt = (transopt&3) | ((!!isflop)<<2);
  incx = incx_tab[opt]*ai;
  incy = incy_tab[opt]*ai;
  dst += beg_tab[opt]*ai;
  switch (ai) {
    MAT_TRANS_CASE(1);
    MAT_TRANS_CASE(2);
    MAT_TRANS_CASE(3);
    MAT_TRANS_CASE(4);
    MAT_TRANS_CASE(6);
    MAT_TRANS_CASE(8);
  }
  
  if (opt & 1) {
    *pw = h, *ph = w, *pal = h * ai;
  }
  else {
    *pw = w, *ph = h, *pal = w * ai;
  }
  
  return 0;
}
static int mat_trans2(int h, int w, const uchar* img, int al, int ai, int* ph, int* pw, uchar* dst, int* pal, int transopt, int isflop)
{
  int i, j, bl;
  transopt &=3;
  bl = (transopt&1) ? h : w;
  
  if (3==ai) {
    for (i = 0; i < h; ++i) {
      const uchar* src0 = img + (i+0) * al;
      for (j = 0; j < w; ++j) {
        int i0 = i+0, j0 = j;

        switch (transopt) {
        case 1:
          i0 = j;
          j0 = h - i - 1;
          break;
        case 2:
          i0 = h - i - 1;
          j0 = w - j - 1;
          break;
        case 3:
          i0 = w - j - 1;
          j0 = i;
          break;
        }
        
        if (isflop) {
          j0 = bl - 1 - j0;
        }
        
        {
          uchar* dst0 = dst + (i0 * bl + j0) * ai;
          dst0[0] = src0[0], dst0[1] = src0[1], dst0[2] = src0[2];
        }
        
        src0 += ai;
      }
    }
  } else if (1==ai) {
    for (i = 0; i < h; ++i) {
      const uchar* src0 = img + (i+0) * al;
      for (j = 0; j < w; ++j) {
        int i0 = i+0, j0 = j;
        
        switch (transopt) {
        case 1:
          i0 = j;
          j0 = h - i - 1;
          break;
        case 2:
          i0 = h - i - 1;
          j0 = w - j - 1;
          break;
        case 3:
          i0 = w - j - 1;
          j0 = i;
          break;
        }
        
        if (isflop) {
          j0 = bl - 1 - j0;
        }
        
        {
          uchar* dst0 = dst + (i0 * bl + j0) * ai;
          dst0[0] = src0[0];
        }
        
        src0 += ai;
      }
    }

  }
  
  if (transopt % 2) {
    *pw = h, *ph = w, *pal = h * ai;
  }
  else {
    *pw = w, *ph = h, *pal = w * ai;
  }
  
  return 0;
}
static int mat_trans1(int h, int w, const uchar* img, int al, int ai, int* ph, int* pw, uchar* dst, int* pal, int transopt, int isflop)
{
  int i, j, bl;
  transopt &=3;
  bl = (transopt&1) ? h : w;

  if (3==ai) {
    for (i = 0; i < h; i+=2) {
      const uchar* src0 = img + (i+0) * al;
      const uchar* src1 = img + (i+1) * al;
      for (j = 0; j < w; ++j) {
        uchar* dst0;
        uchar* dst1;
        int i0 = i+0, j0 = j;
        int i1 = i+1, j1 = j;

        if (1 == transopt) {
          i0 = j;
          j0 = h - i - 1;
          i1 = j;
          j1 = h - (i+1) - 1;
        }
        else if (2 == transopt) {
          i0 = h - i - 1;
          j0 = w - j - 1;
          i1 = h - (i+1) - 1;
          j1 = w - j - 1;
        }
        else if (3 == transopt) {
          i0 = w - j - 1;
          j0 = i + 0;
          i1 = w - j - 1;
          j1 = i + 1;
        }
        
        if (isflop) {
          j0 = bl - 1 - j0;
          j1 = bl - 1 - j1;
        }
        
        dst0 = dst + (i0 * bl + j0) * ai;
        dst1 = dst + (i1 * bl + j1) * ai;
        dst0[0] = src0[0];
        dst0[1] = src0[1];
        dst0[2] = src0[2];
        
        dst1[0] = src1[0];
        dst1[1] = src1[1];
        dst1[2] = src1[2];
        src0 += ai;
        src1 += ai;
      }
    }
  } 

  if (transopt % 2) {
    *pw = h, *ph = w, *pal = h * ai;
  }
  else {
    *pw = w, *ph = h, *pal = w * ai;
  }

  return 0;
}
static int imtrans2(const img_t* im, img_t* im2, int transopt, int isflop)
{
  img_t t, im3[1] _INIT0;
  imsetsize(im3, im->h, im->w, im->c, 1);
  mat_trans(im->h, im->w, im->tt.data, im->s, im->c, &im3->h, &im3->w, im3->tt.data, &im3->s, transopt, isflop);
  CC_SWAP(*im3, *im2, t);
  imfree(im3);
  return 0;
}
static int imtrans3(const img_t* im, img_t* im2, int transopt, int isflop)
{
  img_t t, im3[1] _INIT0;
  imsetsize(im3, im->h, im->w, im->c, 1);
  mat_trans2(im->h, im->w, im->tt.data, im->s, im->c, &im3->h, &im3->w, im3->tt.data, &im3->s, transopt, isflop);
  CC_SWAP(*im3, *im2, t);
  imfree(im3);
  return 0;
}
static int alpha_copy(int h, int w, const void* A, int al, int ai, void* dst, int bl, int bi)
{
  int i, j;
  const uchar* A1 = (const uchar*)A;
  uchar* B1 = (uchar*)dst;

  for (i = 0; i < h; ++i, A1 += al, B1 += bl) {
    const uchar* A0 = A1;
    uchar* dst0 = B1;

    for (j = 0; j < w; ++j, A0 += ai, dst0 += bi) {
      uchar a = 255 - A0[ 3 ];

      if (a == 255) {
        dst0[ 0 ] = A0[ 0 ];
        dst0[ 1 ] = A0[ 1 ];
        dst0[ 2 ] = A0[ 2 ];
        dst0[ 3 ] = A0[ 3 ];
      }
      else {
        alpha_composite1(dst0[ 0 ], A0[ 0 ], a, dst0[ 0 ]);
        alpha_composite1(dst0[ 1 ], A0[ 1 ], a, dst0[ 1 ]);
        alpha_composite1(dst0[ 2 ], A0[ 2 ], a, dst0[ 2 ]);
        alpha_composite1(dst0[ 3 ], A0[ 3 ], a, dst0[ 3 ]);
      }
    }
  }

  return 0;
}
static int TransparentCopy2d_32u(int h, int w, const void* A, int al, void* dst, int bl, COLOR col, int revY)
{
#if 1
  alpha_copy(h, w, A, al, 4, dst, bl, 4);
#else
  int i, j;

  for (i = 0; i < h; ++i) {
    const uint32* a = (const uint32*)((const uchar*)A + i * al);
    uint32* b = revY ? (uint32*)((uchar*)dst + (h - 1 - i) * bl) : (uint32*)((uchar*)dst + i * bl);

    for (j = 0; j < w; ++j) {
      if (col != a[j]) {
        b[j] = a[j];
      }
    }
  }

#endif
  return 0;
}
// C=A&dst
static int imand_32u(int h, int w, const uint* A, int al, const uint* dst, int bl, uint* C, int cl, uint v)
{
  int i, j;
  //uint32* C0 = C;

  for (i = 0; i < h; ++i, A += al, dst += bl, C += cl) {
    for (j = 0; j < w; ++j) {
      C[j] = (A[j] == dst[j]) ? A[j] : v;
    }
  }

  return 0;
}

static int MapBitBlt(img_t* dst, int x, int y, int w, int h, const img_t* src, int i)
{
  //int sh = src->h / h;
  int sw = src->w / w;
  int y0 = i / sw, x0 = i % sw;
  uchar* s = ((uchar*)(src->tt.data)) + y0 * h * src->s + x0 * w * 4;
  uchar* d = ((uchar*)(dst->tt.data)) + y * h * dst->s + x * w * 4;
  alpha_copy(h, w, s, src->s, 4, d, dst->s, 4);
  return 0;
}
static int alpha_fill(int h, int w, const void* A, int al, int ai, void* dst, int bl, int bi, COLOR clr)
{
  int i, j;
  const uchar* A1 = (const uchar*)A;
  uchar* B1 = (uchar*)dst;

  for (i = 0; i < h; ++i, A1 += al, B1 += bl) {
    const uchar* A0 = A1;
    uchar* dst0 = B1;
    //const uint32* A4 = (const uint32*)A1;
    uint32* B4 = (uint32*)B1;

    for (j = 0; j < w; ++j, A0 += ai, dst0 += bi) {
      uchar a = A0[3];

      if (a) {
        B4[j] = ALPHACOLOR(B4[j], clr, a);
      }
    }
  }

  return 0;
}

static int affine_line(int ah, int aw, uchar* A, int al, int ai, uchar* dst, int bl, int bi, MATRIX3X2 co)
{
  //int _bh = (int)(0);
  int _bw = (int)(0);
  int i = 0, j, x = 0, y = 0;
  int k = 0;
  int iah = _SHIFT_TOINT(ah - 1), iaw = _SHIFT_TOINT(aw - 1);

  for (j = 0; j != _bw; ++j, k += bi) {
    MATRIX3X2_MUL_POINT(co, j, i, x, y);
    BOUND_CLIP(x, 0, iaw);
    BOUND_CLIP(y, 0, iah);

    if (x >= 0 && x <= iaw && y >= 0 && y <= iah) {
      INTER_BILINEARITY(A, x, y, al, ai, (dst + k));
    }
    else if (0) {
      INTER_BILINEARITY(A, 0, 0, al, ai, (dst + k));
    }
  }

  return 0;
}

static int imresize3(const img_t* im, double scal, img_t* im2)
{
  imsetsize(im2, (int)(im->h * scal), (int)(im->w * scal), im->c, 1);
  IMRESIZE(im->h, im->w, im->tt.data, im->s, im->c, im2->h, im2->w, im2->tt.data, im2->s, im->c, INTER_BILINEARITY);
  return 0;
}

static int geometric_scaling(int sh, int sw, int dh, int dw, ISIZE* sz)
{
  int tw = dw, th = dh;
  //int x, y;
  tw = sw * dh;
  th = sh * dw;

  if (tw > th) {
    tw = dw, th /= sw;//, y = (dh - th) / 2;
  }
  else {
    th = dh, tw /= sh;//, x = (dw - tw) / 2;
  }

  sz->w = tw, sz->h = th;
  return 0;
}

static int imresize_bili(const img_t* s, img_t* d)
{
  IMRESIZE((s->h), s->w, s->tt.data, s->s, s->c, (d->h), d->w, d->tt.data, d->s, d->c, INTER_BILINEARITY);
  return 0;
}
// ±£³Ö³¤¿í
//#define imresize2(s, sc, d) imresize(s, (int)((sc)*(s)->h), (int)((sc)*(s)->w), d)
static int imresize_aspect(const img_t* s, int dh, int dw, img_t* d)
{
  int sw = s->w, sh = s->h;
  int tw = dw, th = dh;
  img_t imt[1] _INIT0;
  if (s==d) {
    imclone2(s, imt);
    s = imt;
  }

  if (NULL == s->tt.data || 1 >= s->h || 1 >= s->w || 1 >= s->s) {
    return 0;
  }
  
  if (1) {
    //int x, y;
    tw = sw * dh;
    th = sh * dw;

    if (tw > th) {
      tw = dw, th /= sw;//, y = (dh - th) / 2;
    }
    else {
      th = dh, tw /= sh;//, x = (dw - tw) / 2;
    }
  }
  
  imsetsize(d, th, tw, s->c, s->f);
  IMRESIZE((s->h), s->w, s->tt.data, s->s, s->c, (d->h), d->w, d->tt.data, d->s, d->c, INTER_BILINEARITY);
  imfree(imt);
  return 0;
}
static img_t* imresize(const img_t* s, int dh, int dw, img_t* d) {
  int sw = s->w, sh = s->h;
  int tw = dw, th = dh, i;
  img_t imt[1] _INIT0;
  if (dw == 0 || dh == 0) {
    d->h = dh, d->w = dw;
    d->c = s->c;
    d->f = s->f;
    return d;
  }
  if (s->h==dh && s->w==dw) {
    return imclone2(s, d);
  }
  if (NULL == s->tt.data || 1 >= s->h || 1 >= s->w || 1 >= s->s) {
    return 0;
  }
  
  if (s==d) {
    imclone2(s, imt);
    s = imt;
  }
  
  imsetsize(d, th, tw, s->c, s->f);
  for (i=0; i<s->f; ++i) {
    const uchar* s1 = s->tt.data + i*s->h*s->s;
    uchar* d1 = d->tt.data + i*d->h*d->s;
    IMRESIZE(s->h, s->w, s1, s->s, s->c, d->h, d->w, d1, d->s, d->c, INTER_BILINEARITY);
  }
  imfree(imt);
  return d;
}

static ISIZE samsize(int sh, int sw, int dh, int dw, int tt)
{
  int tw = dw, th = dh;

  if (1) {
    //int x, y;
    tw = sw * dh;
    th = sh * dw;

    if (tt * tw > tt * th) {
      tw = dw, th /= sw;//, y = (dh - th) / 2;
    }
    else {
      th = dh, tw /= sh;//, x = (dw - tw) / 2;
    }
  }

  return iSIZE(tw, th);
}

// 

static int imresize_impl(int ah, int aw, const uchar* A, int al, int ai, int bh, int bw, uchar* dst, int bl, int bi, int type)
{
//#include "impl/imresize_impl_case_cn.inl"
  IMRESIZE(ah, aw, A, al, ai, bh, bw, dst, bl, bi, INTER_BILINEARITY);
  return 0;
}
static int imresize_impl_n(int ah, int aw, const uchar* A, int al, int ai, int bh, int bw, uchar* dst, int bl, int bi, int type, int n)
{
  int i;

  for (i = 0; i < n; ++i) {
    imresize_impl(ah, aw, A, al, ai, bh, bw, dst, bl, bi, type);
    A += ah * al;
    dst += bh * bl;
  }

  return 0;
}
static int imresize_n(const img_t* im, int h, int w, img_t* im2, int n)
{
  if (im->tt.data == im2->tt.data) {
    img_t im3[1] _INIT0;
    imsetsize(im3, h, w, im->c, n);
    imresize_impl_n(im->h, im->w, im->tt.data, im->s, im->c, h, w, im3->tt.data, im3->s, im3->c, CC_INTER_AREA, n);
    T_SWAP(img_t, *im3, *im2);
    imfree(im3);
  }
  else {
    imsetsize(im2, h, w, im->c, n);
    imresize_impl_n(im->h, im->w, im->tt.data, im->s, im->c, h, w, im2->tt.data, im2->s, im2->c, CC_INTER_AREA, n);
  }

  return 0;
}
#define imresize_1(im, h, w, im2) imresize_n(im, h, w, im2, 1)

static int imresizen(const img_t* a, double scaling, img_t* b, int n)
{
  int h = (int)(a->h * scaling), w = (int)(a->w * scaling);
  return imresize_n(a, h, w, b, n);
}

#define imresize1(im, scaling, im2) imresize(im, im->h*scaling, im->w*scaling, im2)

static int bf_imresize_impl(int ah, int aw, const uchar* A, int al, int ai, int bh, int bw, uchar* dst, int bl, int bi, int type)
{
#define IMMALLOC  BF_MALLOC
#define IMFREE    BF_FREE
//#include "impl/imresize_impl_case_cn.inl"
#undef IMFREE
#undef IMMALLOC
  IMRESIZE(ah, aw, A, al, ai, bh, bw, dst, bl, bi, INTER_BILINEARITY);
  return 0;
}
#if 0
static int bf_imresize(buf_t* bf, const img_t* im, int h, int w, img_t* im2)
{
  ASSERT(im->tt.data != im2->tt.data);
  bf_imsetsize(bf, im2, h, w, im->c, 1);
  bf_imresize_impl(bf, im->h, im->w, im->tt.data, im->s, im->c, h, w, im2->tt.data, im2->s, im2->c, CC_INTER_AREA);
  return 0;
}
#endif
static int imresize_rect(int ah, int aw, const uchar* A, int al, int ai, const IRECT* parc, int bh, int bw, uchar* dst, int bl, int bi, const IRECT* pbrc, const IRECT* bclip, int type)
{
  IRECT ar = iRECT(0, 0, aw, ah), br = iRECT(0, 0, bw, bh), cr;
  IRECT arc = parc ? *parc : ar;
  IRECT brc = pbrc ? *pbrc : br;

  if (bclip) {
    br = iRectInter(br, *bclip);
  }

  if (bh > 0 && bw > 0 && ah > 0 && aw > 0) {
    ar = iRectInter(ar, arc), br = iRectInter(br, brc);
    cr = iRectZoom(&brc, &ar, &arc);
    br = iRectInter(br, cr);
    ar = iRectZoom(&arc, &br, &brc);
    imresize_impl(RCH(&ar), RCW(&ar), A + ar.t * al + ar.l * ai, al, ai, RCH(&br), RCW(&br), dst + br.t * bl + br.l * bi, bl, bi, type);
  }

  return 0;
}

static int imsetmask(img_t* im, const img_t* imMask)
{
  int i, j, w = im->w, h = im->h;

  //ASSERT(im->c==4);
  if (4 == im->c) {
    for (i = 0; i < h; ++i) {
      uchar* src = im->tt.data + im->s * i;
      uchar* mar = imMask->tt.data + imMask->s * i;

      for (j = 0; j < w; ++j, src += 4, mar += imMask->c) {
        src[3] = src[3] ? (src[3] * mar[0]) / 255 : mar[0];
      }
    }
  }

  return 0;
}
static int imsetalpha(img_t* im, int alpha)
{
  int i, j, w = im->w, h = im->h;

  //ASSERT(im->c==4);
  if (4 == im->c) {
    for (i = 0; i < h; ++i) {
      uchar* src = im->tt.data + im->s * i;

      for (j = 0; j < w; ++j, src += 4) {
        src[3] = alpha;
      }
    }
  }

  return 0;
}
// ÉèÖÃÍ¸Ã÷É«
static int imsettranscolor(const img_t* im, COLOR transparent_color)
{
  int i, j, w = im->w, h = im->h;

  //ASSERT(im->c==4);
  if (im->tt.data && 4 == im->c) {
    COLOR tclr = transparent_color & 0x00ffffff;

    for (i = 0; i < h; ++i) {
      uint32* pix = (uint32*)(im->tt.data + im->s * i);

      for (j = 0; j < w; ++j) {
        if (tclr == (pix[j] & 0x00ffffff)) {
          pix[i] = 0;
        }
      }
    }
  }

  return 0;
}

#if 0
static int imto1(const img_t* im, img_t* im1, uchar* pmap)
{
  switch (im->c) {
  case 1: {
    enum {__cn = 1};
#define GETBIT1(_i)  (pmap[((src+_i*__cn)[0])] > 0)
#include "impl/imto1_case_cn.inl"
  }
  break;

  case 3:
    break;

  case 4: {
    enum {__cn = 4};
#define GETBIT1(_i)  (pmap[((src+_i*__cn)[0] + (src+_i*__cn)[1] + (src+_i*__cn)[1] + (src+_i*__cn)[2]) >> 2] > 0)
#include "impl/imto1_case_cn.inl"
  }
  break;

  default:
    ASSERT(0);
    break;
  }

  return 0;
}
#endif

static int imto32(const img_t* im, img_t* im32, BOOL swaprb)
{
  int i, j, w, h, al;
  uint32* ptr;
  imsetsize(im32, im->h, im->w, 4, im->f);
  ptr = (uint32*)im32->tt.data;
  swaprb = !!swaprb;
  w = im->w, h = im->h * im->f, al = im32->s >> 2;

  if (4 == im->c) {
    for (i = 0; i < h; ++i) {
      uchar* src = im->tt.data + im->s * i;
      uint32* dst = ptr + al * i;

      if (swaprb) {
        for (j = 0; j < w; ++j, src += 4) {
          dst[j] = _RGBA(src[2], src[1], src[0], src[3]);
        }
      }
      else {
        for (j = 0; j < w; ++j, src += 4) {
          dst[j] = _RGBA(src[0], src[1], src[2], src[3]);
        }
      }
    }
  }
  else if (3 == im->c) {
    for (i = 0; i < h; ++i) {
      uchar* src = im->tt.data + im->s * i;
      uint32* dst = ptr + al * i;

      if (swaprb) {
        for (j = 0; j < w; ++j, src += 3) {
          dst[j] = _RGB(src[2], src[1], src[0]);
        }
      }
      else {
        for (j = 0; j < w; ++j, src += 3) {
          dst[j] = _RGB(src[0], src[1], src[2]);
        }
      }
    }
  }
  else if (2 == im->c) {
    for (i = 0; i < h; ++i) {
      uchar* src = im->tt.data + im->s * i;
      uint32* dst = ptr + al * i;

      for (j = 0; j < w; ++j, src += 2) {
        dst[j] = _RGBA(src[0], src[0], src[0], src[1]);
      }
    }
  }
  else if (1 == im->c) {
    for (i = 0; i < h; ++i) {
      uchar* src = im->tt.data + im->s * i;
      uint32* dst = ptr + al * i;

      for (j = 0; j < w; ++j, src += 1) {
        dst[j] = _RGB(src[0], src[0], src[0]);
      }
    }
  }
  else {
    ASSERT(0);
  }

  return 0;
}
static int RGB_ContrastAdjust(BYTE* pRGB_In, BYTE* pRGB_Out, int Contrast, int ThresholdValue, long DateSize)
{
  int Count, Ytemp/*, tempContrast*/;

  for (Count = 0; Count < DateSize * 3; Count++) {
    Ytemp = pRGB_In[Count] + (pRGB_In[Count] - ThresholdValue) * Contrast / 255;

    if (Ytemp > 255) {
      pRGB_Out[Count] = 255;
    }
    else if (Ytemp < 0) {
      pRGB_Out[Count] = 0;
    }
    else {
      pRGB_Out[Count] = (BYTE)(Ytemp);
    }
  }

  return 0;
}
static int RGB_BrightAdjust(BYTE* pRGB_In, BYTE* pRGB_Out, int Bright, long DateSize)
{
  int Count, Ytemp;

  for (Count = 0; Count < DateSize * 3; Count++) {
    Ytemp = pRGB_In[Count] + Bright;

    if (Ytemp > 255) {
      pRGB_Out[Count] = 255;
    }
    else if (Ytemp < 0) {
      pRGB_Out[Count] = 0;
    }
    else {
      pRGB_Out[Count] = (BYTE)(Ytemp);
    }
  }

  return 0;
}
static int imflip(img_t* im)
{
  return matflip(im->h, im->s, im->tt.data, im->s);
}

// ÉÏÏÂ·­×ª
static int flip_ud(int h, int w, void* A, int al)
{
  int i, j, t;
  char* A0 = (char*)A;

  for (i = 0; i < h / 2; ++i) {
    char* A1 = A0 + i * al;
    char* A2 = A0 + (h - 1 - i) * al;

    for (j = 0; j < w; ++j) {
      CC_SWAP(A1[j], A2[j], t);
    }
  }

  return 0;
}
// ÉÏÏÂ·­×ª
static int flip_ud_fast(int h, int w, void* A, int al)
{
  int i, j, t;
  char* A0 = (char*)A;
  int w4 = w / 4;


  for (i = 0; i < h / 2; ++i) {
    {
      int* A1 = (int*)(A0 + i * al);
      int* A2 = (int*)(A0 + (h - 1 - i) * al);

      for (j = 0; j < w4; ++j) {
        CC_SWAP(A1[j], A2[j], t);
      }
    }

    if (w & 3) {
      char* A1 = (char*)(A0 + i * al + w4 * 4);
      char* A2 = (char*)(A0 + (h - 1 - i) * al + w4 * 4);

      for (j = 0; j < (w & 3); ++j) {
        CC_SWAP(A1[j], A2[j], t);
      }
    }
  }

  return 0;
}

static int flip_ud_copy(int h, int w, void* A, int al, const void* dst, int bl)
{
  int i;
  char* A0 = (char*)A;
  const char* dst0 = (const char*)dst;

  for (i = 0; i < h; ++i) {
    memcpy(A0 + (h - 1 - i)*al, dst0 + i * bl, w);
  }

  return 0;
}

// ×óÓÒ·­×ª
static int flip_lr(int h, int w, void* A, int al, int cn)
{
  int i, j, k, t;
  char* A0 = (char*)A;
  
  for (i = 0; i < h; ++i) {
    char* A1 = A0 + i * al;
    
    for (j = 0; j < w / 2; ++j) {
      char* A2 = A1 + j * cn;
      char* A3 = A1 + (w - 1 - j) * cn;
      
      for (k = 0; k < cn; ++k) {
        CC_SWAP(A2[k], A3[k], t);
      }
    }
  }
  
  return 0;
}
static int imflip_ud(img_t* im)
{
  return flip_ud(im->h, im->s, im->tt.data, im->s);
}
static int imflip_lr(img_t* im)
{
  return flip_lr(im->h, im->w, im->tt.data, im->s, im->c);
}
//////////

#define COPY_REPLICATE_BORDER(src_h, src_w, src, srcstep, dst_h, dst_w, dst, dststep, cn, top1, left1)\
  do { \
    int i, j, isrc=0, idst=0; \
    int left = (left1)*(cn), top = top1; \
    int src_width = (src_w)*(cn), src_height = src_h; \
    int dst_width = (dst_w)*(cn), dst_height = dst_h; \
    for( i = 0; i < dst_height; i++, idst += (dststep) ) { \
      /*memcpy( dst + idst + left, src + isrc, (src_width)*sizeof(*(src)) );*/ \
      COPY( (src_width), (src + isrc), (dst + idst + left) ); \
      for( j = left - 1; j >= 0; j-- ) \
        *(dst+idst+j) = *(dst+idst+j + cn); \
      for( j = left + src_width; j < (dst_width); j++ ) \
        *(dst+idst+j) = *(dst+idst+j - cn); \
      if( i >= top && i < top + src_height - 1 ) \
        isrc += srcstep; \
    } \
  } while(0)
// 
static int imreplicate_border(const img_t* im, img_t* im2, IRECT rc)
{
  img_t im1[1] _INIT0;
  if (im==im2) {
    imclone2(im, im1);
    im = im1;
  }
  imsetsize(im2, im->h + rc.t + rc.b, im->w + rc.l + rc.r, im->c, im->f);
  COPY_REPLICATE_BORDER(im->h, im->w, im->tt.data, im->s, im2->h, im2->w, im2->tt.data, im2->s, im2->c, rc.t, rc.l);
  imfree(im1);
  return 0;
}
#define rgb2gray im2gry
static img_t* im2gry(const img_t* im, img_t* gry)
{
  if (im->c>=3) {
    img_t im1[1] _INIT0;
    if (gry==im) {
      imclone2(im, im1);
      im = im1;
    }
    imsetsize(gry, im->h, im->w, 1, im->f);
    ASSERT(im->c >= 3);
    imsetsize(gry, im->h, im->w, 1, im->f);
    IMTRANS(im->h, im->w, im->tt.data, im->s, 3, gry->tt.data, gry->s, 1, BGR2GRAY);
    imfree(im1);
  } else if (im->c==1) {
    imclone2(im, gry);
  } else {
    ASSERT(0);
  }
  return gry;
}
static int im2rgb(const img_t* gry, img_t* im, int cn)
{
  img_t im1[1] _INIT0;
  if (gry==im) {
    imclone2(gry, im1);
    gry = im1;
  }
  ASSERT(3 == cn || 4 == cn);
  imsetsize(im, gry->h, gry->w, cn, gry->f);
  ASSERT(im->c >= 3);
  IMTRANS(im->h, im->w, gry->tt.data, gry->s, 1, im->tt.data, im->s, cn, GRAY2RGBx);
  imfree(im1);
  return 0;
}

static int u1_lut(int n, const uchar* s, uchar* d, const uchar* lut)
{
  int i;

  for (i = 0; i < n; ++i) {
    d[i] = lut[s[i]];
  }

  return 0;
}
static int imglut_u8(int h, int w, const uchar* A, int al, uchar* dst, int bl, const uchar* lut)
{
  int i, j;

  for (i = 0; i < h; ++i) {
    const uchar* a = A + i * al;
    uchar* b = dst + i * bl;
    j = 0;

    for (; j < w - 3; j += 4) {
      b[j + 0] = lut[a[j + 0]];
      b[j + 1] = lut[a[j + 1]];
      b[j + 2] = lut[a[j + 2]];
      b[j + 3] = lut[a[j + 3]];
    }

    for (; j < w; ++j) {
      b[j] = lut[a[j]];
    }
  }

  return 0;
}
//×ÔÊÊÓ¦
static int im2bw_adapt(const img_t* im, int thd, img_t* bw, int p0, int p1)
{
  int i, j, ithd = thd;
  uchar lut[256] _INIT0;
  //bf_ imsetsize(bw, im->h, im->w, 1, 1);

  if (1) {
    int imax = im->tt.data[0], imin = im->tt.data[0];

    for (i = 0; i < im->h; ++i) {
      for (j = 0; j < im->w; ++j) {
        int pix = im->tt.data[i * im->s + j];

        if (pix < imin) {
          imin = pix;
        }
        else if (pix > imax) {
          imax = pix;
        }
      }
    }

    ithd = (thd * (imax - imin)) / 255 + imin;
  }

  for (i = 0; i < 256; ++i) {
    lut[i] = (i <= ithd) ? p0 : p1;
  }

  imglut_u8(im->h, im->w, im->tt.data, im->s, bw->tt.data, bw->s, lut);
  return ithd;
}
static int im2bw_adapt1(const img_t* im, int thd, img_t* bw, int p0, int p1)
{
  int i, j, ithd = thd;
  uchar lut[256] _INIT0;
  //bf_ imsetsize(bw, im->h, im->w, 1, 1);

  if (1) {
    int cnt[256] _INIT0, sumcnt = 0, tt = thd * im->h * im->w / 256;

    for (i = 0; i < im->h; ++i) {
      for (j = 0; j < im->w; ++j) {
        int pix = im->tt.data[i * im->s + j];
        cnt[pix]++;
      }
    }

    for (i = 0; i < 256 && sumcnt <= tt; ++i) {
      sumcnt += cnt[i];
    }

    ithd = i;
  }

  for (i = 0; i < 256; ++i) {
    lut[i] = (i <= ithd) ? p0 : p1;
  }

  imglut_u8(im->h, im->w, im->tt.data, im->s, bw->tt.data, bw->s, lut);
  return ithd;
}

static int im2avg_u8(const img_t* im)
{
  int i, j, isum = 0, iavg = 0, n = im->h * im->w;;

  if (0 == n) {
    return 0;
  }

  for (i = 0; i < im->h; ++i) {
    const uchar* a = im->tt.data + i * im->s;

    for (j = 0; j < im->w; ++j) {
      isum += a[j];
    }
  }

  iavg = isum / n;
  return iavg;
}

static int get_otsu_thd(const img_t* im)
{
  int i, j, pixelSum = im->w * im->h, threshold = 0, dd = 0;
  int pixelCount[256] _INIT0;
  float pixelPro[256] _INIT0;
  float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;

  //Í³¼Æ»Ò¶È¼¶ÖÐÃ¿¸öÏñËØÔÚÕû·ùÍ¼ÏñÖÐµÄ¸öÊý
  for (j = 0; j < im->h; j++) {
    for (i = 0; i < im->w; i++) {
      dd = im->tt.data[i * im->s + j];
      pixelCount[dd]++;

      if (dd != 255) {
        int kk = 12;
      }
    }
  }

  //¼ÆËãÃ¿¸öÏñËØÔÚÕû·ùÍ¼ÏñÖÐµÄ±ÈÀý
  for (i = 0; i < 256; i++) {
    pixelPro[i] = (float)pixelCount[i] / pixelSum;
  }

  //±éÀú»Ò¶È¼¶[0,255]
  for (i = 0; i < 256; i++) {
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

    for (j = 0; j < 256; j++) {
      if (j <= i) { //±³¾°²¿·Ö
        w0 += pixelPro[j];
        u0tmp += j * pixelPro[j];
      }
      else { //Ç°¾°²¿·Ö
        w1 += pixelPro[j];
        u1tmp += j * pixelPro[j];
      }
    }

    u0 = u0tmp / w0;
    u1 = u1tmp / w1;
    u = u0tmp + u1tmp;
    deltaTmp = w0 * (float)pow((u0 - u), 2) + w1 * (float)pow((u1 - u), 2);

    if (deltaTmp > deltaMax) {
      deltaMax = deltaTmp;
      threshold = i;
    }
  }

  return threshold;
}

static int im2bw(const img_t* im, int thd, img_t* bw)
{
  int i, j, ithd = thd;
  int imax = 0, imin = 1000;
  imsetsize(bw, im->h, im->w, 1, 1);

  if (0) {
    for (i = 0; i < im->h; ++i) {
      for (j = 0; j < im->w; ++j) {
        int pix = im->tt.data[i * im->s + j];
        imax = MAX(imax, pix);
        imin = MIN(imin, pix);
      }
    }

    ithd = (thd * (imax - imin)) / 255 + imin;
  }

  for (i = 0; i < im->h; ++i) {
    for (j = 0; j < im->w; ++j) {
      int px = im->tt.data[i * im->s + j] <= ithd ? 0 : 255;
      bw->tt.data[i * im->s + j] = px;
    }
  }

  return 0;
}

static int im2xor(img_t* im, int val, img_t* bw)
{
  int i, j;
  //imsetsize(bw, im->h, im->w, 1, 1);

  for (i = 0; i < im->h; ++i) {
    for (j = 0; j < im->w; ++j) {
      int px = im->tt.data[i * im->s + j];
      bw->tt.data[i * im->s + j] = px ^ val;
    }
  }

  return 0;
}

//#include ".\draw\imwarp.inl"
#if 0
static int test_imresize()
{
  img_t im[1] _INIT0;
  img_t im2[5] _INIT0;
  int i;
  _chdir("D:/pub/pic");
  imread("lena.BMP", 4, 1, im);
  imsetsize(im2, 800, 800, 4, 1);
  {
    utime_start(_start_time);

    //imresize_nn(im, im2);
    for (i = 10; --i;) {
      imresize_bilinear(im, im2);
    }

    //imresize(im, im2->h, im2->w, im2);
    printf("%f\n", utime_elapsed(_start_time));
  }
  imshow2("im", im);
  imshow2("PicZoom2", im2);
  cvWaitKey(-1);
  imfree(im);
  return 0;
}
#endif

static int mat2gray_u8(img_t* im)
{
  int i, mini, maxi, minv, maxv;
  MINMAX_ELEM2D(im->h, im->w, im->tt.data, im->s, im->c, mini, maxi);
  minv = im->tt.data[mini], maxv = im->tt.data[maxi];

  for (i = 0; i < im->h * im->w; ++i) {
    im->tt.data[i] = (im->tt.data[i] - minv) * 255 / (maxv - minv);
  }

  return 0;
}

static int in2double(const img_t* im, img_t* im2)
{
  img_t im3[1] _INIT0;
  int i;
  double* dst;
  imsetsize(im3, im->h, im->w, sizeof(double), 1);
  dst = (double*)(im3->tt.data);

  for (i = 0; i < im->h * im->w; ++i) {
    dst[i] = im->tt.data[i];
  }

  imswap(im2, im3), imfree(im3);
  return 0;
}

#define MAT2GRAY(IM, typein, elesz, im2) { \
    int i, j, mini, maxi, _step=((IM)->s/elesz), _cn=((IM)->c/elesz); \
    MINMAX_ELEM2D((IM)->h, (IM)->w, (IM)->typein, _step, _cn, mini, maxi); \
    for (i = 0; i < (IM)->h; ++i) { \
      for (j = 0; j < (IM)->w; ++j) { \
        im2->tt.data[i] = (int)(((IM)->typein[i] - (IM)->typein[mini]) * 255 / ((IM)->typein[maxi] - (IM)->typein[mini])); \
      } \
    } \
  }


// ×ÝÏòÆ´½Ó
static int imcatv(int n, const img_t* ims, img_t* im2)
{
  int i, h2 = 0, w2 = 0, c2 = 0, y = 0;

  for (i = 0; i < n; ++i) {
    h2 += ims[i].h, w2 = MAX(w2, ims[i].w), c2 = MAX(c2, ims[i].c);
  }

  imsetsize(im2, h2, w2, c2, 1);

  for (i = 0; i < n; ++i) {
    memcpy2d(im2->tt.data + y * im2->s, im2->s, ims[i].tt.data, ims[i].s, ims[i].h, ims[i].w * ims[i].c);
    y += ims[i].h;
  }

  return 0;
}
static int imcatv2(const img_t* im0, const img_t* im1, img_t* im) {
  img_t ims[2];
  ims[0] = *im0;
  ims[1] = *im1;
  return imcatv(2, ims, im);
}
static int imextend(img_t* im, int h, int w, int cn) {
  int i;
  if (im->h!=h || im->w!=w) {
    img_t im2[1] _INIT0;
    int f = MAX(im->f, 1);
    cn = MAX(cn, im->c);
    imsetsize(im2, h, w, cn, f);
    for (i=0; i<f; ++i) {
      memcpy2d(im2->tt.data + i * im2->s*im2->h, im2->s, im->tt.data + i*im->s*im->h, im->s, im->h, im->w * im->c);
    }
    imfree(im);
    *im = *im2;
  }
  return 0;
}

static int imcatv1(img_t* im, const img_t* im1) {
  int h = im->h, w = im->w;
  imextend(im, im->h+im1->h, MAX(w, im1->w), im1->c);
  memcpy2d(im->tt.data + im->s*h, im->s, im1->tt.data, im1->s, im1->h, im1->w * im1->c);
  return 0;
}

// ºáÏòÆ´½Ó
static int imcath(int n, const img_t* ims, img_t* im2)
{
  int i, h2 = 0, w2 = 0, c2 = 0, x = 0;

  for (i = 0; i < n; ++i) {
    w2 += ims[i].w, h2 = MAX(h2, ims[i].h), c2 = MAX(c2, ims[i].c);
  }

  imsetsize(im2, h2, w2, c2, 1);

  for (i = 0; i < n; ++i) {
    memcpy2d(im2->tt.data + x * im2->c, im2->s, ims[i].tt.data, ims[i].s, ims[i].h, ims[i].w * ims[i].c);
    x += ims[i].w;
  }

  return 0;
}
static int imcath2(const img_t* im0, const img_t* im1, img_t* im) {
  img_t ims[2];
  ims[0] = *im0;
  ims[1] = *im1;
  return imcath(2, ims, im);
}
static int imcat(int rows, int cols, const img_t* ims, img_t* im2)
{
  int i, j, h2 = 0, w2 = 0, c2 = 0, y = 0, n =  rows * cols;
  const img_t* im;

  for (i = 0, im = ims; i < rows; ++i) {
    int w1 = 0, h1 = 0;

    for (j = 0; j < cols; ++j) {
      w1 += im->w, h1 = MAX(h1, im->h), c2 = MAX(c2, im->c);
    }

    h2 += h1, w2 = MAX(w2, w1);
  }

  imsetsize(im2, h2, w2, c2, 1);

  for (i = 0, im = ims; i < rows; ++i) {
    int h1 = 0, x = 0;

    for (j = 0; j < cols; ++j, ++im) {
      memcpy2d(im2->tt.data + y * im2->s + x * im2->c, im2->s, im->tt.data, im->s, im->h, im->w * im->c);
      x += im->w;
      h1 = MAX(h1, im->h);
    }

    y += h1;
  }

  return 0;
}
//ºÏ²¢Í¨µÀ
static int imcat3(const img_t* im1, const img_t* im2, img_t* im) {
  img_t im0[1] _INIT0;
  int i, j;
  ASSERT(im1->h==im2->h && im1->w==im2->w && im1->f==im2->f);
  imsetsize(im0, im1->h, im1->w, im1->c+im2->c, 1);
  for (i = 0; i < im1->h; ++i) {
    uchar* im01 = im0->tt.data + i*im0->s;
    const uchar* im11 = im1->tt.data + i*im1->s;
    const uchar* im21 = im2->tt.data + i*im2->s;
    for (j = 0; j < im1->w; ++j) {
      memcpy(im01, im11, im1->c);
      im01+=im1->c, im11+=im1->c;
      memcpy(im01, im21, im2->c);
      im01+=im2->c, im21+=im2->c;
    }
  }
  imclone2(im0, im);
  imfree(im0);
  return 0;
}
static BOOL imisContinuous(const img_t* im) {
  return im->w*im->c==im->s;
}
// n - number of samples
static int asRowMatrix(int n, img_t* src, double alpha, double beta, img_t* data) {
  int h, w, d, c, i;
  // make sure the input data is a vector of matrices or vector of vector
  // return empty matrix if no matrices given
  if(n == 0) {
    return 0;
  }
  // dimensionality of (reshaped) samples
  h = src->h;
  w = src->w;
  c = src->c;
  d = h*w;
  // create data matrix
  imsetsize(data, n, d, c, 1);
  // now copy data
  for(i = 0; i < n; i++) {
    img_t* im = src + i;
    // make sure data can be reshaped, throw exception if not!
    int d1 = im->h*im->w;
    if(d1 != d || im->c!=c) {
      //CC_Error(CC_StsBadArg, _format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, d, d1));
      return 0;
    }
    // get a hold of the current row
    // make reshape happy by cloning for non-continuous matrices
    memcpy2d(data->tt.data + i * data->s, w*c, im->tt.data, im->s, h, w*c);
  }
  return 1;
}

static int im2bit1(int n, const uchar* A, uchar* dst)
{
  int i = 0;

  for (; i < n - 7; i += 8) {
#define TTBIT(I)  ((A[i+I]>0)<<I)
    *dst++ = TTBIT(0) | TTBIT(1) | TTBIT(2) | TTBIT(3) | TTBIT(4) | TTBIT(5) | TTBIT(6) | TTBIT(7);
#undef TTBIT
  }

  for (; i < n; ++i) {
    dst[i >> 3] |= (A[i] > 0) << (i & 7);
  }

  return 0;
}
static int bit12im(int n, const uchar* A, uchar* dst)
{
  int i = 0;

  for (; i < n - 7; i += 8) {
#define TTBIT(I)  dst[i+I]=((A[i>>3]>>I)&1)
    TTBIT(0), TTBIT(1), TTBIT(2), TTBIT(3), TTBIT(4), TTBIT(5), TTBIT(6), TTBIT(7);
#undef TTBIT
  }

  for (; i < n; ++i) {
    dst[i] = ((A[i >> 3] >> (i & 7)) & 1);
  }

  return 0;
}

static int imsig_base(int h, int w, const uchar* A, int al, int ai, int n, uchar* bbb, void* out)
{
  int i, s = 0, nn = n * n;
  ASSERT(1 == ai);
  //IMRESIZE(h, w, A, al, 1, 8, 8, bbb, 8, 1, INTER_BILINEARITY);
  IMRESIZE_AREA(h, w, A, al, n, n, bbb, n, 1);

  for (i = 0; i < nn; ++i) {
    s += bbb[i];
  }

  s /= nn;

  for (i = 0; i < nn; ++i) {
    bbb[i] = bbb[i] > s;
  }

  im2bit1(nn, bbb, (uchar*)out);
  return 0;
}
static int imsig_8x8(int h, int w, const uchar* A, int al, int ai, void* dst)
{
  enum {N = 8};
  uchar bbb[N * N] _INIT0;
  imsig_base(h, w, A, al, ai, N, bbb, dst);
  return 0;
}

static int imsig_16x16(int h, int w, const uchar* A, int al, int ai, uchar* dst)
{
  enum {N = 16};
  uchar bbb[N * N] _INIT0;
  imsig_base(h, w, A, al, ai, N, bbb, dst);
  return 0;
}


// BD_CONSTANT  Input array values outside the bounds of the array are implicitly assumed to have the value X. When no boundary option is specified, imfilter uses X = 0.
// BD_SYMMETRIC Input array values outside the bounds of the array are computed by mirror-reflecting the array across the array border.
// BD_REPLICATE Input array values outside the bounds of the array are assumed to equal the nearest array border value.
// BD_CIRCULAR  Input array values outside the bounds of the array are computed by implicitly assuming the input array is periodic.

enum { BD_CONSTANT, BD_REPLICATE, BD_WRAP, BD_REFLECT, BD_REFLECT_101, };

// Various border types, image boundaries are denoted with '|'
//  BD_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh
//  BD_REFLECT:       fedcba|abcdefgh|hgfedcb
//  BD_REFLECT_101:   gfedcb|abcdefgh|gfedcba
//  BD_WRAP:          cdefgh|abcdefgh|abcdefg
//  BD_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'

static int border_interpolate(int p, int len, int borderType)
{
  if ((unsigned)p < (unsigned)len)
    ;
  else if (borderType == BD_REPLICATE) {
    p = p < 0 ? 0 : len - 1;
  }
  else if (borderType == BD_REFLECT || borderType == BD_REFLECT_101) {
    int delta = borderType == BD_REFLECT_101;

    if (len == 1) {
      return 0;
    }

    do {
      if (p < 0) {
        p = -p - 1 + delta;
      }
      else {
        p = len - 1 - (p - len) - delta;
      }
    }
    while ((unsigned)p >= (unsigned)len);
  }
  else if (borderType == BD_WRAP) {
    if (p < 0) {
      p -= ((p - len + 1) / len) * len;
    }

    if (p >= len) {
      p %= len;
    }
  }
  else if (borderType == BD_CONSTANT) {
    p = -1;
  }
  else {
    CC_Error(CC_StsBadArg, "Unknown/unsupported border type");
  }

  return p;
}

#if 0

static int mem_fill_border(int ah, int aw, const void* A, int al, int bh, int bw, void* dst, int bl, int cn, int l, int t, void* c, int type)
{
  switch (cn) {
#if 0

  case 3: {
    typedef struct {
      uchar t[3];
    } arrtype;
#include "impl/mem_fill_border_impl.inl"
  }
  break;
#else

  case 1: {
    typedef uchar arrtype;
#include "impl/mem_fill_border_impl.inl"
  }
  break;

  case 2: {
    typedef ushort arrtype;
#include "impl/mem_fill_border_impl.inl"
  }
  break;

  case 3: {
    typedef struct {
      uchar t[3];
    } arrtype;
#include "impl/mem_fill_border_impl.inl"
  }
  break;

  case 4: {
    typedef uint arrtype;
#include "impl/mem_fill_border_impl.inl"
  }
  break;

  case 8: {
    typedef struct {
      uint t[2];
    } arrtype;
#include "impl/mem_fill_border_impl.inl"
  }
  break;
#endif

  default:
    ASSERT(0);
    break;
  }

  return 0;
}
#endif

typedef enum {
  OptRow, OptCol, OptAll
} OptMode;

#define MAT_OPT_DEF(name, name2, arrtype, worktype, outtype, op) static int name(int ah, int aw, const arrtype* A, int al, int bh, int bw, const arrtype* dst, int bl, outtype* C, int cl) { \
int i, j, h = ah, w = aw; \
if ((1==aw && ah==bh) || (1==ah && aw==bh)) { \
  h = bh, w = bw; \
  for (i=0; i<h; ++i) { \
    worktype a1 = A[i]; \
    const worktype* b1 = dst + i*bl; \
    double* c1 = C + i*cl; \
    for (j=0; j<w; ++j) { \
      c1[j] = a1 op b1[j]; \
    } \
  } \
} else if ((1==bw && bh==ah) || (1==bh && bw==ah)) { \
  h = ah, w = aw; \
  for (i=0; i<h; ++i) { \
    const worktype* a1 = A + i*al; \
    worktype b1 = dst[i]; \
    worktype* c1 = C + i*cl; \
    for (j=0; j<w; ++j) { \
      c1[j] = a1[j] op b1; \
    } \
  } \
} else { \
  if ((1==ah && aw==bw) || (1==aw && ah==bw)) { \
    h = bh, w = bw, al = 0; \
  } else if ((1==bh && bw==aw) || (1==bw && bh==aw)) { \
    h = ah, w = aw, bl = 0; \
  } else if (ah!=bh || aw!=bw) {ASSERT(0);return 0;}\
  for (i=0; i<h; ++i) { \
    const worktype* a1 = A + i*al; \
    const worktype* b1 = dst + i*bl; \
    worktype* c1 = C + i*cl; \
    for (j=0; j<w; ++j) { \
      c1[j] = a1[j] op b1[j]; \
    } \
  } \
} return 0;\
  } \
static int name2(const img_t* A, const img_t* dst, img_t* C) { \
  img_t im[1] _INIT0, *C0 = C; \
  C = (A==C || dst==C) ? im : C; \
  ASSERT(A->c==dst->c && A->c==sizeof(double)); \
  imsetsize(C, MAX(A->h, dst->h), MAX(A->w, dst->w), sizeof(double), 1); \
  name(A->h, A->w, A->tt.f64, A->s/sizeof(double), dst->h, dst->w, dst->tt.f64, dst->s/sizeof(double), C->tt.f64, C->s/sizeof(double)); \
  if (im==C) { imswap(im, C0); } \
  imfree(im); \
  return 0; \
}

MAT_OPT_DEF(mat_minus_f8, imminus_f8, double, double, double, -);
MAT_OPT_DEF(mat_divide_f8, imdivide_f8, double, double, double, /);
MAT_OPT_DEF(mat_times_f8, imtimes_f8, double, double, double, *);
MAT_OPT_DEF(mat_plus_f8, implus_f8, double, double, double, +);

static int mat_sum_f8(int h, int w, const double* A, int al, OptMode mode, double* dst) {
  int i, j;
  double bb;
  switch (mode) {
  case OptRow:
    for (j=0; j<w; ++j) {
      dst[j]  = 0;
    }
    for (i=0; i<h; ++i) {
      const double* a1 = A + i*al;
      for (j=0; j<w; ++j) {
        dst[j] += a1[j];
      }
    }
    break;
  case OptCol:
    for (i=0; i<h; ++i) {
      const double* a1 = A + i*al;
      bb=0;
      for (j=0; j<w; ++j) {
        bb += a1[j];
      }
      dst[i] = bb;
    }
    break;
  case OptAll:
    bb=0;
    for (i=0; i<h; ++i) {
      const double* a1 = A + i*al;
      for (j=0; j<w; ++j) {
        bb += a1[j];
      }
    }
    dst[0] = bb;
    break;
  default:
    ASSERT(0);
    break;
  }
  return 0;
}
static int mat_get_nk(int h, int w, OptMode mode, double* pk) {
  int n = 1;
  double k = h*w;
  switch (mode) {
  case OptRow:
    n = w, k = h;
    break;
  case OptCol:
    n = h, k = w;
    break;
  case OptAll:
    n = 1, k = h*w;
    break;
  default:
    ASSERT(0);
    break;
  }
  if (pk) {
    *pk = k;
  }
  return n;
}

static int mat_mean_f8(int h, int w, const double* A, int al, OptMode mode, double* dst) {
  int i, n = 1;
  double k = h*w;
  mat_sum_f8(h, w, A, al, mode, dst);
  n = mat_get_nk(h, w, mode, &k);
  for (i=0; i<n; ++i) {
    dst[i]/=k;
  }
  return 0;
}
static int mat_meanstd_f8(int h, int w, const double* A, int al, OptMode mode, double* dst, double* S, int flag, bool do_sqrt) {
  int i, j;
  double t, ss;
  mat_mean_f8(h, w, A, al, mode, dst);
  switch (mode) {
  case OptRow:
    for (j=0; j<w; ++j) {
      S[j]  = 0;
    }
    for (i=0; i<h; ++i) {
      const double* a1 = A + i*al;
      for (j=0; j<w; ++j) {
        t = a1[j] - dst[j];
        S[j] += t * t;
      }
    }
    break;
  case OptCol:
    for (i=0; i<h; ++i) {
      const double* a1 = A + i*al;
      ss=0;
      for (j=0; j<w; ++j) {
        t = a1[j] - dst[j];
        ss += t * t;
      }
      S[i] = ss;
    }
    break;
  case OptAll:
    ss = 0;
    for (i=0; i<h; ++i) {
      const double* a1 = A + i*al;
      for (j=0; j<w; ++j) {
        t = a1[j] - dst[j];
        ss += t * t;
      }
    }
    S[0] = ss;
  default:
    ASSERT(0);
    break;
  }
  {
    int i, n = 1;
    double k = h*w;
    n = mat_get_nk(h, w, mode, &k);
    k -= !flag;
    ASSERT(k>0);
    if (do_sqrt) {
      for (i=0; i<n; ++i) {
        S[i] = sqrt(S[i]/k);
      }
    } else {
      for (i=0; i<n; ++i) {
        S[i]/=k;
      }
    }
  }
  return 0;
}
static int imsum_f8(const img_t* im, OptMode mode, img_t* sum) {
  int n = 1;
  img_t im1[1] _INIT0;
  if (im==sum) {
    imclone2(im, im1);
    im = im1;
  }
  n = mat_get_nk(im->h, im->w, mode, NULL);
  imsetsize(sum, n, 1, sizeof(double), 1);
  ASSERT(im->c==sizeof(double));
  mat_sum_f8(im->h, im->w, im->tt.f64, im->s/sizeof(double), mode, sum->tt.f64);
  imfree(im1);
  return 0;
}
static int immean_f8(const img_t* im, OptMode mode, img_t* mu) {
  int n = 1;
  img_t im1[1] _INIT0;
  if (im==mu) {
    imclone2(im, im1);
    im = im1;
  }
  n = mat_get_nk(im->h, im->w, mode, NULL);
  imsetsize(mu, n, 1, sizeof(double), 1);
  ASSERT(im->c==sizeof(double));
  mat_mean_f8(im->h, im->w, im->tt.f64, im->s/sizeof(double), mode, mu->tt.f64);
  imfree(im1);
  return 0;
}
// flag = 0 : s = sum(x-mu)/(n-1)
// flag = 1 : s = sum(x-mu)/(n-n)
static int immeanstd_f8(const img_t* im, int flag, OptMode mode, img_t* mu, img_t* st) {
  int n = 1;
  img_t im1[2] _INIT0;
  if (NULL==st) {
    return immean_f8(im, mode, mu);
  }
  if (NULL==mu) {
    mu = im1 + 1;
  }
  if (im==mu || im==st) {
    imclone2(im, im1);
    im = im1;
  }
  n = mat_get_nk(im->h, im->w, mode, NULL);
  imsetsize(mu, n, 1, sizeof(double), 1);
  imsetsize(st, n, 1, sizeof(double), 1);
  ASSERT(im->c==sizeof(double));
  mat_meanstd_f8(im->h, im->w, im->tt.f64, im->s/sizeof(double), mode, mu->tt.f64, st->tt.f64, flag, true);
  imfrees(im1, 2);
  return 0;
}
static int mat_maxs_f8(int h, int w, const double* A, int al, double s, double* dst, int bl) {
  int i, j;
  for (i=0; i<h; ++i) {
    const double* a1 = A + i*al;
    double* b1 = dst + i*bl;
    for (j=0; j<w; ++j) {
      b1[j] = MAX(a1[j], s);
    }
  }
  return 0;
}
static int immaxs_f8(const img_t* im, double s, img_t* y) {
  imsetsize(y, im->h, im->w, sizeof(double), 1);
  mat_maxs_f8(im->h, im->w, im->tt.f64, im->s/sizeof(double), s, y->tt.f64, y->s/sizeof(double));
  return 0;
}
static int mat_max_f8(int h, int w, const double* A, int al, OptMode mode, double* dst) {
  int i, j;
  if (mode) {
    for (j=0; j<w; ++j) {
      dst[j]  = A[j];
    }
    for (i=1; i<h; ++i) {
      const double* a1 = A + i*al;
      for (j=0; j<w; ++j) {
        if (a1[j] > dst[j]) {
          dst[j] = a1[j];
        }
      }
    }
  } else {
    for (i=0; i<h; ++i) {
      const double* a1 = A + i*al;
      double bb=a1[0];
      for (j=1; j<w; ++j) {
        if (a1[j] > bb) {
          bb = a1[j];
        }
      }
      dst[i] = bb;
    }
  }
  return 0;
}
static int mat_minus_dim_f8(int h, int w, const double* A, int al, const double* dst, OptMode mode, double* C, int cl) {
  int i, j;
  if (mode) {
    for (i=0; i<h; ++i) {
      const double* a1 = A + i*al;
      double* c1 = C + i*cl;
      for (j=0; j<w; ++j) {
        c1[j] = a1[j] - dst[j];
      }
    }
  } else {
    for (i=0; i<h; ++i) {
      const double* a1 = A + i*al;
      double* c1 = C + i*cl;
      double bb = dst[i];
      for (j=0; j<w; ++j) {
        c1[j] = a1[j] - bb;
      }
    }
  }
  return 0;
}

#define REPEAT4(n, op)		\
  switch (n) {		\
    default: { int i; for (i = n-4; i > 0; i--) { op; } } \
    case 4:  op;		\
    case 3:  op;		\
    case 2:  op;		\
    case 1:  op;		\
    case 0:  ;			\
}

static void hor_acc8(uchar* cp0, int cc, int stride)
{
	char* cp = (char*) cp0;
	if (cc > stride) {
		cc -= stride;
		/*
		 * Pipeline the most common cases.
		 */
		if (stride == 3)  {
			unsigned int cr = cp[0];
			unsigned int cg = cp[1];
			unsigned int cb = cp[2];
			do {
				cc -= 3, cp += 3;
				cp[0] = (char) (cr += cp[0]);
				cp[1] = (char) (cg += cp[1]);
				cp[2] = (char) (cb += cp[2]);
			} while ((int32) cc > 0);
		} else if (stride == 4)  {
			unsigned int cr = cp[0];
			unsigned int cg = cp[1];
			unsigned int cb = cp[2];
			unsigned int ca = cp[3];
			do {
				cc -= 4, cp += 4;
				cp[0] = (char) (cr += cp[0]);
				cp[1] = (char) (cg += cp[1]);
				cp[2] = (char) (cb += cp[2]);
				cp[3] = (char) (ca += cp[3]);
			} while ((int32) cc > 0);
		} else  {
			do {
				REPEAT4(stride, cp[stride] =
					(char) (cp[stride] + *cp); cp++)
				cc -= stride;
			} while ((int32) cc > 0);
		}
	}
}

#undef REPEAT4

static int imsplit(const img_t* im, img_t* im2, int off, int c) {
  int i, j;
  imsetsize(im2, im->h, im->w, c, im->f);
  switch (c) {
#define IMSPLIT_CASE(_TYPE) \
  case sizeof(_TYPE): \
    for (i=0; i<im->h*im->f; ++i) { \
      const uchar* src = im->tt.data + i * im->s + off; \
      _TYPE* dst = (_TYPE*)(im2->tt.data + i * im2->s); \
      for (j=0; j<im->w; ++j, src += im->c) { \
        dst[j] = *((const _TYPE*)src); \
      } \
    } break
    IMSPLIT_CASE(uchar);
    IMSPLIT_CASE(ushort);
    IMSPLIT_CASE(uint32);
    IMSPLIT_CASE(uint64);
  default:
    for (i=0; i<im->h*im->f; ++i) {
      const uchar* src = im->tt.data + i * im->s + off;
      uchar* dst = im2->tt.data + i * im2->s;
      memcpy2d(dst, c, src, im->c, im->w, c);
    }
    break;
  }
  return 0;
}

//#include "imglib.inl"
//#include "enhance.inl"

#if 0
static int imhisteq(const img_t* im, img_t* im2) {
  if (im!=im2) {
    imclone2(im, im2);
  }
  HistogramEqualize(im2->h, im2->w, im2->tt.data, im2->s);
  return 0;
}

#include "filter2d.inl"

static int imgauss(const img_t* im, img_t* im2, double delta) {
  //imsetsize();
  //SKIP_FILTER_RC( h, w, xx0, xxl, 1, xx, w, 1, 3, 3, zz, GAUSS_I7X7, GAUSS_I7X7 );
  img_t im1[1] _INIT0;
  if (im==im2) {
    imclone2(im, im1);
    im = im1;
  }
  imsetsize(im2, im->h, im->w, im->c, 1);
  filter_guass(im->h, im->w, im->tt.data, im->s, im->c, im2->tt.data, im2->s, im2->c, delta);
  imfree(im1);
  return 0;
}
#endif

//#include "img/imgopt.inl"


static int pic2ascii(int h, int w, const uchar* img, int imgstep, int cn, char* out) {
  static const char* pic2ascii_chs = "   ...',;:clodxkO0KXNWMMMM";
  int x, y, k=cn/2;
  static char pic2ascii_map[256] _INIT0;
  static size_t pic2ascii_len = 0;
  char* out1 = out;
  if (0==pic2ascii_len) {
    pic2ascii_len = strlen(pic2ascii_chs);
    for (x=0; x<256; ++x) {
      pic2ascii_map[x] = pic2ascii_chs[x*pic2ascii_len/256];
    }
  }
  for (y = 0; y < h; y++) {
    const uchar* img1 = img + y*imgstep;
    for (x = 0; x < w; x++, img1+=cn) {
      int gry = img1[k];
      *out1++ = pic2ascii_map[gry];
    }
    *out1++  = '\n';
  }
  *out1  = 0;
  return out1-out;
}

#if 0
//#include "str.h"
static int img2str(const img_t* im, int maxw, str_t* s) {
  img_t im1[1] _INIT0;
  if (1) {
    int w = MIN(maxw, im->w);
    int h = im->h*w/(im->w*2);
    imresize(im, h, maxw, im1);
    im = im1;
  }
  str_setsize(s, im->h*(im->w+4));
  s->l = pic2ascii(im->h, im->w, im->tt.data, im->s, im->c, s->s);
  imfree(im1);
  return s->l;
}
static int imgprint(const img_t* im) {
  str_t s[1] _INIT0;
  img2str(im, 79, s);
  puts(s->s);
  str_free(s);
  return 0;
}
#endif

#endif // _IMOPT_INL_

