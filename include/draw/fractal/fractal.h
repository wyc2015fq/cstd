
#ifndef _FRACTAL_H_
#define _FRACTAL_H_

#include "cstd.h"

/*
"Julia"
"SpotFormat"
"King"
*/
void* fractal_new(const char* name);
void fractal_update(void* sf, unsigned long StepTime_ms);
void fractal_draw(void* sf, int dst_height, int dst_width, unsigned char* dst, int dst_step, int cn);
void fractal_clear(void* sf);


typedef struct IDC {
  unsigned char* data;
  int height;
  int width;
  int step;
  int cn;
  int x, y; // pen pos
  color_t pen_color;
  int thickness;
  int line_type;
  // font
  int bItalic; // 协体
  int bBold; // 粗体
  char* strFontName; // 字体
  int font_cy;
  int font_cx;
}
IDC;

#define db_to_CvScalar(db)  cvScalar(GetRV(db), GetGV(db), GetBV(db), 0)
#define MoveTo(pDC, _x, _y)  ((pDC)->x=_x, (pDC)->y=_y)
#define LineTo(pDC, _x, _y)  DrawLine(pDC->height, pDC->width, pDC->tt.data, pDC->step, pDC->cn, \
    cvPoint(pDC->x, pDC->y), cvPoint(_x, _y), db_to_CvScalar(pDC->pen_color), pDC->thickness, pDC->line_type, 0), \
MoveTo(pDC, _x, _y)
#define Rectangle1(pDC, _l, _t, _r, _b)  DrawRectangle(pDC->height, pDC->width, pDC->tt.data, pDC->step, pDC->cn, \
    cvPoint(_l, _t), cvPoint(_r, _b), db_to_CvScalar(pDC->pen_color), pDC->thickness, pDC->line_type, 0)

#define Polygon(pDC, vpt, count) PolyLineM(pDC->height, pDC->width, pDC->tt.data, pDC->step, pDC->cn, \
    (CvPoint**)(&(vpt)), &count, 1, 0, db_to_CvScalar(pDC->pen_color), pDC->thickness, pDC->line_type, 0)

#define Polyline(pDC, vpt, count) PolyLineM(pDC->height, pDC->width, pDC->tt.data, pDC->step, pDC->cn, \
    (CvPoint**)(&(vpt)), &count, 1, 0, db_to_CvScalar(pDC->pen_color), pDC->thickness, pDC->line_type, 0)

#define PutText(pDC, y, x, str) \
  xDrawText(pDC->height, pDC->width, pDC->tt.data, pDC->step, pDC->cn, \
      y, x, pDC->strFontName, pDC->font_cy, pDC->font_cx, \
      pDC->bItalic, pDC->bBold, str, pDC->pen_color);

#define PUTPIX3(_PTR, cr, cg, cb)  ((_PTR)[0] = cb, (_PTR)[1] = cg, (_PTR)[2] = cr)
#define PUTPIX(_PTR, cb)   ((_PTR)[0] = cb)

#define SetPixel(pDC, _x, _y, _r, _g, _b) (3==(pDC)->cn) ? \
  (PUTPIX3((pDC)->tt.data+(int)(_y)*(pDC)->step+(int)(_x)*(pDC)->cn, _r, _g, _b)) : \
  (PUTPIX((pDC)->tt.data+(int)(_y)*(pDC)->step+(int)(_x)*(pDC)->cn, _b))

#define SetPixelV(pDC, _x, _y, _col)  SetPixel(pDC, _x, _y, GetRV(_col), GetGV(_col), GetBV(_col))
#define SetPixelI(pDC, _x, _y, _col)  SetPixel(pDC, _x, _y, GetRI(_col), GetGI(_col), GetBI(_col))

int InitDC(IDC* pDC, int height, int width, unsigned char* data, int step, int cn,
    color_t pen_color, int thickness);

void Canto(IDC* pDC, int ax, int ay, int bx, int by);
void KochFractal(IDC* pDC, double ax, double ay, double bx, double by);
void Koch(IDC* pDC, int m_Itn);
void FractalTree1(IDC* pDC);
void Hillbert(IDC* pDC, int m_Itn);

void MandlbrotJuliaDraw(int wid, int hgh, unsigned char* dst, int step, int cn,
    double x0, double y0, double x1, double y1,
    int red, int grn, int blu, int nn, double crr, double cii,
    int jd, int t_max, int c_modal);

void MandlbrotJuliaDraw2(int wid, int hgh, unsigned char* dst, int step, int cn, int Index);




#include <math.h>

#define must_inline static

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE  (1)
#endif

typedef struct scene_fractal_base {
  void (*update)(void* sf, unsigned long StepTime_ms);
  void (*draw)(void* sf, int dst_height, int dst_width, unsigned char* dst, int dst_step, int cn);
  void (*clear)(void* sf);
}
scene_fractal_base;

