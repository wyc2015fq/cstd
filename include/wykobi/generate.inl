
CC_INLINE DPOINT2 se2_generate_point_on_segment(const DSEGMENT2 segment, const T t)
{
  if ((t < (0.0)) || (t > (1.0))) {
    return pt2_degenerate();
  }

  return d2_make_point(((1.0) - t) * segment.p0.x + t * segment.p1.x,
      ((1.0) - t) * segment.p0.y + t * segment.p1.y);
}

CC_INLINE DPOINT3 se3_generate_point_on_segment(const DSEGMENT3 segment, const T t)
{
  if ((t < (0.0)) || (t > (1.0))) {
    return pt3_degenerate();
  }

  return d3_make_point(((1.0) - t) * segment.p0.x + t * segment.p1.x,
      ((1.0) - t) * segment.p0.y + t * segment.p1.y,
      ((1.0) - t) * segment.p0.z + t * segment.p1.z);
}

CC_INLINE T d1_generate_random_value(const T range)
{
  return ((1.0 * range * rand()) / RAND_MAX);
}

CC_INLINE DPOINT2 d2_generate_random_point(const T dx, const T dy)
{
  return d2_make_point(d1_generate_random_value(dx), d1_generate_random_value(dy));
}

CC_INLINE DPOINT3 d3_generate_random_point(const T dx, const T dy, const T dz)
{
  return d3_make_point(d1_generate_random_value(dx), d1_generate_random_value(dy), d1_generate_random_value(dz));
}

CC_INLINE DPOINT2 se2_generate_random_point(const DSEGMENT2 segment)
{
  T t = d1_generate_random_value((1.0));
  return d2_make_point(((1 - t) * segment.p0.x) + (t * segment.p1.x),
      ((1 - t) * segment.p0.y) + (t * segment.p1.y));
}

CC_INLINE DPOINT3 se3_generate_random_point(const DSEGMENT3 segment)
{
  T t = d1_generate_random_value((1.0));
  return d3_make_point(((1 - t) * segment.p0.x) + (t * segment.p1.x),
      ((1 - t) * segment.p0.y) + (t * segment.p1.y),
      ((1 - t) * segment.p0.z) + (t * segment.p1.z));
}

CC_INLINE DPOINT2 tr2_generate_random_point(const DTRIANGLE2 triangle)
{
  T a, b, c;
  a = d1_generate_random_value((1.0));
  b = d1_generate_random_value((1.0));

  if ((a + b) > (1.0)) {
    a = 1 - a;
    b = 1 - b;
  }

  c = (1 - a - b);
  return d2_make_point((triangle.p0.x * a) + (triangle.p1.x * b) + (triangle.p2.x * c),
      (triangle.p0.y * a) + (triangle.p1.y * b) + (triangle.p2.y * c));
}

CC_INLINE DPOINT3 tr3_generate_random_point(const DTRIANGLE3 triangle)
{
  T a, b, c;
  a = d1_generate_random_value((1.0));
  b = d1_generate_random_value((1.0));

  if ((a + b) > (1.0)) {
    a = 1 - a;
    b = 1 - b;
  }

  c = (1 - a - b);
  return d3_make_point((triangle.p0.x * a) + (triangle.p1.x * b) + (triangle.p2.x * c),
      (triangle.p0.y * a) + (triangle.p1.y * b) + (triangle.p2.y * c),
      (triangle.p0.z * a) + (triangle.p1.z * b) + (triangle.p2.z * c));
}

