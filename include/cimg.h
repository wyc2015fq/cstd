

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

#define IMG_DEPTH_SIGN 0x80000000
typedef enum {
 IMG_DEPTH_1U   =  1,
 IMG_DEPTH_8U   =  8,
 IMG_DEPTH_16U  = 16,
 IMG_DEPTH_32F  = 32,
 IMG_DEPTH_8S   = (IMG_DEPTH_SIGN| 8),
 IMG_DEPTH_16S  = (IMG_DEPTH_SIGN|16),
 IMG_DEPTH_32S  = (IMG_DEPTH_SIGN|32),
}IMG_DEPTH;
#define IMG_DATA_ORDER_PIXEL  0
#define IMG_DATA_ORDER_PLANE  1
#define IMG_ORIGIN_TL 0
#define IMG_ORIGIN_BL 1
#define IMG_ALIGN_4BYTES   4
#define IMG_ALIGN_8BYTES   8
#define IMG_ALIGN_16BYTES 16
#define IMG_ALIGN_32BYTES 32
#define IMG_ALIGN_DWORD   IMG_ALIGN_4BYTES
#define IMG_ALIGN_QWORD   IMG_ALIGN_8BYTES

#define IMG_IMAGE_HEADER 1
#define IMG_IMAGE_DATA   2
#define IMG_IMAGE_ROI    4
/* extra border mode */

//#define CC_TYPE_NAME_IMAGE "opencv-image"
//#define CC_IS_IMAGE_HDR(img)   ((img) != NULL && ((const IplImage*)(img))->nSize == sizeof(IplImage))
//#define CC_IS_IMAGE(img)   (CC_IS_IMAGE_HDR(img) && ((IplImage*)img)->imageData != NULL)
/* for storing double-precision
   floating point data in IplImage's */
#define IMG_DEPTH_64F  64
/* get reference to pixel at (col,row),
   for multi-channel images (col) should be multiplied by number of channels */
#define CC_IMAGE_ELEM( image, elemtype, row, col )        *img_at(elemtype, image, row, col)
#define COLOR_REV
//#undef RGB
//#define RGB(r,g,b)          ((COLORREF)(((BYTE)(b)|((uint16)((BYTE)(g))<<8))|(((uint32)(BYTE)(r))<<16)))
#ifdef COLOR_REV
#define _RGBA(r,g,b,a)         (((uint32)(((uint8)(b)|((uint16)((uint8)(g))<<8))|(((uint32)(uint8)(r))<<16)))|(( (uint32)(uint8)(a) )<<24))
#define _RGBA16(r,g,b,a)       (((uint64)(((uint16)(b)|((uint32)((uint16)(g))<<16))|(((uint64)(uint16)(r))<<32)))|(( (uint64)(uint16)(a) )<<48))
#else
#define _RGBA(r,g,b,a)         (((uint32)(((uint8)(r)|((uint16)((uint8)(g))<<8))|(((uint32)(uint8)(b))<<16)))|(( (uint32)(uint8)(a) )<<24))
#define _RGBA16(r,g,b,a)       (((uint64)(((uint16)(r)|((uint32)((uint16)(g))<<16))|(((uint64)(uint16)(b))<<32)))|(( (uint64)(uint16)(a) )<<48))
#endif
#define BOUND_RGBA(r,g,b,a)        _RGBA(BOUND(r, 0, 255), BOUND(g, 0, 255), BOUND(b, 0, 255), BOUND(a, 0, 255))
#define _RGB(r,g,b)         _RGBA(r,g,b,255)
#define _RGB16(r,g,b)         _RGBA16(r,g,b,255)
#define INT2RGB(i)          _RGBA(((i>>16)&0xff), ((i>>8)&0xff), ((i)&0xff), 255)
//#define HEX2INT1(H)   (('0'<=(H) && (H)<='9') ? (H)-'0' : ('a'<=(H) && (H)<='f') ? (H)-'a'+10 : ('A'<=(H) && (H)<='F') ? (H)-'A'+10 : 0)
//#define HEX2INT2(H)   ((HEX2INT1((H)[0])<<4)|(HEX2INT1((H)[1])))
//#define S_RGB(S)        _RGBA(HEX2INT2((S)),HEX2INT2((S)+2),HEX2INT2((S)+4),255)
//#define CC_RGB( r, g, b )  cvScalar( (b), (g), (r), 0 )
#define GetBV(rgb) ((BYTE)(rgb))
#define GetGV(rgb) ((BYTE)(((uint16)(rgb)) >> 8))
#define GetRV(rgb) ((BYTE)((rgb)>>16))
#define GetAV(rgb) ((BYTE)((rgb)>>24))
#define CC_RGB( r, g, b )  cScalar(b,g,r, 255)
#define _rgb(r,g,b)  _RGB(255*(r), 255*(g), 255*(b))
#define _rgba(r,g,b,a)  _RGBA(255*(r), 255*(g), 255*(b), 255*(a))
#define _RGB_A(clr, _a)   _RGBA(GetRV(clr),GetGV(clr),GetBV(clr),_a)
#define _rgb_a(clr, _a)   _RGBA(GetRV(clr),GetGV(clr),GetBV(clr),(_a)*255)
#define _A_RGB(_a, clr)    ((clr&0x00ffffff)|(_a<<24))
#define _ARGB(a,r,g,b)  _RGBA(r,g,b,a)
#define _SetAlpha _RGB_A
#define CVRGBDEF( r, g, b )  _RGB(r, g, b)
#define GetRGBV(rgb) ((rgb)&_RGBA(255,255,255,0))
#define SWAPRB(_COL)  _RGBA(GetBV(_COL), GetGV(_COL), GetRV(_COL), GetAV(_COL))
#define CVSCALARDATA(scalar, _I) (((unsigned char*)scalar)[_I])
#define RGBDEF _RGB
#define SCALARDATA(scalar, _I) (((unsigned char*)(scalar))[_I])
#define RGBMUL(rgb, _x)  CC_RGB(GetRV(rgb)*(_x), GetGV(rgb)*(_x), GetBV(rgb)*(_x))
#define SCALARTORAWDATA(_scalar, _data, _cn) {int _cn1 = _cn; while(_cn1--) { (_data)[_cn1] = CVSCALARDATA(_scalar, _cn1); } }


CC_INLINE int fill_color_array(int n, COLOR* array, COLOR clr1, COLOR clr2)
{
  int i;
  for (i = 0; i < n; ++i) {
    array[i] = RGBABLEN2(clr1, clr2, i, (n-1));
  }
  return 0;
}
CC_INLINE int fill_color_array2(int n, COLOR* array, int m, const COLOR* clrs, const float* pos)
{
  int i, a, b;
  if (pos) {
    for (i = 0; i < m - 1; ++i) {
      ASSERT(pos[i]<=1. && pos[i+1]<=1.);
      a = (int)(pos[i] * n);
      b = (int)(pos[i + 1] * n);
      fill_color_array(b - a, array + a, clrs[i], clrs[i + 1]);
    }
  } else {
    for (i = 0; i < m - 1; ++i) {
      a = (i * n)/(m-1);
      b = ((i + 1) * n)/(m-1);
      fill_color_array(b - a, array + a, clrs[i], clrs[i + 1]);
    }
  }
  return 0;
}
CC_INLINE int fill_color_array3(int n, COLOR* array, COLOR clr1, COLOR clr2, int m, const double* pos, const double* fac)
{
  int i, a, b;
  COLOR clra, clrb;
  for (i = 0; i < m - 1; ++i) {
    clra = RGBABLEN2(clr2, clr1, (fac[i]), 1);
    clrb = RGBABLEN2(clr2, clr1, (fac[i + 1]), 1);
    a = (int)(pos[i] * n);
    b = (int)(pos[i + 1] * n);
    fill_color_array(b - a, array + a, clra, clrb);
  }
  return 0;
}


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
// ????????ï¿½ï¿½??????????
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
      int* OFS = (int*)pmalloc(_AREA*sizeof(int)); \
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
      pfree(OFS); \
    } else { \
      IMRESIZE(SRCH, SRCW, SRC, SRCSTEP, CN, DSTH, DSTW, DST, DSTSTEP, CN, INTER_BILINEARITY);\
    } \
  } while(0)
#define IMRESIZE_BILIN(SRCH, SRCW, SRC, SRCSTEP, DSTH, DSTW, DST, DSTSTEP, CN) \
  IMRESIZE(SRCH, SRCW, SRC, SRCSTEP, CN, DSTH, DSTW, DST, DSTSTEP, CN, INTER_BILINEARITY)
// ???ï¿½ï¿½???ï¿½ï¿½
// co[] ????ï¿½ï¿½???
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
struct XFILE;
typedef struct XFILE* ihand_t;
//#define ihand_t XFILE*
//#define IMGHEADFEILD  int width, height, channels
typedef struct PaletteEntry {
  uchar b, g, r, a;
}
PaletteEntry;

//typedef PaletteEntry PaletteEntry;
//typedef PaletteEntry ColorPalette1;
CC_INLINE PaletteEntry Color_rgb(int R, int G, int B)
{
  PaletteEntry c;
  c.a = 255, c.r = R, c.g = G, c.b = B;
  return c;
}
CC_INLINE PaletteEntry Color_argb(int A, int R, int G, int B)
{
  PaletteEntry c;
  c.a = A, c.r = R, c.g = G, c.b = B;
  return c;
}
#if 0
typedef struct arr1_t {
  //IMGHEADFEILD;
  int n, c;
#if 1
  union {
    uchar* data;
#define TYPEDEF(a,b,c,d)  d* b;
TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
  } tt;
#else
  uchar* data;
#endif
}
arr1_t;
CC_INLINE int arr_setsize(arr_t* arr, int n, int c)
{
  if (NULL != arr) {
    if (arr->n == n && arr->c == c && arr->tt.data != NULL) {
    }
    else {
      int sz = n * c;
      uchar* newp = arr->tt.data;
      REALLOC(uchar, newp, sz);
      memset(newp, 0, sz);
      arr->n = n, arr->c = c, arr->tt.data = newp;
    }
  }
  return 0;
}
#endif

static const COLOR map32_1bppGray[] = {0x00000000, 0xFFFFFFFF};
static const uchar map8_1bppGray[] = {0, 0xff};

