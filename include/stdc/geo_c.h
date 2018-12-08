
#ifndef _GEO_C_H_
#define _GEO_C_H_

#include "stdc.h"
#include "error_c.h"
#include <math.h>
#include <float.h>

// 根据父容器来定位:
typedef enum {
  LF_widthFillParent = 0,
  LF_alighParentLeft = 1, // 左对齐
  LF_alighParentRight = 2, //右对齐
  LF_Hcenter = 3, //水平居中
  LF_heightFillParent = 0,
  LF_alighParentTop = 1 << 4, //顶端对齐
  LF_alighParentBottom = 2 << 4, //底部对齐
  LF_Vcenter = 3 << 4, //垂直居中
} LayoutFlags;

//根据兄弟组件来定位
// 右面的属性值为兄弟组件的id
// 左边 layout_toLeftOf
// 右边 layout_toRightOf
// 上方 layout_above
// 下方 layout_below
// 对齐上边界 layout_alignTop
// 对齐下边界 layout_alignBottom
// 对齐左边界 layout_alignLeft
// 对齐右边界 layout_alignRight

CC_INLINE int iRectLayout1D(int l, int r, int w, int fmt, int gap, int* pl, int* pr, int* pl2, int* pr2) {
  switch (fmt) {
  case LF_widthFillParent:
    *pl = l;
    *pr = r;
    *pl2 = 0;
    *pr2 = 0;
    return 0;
  case LF_alighParentLeft:
    *pl = l;
    *pr = l + w;
    *pl2 = l + w + gap;
    *pr2 = r;
    return 1;
  case LF_alighParentRight:
    *pl = r - w;
    *pr = r;
    *pl2 = l;
    *pr2 = r - w - gap;
    return 1;
  case LF_Hcenter:
    *pl = l + (r - l - w) / 2;
    *pr = *pl + w;
    *pl2 = 0;
    *pr2 = 0;
    return 0;
  }
  return 0;
}
CC_INLINE int iRectLayout(IRECT rc, int cx, int cy, UINT fmt, int gap, IRECT* out, IRECT* rc2)
{
  int retx, rety;
  *out = *rc2 = rc;
  retx = iRectLayout1D(rc.l, rc.r, cx, fmt & 0x07, gap, &out->l, &out->r, &rc2->l, &rc2->r);
  rety = iRectLayout1D(rc.t, rc.b, cy, (fmt >> 4) & 0x7, gap, &out->t, &out->b, &rc2->t, &rc2->b);
  return retx || rety;
}

// 文本格式化
typedef enum {
  TF_TOP = 0, // 顶对齐
  TF_LEFT = 0, // 左对齐
  TF_CENTER = 1, // 水平居中
  TF_RIGHT = 2, // 右对齐
  TF_VCENTER = 4, // 垂直居中 只对DT_SINGLELINE有效
  TF_BOTTOM = 8, // 底对齐
  TF_RightToLeft = 0x00000100,//从右至左输出文本 
  TF_Vertical = 0x00000200,//垂直方向输出文本 
  TF_NoFitBlackBox = 0x00000400,
  TF_DisplayFormatControl = 0x00002000,
  TF_NoFontFallback = 0x00040000,
  TF_MeasureTrailingSpaces = 0x00080000,
  TF_NoWrap = 0x00100000,//不自动换行 
  TF_LineLimit = 0x00200000,
  TF_NoClip = 0x00400000,//不使用裁剪
  TF_WrapWidgetWidth = (1 << 27), // 在窗口部件的当前宽度自动换行（这是默认的）。默认在空白符号处自动换行，这可以使用setWrapPolicy()来改变。
  TF_WrapFixedColumnWidth = (1 << 28), // 从窗口部件左侧开始的固定数量的列数自动换行。列数可以通过wrapColumnOrWidth()设置。如果你需要使用等宽文本在设备上显示很好的格式文本，这是很有用的，例如标准的VT100终端，你可以把wrapColumnOrWidth()设置为80。 
  TF_WrapAtWhiteSpace = (1 << 29), // 在空白符号处（空格或者换行）自动换行。否则是任何
  TF_WrapAnyWhere = (1 << 31) // 在任何情况下自动换行，包括单字中。
} TextFormat_;

