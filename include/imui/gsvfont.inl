#include <string.h>
#include <stdio.h>
//#include "agg_gsv_text.h"
//#include "agg_bounding_rect.h"
int8u gsv_default_font[] = {
#include "gsv_default_font.txt"
};
int16u value(const int8u* p)
{
  int16u v;
  if (BIGENDIANNESS) {
    *(int8u*)&v = p[1];
    *((int8u*)&v + 1) = p[0];
  }
  else {
    *(int8u*)&v = p[0];
    *((int8u*)&v + 1) = p[1];
  }
  return v;
}
// bounding_rect
bool bounding_rect(DPOINT* vs, int num, double* x1, double* y1, double* x2, double* y2)
{
  int i=0;
  double x;
  double y;
  bool first = true;
  if (num > 0) {
    x = vs[i].x;
    y = vs[i].y;
    *x1 = (x);
    *y1 = (y);
    *x2 = (x);
    *y2 = (y);
    for (i = 1; i < num; i++) {
      x = vs[i].x;
      y = vs[i].y;
      if ((x) < *x1) {
        *x1 = (x);
      }
      if ((y) < *y1) {
        *y1 = (y);
      }
      if ((x) > *x2) {
        *x2 = (x);
      }
      if ((y) > *y2) {
        *y2 = (y);
      }
    }
    return *x1 <= *x2 && *y1 <= *y2;
  }
  return false;
}
int gsv_text(dpath_t* path, const char* text, IRECT rc, double m_height, double m_width, int fmt)
{
  double m_space;
  double m_line_space;
  const void* m_font;
  char* m_loaded_font;
  bool m_flip;
  int8u* m_indices;
  int8* m_glyphs;
  int8* m_bglyph;
  int8* m_eglyph;
  double m_w;
  double m_h;
  int textlen = strlen(text);
  m_space = (0.0);
  m_line_space = (0.0);
  m_font = (gsv_default_font);
  m_loaded_font = (NULL);
  m_flip = (false);
  m_flip = (true);
  m_width = m_width < 1 ? m_height * 0.5 : m_width;
  m_indices = (int8u*)m_font;
  double base_height = value(m_indices + 4);
  m_indices += value(m_indices);
  m_glyphs = (int8*)(m_indices + 257 * 2);
  m_h = m_height / base_height;
  m_w = (m_width == 0.0) ? m_h : m_width / base_height;
  iRectAlign(rc, m_width * textlen, m_height, fmt, &rc);
  {
    const char* m_cur_chr = text;
    unsigned idx;
    int8 yc, yf;
    int dx, dy;
    bool quit = false;
    double m_x = rc.l;
    double m_y = rc.t;
    double m_start_x = m_x;
    //m_x = 33; m_y = 15;
    if (m_flip) {
      m_y += m_height;
      m_h = -m_h;
    }
    for (; *m_cur_chr;) {
      idx = (*m_cur_chr++) & 0xFF;
      if (idx == '\n') {
        m_x = m_start_x;
        m_y -= m_flip ? -m_height - m_line_space : m_height + m_line_space;
      }
      idx <<= 1;
      m_bglyph = m_glyphs + value(m_indices + idx);
      m_eglyph = m_glyphs + value(m_indices + idx + 2);
      dpath_moveto(path, m_x, m_y);
      for (; m_bglyph < m_eglyph;) {
        dx = (*m_bglyph++);
        yf = (yc = *m_bglyph++) & 0x80;
        yc <<= 1;
        yc >>= 1;
        dy = int(yc);
        m_x += (dx) * m_w;
        m_y += (dy) * m_h;
        if (yf) {
          dpath_moveto(path, m_x, m_y);
        }
        else {
          dpath_lineto(path, m_x, m_y);
        }
      }
      m_x += m_space;
    }
  }
  return 0;
}