static const COLOR map32_4bppGray[] = { 0xff000000, 0xff111111, 0xff222222, 0xff333333, 0xff444444, 0xff555555, 0xff666666, 0xff777777,
0xff888888, 0xff999999, 0xffaaaaaa, 0xffbbbbbb, 0xffcccccc, 0xffdddddd, 0xffeeeeee, 0xffffffff};
static const uchar map8_4bppGray[] = { 0x00, 0x11, 0x22, 0x33,  0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

#define RGB_GRAY(r, g, b)       ( ((r)+((g)<<1)+(b))>>2 )
#define RGBA_GRAY(r, g, b, a)   ( ((r*a)+((g*a)<<1)+(b*a))>>10 )
#define GETPIXEL_1bppIndexed(OUT)  {OUT = palette[(row[x>>3] >> (7 - (x & 7))) & 1]; }
#define GETGRAY_1bppIndexed(OUT)   {OUT = palette[(row[x>>3] >> (7 - (x & 7))) & 1]; }
#define GETPIXEL_1bppGray(OUT)     {OUT = map32_1bppGray[(row[x>>3] >> (7 - (x & 7))) & 1]; }
#define GETGRAY_1bppGray(OUT)      {OUT = map8_1bppGray[(row[x>>3] >> (7 - (x & 7))) & 1]; }
#define GETPIXEL_4bppIndexed(OUT)  {OUT = palette[(x & 1) ? (row[x / 2] & 0xf) : (row[x / 2] >> 4)]; }
#define GETGRAY_4bppIndexed(OUT)   {OUT = palette[(x & 1) ? (row[x / 2] & 0xf) : (row[x / 2] >> 4)]; }
#define GETPIXEL_4bppGray(OUT)  {OUT = map32_4bppGray[(x & 1) ? (row[x / 2] & 0xf) : (row[x / 2] >> 4)]; }
#define GETGRAY_4bppGray(OUT)   {OUT = map8_4bppGray[(x & 1) ? (row[x / 2] & 0xf) : (row[x / 2] >> 4)]; }
#define GETPIXEL_8bppIndexed(OUT)  {OUT = palette[row[x]];}
#define GETGRAY_8bppIndexed(OUT)   {OUT = palette[row[x]];}
#define GETPIXEL_8bppGray(OUT)  {BYTE r, g, b;r = g = b = row[x];OUT = _RGB(r, g, b);}
#define GETGRAY_8bppGray(OUT)   {OUT = row[x];}
#define GETPIXEL_16bppGray(OUT)  {BYTE r, g, b; r = g = b = ((ushort*)(row))[x]>>8; OUT = _RGB(r, g, b);}
#define GETGRAY_16bppGray(OUT)   {OUT = ((ushort*)(row))[x]>>8;}
#define GETPIXEL_16bppRGB555_rgba()  { WORD pixel = *((const WORD*)(row) + x); r = (pixel >> 7 & 0xf8) | (pixel >> 12 & 0x7); g = (pixel >> 2 & 0xf8) | (pixel >> 6 & 0x7); b = (pixel << 3 & 0xf8) | (pixel >> 2 & 0x7); }
#define GETPIXEL_16bppRGB555(OUT)  {BYTE r, g, b; GETPIXEL_16bppRGB555_rgba(); OUT = _RGB(r, g, b);}
#define GETGRAY_16bppRGB555(OUT)   {BYTE r, g, b; GETPIXEL_16bppRGB555_rgba(); OUT = RGB_GRAY(r, g, b);}
#define GETPIXEL_16bppRGB565_rgba() {WORD pixel = *((const WORD*)(row) + x); r = (pixel >> 8 & 0xf8) | (pixel >> 13 & 0x7); g = (pixel >> 3 & 0xfc) | (pixel >> 9 & 0x3); b = (pixel << 3 & 0xf8) | (pixel >> 2 & 0x7); }
#define GETPIXEL_16bppRGB565(OUT)  {BYTE r, g, b; GETPIXEL_16bppRGB565_rgba(); OUT = _RGB(r, g, b);}
#define GETGRAY_16bppRGB565(OUT)   {BYTE r, g, b; GETPIXEL_16bppRGB565_rgba(); OUT = RGB_GRAY(r, g, b);}
#define GETPIXEL_16bppARGB1555_rgba()  {WORD pixel = *((const WORD*)(row) + x);r = (pixel >> 7 & 0xf8) | (pixel >> 12 & 0x7);g = (pixel >> 2 & 0xf8) | (pixel >> 6 & 0x7);b = (pixel << 3 & 0xf8) | (pixel >> 2 & 0x7); a = ((pixel & 0x8000) == 0x8000) ? 255 : 0;}
#define GETPIXEL_16bppARGB1555(OUT)  {BYTE r, g, b, a; GETPIXEL_16bppARGB1555_rgba(); OUT = _RGBA(r, g, b, a);}
#define GETGRAY_16bppARGB1555(OUT)   {BYTE r, g, b, a; GETPIXEL_16bppARGB1555_rgba(); OUT = RGBA_GRAY(r, g, b, a);}
#define GETPIXEL_24bppRGB_rgba()  {r = row[x * 3 + 2];g = row[x * 3 + 1]; b = row[x * 3];}
#define GETPIXEL_24bppRGB(OUT)  {BYTE r, g, b; GETPIXEL_24bppRGB_rgba(); OUT = _RGB(r, g, b); }
#define GETGRAY_24bppRGB(OUT)   {BYTE r, g, b; GETPIXEL_24bppRGB_rgba(); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_32bppRGB_rgba()  {r = row[x * 4 + 2];g = row[x * 4 + 1];  b = row[x * 4];}
#define GETPIXEL_32bppRGB(OUT)  {BYTE r, g, b; GETPIXEL_32bppRGB_rgba(); OUT = _RGB(r, g, b); }
#define GETGRAY_32bppRGB(OUT)   {BYTE r, g, b; GETPIXEL_32bppRGB_rgba(); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_32bppARGB_rgba()  {r = row[x * 4 + 2];g = row[x * 4 + 1];b = row[x * 4];  a = row[x * 4 + 3];}
#define GETPIXEL_32bppARGB(OUT)  {BYTE r, g, b, a; GETPIXEL_32bppARGB_rgba(); OUT = _RGBA(r, g, b, a); }
#define GETGRAY_32bppARGB(OUT)   {BYTE r, g, b, a; GETPIXEL_32bppARGB_rgba(); OUT = RGBA_GRAY(r, g, b, a); }
#define GETPIXEL_32bppPARGB_rgba()  {a = row[x * 4 + 3];if (a == 0) {  r = g = b = 0;} else {  r = row[x * 4 + 2] * 255 / a;  g = row[x * 4 + 1] * 255 / a;  b = row[x * 4] * 255 / a;  }}
#define GETPIXEL_32bppPARGB(OUT)  {BYTE r, g, b, a; GETPIXEL_32bppPARGB_rgba(); OUT = _RGBA(r, g, b, a); }
#define GETGRAY_32bppPARGB(OUT)   {BYTE r, g, b, a; GETPIXEL_32bppPARGB_rgba(); OUT = RGBA_GRAY(r, g, b, a); }
#define GETPIXEL_48bppRGB_rgba()  {r = row[x * 6 + 5];g = row[x * 6 + 3];  b = row[x * 6 + 1];}
#define GETPIXEL_48bppRGB(OUT)  {BYTE r, g, b; GETPIXEL_48bppRGB_rgba(); OUT = _RGB(r, g, b); }
#define GETGRAY_48bppRGB(OUT)   {BYTE r, g, b; GETPIXEL_48bppRGB_rgba(); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_64bppARGB_rgba()  {r = row[x * 8 + 5];g = row[x * 8 + 3];b = row[x * 8 + 1];  a = row[x * 8 + 7];}
#define GETPIXEL_64bppARGB(OUT)  {BYTE r, g, b, a; GETPIXEL_64bppARGB_rgba(); OUT = _RGBA(r, g, b, a); }
#define GETGRAY_64bppARGB(OUT)   {BYTE r, g, b, a; GETPIXEL_64bppARGB_rgba(); OUT = RGB_GRAY(r, g, b);(void)a; }
#define GETPIXEL_64bppPARGB_rgba()  {a = row[x * 8 + 7];if (a == 0) {  r = g = b = 0;}else {  r = row[x * 8 + 5] * 255 / a;  g = row[x * 8 + 3] * 255 / a;  b = row[x * 8 + 1] * 255 / a;}}
#define GETPIXEL_64bppPARGB(OUT)  {BYTE r, g, b, a; GETPIXEL_64bppPARGB_rgba(); OUT = _RGBA(r, g, b, a); }
#define GETGRAY_64bppPARGB(OUT)   {BYTE r, g, b, a; GETPIXEL_64bppPARGB_rgba(); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_32bppGrayFloat(OUT)  {BYTE r, g, b;int t = (int)(((float*)(row))[x]*255); r = g = b = BOUND(t, 0, 255); OUT = _RGB(r, g, b); }
#define GETGRAY_32bppGrayFloat(OUT)   {int t = (int)(((float*)(row))[x]*255); OUT = BOUND(t, 0, 255);}
#define GETPIXEL_64bppGrayDouble(OUT)  {BYTE r, g, b;int t = (int)(((double*)(row))[x]*255); r = g = b = BOUND(t, 0, 255); OUT = _RGB(r, g, b); }
#define GETGRAY_64bppGrayDouble(OUT)   {int t = (int)(((double*)(row))[x]*255); OUT = BOUND(t, 0, 255);}
#define GETPIXEL_128bppARGBFloat(OUT)  {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+0]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+2]*255); OUT = _RGB(r, g, b); }
#define GETGRAY_128bppARGBFloat(OUT)   {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+0]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+2]*255); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_128bppABGRFloat(OUT)  {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+2]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+0]*255); OUT = _RGB(r, g, b); }
#define GETGRAY_128bppABGRFloat(OUT)   {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+2]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+0]*255); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_96bppBGRFloat(OUT)  {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+2]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+0]*255); OUT = _RGB(r, g, b); }
#define GETGRAY_96bppBGRFloat(OUT)   {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+2]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+0]*255); OUT = RGB_GRAY(r, g, b); }

#define GetPixFmtBpp(fmt)  (((fmt)>>8)&0xff)
#define GetPixFmtId(fmt)  ((fmt)&0xff)

CC_INLINE PixFmt cn2PixFmt(int cn) {
  const PixFmt cn2pf[] = {PF_8bppGray, PF_8bppGray, PF_16bppRGB565,PF_24bppRGB, PF_32bppPARGB,};
  return cn<=4 ? cn2pf[cn] : PF_32bppARGB;
}
CC_INLINE PixFmt bpp2PixFmt(int bpp) {
  switch(bpp) {
  case 1: return PF_1bppGray;
  case 4: return PF_4bppGray;
    //case 8: return PF_8bppIndexed;
  case 8: return PF_8bppGray;
  case 16: return PF_16bppGray;
  case 24: return PF_24bppRGB;
  case 32: return PF_32bppARGB;
  default:
    return PixFmtMax;
  }
  return PixFmtMax;
}

CC_INLINE int bmp_free(bitmap_t* bmp) {
  FREE(bmp->data);
  return 0;
}

CC_INLINE int bmp_setsize(bitmap_t* bmp, int height, int width, int bpp) {
  if (NULL != bmp) {
    bmp->free_tex = bmp_free;
    if (bmp->h == height && bmp->w == width && bmp->data != NULL) {
    }
    else {
      int n, step = (width * bpp)/8;
      step = (step+3) & ~3;
      n = sizeof(uchar) * height * step;
      if (n > 0) {
        uchar* newp = bmp->data;
        MYREALLOC(newp, n);
        memset(newp, 0, n);
        BMPINIT(bmp,  height, width, newp, step, bpp);
      }
    }
  }
  return 0;
}
#define FREE_TEX(t)   if ((t)->free_tex) {(t)->free_tex(t); (t)->free_tex = NULL; }
CC_INLINE ISIZE imsize(const img_t* im)
{
  ISIZE sz;
  sz.h = im->h, sz.w = im->w;
  return sz;
}
#define IMARG(im)  (im)->h, (im)->w, (im)->tt.data, (im)->s, (im)->c
#define imcopysize( im, im2 ) imsetsize(im, (im2)->h, (im2)->w, (im2)->c, (im2)->f)
#define imref(_IM, _X, _Y, _W, _H, _IM2)  imsubref(_IM, iRECT(_X, _Y, (_X)+(_W), (_Y)+(_H)), _IM2)
#define imrefrc(_IM, RC, _IM2)  imsubref(_IM, iRECT((RC).x, (RC).y, (RC).x+(RC).w, (RC).y+(RC).h), _IM2)
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
CC_INLINE int imcopyto_mask(const img_t* src, int srcx, int srcy, img_t* dst, int dstx, int dsty, const img_t* mask, int maskx, int masky, int w, int h) {
  const uchar* s;
  uchar* d;
  w = (w<=0) ? INT_MAX : w;
  h = (h<=0) ? INT_MAX : h;
  srcx = BOUND(srcx, 0, src->w-1);
  srcy = BOUND(srcy, 0, src->h-1);
  dstx = BOUND(dstx, 0, dst->w-1);
  dsty = BOUND(dsty, 0, dst->h-1);
  w = MIN(src->w-srcx, dst->w-dstx);
  h = MIN(src->h-srcy, dst->h-dsty);
  ASSERT(src->c==dst->c);
  s = src->tt.data+srcy*src->s + srcx*src->c;
  d = dst->tt.data+dsty*dst->s + dstx*dst->c;
  if (mask) {
    uchar* m;
    maskx = BOUND(maskx, 0, mask->w-1);
    masky = BOUND(masky, 0, mask->h-1);
    w = MIN(w, mask->w-maskx);
    h = MIN(h, mask->h-masky);
    m = mask->tt.data+masky*mask->s + maskx*mask->c;
    memcpy2dmask(d, dst->s, s, src->s, m, mask->s, h, w, src->c);
  } else {
    memcpy2d(d, dst->s, s, src->s, h, w*src->c);
  }
  return 0;
}
CC_INLINE int imcopyto(const img_t* src, int srcx, int srcy, img_t* dst, int dstx, int dsty, int w, int h) {
  return imcopyto_mask(src, srcx, srcy, dst, dstx, dsty, NULL, 0, 0, w, h);
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
#define imtype(im)  CC_MAT_TYPECN(im)
#define imtid(im)  CC_MAT_DEPTH(im)
#define imdepth(im)  CC_MAT_DEPTH(im)
#define imzero(im)  memset((im)->tt.data, 0, (im)->h*(im)->s)
#define imsetsamesize(im, im2)  imsetsize(im, (im2)->h, (im2)->w, (im2)->c, (im2)->f)
#define imsetsize_i4(im, h, w, c, f)  imsetsize(im, h, w, c*sizeof(int), f)
#define imsetsize_f8(im, h, w, c, f)  imsetsize(im, h, w, c*sizeof(double), f)
#define imsetsize_f4(im, h, w, c, f)  imsetsize(im, h, w, c*sizeof(float), f)
#define bf_imsetsamesize(bf, im, im2)  bf_imsetsize(bf, im, (im2)->h, (im2)->w, (im2)->c, (im2)->f)
CC_INLINE void* alignPtr(void* ptr, int n)
{
    return (void*)(((size_t)ptr + n-1) & -n);
}
CC_INLINE void* fastMalloc(size_t size)
{
  unsigned char* udata = (unsigned char*)malloc(size + sizeof(void*) + MALLOC_ALIGN);
  if (udata) {
    unsigned char** adata = (unsigned char**)alignPtr((unsigned char**)udata + 1, MALLOC_ALIGN);
    adata[-1] = udata;
    return adata;
  }
  return 0;
}

CC_INLINE void* fastRealloc(void* ptr, size_t size)
{
    unsigned char** adata;
    unsigned char* udata = NULL;
    if (ptr) {
      udata = ((unsigned char**)ptr)[-1];
      udata = (unsigned char*)realloc(udata, size + sizeof(void*) + MALLOC_ALIGN);
    } else {
      udata = (unsigned char*)malloc(size + sizeof(void*) + MALLOC_ALIGN);
    }
    adata = (unsigned char**)alignPtr((unsigned char**)udata + 1, MALLOC_ALIGN);
    adata[-1] = udata;
    return adata;
}

CC_INLINE void fastFree(void* ptr)
{
    if (ptr)
    {
        unsigned char* udata = ((unsigned char**)ptr)[-1];
        free(udata);
    }
}

CC_INLINE int imfree(img_t* im)
{
  if (im) {
    if (im->tt.data) {
      //fastFree(im->tt.data);
      FREE(im->tt.data);
    }
    memset(im, 0, sizeof(img_t));
  }
  return 0;
}
#if 0
CC_INLINE int imsetsize(img_t* im, int height, int width, int channels, int frames)
{
  if (NULL != im) {
    if (im->h == height && im->w == width && im->c == channels && im->f == frames && im->tt.data != NULL) {
    }
    else {
      //int step = (width*channels+3) & ~3;
      int step = width * channels;
      int oldn = im->h * im->s * im->f;
      int n = height * step * frames;
      if (n > oldn) {
        REALLOC(uchar, im->tt.data, n);
        //im->tt.data = (uchar*)fastRealloc(im->tt.data, n);
        ASSERT(im->tt.data != NULL && "");
      }
      memset(im->tt.data, 0, n);
      IMINIT(im, height, width, im->tt.data, step, channels, frames);
      im->tid = CC_8U;
    }
  }
  return 0;
}
CC_INLINE int bf_imsetsize(buf_t* bf, img_t* im, int height, int width, int channels, int frames)
{
  if (NULL==bf || NULL==bf->data) {
    return imsetsize(im, height, width, channels, frames);
  }
  if (NULL != im) {
    if (im->h == height && im->w == width && im->c == channels && im->f == frames && im->tt.data != NULL) {
    }
    else {
      int n = sizeof(uchar) * height * width * channels * frames;
      uchar* newp;
      BFMALLOC(bf, newp, n);
      if (newp) {
        memset(newp, 0, n);
        IMINIT(im, height, width, newp, width * channels, channels, frames);
      } else {
        BZERO1(im);
      }
    }
  }
  return 0;
}
#else
#define imsetsize(im, height, width, channels, frames) imsetsize_(im, height, width, channels, frames, __FILE__, __LINE__)
CC_INLINE int imsetsize_(img_t* im, int height, int width, int channels, int frames, const char* file, int line)
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
        REALLOC_(uchar, im->tt.data, n);
        //im->tt.data = (uchar*)fastRealloc(im->tt.data, n);
        ASSERT(im->tt.data != NULL && "");
      }
      IMINIT(im, height, width, im->tt.data, step, channels, frames);
      im->tid = CC_8U;
    }
    memset(im->tt.data, 0, n);
  }
  return 0;
}
#define bf_imsetsize(bf, im, height, width, channels, frames) bf_imsetsize_(bf, im, height, width, channels, frames, __FILE__, __LINE__)
CC_INLINE int bf_imsetsize_(buf_t* bf, img_t* im, int height, int width, int channels, int frames, const char* file, int line)
{
  if (NULL==bf || NULL==bf->data) {
    return imsetsize_(im, height, width, channels, frames, file, line);
  }
  if (NULL != im) {
    if (im->h == height && im->w == width && im->c == channels && im->f == frames && im->tt.data != NULL) {
    }
    else {
      int n = sizeof(uchar) * height * width * channels * frames;
      uchar* newp;
      BFMALLOC(bf, newp, n);
      if (newp) {
        memset(newp, 0, n);
        IMINIT(im, height, width, newp, width * channels, channels, frames);
      } else {
        BZERO1(im);
      }
    }
  }
  return 0;
}
#endif

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
CC_INLINE int imchessboard(img_t* im, int height, int width, int cn, int cellsize) {
  int i, j;
  int cellsize2 = cellsize*2;
  int cellwidth = cellsize*cn;
  int cellwidth2 = cellwidth*2;
  uchar bitmap[] = {0xff, 0};
  imsetsize(im, height, width, cn, 1);
#if 0
  for (i=0; i<im->h; ++i) {
    uchar* im1 = img_row(uchar, im, i);
    int i0 = (i%cellsize2)<cellsize;
    for (j=0; j<width*cn; ++j) {
      int j0 = (j%cellwidth2)<cellwidth;
      im1[j] = bitmap[i0 ^ j0];
    }
  }
#else
  for (i=0; i<im->h; i+=cellsize) {
    uchar* im1 = img_row(uchar, im, i);
    int i0 = (i%cellsize2)<cellsize;
    for (j=0; j<width*cn; ++j) {
      int j0 = (j%cellwidth2)<cellwidth;
      im1[j] = bitmap[i0 ^ j0];
    }
    for (j=i;j<im->h-1; ++j, im1+=im->s) {
      memcpy(im1+im->s, im1, width*cn);
    }
  }
#endif
  return 0;
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
  img_t b[1] = {0};
  imclone2(a, b);
  return *b;
}
CC_INLINE int imsubcopy(const img_t* im, IRECT rc, img_t* im2)
{
  img_t im1[1] = {{0}};
  imsubref(im, rc, im1);
  imclone2(im1, im2);
  return 0;
}
CC_INLINE int bf_imclone2(buf_t* bf, const img_t* a, img_t* b)
{
  if (a != b) {
    bf_imsetsamesize(bf, b, a);
    memcpy2d(b->tt.data, b->s, a->tt.data, a->s, a->h, b->w * b->c);
  }
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
    imfree(im+i);
  }
  memset(im, 0, n * sizeof(img_t));
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
#if 0
typedef struct imgv_t {
  img_t* v;
  int n;
} imgv_t;