// 将矩形框对齐//裁剪
CC_INLINE int iRectAlign(IRECT rc, int cx, int cy, UINT uFmt, IRECT* out)
{
  *out = iRECT2(rc.l, rc.t, cx, cy);

  if (TF_CENTER & uFmt) {
    out->l = rc.l + (RCW(&rc) - cx) / 2;
  }

  if (TF_VCENTER & uFmt) {
    out->t = rc.t + (RCH(&rc) - cy) / 2;
  }

  if (TF_RIGHT & uFmt) {
    out->l = rc.r - cx;
  }

  if (TF_BOTTOM & uFmt) {
    out->t = rc.b - cy;
  }

  if ((TF_CENTER | TF_RIGHT | TF_LEFT) & uFmt) {
    out->r = out->l + cx;
  }

  if ((TF_VCENTER | TF_BOTTOM) & uFmt) {
    out->b = out->t + cy;
  }
  if (!(uFmt&TF_NoClip)) {
    *out = iRectInter(*out, rc);
  }

  return 0;
}
CC_INLINE int fRectAlign(FRECT rc, float cx, float cy, UINT uFmt, FRECT* out)
{
  *out = fRECT2(rc.l, rc.t, cx, cy);

  if (TF_CENTER & uFmt) {
    out->l = rc.l + (RCW(&rc) - cx) / 2;
  }

  if (TF_VCENTER & uFmt) {
    out->t = rc.t + (RCH(&rc) - cy) / 2;
  }

  if (TF_RIGHT & uFmt) {
    out->l = rc.r - cx;
  }

  if (TF_BOTTOM & uFmt) {
    out->t = rc.b - cy;
  }

  if ((TF_CENTER | TF_RIGHT | TF_LEFT) & uFmt) {
    out->r = out->l + cx;
  }

  if ((TF_VCENTER | TF_BOTTOM) & uFmt) {
    out->b = out->t + cy;
  }
  if (!(uFmt&TF_NoClip)) {
    *out = fRectInter(*out, rc);
  }

  return 0;
}
// 绕点(x, y) 缩放矩形
CC_INLINE int iRectAddMulAdd(IRECT rc, double x, double y, double ox, double oy, double tx, double ty, IRECT* out) {
  out->l = (int)((rc.l + x)*ox + tx);
  out->r = (int)((rc.r + x)*ox + tx);
  out->t = (int)((rc.t + y)*oy + ty);
  out->b = (int)((rc.b + y)*oy + ty);
  return 0;
}
// 内嵌等比缩放
// 在矩形rc中切出宽高比为cx:cy的最大矩形
CC_INLINE double iRectScaling(IRECT rc, int cx, int cy, IRECT* out)
{
  int sw = cx, sh = cy;
  int dw = RCW(&rc), dh = RCH(&rc);
  int tw = dw, th = dh;
  int x = 0, y = 0;
  tw = sw * dh;
  th = sh * dw;

  if (cx <= 0 || cy <= 0) {
    return 0;
  }

  if (tw > th) {
    tw = dw, th /= sw;
    y = (dh - th) / 2;
  }
  else {
    th = dh, tw /= sh;
    x = (dw - tw) / 2;
  }

  out->l = x + rc.l;
  out->r = out->l + tw;
  out->t = y + rc.t;
  out->b = out->t + th;
  return th*1. / sh;
}
// 把矩形框切分成大小相同的块，块按矩阵排列
CC_INLINE int iRectMatrix(IRECT rc, int row, int col, int gapx, int gapy, int fmt, int n, IRECT* prcs)
{
  int i;
  int cx, cy, h = RCH(&rc) - (row - 1)*gapy, w = RCW(&rc) - (col - 1)*gapx;
  cx = w / col + gapx, cy = h / row + gapy;
  iRectAlign(rc, cx*col + gapx*(col - 1), cy*row + gapy*(row - 1), fmt, &rc);

  for (i = 0; i < n; ++i) {
    prcs[i].l = rc.l + (cx * (i % col));
    prcs[i].r = prcs[i].l + cx - gapx;
    prcs[i].t = rc.t + (int)(cy * (i / col));
    prcs[i].b = prcs[i].t + cy - gapy;
  }

  return 0;
}
CC_INLINE double iRectSplit(IRECT rcIn, BOOL horz, float gap, int count, float* v, IRECT* pout) {
  double x, w, sum1 = 0, ss;
  int len = horz ? RCW(&rcIn) : RCH(&rcIn);
  int i;
  if (len <= 0) { return 0; }
  for (i = 0; i<count; ++i) {
    sum1 += v[i];
  }
  sum1 = MAX(1, sum1);
  ss = (len - gap*(count - 1)) / sum1;
  x = horz ? rcIn.l : rcIn.t;
  for (i = 0; i<count; ++i) {
    w = (ss*v[i]);
    if (horz) {
      pout[i] = iRECT((int)x, rcIn.t, (int)(x + w), rcIn.b);
    }
    else {
      pout[i] = iRECT(rcIn.l, (int)x, rcIn.r, (int)(x + w));
    }
    v[i] = (float)w;
    x += w + gap;
  }
  return ss;
}
CC_INLINE float iRectSplitUpdate(BOOL horz, float gap, int count, float* v, float max_v, const IRECT* rc) {
  float sum1 = 0, ss;
  int i;
  UNUSED(gap);
  for (i = 0; i<count; ++i) {
    if (v[i] <= max_v) {
      if (horz) {
        sum1 += horz ? RCW(rc + i) : RCH(rc + i);
      }
    }
  }
  if (sum1>0) {
    ss = 1 / sum1;
    for (i = 0; i<count; ++i) {
      if (v[i] <= max_v) {
        v[i] = (horz ? RCW(rc + i) : RCH(rc + i))*ss;
      }
    }
  }
  return ss;
}
CC_INLINE int iRectMatrix2(IRECT rcIn, int cx, int cy, int gapx, int gapy, int n, IRECT* rc)
{
  int i, col, row;
  int h = RCH(&rcIn) + gapy, w = RCW(&rcIn) + gapx;
  cx += gapx, cy += gapy;
  col = w / cx, row = h / cy;
  n = MIN(n, row * col);

  for (i = 0; i < n; ++i) {
    rc[i].l = rcIn.l + (int)(cx * (i % col));
    rc[i].r = rc[i].l - gapx + (int)cx;
    rc[i].t = rcIn.t + (int)(cy * (i / col));
    rc[i].b = rc[i].t - gapy + (int)cy;
  }

  return 0;
}
CC_INLINE int iRectMatrix2_HitText(IRECT rcIn, int cx, int cy, int gapx, int gapy, int n, int x, int y)
{
  int i, col, row;
  int h = RCH(&rcIn) + gapy, w = RCW(&rcIn) + gapx;
  IRECT rc;
  cx += gapx, cy += gapy;
  col = w / cx, row = h / cy;
  n = MIN(n, row * col);

  for (i = 0; i < n; ++i) {
    rc.l = rcIn.l + (int)(cx * (i % col));
    rc.r = rc.l - gapx + (int)cx;
    rc.t = rcIn.t + (int)(cy * (i / col));
    rc.b = rc.t - gapy + (int)cy;
    if (iPtInRect(&rc, x, y)) {
      return i;
    }
  }

  return -1;
}
// 从矩形框上边切出高为d的一块, 如果d<0则从下边切
CC_INLINE int iRectCutT(IRECT* rc, int d, int gap, IRECT* out)
{
  if (d >= 0) {
    *out = *rc, out->b = rc->t + d, rc->t = out->b + gap;
  }
  else {
    *out = *rc, out->t = rc->b + d, rc->b = out->t - gap;
  }

  return rc->t <= rc->b;
}
// 从矩形框左边切出宽为d的一块, 如果d<0则从右边切
CC_INLINE int iRectCutL(IRECT* rc, int d, int gap, IRECT* out)
{
  if (d >= 0) {
    *out = *rc, out->r = rc->l + d, rc->l = out->r + gap;
  }
  else {
    *out = *rc, out->l = rc->r + d, rc->r = out->l - gap;
  }

  return rc->l <= rc->r;
}
CC_INLINE int iRectCutB(IRECT* rc, int d, int gap, IRECT* out)
{
  *out = *rc, out->t = rc->b + d, rc->b = out->t - gap;
  return rc->t <= rc->b;
}
CC_INLINE int iRectCutR(IRECT* rc, int d, int gap, IRECT* out)
{
  *out = *rc, out->l = rc->r + d, rc->r = out->l - gap;
  return rc->l <= rc->r;
}
CC_INLINE int iRectImageText(IRECT rc, int n, ISIZE* sz, int gap, UINT uFmt, BOOL ishoriz, IRECT* prc)
{
  ISIZE szall = iSIZE(sz[0].w, sz[0].h);
  int i;

  if (ishoriz) {
    for (i = 1; i < n; ++i) {
      szall.w += sz[i].w + gap;
      szall.h = MAX(szall.h, sz[i].h);
    }

    iRectAlign(rc, szall.w, szall.h, uFmt, prc);

    for (i = n; --i > 0;) {
      iRectCutL(prc, -sz[i].w, gap, prc + i);
      iRectAlign(prc[i], sz[i].w, sz[i].h, uFmt, prc + i);
    }

    iRectAlign(prc[0], sz[0].w, sz[0].h, uFmt, prc);
  }
  else {
    for (i = 1; i < n; ++i) {
      szall.h += sz[i].h + gap;
      szall.w = MAX(szall.w, sz[i].w);
    }

    iRectAlign(rc, szall.w, szall.h, uFmt, prc);

    for (i = n; --i > 0;) {
      iRectCutT(prc, -sz[i].h, gap, prc + i);
      iRectAlign(prc[i], sz[i].w, sz[i].h, uFmt, prc + i);
    }

    iRectAlign(prc[0], sz[0].w, sz[0].h, uFmt, prc);
  }

  return 0;
}
// s 吸附距离
CC_INLINE int iRectCut(IRECT* rc, int cmd, int d, int gap, int x, int y, int s, IRECT* out)
{
  *out = *rc;
  switch (cmd) {
  case 'l':
  {
    if (d>0) {
      d = BOUND(d, s, RCW(rc));
      out->r = rc->l + d, rc->l = out->r + gap;
    }
    else {
      d = BOUND(d, -RCW(rc), -d);
      rc->l = rc->r + d, out->r = rc->l - gap;
    }
    return x > (out->r - s) && x < (rc->l + s) && y > rc->t && y < rc->b;
  }
  break;
  case 'r':
  {
    if (d>0) {
      d = BOUND(d, s, RCW(rc));
      out->l = rc->r - d, rc->r = out->l - gap;
    }
    else {
      d = BOUND(d, -RCW(rc), -d);
      rc->r = rc->l - d, out->l = rc->r + gap;
    }
    return x > (rc->r - s) && x < (out->l + s) && y > rc->t && y < rc->b;
  }
  break;
  case 't':
  {
    if (d>0) {
      d = BOUND(d, s, RCH(rc));
      out->b = rc->t + d, rc->t = out->b + gap;
    }
    else {
      d = BOUND(d, -RCH(rc), -d);
      rc->t = rc->b + d, out->b = rc->t - gap;
    }
    return y > (out->b - s) && y < (rc->t + s) && x > rc->l && x < rc->r;
  }

  break;
  case 'b':
  {
    if (d>0) {
      d = BOUND(d, s, RCH(rc));
      out->t = rc->b - d, rc->b = out->t - gap;
    }
    else {
      d = BOUND(d, -RCH(rc), -d);
      rc->b = rc->t - d, out->t = rc->b + gap;
    }
    return y > (rc->b - s) && y < (out->t + s) && x > rc->l && x < rc->r;
  }
  break;
  }

  return 0;
}
// 求矩形框的邻居矩形框
CC_INLINE int iRectNeighbour(IRECT* rc, int cmd, int d, int gap)
{
  if ('r' == cmd) {
    rc->r = rc->l - gap, rc->l = rc->r - d;
  }

  if ('l' == cmd) {
    rc->l = rc->r + gap, rc->r = rc->l + d;
  }

  if ('b' == cmd) {
    rc->b = rc->t - gap, rc->t = rc->b - d;
  }

  if ('t' == cmd) {
    rc->t = rc->b + gap, rc->b = rc->t + d;
  }

  return 0;
}

