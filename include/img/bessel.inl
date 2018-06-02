// 画5阶的贝塞尔曲线,与windows的贝塞尔曲线屏保差不多
double t[20]; //点数
int k = 5; //阶数
double PtX[6] = { 50, 100, 360, 280, 440, 500}; // 初始5 个点
double PtY[6] = {50, 300, 250, 100, 120, 340};
int nFlagX[6] = {1, 1, 1, 1, 1, 1};
int nFlagY[6] = {1, 1, 1, 1, 1, 1};
int MaxPt = 6; //最以大点数
BOOL bClosed = TRUE;// 是否闭合
int nColor[7] = {0x0000ff, 0xff00, 0xff0000, 0x11ffff, 0xff00ff, 0xffff00, 0xaa044};
int nIndexColor = 0;
double Blend1(int i, int j, double U)
{
  double nRet = 0.001;
  double numer = 0.001;
  double denom = 0.001;
  double V1 = 0.001;
  double V2 = 0.001;
  double newU = 0.001;

  if (i > 0 && bClosed) {
    newU = U - i + MaxPt + 1;

    while (newU >= MaxPt + 1) {
      newU = newU - (MaxPt + 1);
    }

    while (newU < 0) {
      newU = newU + (MaxPt + 1);
    }

    nRet = Blend1(0, j, newU);
    return nRet;
  }

  if (j == 1) {
    if (t[i] <= U && U < t[i + 1]) {
      nRet = 1;
    }
    else if (U = MaxPt && t[i] <= U && U <= t[i + 1]) {
      nRet = 1;
    }
    else {
      nRet = 0;
    }

    return nRet;
  }

  denom = t[i + j - 1] - t[i];

  if (denom == 0) {
    V1 = 0;
  }
  else {
    numer = (U - t[i]) * Blend1(i, j - 1, U);
    V1 = numer / denom;
  }

  denom = t[i + j] - t[i + 1];

  if (denom == 0) {
    V2 = 0.0;
  }
  else {
    numer = (t[i + j] - U) * Blend1(i + 1, j - 1, U);
    V2 = numer / denom;
  }

  nRet = V1 + V2;
  return nRet;
}
double X(double U)
{
  double nRet = 0.001;
  int i = 0;
  double V = 0.001;
  double fTemp = 0.001;

  for (i = 0; i < MaxPt; i++) {
    fTemp = Blend1(i, k, U);
    V = V + PtX[i] * fTemp;
  }

  nRet = V;
  return nRet;
}
double Y(double U)
{
  double nRet = 0.001;
  int i = 0;
  double V = 0.001;
  double fTemp = 0.001;

  for (i = 0; i < MaxPt; i++) {
    fTemp = Blend1(i, k, U);
    V = V + PtY[i] * fTemp;
  }

  nRet = V;
  return nRet;
}
void Fill_T()
{
  int i;

  if (bClosed) {
    for (i = 0; i < 20; i++) {
      t[i] = i;
    }
  }
  else {
    for (i = 0; i < 20; i++) {
      if (i < k) {
        t[i] = 0;
      }
      else if (i <= MaxPt) {
        t[i] = i - k + 1;
      }
      else {
        t[i] = MaxPt - k + 2;
      }
    }
  }
}
void Form_Load()
{
  /*
   PtX[0] = 50; PtY[0] = 50;
   PtX[1] = 100; PtY[1] = 300;
   PtX[2] = 360; PtY[2] = 250;
   PtX[3] = 280; PtY[3] = 100;
   PtX[4] = 440; PtY[4] = 120;
   PtX[5] = 600; PtY[5] = 400;*/
}
void DrawCurve(HDDC Me)
{
  int _x0, _y0;
  COLORREF _clr;
  int i, r, n;
  double U;
  int xx, yy;
  int x1, y1;
  int x2, y2; // = (double)X(50);
  Fill_T();

  if (bClosed) {
    r = MaxPt + 1;
  }
  else {
    r = MaxPt - k + 2;
  }

  _clr = nColor[nIndexColor];
  x1 = (int)(X(0.02) + 0.5);
  y1 = (int)(Y(0.02) + 0.5);
  TMoveTo(Me, x1, y1);

  for (i = 0; i <= r * 21; i++) {
    U = i / 20.0;
    x2 = (int)(X(U) + 0.5);
    y2 = (int)(Y(U) + 0.5);
    xx = x1 - x2;
    yy = y1 - y2;

    if (x2 > 800 || y2 > 600 || abs(xx) > 50 || abs(yy) > 50) {
      continue;
    }

    for (n = 0; n < 6; n++) {
      TMoveTo(Me, x1, y1 + 4 * n);
      _LineTo(Me, x2, y2 + n * 4);
    }

    x1 = x2;
    y1 = y2;
  }
}
struct MyPoint {
  int x;
  int y;
};
void ontimer_bessel()
{
  // TODO: Add your message handler code here and/or call default
  static int nTime = 0;
  int i;
  nTime++;

  if (nTime > 20) {
    nTime = 0;
    nIndexColor++;

    if (nIndexColor > 6) {
      nIndexColor = 0;
    }
  }

  for (i = 0; i < 6; i++) {
    if (nFlagX[i]) {
      PtX[i] += (10 + i);

      if (PtX[i] > 640) {
        nFlagX[i] = 0;
      }
    }
    else {
      PtX[i] -= (15 + i);

      if (PtX[i] < 30) {
        nFlagX[i] = 1;
      }
    }

    if (nFlagY[i]) {
      PtY[i] += (10 + i);

      if (PtY[i] > 480) {
        nFlagY[i] = 0;
      }
    }
    else {
      PtY[i] -= (15 + i);

      if (PtY[i] < 30) {
        nFlagY[i] = 1;
      }
    }
  }
}
void draw_bessel(HDDC pDC)
{
  static int nFlag = 0;

  if (0 == nFlag) {
    nFlag++;
  }

  draw_fillrect(pDC, 0, 0, 640, 480, rgb(0, 0, 0));
  DrawCurve(pDC);
}

