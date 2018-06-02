
enum { NeiYuan, WaiYuan };

int Hooke_law_total1(buf_t* bf, int x0, int y0, const IPOINT* p, int n, int Imax, int ideta, int* ci)
{
  // D 为总的位移 total_distance
  // Op_x，Op_y 为新的平衡点位置 new_balance_point
  // Imax 为最多循环次数
  // 初始化
  int Op_x = (x0);       // 圆点中心点  即圆心  行方向
  int Op_y = (y0);       // 圆点中心  列方向
  //double* p_aa=0;
  int Rp = 0, pre_Rp = 0;
  int C = 0, pre_C = 0;
  int sumx = 0, C_d  = 100 << 16/*, Radius = 0*/;
  int i = 0, m = 1;
  int px = 0, py = 0;
  int* p_rr = 0;
  int nmax = 3 * n;
  //ci[0] = Op_x;  ci[1] = Op_y;  ci[2] = Rp;  return 0;

  if (n < 3) {
    return 0;
  }

  BFMALLOC(bf, p_rr, nmax);
  MEMSET(p_rr, 0, n);

  C = 0;
  // 求极坐标边缘点在直角坐标中的对应点
  sumx = 0;

  pre_Rp = (sumx) / n;    // 平均半径长度
  m = 1;

  while ((m < Imax) && (C_d > ideta)) {
    // 求半径和方向
    int sum_p_rr = 0, d = 0, dRp;
    int D = 0, sum_x = 0, sum_y = 0/*, D_x = 0, D_y = 0*/;

    for (i = 0; i < n; ++i) {
      p_rr[i] = FastDistance2D((p[i].x) - Op_x, (p[i].y) - Op_y); // 4-14
      sum_p_rr += p_rr[i];
    }

    Rp = 1 + (sum_p_rr) / n;
    sum_x = 0, sum_y = 0; // D_x = 0, D_y = 0

    for (i = 0; i < n; ++i) {
      int tt = p_rr[i];
      int t1 = (Rp - p_rr[i]);
      d += t1;
      // 求位移
      sum_x += t1 * (Op_x - p[i].x);
      sum_y += t1 * (Op_y - p[i].y);
    }

    sum_x /= Rp * n;
    sum_y /= Rp * n;

    // 求新的平衡中心点
    Op_x += (sum_x);
    Op_y += (sum_y);
    dRp = Rp - pre_Rp;
    C = IABS(d) + IABS(dRp);
    C_d = (C - pre_C);
    C_d = IABS(C_d);
    pre_C = C;
    pre_Rp = Rp;
    m = m + 1;
  }

  BFFREE(bf, p_rr, nmax);

  ci[0] = Op_x;
  ci[1] = Op_y;
  ci[2] = Rp;
  return 0;
}

int Hooke_law_total(buf_t* bf, int x0, int y0, const IPOINT* p, int n, int Imax, int ideta, int canshu, int* ci)
{
  // D 为总的位移 total_distance
  // Op_x，Op_y 为新的平衡点位置 new_balance_point
  // Imax 为最多循环次数
  // 初始化
  int Op_x = (x0);       // 圆点中心点  即圆心  行方向
  int Op_y = (y0);       // 圆点中心  列方向
  //double* p_aa=0;
  int Rp = 0, pre_Rp = 0;
  int C = 0, pre_C = 0;
  int sumx = 0, C_d  = 100 << 16/*, Radius = 0*/;
  int i = 0, m = 1;
  int px = 0, py = 0;
  int* p_x = 0;
  int* p_y = 0;
  int* p_rr = 0;
  int nmax = 3 * n;

  if (n < 3) {
    return 0;
  }

  BFMALLOC(bf, p_x, nmax);
  MEMSET(p_x, 0, 3 * n);
  p_y = p_x + n;
  p_rr = p_x + 2 * n;

  C = 0;
  // 求极坐标边缘点在直角坐标中的对应点
  sumx = 0;

  if (NeiYuan == canshu) {
    for (i = 0; i < n; ++i) {
      int thea = 0;
      px = p[i].x, py = p[i].y;
      sumx += py;
      ASSERT(py < 360);
      thea = BOUND(py, 0, 359);
      p_x[i] = x0 + ((px * cos360[thea]) >> 16);
      p_y[i] = y0 + ((px * sin360[thea]) >> 16);
    }
  }
  else if (WaiYuan == canshu) {
    for (i = 0; i < n; ++i) {
      int thea = 0;
      px = p[i].x, py = p[i].y;
      sumx += px;

      if (0 < px && py < 24) {
        thea = (py) * 2;
      }
      else if (23 < py && py < 201) {
        thea = 45 + (py - 24) / 2;
      }
      else if (200 < py && py < 247) {
        thea = 135 + (py - 201) * 2;
      }
      else if (246 < py && py < 424) {
        thea = 226 + (py - 247) / 2;
      }
      else if (423 < py && py < 447) {
        thea = 315 + (py - 424) * 2;
      }

      thea = BOUND(thea, 0, 359);
      ASSERT(thea < 360);
      p_x[i] = x0 + ((px * cos360[thea]) >> 16);
      p_y[i] = y0 + ((px * sin360[thea]) >> 16);
    }
  }

  pre_Rp = (sumx) / n;    // 平均半径长度
  m = 1;

  while ((m < Imax) && (C_d > ideta)) {
    // 求半径和方向
    int sum_p_rr = 0, d = 0, dRp;
    int D = 0, sum_x = 0, sum_y = 0/*, D_x = 0, D_y = 0*/;

    for (i = 0; i < n; ++i) {
      p_rr[i] = FastDistance2D((int)(p_x[i] - Op_x), (int)(p_y[i] - Op_y)); // 4-14
      sum_p_rr += p_rr[i];
    }

    Rp = (sum_p_rr) / n;
    sum_x = 0, sum_y = 0; // D_x = 0, D_y = 0

    for (i = 0; i < n; ++i) {
      int tt = p_rr[i] * n;
      int t1 = (Rp - p_rr[i]) << 16;
      // 求位移
      sum_x -= t1 * (p_x[i] - Op_x) / tt;
      sum_y -= t1 * (p_y[i] - Op_y) / tt;
    }

    D = IABS(d);
    // 求新的平衡中心点
    Op_x += (sum_x) >> 16;
    Op_y += (sum_y) >> 16;
    dRp = Rp - pre_Rp;
    dRp = IABS(dRp);
    C = (D >> 16) + (dRp);
    C_d = (C - pre_C);
    C_d = IABS(C_d);
    pre_C = C;
    pre_Rp = Rp;
    m = m + 1;
  }

  BFFREE(bf, p_x, nmax);
  ci[0] = Op_y;
  ci[1] = Op_x;
  ci[2] = pre_Rp;

  return 0;
}
