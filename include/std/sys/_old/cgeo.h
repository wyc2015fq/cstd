
//                   ???
typedef struct IDIM {
  int len[2];
} IDIM;
CC_INLINE IDIM iDIM(int len0, int len1)
{
  IDIM sz;
  sz.len[0] = len0, sz.len[1] = len1;
  return sz;
}
#define VOP1_1(v, op, v1)       (v[0] op v1[0])
#define VOP1_2(v, op, v1)       (v[0] op v1[0], v[1] op v1[1])
#define VOP1_3(v, op, v1)       (v[0] op v1[0], v[1] op v1[1], v[2] op v1[2])
#define VOP1_4(v, op, v1)       (v[0] op v1[0], v[1] op v1[1], v[2] op v1[2], v[3] op v1[3])
#define VOP1a_1(v, op1, v1, op2, a)   (v[0] op1 v1[0] op2 (a))
#define VOP1a_2(v, op1, v1, op2, a)   (v[0] op1 v1[0] op2 (a), v[1] op1 v1[1] op2 (a))
#define VOP1a_3(v, op1, v1, op2, a)   (v[0] op1 v1[0] op2 (a), v[1] op1 v1[1] op2 (a), v[2] op1 v1[2] op2 (a))
#define VOP1a_4(v, op1, v1, op2, a)   (v[0] op1 v1[0] op2 (a), v[1] op1 v1[1] op2 (a), v[2] op1 v1[2] op2 (a), v[3] op1 v1[3] op2 (a))
#define VOP2_1(v, op1, v1, op2, v2)  (v[0] op1 v1[0] op2 v2[0])
#define VOP2_2(v, op1, v1, op2, v2)  (v[0] op1 v1[0] op2 v2[0], v[1] op1 v1[1] op2 v2[1])
#define VOP2_3(v, op1, v1, op2, v2)  (v[0] op1 v1[0] op2 v2[0], v[1] op1 v1[1] op2 v2[1], v[2] op1 v1[2] op2 v2[2])
#define VOP2_4(v, op1, v1, op2, v2)  (v[0] op1 v1[0] op2 v2[0], v[1] op1 v1[1] op2 v2[1], v[2] op1 v1[2] op2 v2[2], v[3] op1 v1[3] op2 v2[3])
#define VOP2a_1(v, op1, v1, op2, v2, a)  (v[0] op1 (v1[0] op2 v2[0]) * (a))
#define VOP2a_2(v, op1, v1, op2, v2, a)  (v[0] op1 (v1[0] op2 v2[0]) * (a), v[1] op1 (v1[1] op2 v2[1]) * (a))
#define VOP2a_3(v, op1, v1, op2, v2, a)  (v[0] op1 (v1[0] op2 v2[0]) * (a), v[1] op1 (v1[1] op2 v2[1]) * (a), v[2] op1 (v1[2] op2 v2[2]) * (a))
#define VOP2a_4(v, op1, v1, op2, v2, a)  (v[0] op1 (v1[0] op2 v2[0]) * (a), v[1] op1 (v1[1] op2 v2[1]) * (a), v[2] op1 (v1[2] op2 v2[2]) * (a), v[3] op1 (v1[3] op2 v2[3]) * (a))
#define VOP2b_1(v, op1, v1, op2, v2, b)  (v[0] op1 v1[0] op2 v2[0] * (b))
#define VOP2b_2(v, op1, v1, op2, v2, b)  (v[0] op1 v1[0] op2 v2[0] * (b), v[1] op1 v1[1] op2 v2[1] * (b))
#define VOP2b_3(v, op1, v1, op2, v2, b)  (v[0] op1 v1[0] op2 v2[0] * (b), v[1] op1 v1[1] op2 v2[1] * (b), v[2] op1 v1[2] op2 v2[2] * (b))
#define VOP2b_4(v, op1, v1, op2, v2, b)  (v[0] op1 v1[0] op2 v2[0] * (b), v[1] op1 v1[1] op2 v2[1] * (b), v[2] op1 v1[2] op2 v2[2] * (b), v[3] op1 v1[3] op2 v2[3] * (b))
#define VSET2(v, r, g)       (v[0] = r, v[1] = g)
#define VSET3(v, r, g, b)    (v[0] = r, v[1] = g, v[2] = b)
#define VSET4(v, r, g, b, a) (v[0] = r, v[1] = g, v[2] = b, v[3] = a)
#define VSET2V(v, v1)       VSET2(v, v1[0], v1[1])
#define VSET3V(v, v1)       VSET3(v, v1[0], v1[1], v1[2])
#define VSET4V(v, v1)       VSET4(v, v1[0], v1[1], v1[2], v1[3])
#define VSET3_(v, r, g, b)   VSET4(v, r, g, b, 1.f)
#define VSET2_(v, r, g, b)   VSET3(v, r, g, 1.f)
#define VADD2(v, v1, v2)  VSET2(v, v1[0]+v2[0], v1[1]+v2[1])
#define VADD3(v, v1, v2)  VSET3(v, v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2])
#define VSUB2(v, v1, v2)  VSET2(v, v1[0]-v2[0], v1[1]-v2[1])
#define VSUB3(v, v1, v2)  VSET3(v, v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2])
#define VSCALE3(v, v1, a)  VSET3(v, v1[0]*(a), v1[1]*(a), v1[2]*(a))
#define VDOT2(v1, v2)   (v1[0]*v2[0]+v1[1]*v2[1])
#define VDOT3(v1, v2)   (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define VLSQ2(v)   VDOT2(v, v)
#define VLSQ3(v)   VDOT3(v, v)
#define VLEN2(v)   sqrt(VDOT2(v, v))
#define VLEN3(v)   sqrt(VDOT3(v, v))
#define _SQ(x)  (x)*(x)
#define VDISQ2(v1, v2)   (_SQ(v1[0]-v2[0])+_SQ(v1[1]-v2[1]))
#define VDISQ3(v1, v2)   (_SQ(v1[0]-v2[0])+_SQ(v1[1]-v2[1])+_SQ(v1[2]-v2[2]))
#define VDIS2(v1, v2)   sqrt(VDISQ2(v1, v2))
#define VDIS3(v1, v2)   sqrt(VDISQ3(v1, v2))
#define VCROSS3(v, v1, v2) VSET3(v, v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0])
//
#if 0
typedef enum _pen_alignment_e {AlignmentNul = 0, AlignmentInset, AlignmentCenter, AlignmentLeft, AlignmentOutset, AlignmentRight } pen_alignment_e;
typedef enum _line_cap_e { round_cap, butt_cap, square_cap } line_cap_e;
typedef enum _line_join_e {round_join, miter_join , miter_join_revert , bevel_join , miter_join_round ,} line_join_e;
typedef enum _inner_join_e {inner_round, inner_bevel, inner_miter, inner_jag,} inner_join_e;
#endif

