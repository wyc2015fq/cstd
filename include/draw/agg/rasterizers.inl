 
int rasterizers(ctrl_t* c) { SYSIO;SOFTGC_ARG;
    //enum flip_y_e { flip_y = true };
    static double m_x[3] = {100 + 120, 369 + 120, 143 + 120};
    static double m_y[3] = {60,170,310};
    static double m_dx;
    static double m_dy;
    static int    m_idx = -1;
    static float m_gamma_value = 0.5;
    static float m_alpha_value = 0.8;
    static int m_test_status = 1;
    static ctrl_t cc[10] = {0};
    int i = 0;
    ctrl_t* m_gamma = cc + i++;
    ctrl_t* m_alpha = cc + i++;
    ctrl_t* m_test = cc + i++;
    IRECT rc = c->rc, rcRight;
    FPOINT path[] = {m_x[0], m_y[0],
        m_x[1], m_y[1], m_x[2], m_y[2]};
    int len = countof(path);
    int n = 1;
    double t1, t2;

    iRectCutR(&rc, -300, 0, &rcRight);
    flexboxlay(rcRight, countof(cc), cc, 2, flex_column);
    slider_float_ctrl(m_gamma, "Gamma=%1.2f", &m_gamma_value, 0.0, 1.0);
    slider_float_ctrl(m_alpha, "Alpha=%1.2f", &m_alpha_value, 0.0, 1.0);
    cbox_ctrl(m_test, "m_test", &m_test_status);
    
    tic;
    for (i=0; i<n; ++i) {
      // draw_anti_aliased()
      brush_set_solid(sg->brush, _rgba(0.7, 0.5, 0.1, m_alpha_value));
      gamma_power(sg->m_gamma, m_gamma_value * 2.0);
      scanline_set_poly(sg->sl, path, &len, 1, false);
    }
    t1 = 1000*toc;

    
    tic;
    for (i=0; i<n; ++i) {
      FPOINT path1[] = {m_x[0] - 200, m_y[0],
        m_x[1] - 200, m_y[1], m_x[2] - 200, m_y[2]};
      //scanline_setSmoothingMode(sg, SmoothingModeNone);
      brush_set_solid(sg->brush, _rgba(0.1, 0.5, 0.7, m_alpha_value));
      gamma_threshold(sg->m_gamma, m_gamma_value);
      scanline_set_poly(sg->sl, path1, &len, 1, false);
    }
    t2 = 1000*toc;
    brush_set_solid(sg->brush, ColorBlack);
    scanline_set_string_format(sg->sl, iRECT(20, 20, 300, 100), io->font, 0, "Ê±¼ä t1 = %.2fms t2 = %.2fms", t1, t2);
    //scanline_set_text(sg->sl, );
#if 0
    //scanline_setSmoothingMode(sg, SmoothingModeAntiAlias);

    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & mouse_left)
        {
            unsigned i;
            for (i = 0; i < 3; i++)
            {
                if(sqrt( (x-m_x[i])     * (x-m_x[i])     + (y-m_y[i]) * (y-m_y[i]) ) < 20.0 ||
                   sqrt( (x-m_x[i]+200) * (x-m_x[i]+200) + (y-m_y[i]) * (y-m_y[i]) ) < 20)
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
                                          x, y) ||
                   point_in_triangle(m_x[0] - 200, m_y[0], 
                                          m_x[1] - 200, m_y[1],
                                          m_x[2] - 200, m_y[2],
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


    virtual void on_ctrl_change()
    {
        if(m_test.status())
        {
            on_draw();
            update_window();
            m_test.status(false);

            start_timer();
            int i;
            for(i = 0; i < 1000; i++)
            {
                draw_aliased();
            }
            double t1 = elapsed_time();

            start_timer();
            for(i = 0; i < 1000; i++)
            {
                draw_anti_aliased();
            }
            double t2 = elapsed_time();

            update_window();
            char buf[100];
            sprintf(buf, "Time Aliased=%.2fms Time Anti-Aliased=%.2fms", t1, t2);
            message(buf);
        }
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
        force_redraw();
    }
#endif
    return 0;
}




