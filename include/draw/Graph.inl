
void SetGraphType(int gType)
{
  graphType = gType;
}

int DrawGraph(HDDC hDC, RECT rc)
{
  int i;
  COLORREF _clr;
  RECT graphRect = rc;

  //reset graph to be clear background
  COLORREF backColor = _RGB(255, 255, 255); //replace with desired background color
  DrawRect(hDC, graphRect, backColor, 0);

  //draw graph title
  TSelectFont(titleFont);
  _clr = BLACK;
  draw_textR2(hDC, graphTitle, rcTitle, DT_VCENTER | DT_CENTER);

  //draw legend
  if (graphHasLegend) {
    DrawRect(hDC, rcLegend, 0, _clr);
    TSelectFont(legendTitleFont);
    _clr = BLACK;
    draw_textR2(hDC, legendTitle, rcLegendTitle, DT_CENTER | DT_VCENTER);

    TSelectFont(legendFont);

    for (i = 0; i < nlegend; i++) {
      draw_textR2(hDC, legend[i].label, legend[i].rcLabel, DT_VCENTER);
      //draw color bar
      DrawRect(hDC, legend[i].rcColor, legend[i].color, BLACK);
    }
  }

  DrawSeries(hDC);
  //draw seriesLabel datay and labels
  return 0;
}

// °Ñx·Ö½â³É10 ºÍ5µÄ´ÎÃÝ
double xto10_5(double x)
{
  int aa[] = {10, 5, 2};
  int bb[countof(aa)];
  double tk = x;
  int i;

  for (i = 0; i < countof(aa); ++i) {
    bb[i] = (int)(log(tk) / log(aa[i])) + (i == countof(aa));
    tk /= pow(aa[i], bb[i]);
  }

  tk = 1;

  for (i = 0; i < countof(aa); ++i) {
    tk *= pow(aa[i], bb[i]);
  }

  return tk;
}

int tickYStep;
SIZE tick;
int miny, maxy;
POINT origin;

void DrawAxis(HDDC hDC)
{
  RECT rc1 = rcAxis;
  SIZE szAxisXLabel;
  SIZE szAxisYLabel;
  SIZE szTickYLabel;
  RECT rcAxisXLabel;
  RECT rcAxisYLabel;
  RECT rcAxisX;
  RECT rcAxisY;
  RECT rcSeriesLabel;
  RECT rcTickYLabel;
  RECT rcContent;
  COLORREF _clr;
  char buf[32];
  int i;
  _snprintf(buf, 32, "%+d", maxy);
  DrawRect(hDC, rcAxis, _RGB(222, 222, 222), 0);
  TSelectFont(axisFont);
  szAxisXLabel = FontGetTextExtent(axisXLabel);
  szAxisYLabel = FontGetTextExtent(axisYLabel);
  szTickYLabel = FontGetTextExtent(buf);

  RectCutTop(&rc1, -szAxisXLabel.h - 10, 0, &rcAxisXLabel);
  RectCutTop(&rc1, -szAxisXLabel.h - 10, 0, &rcSeriesLabel);
  RectCutLeft(&rc1, szAxisYLabel.w + 10, 0, &rcAxisYLabel);
  RectCutLeft(&rc1, szTickYLabel.w + 10, 0, &rcTickYLabel);
  DrawRect(hDC, rcAxisXLabel, _RGB(222, 222, 0), 0);
  DrawRect(hDC, rcAxisYLabel, _RGB(0, 222, 0), 0);
  _clr = BLACK;
  draw_textR2(hDC, axisXLabel, rcAxisXLabel, DT_CENTER | DT_VCENTER);
  draw_textR2(hDC, axisYLabel, rcAxisYLabel, DT_CENTER | DT_VCENTER);

  RectIn(rc1, 10, &rcContent);

  int nTickY = RCH(&rcContent) / 40;
  double tk = xto10_5((maxy - miny) * 1. / nTickY);
  tickYStep = round(tk);
  nTickY = ((maxy - miny) / tickYStep);
  origin = cPOINT(rcContent.left, rcContent.top + RCH(&rcContent) * abs(maxy) / (abs(maxy) + abs(miny)));
  tick.w = RCW(&rcContent) / (nseries);
  tick.h = RCH(&rcContent) / (nTickY + 1);

  if (1) {
    int tklen = 3;
    rcAxisX = rc1, rcAxisX.top = origin.y - 1, rcAxisX.bottom = origin.y + 1;
    rcAxisY = rc1, rcAxisY.left = origin.x - 1, rcAxisY.right = origin.x + 1;
    DrawRect(hDC, rcAxisX, BLACK, 0);
    DrawRect(hDC, rcAxisY, BLACK, 0);
    TSelectFont(sideFont);
    DrawRect(hDC, rcContent, _RGBA(0, 122, 0, 20), 0);
    int tmin = miny / tickYStep;

    for (i = 0; i < nTickY + 1; ++i) {
      int pos = origin.y - (i + tmin) * tick.h;
      RECT rcTick = iRECT(origin.x - tklen, pos - 1, origin.x + tklen, pos);
      DrawRect(hDC, rcTick, BLACK, 0);
      _snprintf(buf, 32, "%d", (i + tmin)*tickYStep);
      SIZE szTickText = FontGetTextExtent(buf);
      szTickText = SZADD2(szTickText, 4, 4);
      RECT rcTickText = iRECT3(cPOINT(origin.x, pos - szTickText.h / 2), szTickText);
      rcTickText.left = rcTickYLabel.left, rcTickText.right = rcTickYLabel.right;
      draw_textR2(hDC, buf, rcTickText, DT_CENTER | DT_VCENTER);
    }

    for (i = 0; i < nseries; ++i) {
      int pos = origin.x + (2 * i + 1) * tick.w / 2;
      RECT rcTick = iRECT(pos, origin.y - tklen, pos + 1, origin.y + tklen);
      DrawRect(hDC, rcTick, BLACK, 0);
      SIZE szTickText = FontGetTextExtent(seriesLabel[i]);
      szTickText = SZADD2(szTickText, 4, 4);
      RECT rcTickText = iRECT3(cPOINT(pos - szTickText.w / 2, origin.y), szTickText);
      rcTickText.top = rcSeriesLabel.top, rcTickText.bottom = rcSeriesLabel.bottom;
      draw_textR2(hDC, seriesLabel[i], rcTickText, DT_CENTER | DT_VCENTER);
    }
  }

  return ;
}

