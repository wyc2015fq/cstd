#include "arrinit.inl"
void bezier4_point(double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4,
    double mu,
    double* x, double* y)
{
  double mum1, mum13, mu3;
  mum1 = 1 - mu;
  mum13 = mum1 * mum1 * mum1;
  mu3 = mu * mu * mu;
  *x = mum13 * x1 + 3 * mu * mum1 * mum1 * x2 + 3 * mu * mu * mum1 * x3 + mu3 * x4;
  *y = mum13 * y1 + 3 * mu * mum1 * mum1 * y2 + 3 * mu * mu * mum1 * y3 + mu3 * y4;
}
typedef struct tets_bezier_div_t {
  uiwindow win[1];
  COLORREF m_ctrl_color;
  bezier_ctrl m_curve1[1];
  slider_ctrl m_angle_tolerance[1];
  slider_ctrl m_approximation_scale[1];
  slider_ctrl m_cusp_limit[1];
  slider_ctrl m_width[1];
  cbox_ctrl m_show_points[1];
  cbox_ctrl m_show_outline[1];
  rbox_ctrl m_curve_type[1];
  rbox_ctrl m_case_type[1];
  rbox_ctrl m_inner_join[1];
  rbox_ctrl m_line_join[1];
  rbox_ctrl m_line_cap[1];
  int m_cur_case_type;
} tets_bezier_div_t;
typedef struct curve_point {
  double x, y, dist, mu;
} curve_point;
curve_point make_curve_point(double x1, double y1, double mu1)
{
  curve_point c = {(x1), (y1), 0, (mu1)};
  return c;
}
bool find_point(int path_size, const vertex_dist* path, double dist, unsigned* i, unsigned* j)
{
  int k;
  *j = path_size - 1;

  for (*i = 0; (*j - *i) > 1;) {
    if (dist < path[k = (*i + *j) >> 1].dist) {
      *j = k;
    }
    else {
      *i = k;
    }
  }

  return true;
}
double calc_max_error(DPOINT* pt, double scale, double* max_angle_error)
{
  //curve.approximation_scale(m_approximation_scale.value() * scale);
  DPOINT pt1[1000];
  vertex_dist curve_points[1000];
  curve_point reference_points[1000];
  double curve_dist = 0;
  int i, n;
  int curve_points_size;
  int reference_points_size;
  double x, y;
  double reference_dist;
  double max_error;
  double aerr;
  unsigned idx1, idx2;
  n = curve4_div_init(
      pt[0].x, pt[0].y,
      pt[1].x, pt[1].y,
      pt[2].x, pt[2].y,
      pt[3].x, pt[3].y,
      pt1);

  for (i = 0; i < n; ++i) {
    curve_points[i].x = pt1[i].x;
    curve_points[i].y = pt1[i].y;
  }

  curve_points_size = i;

  for (i = 1; i < curve_points_size; i++) {
    curve_points[i - 1].dist = curve_dist;
    curve_dist += calc_distance(curve_points[i - 1].x, curve_points[i - 1].y,
        curve_points[i].x, curve_points[i].y);
  }

  curve_points[curve_points_size - 1].dist = curve_dist;
  reference_points_size = 0;

  for (i = 0; i < 4096; i++) {
    double mu = i / 4095.0;
    bezier4_point(
        pt[0].x, pt[0].y,
        pt[1].x, pt[1].y,
        pt[2].x, pt[2].y,
        pt[3].x, pt[3].y,
        mu, &x, &y);
    reference_points[reference_points_size++] = make_curve_point(x, y, mu);
  }

  reference_dist = 0;

  for (i = 1; i < reference_points_size; i++) {
    reference_points[i - 1].dist = reference_dist;
    reference_dist += calc_distance(reference_points[i - 1].x, reference_points[i - 1].y,
        reference_points[i].x, reference_points[i].y);
  }

  reference_points[reference_points_size - 1].dist = reference_dist;
  idx1 = 0;
  idx2 = 1;
  max_error = 0;

  for (i = 0; i < reference_points_size; i++) {
    if (find_point(curve_points_size, curve_points, reference_points[i].dist, &idx1, &idx2)) {
      double err = fabs(calc_line_point_distance(curve_points[idx1].x, curve_points[idx1].y,
          curve_points[idx2].x, curve_points[idx2].y,
          reference_points[i].x, reference_points[i].y));

      if (err > max_error) {
        max_error = err;
      }
    }
  }

  aerr = 0;

  for (i = 2; i < curve_points_size; i++) {
    double a1 = atan2(curve_points[i - 1].y - curve_points[i - 2].y,
        curve_points[i - 1].x - curve_points[i - 2].x);
    double a2 = atan2(curve_points[i].y - curve_points[i - 1].y,
        curve_points[i].x - curve_points[i - 1].x);
    double da = fabs(a1 - a2);

    if (da >= PI) {
      da = 2 * PI - da;
    }

    if (da > aerr) {
      aerr = da;
    }
  }

  *max_angle_error = aerr * 180.0 / PI;
  return max_error * scale;
}
LRESULT CALLBACK tets_bezier_div_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  tets_bezier_div_t* g = (tets_bezier_div_t*)GetWindowLong(hwnd, GWL_USERDATA);
  uiwindow* win;
  int hit = 0;
  int ret, i;
  int x = GET_X_LPARAM(lParam);
  int y = GET_Y_LPARAM(lParam);

  if (NULL == g) {
    if (uMsg == WM_DESTROY) {
      PostQuitMessage(0);
      return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }

  win = g->win;
  ret = uiwindow_event(uMsg, wParam, lParam);

  if (ret) {
    return ret;
  }

  slider_ctrl_event(g->m_angle_tolerance, e);
  slider_ctrl_event(g->m_approximation_scale, e);
  slider_ctrl_event(g->m_cusp_limit, e);
  slider_ctrl_event(g->m_width, e);
  cbox_ctrl_event(g->m_show_points, e);
  cbox_ctrl_event(g->m_show_outline, e);
  rbox_ctrl_event(g->m_curve_type, e);
  rbox_ctrl_event(g->m_case_type, e);
  rbox_ctrl_event(g->m_inner_join, e);
  rbox_ctrl_event(g->m_line_join, e);
  rbox_ctrl_event(g->m_line_cap, e);
  polygon_ctrl_event(g->m_curve1, e);
  x -= win->rcClient.left;
  y -= win->rcClient.top;

  switch (uMsg) {
  case WM_LBUTTONUP: {
    //force_redraw(e);
  }
  break;

  case WM_PAINT: {
    img_t* im = win->im;

    if (im->h > 0 && im->w > 0 && im->data) {
      PAINTSTRUCT ps;
      HDC hdc;
      TDrawDC hDC[2] = {0};
      HDDC hDC1 = hDC + 1;
      double curve_time = 0;
      hdc = BeginPaint(hwnd, &ps);
      SetStretchBltMode(hdc, COLORONCOLOR);
      HDDCSET_IMAGE(hDC, im);
      draw_uiwindow(hDC, win);
      HDDCSET_RECT(hDC + 1, hDC, &win->rcClient);
      draw_clear(hDC + 1, rgb(1, 1, 1));
      //draw_fillrectR(hDC + 1, &win->rcClient, rgb(1, 1, 1));
      {
        double max_angle_error_01 = 0;
        double max_angle_error_1 = 0;
        double max_angle_error1 = 0;
        double max_angle_error_10 = 0;
        double max_angle_error_100 = 0;
        double max_error_01 = 0;
        double max_error_1 = 0;
        double max_error1 = 0;
        double max_error_10 = 0;
        double max_error_100 = 0;
        DPOINT* pt = g->m_curve1->m_polygon;
        DPOINT pt1[1000];
        //DPOINT pt3[4];
        int n;
        double scale = g->m_approximation_scale->m_value;
        //curve.approximation_method(curve_approximation_method_e(m_curve_type.cur_item()));
        //curve.approximation_scale(m_approximation_scale.value());
        //curve.angle_tolerance(deg2rad(m_angle_tolerance.value()));
        //curve.cusp_limit(deg2rad(m_cusp_limit.value()));
        //curve_time = measure_time(curve);
        {
#if 1

          if (g->m_case_type->m_cur_item != g->m_cur_case_type) {
            switch (g->m_case_type->m_cur_item) {
            case 0: { //m_case_type.add_item("Random");
              int w = (int)(hDC->w - 120);
              int h = (int)(hDC->h - 80);
              ARRINIT8(double, pt, rand() % w, rand() % h + 80, rand() % w, rand() % h + 80,
                  rand() % w, rand() % h + 80, rand() % w, rand() % h + 80);
            }
            break;

            case 1: //m_case_type.add_item("13---24");
              ARRINIT8(double, pt, 150, 150, 350, 150, 150, 150, 350, 150);
              //m_curve1.curve(252, 227, 16, 227, 506, 227, 285, 227);
              //m_curve1.curve(252, 227, 16, 227, 387, 227, 285, 227);
              break;

            case 2: //m_case_type.add_item("Smooth Cusp 1");
              ARRINIT8(double, pt, 50, 142, 483, 251, 496, 62, 26, 333);
              break;

            case 3: //m_case_type.add_item("Smooth Cusp 2");
              ARRINIT8(double, pt, 50, 142, 484, 251, 496, 62, 26, 333);
              break;

            case 4: //m_case_type.add_item("Real Cusp 1");
              ARRINIT8(double, pt, 100, 100, 300, 200, 200, 200, 200, 100);
              break;

            case 5: //m_case_type.add_item("Real Cusp 2");
              ARRINIT8(double, pt, 475, 157, 200, 100, 453, 100, 222, 157);
              break;

            case 6: //m_case_type.add_item("Fancy Stroke");
              ARRINIT8(double, pt, 129, 233, 32, 283, 258, 285, 159, 232);
              g->m_width->m_value = (100);
              break;

            case 7: //m_case_type.add_item("Jaw");
              ARRINIT8(double, pt, 100, 100, 300, 200, 264, 286, 264, 284);
              break;

            case 8: //m_case_type.add_item("Ugly Jaw");
              ARRINIT8(double, pt, 100, 100, 413, 304, 264, 286, 264, 284);
              break;
            }

            g->m_cur_case_type = g->m_case_type->m_cur_item;
          }

#endif
        }
#if 0
        max_error_01 = calc_max_error(pt, 0.01, &max_angle_error_01);
        max_error_1 = calc_max_error(pt, 0.1, &max_angle_error_1);
        max_error1 = calc_max_error(pt, 1, &max_angle_error1);
        max_error_10 = calc_max_error(pt, 10, &max_angle_error_10);
        max_error_100 = calc_max_error(pt, 100, &max_angle_error_100);
#endif
        //curve.approximation_scale(m_approximation_scale.value());
        //curve.angle_tolerance(deg2rad(m_angle_tolerance.value()));
        //curve.cusp_limit(deg2rad(m_cusp_limit.value()));
        n = curve4_div_init(
            pt[0].x, pt[0].y,
            pt[1].x, pt[1].y,
            pt[2].x, pt[2].y,
            pt[3].x, pt[3].y, pt1);
        //path.concat_path(curve);
        //path.move_to(m_curve1.x1(), m_curve1.y1());
        //path.line_to(m_curve1.x2(), m_curve1.y2());
        //path.line_to(m_curve1.x3(), m_curve1.y3());
        //path.line_to(m_curve1.x4(), m_curve1.y4());
        //stroke.width(m_width.value());
        //stroke.line_join(line_join_e(m_line_join.cur_item()));
        //stroke.line_cap(line_cap_e(m_line_cap.cur_item()));
        //stroke.inner_join(inner_join_e(m_inner_join.cur_item()));
        //stroke.inner_miter_limit(1.01);
        drawaa_strokepoly_impl(hDC, pt1, n, 0, rgba(0, 0.5, 0, 0.5), g->m_width->m_value,
            (line_cap_e)(g->m_line_cap->m_cur_item),
            (line_join_e)(g->m_line_join->m_cur_item),
            (inner_join_e)(g->m_inner_join->m_cur_item)
                              );

        for (i = 0; i < n; ++i) {
          DPOINT pt2[100];
          int lens[1];
          lens[0] = vcgen_ellipse(pt1[i].x, pt1[i].y, 1.5, 1.5, 8, 0, pt2);
          drawaa_fillpoly_solid(hDC, pt2, lens, 1, rgba(0, 0, 0, 0.5));
        }
      }

      if (g->m_show_outline->m_status) {
        // Draw a stroke of the stroke to see the internals
        //conv_stroke<conv_stroke<path_storage> > stroke2(stroke);
        //ras.add_path(stroke2);
        //ren.color(rgba(0, 0, 0, 0.5));
      }

      // Check ellipse and arc for the number of points
      //---------------
      //ellipse a(100, 100, m_width.value(), m_width.value(), 0);
      //ras.add_path(a);
      //ren.color(rgba(0.5,0,0, 0.5));
      //render_scanlines(ras, sl, ren);
      //a.rewind(0);
      //while(!is_stop(cmd = a.vertex(&x, &y)))
      //{
      // if(is_vertex(cmd))
      // {
      // ellipse ell(x, y, 1.5, 1.5, 8);
      // ras.add_path(ell);
      // ren.color(rgba(0,0,0,0.5));
      // render_scanlines(ras, sl, ren);
      // }
      //}
      // Check a circle with huge radius (10,000,000) and high approximation accuracy
      //---------------
      //double circle_pnt_count = 0;
      //bezier_arc ell(0,0, 10000000, 10000000, 0, 2*pi);
      //conv_curve<bezier_arc, curve3_div, curve4_div3> crv(ell);
      //crv.approximation_scale(10.0);
      //crv.rewind(0);
      //while(crv.vertex(&x, &y)) ++circle_pnt_count;
#if 0
      char buf[512];
      gsv_text t;
      t.size(8.0);
      conv_stroke<gsv_text> pt(t);
      pt.line_cap(round_cap);
      pt.line_join(round_join);
      pt.width(1.5);
      sprintf(buf, "Num Points=%d Time=%.2fmks\n\n"
          " Dist Error: x0.01=%.5f x0.1=%.5f x1=%.5f x10=%.5f x100=%.5f\n\n"
          "Angle Error: x0.01=%.1f x0.1=%.1f x1=%.1f x10=%.1f x100=%.1f",
          num_points1, curve_time,
          max_error_01,
          max_error_1,
          max_error1,
          max_error_10,
          max_error_100,
          max_angle_error_01,
          max_angle_error_1,
          max_angle_error1,
          max_angle_error_10,
          max_angle_error_100);
      t.start_point(10.0, 85.0);
      t.text(buf);
      ras.add_path(pt);
      ren.color(rgba(0, 0, 0));
      render_scanlines(ras, sl, ren);
#endif
      SelectFontToMemDC("ºÚÌå", 14, 0, 0);

      if (1) {
        RECT rc[20] = {0};
        i = 0;
        layout_it("[[0,1]:20,[2,3]:20,[4,5]:20],[6:40,7:150,8:80,9:100,10:60,11]:320", 4, g->win->rcClient.top, hDC->w - 8, hDC->h - g->win->rcClient.top - 10, 10, 10, rc);
        drawaa_slider_ctrl(hDC, g->m_angle_tolerance, rc[i]), ++i;
        drawaa_slider_ctrl(hDC, g->m_approximation_scale, rc[i]), ++i;
        drawaa_slider_ctrl(hDC, g->m_cusp_limit, rc[i]), ++i;
        drawaa_slider_ctrl(hDC, g->m_width, rc[i]), ++i;
        drawaa_cbox_ctrl(hDC, g->m_show_points, rc[i]), ++i;
        drawaa_cbox_ctrl(hDC, g->m_show_outline, rc[i]), ++i;
        drawaa_rbox_ctrl(hDC, g->m_curve_type, rc[i]), ++i;
        drawaa_rbox_ctrl(hDC, g->m_case_type, rc[i]), ++i;
        drawaa_rbox_ctrl(hDC, g->m_inner_join, rc[i]), ++i;
        drawaa_rbox_ctrl(hDC, g->m_line_join, rc[i]), ++i;
        drawaa_rbox_ctrl(hDC, g->m_line_cap, rc[i]), ++i;
        drawaa_bezier_ctrl(hDC, g->m_curve1);
      }

      UpdateLWindow(hwnd, hdc, im);
      EndPaint(hwnd, &ps);
      return 0;
    }
  }
  break;
#if 0
  virtual void on_key(int x, int y, unsigned key, unsigned flags) {
    if (key == ' ') {
      FILE* fd = fopen(full_file_name("coord"), "w");
      fprintf(fd, "%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f",
          m_curve1.x1(), m_curve1.y1(),
          m_curve1.x2(), m_curve1.y2(),
          m_curve1.x3(), m_curve1.y3(),
          m_curve1.x4(), m_curve1.y4());
      fclose(fd);
    }
  }
#endif
  };

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
int tets_bezier_div()
{
  tets_bezier_div_t g[1] = {0};
  DPOINT pt_curve1[] = {170, 424, 13, 87, 488, 423, 26, 333};
  g->m_ctrl_color = (rgba(0, 0.3, 0.5, 0.8));
  polygon_ctrl_set(g->m_curve1, 4, pt_curve1, 0);
  //m_curve1.curve(26.000, 333.000, 276.000, 126.000, 402.000, 479.000, 26.000, 333.000); // Loop with p1==p4
  //m_curve1.curve(378.000, 439.000, 378.000, 497.000, 487.000, 432.000, 14.000, 338.000); // Narrow loop
  //m_curve1.curve(288.000, 283.000, 232.000, 89.000, 66.000, 197.000, 456.000, 241.000); // Loop
  //m_curve1.curve(519.000, 142.000, 97.000, 147.000, 69.000, 147.000, 30.000, 144.000); // Almost straight
  //m_curve1.curve(100, 100, 200, 100, 100, 200, 200, 200); // A "Z" case
  //m_curve1.curve(150, 150, 350, 150, 150, 150, 350, 150); // Degenerate
  //m_curve1.curve(409, 330, 300, 200, 200, 200, 401, 263); // Strange cusp
  //m_curve1.curve(129, 233, 172, 320, 414, 253, 344, 236); // Curve cap
  //m_curve1.curve(100,100, 100,200, 100,100, 110,100); // A "boot"
  //m_curve1.curve(225, 150, 60, 150, 460, 150, 295, 150); // 2----1----4----3
  //m_curve1.curve(162.2, 248.801, 162.2, 248.801, 266, 284, 394, 335); // Coinciding 1-2
  //m_curve1.curve(162.200, 248.801, 162.200, 248.801, 257.000, 301.000, 394.000, 335.000); // Coinciding 1-2
  //m_curve1.curve(394.000, 335.000, 257.000, 301.000, 162.200, 248.801, 162.200, 248.801); // Coinciding 3-4
  //m_curve1.curve(84.200000,302.80100, 84.200000,302.80100, 79.000000,292.40100, 97.001000,304.40100); // From tiger.svg
  //m_curve1.curve(97.001000,304.40100, 79.000000,292.40100, 84.200000,302.80100, 84.200000,302.80100); // From tiger.svg opposite dir
  //m_curve1.curve(475, 157, 200, 100, 453, 100, 222, 157); // Cusp, failure for Adobe SVG
  g->m_cur_case_type = (-1);
  slider_ctrl_set(g->m_angle_tolerance, "Angle Tolerance=%.0f deg", 15, 0, 90, 0);
  slider_ctrl_set(g->m_approximation_scale, "Approximation Scale=%.3f", 1., 0.1, 5, 0);
  slider_ctrl_set(g->m_cusp_limit, "Cusp Limit=%.0f deg", 0, 0, 90, 0);
  slider_ctrl_set(g->m_width, "Width=%.2f", 50, -50, 100, 0);
  //slider_ctrl_set(g->m_width, "Ïß¿í=%.2f", 50, -50, 100, 0);
  cbox_ctrl_set(g->m_show_points, "Show Points", true);
  cbox_ctrl_set(g->m_show_outline, "Show Stroke Outline", true);
  {
    char* m_curve_type_items[] = {("Incremental"), ("Subdiv") };
    rbox_ctrl_set(g->m_curve_type, 1, 2, m_curve_type_items);
  }
  {
    char* m_case_type_items[] = {
      ("Random"),
      ("13---24"),
      ("Smooth Cusp 1"),
      ("Smooth Cusp 2"),
      ("Real Cusp 1"),
      ("Real Cusp 2"),
      ("Fancy Stroke"),
      ("Jaw"),
      ("Ugly Jaw")
    };
    rbox_ctrl_set(g->m_case_type, 0, countof(m_case_type_items), m_case_type_items);
  }
  {
    char* m_inner_join_items[] = {
      ("Inner Bevel"),
      ("Inner Miter"),
      ("Inner Jag"),
      ("Inner Round")
    };
    rbox_ctrl_set(g->m_inner_join, 3, countof(m_inner_join_items), m_inner_join_items);
  }
  {
    char* m_line_join_items[] = {
      ("Miter Join"),
      ("Miter Revert"),
      ("Round Join"),
      ("Bevel Join"),
      ("Miter Round")
    };
    rbox_ctrl_set(g->m_line_join, 1, countof(m_line_join_items), m_line_join_items);
  }
  {
    char* m_line_cap_items[] = {
      ("Butt Cap"),
      ("Square Cap"),
      ("Round Cap")
    };
    rbox_ctrl_set(g->m_line_cap, 1, countof(m_line_cap_items), m_line_cap_items);
  }
  g->win->caption = "AGG Example. tets_bezier_div";
  Win_Create(g->win, 0, tets_bezier_div_WndProc, g);
  Win_Main();
  return 0;
}

