// CLOCK.C - Windows DDEML Clock
//
// DDE Transactions:
// ----------------
// Service: Clock
// Topic : Time
// Item : Now
//
// Use Request or Advise to get the time or use Poke to change the time.
// Time Format Hour:Minute:Seconds where
// Hour = 0-23
// Minute = 0-59
// Seconds = 0-59
/*--------------------------------------------------------------------------*/
/* Typedefs and Structures */
/*--------------------------------------------------------------------------*/
typedef struct {
  SHORT sin;
  SHORT cos;
} TRIG;
TRIG CirTab[] = {
  // circle sin, cos, table
  {0, -7999},
  {836, -7956},
  {1663, -7825},
  {2472, -7608},
  {3253, -7308},
  {3999, -6928},
  {4702, -6472},
  {5353, -5945},
  {5945, -5353},
  {6472, -4702},
  {6928, -4000},
  {7308, -3253},
  {7608, -2472},
  {7825, -1663},
  {7956, -836},
  {8000, 0},
  {7956, 836},
  {7825, 1663},
  {7608, 2472},
  {7308, 3253},
  {6928, 4000},
  {6472, 4702},
  {5945, 5353},
  {5353, 5945},
  {4702, 6472},
  {3999, 6928},
  {3253, 7308},
  {2472, 7608},
  {1663, 7825},
  {836, 7956},
  {0, 7999},
  { -836, 7956},
  { -1663, 7825},
  { -2472, 7608},
  { -3253, 7308},
  { -4000, 6928},
  { -4702, 6472},
  { -5353, 5945},
  { -5945, 5353},
  { -6472, 4702},
  { -6928, 3999},
  { -7308, 3253},
  { -7608, 2472},
  { -7825, 1663},
  { -7956, 836},
  { -7999, -0},
  { -7956, -836},
  { -7825, -1663},
  { -7608, -2472},
  { -7308, -3253},
  { -6928, -4000},
  { -6472, -4702},
  { -5945, -5353},
  { -5353, -5945},
  { -4702, -6472},
  { -3999, -6928},
  { -3253, -7308},
  { -2472, -7608},
  { -1663, -7825},
  { -836, -7956},
};
#define HOURSCALE 65
#define MINUTESCALE 80
#define HHAND TRUE
#define MHAND FALSE
#define SECONDSCALE 80
#define MAXBLOBWIDTH 25
#define BUFLEN 30
#define CLKSCALE ((int)8000)
void DrawFatHand(HDDC hDC, int pos, COLORREF clr, BOOL hHand, POINT clockCenter, int clockRadius)
{
  register int m;
  int n;
  int scale;
  POINT tip;
  POINT stip;
  int _x0, _y0;
  scale = hHand ? 7 : 5;
  n = (pos + 15) % 60;
  m = (int)((((int)clockRadius * scale) / 100));
  stip.y = (int)((int)(CirTab[n].cos) * m / CLKSCALE);
  stip.x = (int)((int)(CirTab[n].sin) * m / CLKSCALE);
  scale = hHand ? 65 : 80;
  tip.y = (int)((int)(CirTab[pos].cos) * (((int)clockRadius * scale) / 100) / CLKSCALE);
  tip.x = (int)((int)(CirTab[pos].sin) * (((int)clockRadius * scale) / 100) / CLKSCALE);
  TMoveTo(hDC, clockCenter.x + stip.x, clockCenter.y + stip.y);
  TLineTo(hDC, clockCenter.x + tip.x, clockCenter.y + tip.y, clr);
  TMoveTo(hDC, clockCenter.x - stip.x, clockCenter.y - stip.y);
  TLineTo(hDC, clockCenter.x + tip.x, clockCenter.y + tip.y, clr);
  scale = hHand ? 15 : 20;
  n = (pos + 30) % 60;
  m = (int)(((int)clockRadius * scale) / 100);
  tip.y = (int)((int)(CirTab[n].cos) * m / CLKSCALE);
  tip.x = (int)((int)(CirTab[n].sin) * m / CLKSCALE);
  TMoveTo(hDC, clockCenter.x + stip.x, clockCenter.y + stip.y);
  TLineTo(hDC, clockCenter.x + tip.x, clockCenter.y + tip.y, clr);
  TMoveTo(hDC, clockCenter.x - stip.x, clockCenter.y - stip.y);
  TLineTo(hDC, clockCenter.x + tip.x, clockCenter.y + tip.y, clr);
}
void DrawHand(HDDC hDC, int pos, COLORREF clr, int scale, int patMode, POINT clockCenter, int clockRadius)
{
  int radius;
  int _x0, _y0;
  TMoveTo(hDC, clockCenter.x, clockCenter.y);
  radius = (int)(((int)clockRadius * scale) / 100);
  TLineTo(hDC, clockCenter.x + (int)(((int)(CirTab[pos].sin) * (radius)) / CLKSCALE), clockCenter.y + (int)(((int)(CirTab[pos].cos) * (radius)) / CLKSCALE), clr);
}
int DrawClock(HDDC hDC, const RECT* lpRect)
{
  struct tm* gTime;
  int nFontSize = 24;
  int clockRadius;
  int i;
  POINT clockCenter;
  RECT clockRect = *lpRect;
  time_t t;
  COLORREF clrText = _RGB(255, 0, 0);
  COLORREF hbrForeground = _RGB(111, 235, 245);
  COLORREF hpenForeground = _RGB(111, 235, 245);
  time(&t);
  gTime = localtime(&t);
  gTime->tm_hour = abs(gTime->tm_hour >= 12 ? gTime->tm_hour - 12 : gTime->tm_hour);

  //DrawBorder
  if (0) {
    RECT rc = clockRect;
    COLORREF clrBorder = _RGB(255, 0, 0);
    //DrawRectangle(hDC, Rect.left+1, Rect.top+1, Rect.right-2, Rect.bottom-2);
    RCOFFSET(&rc, 1, 1, - 1, - 1);
    TDrawRect(hDC, &rc, clrBorder);
  }

  //DrawFace(hDC);
  clockRadius = MIN(RCWIDTH(&clockRect), RCHEIGHT(&clockRect)) / 2 - 20; // (clockRect.right - clockRect.left-6) >> 1;
  clockCenter.y = (clockRect.top + clockRect.bottom) / 2; //clockRect.top + ((clockRect.bottom - clockRect.top) >> 1);
  clockCenter.x = (clockRect.left + clockRect.right) / 2; //clockRect.left + clockRadius+3;

  for (i = 0; i < 60; i++) {
    RECT tRect;
    tRect.top = (int)(((int)(CirTab[i].cos) * clockRadius) / CLKSCALE + clockCenter.y);
    tRect.left = (int)(((int)(CirTab[i].sin) * clockRadius) / CLKSCALE + clockCenter.x);

    if (i % 5) {
      /* Draw a dot. */
      tRect.right = tRect.left + 1;
      tRect.bottom = tRect.top + 1;
      TFillRect(hDC, tRect, hbrForeground);
    }
    else {
      char c[32];
      int ic = (i / 5) ? (i / 5) : 12;
      _itoa(ic, c, 10);
      OffsetRect(&tRect, - (nFontSize >> (1 + (ic < 10))), - (nFontSize >> 1));
      //SelectFontToMemDC("宋体", nFontSize, 0, 0);
      //TDrawText2(hDC, c, strlen(c), &tRect, FONT_ANTIALIAS | FONT_BOLD, clrText);
    }
  }

  DrawFatHand(hDC, gTime->tm_min, hpenForeground, MHAND, clockCenter, clockRadius);
  DrawFatHand(hDC, gTime->tm_hour * 5 + (gTime->tm_min / 12), hpenForeground, HHAND, clockCenter, clockRadius);
  DrawHand(hDC, gTime->tm_sec, hpenForeground, MINUTESCALE, R2_COPYPEN, clockCenter, clockRadius);
  return 0;
}
// 画钻石
int DrawDiamond(HDDC hDC, const RECT* lpRect, COLORREF clr)
{
  int CenterX = RCWIDTH(lpRect) / 2;
  int CenterY = RCHEIGHT(lpRect) / 2;
  int radius = MIN(CenterX, CenterY); //设置金刚石的半径
  int i, j;
  int _x0, _y0;
  static double rotation = 0; //用弧度表示的圆心角
  typedef struct MyPoint {
    double x, y;
  } MyPoint;
  enum {
    MaxPoints = 18
  }; //设定金刚石的角点数 x与y分别存放宝石的顶点的正弦和余弦值
  MyPoint Points[MaxPoints];
  {
    //存放宝石(正多边形)的所有顶点的正弦和余弦值
    double j; //j用来表示旋转每个角点的角度
    const double StepAngle = 2 * PI / MaxPoints; //角点之间的圆心角
    rotation += PI / 32; //设定每次旋转的角度

    if (rotation > StepAngle) {
      rotation -= StepAngle;
    }

    //保持每次旋转角度小于两点之间的圆心角
    for (i = 0, j = rotation; i < MaxPoints; i++, j += StepAngle) {
      Points[i].x = cos(j);
      Points[i].y = sin(j);
    }
  }

  //通过双层循环形成金刚石的图案
  for (i = 0; i < MaxPoints; i++) {
    for (j = i + 1; j < MaxPoints; j++) {
      int x = (int)(CenterX + Points[i].x * radius);
      int y = (int)(CenterY + Points[i].y * radius);
      int m = (int)(CenterX + Points[j].x * radius);
      int n = (int)(CenterY + Points[j].y * radius);
      TMoveTo(hDC, x, y);
      TLineTo(hDC, m, n, clr);
    }
  }

  return 0;
}

