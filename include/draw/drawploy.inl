
int draw_poly_aa(HDDC hDC, const DPOINT* pt, int len, COLORREF clr, int isclose)
{
  int i;

  for (i = 0; i < len - 1; i++) {
    draw_line_aa(hDC, pt[i].x, pt[i].y, pt[i + 1].x, pt[i + 1].y, clr);
  }

  if (isclose) {
    draw_line_aa(hDC, pt[0].x, pt[0].y, pt[len - 1].x, pt[len - 1].y, clr);
  }

  return 0;
}
//扫描线法 填充多边形
int draw_fillpoly2(HDDC hDC, const DPOINT* pt, int len, COLORREF clr)
{
  int i, j, yscan;
  //每根扫描线交点
  double* p = (double*)malloc(len * sizeof(double));
  double* q = (double*)malloc(len * sizeof(double));
  int pmin = (int)(pt[0].y - 0.5), pmax = (int)(pt[0].y - 0.5);
  RECT rc;
  HDDCGET_CLIP(hDC, &rc);

  for (i = 0; i < len; ++i) {
    //建立边表
    j = i == len - 1 ? 0 : i + 1;

    if (pmin > (pt[j].y - 0.5)) {
      pmin = (int)(pt[j].y);
    }

    if (pmax < (pt[j].y - 0.5)) {
      pmax = (int)(pt[j].y);
    }

    q[i] = ((pt[j].y - pt[i].y) ? ((pt[j].x - pt[i].x) / (pt[j].y - pt[i].y)) : (1 << 31));
  }

  pmin = MAX(pmin, rc.top);
  pmax = MIN(pmax, rc.bottom);

  for (yscan = pmin; yscan < pmax; ++yscan) {
    double dyscan = yscan + 0.5;
    //扫描线遵守'“上开下闭”的原则
    int b = 0;

    for (i = 0; i < len; i++) {
      j = i == len - 1 ? 0 : i + 1;

      //判断扫描线与线段是否相交
      if ((dyscan >= pt[i].y && dyscan < pt[j].y) || (dyscan <= pt[i].y && dyscan > pt[j].y)) {
        p[b++] = (pt[i].x) + ((dyscan - pt[i].y) * q[i]);
      }
    }

#define IPLESS(A, B) ((A)<(B))
    INSERTSORT(b, p, IPLESS, double);
#undef IPLESS

    //assert(0 == b % 2);
    for (i = 0; i < b - 1; i += 2) {
      int x0, x1;
      x0 = (int)(p[i] + 0.5);
      x1 = (int)(p[i + 1]);
      DRAWHLINE(hDC, x0, x1, yscan, clr);
    }
  }

  free(p);
  free(q);
  //DrawPoly(hDC, pt, len, clr, 1);
  return 0;
}
int draw_fillpoly(HDDC hDC, const DPOINT* pt, int len, COLORREF clr)
{
  enum {MAX_POLY_CORNERS = 1000};
  int nodes, nodeX[MAX_POLY_CORNERS], pixelY, i, j, swap;
  DWORD* pix = (hDC->data);
  int al = (hDC->bw >> 2);
  int ymin = (int)(pt[0].y), ymax = (int)(pt[0].y);
  RECT rc;
  HDDCGET_CLIP(hDC, &rc);
  j = len - 1;

  for (i = 0; i < len; ++i) {
    if (ymin > (pt[j].y - 0.5)) {
      ymin = (int)(pt[j].y);
    }

    if (ymax < (pt[j].y - 0.5)) {
      ymax = (int)(pt[j].y);
    }
  }

  ymin = MAX(ymin, rc.top);
  ymax = MIN(ymax, rc.bottom);

  // Loop through the rows of the image.
  for (pixelY = ymin; pixelY < ymax; pixelY++) {
    double dpixelY = pixelY;
    // Build a list of nodes.
    nodes = 0;
    j = len - 1;

    for (i = 0; i < len; i++) {
      if ((pt[i].y < dpixelY && pt[j].y >= dpixelY) || (pt[j].y < dpixelY && pt[i].y >= dpixelY)) {
        nodeX[nodes++] = (int)(pt[i].x + (dpixelY - pt[i].y) * (pt[j].x - pt[i].x) / (pt[j].y - pt[i].y));
      }

      j = i;
    }

    // Sort the nodes, via a simple “Bubble” sort.
    i = 0;

    while (i < nodes - 1) {
      if (nodeX[i] > nodeX[i + 1]) {
        swap = nodeX[i];
        nodeX[i] = nodeX[i + 1];
        nodeX[i + 1] = swap;

        if (i) {
          i--;
        }
      }
      else {
        i++;
      }
    }

    // Fill the pixels between node pairs.
    for (i = 0; i < nodes; i += 2) {
      if (nodeX[i ] >= rc.right) {
        break;
      }

      if (nodeX[i + 1] > rc.left) {
        if (nodeX[i ] < rc.left) {
          nodeX[i ] = rc.left ;
        }

        if (nodeX[i + 1] > rc.right) {
          nodeX[i + 1] = rc.right;
        }

#define fillPixel(x, y) pix[(x)+(y)*al]=clr

        for (j = nodeX[i]; j < nodeX[i + 1]; j++) {
          fillPixel(j, pixelY);
        }
      }
    }
  }

  return 0;
}

