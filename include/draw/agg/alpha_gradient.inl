



// A simple function to form the gradient color array 
// consisting of 3 colors, "begin", "middle", "end"
//---------------------------------------------------
static void fill_color_array_3(COLOR* array, COLOR begin, COLOR middle, COLOR end)
{
  unsigned i;
  for(i = 0; i < 128; ++i)
  {
    array[i] = RGBABLEN2(begin, middle, i, 128);
  }
  for(; i < 256; ++i)
  {
    array[i] = RGBABLEN2(middle, end, i - 128, 128);
  }
}



int alpha_gradient(ctrl_t* c)
{
  SYSIO;SOFTGC_ARG;
  double m_x[3];
  double m_y[3];
  int    m_idx = -1;
  static double mxp[] = {0.0,    1.0/5.0,2.0/5.0,3.0/5.0,4.0/5.0,1.0,};
  static double myp[] = {0.0,1.0 - 4.0/5.0,1.0 - 3.0/5.0,1.0 - 2.0/5.0,1.0 - 1.0/5.0,1.0,};
  static double m_alpha_value[256] = {0};
  int num = countof(myp);
  ctrl_t m_alpha[1] = {0};
  int changed = 0;
  IRECT rc = c->rc;
  
  
  m_x[0] = 257;    m_y[0] = 60;
  m_x[1] = 369;   m_y[1] = 170;
  m_x[2] = 143;   m_y[2] = 310;
  
  m_alpha->rc = iRECT(0, 0, 200, 200);
  
  if (spline_ctrl(m_alpha, num, mxp, myp, countof(m_alpha_value), m_alpha_value)) {
    //m_alpha.update_spline();
  }
  
  //imclear(_rgba(1,1,1));
  {
    // Draw some background
    unsigned i;
    int width = RCW(&rc);
    int height = RCH(&rc);
    unsigned w = width;
    unsigned h = height;
    double parallelogram[6];
    COLOR color_array[256];
    uchar alpha_array[256] = {0};

    scanline_pushcliprect(sg->sl, rc);
    srand(1234);
    for(i = 0; i < 100; i++)
    { 
      brush_set_solid(sg->brush, _rgba(rand() / (double)(RAND_MAX), rand() / (double)(RAND_MAX), rand() / (double)(RAND_MAX), rand() / (double)(RAND_MAX) / 2.0));
      scanline_set_ellipse(sg->sl, rand() % w, rand() % h, rand() % 60 + 5, rand() % 60 + 5);
    }
    
    parallelogram[0] = m_x[0];
    parallelogram[1] = m_y[0];
    parallelogram[2] = m_x[1];
    parallelogram[3] = m_y[1];
    parallelogram[4] = m_x[2];
    parallelogram[5] = m_y[2];
    
    
    // Gradient shape function (linear, radial, custom, etc)
    //-----------------
    //typedef agg::gradient_circle gradient_func_type;
    
    // Alpha gradient shape function (linear, radial, custom, etc)
    //-----------------
    //typedef agg::gradient_xy gradient_alpha_func_type;
    
    // Span interpolator. This object is used in all span generators 
    // that operate with transformations during iterating of the spans,
    // for example, image transformers use the interpolator too.
    //-----------------
    //typedef agg::span_interpolator_linear<> interpolator_type;
    
    
    // Span allocator is an object that allocates memory for 
    // the array of colors that will be used to render the 
    // color spans. One object can be shared between different 
    // span generators.
    //-----------------
    //typedef agg::span_allocator<color_type> span_allocator_type;
    
    
    // Gradient colors array adaptor
    //-----------------
    //typedef agg::pod_auto_array<color_type, 256> gradient_colors_type;
    
    
    // Finally, the gradient span generator working with the color_type 
    // color type. 
    //-----------------
    
    
    // Gradient alpha array adaptor
    //-----------------
    
    // The alpha gradient span converter working with the color_type 
    // color type. 
    //-----------------
    
    
    // Span converter type
    //-----------------
    
    
    // The gradient objects declarations
    //----------------
    
    // Declare the gradient span itself. 
    // The last two arguments are so called "d1" and "d2" 
    // defining two distances in pixels, where the gradient starts
    // and where it ends. The actual meaning of "d1" and "d2" depands
    // on the gradient function.
    //----------------
    //span_gradient_type span_gradient(span_interpolator,       gradient_func,       color_array,       0, 150);
    
    // Declare the gradient span itself. 
    // The last two arguments are so called "d1" and "d2" 
    // defining two distances in pixels, where the gradient starts
    // and where it ends. The actual meaning of "d1" and "d2" depands
    // on the gradient function.
    //----------------
    //span_gradient_alpha_type span_gradient_alpha(span_interpolator_alpha,       alpha_func,       alpha_array,       0, 100);
    
    // Span converter declaration
    
    // Finally we can draw a circle.
    //----------------
    //gradient_mtx *= agg::trans_affine_scaling(0.75, 1.2);
    //gradient_mtx *= agg::trans_affine_rotation(-agg::pi/3.0);
    //gradient_mtx *= agg::trans_affine_translation(width()/2, height()/2);
    //gradient_mtx.invert();
    //alpha_mtx.parl_to_rect(parallelogram, -100, -100, 100, 100);
    fill_color_array_3(color_array, _rgb(0,    0.19, 0.19), _rgb(0.7,  0.7,  0.19), _rgb(0.31, 0,    0));
    
    // Fill Alpha array
    //----------------
    for(i = 0; i < 256; i++) {
      alpha_array[i] = m_alpha_value[i] * (base_mask);
    }
    scanline_set_ellipse(sg->sl, width/2, height/2, 150, 150);
    
    // Render the circle with gradient plus alpha-gradient 
    
    // Draw the control points and the parallelogram
    //-----------------
#if 0
    _rgba color_pnt(0, 0.4, 0.4, 0.31);
    ell.init(m_x[0], m_y[0], 5, 5, 20);
    ras.add_path(ell);
    agg::render_scanlines_aa_solid(ras, sl, ren_base, color_pnt);
    ell.init(m_x[1], m_y[1], 5, 5, 20);
    ras.add_path(ell);
    agg::render_scanlines_aa_solid(ras, sg->sl, ren_base, color_pnt);
    ell.init(m_x[2], m_y[2], 5, 5, 20);
    ras.add_path(ell);
    agg::render_scanlines_aa_solid(ras, sl, ren_base, color_pnt);
    
    agg::vcgen_stroke stroke;
    stroke.add_vertex(m_x[0], m_y[0], agg::path_cmd_move_to);
    stroke.add_vertex(m_x[1], m_y[1], agg::path_cmd_line_to);
    stroke.add_vertex(m_x[2], m_y[2], agg::path_cmd_line_to);
    stroke.add_vertex(m_x[0]+m_x[2]-m_x[1], m_y[0]+m_y[2]-m_y[1], agg::path_cmd_line_to);
    stroke.add_vertex(0, 0, agg::path_cmd_end_poly | agg::path_flags_close);
    ras.add_path(stroke);
    agg::render_scanlines_aa_solid(ras, sl, ren_base, _rgba(0, 0, 0));
    
    agg::render_ctrl(ras, sl, ren_base, m_alpha);
#endif
    scanline_popcliprect(sg->sl);
    }
    return 0;
}

    
    
