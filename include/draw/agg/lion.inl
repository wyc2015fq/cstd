
//========================================================render_all_paths
int scanline_all_paths(scanline* sl, const Path* path, const COLOR* as, const unsigned* path_id, unsigned num_paths,
                       brush_t* br, int outline, PenStyle* penstyle)
{
  unsigned i;
  for(i = 0; i < num_paths; i++) {
    int len = path_id[i+1] - path_id[i];
    brush_set_solid(br, as[i]);
    if (outline) {
      scanline_set_path_stroke(sl, path->Points + path_id[i], path->Types + path_id[i], len, penstyle);
    } else {
      scanline_set_path(sl, path->Points + path_id[i], path->Types + path_id[i], len, false);
    }
  }
  return 0;
}

int lion(ctrl_t* c)
{
  SYSIO;SOFTGC_ARG;
  int i = 0;
  static ctrl_t cc[10] = {0};
  ctrl_t* m_alpha_slider = cc + i++;
  ctrl_t* m_magn_slider = cc + i++;
  ctrl_t* m_radius_slider = cc + i++;
  ctrl_t* m_mag = cc + i++;
  ctrl_t* m_inv = cc + i++;
  ctrl_t* m_outline = cc + i++;
  ctrl_t* m_width = cc + i++;
  static float m_alpha_slider_value = (0.1);
  static float m_magn_slider_value = (3.0);
  static float m_radius_slider_value = (70.0);
  
  static Path g_path[1] = {0};
  static COLOR g_colors[100] = {0};
  static unsigned          g_path_idx[100] = {0};
  static unsigned          g_npaths = 0;
  static double            g_base_dx = 0;
  static double            g_base_dy = 0;
  static double            g_angle = 0;
  static double            g_scale = 1.0;
  static double            g_skew_x = 0;
  static double            g_skew_y = 0;
  static double g_x1 = 200;
  static double g_y1 = 150;
  static int               g_nclick = 0;
  int width, height;
  static int m_mag_value = 0;
  static int m_inv_value = 0;
  static int m_outline_value = 1;
  static float m_width_value = 2;
  IRECT rc = c->rc, rcRight;
  
  iRectCutR(&rc, -200, 0, &rcRight);
  flexboxlay(rcRight, countof(cc), cc, 2, flex_column_reverse);
  slider_float_ctrl(m_alpha_slider, "Alpha=%3.3f", &m_alpha_slider_value, 0, 1);
  slider_float_ctrl(m_magn_slider, "Scale=%3.2f", &m_magn_slider_value, 0.01, 4.0);
  slider_float_ctrl(m_radius_slider, "Radius=%3.2f", &m_radius_slider_value, 0.0, 100.0);
  slider_float_ctrl(m_width, "width=%3.2f", &m_width_value, 0.0, 5.0);
  cbox_ctrl(m_mag, "trans_warp_magnifier", &m_mag_value);
  cbox_ctrl(m_inv, "m_inv", &m_inv_value);
  cbox_ctrl(m_outline, "m_outline", &m_outline_value);
  
  if (NULL==g_path->Points) {
    FRECT frc;
    g_npaths = parse_lion(g_path, g_colors, g_path_idx);
    PathGetBounds(g_path, &frc, NULL, NULL);
    g_base_dx = RCW(&frc) / 2.0;
    g_base_dy = RCH(&frc) / 2.0;
  }

  width = RCW(&rc);
  height = RCH(&rc);
  
  if (ISHOVEREDWIN2())
  {
    int hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
    case MSG_RBUTTONDOWN:
      if (hot) {
        SETHIT(c);
      }
      break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        if (m_mag_value) {
          if(mouse->down[0]) {
            g_x1 = x;
            g_y1 = y;
          }
        } else {
          if(mouse->down[0]) {
            x -= width / 2;
            y -= height / 2;
            g_angle = atan2((double)y, (double)x);
            g_scale = sqrt((double)y * y + x * x) / 100.0;
            force_redraw(c);
          }
          if(mouse->down[1]) {
            g_skew_x = x;
            g_skew_y = y;
            force_redraw(c);
          }
        }
      }
      break;
    }
  }
  
  if (1) {
    unsigned i;
    float mtx[6];
    sg->penstyle->lineWidth = m_width_value;
    sg->penstyle->penAlignment = PenAlignmentCenter;
    scanline_pushcliprect(sg->sl, rc);
    M23Set(mtx);
    if (0) {
      for(i = 0; i < g_npaths; i++) {
        g_colors[i] = _RGB_A(g_colors[i], m_alpha_slider_value * 255);
      }
    }
    
    M23Translate(mtx, -g_base_dx, -g_base_dy, MatrixOrderAppend);
    M23Scale(mtx, g_scale, -g_scale, MatrixOrderAppend);
    M23Rotate(mtx, rad2deg(g_angle + CC_PI), MatrixOrderAppend);
    M23Shear(mtx, g_skew_x/1000.0, g_skew_y/1000.0, MatrixOrderAppend);
    if (!m_mag_value) {
    }
    M23Translate(mtx, width/2, height/2, MatrixOrderAppend);
    
    // This code renders the lion:
    {
      Path path2[1] = {0};
      PathClone(g_path, path2);
      PathTransform(path2, mtx);
      if (m_mag_value) {
        trans_warp_magnifier(path2->Points, path2->Count, g_x1, g_y1, m_radius_slider_value, m_magn_slider_value, m_inv_value);
      }
      //agg::render_all_paths(g_rasterizer, g_scanline, r, trans, g_colors, g_path_idx, g_npaths);
      scanline_all_paths(sg->sl, path2, g_colors, g_path_idx, g_npaths, sg->brush, m_outline_value, sg->penstyle);
      PathFree(path2);
    }
    scanline_popcliprect(sg->sl);
  }
  
  return 0;
}




