
#ifndef _IMG_C_H_
#define _IMG_C_H_

#include "types_c.h"
#include "mem_c.h"

typedef struct PaletteEntry {
  uchar b, g, r, a;
}
PaletteEntry;
////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IMARGDEFTT int h=im->h, w=im->w, step=im->s, cn=im->c; uchar* img=im->tt.data
#define PIXBLEND(_d, _s, _a)    (_d += ((((_s)-(_d))*(_a))>>8))
#define PIXBLEND4(_d, _s, _a)   (PIXBLEND(_d[0], _s[0], _a), PIXBLEND(_d[1], _s[1], _a), PIXBLEND(_d[2], _s[2], _a), PIXBLEND(_d[3], _s[3], _a))
#define PIXCOPY(dst, src, cn)  { int i=0; for (; i<cn; ++i) { dst[i] = src[i]; } }
//#define CLR2GRY(C)    ((GetRV(C)+(GetGV(C)<<1)+GetBV(C))>>2)
#define CLR2GRY(C)    (((C)[0]+((C)[1]<<1)+(C)[2])>>2)
//#define showDDC(NAME, hDC) cvShowImage(NAME, hDC->h, hDC->w, (uchar*)(hDC->data), hDC->bw, 4)
#define HDDCSET_IMAGE(hDC, im)  ((hDC)->h = (im)->h, (hDC)->w = (im)->w, (hDC)->bw = (im)->s, (hDC)->data = (DWORD*)((im)->tt.data))
#define HDDCSET_RECT(hDC, hDCS, rc)  ((hDC)->h = MIN(RCH(rc), (hDCS)->h-((rc)->top)), (hDC)->w = MIN(RCW(rc), (hDCS)->w-((rc)->left)), (hDC)->bw = (hDCS)->bw, (hDC)->data = (hDCS)->data+(rc)->left+(rc)->top*((hDCS)->bw>>2))
#define HDDCSET_CLIP(hDC, prc)   (hDC->clip = ClipRect(hDC, prc))
#define HDDCGET_CLIP(hDC, prc)  if ((hDC)->clip) {*(prc)=*(hDC)->clip;} else {RCSET(prc, 0, 0, (hDC)->w, (hDC)->h);}
#define HDDC_BEGIN(hDC, prc)  {const RECT* __prc=hDC->clip; RECT __rc=*prc; hDC->clip = ClipRect(hDC, &__rc);
#define HDDC_END(hDC)    hDC->clip=__prc;}
// ???????
//#define HDDC_BEGIN2(hDC, prc)  {TDrawDC __hDC[1]; __hDC[0]=*(hDC); HDDCSET_RECT(hDC, __hDC, prc); (hDC)->clip=0;
//#define HDDC_END2(hDC)    *(hDC)=__hDC[0];}
#define INITDC(_DC, H, W, BW, _DATA)   ((_DC)->h=H, (_DC)->w=W, (_DC)->bw=BW, (_DC)->data=(DWORD*)(_DATA))
#define _Pixel(hDC, x1, y1)           ((unsigned long*)hDC->tt.data)[(int)(x1)+(int)(y1)*(hDC->s>>2)]
#define _PixelT(hDC, x1, y1, T)           (*((T*)(((char*)hDC->tt.data)+(int)(x1)*sizeof(T)+(int)(y1)*(hDC->bw))))
#define _GetPixel(hDC, x1, y1)          (((x1)>=0&&(x1)<hDC->w && (y1)>=0&&(y1)<hDC->h) ? _Pixel(hDC, (x1), (y1)):0)
#define _SetPixel(hDC, x1, y1, clr)     (((x1)>=0&&(x1)<hDC->w && (y1)>=0&&(y1)<hDC->h) ? (memcpy(hDC->tt.data + (int)(y1)*hDC->s+(int)(x1)*hDC->c, &clr, hDC->c)) : 0)
#define Fill1D(_W, _A, _V)  {int _j=0; for (_j=0;_j<_W;++_j) {*(_A+_j)=_V;}}
#define GRADIENT(_N, _I1, _V1, _V2)    (((_I1)*(_V1)+((_N)-(_I1))*(_V2))/(_N))
#define GRADIENTRGB(_N, _I1, _V1, _V2)    _RGB(GRADIENT(_N, _I1, GetRV(_V1), GetRV(_V2)), GRADIENT(_N, _I1, GetGV(_V1), GetGV(_V2)), GRADIENT(_N, _I1, GetBV(_V1), GetBV(_V2)))
#define BLEN(_A, _B, alpha)       (_A + (int)((((_B) - (_A)) * alpha) >> 8))
#define BLEN2(_A, _B, alpha, _N)  (_A + (int)((((_B) - (_A)) * alpha)/(_N) ))
#define RGBBLENSET(_A, _B)  _A = RGBBLEN(_A, _B, GetAV(_B))
#define GETRGBA(C, R, G, B, A)   (R=GetRV(C), G=GetGV(C), B=GetBV(C), A=GetAV(C))
#define SETRGBA(_P, R, G, B, A)   ((_P)[0]=B, (_P)[1]=G, (_P)[2]=R, (_P)[3]=A)
//#define SETRGB(_P, R, G, B)   ((_P)[0]=B, (_P)[1]=G, (_P)[2]=R)
#define SETGRY(_P, R, G, B)   ((_P)[0]=((B)+((G)<<1)+(R))>>2)
#define SETBLENP(_A, _B, _P, _C)  ((_A)[_P]=BLEN((_A)[_P], (_B)[_P], _C))
#define SETBLEN1(_A, _B, _C)  SETBLENP(_A, _B, 0, _C)
#define SETBLEN3(_A, _B, _C)  (SETBLENP(_A, _B, 0, _C), SETBLENP(_A, _B, 1, _C), SETBLENP(_A, _B, 2, _C))
#define SETBLEN4(_A, _B, _C)  (SETBLENP(_A, _B, 0, _C), SETBLENP(_A, _B, 1, _C), SETBLENP(_A, _B, 2, _C), SETBLENP(_A, _B, 3, _C))
#define RGBBLEN(_A, _B, _C)  _RGBA(BLEN(GetRV(_A), GetRV(_B), _C), BLEN(GetGV(_A), GetGV(_B), _C), BLEN(GetBV(_A), GetBV(_B), _C), 255)
#define RGBBLEN2(_A, _B, _C, _N)  _RGBA(BLEN2(GetRV(_A), GetRV(_B), _C, _N), BLEN2(GetGV(_A), GetGV(_B), _C, _N), BLEN2(GetBV(_A), GetBV(_B), _C, _N), 255)
#define RGBABLEN(_A, _B, _C)  _RGBA(BLEN(GetRV(_A), GetRV(_B), _C), BLEN(GetGV(_A), GetGV(_B), _C), BLEN(GetBV(_A), GetBV(_B), _C), BLEN(GetAV(_A), GetAV(_B), _C))
#define RGBABLEN2(_A, _B, _C, _N)  _RGBA(BLEN2(GetRV(_A), GetRV(_B), _C, _N), BLEN2(GetGV(_A), GetGV(_B), _C, _N), BLEN2(GetBV(_A), GetBV(_B), _C, _N), BLEN2(GetAV(_A), GetAV(_B), _C, _N))
#define MINRGB(R,G,B)  _RGB(MIN(R,255), MIN(G,255), MIN(B,255))
#define GRADIENTRGBSET(_V, _N, _I1, _V1, _V2)    {int _r=GRADIENT(_N, _I1, GetRV(_V1), GetRV(_V2)), _g=GRADIENT(_N, _I1, GetGV(_V1), GetGV(_V2)), _b=GRADIENT(_N, _I1, GetBV(_V1), GetBV(_V2)); _V=MINRGB(_r,_g,_b); if (0) {printf("%08x, %08x, %08x, %d %d %d \n", _V1, _V2, _V, _r,_g,_b); } }
#define GRADIENTRGB_FILL1D1(_W, _A, _V1, _V2)  {int _j=0; for (_j=0;_j<_W;++_j) {*(_A+_j)=GRADIENTRGB(_W, _j, _V1, _V2);}}
#define GRADIENTRGB_FILL1D(_W, _A, _V1, _V2)  {int _j=0; for (_j=0;_j<_W;++_j) {GRADIENTRGBSET(*(_A+_j), _W, _j, _V1, _V2);}}
#define GRADIENTRGB_FILL1DZB(_W, _A, _V1, _V2, _Z, _Z1, _Z2)  {int _j=0; float _z1=_Z1,dz=(_Z2-_Z1)/_W;for (_j=0;_j<_W;++_j,_z1+=dz) {if (*(_Z+_j)>_z1) {*(_Z+_j)=_z1;GRADIENTRGBSET(*(_A+_j), _W, _j, _V1, _V2);}}}
#define GRADIENTRGB_COPY1DZB(_W, _A, _V1, _V2, _Z, _Z1, _Z2)  {int _j=0; float _z1=_Z1,dz=(_Z2-_Z1)/_W;for (_j=0;_j<_W;++_j,_z1+=dz) {if (*(_Z+_j)>_z1) {*(_Z+_j)=_z1;GRADIENTRGBSET(*(_A+_j), _W, _j, _V1, _V2);}}}
#define Fill2D(_H, _W, _A, _AL, _V)  {int _i=0,_j,_ia; for (;_i<_H;++_i) {for (_j=0,_ia=_i*(_AL);_j<_W;++_j,++_ia) {*(_A+_ia)=_V;}}}
#define RCWIDTH(_PRC)   ((_PRC)->right-(_PRC)->left)
#define RCHEIGHT(_PRC)   ((_PRC)->bottom-(_PRC)->top)
#define RCX2(_PRC)   ((_PRC)->right+(_PRC)->left)
#define RCY2(_PRC)   ((_PRC)->bottom+(_PRC)->top)
#define RCOFFSET(_PRC, _L, _T, _R, _B)   ((_PRC)->l+=_L, (_PRC)->t+=_T, (_PRC)->r+=_R, (_PRC)->b+=_B)
//#define RCCENTER(_PRC)  cPOINT(((_PRC)->left+(_PRC)->right)/2, ((_PRC)->top+(_PRC)->bottom)/2)
#define DRAWHLINE(hDC, X1, X2, Y1, _V)  if (Y1>=0 && Y1<hDC->h) {int _X1=MAX(MIN(X1, X2), 0), _X2=MIN(MAX(X1, X2), hDC->w), _W=_X2-_X1; DWORD* _A=&_Pixel(hDC, _X1, Y1); Fill1D(_W, _A, _V);}
#define DRAWHLINE_GRADIENT(hDC, X1, X2, Y1, V1, V2)  if (Y1<hDC->h && Y1>=0) {int _X1=MAX(MIN(X1, X2), 0), _X2=MIN(MAX(X1, X2), hDC->w), _W=1+_X2-_X1; DWORD* _A=&_Pixel(hDC, _X1, Y1), _V1=(X1==_X1)?V1:V2, _V2=(X1==_X1)?V2:V1; GRADIENTRGB_FILL1D(_W, _A, _V1, _V2);}
#define DRAWHLINE_GRADIENTZB(hDC, hZB, X1, X2, Y1, V1, V2, _Z1,_Z2)  if (Y1<hDC->h && Y1>=0) {int _X1=MAX(MIN(X1, X2), 0), _X2=MIN(MAX(X1, X2), hDC->w), _W=1+_X2-_X1; DWORD* _A=&_Pixel(hDC, _X1, Y1), _V1=(X1==_X1)?V1:V2, _V2=(X1==_X1)?V2:V1; float* _Z=&_PixelT(hZB, _X1, Y1, float); GRADIENTRGB_FILL1DZB(_W, _A, _V2, _V1, _Z, _Z1, _Z2);}
#define ALPHAVALUE(_V1, _V2, _AL)       (((_V1)*(256-_AL)+(_V2)*(_AL))>>8)
#define ALPHACOLOR(_COL1, _COL2, _AL)  _RGB(ALPHAVALUE(GetRV(_COL1), GetRV(_COL2), _AL), ALPHAVALUE(GetGV(_COL1), GetGV(_COL2), _AL), ALPHAVALUE(GetBV(_COL1), GetBV(_COL2), _AL))
//#define SWAPRB(_COL)  _RGB(GetBV(_COL), GetGV(_COL), GetRV(_COL))
//#define SWAPRB(_COL)  (_COL)
#define GRADIENT2(_B, _I1, _V1, _V2)    (((_I1)*(_V1)+((1<<(_B))-(_I1))*(_V2))>>(_B))
#define GRADIENTRGB2(_B, _I1, _V1, _V2)    _RGB(GRADIENT2(_B, _I1, GetRV(_V1), GetRV(_V2)), GRADIENT2(_B, _I1, GetGV(_V1), GetGV(_V2)), GRADIENT2(_B, _I1, GetBV(_V1), GetBV(_V2)))
#define F2FIXN(_F, N)    ((int)((_F)*(1<<(N))))
#define FIXN2I(_F, N)    ((int)(((int64)_F)>>(N)))
#define FIXNMUL(A, B, N)    ((int)((((int64)(A))*(B))>>(N)))
#define FIXNDIV(A, B, N)    ((int)((((int64)(A))<<N)/(B)))
#define FIXNMULE(A, B, N)    (A) = FIXNMUL(A, B, N)
#define FIXNDIVE(A, B, N)    (A) = FIXNDIV(A, B, N)
#define F2FIX(_F)    ((int)((_F)*(1<<16)))
#define I2FIX(_F)    ((int)(((_F)<<16)))
#define FIX2I(_F)    ((int)((_F)>>16))
#define FIXMASK      ((1<<16)-1)
#define FIX2R(_F)    ((int)((_F)&FIXMASK))
#define F2FIX8(_F)   ((int)((_F)*(1<<8)))
#define FIX8MUL(A, B)   ((int)(((A)*(B))>>8))
#define I2FIX8(_F)   ((int)(((_F)<<8)))
#define FIX82I(_F)   ((int)((_F)>>8))
#define FIXMASK8     ((1<<8)-1)
#define FIX2R8(_F)   ((int)((_F)&FIXMASK8))
//#define ODS_OVER   0x0800
#define TDrawLine(hDC, _x1, _y1, _x2, _y2, _clr)   imdrawaa_line(hDC, 0, _x1, _y1, _x2, _y2, _clr, 1)
#define TMoveTo(hDC, _x, _y)        (_x0=(_x), _y0=(_y))
#define TLineTo(hDC, _x, _y, _clr)  (TDrawLine(hDC, (_x0), (_y0), (_x), (_y), _clr), TMoveTo(hDC, _x, _y))
#define _LineTo(hDC, _x, _y)        (TDrawLine(hDC, (_x0), (_y0), (_x), (_y), _clr), TMoveTo(hDC, _x, _y))
#define TMoveToPt(hDC, _pt)        TMoveTo(hDC, (_pt).x, (_pt).y)
#define _LineToPt(hDC, _pt)        _LineTo(hDC, (_pt).x, (_pt).y)
#ifndef ROUND
#define ROUNDL(x)  ((long)((x)+0.5))
#define ROUND(x)   ((int)((x)+0.5))
#endif
#define alpha_composite1(composite, fg, alpha, bg) { \
    unsigned short temp = ((unsigned short)(fg)*(unsigned short)(alpha) + \
        (unsigned short)(bg)*(unsigned short)(255 - (unsigned short)(alpha)) + (unsigned short)128); \
    (composite) = (uchar)((temp + (temp >> 8)) >> 8); \
  }


