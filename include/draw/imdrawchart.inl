
static COLOR colortable[] = {
#define COLORTABLEDEF(a, b)  b,
#include "img/colortable.txt"
#undef COLORTABLEDEF
};
// imdraw_text

double xto10_5(double tk)
{
  int aa[] = {10, 5, 2};
  int bb[countof(aa)];
  double precision = 1;
  int i;
  tk = fabs(tk);

  if (tk > 0) {
    while (tk < 100) {
      tk *= 10;
      precision /= 10;
    }
  }

  for (i = 0; i < countof(aa); ++i) {
    double t1 = log(tk);
    double t2 = log(aa[i]);
    bb[i] = (int)(t1 / t2);
    tk /= pow(aa[i], bb[i]);
  }

  tk = 1;

  for (i = 0; i < countof(aa); ++i) {
    tk *= pow(aa[i], bb[i]);
  }

  return tk * precision;
}

#define DCF_XTICKLABEL   (1)
#define DCF_YTICKLABEL   (1<<1)

COLOR color_mul(COLOR clr, double x)
{
  double t = x / 255.;
  double r = GetRV(clr) * t;
  double g = GetGV(clr) * t;
  double b = GetBV(clr) * t;
  double a = GetAV(clr) / 255.;
  r = BOUND(r, 0., 1);
  g = BOUND(g, 0., 1);
  b = BOUND(b, 0., 1);
  return _rgba(r, g, b, a);
}
COLOR color_dark(COLOR clr)
{
  double t1 = GetRV(clr) + GetGV(clr) + GetBV(clr);
  t1 /= 255 * 3;

  if (t1 > 0.5) {
    clr = color_mul(clr, 0.5);
  }

  return clr;
}
int imdrawchart_axis_tick(img_t* im, const IRECT* pclip, IRECT rc, axis_info_t* g, int isx, const font_t* fo, int is_draw_tick_label)
{
  int i, len;
  char buf[32];
  IRECT clip2 = iRectInter(rc, iRectClip(pclip, 0, 0, im->w, im->h));

  if (isx) {
    g->t = rc.r, g->b = rc.l;
  }
  else {
    g->t = rc.t, g->b = rc.b;
  }

  g->tklen = 3;
  len = g->b - g->t;
  g->nTick = len / 100;

  if (g->vMax > 0) {
    if (g->vMin < 0) {
      g->origin = g->t + (int)(len * g->vMax / (g->vMax - g->vMin));
      g->vTickStep = xto10_5((g->vMax - g->vMin) * 1. / g->nTick);
      g->nTick = (int)((g->vMax - g->vMin) / g->vTickStep);
      g->tmin = (int)(g->vMin / g->vTickStep);
    }
    else {
      g->origin = g->b;
      g->vTickStep = xto10_5((g->vMax) * 1. / g->nTick);
      g->nTick = (int)(g->vMax / (g->vTickStep));
      g->tmin = 0;
    }
  }
  else {
    g->origin = g->t;
    g->vTickStep = xto10_5((-g->vMin) * 1. / g->nTick);
    g->nTick = (int)(-g->vMin / g->vTickStep);
    g->tmin = (int)(g->vMin / g->vTickStep);
  }

  g->szTick = len / (g->nTick + 1);

  for (i = -1; i < g->nTick + 1; ++i) {
    int pos;
    IRECT rcTick0, rcTick1, rcTickText;
    ISIZE szTickText;
    _snprintf(buf, 32, "%g", (i + g->tmin)*g->vTickStep);
    szTickText = font_text_size(fo, buf, -1, 0);
    szTickText = SZADD2(szTickText, 4, 4);
    pos = g->origin - (i + g->tmin) * g->szTick;

    if (isx) {
      rcTick0 = iRECT(pos - 1, rc.b - g->tklen, pos, rc.b);
      rcTick1 = iRECT(pos - 1, rc.t, pos, rc.t + g->tklen);
      rcTickText = iRECT(pos - szTickText.w / 2, g->rcTickLabel.t, pos + szTickText.w / 2, g->rcTickLabel.b);
    }
    else {
      rcTick0 = iRECT(rc.l, pos - 1, rc.l + g->tklen, pos);
      rcTick1 = iRECT(rc.r - g->tklen, pos - 1, rc.r, pos);
      rcTickText = iRECT(g->rcTickLabel.l, pos - szTickText.h / 2, g->rcTickLabel.r, pos + szTickText.h / 2);
    }

    imdraw_rect(im, &clip2, rcTick0, Color_Black, 0, 0, 0);
    imdraw_rect(im, &clip2, rcTick1, Color_Black, 0, 0, 0);

    if (is_draw_tick_label) {
      imdraw_text(im, pclip, rcTickText, buf, -1, fo, Color_Black, 0, DT_CENTER | DT_VCENTER);
    }
  }

  if (g->sLabel) {
    int i;
    str_t s, ss;
    s = STR1(g->sLabel);

    for (i = 0; i < g->row; ++i) {
      int pos = g->origin + (2 * i + 1) * g->szTick / 2;
      ISIZE szTickText;
      IRECT rcTickText;
      ss = STRsplit(s, "|", " ", &s);
      szTickText = font_text_size(fo, ss.s, ss.l, 0);
      szTickText = SZADD2(szTickText, 4, 4);

      if (isx) {
        rcTickText = iRECT(pos - szTickText.w / 2, g->rcLabel.t, pos + szTickText.w / 2, g->rcLabel.b);
      }
      else {
        rcTickText = iRECT(g->rcLabel.l, pos - szTickText.h / 2, g->rcLabel.r, pos + szTickText.h / 2);
      }

      imdraw_text(im, pclip, rcTickText, ss.s, ss.l, fo, Color_Black, 0, DT_CENTER | DT_VCENTER);
    }
  }

  return 0;
}

