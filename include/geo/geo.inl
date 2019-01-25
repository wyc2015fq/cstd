#ifndef _GEO_INL_
#define _GEO_INL_
// C计算几何函数库
// 计算几何学库函数
#include "cstd.h"
#include "str.h"
#include "cmath.h"
#define CC_INFINITY ((double)(1<<31))
#define EP 1e-10f
#define RADIAN (float)(CC_PI/180.0)
typedef struct affine_info_t {
  int op;
  double x, y;
} affine_info_t;
//求三角形的面积
/*语法：result = area3(float x1, float y1, float x2, float y2, float x3, float y3);
参数：
x1～3：三角形3个顶点x坐标
y1～3：三角形3个顶点y坐标
返回值：三角形面积
*/
/*
方法：海伦-秦九公式已知三角形三边a,b,c,则S面积＝ √[p(p - a)(p - b)(p - c)] （海伦公式）
（其中p=(a+b+c)/2）

*/
static float area3(float x1, float y1, float x2, float y2, float x3, float y3) {
  float a, b, c, p, s;
  //求三边长度
  a = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
  b = sqrt((x1 - x3)*(x1 - x3) + (y1 - y3)*(y1 - y3));
  c = sqrt((x2 - x3)*(x2 - x3) + (y2 - y3)*(y2 - y3));
  
  p = (a + b + c) * 0.5;
  //求面积
  s = sqrt(p*(p - a)*(p - b)*(p - c));
  
  return s;
}

//面积计算函数
static double tri_area(float x1, float y1, float x2, float y2, float x3, float y3)
{
   double tarea ;
   tarea=0.5*(x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2);//此处可以将上述的marray[i].mx/my直接带入，但是为了简洁，不带入
   return fabs(tarea);//取绝对值
}
CC_INLINE int pt_trans(int n, FPOINT* pt, const double* m3)
{
  double pt2[2];
  int i;
  for (i = 0; i < n; ++i) {
    M3MULV2(pt2, m3, &pt[i].x);
    V2SETV(&pt[i].x, pt2);
  }
  return 0;
}
CC_INLINE int pt_mul(int n, FPOINT* pt, double scaling_x, double scaling_y) {
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x *= scaling_x;
    pt[i].y *= scaling_y;
  }
  return 0;
}
CC_INLINE int pt_add(int n, FPOINT* pt, double x, double y) {
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x += x;
    pt[i].y += y;
  }
  return 0;
}
CC_INLINE int pt_scaling(int n, FPOINT* pt, double scaling_x, double scaling_y, double center_x, double center_y) {
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x = (pt[i].x-center_x)*scaling_x+center_x;
    pt[i].y = (pt[i].y-center_y)*scaling_y+center_y;
  }
  return 0;
}
CC_INLINE int pt_boundrc(int n, const FPOINT* pt, DRECT* boundrc) {
  if (n<=0) {
    return 0;
  } else {
    int i;
    *boundrc = dRECT(pt[0].x, pt[0].y, pt[0].x, pt[0].y);
    for (i=1; i<n; ++i) {
      boundrc->l = MIN(boundrc->l, pt[i].x);
      boundrc->t = MIN(boundrc->t, pt[i].y);
      boundrc->r = MAX(boundrc->r, pt[i].x);
      boundrc->b = MAX(boundrc->b, pt[i].y);
    }
  }
  return n;
}
CC_INLINE int ipt_boundrc(int n, const FPOINT* pt, IRECT* boundrc) {
  if (n<=0) {
    return 0;
  } else {
    int i;
    *boundrc = iRECT((int)pt[0].x, (int)pt[0].y, (int)pt[0].x+1, (int)pt[0].y+1);
    for (i=1; i<n; ++i) {
      boundrc->l = (int)MIN(boundrc->l, pt[i].x);
      boundrc->t = (int)MIN(boundrc->t, pt[i].y);
      boundrc->r = (int)MAX(boundrc->r, pt[i].x+1);
      boundrc->b = (int)MAX(boundrc->b, pt[i].y+1);
    }
  }
  return 0;
}
CC_INLINE int pt_center(int n, FPOINT* pt, double x, double y, double w, double h, double sc_x, double sc_y) {
  DRECT drc = {0};
  double sc;
  pt_boundrc(n, pt, &drc);
  sc = MIN(w/RCW(&drc), h/RCH(&drc));
  pt_add(n, pt, x+w/2 - RCX(&drc)/2, y+h/2 - RCY(&drc)/2);
  pt_scaling(n, pt, sc*sc_x, sc*sc_y, x+w/2, y+h/2);
  return 0;
}
CC_INLINE int affine_info_trans(int nai, affine_info_t* ai, double* m3c)
{
  double m3a[9];
  double m3b[9];
  int i;
  for (i = 0; i < nai; ++i) {
    double x = ai[i].x, y = ai[i].y;
    MEMCPY(m3b, m3c, 9);
    switch (ai[i].op) {
    case GEO_TRANS:
      V9SET(m3a, 1, 0, x, 0, 1, y, 0, 0, 1);
      M3MUL(m3c, m3a, m3b);
      break;
    case GEO_SCALING:
      V9SET(m3a, x, 0, 0, 0, y, 0, 0, 0, 1);
      M3MUL(m3c, m3a, m3b);
      break;
    default:
      V9SET(m3b, 1, 0, 0, 0, 1, 0, 0, 0, 1);
      ASSERT(0);
      break;
    }
  }
  return 0;
}
CC_INLINE int pt_trans_info(int n, FPOINT* pt, int nai, affine_info_t* ai)
{
  double m3c[9];
  M3ID(m3c);
  affine_info_trans(nai, ai, m3c);
  pt_trans(n, pt, m3c);
  return 0;
}
#undef CC_EPS
#define CC_EPS 0.000001
#define FISZERO(f) ((f)<CC_EPS&&(f)>(-CC_EPS))
#define FEQ(a, b) FISZERO((a)-(b))
#define pt_dist(p1, p2) sqrt(V2DIST2(p1, p2))
#define rand01() (1.*(rand() % RAND_MAX)/RAND_MAX)
CC_INLINE int pt_random(int n, int cx, int cy, double* pt)
{
  int i;
  for (i = 0; i < n; i++) {
#if 1
    pt[2 * i + 0] = rand() % cx;// + i * 1. / n;
    pt[2 * i + 1] = rand() % cy;// + i * 1. / n;
#else
    pt[2 * i + 0] = cx * rand01();
    pt[2 * i + 1] = cy * rand01();
#endif
  }
  return 0;
}
CC_INLINE int pt_random2(int n, int cx, int cy, double* pt)
{
  int i;
  for (i = 0; i < n; i++) {
    double ir = (9 * cx + rand() % cx) / 10.;
    pt[2 * i + 0] = (ir) * cos(i * 2 * CC_PI / n) + i * 1. / n;
    pt[2 * i + 1] = (1.*ir * cy / cx) * sin(i * 2 * CC_PI / n) + i * 1. / n;
  }
  return 0;
}
CC_INLINE int pt_random_4(int n, FPOINT* pt, double x, double y, double r1, int ir)
{
  int i;
  for (i = 0; i < n; i++) {
    double r = (rand() % ir) + r1;
    pt[i].x = x + r * cos(i * 2 * CC_PI / n);
    pt[i].y = y + r * sin(i * 2 * CC_PI / n);
  }
  return 0;
}
// 直线交点pt 和 参数ts
// ts[0] 是交点对p1 p2的比例
// ts[1] 是交点对p3 p4的比例
CC_INLINE double line_inter(const double* p1, const double* p2, const double* p3, const double* p4, inter_info_t* ii)
{
  double p21[2], p43[2], p31[2], d, d0, d1 = 0;
  V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
  //p21*ts[0]-p43*ts[1]==p31;
  ii->d = d = DET2(p21[0], p43[0], p21[1], p43[1]);
  if (!FISZERO(d)) {
    V2SUBV(p31, p3, p1);
    d0 = DET2(p31[0], p43[0], p31[1], p43[1]) / d;
    ii->ts[0] = d0;
    ii->ts[1] = DET2(p31[0], p21[0], p31[1], p21[1]) / d;
    ii->pt.x = p1[0] + p21[0] * d0;
    ii->pt.y = p1[1] + p21[1] * d0;
  }
  else {
    // 平行
    V2SUBV(p31, p3, p1);
    d0 = DET2(p21[0], p31[0], p21[1], p31[1]);
    if (FISZERO(d0)) {
      // 四点共线
      d1 = (fabs(p31[0]) + fabs(p31[1])) / (fabs(p21[0]) + fabs(p21[1]));
      if (d1 >= 0 && d1 <= 1) {
        ii->ts[0] = d1;
        ii->ts[1] = 0;
        ii->pt.x = p3[0];
        ii->pt.y = p3[1];
      }
      else {
        d1 = (fabs(p31[0]) + fabs(p31[1])) / (fabs(p43[0]) + fabs(p43[1]));
        ii->ts[0] = d1;
        ii->ts[1] = 0;
        ii->pt.x = p1[0];
        ii->pt.y = p1[1];
      }
    }
  }
  return d;
}
CC_INLINE int my_iline_inter(const int* p1, const int* p2, const int* p3, const int* p4, int* p5)
{
  int p21[2], p43[2], p31[2], d, d0;
  V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
  //p21*ts[0]-p43*ts[1]==p31;
  d = DET2(p21[0], p43[0], p21[1], p43[1]);
  if (0 != d) {
    V2SUBV(p31, p3, p1);
    d0 = DET2(p31[0], p43[0], p31[1], p43[1]);
    p5[0] = p1[0] + p21[0] * d0 / d;
    p5[1] = p1[1] + p21[1] * d0 / d;
  }
  return d;
}
CC_INLINE int iline_inter(const int* p1, const int* p2, const int* p3, const int* p4, iinter_info_t* ii)
{
  int p21[2], p43[2], p31[2], d, d0;
  V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
  //p21*ts[0]-p43*ts[1]==p31;
  ii->d = d = DET2(p21[0], p43[0], p21[1], p43[1]);
  if (0 != d) {
    V2SUBV(p31, p3, p1);
    d0 = DET2(p31[0], p43[0], p31[1], p43[1]);
    //ii->ts[0] = d0;
    //ii->ts[1] = DET2(p31[0], p21[0], p31[1], p21[1]) / d;
    ii->pt.x = p1[0] + p21[0] * d0 / d;
    ii->pt.y = p1[1] + p21[1] * d0 / d;
  }
  else {
    // 平行
    V2SUBV(p31, p3, p1);
    d0 = DET2(p21[0], p31[0], p21[1], p31[1]);
    if (0 == d0) {
      // 四点共线
      if ((abs(p31[0]) + abs(p31[1])) < (abs(p21[0]) + abs(p21[1]))) {
        ii->pt.x = p3[0];
        ii->pt.y = p3[1];
      }
      else {
        ii->pt.x = p1[0];
        ii->pt.y = p1[1];
      }
    }
  }
  return d;
}
#define V2CROSS(p1, p2, op) (((p1)[0] - (op)[0]) * ((p2)[1] - (op)[1]) - ((p2)[0] - (op)[0]) * ((p1)[1] - (op)[1]))
// 线段相交测试
// 返回： 0 不相交 1 相交面积>0 -1相交面积<0
CC_INLINE int seg_inter_test(const double* p1, const double* p2, const double* p3, const double* p4, inter_info_t* ii)
{
  int R = 1 //
      && (MAX(p1[0], p2[0]) >= MIN(p3[0], p4[0])) // 排斥实验
      && (MAX(p3[0], p4[0]) >= MIN(p1[0], p2[0])) // 排斥实验
      && (MAX(p1[1], p2[1]) >= MIN(p3[1], p4[1])) // 排斥实验
      && (MAX(p3[1], p4[1]) >= MIN(p1[1], p2[1])) // 排斥实验
      && (V2CROSS(p3, p2, p1) * V2CROSS(p4, p2, p1) <= 0) // 跨立实验: 点34在线12的两边
      && (V2CROSS(p1, p4, p3) * V2CROSS(p2, p4, p3) <= 0) // 跨立实验: 点12在线34的两边
      ;
  if (R) {
    double d = line_inter(p1, p2, p3, p4, ii);
    if (d < 0) {
      R = -R;
    }
  }
  ii->flag = R;
  return R;
}
CC_INLINE int iseg_inter_test(const int* p1, const int* p2, const int* p3, const int* p4, iinter_info_t* ii)
{
  int R = 1 //
      && (MAX(p1[0], p2[0]) >= MIN(p3[0], p4[0])) // 排斥实验
      && (MAX(p3[0], p4[0]) >= MIN(p1[0], p2[0])) // 排斥实验
      && (MAX(p1[1], p2[1]) >= MIN(p3[1], p4[1])) // 排斥实验
      && (MAX(p3[1], p4[1]) >= MIN(p1[1], p2[1])) // 排斥实验
      && (V2CROSS(p3, p2, p1) * V2CROSS(p4, p2, p1) <= 0) // 跨立实验: 点34在线12的两边
      && (V2CROSS(p1, p4, p3) * V2CROSS(p2, p4, p3) <= 0) // 跨立实验: 点12在线34的两边
      ;
  if (R) {
    int d = iline_inter(p1, p2, p3, p4, ii);
    if (d < 0) {
      R = -R;
    }
  }
  ii->flag = R;
  return R;
}
#define FV2EQ(A, B) (FEQ((A)[0], (B)[0]) && FEQ((A)[1], (B)[1]))
#define V2EQ(A, B) (((A)[0]==(B)[0]) && ((A)[1] == (B)[1]))
#define MAKE4BIT(b0,b1,b2,b3) ((b0)<<0 | (b1)<<1 | (b2)<<2 | (b3)<<3)
CC_INLINE int seg_inter_test2(const double* p1, const double* p2, const double* p3, const double* p4, inter_info_t* ii)
{
  int R0 = 0, R1 = 0, R2 = 0, R = 1 //
      && (MAX(p1[0], p2[0]) >= MIN(p3[0], p4[0])) // 排斥实验
      && (MAX(p3[0], p4[0]) >= MIN(p1[0], p2[0])) // 排斥实验
      && (MAX(p1[1], p2[1]) >= MIN(p3[1], p4[1])) // 排斥实验
      && (MAX(p3[1], p4[1]) >= MIN(p1[1], p2[1])); // 排斥实验
  if (R) {
    // 点在点上
    R0 = MAKE4BIT(FV2EQ(p1, p3), FV2EQ(p1, p4), FV2EQ(p2, p3), FV2EQ(p2, p4));
    ii->ts[0] = ii->ts[1] = 0;
    if (R0 & 1) {
      V2SETV(&ii->pt.x, p1);
    }
    else if (R0 & 2) {
      V2SETV(&ii->pt.x, p1);
    }
    else if (R0 & 2) {
      V2SETV(&ii->pt.x, p2);
    }
    else if (R0 & 4) {
      V2SETV(&ii->pt.x, p2);
    }
    else if (!R0) {
      // 点在线上，跨立实验
      // 如果d1 * d2 < 0，点12在线34的两边，d1==0表示p1在线34上
      double d1 = V2CROSS(p1, p4, p3), d2 = V2CROSS(p2, p4, p3);
      // 点34在线12的两边
      double d3 = V2CROSS(p3, p2, p1), d4 = V2CROSS(p4, p2, p1);
      R1 = (d3 * d4 <= 0) && (d1 * d2 <= 0);
      R2 = MAKE4BIT(FEQ(d1, 0), FEQ(d2, 0), FEQ(d3, 0), FEQ(d4, 0));
      if (R2 & 1) {
        line_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p1);
        ii->ts[0] = 0;
      }
      else if (R2 & 2) {
        line_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p2);
        ii->ts[0] = 1;
      }
      else if (R2 & 4) {
        line_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p3);
        ii->ts[1] = 0;
      }
      else if (R2 & 8) {
        line_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p4);
        ii->ts[1] = 1;
      }
      else if (!R2) {
        line_inter(p1, p2, p3, p4, ii);
      }
    }
  }
  return R | (R0 << 1) | (R1 << 5) | (R2 << 6);
}
#define EQ(a, b) ((a)==(b))
CC_INLINE int iseg_inter_test2(const int* p1, const int* p2, const int* p3, const int* p4, int* p5)
{
  int R = 0;
  if (V2EQ(p1, p3)) {
    V2SETV(p5, p1);
    R = 4;
  }
  else {
    R = 1 //
        && (MAX(p1[0], p2[0]) >= MIN(p3[0], p4[0])) // 排斥实验
        && (MAX(p3[0], p4[0]) >= MIN(p1[0], p2[0])) // 排斥实验
        && (MAX(p1[1], p2[1]) >= MIN(p3[1], p4[1])) // 排斥实验
        && (MAX(p3[1], p4[1]) >= MIN(p1[1], p2[1])) // 排斥实验
        //&& (V2CROSS(p3, p2, p1) * V2CROSS(p4, p2, p1) <= 0) // 跨立实验: 点34在线12的两边
        //&& (V2CROSS(p1, p4, p3) * V2CROSS(p2, p4, p3) <= 0) // 跨立实验: 点12在线34的两边
        ;
    if (R) {
      // 点在线上，跨立实验
      // 如果d1 * d2 < 0，点12在线34的两边，d1==0表示p1在线34上
      int64 d1 = V2CROSS(p1, p4, p3), d2 = V2CROSS(p2, p4, p3);
      // 点34在线12的两边
      int64 d3 = V2CROSS(p3, p2, p1), d4 = V2CROSS(p4, p2, p1);
      R = 0;
      if ((d1 * d2) <= 0 && (d3 * d4) <= 0) {
        if (0 == d2 || 0 == d4) {
          R = 0;
        }
        else {
          R = 1;
          if (0 == d1 || 0 == d3) {
            if (0 == d1) {
              V2SETV(p5, p1);
              R = 2;
            }
            if (0 == d3) {
              V2SETV(p5, p3);
              R = 3;
            }
          }
          else {
            my_iline_inter(p1, p2, p3, p4, p5);
            R = 1;
            if (V2EQ(p2, p5) || V2EQ(p4, p5)) {
              R = 0;
            }
          }
        }
      }
    }
  }
  return R;
}
//排斥实验
static BOOL IsRectCross(const IPOINT* p1, const IPOINT* p2, const IPOINT* q1, const IPOINT* q2)
{
  BOOL ret = MIN(p1->x, p2->x) <= MAX(q1->x, q2->x) &&
      MIN(q1->x, q2->x) <= MAX(p1->x, p2->x) &&
      MIN(p1->y, p2->y) <= MAX(q1->y, q2->y) &&
      MIN(q1->y, q2->y) <= MAX(p1->y, p2->y);
  return ret;
}
//跨立判断
static BOOL IsLineSegmentCross(const IPOINT* p1, const IPOINT* p2, const IPOINT* q1, const IPOINT* q2)
{
  long line1, line2;
  line1 = p1->x * (q1->y - p2->y) + p2->x * (p1->y - q1->y) + q1->x * (p2->y - p1->y);
  line2 = p1->x * (q2->y - p2->y) + p2->x * (p1->y - q2->y) + q2->x * (p2->y - p1->y);
  if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0)) {
    return FALSE;
  }
  line1 = q1->x * (p1->y - q2->y) + q2->x * (q1->y - p1->y) + p1->x * (q2->y - q1->y);
  line2 = q1->x * (p2->y - q2->y) + q2->x * (q1->y - p2->y) + p2->x * (q2->y - q1->y);
  if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0)) {
    return FALSE;
  }
  return TRUE;
}
static BOOL GetCrossPoint(const IPOINT* p1, const IPOINT* p2, const IPOINT* q1, const IPOINT* q2, IPOINT* out)
{
  if (IsRectCross(p1, p2, q1, q2)) {
    if (IsLineSegmentCross(p1, p2, q1, q2)) {
      //求交点
      int tmpLeft, tmpRight;
      tmpLeft = (q2->x - q1->x) * (p1->y - p2->y) - (p2->x - p1->x) * (q1->y - q2->y);
      tmpRight = (p1->y - q1->y) * (p2->x - p1->x) * (q2->x - q1->x) + q1->x * (q2->y - q1->y) * (p2->x - p1->x) - p1->x * (p2->y - p1->y) * (q2->x - q1->x);
      out->x = (int)((double)tmpRight / (double)tmpLeft);
      tmpLeft = (p1->x - p2->x) * (q2->y - q1->y) - (p2->y - p1->y) * (q1->x - q2->x);
      tmpRight = p2->y * (p1->x - p2->x) * (q2->y - q1->y) + (q2->x - p2->x) * (q2->y - q1->y) * (p1->y - p2->y) - q2->y * (q1->x - q2->x) * (p2->y - p1->y);
      out->y = (int)((double)tmpRight / (double)tmpLeft);
      return TRUE;
    }
  }
  return FALSE;
}
CC_INLINE int pt_slope(const double* pa, const double* pb, const double* pc, double* pd)
{
  double pab[2], pcb[2], dba, dbc;
  V2SUBV(pab, pa, pb), V2SUBV(pcb, pc, pb);
  dba = sqrt(V2NORM(pab)), dbc = sqrt(V2NORM(pcb));
  V2MIX3(pd, pb, 1, pab, dbc, pcb, dba);
  return 0;
}
//返回角b平分线上一点
CC_INLINE double pt_slope2(const double* p1, const double* p2, const double* p3, const double* p4, double* p5, double* p6)
{
  double p21[2], p43[2], p31[2], d, d0;
  V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
  //p21*ts[0]-p43*ts[1]==p31;
  d = DET2(p21[0], p43[0], p21[1], p43[1]);
  if (!FISZERO(d)) {
    V2SUBV(p31, p3, p1);
    d0 = DET2(p31[0], p43[0], p31[1], p43[1]) / d;
    V2MIX2(p5, p1, 1, p21, d0);
    pt_slope(p2, p5, p4, p6);
  }
  else {
    V2MIX2(p5, p1, 0.5, p3, 0.5);
    V2MIX2(p6, p2, 0.5, p4, 0.5);
  }
  return 0;
}
// c到直线垂足 pd, 和距离 pdis
CC_INLINE int pt_pedal(const float* pa, const float* pb, const float* pc, float* pd, float* pdis, float* ts)
{
  double pba[2], pca[2], r, pd0[2];
  V2SUBV(pba, pb, pa), V2SUBV(pca, pc, pa);
  r = V2DOT(pba, pca) / V2DOT(pba, pba);
  V2MIX2(pd0, pa, 1, pba, r);
  if (ts) {
    *ts = r;
  }
  if (pd) {
    V2SETV(pd, pd0);
  }
  if (pdis) {
    *pdis = pt_dist(pc, pd0);
  }
  return 0;
}