////////////////////////////////////////////////////
#define COLOR_TYPE_DEF_DEF(COLOR_TYPE_DEF) \
COLOR_TYPE_DEF(0, 1, GRAY) \
COLOR_TYPE_DEF(1, 2, YUV420P) \
  COLOR_TYPE_DEF(2, 2, YUV420) \
  COLOR_TYPE_DEF(3, 2, YUV411P) \
  COLOR_TYPE_DEF(4, 2, YUYV) \
  COLOR_TYPE_DEF(5, 2, UYVY) \
  COLOR_TYPE_DEF(6, 4, BGRA) \
  COLOR_TYPE_DEF(7, 1, BayerGR) \
  COLOR_TYPE_DEF(8, 1, BayerGB) \
  COLOR_TYPE_DEF(9, 1, BayerBG) \
COLOR_TYPE_DEF(10, 3, RGB) \
COLOR_TYPE_DEF(11, 3, BGR) \
COLOR_TYPE_DEF(12, 2, BGR555) \
COLOR_TYPE_DEF(13, 2, BGR565) \
COLOR_TYPE_DEF(14, 3, HSV) \
COLOR_TYPE_DEF(15, 3, XYZ) \
COLOR_TYPE_DEF(16, 3, HLS) \
COLOR_TYPE_DEF(17, 3, Lab) \
COLOR_TYPE_DEF(18, 3, Luv) \
COLOR_TYPE_DEF(19, 3, YCrCb) \
COLOR_TYPE_DEF(20, 2, YCrYCb) \
/* COLOR_TYPE_DEF(2, SBGGR8) */ \
COLOR_TYPE_DEF(21, 2, SN9C10X) \
COLOR_TYPE_DEF(22, 2, SGBRG) \
COLOR_TYPE_DEF(23, 12, HSV32F)
typedef enum //
{
#define COLOR_TYPE_DEF(a, b, c)  T_##c = a,
  COLOR_TYPE_DEF_DEF(COLOR_TYPE_DEF)
#undef COLOR_TYPE_DEF
  T_MaxColorSpace,
} ColorSpace;

