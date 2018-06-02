
enum {
#define DEMO_COLOR_DEF(x) cl##x = x,
#include "wykobi/test_color_def.txt"
#undef DEMO_COLOR_DEF
};

COLOR demo_color_tab[] = {
#define DEMO_COLOR_DEF(x) x,
#include "wykobi/test_color_def.txt"
#undef DEMO_COLOR_DEF
};

int imdrawaa_circles(img_t* im, const DCIRCLE* circle, int n, COLOR crFill, COLOR clr, double wline)
{
  int i;

  for (i = 0; i < n; ++i) {
    imdrawaa_circle(im, circle[i].x, circle[i].y, circle[i].radius, crFill, clr, wline);
  }

  return 0;
}

int imdrawaa_segments(img_t* im, const DSEGMENT2* segment, int n, COLOR clr, double wline)
{
  int i;

  for (i = 0; i < n; ++i) {
    imdrawaa_line(im, segment[i].p0.x, segment[i].p0.y, segment[i].p1.x, segment[i].p1.y, clr, wline);
  }

  return 0;
}

int imdrawaa_triangles(img_t* im, const DTRIANGLE2* triangle, int n, COLOR crFill, COLOR crLine, double wline)
{
  int i;

  for (i = 0; i < n; ++i) {
    imdrawaa_triangle(im, triangle[i].p0.x, triangle[i].p0.y, triangle[i].p1.x, triangle[i].p1.y, triangle[i].p2.x, triangle[i].p2.y, crFill, crLine, wline);
  }

  return 0;
}

int imdrawaa_quadixs(img_t* im, const DQUADIX2* quadix, int n, COLOR crFill, COLOR crLine, double wline)
{
  int i;

  for (i = 0; i < n; ++i) {
    imdrawaa_poly(im, &quadix[i].p0, 4, 1, crFill, crLine, wline);
  }

  return 0;
}

int imdrawaa_rectangles(img_t* im, const DRECTANGLE* rectangle, int n, COLOR crFill, COLOR crLine, double wline)
{
  int i;

  for (i = 0; i < n; ++i) {
    imdrawaa_rect(im, rectangle[i].p0.x, rectangle[i].p0.y, rectangle[i].p1.x, rectangle[i].p1.y, crFill, crLine, wline);
  }

  return 0;
}

int imdrawaa_qbezier(img_t* im, DQUADRATIC_BEZIER2 bezier, COLOR clr, double wline)
{
  DPOINT2 point[1024];
  int len = (int)pt2_distance(bezier.p0, bezier.p2);
  len = MIN(len, 1024);
  pt2_generate_qbezier(bezier, point, len);
  imdrawaa_poly(im, point, len, 0, 0, clr, wline);
  return 0;
}

int imdrawaa_cbezier(img_t* im, DCUBIC_BEZIER2 bezier, COLOR clr, double wline)
{
  DPOINT2 point[1024];
  int len = (int)pt2_distance(bezier.p0, bezier.p3);
  len = MIN(len, 1024);
  pt2_generate_cbezier(bezier, point, len);
  imdrawaa_poly(im, point, len, 0, 0, clr, wline);
  return 0;
}

int test_random_cubic_bezier(img_t* im)
{
  enum { bezier_count = 20 };
  int i;

  for (i = 0; i < bezier_count; ++i) {
    DCUBIC_BEZIER2 bezier;
    bezier.p0 = d2_generate_random_point(im->w, im->h);
    bezier.p1 = d2_generate_random_point(im->w, im->h);
    bezier.p2 = d2_generate_random_point(im->w, im->h);
    bezier.p3 = d2_generate_random_point(im->w, im->h);
    imdrawaa_cbezier(im, bezier, demo_color_tab[i % 15], 1);
  }

  return 0;
}

int randomize()
{
  srand((unsigned)time(NULL));
  return 0;
}

int generate_polygon_type1(const T width, const T height, int n, DPOINT2* out)
{
  int i;
  DCIRCLE circle = d2_make_circle(width / (2.0), height / (2.0), min((1.0) * width, (1.0) * height) * (0.5) - (20.0));
  ci2_make_polygon(circle, n, out);

  for (i = 0; i < n; ++i) {
    if (i % 3 == 0) {
      out[i] = pt2_project_point(out[i], d2_make_point(width / (2.0), height / (2.0)), circle.radius / (2.0));
    }
  }

  return 0;
}

int generate_polygon_type2(const T width, const T height, int n, DPOINT2* out)
{
  int i;
  DCIRCLE circle = d2_make_circle(width * (0.5), height * (0.5), min(width, height) * (0.5) - (20.0));
  DPOINT2 center_point = ci_make_point(circle);
  ci2_make_polygon(circle, n, out);

  for (i = 0; i < n; ++i) {
    DVECTOR2 v = v2_normalize(v2_sub(out[i], center_point));
    v = v2_scale(v, d1_generate_random_value(circle.radius * (0.75)));
    out[i] = v2_sub(out[i], v);
  }

  return 0;
}

int generate_random_segments(const T x1, const T y1, const T x2, const T y2, int segment_count, DSEGMENT2* out)
{
  int i;

  for (i = 0; i < segment_count; i++) {
    DSEGMENT2 tmp_segment;
    se2_generate_random_object(x1, y1, x2, y2, &tmp_segment);
    out[i] = tmp_segment;
  }

  return 0;
}

int test_convex_hull_graham_scan(img_t* im)
{
  enum { max_points = 1000 };
  DQUADIX2 quadix;
  DPOINT2 point[max_points];
  DPOINT2 convex_hull[max_points];
  int n;
  imdraw_fill(im, clWhite);
  qu2_generate_random_object(0.0, 0.0, im->w, im->h, &quadix);
  qu2_generate_random_points(quadix, max_points, point);
  imdraw_dpoints(im, point, max_points, clBlack);
  n = convex_hull_graham_scan(max_points, point, convex_hull);
  imdrawaa_poly(im, convex_hull, n, 1, 0, clRed, 1);
  return 0;
}

int test_convex_hull_jarvis_march(img_t* im)
{
  enum { max_points = 1000 };
  DQUADIX2 quadix;
  DPOINT2 point[max_points];
  DPOINT2 convex_hull[max_points];
  int n;
  imdraw_fill(im, clWhite);
  qu2_generate_random_object(0.0, 0.0, im->w, im->h, &quadix);
  qu2_generate_random_points(quadix, max_points, (point));

  imdraw_dpoints(im, point, max_points, clBlack);
  n = convex_hull_jarvis_march(max_points, point, convex_hull);
  imdrawaa_poly(im, convex_hull, n, 1, 0, clRed, 1);
  return 0;
}