CC_INLINE int x_cover(int a, int an, int b, int bn, int fmt)
{
  if (an < bn) {
    if (TF_CENTER & fmt) {
      return b + (bn - an) / 2;
    }
    else if (TF_RIGHT & fmt) {
      return b + (bn - an);
    }
    else {
      return b;
    }
  }
  else {
    a = MAX(a, b + bn - an);
    a = MIN(a, b);
  }

  return a;
}

// 移动rc1去覆盖rc2，使移动距离最小，覆盖面积最大
CC_INLINE IPOINT iRectCover(IRECT rc1, IRECT rc2, IRECT* outrc1, int fmt)
{
  int fmtx = fmt, fmty = (TF_VCENTER & fmt) ? TF_CENTER : (TF_BOTTOM & fmt ? TF_RIGHT : 0);
  IPOINT pt = { 0, 0 };
  ISIZE sz = RCSZ(&rc1);
  pt.x = x_cover(rc1.l, RCW(&rc1), rc2.l, RCW(&rc2), fmtx);
  pt.y = x_cover(rc1.t, RCH(&rc1), rc2.t, RCH(&rc2), fmty);
  *outrc1 = iRECT3(pt, sz);
  return pt;
}

CC_INLINE IRECT iRectInter3(IRECT r1, IRECT r2, IRECT r3)
{
  return iRectInter(r1, iRectInter(r2, r3));
}
CC_INLINE int iRectIntersect(IRECT* prc, IRECT r1, IRECT r2)
{
  IRECT temp = { 0,0,0,0 };
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  // check for total exclusion
  if ((r1.r > r2.l) && (r1.l < r2.r) && (r1.b > r2.t) && (r1.t < r2.b)) {
    temp.l = (r1.l > r2.l) ? r1.l : r2.l;
    temp.r = (r1.r < r2.r) ? r1.r : r2.r;
    temp.t = (r1.t > r2.t) ? r1.t : r2.t;
    temp.b = (r1.b < r2.b) ? r1.b : r2.b;
  }
  if (prc) {
    *prc = temp;
  }
  return RCW(&temp) > 0 && RCH(&temp) > 0;
}
CC_INLINE IRECT iRectScale(IRECT rc, double t)
{
  IRECT rc1 = iRECT((int)(rc.l * t), (int)(rc.t * t), (int)(rc.r * t), (int)(rc.b * t));
  return rc1;
}
// a/b=c/d; a=?
CC_INLINE IRECT iRectZoom(const IRECT* b, const IRECT* c, const IRECT* d)
{
  return iRECT(b->l + (c->l - d->l) * RCW(b) / RCW(d),
    b->t + (c->t - d->t) * RCH(b) / RCH(d),
    b->r + (c->r - d->r) * RCW(b) / RCW(d),
    b->b + (c->b - d->b) * RCH(b) / RCH(d));
}
CC_INLINE IRECT iRectClip(const IRECT* pclip, int l, int t, int r, int b)
{
  IRECT rc = iRECT(l, t, r, b);
  if (pclip) {
    rc = iRectInter(rc, *pclip);
  }
  return rc;
}
CC_INLINE IRECT iRectUnion(IRECT r1, IRECT r2)
{
  IRECT temp = { 0,0,0,0 };
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  temp.l = (r1.l < r2.l) ? r1.l : r2.l;
  temp.r = (r1.r > r2.r) ? r1.r : r2.r;
  temp.t = (r1.t < r2.t) ? r1.t : r2.t;
  temp.b = (r1.b > r2.b) ? r1.b : r2.b;
  return temp;
}
typedef struct tRIVERTEX {
  double x, y;
  COLOR clr;
} tRIVERTEX;
#define RectClipR(rc, rc1)  *(rc) = iRectInter(rc, *(rc1))
#define RectClip(rc, l, t, r, b)  *(rc) = iRectInter(rc, iRECT(l, t, r, b))
typedef struct inter_info2_t {
  DPOINT pt;
  //int ee[2]; // ??? (ee[0]!=ee[1]) ????????????????????? ee[0] ee[1]
  double ts[2];
  double d;
} inter_info2_t;
// ???????
typedef struct inter_info_t {
  DPOINT pt;
  //int ee[2]; // ??? (ee[0]!=ee[1]) ????????????????????? ee[0] ee[1]
  double ts[2];
  double d;
  //int next, prev;
  int next_inter0; // ?????????
  int next_inter1; // ?????????
  int flag; // 0 ????????锟斤拷???-1??1????锟斤拷????
  int samp; // ??????????
} inter_info_t;
typedef struct iinter_info_t {
  IPOINT pt;
  //int ee[2]; // ??? (ee[0]!=ee[1]) ????????????????????? ee[0] ee[1]
  //int ts[2];
  int d;
  //int next, prev;
  int next_inter0; // ?????????
  int next_inter1; // ?????????
  int flag; // 0 ????????锟斤拷???-1??1????锟斤拷????
  int samp; // ??????????
} iinter_info_t;
typedef struct iINTER {
  IPOINT pt;
  //int ee[2]; // ??? (ee[0]!=ee[1]) ????????????????????? ee[0] ee[1]
  int ts[2];
  int d;
  //int next, prev;
  int next_inter0; // ?????????
  int next_inter1; // ?????????
  int flag; // 0 ????????锟斤拷???-1??1????锟斤拷????
  int samp; // ??????????
} iINTER;
typedef struct CONTOURINFO {
  int pos;
  int flag;
  int parent;
} CONTOURINFO;
typedef struct IVC {
  IPOINT* pt;
  CONTOURINFO* cc;
  int n, maxn, l, maxl;
} IVC;
typedef struct DVC {
  DPOINT* pt;
  CONTOURINFO* cc;
  int n, maxn, l, maxl;
} DVC;
typedef struct FVC {
  FPOINT* pt;
  CONTOURINFO* cc;
  int n, maxn, l, maxl;
} FVC;