typedef struct imgvv_t {
  imgv_t* v;
  int n;
} imgvv_t;

CC_INLINE int imgv_free(imgv_t* iv)
{
  if (iv->v) {
    imfrees(iv->v, iv->n);
    pfree(iv->v);
  }
  iv->v = 0;
  iv->n = 0;
  return 0;
}

CC_INLINE int imgv_free(imgv_t* iv)
{
  VEC_FREES(iv->v, iv->n, imfree);
  return 0;
}

CC_INLINE int imgv_setsize(imgv_t* iv, int n)
{
  VEC_SETSIZE(iv->v, iv->n, n, imfree);
  return 0;
}
#else
#endif
VEC_DEF(vimg_t, img_t, vimg_free, vimg_setsize, imfree);
VEC_DEF(vvimg_t, vimg_t, vvimg_free, vvimg_setsize, vimg_free);

//int fmtpng_write16( FILE* m_fp, int height, int width, const uchar* data, int step, int channels );
CC_INLINE int imwritedat(const char* picname, int height, int width, const uchar* data, int step, int channels)
{
  FILE* pf = fopen(picname, "wb");
  if (pf != NULL) {
    int i;
    fwrite(&height, sizeof(int), 1, pf);
    fwrite(&width, sizeof(int), 1, pf);
    fwrite(&channels, sizeof(int), 1, pf);
    for (i = 0; i < height; ++i, data += step) {
      fwrite(data, width * channels, 1, pf);
    }
    return 1;
  }
  return 0;
}
CC_INLINE int imreaddat2(const char* picname, img_t* im)
{
  FILE* pf = fopen(picname, "rb");
  if (pf != NULL) {
    int height, width, channels;
    fread(&height, sizeof(int), 1, pf);
    fread(&width, sizeof(int), 1, pf);
    fread(&channels, sizeof(int), 1, pf);
    imsetsize(im, height, width, channels, 1);
    fread(im->tt.data, height * width * channels, 1, pf);
  }
  return 0;
}

//#define CC_IS_SPARSE_MAT(mat)   CC_IS_SPARSE_MAT_HDR(mat)
/**************** iteration through a sparse array *****************/
#define CC_NODE_VAL(mat,node)   ((void*)((uchar*)(node) + (mat)->valoffset))
#define CC_NODE_IDX(mat,node)   ((int*)((uchar*)(node) + (mat)->idxoffset))
//                           Histogram                                      *
typedef int CvHistType;
#define CC_HIST_MAGIC_VAL     0x42450000
#define CC_HIST_UNIFORM_FLAG  (1 << 10)
/* indicates whether bin ranges are set already or not */
#define CC_HIST_RANGES_FLAG   (1 << 11)
#define CC_HIST_ARRAY         0
#define CC_HIST_SPARSE        1
#define CC_HIST_TREE          CC_HIST_SPARSE
/* should be used as a parameter only,
   it turns to CC_HIST_UNIFORM_FLAG of hist->type */
#define CC_HIST_UNIFORM       1
#define CC_IS_HIST( hist ) \
  ((hist) != NULL  && \
      (((CvHistogram*)(hist))->type & CC_MAGIC_MASK) == CC_HIST_MAGIC_VAL && \
      (hist)->bins != NULL)
#define CC_IS_UNIFORM_HIST( hist ) \
  (((hist)->type & CC_HIST_UNIFORM_FLAG) != 0)
#define CC_IS_SPARSE_HIST( hist ) \
  CC_IS_SPARSE_MAT((hist)->bins)
#define CC_HIST_HAS_RANGES( hist ) \
  (((hist)->type & CC_HIST_RANGES_FLAG) != 0)
//        Other supplementary data type definitions                         *
/*********************************** CvTermCriteria *************************************/
#define CC_TERMCRIT_ITER    1
#define CC_TERMCRIT_NUMBER  CC_TERMCRIT_ITER
#define CC_TERMCRIT_EPS     2
typedef struct bgra_t {
  BYTE b, g, r, a;
} rgba_t;
#define CC_WHOLE_SEQ_END_INDEX 0x3fffffff
#define CC_WHOLE_SEQ  cvSlice(0, CC_WHOLE_SEQ_END_INDEX)
//                     Dynamic Data structures                              *
/*********************************** Chain/Countour *************************************/
//                      Sequence types                                      *
#define CC_SEQ_MAGIC_VAL             0x42990000
#define CC_IS_SEQ(seq) \
  ((seq) != NULL && (((CvSeq*)(seq))->flags & CC_MAGIC_MASK) == CC_SEQ_MAGIC_VAL)
#define CC_SET_MAGIC_VAL             0x42980000
#define CC_IS_SET(set) \
  ((set) != NULL && (((CvSeq*)(set))->flags & CC_MAGIC_MASK) == CC_SET_MAGIC_VAL)
