#define CHART_TITLE_HEIGHT 20 // This reserves enough room for the title at the bottom of the chart. If you don't need a title, set it to zero
#define GRID_MARGIN_WIDTH 0 // Distance between Top & Right side of the control and the Grid box
#define GRID_DEFAULT_SIZE 10 // WARNING: Should not set to 0 - This is the size of the grid rectangles
#define BAR_DEFAULT_HEIGHT 100 // Default height for each bar. Almost neutral since it will be replaced by the height of the control
#define BAR_DEFAULT_WIDTH 24 // Default width of each bar, increase it to make bars fat
#define BAR_DEFAULT_GAP 4 // Distance between bars
#define BAR_REFLECT_AMOUNT 32 // Decrease to make WHITE shines more
#define BAR_TXT_PERCENT_HEIGHT 10 // Room for a Value/Percent at the top of each bar
#define BAR_TXT_LABEL_HEIGHT 10 // Room for the label at the buttom of each bar
#define BAR_TXT_VLABEL_MARGIN 30 // The right margin, room for a value at the right side and in front of each bar in % mode
#define TIP_BK_COLOR _RGB(255, 255, 220) // Background color for tooltip
#define TIP_FRAME_COLOR _RGB(240, 210, 80) // Frame color for tooltip
#define TIP_TITLE_COLOR _RGB(255, 100, 0) // Color of the tip's top most text
#define TIP_TEXT_COLOR _RGB(10, 100, 10) // Color of the tip's middle text
#define TIP_PERCENT_COLOR _RGB(150, 150, 150) // Color of the tip's bottom most text
#define TIP_LEFT_MARGIN 6 // Left margin for text in the tip
#define TIP_TOP_MARGIN 2 // Top margin for text in the tip
#define TIP_FRAME_THICKNESS 2 // Thickness of the frame line around the tooltip
#define TIP_TEXT_MAX_WIDTH 210 // Maximum width of the text within the tip
#define TIP_TEXT_MAX_HEIGHT 26 // Maximum height of the text within the tip
#define TIP_RGN_ROUND_CORNERS 10 // Increase to round tip corners more
#define TIP_SHADOW_SIZE 4 // NOT USED YET
#define TIP_SHADOW_COLOR _RGB(155, 155, 155) // NOT USED YET
#define TIP_TOP_OFFSET 20
#define TIP_RIGHT_OFFSET 10
#define TIP_DELAY_TIMER 1 // Don't change this, please. It does not have any influence.
#define CHART_BORDER_DARKNESS 100 // Don't change this, please.
#define GRID_FRAME_HEIGHT 2 // Don't change this, please.
#define GABS(NMBR) (((NMBR) > 0) ? (NMBR) : -(NMBR))
typedef struct _CChartData {
  double m_udValue;
  char* m_szLabel;
  COLORREF m_color;
} CChartData;
COLORREF DifColor(COLORREF color, int nDist)
{
  return _RGB(GABS(GetRV(color) - nDist), GABS(GetGV(color) - nDist), GABS(GetBV(color) - nDist));
}
COLORREF OrColor(COLORREF colFore, COLORREF colBK, uint nForePercent)
{
  return _RGB((GetRV(colBK) * 100 + (nForePercent * (GetRV(colFore) - GetRV(colBK)))) / 100, (GetGV(colBK) * 100 + (nForePercent * (GetGV(colFore) - GetGV(colBK)))) / 100, (GetBV(colBK) * 100 + (nForePercent * (GetBV(colFore) - GetBV(colBK)))) / 100);
}
int DrawGrid(HDDC hDC, LPRECT rcBound0, DWORD crGrid)
{
  int i;
  int _x0, _y0;
  RECT rcBound = *rcBound0;
  int nGapSize = GRID_DEFAULT_SIZE;

  if (nGapSize <= 0) {
    nGapSize = 1;
  }

  for (i = rcBound.left + GRID_MARGIN_WIDTH; i < rcBound.right - 2 * GRID_MARGIN_WIDTH; i += nGapSize) {
    TMoveTo(hDC, i, GRID_MARGIN_WIDTH);
    TLineTo(hDC, i, RCHEIGHT(&rcBound), crGrid);
  }

  nGapSize = GRID_DEFAULT_SIZE;

  if (nGapSize <= 0) {
    nGapSize = 1;
  }

  for (i = rcBound.bottom; i >= rcBound.top + GRID_MARGIN_WIDTH; i -= nGapSize) {
    TMoveTo(hDC, GRID_MARGIN_WIDTH, i);
    TLineTo(hDC, RCWIDTH(&rcBound) - 2 * GRID_MARGIN_WIDTH, i, crGrid);
  }

  return 0;
}
#define TTextOut3(hDC, x, y, text, clr) TTextOut2(hDC, x, y, text, strlen(text), clr)
#define TTextOut4(hDC, x, y, text, clr) TTextOut2(hDC, x, y, text, strlen(text), clr)
int TDrawBarChart(HDDC hDC, int m_chartDataCount, CChartData* m_chartData, const char* m_szTitle)
{
  COLORREF m_colorBK = _RGB(255, 255, 240);
  BOOL m_bShowGrid = TRUE, m_bAutoScale = FALSE, m_bShowTip = TRUE;
  BOOL m_bShowLabel = TRUE, m_bShowBarText = TRUE;
  RECT m_rctMemPic = {
    0, 0, hDC->w - 1, hDC->h - 1
  };
  int m_nBarTextType = 1;
  //TFillRect(hDC, m_rctMemPic, m_colorBK);

  // Draw gridlines
  if (m_bShowGrid) {
    DWORD clr = DifColor(m_colorBK, 50);
    m_rctMemPic.bottom -= (CHART_TITLE_HEIGHT + (m_bShowLabel * BAR_TXT_LABEL_HEIGHT) + 3);
    DrawGrid(hDC, &m_rctMemPic, clr);
    m_rctMemPic.bottom += (CHART_TITLE_HEIGHT + (m_bShowLabel * BAR_TXT_LABEL_HEIGHT) + 1);
  }

  // Draw bar chart
  {
    double nHeight = 0;
    int r1, g1, b1;
    int r2, g2, b2;
    int r, g, b;
    COLORREF colorG, colorEnd, colorSmooth, colorBorder;
    int x = 0, y = 0, i, j;
    RECT rcBound = m_rctMemPic;
    SIZE m_sBarPicSize = {
      RCWIDTH(&rcBound), RCHEIGHT(&rcBound)
    };
    int m_nBarWidth = BAR_DEFAULT_WIDTH;
    int m_nBarGap = BAR_DEFAULT_GAP;
    double m_udMax = 0;
    double m_rMargin;
    DWORD crText = _RGB(0, 0, 0);
    int cy = m_sBarPicSize.h;

    for (i = 0; i < m_chartDataCount; i++) {
      if (m_udMax < m_chartData[i].m_udValue) {
        m_udMax = m_chartData[i].m_udValue;
      }
    }

    m_udMax += BAR_TXT_PERCENT_HEIGHT + 20;
    m_sBarPicSize.w = m_chartDataCount * (m_nBarWidth + m_nBarGap);
    // If there is a title
    // if ()
    // {
    cy -= CHART_TITLE_HEIGHT;

    // }
    if (m_bShowBarText) {
      cy -= BAR_TXT_PERCENT_HEIGHT;
    }

    if (m_bShowLabel && 0) {
      RECT rc = iRECT(rcBound.left, rcBound.bottom - BAR_TXT_LABEL_HEIGHT - CHART_TITLE_HEIGHT - 1, rcBound.right, rcBound.bottom - CHART_TITLE_HEIGHT);
      cy -= BAR_TXT_LABEL_HEIGHT;
      TFillRect(hDC, rc, m_colorBK);
    }

    for (i = 0; i < m_chartDataCount; i++) {
      colorBorder = DifColor(m_chartData[i].m_color, CHART_BORDER_DARKNESS);
      colorEnd = DifColor(m_chartData[i].m_color, CHART_BORDER_DARKNESS - 14);
      colorSmooth = OrColor(colorBorder, m_colorBK, 40); // A correction color to smooth top 2 rounded edges of the border
      nHeight = (m_chartData[i].m_udValue * cy) / (m_udMax);
      x = i * (m_nBarWidth + m_nBarGap);
      y = m_sBarPicSize.h - (int)nHeight - (m_bShowLabel * BAR_TXT_LABEL_HEIGHT) - CHART_TITLE_HEIGHT;
      // Center bars
      m_rMargin = (double)(RCWIDTH(&rcBound) - m_sBarPicSize.w) / 2.0;
      x += (int)m_rMargin;
      {
        RECT rc = iRECT(x, y, x + m_nBarWidth, y + (int)nHeight);
        TFillRect(hDC, rc, colorBorder);
      }
      // Draw a gradient, thanks to Nishant.S
      r1 = GetRV(colorEnd);
      g1 = GetGV(colorEnd);
      b1 = GetBV(colorEnd);
      r2 = GetRV(m_chartData[i].m_color);
      g2 = GetGV(m_chartData[i].m_color);
      b2 = GetBV(m_chartData[i].m_color);
      // Don't corrupt border by over painting the gradient
      nHeight -= 2;

      // Draw gradient
      for (j = 1; j <= m_nBarWidth - 2; j++) {
        r = r1 + (j * (r2 - r1) / m_nBarWidth);
        g = g1 + (j * (g2 - g1) / m_nBarWidth);
        b = b1 + (j * (b2 - b1) / m_nBarWidth);

        if (j < m_nBarWidth / 2) {
          colorG = OrColor(_RGB(r, g, b), _RGB(255, 255, 255), (BAR_REFLECT_AMOUNT + 5 * j) > 100 ? 100 : (BAR_REFLECT_AMOUNT + 5 * j));
        }
        else {
          colorG = _RGB(r, g, b);
        }

        {
          RECT rc = iRECT(j + x, y + 1, j + x + 1, y + 1 + (int)nHeight);
          TFillRect(hDC, rc, colorG);
        }
      }

      // Return borders to usual
      nHeight += 2;
      // Do some Corrections
      _SetPixel(hDC, x, y, m_colorBK);
      _SetPixel(hDC, x + m_nBarWidth - 1, y, m_colorBK);
      // Smooth these pixels
      _SetPixel(hDC, x + 1, y, colorSmooth);
      _SetPixel(hDC, x, y + 1, colorSmooth);
      _SetPixel(hDC, x + m_nBarWidth - 2, y, colorSmooth);
      _SetPixel(hDC, x + m_nBarWidth - 1, y + 1, colorSmooth);
      // Repair these pixels
      _SetPixel(hDC, x + m_nBarWidth - 2, y + 1, colorBorder);
      _SetPixel(hDC, x + 1, y + 1, colorBorder);

      // Add a Percentage/Value at the top of each bar
      if (m_bShowBarText || m_bShowLabel) {
        if (m_bShowBarText) {
          char buf[256];

          // If we shall display percentage
          if (m_nBarTextType == 0) {
            _snprintf(buf, 256, "%0.01f%%", (m_chartData[i].m_udValue * 100) / m_udMax);
            //TTextOut3(hDC, x, y - BAR_TXT_PERCENT_HEIGHT, buf, crText);
          }
          else if (m_nBarTextType == 1) {
            // Draw value at the top of the bar
            _snprintf(buf, 256, "%0.02f", m_chartData[i].m_udValue);
            //TTextOut3(hDC, x, y - BAR_TXT_PERCENT_HEIGHT, buf, crText);
          }
        }

        if (m_bShowLabel) {
          //TTextOut3(hDC, x, y + (int)nHeight, m_chartData[i].m_szLabel, crText);
        }
      }
    }
  }
  // Draw the chart title
  //DrawTitle(hDC);
  // Draw a frame around the window
  //m_dcMem.FrameRect(&m_rctMemPic, &m_brFrame);
  {
    RECT rcTitle = {
      m_rctMemPic.left, m_rctMemPic.bottom - CHART_TITLE_HEIGHT, m_rctMemPic.right, m_rctMemPic.bottom
    };
    //TTextOut4(hDC, rcTitle.left + 2, rcTitle.top + 2, m_szTitle, _RGB(200, 200, 200));
    //TTextOut4(hDC, rcTitle.left, rcTitle.top, m_szTitle, _RGB(0, 0, 0));
  }
  return 0;
}
int test_TDrawBarChart(HDDC hDC, int ch)
{
  static int t = 0;
  CChartData m_chartData[] = {
    1600.356, "Jan", _RGB(255, 255, 0), 2841.5468, "Feb", _RGB(255, 0, 0), 1045.3258, "Mar", _RGB(100, 100, 200), 1502.215, "Apr", _RGB(0, 255, 0), 1467, "MAY", _RGB(255, 255, 255), 1678.354, "JUN", _RGB(200, 255, 255), 1785.689, "JUL", _RGB(255, 240, 40), 1283.099, "AUG", _RGB(255, 60, 130), 1554.879, "SEP", _RGB(255, 255, 200), 1400.10, "OCT", _RGB(130, 235, 250), 1600.556, "NOV", _RGB(100, 150, 200), 1900.3546, "DES", _RGB(150, 240, 80),
  };

  if (' ' == ch) {
    t = (t + 1) % 9;
  }

  if (1) {
    utime_start(_start_time);
    TDrawBarChart(hDC, countof(m_chartData), m_chartData, "A test chart, displaying some dummy data...");
    printf("%f ", utime_elapsed(_start_time));
  }

  return 0;
}