////////////////////////////////////////////////////

#ifndef _SHIFT_MUL
enum { _SHIFT = 13, _SHIFT1 = (1 << _SHIFT), _SHIFT_MARK = (_SHIFT1 - 1) };
//#define _SHIFT                 (13)
//#define _SHIFT1                (1<<_SHIFT)
//#define _SHIFT_MARK            (_SHIFT1 - 1)
#define _SHIFT_MUL(x, y)       (((x)*(y))>>_SHIFT)
#define _SHIFT_DIV(x, y)       (((int)(x)<<_SHIFT)/(y))
#define _SHIFT_TOFLOAT(x)      (((double)(x))/_SHIFT1)
#define _SHIFT_TOINT(x)        (int)((x)*_SHIFT1)
#define _SHIFT_TOINT2(x, _S)   (int)((x)*(1<<(_S)))
#define _HALFSHIFT1            (1<<12)
#endif
// RGB2GRAY
#ifndef GREYSCALIZE
enum { _CR = (299 * (1 << _SHIFT) / 1000), _CG = (587 * (1 << _SHIFT) / 1000), _CB = (_SHIFT1 - _CR - _CG) };
//#define _CR                  (299*(1 << _SHIFT)/1000)
//#define _CG                  (587*(1 << _SHIFT)/1000)
//#define _CB                  (_SHIFT1 - _CR - _CG)
#define GREYSCALIZE(R, G, B)   ((_CR*(R) + _CG*(G) + _CB*(B) )>>_SHIFT)
#endif // GREYSCALIZE
#define BGR2GRAY(bgr, gry)   (*(gry) = (unsigned char)GREYSCALIZE(*(bgr+2), *(bgr+1), *(bgr)))
#define RGB2GRAY(bgr, gry)   (*(gry) = (unsigned char)GREYSCALIZE(*(bgr), *(bgr+1), *(bgr+2)))
#define BGRA2GRAY(bgr, gry)  BGR2GRAY(bgr, gry)
#define BGRA2BGR(bgra, bgr)  do { *(bgr) = *(bgra); *(bgr+1) = *(bgra+1); *(bgr+2) = *(bgra+2); } while(0)
#define RGBA2BGR(bgra, bgr)  do { *(bgr) = *(bgra+2); *(bgr+1) = *(bgra+1); *(bgr+2) = *(bgra); } while(0)
#define RGBA2RGB(bgra, bgr)  do { *(bgr) = *(bgra); *(bgr+1) = *(bgra+1); *(bgr+2) = *(bgra+2); } while(0)
#define RGB2BGR(rgb, bgr)    do { *(bgr) = *(rgb+2); *(bgr+1) = *(rgb+1); *(bgr+2) = *(rgb); } while(0)
#define BGR2RGB(bgr, rgb)    RGB2BGR(bgr, rgb)
#define GRAY2RGBx(gry, rgb)  do { *(rgb) = *gry; *(rgb+1) = *gry; *(rgb+2) = *gry; } while(0)
#define GRAYRANGE(P)       ((P > 255) ? 255 : (P < 0) ? 0 : P)
#define PIXELRANGE(P)      ((P > 255) ? 255 : (P < 0) ? 0 : P)
#define DOT_MULT3_3(X, Y, Z, R, G, B)  ((X)*(R)+(Y)*(G)+(Z)*(B))
#define GetY_YCbCr(R, G, B)       (DOT_MULT3_3( 0.2990, 0.5870, 0.1140,R, G, B)    )
#define GetCb_YCbCr(R, G, B)      (DOT_MULT3_3(-0.1687,-0.3313, 0.5000,R, G, B)+128)
#define GetCr_YCbCr(R, G, B)      (DOT_MULT3_3( 0.5000,-0.4187,-0.0813,R, G, B)+128)
#define GetY_RGB(R, G, B)      ((19595*R + 38470*G + 7471 *B) >> 16)
#define GetI_RGB(R, G, B)      ((39059*R - 18021*G - 21036*B) >> 16)
#define GetQ_RGB(R, G, B)      ((13894*R - 34274*G - 20381*B) >> 16)
#define BICNT_GRAY  8
#define BICNT_RGB   24
#define BICNT_RGBA  32
#define BGR565(r,g,b)  ( (((b)>>3)<<11) | (((g)>>2)<<5) | ((r)>>3) )
#define BGR555(r,g,b)  ( (((b)>>3)<<10) | (((g)>>3)<<5) | ((r)>>3) )
#define BGR5552GRAY(bgr555, gry) \
  do { \
    *gry = (unsigned char)GREYSCALIZE( \
        ((((ushort*)bgr555)[0] >> 7) & 0xf8), \
        ((((ushort*)bgr555)[0] >> 2) & 0xf8), \
        ((((ushort*)bgr555)[0] << 3) & 0xf8)); \
  } while(0)