int FillD(DWORD* pix, DWORD x, int n)
{
  int i;

  for (i = 0; i < n; ++i) {
    pix[i] = x;
  }

  return 0;
}
//判断点q是否在多边形内
BOOL InsidePoly(const POINT* pt, int len, int x, int y)
{
  int i, j, k = 0, Xcoss;

  for (i = 0; i < len; i++) {
    j = (i == len - 1) ? 0 : (i + 1);

    if ((y < pt[i].y && y >= pt[j].y) || (y >= pt[i].y && y < pt[j].y)) {
      Xcoss = (y - pt[i].y) * (pt[i].x - pt[j].x) / (pt[i].y - pt[j].y) + pt[i].x;

      if (x <= Xcoss) {
        k++;
      }
    }
  }

  return k % 2;
}
// 逐点判断法 填充多边形，太慢
int PointFill(HDDC hDC, const POINT* pt, int len, COLORREF clr)
{
  POINT Pmax, Pmin;
  int i, j;
  Pmax = pt[0];
  Pmin = pt[0];

  for (i = 1; i < len; i++) {
    if (pt[i].x > Pmax.x) {
      Pmax.x = pt[i].x;
    }

    if (pt[i].y > Pmax.y) {
      Pmax.y = pt[i].y;
    }

    if (pt[i].x < Pmin.x) {
      Pmin.x = pt[i].x;
    }

    if (pt[i].y < Pmin.y) {
      Pmin.y = pt[i].y;
    }
  }

  for (i = Pmin.y; i <= Pmax.y; i++) {
    for (j = Pmin.x; j <= Pmax.x; j++) {
      if (InsidePoly(pt, len, j, i) == TRUE) {
        _SetPixel(hDC, j, i, clr);
      }
    }
  }

  return 0;
}
typedef float MATRIX4X4[4][4];
int trans2d(int n, float* X, float* Y, float* XT, float* YT, MATRIX4X4 B, float* C)
{
  int i;

  for (i = 1; i <= 10; ++i) {
    XT[i] = X[i] * B[1][1] + Y[i] * B[2][1] + C[i] * B[3][1];
    YT[i] = X[i] * B[1][2] + Y[i] * B[2][2] + C[i] * B[3][2];
  }

  return 0;
}
// 三角形顶点排序 从上到下
#define TriSort(pt, t) {if (pt[0].y>pt[1].y) {CC_SWAP(pt[0], pt[1], t);}if (pt[1].y>pt[2].y) {CC_SWAP(pt[1], pt[2], t);}if (pt[0].y>pt[1].y) {CC_SWAP(pt[0], pt[1], t);}}
#define GET_LINE_X(X1, Y1, X2, Y2, Y3) (((Y3-Y1)*(X2-X1)/(Y2-Y1))+X1)
// 填充平顶(底)三角形
// y3==y2
int FillTriUp(HDDC hDC, int x1, int y1, int x2, int y2, int x3, COLORREF clr)
{
  int dy = y1 < y2 ? 1 : - 1, y = y1;
  int x12, x13;

  for (; y != y2; y += dy) {
    x12 = GET_LINE_X(x1, y1, x2, y2, y);
    x13 = GET_LINE_X(x1, y1, x3, y2, y);
    DRAWHLINE(hDC, x12, x13, y, clr);
  }

  return 0;
}
// 填充平顶(底)梯形
int FillTiXin(HDDC hDC, int x1, int x2, int y1, int x3, int x4, int y2, COLORREF clr)
{
  int dy = y1 < y2 ? 1 : - 1, y = y1;

  for (; y != y2; y += dy) {
    int x11 = GET_LINE_X(x1, y1, x3, y2, y);
    int x12 = GET_LINE_X(x2, y1, x4, y2, y);
    DRAWHLINE(hDC, (int)x11, (int)x12, y, clr);
  }

  return 0;
}
// 填充m种颜色的棋盘
int FillChessBoard(HDDC hDC, int cx, int cy, double ix, double iy, double jx, double jy, DWORD* pclr, int m)
{
  int i, j, k;
  int al = hDC->bw;
  //double jy=1, iy=0;
  //double jx=0, ix=1;
  al >>= 2;

  for (i = 0; i < hDC->h; ++i) {
    DWORD* A0 = hDC->data + i * al;

    for (j = 0; j < hDC->w; ++j) {
      k = ((int)((int)(jx * j + ix * i) / cx + (int)(jy * j - iy * i) / cy - (iy * i > jy * j))) % m;
      k = (k + m) % m; // 斜纹
      //k=((i)/cx+(j)/cy)%m;
      A0[j] = pclr[k];
    }
  }

  return 0;
}
// 填充一般三角形
int FillTri(HDDC hDC, POINT a, POINT b, POINT c, COLORREF clr)
{
  int x12;
  POINT pt3[3], t;
  pt3[0] = a, pt3[1] = b, pt3[2] = c;
  TriSort(pt3, t);
  x12 = GET_LINE_X(pt3[0].x, pt3[0].y, pt3[2].x, pt3[2].y, pt3[1].y);
  DRAWHLINE(hDC, (int)x12, pt3[1].x, pt3[1].y, clr);
  FillTriUp(hDC, pt3[0].x, pt3[0].y, pt3[1].x, pt3[1].y, x12, clr);
  FillTriUp(hDC, pt3[2].x, pt3[2].y, pt3[1].x, pt3[1].y, x12, clr);
  return 0;
}
// 渐变填充梯形，用四种颜色差值
int GradientFillTiXin(HDDC hDC, int x1, int x2, int y1, int x3, int x4, int y2, COLORREF clr1, COLORREF clr2, COLORREF clr3, COLORREF clr4)
{
  int dy = y1 < y2 ? 1 : - 1, y = y1, n = y2 - y1;

  for (; y != y2; y += dy) {
    int x11 = GET_LINE_X(x1, y1, x3, y2, y);
    int x12 = GET_LINE_X(x2, y1, x4, y2, y);
    int i1 = y - y1;
    COLORREF clr13 = GRADIENTRGB(n, i1, clr3, clr1);
    COLORREF clr24 = GRADIENTRGB(n, i1, clr4, clr2);
    DRAWHLINE_GRADIENT(hDC, x11, x12, y, clr13, clr24);
  }

  return 0;
}
