
int draw_hline(HDDC hDC, int x1, int y1, int x2, COLORREF clr)
{
  RECT rc;
  HDDCGET_CLIP(hDC, &rc);

  if (rc.top <= y1 && y1 < rc.bottom) {
    DWORD* ptr;
    int t;
    MINMAX(x1, x2, t);
    x1 = MAX(rc.left, x1);
    x2 = MIN(rc.right, x2);
    ptr = &_Pixel(hDC, x1, y1);

    for (; x1 < x2; ++x1, ++ptr) {
      //*ptr = clr;
      RGBBLENSET(*ptr, clr);
    }
  }

  return 0;
}
int draw_vline(HDDC hDC, int x1, int y1, int y2, COLORREF clr)
{
  RECT rc;
  HDDCGET_CLIP(hDC, &rc);

  if (rc.left <= x1 && x1 < rc.right) {
    DWORD* ptr;
    int t, al = hDC->bw >> 2;
    MINMAX(y1, y2, t);
    y1 = MAX(rc.top, y1);
    y2 = MIN(rc.bottom, y2);
    ptr = &_Pixel(hDC, x1, y1);

    for (; y1 < y2; ++y1, ptr += al) {
      //*ptr = clr;
      RGBBLENSET(*ptr, clr);
    }
  }

  return 0;
}
int draw_line(HDDC hDC, double x1, double y1, double x2, double y2, COLORREF clr)
{
  int i, steps, increx, increy, x, y, step = hDC->bw >> 2;
  DWORD* ptr = (DWORD*)(hDC->data);
  RECT rc;
  Rectd rcd[1];
  HDDCGET_CLIP(hDC, &rc);
  //x1=(int)x1, y1=(int)y1, x2=(int)x2, y2=(int)y2;
  SETRECTD(rcd, rc.left, rc.top, rc.right - 1, rc.bottom - 1);
  i = TClipLine(rcd, &x1, &y1, &x2, &y2);

  if (!i) {
    return 0;
  }

  steps = (int)(MAX(ABS(x2 - x1), ABS(y2 - y1)));

  if (0 == steps) {
    return 0;
  }

  ptr = &_Pixel(hDC, (int)x1, (int)y1);
  x = F2FIX(x1 - (int)(x1)), y = F2FIX(y1 - (int)(y1));
  increx = (F2FIX(x2 - x1)) / steps;
  increy = (F2FIX(y2 - y1)) / steps;

  for (i = 0; i < steps; ++i) {
    int ix = x >> 16, iy = y >> 16;
    //if ((1<<(i/factor))&pattern)
    //ASSERT(ix >= 0 && ix <= RCW(&rc));
    //ASSERT(iy >= 0 && iy <= RCH(&rc));
    ptr[ix + iy * step] = clr;
    x += increx, y += increy;
  }

  return 0;
}
int draw_line_aa(HDDC hDC, double x1, double y1, double x2, double y2, COLORREF clr)
{
  int i, steps, increx, increy, x, y, step = hDC->bw >> 2;
  DWORD* ptr = (DWORD*)(hDC->data);
  RECT rc;
  Rectd rcd[1];
  HDDCGET_CLIP(hDC, &rc);
  SETRECTD(rcd, rc.left, rc.top, rc.right - 1, rc.bottom - 1);
  //x1=(int)x1, y1=(int)y1, x2=(int)x2, y2=(int)y2;
  i = TClipLine(rcd, &x1, &y1, &x2, &y2);

  if (!i) {
    return 0;
  }

  steps = (int)(MAX(ABS(x2 - x1), ABS(y2 - y1)));

  if (0 == steps) {
    return 0;
  }

  ptr = &_Pixel(hDC, (int)x1, (int)y1);
  x = F2FIX(x1 - (int)(x1)), y = F2FIX(y1 - (int)(y1));
  increx = (F2FIX(x2 - x1)) / steps;
  increy = (F2FIX(y2 - y1)) / steps;

  if (x1 == x2 || y1 == y2) {
    for (i = 0; i <= steps; ++i) {
      int ix = x >> 16, iy = y >> 16;
      //if ((1<<(i/factor))&pattern)
      ptr[ix + iy * step] = clr;
      x += increx, y += increy;
    }
  }
  else {
    // 反走样
    int off = (ABS(x2 - x1) > ABS(y2 - y1)) ? step : 1;

    for (i = 0; i <= steps; ++i) {
      int ix = x >> 16, iy = y >> 16;
      int d0 = (off == 1 ? x - I2FIX(ix) : y - I2FIX(iy));
      DWORD bg = ptr[ix + iy * step], bg1 = ptr[ix + iy * step + off];
      ptr[ix + iy * step] = GRADIENTRGB2(16, d0, bg, clr);
      ptr[ix + iy * step + off] = GRADIENTRGB2(16, d0, clr, bg1);
      x += increx, y += increy;
    }
  }

  return 0;
}
int draw_line_dot(HDDC hDC, double x1, double y1, double x2, double y2, COLORREF clr)
{
  int i, off, steps, increx, increy, x, y, step = hDC->bw >> 2;
  DWORD* ptr = (DWORD*)(hDC->data);
  //x1=(int)x1, y1=(int)y1, x2=(int)x2, y2=(int)y2;
  Rectd rcd[1];
  SETRECTD(rcd, 0, 0, hDC->w - 1, hDC->h - 1);
  i = TClipLine(rcd, &x1, &y1, &x2, &y2);

  if (!i) {
    return 0;
  }

  steps = (int)(MAX(ABS(x2 - x1), ABS(y2 - y1)));

  if (0 == steps) {
    return 0;
  }

  ptr = &_Pixel(hDC, (int)x1, (int)y1);
  off = (int)x1 + (int)y1;
  x = F2FIX(x1 - (int)(x1)), y = F2FIX(y1 - (int)(y1));
  increx = (F2FIX(x2 - x1)) / steps;
  increy = (F2FIX(y2 - y1)) / steps;

  if (x1 == x2 || y1 == y2) {
    for (i = 0; i <= steps; ++i) {
      int ix = x >> 16, iy = y >> 16;

      //if ((1<<(i/factor))&pattern)
      if ((off + ix + iy) & 1) {
        ptr[ix + iy * step] = clr;
      }

      x += increx, y += increy;
    }
  }
  else {
    // 反走样
    int off = (ABS(x2 - x1) > ABS(y2 - y1)) ? step : 1;

    for (i = 0; i <= steps; ++i) {
      int ix = x >> 16, iy = y >> 16;
      int d0 = (off == 1 ? x - I2FIX(ix) : y - I2FIX(iy));
      DWORD bg = ptr[ix + iy * step], bg1 = ptr[ix + iy * step + off];
      ptr[ix + iy * step] = GRADIENTRGB2(16, d0, bg, clr);
      ptr[ix + iy * step + off] = GRADIENTRGB2(16, d0, clr, bg1);
      x += increx, y += increy;
    }
  }

  return 0;
}
// 多边形描边
int draw_lines(HDDC hDC, const DPOINT* pt, int len, COLORREF clr, int isclose)
{
  int i;

  for (i = 0; i < len - 1; i++) {
    draw_line(hDC, pt[i].x, pt[i].y, pt[i + 1].x, pt[i + 1].y, clr);
  }

  if (isclose) {
    draw_line(hDC, pt[len - 1].x, pt[len - 1].y, pt[0].x, pt[0].y, clr);
  }

  return 0;
}
int TDrawDashLine2(HDDC hDC, double x1, double y1, double x2, double y2, COLORREF clr, double* dash_start, double* m_dashes, int m_num_dashes)
{
  int i, m_curr_dash = 0;
  double ds = *dash_start, d, x = x1, y = y1;
  double dis = calc_distance(x1, y1, x2, y2);
  double dx = (x2 - x1) / dis;
  double dy = (y2 - y1) / dis;
  double m_curr_dash_start = 0;
  i = 0;

  for (; ds > m_dashes[i];) {
    ds -= m_dashes[i], ++i;

    if (i >= m_num_dashes) {
      i = 0;
    }
  }

  d = m_dashes[i] - ds;

  for (; d < dis;) {
    double x0 = x1 + dx * d, y0 = y1 + dy * d;

    if (i % 2 == 0) {
      //double dis=calc_distance(x, y, x0, y0);
      //TDrawLine(hDC, x, y, x0, y0, clr);
    }

    ++i;

    if (i >= m_num_dashes) {
      i = 0;
    }

    x = x0, y = y0;
    d += m_dashes[i];
  }

  return 0;
}