#define BGR5652GRAY(bgr565, gry) \
  do { \
    *gry = (unsigned char)GREYSCALIZE( \
        ((((ushort*)bgr565)[0] >> 8) & 0xf8), \
        ((((ushort*)bgr565)[0] >> 3) & 0xfc), \
        ((((ushort*)bgr565)[0] << 3) & 0xf8) ); \
  } while(0)
#define BGR5552BGR(bgr555, bgr) \
  do { \
    int t0 = (*((ushort*)bgr555) << 3) & 0xf8; \
    int t1 = (*((ushort*)bgr555) >> 2) & 0xf8; \
    int t2 = (*((ushort*)bgr555) >> 7) & 0xf8; \
    *(bgr+0) = (unsigned char)t0; \
    *(bgr+1) = (unsigned char)t1; \
    *(bgr+2) = (unsigned char)t2; \
  } while(0)
#define BGR5652BGR(bgr565, bgr) \
  do { \
    int t0 = (*((ushort*)bgr565) << 3) & 0xf8; \
    int t1 = (*((ushort*)bgr565) >> 3) & 0xfc; \
    int t2 = (*((ushort*)bgr565) >> 8) & 0xf8; \
    *(bgr+0) = (unsigned char)t0; \
    *(bgr+1) = (unsigned char)t1; \
    *(bgr+2) = (unsigned char)t2; \
  } while(0)
// 4 => 3
#define CMYK2BGR(cmyk, bgr) \
  do { \
    int c = *(cmyk), m = *(cmyk+1), y = *(cmyk+2), k = *(cmyk+3); \
    c = k - ((255 - c)*k>>8); \
    m = k - ((255 - m)*k>>8); \
    y = k - ((255 - y)*k>>8); \
    *(bgr+2) = (unsigned char)c; \
    *(bgr+1) = (unsigned char)m; \
    *(bgr+0) = (unsigned char)y; \
  } while(0)