#define VC_IS_HOLE(vc)   ((vc)->cc[(vc)->l-1].flag&CC_SEQ_FLAG_HOLE)

#define SETRECTD(rc, l, t, r, b) ((rc)->l=l, (rc)->t=t, (rc)->r=r, (rc)->b=b)
typedef struct {
  DPOINT* pt;
  int n;
} DSPOLYGON;
// polygon
typedef struct {
  DPOINT* pt;
  int* len;
  int n;
} DPOLYGON;
typedef struct {
  FPOINT* pt;
  int* len;
  int n;
} FPOLYGON;
typedef struct {
  IPOINT* pt;
  int* len;
  int n;
} IPOLYGON;
typedef struct {
  IPOINT* pt;
  int* len;
  int n;
} ipoly_t;
CC_INLINE int dPOLYGON_npt(const DPOLYGON* s)
{
  int i, m = 0;
  for (i = 0; i < s->n; ++i) {
    m += s->len[i];
  }
  return m;
}
CC_INLINE int ipoly_npt(const ipoly_t* s)
{
  int i, m = 0;
  for (i = 0; i < s->n; ++i) {
    m += s->len[i];
  }
  return m;
}
CC_INLINE int dPOLYGON_setsize(DPOLYGON* s, int n, int npt)
{
  REALLOC(int, s->len, n);
  REALLOC(DPOINT, s->pt, npt);
  memset(s->len, 0, sizeof(int)*n);
  s->n = n;
  return npt;
}
CC_INLINE int dPOLYGON_add(DPOLYGON* s, int isaddn, int addnpt, const DPOINT* pt)
{
  int npt = dPOLYGON_npt(s);
  isaddn = s->n ? (!!isaddn) : 1;
  if (isaddn) {
    int n = isaddn + s->n;
    REALLOC(int, s->len, n + 1);
  }
  if (addnpt) {
    npt += addnpt;
    REALLOC(DPOINT, s->pt, npt);
    if (pt) {
      MEMCPY(s->pt + npt - addnpt, pt, addnpt);
    }
  }
  return npt;
}
CC_INLINE int ipoly_setsize(ipoly_t* s, int n, int npt)
{
  REALLOC(int, s->len, n);
  REALLOC(IPOINT, s->pt, npt);
  memset(s->len, 0, sizeof(int)*n);
  s->n = n;
  return 0;
}
CC_INLINE int dPOLYGON_to_ipoly(const DPOLYGON* s, ipoly_t* c, double t)
{
  int i, npt = dPOLYGON_npt(s);
  ipoly_setsize(c, s->n, npt);
  MEMCPY(c->len, s->len, s->n);
  for (i = 0; i < npt; ++i) {
    c->pt[i].x = (int)(s->pt[i].x * t);
    c->pt[i].y = (int)(s->pt[i].y * t);
  }
  return 0;
}
CC_INLINE int ipoly_to_poly2d(const ipoly_t* s, DPOLYGON* c, double t)
{
  int i, npt = ipoly_npt(s);
  dPOLYGON_setsize(c, s->n, npt);
  MEMCPY(c->len, s->len, s->n);
  for (i = 0; i < npt; ++i) {
    c->pt[i].x = s->pt[i].x * t;
    c->pt[i].y = s->pt[i].y * t;
  }
  return 0;
}
CC_INLINE int dPOLYGON_setpts(DPOLYGON* s, int n, int npt, int* len, const DPOINT* pt)
{
  dPOLYGON_setsize(s, n, npt);
  memcpy(s->pt, pt, sizeof(DPOINT)*npt);
  memcpy(s->len, len, sizeof(int)*n);
  return 0;
}
CC_INLINE int dPOLYGON_setpts_f32(DPOLYGON* s, int n, int npt, int* len, const FPOINT* pt)
{
  int i;
  dPOLYGON_setsize(s, n, npt);
  for (i = 0; i<npt; ++i) {
    s->pt[i].x = pt[i].x;
    s->pt[i].y = pt[i].y;
  }
  memcpy(s->len, len, sizeof(int)*n);
  return 0;
}
CC_INLINE int dPOLYGON_pos(const DPOLYGON* s, int i)
{
  int j = 0, pos = 0;
  for (j = 0; j < s->n; ++j) {
    pos += s->len[j];
    if (pos >= i) {
      break;
    }
  }
  return j;
}
CC_INLINE int dPOLYGON_getspoly(DPOLYGON* s, int j, DSPOLYGON* out)
{
  int i, pos = 0;
  for (i = 0; i < s->n; ++i) {
    int npt = s->len[i];
    if (pos + npt > j) {
      out->pt = s->pt + pos;
      out->n = npt;
      return 1;
    }
    pos += npt;
  }
  return 0;
}
CC_INLINE int dPOLYGON_delpt(DPOLYGON* s, int i)
{
  int npt = dPOLYGON_npt(s);
  int pos = dPOLYGON_pos(s, i);
  memmove(s->pt + i, s->pt + i + 1, (npt - i - 1) * sizeof(DPOINT));
  s->len[pos]--;
  return 0;
}
CC_INLINE int dPOLYGON_addpt(DPOLYGON* s, int i, double x, double y)
{
  int npt = dPOLYGON_npt(s);
  int pos = dPOLYGON_pos(s, i);
  REALLOC(DPOINT, s->pt, npt + 1);
  memmove(s->pt + i + 1, s->pt + i, (npt - i - 1) * sizeof(DPOINT));
  s->pt[i].x = x, s->pt[i].y = y;
  s->len[pos]++;
  return 0;
}
CC_INLINE int dPOLYGON_addpts(DPOLYGON* s, int len, const DPOINT* pt)
{
  int npt = dPOLYGON_npt(s);
  REALLOC(int, s->len, s->n + 1);
  REALLOC(DPOINT, s->pt, npt + len);
  s->len[s->n++] = len;
  if (pt) {
    memcpy(s->pt + npt, pt, sizeof(DPOINT)*len);
  }
  return npt + len;
}
CC_INLINE int ipoly_addpts(ipoly_t* s, int len, const IPOINT* pt)
{
  int npt = ipoly_npt(s);
  REALLOC(int, s->len, s->n + 1);
  REALLOC(IPOINT, s->pt, npt + len);
  s->len[s->n++] = len;
  memcpy(s->pt + npt, pt, sizeof(IPOINT)*len);
  return 0;
}
CC_INLINE int dPOLYGON_free(DPOLYGON* s)
{
  FREE(s->pt);
  FREE(s->len);
  s->n = 0;
  return 0;
}
CC_INLINE int ipoly_free(ipoly_t* s)
{
  FREE(s->pt);
  FREE(s->len);
  s->n = 0;
  return 0;
}
enum {
  GEO_TRANS,
  GEO_SCALING,
  GEO_SKEWING,
  GEO_ROTATION
};
// returns squared distance between two 2D points with floating-point coordinates.
CC_INLINE double icvSqDist2D32f(FPOINT pt1, FPOINT pt2)
{
  double dx = pt1.x - pt2.x;
  double dy = pt1.y - pt2.y;
  return dx * dx + dy * dy;
}
/* Shape orientation */
enum {
  CC_CLOCKWISE = 1,
  CC_COUNTER_CLOCKWISE = 2
};
typedef DSEGMENT2 DLINE2;
typedef DSEGMENT3 DLINE3;
typedef struct DLINESEG2 {
  DPOINT a, b;
} DLINESEG2;
typedef struct FLINESEG2 {
  FPOINT a, b;
} FLINESEG2;
// ?????????????? a*x+b*y+c=0 ??????, ??? a>= 0
typedef struct DFLINE2 {
  double a, b, c;
} DFLINE2;
// ?????????????? a*x+b*y+c*z+d=0 ??????, ??? a>= 0
typedef struct DFLINE3 {
  double a, b, c, d;
} DFLINE3;
typedef struct DTRIANGLE2 {
  DPOINT2 p0, p1, p2;
} DTRIANGLE2;
typedef struct DTRIANGLE3 {
  DPOINT3 p0, p1, p2;
} DTRIANGLE3;
typedef struct DRECTANGLE {
  DPOINT2 p0, p1;
} DRECTANGLE;
typedef struct DQUADIX2 {
  DPOINT2 p0, p1, p2, p3;
} DQUADIX2;
typedef struct DQUADIX3 {
  DPOINT3 p0, p1, p2, p3;
} DQUADIX3;
typedef struct DCIRCLE {
  double x, y, r;
} DCIRCLE;
typedef struct FCIRCLE {
  float x, y, r;
} FCIRCLE;
typedef struct IELLIPSE {
  int x, y, ra, rb, angle;
} IELLIPSE;
typedef struct FELLIPSE {
  float x, y, ra, rb, angle;
} FELLIPSE;
typedef struct DELLIPSE {
  double x, y, ra, rb, angle;
} DELLIPSE;
typedef struct DSPHERE {
  double x, y, z, radius;
} DSPHERE;
typedef struct DHYPERSPHERE {
  DPOINT2 center;
  double radius;
} DHYPERSPHERE;
enum { SegmentPointCount = 2, LinePointCount = 2, RectanglePointCount = 2, BoxPointCount = 2, TrianglePointCount = 3, QuadixPointCount = 4, };
typedef struct DCIRCULAR_ARC {
  double x1, y1, x2, y2, cx, cy, px, py, angle1, angle2;
  int orientation;
} DCIRCULAR_ARC;
typedef struct DQUADRATIC_BEZIER2 {
  DPOINT2 p0, p1, p2;
} DQUADRATIC_BEZIER2;
typedef struct DQUADRATIC_BEZIER3 {
  DPOINT3 p0, p1, p2;
} DQUADRATIC_BEZIER3;
typedef struct DCUBIC_BEZIER2 {
  DPOINT2 p0, p1, p2, p3;
} DCUBIC_BEZIER2;
typedef struct DCUBIC_BEZIER3 {
  DPOINT3 p0, p1, p2, p3;
} DCUBIC_BEZIER3;
typedef struct DBEZIER_COEFFICIENTS2 {
  DPOINT2 p0, p1, p2, p3;
} DBEZIER_COEFFICIENTS2;
typedef struct DBEZIER_COEFFICIENTS3 {
  DPOINT3 p0, p1, p2, p3;
} DBEZIER_COEFFICIENTS3;
typedef struct DCURVE_POINT2 {
  DPOINT2 p0;
  double t;
} DCURVE_POINT2;
typedef struct DCURVE_POINT3 {
  DPOINT3 p0;
  double t;
} DCURVE_POINT3;
typedef DPOINT2 DVECTOR2;
typedef DPOINT3 DVECTOR3;
typedef struct DRAY2 {
  DPOINT2 origin;
  DVECTOR2 direction;
} DRAY2;
typedef struct DRAY3 {
  DPOINT3 origin;
  DVECTOR3 direction;
} DRAY3;
typedef struct DPLANE3 {
  double constant;
  DVECTOR3 normal;
} DPLANE3;
typedef struct DBOX2 {
  DPOINT2 p0, p1;
} DBOX2;
typedef struct DBOX3 {
  DPOINT3 p0, p1;
} DBOX3;
typedef struct DPOLYGON2 {
  DPOINT2* p;
  int n;
} DPOLYGON2;
typedef struct DPOLYGON3 {
  DPOINT3* p;
  int n;
} DPOLYGON3;
CC_INLINE int dPOLYGON2_free(DPOLYGON2* po)
{
  FREE(po->p);
  po->p = 0;
  po->n = 0;
  return 0;
}
CC_INLINE int dPOLYGON3_free(DPOLYGON3* po)
{
  FREE(po->p);
  po->p = 0;
  po->n = 0;
  return 0;
}
CC_INLINE int dPOLYGON2_setsize1(DPOLYGON2* po, int n)
{
  MYREALLOC(po->p, n);
  po->n = n;
  return 0;
}
CC_INLINE int dPOLYGON3_setsize1(DPOLYGON3* po, int n)
{
  MYREALLOC(po->p, n);
  po->n = n;
  return 0;
}
CC_INLINE int dPOLYGON2_insert(DPOLYGON2* po, int i, DPOINT2 pt)
{
  int n = po->n;
  dPOLYGON2_setsize1(po, po->n + 1);
  //MEMINST(po->p, po->n-1, i, &pt, 1);
  MEMMOVE(po->p + i, po->p + i + 1, n - i);
  po->p[i] = pt;
  return 0;
}
CC_INLINE int dPOLYGON2_add(DPOLYGON2* po, DPOINT2 pt)
{
  dPOLYGON2_setsize1(po, po->n + 1);
  po->p[po->n - 1] = pt;
  return 0;
}

