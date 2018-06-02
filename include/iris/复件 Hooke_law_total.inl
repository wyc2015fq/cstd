
enum { NeiYuan, WaiYuan };

int Hooke_law_total(int x0, int y0, const IPOINT* p, int n, int Imax, double deta, int canshu, double* ci)
{
  // D 为总的位移 total_distance
  // Op_x，Op_y 为新的平衡点位置 new_balance_point
  // Imax 为最多循环次数
  // 初始化
  double Op_x = (x0);       // 圆点中心点  即圆心  行方向
  double Op_y = (y0);       // 圆点中心  列方向
  double k = 1. / n;
  double* p_x = 0;
  double* p_y = 0;
  double* p_rr = 0;
  double* p_aa = 0;
  double Rp = 0, pre_Rp = 0;
  double C = 0, pre_C = 0;
  double* e_x = 0;
  double* e_y = 0;
  double* d = 0;
  double sumx = 0, C_d  = 100/*, Radius = 0*/;
  int i = 0, m = 1;
  int nmax = Imax + 10;

  p_x = GMALLOC(double, nmax * 7);

  for (i = 0; i < 7 * nmax; ++i) {
    p_x[i] = 0;
  }

  p_y = p_x + nmax * (1);
  p_rr = p_x + nmax * (2);
  p_aa = p_x + nmax * (3);
  e_x = p_x + nmax * (4);
  e_y = p_x + nmax * (5);
  d = p_x + nmax * (6);

  C = 0;
  // 求极坐标边缘点在直角坐标中的对应点
  sumx = 0;

  if (NeiYuan == canshu) {
    for (i = 0; i < n; ++i) {
      sumx += p[i].y;
      p_x[i] = x0 + p[i].x * cos(p[i].y * PI / 180);
      p_y[i] = y0 + p[i].x * sin(p[i].y * PI / 180);
    }
  }
  else if (WaiYuan == canshu) {
    n = MIN(countof(p_x), n);

    for (i = 0; i < n; ++i) {
      double thea = 0;
      sumx += p[i].x;

      if (0 < p[i].x && p[i].y < 24) {
        thea = 1 + (p[i].y - 1) * 2.;
      }
      else if (23 < p[i].y && p[i].y < 201) {
        thea = 46 + (p[i].y - 24) / 2.;
      }
      else if (200 < p[i].y && p[i].y < 247) {
        thea = 135 + (p[i].y - 201) * 2.;
      }
      else if (246 < p[i].y && p[i].y < 424) {
        thea = 226 + (p[i].y - 247) / 2.;
      }
      else if (423 < p[i].y && p[i].y < 447) {
        thea = 315 + (p[i].y - 424) * 2.;
      }

      p_x[i] = x0 + p[i].x * cos(thea * PI / 180);
      p_y[i] = y0 + p[i].x * sin(thea * PI / 180);
    }
  }

  pre_Rp = sumx / n;    // 平均半径长度
  m = 1;

#if 1

  //p_rr = zeros(1, N);
  //p_aa = zeros(1, N);
  while ((m < Imax) && (C_d > deta)) {
    // 求半径和方向
    double sum_p_rr = 0;
    double D = 0, sum_x = 0, sum_y = 0/*, D_x = 0, D_y = 0*/;

    for (i = 0; i < n; ++i) {
      p_rr[i] = sqrt(SQR(p_x[i] - Op_x) + SQR(p_y[i] - Op_y)); // 4-14

      if ((p_x[i] > Op_x) && (p_y[i] >= Op_y)) {
        p_aa[i] = asin(fabs(p_y[i] - Op_y) / p_rr[i]);
      }

      if ((p_x[i] <= Op_x) && (p_y[i] > Op_y)) {
        p_aa[i] = asin(fabs(p_x[i] - Op_x) / p_rr[i]) + 0.5 * PI;
      }

      if ((p_x[i] < Op_x) && (p_y[i] <= Op_y)) {
        p_aa[i] = asin(fabs(p_y[i] - Op_y) / p_rr[i]) + PI;
      }

      if ((p_x[i] >= Op_x) && (p_y[i] < Op_y)) {
        p_aa[i] = asin(fabs(p_x[i] - Op_x) / p_rr[i]) + 1.5 * PI;
      }

      sum_p_rr += p_rr[i];
    }

    Rp = sum_p_rr / n;

    // 求分位移
    for (i = 0; i < n; ++i) {
      e_x[i] = cos(p_aa[i]);
      e_y[i] = sin(p_aa[i]);
    }

    D = 0, sum_x = 0, sum_y = 0; // D_x = 0, D_y = 0

    // 求总位移
    for (i = 0; i < n; ++i) {
      d[i] = (-k) * (Rp - p_rr[i]);
      sum_x += d[i] * e_x[i];
      sum_y += d[i] * e_y[i];
    }

    D = d[n - 1];
    // 求新的平衡中心点
    Op_x = Op_x + sum_x;
    Op_y = Op_y + sum_y;
    C = fabs(D) + fabs(Rp - pre_Rp);
    C_d = fabs(C - pre_C);
    pre_C = C;
    pre_Rp = Rp;
    m = m + 1;
  }

#endif
  ci[1] = Op_x;
  ci[0] = Op_y;
  ci[2] = pre_Rp;

  GFREE(p_x);

  return 0;
}