#define CC_SEQ_ELTYPE_BITS           9
#define CC_SEQ_ELTYPE_MASK           ((1 << CC_SEQ_ELTYPE_BITS) - 1)
//#define CC_SEQ_ELTYPE_POINT          CC_32SC2  /* (x,y) */
#define CC_SEQ_ELTYPE_POINT          123  /* (x,y) */
#define CC_SEQ_ELTYPE_CODE           CC_8UC1   /* freeman code: 0..7 */
#define CC_SEQ_ELTYPE_GENERIC        0
#define CC_SEQ_ELTYPE_PTR            CC_USRTYPE1
#define CC_SEQ_ELTYPE_PPOINT         CC_SEQ_ELTYPE_PTR  /* &(x,y) */
#define CC_SEQ_ELTYPE_INDEX          CC_32SC1  /* #(x,y) */
#define CC_SEQ_ELTYPE_GRAPH_EDGE     0  /* &next_o, &next_d, &vtx_o, &vtx_d */
#define CC_SEQ_ELTYPE_GRAPH_VERTEX   0  /* first_edge, &(x,y) */
#define CC_SEQ_ELTYPE_TRIAN_ATR      0  /* vertex of the binary tree   */
#define CC_SEQ_ELTYPE_CONNECTED_COMP 0  /* connected component  */
#define CC_SEQ_ELTYPE_POINT3D        CC_32FC3  /* (x,y,z)  */
#define CC_SEQ_KIND_BITS        3
#define CC_SEQ_KIND_MASK        (((1 << CC_SEQ_KIND_BITS) - 1)<<CC_SEQ_ELTYPE_BITS)
/* types of sequences */
#define CC_SEQ_KIND_GENERIC     (0 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_KIND_CURVE       (1 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_KIND_BIN_TREE    (2 << CC_SEQ_ELTYPE_BITS)
/* types of sparse sequences (sets) */
#define CC_SEQ_KIND_GRAPH       (3 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_KIND_SUBDIV2D    (4 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_FLAG_SHIFT       (CC_SEQ_KIND_BITS + CC_SEQ_ELTYPE_BITS)
/* flags for curves */
#define CC_SEQ_FLAG_CLOSED     (1 << CC_SEQ_FLAG_SHIFT)
#define CC_SEQ_FLAG_SIMPLE     (2 << CC_SEQ_FLAG_SHIFT)
#define CC_SEQ_FLAG_CONVEX     (4 << CC_SEQ_FLAG_SHIFT)
#define CC_SEQ_FLAG_HOLE       (8 << CC_SEQ_FLAG_SHIFT)
/* flags for graphs */
#define CC_GRAPH_FLAG_ORIENTED (1 << CC_SEQ_FLAG_SHIFT)
#define CC_GRAPH               CC_SEQ_KIND_GRAPH
#define CC_ORIENTED_GRAPH      (CC_SEQ_KIND_GRAPH|CC_GRAPH_FLAG_ORIENTED)
/* point sets */
#define CC_SEQ_POINT_SET       (CC_SEQ_KIND_GENERIC| CC_SEQ_ELTYPE_POINT)
#define CC_SEQ_POINT3D_SET     (CC_SEQ_KIND_GENERIC| CC_SEQ_ELTYPE_POINT3D)
#define CC_SEQ_POLYLINE        (CC_SEQ_KIND_CURVE  | CC_SEQ_ELTYPE_POINT)
#define CC_SEQ_POLYGON         (CC_SEQ_FLAG_CLOSED | CC_SEQ_POLYLINE )
#define CC_SEQ_CONTOUR         CC_SEQ_POLYGON
#define CC_SEQ_SIMPLE_POLYGON  (CC_SEQ_FLAG_SIMPLE | CC_SEQ_POLYGON  )
/* chain-coded curves */
#define CC_SEQ_CHAIN           (CC_SEQ_KIND_CURVE  | CC_SEQ_ELTYPE_CODE)
#define CC_SEQ_CHAIN_CONTOUR   (CC_SEQ_FLAG_CLOSED | CC_SEQ_CHAIN)
/* binary tree for the contour */
#define CC_SEQ_POLYGON_TREE    (CC_SEQ_KIND_BIN_TREE  | CC_SEQ_ELTYPE_TRIAN_ATR)
/* sequence of the connected components */
#define CC_SEQ_CONNECTED_COMP  (CC_SEQ_KIND_GENERIC  | CC_SEQ_ELTYPE_CONNECTED_COMP)
/* sequence of the integer numbers */
#define CC_SEQ_INDEX           (CC_SEQ_KIND_GENERIC  | CC_SEQ_ELTYPE_INDEX)
#define CC_SEQ_ELTYPE( seq )   ((seq)->flags & CC_SEQ_ELTYPE_MASK)
#define CC_SEQ_KIND( seq )     ((seq)->flags & CC_SEQ_KIND_MASK )
/* flag checking */
#define CC_IS_SEQ_INDEX( seq )      ((CC_SEQ_ELTYPE(seq) == CC_SEQ_ELTYPE_INDEX) && (CC_SEQ_KIND(seq) == CC_SEQ_KIND_GENERIC))
#define CC_IS_SEQ_CURVE( seq )      (CC_SEQ_KIND(seq) == CC_SEQ_KIND_CURVE)
#define CC_IS_SEQ_CLOSED( seq )     (((seq)->flags & CC_SEQ_FLAG_CLOSED) != 0)
#define CC_IS_SEQ_CONVEX( seq )     (((seq)->flags & CC_SEQ_FLAG_CONVEX) != 0)
#define CC_IS_SEQ_HOLE( seq )       (((seq)->flags & CC_SEQ_FLAG_HOLE) != 0)
#define CC_IS_SEQ_SIMPLE( seq )     ((((seq)->flags & CC_SEQ_FLAG_SIMPLE) != 0) || CC_IS_SEQ_CONVEX(seq))
/* type checking macros */
#define CC_IS_SEQ_POINT_SET( seq )   ((CC_SEQ_ELTYPE(seq) == CC_32SC2 || CC_SEQ_ELTYPE(seq) == CC_32FC2))
#define CC_IS_SEQ_POINT_SUBSET( seq )   (CC_IS_SEQ_INDEX( seq ) || CC_SEQ_ELTYPE(seq) == CC_SEQ_ELTYPE_PPOINT)
#define CC_IS_SEQ_POLYLINE( seq )   (CC_SEQ_KIND(seq) == CC_SEQ_KIND_CURVE && CC_IS_SEQ_POINT_SET(seq))
#define CC_IS_SEQ_POLYGON( seq )   (CC_IS_SEQ_POLYLINE(seq) && CC_IS_SEQ_CLOSED(seq))
#define CC_IS_SEQ_CHAIN( seq )   (CC_SEQ_KIND(seq) == CC_SEQ_KIND_CURVE && (seq)->elem_size == 1)
#define CC_IS_SEQ_CONTOUR( seq )   (CC_IS_SEQ_CLOSED(seq) && (CC_IS_SEQ_POLYLINE(seq) || CC_IS_SEQ_CHAIN(seq)))
#define CC_IS_SEQ_CHAIN_CONTOUR( seq )   (CC_IS_SEQ_CHAIN( seq ) && CC_IS_SEQ_CLOSED( seq ))
#define CC_IS_SEQ_POLYGON_TREE( seq )   (CC_SEQ_ELTYPE (seq) ==  CC_SEQ_ELTYPE_TRIAN_ATR && CC_SEQ_KIND( seq ) ==  CC_SEQ_KIND_BIN_TREE )
#define CC_IS_GRAPH( seq )   (CC_IS_SET(seq) && CC_SEQ_KIND((CvSet*)(seq)) == CC_SEQ_KIND_GRAPH)
#define CC_IS_GRAPH_ORIENTED( seq )   (((seq)->flags & CC_GRAPH_FLAG_ORIENTED) != 0)
#define CC_IS_SUBDIV2D( seq )   (CC_IS_SET(seq) && CC_SEQ_KIND((CvSet*)(seq)) == CC_SEQ_KIND_SUBDIV2D)
/****************************************************************************************/
///                  Operations on sequences                               */
/****************************************************************************************/
#define CC_SEQ_ELEM( seq, elem_type, index ) \
  /* assert gives some guarantee that <seq> parameter is valid */ \
  (   assert(sizeof((seq)->first[0]) == sizeof(CvSeqBlock) && \
      (seq)->elem_size == sizeof(elem_type)), \
      (elem_type*)((seq)->first && (unsigned)index < \
          (unsigned)((seq)->first->count) ? \
          (seq)->first->tt.data + (index) * sizeof(elem_type) : \
          cvGetSeqElem( (CvSeq*)(seq), (index) )))
#define CC_GET_SEQ_ELEM( elem_type, seq, index ) CC_SEQ_ELEM( (seq), elem_type, (index) )
/* Add element to sequence: */
#define CC_WRITE_SEQ_ELEM_VAR( elem_ptr, writer ) \
  { \
    if( (writer).ptr >= (writer).block_max ) \
    { \
      cvCreateSeqBlock( &writer); \
    } \
    memcpy((writer).ptr, elem_ptr, (writer).seq->elem_size);\
    (writer).ptr += (writer).seq->elem_size; \
  }
#define CC_WRITE_SEQ_ELEM1( elem, writer ) \
  { \
    assert( (writer).seq->elem_size == sizeof(elem)); \
    if( (writer).ptr >= (writer).block_max ) \
    { \
      cvCreateSeqBlock( &writer); \
    } \
    assert( (writer).ptr <= (writer).block_max - sizeof(elem));\
    memcpy((writer).ptr, &(elem), sizeof(elem)); \
    (writer).ptr += sizeof(elem); \
  }
/* Move reader position forward: */
#define CC_NEXT_SEQ_ELEM( elem_size, reader ) \
  { \
    if( ((reader).ptr += (elem_size)) >= (reader).block_max ) \
    { \
      cvChangeSeqBlock( &(reader), 1 ); \
    } \
  }
/* Move reader position backward: */
#define CC_PREV_SEQ_ELEM( elem_size, reader ) \
  { \
    if( ((reader).ptr -= (elem_size)) < (reader).block_min ) \
    { \
      cvChangeSeqBlock( &(reader), -1 ); \
    } \
  }
/* Read element and move read position forward: */
#define CC_READ_SEQ_ELEM( elem, reader ) \
  { \
    assert( (reader).seq->elem_size == sizeof(elem)); \
    memcpy( &(elem), (reader).ptr, sizeof((elem))); \
    CC_NEXT_SEQ_ELEM( sizeof(elem), reader ) \
  }
/* Read element and move read position backward: */
#define CC_REV_READ_SEQ_ELEM( elem, reader ) \
  { \
    assert( (reader).seq->elem_size == sizeof(elem)); \
    memcpy(&(elem), (reader).ptr, sizeof((elem))); \
    CC_PREV_SEQ_ELEM( sizeof(elem), reader ) \
  }
#define CC_READ_CHAIN_POINT( _pt, reader ) \
  { \
    (_pt) = (reader).pt; \
    if( (reader).ptr ) \
    { \
      CC_READ_SEQ_ELEM( (reader).code, (reader)); \
      assert( ((reader).code & ~7) == 0 ); \
      (reader).pt.x += (reader).deltas[(int)(reader).code][0]; \
      (reader).pt.y += (reader).deltas[(int)(reader).code][1]; \
    } \
  }
#define CC_CURRENT_POINT( reader )  (*((CvPoint*)((reader).ptr)))
#define CC_PREV_POINT( reader )     (*((CvPoint*)((reader).prev_elem)))
#define CC_READ_EDGE( pt1, pt2, v, i ) \
  { img_ptr()\
  }
/************ Graph macros ************/
/* Return next graph edge for given vertex: */
#define CC_NEXT_GRAPH_EDGE( edge, vertex ) \
  (assert((edge)->vtx[0] == (vertex) || (edge)->vtx[1] == (vertex)), \
      (edge)->next[(edge)->vtx[1] == (vertex)])
//                    Basic GUI functions                                   *
enum {
  /* 8bit, color or not */
  CC_LOAD_IMAGE_UNCHANGED  = -1,
  /* 8bit, gray */
  CC_LOAD_IMAGE_GRAYSCALE  = 0,
  /* ?, color */
  CC_LOAD_IMAGE_COLOR      = 1,
  /* any depth, ? */
  CC_LOAD_IMAGE_ANYDEPTH   = 2,
  /* ?, any color */
  CC_LOAD_IMAGE_ANYCOLOR   = 4
};
enum {
  CC_IMWRITE_JPEG_QUALITY = 1,
  CC_IMWRITE_PNG_COMPRESSION = 16,
  CC_IMWRITE_PXM_BINARY = 32
};
enum {
  CC_CVTIMG_FLIP      = 1,
  CC_CVTIMG_SWAP_RB   = 2
};
enum {
  CC_CAP_ANY      = 0,    // autodetect
  CC_CAP_MIL      = 100,  // MIL proprietary drivers
  CC_CAP_VFW      = 200,  // platform native
  CC_CAP_V4L      = 200,
  CC_CAP_V4L2     = 200,
  CC_CAP_FIREWARE = 300,  // IEEE 1394 drivers
  CC_CAP_FIREWIRE = 300,
  CC_CAP_IEEE1394 = 300,
  CC_CAP_DC1394   = 300,
  CC_CAP_CMU1394  = 300,
  CC_CAP_STEREO   = 400,  // TYZX proprietary drivers
  CC_CAP_TYZX     = 400,
  CC_TYZX_LEFT    = 400,
  CC_TYZX_RIGHT   = 401,
  CC_TYZX_COLOR   = 402,
  CC_TYZX_Z       = 403,
  CC_CAP_QT       = 500,  // QuickTime
  CC_CAP_UNICAP   = 600,  // Unicap drivers
  CC_CAP_DSHOW    = 700,  // DirectShow (via videoInput)
  CC_CAP_PVAPI    = 800  // PvAPI, Prosilica GigE SDK
};
enum {
  CC_CAP_PROP_POS_MSEC       = 0,
  CC_CAP_PROP_POS_FRAMES     = 1,
  CC_CAP_PROP_POS_AVI_RATIO  = 2,
  CC_CAP_PROP_FRAME_WIDTH    = 3,
  CC_CAP_PROP_FRAME_HEIGHT   = 4,
  CC_CAP_PROP_FPS            = 5,
  CC_CAP_PROP_FOURCC         = 6,
  CC_CAP_PROP_FRAME_COUNT    = 7,
  CC_CAP_PROP_FORMAT         = 8,
  CC_CAP_PROP_MODE           = 9,
  CC_CAP_PROP_BRIGHTNESS    = 10,
  CC_CAP_PROP_CONTRAST      = 11,
  CC_CAP_PROP_SATURATION    = 12,
  CC_CAP_PROP_HUE           = 13,
  CC_CAP_PROP_GAIN          = 14,
  CC_CAP_PROP_EXPOSURE      = 15,
  CC_CAP_PROP_CONVERT_RGB   = 16,
  CC_CAP_PROP_WHITE_BALANCE = 17,
  CC_CAP_PROP_RECTIFICATION = 18
};
/* "black box" video file writer structure */
typedef struct CvVideoWriter CvVideoWriter;
CC_INLINE int CC_FOURCC(char c1, char c2, char c3, char c4)
{
  return (c1 & 255) + ((c2 & 255) << 8) + ((c3 & 255) << 16) + ((c4 & 255) << 24);
}
#define CC_FOURCC_PROMPT -1  /* Open Codec Selection Dialog (Windows only) */
#define CC_FOURCC_DEFAULT CC_FOURCC('I', 'Y', 'U', 'V') /* Use default codec for specified filename (Linux only) */