typedef struct {
  QPOINT* pt;
  int* len;
  int n;
} QPOLYGON;
CC_INLINE int qPOLYGON_setsize(QPOLYGON* s, int n, int npt)
{
  REALLOC(int, s->len, n);
  REALLOC(QPOINT, s->pt, npt);
  memset(s->len, 0, sizeof(int)*n);
  s->n = n;
  return npt;
}

typedef struct MATRIX3X2 {
  int a, b, c, d, e, f;
}
MATRIX3X2;

#define MAKE_MATRIX3X2(m, A, B, C, D, E, F) \
  (  m.a=(int)(A), m.b=(int)(B), m.c=(int)(C), m.d=(int)(D), m.e=(int)(E), m.f=(int)(F)  )
#define MAKE_MATRIX3X2_TOINT(m, A, B, C, D, E, F) \
  m.a=_SHIFT_TOINT(A), m.b=_SHIFT_TOINT(B), m.c=_SHIFT_TOINT(C), m.d=_SHIFT_TOINT(D), m.e=_SHIFT_TOINT(E), m.f=_SHIFT_TOINT(F)
// |out_x|   |a c e|   |x|
// |out_y| = |b d f| * |y|
// |     |   |0 0 0|   |1|
#define MATRIX3X2_MUL_POINT(m, x, y, out_x, out_y)  out_x=m.a*(x)+m.c*(y)+m.e, out_y=m.b*(x)+m.d*(y)+m.f
//m=x*y
// |a c e|   |y.a y.c y.e|   |x.a x.c x.e|   | (a1*a2+c1*b2) ()
// |b d f| = |y.b y.d y.f| * |x.b x.d x.f| = |
// |0 0 0|   |y.0 y.0 y.0|   |x.0 x.0 x.0|   |
#define MATRIX3X2_MUL_MATRIX3X2(m, x, y)  MAKE_MATRIX3X2(m, \
    (int)_SHIFT_TOFLOAT((double)x.a * y.a + (double)x.b * y.c      ), \
    (int)_SHIFT_TOFLOAT((double)x.a * y.b + (double)x.b * y.d      ), \
    (int)_SHIFT_TOFLOAT((double)x.c * y.a + (double)x.d * y.c      ), \
    (int)_SHIFT_TOFLOAT((double)x.c * y.b + (double)x.d * y.d      ), \
    (int)_SHIFT_TOFLOAT((double)x.e * y.a + (double)x.f * y.c + y.e), \
    (int)_SHIFT_TOFLOAT((double)x.e * y.b + (double)x.f * y.d + y.f) )
