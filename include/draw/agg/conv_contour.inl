
int conv_contour(ctrl_t* c)
{
  static ctrl_t cc[10] = {0};
  int i = 0;
  ctrl_t* m_close = cc + i++;
  ctrl_t* m_width = cc + i++;
  ctrl_t* m_auto_detect = cc + i++;
  static Path m_path[1] = {0};
  IRECT rc = c->rc, rcRight;
  static int m_auto_detect_status = 0;
  static int m_close_cur_item = 0;
  static float m_width_value = 0;
  int width, height;
  iRectCutR(&rc, -500, 0, &rcRight);
  width = RCW(&rc), height = RCH(&rc);
  
  flexboxlay(rcRight, countof(cc), cc, 2, flex_column_reverse);
  rbox_ctrl(m_close, "m_close", "|Close|Close CW|Close CCW|", &m_close_cur_item);
  slider_float_ctrl(m_width, "Width=%1.2f", &m_width_value, -100, 100);
  cbox_ctrl(m_auto_detect, "Autodetect orientation if not defined", &m_auto_detect_status);
  
#if 0
    
    if (NULL==path->Points) {
        unsigned flag = 0;
        m_path.move_to(28.47, 6.45);
        m_path.curve3(21.58, 1.12, 19.82, 0.29);
        m_path.curve3(17.19, -0.93, 14.21, -0.93);
        m_path.curve3(9.57, -0.93, 6.57, 2.25);
        m_path.curve3(3.56, 5.42, 3.56, 10.60);
        m_path.curve3(3.56, 13.87, 5.03, 16.26);
        m_path.curve3(7.03, 19.58, 11.99, 22.51);
        m_path.curve3(16.94, 25.44, 28.47, 29.64);
        m_path.line_to(28.47, 31.40);
        m_path.curve3(28.47, 38.09, 26.34, 40.58);
        m_path.curve3(24.22, 43.07, 20.17, 43.07);
        m_path.curve3(17.09, 43.07, 15.28, 41.41);
        m_path.curve3(13.43, 39.75, 13.43, 37.60);
        m_path.line_to(13.53, 34.77);
        m_path.curve3(13.53, 32.52, 12.38, 31.30);
        m_path.curve3(11.23, 30.08, 9.38, 30.08);
        m_path.curve3(7.57, 30.08, 6.42, 31.35);
        m_path.curve3(5.27, 32.62, 5.27, 34.81);
        m_path.curve3(5.27, 39.01, 9.57, 42.53);
        m_path.curve3(13.87, 46.04, 21.63, 46.04);
        m_path.curve3(27.59, 46.04, 31.40, 44.04);
        m_path.curve3(34.28, 42.53, 35.64, 39.31);
        m_path.curve3(36.52, 37.21, 36.52, 30.71);
        m_path.line_to(36.52, 15.53);
        m_path.curve3(36.52, 9.13, 36.77, 7.69);
        m_path.curve3(37.01, 6.25, 37.57, 5.76);
        m_path.curve3(38.13, 5.27, 38.87, 5.27);
        m_path.curve3(39.65, 5.27, 40.23, 5.62);
        m_path.curve3(41.26, 6.25, 44.19, 9.18);
        m_path.line_to(44.19, 6.45);
        m_path.curve3(38.72, -0.88, 33.74, -0.88);
        m_path.curve3(31.35, -0.88, 29.93, 0.78);
        m_path.curve3(28.52, 2.44, 28.47, 6.45);
        m_path.close_polygon(flag);

        m_path.move_to(28.47, 9.62);
        m_path.line_to(28.47, 26.66);
        m_path.curve3(21.09, 23.73, 18.95, 22.51);
        m_path.curve3(15.09, 20.36, 13.43, 18.02);
        m_path.curve3(11.77, 15.67, 11.77, 12.89);
        m_path.curve3(11.77, 9.38, 13.87, 7.06);
        m_path.curve3(15.97, 4.74, 18.70, 4.74);
        m_path.curve3(22.41, 4.74, 28.47, 9.62);
        m_path.close_polygon(flag);
    }
    


    {
        typedef agg::renderer_base<agg::pixfmt_bgr24> ren_base;

        agg::pixfmt_bgr24 pixf(rbuf_window);
        ren_base renb(pixf);
        renb.clear(_rgba(1, 1, 1));

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;

        agg::trans_affine mtx;
        mtx *= agg::trans_affine_scaling(4.0);
        mtx *= agg::trans_affine_translation(150, 100);


        agg::conv_transform<agg::path_storage> trans(m_path, mtx);
        agg::conv_curve<agg::conv_transform<agg::path_storage> > curve(trans);

        agg::conv_contour
            <agg::conv_curve
                <agg::conv_transform
                    <agg::path_storage> > > contour(curve);

        contour.width(m_width.value);
        //contour.inner_join(agg::inner_bevel);
        //contour.line_join(agg::miter_join);
        //contour.inner_line_join(agg::miter_join);
        //contour.inner_miter_limit(4.0);
        contour.auto_detect_orientation(m_auto_detect_status);

        compose_path;
        ras.add_path(contour);
        agg::render_scanlines_aa_solid(ras, sl, renb, _rgba(0,0,0));

        agg::render_ctrl(ras, sl, renb, m_close);
        agg::render_ctrl(ras, sl, renb, m_width);
        agg::render_ctrl(ras, sl, renb, m_auto_detect);
    }
#endif
    return 0;
}