CC_INLINE DPOINT2 qu2_generate_random_point(const DQUADIX2 quadix)
{
  T a = (2 * d1_generate_random_value((1.0))) - 1;
  T b = (2 * d1_generate_random_value((1.0))) - 1;

  T a1 = (1.0) - a;
  T a2 = (1.0) + a;

  T b1 = (1.0) - b;
  T b2 = (1.0) + b;

  T r1 = a1 * b1;
  T r2 = a2 * b1;
  T r3 = a2 * b2;
  T r4 = a1 * b2;

  return d2_make_point(((r1 * quadix.p0.x) + (r2 * quadix.p1.x) + (r3 * quadix.p2.x) + (r4 * quadix.p3.x)) * (0.25),
      ((r1 * quadix.p0.y) + (r2 * quadix.p1.y) + (r3 * quadix.p2.y) + (r4 * quadix.p3.y)) * (0.25));
}

CC_INLINE DPOINT3 qu3_generate_random_point(const DQUADIX3 quadix)
{
  T a = (2 * d1_generate_random_value((1.0))) - 1;
  T b = (2 * d1_generate_random_value((1.0))) - 1;

  T a1 = (1.0) - a;
  T a2 = (1.0) + a;

  T b1 = (1.0) - b;
  T b2 = (1.0) + b;

  T r1 = a1 * b1;
  T r2 = a2 * b1;
  T r3 = a2 * b2;
  T r4 = a1 * b2;

  return d3_make_point(((r1 * quadix.p0.x) + (r2 * quadix.p1.x) + (r3 * quadix.p2.x) + (r4 * quadix.p3.x)) * (0.25),
      ((r1 * quadix.p0.y) + (r2 * quadix.p1.y) + (r3 * quadix.p2.y) + (r4 * quadix.p3.y)) * (0.25),
      ((r1 * quadix.p0.z) + (r2 * quadix.p1.z) + (r3 * quadix.p2.z) + (r4 * quadix.p3.z)) * (0.25));
}

CC_INLINE DPOINT2 rt_generate_random_point(const DRECTANGLE rectangle)
{
  return pt2_translate(min(rectangle.p0.x, rectangle.p1.x), min(rectangle.p0.y, rectangle.p1.y),
      d2_generate_random_point(T_ABS(rectangle.p1.x - rectangle.p0.x),
          T_ABS(rectangle.p1.y - rectangle.p0.y)));
}

CC_INLINE DPOINT3 box3_generate_random_point(const DBOX3 box)
{
  return pt3_translate(min(box.p0.x, box.p1.x), min(box.p0.y, box.p1.y), min(box.p0.z, box.p1.z),
      d3_generate_random_point(T_ABS(box.p1.x - box.p0.x),
          T_ABS(box.p1.y - box.p0.y),
          T_ABS(box.p1.z - box.p0.z)));
}

CC_INLINE void d2_generate_random_points(const T x1, const T y1,
    const T x2, const T y2,
    const int point_count,
    DPOINT2* out)
{
  int i;
  T dx = T_ABS(x2 - x1);
  T dy = T_ABS(y2 - y1);

  for (i = 0; i < point_count; ++i) {
    out[i] = pt2_translate(x1, y1, d2_generate_random_point(dx, dy));
  }
}

CC_INLINE void d3_generate_random_points(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const int point_count,
    DPOINT3* out)
{
  int i;
  T dx = T_ABS(x2 - x1);
  T dy = T_ABS(y2 - y1);
  T dz = T_ABS(z2 - z1);

  for (i = 0; i < point_count; ++i) {
    out[i] = pt3_translate(x1, y1, z1, d3_generate_random_point(dx, dy, dz));
  }
}

CC_INLINE void rt_generate_random_points(const DRECTANGLE rectangle, const int point_count, DPOINT2* out)
{
  d2_generate_random_points(rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y, point_count, out);
}

CC_INLINE void box3_generate_random_points(const DBOX3 box, const int point_count, DPOINT3* out)
{
  d3_generate_random_points(box.p0.x, box.p0.y, box.p0.z, box.p1.x, box.p1.y, box.p1.z, point_count, out);
}

CC_INLINE void se2_generate_random_points(const DSEGMENT2 segment, const int point_count, DPOINT2* out)
{
  int i;

  for (i = 0; i < point_count; ++i) {
    out[i] = se2_generate_random_point(segment);
  }
}