int test_convex_hull_melkman(img_t* im)
{
  enum { max_points = 100 };
  DPOINT2 point[max_points];
  DPOINT2 convex_hull[max_points];
  int n;
  generate_polygon_type2(im->w, im->h, max_points, point);

  po2_rotate_o(d1_generate_random_value(90.0), max_points, point, d2_make_point(im->w / 2.0, im->h / 2.0), point);
  n = convex_hull_melkman(max_points, point, convex_hull);
  imdraw_fill(im, clWhite);
  imdrawaa_poly(im, point, max_points, 1, 0, clBlack, 1);
  imdrawaa_poly(im, convex_hull, n, 1, 0, clRed, 1);

  return 0;
}

int test_segment_intersect(img_t* im)
{
  enum { max_segments = 100 };
  int i, j;
  DSEGMENT2 segment_list[max_segments];
  generate_random_segments(0.0, 0.0, im->w - 10.0, im->h - 10.0, max_segments, (segment_list));

  imclear(im, clWhite);

  for (i = 0; i < max_segments; ++i) {
    imdrawaa_line(im, segment_list[i].p0.x, segment_list[i].p0.y, segment_list[i].p1.x, segment_list[i].p1.y, clBlack, 1);

    for (j = i + 1; j < max_segments; ++j) {
      DPOINT2 pt;

      if (se2_intersect_at(segment_list[i], segment_list[j], &pt)) {
        imdrawaa_circle(im, pt.x, pt.y, (3.0), 0, clRed, 1);
      }
    }
  }

  return 0;
}

int test_circle_intersect(img_t* im)
{
  enum { max_circles = 100 };
  DCIRCLE circle[max_circles];
  int i, j;

  for (i = 0; i < max_circles; ++i) {
    T radius = d1_generate_random_value(50.0) + 20.0;
    circle[i] = (d2_make_circle(d1_generate_random_value(im->w - (radius * 2.0)) + radius,
        d1_generate_random_value(im->h - (radius * 2.0)) + radius, radius));
  }

  for (i = 0; i < max_circles; ++i) {
    imdrawaa_circle(im, circle[i].x, circle[i].y, circle[i].radius, 0, clBlack, 1);

    for (j = i + 1; j < max_circles; ++j) {
      if (cici2_intersect(circle[i], circle[j]) &&
          (d2_distance(circle[i].x, circle[i].y, circle[j].x, circle[j].y) >= fabs(circle[i].radius - circle[j].radius))
         ) {
        DPOINT2 i_pnt[2];
        cici2_intersection_point(circle[i], circle[j], i_pnt);
        imdrawaa_circle(im, i_pnt[0].x, i_pnt[0].y, (3.0), 0, clRed, 1);
        imdrawaa_circle(im, i_pnt[1].x, i_pnt[1].y, (3.0), 0, clRed, 1);
      }
    }
  }

  return 0;
}

int test_minimum_bounding_ball(img_t* im)
{
  enum { max_points = 1000 };
  DPOINT2 point[max_points];
  DCIRCLE minimum_bounding_ball;
  DCIRCLE base_circle = d2_make_circle((im->w / 2.0), (im->h / 2.0), min(((im->w - 5.0) / 2.0), ((im->h - 5.0) / 2.0)));
  imdraw_fill(im, clWhite);
  ci2_generate_random_points(base_circle, max_points, point);
  minimum_bounding_ball = randomized_minimum_bounding_ball(max_points, point);
  imdraw_dpoints(im, point, max_points, clBlack);
  imdrawaa_circle(im, minimum_bounding_ball.x, minimum_bounding_ball.y, minimum_bounding_ball.radius, 0, clRed, 3);
  return 0;
}

int imdraw_pixel(img_t* im, DPOINT2 pt)
{
  COLOR clr = clBlack;
  int x = (int)pt.x, y = (int)pt.y;

  if (x >= 0 && x < im->w && y >= 0 && y < im->h) {
    memcpy(im->tt.data + y * im->s + x * im->c, &clr, im->c);
  }

  return 0;
}

int test_random_points_AABB(img_t* im)
{
  enum { max_points = 1000 };
  DRECTANGLE rectangle;
  DPOINT2 point[max_points];
  int i;

  rt2_generate_random_object(0.0, 0.0, im->w, im->h, &rectangle);
  imdraw_fill(im, clWhite);
  imdrawaa_rect(im, rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y, 0, clBlack, 3);
  //imdrawaa_rect(im, 100, 100, 200, 200, 0, clBlack, 3);
  rt_generate_random_points(rectangle, max_points, point);

  for (i = 0; i < max_points; ++i) {
    if (rt2_point_in_rectangle(point[i], rectangle)) {
      imdraw_pixel(im, point[i]);
    }
    else {
      imdrawaa_circle(im, point[i].x, point[i].y, (10.0), 0, clBlack, 1);
    }
  }

  return 0;
}

int test_random_points_triangle(img_t* im)
{
  enum { max_points = 1000 };
  DTRIANGLE2 triangle;
  DPOINT2 point[max_points];
  int i;
  imdraw_fill(im, clWhite);
  tr2_generate_random_object(0.0, 0.0, im->w, im->h, &triangle);
  imdrawaa_triangle(im, triangle.p0.x, triangle.p0.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, 0, clBlack, 3);
  tr2_generate_random_points(triangle, max_points, (point));

  for (i = 0; i < max_points; ++i) {
    if (tr2_point_in_triangle(point[i], triangle)) {
      imdrawaa_circle(im, point[i].x, point[i].y, 1, clBlack, 0, 0);
    }
    else {
      imdrawaa_circle(im, point[i].x, point[i].y, (10.0), 0, clBlack, 1);
    }
  }

  return 0;
}

int test_random_points_quadix(img_t* im)
{
  enum { max_points = 1000 };
  DQUADIX2 quadix;
  DPOINT2 point[max_points];
  int i;
  imdraw_fill(im, clWhite);
  qu2_generate_random_object(0.0, 0.0, im->w, im->h, &quadix);
  imdrawaa_poly(im, &quadix.p0, 4, 1, 0, clBlack, 3);
  qu2_generate_random_points(quadix, max_points, (point));

  for (i = 0; i < max_points; ++i) {
    if (qu2_point_in_quadix(point[i], quadix)) {
      imdraw_dpoints(im, point + i, 1, clBlack);
    }
    else {
      imdrawaa_circle(im, point[i].x, point[i].y, (10.0), 0, clBlack, 1);
    }
  }

  return 0;
}