// <4, 1>
#define CMYK2GRAY(cmyk, gry) \
  do { \
    int c = *(cmyk), m = *(cmyk+1), y = *(cmyk+2), k = *(cmyk+3); \
    c = k - ((255 - c)*k>>8); \
    m = k - ((255 - m)*k>>8); \
    y = k - ((255 - y)*k>>8); \
    *(gry) = (unsigned char)GREYSCALIZE(c, m, y); \
  } while(0)
// ??????
#define RGB2YIQ_SKIN_T(in, out) \
  do { \
    int Y, I, Q; \
    Y = GetY_RGB((in+2), *(in+1), *(in)); \
    I = GetI_RGB((in+2), *(in+1), *(in)); \
    Q = GetQ_RGB((in+2), *(in+1), *(in)); \
    *(out) = (((Y < 235) && (I > 12) && (pow(I / 58, 2) + pow((Y - 142) / 104, 2)) < 1.) ? (0) : (255));\
  } while(0)
// ????????
#define SKINLIKEHOOD_T(in, out) \
  do { \
    static const double B_Mean = 117.4361, R_Mean = 156.5599; \
    static const double Brcov[4]  = { 160.1301, 12.1430, 12.1430, 299.4574 }; \
    static const double bb        = 2 * (Brcov1 * Brcov2 - Brcov0 * Brcov3); \
    double x1, x2, t; \
    x1 = GetCb_YCbCr(*(in+2), *(in+1), *(in)) - B_Mean; \
    x2 = GetCr_YCbCr(*(in+2), *(in+1), *(in)) - R_Mean; \
    t = x1 * (x1 * Brcov3 - x2 * Brcov2) + x2 * (-x1 * Brcov1 + x2 * Brcov0); \
    t = exp(t / bb) * 255; \
    *out = t;/*>80 ? 255 : 0;*/ \
  } while(0)
#define UNDEFINEDCOLOR 0
/*
r,g,b values are from 0 to 1
h = [0,360], s = [0,1], v = [0,1]
if s == 0, then h = -1 (undefined)
*/
#define RGB2HSV(in, out) \
  do { \
    float R, G, B; \
    float H, S, V; \
    float min, max, delta, tmp; \
    B = *(in  ); \
    G = *(in+1); \
    R = *(in+2); \
    tmp = MIN(R, G); \
    min = MIN(tmp, B); \
    tmp = MAX(R, G); \
    max = MAX(tmp, B); \
    V = max; \
    delta = max - min; \
    if( max != 0 ) { \
      S = delta / max; \
    } else { \
      /* r = g = b = 0 s = 0, v is undefined */ \
      S = 0; \
      H = UNDEFINEDCOLOR; \
      break; \
    } \
    if( R == max ) \
      H = ( G - B ) / delta; /* between yellow & magenta */ \
    else if( G == max ) \
      H = 2 + ( B - R ) / delta; /* between cyan & yellow */ \
    else \
      H = 4 + ( R - G ) / delta; /* between magenta & cyan */ \
    \
    H *= 60; /* degrees */ \
    if( H < 0 ) { \
      H += 360; \
    } \
    *(out  ) = (unsigned char)H; \
    *(out+1) = (unsigned char)(S*255); \
    *(out+2) = (unsigned char)V; \
  } while(0)
#if 0
// HSV??????????RGB?????????
void Hsv2Rgb(float H, float S, float V, float& R, float& G, float& B)
{
  int i;
  float f, p, q, t;
  if (S == 0) {
    // achromatic (grey)
    R = G = B = V;
    return;
  }
  H /= 60; // sector 0 to 5
  i = floor(H);
  f = H - i; // factorial part of h
  p = V * (1 - S);
  q = V * (1 - S * f);
  t = V * (1 - S * (1 - f));
  switch (i) {
  case 0:
    R = V;
    G = t;
    B = p;
    break;
  case 1:
    R = q;
    G = V;
    B = p;
    break;
  case 2:
    R = p;
    G = V;
    B = t;
    break;
  case 3:
    R = p;
    G = q;
    B = V;
    break;
  case 4:
    R = t;
    G = p;
    B = V;
    break;
  default:    // case 5:
    R = V;
    G = p;
    B = q;
    break;
  }
}
#endif
#define IMTRANS1D(aw, A, ai, B, bi, FUNC) \
  do { \
    int j, As=0, Bs=0; \
    for (j=0; j<aw; ++j, As+=ai, Bs+=bi) { \
      FUNC((A+As), (B+Bs)); \
    } \
  } while(0)
#define IMTRANS(ah, aw, A, al, ai, B, bl, bi, FUNC) \
  do { \
    int i, j; \
    int As=0, Bs=0; \
    int _al=(al)-aw*(ai); \
    int _bl=(bl)-aw*(bi); \
    for (i=0; i<ah; ++i, As+=_al, Bs+=_bl) { \
      for (j=0; j<aw; ++j, As+=ai, Bs+=bi) { \
        FUNC((A+As), (B+Bs)); \
      } \
    } \
  } while(0)
#define IM2IM(h, w, A, al, ai, B, bl, bi) \
  do { \
    if (ai==bi) { \
      SKIP_COPY(h, w*ai, A, al, 1, B, bl, 1); \
    } else { \
      int t = ai*10+bi; \
      switch (t) { \
      case 31: { IMTRANS(h, w, A, al, 3, B, bl, 1,  BGR2GRAY ); break; } \
      case 13: { IMTRANS(h, w, A, al, ai, B, bl, 3, GRAY2RGBx); break; } \
      case 23: { IMTRANS(h, w, A, al, ai, B, bl, 3, GRAY2RGBx); break; } \
      case 14: { IMTRANS(h, w, A, al, ai, B, bl, 4, GRAY2RGBx); break; } \
      case 43: { IMTRANS(h, w, A, al, ai, B, bl, 3, RGBA2RGB); break; } \
      } /* switch */ \
    } \
  } while(0)