#if 0
#define LINE_SOLID               0
#define LINE_DASH                1       /* -------  */
#define LINE_DOT                 2       /* .......  */
#define LINE_DASHDOT             3       /* _._._._  */
#define LINE_DASHDOTDOT          4       /* _.._.._  */
#define LINE_NULL                5
#define LINE_INSIDEFRAME         6
#define LINE_USERSTYLE           7
#define LINE_ALTERNATE           8
#define LINE_STYLE_MASK          0x0000000F

#define ALIGN_NUL            0x00000000
#define ALIGN_NOCLOSE        0x00000000
#define ALIGN_INSET          0x00000010
#define ALIGN_CENTER         0x00000020
#define ALIGN_LEFT           0x00000030
#define ALIGN_OUTSET         0x00000040
#define ALIGN_RIGHT          0x00000050
#define ALIGN_MASK           0x000000F0

#define CAP_ROUND            0x00000000
#define CAP_SQUARE           0x00000100
#define CAP_FLAT             0x00000200
#define CAP_BUTT             0x00000200
#define CAP_MASK             0x00000F00

#define JOIN_ROUND           0x00000000
#define JOIN_BEVEL           0x00001000
#define JOIN_MITER           0x00002000
#define JOIN_MITER_REVERT    0x00003000
#define JOIN_MITER_ROUND     0x00004000
#define JOIN_MASK            0x0000F000

#define INNER_ROUND          0x00000000
#define INNER_BEVEL          0x00010000
#define INNER_MITER          0x00020000
#define INNER_JAG            0x00030000
#define INNER_MASK           0x000F0000
#endif

