
//计算三角形权值的函数
double w(DPOINT2 X, DPOINT2 Y, DPOINT2 Z)
{
  return pt2_distance(X, Y) + pt2_distance(Y, Z) + pt2_distance(Z, X);
}

int is_co(int n, const DPOINT2* v, double* total)
{
  int j, k;
  double a, b, c;

  //根据顶点坐标判断是否能构成一个凸多边形
  for (j = 0 ; j < n ; j++) {
    double p = 0;
    double q = 0;
    int j1 = (j + n + 1) % n;
    a = v[j].y - v[j1].y;
    b = v[j].x - v[j1].x;
    c = b * v[j].y - a * v[j].x;

    for (k = 0 ; k < n ; k++) {
      total[k] = a * v[k].x - b * v[k].y + c;

      if (total[k] > 0) {
        p = p + 1;
      }
      else if (total[k] < 0) {
        q = q + 1;
      }
    }

    if ((p > 0 && q > 0) || (p == 0 && q == 0)) {
      // 无法构成凸多边形！
      return false;
    }
  }

  return true;
}


//用递归的方法输出剖分后的各个三角形
int Traceback(int i, int j, int** s, int* index, int* nindex)
{
  int* ind = 0;

  if (i == j) {
    return 0;
  }

  Traceback(i, s[i][j], s, index, nindex);
  Traceback(s[i][j] + 1, j, s, index, nindex);

  ind = index + (*nindex) * 3;
  ARRINIT3(ind, i - 1, s[i][j], j);
  (*nindex)++;
  //printf("三角形：v%dv%dv%d\n", i-1, s[i][j], j);
  return 0;
}

// DPOINT2* v 记录凸多边形各顶点坐标

int po_triangle(int n, const DPOINT2* v, int* index)
{
  int ret = 0;
  int i, j, r, k;
  int nindex = 0;
  //记录最优三角剖分中所有三角形信息
  int** s;
  //记录最优三角剖分所对应的权函数值
  double** t;
  //记录坐标在直线方程中的值
  double* total;

  t = MALLOC(double*, n);
  s = MALLOC(int*, n);

  for (i = 0 ; i < n ; i++) {
    t[i] = MALLOC(double, n);
    s[i] = MALLOC(int, n);
  }

  total = MALLOC(double, n);

  if (is_co(n, v, total)) {
    //计算最优值算法
    for (i = 0 ; i < n ; i++) {
      t[i][i] = 0;
    }

    for (r = 2; r < n ; r++) {
      for (i = 1; i < n - r + 1 ; i++) {
        j = i + r - 1;
        t[i][j] = t[i + 1][j] + w(v[i - 1], v[i], v[j]);
        s[i][j] = i;

        for (k = i + 1 ; k < i + r - 1 ; k++) {
          double u = t[i][k] + t[k + 1][j] + w(v[i - 1], v[k], v[j]);

          if (u < t[i][j]) {
            t[i][j] = u;
            s[i][j] = k;
          }
        }
      }
    }

    Traceback(1, n - 1, s, index, &nindex);
    //cout << endl;
    // "最优权值之和为：" << t[1][n-1] << endl;
    ret =  true;
  }

  for (i = 0 ; i < n ; i++) {
    FREE(t[i]);
    FREE(s[i]);
  }

  FREE(t);
  FREE(s);
  FREE(total);

  return nindex;
}

bool test_triangle(img_t* im)
{
  COLOR brushes[] = { BlueViolet, Salmon, Yellow, };
  COLOR brushes2[] = { Red, Green, Blue, };
  DPOINT2 po[] = {8, 26, 0, 20, 0, 10, 10, 0, 22, 12, 27, 21, 15, 26};
  enum {nn = countof(po)};
  int tr2[3 * nn] = {0};
  int i, n = nn;
  int ntr2;

  po2_scale(10, 10, n, po, po);
  po2_translate(100, 10, n, po, po);
  ntr2 = po_triangle(nn, po, tr2);
  imdrawaa_poly(im, po, n, 1, 0, Black, 1);

  // for every 3 triangle, use different color
  for (i = 0; i < ntr2; ++i) {
    DTRIANGLE2 dtr2 = pt2_make_triangle(po[tr2[i * 3 + 0]], po[tr2[i * 3 + 1]], po[tr2[i * 3 + 2]]);
    imdrawaa_triangles(im, &dtr2, 1, brushes[i % 3], Black, 1);
  }

  return 0;
}