int imdrawchart_axis(img_t* im, const IRECT* pclip, IRECT rc, graph_info_t* g, const font_t* fo, int flag)
{
  IRECT rcAxis = rc;
  IRECT rc1 = rc;
  ISIZE sztmp, szTickLabel = {0};
  double tk = 0;
  str_t s, ss;
  char buf[32];

  tk = xto10_5(g->y.vMax);
  _snprintf(buf, 32, "++%g", tk);
  szTickLabel = font_text_size(fo, buf, -1, 0);

  imdraw_rect(im, pclip, rcAxis, CLR_3DFACE, 0, 0, 0);

  if (g->x.sAxisLabel) {
    sztmp = font_text_size(fo, g->x.sAxisLabel, -1, 0);
    iRectCutT(&rc1, -sztmp.h - 4, 0, &g->x.rcAxisLabel);
    imdraw_text(im, pclip, g->x.rcAxisLabel, g->x.sAxisLabel, -1, fo, Color_Black, 0, DT_CENTER | DT_VCENTER);
  }

  if (g->x.sLabel) {
    sztmp = font_text_size(fo, "0", -1, 0);
    iRectCutT(&rc1, -sztmp.h - 4, 0, &g->x.rcLabel);
  }

  if (flag & DCF_XTICKLABEL) {
    iRectCutT(&rc1, -szTickLabel.h - 4, 0, &g->x.rcTickLabel);
  }

  if (g->y.sAxisLabel) {
    sztmp = font_text_size(fo, g->y.sAxisLabel, -1, DT_R270);
    iRectCutL(&rc1, sztmp.w + 4, 0, &g->y.rcAxisLabel);
    imdraw_text(im, pclip, g->y.rcAxisLabel, g->y.sAxisLabel, -1, fo, Color_Black, 0, DT_CENTER | DT_VCENTER | DT_R270);
  }

  if (g->y.sLabel) {
    s = STR1(g->y.sLabel);
    ss = STRsplit(s, "|", " ", &s);
    sztmp = font_text_size(fo, ss.s, ss.l, 0);
    iRectCutL(&rc1, sztmp.w + 4, 0, &g->y.rcLabel);
  }

  if (flag & DCF_YTICKLABEL) {
    iRectCutL(&rc1, szTickLabel.w + 4, 0, &g->y.rcTickLabel);
  }

  //imdraw_rect(im, pclip, g->rcXAxisLabel, _RGB(222, 222, 0), 0, 0, 0);
  //imdraw_rect(im, pclip, g->rcYAxisLabel, _RGB(0, 222, 0), 0, 0, 0);

  g->rcContent = rc1;
  iRectDeflate(&g->rcContent, 0, 10, 10, 0);
  imdraw_rect(im, pclip, g->rcContent, Color_White, Color_Black, 1, 0);

  //g->szTick.w = RCW(&g->rcContent) / (g->x.row);
  //g->szTick.h = RCH(&g->rcContent) / (g->x.col);
  g->x.origin = g->rcContent.l;
  g->y.origin = g->rcContent.b;

  imdrawchart_axis_tick(im, pclip, g->rcContent, &g->y, 0, fo, flag & DCF_YTICKLABEL);
  imdrawchart_axis_tick(im, pclip, g->rcContent, &g->x, 1, fo, flag & DCF_XTICKLABEL);
  return 0;
}

//#define get_x(_x) (g->x.origin - g->x.szTick * _x / g->x.vTickStep)
//#define get_y(_y) (g->y.origin - g->y.szTick * _y / g->y.vTickStep)

