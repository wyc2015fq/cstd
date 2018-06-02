
//--------------------------------------------------------------------
void renderer_enlarged(scanline* sl, const spanv_t* sp, brush_t* br, double m_size, COLOR m_color)
{
  int i;
  uchar a = GetAV(m_color);
  for (i=0; i<sp->l; ++i) {
    span_t* span = sp->s + i;
    int x = span->b;
    int y = span->y;
    uchar cover = span->cover;
    int alpha = (cover * a) >> 8;
    COLOR clr = _RGB_A(m_color, alpha);
    brush_set_solid(br, clr);
    for (; x<span->e; ++x) {
      scanline_set_rect(sl, x*m_size, y*m_size, m_size, m_size);
    }
  }
}
int aa_demo(ctrl_t* c)
{
  SYSIO;SOFTGC_ARG;
  img_t* im = sg->im;
  static FPOINT mpt[] = { 57, 100, 369, 170, 143, 310, 143, 110};
  static double m_dx;
  static double m_dy;
  static int    m_idx = -1;
  static int inited=0;
  static float m_slider1_value = 32;
  static float m_slider2_value = 1.0;
  static ctrl_t cc[2] = {0};
  ctrl_t* m_slider1 = cc+0;
  ctrl_t* m_slider2 = cc+1;
  static ctrl_t m_polygon[1] = {0};
  static Path path[1] = {0};
  IRECT rc = c->rc, rcRight;
  int len = countof(mpt);
  
  iRectCutR(&rc, -500, 0, &rcRight);
  m_polygon->rc = rc = c->rc;
  flexboxlay(rcRight, countof(cc), cc, 2, flex_column|flex_column_reverse);
  slider_float_ctrl(m_slider1, "Pixel size=%1.0f", &m_slider1_value, 8.0, 100.0);
  slider_float_ctrl(m_slider2, "Gamma=%4.3f", &m_slider2_value, 0.1, 3.0);
  {
    spanv_t sp[1] = {0};
    int size_mul = (int)(m_slider1_value);
    //imclear(im, &rc, _rgb(1,1,1));
    //ras.gamma(agg::gamma_power(m_slider2_value));
    
    FPOINT pt[countof(mpt)];
    int i;
    for (i=0; i<countof(mpt); ++i) {
      pt[i].x = mpt[i].x/size_mul;
      pt[i].y = mpt[i].y/size_mul;
    }
    spanv_set_poly(sp, im, &rc, pt, &len, 1, false);
    renderer_enlarged(sg->sl, sp, sg->brush, size_mul, _RGBA(0,0,0, 255));
    brush_set_solid(sg->brush, _RGB(0,0,0));
    scanline_set_spanv(sg->sl, sp);
    //imdrawaa_poly_stroke_solid(im, flip_y, &rc, mpt, len, 1, _RGBA(0,150,160, 200), 1);
    spanv_free(sp);
  }
  polygon_ctrl(m_polygon, mpt, len, true, 10);
  if (0) {
    path_ctrl2(m_polygon, path, 0.1);
  }
  
#if 0
  if (ISHOVEREDWIN2()) {
    int hot;
    hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      {
        unsigned i;
        for (i = 0; i < 4; i++)
        {
          if(sqrt( (x-m_x[i]) * (x-m_x[i]) + (y-m_y[i]) * (y-m_y[i]) ) < 10.0)
          {
            m_dx = x - m_x[i];
            m_dy = y - m_y[i];
            m_idx = i;
            break;
          }
        }
        if(i == 4)
        {
          if(agg::point_in_triangle(m_x[0], m_y[0], 
            m_x[1], m_y[1],
            m_x[2], m_y[2],
            x, y))
          {
            m_dx = x - m_x[0];
            m_dy = y - m_y[0];
            m_idx = 30;
          }
        }
      }
      break;
    }
  }
  
  
#endif
  return 0;
}