typedef struct DPOINT3 {
  double x, y, z;
} DPOINT3;
typedef DPOINT3 D3VECTOR;
typedef struct FPOINT3 {
  float x, y, z;
} FPOINT3;
typedef FPOINT3 F3VECTOR;
typedef struct DPOINT4 {
  double x, y, z, w;
} DPOINT4;
typedef DPOINT4 D4VECTOR;
typedef struct FPOINT4 {
  float x, y, z, w;
} FPOINT4;
typedef FPOINT4 F4VECTOR;
typedef struct {
  short x, y;
} SPOINT;
typedef struct {
  int x, y;
} IPOINT;
typedef IPOINT IPOINT2;
typedef struct IPOINT3 {
  int x, y, z;
} IPOINT3;
typedef struct {
  int64 x, y;
} QPOINT;
typedef QPOINT LPOINT2;
typedef struct {
  int64 x, y, z;
} QPOINT3;
typedef QPOINT3 LPOINT3;
typedef struct {
  double x, y;
} DPOINT;
typedef DPOINT D2VECTOR;
#ifndef __FPOINT_DEFINED__
#define __FPOINT_DEFINED__
typedef struct {
  float x, y;
} FPOINT, FPOINT2;
#endif // __FPOINT_DEFINED__
typedef FPOINT F2VECTOR;
typedef struct {
  union {int cx; int w; int width;int c;};
  union {int cy; int h; int height;int r;};
} ISIZE;
typedef struct {
  union {int64 w; int64 width;};
  union {int64 h; int64 height;};
} LSIZE;
typedef struct {
  union {float w; float width;};
  union {float h; float height;};
} FSIZE;
typedef struct {
  union {double w; float width;};
  union {double h; float height;};
} DSIZE;
typedef struct {
  int l, t, r, b;
} IRECT;
typedef struct {
  float l, t, r, b;
} FRECT;
typedef struct {
  double l, t, r, b;
} DRECT;
typedef struct {
  int l, r;
} IRANGE;
typedef struct {
  double l, r;
} DRANGE;
typedef struct {
  double a00, a01, a02, a10, a11, a12, a20, a21, a22;
} DMATRIX33;
typedef struct {
  IPOINT p0, p1;
} ISEGMENT;
typedef struct {
  LPOINT2 p0, p1;
} LSEGMENT;
typedef struct {
  DPOINT p0, p1;
} DSEGMENT;
typedef struct {
  DPOINT3 p0, p1, p2;
} DSEGMENT3;
typedef DPOINT DPOINT2;
//typedef DPOINT3 DPOINT3;
typedef DSEGMENT DSEGMENT2;
typedef struct {
  int x, y, cmd;
} vertex_i;
typedef struct {
  int x1, x2;
  uint32* ptr;
} row_info;

typedef struct CRange {
  int start;
  int end;
} CRange;
#define Data_2DSize(d) ((d).w * (d).h)
#define Data_3DSize(d) ((d).w * (d).h * (d).c)
#define Data_4DSize(d) ((d).w * (d).h * (d).c * (d).n)
#define Data_total(d)  Data_4DSize(d)
#define Data_count(d)  Data_4DSize(d)
#define Data_4DEqu(a, b) ((a).w == (b).w && (a).h == (b).h && (a).c == (b).c && (a).n == (b).n)
#define Data_4DNeq(a, b) (!Data_4DEqu(a, b))
#define Data_2DIndex(d, ih, iw) ((ih) * (d).w + (iw))
#define Data_3DIndex(d, ic, ih, iw) (((ic) * (d).h + ih) * (d).w + (iw))
#define Data_4DIndex(d, in, ic, ih, iw)   ((((in) * (d).c + ic) * (d).h + ih) * (d).w + (iw))
#define Data_4DIndex1(d, in)           Data_4DIndex(d, in, 0, 0, 0)
#define Data_4DIndex2(d, in, ic)       Data_4DIndex(d, in, ic, 0, 0)
#define Data_4DIndex3(d, in, ic, ih)   Data_4DIndex(d, in, ic, ih, 0)

typedef union DataSize {
  struct {int n, c, h, w;};
  int dim[4];
} DataSize;
static DataSize dataSize(int _number, int _channels, int _height, int _width)
{
  DataSize ret = {_number, _channels, _height, _width};
  ret.n = _number;
  ret.c = _channels;
  ret.h = _height;
  ret.w = _width;
  return ret;
}
static int dataCount(DataSize size, int start, int end) {
  int count = 1;
  start = BOUND(start, 0, end);
  end = BOUND(end, start, 4);
  for (; start < end; ++start) {
    count *= size.dim[start];
  }
  return count;
}
typedef FPOINT2 CPoint2D32f;
typedef FPOINT3 CPoint3D32f;
typedef DPOINT2 CPoint2D64f;
typedef DPOINT3 CPoint3D64f;

typedef IPOINT2 CPoint2i;
typedef IPOINT3 CPoint3i;
typedef LPOINT2 CPoint2l;
typedef LPOINT3 CPoint3l;
typedef FPOINT2 CPoint2f;
typedef FPOINT3 CPoint3f;
typedef DPOINT2 CPoint2d;
typedef DPOINT3 CPoint3d;

typedef LSIZE CSize2l;
typedef ISIZE CSize;
typedef IPOINT CPoint;
typedef FPOINT CPoint2D32f;
#define CC_SIZE_AREA(sz)   ((sz).width * (sz).height)
#define CC_SIZE_ISEQ(sz1, sz2)   ( ((sz1).width==(sz1).width) && ((sz1).height==(sz2).height) )

#define SETPT2(pt, _x, _y)  ((pt).x=_x, (pt).y=_y)
#define SETPT3(pt, _x, _y, _z)  ((pt).x=_x, (pt).y=_y, (pt).z=_z)


