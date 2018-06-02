

//#include "agg_gamma_lut.h"

int svg_test(ctrl_t* c)
{
  SYSIO;SOFTGC_ARG;
#if 0
    agg::svg::path_renderer m_path;

    agg::slider_ctrl<_rgba8> m_expand;
    agg::slider_ctrl<_rgba8> m_gamma;
    agg::slider_ctrl<_rgba8> m_scale;
    agg::slider_ctrl<_rgba8> m_rotate;

    double m_min_x;
    double m_min_y;
    double m_max_x;
    double m_max_y;

    double m_x;
    double m_y;
    double m_dx;
    double m_dy;
    bool   m_drag_flag;

    {
        add_ctrl(m_expand);
        add_ctrl(m_gamma);
        add_ctrl(m_scale);
        add_ctrl(m_rotate);

        m_expand.label("Expand=%3.2f");
        m_expand.range(-1, 1.2);
        m_expand.value(0.0);

        m_gamma.label("Gamma=%3.2f");
        m_gamma.range(0.0, 3.0);
        m_gamma.value(1.0);

        m_scale.label("Scale=%3.2f");
        m_scale.range(0.2, 10.0);
        m_scale.value(1.0);

        m_rotate.label("Rotate=%3.2f");
        m_rotate.range(-180.0, 180.0);
        m_rotate.value(0.0);
    }
    
#endif
    static int inited=0;
    static dstrv_t dv[1] = {0};
    if (!inited) {
      char* xmlfn;
      inited = 1;
      xmlfn = "tiger.svg";
      xmlfn = "wallpaper.svg";
      sys_chdir("E:/pub/bin/json");
      dstrv_load_xml(dv, xmlfn);
    }


    if (1) {
        gcDrawSvg(g, dv->v, 0, dv->l);
        //rb.clear(_rgba(1,1,1));
#if 0
        ras.gamma(agg::gamma_power(m_gamma.value()));
        mtx *= agg::trans_affine_translation((m_min_x + m_max_x) * -0.5, (m_min_y + m_max_y) * -0.5);
        mtx *= agg::trans_affine_scaling(m_scale.value());
        mtx *= agg::trans_affine_rotation(agg::deg2rad(m_rotate.value()));
        mtx *= agg::trans_affine_translation((m_min_x + m_max_x) * 0.5 + m_x, (m_min_y + m_max_y) * 0.5 + m_y + 30);
        
        m_path.expand(m_expand.value());
        start_timer();
        m_path.render(ras, sl, ren, mtx, rb.clip_box(), 1.0);
        double tm = elapsed_time();
        unsigned vertex_count = m_path.vertex_count();

        ras.gamma(agg::gamma_none());
        agg::render_ctrl(ras, sl, rb, m_expand);
        agg::render_ctrl(ras, sl, rb, m_gamma);
        agg::render_ctrl(ras, sl, rb, m_scale);
        agg::render_ctrl(ras, sl, rb, m_rotate);


        char buf[128]; 
        agg::gsv_text t;
        t.size(10.0);
        t.flip(true);

        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.width(1.5);

        sprintf(buf, "Vertices=%d Time=%.3f ms", vertex_count, tm);

        t.start_point(10.0, 40.0);
        t.text(buf);

        ras.add_path(pt);
        ren.color(_rgba(0,0,0));
        agg::render_scanlines(ras, sl, ren);
#endif
    }

#if 0
    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        m_dx = x - m_x;
        m_dy = y - m_y;
        m_drag_flag = true;
    }

    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags == 0)
        {
            m_drag_flag = false;
        }

        if(m_drag_flag)
        {
            m_x = x - m_dx;
            m_y = y - m_dy;
            force_redraw();
        }
    }

    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
        m_drag_flag = false;
    }

    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        if(key == ' ')
        {

            agg::trans_affine mtx;
            mtx *= agg::trans_affine_translation((m_min_x + m_max_x) * -0.5, (m_min_y + m_max_y) * -0.5);
            mtx *= agg::trans_affine_scaling(m_scale.value());
            mtx *= agg::trans_affine_rotation(agg::deg2rad(m_rotate.value()));
            mtx *= agg::trans_affine_translation((m_min_x + m_max_x) * 0.5, (m_min_y + m_max_y) * 0.5);
            mtx *= agg::trans_affine_translation(m_x, m_y);

            double m[6];
            mtx.store_to(m);

            char buf[128];
            sprintf(buf, "%3.3f, %3.3f, %3.3f, %3.3f, %3.3f, %3.3f",
                         m[0], m[1], m[2], m[3], m[4], m[5]);

            message(buf);
            FILE* fd = fopen(full_file_name("transform.txt"), "a");
            fprintf(fd, "%s\n", buf);
            fclose(fd);
        }
    }
#endif
    return 0;
}