////////////////////////////////////////////////////
#define CC_DEPTH_MAX  (1 << CC_CN_SHIFT)
#define CC_MAT_DEPTH_MASK       (CC_DEPTH_MAX - 1)
//#define CC_MAKE_TYPE CC_MAKETYPE
#define CC_AUTO_STEP  0x7fffffff
#define CC_WHOLE_ARR  cvSlice( 0, 0x3fffffff )
#define CC_MAT_NEEDED(mat)        (1)
#define CC_MAKETYPECN(depth,cn)   ((TypeCnId)_MAKETYPECN(depth,cn))
#define CC_MAKETYPE CC_MAKETYPECN
#define CC_MAT_TYPECN(mat)        CC_MAKETYPECN(mat->tid, CC_MAT_CN(mat))
#define _MAT_CN(c, t)             (c)/CC_TYPE_SIZE(t)
#if 0
#define CC_MAT_TYPE(mat)          CC_MAT_TYPECN(mat)
#define CC_MAT_DEPTH(mat)         (mat)->tid
#define CC_MAT_CN(im)             ((im)->c)/CC_TYPE_SIZE((im->tid))
#else
#define CC_MAT_TYPE(type)          (type)
#define CC_MAT_DEPTH(type)         CC_TYPECN_TYPE(type)
#define CC_MAT_CN(type)            CC_TYPECN_CN(type)
#define CC_ELEM_SIZE(type)         (CC_TYPECN_CN(type) * CC_TYPE_SIZE(CC_TYPECN_TYPE(type)))
#endif
#define CC_MAT_TOTAL(mat)         ((mat)->rows * (mat)->cols)
#define CC_MAT_ELEM( mat, elemtype, row, col ) *img_at(elemtype, mat, row, col)
#define CC_MAT_CONT_FLAG        (0xff)
#define CC_IS_MAT_CONT(im)    ((im)->w*(im)->c==(im)->s)
#define CC_IS_CONT_MAT          CC_IS_MAT_CONT
#define CC_MAT_TEMP_FLAG_SHIFT  15
#define CC_MAT_TEMP_FLAG        (1 << CC_MAT_TEMP_FLAG_SHIFT)
#define CC_IS_TEMP_MAT(flags)   ((flags) & CC_MAT_TEMP_FLAG)
#define CC_IS_MASK_ARR(mat)     ( (mat)->c ==1 )
#define CC_ARE_TYPES_EQ(mat1, mat2)   ( (mat1)->tid == (mat2)->tid && (mat1)->c == (mat2)->c )
#define CC_ARE_DEPTHS_EQ(mat1, mat2)  ( (mat1)->tid == (mat2)->tid )
#define CC_ARE_CNS_EQ(mat1, mat2)     ( CC_MAT_CN(mat1) == CC_MAT_CN(mat2) )
#define CC_ARE_SIZES_EQ(mat1, mat2)   ( (mat1)->rows == (mat2)->rows && (mat1)->cols == (mat2)->cols )
#define CC_ARE_SIZES_EQSZ(mat1, _rows, _cols)   ( (mat1)->rows == _rows && (mat1)->cols == _cols )
#define CC_ARE_SIZES_EQSZ1(mat1, _cols, _rows)   ( (mat1)->rows == _rows && (mat1)->cols == _cols )
#define CC_ARE_SIZES_EQSZ2(mat1, _rows, _cols)   ( CC_ARE_SIZES_EQSZ(mat1, _rows, _cols) || CC_ARE_SIZES_EQSZ(mat1, _cols, _rows) )
#define CC_IS_MAT_CONST(mat)   (((mat)->rows|(mat)->cols) == 1)
/* Size of each channel item,
0x124489 = 1000 0100 0100 0010 0010 0001 0001 ~ array of sizeof(arr_type_elem) */
//#define CC_ELEM_SIZE1(mat)   cvTypeSize(mat->tid)
#define CC_ELEMTYPE_SIZE(mat)   cvTypeSize(mat->tid)
//#define CC_ELEM_SIZE_(mat)      mat->c
//#define CC_ELEM_SIZE(type)   (CC_ELEM_SIZE1(mat)*(mat)->c)
/* 0x3a50 = 11 10 10 01 01 00 00 ~ array of log2(sizeof(arr_type_elem)) */
#define CC_MAT_ELEM_PTR( mat, row, col )   CC_MAT_ELEM_PTR_FAST( mat, row, col, CC_ELEM_SIZE((mat).type) )
//#define CC_MAT_ELEM( mat, elemtype, row, col )   (*(elemtype*)CC_MAT_ELEM_PTR_FAST( mat, row, col, sizeof(elemtype)))
//         Multi-dimensional dense array (CMatND)                          *
#define CC_MATND_MAGIC_VAL    0x42430000
#define CC_TYPE_NAME_MATND    "opencv-nd-matrix"
#define CC_MAX_DIM            32
#define CC_MAX_DIM_HEAP       (1 << 16)
#define CC_IS_MATND_HDR(mat) \
  ((mat) != NULL && (((const CMatND*)(mat))->type & CC_MAGIC_MASK) == CC_MATND_MAGIC_VAL)
//#define CC_IS_MATND(mat)   (CC_IS_MATND_HDR(mat) && ((const CMatND*)(mat))->tt.data.ptr != NULL)
//        Multi-dimensional sparse array (CSparseMat)                      *
#define CC_SPARSE_MAT_MAGIC_VAL    0x42440000
#define CC_TYPE_NAME_SPARSE_MAT    "opencv-sparse-matrix"
#define CC_IS_SPARSE_MAT_HDR(mat) \
  ((mat) != NULL && \
      (((const CSparseMat*)(mat))->type & CC_MAGIC_MASK) == CC_SPARSE_MAT_MAGIC_VAL)