#define PTSET(pt, _x, _y)   ((pt).x=_x, (pt).y=_y)
CC_INLINE IRANGE iRANGE(int l, int r)
{
  IRANGE ra;
  ra.l = l, ra.r = r;
  return ra;
}
CC_INLINE DRANGE dRANGE(double l, double r)
{
  DRANGE ra;
  ra.l = l, ra.r = r;
  return ra;
}
#define iPOINT_PT(pt)  iPOINT((int)(pt).x, (int)(pt).y)
CC_INLINE IPOINT iPOINT(int x, int y)
{
  IPOINT pt;
  pt.x = x, pt.y = y;
  return pt;
}
CC_INLINE int iPOINT_cmp(IPOINT a, IPOINT b)
{
  return a.y == b.y ? CC_CMP(a.x, b.x) : CC_CMP(a.y, b.y);
}
CC_INLINE FPOINT fPOINT(double x, double y)
{
  FPOINT p;
  p.x = (float) x;
  p.y = (float) y;
  return p;
}
CC_INLINE DPOINT dPOINT(double x, double y)
{
  DPOINT pt;
  pt.x = x, pt.y = y;
  return pt;
}
CC_INLINE IRANGE cRANGE(int l, int r)
{
  IRANGE ra;
  ra.l = l, ra.r = r;
  return ra;
}
CC_INLINE FPOINT iPointTof(IPOINT point)
{
  return fPOINT((float) point.x, (float) point.y);
}
CC_INLINE IPOINT fPointToi(FPOINT point)
{
  IPOINT ipt;
  ipt.x = (int)(point.x + .5);
  ipt.y = (int)(point.y + .5);
  return ipt;
}
CC_INLINE FPOINT3 fPOINT3(double x, double y, double z)
{
  FPOINT3 p;
  p.x = (float) x, p.y = (float) y, p.z = (float) z;
  return p;
}
#define cPoint3d dPOINT3
CC_INLINE DPOINT3 dPOINT3(double x, double y, double z)
{
  DPOINT3 p;
  p.x = x, p.y = y, p.z = z;
  return p;
}
CC_INLINE DSEGMENT dSEGMENT(double x0, double y0, double x1, double y1)
{
  DSEGMENT d;
  d.p0 = dPOINT(x0, y0), d.p1 = dPOINT(x1, y1);
  return d;
}
CC_INLINE ISEGMENT iSEGMENT(int x0, int y0, int x1, int y1)
{
  ISEGMENT d;
  d.p0 = iPOINT(x0, y0), d.p1 = iPOINT(x1, y1);
  return d;
}
/////////////////////////////////////////////////////////////////////////////
typedef struct {
  int x, y;
} IMPOINT;
typedef struct {
  int l, t, r, b;
} IMRECT;
#define fVec2opt(a, op, b) fVec2((a).x op (b).x, (a).y op (b).y)
#define fVec2opt_s(a, op, b) fVec2((a).x op (b), (a).y op (b))
#define fVec2Add(c, _x, _y) fVec2((c).x + (_x), (c).y + (_y))
CC_INLINE FPOINT fVec2(float _x, float _y)
{
  FPOINT v;
  v.x = _x, v.y = _y;
  return v;
}
CC_INLINE FPOINT fVec2mul_s1(FPOINT a, float s)
{
  return fVec2(a.x * s, a.y * s);
}
CC_INLINE FPOINT4 fVec4(float _x, float _y, float _z, float _w)
{
  FPOINT4 v;
  v.x = _x, v.y = _y, v.z = _z, v.w = _w;
  return v;
}
#define PT2OPDEF(p1, op, p2)  (p1).x op (p2).x, (p1).y op (p2).y
#define PT3OPDEF(p1, op, p2)   (p1).x op (p2).x, (p1).y op (p2).y, (p1).z op (p2).z
#define RAND01() (rand()*1./RAND_MAX)
#define RANDX(_X) (rand()*(_X*1.)/RAND_MAX)
#define iround(v) (int)((v < 0.0) ? v - 0.5 : v + 0.5)
#define uround(v) (unsigned)(v + 0.5)
#define ufloor(v) (unsigned)(v)
#define uceil(v) (unsigned)(ceil(v))
#define PREV_INDEX(i, n) (i-1+n)%(n)
#define NEXT_INDEX(i, n) (i+1 )%(n)
#define PTADD(c, a, b) (c.x=a.x+b.x, c.y=a.y+b.y)
#define PTSUB(c, a, b) (c.x=a.x-b.x, c.y=a.y-b.y)
#define PTCLIP(pt, l, t, r, b) iPOINT(BOUND(pt.x, l, r), BOUND(pt.y, t, b))
#define RCLT(rc) iPOINT((rc)->l, (rc)->t)
#define RCLB(rc) iPOINT((rc)->l, (rc)->b)
#define RCRB(rc) iPOINT((rc)->r, (rc)->b)
#define RCSZ(rc) iSIZE(RCW(rc), RCH(rc))
#define SZMAX(sz1, sz2) iSIZE(MAX(sz1.w, sz2.w), MAX(sz1.h, sz2.h))
#define SZADD(sz1, sz2) iSIZE(sz1.w+sz2.w, sz1.h+sz2.h)
#define SZADD2(sz1, CX, CY) iSIZE(sz1.w+CX, sz1.h+CY)
#define RCSET(prc, L, T, R, B) ((prc)->l=L, (prc)->t=T, (prc)->r=R, (prc)->b=B)
#define RCX(prc) ((prc)->r + (prc)->l)
#define RCY(prc) ((prc)->b + (prc)->t)
#define RCW(prc) ((prc)->r - (prc)->l)
#define RCH(prc) ((prc)->b - (prc)->t)
#define RCCENTER(rc)    iPOINT(RCX(rc)/2, RCY(rc)/2)
#define RCCENTERF(rc)   dPOINT(RCX(rc)/2., RCY(rc)/2.)
#define RCEQ(R1, R2)   (((R1)->l==(R2)->l) && ((R1)->t==(R2)->t) && ((R1)->r==(R2)->r) && ((R1)->b==(R2)->b))
CC_INLINE ISIZE iSIZE(int w, int h)
{
  ISIZE sz;
  sz.w = w, sz.h = h;
  return sz;
}
CC_INLINE IRECT iRECT(int l, int t, int r, int b)
{
  IRECT rc;
  RCSET(&rc, l, t, r, b);
  return rc;
}
CC_INLINE IRECT iRECT1(int x)
{
  IRECT rc;
  RCSET(&rc, x, x, x, x);
  return rc;
}
CC_INLINE IRECT iRECT_sz(ISIZE sz)
{
  IRECT rc;
  RCSET(&rc, 0, 0, sz.w, sz.h);
  return rc;
}
CC_INLINE IRECT iRECT_ps(IPOINT pt, ISIZE sz)
{
  IRECT rc;
  RCSET(&rc, pt.x, pt.y, pt.x + sz.w, pt.y + sz.h);
  return rc;
}
CC_INLINE IRECT iRECT_pp(IPOINT p1, IPOINT p2)
{
  IRECT rc;
  RCSET(&rc, p1.x, p1.y, p2.x, p2.y);
  return rc;
}
#define iRECT3(pt, sz) iRECT2(pt.x, pt.y, sz.w, sz.h)
CC_INLINE IRECT iRECT2(int x, int y, int w, int h)
{
  IRECT rc;
  RCSET(&rc, x, y, x + w, y + h);
  return rc;
}
CC_INLINE FRECT fRECT(float l, float t, float r, float b)
{
  FRECT rc;
  RCSET(&rc, l, t, r, b);
  return rc;
}
CC_INLINE FRECT fRECT2(float l, float t, float w, float h)
{
  FRECT rc;
  RCSET(&rc, l, t, l + w, t + h);
  return rc;
}