#if 0
typedef struct capcam_ capcam;
#define CAP_HEADER \
  int height, width, channel, imageSize; \
  unsigned char* data; \
  void ( *Close ) ( capcam* ); \
  int ( *GrabFrame ) ( capcam* ); \
  int ( *SetFramesPerSec )( capcam* capture, float FramesPerSec ); \
  int ( *writeFrame )( capcam* capture, const unsigned char* image, int image_step )
struct capcam_ {
  CAP_HEADER;
};
//typedef unsigned char* cam_iterator;
typedef struct camera_param_t {
  long   m_CaptureW;                   /*????? */
  long   m_CaptureH;                   /*?????*/
  long   m_FramRate;                   /*???*/
  //BOOL   m_Eddy;                     /*??????*/
  //BOOL   m_Repeat;                   /*???????????*/
} camera_param_t;

//#define cam_start(i)           NewCAM_VFW(i, 480, 640)
capcam* cam_start(int index, const camera_param_t* p);

// #define cam_startavi(fn)       NewAVI_VFW(fn)
capcam* cap_open_avi(const char* filename);
capcam* cap_create_avi(const char* filename, int height, int width, int nChannels);
//??? CaptureCAM_VFW ?? capture ???????????
#define cam_stop(cap)          ((capcam*)cap)->Close(cap)

#define cam_GrabFrame(cap)     ((capcam*)cap)->GrabFrame(cap)
#define cam_SetFps(cap, fps)   ((capcam*)cap)->SetFramesPerSec(cap, fps)
//#define cam_begin(cap)         (cam_iterator)(((capcam*)cap)->data)
#define cam_height(cap)        (((capcam*)cap)->height)
#define cam_width(cap)         (((capcam*)cap)->width)
#define cam_step(cap)          (((capcam*)cap)->width * ((capcam*)cap)->channel)
#define cam_channel(cap)       (((capcam*)cap)->channel)
#define cap_writeFrame(cap, _A, _L)    ((capcam*)cap)->writeFrame(cap, _A, _L)
#define cam_GetFrame(cap, _H, _W, _A, _L, _I) \
  if (cap) {_H = cam_height(cap),_W = cam_width(cap),_A = cam_begin(cap),_L = cam_step(cap), _I = cam_channel(cap);} else {_H = 0,_W = 0,_A = 0,_L = 0, _I = 0;}
#define cam_QueryFrame(cap, _H, _W, _A, _L, _I) \
  if ( ((capcam*)cap)->GrabFrame(cap) ) {_H = cam_height(cap),_W = cam_width(cap),_A = cam_begin(cap),_L = cam_step(cap), _I = cam_channel(cap);} else {_H = 0,_W = 0,_A = 0,_L = 0, _I = 0;}

#endif


typedef unsigned char* seq_iterator;
#define Scalar_convertTo(s, T2, buf0, cn, unroll_to) \
  do { \
    int i; T2* buf1=(T2*)buf0; \
    CC_Assert(cn <= 4); \
    for( i = 0; i < cn; i++ ) \
      buf1[i] = (T2)(s->val[i]); \
    for( ; i < unroll_to; i++ ) \
      buf1[i] = buf1[i-cn]; \
  } while(0)
enum CC_FONT_FACE {
  CC_FONT_HERSHEY_SIMPLEX = 0,
  CC_FONT_HERSHEY_PLAIN = 1,
  CC_FONT_HERSHEY_DUPLEX = 2,
  CC_FONT_HERSHEY_COMPLEX = 3,
  CC_FONT_HERSHEY_TRIPLEX = 4,
  CC_FONT_HERSHEY_COMPLEX_SMALL = 5,
  CC_FONT_HERSHEY_SCRIPT_SIMPLEX = 6,
  CC_FONT_HERSHEY_SCRIPT_COMPLEX = 7,
  CC_FONT_ITALIC = 16,
};
// -128.f ... 255.f
extern const float g_8x32fTab[];
//#define CC_8TO32F(x)  cv::g_8x32fTab[(x)+128]
extern const ushort g_8x16uSqrTab[];
#define CC_SQR_8U(x)  g_8x16uSqrTab[(x)+255]
extern const char* g_HersheyGlyphs[];
extern const uchar g_Saturate8u[];
//#define CC_FAST_CAST_8U(t)   (assert(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CC_MIN_8U(a,b)       ((a) - CC_FAST_CAST_8U((a) - (b)))
#define CC_MAX_8U(a,b)       ((a) + CC_FAST_CAST_8U((b) - (a)))

#if 0
CC_INLINE int64 getTickCount()
{
#if defined _WIN
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (int64)counter.QuadPart;
#elif (defined __linux || defined __linux__) && defined CLOCK_MONOTONIC && 0
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  return (int64)tp.tv_sec * 1000000000 + tp.tv_nsec;
#elif defined __MACH__
  return (int64)mach_absolute_time();
#elif defined __GNUC__
  //struct timeval tv1;
  //struct timezone tz;
  //gettimeofday( &tv, 0 );
  return 0;//(int64)tv1.tv_sec*1000000 + tv1.tv_usec;
#else
  return 0;
#endif
}

CC_INLINE double getTickFrequency()
{
#if defined _WIN || defined WIN64 || defined _WIN64 || defined WINCE
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  return (double)freq.QuadPart;
#elif defined __linux || defined __linux__
  return 1e9;
#elif defined __MACH__
  static double freq = 0;
  if (freq == 0) {
    mach_timebase_info_data_t sTimebaseInfo;
    mach_timebase_info(&sTimebaseInfo);
    freq = sTimebaseInfo.denom * 1e9 / sTimebaseInfo.numer;
  }
  return freq;
#else
  return 1e6;
#endif
}
#if defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#if defined(__i386__)
CC_INLINE int64 getCPUTickCount(void)
{
  int64 x;
  __asm__ volatile(".byte 0x0f, 0x31" : "=A"(x));
  return x;
}
#elif defined(__x86_64__)
CC_INLINE int64 getCPUTickCount(void)
{
  unsigned hi, lo;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return (int64)lo | ((int64)hi << 32);
}
#elif defined(__ppc__)
CC_INLINE int64 getCPUTickCount(void)
{
  int64 result = 0;
  unsigned upper, lower, tmp;
  __asm__ volatile(
      "0: \n"
      "\tmftbu   %0 \n"
      "\tmftb    %1 \n"
      "\tmftbu   %2 \n"
      "\tcmpw    %2,%0 \n"
      "\tbne     0b \n"
      : "=r"(upper), "=r"(lower), "=r"(tmp)
  );
  return lower | ((int64)upper << 32);
}
#else
#error "RDTSC not defined"
#endif
#elif defined _MSC_VER && defined _WIN && !defined _WIN64
#pragma warning(disable: 4035)
CC_INLINE int64 getCPUTickCount(void)
{
  __asm _emit 0x0f;
  __asm _emit 0x31;
}
#else
CC_INLINE int64 getCPUTickCount()
{
  return getTickCount();
}
#endif

#endif

CC_INLINE const char* ccErrorStr(int status)
{
  static char buf[256];
  switch (status) {
  case CC_StsOk :
    return "No Error";
  case CC_StsBackTrace :
    return "Backtrace";
  case CC_StsError :
    return "Unspecified error";
  case CC_StsInternal :
    return "Internal error";
  case CC_StsNoMem :
    return "Insufficient memory";
  case CC_StsBadArg :
    return "Bad argument";
  case CC_StsNoConv :
    return "Iterations do not converge";
  case CC_StsAutoTrace :
    return "Autotrace call";
  case CC_StsBadSize :
    return "Incorrect size of input array";
  case CC_StsNullPtr :
    return "Null pointer";
  case CC_StsDivByZero :
    return "Division by zero occured";
  case CC_BadStep :
    return "Image step is wrong";
  case CC_StsInplaceNotSupported :
    return "Inplace operation is not supported";
  case CC_StsObjectNotFound :
    return "Requested object was not found";
  case CC_BadDepth :
    return "Input image depth is not supported by function";
  case CC_StsUnmatchedFormats :
    return "Formats of input arguments do not match";
  case CC_StsUnmatchedSizes :
    return "Sizes of input arguments do not match";
  case CC_StsOutOfRange :
    return "One of arguments\' values is out of range";
  case CC_StsUnsupportedFormat :
    return "Unsupported format or combination of formats";
  case CC_BadCOI :
    return "Input COI is not supported";
  case CC_BadNumChannels :
    return "Bad number of channels";
  case CC_StsBadFlag :
    return "Bad flag (parameter or structure field)";
  case CC_StsBadPoint :
    return "Bad parameter of type CvPoint";
  case CC_StsBadMask :
    return "Bad type of mask argument";
  case CC_StsParseError :
    return "Parsing error";
  case CC_StsNotImplemented :
    return "The function/feature is not implemented";
  case CC_StsBadMemBlock :
    return "Memory block has been corrupted";
  case CC_StsAssert :
    return "Assertion failed";
  };
  sprintf(buf, "Unknown %s code %d", status >= 0 ? "status" : "error", status);
  return buf;
}
/* Errors */
#define HG_OK          0 /* Don't bet on it! */
#define HG_BADNAME    -1 /* Bad window or file name */
#define HG_INITFAILED -2 /* Can't initialize HigHGUI */
#define HG_WCFAILED   -3 /* Can't create a window */
#define HG_NULLPTR    -4 /* The null pointer where it should not appear */
#define HG_BADPARAM   -5
#define __BEGIN__ __CC_BEGIN__
#define __END__  __CC_END__
#define EXIT __CC_EXIT__
#define CC_WINDOW_MAGIC_VAL     0x00420042
#define CC_TRACKBAR_MAGIC_VAL   0x00420043
#define CC_CAP_GSTREAMER_1394   0
#define CC_CAP_GSTREAMER_V4L    1
#define CC_CAP_GSTREAMER_V4L2   2
#define CC_CAP_GSTREAMER_FILE   3
#define cvSet2D1(_PIX, _CN, _COL, TYPE) \
  do { \
    if (1==(_CN)) { \
      *(_PIX+0) = (TYPE)((_COL).val[0]); \
    } else if (2==(_CN)) { \
      *(_PIX+0) = (TYPE)((_COL).val[0]); \
      *(_PIX+1) = (TYPE)((_COL).val[1]); \
    } else if (3==(_CN)) { \
      *(_PIX+0) = (TYPE)((_COL).val[0]); \
      *(_PIX+1) = (TYPE)((_COL).val[1]); \
      *(_PIX+2) = (TYPE)((_COL).val[2]); \
    } else if (4==(_CN)) { \
      *(_PIX+0) = (TYPE)((_COL).val[0]); \
      *(_PIX+1) = (TYPE)((_COL).val[1]); \
      *(_PIX+2) = (TYPE)((_COL).val[2]); \
      *(_PIX+3) = (TYPE)((_COL).val[3]); \
    } \
  } while(0)