////
#define imtype(im)  CC_MAT_TYPECN(im)
#define imtid(im)  CC_MAT_DEPTH(im)
#define imdepth(im)  CC_MAT_DEPTH(im)
#define imzero(im)  memset((im)->tt.data, 0, (im)->h*(im)->s)
#define imsetsamesize(im, im2)  imsetsize(im, (im2)->h, (im2)->w, (im2)->c, (im2)->f)
#define imsetsize_i4(im, h, w, c, f)  imsetsize(im, h, w, c*sizeof(int), f)
#define imsetsize_f8(im, h, w, c, f)  imsetsize(im, h, w, c*sizeof(double), f)
#define imsetsize_f4(im, h, w, c, f)  imsetsize(im, h, w, c*sizeof(float), f)
#define bf_imsetsamesize(bf, im, im2)  bf_imsetsize(bf, im, (im2)->h, (im2)->w, (im2)->c, (im2)->f)
//
#define IMARG(im)  (im)->h, (im)->w, (im)->tt.data, (im)->s, (im)->c
#define imcopysize( im, im2 ) imsetsize(im, (im2)->h, (im2)->w, (im2)->c, (im2)->f)
#define imref(_IM, _X, _Y, _W, _H, _IM2)  imsubref(_IM, iRECT(_X, _Y, (_X)+(_W), (_Y)+(_H)), _IM2)
#define imrefrc(_IM, RC, _IM2)  imsubref(_IM, iRECT((RC).x, (RC).y, (RC).x+(RC).w, (RC).y+(RC).h), _IM2)
///
#define IMINIT(IM, H, W, A, S, C, F)  ((IM)->h=H,(IM)->w=W,(IM)->tt.data=(uchar*)(A),(IM)->s=S,(IM)->c=(short)(C),(IM)->f=F)
#define img_ptr(type, img)            ((type*)(img)->tt.data)
#define img_end(type, img)            ((type*)(img)->tt.data+(img)->h*(img)->s)
#define img_row(type, img, row)       ((type*)((img)->tt.data + (row)*(img)->s))
#define img_at(type, img, row, col)   ((type*)((img)->tt.data + (row)*(img)->s + (col)*(img)->c))

int getElemSize(int type) {
  TypeId t = CC_TYPECN_TYPE(type);
  int cn = CC_TYPECN_CN(type);
  int size = cn * CC_TYPE_SIZE(t);
  return size;
}
struct img_t;
CC_INLINE int imsetsize_(img_t* im, int height, int width, int channels, int frames, const char* file, int line);
static img_t* imcreate(img_t* im, ISize size, int type, const void* data, int step = -1);
static img_t* imconvert(const img_t* src, img_t* _dst, TypeId _type, double alpha, double beta);

struct img_t {
  //IMGHEADFEILD;
  union { int w;    int width; int cols; };
  union { int h;    int height; int rows; };
  union { int s;    int step; int widthStep; };
  union { int c;    int cn; int ch; };
  union { int f;    int frame; };
  union { TypeId t;    TypeId type_;  TypeId tid; };
  mem_t* mem;
#if 1
  union {
    union {
      uchar* data;
      uchar* ptr;
      short* s;
      int* i;
      float* fl;
      double* db;
#define TYPEDEF(a,b,c,d)  d* b;
      TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
    } tt;
    union {
      float* fl;
      double* db;
      int* i32;
#define TYPEDEF(a,b,c,d)  d* b;
      TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
    };
    uchar* data;
    uchar* imageData;
  };
#else
  uchar* data;
#endif
#ifdef __cplusplus
  img_t() { BZERO(this, 1); printf("img_t\n"); }
  ~img_t() { FREE(data); BZERO(this, 1); }
  int type() const {    return CC_MAKETYPECN(t, channels());  }
  TypeId depth() const { return t; }
  int channels() const { return _MAT_CN(this->c, this->t); }
  ISize size() const { return iSIZE(w, h); }
  img_t* create(ISize size, int type) { return imcreate(this, size, type, NULL, 0); }
  img_t* create(int rows, int cols, int type) { return imcreate(this, iSize(cols, rows), type, NULL, 0); }
  img_t& getMat() { return *this; }
  const img_t& getMat() const { return *this; }
  bool isSubmatrix() const { return false; }
  img_t* copyTo(img_t* _dst) const {    return imcreate(_dst, size(), type(), data, s);  }
  bool isContinuous() const { return w*c == s; }
  void locateROI(ISize& wsz, IPoint& ofs) const { wsz = size(); ofs = iPOINT(0, 0); }
  img_t* clone(img_t* _dst) const {    return copyTo(_dst);  }
  img_t* convertTo(img_t* _dst, TypeId _type, double alpha=1, double beta = 0) const {
    return imconvert(this, _dst, _type, alpha, beta);
  }
  size_t total() const { return h*w; }
  int checkVector(int _elemChannels, TypeId _depth= CC_NUL, bool _requireContinuous = true) const
  {
    int dims = 2;
    return data && (depth() == _depth || _depth <= 0) &&
      (isContinuous() || !_requireContinuous) &&
      ((dims == 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
      (cols == _elemChannels && channels() == 1)))
         //||(dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) && (isContinuous() || step.p[1] == step.p[2] * size.p[2]))
        )
      ? (int)(total()*channels() / _elemChannels) : -1;
  }

  //template <typename T> T* ptr() { return (T*)data; }
  //template <typename T> const T* ptr() const { return (const T*)data; }
  //template <typename T> T at(int row, int col) const { return *img_at(T, this, row, col); }
#endif
};

CC_INLINE int imfree(img_t* im)
{
  if (im) {
    if (im->tt.data && im->mem && im->mem->free_) {
      //fastFree(im->tt.data);
      im->mem->free_(im->tt.data);
    }
    memset(im, 0, sizeof(img_t));
  }
  return 0;
}
CC_INLINE int imsetsize(img_t* im, int height, int width, int channels, int frames, mem_t* mem = cpu_mem)
{
  if (NULL != im) {
    int step = width * channels;
    int n = height * step * frames;
    if (im->h == height && im->w == width && im->c == channels && im->f == frames && im->tt.data != NULL) {
    }
    else {
      int oldn = im->h * im->s * im->f;
      //int step = (width*channels+3) & ~3;
      if (n > oldn) {
        im->tt.data = (uchar*)mem_realloc(im->tt.data, n, mem, oldn, im->mem);
        //im->tt.data = (uchar*)fastRealloc(im->tt.data, n);
        im->mem = mem;
        ASSERT(im->tt.data != NULL && "");
      }
      IMINIT(im, height, width, im->tt.data, step, channels, frames);
      im->tid = CC_8U;
    }
    memset(im->tt.data, 0, n);
  }
  return 0;
}