#define RECT2RECT(fun, T, rc) fun((T)((rc)->l), (T)((rc)->t), (T)((rc)->r), (T)((rc)->b));

CC_INLINE DRECT dRECT(double l, double t, double r, double b)
{
  DRECT rc;
  RCSET(&rc, l, t, r, b);
  return rc;
}
CC_INLINE DRECT dRECT2(double x, double y, double w, double h)
{
  DRECT rc;
  RCSET(&rc, x, y, x + w, y + h);
  return rc;
}

CC_INLINE int iRectIsEmpty(const IRECT* rc)
{
  return rc->l >= rc->r || rc->t >= rc->b;
}
CC_INLINE int iRectNormalize(IRECT* prc)
{
  int nTemp;
  if (prc->l > prc->r) {
    nTemp = prc->l;
    prc->l = prc->r;
    prc->r = nTemp;
  }
  if (prc->t > prc->b) {
    nTemp = prc->t;
    prc->t = prc->b;
    prc->b = nTemp;
  }
  return 0;
}
CC_INLINE int fRectNormalize(FRECT* prc)
{
  float nTemp;
  if (prc->l > prc->r) {
    nTemp = prc->l;
    prc->l = prc->r;
    prc->r = nTemp;
  }
  if (prc->t > prc->b) {
    nTemp = prc->t;
    prc->t = prc->b;
    prc->b = nTemp;
  }
  return 0;
}
CC_INLINE int iRectInclude(IRECT r1, IRECT r2)
{
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  return r1.l <= r2.l && r1.t <= r2.l && r1.r >= r2.r && r1.b >= r2.b;
}
CC_INLINE int iRectBoundUpdate(IRECT* prc, int x, int y)  {
  if (x<prc->l) {
    prc->l = x;
  } else if (x>prc->r) {
    prc->r = x;
  }
  if (y<prc->t) {
    prc->t = y;
  } else if (y>prc->b) {
    prc->b = y;
  }
  return 0;
}
CC_INLINE IRECT iRectInter(IRECT r1, IRECT r2)
{
  IRECT temp = {0,0,0,0};
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  // check for total exclusion
  if ((r1.r > r2.l) && (r1.l < r2.r) && (r1.b > r2.t) && (r1.t < r2.b)) {
    temp.l = (r1.l > r2.l) ? r1.l : r2.l;
    temp.r = (r1.r < r2.r) ? r1.r : r2.r;
    temp.t = (r1.t > r2.t) ? r1.t : r2.t;
    temp.b = (r1.b < r2.b) ? r1.b : r2.b;
  }
  return temp;
}
CC_INLINE FRECT fRectInter(FRECT r1, FRECT r2)
{
  FRECT temp = {0,0,0,0};
  fRectNormalize(&r1);
  fRectNormalize(&r2);
  // check for total exclusion
  if ((r1.r > r2.l) && (r1.l < r2.r) && (r1.b > r2.t) && (r1.t < r2.b)) {
    temp.l = (r1.l > r2.l) ? r1.l : r2.l;
    temp.r = (r1.r < r2.r) ? r1.r : r2.r;
    temp.t = (r1.t > r2.t) ? r1.t : r2.t;
    temp.b = (r1.b < r2.b) ? r1.b : r2.b;
  }
  return temp;
}
CC_INLINE BOOL iRectIsNull(const IRECT* pRect)
{
  return (pRect->l >= pRect->r || pRect->t >= pRect->b);
}
CC_INLINE BOOL iRectSet(IRECT* pRect, int l, int t, int r, int b)
{
  pRect->l = l, pRect->r = r, pRect->t = t, pRect->b = b;
  return 0;
}
CC_INLINE BOOL iRectSetEmpty(IRECT* pRect)
{
  pRect->l = pRect->r = pRect->t = pRect->b = 0;
  return 0;
}