//x*=y
#define MATRIX3X2_MUL(x, y) do { MATRIX3X2 _m; MATRIX3X2_MUL_MATRIX3X2(_m, x, y); x=_m; } while(0)
#define MATRIX3X2_GET_TRANSLATE(m, x, y)     MAKE_MATRIX3X2(m,1,0,0,1,x,y)
#define MATRIX3X2_GET_SCALE(m1, s, x, y)     MAKE_MATRIX3X2(m1,s,0,0,s, (s)*(x), (s)*(y) )
#define MATRIX3X2_INIT(m)  MAKE_MATRIX3X2(m, _SHIFT1,0,0,_SHIFT1,0,0)
#define MATRIX3X2_MOVE(m, x, y)  (m.e-=_SHIFT_TOINT(x), m.f-=_SHIFT_TOINT(y))
#define MATRIX3X2_ROTATE(m, rads)  {\
    int c=_SHIFT_TOINT(cos(rads)), s=_SHIFT_TOINT(sin(rads)); \
    MATRIX3X2 _x; MAKE_MATRIX3X2(_x, c,s,-s,c, 0, 0); MATRIX3X2_MUL(m, _x); }
// | c, s|
// |-s, c|
// | 0, 0|
#define MATRIX3X2_GET_ROTATE(m, rads, x, y) do { \
    int c=_SHIFT_TOINT(cos(rads)), s=_SHIFT_TOINT(sin(rads)); \
    MAKE_MATRIX3X2(m, c,s,-s,c,-(y)*c+(x)*s+x,-(y)*s+(x)*c+y); \
  } while(0)
