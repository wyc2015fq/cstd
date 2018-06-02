#define Min3(t1, t2, t3) ((t2 < t3) ? (t1 < t2 ? t1 : t2) : (t1 < t3 ? t1 : t3))
#define Max3(t1, t2, t3) ((t2 > t3) ? (t1 > t2 ? t1 : t2) : (t1 > t3 ? t1 : t3))
typedef struct HSL {
  double m_hue;
  double m_saturation;
  double m_lightness;
} HSL;
HSL SetHSL(double hue, double saturation, double lightness)
{
  HSL hsl;
  hsl.m_hue = (hue), hsl.m_saturation = (saturation), hsl.m_lightness = (lightness);
  ASSERT(hue >= 0. && hue < 360.);
  ASSERT(saturation >= 0. && saturation <= 1.);
  ASSERT(lightness >= 0. && lightness <= 1.);
  return hsl;
}
HSL rgb2HSL(COLORREF rgb)
{
  HSL hsl = {0, 0, 0};
  double red = GetRV(rgb) / 255.;
  double green = GetGV(rgb) / 255.;
  double blue = GetBV(rgb) / 255.;
  double rgbMin = Min3(red, green, blue);
  double rgbMax = Max3(red, green, blue);
  double chroma = rgbMax - rgbMin;
  hsl.m_lightness = (rgbMin + rgbMax) / 2.;

  // saturation and hue are equal to 0
  if (chroma == 0) {
    return hsl;
  }

  if (hsl.m_lightness <= 0.5) {
    hsl.m_saturation = chroma / 2. / hsl.m_lightness;
  }
  else {
    hsl.m_saturation = chroma / (2. - 2. * hsl.m_lightness);
  }

  // find dominant color
  if (rgbMax == red) {
    hsl.m_hue = 60. * (green - blue) / chroma;

    if (hsl.m_hue < 0) {
      hsl.m_hue += 360;
    }
  }
  else if (rgbMax == green) {
    hsl.m_hue = 120. + 60. * (blue - red) / chroma;
  }
  else { // rgbMax == blue
    hsl.m_hue = 240. + 60. * (red - green) / chroma;
  }

  return hsl;
}
COLORREF HSL2RGB(HSL hsl)
{
  int i, r, g, b;
  double q = (hsl.m_lightness < 0.5) ? hsl.m_lightness * (1. + hsl.m_saturation) : hsl.m_lightness + hsl.m_saturation - (hsl.m_lightness * hsl.m_saturation);
  double p = 2. * hsl.m_lightness - q;
  double h = hsl.m_hue / 360.;
  // t holds r, g, and b components
  double t[3] = {
    h + 1. / 3.,
    h,
    h - 1. / 3.
  };

  for (i = 0 ; i < 3 ; ++i) {
    if (t[i] < 0) {
      t[i] += 1.;
    }
    else if (t[i] > 1.) {
      t[i] -= 1.;
    }

    // calculate the color
    if (t[i] < 1. / 6.) {
      t[i] = p + ((q - p) * t[i] * 6.);
    }
    else if (t[i] < 0.5) {
      t[i] = q;
    }
    else if (t[i] < 2. / 3.) {
      t[i] = p + (q - p) * (4. - 6. * t[i]);
    }
    else {
      t[i] = p;
    }
  }

  ASSERT(t[0] >= 0 && t[0] <= 1.);
  r = (int)(t[0] * 255. + 0.5);
  ASSERT(t[1] >= 0 && t[1] <= 1.);
  g = (int)(t[1] * 255. + 0.5);
  ASSERT(t[2] >= 0 && t[2] <= 1.);
  b = (int)(t[2] * 255. + 0.5);
  return _RGB(r, g, b);
}

typedef struct CEvaluateColor {
  double m_hueMin;
  double m_hueMax;
  double m_displayedHueRange;
} CEvaluateColor;

#define HUE_RED 0
#define HUE_GREEN 120
#define HUE_RANGE_DEFAULT 35
int CEvaluateColor_Init(CEvaluateColor* ec)
{
  ec->m_hueMin = (HUE_RED);
  ec->m_hueMax = (HUE_GREEN);
  ec->m_displayedHueRange = (HUE_RANGE_DEFAULT);
  ASSERT(ec->m_hueMax - ec->m_hueMin > ec->m_displayedHueRange);
  return 0;
}
double GetHueStart(CEvaluateColor* ec, double value)
{
  return ec->m_hueMin + (ec->m_hueMax - ec->m_hueMin - ec->m_displayedHueRange) * value;
}
double GetHue(CEvaluateColor* ec, double value, int currentColumn, int totalColumns)
{
  return GetHueStart(ec, value) + ec->m_displayedHueRange * currentColumn / totalColumns;
}
COLORREF GetRgbCorrected(CEvaluateColor* ec, double value, int currentColumn, int totalColumns, COLORREF originalColor)
{
  double hue = GetHue(ec, value, currentColumn, totalColumns);
  HSL hsl = rgb2HSL(originalColor);
  hsl.m_hue = (hue);
  return HSL2RGB(hsl);
}
COLORREF GetRgb(CEvaluateColor* ec, double value, int currentColumn, int totalColumns)
{
  double hue = GetHue(ec, value, currentColumn, totalColumns);
  HSL hsl = {hue, 1., 0.5};
  return HSL2RGB(hsl);
}
typedef struct progress_ctrl {
  uictrl base;
  const char* m_label;
  double m_value;
  BOOL m_displayValue;
} progress_ctrl;
int progress_ctrl_event(progress_ctrl* s, const event* e)
{
  return FALSE;
}
int draw_fillrect_hslR(HDDC hDC, const RECT* rc, double m_value)
{
  int x, y, w, h;
  CEvaluateColor ec[1];
  w = RCW(rc), h = RCH(rc);
  CEvaluateColor_Init(ec);

  for (x = 0; x < w; ++x) {
    COLORREF color = GetRgb(ec, m_value, x, w);

    for (y = 0; y < h; ++y) {
      _SetPixel(hDC, rc->left + x, rc->top + y, color);
    }
  }

  return 0;
}
int draw_progress_ctrl(const progress_ctrl* s, const event* e)
{
  UIHDC(e);
  RECT rc = s->base.rc;
  RECT rect = rc;
  DWORD m_text_color = _RGB(0, 0, 0);
  draw_3drectR(hDC, &rect, 1);
  DeflateRect(&rect, 1, 1);
  draw_fillrect_hslR(hDC, &rect, s->m_value);
  {
    char buf[256];
    _snprintf(buf, 256, "%.f%%", 100 * s->m_value);
    draw_textR(hDC, buf, strlen(buf), &rect, DT_CENTER | DT_VCENTER, m_text_color);
  }
  return 0;
}
int progress_ctrl_set(progress_ctrl* s, const char* m_label, double m_value)
{
  UISETCALL(s, draw_progress_ctrl, progress_ctrl_event);
  s->m_label = m_label;
  s->m_value = m_value;
  return 0;
}