#define iRectIn(rc, d)  iRectOffset(rc, d, d, -(d), -(d))
#define iRectOff2(rc, x, y)  iRectOffset((rc), x, y, x, y)
#define iRectOff0(rc)  iRectOff2((rc), -(rc)->l, -(rc)->t)
CC_INLINE IRECT iRectOffset(IRECT rc, int l, int t, int r, int b)
{
  rc.l += l, rc.t += t;
  rc.r += r, rc.b += b;
  return rc;
}
CC_INLINE IRECT iRectOffset2(IRECT rc, int x, int y)
{
  rc.l += x, rc.t += y;
  rc.r += x, rc.b += y;
  return rc;
}
CC_INLINE IRECT iRectOffsetTo2(IRECT rc, int x, int y) {
  rc.l += x, rc.t += y;
  rc.r += x, rc.b += y;
  return rc;
}
CC_INLINE IRECT iRectDeflate(IRECT rc, int l, int t, int r, int b)
{
  rc.l += l, rc.t += t;
  rc.r -= r, rc.b -= b;
  return rc;
}
CC_INLINE IRECT iRectDeflate2(IRECT rc, int x, int y)
{
  rc.l += x, rc.t += y;
  rc.r -= x, rc.b -= y;
  return rc;
}
CC_INLINE IRECT iRectInflate(IRECT rc, int l, int t, int r, int b)
{
  rc.l -= l, rc.t -= t;
  rc.r += r, rc.b += b;
  return rc;
}
CC_INLINE IRECT iRectOffsetXY(IRECT rc, int x, int y)
{
  rc.l = rc.l + x, rc.t = rc.t + y;
  rc.r = rc.r + x, rc.b = rc.b + y;
  return rc;
}
CC_INLINE IRECT iRectAdd(IRECT rc, const IRECT rc2)
{
  rc.l += rc2.l, rc.t += rc2.t;
  rc.r += rc2.r, rc.b += rc2.b;
  return rc;
}
CC_INLINE IRECT iRectDeflateR(IRECT rc, const IRECT rc2)
{
  rc.l += rc2.l, rc.t += rc2.t;
  rc.r -= rc2.r, rc.b -= rc2.b;
  return rc;
}
CC_INLINE IRECT iRectInflateR(IRECT rc, const IRECT rc2)
{
  rc.l -= rc2.l, rc.t -= rc2.t;
  rc.r += rc2.r, rc.b += rc2.b;
  return rc;
}
CC_INLINE IRECT iRectInflate2(IRECT rc, int x, int y)
{
  rc.l -= x, rc.t -= y;
  rc.r += x, rc.b += y;
  return rc;
}
CC_INLINE IRECT iRectInflate1(IRECT rc, int x)
{
  rc.l -= x, rc.t -= x;
  rc.r += x, rc.b += x;
  return rc;
}
#define iPtInRect2(rc, pt)  iPtInRect(rc, (int)(pt).x, (int)(pt).y)
CC_INLINE int iPtInRect(const IRECT* rc, int x, int y)
{
  return (rc->l <= x && x < rc->r) && (rc->t <= y && y < rc->b);
}