int test_random_points_circle(img_t* im)
{
  enum { max_points = 1000 };
  DCIRCLE circle;
  DPOINT2 point[max_points];
  int i;
  imdraw_fill(im, clWhite);
  ci_generate_random_object(0.0, 0.0, im->w, im->h, &circle);
  imdrawaa_circle(im, circle.x, circle.y, circle.radius, 0, clBlack, 3);
  ci2_generate_random_points(circle, max_points, (point));

  for (i = 0; i < max_points; ++i) {
    if (ci2_point_in_circle(point[i], circle)) {
      imdraw_dpoints(im, point + i, 1, clBlack);
    }
    else {
      imdrawaa_circle(im, point[i].x, point[i].y, (10.0), 0, clBlack, 1);
    }
  }

  return 0;
}

int oriented_ellipse(const T cx, const T cy, const T a_length, const T b_length, const T rotation, img_t* im)
{
  /*
  Credits: Dave Eberly (Geometric Tools)
   */
  if (d_is_equal(a_length, (0.0)) || d_is_equal(b_length, (0.0))) {
    return 0;
  }

  {
    T ax = (b_length * cos(rotation * PIDiv180));
    T ay = (b_length * sin(rotation * PIDiv180));
    T bx = -(a_length * sin(rotation * PIDiv180));
    T by = (a_length * cos(rotation * PIDiv180));

    if (d_less_than_or_equal(ax, (0.0)) && (ay > (0.0))) {
      T t = ax;
      ax = -bx;
      bx = t;
      t = ay;
      ay = -by;
      by = t;
    }
    else if ((ax < (0.0)) && d_less_than_or_equal(ay, (0.0))) {
      ax = -ax;
      ay = -ay;
      bx = -bx;
      by = -by;
    }
    else if (d_greater_than_or_equal(ax, (0.0)) && (ay < (0.0))) {
      T t = ax;
      ax = bx;
      bx = -t;
      t = ay;
      ay = by;
      by = -t;
    }

    {

      T a2x = ax * ax;
      T a2y = ay * ay;
      T b2x = bx * bx;
      T b2y = by * by;
      T axay = ax * ay;
      T bxby = bx * by;
      T la2 = a2x + a2y;
      T la4 = la2 * la2;
      T lb2 = b2x + b2y;
      T lb4 = lb2 * lb2;
      T a = a2x * lb4 + b2x * la4;
      T b = axay * lb4 + bxby * la4;
      T c = a2y * lb4 + b2y * la4;
      T d = la4 * lb4;
      T dx = (0.0);
      T dy = (0.0);
      T x = (0.0);
      T y = (0.0);
      T sigma = (0.0);
      T p1x = (0.0);
      T m1y = (0.0);
      T p1y = (0.0);

      if (d_less_than_or_equal(ay, ax)) {
        x = -ax;
        y = -ay;
        dx = -(b * ax + c * ay);
        dy = a * ax + b * ay;

        while (d_less_than_or_equal(dx, (0.0))) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          y++;
          sigma = a * x * x + 2 * b * x * y + c * y * y - d;

          if (sigma < (0.0)) {
            dx -= b;
            dy += a;
            x--;
          }

          dx += c;
          dy -= b;
        }

        while (d_less_than_or_equal(dx, dy)) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          y++;
          p1x = x + 1;
          sigma = a * p1x * p1x + 2 * b * p1x * y + c * y * y - d;

          if (d_greater_than_or_equal(sigma, (0.0))) {
            dx += b;
            dy -= a;
            x = p1x;
          }

          dx += c;
          dy -= b;
        }

        while (d_greater_than_or_equal(dy, (0.0))) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          x++;
          sigma = a * x * x + 2 * b * x * y + c * y * y - d;

          if (sigma < (0.0)) {
            dx += c;
            dy -= b;
            y++;
          }

          dx += b;
          dy -= a;
        }

        while (d_greater_than_or_equal(dy, -dx)) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          x++;
          m1y = y - 1;
          sigma = a * x * x + 2 * b * x * m1y + c * m1y * m1y - d;

          if (d_greater_than_or_equal(sigma, (0.0))) {
            dx -= c;
            dy += b;
            y = m1y;
          }

          dx += b;
          dy -= a;
        }

        while (d_greater_than_or_equal(y, ay)) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          y--;
          sigma = a * x * x + 2 * b * x * y + c * y * y - d;

          if (sigma < (0.0)) {
            dx += b;
            dy -= a;
            x++;
          }

          dx -= c;
          dy += b;
        }
      }
      else {
        x = -ax;
        y = -ay;
        dx = -(b * ax + c * ay);
        dy = a * ax + b * ay;

        while (d_greater_than_or_equal(-dx, dy)) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          x--;
          p1y = y + 1;
          sigma = a * x * x + 2 * b * x * p1y + c * p1y * p1y - d;

          if (d_greater_than_or_equal(sigma, (0.0))) {
            dx += c;
            dy -= b;
            y = p1y;
          }

          dx -= b;
          dy += a;
        }

        while (d_less_than_or_equal(dx, (0.0))) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          y++;
          sigma = a * x * x + 2 * b * x * y + c * y * y - d;

          if (sigma < (0.0)) {
            dx -= b;
            dy += a;
            x--;
          }

          dx += c;
          dy -= b;
        }

        while (d_less_than_or_equal(dx, dy)) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          y++;
          p1x = x + 1;
          sigma = a * p1x * p1x + 2 * b * p1x * y + c * y * y - d;

          if (d_greater_than_or_equal(sigma, (0.0))) {
            dx += b;
            dy -= a;
            x = p1x;
          }

          dx += c;
          dy -= b;
        }

        while (d_greater_than_or_equal(dy, (0.0))) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          x++;
          sigma = a * x * x + 2 * b * x * y + c * y * y - d;

          if (sigma < (0.0)) {
            dx += c;
            dy -= b;
            y++;
          }

          dx += b;
          dy -= a;
        }

        while (d_less_than_or_equal(x, ax)) {
          imdraw_pixel(im, d2_make_point(cx + x, cy + y));
          imdraw_pixel(im, d2_make_point(cx - x, cy - y));
          x++;
          m1y = y - 1;
          sigma = a * x * x + 2 * b * x * m1y + c * m1y * m1y - d;

          if (d_greater_than_or_equal(sigma, (0.0))) {
            dx -= c;
            dy += b;
            y = m1y;
          }

          dx += b;
          dy -= a;
        }
      }
    }
  }

  return 0;
}