#define SCENE_FRACTAL_SET_FUNC(sf, _update, _draw, _clear) (\
    ((scene_fractal_base*)(sf))->update = (_update), \
    ((scene_fractal_base*)(sf))->draw = (_draw), \
    ((scene_fractal_base*)(sf))->clear = (_clear) )


#define DEFINE_SCENE_FRACTAL_BASE(BASE_CLASS) BASE_CLASS base
#define SCENE_FRACTAL_SET_CLASS(sf, _CLASS)  \
  SCENE_FRACTAL_SET_FUNC(sf, _CLASS##Update, _CLASS##DoDraw, _CLASS##Clear)

//颜色查表
extern const unsigned char* color_table;

#define sqr(_x)             ((_x)*(_x))
#define PackColor32Data(r8, g8, b8) (b8|(g8<<8)|(r8<<16))
#define PUTPIX3(_PTR, cr, cg, cb)  ((_PTR)[0] = cb, (_PTR)[1] = cg, (_PTR)[2] = cr)
#define PUTPIX(_PTR, cb)   ((_PTR)[0] = cb)
#define GETPIX3(_PTR, cr, cg, cb)  (cb = (_PTR)[0], cg = (_PTR)[1], cr = (_PTR)[2])
#define PUTPIX3C(_PTR, _COLOR)  PUTPIX3(_PTR, GetRV(_COLOR), GetGV(_COLOR), GetBV(_COLOR))
#define PUTPIX1C(_PTR, _COLOR)  PUTPIX(_PTR, GetBV(_COLOR))

enum { csMaxColorRoundValue = 512};

//颜色查表
extern const unsigned char* colorRound_table;

typedef struct CColorMover {
  double m_kR;  //颜色变化强度
  double m_kG;
  double m_kB;
  double m_R0;  //初始颜色
  double m_G0;
  double m_B0;
  double m_VR;  //颜色变化速度
  double m_VG;
  double m_VB;
}
CColorMover;

#if 0
static unsigned char CColorMovergetColor(const double Color0, const double k, const double Gene)
{
  long rd = (long)(Color0 + k * Gene);
  rd = rd % csMaxColorRoundValue;
  return colorRound_table[ rd ];
}
#else
#define CColorMovergetColor(_COLOR0, _K, _GENE) colorRound_table[((int)((_COLOR0)+(_K)*(_GENE)))%csMaxColorRoundValue]
#endif

#define getR8(cm, GeneR)  CColorMovergetColor(cm->m_R0, cm->m_kR, GeneR)
#define getG8(cm, GeneG)  CColorMovergetColor(cm->m_G0, cm->m_kG, GeneG)
#define getB8(cm, GeneB)  CColorMovergetColor(cm->m_B0, cm->m_kB, GeneB)

void CColorMoverInti(CColorMover* cm, double kMin, double kMax, double ColorVMin, double ColorVMax);
void CColorMoverUpdate(CColorMover* cm, unsigned long StepTime_ms);


typedef struct CPointMover { //点移动类型
  double m_Value;       //当前值
  double m_V;             //移动速度
  double m_MinValue;      //下界
  double m_MaxValue;      //上界
  double m_MinV;          //最小速度
  double m_MaxV;          //最大速度
}
CPointMover;

void CPointMoverInti(CPointMover* pm, double MinValue, double MaxValue, double MinV, double MaxV);

#define CPointMoverGetValue(pm) ((pm)->m_Value)
#define CPointMoverGetV(pm)     ((pm)->m_V)

#define CPointMoverGetRandomValue(pm) \
  ((pm)->m_Value=rand()*(1.0/RAND_MAX)*((pm)->m_MaxValue-(pm)->m_MinValue)+(pm)->m_MinValue)

#if 0
static void CPointMoverGetRandomV(CPointMover* pm)
{
  int old_sign = -1;

  //old_sign = ((pm)->m_V<0.) ? 1 : -1;
  if (pm->m_V < 0.) {
    old_sign = 1;
  }

  pm->m_V = old_sign * (rand() * (1.0 / RAND_MAX) * (pm->m_MaxV - pm->m_MinV) + pm->m_MinV);
}
#else
#define CPointMoverGetRandomV(pm) \
  (pm)->m_V=(((pm)->m_V<0.) ? 1. : -1.)*( rand()*(1.0/RAND_MAX)*((pm)->m_MaxV-(pm)->m_MinV)+(pm)->m_MinV);
#endif

void CPointMoverUpdate(CPointMover* pm, unsigned long StepTime_ms);
void CPointMoverGetNextCV(CPointMover* pm, double StepTime_ms);

typedef struct CScenePointFractal {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
  CPointMover* m_MoverList;
  CColorMover* m_ColorMoverList;
  int m_MoverListSize;
  int m_ColorMoverListSize;
}
CScenePointFractal;

void darkle(int height, int width, unsigned char* pdata, int byte_width, int cn, long del_color);

void CScenePointFractalUpdateMover(void* pf0, unsigned long StepTime_ms);

#undef SetPixel

#endif // _FRACTAL_H_