CC_INLINE int iPtInRects(int n, const IRECT* rc, int x, int y)
{
  int i;
  for (i = 0; i < n; ++i) {
    if (iPtInRect(rc + i, x, y)) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int iPtInRectsBorder(IRECT rc, BOOL horz, int n, const float* v, int gap, int x, int y, int ex) {
  int i;
  float l = (float)rc.l;
  float t = (float)rc.t;
  if (horz) {
    for (i=0; i<(n-1); ++i) {
      float r = l+v[i];
      float l1 = r+gap;
      if (x>(r-ex) && x<(l1+ex) && y>rc.t && y<rc.b) {
        return i;
      }
      l = l1;
    }
  } else {
    for (i=0; i<(n-1); ++i) {
      float b = t+v[i];
      float t1 = b+gap;
      if (y>(b-ex) && y<(t1+ex) && x>rc.l && x<rc.r) {
        return i;
      }
      t = t1;
    }
  }
  return -1;
}

CC_INLINE int iPtInRectBorder(const IRECT* prc, int x, int y, int l, int t, int r, int b)
{
  IRECT rc = iRectOffset(*prc, -l, -t, r, b);

  if (iPtInRect(&rc, x, y) && !iPtInRect(prc, x, y)) {
    return (x < prc->l) << 0 | (x > prc->r) << 1 | (y < prc->t) << 2 | (y > prc->b) << 3;
  }

  return 0;
}
CC_INLINE IRECT iRectToSquare(IRECT rc)
{
  IRECT rc1 = rc;
  int d = MIN(RCH(&rc), RCW(&rc));
  rc1.r = rc.l + d, rc1.b = rc.t + d;
  return rc1;
}

// 自定义函数，计算两矩形 IOU，传入为均为矩形对角线，（x,y）  坐标。·
CC_INLINE double iRectIOU(IRECT Reframe, IRECT GTframe) {
  double ratio;
  int x1 = Reframe.l;
  int y1 = Reframe.t;
  int width1 = Reframe.r-Reframe.l;
  int height1 = Reframe.b-Reframe.t;
  int x2 = GTframe.l;
  int y2 = GTframe.t;
  int width2 = GTframe.r-GTframe.l;
  int height2 = GTframe.b-GTframe.t;
  
  int endx = MAX(x1+width1,x2+width2);
  int startx = MIN(x1,x2);
  int width = width1+width2-(endx-startx);
  
  int endy = MAX(y1+height1,y2+height2);
  int starty = MIN(y1,y2);
  int height = height1+height2-(endy-starty);
  
  if (width <=0 || height <= 0) {
    ratio = 0; // 重叠率为 0 
  } else {
    int Area = width*height; // 两矩形相交面积
    int Area1 = width1*height1;
    int Area2 = width2*height2;
    ratio = Area*1./(Area1+Area2-Area);
  }
  // return IOU
  return ratio;
}

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
    *pr = l+w;
    *pl2 = l+w+gap;
    *pr2 = r;
    return 1;
  case LF_alighParentRight:
    *pl = r-w;
    *pr = r;
    *pl2 = l;
    *pr2 = r-w-gap;
    return 1;
  case LF_Hcenter:
    *pl = l+(r-l-w)/2;
    *pr = *pl+w;
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
  retx = iRectLayout1D(rc.l, rc.r, cx, fmt&0x07, gap, &out->l, &out->r, &rc2->l, &rc2->r);
  rety = iRectLayout1D(rc.t, rc.b, cy, (fmt>>4)&0x7, gap, &out->t, &out->b, &rc2->t, &rc2->b);
  return retx||rety;
}

// 文本格式化
typedef enum {
  TF_TOP=0, // 顶对齐
    TF_LEFT=0, // 左对齐
    TF_CENTER=1, // 水平居中
    TF_RIGHT=2, // 右对齐
    TF_VCENTER=4, // 垂直居中 只对DT_SINGLELINE有效
    TF_BOTTOM=8, // 底对齐
TF_RightToLeft = 0x00000100,//从右至左输出文本 
TF_Vertical = 0x00000200,//垂直方向输出文本 
TF_NoFitBlackBox = 0x00000400, 
TF_DisplayFormatControl = 0x00002000, 
TF_NoFontFallback = 0x00040000, 
TF_MeasureTrailingSpaces = 0x00080000, 
TF_NoWrap = 0x00100000,//不自动换行 
TF_LineLimit = 0x00200000, 
TF_NoClip = 0x00400000,//不使用裁剪
TF_WrapWidgetWidth = (1<<27), // 在窗口部件的当前宽度自动换行（这是默认的）。默认在空白符号处自动换行，这可以使用setWrapPolicy()来改变。
TF_WrapFixedColumnWidth = (1<<28), // 从窗口部件左侧开始的固定数量的列数自动换行。列数可以通过wrapColumnOrWidth()设置。如果你需要使用等宽文本在设备上显示很好的格式文本，这是很有用的，例如标准的VT100终端，你可以把wrapColumnOrWidth()设置为80。 
TF_WrapAtWhiteSpace = (1<<29), // 在空白符号处（空格或者换行）自动换行。否则是任何
TF_WrapAnyWhere = (1<<31) // 在任何情况下自动换行，包括单字中。
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
  out->l = (int)((rc.l+x)*ox+tx);
  out->r = (int)((rc.r+x)*ox+tx);
  out->t = (int)((rc.t+y)*oy+ty);
  out->b = (int)((rc.b+y)*oy+ty);
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
  return th*1./sh;
}
// 把矩形框切分成大小相同的块，块按矩阵排列
CC_INLINE int iRectMatrix(IRECT rc, int row, int col, int gapx, int gapy, int fmt, int n, IRECT* prcs)
{
  int i;
  int cx, cy, h = RCH(&rc) - (row-1)*gapy, w = RCW(&rc) - (col-1)*gapx;
  cx = w / col + gapx, cy = h / row + gapy;
  iRectAlign(rc, cx*col + gapx*(col-1), cy*row+gapy*(row-1), fmt, &rc );

  for (i = 0; i < n; ++i) {
    prcs[i].l = rc.l + (cx * (i % col));
    prcs[i].r = prcs[i].l + cx - gapx;
    prcs[i].t = rc.t + (int)(cy * (i / col));
    prcs[i].b = prcs[i].t + cy - gapy;
  }

  return 0;
}
CC_INLINE double iRectSplit(IRECT rcIn, BOOL horz, float gap, int count, float* v, IRECT* pout) {
  double x, w, sum1=0, ss;
  int len = horz ? RCW(&rcIn) : RCH(&rcIn);
  int i;
  if (len<=0) {return 0;}
  for (i=0; i<count; ++i) {
    sum1 += v[i];
  }
  sum1 = MAX(1, sum1);
  ss = (len- gap*(count-1))/sum1;
  x=horz ? rcIn.l : rcIn.t;
  for (i=0; i<count; ++i) {
    w = (ss*v[i]);
    if (horz) {
      pout[i] = iRECT((int)x, rcIn.t, (int)(x+w), rcIn.b);
    } else {
      pout[i] = iRECT(rcIn.l, (int)x, rcIn.r, (int)(x+w));
    }
    v[i] = (float)w;
    x += w + gap;
  }
  return ss;
}
CC_INLINE float iRectSplitUpdate(BOOL horz, float gap, int count, float* v, float max_v, const IRECT* rc) {
  float sum1=0, ss;
  int i;
  UNUSED(gap);
  for (i=0; i<count; ++i) {
    if (v[i]<=max_v) {
      if (horz) {
        sum1 += horz ? RCW(rc+i) : RCH(rc+i);
      } 
    }
  }
  if (sum1>0) {
    ss = 1/sum1;
    for (i=0; i<count; ++i) {
      if (v[i]<=max_v) {
        v[i] = (horz ? RCW(rc+i) : RCH(rc+i))*ss;
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
  switch(cmd) {
  case 'l':
    {
      if (d>0) {
        d = BOUND(d, s, RCW(rc));
        out->r = rc->l + d, rc->l = out->r + gap;
      } else {
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
      } else {
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
      } else {
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
      } else {
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
  IPOINT pt = {0, 0};
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
  IRECT temp = {0,0,0,0};
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
  IRECT temp = {0,0,0,0};
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
  for (i=0; i<npt; ++i) {
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
  memmove(s->pt + i, s->pt + i + 1, (npt - i - 1)*sizeof(DPOINT));
  s->len[pos]--;
  return 0;
}
CC_INLINE int dPOLYGON_addpt(DPOLYGON* s, int i, double x, double y)
{
  int npt = dPOLYGON_npt(s);
  int pos = dPOLYGON_pos(s, i);
  REALLOC(DPOINT, s->pt, npt + 1);
  memmove(s->pt + i + 1, s->pt + i, (npt - i - 1)*sizeof(DPOINT));
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
#define CC_CLOCKWISE         1   // ??????
#define CC_COUNTER_CLOCKWISE 2   // ???????
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
enum {SegmentPointCount = 2, LinePointCount = 2, RectanglePointCount = 2, BoxPointCount = 2, TrianglePointCount = 3, QuadixPointCount = 4, };
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