CC_INLINE void se3_generate_random_points(const DSEGMENT3 segment, const int point_count, DPOINT3* out)
{
  int i;

  for (i = 0; i < point_count; ++i) {
    out[i] = se3_generate_random_point(segment);
  }
}

CC_INLINE void tr2_generate_random_points(const DTRIANGLE2 triangle, const int point_count, DPOINT2* out)
{
  int i;

  for (i = 0; i < point_count; ++i) {
    out[i] = tr2_generate_random_point(triangle);
  }
}

CC_INLINE void tr3_generate_random_points(const DTRIANGLE3 triangle, const int point_count, DPOINT3* out)
{
  int i;

  for (i = 0; i < point_count; ++i) {
    out[i] = tr3_generate_random_point(triangle);
  }
}

CC_INLINE void qu2_generate_random_points(const DQUADIX2 quadix, const int point_count, DPOINT2* out)
{
  int i;

  for (i = 0; i < point_count; ++i) {
    out[i] = qu2_generate_random_point(quadix);
  }
}

CC_INLINE void qu3_generate_random_points(const DQUADIX3 quadix, const int point_count, DPOINT3* out)
{
  int i;

  for (i = 0; i < point_count; ++i) {
    out[i] = qu3_generate_random_point(quadix);
  }
}

CC_INLINE void ci2_generate_random_points(const DCIRCLE circle, const int point_count, DPOINT2* out)
{
  int i;
  DPOINT2 cpoint;
  DPOINT2 point_;
  cpoint.x = circle.x, cpoint.y = circle.y;

  for (i = 0; i < point_count; ++i) {
    T random_angle = d1_generate_random_value((360.0));
    point_.x = circle.x + circle.radius * sqrt(d1_generate_random_value((1.0)));
    point_.y = circle.y;
    out[i] = pt2_rotate_o(random_angle, point_, cpoint);
  }
}

CC_INLINE DPOINT2 d2_create_point_on_qbezier(const DPOINT2 start_point,
    const T ax, const T bx,
    const T ay, const T by,
    const T t)
{
  T tsqr = t * t;
  return d2_make_point(
      (ax * tsqr) + (bx * t) + start_point.x,
      (ay * tsqr) + (by * t) + start_point.y
      );
}

CC_INLINE DPOINT3 d3_create_point_on_qbezier(const DPOINT3 start_point,
    const T ax, const T bx,
    const T ay, const T by,
    const T az, const T bz,
    const T t)
{
  T tsqr = t * t;
  return d3_make_point(
      (ax * tsqr) + (bx * t) + start_point.x,
      (ay * tsqr) + (by * t) + start_point.y,
      (az * tsqr) + (bz * t) + start_point.z
      );
}

CC_INLINE DPOINT2 d2_create_point_on_cbezier(const DPOINT2 start_point,
    const T ax, const T bx, const T cx,
    const T ay, const T by, const T cy,
    const T t)
{
  T tsqr = t * t;
  T tcube = tsqr * t;
  return d2_make_point(
      (ax * tcube) + (bx * tsqr) + (cx * t) + start_point.x,
      (ay * tcube) + (by * tsqr) + (cy * t) + start_point.y
      );
}

CC_INLINE DPOINT3 d3_create_point_on_cbezier(const DPOINT3 start_point,
    const T ax, const T bx, const T cx,
    const T ay, const T by, const T cy,
    const T az, const T bz, const T cz,
    const T t)
{
  T tsqr = t * t;
  T tcube = tsqr * t;
  return d3_make_point(
      (ax * tcube) + (bx * tsqr) + (cx * t) + start_point.x,
      (ay * tcube) + (by * tsqr) + (cy * t) + start_point.y,
      (az * tcube) + (bz * tsqr) + (cz * t) + start_point.z
      );
}