double di_get(const axis_info_t* g, int x, int y)
{
  double _x = g->data[y * g->step + x];
  return g->origin - g->szTick * _x / g->vTickStep;
}
int axis_set(axis_info_t* g, int h, int w, const double* data, int step)
{
  g->row = h;
  g->col = w;
  g->step = step;
  g->data = data;
  return 0;
}

int cacl_axis_minmax(axis_info_t* g, int h, int w, const double* data, int step)
{
  int i, j;
  axis_set(g, h, w, data, step);

  if (data) {
    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        g->vMin = MIN(g->vMin, data[i * step + j]);
        g->vMax = MAX(g->vMax, data[i * step + j]);
      }
    }
  }

  return 0;
}

int imdrawchart_bar(img_t* im, const IRECT* pclip, IRECT rc, int row, int col, const double* data, int step, const char* xAxisLabel, const char* yAxisLabel, const char* xLabel, const char* yLabel, const COLOR* xColor, const font_t* fo)
{
  int i, j;
  int barcx;
  graph_info_t g[1] = {0};
  xColor = xColor ? xColor : colortable;

  if (row * col <= 0 || NULL == data) {
    return 0;
  }

  g->x.sAxisLabel = xAxisLabel;
  g->y.sAxisLabel = yAxisLabel;
  g->x.sLabel = xLabel;
  g->y.sLabel = yLabel;

  cacl_axis_minmax(&g->y, row, col, data, step);
  imdrawchart_axis(im, pclip, rc, g, fo, DCF_YTICKLABEL);
  barcx = g->x.szTick / (col + 1);

  for (i = 0; i < row; ++i) {
    int x = g->x.origin + (2 * i + 1) * g->x.szTick / 2;
    const double* dy = data + i * step;

    for (j = 0; j < col; ++j) {
      int barcy = (int)(g->y.szTick * dy[j] / g->y.vTickStep);
      IRECT rcBar = iRECT2(x - (barcx * col) / 2 + j * barcx, g->y.origin - barcy, barcx, barcy);
      iRectNormalize(&rcBar);
      imdraw_rect(im, pclip, rcBar, xColor[j], Color_Black, 1, BDM_ALL);
    }
  }

  return 0;
}

int imdrawchart_pie(img_t* im, const IRECT* pclip, IRECT rc, int row, int col, const double* data, int step, const char* xAxisLabel, const char* yAxisLabel, const char* xLabel, const char* yLabel, const COLOR* xColor, const font_t* fo)
{
  double dataSum = 0.00;  //for storing cumulative sum
  double percent = 0.00;
  double labelPercent = 0.00;
  double totalSum = 0.00;
  int radius;
  int seriesSpace;
  int nSeries = row;
  int seriesSize = col;
  int centerYPie, x, s;
  str_t xs, xss;
  xColor = xColor ? xColor : colortable;

  //pie labels will be stored in a list and drawn after entire pie
  //is completed.

  //determine width of pie display area
  seriesSpace = RCW(&rc) / nSeries;
  radius = MIN(seriesSpace / 3, RCH(&rc) / 3);
  centerYPie = RCY(&rc) / 2;

  xs = STR1(xLabel);

  for (x = 0; x < nSeries; x++) {
    int pieLeft, pieRight;
    int centerXPie;
    IRECT pieRect, rcLabel;
    const double* tmpSeries = data + x * step;
    totalSum = 0;

    for (s = 0; s < seriesSize; s++) {
      totalSum += tmpSeries[s];
    }

    pieLeft = rc.l + ((x * 2 + 1) * (seriesSpace / 2)) - radius;
    pieRight = pieLeft + (2 * radius);
    pieRect = iRECT(pieLeft, rc.t, pieRight, rc.b);
    centerXPie = pieLeft + radius;
    xss = STRsplit(xs, "|", " ", &xs);
    iRectCutT(&pieRect, -20, 0, &rcLabel);

    //plot series label
    dataSum = 0;

    for (s = 0; s < seriesSize; s++) {
      if (tmpSeries[s] > 0) {
        char buf[32];
        IPOINT ptText;
        ISIZE szText;
        double percent, pre_percent, degrees, pre_degrees;
        pre_percent = (dataSum / totalSum) * 100;
        pre_degrees = ((360 * pre_percent) / 100);

        dataSum += tmpSeries[s];
        percent = (dataSum / totalSum) * 100;
        degrees = ((360 * percent) / 100);

        imdrawaa_pie(im, pclip, centerXPie, centerYPie, radius, radius, pre_degrees, degrees, xColor[s], 0, 0.5);
        _snprintf(buf, 32, "%d%%", (int)(percent - pre_percent));
        szText = font_text_size(fo, buf, -1, 0);
        ptText.x = centerXPie + (int)(cos((pre_degrees + degrees) * 0.5 * PI / 180) * (radius + szText.w));
        ptText.y = centerYPie + (int)(sin((pre_degrees + degrees) * 0.5 * PI / 180) * (radius + szText.w));
        imdraw_text_line_str(im, pclip, rc, ptText.x - szText.w / 2, ptText.y - szText.h / 2, buf, -1, 0, fo, Color_Black, 0);
      }

      imdraw_text(im, pclip, rcLabel, xss.s, xss.l, fo, Color_Black, 0, DT_VCENTER | DT_CENTER);
    }

    //draw lines and labels for pies slices
  }

  return 0;
}

