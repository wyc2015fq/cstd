
int conv_dash_marker(ctrl_t* c)
{
  SYSIO;SOFTGC_ARG;
    static double m_x[3] = {57  + 100, 369 + 100, 143 + 100,     };
    static double m_y[3] = { 60,170,310     };
    static double m_dx;
    static double m_dy;
    static int    m_idx = -1;
    static float m_width_value = 3;
    static float m_smooth_value = 0.5;
    static int m_even_odd_status = 0;
    static int m_cap_cur_item = 0;
    static int m_close_status = 0;
    static ctrl_t cc[10] = {0};
    int i=0;
    ctrl_t* m_cap = cc + i++;
    ctrl_t* m_width = cc + i++;
    ctrl_t* m_smooth = cc + i++;
    ctrl_t* m_close = cc + i++;
    ctrl_t* m_even_odd = cc + i++;
    IRECT rc = c->rc, rcRight;
    if (1) {
      iRectCutR(&rc, -300, 0, &rcRight);
      flexboxlay(rc, countof(cc), cc, 2, flex_column_reverse);
      rbox_ctrl(m_cap, "m_cap", "|Butt Cap|Square Cap|Round Cap|", &m_cap_cur_item);
      slider_float_ctrl( m_width, "Width=%1.2f", &m_width_value, 0.0, 10.0);
      slider_float_ctrl( m_smooth, "Smooth=%1.2f", &m_smooth_value, 0.0, 2.0);
      cbox_ctrl(   m_close, "m_close", &m_close_status);
      cbox_ctrl(   m_even_odd, "m_even_odd", &m_even_odd_status);
    }
    {
        //imclear(_rgba(1, 1, 1));
        LineCap cap = LineCapButt;

        // Here we declare a very cheap-in-use path storage.
        // It allocates space for at most 20 vertices in stack and
        // never allocates memory. But be aware that adding more than
        // 20 vertices is fatal! 
        //------------------------

        Path path[10] = {0};
        int i = 1;
        Path* smooth = path + i++;
        Path* dash = path + i++;
        Path* dashstroke = path + i++;
        Path* path4 = path + i++;
        
        if(m_cap_cur_item == 1) cap = LineCapSquare;
        if(m_cap_cur_item == 2) cap = LineCapRound;
        
        PathMoveTo(path, m_x[0], m_y[0]);
        PathLineTo(path, m_x[1], m_y[1]);
        PathLineTo(path, (m_x[0]+m_x[1]+m_x[2]) / 3.0, (m_y[0]+m_y[1]+m_y[2]) / 3.0);
        PathLineTo(path, m_x[2], m_y[2]);
        if(m_close_status) {
          PathCloseFigure(path);
        }
#if 0
        PathMoveTo(path, (m_x[0] + m_x[1]) / 2, (m_y[0] + m_y[1]) / 2);
        PathLineTo(path, (m_x[1] + m_x[2]) / 2, (m_y[1] + m_y[2]) / 2);
        PathLineTo(path, (m_x[2] + m_x[0]) / 2, (m_y[2] + m_y[0]) / 2);
        if(m_close_status)
          PathCloseFigure(path);
#endif
        // (1)
        brush_set_solid(sg->brush, _rgba(0.7, 0.5, 0.1, 0.5));
        scanline_set_path2(sg->sl, path, m_even_odd_status);
        //scanline_set_path_stroke2(sg->sl, path, NULL);
        // (1)
        
        //PathStroke(path, 0, sg->penstyle, dashstroke);
        //PathPrint(dashstroke);
        //brush_set_solid(sg->brush, _rgb(0.0, 0.0, 0.0));
        //scanline_set_path2(sg->sl, dashstroke);

        // Start of (2, 3, 4)
        PathSmooth1(path, 0, m_smooth_value, smooth);
        //PathPrint(path1);

        // (2)
        brush_set_solid(sg->brush, _rgba(0.1, 0.5, 0.7, 0.1));
        scanline_set_path2(sg->sl, smooth, m_even_odd_status);
        //gcDrawPathCtrl2(g, smooth->Points, smooth->Types, smooth->Count, io->font, 0, 5, 0.01);
        // (2)

        // (3)
        brush_set_solid(sg->brush, _rgba(0.0, 0.6, 0.0, 0.8));
        scanline_set_poly_stroke2(sg->sl, smooth->Points, smooth->Types, smooth->Count, sg->penstyle);

        sg->penstyle->startCap = cap;
        sg->penstyle->endCap = cap;
        sg->penstyle->lineWidth = m_width_value;
        PathFlatten(smooth, 0, smooth);
        {
          float fdash[] = { 20.0, 5.0, 5.0, 5.0, 5.0, 5.0};
          PathDash(smooth, 0.01, fdash, countof(fdash), 10, dash);
          //PathPrint(dash);
          //PathDashStroke(path, 0, sg->penstyle, fdash, countof(fdash), 10, dash);
          PathStroke(dash, 0, sg->penstyle, dashstroke);
        }
        brush_set_solid(sg->brush, _rgb(0.0, 0.0, 0.0));
        scanline_set_path2(sg->sl, dashstroke, m_even_odd_status);
        // (3)

        // (4)
        {
          double k = pow((double)m_width_value, 0.7);
        FPOINT pts[20];
        int lens[2];
        lens[0] = vcgen_arrowhead_head(pts, 0, 4 * k, 4   * k, 3 * k, 2 * k);
        lens[1] = vcgen_arrowhead_tail(pts+lens[0], 0, 1 * k, 1.5 * k, 3 * k, 5 * k);
        trans_marker2(smooth->Points, smooth->Count, pts, lens[0], pts+lens[0], lens[1]);
        scanline_set_poly(sg->sl, pts, lens, 2, m_even_odd_status);
        }
        
#if 0
        if(!m_close_status) ah.tail();

        conv_marker<vcgen_markers_term, arrowhead> arrow(dash.markers, ah);
        ras.add_path(stroke);
        ras.add_path(arrow);
        render_scanlines_aa_solid(ras, sl, renb, _rgba(0.0, 0.0, 0.0));
        // (4)
#endif
        PathFrees(path, countof(path));
    }

#if 0
    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & mouse_left)
        {
            unsigned i;
            for (i = 0; i < 3; i++)
            {
                if(sqrt( (x-m_x[i]) * (x-m_x[i]) + (y-m_y[i]) * (y-m_y[i]) ) < 20.0)
                {
                    m_dx = x - m_x[i];
                    m_dy = y - m_y[i];
                    m_idx = i;
                    break;
                }
            }
            if(i == 3)
            {
                if(point_in_triangle(m_x[0], m_y[0], 
                                          m_x[1], m_y[1],
                                          m_x[2], m_y[2],
                                          x, y))
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
        if(flags & mouse_left)
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
                force_redraw;
                return;
            }

            if(m_idx >= 0)
            {
                m_x[m_idx] = x - m_dx;
                m_y[m_idx] = y - m_dy;
                force_redraw;
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
        case key_left:  dx = -0.1; break;
        case key_right: dx =  0.1; break;
        case key_up:    dy =  0.1; break;
        case key_down:  dy = -0.1; break;
        }
        m_x[0] += dx;
        m_y[0] += dy;
        m_x[1] += dx;
        m_y[1] += dy;
        force_redraw;
    }
#endif
    return 0;
}