#define MATRIX3X2_GET_ROTATE(m, rads, x, y) do { \
    int c=_SHIFT_TOINT(cos(rads)), s=_SHIFT_TOINT(sin(rads)); \
    MAKE_MATRIX3X2(m, c,s,-s,c,-(y)*c+(x)*s+x,-(y)*s+(x)*c+y); \
  } while(0)
#define MATRIX3X2_GET_STD_TOINT(m, c, s, s1, s2) \
  MAKE_MATRIX3X2_TOINT(m, c, -s, s, c, ((-s2.x*(c) - s2.y*(s))+s1.x), ((s2.x*(s) - s2.y*(c))+s1.y));
///////////////////////////////////
//typedef FPOINT FPOINT;
//typedef FSIZE FSIZE;
//FPOINT operator + (const FPOINT& a, const FPOINT& b) { return fPOINT(a.x + b.x, a.y + b.y); }
///////////////////////////////////
static double sdot2(const float* a, const float* b) {
  return a[0] * b[0] + a[1] * b[1];
}
static double snorm2(const float* a) {
  return sqrt(sdot2(a, a));
}
///////////////////////////////////

///////////////////////////////////
struct CRotatedRect
{
  FPOINT center; //< the rectangle mass center
  FSIZE size;    //< width and height of the rectangle
  float angle;    //< the rotation angle. When the angle is 0, 90, 180, 270 etc., the rectangle becomes an up-right rectangle.
};