//                    Eigen objects                                         *
#define CC_EIGOBJ_NO_CALLBACK     0
#define CC_EIGOBJ_INPUT_CALLBACK  1
#define CC_EIGOBJ_OUTPUT_CALLBACK 2
#define CC_EIGOBJ_BOTH_CALLBACK   3
//                         1D/2D HMM                                        *
typedef struct CvImgObsInfo {
  int obs_x;
  int obs_y;
  int obs_size;
  float* obs;//consequtive observations
  int* state;/* arr of pairs superstate/state to which observation belong */
  int* mix;  /* number of mixture to which observation belong */
}
CvImgObsInfo;/*struct for 1 image*/
typedef CvImgObsInfo Cv1DObsInfo;
typedef struct CvEHMMState {
  int num_mix;        /*number of mixtures in this state*/
  float* mu;          /*mean vectors corresponding to each mixture*/
  float* inv_var;     /* square root of inversed variances corresp. to each mixture*/
  float* log_var_val; /* sum of 0.5 (LN2PI + ln(variance[i]) ) for i=1,n */
  float* weight;      /*array of mixture weights. Summ of all weights in state is 1. */
}
CvEHMMState;
typedef struct CvEHMM {
  int level; /* 0 - lowest(i.e its states are double states), ..... */
  int num_states; /* number of HMM states */
  float*  transP;/*transition probab. matrices for states */
  float** obsProb; /* if level == 0 - array of brob matrices corresponding to hmm
                        if level == 1 - martix of matrices */
  union {
    CvEHMMState* state; /* if level == 0 points to double states array,
                               if not - points to embedded hmms */
    struct CvEHMM* ehmm; /* pointer to an embedded model or NULL, if it is a leaf */
  } u;
}
CvEHMM;
// Embedded HMMs
// Creates 2D HMM
#define CC_COUNT_OBS(roi, win, delta, numObs) ( \
  (numObs)->w =((roi)->w - (win)->w + (delta)->w)/(delta)->w, \
  (numObs)->h =((roi)->h - (win)->h + (delta)->h)/(delta)->h)

#define CC_CURRENT_INT( reader ) (*((int *)(reader).ptr))
#define CC_PREV_INT( reader ) (*((int *)(reader).prev_elem))
#define CC_GRAPH_WEIGHTED_VERTEX_FIELDS() CC_GRAPH_VERTEX_FIELDS()\
  float weight;
#define CC_GRAPH_WEIGHTED_EDGE_FIELDS() CC_GRAPH_EDGE_FIELDS()
typedef enum CvGraphWeightType {
  CC_NOT_WEIGHTED,
  CC_WEIGHTED_VTX,
  CC_WEIGHTED_EDGE,
  CC_WEIGHTED_ALL
} CvGraphWeightType;
// Stereo correspondence
#define CLIQUE_TIME_OFF 2
#define CLIQUE_FOUND 1
#define CLIQUE_END   0

#define CC_UNDEF_SC_PARAM         12345 //default value of parameters
#define CC_IDP_BIRCHFIELD_PARAM1  25
#define CC_IDP_BIRCHFIELD_PARAM2  5
#define CC_IDP_BIRCHFIELD_PARAM3  12
#define CC_IDP_BIRCHFIELD_PARAM4  15
#define CC_IDP_BIRCHFIELD_PARAM5  25
#define CC_DISPARITY_BIRCHFIELD  0
//    Name:    cvFindStereoCorrespondence
//    Purpose: find stereo correspondence on stereo-pair
//    Context:
//    Parameters:
//      leftImage - left image of stereo-pair (format 8uC1).
//      rightImage - right image of stereo-pair (format 8uC1).
//   mode - mode of correspondence retrieval (now CC_DISPARITY_BIRCHFIELD only)
//      dispImage - destination disparity image
//      maxDisparity - maximal disparity
//      param1, param2, param3, param4, param5 - parameters of algorithm
//    Returns:
//    Notes:
//      Images must be rectified.
//      All images must have format 8uC1.
//
///                      Older definitions 
typedef float*   CVect32f;
typedef float*   CMatr32f;
typedef double*  CVect64d;
typedef double*  CMatr64d;
typedef struct CMatrix3 {
  float m[3][3];
}
CMatrix3;
typedef struct CConDensation {
  int MP;
  int DP;
  float* DynamMatr;       /* Matrix of the linear Dynamics system  */
  float* State;           /* Vector of State                       */
  int SamplesNum;         /* Number of the Samples                 */
  float** flSamples;      /* arr of the Sample Vectors             */
  float** flNewSamples;   /* temporary array of the Sample Vectors */
  float* flConfidence;    /* Confidence for each Sample            */
  float* flCumulative;    /* Cumulative confidence                 */
  float* Temp;            /* Temporary vector                      */
  float* RandomSample;    /* RandomVector to update sample set     */
  struct CvRandState* RandS; /* Array of structures to generate random vectors */
}
CConDensation;
/*
standard Kalman filter (in G. Welch' and G. Bishop's notation):
  x(k)=A*x(k-1)+B*u(k)+w(k)  p(w)~N(0,Q)
  z(k)=H*x(k)+v(k),   p(v)~N(0,R)
*/
typedef struct CKalman {
  int MP;                     /* number of measurement vector dimensions */
  int DP;                     /* number of state vector dimensions */
  int CP;                     /* number of control vector dimensions */
  /* backward compatibility fields */
#if 1
  float* PosterState;         /* =state_pre->tt.data.fl */
  float* PriorState;          /* =state_post->tt.data.fl */
  float* DynamMatr;           /* =transition_matrix->tt.data.fl */
  float* MeasurementMatr;     /* =measurement_matrix->tt.data.fl */
  float* MNCovariance;        /* =measurement_noise_cov->tt.data.fl */
  float* PNCovariance;        /* =process_noise_cov->tt.data.fl */
  float* KalmGainMatr;        /* =gain->tt.data.fl */
  float* PriorErrorCovariance;/* =error_cov_pre->tt.data.fl */
  float* PosterErrorCovariance;/* =error_cov_post->tt.data.fl */
  float* Temp1;               /* temp1->tt.data.fl */
  float* Temp2;               /* temp2->tt.data.fl */
#endif
#if 0
  CvMat* state_pre;           /* predicted state (x'(k)):
                                    x(k)=A*x(k-1)+B*u(k) */
  CvMat* state_post;          /* corrected state (x(k)):
                                    x(k)=x'(k)+K(k)*(z(k)-H*x'(k)) */
  CvMat* transition_matrix;   /* state transition matrix (A) */
  CvMat* control_matrix;      /* control matrix (B)
                                   (it is not used if there is no control)*/
  CvMat* measurement_matrix;  /* measurement matrix (H) */
  CvMat* process_noise_cov;   /* process noise covariance matrix (Q) */
  CvMat* measurement_noise_cov; /* measurement noise covariance matrix (R) */
  CvMat* error_cov_pre;       /* priori error estimate covariance matrix (P'(k)):
                                    P'(k)=A*P(k-1)*At + Q)*/
  CvMat* gain;                /* Kalman gain matrix (K(k)):
                                    K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)*/
  CvMat* error_cov_post;      /* posteriori error estimate covariance matrix (P(k)):
                                    P(k)=(I-K(k)*H)*P'(k) */
  CvMat* temp1;               /* temporary matrices */
  CvMat* temp2;
  CvMat* temp3;
  CvMat* temp4;
  CvMat* temp5;
#endif
}
CKalman;
/*********************** Haar-like Object Detection structures **************************/
#define CC_HAAR_MAGIC_VAL    0x42500000
#define CC_TYPE_NAME_HAAR    "opencv-haar-classifier"
#define CC_IS_HAAR_CLASSIFIER( haar ) \
  ((haar) != NULL && \
      (((const CvHaarClassifierCascade*)(haar))->flags & CC_MAGIC_MASK)==CC_HAAR_MAGIC_VAL)
#define CC_HAAR_FEATURE_MAX  3
//                     Contour Morphing                                     *
//                      Texture Descriptors                                 *
#define CC_GLCM_OPTIMIZATION_NONE                   -2
#define CC_GLCM_OPTIMIZATION_LUT                    -1
#define CC_GLCM_OPTIMIZATION_HISTOGRAM              0
#define CC_GLCMDESC_OPTIMIZATION_ALLOWDOUBLENEST    10
#define CC_GLCMDESC_OPTIMIZATION_ALLOWTRIPLENEST    11
#define CC_GLCMDESC_OPTIMIZATION_HISTOGRAM          4
#define CC_GLCMDESC_ENTROPY                         0
#define CC_GLCMDESC_ENERGY                          1
#define CC_GLCMDESC_HOMOGENITY                      2
#define CC_GLCMDESC_CONTRAST                        3
#define CC_GLCMDESC_CLUSTERTENDENCY                 4
#define CC_GLCMDESC_CLUSTERSHADE                    5
#define CC_GLCMDESC_CORRELATION                     6
#define CC_GLCMDESC_CORRELATIONINFO1                7
#define CC_GLCMDESC_CORRELATIONINFO2                8
#define CC_GLCMDESC_MAXIMUMPROBABILITY              9
#define CC_GLCM_ALL                                 0
#define CC_GLCM_GLCM                                1
#define CC_GLCM_DESC                                2
typedef struct CvGLCM CvGLCM;
//                    Face eyes&mouth tracking                              *
typedef struct CvFaceTracker CvFaceTracker;
#define CC_NUM_FACE_ELEMENTS    3
enum CC_FACE_ELEMENTS {
  CC_FACE_MOUTH = 0,
  CC_FACE_LEFT_EYE = 1,
  CC_FACE_RIGHT_EYE = 2
};
/****************************************************************************************
 tracking_info is a rectangular array; one row per camera, num_objects elements per row.
 The id field of any unused slots must be -1. Ids need not be ordered or consecutive. On
 completion, the return value is the number of objects located; i.e., the number of objects
 visible by more than one camera. The id field of any unused slots in tracked objects is
 set to -1.
****************************************************************************************/
//             Skeletons and Linear-Contour Models                          *
typedef enum CvLeeParameters {
  CC_LEE_INT = 0,
  CC_LEE_FLOAT = 1,
  CC_LEE_DOUBLE = 2,
  CC_LEE_AUTO = -1,
  CC_LEE_ERODE = 0,
  CC_LEE_ZOOM = 1,
  CC_LEE_NON = 2
} CvLeeParameters;
#define CC_NEXT_VORONOISITE2D( SITE ) ((SITE)->edge[0]->site[((SITE)->edge[0]->site[0] == (SITE))])
#define CC_PREV_VORONOISITE2D( SITE ) ((SITE)->edge[1]->site[((SITE)->edge[1]->site[0] == (SITE))])
#define CC_FIRST_VORONOIEDGE2D( SITE ) ((SITE)->edge[0])
#define CC_LAST_VORONOIEDGE2D( SITE ) ((SITE)->edge[1])
#define CC_NEXT_VORONOIEDGE2D( EDGE, SITE ) ((EDGE)->next[(EDGE)->site[0] != (SITE)])
#define CC_PREV_VORONOIEDGE2D( EDGE, SITE ) ((EDGE)->next[2 + ((EDGE)->site[0] != (SITE))])
#define CC_VORONOIEDGE2D_BEGINNODE( EDGE, SITE ) ((EDGE)->node[((EDGE)->site[0] != (SITE))])
#define CC_VORONOIEDGE2D_ENDNODE( EDGE, SITE ) ((EDGE)->node[((EDGE)->site[0] == (SITE))])
#define CC_TWIN_VORONOISITE2D( SITE, EDGE ) ( (EDGE)->site[((EDGE)->site[0] == (SITE))])
/* Computes Voronoi Diagram for given polygons with holes */
/*********************** Linear-Contour Model ****************************/
struct CvLCMEdge;
struct CvLCMNode;
/* Computes hybrid model from Voronoi Diagram */
/*************************** View Morphing Functions ************************/
/* The order of the function corresponds to the order they should appear in
   the view morphing pipeline */
/* Finds ending points of scanlines on left and right images of stereo-pair */
//             Background/foreground segmentation                           *
#define CC_BG_MODEL_FGD     0
#define CC_BG_MODEL_MOG     1
#define CC_BG_MODEL_FGD_SIMPLE   2
struct CvBGStatModel;
typedef void (* CvReleaseBGStatModel)(struct CvBGStatModel** bg_model);
typedef int (* CvUpdateBGStatModel)(unsigned char* imageData, int widthStep, struct CvBGStatModel* bg_model);
#define MINIVEC(_TYPE)            struct { _TYPE* data; unsigned int used, size; }
#define MINIVEC_INIT(_V, _P, _U, _N)  (((_V)->tt.data) = (void*)(_P), ((_V)->used) = (_U), ((_V)->size) = (_N))
#define MINIVEC_ALLOC(_V, _N) MINIVEC_INIT(_V, pmalloc((_N)*sizeof(*((_V)->tt.data))), 0, _N)
#define MINIVEC_FREE(_V)      (pfree((_V)->tt.data), (_V)->tt.data=0, (_V)->used = (_V)->size=0)
#define MINIVEC_AT(_V, _i)    *(((_V)->tt.data)+(_i))
//#define MINIVEC_ADD(_V, _X)   (ASSERT((_V)->used<(_V)->size), MINIVEC_AT(_V, (_V)->used++) = _X)
#define MINIVEC_ADD(_V, _X)   if((_V)->used<(_V)->size) MINIVEC_AT(_V, (_V)->used++) = _X
#define MINIVEC_POP(_V)       ASSERT((_V)->used>0), MINIVEC_AT(_V, --(_V)->used)
#define CC_BG_STAT_MODEL_FIELDS() \
  int             type, height, width, nChannels; /*type of BG model*/ \
  CvReleaseBGStatModel release; \
  CvUpdateBGStatModel update; \
  unsigned char*       background;   /*8UC3 reference background image*/ \
  unsigned char*       foreground;   /*8UC1 foreground image*/ \
  unsigned char**      layers;       /*8UC3 reference background image, can be null */ \
  int             layer_count  /* can be zero */