#if 0
    
    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
      unsigned i;
      if(flags & agg::mouse_left)
      {
        for (i = 0; i < 3; i++)
        {
          if(sqrt( (x-m_x[i]) * (x-m_x[i]) + (y-m_y[i]) * (y-m_y[i]) ) < 10.0)
          {
            m_dx = x - m_x[i];
            m_dy = y - m_y[i];
            m_idx = i;
            break;
          }
        }
        if(i == 3)
        {
          if(ipoint_in_triangle(m_x[0], m_y[0], m_x[1], m_y[1],m_x[2], m_y[2],x, y))
          {
            m_dx = x - m_x[0];
            m_dy = y - m_y[0];
            m_idx = 3;
          }
          
        }
      }
    }
    
    
    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
      if(flags & agg::mouse_left)
      {
        if(m_idx == 3)
        {
          double dx = x - m_dx;
          double dy = y - m_dy;
          m_x[1] -= m_x[0] - dx;
          m_y[1] -= m_y[0] - dy;
          m_x[2] -= m_x[0] - dx;
          m_y[2] -= m_y[0] - dy;
          m_x[0] = dx;
          m_y[0] = dy;
          force_redraw();
          return;
        }
        
        if(m_idx >= 0)
        {
          m_x[m_idx] = x - m_dx;
          m_y[m_idx] = y - m_dy;
          force_redraw();
        }
      }
      else
      {
        on_mouse_button_up(x, y, flags);
      }
    }
    
    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
      m_idx = -1;
    }
    
    
    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
      double dx = 0;
      double dy = 0;
      switch(key)
      {
      case agg::key_left:  dx = -0.1; break;
      case agg::key_right: dx =  0.1; break;
      case agg::key_up:    dy =  0.1; break;
      case agg::key_down:  dy = -0.1; break;
      }
      m_x[0] += dx;
      m_y[0] += dy;
      m_x[1] += dx;
      m_y[1] += dy;
      force_redraw();
    }
    
    
    
    
    
#endif
