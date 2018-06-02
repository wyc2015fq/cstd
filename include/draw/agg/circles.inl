
#include "draw/gsv_text.inl"

double random_dbl(double start, double end)
{
  unsigned r = rand() & 0x7FFF;
  return (r) * (end - start) / 32768.0 + start;
}


int circles(ctrl_t* c)
{
  SYSIO;SOFTGC_ARG;
  int i=0, width, height;
  static ctrl_t cc[3] = {0};
  ctrl_t* m_sel = cc+i++;
  ctrl_t* m_size = cc+i++;
  ctrl_t* m_z = cc+i++;
  static int inited=0;
  static float m_size_value = 2.5f;
  static float m_sel_value = 0.5f;
  static float m_z_value1 = 0.3f;
  static float m_z_value2 = 0.7f;
  int changed = 0;
  IRECT rc = c->rc;
  
  struct scatter_point
  {
    double     x;
    double     y;
    double     z;
    COLOR  color;
  };
  enum { default_num_points = 1000 };
  static struct scatter_point m_points[default_num_points];
  static unsigned       m_num_points = countof(m_points);
  
  //bool flip_y = true;
  
  static double spline_r_x[] = { 0.000000, 0.200000, 0.400000, 0.910484, 0.957258, 1.000000 };
  static double spline_r_y[] = { 1.000000, 0.800000, 0.600000, 0.066667, 0.169697, 0.600000 };
  static double spline_r_z[] = { 1.000000, 0.800000, 0.600000, 0.066667, 0.169697, 0.600000 };
  static double spline_g_x[] = { 0.000000, 0.292244, 0.485655, 0.564859, 0.795607, 1.000000 };
  static double spline_g_y[] = { 0.000000, 0.607260, 0.964065, 0.892558, 0.435571, 0.000000 };
  static double spline_g_z[] = { 0.000000, 0.607260, 0.964065, 0.892558, 0.435571, 0.000000 };
  static double spline_b_x[] = { 0.000000, 0.055045, 0.143034, 0.433082, 0.764859, 1.000000 };
  static double spline_b_y[] = { 0.385480, 0.128493, 0.021416, 0.271507, 0.713974, 1.000000 };
  static double spline_b_z[] = { 0.385480, 0.128493, 0.021416, 0.271507, 0.713974, 1.000000 };
  
  width = RCW(&rc);
  height = RCH(&rc);
  if (!inited) {
    inited = 1;
    bspline_set(6, spline_r_x, spline_r_y, spline_r_z);
    bspline_set(6, spline_g_x, spline_g_y, spline_g_z);
    bspline_set(6, spline_b_x, spline_b_y, spline_b_z);
    changed = 1;
    {
      double rx = width/3.5;
      double ry = height/3.5;
      for(i = 0; i < m_num_points; i++)
      {
        double z = m_points[i].z = random_dbl(0.0, 1.0);
        double x = cos(z * 2.0 * M_PI) * rx;
        double y = sin(z * 2.0 * M_PI) * ry;
        
        double dist  = random_dbl(0.0, rx/2.0);
        double angle = random_dbl(0.0, M_PI * 2.0);
        double r0, g0, b0;
        
        
        m_points[i].x = width/2.0  + x + cos(angle) * dist;
        m_points[i].y = height/2.0 + y + sin(angle) * dist;
        r0 = bspline_get(6, spline_r_x, spline_r_y, spline_r_z, z) * 0.8;
        g0 = bspline_get(6, spline_g_x, spline_g_y, spline_g_z, z) * 0.8;
        b0 = bspline_get(6, spline_b_x, spline_b_y, spline_b_z, z) * 0.8;
        m_points[i].color = _rgba(r0, g0, b0, 1.0);
      }
    }
  }
  
  flexboxlay(rc, countof(cc), cc, 2, flex_column_reverse);
  if (scale_float_ctrl(m_z, &m_z_value1, &m_z_value2)) {
    changed = 1;
  }
  if (slider_float_ctrl(m_sel, "Selectivity=%.2f", &m_sel_value, 0, 1)) {
    changed = 1;
  }
  if (slider_float_ctrl(m_size, "Size=%.2f", &m_size_value, 1, 20)) {
    changed = 1;
  }
  if (changed) {
    for(i = 0; i < m_num_points; i++)
    {
      m_points[i].x += random_dbl(0, m_sel_value) - m_sel_value*0.5;
      m_points[i].y += random_dbl(0, m_sel_value) - m_sel_value*0.5;
      m_points[i].z += random_dbl(0, m_sel_value*0.01) - m_sel_value*0.005;
      if(m_points[i].z < 0.0) m_points[i].z = 0.0;
      if(m_points[i].z > 1.0) m_points[i].z = 1.0;
    }
  }
  {
    unsigned i;
    unsigned n_drawn = 0;
    
    scanline_pushcliprect(sg->sl, rc);
    //imclear(im, &rc, _rgb(1,1,1));
    for(i = 0; i < m_num_points; i++)
    {
      double z = m_points[i].z;
      double alpha = 1.0;
      if(z < m_z_value1)
      {
        alpha = 
          1.0 - 
          (m_z_value1 - z) * 
          m_sel_value * 100.0;
      }
      
      if(z > m_z_value2)
      {
        alpha = 
          1.0 - 
          (z - m_z_value2) * 
          m_sel_value * 100.0;
      }
      
      if(alpha > 1.0) alpha = 1.0;
      if(alpha < 0.0) alpha = 0.0;
      
      if(alpha > 0.0)
      {
        int a = (int)(alpha*255);
        COLOR clr = _RGB_A(m_points[i].color, a);
        brush_set_solid(sg->brush, clr);
        scanline_set_ellipse(sg->sl, m_points[i].x, m_points[i].y, m_size_value, m_size_value);
        n_drawn++;
      }
    }
    {
      char buf[256];
      Path path[1] = {0};
      brush_set_solid(sg->brush, _rgb(0,0,0));
      _snprintf(buf, countof(buf), "n_drawn = %08u", n_drawn);
      gsv_text(path, 10.0, height - 20.0, 0, 15.0, 0, 0, buf, NULL, false);
      scanline_set_path_stroke2(sg->sl, path, NULL);
      PathFree(path);
    }
    scanline_popcliprect(sg->sl);
  }
  tictoc;
  return 0;
}
