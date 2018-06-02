
CC_INLINE double frand(double x)
{ 
  return ((((rand() << 15) | rand()) & 0x3FFFFFFF) % 1000000) * x / 1000000.0;
}

void draw_dash(scanline* sl, double x1, double y1, double x2, double y2, double line_width, double dash_length)
{
  FPOINT pt[] = {x1 + 0.5, y1 + 0.5, x2 + 0.5, y2 + 0.5};
  PenStyle penstyle[1] = {0};
  PenStyle_Init(penstyle);
  penstyle->lineWidth = line_width;
  
  if(dash_length > 0.0) {
    float m_dashes[] = {dash_length, dash_length};
    PenStyleSetLineDash(penstyle, 0, m_dashes, 2);
    scanline_set_poly_dash_stroke(sl, pt, 2, 0, penstyle);
  } else {
    scanline_set_poly_dash_stroke(sl, pt, 2, 0, penstyle);
  }
}


#if 0
// Calculate the affine transformation matrix for the linear gradient 
// from (x1, y1) to (x2, y2). gradient_d2 is the "base" to scale the
// gradient. Here d1 must be 0.0, and d2 must equal gradient_d2.
//---------------------------------------------------------------
void calc_linear_gradient_transform(double x1, double y1, double x2, double y2, 
                                    agg::trans_affine& mtx,
                                    double gradient_d2 = 100.0)
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  mtx.reset();
  mtx *= agg::trans_affine_scaling(sqrt(dx * dx + dy * dy) / gradient_d2);
  mtx *= agg::trans_affine_rotation(atan2(dy, dx));
  mtx *= agg::trans_affine_translation(x1 + 0.5, y1 + 0.5);
  mtx.invert();
}
#endif

