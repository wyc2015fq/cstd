// NSChart styles
#define NSCS_BAR  0x001
#define NSCS_PIE  0x002
#define MAXCHARLABEL  60
//#define NSCS_LINE  0x003
// Notify codes
#define NSCS_SELECTEDITEM  0x001
typedef struct _CNSChartValue {
  double m_dValue;
  char* m_sLabel;
} CNSChartValue;
enum DefaultColors {
  EMPY,
  GrayScale,
  SimpleColors
};
#define FillSolidRect TFillRect
#define TRectangle(hDC, rc, clr)      TFillRect(hDC, rc, clr)
#define AddSolidBrush(cr) m_BrushArray[iColors++]=(cr)
int DrawNSChart(HDDC dc, const RECT* rc, const char* szTitle, CNSChartValue* m_ChartValues, int iValues, DWORD m_dwStyle, int defColor)
{
  HFONT m_boldFont;
  HFONT m_txtFont;
  HFONT m_titleFont;
  int m_iCurSel;
  double m_dMax;
  double m_dTotal;
  DWORD m_BrushArray[100];
  int iColors = 0;
  HDDC pDC = dc;
  int i;
  char* str;
  RECT rect = *rc;
  m_txtFont = GetStockObject(DEFAULT_GUI_FONT);
  m_titleFont = FindGFont("Arial", 15, 1, 0);
  m_boldFont = FindGFont("Arial", 15, 1, 0);
  m_dTotal = 0.0L;
  m_dMax = 0.0l;
  m_iCurSel = -1;

  for (i = 0; i < iValues; ++i) {
    m_dTotal += m_ChartValues[i].m_dValue;

    if (m_dMax < m_ChartValues[i].m_dValue) {
      m_dMax = m_ChartValues[i].m_dValue;
    }
  }

  switch (defColor) {
  case EMPY:
    break;

  case GrayScale:
    AddSolidBrush(0x00666666);
    AddSolidBrush(0x00DDDDDD);
    AddSolidBrush(0x00888888);
    AddSolidBrush(0x00444444);
    AddSolidBrush(0x00CCCCCC);
    AddSolidBrush(0x00AAAAAA);
    //AddSolidBrush(0x00000000);
    break;

  case SimpleColors:
    AddSolidBrush(0x0000CC00);
    AddSolidBrush(0x00FF0000);
    AddSolidBrush(0x00CCCC00);
    AddSolidBrush(0x000000FF);
    AddSolidBrush(0x000000CC);
    AddSolidBrush(0x0000FF00);
    AddSolidBrush(0x00FF00FF);
    AddSolidBrush(0x00CC00CC);
    AddSolidBrush(0x00CC0000);
    AddSolidBrush(0x0000FFFF);
    AddSolidBrush(0x0000CCCC);
    AddSolidBrush(0x00FFFF00);
    break;
  };

#undef AddSolidBrush

  // Check values count
  if (iValues <= 0) {
    SelectFontToMemDC1(m_txtFont);
    str = "Values count must be > 0";
    FillSolidRect(dc, &rect, _RGB(255, 255, 255));
    rect.top += 50;
    TDrawText(dc, str, &rect, DT_CENTER | DT_VCENTER);
    return 0;
  }

  // Check color count
  if (iColors <= 1) {
    SelectFontToMemDC1(m_txtFont);
    str = "Color count mus be > 1";
    FillSolidRect(dc, &rect, _RGB(255, 255, 255));
    rect.top += 50;
    TDrawText(dc, str, &rect, DT_CENTER | DT_VCENTER);
    return 0;
  }

  //Creating double buffer painting
  FillSolidRect(dc, &rect, RGB(255, 255, 255));

  //  rect.DeflateRect(0,5);
  // Drawing the chart
  if (m_dwStyle & NSCS_BAR) {
    char str[256];
    int i;
    int _x0, _y0;
    RECT rect = *rc, rcBar;
    DWORD _clr = _RGB(0, 0, 0);
    CNSChartValue* pTmp;
    double t = 1.0L;
    int scale, height;
    RECT tmpRect;
    ASSERT(m_dTotal > 0.0L);

    if (m_dMax >= 1.0L) {
      int d = 1;
      _snprintf(str, 256, "%.0f", m_dMax);

      for (i = 0; i < (int)strlen(str) - 1; i++) {
        d *= 10;
      }

      t = MAX((((double)str[(0)] - 48) + 1) * d, m_dMax);
    }

    DeflateRect4(&rect, 40, 30, 20, 20);
    rcBar.left   =  5;
    rcBar.right  = rcBar.left + 25;
    rcBar.top    = 25;
    rcBar.bottom = rcBar.top + 15;
    i = RCHEIGHT(&rect);
    SelectFontToMemDC1(m_txtFont);
    TMoveTo(pDC, rect.left - 7, rect.top);
    _LineTo(pDC, rect.left + 7, rect.top);
    _snprintf(str, 256, (t < 1.0) ? ("%.1f") : ("%.0f"), t);
    TDrawText(pDC, str, &rcBar, DT_RIGHT);
    TMoveTo(pDC, &rect.left - 5, rect.top + i / 4);
    _LineTo(pDC, &rect.left + 5, rect.top + i / 4);
    TMoveTo(pDC, &rect.left - 7, rect.top + i / 2);
    _LineTo(pDC, &rect.left + 7, rect.top + i / 2);
    _snprintf(str, 256, (t < 1.0) ? ("%.1f") : ("%.0f"), t / 2);
    rcBar.top    = rect.top + (int)(i / 2);
    rcBar.bottom = rcBar.top + 15;
    TDrawText(pDC, str, &rcBar, DT_RIGHT);
    TMoveTo(pDC, rect.left - 5, rect.top + i * 3 / 4);
    _LineTo(pDC, rect.left + 5, rect.top + i * 3 / 4);
    rcBar = rect;
    DeflateRect4(&rcBar, 0, 10, 0, 0);
    TMoveTo(pDC, rect.left  - 15, rect.bottom);
    _LineTo(pDC, rect.right + 15, rect.bottom);
    TMoveTo(pDC, rect.left , rect.top    - 15);
    _LineTo(pDC, rect.left , rect.bottom + 15);
    scale = RCWIDTH(&rcBar) / iValues;
    height = RCHEIGHT(&rcBar);

    for (i = 0; i < iValues; i++) {
      pTmp = m_ChartValues + (i);
      rcBar.left  = rect.left + scale * i + 2;
      rcBar.right = rcBar.left + scale - 4;
      rcBar.top = (rcBar.bottom - (long)(pTmp->m_dValue * height / t));
      rcBar.bottom = rect.bottom - 1;
      //Drawing bar's
      TRectangle(pDC, &rcBar, m_BrushArray[i % iColors]);
      //Drawing top text percents
      tmpRect = rcBar;
      tmpRect.bottom = tmpRect.top - 1;
      tmpRect.top = tmpRect.bottom - 15;
      _snprintf(str, 256, "%.1f%%", 100 * pTmp->m_dValue / m_dTotal);

      if (m_iCurSel == i) {
        SelectFontToMemDC1(m_boldFont);
        TDrawText(pDC, str, &tmpRect, DT_BOTTOM | DT_CENTER);
      }
      else {
        TDrawText(pDC, str, &tmpRect, DT_BOTTOM | DT_CENTER);
      }

      //Drawing bottom text labels
      tmpRect.top    = rcBar.bottom + 2;
      tmpRect.bottom  = tmpRect.top + 15;
      TDrawText(pDC, pTmp->m_sLabel, &tmpRect, DT_CENTER);
    }
  }
  else if (m_dwStyle & NSCS_PIE) {
    double pi  = 3.1415926535 * 2;
    CNSChartValue* pTmp;
    char str[256];
    RECT  rect = *rc;
    double dAngle = 0;
    double dMidAngle = 0;
    double r = MIN((RCWIDTH(&rect) / 2) - 50, (RCHEIGHT(&rect) / 2) - 40);
    POINT p1 = iPOINT(RCX2(&rect) / 2, 15 + RCY2(&rect) / 2);
    POINT p2 = iPOINT(p1.x + (int)r , p1.y);
    POINT p3;
    POINT p4;
    int i;
    int _x0, _y0;
    DWORD _clr = _RGB(0, 0, 0);
    ASSERT(m_dTotal > 0.0L);
    TDrawEllipse(pDC, p1.x, p1.y, r, r, _clr);
    TMoveTo(pDC, p1.x, p1.y);
    _LineTo(pDC, p2.x, p2.y);
    SelectFontToMemDC1(m_txtFont);

    for (i = 0; i < iValues; i++) {
      pTmp  = m_ChartValues + (i);
      dMidAngle = pTmp->m_dValue * pi / m_dTotal;
      dAngle += dMidAngle;
      p2.x = (int)(cos(dAngle) * r + p1.x);
      p2.y = (int)(p1.y - sin(dAngle) * r);
      TMoveTo(pDC, p1.x, p1.y);
      _LineTo(pDC, p2.x, p2.y);
      dMidAngle = dAngle - dMidAngle / 2;
      p3.x = (int)(cos(dMidAngle) * r + p1.x);
      p3.y = (int)(p1.y - sin(dMidAngle) * r);
      p4.x = (int)(cos(dMidAngle) * (r * 1.2) + p1.x);
      p4.y = (int)(p1.y - sin(dMidAngle) * (r * 1.2));
      TMoveTo(pDC, p3.x, p3.y);
      _LineTo(pDC, p4.x, p4.y);
      TMoveTo(pDC, p4.x, p4.y);
      p4.x += 40 * ((p4.x > p1.x) ? (1) : (-1));
      _LineTo(pDC, p4.x, p4.y);
      rect.left = p4.x - 30;
      rect.right = p4.x + 30;
      rect.top = p4.y - 15;
      rect.bottom = p4.y + 15;
      _snprintf(str, 256, _T("%s\n%.1f%%"), pTmp->m_sLabel, (pTmp->m_dValue * 100.0) / m_dTotal);

      if (m_iCurSel == i) {
        SelectFontToMemDC1(m_boldFont);
        InflateRect(&rect, 2, 0);
        TDrawText(pDC, str, &rect, DT_CENTER | DT_VCENTER);
      }
      else {
        TDrawText(pDC, str, &rect, DT_CENTER | DT_VCENTER);
      }

      p4.x = (int)(cos(dMidAngle) * (r * 0.8) + p1.x);
      p4.y = (int)(p1.y - sin(dMidAngle) * (r * 0.8));

      if (_GetPixel(pDC, p4.x, p4.y) > 0x0) {
        //CBrush* tmpBr = pDC->SelectObject(m_BrushArray[(i%iColors)]);
        //pDC->FloodFill(p4.x,p4.y,0x0);
      }
    }
  }

  // Drawing the Title
  SelectFontToMemDC1(m_titleFont);
  TDrawText(dc, szTitle, &rect, DT_CENTER);
  return 0;
}
void DrawSymbol(HDDC pDC, RECT rect, DWORD clr, int b, int n)
{
  int _x0, _y0;
  DWORD _clr = clr;
  // Draw symbol [+]
  TMoveTo(pDC, rect.left, rect.top + 2);
  _LineTo(pDC, rect.left + 8, rect.top + 2);
  _LineTo(pDC, rect.left + 8, rect.top + 10);
  _LineTo(pDC, rect.left, rect.top + 10);
  _LineTo(pDC, rect.left, rect.top + 2);
  TMoveTo(pDC, rect.left + 2, rect.top + 6);
  _LineTo(pDC, rect.left + 7, rect.top + 6);

  if (b) {
    TMoveTo(pDC, rect.left + 4, rect.top + 10);
    _LineTo(pDC, rect.left + 4, rect.bottom);
  }
  else {
    TMoveTo(pDC, rect.left + 4, rect.top + 4);
    _LineTo(pDC, rect.left + 4, rect.top + 9);
  }

  rect.left += 12;

  // Draw dot lines
  if (1) {
    int k, countLines = 1;
    int nNumLines = 10;

    for (k = 0; k < n; k++)
      if (countLines > -nNumLines) {
        TMoveTo(pDC, rect.left + 4, rect.top);

        if (countLines - 1 == -nNumLines) {
          _LineTo(pDC, rect.left + 4, rect.top + 6);
          _LineTo(pDC, rect.left + 8, rect.top + 6);
        }
        else {
          _LineTo(pDC, rect.left + 4, rect.bottom);
        }
      }

    rect.left += 12;
  }

  return;
}

int test_DrawNSChart(HDDC hDC, int ch)
{
  RECT rc = {0, 0, 800, 500};
  CNSChartValue aa[] = {
    10, "One",
    20, "Two",
    40, "Three",
    30, "Four",
  };
  DrawNSChart(hDC, &rc, "test DrawNSChart", aa, 4, NSCS_BAR, SimpleColors);
  DrawSymbol(hDC, rc, _RGB(0, 0, 0), 1, 10);
  return 0;
}