CC_INLINE DPOINT2 pt2_create_point_on_qbezier(const DPOINT2 start_point,
    const DPOINT2* coeffs,
    const T t)
{
  return d2_create_point_on_qbezier(start_point,
      coeffs[0].x, coeffs[0].y,
      coeffs[1].x, coeffs[1].y, t);
}

CC_INLINE DPOINT3 pt3_create_point_on_qbezier(const DPOINT3 start_point,
    const DPOINT3* coeffs,
    const T t)
{
  return d3_create_point_on_qbezier(start_point,
      coeffs[0].x, coeffs[0].y, coeffs[0].z,
      coeffs[1].x, coeffs[1].y, coeffs[1].z, t);
}

CC_INLINE DPOINT2 pt2_create_point_on_cbezier(const DPOINT2 start_point,
    const DPOINT2* coeffs,
    const T t)
{
  return d2_create_point_on_cbezier(start_point,
      coeffs[0].x, coeffs[0].y,
      coeffs[1].x, coeffs[1].y,
      coeffs[2].x, coeffs[2].y, t);
}

CC_INLINE DPOINT3 pt3_create_point_on_cbezier(const DPOINT3 start_point,
    const DPOINT3* coeffs,
    const T t)
{
  return d3_create_point_on_cbezier(start_point,
      coeffs[0].x, coeffs[0].y, coeffs[0].z,
      coeffs[1].x, coeffs[1].y, coeffs[1].z,
      coeffs[2].x, coeffs[2].y, coeffs[2].z, t);
}

CC_INLINE void pt2_generate_qbezier(const DQUADRATIC_BEZIER2 bezier, DPOINT2* out, const int point_count)
{
  if (point_count) {
    int i;
    T t = (0.0), dt = (1.0) / ((1.0) * point_count - (1.0));
    DPOINT2 coeffs[2] = {0};
    pt2_calculate_qbezier_coefficients(bezier, coeffs);

    for (i = 0; i < point_count; t += dt, ++i) {
      out[i] = pt2_create_point_on_qbezier(bezier.p0, coeffs, t);
    }
  }
}

CC_INLINE void pt3_generate_qbezier(const DQUADRATIC_BEZIER3 bezier, DPOINT3* out, const int point_count)
{
  if (point_count) {
    int i;
    T t = (0.0);
    T dt = (1.0) / ((1.0) * point_count - (1.0));
    DPOINT3 coeffs[2] = {0};
    pt3_calculate_qbezier_coefficients(bezier, coeffs);

    for (i = 0; i < point_count; t += dt, ++i) {
      (*out++) = pt3_create_point_on_qbezier(bezier.p0, coeffs, t);
    }
  }
}

CC_INLINE void pt2_generate_cbezier(const DCUBIC_BEZIER2 bezier, DPOINT2* out, const int point_count)
{
  if (0 == point_count) {
    return;
  }
  else {
    int i;
    T t = (0.0);
    T dt = (1.0) / ((1.0) * point_count - (1.0));
    DPOINT2 coeffs[3] = {0};
    pt2_calculate_cbezier_coefficients(bezier, coeffs);

    for (i = 0; i < point_count; t += dt, ++i) {
      (*out++) = pt2_create_point_on_cbezier(bezier.p0, coeffs, t);
    }
  }
}

CC_INLINE void pt3_generate_cbezier(const DCUBIC_BEZIER3 bezier, DPOINT3* out, const int point_count)
{
  if (0 == point_count) {
    return;
  }
  else {
    int i;
    T t = (0.0);
    T dt = (1.0) / ((1.0) * point_count - (1.0));
    DPOINT3 coeffs[3] = {0};
    pt3_calculate_cbezier_coefficients(bezier, coeffs);

    for (i = 0; i < point_count; t += dt, ++i) {
      (*out++) = pt3_create_point_on_cbezier(bezier.p0, coeffs, t);
    }
  }
}

