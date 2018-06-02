//#define INF 1E200
//#define EP 1E-10
#define MAXV 300
#define PT_OUT_POLYGON 0
#define PT_IN_POLYGON 1
#define PT_IN_POINT 2
#define PT_IN_EDGE 3
//const double PI = 3.14159265;
typedef struct _LINESEG {
  DPOINT s, e;
} LINESEG;
// 直线的解析方程 a*x+b*y+c=0 为统一表示，约定 a >= 0
typedef struct _LINE {
  double a, b, c;
} LINE;
// 交点
typedef struct _IPT {
  DPOINT p;
  int status;
} IPT;
//计算差积
double Multiply(DPOINT sp, DPOINT ep, DPOINT op)
{
  return ((sp.x - op.x) * (ep.y - op.y) - (ep.x - op.x) * (sp.y - op.y));
}
double Max(double a, double b)
{
  if (a >= b) {
    return a;
  }
  else {
    return b;
  }
}
double Min(double a, double b)
{
  if (a <= b) {
    return a;
  }
  else {
    return b;
  }
}
//判断线段是否相交
BOOL IsCross(LINESEG u, LINESEG v)
{
  return ((Max(u.s.x, u.e.x) >= Min(v.s.x, v.e.x)) && (Max(v.s.x, v.e.x) >= Min(u.s.x, u.e.x)) && (Max(u.s.y, u.e.y) >= Min(v.s.y, v.e.y)) && (Max(v.s.y, v.e.y) >= Min(u.s.y, u.e.y)) && //排斥实验
      (Multiply(v.s, u.e, u.s) * Multiply(u.e, v.e, u.s) >= 0) && (Multiply(u.s, v.e, v.s) * Multiply(v.e, u.e, v.s) >= 0)); //跨立实验
}
//根据两点生成直线
LINE GenLine(DPOINT p1, DPOINT p2)
{
  LINE tl;
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
//计算直线L1、L2的交点
DPOINT CalIntersect(LINE l1, LINE l2) // 是 L1，L2
{
  DPOINT p;
  double d = l1.a * l2.b - l2.a * l1.b;
  if (fabs(d) < EP) {
    p.x = - 1;
    p.y = - 1;
  }
  else {
    double d = l1.a * l2.b - l2.a * l1.b;
    p.x = (l2.c * l1.b - l1.c * l2.b) / d;
    p.y = (l2.a * l1.c - l1.a * l2.c) / d;
  }
  return p;
}
//计算直线的斜率
double CalSlope(LINE l)
{
  if (fabs(l.a) < 1e-20) {
    return 0;
  }
  if (fabs(l.b) < 1e-20) {
    return INF;
  }
  return - (l.a / l.b);
}
//判断线段是否平行
BOOL IsParallel(LINESEG u, LINESEG v)
{
  LINE l1, l2;
  l1 = GenLine(u.s, u.e);
  l2 = GenLine(v.s, v.e);
  if (fabs(fabs(CalSlope(l1)) - fabs(CalSlope(l2))) < EP) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
//判断点是否在线段上
BOOL IsOnline(LINESEG l, DPOINT p)
{
  return ((Multiply(l.e, p, l.s) == 0) && (((p.x - l.s.x) * (p.x - l.e.x) <= 0) && ((p.y - l.s.y) * (p.y - l.e.y) <= 0)));
}
//计算角度
double CalAngle(DPOINT s, DPOINT e)
{
  double a;
  a = atan2(e.y - s.y, e.x - s.x);
  return a;
}
//判断点与多边形的关系
int ClipTest(DPOINT p, int vcount, DPOINT* pa)
{
  double sum_angle, a;
  DPOINT p1, p2;
  int i;
  sum_angle = 0;
  for (i = 0; i < vcount; i++) {
    p1.x = pa[(i + 1) % vcount].x;
    p1.y = pa[(i + 1) % vcount].y;
    p2.x = pa[i % vcount].x;
    p2.y = pa[i % vcount].y;
    if (((fabs(p.x - p1.x) < EP) && (fabs(p.y - p1.y) < EP)) || ((fabs(p.x - p2.x) < EP) && (fabs(p.y - p2.y) < EP))) {
      return 2; //点在多边形边的端点上
      break;
    }
    //计算P1P、P2P矢量的夹角
    a = CalAngle(p, p1) - CalAngle(p, p2);
    if (fabs(fabs(a) - PI) < 0.00001) {
      return PT_IN_EDGE; //点在多边形的边上（不包括端点）
      break;
    }
    if (a > PI) {
      a = a - 2 * PI;
    }
    if (a < - PI) {
      a = 2 * PI + a;
    }
    sum_angle = sum_angle + a;
  }
  if (fabs(sum_angle) < EP) {
    return PT_OUT_POLYGON; //点在多边形的外面
  }
  else if (fabs(fabs(sum_angle) - 2 * PI) < EP) {
    return PT_IN_POLYGON; //点在多边形的内部
  }
  else {
    return PT_IN_POINT; //点在多边形边的端点上
  }
}
//把新的点在x方向按升序或降序的方式排序后，插入交点数组
void SortX(int flag, int index, DPOINT p, IPT* ls)
{
  if (flag == 1) {
    while ((index > 0) && (ls[index - 1].p.x > p.x)) {
      ls[index] = ls[index - 1];
      index--;
    }
    ls[index].p.x = p.x;
    ls[index].p.y = p.y;
    ls[index].status = 3;
  }
  else {
    while ((index > 0) && (ls[index - 1].p.x < p.x)) {
      ls[index] = ls[index - 1];
      index--;
    }
    ls[index].p.x = p.x;
    ls[index].p.y = p.y;
    ls[index].status = 3;
  }
}
//把新的点在y方向按升序或降序的方式排序后，插入交点数组
int SortY(int flag, int index, DPOINT p, IPT* ls)
{
  if (flag == 1) {
    while ((index > 0) && (ls[index - 1].p.y < p.y)) {
      ls[index] = ls[index - 1];
      index--;
    }
    ls[index].p.x = p.x;
    ls[index].p.y = p.y;
    ls[index].status = 3;
  }
  else {
    while ((index > 0) && (ls[index - 1].p.y > p.y)) {
      ls[index] = ls[index - 1];
      index--;
    }
    ls[index].p.x = p.x;
    ls[index].p.y = p.y;
    ls[index].status = 3;
  }
  return 0;
}
int inCross(LINESEG l1, LINESEG l2, IPT ip1, IPT ip2, int k, IPT* intsp0)
{
  //如果相交
  if (IsCross(l1, l2)) {
    //如果平行,表明线段L1和线段L2重叠
    if (IsParallel(l1, l2)) {
      //线段L1的起点不在线段L2上，终点在线段L2上
      //或者L1的起点在线段L2上，终点不在在线段L2上
      //则将界于L1的起点和终点之间的L2的起点或终点
      //作为交点
      if (((!IsOnline(l2, l1.s)) && IsOnline(l2, l1.e)) || ((IsOnline(l2, l1.s)) && (!IsOnline(l2, l1.e)))) {
        //线段L1与y轴平行
        if (fabs(l1.s.x - l1.e.x) < EP) {
          //将交点按y的大小排序，插入交点数组中
          if ((l2.e.y > l1.s.y) && (l2.e.y < l1.e.y)) {
            SortY(1, k, l2.e, intsp0);
            k++;
          }
          else if ((l2.e.y < l1.s.y) && (l2.e.y > l1.e.y)) {
            SortY(- 1, k, l2.e, intsp0);
            k++;
          }
          else if ((l2.s.y > l1.s.y) && (l2.s.y < l1.e.y)) {
            SortY(1, k, l2.s, intsp0);
            k++;
          }
          else if ((l2.s.y < l1.s.y) && (l2.s.y > l1.e.y)) {
            SortY(- 1, k, l2.s, intsp0);
            k++;
          }
        } //如果与y轴不平行
        else {
          //将交点按x的大小排序，插入交点数组中
          if ((l2.e.x > l1.s.x) && (l2.e.x < l1.e.x)) {
            SortX(1, k, l2.e, intsp0);
            k++;
          }
          else if ((l2.e.x < l1.s.x) && (l2.e.x > l1.e.x)) {
            SortX(- 1, k, l2.e, intsp0);
            k++;
          }
          else if ((l2.s.x > l1.s.x) && (l2.s.x < l1.e.x)) {
            SortX(1, k, l2.s, intsp0);
            k++;
          }
          else if ((l2.s.x < l1.s.x) && (l2.s.x > l1.e.x)) {
            SortX(- 1, k, l2.s, intsp0);
            k++;
          }
        }
      }
      //线段L1的起点不在线段L2上，终点也不在线段L2上
      //则将L2的起点和终点作为交点
      else if ((!IsOnline(l2, l1.s)) && (!IsOnline(l2, l1.e))) {
        //如果线段L1与y轴平行
        if (fabs(l1.s.x - l1.e.x) < EP) {
          //将交点按y的大小排序，插入交点数组中
          if (l1.s.y < l1.e.y) {
            SortY(1, k, l2.e, intsp0);
            k++;
            SortY(1, k, l2.s, intsp0);
            k++;
          }
          else {
            SortY(- 1, k, l2.e, intsp0);
            k++;
            SortY(- 1, k, l2.s, intsp0);
            k++;
          }
        } //如果不平行
        else {
          //将交点按x的大小排序，插入交点数组中
          if (l1.s.x < l1.e.x) {
            SortX(1, k, l2.e, intsp0);
            k++;
            SortX(1, k, l2.s, intsp0);
            k++;
          }
          else {
            SortX(- 1, k, l2.e, intsp0);
            k++;
            SortX(- 1, k, l2.s, intsp0);
            k++;
          }
        }
      }
    }
    //如果线段L1和线段L2不平行
    else {
      //排除线段L1的端点和线段L2的端点部分或全部重合的情况
      if (ip1.status != 2 || ip2.status != 2) {
        //求交点
        DPOINT p = CalIntersect(GenLine(l1.s, l1.e), GenLine(l2.s, l2.e));
        if (fabs(l1.s.x - l1.e.x) < EP) {
          if ((p.y > l1.s.y) && (p.y < l1.e.y)) {
            SortY(1, k, p, intsp0);
            k++;
          }
          else if ((p.y < l1.s.y) && (p.y > l1.e.y)) {
            SortY(- 1, k, p, intsp0);
            k++;
          }
        }
        else {
          if ((p.x > l1.s.x) && (p.x < l1.e.x)) {
            SortX(1, k, p, intsp0);
            k++;
          }
          else if ((p.x < l1.s.x) && (p.x > l1.e.x)) {
            SortX(- 1, k, p, intsp0);
            k++;
          }
        }
      }
    }
  }
  return k;
}
int total_ls = 0; // 计算后的交点个数
int type = - 1; // 运算类型
LINESEG p_l[100];
IPT intsp[20];
#define SETLINESEG(L1, PA1, VCOUNT1, _I) (L1.s=PA1[_I], L1.e=PA1[(_I+1)%VCOUNT1])
//多边形A的每一条边分别与多边形B的每一条边求交(并、差)
int Intersect1(int vcount1, DPOINT* pa1, int vcount2, DPOINT* pa2)
{
  int i, j, k, n;
  LINESEG l1, l2;
  IPT ip1, ip2;
  DPOINT p;
  for (i = 0; i < vcount1; i++) {
    SETLINESEG(l1, pa1, vcount1, i);
    ip1.p = l1.s;
    ip1.status = ClipTest(l1.s, vcount2, pa2);
    intsp[0] = ip1;
    ip2.p = l1.e;
    ip2.status = ClipTest(l1.e, vcount2, pa2);
    intsp[1] = ip2;
    k = 2;
    for (j = 0; j < vcount2; j++) {
      SETLINESEG(l2, pa2, vcount2, j);
      k = inCross(l1, l2, ip1, ip2, k, intsp);
    }
    //对线段和多边形的交点进行处理，根据交点在对方多边形的位置，决定其保留或丢弃
    for (n = 0; n < k - 1; n++) {
      //如果起始点在多边形的外面,则保留
      if (intsp[n].status == 0) {
        p_l[total_ls].s = intsp[n].p;
        p_l[total_ls].e = intsp[n + 1].p;
        total_ls++;
      }
      //如果起始点和终点分别在多边形的边或端点上，则根据起始点和终点的中点在多边形的
      //外面或内部，决定保留(在外面)或丢弃(在内部)
      else if ((intsp[n].status == 2 || intsp[n].status == 3) && (intsp[n + 1].status == 2 || intsp[n + 1].status == 3)) {
        p.x = (intsp[n].p.x + intsp[n + 1].p.x) / 2;
        p.y = (intsp[n].p.y + intsp[n + 1].p.y) / 2;
        if (ClipTest(p, vcount2, pa2) == 0) {
          p_l[total_ls].s = intsp[n].p;
          p_l[total_ls].e = intsp[n + 1].p;
          total_ls++;
        }
      }
      //如果起始点在多边形的边或端点上，终点在多边形的外部,则保留
      else if ((intsp[n].status == 2 || intsp[n].status == 3) && (intsp[n + 1].status == 0)) {
        p_l[total_ls].s = intsp[n].p;
        p_l[total_ls].e = intsp[n + 1].p;
        total_ls++;
      } //其余情况都丢弃
    }
  }
  return total_ls;
}
//多边形A的每一条边分别与多边形B的每一条边求交(交)
int Intersect2(int vcount1, DPOINT* pa1, int vcount2, DPOINT* pa2)
{
  int i, j, k, n;
  LINESEG l1, l2;
  IPT ip1, ip2;
  DPOINT p;
  for (i = 0; i < vcount1; i++) {
    SETLINESEG(l1, pa1, vcount1, i);
    ip1.p = l1.s;
    ip1.status = ClipTest(l1.s, vcount2, pa2);
    intsp[0] = ip1;
    ip2.p = l1.e;
    ip2.status = ClipTest(l1.e, vcount2, pa2);
    intsp[1] = ip2;
    k = 2;
    for (j = 0; j < vcount2; j++) {
      SETLINESEG(l2, pa2, vcount2, j);
      k = inCross(l1, l2, ip1, ip2, k, intsp);
    }
    //对线段和多边形的交点进行处理，根据交点在对方多边形的位置，决定其保留或丢弃
    for (n = 0; n < k - 1; n++) {
      //如果起始点在多边形的边或端点上并且终点在多边形的内部,则保留
      if ((intsp[n].status == 2 || intsp[n].status == 3) && (intsp[n + 1].status == 1)) {
        p_l[total_ls].s = intsp[n].p;
        p_l[total_ls].e = intsp[n + 1].p;
        total_ls++;
      }
      //如果起始点在多边形的内部并且终点在多边形的边或端点上,则保留
      else if ((intsp[n].status == 1) && (intsp[n + 1].status == 2 || intsp[n + 1].status == 3)) {
        p_l[total_ls].s = intsp[n].p;
        p_l[total_ls].e = intsp[n + 1].p;
        total_ls++;
      }
      //如果起始点在多边形的内部并且终点在多边形的内部,则保留
      else if ((intsp[n].status == 1) && (intsp[n + 1].status == 1)) {
        p_l[total_ls].s = intsp[n].p;
        p_l[total_ls].e = intsp[n + 1].p;
        total_ls++;
      }
      //如果起始点和终点分别在多边形的边或端点上，则根据起始点和终点的中点在多边形的
      //外面或内部，决定保留(在外面)或丢弃(在内部)
      else if ((intsp[n].status == 2 || intsp[n].status == 3) && (intsp[n + 1].status == 2 || intsp[n + 1].status == 3)) {
        p.x = (intsp[n].p.x + intsp[n + 1].p.x) / 2;
        p.y = (intsp[n].p.y + intsp[n + 1].p.y) / 2;
        if (ClipTest(p, vcount2, pa2) == 1) {
          p_l[total_ls].s = intsp[n].p;
          p_l[total_ls].e = intsp[n + 1].p;
          total_ls++;
        }
      }
      //其余情况都丢弃
    }
  }
  return total_ls;
}
//多边形B的每一条边分别与多边形A的每一条边求交
int Intersect3(int vcount1, DPOINT* pa1, int vcount2, DPOINT* pa2)
{
  int i, j, k, n;
  LINESEG l1, l2;
  IPT ip1, ip2;
  DPOINT p;
  for (i = 0; i < vcount1; i++) {
    SETLINESEG(l1, pa1, vcount1, i);
    ip1.p = l1.s;
    ip1.status = ClipTest(l1.s, vcount2, pa2);
    intsp[0] = ip1;
    ip2.p = l1.e;
    ip2.status = ClipTest(l1.e, vcount2, pa2);
    intsp[1] = ip2;
    k = 2;
    for (j = 0; j < vcount2; j++) {
      SETLINESEG(l2, pa2, vcount2, j);
      k = inCross(l1, l2, ip1, ip2, k, intsp);
    }
    //对线段和多边形的交点进行处理，根据交点在对方多边形的位置，决定其保留或丢弃
    for (n = 0; n < k - 1; n++) {
      //如果起始点和终点分别在多边形的边或端点上，终点在多边形的内部
      if ((intsp[n].status == 2 || intsp[n].status == 3) && (intsp[n + 1].status == 1)) {
        p_l[total_ls].s = intsp[n].p;
        p_l[total_ls].e = intsp[n + 1].p;
        total_ls++;
      }
      //如果起始点和终点分别在多边形的边或端点上，则根据起始点和终点的中点在多边形的
      //外面或内部，决定保留(在外面)或丢弃(在内部)
      else if ((intsp[n].status == 2 || intsp[n].status == 3) && (intsp[n + 1].status == 2 || intsp[n + 1].status == 3)) {
        p.x = (intsp[n].p.x + intsp[n + 1].p.x) / 2;
        p.y = (intsp[n].p.y + intsp[n + 1].p.y) / 2;
        if (ClipTest(p, vcount2, pa2) == 1) {
          p_l[total_ls].s = intsp[n].p;
          p_l[total_ls].e = intsp[n + 1].p;
          total_ls++;
        }
      }
      //如果起始点在多边形的内部，终点在多边形的边或端点上或内部,则保留
      else if ((intsp[n].status == 1) && (intsp[n + 1].status == 1 || intsp[n + 1].status == 2 || intsp[n + 1].status == 3)) {
        p_l[total_ls].s = intsp[n].p;
        p_l[total_ls].e = intsp[n + 1].p;
        total_ls++;
      } //其余情况都丢弃
    }
  }
  return total_ls;
}
//将经过舍弃处理的线段加以显示
void DrawLine11(HDDC hDC)
{
  int i, j;
  int _x0, _y0;
  DWORD clr = _RGB(255, 0, 0);
  for (i = 0; i < total_ls; i++) {
    TMoveTo(hDC, (int)(p_l[i].s.x), (int)(p_l[i].s.y));
    TLineTo(hDC, (int)(p_l[i].e.x), (int)(p_l[i].e.y), clr);
  }
  for (j = 0; j < total_ls; j++) {
    p_l[j].s.x = 0, p_l[j].s.y = 0;
    p_l[j].e.x = 0, p_l[j].e.y = 0;
  }
  total_ls = 0; // 清除图形,很重要
}
void Shape(DPOINT* p1, DPOINT* p2, int dx, int dy)
{
  int xc = 400;
  int yc = 400;
  // 第一个多边形A数据
  p1[0] = dPOINT(xc - 14 + dx, yc - 273 - dy);
  p1[1] = dPOINT(xc - 99 + dx, yc - 185 - dy);
  p1[2] = dPOINT(xc - 42 + dx, yc - 104 - dy);
  p1[3] = dPOINT(xc + 51 + dx, yc - 125 - dy);
  p1[4] = dPOINT(xc + 54 + dx, yc - 210 - dy);
  p1[5] = dPOINT(xc - 11 + dx, yc - 176 - dy);
  // 第二个多边形B数据
  p2[0] = dPOINT(xc - 71 + dx, yc - 294 - dy);
  p2[1] = dPOINT(xc - 36 + dx, yc - 189 - dy);
  p2[2] = dPOINT(xc + 100 + dx, yc - 170 - dy);
  p2[3] = dPOINT(xc + 85 + dx, yc - 256 - dy);
  p2[4] = dPOINT(xc + 44 + dx, yc - 201 - dy);
}
#if 0
// 集合运算操作
int test_ClipPolygon1(HDDC hDC)
{
  int xc = 400;
  int yc = 400;
  DPOINT p1[6]; // 存放模拟多边形数据
  DPOINT p2[5]; // 存放模拟多边形数据
  int i;
  int _x0, _y0;
  int vcount1 = 6, vcount2 = 5; // 两个多边形顶点数
  DWORD newpenA = _RGB(255, 0, 0);
  DWORD newpenB = _RGB(0, 255, 0);
  DWORD crText = _RGB(255, 255, 255);
  //绘制原图
  Shape(p1, p2, 0, 0);
  for (i = 0; i < vcount1 - 1; i++) {
    TMoveTo(hDC, p1[i].x, p1[i].y);
    TLineTo(hDC, p1[i + 1].x, p1[i + 1].y, newpenA);
  }
  TMoveTo(hDC, p1[vcount1 - 1].x, p1[vcount1 - 1].y);
  TLineTo(hDC, p1[0].x, p1[0].y, newpenA);
  for (i = 0; i < vcount2 - 1; i++) {
    TMoveTo(hDC, p2[i].x, p2[i].y);
    TLineTo(hDC, p2[i + 1].x, p2[i + 1].y, newpenB);
  }
  TMoveTo(hDC, p2[vcount2 - 1].x, p2[vcount2 - 1].y);
  TLineTo(hDC, p2[0].x, p2[0].y, newpenB);
  TTextOut(hDC, xc - 120, yc - 60, "集合运算前:A(红色实线),B(绿色虚线)", crText);
  //分别求两个多边形的交点
  Shape(p1, p2, - 250, - 250);
  Intersect1(vcount1, p1, vcount2, p2);
  Intersect1(vcount2, p2, vcount1, p1);
  TTextOut(hDC, xc - 300, yc + 180, "并集: A∪B", crText);
  Shape(p1, p2, 0, - 250);
  Intersect2(vcount1, p1, vcount2, p2);
  Intersect2(vcount2, p2, vcount1, p1);
  TTextOut(hDC, xc - 50, yc + 180, "交集: A∩B", crText);
  Shape(p1, p2, 250, - 250);
  Intersect1(vcount1, p1, vcount2, p2);
  Intersect3(vcount2, p2, vcount1, p1);
  TTextOut(hDC, xc + 200, yc + 180, "差集: A－B", crText);
  //显示集合运算后新的多边形
  DrawLine11(hDC);
  total_ls = 0; // 清除图形,很重要
  return 0;
}
#endif
//将经过舍弃处理的线段加以显示
int DrawLines2(HDDC hDC, DPOINT* pt, int* next, int n, DWORD clr)
{
  int i, j;
  int _x0, _y0;
  for (i = 0; i < n; ++i) {
    j = next[i];
    TMoveTo(hDC, (int)(pt[i].x), (int)(pt[i].y));
    TLineTo(hDC, (int)(pt[j].x), (int)(pt[j].y), clr);
  }
  return 0;
}
int DrawLines3(HDDC hDC, DPOINT* pt, int* next, int first, DWORD clr)
{
  int i = first;
  int _x0, _y0;
  int j = 0;
  TMoveTo(hDC, (int)(pt[i].x), (int)(pt[i].y));
  for (; 1;) {
    i = next[i];
    TLineTo(hDC, (int)(pt[i].x), (int)(pt[i].y), clr);
    //printf("%d\n", i);
    if (i == first) {
      break;
    }
    //showDDC("ffff", hDC); cvWaitKey(-1);
    //++j;if (j>40)break;
  }
  return 0;
}
int pushPolygon(int n, DPOINT* pt, int* next, int p1n, const DPOINT* p1)
{
  int i;
  for (i = 0; i < p1n; ++i) {
    pt[n + i] = p1[i];
    next[n + i] = n + i + 1;
  }
  next[n + p1n - 1] = n;
  return n + p1n;
}
//计算差积
double Multiply2(const DPOINT* sp, const DPOINT* ep, const DPOINT* op)
{
  return ((sp->x - op->x) * (ep->y - op->y) - (ep->x - op->x) * (sp->y - op->y));
}
//根据两点生成直线
LINE* GenLine2(const DPOINT* p1, const DPOINT* p2, LINE* tl)
{
  tl->a = p2->y - p1->y;
  tl->b = (p1->x - p2->x);
  tl->c = (p1->y * p2->x - p1->x * p2->y);
  return tl;
}
//计算直线L1、L2的交点
int CalIntersect2(LINE* l1, LINE* l2, DPOINT* p) // 是 L1，L2
{
  double d = l1->a * l2->b - l2->a * l1->b;
  if (fabs(d) < EP) {
    return 0;
  }
  p->x = (l2->c * l1->b - l1->c * l2->b) / d;
  p->y = (l2->a * l1->c - l1->a * l2->c) / d;
  return 1;
}
//判断线段是否相交
int PtCross2(const DPOINT* s1, const DPOINT* e1, const DPOINT* s2, const DPOINT* e2, DPOINT* out)
{
  LINE l1, l2;
  int ret = ((Max(s1->x, e1->x) >= Min(s2->x, e2->x)) && (Max(s2->x, e2->x) >= Min(s1->x, e1->x)) && (Max(s1->y, e1->y) >= Min(s2->y, e2->y)) && (Max(s2->y, e2->y) >= Min(s1->y, e1->y)) && //排斥实验
      (Multiply2(s2, e1, s1) * Multiply2(e1, e2, s1) >= 0) && (Multiply2(s1, e2, s2) * Multiply2(e2, e1, s2) >= 0)); //跨立实验
  GenLine2(s1, e1, &l1);
  GenLine2(s2, e2, &l2);
  CalIntersect2(&l1, &l2, out);
  return ret;
}
int pushPolygonBool(int n, DPOINT* pt, int* next, int p1n, const DPOINT* p1, int next_offset)
{
  int i, j, k, n2 = n + p1n, n1 = n + p1n;
  for (i = 0; i < p1n; ++i) {
    int n3 = n2, j1, i1 = (i + next_offset + p1n) % p1n;
    pt[n + i] = p1[i];
    next[n + i] = n + i1;
    for (j = 0; j < n3; ++j) {
      j1 = next[j];
      if ((j >= n && j < n1) || (j1 >= n && j1 < n1)) {
        continue;
      }
      k = PtCross2(p1 + i, p1 + i1, pt + j, pt + j1, pt + n2);
      if (k) {
        next[n2] = next[j], next[j] = n2 + 1;
        next[n2 + 1] = next[n + i], next[n + i] = n2;
        pt[n2 + 1] = pt[n2];
        n2 += 2;
      }
    }
  }
  return n2;
}
int offsetPolygon(int n, DPOINT* pt, double x, double y)
{
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x += x, pt[i].y += y;
  }
  return n;
}
//PolygonBool(int n, DPOINT* pt)
// 集合运算操作
int test_ClipPolygon(HDDC hDC, int ch)
{
  DWORD clrs[] = {
    _RGB(255, 0, 0), _RGB(0, 255, 0)
  };
  DWORD crText = _RGB(255, 255, 255);
  DPOINT pt[30];
  int i, n = 0, next[30];
  // 存放模拟多边形数据
  DPOINT p1[] = {
    -14, -273, -99, -185, -42, -104, +51, -125, +54, -210, -11, -176
  };
  DPOINT p2[] = {
    -71, -294, -36, -189, +100, -170, +85, -256, +44, -201
  };
  {
    n = 0;
    n = pushPolygon(n, pt, next, countof(p1), p1);
    n = pushPolygon(n, pt, next, countof(p2), p2);
    offsetPolygon(n, pt, 400, 400);
    DrawLines2(hDC, pt, next, n, crText);
  }
  {
    static int tt = 0;
    if (' ' == ch) {
      ++tt;
    }
    n = 0;
    n = pushPolygon(n, pt, next, countof(p1), p1);
    n = pushPolygonBool(n, pt, next, countof(p2), p2, 1);
    offsetPolygon(n, pt, 400, 600);
    i = countof(p1) + countof(p2);
    tt %= (n - i);
    for (; i < n; i += 4) {
      DrawLines3(hDC, pt, next, i, clrs[0]);
      DrawLines3(hDC, pt, next, i + 1, clrs[1]);
    }
  }
  return 0;
}
#undef EP