int test_random_orietnted_ellipses(img_t* im)
{
  int i;
  imdraw_fill(im, clWhite);

  for (i = 0; i < 100; ++i) {
    //im.set_color(i % 15);
    oriented_ellipse(d1_generate_random_value(im->w),
        d1_generate_random_value(im->h),
        d1_generate_random_value(200),
        d1_generate_random_value(50),
        d1_generate_random_value(360), im);
  }

  return 0;
}

int test_random_quadratic_bezier(img_t* im)
{
  enum { bezier_count = 20 };
  int i;

  for (i = 0; i < bezier_count; ++i) {
    DQUADRATIC_BEZIER2 bezier;
    bezier.p0 = d2_generate_random_point(im->w, im->h);
    bezier.p1 = d2_generate_random_point(im->w, im->h);
    bezier.p2 = d2_generate_random_point(im->w, im->h);
    imdrawaa_qbezier(im, bezier, demo_color_tab[i % 15], 1);
  }

  return 0;
}

int test_rectangle_clipper(img_t* im)
{
  enum { max_segments = 50 };
  DSEGMENT2 segment_list[max_segments];
  DRECTANGLE rectangle;
  int i;
  generate_random_segments(0.0, 0.0, im->w - 10.0, im->h - 10.0, max_segments, (segment_list));
  imdrawaa_segments(im, segment_list, max_segments, clBlack, 1);
  rt2_generate_random_object(0.0, 0.0, im->w, im->h, &rectangle);
  imdrawaa_rectangles(im, &rectangle, 1, 0, clBlack, 3);

  for (i = 0; i < max_segments; ++i) {
    DSEGMENT2 clipped_segment;

    if (sert2_clip(segment_list[i], rectangle, &clipped_segment)) {
      imdrawaa_segments(im, &clipped_segment, 1, clLimeGreen, 1);
      imdrawaa_circle(im, clipped_segment.p0.x, clipped_segment.p0.y, 3, 0, clRed, 1);
      imdrawaa_circle(im, clipped_segment.p1.x, clipped_segment.p1.y, 3, 0, clRed, 1);
    }
  }

  return 0;
}

int test_triangle_clipper(img_t* im)
{
  enum { max_segments = 50 };
  DSEGMENT2 segment_list[max_segments];
  DTRIANGLE2 triangle;
  int i;
  generate_random_segments(0.0, 0.0, im->w - 10.0, im->h - 10.0, max_segments, segment_list);
  imdrawaa_segments(im, segment_list, max_segments, clBlack, 1);
  tr2_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &triangle);
  imdrawaa_triangles(im, &triangle, 1, 0, clBlack, 3);

  for (i = 0; i < max_segments; ++i) {
    DSEGMENT2 clipped_segment;

    if (setr2_clip(segment_list[i], triangle, &clipped_segment)) {
      imdrawaa_segments(im, &clipped_segment, 1, clLimeGreen, 1);
      imdrawaa_circle(im, clipped_segment.p0.x, clipped_segment.p0.y, 3, 0, clRed, 1);
      imdrawaa_circle(im, clipped_segment.p1.x, clipped_segment.p1.y, 3, 0, clRed, 1);
    }
  }

  return 0;
}

int test_quadix_clipper(img_t* im)
{
  enum { max_segments = 50 };
  DSEGMENT2 segment_list[max_segments];
  DQUADIX2 quadix;
  int i;
  generate_random_segments(0.0, 0.0, im->w - 10.0, im->h - 10.0, max_segments, segment_list);
  imdrawaa_segments(im, segment_list, max_segments, clBlack, 1);
  qu2_generate_random_object(0.0, 0.0, im->w, im->h, &quadix);
  imdrawaa_quadixs(im, &quadix, 1, 0, clBlack, 3);

  for (i = 0; i < max_segments; ++i) {
    DSEGMENT2 clipped_segment;

    if (sequ2_clip(segment_list[i], quadix, &clipped_segment)) {
      imdrawaa_segments(im, &clipped_segment, 1, clLimeGreen, 1);
      imdrawaa_circle(im, clipped_segment.p0.x, clipped_segment.p0.y, 3, 0, clRed, 1);
      imdrawaa_circle(im, clipped_segment.p1.x, clipped_segment.p1.y, 3, 0, clRed, 1);
    }
  }

  return 0;
}

int test_circle_clipper(img_t* im)
{
  enum { max_segments = 50 };
  DSEGMENT2 segment_list[max_segments];
  DCIRCLE circle;
  int i;

  generate_random_segments(0.0, 0.0, im->w - 10.0, im->h - 10.0, max_segments, (segment_list));
  imdrawaa_segments(im, segment_list, max_segments, clBlack, 1);
  ci_generate_random_object(0.0, 0.0, im->w, im->h, &circle);
  imdrawaa_circles(im, &circle, 1, 0, clBlack, 3);

  for (i = 0; i < max_segments; ++i) {
    DSEGMENT2 clipped_segment;

    if (seci2_clip(segment_list[i], circle, &clipped_segment)) {
      imdrawaa_segments(im, &clipped_segment, 1, clLimeGreen, 1);
      imdrawaa_circle(im, clipped_segment.p0.x, clipped_segment.p0.y, 3, 0, clRed, 1);
      imdrawaa_circle(im, clipped_segment.p1.x, clipped_segment.p1.y, 3, 0, clRed, 1);
    }
  }

  return 0;
}