void DrawSeries(HDDC hDC)
{
  int _x0, _y0;
  COLORREF _clr;

  switch (graphType) {
  case BAR_GRAPH :
    if (hDC) {
      int i, j;
      miny = 0, maxy = 0;

      for (i = 0; i < nlegend * nseries; ++i) {
        miny = MIN(miny, datay[i]);
        maxy = MAX(maxy, datay[i]);
      }

      DrawAxis(hDC);
      int barcx = tick.w / (nlegend + 1);

      for (i = 0; i < nseries; ++i) {
        int x = origin.x + (2 * i + 1) * tick.w / 2;
        int* dy = datay + i * nlegend;

        for (j = 0; j < nlegend; ++j) {
          SIZE szBar = iSIZE(barcx, tick.h * dy[j] / tickYStep);
          RECT rcBar = iRECT3(cPOINT(x - (barcx * nlegend) / 2 + j * barcx, origin.y - szBar.h), szBar);
          DrawRect(hDC, rcBar, legend[j].color, BLACK);
        }
      }
    }

    break;

  case LINE_GRAPH :
    if (hDC) {
      int i, j;
      miny = 0, maxy = 0;

      for (i = 0; i < nlegend * nseries; ++i) {
        miny = MIN(miny, datay[i]);
        maxy = MAX(maxy, datay[i]);
      }

      DrawAxis(hDC);

      for (j = 0; j < nlegend; ++j) {
        _clr = legend[j].color;

        for (i = 0; i < nseries; ++i) {
          int x = origin.x + (2 * i + 1) * tick.w / 2;
          int* dy = datay + i * nlegend;
          int y = origin.y - (tick.h * dy[j] / tickYStep);

          if (0) {
            if (0 == i) {
              TMoveTo(hDC, x, y);
            }
            else {
              _LineTo(hDC, x, y);
            }
          }
          else {
            if (i > 0) {
              drawaa_strokeline(hDC, _x0, _y0, x, y, _clr, 2);
            }

            TMoveTo(hDC, x, y);
          }
        }
      }

      for (j = 0; j < nlegend; ++j) {
        _clr = legend[j].color;

        for (i = 0; i < nseries; ++i) {
          RECT rcEllipse;
          int x = origin.x + (2 * i + 1) * tick.w / 2;
          int* dy = datay + i * nlegend;
          int y = origin.y - (tick.h * dy[j] / tickYStep);
          rcEllipse = iRECT(x, y, x, y);
          RectIn(rcEllipse, -4, &rcEllipse);
          TDrawEllipse(hDC, rcEllipse, _clr, BLACK);
        }
      }
    }

    break;

  case PIE_GRAPH :
    break;

  case SCATTER_GRAPH :
    break;

  case BOX_WHISKER_GRAPH :
    break;

  case STACKED_BAR_GRAPH :
    if (hDC) {
      int i, j;
      miny = 0, maxy = 0;

      for (i = 0; i < nseries; ++i) {
        int* dy = datay + i * nlegend;
        int d = 0;

        for (j = 0; j < nlegend; ++j) {
          d += abs(dy[j]);
        }

        miny = MIN(miny, d);
        maxy = MAX(maxy, d);
      }

      DrawAxis(hDC);
      int barcx = tick.w / 2;

      for (i = 0; i < nseries; ++i) {
        int x = origin.x + (2 * i + 1) * tick.w / 2;
        int* dy = datay + i * nlegend;
        int y = origin.y;

        for (j = 0; j < nlegend; ++j) {
          SIZE szBar = iSIZE(barcx, tick.h * abs(dy[j]) / tickYStep);
          RECT rcBar = iRECT3(cPOINT(x - (barcx) / 2, y - szBar.h), szBar);
          DrawRect(hDC, rcBar, legend[j].color, BLACK);
          y -= szBar.h;
        }
      }
    }

    break;

  case XY_LINE_GRAPH :
    break;

  case BAR_GRAPH_3D :
    if (hDC) {
      int i, j;
      miny = 0, maxy = 0;

      for (i = 0; i < nlegend * nseries; ++i) {
        miny = MIN(miny, datay[i]);
        maxy = MAX(maxy, datay[i]);
      }

      DrawAxis(hDC);
      int barcx = tick.w / (nlegend + 1);

      for (i = 0; i < nseries; ++i) {
        int x = origin.x + (2 * i + 1) * tick.w / 2;
        int* dy = datay + i * nlegend;

        for (j = 0; j < nlegend; ++j) {
          SIZE szBar = iSIZE(barcx, tick.h * dy[j] / tickYStep);
          RECT rcBar = iRECT3(cPOINT(x - (barcx * nlegend) / 2 + j * barcx, origin.y - szBar.h), szBar);
          DrawRect(hDC, rcBar, legend[j].color, BLACK);
        }
      }
    }

    break;

  case LINE_GRAPH_3D :
    break;

  case STACKED_BAR_GRAPH_3D :
    break;

  case PIE_GRAPH_3D :
    break;

  default :
    ASSERT(0 && "No graph type to display");
    break;
  }

  return ;
}