int imdrawchart_line(img_t* im, const IRECT* pclip, IRECT rc, const data_info_t* di, int ndi, const char* xAxisLabel, const char* yAxisLabel, const COLOR* xColor, const font_t* fo)
{
  int i, j, k;
  IRECT clip2;
  graph_info_t g[1] = {0};
  const data_info_t* di0 = di;
  xColor = xColor ? xColor : colortable;

  for (k = 0; k < ndi; ++k) {
    di = di0 + k;

    if (di->h * di->w <= 0) {
      continue;
    }

    g->x.sAxisLabel = xAxisLabel;
    g->y.sAxisLabel = yAxisLabel;
    g->x.sLabel = NULL;
    g->y.sLabel = NULL;
    cacl_axis_minmax(&g->x, di->h, di->w, di->x, di->xl);
    cacl_axis_minmax(&g->y, di->h, di->w, di->y, di->yl);
  }

  imdrawchart_axis(im, pclip, rc, g, fo, DCF_XTICKLABEL | DCF_YTICKLABEL);
  clip2 = iRectInter(g->rcContent, iRectClip(pclip, 0, 0, im->w, im->h));

  for (k = 0; k < ndi; ++k) {
    double x, y, x1, y1;
    di = di0 + k;
    axis_set(&g->x, di->h, di->w, di->x, di->xl);
    axis_set(&g->y, di->h, di->w, di->y, di->yl);

    for (i = 0; i < di->h; ++i) {
      COLOR clr = color_dark(xColor[i]);
      x = di_get(&g->x, 0, i);
      y = di_get(&g->y, 0, i);

      for (j = 1; j < di->w; ++j) {
        x1 = di_get(&g->x, j, i);
        y1 = di_get(&g->y, j, i);
        imdrawaa_line(im, &clip2, x, y, x1, y1, clr, 1);
        x = x1, y = y1;
      }
    }
  }

  return 0;
}

// 爱的方程式
int fun_love(DPOINT* pt, int i, int n)
{
  return 0;
}

int test_imdrawchart(img_t* im, int testid, const font_t* fo)
{
  IRECT rc = {0, 0, im->w, im->h};
  COLOR xColor[100] = {0};

  if (0) {
    double data[] = {
      150, 202, 230,
      199, 140, 279,
      204, 221, 208,
      204, 221, 208,
    };
    int i;

    for (i = 0; i < countof(data); ++i) {
      data[i] *= 0.01;
    }

    imdrawchart_bar(im, 0, rc, 4, 3, data, 3, "Games", "Scores", "day 1|day 2|day 3|day 4", 0 ? 0 : "game 1|game 2|game 3", NULL, fo);
  }

  if (0) {
    double data[] = {15, 5, 2, 8, 30, 6, 7, 9, 8, 10};
    const char* lab = "game 1|game 2|game 3|game 4|game 5|game 6|game 7|game 8|game 9|game 10";
    int m = 2;
    int n = countof(data) / m;
    imdrawchart_pie(im, 0, rc, m, n, data, n, "Games", "Scores", lab, lab, NULL, fo);
  }

  if (1) {
    enum {M = 2, N = 1000};
    double xdata[M * N];
    double ydata[M * N];
    data_info_t di[1] = {0};
    int i, j;

    for (i = 0; i < M; ++i) {
      for (j = 0; j < N; ++j) {
        double x = j * 0.2 + i;
        xdata[i * N + j] = x;
        ydata[i * N + j] = sin(x) + 0.01 * j * i;
      }

      xColor[i] = hsv2rgb(i * 180.f / M);
    }

    di->h = M, di->w = N, di->xl = N, di->yl = N, di->x = xdata, di->y = ydata;
    imdrawchart_line(im, 0, rc, di, 1, NULL, NULL, xColor, fo);
  }

  return 0;
}