int test_rectangle_to_rectangle_clipper(img_t* im)
{
  enum { max_rectangles = 20 };
  DRECTANGLE clip_rectangle;
  DRECTANGLE clipped_rectangle;
  DRECTANGLE rect_list[max_rectangles];
  int i;

  rt2_generate_random_object(5.0, 5.0, im->w - 5.0, im->h - 5.0, &clip_rectangle);

  for (i = 0; i < max_rectangles; ++i) {
    rt2_generate_random_object(5.0, 5.0, im->w - 5.0, im->h - 5.0, rect_list + i);
  }

  imdrawaa_rectangles(im, &clip_rectangle, 1, 0, clBlack, 3);
  imdrawaa_rectangles(im, rect_list, max_rectangles, 0, clBlack, 1);

  for (i = 0; i < max_rectangles; ++i) {
    if (rtrt2_clip(rect_list[i], clip_rectangle, &clipped_rectangle)) {
      //imdrawaa_segments(im, &clipped_segment, 1, clLimeGreen, 1);
      imdrawaa_circle(im, clipped_rectangle.p0.x, clipped_rectangle.p0.y, 3, 0, clRed, 1);
      imdrawaa_circle(im, clipped_rectangle.p1.x, clipped_rectangle.p1.y, 3, 0, clRed, 1);
      imdrawaa_circle(im, clipped_rectangle.p0.x, clipped_rectangle.p1.y, 3, 0, clRed, 1);
      imdrawaa_circle(im, clipped_rectangle.p1.x, clipped_rectangle.p0.y, 3, 0, clRed, 1);
    }
  }

  return 0;
}

int test_polygon_rectangle_clipper(img_t* im)
{
  enum {point_count = 100};
  DPOINT2 polygon[point_count];
  DPOINT2 clipped_polygon[point_count + 10];
  DRECTANGLE clip_boundry;
  int n;
  rt2_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &clip_boundry);
  generate_polygon_type2(im->w, im->h, point_count, polygon);
  po2_rotate_o(d1_generate_random_value((360.0)), point_count, polygon, d2_make_point(im->w / 2.0, im->h / 2.0), polygon);
  n = rt2_sutherland_hodgman_polygon_clipper(clip_boundry, point_count, polygon, clipped_polygon);
  imdrawaa_rectangles(im, &clip_boundry, 1, 0, clLimeGreen, 3);
  imdrawaa_poly(im, polygon, point_count, 1, 0, clBlack, 1);
  imdrawaa_poly(im, clipped_polygon, n, 1, 0, clRed, 3);
  return 0;
}

int test_polygon_triangle_clipper(img_t* im)
{
  enum {point_count = 100};
  DPOINT2 polygon[point_count];
  DPOINT2 clipped_polygon[2 * point_count];
  DTRIANGLE2 clip_boundry;
  int n;
  tr2_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &clip_boundry);
  generate_polygon_type2(im->w, im->h, point_count, polygon);
  po2_rotate_o(d1_generate_random_value((360.0)), point_count, polygon, d2_make_point(im->w / 2.0, im->h / 2.0), polygon);

  n = po2_sutherland_hodgman_polygon_clipper(3, &clip_boundry.p0, point_count, polygon, clipped_polygon);

  imdrawaa_triangles(im, &clip_boundry, 1, 0, clLimeGreen, 3);
  imdrawaa_poly(im, polygon, point_count, 1, 0, clBlack, 1);
  imdrawaa_poly(im, clipped_polygon, n, 1, 0, clRed, 3);
  return 0;
}

int test_polygon_quadix_clipper(img_t* im)
{
  enum {point_count = 100};
  DPOINT2 polygon[point_count];
  DPOINT2 clipped_polygon[2 * point_count];
  DQUADIX2 clip_boundry;
  int n;
  qu2_generate_random_object(50.0, 50.0, im->w - 50.0, im->h - 50.0, &clip_boundry);
  generate_polygon_type2(im->w, im->h, point_count, polygon);

  po2_rotate_o(d1_generate_random_value((360.0)), point_count, polygon, d2_make_point(im->w / 2.0, im->h / 2.0), polygon);

  n = po2_sutherland_hodgman_polygon_clipper(4, &clip_boundry.p0, point_count, polygon, clipped_polygon);

  imdrawaa_quadixs(im, &clip_boundry, 1, 0, clLimeGreen, 3);
  imdrawaa_poly(im, polygon, point_count, 1, 0, clBlack, 1);
  imdrawaa_poly(im, clipped_polygon, n, 1, 0, clRed, 3);

  return 0;
}

int test_polygon_polygon_clipper(img_t* im)
{
  enum {point_count = 100, clip_boundry_count = 360};
  DPOINT2 polygon[point_count];
  DPOINT2 clipped_polygon[2 * point_count];
  DPOINT2 clip_boundry[clip_boundry_count];
  DCIRCLE circle;
  int n;

  ci_generate_random_object(0.0, 0.0, im->w, im->h, &circle);
  ci2_make_polygon(circle, clip_boundry_count, clip_boundry);
  generate_polygon_type2(im->w, im->h, point_count, polygon);

  po2_rotate_o(d1_generate_random_value((360.0)), point_count, polygon, d2_make_point(im->w / 2.0, im->h / 2.0), polygon);

  n = po2_sutherland_hodgman_polygon_clipper(clip_boundry_count, clip_boundry, point_count, polygon, clipped_polygon);

  imdrawaa_circles(im, &circle, 1, 0, clLimeGreen, 3);
  imdrawaa_poly(im, polygon, point_count, 1, 0, clBlack, 1);
  imdrawaa_poly(im, clipped_polygon, n, 1, 0, clRed, 3);
  return 0;
}

int test_quadratic_bezier_intersect(img_t* im)
{
  enum { bezier_count = 20};
  enum { segment_count = 20};
  enum { point_count = bezier_count * segment_count};
  DQUADRATIC_BEZIER2 bezier_list[bezier_count];
  DSEGMENT2 segment_list[segment_count];
  DPOINT2 ipoint_list[point_count];
  int i, j, ipoint_count = 0;

  for (i = 0; i < bezier_count; ++i) {
    bezier_list[i].p0 = d2_generate_random_point(im->w, im->h);
    bezier_list[i].p1 = d2_generate_random_point(im->w, im->h);
    bezier_list[i].p2 = d2_generate_random_point(im->w, im->h);
    imdrawaa_qbezier(im, bezier_list[i], demo_color_tab[i % 15], 1);
  }

  for (i = 0; i < segment_count; ++i) {
    se2_generate_random_object(0.0, 0.0, im->w, im->h, segment_list + i);
  }

  imdrawaa_segments(im, segment_list, segment_count, clBlue, 2);

  for (i = 0; i < bezier_count; ++i) {
    for (j = 0; j < segment_count; ++j) {
      ipoint_count += seqb2_intersection_point(segment_list[j], bezier_list[i], ipoint_list + ipoint_count, 100);
    }
  }

  for (i = 0; i < ipoint_count; ++i) {
    imdrawaa_circle(im, ipoint_list[i].x, ipoint_list[i].y, 5, 0, clRed, 1);
  }

  return 0;
}

