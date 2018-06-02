
// 最小二乘曲线拟合y=a+bx+cx^2
int my_two_polyfit(IPOINT* pt, int m, double* h)
{
  // x = a(1,:);
  // y = a(2,:);
  // m为数据个数
  int i, j, k, n;
  double d[10] = {0};
  double z[10] = {0};
  double a[5][5] = {0};

  for (i = 0; i < 10; ++i) {
    d[i] = 0;
    z[i] = 0;
  }

  for (i = 0; i < 5; ++i) {
    for (j = 0; j < 5; ++j) {
      a[i][j] = 0;
    }
  }

  for (i = 0; i < m; ++i) {
    double x = pt[i].x;
    double y = pt[i].y;
    d[1] = d[1] + x;
    a[1][2] = d[1];
    d[2] = d[2] + SQR(x);
    a[1][3] = d[2];
    d[3] = d[3] + y;
    a[1][4] = d[3];
    d[4] = d[4] + SQR3(x);
    a[2][3] = d[4];
    d[5] = d[5] + x * y;
    a[2][4] = d[5];
    d[6] = d[6] + SQR4(x);
    a[3][3] = d[6];
    d[7] = d[7] + x * x * y;
    a[3][4] = d[7];
  }

  a[1][1] = m;
  a[2][1] = a[1][2];
  a[2][2] = a[1][3];
  a[3][1] = a[1][3];
  a[3][2] = a[2][3];
  n = 3;      //n为行数
  m = 4;      //m为列数

  for (k = 1; k <= n - 1; ++k) {
    for (i = k + 1; i <= n; ++i) {
      for (j = k + 1; j <= m; ++j) {
        a[i][j] = a[i][j] - a[i][k] * a[k][j] / a[k][k];
      }
    }
  }

  z[n] = a[n][m] / a[n][n];

  for (k = n - 1; k >= 1; --k) {
    z[k] = 0;

    for (j = n; j >= k + 1; --j) {
      z[k] = z[k] + a[k][j] * z[j];
    }

    z[k] = (a[k][m] - z[k]) / a[k][k];
  }

  h[0] = z[3];
  h[1] = z[2];
  h[2] = z[1];
  return 0;
}