CRotatedRect cRotatedRect(FPOINT center, FSIZE size, float angle) {
  CRotatedRect rr;
  rr.center = center;
  rr.size = size;
  rr.angle = angle;
  return rr;
}
static CRotatedRect cRotatedRect2(FPOINT _point1, FPOINT _point2, FPOINT _point3) {
  FPOINT _center;
  float vecs[2][2];
  _center.x = 0.5f * (_point1.x + _point3.x);
  _center.y = 0.5f * (_point1.y + _point3.y);
  vecs[0][0] = (_point1.x - _point2.x);
  vecs[0][1] = (_point1.y - _point2.y);
  vecs[1][0] = (_point2.x - _point3.x);
  vecs[1][1] = (_point2.y - _point3.y);
  // check that given sides are perpendicular
  CC_Assert(fabs(sdot2(vecs[0], vecs[1])) / (snorm2(vecs[0]) * snorm2(vecs[1])) <= FLT_EPSILON);

  // wd_i stores which vector (0,1) or (1,2) will make the width
  // One of them will definitely have slope within -1 to 1
  int wd_i = 0;
  if (fabs(vecs[1][1]) < fabs(vecs[1][0])) wd_i = 1;
  int ht_i = (wd_i + 1) % 2;

  float _angle = atan(vecs[wd_i][1] / vecs[wd_i][0]) * 180.0f / (float)CC_PI;
  float _width = (float)snorm2(vecs[wd_i]);
  float _height = (float)snorm2(vecs[ht_i]);

  return cRotatedRect(_center, fSIZE(_width, _height), _angle);
}

void cRotatedRect_points(CRotatedRect rr, FPOINT pt[])
{
  FPOINT center = rr.center;
  FSIZE size = rr.size;
  double _angle = rr.angle*CC_PI / 180.;
  float b = (float)cos(_angle)*0.5f;
  float a = (float)sin(_angle)*0.5f;

  pt[0].x = center.x - a*size.height - b*size.width;
  pt[0].y = center.y + b*size.height - a*size.width;
  pt[1].x = center.x + a*size.height - b*size.width;
  pt[1].y = center.y - b*size.height - a*size.width;
  pt[2].x = 2 * center.x - pt[0].x;
  pt[2].y = 2 * center.y - pt[0].y;
  pt[3].x = 2 * center.x - pt[1].x;
  pt[3].y = 2 * center.y - pt[1].y;
}

IRect cRotatedRect_boundingRect(CRotatedRect rr)
{
  FPOINT center = rr.center;
  FSIZE size = rr.size;
  FPOINT pt[4];
  cRotatedRect_points(rr, pt);
  IRect r = iRect(floor(MIN(MIN(MIN(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
    floor(MIN(MIN(MIN(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
    ceil(MAX(MAX(MAX(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
    ceil(MAX(MAX(MAX(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
  r.width -= r.x - 1;
  r.height -= r.y - 1;
  return r;
}



///////////////////////////////////
struct CKeyPoint
{
  FPoint pt; //!< coordinates of the keypoints
  float size; //!< diameter of the meaningful keypoint neighborhood
  float angle; //!< computed orientation of the keypoint (-1 if not applicable);
               //!< it's in [0,360) degrees and measured relative to
               //!< image coordinate system, ie in clockwise.
  float response; //!< the response by which the most strong keypoints have been selected. Can be used for the further sorting or subsampling
  int octave; //!< octave (pyramid layer) from which the keypoint has been extracted
  int class_id; //!< object class (if the keypoints need to be clustered by an object they belong to)
};
CKeyPoint cKeyPoint(FPoint _pt, float _size, float _angle = -1, float _response = 0, int _octave = 0, int _class_id = -1) {
  CKeyPoint k;
  k.pt = _pt;
  k.size = _size;
  k.response = _response;
  k.octave = _octave;
  k.class_id = _class_id;
  return k;
}
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
#endif // _GEO_C_H_