int test_cubic_bezier_intersect(img_t* im)
{
  enum { bezier_count = 20 , segment_count = 20, point_count = bezier_count * segment_count};
  DCUBIC_BEZIER2 bezier_list[bezier_count];
  DSEGMENT2 segment_list[bezier_count];

  DPOINT2 ipoint_list[point_count];
  int i, j, ipoint_count = 0;

  for (i = 0; i < bezier_count; ++i) {
    DCUBIC_BEZIER2 bezier;
    bezier.p0 = d2_generate_random_point(im->w, im->h);
    bezier.p1 = d2_generate_random_point(im->w, im->h);
    bezier.p2 = d2_generate_random_point(im->w, im->h);
    bezier.p3 = d2_generate_random_point(im->w, im->h);
    bezier_list[i] = bezier;
    imdrawaa_cbezier(im, bezier_list[i], demo_color_tab[i % 15], 1);
  }

  for (i = 0; i < segment_count; ++i) {
    se2_generate_random_object(0.0, 0.0, im->w, im->h, segment_list + i);
  }

  imdrawaa_segments(im, segment_list, segment_count, clBlue, 2);

  for (i = 0; i < bezier_count; ++i) {
    for (j = 0; j < segment_count; ++j) {
      ipoint_count += secb2_intersection_point(segment_list[j], bezier_list[i], ipoint_list + ipoint_count, 100);
    }
  }

  for (i = 0; i < ipoint_count; ++i) {
    imdrawaa_circle(im, ipoint_list[i].x, ipoint_list[i].y, 5, 0, clRed, 1);
  }

  return 0;
}

DTRIANGLE2 shrink_down(const T amount, const DTRIANGLE2 triangle)
{
  DTRIANGLE2 _triangle = triangle;
  DPOINT2 original_center = tr2_centroid(_triangle);
  _triangle = trpt2_center_at_location(tr2_scale(amount, amount, tr2_center_at_location(_triangle, 0.0, 0.0)), original_center);
  return _triangle;
}

int test_polygon_triangulation(img_t* im)
{
  enum {point_count = 10};
  DPOINT2 polygon[point_count];
  DTRIANGLE2 triangle_list[point_count];
  DTRIANGLE2 triangle_list2[point_count];
  int i, n;

  generate_polygon_type2(im->w - 1, im->h - 1, point_count, polygon);
  n = po2_polygon_triangulate(point_count, polygon, triangle_list);
  imdrawaa_poly(im, polygon, point_count, 1, 0, clBlack, 1);

  for (i = 0; i < n; ++i) {
    triangle_list2[i] = shrink_down(0.97, triangle_list[i]);
  }

  imdrawaa_triangles(im, triangle_list2, n, 0, clRed, 1);
  return 0;
}

int test_torricelli_point(img_t* im)
{
  DTRIANGLE2 triangle;
  DPOINT2 torricelli_point;
  DSEGMENT2 seg[3];
  tr2_generate_random_object(10.0, 10.0, im->w - 10.0, im->h - 10.0, &triangle);
  torricelli_point = tr2_torricelli_point(triangle);

  imdrawaa_triangles(im, &triangle, 1, 0, clBlack, 2);
  seg[0] = pt2_make_segment(triangle.p0, torricelli_point);
  seg[1] = pt2_make_segment(triangle.p1, torricelli_point);
  seg[2] = pt2_make_segment(triangle.p2, torricelli_point);
  imdrawaa_segments(im, seg, 3, clBlue, 1);

  imdrawaa_circle(im, torricelli_point.x, torricelli_point.y, 5, 0, clRed, 1);
  imdrawaa_circle(im, torricelli_point.x, torricelli_point.y, 2, 0, clRed, 1);

  imdrawaa_circle(im, triangle.p0.x, triangle.p0.y, 5, 0, clBlue, 1);
  imdrawaa_circle(im, triangle.p1.x, triangle.p1.y, 5, 0, clBlue, 1);
  imdrawaa_circle(im, triangle.p2.x, triangle.p2.y, 5, 0, clBlue, 1);
  return 0;
}

int test_closest_point_on_segment(img_t* im)
{
  enum { max_points = 40 };
  DPOINT2 point_list[max_points];
  DSEGMENT2 segment;
  int i;
  double m_dashes[] = {3, 3};
  se2_generate_random_object((0.0), (0.0), im->w - 1.0, im->h - 1.0, &segment);
  d2_generate_random_points((5.0), (5.0), im->w - 5.0, im->h - 5.0, max_points, point_list);

  imdrawaa_segments(im, &segment, 1, clBlack, 1);

  for (i = 0; i < max_points; ++i) {
    DPOINT2 closest_point = pt2_closest_point_on_segment_from_point(segment, point_list[i]);

    if (pt2_distance(closest_point, point_list[i]) > (1.0)) {
      imdrawaa_dashline(im, closest_point.x, closest_point.y, point_list[i].x, point_list[i].y, clBlack, 1, 0, m_dashes, countof(m_dashes));
    }

    imdrawaa_circle(im, point_list[i].x, point_list[i].y, 3, 0, clBlue, 1);
    imdrawaa_circle(im, closest_point.x, closest_point.y, 2, 0, clRed, 1);
  }

  return 0;
}

double m_dashes[] = {3, 3};
int test_closest_point_on_triangle(img_t* im)
{
  enum { max_points = 40 };
  DPOINT2 point_list[max_points];
  DTRIANGLE2 triangle;
  int i;
  tr2_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &triangle);
  d2_generate_random_points(5.0, 5.0, im->w - 5.0, im->h - 5.0, max_points, (point_list));
  imdrawaa_triangles(im, &triangle, 1, 0, clBlack, 1);

  for (i = 0; i < max_points; ++i) {
    DPOINT2 closest_point = pt2_closest_point_on_triangle_from_point(triangle, point_list[i]);

    if (distance(closest_point, point_list[i]) > (1.0)) {
      imdrawaa_dashline(im, closest_point.x, closest_point.y, point_list[i].x, point_list[i].y, clBlack, 1, 0, m_dashes, countof(m_dashes));
    }

    imdrawaa_circle(im, point_list[i].x, point_list[i].y, 3, 0, clBlue, 1);
    imdrawaa_circle(im, closest_point.x, closest_point.y, 2, 0, clRed, 1);
  }

  return 0;
}

