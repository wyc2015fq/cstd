#if 0
struct the_application : public platform_support {
  slider_ctrl<rgba8> m_gamma;
  scanline_u8 m_scanline;
  rasterizer_scanline_aa<> m_rasterizer;
  span_allocator_type m_alloc;
  color_func_type m_gradient_lut;
  gamma_lut_type m_gamma_lut;
  double m_mouse_x, m_mouse_y;
  double m_old_gamma;
public:
  the_application(pix_format_e format, BOOL flip_y) :
    platform_support(format, flip_y),
    m_gamma(5.0, 5.0, 340.0, 12.0, !flip_y),
    m_mouse_x(200), m_mouse_y(200) {
    m_gamma.range(0.5, 2.5);
    m_gamma.value(1.8);
    m_gamma.label("Gamma = %.3f");
    add_ctrl(m_gamma);
    m_gamma.no_transform();
    m_gamma_lut.gamma(m_gamma.value());
    m_old_gamma = m_gamma.value();
    build_gradient_lut();
  }
  virtual void on_init() {
    m_mouse_y = initial_height() / 2;
    m_mouse_x = initial_width() / 2;
  }
  void build_gradient_lut() {
    m_gradient_lut.remove_all();
    m_gradient_lut.add_color(0.0, rgba8_gamma_dir(rgba8(0, 255, 0), m_gamma_lut));
    m_gradient_lut.add_color(0.2, rgba8_gamma_dir(rgba8(120, 0, 0), m_gamma_lut));
    m_gradient_lut.add_color(0.7, rgba8_gamma_dir(rgba8(120, 120, 0), m_gamma_lut));
    m_gradient_lut.add_color(1.0, rgba8_gamma_dir(rgba8(0, 0, 255), m_gamma_lut));
    //m_gradient_lut.add_color(0.0, rgba8::from_wavelength(380, m_gamma.value()));
    //m_gradient_lut.add_color(0.1, rgba8::from_wavelength(420, m_gamma.value()));
    //m_gradient_lut.add_color(0.2, rgba8::from_wavelength(460, m_gamma.value()));
    //m_gradient_lut.add_color(0.3, rgba8::from_wavelength(500, m_gamma.value()));
    //m_gradient_lut.add_color(0.4, rgba8::from_wavelength(540, m_gamma.value()));
    //m_gradient_lut.add_color(0.5, rgba8::from_wavelength(580, m_gamma.value()));
    //m_gradient_lut.add_color(0.6, rgba8::from_wavelength(620, m_gamma.value()));
    //m_gradient_lut.add_color(0.7, rgba8::from_wavelength(660, m_gamma.value()));
    //m_gradient_lut.add_color(0.8, rgba8::from_wavelength(700, m_gamma.value()));
    //m_gradient_lut.add_color(0.9, rgba8::from_wavelength(740, m_gamma.value()));
    //m_gradient_lut.add_color(1.0, rgba8::from_wavelength(780, m_gamma.value()));
    m_gradient_lut.build_lut();
  }
  virtual void on_draw() {
    pixfmt pixf(rbuf_window());
    renderer_base rb(pixf);
    renderer_solid rs(rb);
    rb.clear(rgba(1, 1, 1));

    // When Gamma changes rebuild the gamma and gradient LUTs
    //------------------
    if (m_old_gamma != m_gamma.value()) {
      m_gamma_lut.gamma(m_gamma.value());
      build_gradient_lut();
      m_old_gamma = m_gamma.value();
    }

    // Gradient center. All gradient functions assume the
    // center being in the origin (0,0) and you can't
    // change it. But you can apply arbitrary transformations
    // to the gradient (see below).
    //------------------
    double cx = initial_width() / 2;
    double cy = initial_height() / 2;
    double r = 100;
    // Focal center. Defined in the gradient coordinates,
    // that is, with respect to the origin (0,0)
    //------------------
    double fx = m_mouse_x - cx;
    double fy = m_mouse_y - cy;
    gradient_func_type gradient_func(r, fx, fy);
    gradient_adaptor_type gradient_adaptor(gradient_func);
    trans_affine gradient_mtx;
    // Making the affine matrix. Move to (cx,cy),
    // apply the resizing transformations and invert
    // the matrix. Gradients and images always assume the
    // inverse transformations.
    //------------------
    gradient_mtx.translate(cx, cy);
    gradient_mtx *= trans_affine_resizing();
    gradient_mtx.invert();
    interpolator_type span_interpolator(gradient_mtx);
    span_gradient_type span_gradient(span_interpolator,
        gradient_adaptor,
        m_gradient_lut,
        0, r);
    // Form the simple rectangle
    //------------------
    m_rasterizer.reset();
    m_rasterizer.move_to_d(0, 0);
    m_rasterizer.line_to_d(width(), 0);
    m_rasterizer.line_to_d(width(), height());
    m_rasterizer.line_to_d(0, height());
    // Render the gradient to the whole screen and measure the time
    //------------------
    start_timer();
    render_scanlines_aa(m_rasterizer, m_scanline, rb, m_alloc, span_gradient);
    double tm = elapsed_time();
    // Draw the transformed circle that shows the gradient boundary
    //------------------
    ellipse e(cx, cy, r, r);
    conv_stroke<ellipse> estr(e);
    conv_transform <
    conv_stroke <
    ellipse > > etrans(estr, trans_affine_resizing());
    m_rasterizer.add_path(etrans);
    render_scanlines_aa_solid(m_rasterizer, m_scanline, rb, rgba(1, 1, 1));
    // Show the gradient time
    //------------------
    char buf[64];
    gsv_text t;
    t.size(10.0);
    conv_stroke<gsv_text> pt(t);
    pt.width(1.5);
    sprintf(buf, "%3.2f ms", tm);
    t.start_point(10.0, 35.0);
    t.text(buf);
    m_rasterizer.add_path(pt);
    render_scanlines_aa_solid(m_rasterizer, m_scanline, rb, rgba(0, 0, 0));
    // Show the controls
    //------------------
    render_ctrl(m_rasterizer, m_scanline, rb, m_gamma);
    // Apply the inverse gamma to the whole buffer
    // (transform the colors to the perceptually uniform space)
    //------------------
    pixf.apply_gamma_inv(m_gamma_lut);
  }
  virtual void on_mouse_move(int x, int y, unsigned flags) {
    if (flags & mouse_left) {
      m_mouse_x = x;
      m_mouse_y = y;
      trans_affine_resizing().inverse_transform(&m_mouse_x, &m_mouse_y);
      force_redraw();
    }
  }
  virtual void on_mouse_button_down(int x, int y, unsigned flags) {
    if (flags & mouse_left) {
      m_mouse_x = x;
      m_mouse_y = y;
      trans_affine_resizing().inverse_transform(&m_mouse_x, &m_mouse_y);
      force_redraw();
    }
  }
};
int agg_main(int argc, char* argv[])
{
  the_application app(pix_format, flip_y);
  app.caption("AGG Example. PDF linear and radial gradients");

  if (app.init(600, 400, window_resize)) {
    return app.run();
  }

  return 1;
}
#endif
typedef struct test_gradient_focal_t {
  UI_engine ui[1];
  rbox_ctrl m_method[1];
  slider_ctrl m_radius[1];
  polygon_ctrl m_shadow_ctrl[1];
} test_gradient_focal_t;
LRESULT test_gradient_focal_paint_proc(test_gradient_focal_t* g, HDDC hDC)
{
  draw_clear(hDC, rgb(1, 0.95, 0.95));

  //draw_fillrectR(hDC1, &win->rcClient, rgb(1, 1, 1));
  if (1) {
    double t;
    int i = 0;
    utime_start(_start_time);

    if (0) {
      double pos[256];
      COLORREF cols[256];
      COLORREF array[256];
      DPOINT pt[100];
      int n;
      i = 0;
#define add_color(p, c) (pos[i]=p, cols[i]=c, ++i)
      add_color(0.0, _RGB(0, 255, 0));
      add_color(0.2, _RGB(120, 0, 0));
      add_color(0.7, _RGB(120, 120, 0));
      add_color(1.0, _RGB(0, 0, 255));
      //m_gradient_lut.add_color(0.0, rgba8::from_wavelength(380, m_gamma.value()));
      //m_gradient_lut.add_color(0.1, rgba8::from_wavelength(420, m_gamma.value()));
      //m_gradient_lut.add_color(0.2, rgba8::from_wavelength(460, m_gamma.value()));
      //m_gradient_lut.add_color(0.3, rgba8::from_wavelength(500, m_gamma.value()));
      //m_gradient_lut.add_color(0.4, rgba8::from_wavelength(540, m_gamma.value()));
      //m_gradient_lut.add_color(0.5, rgba8::from_wavelength(580, m_gamma.value()));
      //m_gradient_lut.add_color(0.6, rgba8::from_wavelength(620, m_gamma.value()));
      //m_gradient_lut.add_color(0.7, rgba8::from_wavelength(660, m_gamma.value()));
      //m_gradient_lut.add_color(0.8, rgba8::from_wavelength(700, m_gamma.value()));
      //m_gradient_lut.add_color(0.9, rgba8::from_wavelength(740, m_gamma.value()));
      //m_gradient_lut.add_color(1.0, rgba8::from_wavelength(780, m_gamma.value()));
      fill_color_array2(256, array, i, cols, pos);
      n = vcgen_rect(pt, 0, 0, hDC->w, hDC->h);
      //drawaa_fillpoly_lut(hDC, pt, &n, 1, 0, 0, 0, hDC->h, array);
      drawaa_fillpoly_lut_radial(hDC, pt, &n, 1, hDC->w / 2, hDC->h / 2, 100, 50, array);
    }

    if (0) {
      test_brush(hDC);
    }

    if (1) {
      RECT rc = iRECT(100, 100, 200, 130);
      VistaButton1(hDC, rc);
    }

    t = 1000 * utime_elapsed(_start_time);

    if (1) {
      RECT rc[10] = {0};
      RECT rc2;
      char buf[256];
      _snprintf(buf, 256, "Time: %.3fms", t);
      UIGetWorkArea(g->ui, &rc2);
      InflateRect(&rc2, -3, -3);
      layout_itR("[0:40]:200,[1:20,2:20]", &rc2, 10, 10, rc);
      i = 0;
      drawaa_polygon_ctrl(hDC, g->m_shadow_ctrl);
      drawaa_rbox_ctrl(hDC, g->m_method);
      drawaa_slider_ctrl(hDC, g->m_radius);
      draw_textR(hDC, buf, -1, &rc[i], 0, rgb(0, 0, 0));
    }
  }

  return 0;
}
LRESULT test_gradient_focal_frame_proc(test_gradient_focal_t* g, event* e)
{
  int hit = 0;
  int ret = 0;
  int x = e->x;
  int y = e->y;
  UI_engine* ui = toUI(e);
  HWND hwnd = ui->hwnd;
  img_t* im = ui->im;
  rbox_ctrl_event(g->m_method, e);
  slider_ctrl_event(g->m_radius, e);
  polygon_ctrl_event(g->m_shadow_ctrl, e);
  //System_TraceMsg(hwnd, uMsg, wParam, lParam);
  return 0;
}
int test_gradient_focal()
{
  test_gradient_focal_t g[1] = {0};
  DPOINT pt[4];
  RECT padd[1] = {30, 40, 30, 40};
  COLORREF trans_color = _RGB(255, 0, 255);
  char* m_method_items[] = {"Single Color", "Color LUT"};
  rbox_ctrl_set(g->m_method, 1, 2, m_method_items);
  slider_ctrl_set(g->m_radius, "Blur Radius=%1.2f", 15.0, 0.0, 40.0, 0);
  vcgen_rect(pt, 100, 400, 400, 100);
  polygon_ctrl_set(g->m_shadow_ctrl, 4, pt, 1);
  UI_set(g->ui, "test_gradient_focal", g, test_gradient_focal_frame_proc, test_gradient_focal_frame_proc, 0, 0);
  //g->win->hwnd = Win_Create(0, test_gradient_focal_WndProc, g);
  waitkey(g->ui, -1);
  return 0;
}