/*CONTOURS        foreground_regions; */
/*CvMemStorage*   storage;      storage for ?foreground_regions*/
/*CvSeq*          foreground_regions foreground object contours*/
typedef struct CvBGStatModel {
  CC_BG_STAT_MODEL_FIELDS();
} CvBGStatModel;
// Releases memory used by BGStatModel
#define cvReleaseBGStatModel( bg_model )  if( bg_model && (*bg_model) && (*bg_model)->release ) { (*bg_model)->release( bg_model ); }
// Updates statistical model and returns number of found foreground regions
#define cvUpdateBGStatModel( imageData, widthStep, bg_model )   (bg_model && bg_model->update ? bg_model->update( imageData, widthStep, bg_model ) : 0)
// Performs FG post-processing using segmentation
// (all pixels of a region will be classified as foreground if majority of pixels of the region are FG).
// parameters:
//      segments - pointer to result of segmentation (for example MeanShiftSegmentation)
//      bg_model - pointer to CvBGStatModel structure
/*
  Interface of ACM MM2003 algorithm
  (Liyuan Li, Weimin Huang, Irene Y.H. Gu, and Qi Tian.
  "Foreground Object Detection from Videos Containing Complex Background. ACM MM2003")
*/
/* default paremeters of foreground detection algorithm */
#define CC_BGFG_FGD_LC              128
#define CC_BGFG_FGD_N1C             15
#define CC_BGFG_FGD_N2C             25
#define CC_BGFG_FGD_LCC             64
#define CC_BGFG_FGD_N1CC            25
#define CC_BGFG_FGD_N2CC            40
/* BG reference image update parameter */
#define CC_BGFG_FGD_ALPHA_1         0.1f
/* stat model update parameter
   0.002f ~ 1K frame(~45sec), 0.005 ~ 18sec (if 25fps and absolutely static BG) */
#define CC_BGFG_FGD_ALPHA_2         0.005f
/* start value for alpha parameter (to fast initiate statistic model) */
#define CC_BGFG_FGD_ALPHA_3         0.1f
#define CC_BGFG_FGD_DELTA           2
#define CC_BGFG_FGD_T               0.9f
#define CC_BGFG_FGD_MINAREA         15.f
#define CC_BGFG_FGD_BG_UPDATE_TRESH 0.5f
typedef struct CvFGDStatModelParams {
  int           Lc, N1c, N2c, Lcc, N1cc, N2cc, is_obj_without_holes, perform_morphing;
  float         alpha1, alpha2, alpha3, delta, T, minArea;
}
CvFGDStatModelParams;
typedef struct CvBGPixelCStatTable {
  float          Pv, Pvb;
  uchar          v[3];
}
CvBGPixelCStatTable;
typedef struct CvBGPixelCCStatTable {
  float          Pv, Pvb;
  uchar          v[6];
}
CvBGPixelCCStatTable;
typedef struct CvBGPixelStat {
  float                 Pbc;
  float                 Pbcc;
  CvBGPixelCStatTable*  ctable;
  CvBGPixelCCStatTable* cctable;
  uchar                 is_trained_st_model;
  uchar                 is_trained_dyn_model;
}
CvBGPixelStat;
typedef struct CvFGDStatModel {
  CC_BG_STAT_MODEL_FIELDS();
  CvBGPixelStat*         pixel_stat;
  unsigned char*         Ftd;
  unsigned char*         Fbd;
  unsigned char*         prev_frame;
  CvFGDStatModelParams   params;
}
CvFGDStatModel;
/* Creates FGD model */
/*
   Interface of Gaussian mixture algorithm
   (P. KadewTraKuPong and R. Bowden,
   "An improved adaptive background mixture model for double-time tracking with shadow detection"
   in Proc. 2nd European Workshp on Advanced Video-Based Surveillance Systems, 2001.")
*/
#define CC_BGFG_MOG_MAX_NGAUSSIANS 500
/* default parameters of gaussian background detection algorithm */
#define CC_BGFG_MOG_BACKGROUND_THRESHOLD     0.7f    // ?ï¿½ï¿½???????????????
#define CC_BGFG_MOG_STD_THRESHOLD            2.5f    // lambda=2.5 is 99% ??????
#define CC_BGFG_MOG_WINDOW_SIZE              200     // ?????????1/200??0.005
#define CC_BGFG_MOG_NGAUSSIANS               5       // ??????????
#define CC_BGFG_MOG_WEIGHT_INIT              0.05f   // ??
#define CC_BGFG_MOG_SIGMA_INIT               30      // ????
#define CC_BGFG_MOG_MINAREA                  25.f    // ?????????????????
#define CC_BGFG_MOG_NCOLORS                  3
typedef float GaussReal_t;
// win_size       ???????ï¿½ï¿½?????????????????? ???a=1/win_size;
// bg_threshold   ????????????????????????
// std_threshold  ??????????????
// weight_init    ????????????
// variance_init  ??????????????
// minArea        ??ï¿½ï¿½??????????????????????????????????????ï¿½ï¿½????minArea?????????????????
// n_gauss        ???????????????
typedef struct CvGaussBGStatModelParams {
  int     win_size;               /* = 1/alpha */
  int     n_gauss;
  GaussReal_t  bg_threshold, std_threshold, minArea;
  GaussReal_t  weight_init, variance_init;
} CvGaussBGStatModelParams;
typedef struct CvGaussBGValues {
  int         match_sum;
  GaussReal_t      weight;
  GaussReal_t      variance[CC_BGFG_MOG_NCOLORS];
  GaussReal_t      mean[CC_BGFG_MOG_NCOLORS];
}
CvGaussBGValues;
typedef struct CvGaussBGPoint {
  CvGaussBGValues* g_values;
}
CvGaussBGPoint;
typedef struct CvGaussBGModel {
  CC_BG_STAT_MODEL_FIELDS();
  CvGaussBGStatModelParams   params;
  CvGaussBGPoint*            g_point;
  int                        countFrames;
}
CvGaussBGModel;

#ifndef COLOR_TYPE_DEF_DEF
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
#endif // COLOR_TYPE_DEF_DEF

//#include "libfacerecog.h"
//int alg_memsize( int algtype, void* param );
//int alg_process( int algtype, void* param );
int cascadesize(const void* p);
#define MAX_CAS_NUM (4)
#ifndef USE_FIX_FLOAT
#define USE_FIX_FLOAT 1        // ????????
#endif // USE_FIX_FLOAT
#ifdef USE_FIX_FLOAT
typedef int FIX_FLOAT;
#else
typedef double FIX_FLOAT;
#endif
#ifndef XRECT_DEFINED
#define XRECT_DEFINED
// ¾ØÐÎ¿ò
typedef struct XRECT {
  int x, y, w, h;      /* Ä¿±êµÄËùÓÐ¿òµÄÎ»ÖÃÆ½¾ùÖµ */
  int score;           /* Ä¿±êµÄËùÓÐ¿òµÄµÃ·ÖÆ½¾ùÖµ */
  int count;           /* Ä¿±êµÄËùÓÐ¿òµÄ¸öÊý       */
  //INT id;
}
XRECT;

CC_INLINE int XRECT_int(const XRECT* r, int* out) {
  if (r && out) {
    out[0] = r->x;
    out[1] = r->y;
    out[2] = r->w;
    out[3] = r->h;
  }
  return 0;
}
#endif // XRECT_DEFINED
typedef struct lut_detect_param_t {
  //????
  int height, width, datastep, maskstep, stepxy,
  mincnt, fastmode, maxoutlen, casnum;
  int is_trans, is_flop, is_flip; // ??????????
  double sc, ssmin, ssmax, ss, thd;
  const unsigned char* data, *mask;
  void const* cas[MAX_CAS_NUM];
  XRECT roi;
  //???
  int outlen;
  XRECT* out;
  int buflen;
  unsigned char* buf;
  //??????
  //int isc, issmin, issmax, iss, ithd; /*???ARM??DSP?????????????DM6446?????????????*/
  int h, w; //h=height*sc; w=width*sc;
  void* pWirk[MAX_CAS_NUM]; // = MALLOC( iHaarWork, ca->ihlen );
  FIX_FLOAT* inter; // = MALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
  FIX_FLOAT* inter1; // = MALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
  unsigned char* tmpimg0; // = MALLOC( unsigned char, w * h );
  unsigned char* tmpmask0; // = MALLOC( unsigned char, w * h );
  unsigned char* tmpimg; // = MALLOC( unsigned char, w * h );
  unsigned char* tmpmask; // = MALLOC( unsigned char, w * h );
  unsigned char* rect_partition_buf;
} lut_detect_param_t;
int detect_memsize(void* p);
int detect_process(void* p);
#ifndef REAL_TYPE
// ????????
#define USEFLOT
#ifdef USEFLOT
#define REAL_TYPE double
#else
#define REAL_TYPE int
#endif // USEFLOT
#endif
#define CHI_DIST(a, b, out)  { out = (a) + (b); (out) = (out) ? (((a)-(b))*((a)-(b))/(out)) : 0; }
#define ABS_DIST(a, b, out)  { (out) = ((a)>(b)) ? ((a)-(b)) : ((b)-(a)); }
#define RECOGNITION_0 (0)  /* ???ï¿½ï¿½??????????????????????????????11?????????????0????????????80*80???????????64000??????ï¿½ï¿½??????????                                    ????????????????1-10??????????????????ï¿½ï¿½??0???????50000??????0.52??????0.52???????????ï¿½ï¿½??0.52??????*/
#define RECOGNITION_1 (1)  /* ???ï¿½ï¿½?????????????????????????????????????????????????11????????????2????????????80*80??????                                               ?????64000??????????????ï¿½ï¿½??0???????288550??????0.52??ï¿½ï¿½??0.52???????????????0.52??????*/
#define RECOGNITION_2 (2)  /* ???ï¿½ï¿½???????????????????????????????1-10?????????????????11????????????2????????????80*80???????????64000???                                                     ????????????????ï¿½ï¿½??0???????169000 ??????0.52??ï¿½ï¿½??0.52???????????????0.52??????*/
#define RECOGNITION_3 (3)  /* ???ï¿½ï¿½????PC????,??????????1-600???????????????????????????????????????????????6?????????????0???????                                    ??????100*100???????????2243????????????ï¿½ï¿½??0???????8000??????0.51?????ï¿½ï¿½????0.51?????????ï¿½ï¿½??0.51??                                    ??????*/
#define RECOGNITION_4 (4)  /* ???ï¿½ï¿½??????????????????ï¿½ï¿½???ï¿½ï¿½????????????????????????????????9????????????100*100???ï¿½ï¿½????100000???????                                     ??????????? */
#define RECOGNITION_5 (5)  /* ???ï¿½ï¿½????PC????,??????????1-600???????????????????????????????????????????????12?????????????0?????                                   ??????100*100???????????2243????????????ï¿½ï¿½??0???????12154??????0.52?????ï¿½ï¿½????0.52?????????ï¿½ï¿½??0.52                                       ?????????ï¿½ï¿½?????????????????????  */
#define RECOGNITION_6 (6)  /* ??ï¿½ï¿½???????????????ï¿½ï¿½???????????????????ï¿½ï¿½????????????ï¿½ï¿½?????????2.??????????????????3???????????                                      ?2.?????????5900????????????ï¿½ï¿½??0???????11719??????0.52?????ï¿½ï¿½????0.52???????ï¿½ï¿½?????????*/
#define RECOGNITION_7 (7)  /* ??ï¿½ï¿½??????????????????????????????ï¿½ï¿½???????????????2165??????????????13?????????????3????????????                                   25000????ï¿½ï¿½??0??????0.52?????ï¿½ï¿½?ï¿½ï¿½??0.52????????????????*/
typedef enum feature_type {
  LBP8BIN = 0,          /* 8??bin??lbp????*/
  LBP59BIN,             /* 59??bin??lbp????*/
  PPLBP8BIN,            /* ??????????8??bin??lbp????*/
  PPLBP59BIN,           /* ??????????59??bin??lbp????*/
  BOOSTLBP59BIN,        /* ????boost????????59??bin??lbp???????????????*/
  PPBOOSTLBP59BIN,      /* ????boost????????59??bin??lbp???????????????*/
  PPBOOSTLBPXBIN,       /* ????boost????????X??bin??lbp?????????100??100?????*/
  PPBOOSTLBPXBINFIX,    /* ????boost????????X??bin??lbp????(fix bug)?????100??100?????*/
  PPLBPXBIN,            /* ?????boost????????59??bin??lbp?????????100??100?????*/
  LBP59RECTBIN,         /* ????????????ï¿½ï¿½???????????100??100?????*/
  PPLBP59BIN2,
  PPLGBP8BINCOMPRESS,
  BOOSTLBPXBIN,         /* ????boost????????X??bin??lbp?????????100??100????????PP??*/
  PPBOOSTLBPXBINABS,    /* ????boost????????X??bin??lbp?????????100??100???????????????????????*/
  EDGEGRADLBP,          /* EDGEGRADLBP?????????100??100???????????????????????*/
  BOOSTEDGEGRADLBP,     /* ????boost????????EDGEGRADLBP?????????100??100???????????????????????*/
  MAX_FEAT
} feature_type;
typedef enum distance_type {
  CPM_CHISQUARE,       /* ???????? */
  CPM_COSINE,          /* ??????? */
  CPM_HISTINTER,       /* ???????????? */
  CPM_ABSSUB           /* ?????????? */
} distance_type;
/* ????? */
typedef struct RECOALGO {
  int feat_type;    /* ???????? */
  int feat_dims;    /* ??????????????? */
  int std_height;   /* ??????????0???????? */
  int std_width;    /* ??????????0???????? */
  int std_cn;       /* ???????????? */
  int is_dist;      /* 1????????0???????? */
  double dist_min;  /* ??????ï¿½ï¿½?????????????????0~1??? */
  double dist_max;  /* ???????????????(?????-dist_min)/(dist_max-dist_min) */
  double dist_th;   /* ??????? */
  int dist_type;    /* ???????? */
}
RECOALGO;
typedef struct pplight_param_t {
  int height, width, srcstep, srccn, dststep;
  const unsigned char* src;
  int dstlen;
  unsigned char* dst;
  int buflen;
  char* buf;
} pplight_param_t;
int pplight_memsize(void* p0);
int pplight_process(void* p0);