int test_closest_point_on_rectangle(img_t* im)
{
  enum { max_points = 40 };
  DPOINT2 point_list[max_points];
  DRECTANGLE rectangle;
  int i;

  rt2_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &rectangle);
  d2_generate_random_points(5.0, 5.0, im->w - 5.0, im->h - 5.0, max_points, (point_list));

  imdrawaa_rectangles(im, &rectangle, 1, 0, clBlack, 1);

  for (i = 0; i < max_points; ++i) {
    DPOINT2 closest_point = pt2_closest_point_on_rectangle_from_point(rectangle, point_list[i]);

    if (distance(closest_point, point_list[i]) > (1.0)) {
      imdrawaa_dashline(im, closest_point.x, closest_point.y, point_list[i].x, point_list[i].y, clBlack, 1, 0, m_dashes, countof(m_dashes));
    }

    imdrawaa_circle(im, point_list[i].x, point_list[i].y, 3, 0, clBlue, 1);
    imdrawaa_circle(im, closest_point.x, closest_point.y, 2, 0, clRed, 1);
  }

  return 0;
}

int test_closest_point_on_quadix(img_t* im)
{
  enum { max_points = 40 };
  DPOINT2 point_list[max_points];
  DQUADIX2 quadix;
  int i;
  qu2_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &quadix);
  d2_generate_random_points(5.0, 5.0, im->w - 5.0, im->h - 5.0, max_points, (point_list));
  imdrawaa_quadixs(im, &quadix, 1, 0, clBlack, 1);

  for (i = 0; i < max_points; ++i) {
    DPOINT2 closest_point = pt2_closest_point_on_quadix_from_point(quadix, point_list[i]);

    if (distance(closest_point, point_list[i]) > (1.0)) {
      imdrawaa_dashline(im, closest_point.x, closest_point.y, point_list[i].x, point_list[i].y, clBlack, 1, 0, m_dashes, countof(m_dashes));
    }

    imdrawaa_circle(im, point_list[i].x, point_list[i].y, 3, 0, clBlue, 1);
    imdrawaa_circle(im, closest_point.x, closest_point.y, 2, 0, clRed, 1);
  }

  return 0;
}

int test_closest_point_on_circle(img_t* im)
{
  enum { max_points = 40 };
  DPOINT2 point_list[max_points];
  DCIRCLE circle;
  int i;
  ci_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &circle);
  d2_generate_random_points(5.0, 5.0, im->w - 5.0, im->h - 5.0, max_points, (point_list));

  imdrawaa_circles(im, &circle, 1, 0, clBlack, 1);

  for (i = 0; i < max_points; ++i) {
    DPOINT2 closest_point = pt2_closest_point_on_circle_from_point(circle, point_list[i]);

    if (distance(closest_point, point_list[i]) > (1.0)) {
      imdrawaa_dashline(im, closest_point.x, closest_point.y, point_list[i].x, point_list[i].y, clBlack, 1, 0, m_dashes, countof(m_dashes));
    }

    imdrawaa_circle(im, point_list[i].x, point_list[i].y, 3, 0, clBlue, 1);
    imdrawaa_circle(im, closest_point.x, closest_point.y, 2, 0, clRed, 1);
  }

  return 0;
}

int test_closest_point_on_polygon(img_t* im)
{
  enum { max_points = 80 };
  DPOINT2 point_list[2 * max_points];
  DPOINT2 polygon[max_points];
  int i;

  generate_polygon_type2(im->w, im->h, max_points, polygon);
  d2_generate_random_points(5.0, 5.0, im->w - 5.0, im->h - 5.0, max_points, (point_list));

  imdrawaa_poly(im, polygon, max_points, 1, 0, clBlack, 1);

  for (i = 0; i < max_points; ++i) {
    DPOINT2 closest_point = pt2_closest_point_on_polygon_from_point(max_points, polygon, point_list[i]);

    if (distance(closest_point, point_list[i]) > (1.0)) {
      imdrawaa_dashline(im, closest_point.x, closest_point.y, point_list[i].x, point_list[i].y, clBlack, 1, 0, m_dashes, countof(m_dashes));
    }

    imdrawaa_circle(im, point_list[i].x, point_list[i].y, 3, 0, clBlue, 1);
    imdrawaa_circle(im, closest_point.x, closest_point.y, 2, 0, clRed, 1);
  }

  return 0;
}

int test_closest_point_on_quadratic_bezier(img_t* im)
{
  enum { max_points = 40 };
  DPOINT2 point_list[max_points];
  DQUADRATIC_BEZIER2 bezier;
  int i;
  bezier.p0 = d2_generate_random_point(im->w - 5.0, im->h - 5.0);
  bezier.p1 = d2_generate_random_point(im->w - 5.0, im->h - 5.0);
  bezier.p2 = d2_generate_random_point(im->w - 5.0, im->h - 5.0);
  d2_generate_random_points(5.0, 5.0, im->w - 5.0, im->h - 5.0, max_points, (point_list));

  imdrawaa_qbezier(im, bezier, clBlack, 1);

  for (i = 0; i < max_points; ++i) {
    DPOINT2 closest_point = pt2_closest_point_on_qbezier_from_point(bezier, point_list[i], 100);

    if (distance(closest_point, point_list[i]) > (1.0)) {
      imdrawaa_dashline(im, closest_point.x, closest_point.y, point_list[i].x, point_list[i].y, clBlack, 1, 0, m_dashes, countof(m_dashes));
    }

    imdrawaa_circle(im, point_list[i].x, point_list[i].y, 3, 0, clBlue, 1);
    imdrawaa_circle(im, closest_point.x, closest_point.y, 2, 0, clRed, 1);
  }

  return 0;
}