int aa_test(ctrl_t* c)
{
  SYSIO;SOFTGC_ARG;
  static ctrl_t m_slider_gamma[1] = {0};
  IRECT rc = c->rc;
  static float m_slider_gamma_value = 1.6;
  iRectCutB(&rc, -20, 0, &m_slider_gamma->rc);
  slider_float_ctrl(m_slider_gamma, "Gamma=%4.3f", &m_slider_gamma_value, 0.1, 3.0);
  //aa_test_draw(im, rc, m_slider_gamma_value);
  {

  int i, width, height;
  double cx, cy;
  double x1, y1, x2, y2;
  sg->penstyle->lineWidth = 1.;
  
  gcClear(g, &rc, _rgb(0,0,0));
  
  width = RCW(&rc);
  height = RCH(&rc);
  // Reset AA Gamma and render the controls
  gamma_power(sg->m_gamma, m_slider_gamma_value);
  
  // gamma correction
  //ras.gamma(agg::gamma_power());
  //m_gamma.gamma(m_slider_gamma_value);
  
  
  // radial line test
  //-------------------------
  //dashed_line(ras, ren_sl, sl);
  
  cx = width / 2.0;
  cy = height / 2.0;
  
  brush_set_solid(sg->brush, _rgba(1.0, 1.0, 1.0, 0.2) );
  for(i = 180; i > 0; i--) {
    double n = 2.0 * M_PI * i / 180.0;
    draw_dash(sg->sl, cx + MIN(cx, cy) * sin(n), cy + MIN(cx, cy) * cos(n), cx, cy, 1.0, (i < 90) ? i : 0.0);
  }
  //dashed_line<rasterizer_type, renderer_gradient_type, scanline_type> dash_gradient(ras, ren_gradient, sl);
  for(i = 1; i <= 20; i++)
  {
    brush_set_solid(sg->brush, _rgb(1,1,1));
    // integral point sizes 1..20
    //----------------
    scanline_set_ellipse(sg->sl, 20 + i * (i + 1) + 0.5, 20.5, i / 2.0, i / 2.0);
    // fractional point sizes 0..2
    //----------------
    scanline_set_ellipse(sg->sl, 18 + i * 4 + 0.5, 33 + 0.5, i/20.0, i/20.0);
    
    // fractional point positioning
    //---------------
    scanline_set_ellipse(sg->sl, 18 + i * 4 + (i-1) / 10.0 + 0.5, 27 + (i - 1) / 10.0 + 0.5, 0.5, 0.5);
    
    // integral line widths 1..20
    //----------------
    x1 = 20 + i* (i + 1);
    y1 = 40.5;
    x2 = 20 + i * (i + 1) + (i - 1) * 4;
    y2 = 100.5;
    brush_set_gradient(sg->brush, x1, y1, x2, y2, _rgb(1,1,1), _rgb(i % 2, (i % 3) * 0.5, (i % 5) * 0.25));
    //calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx);
    draw_dash(sg->sl, x1, y1, x2, y2, i, 0);
    
    
    // fractional line lengths H (red/blue)
    //----------------
    x1 = 17.5 + i * 4;
    y1 = 107;
    x2 = 17.5 + i * 4 + i/6.66666667;
    y2 = 107;
    brush_set_gradient(sg->brush, x1, y1, x2, y2, _rgb(1,0,0), _rgb(0,0,1));
    //calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx);
    draw_dash(sg->sl, x1, y1, x2, y2, 1.0, 0);
    
    // fractional line lengths V (red/blue)
    //---------------
    x1 = 18 + i * 4;
    y1 = 112.5;
    x2 = 18 + i * 4;
    y2 = 112.5 + i / 6.66666667;
    //calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx);
    draw_dash(sg->sl, x1, y1, x2, y2, 1.0, 0);
    
    // fractional line positioning (red)
    //---------------
    x1 = 21.5;
    y1 = 120 + (i - 1) * 3.1;
    x2 = 52.5;
    y2 = 120 + (i - 1) * 3.1;
    //calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx);
    brush_set_gradient(sg->brush, x1, y1, x2, y2, _rgb(1,0,0), _rgb(1,1,1));
    draw_dash(sg->sl, x1, y1, x2, y2, 1.0, 0);
    
    // fractional line width 2..0 (green)
    x1 = 52.5;
    y1 = 118 + i * 3;
    x2 = 83.5;
    y2 = 118 + i * 3;
    //calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx);
    brush_set_gradient(sg->brush, x1, y1, x2, y2, _rgb(0,1,0), _rgb(1,1,1));
    draw_dash(sg->sl, x1, y1, x2, y2, 2.0 - (i - 1) / 10.0, 0);
    
    // stippled fractional width 2..0 (blue)
    x1 = 83.5;
    y1 = 119 + i * 3;
    x2 = 114.5;
    y2 = 119 + i * 3;
    //calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx);
    brush_set_gradient(sg->brush, x1, y1, x2, y2, _rgb(0,0,1), _rgb(1,1,1));
    draw_dash(sg->sl, x1, y1, x2, y2, 2.0 - (i - 1) / 10.0, 3.0);
    
    brush_set_solid(sg->brush, _rgb(1,1,1));
    if(i <= 10)
    {
      // integral line width, horz aligned (mipmap test)
      //-------------------
      draw_dash(sg->sl, 125.5, 119.5 + (i + 2) * (i / 2.0),
        135.5, 119.5 + (i + 2) * (i / 2.0),
        i, 0.0);
    }
    
    // fractional line width 0..2, 1 px H
    //-----------------
    draw_dash(sg->sl, 17.5 + i * 4, 192, 18.5 + i * 4, 192, i / 10.0, 0);
    
    // fractional line positioning, 1 px H
    //-----------------
    draw_dash(sg->sl, 17.5 + i * 4 + (i - 1) / 10.0, 186, 18.5 + i * 4 + (i - 1) / 10.0, 186,1.0, 0);
  }
  
  // Triangles
  //---------------
  for (i = 1; i <= 13; i++) 
  {
    FPOINT pt[] = {width - 150, height - 20 - i * (i + 1.5),
      width - 20,  height - 20 - i * (i + 1),
      width - 20,  height - 20 - i * (i + 2)};
    int len = countof(pt);
    //fill_color_array(gradient_colors, _rgba(1,1,1), _rgba(i % 2, (i % 3) * 0.5, (i % 5) * 0.25));
    x1 = width  - 150, y1 = height - 20 - i * (i + 1.5);
    x2 = width  - 20, y2 = height - 20 - i * (i + 1);
    //calc_linear_gradient_transform(width  - 150, height - 20 - i * (i + 1.5),width()  - 20, height() - 20 - i * (i + 1), gradient_mtx);
    brush_set_gradient(sg->brush, x1, y1, x2, y2, _rgb(1,1,1), _rgb(i % 2, (i % 3) * 0.5, (i % 5) * 0.25));
    scanline_set_poly(sg->sl, pt, &len, 1, false);
  }
  
  
  if (1) {
    
    int i;
    
    double w = width;
    double h = height;
    double x1, y1, x2, y2, x3, y3;
    double t1, t2, t3;
    enum {nn = 100};
    int n1=nn, n2=nn, n3=nn;
    COLOR clr1, clr2, clr3;
    
    srand(123);
    brush_set_solid(sg->brush, _rgb(0,0,0));
    {
      utime_start(_start_time);
      for(i = 0; i < n1; i++) {
        double r = frand(20.0) + 1.0;
        clr1=_rgba(frand(1.0), frand(1.0), frand(1.0), 0.5+frand(0.5));
        brush_set_solid(sg->brush, clr1);
        scanline_set_ellipse(sg->sl, frand(w), frand(h), r/2, r/2);
      }
      t1 = utime_elapsed(_start_time);
    }
    
    {
      utime_start(_start_time);
      for(i = 0; i < n2; i++)
      {
        x1 = frand(w); 
        y1 = frand(h);
        x2 = x1 + frand(w * 0.5) - w * 0.25;
        y2 = y1 + frand(h * 0.5) - h * 0.25;
        clr1 = _rgba(frand(1.0), frand(1.0), frand(1.0), 0.5+frand(0.5));
        clr2 = _rgba(frand(1.0), frand(1.0), frand(1.0), frand(1.0));
        
        brush_set_gradient(sg->brush, x1, y1, x2, y2, clr1, clr2);
        //calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx);
        draw_dash(sg->sl, x1, y1, x2, y2, 10.0, 0);
      }
      t2 = utime_elapsed(_start_time);
    }
    
    
    {
      utime_start(_start_time);
      for(i = 0; i < n3; i++)
      {
        x1 = frand(w); 
        y1 = frand(h);
        x2 = x1 + frand(w * 0.4) - w * 0.2;
        y2 = y1 + frand(h * 0.4) - h * 0.2;
        x3 = x1 + frand(w * 0.4) - w * 0.2;
        y3 = y1 + frand(h * 0.4) - h * 0.2;
        clr1 = _rgba(frand(1.0), frand(1.0), frand(1.0), 0.5+frand(0.5));
        clr2 = _rgba(frand(1.0), frand(1.0), frand(1.0), 0.5+frand(0.5));
        clr3 = _rgba(frand(1.0), frand(1.0), frand(1.0), 0.5+frand(0.5));
        
        brush_set_gouraud(sg->brush, x1, y1, x2, y2, x3, y3, clr1, clr2, clr3);
        scanline_set_triangle(sg->sl, x1, y1, x2, y2, x3, y3);
      }
      t3 = utime_elapsed(_start_time);
    }
    t1*=1000;
    t2*=1000;
    t3*=1000;
    printf("Points=%.2fK/sec, Lines=%.2fK/sec, Triangles=%.2fK/sec\n", n1/t1, n2/t2, n3/t3);
  }
  }
  return 0;
}