// 二维空间点到直线的垂足  
CC_INLINE FPOINT GetFootOfPerpendicular(FPOINT pt,     // 直线外一点  
                             FPOINT begin,  // 直线开始点  
                             FPOINT end)   // 直线结束点  
{  
  FPOINT retVal;  
  double u;
  double dx = begin.x - end.x;  
  double dy = begin.y - end.y;
  if(fabs(dx) < 0.00000001 && fabs(dy) < 0.00000001 )  
  {  
    retVal = begin;  
    return retVal;  
  }
  u = (pt.x - begin.x)*(begin.x - end.x) + (pt.y - begin.y)*(begin.y - end.y);  
  u = u/((dx*dx)+(dy*dy));  
  
  retVal.x = begin.x + u*dx;  
  retVal.y = begin.y + u*dy;  
  return retVal;  
}
// 三维空间点到直线的垂足
CC_INLINE DPOINT3 GetFootOfPerpendicular3(DPOINT3 pt,     // 直线外一点  
                             DPOINT3 begin,  // 直线开始点  
                             DPOINT3 end)   // 直线结束点  
{  
  DPOINT3 retVal;  
  double u;
  double dx = begin.x - end.x;  
  double dy = begin.y - end.y;
  double dz = begin.z - end.z;  
  if(fabs(dx) < 0.00000001 && fabs(dy) < 0.00000001 && fabs(dz) < 0.00000001 )  
  {  
    retVal = begin;  
    return retVal;  
  }  
  
  u = (pt.x - begin.x)*(begin.x - end.x) +  
    (pt.y - begin.y)*(begin.y - end.y) + (pt.z - begin.z)*(begin.z - end.z);  
  u = u/((dx*dx)+(dy*dy)+(dz*dz));  
  
  retVal.x = begin.x + u*dx;  
  retVal.y = begin.y + u*dy;
  retVal.y = begin.z + u*dz;
  return retVal;  
}  
// 离p0最近的线段终点
CC_INLINE int line_nearest(int n, const FPOINT* pt, FPOINT p0, double mind)
{
  int i, j, minj = -1;
  float r, dis = 0, mindis = mind;
  for (i = 0; i < n; ++i) {
    j = (i + 1) % n;
    pt_pedal(&pt[i].x, &pt[j].x, &p0.x, 0, &dis, &r);
    if (r >= 0 && r < 1 && dis < mindis) {
      mindis = dis;
      minj = j;
    }
  }
  return minj;
}
CC_INLINE int dPOLYGON_line_nearest(const FPOLYGON* s, double x, double y, double mind)
{
  int k, i, j = 0, minj = -1;
  float r, dis = 0, mindis = mind;
  FPOINT p0 = fPOINT(x, y);
  const FPOINT* pt = s->pt;
  for (k = 0; k < s->n; ++k) {
    int npt = s->len[k];
    for (i = 0; i < npt; ++i) {
      j = (i + 1) % npt;
      pt_pedal(&pt[i].x, &pt[j].x, &p0.x, 0, &dis, &r);
      if (r >= 0 && r < 1 && dis < mindis) {
        mindis = dis;
        minj = j + (pt - s->pt);
      }
    }
    pt += npt;
  }
  return minj;
}
CC_INLINE IPOINT ipt2_add(IPOINT p1, IPOINT p2)
{
  return iPOINT(p1.x + p2.x, p1.y + p2.y);
}
CC_INLINE IPOINT ipt2_sub(IPOINT p1, IPOINT p2)
{
  return iPOINT(p1.x - p2.x, p1.y - p2.y);
}
#define euclidean_distance_sqr(X1, Y1, X2, Y2) (((X1)-(X2))*((X1)-(X2)) + ((Y1)-(Y2))*((Y1)-(Y2)))
// 整数点吸附
CC_INLINE int pt2grid(int n, FPOINT* pt, int g)
{
  int i = 0;
  for (i = 0; i < n; ++i) {
    pt[i].x = g * (int)(0.5 + pt[i].x / g);
    pt[i].y = g * (int)(0.5 + pt[i].y / g);
  }
  return 0;
}
//变换公式:设P为变换前的点,P1为变换后的点,A为变换矩阵,则
// P1 = P * A
//|P1.x| |P.x| |a00,a01,a02|
//|P1.y| = |P.y| * |a10,a11,a12|
//|P1.z| |P.z| |a20,a21,a22|
CC_INLINE int PtTransform(int n, FPOINT* out, const FPOINT* pt, DMATRIX33* m)
{
  int i;
  double x, y;
  for (i = 0; i < n; ++i) {
    x = pt[i].x, y = pt[i].y;
    M2TRANSFORM_3X3_D2(double, (*m), x, y, 1, out[i].x, out[i].y);
  }
  return 0;
}
CC_INLINE int PtRotate(int n, FPOINT* out, const FPOINT* pt, double angle, double cx, double cy, double sx, double sy, double tx, double ty)
{
  int i;
  double x, y, sa = sin(angle), ca = cos(angle);
  UNUSED(sy);
  for (i = 0; i < n; ++i) {
    x = sx * (pt[i].x - cx), y = sx * (pt[i].y - cy);
    out[i].x = (double)(x * ca - y * sa + cx + tx);
    out[i].y = (double)(x * sa + y * ca + cy + ty);
  }
  return 0;
}
CC_INLINE int RandPoints(FPOINT* pt, int len, double xmax, double ymax)
{
  int i;
  for (i = 0; i < len; ++i) {
    pt[i].x = (double)RANDX(xmax);
    pt[i].y = (double)RANDX(ymax);
  }
  return 0;
}
CC_INLINE FPOINT PtCenter(int n, const FPOINT* pt)
{
  FPOINT c = pt[0];
  int i;
  for (i = 1; i < n; ++i) {
    c.x += pt[i].x, c.y += pt[i].y;
  }
  c.x /= n, c.y /= n;
  return c;
}
CC_INLINE int point_in_polygon_point(const FPOINT* m_polygon, int m_num_points, double x, double y, double m_point_radius)
{
  int i;
  double sq = m_point_radius * m_point_radius;
  for (i = 0; i < m_num_points; i++) {
    double sqd = euclidean_distance_sqr(x, y, m_polygon[i].x, m_polygon[i].y);
    if (sqd < sq) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int point_in_polygon_edge(const FPOINT* m_polygon, int m_num_points, double x, double y, double m_point_radius)
{
  int i;
  for (i = 0; i < m_num_points; i++) {
    unsigned n1 = i;
    unsigned n2 = NEXT_INDEX(i, m_num_points);
    double x1 = m_polygon[n1].x;
    double y1 = m_polygon[n1].y;
    double x2 = m_polygon[n2].x;
    double y2 = m_polygon[n2].y;
    double dx = x2 - x1;
    double dy = y2 - y1;
    if ((dx * dx + dy * dy) > 0.0000001) {
      double x3 = x;
      double y3 = y;
      double x4 = x3 - dy;
      double y4 = y3 + dx;
      double den = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
      double u1 = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / den;
      double xi = x1 + u1 * (x2 - x1);
      double yi = y1 + u1 * (y2 - y1);
      dx = xi - x;
      dy = yi - y;
      if (u1 > 0.0 && u1 < 1.0 && sqrt(dx * dx + dy * dy) <= m_point_radius) {
        return i;
      }
    }
  }
  return -1;
}

//======= Crossings Multiply algorithm of InsideTest ========================
//
// By Eric Haines, 3D/Eye Inc, erich@eye.com
//
// This version is usually somewhat faster than the original published in
// Graphics Gems IV; by turning the division for testing the X axis crossing
// into a tricky multiplication test this part of the test became faster,
// which had the additional effect of making the test for "both to left or
// both to right" a bit slower for triangles than simply computing the
// intersection each time. The main increase is in triangle testing speed,
// which was about 15% faster; all other polygon complexities were pretty much
// the same as before. On machines where division is very expensive (not the
// case on the HP 9000 series on which I tested) this test should be much
// faster overall than the old code. Your mileage may (in fact, will) vary,
// depending on the machine and the test data, but in general I believe this
// code is both shorter and faster. This test was inspired by unpublished
// Graphics Gems submitted by Joseph Samosky and Mark Haigh-Hutchinson.
// Related work by Samosky is in:
//
// Samosky, Joseph, "SectionView: A system for interactively specifying and
// visualizing sections through three-dimensional medical image data",
// M.S. Thesis, Department of Electrical Engineering and Computer Science,
// Massachusetts Institute of Technology, 1993.
//
// Shoot a test ray along +X axis. The strategy is to compare vertex Y values
// to the testing point's Y and quickly discard edges which are entirely to one
// side of the test ray. Note that CONVEX and WINDING code can be added as
// for the CrossingsTest() code; it is left out here for clarity.
//
// Input 2D polygon _pgon_ with _numverts_ number of vertices and test point
// _point_, returns 1 if inside, 0 if outside.
CC_INLINE BOOL point_in_polygon(const FPOINT* m_polygon, int m_num_points, double tx, double ty)
{
  int j, k;
  int yflag0, yflag1, inside_flag;
  double vtx0, vty0, vtx1, vty1;
  if (m_num_points < 3) {
    return FALSE;
  }
  vtx0 = m_polygon[m_num_points - 1].x;
  vty0 = m_polygon[m_num_points - 1].y;
  // get test bit for above/below X axis
  yflag0 = (vty0 >= ty);
  vtx1 = m_polygon[0].x;
  vty1 = m_polygon[0].y;
  inside_flag = 0;
  for (j = 1; j <= m_num_points; ++j) {
    yflag1 = (vty1 >= ty);
    if (yflag0 != yflag1) {
      if (((vty1 - ty) * (vtx0 - vtx1) >= (vtx1 - tx) * (vty0 - vty1)) == yflag1) {
        inside_flag ^= 1;
      }
    }
    // Move to the next pair of vertices, retaining info as possible.
    yflag0 = yflag1;
    vtx0 = vtx1;
    vty0 = vty1;
    k = (j >= m_num_points) ? j - m_num_points : j;
    vtx1 = m_polygon[k].x;
    vty1 = m_polygon[k].y;
  }
  return inside_flag != 0;
}

#define POHIT_POINT (1<<28)
#define POHIT_EDGE (1<<29)
#define POHIT_FACE (1<<30)
#define POHIT_MARK (POHIT_POINT-1)
CC_INLINE int point_in_polygon_hittest(const FPOINT* m_polygon, int m_num_points, double x, double y, double m_point_radius, int hitmark)
{
  int i;
  if ((hitmark & POHIT_POINT) && (i = point_in_polygon_point(m_polygon, m_num_points, x, y, m_point_radius)) >= 0) {
    return i | POHIT_POINT;
  }
  if ((hitmark & POHIT_EDGE) && (i = point_in_polygon_edge(m_polygon, m_num_points, x, y, m_point_radius)) >= 0) {
    return i | POHIT_EDGE;
  }
  if ((hitmark & POHIT_FACE) && point_in_polygon(m_polygon, m_num_points, x, y)) {
    return POHIT_FACE;
  }
  return 0;
}
CC_INLINE int point_in_polygon_hittest2(const FPOINT* m_polygon, const int* lens, int n, double x, double y, double m_point_radius, int hitmark) {
  int i, ret = 0;
  for (i=0; i<n; ++i) {
  ret = point_in_polygon_hittest(m_polygon, lens[i], x, y, m_point_radius, hitmark);
    if (ret) {
      return ret;
    }
    m_polygon += lens[i];
  }
  return 0;
}
// 线段树struct LINETREE{};
// 浮点误差的处理
CC_INLINE int dblcmp(double d)
{
  if (FABS(d) < CC_EPS) {
    return 0;
  }
  return (d > 0) ? 1 : - 1;
}
// <一>点的基本运算
// 返回两点之间欧氏距离
CC_INLINE double dist(FPOINT p1, FPOINT p2)
{
  return (sqrt(((p1).x - (p2).x) * ((p1).x - (p2).x) + ((p1).y - (p2).y) * ((p1).y - (p2).y)));
}
// 判断两个点是否重合
CC_INLINE BOOL equal_fpoint(FPOINT p1, FPOINT p2)
{
  return ((ABS(p1.x - p2.x) < CC_EPS) && (ABS(p1.y - p2.y) < CC_EPS));
}
/*(sp-op)*(ep-op)的叉积
r=multiply(sp, ep, op), 得到(sp-op)*(ep-op)的叉积
r>0: sp在矢量op ep的顺时针方向;
r=0：op sp ep三点共线;
r<0: sp在矢量op ep的逆时针方向 */
CC_INLINE double multiply(FPOINT sp, FPOINT ep, FPOINT op)
{
  return ((sp.x - op.x) * (ep.y - op.y) - (ep.x - op.x) * (sp.y - op.y));
}
CC_INLINE double amultiply(FPOINT sp, FPOINT ep, FPOINT op)
{
  double x = (sp.x - op.x) * (ep.y - op.y) - (ep.x - op.x) * (sp.y - op.y);
  return FABS(x);
}
/*矢量(p1-op)和(p2-op)的点积
r=dotmultiply(p1, p2, op), 得到矢量(p1-op)和(p2-op)的点积如果两个矢量都非零矢量
r < 0: 两矢量夹角为锐角;
r = 0：两矢量夹角为直角;
r > 0: 两矢量夹角为钝角 */
CC_INLINE double dotmultiply(FPOINT p1, FPOINT p2, FPOINT p0)
{
  return ((p1.x - p0.x) * (p2.x - p0.x) + (p1.y - p0.y) * (p2.y - p0.y));
}
/* 判断点p是否在线段l上
条件：(p在线段l所在的直线上)&& (点p在以线段l为对角线的矩形内) */
CC_INLINE BOOL online(FLINESEG2 l, FPOINT p)
{
  return ((multiply(l.b, p, l.a) == 0) && (((p.x - l.a.x) * (p.x - l.b.x) <= 0) && ((p.y - l.a.y) * (p.y - l.b.y) <= 0)));
}
// 返回点p以点o为圆心逆时针旋转alpha(单位：弧度)后所在的位置
CC_INLINE FPOINT rotate(FPOINT o, double alpha, FPOINT p)
{
  FPOINT tp;
  p.x -= o.x;
  p.y -= o.y;
  tp.x = (double)(p.x * cos(alpha) - p.y * sin(alpha) + o.x);
  tp.y = (double)(p.y * cos(alpha) + p.x * sin(alpha) + o.y);
  return tp;
}
/* 返回顶角在o点, 起始边为os, 终止边为oe的夹角(单位：弧度)
角度小于pi, 返回正值
角度大于pi, 返回负值
可以用于求线段之间的夹角 */
CC_INLINE double angle(FPOINT o, FPOINT s, FPOINT e)
{
  double cosfi, fi, norm;
  double dsx = s.x - o.x;
  double dsy = s.y - o.y;
  double dex = e.x - o.x;
  double dey = e.y - o.y;
  cosfi = dsx * dex + dsy * dey;
  norm = (dsx * dsx + dey * dey) * (dex * dex + dey * dey);
  cosfi /= SQRT(norm);
  if (cosfi >= 1.f) {
    return 0;
  }
  if (cosfi <= - 1.f) {
    return - (double)CC_PI;
  }
  fi = ACOS(cosfi);
  if (dsx * dey - dsy * dex > 0) {
    return fi;
  }
  // 说明矢量os 在矢量 oe的顺时针方向
  return - fi;
}
// <二>线段及直线的基本运算
// 判断点C在线段AB所在的直线l上垂足P的与线段AB的关系
// 本函数是根据下面的公式写的, P是点C到线段AB所在直线的垂足
// AC dot AB
// r = -----------
// ||AB||^2
// (Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
// = ---------------------------------
// L^2
// r has the following meaning:
// r=0 P = A
// r=1 P = B
// r<0 P is on the backward extension of AB
// r>1 P is on the forward extension of AB
// 0<r<1 P is interior to AB
CC_INLINE double relation(FPOINT c, FLINESEG2 l)
{
  FLINESEG2 tl;
  tl.a = l.a;
  tl.b = c;
  return dotmultiply(tl.b, l.b, l.a) / (dist(l.a, l.b) * dist(l.a, l.b));
}
// 求点C到线段AB所在直线的垂足 P
CC_INLINE FPOINT perpendicular(FPOINT p, FLINESEG2 l)
{
  FPOINT tp;
  double r = relation(p, l);
  tp.x = (double)(l.a.x + r * (l.b.x - l.a.x));
  tp.y = (double)(l.a.y + r * (l.b.y - l.a.y));
  return tp;
}
/* 求点p到线段l的最短距离
返回线段上距该点最近的点np
注意：np是线段l上到点p最近的点, 不一定是垂足 */
CC_INLINE double ptoTLineSegdist(FPOINT p, FLINESEG2 l, FPOINT* np)
{
  FPOINT n;
  double r = relation(p, l);
  if (r < 0) {
    n = l.a;
  }
  else if (r > 1) {
    n = l.b;
    return dist(p, l.b);
  }
  else {
    n = perpendicular(p, l);
  }
  if (np) {
    *np = n;
  }
  return dist(p, n);
}
// 求点p到线段l所在直线的距离
// 请注意本函数与上个函数的区别
CC_INLINE double ptoldist(FPOINT p, FLINESEG2 l)
{
  double m = multiply(p, l.b, l.a);
  return ABS(m) / dist(l.a, l.b);
}
/* 计算点到折线集的最近距离, 并返回最近点.
注意：调用的是ptoTLineSeg()函数 */
CC_INLINE double ptopointset(int vcount, FPOINT* pointset, FPOINT p, FPOINT* q)
{
  int i;
  double cd = CC_INFINITY, td;
  FLINESEG2 l;
  FPOINT tq = {0}, cq = {0};
  for (i = 0; i < vcount - 1; i++) {
    l.a = pointset[i];
    l.b = pointset[i + 1];
    td = ptoTLineSegdist(p, l, &tq);
    if (td < cd) {
      cd = td;
      cq = tq;
    }
  }
  if (q) {
    *q = cq;
  }
  return cd;
}
/* 判断圆是否在多边形内*/
CC_INLINE BOOL circle_inside_polygon(int vcount, FPOINT* polygon, FPOINT center, double radius)
{
  FPOINT q;
  double d;
  q.x = 0;
  q.y = 0;
  d = ptopointset(vcount, polygon, center, &q);
  return (d < radius || FABS(d - radius) < CC_EPS);
}
/* 返回两个矢量l1和l2的夹角的余弦 (-1 ~ 1)
注意：如果想从余弦求夹角的话, 注意反余弦函数的值域是从 0到pi */
CC_INLINE double coSINe(FLINESEG2 l1, FLINESEG2 l2)
{
  double aa = (l1.b.x - l1.a.x) * (l2.b.x - l2.a.x) + (l1.b.y - l1.a.y) * (l2.b.y - l2.a.y);
  double dd = (dist(l1.b, l1.a) * dist(l2.b, l2.a));
  return (aa / dd);
}
// 返回线段l1与l2之间的夹角
// 单位：弧度 范围(-pi, pi)
CC_INLINE double lsangle(FLINESEG2 l1, FLINESEG2 l2)
{
  FPOINT o, s, e;
  o.x = o.y = 0;
  s.x = l1.b.x - l1.a.x;
  s.y = l1.b.y - l1.a.y;
  e.x = l2.b.x - l2.a.x;
  e.y = l2.b.y - l2.a.y;
  return angle(o, s, e);
}
// 判断线段u和v相交(包括相交在端点处)
CC_INLINE BOOL intersect(FLINESEG2 u, FLINESEG2 v)
{
  return ((MAX(u.a.x, u.b.x) >= MIN(v.a.x, v.b.x)) && // 排斥实验
      (MAX(v.a.x, v.b.x) >= MIN(u.a.x, u.b.x)) && (MAX(u.a.y, u.b.y) >= MIN(v.a.y, v.b.y)) && (MAX(v.a.y, v.b.y) >= MIN(u.a.y, u.b.y)) && (multiply(v.a, u.b, u.a) * multiply(u.b, v.b, u.a) >= 0) && // 跨立实验
      (multiply(u.a, v.b, v.a) * multiply(v.b, u.b, v.a) >= 0));
}
//判断两个点是否相等
#define Euqal_Point(p1, p2) ((fabs(p1.x - p2.x) < EP) && (fabs(p1.y - p2.y) < EP))
//当且仅当u,v相交并且交点不是u,v的端点时函数为true;
CC_INLINE int intersect_Av2(FLINESEG2 u, FLINESEG2 v)
{
  return ((intersect(u, v)) && (!Euqal_Point(u.a, v.a)) && (!Euqal_Point(u.a, v.b)) && (!Euqal_Point(u.b, v.a)) && (!Euqal_Point(u.b, v.b)));
}
// 判断线段u和v相交（不包括双方的端点）
CC_INLINE BOOL intersect_A(FLINESEG2 u, FLINESEG2 v)
{
  return ((intersect(u, v)) && (!online(u, v.a)) && (!online(u, v.b)) && (!online(v, u.b)) && (!online(v, u.a)));
}
// 判断线段v所在直线与线段u相交
// 方法：判断线段u是否跨立线段v
CC_INLINE BOOL intersect_l(FLINESEG2 u, FLINESEG2 v)
{
  return multiply(u.a, v.b, v.a) * multiply(v.b, u.b, v.a) >= 0;
}
// 根据已知两点坐标, 求过这两点的直线解析方程： a*x+b*y+c = 0 (a >= 0)
CC_INLINE DFLINE2 makeline(FPOINT p1, FPOINT p2)
{
  DFLINE2 tl;
  int sign = 1;
  tl.a = p2.y - p1.y;
  if (tl.a < 0) {
    sign = - 1;
    tl.a = sign * tl.a;
  }
  tl.b = sign * (p1.x - p2.x);
  tl.c = sign * (p1.y * p2.x - p1.x * p2.y);
  return tl;
}
// 根据直线解析方程返回直线的斜率k, 水平线返回 0, 竖直线返回 1e200
CC_INLINE double slope(DFLINE2 l)
{
  if (ABS(l.a) < 1e-20) {
    return 0;
  }
  if (ABS(l.b) < 1e-20) {
    return CC_INFINITY;
  }
  return - (l.a / l.b);
}
// 返回直线的倾斜角alpha ( 0 - pi)
// 注意：atan()返回的是 -CC_PI/2 ~ CC_PI/2
CC_INLINE double line_alpha(DFLINE2 l)
{
  double k;
  if (ABS(l.a) < CC_EPS) {
    return 0;
  }
  if (ABS(l.b) < CC_EPS) {
    return (double)CC_PI / 2.f;
  }
  k = slope(l);
  if (k > 0) {
    return ATAN(k);
  }
  else {
    return (double)CC_PI + ATAN(k);
  }
}
// 求点p关于直线l的对称点
CC_INLINE FPOINT symmetry(DFLINE2 l, FPOINT p)
{
  FPOINT tp;
  tp.x = (double)(((l.b * l.b - l.a * l.a) * p.x - 2 * l.a * l.b * p.y - 2 * l.a * l.c) / (l.a * l.a + l.b * l.b));
  tp.y = (double)(((l.a * l.a - l.b * l.b) * p.y - 2 * l.a * l.b * p.x - 2 * l.b * l.c) / (l.a * l.a + l.b * l.b));
  return tp;
}
// 如果两条直线 l1(a1*x+b1*y+c1 = 0), l2(a2*x+b2*y+c2 = 0)相交, 返回TRUE, 且返回交点p
CC_INLINE BOOL lineintersect(DFLINE2 l1, DFLINE2 l2, FPOINT* p)
{
  // 是 L1, L2
  double d = l1.a * l2.b - l2.a * l1.b;
  if (ABS(d) < CC_EPS)
    // 不相交
  {
    return FALSE;
  }
  (*p).x = (double)((l2.c * l1.b - l1.c * l2.b) / d);
  (*p).y = (double)((l2.a * l1.c - l1.a * l2.c) / d);
  return TRUE;
}
// 如果线段l1和l2相交, 返回TRUE且交点由(inter)返回, 否则返回FALSE
CC_INLINE BOOL intersection(FLINESEG2 l1, FLINESEG2 l2, FPOINT* inter)
{
  DFLINE2 ll1, ll2;
  ll1 = makeline(l1.a, l1.b);
  ll2 = makeline(l2.a, l2.b);
  if (lineintersect(ll1, ll2, inter)) {
    return online(l1, *inter);
  }
  else {
    return FALSE;
  }
}
// 求直线交点
// 返回0 两直线平行
CC_INLINE double lineintersect2(FPOINT p1, FPOINT p2, FPOINT p3, FPOINT p4, FPOINT* pt)
{
  double x21 = p2.x - p1.x, y21 = p2.y - p1.y;
  double x43 = p4.x - p3.x, y43 = p4.y - p3.y;
  double t3 = x43 * y21 - x21 * y43;
  if (t3) {
    double t1 = x43 * p3.y - y43 * p3.x;
    double t2 = y21 * p1.x - x21 * p1.y;
    pt->x = (x21 * t1 + x43 * t2) / t3;
    pt->y = (y21 * t1 + y43 * t2) / t3;
  }
  return t3;
}
// <三> 多边形常用算法模块
// 如果无特别说明, 输入多边形顶点要求按逆时针排列
// 输入的多边形是简单多边形，返回TRUE
// 要 求: 输入顶点序列按逆时针排序
// 说 明: 简单多边形定义:
// 1: 循环排序中相邻线段对的交是他们之间共有的单个点
// 2: 不相邻的线段不相交
// 本程序默认第一个条件已经满足
CC_INLINE BOOL issimple(int vcount, const FPOINT* polygon)
{
  int i, cn;
  FLINESEG2 l1, l2;
  for (i = 0; i < vcount; i++) {
    l1.a = polygon[i];
    l1.b = polygon[(i + 1) % vcount];
    cn = vcount - 3;
    while (cn) {
      l2.a = polygon[(i + 2) % vcount];
      l2.b = polygon[(i + 3) % vcount];
      if (intersect(l1, l2)) {
        break;
      }
      cn--;
    }
    if (cn) {
      return FALSE;
    }
  }
  return TRUE;
}
// 多边形polygon是凸多边形时，返回TRUE
// 按输入顺序返回多边形顶点的凸凹性判断，bc[i]=1,iff:第i个顶点是凸顶点
CC_INLINE BOOL isconvex(int vcount, const FPOINT* polygon, BOOL* bc)
{
  int i, count, index = 0, ret = 1;
  FPOINT tp = polygon[0];
  // 寻找第一个凸顶点
  for (i = 1; i < vcount; ++i) {
    if (polygon[i].y < tp.y || (polygon[i].y == tp.y && polygon[i].x < tp.x)) {
      tp = polygon[i];
      index = i;
    }
  }
  count = vcount - 1;
  if (bc) {
    bc[index] = 1;
  }
  // 判断凸凹性
  while (count) {
    if (multiply(polygon[(index + 1) % vcount], polygon[(index + 2) % vcount], polygon[index]) >= 0) {
      if (bc) {
        bc[(index + 1) % vcount] = 1;
      }
      else {
        if (bc) {
          bc[(index + 1) % vcount] = 0;
        }
        ret = 0;
      }
    }
    index++;
    count--;
  }
  return ret;
}
// 计算多边形的面积的两倍
// 要求按照逆时针方向输入多边形的顶点
// 可以是凸多边形或凹多边形
CC_INLINE double poly_area(int n, FPOINT* pt)
{
  int i;
  double s;
  if (n < 3) {
    return 0;
  }
  s = pt[0].y * (pt[n - 1].x - pt[1].x);
  for (i = 1; i < n; i++) {
    s += pt[i].y * (pt[(i - 1)].x - pt[(i + 1) % n].x);
  }
  return s;
}
// 返回多边形面积(signed);
// 输入顶点按逆时针排列时, 返回正值; 否则返回负值
#define AREA_OF_POLYGON( vcount, polygon ) (0.5*poly_area(vcount, polygon))
// 如果输入顶点按逆时针排列, 返回TRUE
#define is_conter_clock( vcount, polygon ) (poly_area( vcount, polygon ) > 0)
// 另一种判断多边形顶点排列方向的方法
CC_INLINE BOOL isccwize(int vcount, FPOINT* polygon)
{
  int i, index;
  FPOINT a, b, v;
  v = polygon[0];
  index = 0;
  for (i = 1; i < vcount; ++i) {
    // 找到最低且最左顶点，肯定是凸顶点
    if (polygon[i].y < v.y || (polygon[i].y == v.y && polygon[i].x < v.x)) {
      index = i;
    }
  }
  // 顶点v的前一顶点
  a = polygon[(index - 1 + vcount) % vcount];
  // 顶点v的后一顶点
  b = polygon[(index + 1) % vcount];
  return (multiply(v, b, a) > 0);
}
// 射线法判断点q与多边形polygon的位置关系
// 要求polygon为简单多边形，顶点逆时针排列
// 如果点在多边形内: 返回0
// 如果点在多边形边上: 返回1
// 如果点在多边形外: 返回2
CC_INLINE int inside_polygon(int vcount, const FPOINT* polygon, FPOINT q)
{
  int c = 0, i;
  FLINESEG2 l1, l2;
  l1.a = q;
  l1.b = q;
  l1.b.x = CC_INFINITY;
  for (i = 0; i < vcount; i++) {
    l2.a = polygon[i];
    l2.b = polygon[(i + 1) % vcount];
    if (online(l2, q)) {
      return 1;
    }
    if (intersect_A(l1, l2)) {
      c++;
    }
    // 相交且不在端点
    if (online(l1, l2.b) && !online(l1, l2.a) && l2.b.y > l2.b.y) {
      c++;
    }
    // l2的一个端点在l1上且该端点是两端点中纵坐标较大的那个
    if (!online(l1, l2.b) && online(l1, l2.a) && l2.b.y < l2.b.y) {
      c++;
    }
    // 忽略平行边
  }
  return (c % 2 == 1) ? 0 : 2;
}
CC_INLINE int inside_polygon2(int vcount, FPOINT* polygon, FPOINT q)
{
  int c = 0, i, n;
  FLINESEG2 l1, l2;
  double r1, r2;
  l1.a = q;
  l1.b = q;
  l1.b.x = CC_INFINITY;
  n = vcount;
  for (i = 0; i < vcount; ++i) {
    l2.a = polygon[i];
    l2.b = polygon[(i + 1) % n];
    if (online(l2, q)) {
      // 如果点在边上，返回1
      return 1;
    }
    if ((intersect_A(l1, l2)) || // 相交且不在端点
        (((online(l1, polygon[(i + 1) % n])) && // 第二个端点在射线上
            ((!(online(l1, polygon[(i + 2) % n]))) &&
                // 前一个端点和后一个端点在射线两侧
                (((r1 = multiply(polygon[i], polygon[(i + 1) % n], l1.a) * multiply(polygon[(i + 1) % n], polygon[(i + 2) % n], l1.a)) > 0) || 
				((online(l1, polygon[(i + 2) % n])) &&
                // 下一条边是水平线，前一个端点和后一个端点在射线两侧
                ((r2 = multiply(polygon[i], polygon[(i + 2) % n], l1.a) * multiply(polygon[(i + 2) % n], polygon[(i + 3) % n], l1.a)) > 0))))))) {
      c++;
    }
  }
  if (c % 2 == 1) {
    return 0;
  }
  else {
    return (2);
  }
}
// 判断点q在凸多边形polygon内
// 点q是凸多边形polygon内[包括边上]时, 返回TRUE
// 注意：多边形polygon一定要是凸多边形
CC_INLINE BOOL inside_convex_polygon(int vcount, const FPOINT* polygon, FPOINT q)
{
  FPOINT p;
  FLINESEG2 l;
  int i;
  p.x = 0;
  p.y = 0;
  // 寻找一个肯定在多边形polygon内的点p：多边形顶点平均值
  for (i = 0; i < vcount; i++) {
    p.x += polygon[i].x;
    p.y += polygon[i].y;
  }
  p.x /= vcount;
  p.y /= vcount;
  for (i = 0; i < vcount; i++) {
    l.a = polygon[i];
    l.b = polygon[(i + 1) % vcount];
    if (multiply(p, l.b, l.a) *multiply(q, l.b, l.a) < 0) {
      /* 点p和点q在边l的两侧, 说明点q肯定在多边形外 */
      return FALSE;
    }
  }
  return TRUE;
}

// 寻找凸包的 graham 扫描法
// pointset为输入的点集;
// ch为输出的凸包上的点集, 按照逆时针方向排列;
// setnum为pointset中的点的数目
// len为输出的凸包上的点的个数
CC_INLINE int graham_scan(int setnum, FPOINT* pointset, FPOINT* ch)
{
  int i, j, k = 0, top = 2, len;
  FPOINT tmp;
  // 选取pointset中y坐标最小的点pointset[k], 如果这样的点有多个, 则取最左边的一个
  for (i = 1; i < setnum; ++i) {
    if (pointset[i].y < pointset[k].y || ((pointset[i].y == pointset[k].y) && (pointset[i].x < pointset[k].x))) {
      k = i;
    }
  }
  tmp = pointset[0];
  pointset[0] = pointset[k];
  pointset[k] = tmp; // 现在pointset中y坐标最小的点在pointset[0]
  // 对顶点按照相对pointset[0]的极角从小到大进行排序,
  // 极角相同的按照距离pointset[0]从近到远进行排序
  for (i = 1; i < setnum - 1; ++i) {
    for (k = i, j = i + 1; j < setnum; ++j) {
      if (multiply(pointset[j], pointset[k], pointset[0]) > 0 || // 极角更小
          ((multiply(pointset[j], pointset[k], pointset[0]) == 0) && //极角相等, 距离更短
          dist(pointset[0], pointset[j]) < dist(pointset[0], pointset[k]))) {
        k = j;
      }
    }
    tmp = pointset[i];
    pointset[i] = pointset[k];
    pointset[k] = tmp;
  }
  ch[0] = pointset[0];
  ch[1] = pointset[1];
  ch[2] = pointset[2];
  for (i = 3; i < setnum; i++) {
    while (multiply(pointset[i], ch[top], ch[top - 1]) >= 0) {
      top--;
    }
    ch[++top] = pointset[i];
  }
  len = top + 1;
  return len;
}
// 卷包裹法求点集凸壳, 参数说明同 graham 算法
CC_INLINE int convex_closure(int setnum, const FPOINT* pointset, FPOINT* ch)
{
  int top = 0, i, index, first, len;
  double curmax, curcos, curdis;
  FPOINT tmp;
  FLINESEG2 l1, l2;
  BOOL* use = (BOOL*)pmalloc(setnum * sizeof(BOOL));
  if (setnum <= 0) {
    return 0;
  }
  tmp = pointset[0];
  index = 0;
  // 选取y最小点, 如果多于一个, 则选取最左点
  for (i = 0; i < setnum; ++i) {
    use[i] = FALSE;
    if ((pointset[i].y < tmp.y) || (pointset[i].y == tmp.y && pointset[i].x < tmp.x)) {
      index = i;
    }
    use[i] = FALSE;
  }
  tmp = pointset[index];
  first = index;
  use[index] = TRUE;
  index = - 1;
  ch[top++] = tmp;
  tmp.x -= 100;
  l1.a = tmp;
  l1.b = ch[0];
  l2.a = ch[0];
  while (index != first) {
    curmax = - 100;
    curdis = 0;
    index = - 1;
    // 选取与最后一条确定边夹角最小的点, 即余弦值最大者
    for (i = 0; i < setnum; ++i) {
      if (use[i]) {
        continue;
      }
      l2.b = pointset[i];
      curcos = coSINe(l1, l2); // 根据cos值求夹角余弦, 范围在 （-1 -- 1 ）
      if (curcos > curmax || (FABS(curcos - curmax) < CC_EPS && dist(l2.a, l2.b) > curdis)) {
        curmax = curcos;
        index = i;
        curdis = dist(l2.a, l2.b);
      }
    }
    if (index < 0) {
      break;
    }
    use[first] = FALSE; // 清空第first个顶点标志, 使最后能形成封闭的hull
    use[index] = TRUE;
    ch[top++] = pointset[index];
    l1.a = ch[top - 2];
    l1.b = ch[top - 1];
    l2.a = ch[top - 1];
  }
  len = top;
  pfree(use);
  return len;
}
// 求任意简单多边形polygon的重心
// 需要调用下面几个函数:
// void AddPosPart(); 增加右边区域的面积
// void AddNegPart(); 增加左边区域的面积
// void AddRegion(); 增加区域面积
// 在使用该程序时，如果把xtr,ytr,wtr,xtl,ytl,wtl设成全局变量就可以使
// 这些函数的形式得到化简,但要注意函数的声明和调用要做相应变化
CC_INLINE void AddPosPart(double x, double y, double w, double* xtr, double* ytr, double* pwtr)
{
  if (fabs(*pwtr + w) < 1e-10) {
    return ;
  } // detect zero regions
  *xtr = (*pwtr** xtr + w * x) / (*pwtr + w);
  *ytr = (*pwtr** ytr + w * y) / (*pwtr + w);
  *pwtr = w + *pwtr;
  return ;
}
CC_INLINE void AddNegPart(double x, double y, double w, double* xtl, double* ytl, double* wtl)
{
  if (fabs(*wtl + w) < 1e-10) {
    return ;
  }
  // detect zero regions
  *xtl = (*wtl** xtl + w * x) / (*wtl + w);
  *ytl = (*wtl** ytl + w * y) / (*wtl + w);
  *wtl = w + *wtl;
  return ;
}
CC_INLINE void AddRegion(double x1, double y1, double x2, double y2, double* xtr, double* ytr, double* wtr, double* xtl, double* ytl, double* wtl)
{
  if (fabs(x1 - x2) < 1e-10) {
    return ;
  }
  if (x2 > x1) {
    // rectangle 全局变量变化处
    AddPosPart((x2 + x1) / 2, y1 / 2, (x2 - x1) *y1, xtr, ytr, wtr);
    // triangle 全局变量变化处
    AddPosPart((x1 + x2 + x2) / 3, (y1 + y1 + y2) / 3, (x2 - x1) * (y2 - y1) / 2, xtr, ytr, wtr);
  }
  else {
    // rectangle 全局变量变化处
    AddNegPart((x2 + x1) / 2, y1 / 2, (x2 - x1) *y1, xtl, ytl, wtl);
    // triangle 全局变量变化处
    AddNegPart((x1 + x2 + x2) / 3, (y1 + y1 + y2) / 3, (x2 - x1) * (y2 - y1) / 2, xtl, ytl, wtl);
  }
}
CC_INLINE FPOINT cg_simple(int vcount, FPOINT* polygon)
{
  int i;
  //注意: 如果把xtr,ytr,wtr,xtl,ytl,wtl改成全局变量后这里要删去
  double xtr, ytr, wtr, xtl, ytl, wtl;
  FPOINT p1, p2, tp;
  xtr = ytr = wtr = 0.0;
  xtl = ytl = wtl = 0.0;
  for (i = 0; i < vcount; ++i) {
    p1 = polygon[i];
    p2 = polygon[(i + 1) % vcount];
    // 全局变量变化处
    AddRegion(p1.x, p1.y, p2.x, p2.y, &xtr, &ytr, &wtr, &xtl, &ytl, &wtl);
  }
  tp.x = (double)((wtr * xtr + wtl * xtl) / (wtr + wtl));
  tp.y = (double)((wtr * ytr + wtl * ytl) / (wtr + wtl));
  return tp;
}
// 求凸多边形的重心, 要求输入多边形按逆时针排序
CC_INLINE FPOINT gravity_center(int vcount, const FPOINT* polygon)
{
  FPOINT tp;
  double x, y, s, x0, y0, cs, k;
  int i;
  x = y = s = 0.f;
  for (i = 1; i < vcount - 1; i++) {
    x0 = (polygon[0].x + polygon[i].x + polygon[i + 1].x) / 3;
    y0 = (polygon[0].y + polygon[i].y + polygon[i + 1].y) / 3; // 求当前三角形的重心
    cs = multiply(polygon[i], polygon[i + 1], polygon[0]) / 2;
    // 三角形面积可以直接利用该公式求解
    if (ABS(s) < 1e-20) {
      x = x0;
      y = y0;
      s += cs;
      continue;
    }
    k = cs / s; // 求面积比例
    x = (x + k * x0) / (1 + k);
    y = (y + k * y0) / (1 + k);
    s += cs;
  }
  tp.x = (double)(x);
  tp.y = (double)(y);
  return tp;
}
// 可用射线法来判断一个点是否在多边形的内部:
// 射线法就是以这个要判断的点作一射线(为了方便，直接作一水平向右的射线)，数一下线段与
// 多边形边的交点数，奇数时就是在多边形内，偶数时就是在多边形外。
// 判断线段是否在简单多边形内(注意: 如果多边形是凸多边形，下面的算法可以化简)
// 原理:
// 必要条件一: 线段的两个端点都在多边形内；
// 必要条件二: 线段和多边形的所有边都不内交；
// 用途: 1. 判断折线是否在简单多边形内
// 2. 判断简单多边形是否在另一个简单多边形内
CC_INLINE BOOL TLineSeg_inside_polygon(int vcount, FPOINT* polygon, FLINESEG2 l)
{
  int top = 0, i, j;
  FPOINT* PointSet, tmp;
  FLINESEG2 s;
  // 判断线端l的端点是否不都在多边形内
  if (!inside_polygon(vcount, polygon, l.a) || !inside_polygon(vcount, polygon, l.b)) {
    return FALSE;
  }
  PointSet = MALLOC(FPOINT, vcount);
  for (i = 0; i < vcount; ++i) {
    s.a = polygon[i];
    s.b = polygon[(i + 1) % vcount];
    if (online(s, l.a)) {
      // 线段l的起始端点在线段s上
      PointSet[top++] = l.a;
    }
    else if (online(s, l.b)) {
      // 线段l的终止端点在线段s上
      PointSet[top++] = l.b;
    }
    else {
      if (online(l, s.a)) {
        // 线段s的起始端点在线段l上
        PointSet[top++] = s.a;
      }
      else if (online(l, s.b)) {
        // 线段s的终止端点在线段l上
        PointSet[top++] = s.b;
      }
      else {
        if (intersect(l, s)) {
          // 这个时候如果相交，肯定是内交，返回FALSE
          pfree(PointSet);
          return FALSE;
        }
      }
    }
  }
  // 冒泡排序，x坐标小的排在前面；x坐标相同者，
  // y坐标小的排在前面
  for (i = 0; i < top - 1; ++i) {
    for (j = i + 1; j < top; ++j) {
      if (PointSet[i].x > PointSet[j].x || (fabs(PointSet[i].x - PointSet[j].x) < CC_EPS && PointSet[i].y > PointSet[j].y)) {
        tmp = PointSet[i];
        PointSet[i] = PointSet[j];
        PointSet[j] = tmp;
      }
    }
  }
  for (i = 0; i < top - 1; ++i) {
    // 得到两个相邻交点的中点
    tmp.x = (PointSet[i].x + PointSet[i + 1].x) / 2;
    tmp.y = (PointSet[i].y + PointSet[i + 1].y) / 2;
    if (!inside_polygon(vcount, polygon, tmp)) {
      FREE(PointSet);
      return FALSE;
    }
  }
  FREE(PointSet);
  return TRUE;
}
// 给定一简单多边形，找出一个肯定在该多边形内的点
// 定理1: 每个多边形至少有一个凸顶点
// 定理2: 顶点数>=4的简单多边形至少有一条对角线
// 结论: x坐标最大，最小的点肯定是凸顶点
// y坐标最大，最小的点肯定是凸顶点
CC_INLINE FPOINT a_point_insidepoly(int vcount, FPOINT* polygon)
{
  FPOINT v = {0}, a = {0}, b = {0}, r = {0};
  FPOINT tri[3], q = {0};
  int i, index;
  double md;
  BOOL bin;
  v = polygon[0];
  index = 0;
  for (i = 1; i < vcount; ++i) {
    //寻找一个凸顶点
    if (polygon[i].y < v.y) {
      v = polygon[i];
      index = i;
    }
  }
  //得到v的前一个顶点
  a = polygon[(index - 1 + vcount) % vcount];
  //得到v的后一个顶点
  b = polygon[(index + 1) % vcount];
  tri[0] = a;
  tri[1] = v;
  tri[2] = b;
  md = CC_INFINITY;
  //in1 = index;
  bin = FALSE;
  //寻找在三角形avb内且离顶点v最近的顶点q
  for (i = 0; i < vcount; ++i) {
    if (i == index) {
      continue;
    }
    if (i == (index - 1 + vcount) % vcount) {
      continue;
    }
    if (i == (index + 1) % vcount) {
      continue;
    }
    if (!inside_convex_polygon(3, tri, polygon[i])) {
      continue;
    }
    bin = TRUE;
    if (dist(v, polygon[i]) < md) {
      q = polygon[i];
      md = dist(v, q);
    }
  }
  //没有顶点在三角形avb内，返回线段ab中点
  if (!bin) {
    r.x = (a.x + b.x) / 2;
    r.y = (a.y + b.y) / 2;
    return r;
  }
  //返回线段vq的中点
  r.x = (v.x + q.x) / 2;
  r.y = (v.y + q.y) / 2;
  return r;
}
// 求从多边形外一点p出发到一个简单多边形的切线,如果存在返回切点,其中rp点是右切点,lp是左切点
// 注意: p点一定要在多边形外
// 输入顶点序列是逆时针排列
// 原 理:如果点在多边形内肯定无切线; 凸多边形有唯一的两个切点,凹多边形就可能有多于两个的切点;
// 如果polygon是凸多边形，切点只有两个只要找到就可以,可以化简此算法
// 如果是凹多边形还有一种算法可以求解:先求凹多边形的凸包,然后求凸包的切线
CC_INLINE void pointtangentpoly(int vcount, FPOINT* polygon, FPOINT p, FPOINT* rp, FPOINT* lp)
{
  FLINESEG2 ep, en;
  BOOL blp, bln;
  int i;
  *rp = polygon[0];
  *lp = polygon[0];
  for (i = 1; i < vcount; ++i) {
    ep.a = polygon[(i + vcount - 1) % vcount];
    ep.b = polygon[i];
    en.a = polygon[i];
    en.b = polygon[(i + 1) % vcount];
    blp = multiply(ep.b, p, ep.a) >= 0; // p is to the left of pre edge
    bln = multiply(en.b, p, en.a) >= 0; // p is to the left of next edge
    if (!blp && bln) {
      if (multiply(polygon[i], *rp, p) > 0) {
        // polygon[i] is above rp
        *rp = polygon[i];
      }
    }
    if (blp && !bln) {
      if (multiply(*lp, polygon[i], p) > 0) {
        // polygon[i] is below lp
        *lp = polygon[i];
      }
    }
  }
  return ;
}
// 如果多边形polygon的核存在，返回TRUE，返回核上的一点p.顶点按逆时针方向输入
CC_INLINE BOOL core_exist(int vcount, FPOINT* polygon, FPOINT* p)
{
  int i, j, k;
  FLINESEG2 l;
  DFLINE2* lineset;
  lineset = MALLOC(DFLINE2, vcount);
  for (i = 0; i < vcount; ++i) {
    lineset[i] = makeline(polygon[i], polygon[(i + 1) % vcount]);
  }
  for (i = 0; i < vcount; ++i) {
    for (j = 0; j < vcount; ++j) {
      if (i == j) {
        continue;
      }
      if (lineintersect(lineset[i], lineset[j], p)) {
        for (k = 0; k < vcount; ++k) {
          l.a = polygon[k];
          l.b = polygon[(k + 1) % vcount];
          //多边形顶点按逆时针方向排列，核肯定在每条边的左侧或边上
          if (multiply(*p, l.b, l.a) > 0) {
            break;
          }
        }
        //找到了一个核上的点
        if (k == vcount) {
          break;
        }
      }
    }
    if (j < vcount) {
      break;
    }
  }
  FREE(lineset);
  return (i < vcount);
}
// 所谓凸多边形的直径, 即凸多边形任两个顶点的最大距离。下面的算法
// 仅耗时O(n), 是一个优秀的算法。 输入必须是一个凸多边形, 且顶点
// 必须按顺序（顺时针、逆时针均可）依次输入。若输入不是凸多边形
// 而是一般点集, 则要先求其凸包。 就是先求出所有跖对, 然后求出每
// 个跖对的距离, 取最大者。点数要多于5个。
CC_INLINE double diameter(int n, const FPOINT* ch)
{
  int znum = 0, i, j, k = 1;
  int* zd = MALLOC(int, n * 2);
  double tmp, dia;
  while (amultiply(ch[0], ch[k + 1], ch[n - 1]) > amultiply(ch[0], ch[k], ch[n - 1]) - CC_EPS) {
    k++;
  }
  i = 0;
  j = k;
  while (i <= k && j < n) {
    zd[znum * 2] = i;
    zd[znum * 2 + 1] = j;
    ++znum;
    while (amultiply(ch[i + 1], ch[j + 1], ch[i]) > amultiply(ch[i + 1], ch[j], ch[i]) - CC_EPS && j < n - 1) {
      zd[znum * 2] = i;
      zd[znum * 2 + 1] = j;
      ++znum;
      j++;
    }
    i++;
  }
  dia = - 1.0;
  for (i = 0; i < znum; i++) {
    printf("%d %d\n", zd[i * 2], zd[i * 2 + 1]);
    tmp = dist(ch[zd[i * 2]], ch[zd[i * 2 + 1]]);
    if (dia < tmp) {
      dia = tmp;
    }
  }
  FREE(zd);
  return dia;
}
CC_INLINE int sklansky(FPOINT** array, int start, int end, int* stack, int nsign, int sign2)
{
  int incr = end > start ? 1 : - 1;
  /* prepare first triangle */
  int pprev = start, pcur = pprev + incr, pnext = pcur + incr;
  int stacksize = 3;
  if (start == end || (array[start]->x == array[end]->x && array[start]->y == array[end]->y)) {
    stack[0] = start;
    return 1;
  }
  stack[0] = pprev;
  stack[1] = pcur;
  stack[2] = pnext;
  end += incr; /* make end = afterend */
  while (pnext != end) {
    /* check the angle p1,p2,p3 */
    double cury = array[pcur]->y;
    double nexty = array[pnext]->y;
    double by = nexty - cury;
    if (CC_SIGN(by) != nsign) {
      double ax = array[pcur]->x - array[pprev]->x;
      double bx = array[pnext]->x - array[pcur]->x;
      double ay = cury - array[pprev]->y;
      double convexity = ay * bx - ax * by; /* if >0 then convex angle */
      if (CC_SIGN(convexity) == sign2 && (ax != 0 || ay != 0)) {
        pprev = pcur;
        pcur = pnext;
        pnext += incr;
        stack[stacksize] = pnext;
        stacksize++;
      }
      else {
        if (pprev == start) {
          pcur = pnext;
          stack[1] = pcur;
          pnext += incr;
          stack[2] = pnext;
        }
        else {
          stack[stacksize - 2] = pnext;
          pcur = pprev;
          pprev = stack[stacksize - 4];
          stacksize--;
        }
      }
    }
    else {
      pnext += incr;
      stack[stacksize - 1] = pnext;
    }
  }
  return --stacksize;
}
CC_INLINE int convex_hull2(int total, const FPOINT* array, FPOINT* hull, int orientation)
{
  FPOINT** pointer = 0;
  int* stack = 0;
  int* t_stack;
  int t_count, i, stop_idx;
  int hull_len = 0, miny_ind = 0, maxy_ind = 0;
  if (total == 0) {
    //if( array )
    // CC_ERROR( CC_StsBadSize,
    // "Point sequence can not be empty if the output is matrix" );
    return 0;
  }
  pointer = (FPOINT**)pmalloc(total * sizeof(pointer[0]));
  stack = (int*)pmalloc((total + 2) * sizeof(stack[0]));
  for (i = 0; i < total; i++) {
    pointer[i] = (FPOINT*)array + i;
  }
#define CMP_PTS( pt1, pt2 ) \
  ((pt1)->x < (pt2)->x || ((pt1)->x == (pt2)->x && (pt1)->y < (pt2)->y))
  // sort the point set by x-coordinate, find MIN and MAX y
  QSORT(pointer, pointer + total, CMP_PTS, FPOINT*);
#undef CMP_PTS
  //icvSortPointsByPointers_32s( pointer, total, 0 );
  for (i = 1; i < total; i++) {
    double y = pointer[i]->y;
    if (pointer[miny_ind]->y > y) {
      miny_ind = i;
    }
    if (pointer[maxy_ind]->y < y) {
      maxy_ind = i;
    }
  }
  if (pointer[0]->x == pointer[total - 1]->x && pointer[0]->y == pointer[total - 1]->y) {
    hull[0] = pointer[0][0];
    pfree(pointer);
    pfree(stack);
    return 1;
  }
  /*upper half */
  {
    int* tl_stack = stack;
    int tl_count = sklansky(pointer, 0, maxy_ind, tl_stack, - 1, 1);
    int* tr_stack = tl_stack + tl_count;
    int tr_count = sklansky(pointer, total - 1, maxy_ind, tr_stack, - 1, - 1);
    /* gather upper part of convex hull to output */
    if (orientation == CC_COUNTER_CLOCKWISE) {
      CC_SWAP(tl_stack, tr_stack, t_stack);
      CC_SWAP(tl_count, tr_count, t_count);
    }
    hull_len = 0;
    for (i = 0; i < tl_count - 1; i++) {
      hull[hull_len++] = pointer[tl_stack[i]][0];
    }
    for (i = tr_count - 1; i > 0; i--) {
      hull[hull_len++] = pointer[tr_stack[i]][0];
    }
    stop_idx = tr_count > 2 ? tr_stack[1] : tl_count > 2 ? tl_stack[tl_count - 2] : - 1;
  }
  /* lower half */
  {
    int* bl_stack = stack;
    int bl_count = sklansky(pointer, 0, miny_ind, bl_stack, 1, - 1);
    int* br_stack = stack + bl_count;
    int br_count = sklansky(pointer, total - 1, miny_ind, br_stack, 1, 1);
    if (orientation != CC_COUNTER_CLOCKWISE) {
      CC_SWAP(bl_stack, br_stack, t_stack);
      CC_SWAP(bl_count, br_count, t_count);
    }
    if (stop_idx >= 0) {
      int check_idx = bl_count > 2 ? bl_stack[1] : bl_count + br_count > 2 ? br_stack[2 - bl_count] : - 1;
      if (check_idx == stop_idx || (check_idx >= 0 && pointer[check_idx]->x == pointer[stop_idx]->x && pointer[check_idx]->y == pointer[stop_idx]->y)) {
        /* if all the points lie on the same line, then the bottom part of
        the convex hull is the mirrored top part (except the exteme points).*/
        bl_count = MIN(bl_count, 2);
        br_count = MIN(br_count, 2);
      }
    }
    for (i = 0; i < bl_count - 1; i++) {
      hull[hull_len++] = pointer[bl_stack[i]][0];
    }
    for (i = br_count - 1; i > 0; i--) {
      hull[hull_len++] = pointer[br_stack[i]][0];
    }
  }
  pfree(pointer);
  pfree(stack);
  // cvBoundingRect
  return hull_len;
}
// 圆的基本运算
// 点p在圆内(包括边界)时，返回TRUE
// 用途: 因为圆为凸集，所以判断点集，折线，多边形是否在圆内时，
// 只需要逐一判断点是否在圆内即可。
CC_INLINE BOOL point_in_circle1(FPOINT o, double r, FPOINT p)
{
  double d2 = (p.x - o.x) * (p.x - o.x) + (p.y - o.y) * (p.y - o.y);
  double r2 = r * r;
  return d2 < r2 || fabs(d2 - r2) < CC_EPS;
}
// 用 途: 求不共线的三点确定一个圆
// 输 入: 三个点p1,p2,p3
// 如果三点共线，返回FALSE；反之，返回TRUE。圆心由q返回，半径由r返回
CC_INLINE BOOL cocircle(FPOINT p1, FPOINT p2, FPOINT p3, FPOINT* q, double* r)
{
  double x12 = p2.x - p1.x, y12 = p2.y - p1.y;
  double x13 = p3.x - p1.x, y13 = p3.y - p1.y;
  double z2 = x12 * (p1.x + p2.x) + y12 * (p1.y + p2.y);
  double z3 = x13 * (p1.x + p3.x) + y13 * (p1.y + p3.y);
  double d = 2.f * (x12 * (p3.y - p2.y) - y12 * (p3.x - p2.x));
  //共线，圆不存在
  if (fabs(d) < CC_EPS) {
    return FALSE;
  }
  q->x = (double)((y13 * z2 - y12 * z3) / d);
  q->y = (double)((x12 * z3 - x13 * z2) / d);
  *r = dist(p1, *q);
  return TRUE;
}
CC_INLINE int line_circle(DFLINE2 l, FPOINT o, double r, FPOINT* p1, FPOINT* p2)
{
  return TRUE;
}
// 矩形的基本运算
// 说明: 因为矩形的特殊性，常用算法可以化简:
// 1.判断矩形是否包含点
// 只要判断该点的横坐标和纵坐标是否夹在矩形的左右边和上下边之间。
// 2.判断线段、折线、多边形是否在矩形中
// 因为矩形是个凸集，所以只要判断所有端点是否都在矩形中就可以了。
// 3.判断圆是否在矩形中
// 圆在矩形中的充要条件是: 圆心在矩形中且圆的半径小于等于圆心到矩形四边的距离的最小值。
// 已知矩形的三个顶点(a,b,c)，计算第四个顶点d的坐标. 注意: 已知的三个顶点可以是无序的
CC_INLINE FPOINT rect4th(FPOINT a, FPOINT b, FPOINT c)
{
  FPOINT d;
  // 说明c点是直角拐角处
  if (fabs(dotmultiply(a, b, c)) < CC_EPS) {
    d.x = a.x + b.x - c.x;
    d.y = a.y + b.y - c.y;
  }
  // 说明b点是直角拐角处
  if (fabs(dotmultiply(a, c, b)) < CC_EPS) {
    d.x = a.x + c.x - b.x;
    d.y = a.y + c.y - b.x;
  }
  // 说明a点是直角拐角处
  if (fabs(dotmultiply(c, b, a)) < CC_EPS) {
    d.x = c.x + b.x - a.x;
    d.y = c.y + b.y - a.y;
  }
  return d;
}
// 常用算法的描述
//
// 尚未实现的算法:
// 1. 求包含点集的最小圆
// 2. 求多边形的交
// 3. 简单多边形的三角剖分
// 4. 寻找包含点集的最小矩形
// 5. 折线的化简
// 6. 判断矩形是否在矩形中
// 7. 判断矩形能否放在矩形中
// 8. 矩形并的面积与周长
// 9. 矩形并的轮廓
// 10.矩形并的闭包
// 11.矩形的交
// 12.点集中的最近点对
// 13.多边形的并
// 14.圆的交与并
// 15.直线与圆的关系
// 16.线段与圆的关系
// 17.求多边形的核监视摄象机
// 18.求点集中不相交点对 railwai
// 寻找包含点集的最小矩形
// 原理: 该矩形至少一条边与点集的凸壳的某条边共线
// First take the convex hull of the points. Let the resulting convex
// polygon be P. It has been known for some time that the minimum
// area rectangle encloSINg P must have one rectangle side flush with
// (i.b., collinear with and overlapping) one edge of P. This geometric
// fact was used by Godfried Toussaint to develop the "rotating calipers"
// algorithm in a hard-to-find 1983 paper, "Solving Geometric Problems
// with the Rotating Calipers" (Proc. IEEE MELECON). The algorithm
// rotates a surrounding rectangle from one flush edge to the next,
// keeping track of the minimum area for each edge. It achieves O(n)
// time (after hull computation). See the "Rotating Calipers Homepage"
// http://www.cs.mcgill.ca/~orm/rotcal.frame.html for a description
// and applet.
// 折线的化简 伪码如下:
// Input: tol = the approximation tolerance
// L = {V0,V1,...,Vn-1} is any n-vertex polyline
//
// Set start = 0;
// Set k = 0;
// Set W0 = V0;
// for each vertex Vi (i=1,n-1)
// {
// if Vi is within tol from Vstart
// then ignore it, and continue with the next vertex
//
// Vi is further than tol away from Vstart
// so add it as a new vertex of the reduced polyline
// Increment k++;
// Set Wk = Vi;
// Set start = i; as the new initial vertex
// }
//
// Output: W = {W0,W1,...,Wk-1} = the k-vertex simplified polyline
//
// 补充
// 两圆关系:
// 外切: return 1；
// 内切: return 2；
// 相离: return 3；
// 内含: return 4；
// 相交: return 5；
CC_INLINE int CircleRelation(FPOINT p1, double r1, FPOINT p2, double r2)
{
  double d = SQRT((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
  // 必须保证前两个if先被判定！
  if (fabs(d - r1 - r2) < CC_EPS) {
    return 1;
  }
  if (fabs(d - fabs(r1 - r2)) < CC_EPS) {
    return 2;
  }
  if (d > r1 + r2) {
    return 3;
  }
  if (d < fabs(r1 - r2)) {
    return 4;
  }
  if (fabs(r1 - r2) < d && d < r1 + r2) {
    return 5;
  }
  return 0; // indicate an error!
}
// 判断圆是否在矩形内:
// 判定圆是否在矩形内，是就返回TRUE（设矩形水平，且其四个顶点由左上开始按顺时针排列）
// 调用ptoldist函数，在第4页
CC_INLINE BOOL CircleRecRelation(FPOINT pc, double r, FPOINT pr1, FPOINT pr2, FPOINT pr3, FPOINT pr4)
{
  if (pr1.x < pc.x && pc.x < pr2.x && pr3.y < pc.y && pc.y < pr2.y) {
    FLINESEG2 line1, line2, line3, line4;
    line1.a = pr1, line1.b = pr2;
    line2.a = pr2, line2.b = pr3;
    line3.a = pr3, line3.b = pr4;
    line4.a = pr4, line4.b = pr1;
    if (r < ptoldist(pc, line1) && r < ptoldist(pc, line2) && r < ptoldist(pc, line3) && r < ptoldist(pc, line4)) {
      return TRUE;
    }
  }
  return FALSE;
}
// 点到平面的距离:
// 平面用一般式表示ax+by+cz+d=0
CC_INLINE double P2planeDist(double x, double y, double z, double a, double b, double c, double d)
{
  return (double)fabs(a * x + b * y + c * z + d) / SQRT(a * a + b * b + c * c);
}
//点是否在直线同侧:
// 两个点是否在直线同侧，是则返回TRUE
CC_INLINE BOOL SameSide(FPOINT p1, FPOINT p2, DFLINE2 line)
{
  return (line.a * p1.x + line.b * p1.y + line.c) * (line.a * p2.x + line.b * p2.y + line.c) > 0;
}
// 镜面反射线:
// 已知入射线、镜面，求反射线。
// a1,b1,c1为镜面直线方程(a1 x + b1 y + c1 = 0 ,下同)系数;
// a2,b2,c2为入射光直线方程系数;
// a,b,c为反射光直线方程系数.
// 光是有方向的，使用时注意: 入射光向量:<-b2,a2>；反射光向量:<b,-a>.
// 不要忘记结果中可能会有"negative zeros"
CC_INLINE void reflect(double a1, double b1, double c1, double a2, double b2, double c2, double* a, double* b, double* c)
{
  double n, m, tpb, tpa, xx, yy; //(xx,yy)是入射线与镜面的交点。
  tpb = b1 * b2 + a1 * a2;
  tpa = a2 * b1 - a1 * b2;
  m = (tpb * b1 + tpa * a1) / (b1 * b1 + a1 * a1);
  n = (tpa * b1 - tpb * a1) / (b1 * b1 + a1 * a1);
  if (fabs(a1 * b2 - a2 * b1) < 1e-20) {
    *a = a2;
    *b = b2;
    *c = c2;
    return ;
  }
  xx = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
  yy = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);
  *a = n;
  *b = - m;
  *c = m * yy - xx * n;
}
// 矩形包含
CC_INLINE BOOL rect2_in_rect1(double A, double B, double C, double D)
{
  // 矩形2（C，D）是否在1（A，B）内
  double X, Y, L, K, DMax, tmp;
  if (A < B) {
    tmp = A;
    A = B;
    B = tmp;
  }
  if (C < D) {
    tmp = C;
    C = D;
    D = tmp;
  }
  if (A > C && B > D) {
    // trivial case
    return TRUE;
  }
  else {
    if (D >= B) {
      return FALSE;
    }
    else {
      X = SQRT(A * A + B * B); // outer rectangle's diagonal
      Y = SQRT(C * C + D * D); // inner rectangle's diagonal
      if (Y < B) {
        // check for marginal conditions
        return TRUE;
      }
      else {
        // the inner rectangle can freely rotate inside
        if (Y > X) {
          return FALSE;
        }
        else {
          L = (B - SQRT(Y * Y - A * A)) / 2;
          K = (A - SQRT(Y * Y - B * B)) / 2;
          DMax = SQRT(L * L + K * K);
          return (D < DMax);
        }
      }
    }
  }
}
// 两圆交点:
// 两圆已经相交（相切）
CC_INLINE void circle2point(FPOINT p1, double r1, FPOINT p2, double r2, FPOINT* rp1, FPOINT* rp2)
{
  double a, b, r, delta;
  a = p2.x - p1.x;
  b = p2.y - p1.y;
  r = (a * a + b * b + r1 * r1 - r2 * r2) / 2;
  if (a == 0 && b != 0) {
    rp1->y = rp2->y = (double)(r / b);
    rp1->x = (double)(sqrt(r1 * r1 - rp1->y * rp1->y));
    rp2->x = - rp1->x;
  }
  else if (a != 0 && b == 0) {
    rp1->x = rp2->x = (double)(r / a);
    rp1->y = (double)(sqrt(r1 * r1 - rp1->x * rp2->x));
    rp2->y = - rp1->y;
  }
  else if (a != 0 && b != 0) {
    delta = b * b * r * r - (a * a + b * b) * (r * r - r1 * r1 * a * a);
    rp1->y = (double)((b * r + sqrt(delta)) / (a * a + b * b));
    rp2->y = (double)((b * r - sqrt(delta)) / (a * a + b * b));
    rp1->x = (double)((r - b * rp1->y) / a);
    rp2->x = (double)((r - b * rp2->y) / a);
  }
  rp1->x += p1.x;
  rp1->y += p1.y;
  rp2->x += p1.x;
  rp2->y += p1.y;
}
// 两圆公共面积:
// 必须保证相交
CC_INLINE double circle2area(FPOINT p1, double r1, FPOINT p2, double r2)
{
  double sita1, sita2, dx1, dy1, dx2, dy2, a, b, rr, s, tmp;
  FPOINT rp1, rp2, t;
  circle2point(p1, r1, p2, r2, &rp1, &rp2);
  if (r1 > r2) {
    // 保证r2>r1
    CC_SWAP(p1, p2, t);
    CC_SWAP(r1, r2, tmp);
  }
  a = p1.x - p2.x;
  b = p1.y - p2.y;
  rr = SQRT(a * a + b * b);
  dx1 = rp1.x - p1.x;
  dy1 = rp1.y - p1.y;
  dx2 = rp2.x - p1.x;
  dy2 = rp2.y - p1.y;
  sita1 = ACOS((dx1 * dx2 + dy1 * dy2) / r1 / r1);
  dx1 = rp1.x - p2.x;
  dy1 = rp1.y - p2.y;
  dx2 = rp2.x - p2.x;
  dy2 = rp2.y - p2.y;
  sita2 = ACOS((dx1 * dx2 + dy1 * dy2) / r2 / r2);
  s = 0;
  if (rr < r2) {
    //相交弧为优弧
    s = r1 * r1 * ((double)CC_PI - sita1 / 2.f + SIN(sita1) / 2.f) + r2 * r2 * (sita2 - SIN(sita2)) / 2.f;
  }
  else {
    //相交弧为劣弧
    s = (r1 * r1 * (sita1 - SIN(sita1)) + r2 * r2 * (sita2 - SIN(sita2))) / 2;
  }
  return s;
}
// 圆和直线关系:
// 0----相离 1----相切 2----相交
CC_INLINE int circle_line_point(FPOINT p, double r, double a, double b, double c, FPOINT* rp1, FPOINT* rp2)
{
  int res = 0;
  double tmp;
  c = c + a * p.x + b * p.y;
  if (a == 0 && b != 0) {
    tmp = - c / b;
    if (r * r < tmp * tmp) {
      res = 0;
    }
    else if (r * r == tmp * tmp) {
      res = 1;
      rp1->y = (double)(tmp);
      rp1->x = 0;
    }
    else {
      res = 2;
      rp1->y = rp2->y = (double)(tmp);
      rp1->x = (double)(sqrt(r * r - tmp * tmp));
      rp2->x = - rp1->x;
    }
  }
  else if (a != 0 && b == 0) {
    tmp = - c / a;
    if (r * r < tmp * tmp) {
      res = 0;
    }
    else if (r * r == tmp * tmp) {
      res = 1;
      rp1->x = (double)(tmp);
      rp1->y = 0;
    }
    else {
      res = 2;
      rp1->x = rp2->x = (double)(tmp);
      rp1->y = (double)(sqrt(r * r - tmp * tmp));
      rp2->y = - rp1->y;
    }
  }
  else if (a != 0 && b != 0) {
    double delta;
    delta = b * b * c * c - (a * a + b * b) * (c * c - a * a * r * r);
    if (delta < 0) {
      res = 0;
    }
    else if (delta == 0) {
      res = 1;
      rp1->y = (double)(- b * c / (a * a + b * b));
      rp1->x = (double)((- c - b * rp1->y) / a);
    }
    else {
      res = 2;
      rp1->y = (double)((- b * c + SQRT(delta)) / (a * a + b * b));
      rp2->y = (double)((- b * c - SQRT(delta)) / (a * a + b * b));
      rp1->x = (double)((- c - b * rp1->y) / a);
      rp2->x = (double)((- c - b * rp2->y) / a);
    }
  }
  rp1->x += p.x;
  rp1->y += p.y;
  rp2->x += p.x;
  rp2->y += p.y;
  return res;
}
// 内切圆:
CC_INLINE void incircle(FPOINT p1, FPOINT p2, FPOINT p3, FPOINT* rp, double* r)
{
  double dx31, dy31, dx21, dy21, d31, d21, a1, b1, c1;
  double dx32, dy32, dx12, dy12, d32, d12, a2, b2, c2;
  dx31 = p3.x - p1.x;
  dy31 = p3.y - p1.y;
  dx21 = p2.x - p1.x;
  dy21 = p2.y - p1.y;
  d31 = SQRT(dx31 * dx31 + dy31 * dy31);
  d21 = SQRT(dx21 * dx21 + dy21 * dy21);
  a1 = dx31 * d21 - dx21 * d31;
  b1 = dy31 * d21 - dy21 * d31;
  c1 = a1 * p1.x + b1 * p1.y;
  dx32 = p3.x - p2.x;
  dy32 = p3.y - p2.y;
  dx12 = - dx21;
  dy12 = - dy21;
  d32 = SQRT(dx32 * dx32 + dy32 * dy32);
  d12 = d21;
  a2 = dx12 * d32 - dx32 * d12;
  b2 = dy12 * d32 - dy32 * d12;
  c2 = a2 * p2.x + b2 * p2.y;
  rp->x = (double)((c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1));
  rp->y = (double)((c2 * a1 - c1 * a2) / (a1 * b2 - a2 * b1));
  *r = FABS(dy21 * rp->x - dx21 * rp->y + dx21 * p1.y - dy21 * p1.x) / d21;
}
// 求切点:
// p---圆心坐标， r---圆半径， sp---圆外一点， rp1,rp2---切点坐标
CC_INLINE void cut_point(FPOINT p, double r, FPOINT sp, FPOINT* rp1, FPOINT* rp2)
{
  double dx2, dy2, r2;
  FPOINT p2;
  p2.x = (p.x + sp.x) / 2;
  p2.y = (p.y + sp.y) / 2;
  dx2 = p2.x - p.x;
  dy2 = p2.y - p.y;
  r2 = SQRT(dx2 * dx2 + dy2 * dy2);
  circle2point(p, r, p2, r2, rp1, rp2);
}
// 线段的左右旋:
// l2在l1的左/右方向（l1为基准线）
// 返回 0: 重合；
// 返回 1: 右旋；
// 返回 -1: 左旋；
CC_INLINE int rotat(FLINESEG2 l1, FLINESEG2 l2)
{
  double dx1, dx2, dy1, dy2, d;
  dx1 = l1.a.x - l1.b.x;
  dy1 = l1.a.y - l1.b.y;
  dx2 = l2.a.x - l2.b.x;
  dy2 = l2.a.y - l2.b.y;
  d = dx1 * dy2 - dx2 * dy1;
  if (d == 0) {
    return 0;
  }
  else if (d > 0) {
    return - 1;
  }
  else {
    return 1;
  }
}
// 公式:
// 球坐标公式:
// 直角坐标为 P(x, y, z) 时，对应的球坐标是(rSINφcosθ, rSINφSINθ, rcosφ),
// 其中φ是向量OP与Z轴的夹角，范围[0，π]；是OP在XOY面上的投影到X轴的旋角，范围[0，2π]
//
// 直线的一般方程转化成向量方程:
// ax+by+c=0
// x-x0 y-y0
// ------ = -------
// m n
// (x0,y0)为直线上一点，m,n为向量
// 转换关系:
// a=n；b=-m；c=m·y0-n·x0；
// m=-b; n=a;
//
// 三点平面方程:
// 三点为P1，P2，P3
// 设向量 M1=P2-P1; M2=P3-P1;
// 平面法向量: M=M1 x M2()
// 平面方程: M.i(x-P1.x)+M.j(y-P1.y)+M.k(z-P1.z)=0
//求平面上两点之间的距离
CC_INLINE double dpt_distance(FPOINT p1, FPOINT p2)
{
  return ((double)sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
}
CC_INLINE double dpt_linecross(FPOINT p1, FPOINT p2)
{
  return ((double)sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
}
//判断点q是否在多边形Polygon内，
//其中多边形是任意的凸或凹多边形，
//Polygon中存放多边形的逆时针顶点序列
CC_INLINE int InsidePolygon(int vcount, FPOINT* Polygon, FPOINT q)
{
  int c = 0, i, n;
  FLINESEG2 l1, l2;
  l1.a = q;
  l1.b = q;
  l1.b.x = CC_INFINITY;
  n = vcount;
  for (i = 0; i < vcount; i++) {
    l2.a = Polygon[i];
    l2.b = Polygon[(i + 1) % n];
    if ((intersect_A(l1, l2)) ||
	((online(l1, Polygon[(i + 1) % n])) &&
	((!online(l1, Polygon[(i + 2) % n])) &&
	((multiply(Polygon[i], Polygon[(i + 1) % n], l1.a) * multiply(Polygon[(i + 1) % n], Polygon[(i + 2) % n], l1.a) > 0) ||
	((online(l1, Polygon[(i + 2) % n])) &&
	(multiply(Polygon[i], Polygon[(i + 2) % n], l1.a) *multiply(Polygon[(i + 2) % n], Polygon[(i + 3) % n], l1.a) > 0)))))) {
      c++;
    }
  }
  return (c % 2 != 0);
}
//寻找凸包 graham 扫描法
//PointSet为输入的点集；
//ch为输出的凸包上的点集，按照逆时针方向排列;
//n为PointSet中的点的数目
//len为输出的凸包上的点的个数；
// 设下一个扫描的点PointSet[i]=P2,当前栈顶的两个点ch[top]=P1,ch[top-1]=P0,
// 如果P1P2相对于P0P1在点P1向左旋转(共线也不行)，则P0,P1一定是凸包上的点；
// 否则P1一定不是凸包上的点，应该将其出栈。
// 比如下面左图中点1就一定是凸包上的点，右图中点1就一定不是凸包上的点，因为
// 可以连接点0,2构成凸包的边
// 2
// |
// | _____2
// 1 1
// / /
// ____0/ ____0/
CC_INLINE int hull_Graham_scan(FPOINT* PointSet, FPOINT* ch, int n)
{
  int i, j, k = 0, top = 2;
  FPOINT tmp;
  //选取PointSet中y坐标最小的点PointSet[k]，如果这样的点右多个，则取最左边的一个
  for (i = 1; i < n; i++) {
    if ((PointSet[i].y < PointSet[k].y) || ((PointSet[i].y == PointSet[k].y) && (PointSet[i].x < PointSet[k].x))) {
      k = i;
    }
  }
  tmp = PointSet[0];
  PointSet[0] = PointSet[k];
  PointSet[k] = tmp; //现在PointSet中y坐标最小的点在PointSet[0]
  for (i = 1; i < n - 1; i++) {
    //对顶点按照相对PointSet[0]的极角从小到大进行排序，极角相同的按照距离PointSet[0]从近到远进行排序
    k = i;
    for (j = i + 1; j < n; j++) {
      if ((multiply(PointSet[j], PointSet[k], PointSet[0]) > 0) || ((multiply(PointSet[j], PointSet[k], PointSet[0]) == 0) && (dpt_distance(PointSet[0], PointSet[j]) < dpt_distance(PointSet[0], PointSet[k])))) {
        k = j;
      }
    }
    tmp = PointSet[i];
    PointSet[i] = PointSet[k];
    PointSet[k] = tmp;
  }
  ch[0] = PointSet[0];
  ch[1] = PointSet[1];
  ch[2] = PointSet[2];
  for (i = 3; i < n; i++) {
    while (multiply(PointSet[i], ch[top], ch[top - 1]) >= 0) {
      top--;
    }
    ch[++top] = PointSet[i];
  }
  return top + 1;
}
CC_INLINE int test_geometry1()
{
  enum { MAX_points = 100 };
  int i, len;
  FPOINT pt1[MAX_points], pt2[MAX_points];
  //寻找凸包 graham 扫描法
  // Graham_scan
  {
    RandPoints(pt1, MAX_points, 500, 500);
    len = hull_Graham_scan(pt1, pt2, MAX_points);
    printf("%d\n", len);
    for (i = 0; i < len; i++) {
      printf("%d %d\n", (int)pt2[i].x, (int)pt2[i].y);
    }
  }
  // intersect
  {
    int probN = 0;
    FLINESEG2 l1 = {0} , l2 = {0};
    printf("Case %d:\n", probN);
    printf("Line ((%0.2f,%0.2f),(%0.2f,%0.2f)) and ((%0.2f,%0.2f),(%0.2f,%0.2f)) are %sintersect.\n", l1.a.x, l1.a.y, l1.b.x, l1.b.y, l2.a.x, l2.a.y, l2.b.x, l2.b.y, (intersect(l1, l2) ? "" : "not "));
  }
  // dpolygon_area
  {
    double arr;
    arr = dpolygon_area(MAX_points, pt1);
    printf("The area of polygon is %f\n\n", arr);
  }
  // InsidePolygon
  {
    FPOINT p = {
      110, 110
    };
    int tt = InsidePolygon(MAX_points, pt1, p);
    printf("The point is %sin the polygon!\n", (tt ? "" : "not "));
  }
  return 0;
}
CC_INLINE int poly_tt(FPOLYGON* s)
{
  int i, off = 0;
  FPOINT t;
  for (i = 0; i < s->n; ++i) {
    double area = poly_area(s->len[i], s->pt + off);
    if (area < 0) {
      ARRREV(s->len[i], s->pt + off, t);
    }
    off += s->len[i];
  }
  return 0;
}
CC_INLINE int pt_in_poly(double x, double y, const FPOLYGON* s)
{
  int i, off = 0;
  for (i = 0; i < s->n; ++i) {
    if (point_in_polygon(s->pt + off, s->len[i], x, y)) {
      return TRUE;
    }
    off += s->len[i];
  }
  return FALSE;
}
#ifdef _WINDOWS_
CC_INLINE int poly_setstr(FPOLYGON* s, str_t ss)
{
  int i, n, npt = 0;
  str_t s1, s2;
  const char* nextline = "\r\n";
  const char* spacechr = " \t";
  s1 = STRsplit(ss, "\n", nextline, &ss);
  s->n = n = str_toi(s1);
  REALLOC(int, s->len, n);
  for (i = 0; i < n; ++i) {
    s1 = STRsplit(ss, "\n", nextline, &ss);
    s->len[i] = str_toi(s1);
    npt += s->len[i];
  }
  REALLOC(FPOINT, s->pt, npt);
  for (i = 0; i < npt; ++i) {
    s1 = STRsplit(ss, "\n", nextline, &ss);
    s2 = STRsplit(s1, ",", spacechr, &s1);
    s->pt[i].x = str_tof(s2);
    s2 = STRsplit(s1, ",", spacechr, &s1);
    s->pt[i].y = str_tof(s2);
  }
  return 0;
}
#include "cfile.h"
CC_INLINE int poly_load(FPOLYGON* pc, const char* fn)
{
  str_t bf[1] = {0};
  str_load(fn, bf);
  if (bf->l > 0) {
    poly_setstr(pc, bf[0]);
  }
  str_free(bf);
  return 0;
}
CC_INLINE int poly_save(FPOLYGON* s, const char* fn)
{
  FILE* pf = fopen(fn, "wb");
  if (pf) {
    int i, npt = 0;
    fprintf(pf, "%d\n", s->n);
    for (i = 0; i < s->n; ++i) {
      fprintf(pf, "%d\n", s->len[i]);
      npt += s->len[i];
    }
    for (i = 0; i < npt; ++i) {
      fprintf(pf, "%.1f,%.1f\n", s->pt[i].x, s->pt[i].y);
    }
    fclose(pf);
  }
  return 0;
}
#endif
////////////////////////////////////////////////////////
//rect
#if 0
CENTER 按图片的原来size居中显示，当图片长 / 宽超过View的长 / 宽，则截取图片的居中部分显示
CENTER_CROP 按比例扩大图片的size居中显示，使得图片长(宽)等于或大于View的长(宽)
CENTER_INSIDE 将图片的内容完整居中显示，通过按比例缩小或原来的size使得图片长 / 宽等于或小于View的长 / 宽
FIT_CENTER 把图片按比例扩大 / 缩小到View的宽度，居中显示
FIT_START, FIT_END 在图片缩放效果上与 FIT_CENTER 一样，只是显示的位置不同，FIT_START是置于顶部，FIT_CENTER居中，FIT_END置于底部。
FIT_XY 不按比例缩放图片，目标是把图片塞满整个View。
#endif
CC_INLINE BOOL iPtInPolygon(const IPOINT* m_polygon, int m_num_points, int tx, int ty)
{
  int j, k;
  int yflag0, yflag1, inside_flag;
  int vtx0, vty0, vtx1, vty1;
  if (m_num_points < 3) {
    return FALSE;
  }
  vtx0 = m_polygon[m_num_points - 1].x;
  vty0 = m_polygon[m_num_points - 1].y;
  // get test bit for above/below X axis
  yflag0 = (vty0 >= ty);
  vtx1 = m_polygon[0].x;
  vty1 = m_polygon[0].y;
  inside_flag = 0;
  for (j = 1; j <= m_num_points; ++j) {
    yflag1 = (vty1 >= ty);
    if (yflag0 != yflag1) {
      if (((vty1 - ty) * (vtx0 - vtx1) >= (vtx1 - tx) * (vty0 - vty1)) == yflag1) {
        inside_flag ^= 1;
      }
    }
    // Move to the next pair of vertices, retaining info as possible.
    yflag0 = yflag1;
    vtx0 = vtx1;
    vty0 = vty1;
    k = (j >= m_num_points) ? j - m_num_points : j;
    vtx1 = m_polygon[k].x;
    vty1 = m_polygon[k].y;
  }
  return inside_flag != 0;
}
CC_INLINE int iPtInPolygon1(const IPOINT* ptPolygon, int nCount, int x, int y)
{
  int i, nCross = 0;
  for (i = 0; i < nCount; i++) {
    IPOINT p1 = ptPolygon[i];
    IPOINT p2 = ptPolygon[(i + 1) % nCount];
    // 求解 y=p.y 与 p1p2 的交点
    if (p1.y == p2.y) { // p1p2 与 y=p0.y平行
      continue;
    }
    if (y < MIN(p1.y, p2.y)) { // 交点在p1p2延长线上
      continue;
    }
    if (y >= MAX(p1.y, p2.y)) { // 交点在p1p2延长线上
      continue;
    }
    if ((y - p1.y) * (p2.x - p1.x) > (x - p1.x) * (p2.y - p1.y)) {
      nCross++; // 只统计单边交点
    }
  }
  // 单边交点为偶数，点在多边形之外 ---
  return (nCross % 2 == 1);
}
// 左闭右开
typedef FPOINT point2d;
CC_INLINE int dRectSet(DRECT* prc, double l, double t, double r, double b)
{
  prc->l = l, prc->t = t, prc->r = r, prc->b = b;
  return 0;
}
#define rectd_setrect(prc, pr) rectd_set(prc, (pr)->l, (pr)->t, (pr)->r, (pr)->b)
#define rectd_torect(prc) iRECT((int)(prc)->l, (int)(prc)->t, (int)(prc)->r, (int)(prc)->b)
CC_INLINE DRECT dRectZoom(const DRECT* b, const DRECT* c, const DRECT* d)
{
  return dRECT(b->l + (c->l - d->l) * RCW(b) / RCW(d),
      b->t + (c->t - d->t) * RCH(b) / RCH(d),
      b->l + (c->r - d->l) * RCW(b) / RCW(d),
      b->t + (c->b - d->t) * RCH(b) / RCH(d));
}
/****************************************************************************************\
*** vcgen
\****************************************************************************************/
// 二维图形的剪裁
// Cohen-Sutherland 剪裁算法
// 返回:
// 0: 线段完全在矩形区域外
// 1: 点x1, y1被裁剪点替换
// 2: 点x2, y2被裁剪点替换
// 3: 两点都被裁剪点替换
// 4: 线段完全在矩形区域内
CC_INLINE int TClipLine(const DRECT* edge, double* x1, double* y1, double* x2, double* y2)
{
  int result = 0;
  int c1, c2;
  if (edge->r < edge->l || edge->b < edge->t) {
    return (0); // 完全排除不用绘制
  }
  // c1 非0 表示在矩形框外
  // 0101 | 0100 | 0110
  // 0001 | 0000 | 0010
  // 1001 | 1000 | 1010
  c1 = (*x1 < edge->l) + (*x1 > edge->r) * 2 + (*y1 < edge->t) * 4 + (*y1 > edge->b) * 8;
  c2 = (*x2 < edge->l) + (*x2 > edge->r) * 2 + (*y2 < edge->t) * 4 + (*y2 > edge->b) * 8;
  if (0 == (c1 | c2)) {
    result = 4;
  }
  if ((c1 & c2) == 0 && (c1 | c2) != 0) {
    double a, dx = *x2 - *x1, dy = *y2 - *y1;
    if (c1 & 12) {
      a = c1 < 8 ? edge->t : edge->b;
      *x1 += (a - *y1) * dx / dy;
      *y1 = a;
      c1 = (*x1 < edge->l) + (*x1 > edge->r) * 2;
      result |= 1;
    }
    if (c2 & 12) {
      a = c2 < 8 ? edge->t : edge->b;
      *x2 += (a - *y2) * dx / dy;
      *y2 = a;
      c2 = (*x2 < edge->l) + (*x2 > edge->r) * 2;
      result |= 2;
    }
    if ((c1 & c2) == 0 && (c1 | c2) != 0) {
      if (c1) {
        a = c1 == 1 ? edge->l : edge->r;
        *y1 += (a - *x1) * dy / dx;
        *x1 = a;
        c1 = 0;
        result |= 1;
      }
      if (c2) {
        a = c2 == 1 ? edge->l : edge->r;
        *y2 += (a - *x2) * dy / dx;
        *x2 = a;
        c2 = 0;
        result |= 2;
      }
    }
  }
  return result;
}
// Sutherland - Hodgman剪裁算法
CC_INLINE int TClipPoly(const DRECT* edge, const FPOINT* pt, int n, FPOINT* pt0)
{
  double x1, y1, x2, y2;
  int i, ret, len = n;
  FPOINT* pt1 = pt0;
  int t = 0;
  x1 = pt[len - 1].x, y1 = pt[len - 1].y, x2 = pt[0].x, y2 = pt[0].y;
  //x1=1,y1=100,x2=2,y2=200;
  ret = TClipLine(edge, &x1, &y1, &x2, &y2);
  if (2 & ret) {
    //prex = x2, prey = y2; TRACE
  }
  for (i = 0; i < len; ++i) {
    int j = (i + 1) % len;
    x1 = pt[i].x, y1 = pt[i].y, x2 = pt[j].x, y2 = pt[j].y;
    ret = TClipLine(edge, &x1, &y1, &x2, &y2);
    if (4 == ret) {
      int asdf = 0;
    }
    if (3 == ret) {
      int asdf = 0;
    }
    if (ret) {
      if (4 == ret) {
        *pt1++ = fPOINT(x1, y1);
      }
      else {
        if (1 == ret) {
          *pt1++ = fPOINT(x1, y1);
          *pt1++ = fPOINT(x2, y2);
        }
        if (2 == ret) {
          *pt1++ = fPOINT(x1, y1);
          *pt1++ = fPOINT(x2, y2);
        }
        if (3 == ret) {
          *pt1++ = fPOINT(x1, y1);
          *pt1++ = fPOINT(x2, y2);
        }
      }
    }
  }
  return n;
}
#include "vcgen.inl"

// Spline Curve Horizontal and Vertical Extent (Span, Bounding Rectangle)
CC_INLINE double splineXMax(double Sx, double a, double Ex)
{
  double c = Sx - a, d = c + Ex - a, F, X, max1 = Sx ;
  if (Ex > Sx) {
    max1 = Ex;
  }
  if (d != 0.) {
    F = c / d;
    if (F > 0. && F < 1.) {
      X = Sx - c * F;
      if (X > max1) {
        max1 = X;
      }
    }
  }
  return max1;
}
CC_INLINE double splineXMin(double Sx, double a, double Ex)
{
  double c = Sx - a, d = c + Ex - a, F, X, min = Sx ;
  if (Ex < Sx) {
    min = Ex;
  }
  if (d != 0.) {
    F = c / d;
    if (F > 0. && F < 1.) {
      X = Sx - c * F;
      if (X < min) {
        min = X;
      }
    }
  }
  return min;
}
CC_INLINE double splineYMax(double Sy, double b, double Ey)
{
  double c = Sy - b, d = c + Ey - b, F, Y, max1 = Sy ;
  if (Ey > Sy) {
    max1 = Ey;
  }
  if (d != 0.) {
    F = c / d;
    if (F > 0. && F < 1.) {
      Y = Sy - c * F;
      if (Y > max1) {
        max1 = Y;
      }
    }
  }
  return max1;
}
CC_INLINE double splineYMin(double Sy, double b, double Ey)
{
  double c = Sy - b, d = c + Ey - b, F, Y, min = Sy ;
  if (Ey < Sy) {
    min = Ey;
  }
  if (d != 0.) {
    F = c / d;
    if (F > 0. && F < 1.) {
      Y = Sy - c * F;
      if (Y < min) {
        min = Y;
      }
    }
  }
  return min;
}
// public-domain code by Darel Rex Finley, January 2009
#define CIRCLE_RADIANS 6.283185307179586476925286766559
// Determines the radian angle of the specified point (as it relates to the origin).
//
// Warning: Do not pass zero in both parameters, as this will cause division-by-zero.
CC_INLINE double angleOf(double x, double y)
{
  double dist = sqrt(x * x + y * y) ;
  if (y >= 0.) {
    return acos(x / dist);
  }
  else {
    return acos(-x / dist) + .5 * CIRCLE_RADIANS;
  }
}
// Pass in a set of 2D points in x,y,points. Returns a polygon in polyX,polyY,polyCorners.
//
// To be safe, polyX and polyY should have enough space to store all the points passed in x,y,points.
CC_INLINE void findSmallestPolygon(double* x, double* y, long points, double* polyX, double* polyY, long* polyCorners)
{
  double newX = x[0], newY = y[0], xDif, yDif, oldAngle = .5 * CIRCLE_RADIANS, newAngle, angleDif, minAngleDif ;
  long i ;
  // Find a starting point.
  for (i = 0; i < points; i++) if (y[i] > newY || (y[i] == newY && x[i] < newX)) {
      newX = x[i];
      newY = y[i];
    }
  *polyCorners = 0;
  // Polygon-construction loop.
  while (!(*polyCorners) || newX != polyX[0] || newY != polyY[0]) {
    polyX[*polyCorners] = newX;
    polyY[*polyCorners] = newY;
    minAngleDif = CIRCLE_RADIANS;
    for (i = 0; i < points; i++) {
      xDif = x[i] - polyX[*polyCorners];
      yDif = y[i] - polyY[*polyCorners];
      if (xDif || yDif) {
        newAngle = angleOf(xDif, yDif);
        angleDif = oldAngle - newAngle;
        while (angleDif < 0.) {
          angleDif += CIRCLE_RADIANS;
        }
        while (angleDif >= CIRCLE_RADIANS) {
          angleDif -= CIRCLE_RADIANS;
        }
        if (angleDif < minAngleDif) {
          minAngleDif = angleDif;
          newX = x[i];
          newY = y[i];
        }
      }
    }
    (*polyCorners)++;
    oldAngle += .5 * CIRCLE_RADIANS - minAngleDif;
  }
}
// Point-In-Spline-Polygon Algorithm — Testing Whether A Point Is Inside A Complex Polygon With Spline Curves
// http://alienryderflex.com/polyspline/
//This five-cornered polygon makes a simple house shape:
//1,0, 1,-1, -1,-1, -1,0, 0,1, SP_END polygon
//This is the same house shape, but with the top corner turned into a spline point:
//1,0, 1,-1, -1,-1, -1,0, SP_SPLINE,0,1, SP_END spline polygon
//Same house shape, but with the two roof’s-eve corners turned into spline points:
//SP_SPLINE,1,0, 1,-1, -1,-1, SP_SPLINE,-1,0, 0,1, SP_END
//Note that this case would not work without the division-by-zero protection, because the eve points are vertically at the midpoint between the top and bottom of the house. spline polygon
//Same house shape, with the two bottom corner points turned into spline points:
//1,0, SP_SPLINE,1,-1, SP_SPLINE,-1,-1, -1,0, 0,1, SP_END spline polygon
//Same house shape, with all five corners turned into spline points:
//SP_SPLINE,1,0, SP_SPLINE,1,-1, SP_SPLINE,-1,-1, SP_SPLINE,-1,0, SP_SPLINE,0,1, SP_END spline polygon
//Here’s an example of using the “SP_NEW_LOOP” tag to put two polygons in one. In this case, the smaller polygon is a hole in the larger one, since it is inside it:
//SP_SPLINE,1,1, SP_SPLINE,1,-1, SP_SPLINE,-1,-1, SP_SPLINE,-1,1, SP_NEW_LOOP, SP_SPLINE,.5,.5, SP_SPLINE,.5,-.5, SP_SPLINE,-.5,-.5, SP_SPLINE,-.5,.5, SP_END spline polygon with hole
//This uses the “SP_NEW_LOOP” tag to make two loops, but they don’t overlap each other:
//-.25,0, .125,0, .125,-.875, .25,-.875, .25,-1, -.25,-1, -.25,-.875, -.125,-.875, -.125,-.125, -.25,-.125, SP_NEW_LOOP, SP_SPLINE,-.125,.375, SP_SPLINE,.125,.375, SP_SPLINE,.125,.25, SP_SPLINE,-.125,.25, SP_END spline lower-case letter i
//You can’t do a real circle with splines, but this eight-point polygon makes a sweet fake:
//SP_SPLINE,.4142,1, SP_SPLINE,1,.4142, SP_SPLINE,1,-.4142, SP_SPLINE,.4142,-1, SP_SPLINE,-.4142,-1, SP_SPLINE,-1,-.4142, SP_SPLINE,-1,.4142, SP_SPLINE,-.4142,1, SP_END
//FYI, .4142 is sqrt(2)-1.
#define NO 0
#define YES 1
#define SP_END (20000000.)
#define SP_SPLINE (SP_END+1.)
#define SP_NEW_LOOP (SP_END+2.)
CC_INLINE BOOL pointInSplinePoly(double* poly, double X, double Y)
{
  double Sx, Sy, Ex, Ey, a, b, sRoot, F, plusOrMinus, topPart, bottomPart, xPart ;
  int i = 0, j, k, start = 0 ;
  BOOL oddNodes = NO ;
  Y += .000001; // prevent the need for special tests when F is exactly 0 or 1
  while (poly[i] != SP_END) {
    j = i + 2;
    if (poly[i] == SP_SPLINE) {
      j++;
    }
    if (poly[j] == SP_END || poly[j] == SP_NEW_LOOP) {
      j = start;
    }
    if (poly[i] != SP_SPLINE && poly[j] != SP_SPLINE) { // STRAIGHT LINE
      if ((poly[i + 1] < Y && poly[j + 1] >= Y) || (poly[j + 1] < Y && poly[i + 1] >= Y)) {
        if (poly[i] + (Y - poly[i + 1]) / (poly[j + 1] - poly[i + 1]) * (poly[j] - poly[i]) < X) {
          oddNodes = !oddNodes;
        }
      }
    }
    else if (poly[j] == SP_SPLINE) { // SP_SPLINE CURVE
      a = poly[j + 1];
      b = poly[j + 2];
      k = j + 3;
      if (poly[k] == SP_END || poly[k] == SP_NEW_LOOP) {
        k = start;
      }
      if (poly[i] != SP_SPLINE) {
        Sx = poly[i];
        Sy = poly[i + 1];
      }
      else { // interpolate hard corner
        Sx = (poly[i + 1] + poly[j + 1]) / 2.;
        Sy = (poly[i + 2] + poly[j + 2]) / 2.;
      }
      if (poly[k] != SP_SPLINE) {
        Ex = poly[k];
        Ey = poly[k + 1];
      }
      else { // interpolate hard corner
        Ex = (poly[j + 1] + poly[k + 1]) / 2.;
        Ey = (poly[j + 2] + poly[k + 2]) / 2.;
      }
      bottomPart = 2.*(Sy + Ey - b - b);
      if (bottomPart == 0.) { // prevent division-by-zero
        b += .0001;
        bottomPart = -.0004;
      }
      sRoot = 2.*(b - Sy);
      sRoot *= sRoot;
      sRoot -= 2.*bottomPart * (Sy - Y);
      if (sRoot >= 0.) {
        sRoot = sqrt(sRoot);
        topPart = 2.*(Sy - b);
        for (plusOrMinus = -1.; plusOrMinus < 1.1; plusOrMinus += 2.) {
          F = (topPart + plusOrMinus * sRoot) / bottomPart;
          if (F >= 0. && F <= 1.) {
            xPart = Sx + F * (a - Sx);
            if (xPart + F * (a + F * (Ex - a) - xPart) < X) {
              oddNodes = !oddNodes;
            }
          }
        }
      }
    }
    if (poly[i] == SP_SPLINE) {
      i++;
    }
    i += 2;
    if (poly[i] == SP_NEW_LOOP) {
      i++;
      start = i;
    }
  }
  return oddNodes;
}
// public domain function by Darel Rex Finley, 2009
CC_INLINE double splinePolyArea(double* poly)
{
  double area = 0., a, b, Sx, Sy, Ex, Ey ;
  int i = 0, j, k ;
  while (poly[i] != SP_END) {
    j = i + 2;
    if (poly[i] == SP_SPLINE) {
      j++;
    }
    if (poly[j] == SP_END) {
      j = 0;
    }
    if (poly[i] != SP_SPLINE && poly[j] != SP_SPLINE) { // STRAIGHT LINE
      area += (poly[i] + poly[j]) * (poly[i + 1] - poly[j - 1]);
    }
    else if (poly[j] == SP_SPLINE) { // SP_SPLINE CURVE
      a = poly[j + 1];
      b = poly[j + 2];
      k = j + 3;
      if (poly[k] == SP_END) {
        k = 0;
      }
      if (poly[i] != SP_SPLINE) {
        Sx = poly[i];
        Sy = poly[i + 1];
      }
      else { // interpolate hard corner
        Sx = (poly[i + 1] + poly[j + 1]) / 2.;
        Sy = (poly[i + 2] + poly[j + 2]) / 2.;
      }
      if (poly[k] != SP_SPLINE) {
        Ex = poly[k];
        Ey = poly[k + 1];
      }
      else { // interpolate hard corner
        Ex = (poly[j + 1] + poly[k + 1]) / 2.;
        Ey = (poly[j + 2] + poly[k + 2]) / 2.;
      }
      area += (Sx + Ex) * (Sy - Ey);
      area += ((Sx + a) * (Sy - b) + (a + Ex) * (b - Ey) + (Ex + Sx) * (Ey - Sy)) * (2. / 3.);
    }
    if (poly[i] == SP_SPLINE) {
      i++;
    }
    i += 2;
  }
  return area * .5;
}
typedef struct polySet {
  struct {
    int corners;
    double* y;
    double* x;
  }* poly;
  int count;
} polySet;
typedef struct sp_point {
  double x, y;
  int prev;
  double totalDist;
} sp_point;
// Public-domain code by Darel Rex Finley, 2006.
// (This function automatically knows that enclosed polygons are "no-go" areas.)
CC_INLINE BOOL pointInPolygonSet(double testX, double testY, polySet allPolys)
{
  BOOL oddNodes = NO;
  int polyI, i, j ;
  for (polyI = 0; polyI < allPolys.count; polyI++) {
    for (i = 0; i < allPolys.poly[polyI].corners; i++) {
      j = i + 1;
      if (j == allPolys.poly[polyI].corners) {
        j = 0;
      }
      if ((allPolys.poly[polyI].y[i] < testY && allPolys.poly[polyI].y[j] >= testY)
          || (allPolys.poly[polyI].y[j] < testY && allPolys.poly[polyI].y[i] >= testY)) {
        if (allPolys.poly[polyI].x[i] + (testY - allPolys.poly[polyI].y[i])
            / (allPolys.poly[polyI].y[j] - allPolys.poly[polyI].y[i])
            * (allPolys.poly[polyI].x[j] - allPolys.poly[polyI].x[i]) < testX) {
          oddNodes = !oddNodes;
        }
      }
    }
  }
  return oddNodes;
}
// Shortest Path Through A Concave Polygon With Holes
// This function should be called with the full set of *all* relevant polygons.
// (The algorithm automatically knows that enclosed polygons are “no-go” areas.)
//
// Note: As much as possible, this algorithm tries to return YES when the
// test line-segment is exactly on the border of the polygon, particularly
// if the test line-segment *is* a side of a polygon.
CC_INLINE BOOL lineInPolygonSet(double testSX, double testSY, double testEX, double testEY, polySet allPolys)
{
  double theCos, theSin, dist, sX, sY, eX, eY, rotSX, rotSY, rotEX, rotEY, crossX ;
  int i, j, polyI ;
  testEX -= testSX;
  testEY -= testSY;
  dist = sqrt(testEX * testEX + testEY * testEY);
  theCos = testEX / dist;
  theSin = testEY / dist;
  for (polyI = 0; polyI < allPolys.count; polyI++) {
    for (i = 0; i < allPolys.poly[polyI].corners; i++) {
      j = i + 1;
      if (j == allPolys.poly[polyI].corners) {
        j = 0;
      }
      sX = allPolys.poly[polyI].x[i] - testSX;
      sY = allPolys.poly[polyI].y[i] - testSY;
      eX = allPolys.poly[polyI].x[j] - testSX;
      eY = allPolys.poly[polyI].y[j] - testSY;
      if ((sX == 0. && sY == 0. && eX == testEX && eY == testEY)
          || (eX == 0. && eY == 0. && sX == testEX && sY == testEY)) {
        return YES;
      }
      rotSX = sX * theCos + sY * theSin;
      rotSY = sY * theCos - sX * theSin;
      rotEX = eX * theCos + eY * theSin;
      rotEY = eY * theCos - eX * theSin;
      if ((rotSY < 0. && rotEY > 0.) || (rotEY < 0. && rotSY > 0.)) {
        crossX = rotSX + (rotEX - rotSX) * (0. - rotSY) / (rotEY - rotSY);
        if (crossX >= 0. && crossX <= dist) {
          return NO;
        }
      }
      if (rotSY == 0. && rotEY == 0.
          && (rotSX >= 0. || rotEX >= 0.)
          && (rotSX <= dist || rotEX <= dist)
          && (rotSX < 0. || rotEX < 0.
              || rotSX > dist || rotEX > dist)) {
        return NO;
      }
    }
  }
  return pointInPolygonSet(testSX + testEX / 2., testSY + testEY / 2., allPolys);
}
CC_INLINE double calcDist(double sX, double sY, double eX, double eY)
{
  eX -= sX, eY -= sY;
  return sqrt(eX * eX + eY * eY);
}
CC_INLINE void swapPoints(sp_point* a, sp_point* b)
{
  sp_point swap = *a;
  *a = *b;
  *b = swap;
}
// Finds the shortest path from sX,sY to eX,eY that stays within the polygon set.
//
// Note: To be safe, the solutionX and solutionY arrays should be large enough
// to accommodate all the corners of your polygon set (although it is
// unlikely that anywhere near that many elements will ever be needed).
//
// Returns YES if the optimal solution was found, or NO if there is no solution.
// If a solution was found, solutionX and solutionY will contain the coordinates
// of the intermediate nodes of the path, in order. (The startpoint and endpoint
// are assumed, and will not be included in the solution.)
CC_INLINE BOOL shortestPath(int pointcount, double sX, double sY, double eX, double eY, polySet allPolys,
    double* solutionX, double* solutionY, int* solutionNodes)
{
  //#define CC_INF 9999999. // (larger than total solution dist could ever be)
  sp_point pointList[1000] ; // (enough for all polycorners plus two)
  int pointCount ;
  int treeCount, polyI, i, j, bestI, bestJ ;
  double bestDist, newDist ;
  // Fail if either the startpoint or endpoint is outside the polygon set.
  if (!pointInPolygonSet(sX, sY, allPolys)
      || !pointInPolygonSet(eX, eY, allPolys)) {
    return NO;
  }
  // If there is a straight-line solution, return with it immediately.
  if (lineInPolygonSet(sX, sY, eX, eY, allPolys)) {
    (*solutionNodes) = 0;
    return YES;
  }
  // Build a point list that refers to the corners of the
  // polygons, as well as to the startpoint and endpoint.
  pointList[0].x = sX;
  pointList[0].y = sY;
  pointCount = 1;
  for (polyI = 0; polyI < allPolys.count; polyI++) {
    for (i = 0; i < allPolys.poly[polyI].corners; i++) {
      pointList[pointCount].x = allPolys.poly[polyI].x[i];
      pointList[pointCount].y = allPolys.poly[polyI].y[i];
      pointCount++;
    }
  }
  pointList[pointCount].x = eX;
  pointList[pointCount].y = eY;
  pointCount++;
  // Initialize the shortest-path tree to include just the startpoint.
  treeCount = 1;
  pointList[0].totalDist = 0.;
  // Iteratively grow the shortest-path tree until it reaches the endpoint
  // -- or until it becomes unable to grow, in which case exit with failure.
  bestJ = 0;
  while (bestJ < pointcount - 1) {
    bestDist = CC_INF;
    for (i = 0; i < treeCount; i++) {
      for (j = treeCount; j < pointCount; j++) {
        if (lineInPolygonSet(
            pointList[i].x, pointList[i].y,
            pointList[j].x, pointList[j].y, allPolys)) {
          newDist = pointList[i].totalDist + calcDist(
              pointList[i].x, pointList[i].y,
              pointList[j].x, pointList[j].y);
          if (newDist < bestDist) {
            bestDist = newDist;
            bestI = i;
            bestJ = j;
          }
        }
      }
    }
    if (bestDist == CC_INF) {
      return NO; // (no solution)
    }
    pointList[bestJ].prev = bestI ;
    pointList[bestJ].totalDist = bestDist;
    swapPoints(&pointList[bestJ], &pointList[treeCount]);
    treeCount++;
  }
  // Load the solution arrays.
  (*solutionNodes) = -1;
  i = treeCount - 1;
  while (i > 0) {
    i = pointList[i].prev;
    (*solutionNodes)++;
  }
  j = (*solutionNodes) - 1;
  i = treeCount - 1;
  while (j >= 0) {
    i = pointList[i].prev;
    solutionX[j] = pointList[i].x;
    solutionY[j] = pointList[i].y;
    j--;
  }
  // Success.
  return YES;
}
// public-domain code by Darel Rex Finley,
// 2010. See diagrams at http://
// alienryderflex.com/point_left_of_ray
CC_INLINE BOOL isPointLeftOfRay(double x, double y, double raySx, double raySy, double rayEx, double rayEy)
{
  double theCos, theSin, dist ;
  // Translate the system so that the ray
  // starts on the origin.
  rayEx -= raySx;
  rayEy -= raySy;
  x -= raySx;
  y -= raySy;
  // Discover the ray’s length.
  dist = sqrt(rayEx * rayEx + rayEy * rayEy);
  // Rotate the system so that the ray
  // points along the positive X-axis.
  theCos = rayEx / dist;
  theSin = rayEy / dist;
  y = y * theCos - x * theSin;
  // Return the result.
  return y > 0.;
}
//Now here’s the same function, but stripped of unneeded calculations:
// public-domain code by Darel Rex Finley,
// 2010. See diagrams at http://alienryderflex.com/point_left_of_ray
CC_INLINE BOOL isPointLeftOfRay1(double x, double y, double raySx, double raySy, double rayEx, double rayEy)
{
  return (y - raySy) * (rayEx - raySx) > (x - raySx) * (rayEy - raySy);
}
CC_INLINE double polygonArea(double* X, double* Y, int points)
{
  double area = 0. ;
  int i, j = points - 1 ;
  for (i = 0; i < points; i++) {
    area += (X[j] + X[i]) * (Y[j] - Y[i]);
    j = i;
  }
  return area * .5;
}
// public-domain code by Darel Rex Finley, 2009
// See diagrams at http://alienryderflex.com/polygon_hatchline_fill
// This function returns NO if MAX_NODES is exceeded, in which case some
// of the stripes may have been drawn, but not all of them.
CC_INLINE BOOL drawDiagonalStripes(long polygonCount, long* polygonCorners, double** polygons, double spacing)
{
#define MAX_NODES 1000
#define FAR_FAR_AWAY 999999999.
  double spanMin = FAR_FAR_AWAY, theCos, theSin, nodeX[MAX_NODES] ;
  double spanMax = -FAR_FAR_AWAY, x, y, a, b, newX, stripeY, swap ;
  long i, j, k, spanStart, spanEnd, nodeCount, step ;
  // Create a 45-degree angle for stripes.
  theCos = sqrt(.5);
  theSin = sqrt(.5);
  // Loop to determine the span over which diagonal lines must be drawn.
  for (i = 0; i < polygonCount ; i++) {
    for (j = 0; j < polygonCorners[i]; j++) {
      x = polygons[i][j * 2 ];
      y = polygons[i][j * 2 + 1];
      // Rotate the point, since the stripes may be at an angle.
      y = y * theCos - x * theSin;
      // Adjust the span.
      if (spanMin > y) {
        spanMin = y;
      }
      if (spanMax < y) {
        spanMax = y;
      }
    }
  }
  // Turn the span into a discrete step range.
  spanStart = (long) floor(spanMin / spacing) - 1;
  spanEnd = (long) floor(spanMax / spacing) + 1;
  // Loop to create all stripes.
  for (step = spanStart; step <= spanEnd; step++) {
    nodeCount = 0;
    stripeY = spacing * (double) step;
    // Loop to build a node list for one row of stripes.
    for (i = 0; i < polygonCount ; i++) {
      k = polygonCorners[i] - 1;
      for (j = 0; j < polygonCorners[i]; j++) {
        a = polygons[i][k * 2 ];
        b = polygons[i][k * 2 + 1];
        x = polygons[i][j * 2 ];
        y = polygons[i][j * 2 + 1];
        // Rotate the points, since the stripes may be at an angle.
        newX = a * theCos + b * theSin;
        b = b * theCos - a * theSin;
        a = newX;
        newX = x * theCos + y * theSin;
        y = y * theCos - x * theSin;
        x = newX;
        // Find the node, if any.
        if ((b < stripeY && y >= stripeY)
            || (y < stripeY && b >= stripeY)) {
          if (nodeCount >= MAX_NODES) {
            return NO;
          }
          nodeX[nodeCount++] = a + (x - a) * (stripeY - b) / (y - b);
        }
        k = j;
      }
      // Sort the node list.
      i = 0;
      while (i < nodeCount - 1) {
        if (nodeX[i] <= nodeX[i + 1]) {
          i++;
        }
        else {
          swap = nodeX[i];
          nodeX[i] = nodeX[i + 1];
          nodeX[i + 1] = swap;
          if (i) {
            i--;
          }
        }
      }
      // Loop to draw one row of stripe segments.
      for (i = 0; i < nodeCount; i += 2) {
        // Rotate the points back to their original coordinate system.
        a = nodeX[i ] * theCos - stripeY * theSin;
        b = stripeY * theCos + nodeX[i ] * theSin;
        x = nodeX[i + 1] * theCos - stripeY * theSin;
        y = stripeY * theCos + nodeX[i + 1] * theSin;
#define drawLineSegmentABtoXY(a, b, x, y)
        // Draw a single stripe segment.
        drawLineSegmentABtoXY(a, b, x, y);
      }
    }
  }
  // Success.
  return YES;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// public-domain code by Darel Rex Finley, 2007
// See examples at http://alienryderflex.com/polygon_perimeter
#define CIRCLE_RADIANS 6.283185307179586476925286766559
#define MAX_SEGS 1000
// public domain function by Darel Rex Finley, 2006
// Determines the intersection point of the line defined by points A and B with the
// line defined by points C and D.
//
// Returns YES if the intersection point was found, and stores that point in X,Y.
// Returns NO if there is no determinable intersection point, in which case X,Y will
// be unmodified.
CC_INLINE BOOL lineIntersection(double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Dx, double Dy, double* X, double* Y)
{
  double distAB, theCos, theSin, newX, ABpos ;
  // Fail if either line is undefined.
  if ((Ax == Bx && Ay == By) || (Cx == Dx && Cy == Dy)) {
    return NO;
  }
  // (1) Translate the system so that point A is on the origin.
  Bx -= Ax;
  By -= Ay;
  Cx -= Ax;
  Cy -= Ay;
  Dx -= Ax;
  Dy -= Ay;
  // Discover the length of segment A-B.
  distAB = sqrt(Bx * Bx + By * By);
  // (2) Rotate the system so that point B is on the positive X axis.
  theCos = Bx / distAB;
  theSin = By / distAB;
  newX = Cx * theCos + Cy * theSin;
  Cy = Cy * theCos - Cx * theSin;
  Cx = newX;
  newX = Dx * theCos + Dy * theSin;
  Dy = Dy * theCos - Dx * theSin;
  Dx = newX;
  // Fail if the lines are parallel.
  if (Cy == Dy) {
    return NO;
  }
  // (3) Discover the position of the intersection point along line A-B.
  ABpos = Dx + (Cx - Dx) * Dy / (Dy - Cy);
  // (4) Apply the discovered position to line A-B in the original coordinate system.
  *X = Ax + ABpos * theCos;
  *Y = Ay + ABpos * theSin;
  // Success.
  return YES;
}
//If you need to find out only when (and where) the line segments intersect, you can modify the function as follows:
// public domain function by Darel Rex Finley, 2006
// Determines the intersection point of the line segment defined by points A and B
// with the line segment defined by points C and D.
//
// Returns YES if the intersection point was found, and stores that point in X,Y.
// Returns NO if there is no determinable intersection point, in which case X,Y will
// be unmodified.
CC_INLINE BOOL lineSegmentIntersection(double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Dx, double Dy, double* X, double* Y)
{
  double distAB, theCos, theSin, newX, ABpos ;
  // Fail if either line segment is zero-length.
  if ((Ax == Bx && Ay == By) || (Cx == Dx && Cy == Dy)) {
    return NO;
  }
  // Fail if the segments share an end-point.
  if ((Ax == Cx && Ay == Cy) || (Bx == Cx && By == Cy)
      || (Ax == Dx && Ay == Dy) || (Bx == Dx && By == Dy)) {
    return NO;
  }
  // (1) Translate the system so that point A is on the origin.
  Bx -= Ax;
  By -= Ay;
  Cx -= Ax;
  Cy -= Ay;
  Dx -= Ax;
  Dy -= Ay;
  // Discover the length of segment A-B.
  distAB = sqrt(Bx * Bx + By * By);
  // (2) Rotate the system so that point B is on the positive X axis.
  theCos = Bx / distAB;
  theSin = By / distAB;
  newX = Cx * theCos + Cy * theSin;
  Cy = Cy * theCos - Cx * theSin;
  Cx = newX;
  newX = Dx * theCos + Dy * theSin;
  Dy = Dy * theCos - Dx * theSin;
  Dx = newX;
  // Fail if segment C-D doesn't cross line A-B.
  if ((Cy < 0. && Dy < 0.) || (Cy >= 0. && Dy >= 0.)) {
    return NO;
  }
  // (3) Discover the position of the intersection point along line A-B.
  ABpos = Dx + (Cx - Dx) * Dy / (Dy - Cy);
  // Fail if segment C-D crosses line A-B outside of segment A-B.
  if (ABpos < 0. || ABpos > distAB) {
    return NO;
  }
  // (4) Apply the discovered position to line A-B in the original coordinate system.
  *X = Ax + ABpos * theCos;
  *Y = Ay + ABpos * theSin;
  // Success.
  return YES;
}
// Returns the perimeter polygon in newX and newY (which must have at least
// MAX_SEGS elements each to prevent the possibility of overrun). "corners" is
// used to pass in the size of the original polygon, and to return the size of
// the new, perimeter polygon.
//
// If for any reason the procedure fails, it will return NO in its BOOL return
// value, in which case the values in "newX", "newY", and "corners" should not
// be trusted.
CC_INLINE BOOL polygonPerimeter(double* x, double* y, int* corners, double* newX, double* newY)
{
  double segSx[MAX_SEGS], segSy[MAX_SEGS], segEx[MAX_SEGS], segEy[MAX_SEGS] ;
  double segAngle[MAX_SEGS], intersectX, intersectY ;
  double startX = x[0], startY = y[0], lastAngle = .5 * CIRCLE_RADIANS, turnAngle = 0 ;
  double a, b, c, d, e, f, angleDif, bestAngleDif ;
  int i, j = (*corners) - 1, segs = 0 ;
  if ((*corners) > MAX_SEGS) {
    return NO;
  }
  // 1,3. Reformulate the polygon as a set of line segments, and choose a
  // starting point that must be on the perimeter.
  for (i = 0; i < (*corners); i++) {
    if (x[i] != x[j] || y[i] != y[j]) {
      segSx[segs] = x[i];
      segSy[segs] = y[i];
      segEx[segs] = x[j];
      segEy[segs++] = y[j];
    }
    j = i;
    if (y[i] > startY || (y[i] == startY && x[i] < startX)) {
      startX = x[i];
      startY = y[i];
    }
  }
  if (segs == 0) {
    return NO;
  }
  // 2. Break the segments up at their intersection points.
  for (i = 0; i < segs - 1; i++) {
    for (j = i + 1; j < segs ; j++) {
      if (lineSegmentIntersection(
          segSx[i], segSy[i], segEx[i], segEy[i],
          segSx[j], segSy[j], segEx[j], segEy[j], &intersectX, &intersectY)) {
        if ((intersectX != segSx[i] || intersectY != segSy[i])
            && (intersectX != segEx[i] || intersectY != segEy[i])) {
          if (segs == MAX_SEGS) {
            return NO;
          }
          segSx[segs] = segSx[i] ;
          segSy[segs ] = segSy[i] ;
          segEx[segs] = intersectX;
          segEy[segs++] = intersectY;
          segSx[i ] = intersectX;
          segSy[i ] = intersectY;
        }
        if ((intersectX != segSx[j] || intersectY != segSy[j])
            && (intersectX != segEx[j] || intersectY != segEy[j])) {
          if (segs == MAX_SEGS) {
            return NO;
          }
          segSx[segs] = segSx[j] ;
          segSy[segs ] = segSy[j] ;
          segEx[segs] = intersectX;
          segEy[segs++] = intersectY;
          segSx[j ] = intersectX;
          segSy[j ] = intersectY;
        }
      }
    }
  }
  // Calculate the angle of each segment.
  for (i = 0; i < segs; i++) {
    segAngle[i] = angleOf(segEx[i] - segSx[i], segEy[i] - segSy[i]);
  }
  // 4. Build the perimeter polygon.
  c = startX;
  d = startY;
  a = c - 1.;
  b = d;
  newX[0] = c;
  newY[0] = d;
  *corners = 1;
  while (YES) {
    bestAngleDif = CIRCLE_RADIANS;
    for (i = 0; i < segs; i++) {
      if (segSx[i] == c && segSy[i] == d && (segEx[i] != a || segEy[i] != b)) {
        angleDif = lastAngle - segAngle[i];
        while (angleDif >= CIRCLE_RADIANS) {
          angleDif -= CIRCLE_RADIANS;
        }
        while (angleDif < 0) {
          angleDif += CIRCLE_RADIANS;
        }
        if (angleDif < bestAngleDif) {
          bestAngleDif = angleDif;
          e = segEx[i];
          f = segEy[i];
        }
      }
      if (segEx[i] == c && segEy[i] == d && (segSx[i] != a || segSy[i] != b)) {
        angleDif = lastAngle - segAngle[i] + .5 * CIRCLE_RADIANS;
        while (angleDif >= CIRCLE_RADIANS) {
          angleDif -= CIRCLE_RADIANS;
        }
        while (angleDif < 0) {
          angleDif += CIRCLE_RADIANS;
        }
        if (angleDif < bestAngleDif) {
          bestAngleDif = angleDif;
          e = segSx[i];
          f = segSy[i];
        }
      }
    }
    if ((*corners) > 1 && c == newX[0] && d == newY[0] && e == newX[1] && f == newY[1]) {
      (*corners)--;
      return YES;
    }
    if (bestAngleDif == CIRCLE_RADIANS || (*corners) == MAX_SEGS) {
      return NO;
    }
    newX[ *corners ] = e;
    lastAngle -= bestAngleDif + .5 * CIRCLE_RADIANS;
    newY[(*corners)++] = f;
    a = c, b = d, c = e, d = f;
  }
}
// Given the sequentially connected points (a,b), (c,d), and (e,f), this
// function returns, in (C,D), a bevel-inset replacement for point (c,d).
//
// Note: If vectors (a,b)->(c,d) and (c,d)->(e,f) are exactly 180° opposed,
// or if either segment is zero-length, this function will do
// nothing; i.e. point (C,D) will not be set.
CC_INLINE void insetCorner(
    double a, double b, // previous point
    double c, double d, // current point that needs to be inset
    double e, double f, // next point
    double* C, double* D, // storage location for new, inset point
    double insetDist) // amount of inset (perpendicular to each line segment)
{
  double c1 = c, d1 = d, c2 = c, d2 = d, dx1, dy1, dist1, dx2, dy2, dist2, insetX, insetY ;
  // Calculate length of line segments.
  dx1 = c - a, dy1 = d - b;
  dist1 = sqrt(dx1 * dx1 + dy1 * dy1);
  dx2 = e - c, dy2 = f - d;
  dist2 = sqrt(dx2 * dx2 + dy2 * dy2);
  // Exit if either segment is zero-length.
  if (dist1 == 0. || dist2 == 0.) {
    return;
  }
  // Inset each of the two line segments.
  insetX = dy1 / dist1 * insetDist;
  a += insetX;
  c1 += insetX;
  insetY = -dx1 / dist1 * insetDist;
  b += insetY;
  d1 += insetY;
  insetX = dy2 / dist2 * insetDist;
  e += insetX;
  c2 += insetX;
  insetY = -dx2 / dist2 * insetDist;
  f += insetY;
  d2 += insetY;
  // If inset segments connect perfectly, return the connection point.
  if (c1 == c2 && d1 == d2) {
    *C = c1, *D = d1;
    return;
  }
  // Return the intersection point of the two inset segments (if any).
  if (lineIntersection(a, b, c1, d1, c2, d2, e, f, &insetX, &insetY)) {
    *C = insetX, *D = insetY;
  }
}
// public-domain code by Darel Rex Finley, 2007
// See diagrams at http://alienryderflex.com/polygon_inset
CC_INLINE void insetPolygon(double* x, double* y, int corners, double insetDist)
{
  double startX = x[0], startY = y[0], a, b, c, d, e, f ;
  int i ;
  // Polygon must have at least three corners to be inset.
  if (corners < 3) {
    return;
  }
  // Inset the polygon.
  c = x[corners - 1], d = y[corners - 1];
  e = x[0], f = y[0];
  for (i = 0; i < corners - 1; i++) {
    a = c, b = d, c = e, d = f;
    e = x[i + 1], f = y[i + 1];
    insetCorner(a, b, c, d, e, f, &x[i], &y[i], insetDist);
  }
  insetCorner(c, d, e, f, startX, startY, &x[i], &y[i], insetDist);
}
#undef NO
#undef YES
#undef SP_END
#undef SP_SPLINE
#undef SP_NEW_LOOP
// http://baike.baidu.com/link?url=JJswACC_uCnTpRrM15IM4iEY-VuEw3IxvXoA1GxgnNBdfys2NFg9aa2IRb5bN6MvVIP-0mkTpCSx7Uxs7b7Z3a
// 点到直线距离 (x, y) (a*x+b*y+c==0)
CC_INLINE double dist_pt2line(double x, double y, double a, double b, double c)
{
  return fabs(a * x + b * y + c) / sqrt(a * a + b * b);
}
// 平行线之间的间距
CC_INLINE double dist_line2line(double a, double b, double c1, double c2)
{
  return fabs(c1 - c2) / sqrt(a * a + b * b);
}
// 直线的夹角
CC_INLINE double dist_line_angle(double a1, double b1, double a2, double b2)
{
  return fabs((a1 * b2 - a2 * b1) / (a1 * a2 + b1 * b2));
}

//////////////////////////////////////////////////////////////////////////////////
typedef struct {
  FPOINT center;
  double r;
} circle_t;
static int double_equals(double const a, double const b)
{
  static const double ZERO = 1e-9;
  return fabs(a - b) < ZERO;
}
static double distance_sqr(FPOINT const* a, FPOINT const* b)
{
  return (a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y);
}
static double distance(FPOINT const* a, FPOINT const* b)
{
  return sqrt(distance_sqr(a, b));
}

static int circle_insect(circle_t circles[], FPOINT* points)
{
  double d, a, b, c, p, q, r;
  double cos_value[2], sin_value[2];
  if (double_equals(circles[0].center.x, circles[1].center.x)
      && double_equals(circles[0].center.y, circles[1].center.y)
      && double_equals(circles[0].r, circles[1].r)) {
    return -1;
  }
  d = distance(&circles[0].center, &circles[1].center);
  if (d > circles[0].r + circles[1].r
      || d < fabs(circles[0].r - circles[1].r)) {
    return 0;
  }
  a = 2.0 * circles[0].r * (circles[0].center.x - circles[1].center.x);
  b = 2.0 * circles[0].r * (circles[0].center.y - circles[1].center.y);
  c = circles[1].r * circles[1].r - circles[0].r * circles[0].r
      - distance_sqr(&circles[0].center, &circles[1].center);
  p = a * a + b * b;
  q = -2.0 * a * c;
  if (double_equals(d, circles[0].r + circles[1].r)
      || double_equals(d, fabs(circles[0].r - circles[1].r))) {
    cos_value[0] = -q / p / 2.0;
    sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);
    points[0].x = circles[0].r * cos_value[0] + circles[0].center.x;
    points[0].y = circles[0].r * sin_value[0] + circles[0].center.y;
    if (!double_equals(distance_sqr(&points[0], &circles[1].center),
        circles[1].r * circles[1].r)) {
      points[0].y = circles[0].center.y - circles[0].r * sin_value[0];
    }
    return 1;
  }
  r = c * c - b * b;
  cos_value[0] = (sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
  cos_value[1] = (-sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
  sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);
  sin_value[1] = sqrt(1 - cos_value[1] * cos_value[1]);
  points[0].x = circles[0].r * cos_value[0] + circles[0].center.x;
  points[1].x = circles[0].r * cos_value[1] + circles[0].center.x;
  points[0].y = circles[0].r * sin_value[0] + circles[0].center.y;
  points[1].y = circles[0].r * sin_value[1] + circles[0].center.y;
  if (!double_equals(distance_sqr(&points[0], &circles[1].center), circles[1].r * circles[1].r)) {
    points[0].y = circles[0].center.y - circles[0].r * sin_value[0];
  }
  if (!double_equals(distance_sqr(&points[1], &circles[1].center),
      circles[1].r * circles[1].r)) {
    points[1].y = circles[0].center.y - circles[0].r * sin_value[1];
  }
  if (double_equals(points[0].y, points[1].y) && double_equals(points[0].x, points[1].x)) {
    if (points[0].y > 0) {
      points[1].y = -points[1].y;
    }
    else {
      points[0].y = -points[0].y;
    }
  }
  return 2;
}
#ifdef TEST
static int test_circle_insect()
{
  circle_t circles[2];
  FPOINT points[2];
  printf("请输入两圆x，y，半径(以逗号分开)：");
  while (EOF != scanf("%lf,%lf,%lf,%lf,%lf,%lf",
      &circles[0].center.x, &circles[0].center.y, &circles[0].r,
      &circles[1].center.x, &circles[1].center.y, &circles[1].r)) {
    switch (circle_insect(circles, points)) {
    case -1:
      printf("THE CIRCLES ARE THE SAME/n");
      break;
    case 0:
      printf("NO INTERSECTION/n");
      break;
    case 1:
      printf("(%.3lf %.3lf)\n", points[0].x, points[0].y);
      break;
    case 2:
      printf("(%.3lf %.3lf) (%.3lf %.3lf)\n",
          points[0].x, points[0].y,
          points[1].x, points[1].y);
    }
  }
  return 0;
}
#endif // TEST
#undef EQ
// 线段平移
static int fseg_offset(double x0, double y0, double x1, double y1, double width, FPOINT* pt) {
  double dx = x1 - x0, dy = y1 - y0;
  double len = sqrt(dx*dx + dy*dy);
  double cx = width*dy/len, cy = width*dx/len;
  pt[0].x = x0 + cx, pt[0].y = y0 - cy;
  pt[1].x = x1 + cx, pt[1].y = y1 - cy;
  return 0;
}
static FPOINT fseg_offset1(const FPOINT* p1, const FPOINT* p2, double width) {
  FPOINT pt[2] = {0};
  fseg_offset(p1->x, p1->y, p2->x, p2->y, width, pt);
  return *pt;
}
// 求两直线交点
// 直线 1 上的两点：(ax1, ay1), (ax2, ay2)
// 直线 2 上的两点：(bx1, by1), (bx2, by2)
// 返回值：true: 有交点，保存在 (ix, iy) 中
//    ：false: 两直线平行
static bool fseg_intersection(double ax1, double ay1, double ax2, double ay2, double bx1, double by1, double bx2, double by2, double *ix, double *iy)
{
  double d = (ay2-ay1) * (bx2-bx1) - (by2-by1) * (ax2-ax1);
  
  if (d==0) {
    return false; // 两直线平行就退出，避免除数为 0 的情况
  }
  
  *ix = ((ax2-ax1)*(bx2-bx1)*(by1-ay1)+(ay2-ay1)*(bx2-bx1)*ax1-(by2-by1)*(ax2-ax1)*bx1) / d;
  *iy = ((ay2-ay1)*(by2-by1)*(bx1-ax1)+(ax2-ax1)*(by2-by1)*ay1-(bx2-bx1)*(ay2-ay1)*by1) / -d;
  
  return true;
}
static int fseg_joint(const FPOINT *p1, const FPOINT *p2, const FPOINT *p3, double distance, FPOINT* pt) {
  if ((p2->x - p1->x) * (p3->y - p1->y) > (p2->y - p1->y) * (p3->x - p1->x))
  {
    float length_0 = sqrtf((p2->x-p1->x)*(p2->x-p1->x)+(p2->y-p1->y)*(p2->y-p1->y));
    float length_1 = sqrtf((p3->x-p2->x)*(p3->x-p2->x)+(p3->y-p2->y)*(p3->y-p2->y));
    float dx0 = distance * (p2->x - p1->x) / length_0;
    float dy0 = distance * (p2->y - p1->y) / length_0;
    float dx1 = distance * (p3->x - p2->x) / length_1;
    float dy1 = distance * (p3->y - p2->y) / length_1;
    float det = (dy0*dx1 - dx0*dy1);
    float dx = (dx0*dx1*(dx0-dx1) + dy0*dy0*dx1 - dy1*dy1*dx0)/det;
    float dy = (dy0*dy1*(dy0-dy1) + dx0*dx0*dy1 - dx1*dx1*dy0)/det;
    if (dx*dx + dy*dy < distance*distance)
    {
    }
    pt->x = p2->x + dx;
    pt->y = p2->y + dy;
  }
  return 0;
}

static void bezier3_point2(double x1, double y1, double x2, double y2,
                   double x3, double y3, double x4, double y4, double mu,
                   double* x, double* y)
{
  double mum1, mum13, mu3;
  
  mum1 = 1 - mu;
  mum13 = mum1 * mum1 * mum1;
  mu3 = mu * mu * mu;
  
  *x = mum13*x1 + 3*mu*mum1*mum1*x2 + 3*mu*mu*mum1*x3 + mu3*x4;
  *y = mum13*y1 + 3*mu*mum1*mum1*y2 + 3*mu*mu*mum1*y3 + mu3*y4;
}

static void bezier3_point1(const FPOINT *p0, const FPOINT *p1, const FPOINT *p2, const FPOINT *p3, double t, FPOINT* pt)
{
  double x, y;
  bezier3_point2(p0->x, p0->y, p1->x, p1->y, p2->x, p2->y, p3->x, p3->y, t, &x, &y);
  pt->x = x;
  pt->y = y;
}
// <summary>
// 获得二次贝塞尔曲线在某个时刻的位置
// B(t) = P0(1-t)^2+ 2P1t(1-t) + P2t^2
// </summary>
// <param name="p0">起始点</param>
// <param name="bs">二次贝塞尔曲线</param>
// <param name="t">时间t 0~1</param>
static FPOINT bezier2_point(const FPOINT* p0, const FPOINT* p1, const FPOINT* p2, double t) {
  FPOINT pr;
  double r = 1 - t;
  double rr = r*r, tr = t*r, tt = t*t;
  pr.x = p0->x * rr + 2 * p1->x * tr + p2->x * tt;
  pr.y = p0->y * rr + 2 * p1->y * tr + p2->y * tt;
  return pr;
}

// <summary>
// 获得二次贝塞尔曲线在某个时刻的梯度
// </summary>
// <param name ="p0">起始点</param>
// <param name="bs">二次贝塞尔曲线</param>
// <param name="t">时间t 0~1</param>
// <returns></returns>
// 变量x受自变量时间t 在x方向的变化率
static FPOINT bezier2_angle(const FPOINT* p0, const FPOINT* p1, const FPOINT* p2, double t) {
  FPOINT dr;
  double r = 1 - t;
  dr.x = p0->x * 2 * r * -1 + 2 * p1->x * (r + (-1) * t) + p2->x * 2 * t;
  dr.y = p0->y * 2 * r * -1 + 2 * p1->y * (r + (-1) * t) + p2->y * 2 * t;
  return dr;
}

// <summary>
// 获得三次贝塞尔曲线在某个时刻的位置
// b(t) = p0(1-t)^3 + 3p1t(1-t^2) + 3p2t^2(1-t) + p3t^3
// </summary>
// <param name="p0">起始点</param>
// <param name="bs">三次贝塞尔曲线</param>
// <param name="t">时间t 0~1</param>
static FPOINT bezier3_point(const FPOINT* p0, const FPOINT* p1, const FPOINT* p2, const FPOINT* p3, double t) {
  FPOINT pr;
  double r = 1 - t;
  double rr = r*r, tt = t*t;
  double rrr = r*rr, trr3 = t*rr*3, ttr3 = r*tt*3, ttt=t*tt;
  pr.x = p0->x * rrr + p1->x * trr3 + p2->x * ttr3 + p3->x * ttt;
  pr.y = p0->y * rrr + p1->y * trr3 + p2->y * ttr3 + p3->y * ttt;
  return pr;
}

// <summary>
// 获得三次贝塞尔曲线在某个时刻的切线方向
// </summary>
// <param name ="p0">起始点</param>
// <param name="bs">三次贝塞尔曲线</param>
// <param name="t">时间t 0~1</param>
// <returns></returns>
static FPOINT bezier3_angle(const FPOINT* p0, const FPOINT* p1, const FPOINT* p2, const FPOINT* p3, double t) {
  FPOINT dr;
  double r = 1 - t;
  double rr = r*r, tt = t*t, tr = t*r;
  double rrr = r*rr, trr3 = t*rr*3, ttr3 = r*tt*3, ttt=t*tt;
  // 变量x受自变量时间t 在x方向的变化率
  dr.x = p0->x * 3 * rr * (-1) + 3 * p1->x * (rr + 2 * tr * (-1)) + 3 * p2->x * (2 * tr + tt * (-1)) + p3->x * 3 * tt;
  dr.y = p0->y * 3 * rr * (-1) + 3 * p1->y * (rr + 2 * tr * (-1)) + 3 * p2->y * (2 * tr + tt * (-1)) + p3->y * 3 * tt;
  return dr;
}

// [x, y] = solve('c1=a1*x + b1*y', 'c2=a2*x + b2*y')
//x = (-c2*b1+b2*c1)/(-a2*b1+a1*b2)
//y = -(a2*c1-a1*c2)/(-a2*b1+a1*b2)
static int fsolve2(double a1, double b1, double c1, double a2, double b2, double c2, float* x, float* y) {
  double d = (-a2*b1+a1*b2);
  *x = (-c2*b1+b2*c1)/d;
  *y = -(a2*c1-a1*c2)/d;
  return d!=0;
}
// solve('P1=(1-t1)^3*A + 3*t1*((1-t1)^2)*x + 3*(t1^2)*(1-t1)*y + t^3*D', 'P2=(1-t2)^3*A + 3*t2*((1-t2)^2)*x + 3*(t2^2)*(1-t2)*y + t2^3*D')
// a1 = 3*t1*((1-t1)^2)
// b1 = 3*(t1^2)*(1-t1)
// c1 = P1 - (1-t1)^3*A - t^3*D;
  //FPOINT pt3[4];
static int bezier3_offset(const FPOINT *a, const FPOINT *b, const FPOINT *c, const FPOINT *d, double distance, FPOINT* pt) {
  double t1 = 1./3, t2 = 2./3;
  FPOINT p1, p2, a1, a2;
  FPOINT ca, cb, cc, cd;
  p1 = bezier3_point(a,b,c,d,t1);
  a1 = bezier3_angle(a,b,c,d,t1);
  p2 = bezier3_point(a,b,c,d,t2);
  a2 = bezier3_angle(a,b,c,d,t2);
  ca = fseg_offset1(a, b, distance);
  cd = fseg_offset1(d, c, -distance);
  cb = fseg_offset1(&p1, &a1, distance);
  cc = fseg_offset1(&p2, &a2, distance);
  //pt3[0] = ca, pt3[1] = cb, pt3[2] = cc, pt3[3] = cd;
  {
    double r1 = (1-t1), r2 = 1-t2;
    fsolve2(3*t1*r1*r1, 3*t1*t1*r1, cb.x - r1*r1*r1*ca.x - t1*t1*t1*cd.x, 3*t2*r2*r2, 3*t2*t2*r2, cc.x - r2*r2*r2*ca.x - t2*t2*t2*cd.x, &pt[0].x, &pt[1].x);
    fsolve2(3*t1*r1*r1, 3*t1*t1*r1, cb.y - r1*r1*r1*ca.y - t1*t1*t1*cd.y, 3*t2*r2*r2, 3*t2*t2*r2, cc.y - r2*r2*r2*ca.y - t2*t2*t2*cd.y, &pt[0].y, &pt[1].y);
  }
  //fsolve2();
  return 0;
}
static int fpoly_offset(int n, const FPOINT *p1, bool closed, double distance, FPOINT* pt) {
  int i;
  if (n<=1) {
    return 0;
  }
  if (!closed) {
    fseg_offset(p1[0].x, p1[0].y, p1[1].x, p1[1].y, distance, pt);
    fseg_offset(p1[n-2].x, p1[n-2].y, p1[n-1].x, p1[n-1].y, distance, pt+n-2);
    for (i=1; i<n-1; ++i) {
      fseg_joint(p1+i-1, p1+i, p1+i+1, distance, pt+i);
    }
  } else {
    fseg_joint(p1+n-1, p1, p1+1, distance, pt);
    fseg_joint(p1+n-2, p1+n-1, p1, distance, pt+n-1);
    for (i=1; i<n-1; ++i) {
      fseg_joint(p1+i-1, p1+i, p1+i+1, distance, pt+i);
    }
  }
  return 0;
}

static int path_offset1(const FPOINT* Points, const BYTE* Types, int Count, bool closed, double distance, FPOINT* pt2) {
  int i, j, k, l, t;
  if (closed) {
    for (i=0; i<Count; ++i) {
      j = (i + Count - 1)%Count;
      k = (i + 1)%Count;
      l = (i + 2)%Count;
      t = Types[i]&PathPointTypePathTypeMask;
      switch (t) {
      case PathPointTypeStart:
      case PathPointTypeLine:
      case PathPointTypeBezier2:
        fseg_joint(Points+j, Points+i, Points+k, distance, pt2+i);
        break;
      case PathPointTypeBezier3:
        bezier3_offset(Points+j, Points+i, Points+k, Points+l, distance, pt2+i);
        ++i;
        break;
      }
    }
  } else {
    int n = Count;
    fseg_offset(Points[0].x, Points[0].y, Points[1].x, Points[1].y, distance, pt2);
    fseg_offset(Points[n-2].x, Points[n-2].y, Points[n-1].x, Points[n-1].y, distance, pt2+n-2);
    for (i=1; i<Count-1; ++i) {
      j = (i + Count - 1)%Count;
      k = (i + 1)%Count;
      l = (i + 2)%Count;
      t = Types[i]&PathPointTypePathTypeMask;
      switch (t) {
      case PathPointTypeStart:
      case PathPointTypeLine:
      case PathPointTypeBezier2:
        fseg_joint(Points+j, Points+i, Points+k, distance, pt2+i);
        break;
      case PathPointTypeBezier3:
        bezier3_offset(Points+j, Points+i, Points+k, Points+l, distance, pt2+i);
        ++i;
        break;
      }
    }
  }
  return 0;
}

//------------------------------------------------------------------------
static void trans_warp_magnifier(FPOINT* pt, int n, double m_xc, double m_yc, double m_radius, double m_magn, int inv)
{
  int i;
  if (inv) {
    // New version by Andrew Skalkin
    for (i=0; i<n; ++i) {
      double x = pt[i].x, y = pt[i].y;
      double dx = x - m_xc;
      double dy = y - m_yc;
      double r = sqrt(dx * dx + dy * dy);
      
      if(r < m_radius * m_magn) 
      {
        pt[i].x = m_xc + dx / m_magn;
        pt[i].y = m_yc + dy / m_magn;
      }
      else
      {
        double rnew = r - m_radius * (m_magn - 1.0);
        pt[i].x = m_xc + rnew * dx / r; 
        pt[i].y = m_yc + rnew * dy / r;
      }
    }
  } else {
    for (i=0; i<n; ++i) {
      double x = pt[i].x, y = pt[i].y;
      double dx = x - m_xc;
      double dy = y - m_yc;
      double r = sqrt(dx * dx + dy * dy);
      if(r < m_radius)
      {
        pt[i].x = m_xc + dx * m_magn;
        pt[i].y = m_yc + dy * m_magn;
      } else {
        double m = (r + m_radius * (m_magn - 1.0)) / r;
        pt[i].x = m_xc + dx * m;
        pt[i].y = m_yc + dy * m;
      }
    }
  }
}

#endif // _GEO_INL_

