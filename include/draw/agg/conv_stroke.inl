

int conv_stroke(ctrl_t* c)
{
  SYSIO;SOFTGC_ARG;
  static double m_x[3] = {57  + 100,369 + 100,143 + 100,};
  static double m_y[3] = { 60,170,310,};
  static double m_dx;
  static double m_dy;
  static int    m_idx = -1;
  static int m_join_cur_item = 2;
  static int m_cap_cur_item = 2;
  static float m_miter_limit_value = 4.0;
  static float m_width_value = 20.0;
  static ctrl_t cc[10] = {0};
  int i = 0;
  ctrl_t* m_join = cc + i++;
  ctrl_t* m_cap = cc + i++;
  ctrl_t* m_width = cc + i++;
  ctrl_t* m_miter_limit = cc + i++;
  IRECT rc = c->rc, rcRight;
  
  iRectCutR(&rc, -200, 0, &rcRight);
  flexboxlay(rcRight, countof(cc), cc, 2, flex_column_reverse);
  rbox_ctrl(m_join, "m_join", "|Miter Join|Miter Join Revert|Round Join|Bevel Join|", &m_join_cur_item);
  rbox_ctrl(m_cap, "m_cap", "|Butt Cap|Square Cap|Round Cap|", &m_cap_cur_item);
  slider_float_ctrl(m_width, "Width=%1.2f", &m_width_value, 2.0, 40.0);
  slider_float_ctrl(m_miter_limit, "Miter Limit=%1.2f", &m_miter_limit_value, 1.0, 10.0);
  
  
  if (ISHOVEREDWIN2()) {
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      
        for (i = 0; i < 3; i++)
        {
          if(sqrt( (x-m_x[i]) * (x-m_x[i]) + (y-m_y[i]) * (y-m_y[i]) ) < 20.0)
          {
            m_dx = x - m_x[i];
            m_dy = y - m_y[i];
            m_idx = i;
            SETHIT(c);
            break;
          }
        }
        if(i == 3)
        {
          if(ipoint_in_triangle(m_x[0], m_y[0], m_x[1], m_y[1], m_x[2], m_y[2], x, y))
          {
            m_dx = x - m_x[0];
            m_dy = y - m_y[0];
            m_idx = 3;
            SETHIT(c);
          }
        }
      break;
      
      
    case MSG_MOUSEMOVE:
      {
        if(ISHIT(c))
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
            force_redraw(c);
          } else if(m_idx >= 0) {
            m_x[m_idx] = x - m_dx;
            m_y[m_idx] = y - m_dy;
            force_redraw(c);
          }
        } else {
          m_idx = -1;
        }
      }
      break;
      
    case MSG_LBUTTONUP:
      {
        m_idx = -1;
      }
      break;
      
      
    case MSG_KEYDOWN:
      {
        double dx = 0;
        double dy = 0;
        switch(io->last_key)
        {
        case KEY_LEFT:  dx = -0.1; break;
        case KEY_RIGHT: dx =  0.1; break;
        case KEY_UP:    dy =  0.1; break;
        case KEY_DOWN:  dy = -0.1; break;
        }
        m_x[0] += dx;
        m_y[0] += dy;
        m_x[1] += dx;
        m_y[1] += dy;
        force_redraw(c);
      }
      break;
    }
  }
  
  
  {
    Path path[10] = {0};
    //imclear(im, &rc, _rgb(1, 1, 1));
    Path* stroke1 = path+1;
    Path* stroke2 = path+2;
    Path* stroke3 = path+3;
    PenStyle* penstyle = sg->penstyle;
    LineCap cap;
    LineJoin join;

    scanline_pushcliprect(sg->sl, rc);
    PathMoveTo(path,m_x[0], m_y[0]);
    PathLineTo(path,(m_x[0] + m_x[1]) / 2, (m_y[0] + m_y[1]) / 2); // This point is added only to check for numerical stability
    PathLineTo(path,m_x[1], m_y[1]);
    PathLineTo(path,m_x[2], m_y[2]);
    PathLineTo(path,m_x[2], m_y[2]);                               // This point is added only to check for numerical stability
    
    PathMoveTo(path,(m_x[0] + m_x[1]) / 2, (m_y[0] + m_y[1]) / 2);
    PathLineTo(path,(m_x[1] + m_x[2]) / 2, (m_y[1] + m_y[2]) / 2);
    PathLineTo(path,(m_x[2] + m_x[0]) / 2, (m_y[2] + m_y[0]) / 2);
    PathCloseFigure(path);
    
    cap = LineCapButt;
    if(m_cap_cur_item == 1) cap = LineCapSquare;
    if(m_cap_cur_item == 2) cap = LineCapRound;
    
    join = LineJoinMiter;
    if(m_join_cur_item == 1) join = LineJoinMiterRevert;
    if(m_join_cur_item == 2) join = LineJoinRound;
    if(m_join_cur_item == 3) join = LineJoinBevel;
    
    // (1)
    penstyle->lineJoin = join;
    penstyle->startCap = cap = cap;
    penstyle->miterLimit = (m_miter_limit_value);
    penstyle->lineWidth = (m_width_value);
    penstyle->penAlignment = PenAlignmentCenter;
    
    brush_set_solid(sg->brush, _rgb(0.8, 0.7, 0.6));
    PathStroke(path, 0.1, penstyle, stroke1);
    scanline_set_path2(sg->sl, stroke1, false);
    // (1)
    // (2)
    penstyle->lineWidth = (1.5);
    brush_set_solid(sg->brush, _rgb(0,0,0));
    PathStroke(path, 0.1, penstyle, stroke2);
    scanline_set_path2(sg->sl, stroke2, false);
    
    // (2)
    // (3)
    {
      float dash[] = {20.0, m_width_value / 2.5};
      penstyle->miterLimit = (4.0);
      penstyle->lineWidth = (m_width_value / 5.0);
      PathDashStroke(stroke1, 0.1, penstyle, dash, countof(dash), 0, stroke3);
      brush_set_solid(sg->brush, _rgb(0,0,0.3));
      scanline_set_path2(sg->sl, stroke3, false);
    }
    // (3)
    // (4)
    brush_set_solid(sg->brush, _rgba(0.0, 0.0, 0.0, 0.2));
    scanline_set_path2(sg->sl, path, false);
    // (4)
    
    PathFrees(path, countof(path));
    scanline_popcliprect(sg->sl);
  }
  
  return 0;
}
