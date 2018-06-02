// 三次样条插值函数.C
// 进行自然边界，夹持边界，非扭结边界
// 条件下的插值
// 2008-12-27
#include <stdio.h>
#include <math.h>
typedef enum _condition {
  NATURAL,
  CLAMPED,
  NOTaKNOT
} condition;
typedef struct _coefficient {
  double a3;
  double b2;
  double c1;
  double d0;
} coefficient;
typedef struct _point {
  coefficient* coe;
  double* xCoordinate;
  double* yCoordinate;
  double f0;
  double fn;
  int num;
  condition con;
} point;
int spline(point* point)
{
  double* x = (*point).xCoordinate, *y = (*point).yCoordinate;
  int n = (*point).num - 1;
  coefficient* coe = (*point).coe;
  condition con = (*point).con;
  double* h, *d;
  double* a, *b, *c, *f, *m;
  double temp;
  int i;
  h = (double*)pmalloc(n * sizeof(double)); /* 0,1--(n-1),n */
  d = (double*)pmalloc(n * sizeof(double)); /* 0,1--(n-1),n */
  a = (double*)pmalloc((n + 1) * sizeof(double)); /* 特别使用,1--(n-1), n */
  b = (double*)pmalloc((n + 1) * sizeof(double)); /* 0,1--(n-1), n */
  c = (double*)pmalloc((n + 1) * sizeof(double)); /* 0,1--(n-1),特别使用 */
  f = (double*)pmalloc((n + 1) * sizeof(double)); /* 0,1--(n-1), n */
  m = b;
  if (f == NULL) {
    pfree(h);
    pfree(d);
    pfree(a);
    pfree(b);
    pfree(c);
    return 1;
  }
  /* 计算 h[] d[] */
  for (i = 0; i < n; i++) {
    h[i] = x[i + 1] - x[i];
    d[i] = (y[i + 1] - y[i]) / h[i];
    /* printf("%f\t%f\n", h[i], d[i]); */
  }
  // 初始化系数增广矩阵
  a[0] = (*point).f0;
  c[n] = (*point).fn;
  /* 计算 a[] b[] d[] f[] */
  switch (con) {
  case NATURAL:
    f[0] = a[0];
    f[n] = c[n];
    a[0] = 0;
    c[n] = 0;
    c[0] = 0;
    a[n] = 0;
    b[0] = 1;
    b[n] = 1;
    break;
  case CLAMPED:
    f[0] = 6 * (d[0] - a[0]);
    f[n] = 6 * (c[n] - d[n - 1]);
    a[0] = 0;
    c[n] = 0;
    c[0] = h[0];
    a[n] = h[n - 1];
    b[0] = 2 * h[0];
    b[n] = 2 * h[n - 1];
    break;
  case NOTaKNOT:
    f[0] = 0;
    f[n] = 0;
    a[0] = h[0];
    c[n] = h[n - 1];
    c[0] = -(h[0] + h[1]);
    a[n] = -(h[n - 2] + h[n - 1]);
    b[0] = h[1];
    b[n] = h[n - 2];
    break;
  }
  for (i = 1; i < n; i++) {
    a[i] = h[i - 1];
    b[i] = 2 * (h[i - 1] + h[i]);
    c[i] = h[i];
    f[i] = 6 * (d[i] - d[i - 1]);
  }
  /* for (i = 0; i < n+1; i++) printf("%f\n", c[i]); */
  // 高斯消元
  // 第0行到第(n-3)行的消元
  for (i = 0; i <= n - 3; i++) {
    // 选主元
    if (fabs(a[i + 1]) > fabs(b[i])) {
      temp = a[i + 1];
      a[i + 1] = b[i];
      b[i] = temp;
      temp = b[i + 1];
      b[i + 1] = c[i];
      c[i] = temp;
      temp = c[i + 1];
      c[i + 1] = a[i];
      a[i] = temp;
      temp = f[i + 1];
      f[i + 1] = f[i];
      f[i] = temp;
    }
    temp = a[i + 1] / b[i];
    a[i + 1] = 0;
    b[i + 1] = b[i + 1] - temp * c[i];
    c[i + 1] = c[i + 1] - temp * a[i];
    f[i + 1] = f[i + 1] - temp * f[i];
  }
  // 最后3行的消元
  // 选主元
  if (fabs(a[n - 1]) > fabs(b[n - 2])) {
    temp = a[n - 1];
    a[n - 1] = b[n - 2];
    b[n - 2] = temp;
    temp = b[n - 1];
    b[n - 1] = c[n - 2];
    c[n - 2] = temp;
    temp = c[n - 1];
    c[n - 1] = a[n - 2];
    a[n - 2] = temp;
    temp = f[n - 1];
    f[n - 1] = f[n - 2];
    f[n - 2] = temp;
  }
  /* 选主元 */
  if (fabs(c[n]) > fabs(b[n - 2])) {
    temp = c[n];
    c[n] = b[n - 2];
    b[n - 2] = temp;
    temp = a[n];
    a[n] = c[n - 2];
    c[n - 2] = temp;
    temp = b[n];
    b[n] = a[n - 2];
    a[n - 2] = temp;
    temp = f[n];
    f[n] = f[n - 2];
    f[n - 2] = temp;
  }
  /* 第(n-1)行消元 */
  temp = a[n - 1] / b[n - 2];
  a[n - 1] = 0;
  b[n - 1] = b[n - 1] - temp * c[n - 2];
  c[n - 1] = c[n - 1] - temp * a[n - 2];
  f[n - 1] = f[n - 1] - temp * f[n - 2];
  /* 第n行消元 */
  temp = c[n] / b[n - 2];
  c[n] = 0;
  a[n] = a[n] - temp * c[n - 2];
  b[n] = b[n] - temp * a[n - 2];
  f[n] = f[n] - temp * f[n - 2];
  /* 选主元 */
  if (fabs(a[n]) > fabs(b[n - 1])) {
    temp = a[n];
    a[n] = b[n - 1];
    b[n - 1] = temp;
    temp = b[n];
    b[n] = c[n - 1];
    c[n - 1] = temp;
    temp = f[n];
    f[n] = f[n - 1];
    f[n - 1] = temp;
  }
  /* 最后一次消元 */
  temp = a[n] / b[n - 1];
  a[n] = 0;
  b[n] = b[n] - temp * c[n - 1];
  f[n] = f[n] - temp * f[n - 1];
  /* 回代求解 m[] */
  m[n] = f[n] / b[n];
  m[n - 1] = (f[n - 1] - c[n - 1] * m[n]) / b[n - 1];
  for (i = n - 2; i >= 0; i--) {
    m[i] = (f[i] - (m[i + 2] * a[i] + m[i + 1] * c[i])) / b[i];
  }
  /* for (i = 0; i < n+1; i++) printf("%f\n", m[i]); */
  pfree(a);
  pfree(c);
  pfree(f);
  // 放置参数
  for (i = 0; i < n; i++) {
    coe[i].a3 = (m[i + 1] - m[i]) / (6 * h[i]);
    coe[i].b2 = m[i] / 2;
    coe[i].c1 = d[i] - (h[i] / 6) * (2 * m[i] + m[i + 1]);
    coe[i].d0 = y[i];
  }
  pfree(h);
  pfree(d);
  pfree(b);
  return n + 1;
}