// bgmodel µÄ¹¤×÷Ä£Ê½
#define BGMODEL_INIT    0 // ÖØÐÂ³õÊ¼»¯
#define BGMODEL_UPDATE  1 // ¸üÐÂ±³¾°
typedef int FIX16;
typedef struct bgmodel {
  // ÊäÈë
  int height, width, step, nChannels; // ÊäÈë
  int mode;             // ¹¤×÷Ä£Ê½ BGMODEL_INIT »ò BGMODEL_UPDATE
  unsigned char* frame; // ÊäÈëµÄµ±Ç°Ö¡
  int     update_thd;   // ¸üÐÂãÐÖµ È¡Öµ·¶Î§ 0~255  ½¨ÒéÖµ 10
  FIX16   update_rate;  // ¸üÐÂÂÊ   È¡Öµ·¶Î§ 0~2^13
  // Êä³ö
  unsigned char* background;   //RGB3Í¨µÀ±³¾°Í¼Ïñ [h,w,w*3]
  unsigned char* foreground;   //¶þÖµµ¥Í¨µÀÇ°¾°   [h,w,w]
  // ÊäÈëÊäÈë
  char* buf;            // ¸üÐÂµÄÊý¾ÝÇø£¬¼°ÁÙÊ±»º³åÇø
  int   buflen;         // ¸üÐÂµÄÊý¾ÝÇø£¬¼°ÁÙÊ±»º³åÇøµÄ×Ö½Ú×Ü´óÐ¡
}
bgmodel;
int bgmodel_memsize(bgmodel* b);
  int bgmodel_process(bgmodel* b);
#define ARRSIZE(_T, _N)   (sizeof(_T)*(_N))
// ???lbp????
// select_type - ??????????? 0-?????????1-2443??2-2165??????????????????
typedef struct lbpex_param_t {
  //????
  int height, width, srcstep, srccn;
  const unsigned char* src;
  int do_pp; //????? pplight
  int fillborder; //??????????????????
  //lbp????
  int select_type;
  int isall;
  //int* lbp_Feature_idyw; // ??????????????????len=[featurelen]
  //???
  int featurelen;
  unsigned char* feature;
  int buflen;
  unsigned char* buf;
} lbpex_param_t;
int lbpex_memsize(void* p0);
int lbpex_process(void* p0);
//image_feature_memsize
//321
//4 0
//567
static const IPOINT icvCodeDeltas[8] =
{ {1, 0}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}, { -1, 1}, {0, 1}, {1, 1} };
#undef CC_READ_CHAIN_POINT
#define CC_READ_CHAIN_POINT( _pt, code )  (ASSERT( (code & ~7) == 0 ), _pt.x += icvCodeDeltas[(int)(code)].x, _pt.y += icvCodeDeltas[(int)(code)].y )
#define THRESH_OTSU(H, W, A, AL, B, BL, inv) otsu_histeq(H, W, A, AL, B, BL, inv);
#define THRESH_RATE(H, W, A, AL, B, BL, M, ML, thdrate) cvThreshold(H, W, A, AL, B, BL, GetThreshVal_rate(H, W, A, AL, M, ML, thdrate), 255, CC_THRESH_BINARY);
int imcalchist(int h, int w, const unsigned char* a, int al, int ai);

//int asmfit_memsize(void* p0);
//int asmfit_process(void* p0);
// 2D piecewise affine warp
typedef struct tagAAMPAW {
  int pixTri;
  double alpha;
  double belta;
  double gamma;
}
AAMPAW;
typedef struct tagIMAGE11 {
  int height;
  int width;
  int cn;
  unsigned char* data;
}
IMAGE11, *PIMAGE;
typedef struct tagAAM {
  double* AllShapes;
  double* MeanShape;
  AAMPAW* paw;
  PIMAGE* AllImages;
  int* tris; // ?????? tris[i*3+j]
  int* edges; // ??? edges[i*2+j]
  // ???????
  int* mask;
  int height;
  int width;
  int step;
  int nSamples;
  int nPoints;
  int tri_num;
  int edge_num;
  int pix_num;
}
AAM, *PAAM;
AAM* new_aam(const char* txtfile);
void del_aam(AAM* paam);
typedef struct delaunay_t {
  const FPOINT* pt;
  int point_num;
  int* edges;
  int edge_num;
  int* tris;
  int tri_num;
  char* buf;
  int buflen;
} delaunay_t;
enum { CP_LZF };
typedef struct comp_param_t {
  int buflen, srclen, dstlen, mode;
  char* buf;
  char* src;
  char* dst;
} comp_param_t;
#define MEMCAT(_T, _D, _DN, _S, _SN)   _D=(_T*)memcat(_D, (_DN)*sizeof(_T), _S, (_SN)*sizeof(_T))
CC_INLINE void* memcat(void* pDest, size_t Len, const void* pSrc, size_t Len2)
{
  pDest = prealloc(pDest, Len + Len2);
  memmove((char*)pDest + Len, pSrc, Len2);
  return pDest;
}

//|xa, ya, za|
//|xb, yb, zb|
//|xc, yc, zc|
//#define DET3(xa, ya, za, xb, yb, zb, xc, yc, zc) (xa*yb*zc-xa*yc*zb+yc*xb*za+xc*ya*zb-yb*xc*za-xb*ya*zc)

#define DET1(A00)  (A00)
#define DET2(A00,A01,A10,A11)   (+(A00)*DET1(A11)-(A01)*DET1(A10))
#define DET3(A00,A01,A02,A10,A11,A12,A20,A21,A22)   (+(A00)*DET2(A11,A12,A21,A22)-(A01)*DET2(A10,A12,A20,A22)+(A02)*DET2(A10,A11,A20,A21))
#define DET3x(x, y, z) DET3((x)[0], (y)[0], (z)[0], (x)[1], (y)[1], (z)[1], (x)[2], (y)[2], (z)[2])
//#define DET3x(x, y, z) DET3((x)[0], (x)[1], (x)[2], (y)[0], (y)[1], (y)[2], (z)[0], (z)[1], (z)[2])
// Ax=b   xi=Di/D  D=det(A) Di=det(Ai) 
CC_INLINE double slove3(const double* A, const double* b, double* x)
{
  double d = DET3x(A, A + 3, A + 6);
  x[0] = DET3x(b, A + 3, A + 6) / d;
  x[1] = DET3x(A + 0, b, A + 6) / d;
  x[2] = DET3x(A + 0, A + 3, b) / d;
  return d;
}
CC_INLINE double slove3n(const double* A, const double* b, double* x, int n)
{
  int i;
  for (i = 0; i < n; ++i) {
    slove3(A, b + i * 3, x + i * 3);
  }
  return 0;
}
CC_INLINE int popcnt_lookup2(unsigned int u)
{
  static const uchar poptable[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
      4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
  };
  return poptable [u & 0xFF] + poptable [(u >> 8) & 0xFF] + poptable [(u >> 16) & 0xFF] + poptable [(u >> 24) & 0xFF];
}
CC_INLINE int hamming_distance(const void* a, const void* b, int n) {
  int i, n2 = n/4, dis=0;
  const unsigned int* ua = (const unsigned int*)a;
  const unsigned int* ub = (const unsigned int*)b;
  const unsigned char* ca = (const unsigned char*)(ua+n2);
  const unsigned char* cb = (const unsigned char*)(ub+n2);
  for (i=0; i<n2; ++i) {
    dis += popcnt_lookup2(ua[i]^ub[i]);
  }
  for (i=n2*4; i<n; ++i) {
    dis += popcnt_lookup2(ca[i]^cb[i]);
  }
  return dis;
}
CC_INLINE int imcvttype(img_t* dst, TypeId dsttype, const img_t* src, TypeId srctype) {
  int dst_elemsize = dsttype>>16;
  img_t im[1] = {0};
  if (dst==src) {
    imclone2(src, im);
    src = im;
  }
  imsetsize(dst, src->h, src->w, dst_elemsize, src->f);
  arrcvt2d(dst->tt.data, dsttype, dst->s, src->tt.data, srctype, src->s, src->h, src->w);
  imfree(im);
  return 0;
}

CC_INLINE int imchannels(const img_t* im) {
  return CC_MAT_CN(im);
}
CC_INLINE bool imempty(const img_t* im) {
  return NULL==im || (0 == im->h*im->w*im->c*im->f);
}

int CPM_FaceDetect( const void* cas, int height, int width, const unsigned char* data, int datastep,
                   const unsigned char* mask, int maskstep, double sc, double ssmin, double ssmax, double ss, int stepxy,
                   double thd, int mincnt, XRECT* B, int B_len );


double invert( const img_t* src, img_t* dst, int method );
CC_IMPL double
cvInvert(const img_t* src, img_t* dst, int method);

double
icvInvert2(const void* src, int src_step, void* dst, int dst_step, int rows, int cols, TypeId type, int method);

//! matrix decomposition types
typedef enum {
    /** Gaussian elimination with the optimal pivot element chosen. */
    CC_DECOMP_LU       = 0,
    CC_LU       = 0,
    /** singular value decomposition (SVD) method; the system can be over-defined and/or the matrix
    src1 can be singular */
    CC_DECOMP_SVD      = 1,
    CC_SVD      = 1,
    /** eigenvalue decomposition; the matrix src1 must be symmetrical */
    CC_DECOMP_EIG      = 2,
    CC_SVD_SYM         = 2,
    /** Cholesky \f$LL^T\f$ factorization; the matrix src1 must be symmetrical and positively
    defined */
    CC_DECOMP_CHOLESKY = 3,
    CC_CHOLESKY = 3,
    /** QR factorization; the system can be over-defined and/or the matrix src1 can be singular */
    CC_DECOMP_QR       = 4,
    CC_QR       = 4,
    /** while all the previous flags are mutually exclusive, this flag can be used together with
    any of the previous; it means that the normal equations
    \f$\texttt{src1}^T\cdot\texttt{src1}\cdot\texttt{dst}=\texttt{src1}^T\texttt{src2}\f$ are
    solved instead of the original system
    \f$\texttt{src1}\cdot\texttt{dst}=\texttt{src2}\f$ */
    CC_DECOMP_NORMAL   = 16,
    CC_NORMAL   = 16,
} CDecompTypes;


