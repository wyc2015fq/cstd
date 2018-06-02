
int vegen_arrowhead(int n, DPOINT* pt,
    int m_head_flag, double m_head_d1, double m_head_d2, double m_head_d3, double m_head_d4,
    int m_tail_flag, double m_tail_d1, double m_tail_d2, double m_tail_d3, double m_tail_d4)
{
  int i = 0;

  if (m_tail_flag) {
    pt[i++] = dPOINT(m_tail_d1,               0.0);
    pt[i++] = dPOINT(m_tail_d1 - m_tail_d4,   m_tail_d3);
    pt[i++] = dPOINT(-m_tail_d2 - m_tail_d4,   m_tail_d3);
    pt[i++] = dPOINT(-m_tail_d2,               0.0);
    pt[i++] = dPOINT(-m_tail_d2 - m_tail_d4,  -m_tail_d3);
    pt[i++] = dPOINT(m_tail_d1 - m_tail_d4,  -m_tail_d3);
  }

  if (m_tail_flag) {
    pt[i++] = dPOINT(-m_head_d1,             0.0);
    pt[i++] = dPOINT(m_head_d2 + m_head_d4,  -m_head_d3);
    pt[i++] = dPOINT(m_head_d2,              0.0);
    pt[i++] = dPOINT(m_head_d2 + m_head_d4,  m_head_d3);
  }

  return i;
}

int blend_hline(int h, int w, uchar* ptr, int step, int cn, int x1, int y, int x2, COLOR color, int8u cover)
{
  if ((y) >= 0 && (y) < h) {
    x1 = BOUND(x1, 0, w);
    x2 = BOUND(x2 + 1, 0, w);

    if (x2 > x1) {
      int aa = GetAV(color) * cover / 255;
      uchar* ptr_end;
      ptr = ptr + (y) * step + x1 * cn;
      ptr_end = ptr + (x2 - x1) * cn;
#if 0
      memcpy(ptr, &color, cn);

      for (ptr += cn; ptr != ptr_end; ++ptr) {
        *ptr = ptr[-cn];
      }

#else

      for (; ptr != ptr_end; ptr += cn) {
        PUT_PIXN(ptr, aa, color);
      }

#endif
    }
  }

  return 0;
}

int imdraw_solid_ellipse(int h, int w, uchar* ptr, int step, int cn, int x, int y, int rx, int ry, COLOR m_fill_color)
{
  int m_rx2 = (rx * rx), m_ry2 = (ry * ry),
      m_two_rx2 = (m_rx2 << 1), m_two_ry2 = (m_ry2 << 1),
      m_dx = (0), m_dy = (0), m_inc_x = (0),
      m_inc_y = (-ry * m_two_rx2), m_cur_f = (0);

  int dx = 0;
  int dy = -ry;
  int dy0 = dy;
  int dx0 = dx;

  do {
    dx += m_dx;
    dy += m_dy;

    if (dy != dy0) {
      blend_hline(h, w, ptr, step, cn, x - dx0, y + dy0, x + dx0, m_fill_color, cover_full);
      blend_hline(h, w, ptr, step, cn, x - dx0, y - dy0, x + dx0, m_fill_color, cover_full);
    }

    dx0 = dx;
    dy0 = dy;
#include "ellipse_bresenham_interpolator.inl"
  }
  while (dy < 0);

  blend_hline(h, w, ptr, step, cn, x - dx0, y + dy0, x + dx0, m_fill_color, cover_full);
  return 0;
}

int imdraw_outlined_ellipse(int h, int w, uchar* ptr, int step, int cn, int x, int y, int rx, int ry, COLOR m_fill_color, COLOR m_line_color)
{
  int m_rx2 = (rx * rx), m_ry2 = (ry * ry),
      m_two_rx2 = (m_rx2 << 1), m_two_ry2 = (m_ry2 << 1),
      m_dx = (0), m_dy = (0), m_inc_x = (0),
      m_inc_y = (-ry * m_two_rx2), m_cur_f = (0);

  int dx = 0;
  int dy = -ry;
  int m_line_color_aa = GetAV(m_line_color);

  do {
    dx += m_dx;
    dy += m_dy;
#define blend_pixel(x, y, clr, aa)  if ((x)>=0 && (x)<w && (y)>=0 && (y)<h) { uchar* xptr = ptr + (y)*step + (x)*cn; PUT_PIXN(xptr, m_line_color_aa, clr); }//memcpy(ptr + (y)*step + (x)*cn, &clr, cn);
    blend_pixel(x + dx, y + dy, m_line_color, cover_full);
    blend_pixel(x + dx, y - dy, m_line_color, cover_full);
    blend_pixel(x - dx, y - dy, m_line_color, cover_full);
    blend_pixel(x - dx, y + dy, m_line_color, cover_full);
#undef blend_pixel

    if (m_dy && dx) {
      blend_hline(h, w, ptr, step, cn, x - dx + 1, y + dy, x + dx - 1, m_fill_color, cover_full);

      if (dy) {
        blend_hline(h, w, ptr, step, cn, x - dx + 1, y - dy, x + dx - 1, m_fill_color, cover_full);
      }
    }

#include "ellipse_bresenham_interpolator.inl"
  }
  while (dy < 0);

  return 0;
}