int test_closest_point_on_cubic_bezier(img_t* im)
{
  enum { max_points = 40 };
  DPOINT2 point_list[max_points];
  DCUBIC_BEZIER2 bezier;
  int i;
  bezier.p0 = d2_generate_random_point(im->w, im->h);
  bezier.p1 = d2_generate_random_point(im->w, im->h);
  bezier.p2 = d2_generate_random_point(im->w, im->h);
  bezier.p3 = d2_generate_random_point(im->w, im->h);
  d2_generate_random_points(5.0, 5.0, im->w - 5.0, im->h - 5.0, max_points, (point_list));
  imdrawaa_cbezier(im, bezier, clBlack, 1);

  for (i = 0; i < max_points; ++i) {
    DPOINT2 closest_point = pt2_closest_point_on_cbezier_from_point(bezier, point_list[i], 100);

    if (distance(closest_point, point_list[i]) > (1.0)) {
      imdrawaa_dashline(im, closest_point.x, closest_point.y, point_list[i].x, point_list[i].y, clBlack, 1, 0, m_dashes, countof(m_dashes));
    }

    imdrawaa_circle(im, point_list[i].x, point_list[i].y, 3, 0, clBlue, 1);
    imdrawaa_circle(im, closest_point.x, closest_point.y, 2, 0, clRed, 1);
  }

  return 0;
}

int test_closest_point_between_circles(img_t* im)
{
  DCIRCLE circle1;
  DCIRCLE circle2;
  ci_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &circle1);
  ci_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &circle2);
  imdrawaa_circles(im, &circle1, 1, 0, clBlack, 1);
  imdrawaa_circles(im, &circle2, 1, 0, clBlack, 1);

  if (cici2_distance(circle1, circle2) > (0.0)) {
    DPOINT2 closest_point1 = pt2_closest_point_on_circle_from_point(circle1, ci_make_point(circle2));
    DPOINT2 closest_point2 = pt2_closest_point_on_circle_from_point(circle2, ci_make_point(circle1));

    imdrawaa_dashline(im, closest_point1.x, closest_point1.y, closest_point2.x, closest_point2.y, clRed, 1, 0, m_dashes, countof(m_dashes));

    imdrawaa_circle(im, closest_point1.x, closest_point1.y, 4, 0, clRed, 1);
    imdrawaa_circle(im, closest_point2.x, closest_point2.y, 4, 0, clRed, 1);
  }
  else {
    imdrawaa_circle(im, circle1.x, circle1.y, 4, 0, clRed, 1);
    imdrawaa_circle(im, circle1.x, circle1.y, 4, 0, clRed, 1);
  }

  return 0;
}

int test_closest_points_between_segment_circle(img_t* im)
{
  enum { max_segments = 10 };
  DSEGMENT2 segment_list[max_segments];
  DCIRCLE circle;
  int i;
  generate_random_segments((0.0), (0.0), (im->w - 10.0), (im->h - 10.0), max_segments, segment_list);
  ci_generate_random_object(0.0, 0.0, im->w - 1.0, im->h - 1.0, &circle);

  imdrawaa_circles(im, &circle, 1, 0, clBlack, 1);

  for (i = 0; i < max_segments; ++i) {
    if (! seci2_intersect(segment_list[i], circle)) {
      DSEGMENT2 se;
      DPOINT2 closest_point_on_segment = pt2_closest_point_on_segment_from_point(segment_list[i], ci_make_point(circle));
      DPOINT2 closest_point_on_circle = pt2_closest_point_on_circle_from_point(circle, closest_point_on_segment);
      imdrawaa_segments(im, segment_list + i, 1, clBlue, 1);
      imdrawaa_dashline(im, closest_point_on_segment.x, closest_point_on_segment.y, closest_point_on_circle.x, closest_point_on_circle.y, clRed, 1, 0, m_dashes, countof(m_dashes));
      se = pt2_make_segment(closest_point_on_segment, closest_point_on_circle);
      imdrawaa_segments(im, &se, 1, clBlack, 1);
      imdrawaa_circle(im, closest_point_on_segment.x, closest_point_on_segment.y, 4, 0, clRed, 1);
      imdrawaa_circle(im, closest_point_on_circle.x, closest_point_on_circle.y, 4, 0, clRed, 1);
    }
    else {
      imdrawaa_segments(im, segment_list + i, 1, clBlue, 1);
      imdrawaa_circle(im, segment_list[i].p0.x, segment_list[i].p0.y, 3, 0, clLimeGreen, 1);
      imdrawaa_circle(im, segment_list[i].p1.x, segment_list[i].p1.y, 3, 0, clLimeGreen, 1);
    }
  }

  return 0;
}

int hull_expand(const T expansion_length, int n, const DPOINT2* input_polygon, DPOINT2* output_polygon)
{
  DSEGMENT2 edge = po2_edge(n, input_polygon, 0);
  T inverter = (fabs(expansion_length));
  int i, j = 0;

  if (po2_point_in_polygon(v2_add(se2_segment_mid_point(edge), v2_normalize(v2_perpendicular(v2_sub(edge.p1, edge.p0)))), n, input_polygon)) {
    inverter = (-expansion_length);
  }

  for (i = 0; i < n; ++i) {
    DSEGMENT2 edge = po2_edge(n, input_polygon, i);
    DVECTOR2 v = v2_scale(v2_normalize(v2_perpendicular(v2_sub(edge.p1, edge.p0))), inverter);
    output_polygon[j++] = v2_add(edge.p0, v);
    output_polygon[j++] = v2_add(edge.p1, v);
  }

  return j;
}

int test_hull_expander(img_t* im)
{
  enum {point_count = 1000};
  const T expander_length = 10.0;
  DPOINT2 _polygon_1[point_count] = {0}, *polygon_1 = _polygon_1;
  DPOINT2 _polygon_2[point_count] = {0}, *polygon_2 = _polygon_2;
  DTRIANGLE2 triangle = d2_create_equilateral_triangle_l((im->w / 2.), im->h / 2., (450.0));
  /*generate_random_object(T(im.center_x() - 100), T(im.center_y() - 100),
  T(im.center_x() + 100), T(im.center_y() + 100), triangle);*/
  int i, n1 = 0, n2 = 0;
  triangle.p2.x += 70;
  imdrawaa_triangles(im, &triangle, 1, 0, clRed, 1);
  MEMCPY(polygon_1, &triangle.p0, 3);
  n1 = hull_expand(expander_length, 3, polygon_1, polygon_2);
  T_SWAP(DPOINT2*, polygon_1, polygon_2);

  for (i = 1; i < 5; ++i) {
    n2 = n1;
    n1 = hull_expand(expander_length, n1, polygon_1, polygon_2);
    T_SWAP(DPOINT2*, polygon_1, polygon_2);
  }

  imdrawaa_poly(im, polygon_1, n1, 1, 0, clBlue, 1);
  imdrawaa_poly(im, polygon_2, n2, 1, 0, clBlue, 1);
  return 0;
}

