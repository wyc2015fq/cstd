
#if 0

template<class Curve> double measure_time(Curve& curve)
{
  start_timer();
  for(int i = 0; i < 100; i++)
  {
    double x, y;
    curve.init(m_curve1.x1(), m_curve1.y1(),
      m_curve1.x2(), m_curve1.y2(),
      m_curve1.x3(), m_curve1.y3(),
      m_curve1.x4(), m_curve1.y4());
    curve.rewind(0);
    while(!is_stop(curve.vertex(&x, &y)));
  }
  return elapsed_time() * 10;
}

double calc_max_error(Curve& curve, double scale, double* max_angle_error)
{
  curve.approximation_scale(m_approximation_scale_value * scale);
  curve.init(m_curve1.x1, m_curve1.y1,
    m_curve1.x2, m_curve1.y2,
    m_curve1.x3, m_curve1.y3,
    m_curve1.x4, m_curve1.y4);
  
  vertex_dist curve_points[8];
  unsigned cmd;
  double x, y;
  curve.rewind(0);
  while(!is_stop(cmd = curve.vertex(&x, &y)))
  {
    if(is_vertex(cmd))
    {
      curve_points.add(vertex_dist(x, y));
    }
  }
  unsigned i;
  double curve_dist = 0;
  for(i = 1; i < curve_points.size; i++)
  {
    curve_points[i - 1].dist = curve_dist;
    curve_dist += calc_distance(curve_points[i-1].x, curve_points[i-1].y, 
      curve_points[i].x,   curve_points[i].y);
  }
  curve_points[curve_points.size - 1].dist = curve_dist;
  
  pod_bvector<curve_point, 8> reference_points;
  for(i = 0; i < 4096; i++)
  {
    double mu = i / 4095.0;
    bezier4_point(m_curve1.x1, m_curve1.y1,
      m_curve1.x2, m_curve1.y2,
      m_curve1.x3, m_curve1.y3,
      m_curve1.x4, m_curve1.y4,
      mu, &x, &y);
    reference_points.add(curve_point(x, y, mu));
  }
  
  double reference_dist = 0;
  for(i = 1; i < reference_points.size; i++)
  {
    reference_points[i - 1].dist = reference_dist;
    reference_dist += calc_distance(reference_points[i-1].x, reference_points[i-1].y, 
      reference_points[i].x,   reference_points[i].y);
  }
  reference_points[reference_points.size - 1].dist = reference_dist;
  
  
  unsigned idx1 = 0;
  unsigned idx2 = 1;
  double max_error = 0;
  for(i = 0; i < reference_points.size; i++)
  {
    if(find_point(curve_points, reference_points[i].dist, &idx1, &idx2))
    {
      double err = fabs(calc_line_point_distance(curve_points[idx1].x,  curve_points[idx1].y,
        curve_points[idx2].x,  curve_points[idx2].y,
        reference_points[i].x, reference_points[i].y));
      if(err > max_error) max_error = err;
    }
  }
  
  double aerr = 0;
  for(i = 2; i < curve_points.size; i++)
  {
    double a1 = atan2(curve_points[i-1].y - curve_points[i-2].y, 
      curve_points[i-1].x - curve_points[i-2].x);
    double a2 = atan2(curve_points[i].y - curve_points[i-1].y, 
      curve_points[i].x - curve_points[i-1].x);
    
    double da = fabs(a1 - a2);
    if(da >= pi) da = 2*pi - da;
    if(da > aerr) aerr = da;
  }
  
  
  *max_angle_error = aerr * 180.0 / pi;
  return max_error * scale;
}

#endif

int bezier4_offset(double x1, double y1, double x2, double y2,
                   double x3, double y3, double x4, double y4,
                   double width, FPOINT* pt) {
  return 0;
}

struct curve_point {
  double x, y, dist, mu;
};

bool find_point(const struct curve_point* path, int path_size, double dist, unsigned* i, unsigned* j)
{
  int k;
  *j = path_size - 1;
  
  for(*i = 0; (*j - *i) > 1; ) {
    if(dist < path[k = (*i + *j) >> 1].dist) {
      *j = k;
    } else {
      *i = k;
    }
  }
  return true;
}
int path_offset(const FPOINT* Points, const BYTE* Types, int Count, double distance, FPOINT* pt2) {
  PATH_LOOP_BEGIN;
  path_offset1(Points + pos, Types + pos, end-pos, closed, distance, pt2);
  PATH_LOOP_END;
  return 0;
}