img_t* imcreate(img_t* im, ISize size, int type, const void* data, int step) {
  int _cn = CC_TYPECN_CN(type);
  TypeId t = CC_TYPECN_TYPE(type);
  int c = _cn * CC_TYPE_SIZE(im->t);
  int s = c*size.h;
  if (data) {
    step = MAX(step, s);
    if (im->mem && im->mem->free_) { im->mem->free_(im->data); }
    IMINIT(im, size.height, size.width, data, step, c, 1);
  }
  else {
    imsetsize(im, size.height, size.width, c, 1);
  }
  im->t = t;
  return im;
}
img_t* imcreate2(img_t* im, int h, int w, int type, const void* data=0, int step=0) {
  return imcreate(im, iSize(w, h), type, data, step);
}

CC_INLINE img_t* imsubref(const img_t* im, IRECT rc, img_t* im2)
{
  *im2 = *im;
  rc = iRectInter(rc, iRECT(0, 0, im->w, im->h));
  im2->h = RCH(&rc);
  im2->w = RCW(&rc);
  im2->tt.data = im->tt.data + rc.t * im->s + rc.l * im->c;
  return im2;
}
CC_INLINE img_t imroi(const img_t* im, IRECT rc)
{
  img_t im2[2];
  *im2 = *im;
  rc = iRectInter(rc, iRECT(0, 0, im->w, im->h));
  im2->h = RCH(&rc);
  im2->w = RCW(&rc);
  im2->tt.data = im->tt.data + rc.t * im->s + rc.l * im->c;
  return *im2;
}

CC_INLINE int imzeros(img_t* im, int height, int width, int channels, int frames) {
  if (imsetsize(im, height, width, channels, frames)) {
    memset(im->tt.data, 0, height*width*channels*frames);
    return 1;
  }
  return 0;
}
CC_INLINE int imsetdata(img_t* im, int height, int width, int channels, int frames, void* data) {
  if (imsetsize(im, height, width, channels, frames)) {
    memcpy(im->tt.data, data, height*width*channels*frames);
    return 1;
  }
  return 0;
}
CC_INLINE int imgetframe(const img_t* im, int iframe, img_t* im2)
{
  int n = im->s * im->h;
  uchar* data = im->tt.data + n * iframe;
  IMINIT(im2, im->h, im->w, data, im->s, im->c, 1);
  return 0;
}
CC_INLINE int imsetframe(img_t* im, int iframe, img_t* im2)
{
  int n;
  if (0 == im->f || im->h != im2->h || im->w != im2->w || im->c != im2->c) {
    imsetsize(im, im2->h, im2->w, im2->c, iframe + 1);
  }
  ASSERT(im->h == im2->h && im->w == im2->w && im->c == im2->c);
  n = im->s * im->h;
  if (im->f <= iframe) {
    im->f = iframe + 1;
    REALLOC(uchar, im->tt.data, n * im->f);
  }
  memcpy(im->tt.data + n * iframe, im2->tt.data, n);
  return 0;
}
static img_t* imconvert(const img_t* src, img_t* _dst, TypeId _type, double alpha, double beta) {
  _dst->create(src->size(), CC_MAKETYPECN(_type, src->channels()));
  arrcvt2d(_dst->data, _type, _dst->s, src->data, src->t, src->s, src->h, src->w, alpha, beta);
  return _dst;
}

CC_INLINE int imcopy(const img_t* a, img_t* b)
{
  if (a != b) {
    memcpy2d(b->tt.data, b->s, a->tt.data, a->s, a->h, b->w * b->c);
    b->t = a->t;
    b->f = a->f;
  }
  return 0;
}
CC_INLINE int imflip_ud_copy(const img_t* a, img_t* b)
{
  if (a != b) {
    memflip(b->tt.data, b->s, a->tt.data, a->s, a->h, b->w * b->c);
  }
  return 0;
}
CC_INLINE int imcopy3(const img_t* a, int f, img_t* b)
{
  if (a != b) {
    if (a->f > 0) {
      f %= a->f;
      imsetsize(b, (a)->h, (a)->w, (a)->c, 1);
      memcpy2d(b->tt.data, b->s, a->tt.data + f * a->h * a->s, a->s, a->h, b->w * b->c);
    }
  }
  return 0;
}
CC_INLINE img_t* imclone2(const img_t* a, img_t* dst)
{
  if (a != dst) {
    imsetsamesize(dst, a);
    dst->tid = a->tid;
    imcopy(a, dst);
  }
  return dst;
}
CC_INLINE img_t imclone1(const img_t* a)
{
  img_t b[1];
  imclone2(a, b);
  return *b;
}
CC_INLINE int imsubcopy(const img_t* im, IRECT rc, img_t* im2)
{
  img_t im1[1];// = { 0 };
  imsubref(im, rc, im1);
  imclone2(im1, im2);
  return 0;
}
#define IM_DEF(im, n)  img_t _##im [n]={0}; img_t* im = _##im
#define IM_BEGIN(im, n) {IM_DEF(im, n)
#define IM_END(im)    ASSERT((im-_##im) <= countof(_##im)); freeims(_##im, countof(_##im)); }
#define imfrees(im, n) freeims(im, n)
#define imfrees2(im) freeims(im, countof(im))
CC_INLINE int freeims(img_t* im, int n)
{
  int i;
  for (i = 0; i < n; ++i) {
    imfree(im + i);
  }
  memset(im, 0, n * sizeof(img_t));
  return 0;
}
CC_INLINE int imswap(img_t* a, img_t* b)
{
  img_t t;
  CC_SWAP(*a, *b, t);
  return 0;
}
CC_INLINE int bf_imfree(buf_t* bf, img_t* im)
{
  if (im) {
    if (im->tt.data) {
      BFFREE(bf, im->tt.data);
    }
    memset(im, 0, sizeof(img_t));
  }
  return 0;
}
CC_INLINE int bf_imfrees(buf_t* bf, img_t* im, int n)
{
  int i;
  for (i = n - 1; i >= 0; --i) {
    bf_imfree(bf, im + i);
  }
  memset(im, 0, n * sizeof(img_t));
  return 0;
}
#endif // _IMG_C_H_