int bezier_div(ctrl_t* c)
{
    COLOR m_ctrl_color;
    static ctrl_t cc[20] = {0};
    int i=0;
    ctrl_t* m_flatness = cc + i++;
    ctrl_t* m_curve1 = cc + i++;
    ctrl_t* m_angle_tolerance = cc + i++;
    ctrl_t* m_approximation_scale = cc + i++;
    ctrl_t* m_cusp_limit = cc + i++;
    ctrl_t* m_width = cc + i++;
    ctrl_t* m_show_points = cc + i++;
    ctrl_t* m_show_outline = cc + i++;
    ctrl_t* m_curve_type = cc + i++;
    ctrl_t* m_case_type = cc + i++;
    ctrl_t* m_inner_join = cc + i++;
    ctrl_t* m_line_join = cc + i++;
    ctrl_t* m_line_cap = cc + i++;
    
    static float m_flatness_value = 0.5;
    static float m_angle_tolerance_value = 15;
    static float m_approximation_scale_value = 1.0;
    static float m_cusp_limit_value = 0;
    static float m_width_value = 50.0;
    static int m_show_points_status = true;
    static int m_show_outline_status = true;
    
    static int m_curve_type_cur_item = (1);
    static int m_case_type_cur_item = 0;
    static int m_inner_join_cur_item = (3);
    static int m_line_join_cur_item = (1);
    static int m_line_cap_cur_item = (0);
    IRECT rc = c->rc, rcRight;
    static Path path[1] = {0};
    int changed = 0;
    int width, height;
    
#define CURVEDEF_DEF(CURVEDEF) \
  CURVEDEF(13---24, 150, 150, 350, 150, 150, 150, 350, 150) \
  CURVEDEF(Smooth Cusp 1, 50, 142, 483, 251, 496, 62, 26, 333) \
  CURVEDEF(Smooth Cusp 2, 50, 142, 484, 251, 496, 62, 26, 333) \
  CURVEDEF(Real Cusp 1, 100, 100, 300, 200, 200, 200, 200, 100) \
  CURVEDEF(Real Cusp 2, 475, 157, 200, 100, 453, 100, 222, 157) \
  CURVEDEF(Fancy Stroke, 129, 233, 32, 283, 258, 285, 159, 232) \
  CURVEDEF(Jaw, 100, 100, 300, 200, 264, 286, 264, 284) \
  CURVEDEF(Ugly Jaw, 100, 100, 413, 304, 264, 286, 264, 284) \
  CURVEDEF(Loop with p1==p4, 26, 333, 276, 126, 402, 479, 26, 333) \
  CURVEDEF(Narrow loop, 378, 439, 378, 497, 487, 432, 14, 338) \
  CURVEDEF(Almost straight, 519, 142, 97, 147, 69, 147, 30, 144) \
  CURVEDEF(A Z case, 100, 100, 200, 100, 100, 200, 200, 200) \
  CURVEDEF(Degenerate, 150, 150, 350, 150, 150, 150, 350, 150) \
  CURVEDEF(Strange cusp, 409, 330, 300, 200, 200, 200, 401, 263) \
  CURVEDEF(Curve cap, 129, 233, 172, 320, 414, 253, 344, 236) \
  CURVEDEF(A boot, 100,100, 100,200, 100,100, 110,100) \
  CURVEDEF(1---0---3---2, 225, 150, 60, 150, 460, 150, 295, 150) \
  CURVEDEF(Coinciding 1-2, 162.2, 248.801, 162.2, 248.801, 266, 284, 394, 335) \
  CURVEDEF(Coinciding 3-4, 394, 335, 257, 301, 162.200, 248.801, 162.200, 248.801) \
  CURVEDEF(From tiger.svg, 84.2,302.80100, 84.2,302.801, 79,292.401, 97.001,304.401) \
CURVEDEF(Cusp failure for Adobe SVG, 475, 157, 200, 100, 453, 100, 222, 157) \
  CURVEDEF(Random, 0, 0, 0, 0, 0, 0, 0, 0)
    FPOINT curve[][4] = {
#define CURVEDEF(NAME, X1, Y1, X2, Y2, X3, Y3, X4, Y4)   {X1, Y1, X2, Y2, X3, Y3, X4, Y4},
CURVEDEF_DEF(CURVEDEF)
#undef CURVEDEF
    };
    const char* s_curve = "|"
#define CURVEDEF(NAME, X1, Y1, X2, Y2, X3, Y3, X4, Y4)   #NAME "|"
      CURVEDEF_DEF(CURVEDEF)
#undef CURVEDEF
      ;
    

    iRectCutR(&rc, -200, 0, &rcRight);
    width = RCW(&rc), height = RCH(&rc);
    //add_ctrl(m_curve1);
    //m_curve1.no_transform();
  
    flexboxlay(rcRight, countof(cc), cc, 2, flex_column);
    //     bezier_ctrl m_curve1;
    slider_float_ctrl(m_flatness, "flatness=%.2f", &m_flatness_value, 0, 2);
    slider_float_ctrl(m_angle_tolerance, "Angle Tolerance=%.0f deg", &m_approximation_scale_value, 0, 90);
    slider_float_ctrl(m_approximation_scale, "Approximation Scale=%.3f", &m_approximation_scale_value, 0.1, 5);
    slider_float_ctrl(m_cusp_limit, "Cusp Limit=%.0f deg", &m_cusp_limit_value, 0, 90);
    slider_float_ctrl(m_width, "Width=%.2f", &m_width_value, -50, 100);

    cbox_ctrl(m_show_points, "Show Points", &m_show_points_status);
    cbox_ctrl(m_show_outline, "Show Stroke Outline", &m_show_outline_status);
    rbox_ctrl(m_curve_type, "m_curve_type", "|Incremental|Subdiv|", &m_curve_type_cur_item);


    rbox_ctrl(m_inner_join, "m_inner_join", "|Inner Bevel|Inner Miter|Inner Jag|Inner Round|", &m_inner_join_cur_item);
    rbox_ctrl(m_line_join, "m_line_join", "|Miter Join|Miter Revert|Round Join|Bevel Join|Miter Round", &m_line_join_cur_item);
    rbox_ctrl(m_line_cap, "", "|Butt Cap|Square Cap|Round Cap", &m_line_cap_cur_item);
    
    if (0==path->Count) {
      static FPOINT pt0[] = {170, 424, 13, 87, 488, 423, 26, 333};
      PathAddPolygon(path, pt0, countof(pt0), true);
      path->Types[1] = PathPointTypeBezier;
      path->Types[2] = PathPointTypeBezier;
    }

    if (rbox_ctrl(m_case_type, "m_case_type", s_curve, &m_case_type_cur_item)) {
      if (m_case_type_cur_item>=0 && m_case_type_cur_item<countof(curve)) {
        FPOINT* p = curve[m_case_type_cur_item];
        if(m_case_type_cur_item==(countof(curve)-1)) {
          int w = (int)(width - 120);
          int h = (int)(height - 80);
          for (i=0; i<4; ++i) {
            p[i].x = rand() % w;
            p[i].y = rand() % h + 80;
          }
        }
        path->Count = 0;
        PathAddPolygon(path, p, 4, true);
        path->Types[1] = PathPointTypeBezier;
        path->Types[2] = PathPointTypeBezier;
      }
    }
    
    //m_cur_case_type=(-1);
    {
      m_ctrl_color = (_rgba(0, 0.3, 0.5, 0.8));
      //m_curve1.line_color(m_ctrl_color);
      
    }

    {
  SYSIO;
      gcClear(g, &rc, _rgb(1.0, 1.0, 0.95));
      m_curve1->rc = c->rc;
      path_ctrl2(m_curve1, path, m_flatness_value);
      if (0) {
        FPOINT* pt = path->Points;
        FPOINT pt2[4] = {0};
        gcPen(g, ColorRed, 2);
        //fpoly_offset(4, pt, 0, 30, pt2);
        path_offset1(pt, path->Types, path->Count, 0, 40, pt2);
        if (0) {
          double tt[] = {1./3, 2./3};
          for (i=0; i<countof(tt); ++i) {
            double t = tt[i];
            FPOINT pt1[4] = {0};
            pt1[0] = bezier3_point(pt, pt+1, pt+2, pt+3, t);
            pt1[1] = bezier3_angle(pt, pt+1, pt+2, pt+3, t);
            pt1[1].x = pt1[0].x + pt1[1].x;
            pt1[1].y = pt1[0].y + pt1[1].y;
            gcDrawLine(g, pt1[0].x, pt1[0].y, pt1[1].x, pt1[1].y);
          }
        }
        if (1) {
          gcDrawPolygonCtrl(g, pt3, countof(pt3), 0, io->font, 0);
        }
        //gcDrawLine(g, pt2[0].x, pt2[0].y, pt2[1].x, pt2[1].y);
        //gcDrawPolygon(g, pt2, 4, 0, GC_FLOAT, sizeof(*pt2));
        //gcDrawPathCtrl(g, pt2, path->Types, path->Count, io->font, 0, 5, true, true, true, true);
        gcDrawPathCtrl2(g, pt2, path->Types, path->Count, io->font, 0, 5, m_flatness_value);
      }

#if 0
        double x, y;
        double curve_time = 0;

        curve.approximation_method(curve_approximation_method_e(m_curve_type_cur_item()));
        curve.approximation_scale(m_approximation_scale_value);
        curve.angle_tolerance(deg2rad(m_angle_tolerance_value));
        curve.cusp_limit(deg2rad(m_cusp_limit_value));
        curve_time = measure_time(curve);
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

        max_error_01   = calc_max_error(curve, 0.01, &max_angle_error_01);
        max_error_1    = calc_max_error(curve, 0.1,  &max_angle_error_1);
        max_error1     = calc_max_error(curve, 1,    &max_angle_error1);
        max_error_10   = calc_max_error(curve, 10,   &max_angle_error_10);
        max_error_100  = calc_max_error(curve, 100,  &max_angle_error_100);

        curve.approximation_scale(m_approximation_scale_value);
        curve.angle_tolerance(deg2rad(m_angle_tolerance_value));
        curve.cusp_limit(deg2rad(m_cusp_limit_value));
        curve.init(m_curve1.x1(), m_curve1.y1(),
                   m_curve1.x2(), m_curve1.y2(),
                   m_curve1.x3(), m_curve1.y3(),
                   m_curve1.x4(), m_curve1.y4());

        path.concat_path(curve);
        //path.move_to(m_curve1.x1(), m_curve1.y1());
        //path.line_to(m_curve1.x2(), m_curve1.y2());
        //path.line_to(m_curve1.x3(), m_curve1.y3());
        //path.line_to(m_curve1.x4(), m_curve1.y4());


        conv_stroke<path_storage> stroke(path);
        stroke.width(m_width_value);
        stroke.line_join(line_join_e(m_line_join_cur_item()));
        stroke.line_cap(line_cap_e(m_line_cap_cur_item()));
        stroke.inner_join(inner_join_e(m_inner_join_cur_item()));
        stroke.inner_miter_limit(1.01);

        ras.add_path(stroke);
        ren.color(_rgba(0, 0.5, 0, 0.5));
        render_scanlines(ras, sl, ren);

        unsigned cmd;
        unsigned num_points1 = 0;
        path.rewind(0);
        while(!is_stop(cmd = path.vertex(&x, &y)))
        {
            if(m_show_points_status())
            {
                ellipse ell(x, y, 1.5, 1.5, 8);
                ras.add_path(ell);
                ren.color(_rgba(0,0,0, 0.5));
                render_scanlines(ras, sl, ren);
            }
            ++num_points1;
        }

        if(m_show_outline_status())
        {
            // Draw a stroke of the stroke to see the internals
            //--------------
            conv_stroke<conv_stroke<path_storage> > stroke2(stroke);
            ras.add_path(stroke2);
            ren.color(_rgba(0,0,0, 0.5));
            render_scanlines(ras, sl, ren);
        }

        // Check ellipse and arc for the number of points
        //---------------
        //ellipse a(100, 100, m_width_value, m_width_value, 0);
        //ras.add_path(a);
        //ren.color(_rgba(0.5,0,0, 0.5));
        //render_scanlines(ras, sl, ren);
        //a.rewind(0);
        //while(!is_stop(cmd = a.vertex(&x, &y)))
        //{
        //    if(is_vertex(cmd))
        //    {
        //        ellipse ell(x, y, 1.5, 1.5, 8);
        //        ras.add_path(ell);
        //        ren.color(_rgba(0,0,0,0.5));
        //        render_scanlines(ras, sl, ren);
        //    }
        //}


        // Check a circle with huge radius (10,000,000) and high approximation accuracy
        //---------------
        //double circle_pnt_count = 0;
        //bezier_arc ell(0,0, 10000000, 10000000, 0, 2*pi);
        //conv_curve<bezier_arc, curve3_div, curve4_div3> crv(ell);
        //crv.approximation_scale(10.0);
        //crv.rewind(0);
        //while(crv.vertex(&x, &y)) ++circle_pnt_count;


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
        ren.color(_rgba(0,0,0));
        render_scanlines(ras, sl, ren);
#endif
    }

#if 0
    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        if(key == ' ')
        {
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

return 0;    
}



