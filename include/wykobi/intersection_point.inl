
CC_INLINE DPOINT2 d2_intersection_point(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  DPOINT2 pt;
  T dx1 = x2 - x1;
  T dx2 = x4 - x3;
  T dx3 = x1 - x3;

  T dy1 = y2 - y1;
  T dy2 = y1 - y3;
  T dy3 = y4 - y3;

  T ratio = dx1 * dy3 - dy1 * dx2;

  if (d_not_equal(ratio, (0.0))) {
    ratio = (dy2 * dx2 - dx3 * dy3) / ratio;
    pt.x = x1 + ratio * dx1;
    pt.y = y1 + ratio * dy1;
  }
  else {
    if (d_is_equal((dx1 * -dy2), (-dx3 * dy1))) {
      pt.x = x3;
      pt.y = y3;
    }
    else {
      pt.x = x4;
      pt.y = y4;
    }
  }

  return pt;
}

CC_INLINE DPOINT2 pt2_intersection_point(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d2_intersection_point(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, point4.x, point4.y);
}

CC_INLINE DPOINT2 sese2_intersection_point(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return pt2_intersection_point(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE DPOINT3 d3_intersection_point(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  DPOINT3 pt;
  T ux = x2 - x1;
  T uy = y2 - y1;
  T uz = z2 - z1;

  T vx = x4 - x3;
  T vy = y4 - y3;
  T vz = z4 - z3;

  T wx = x1 - x3;
  T wy = y1 - y3;
  T wz = z1 - z3;

  T a = (ux * ux + uy * uy + uz * uz);
  T b = (ux * vx + uy * vy + uz * vz);
  T c = (vx * vx + vy * vy + vz * vz);
  T d = (ux * wx + uy * wy + uz * wz);
  T e = (vx * wx + vy * wy + vz * wz);
  T dt = a * c - b * b;

  T sd = dt;
  T td = dt;

  T sn = (0.0);
  T tn = (0.0);
  T sc, tc, dx, dy, dz;

  if (d_is_equal(dt, (0.0))) {
    sn = (0.0);
    sd = (1.00);
    tn = e;
    td = c;
  }
  else {
    sn = (b * e - c * d);
    tn = (a * e - b * d);

    if (sn < (0.0)) {
      sn = (0.0);
      tn = e;
      td = c;
    }
    else if (sn > sd) {
      sn = sd;
      tn = e + b;
      td = c;
    }
  }

  if (tn < (0.0)) {
    tn = (0.0);

    if (-d < (0.0)) {
      sn = (0.0);
    }
    else if (-d > a) {
      sn = sd;
    }
    else {
      sn = -d;
      sd = a;
    }
  }
  else if (tn > td) {
    tn = td;

    if ((-d + b) < (0.0)) {
      sn = (0.0);
    }
    else if ((-d + b) > a) {
      sn = sd;
    }
    else {
      sn = (-d + b);
      sd = a;
    }
  }

  sc = (0.0);
  tc = (0.0);

  if (d_is_equal(sn, (0.0))) {
    sc = (0.0);
  }
  else {
    sc = sn / sd;
  }

  if (d_is_equal(tn, (0.0))) {
    tc = (0.0);
  }
  else {
    tc = tn / td;
  }

  dx = wx + (sc * ux) - (tc * vx);
  dy = wy + (sc * uy) - (tc * vy);
  dz = wz + (sc * uz) - (tc * vz);

  if ((dx * dx + dy * dy + dz * dz) <= sqr(fuzzy)) {
    pt.x = ((x1 + (sc * ux)) + (x3 + (tc * vx))) * (0.5);
    pt.y = ((y1 + (sc * uy)) + (y3 + (tc * vy))) * (0.5);
    pt.z = ((z1 + (sc * uz)) + (z3 + (tc * vz))) * (0.5);
  }
  else {
    pt.x = +InfinityT;
    pt.y = +InfinityT;
    pt.z = +InfinityT;
  }

  return pt;
}

CC_INLINE DPOINT3 pt3_intersection_point(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4)
{
  return d3_intersection_point(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z, point4.x, point4.y, point4.z);
}

CC_INLINE DPOINT3 sese3_intersection_point(const DSEGMENT3 segment1, const DSEGMENT3 segment2)
{
  return pt3_intersection_point(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE DPOINT2 seli2_intersection_point(const DSEGMENT2 segment, const DLINE2 line)
{
  return sese2_intersection_point(segment, pt2_make_segment(pt2_closest_point_on_line_from_point(line, segment.p0),
      pt2_closest_point_on_line_from_point(line, segment.p1)));
}

CC_INLINE DPOINT3 seli3_intersection_point(const DSEGMENT3 segment, const DLINE3 line)
{
  return sese3_intersection_point(segment, pt3_make_segment(pt3_closest_point_on_line_from_point(line, segment.p0),
      pt3_closest_point_on_line_from_point(line, segment.p1)));
}

CC_INLINE DPOINT3 sepl3_intersection_point(const DSEGMENT3 segment, const DPLANE3 plane)
{
  DVECTOR3 seg_vec = v3_sub(segment.p1, segment.p0);
  T denom = v3_dot_product(seg_vec, plane.normal);
  DPOINT3 ipoint = pt3_degenerate();

  if (d_not_equal(denom, (0.0))) {
    T t = -ptpl3_distance(segment.p0, plane) / denom;

    if ((t > (0.0)) && (t < (1.0))) {
      ipoint = v3_add(segment.p0, v3_scale(v3_sub(segment.p1, segment.p0), t));
    }
  }

  return ipoint;
}

#define SIMPLEX_TO_BEZIER_INTERSECTION_POINT(PointType, simplex, bezier, out, steps, calculate_bezier_coefficients, create_point_on_bezier, make_segment, intersect_at) \
  { \
    if (steps>0) { \
      int i, j=0; \
      T t = (0.0), dt = (1.0) / ((1.0) * steps - (1.0)); \
      PointType ipoint, previous_point, coeffs[3]; \
      calculate_bezier_coefficients(bezier, coeffs); \
      previous_point = create_point_on_bezier(bezier.p0, coeffs, t); \
      t += dt; \
      for (i = 1; i < steps; t += dt, ++i) { \
        PointType current_point = create_point_on_bezier(bezier.p0, coeffs, t); \
        if (intersect_at(make_segment(previous_point, current_point), simplex, &ipoint)) { out[j++] = ipoint; } \
        previous_point = current_point; \
      } return j;\
    } return 0; \
  }

CC_INLINE int seqb2_intersection_point(const DSEGMENT2 segment, const DQUADRATIC_BEZIER2 bezier, DPOINT2* out, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECTION_POINT(DPOINT2, segment, bezier, out, steps, pt2_calculate_qbezier_coefficients, pt2_create_point_on_qbezier, pt2_make_segment, se2_intersect_at);

}

CC_INLINE int secb2_intersection_point(const DSEGMENT2 segment, const DCUBIC_BEZIER2 bezier, DPOINT2* out, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECTION_POINT(DPOINT2, segment, bezier, out, steps, pt2_calculate_cbezier_coefficients, pt2_create_point_on_cbezier, pt2_make_segment, se2_intersect_at);
}

CC_INLINE int seqb3_intersection_point(const DSEGMENT3 segment, const DQUADRATIC_BEZIER3 bezier, DPOINT3* out, const int steps)
{
  //SIMPLEX_TO_BEZIER_INTERSECTION_POINT(DPOINT3, segment, bezier, out, steps, pt3_calculate_qbezier_coefficients, pt3_create_point_on_qbezier, pt3_make_segment, se3_intersect_at );
  //simplex_to_bezier_intersection_point<T, 3, DSEGMENT3, DQUADRATIC_BEZIER3, DPOINT2*>(segment, bezier, out, steps);
}

CC_INLINE int secb3_intersection_point(const DSEGMENT3 segment, const DCUBIC_BEZIER3 bezier, DPOINT3* out, const int steps)
{
  //simplex_to_bezier_intersection_point<T, 3, DSEGMENT3, DCUBIC_BEZIER3, DPOINT2*>(segment, bezier, out, steps);
}

CC_INLINE DPOINT2 lili2_intersection_point(const DLINE2 line1, const DLINE2 line2)
{
  T dx1 = line1.p0.x - line1.p1.x;
  T dx2 = line2.p0.x - line2.p1.x;
  T dx3 = line2.p1.x - line1.p1.x;
  T dy1 = line1.p0.y - line1.p1.y;
  T dy2 = line2.p0.y - line2.p1.y;
  T dy3 = line2.p1.y - line1.p1.y;

  T ratio, det = (dx2 * dy1) - (dy2 * dx1);

  DPOINT2 point_ = d2_make_point((0.0), (0.0));

  if (d_is_equal(det, (0.0))) {
    if (d_is_equal((dx2 * dy3), (dy2 * dx3))) {
      point_.x = line2.p1.x;
      point_.y = line2.p1.y;
    }

    return point_;
  }

  ratio = ((dx1 * dy3) - (dy1 * dx3)) / det;
  point_.x = (ratio * dx2) + line2.p1.x;
  point_.y = (ratio * dy2) + line2.p1.y;
  return point_;
}

CC_INLINE DPOINT3 d3_intersection_point_line_to_line(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  DPOINT3 pt;
  T ux = x2 - x1;
  T uy = y2 - y1;
  T uz = z2 - z1;

  T vx = x4 - x3;
  T vy = y4 - y3;
  T vz = z4 - z3;

  T wx = x1 - x3;
  T wy = y1 - y3;
  T wz = z1 - z3;

  T a = (ux * ux + uy * uy + uz * uz);
  T b = (ux * vx + uy * vy + uz * vz);
  T c = (vx * vx + vy * vy + vz * vz);
  T d = (ux * wx + uy * wy + uz * wz);
  T e = (vx * wx + vy * wy + vz * wz);
  T dt = a * c - b * b;
  T sc = (0.0);
  T tc = (0.0);
  T dx, dy, dz;

  if (d_is_equal(dt, (0.0))) {
    sc = (0.0);

    if (b > c) {
      tc = d / b;
    }
    else {
      tc = e / c;
    }
  }
  else {
    sc = (b * e - c * d) / dt;
    tc = (a * e - b * d) / dt;
  }

  dx = wx + (sc * ux) - (tc * vx);
  dy = wy + (sc * uy) - (tc * vy);
  dz = wz + (sc * uz) - (tc * vz);

  if ((dx * dx + dy * dy + dz * dz) <= sqr(fuzzy)) {
    pt.x = ((x1 + (sc * ux)) + (x3 + (tc * vx))) * (0.5);
    pt.y = ((y1 + (sc * uy)) + (y3 + (tc * vy))) * (0.5);
    pt.z = ((z1 + (sc * uz)) + (z3 + (tc * vz))) * (0.5);
  }
  else {
    pt.x = +InfinityT;
    pt.y = +InfinityT;
    pt.z = +InfinityT;
  }

  return pt;
}

CC_INLINE DPOINT3 lili3_intersection_point(const DLINE3 line1, const DLINE3 line2)
{
  return d3_intersection_point_line_to_line(line1.p0.x, line1.p0.y, line1.p0.z, line1.p1.x, line1.p1.y, line1.p1.z,
      line2.p0.x, line2.p0.y, line2.p0.z, line2.p1.x, line2.p1.y, line2.p1.z);
}

CC_INLINE int cici2_intersection_point(const DCIRCLE circle1, const DCIRCLE circle2, DPOINT2* out)
{
  T dist = d2_distance(circle1.x, circle1.y, circle2.x, circle2.y);

  T dstsqr = dist * dist;
  T r1sqr = circle1.radius * circle1.radius;
  T r2sqr = circle2.radius * circle2.radius;

  T a = (dstsqr - r2sqr + r1sqr) / (2 * dist);
  T h = sqrt(r1sqr - (a * a));

  T ratio_a = a / dist;
  T ratio_h = h / dist;

  T dx = circle2.x - circle1.x;
  T dy = circle2.y - circle1.y;

  T phix = circle1.x + (ratio_a * dx);
  T phiy = circle1.y + (ratio_a * dy);

  dx = dx * ratio_h;
  dy = dy * ratio_h;

  out[0].x = phix + dy;
  out[0].y = phiy - dx;

  out[1].x = phix - dy;
  out[1].y = phiy + dx;
  return 2;
}

CC_INLINE int setr2_intersection_point(const DSEGMENT2 segment, const DTRIANGLE2 triangle, DPOINT2* out)
{
  DPOINT2 pt = {0};
  int i = 0, int_count = 0;

  if (se2_intersect_at(segment, tr2_edge(triangle, 0), &pt)) {
    out[i++] = pt;
    ++int_count;
  }

  if (se2_intersect_at(segment, tr2_edge(triangle, 1), &pt)) {
    if (int_count) {
      out[i++] = pt;
      return i;
    }

    out[i++] = pt;
    ++int_count;
  }

  if (se2_intersect_at(segment, tr2_edge(triangle, 2), &pt)) {
    if (int_count) {
      out[i++] = pt;
      return i;
    }

    out[i++] = pt;
  }

  return i;
}

CC_INLINE int litr3_intersection_point(const DLINE3 line, const DTRIANGLE3 triangle, DPOINT3* ipoint)
{
  DVECTOR3 dir = v3_sub(line.p1, line.p0);
  DVECTOR3 u = v3_sub(triangle.p1, triangle.p0);
  DVECTOR3 v = v3_sub(triangle.p2, triangle.p0);
  DVECTOR3 n = v3_mul(u, v);
  DVECTOR3 w = v3_sub(line.p0, triangle.p0);
  T a = -v3_dot_product(n, w);
  T b = v3_dot_product(n, dir);
  T r = a / b;
  *ipoint = v3_add(line.p0, v3_scale(dir, r));
  return 1;
}

CC_INLINE DPOINT3 lipl3_intersection_point(const DLINE3 line, const DPLANE3 plane)
{
  DVECTOR3 line_vec = v3_sub(line.p1, line.p0);
  T denom = v3_dot_product(line_vec, plane.normal);
  DPOINT3 ipoint = pt3_degenerate();

  if (d_not_equal(denom, (0.0))) {
    T t = -ptpl3_distance(line.p0, plane) / denom;
    ipoint = v3_add(line.p0, v3_scale(v3_sub(line.p1, line.p0), t));
  }

  return ipoint;
}

CC_INLINE int dci2_intersection_point(const T x1, const T y1, const T x2, const T y2, const T cx, const T cy, const T radius, DPOINT2* out)
{
  int i = 0;
  BOOL p1_in_circle = d2_point_in_circle(x1, y1, cx, cy, radius);
  BOOL p2_in_circle = d2_point_in_circle(x2, y2, cx, cy, radius);
  DPOINT2 p = {0};

  if (p1_in_circle && p2_in_circle) {
    out[i++] = d2_make_point(x1, y1);
    out[i++] = d2_make_point(x2, y2);
    return i;
  }

  if (p1_in_circle || p2_in_circle) {
    p = d2_closest_point_on_line_from_point(x1, y1, x2, y2, cx, cy);

    if (p1_in_circle) {
      T h = d2_distance(p.x, p.y, cx, cy);
      T a = sqrt((radius * radius) - (h * h));
      out[i++] = d2_make_point(x1, y1);
      out[i++] = d2_project_point(p.x, p.y, x2, y2, a);
    }
    else if (p2_in_circle) {
      T h = d2_distance(p.x, p.y, cx, cy);
      T a = sqrt((radius * radius) - (h * h));
      out[i++] = d2_make_point(x2, y2);
      out[i++] = d2_project_point(p.x, p.y, x1, y1, a);
    }

    return i;
  }

  p = d2_closest_point_on_segment_from_point(x1, y1, x2, y2, cx, cy);

  if (
      (d_is_equal(x1, p.x) && d_is_equal(y1, p.y)) ||
      (d_is_equal(x2, p.x) && d_is_equal(y2, p.y))
  ) {
    return i;
  }
  else {
    T h = d2_distance(p.x, p.y, cx, cy);

    if (h > radius) {
      return i;
    }
    else if (d_is_equal(h, radius)) {
      out[i++] = d2_make_point(x1, y1);
      return i;
    }
    else if (d_is_equal(h, (0.0))) {
      out[i++] = d2_project_point(cx, cy, x1, y1, radius);
      out[i++] = d2_project_point(cx, cy, x2, y2, radius);
      return i;
    }
    else {
      T a = sqrt((radius * radius) - (h * h));
      out[i++] = d2_project_point(p.x, p.y, x1, y1, a);
      out[i++] = d2_project_point(p.x, p.y, x2, y2, a);
    }
  }

  return i;
}

CC_INLINE int seci2_intersection_point(const DSEGMENT2 segment, const DCIRCLE circle, DPOINT2* out)
{
  return dci2_intersection_point(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, circle.x, circle.y, circle.radius, out);
}

CC_INLINE int lici2_intersection_point(const DLINE2 line, const DCIRCLE circle, DPOINT2* out)
{
  int i = 0;
  T a = sqr(line.p1.x - line.p0.x) + sqr(line.p1.y - line.p0.y);
  T b = (2.0) * ((line.p1.x - line.p0.x) * (line.p0.x - circle.x) + (line.p1.y - line.p0.y) * (line.p0.y - circle.y));
  T c = sqr(circle.x) + sqr(circle.y) + sqr(line.p0.x) + sqr(line.p0.y) - (2.0) * (circle.x * line.p0.x + circle.y * line.p0.y) - sqr(circle.radius);
  T det = b * b - (4.0) * a * c;

  if (det < (0.0)) {
    return i;
  }
  else if (d_is_equal(det, (0.0))) {
    T delta = -b / ((2.0) * a);
    out[i++] = d2_make_point(line.p0.x + delta * (line.p1.x - line.p0.x), line.p0.y + delta * (line.p1.y - line.p0.y));
    return i;
  }
  else if (det > (0.0)) {
    T sqrt_det = sqrt(det);
    T delta = (-b + sqrt_det) / ((2.0) * a);
    out[i++] = d2_make_point(line.p0.x + delta * (line.p1.x - line.p0.x), line.p0.y + delta * (line.p1.y - line.p0.y));

    delta = (-b - sqrt_det) / ((2.0) * a);
    out[i++] = d2_make_point(line.p0.x + delta * (line.p1.x - line.p0.x), line.p0.y + delta * (line.p1.y - line.p0.y));
    return i;
  }

  return i;
}

CC_INLINE int lisp3_intersection_point(const DLINE3 line, const DSPHERE sphere, DPOINT3* out)
{
  int i = 0;
  T a = sqr(line.p1.x - line.p0.x) + sqr(line.p1.y - line.p0.y) + sqr(line.p1.z - line.p0.z);
  T b = (2.0) * ((line.p1.x - line.p0.x) * (line.p0.x - sphere.x) + (line.p1.y - line.p0.y) * (line.p0.y - sphere.y) + (line.p1.z - line.p0.z) * (line.p0.z - sphere.z));
  T c = sqr(sphere.x) + sqr(sphere.y) + sqr(sphere.z) + sqr(line.p0.x) + sqr(line.p0.y) + sqr(line.p0.z) - (2.0) * (sphere.x * line.p0.x + sphere.y * line.p0.y + sphere.z * line.p0.z) - sqr(sphere.radius);
  T det = b * b - (4.0) * a * c ;

  if (det < (0.0)) {
    return i;
  }
  else if (d_is_equal(det, (0.0))) {
    T delta = -b / ((2.0) * a);
    out[i++] = d3_make_point(line.p0.x + delta * (line.p1.x - line.p0.x), line.p0.y + delta * (line.p1.y - line.p0.y), line.p0.z + delta * (line.p1.z - line.p0.z));
    return i;
  }
  else if (det > (0.0)) {
    T delta = (-b + sqrt(det)) / ((2.0) * a);
    out[i++] = d3_make_point(line.p0.x + delta * (line.p1.x - line.p0.x), line.p0.y + delta * (line.p1.y - line.p0.y), line.p0.z + delta * (line.p1.z - line.p0.z));
    delta = (-b - sqrt(det)) / ((2.0) * a);
    out[i++] = d3_make_point(line.p0.x + delta * (line.p1.x - line.p0.x), line.p0.y + delta * (line.p1.y - line.p0.y), line.p0.z + delta * (line.p1.z - line.p0.z));
    return i;
  }

  return i;
}

CC_INLINE int sesp3_intersection_point(const DSEGMENT3 segment, const DSPHERE sphere, DPOINT3* out)
{
  int i, n, j = 0;
  DPOINT3 ipoint_[3];
  n = lisp3_intersection_point(se3_make_line(segment), sphere, ipoint_);

  for (i = 0; i < n; ++i) {
    if (se3_point_in_box(ipoint_[i], segment)) {
      out[j++] = ipoint_[i];
    }
  }

  return j;
}

CC_INLINE DPOINT2 rayray2_intersection_point(const DRAY2 ray1, const DRAY2 ray2)
{
  T denom = (ray2.direction.y * ray1.direction.x - ray2.direction.x * ray1.direction.y);

  if (denom != (0.0)) {
    T ta = (ray2.direction.x * (ray1.origin.y - ray2.origin.y) - ray2.direction.y * (ray1.origin.x - ray2.origin.x)) / denom;
    T tb = (ray1.direction.y * (ray2.origin.x - ray1.origin.x) - ray1.direction.x * (ray2.origin.y - ray1.origin.y)) / denom;

    if ((d_greater_than_or_equal(ta, (0.0)) && d_greater_than_or_equal(tb, (0.0)))) {
      return d2_make_point(ray1.origin.x + (ray1.direction.x * ta), ray1.origin.y + (ray1.direction.y * ta));
    }
    else {
      return pt2_degenerate();
    }

  }
  else if (pt2_point_on_ray(ray2.origin, ray1)) {
    return ray2.origin;
  }
  else if (pt2_point_on_ray(ray1.origin, ray2)) {
    return ray1.origin;
  }
  else {
    return pt2_degenerate();
  }
}

CC_INLINE DPOINT3 raytr3_intersection_point(const DRAY3 ray, const DTRIANGLE3 triangle)
{
  T edge1_x = triangle.p1.x - triangle.p0.x;
  T edge1_y = triangle.p1.y - triangle.p0.y;
  T edge1_z = triangle.p1.z - triangle.p0.z;

  T edge2_x = triangle.p2.x - triangle.p0.x;
  T edge2_y = triangle.p2.y - triangle.p0.y;
  T edge2_z = triangle.p2.z - triangle.p0.z;

  T pvec_x = ray.direction.y * edge2_z - ray.direction.z * edge2_y;
  T pvec_y = ray.direction.z * edge2_x - ray.direction.x * edge2_z;
  T pvec_z = ray.direction.x * edge2_y - ray.direction.y * edge2_x;

  T det = edge1_x * pvec_x + edge1_y * pvec_y + edge1_z * pvec_z;

  if (d_is_equal(det, (0.0))) {
    return pt3_degenerate();
  }
  else {
    T inv_det = (1.0) / det;

    T tvec_x = ray.origin.x - triangle.p0.x;
    T tvec_y = ray.origin.y - triangle.p0.y;
    T tvec_z = ray.origin.z - triangle.p0.z;

    T u = (tvec_x * pvec_x + tvec_y * pvec_y + tvec_z * pvec_z) * inv_det;

    if (u < 0.0 || u > 1.0) {
      return pt3_degenerate();
    }
    else {
      T qvec_x = tvec_y * edge1_z - tvec_z * edge1_y;
      T qvec_y = tvec_z * edge1_x - tvec_x * edge1_z;
      T qvec_z = tvec_x * edge1_y - tvec_y * edge1_x;

      T t, v = (ray.direction.x * qvec_x + ray.direction.y * qvec_y + ray.direction.z * qvec_z) * inv_det;

      if ((v < 0.0) || ((u + v) > 1.0)) {
        return pt3_degenerate();
      }

      t = (edge2_x * qvec_x + edge2_y * qvec_y + edge2_z * qvec_z) * inv_det;

      return d3_make_point(ray.origin.x + (ray.direction.x * t), ray.origin.y + (ray.direction.y * t), ray.origin.z + (ray.direction.z * t));
    }
  }
}

CC_INLINE DPOINT3 raypl3_intersection_point(const DRAY3 ray, const DPLANE3 plane)
{
  T denom = v3_dot_product(ray.direction, plane.normal);
  DPOINT3 ipoint = pt3_degenerate();

  if (d_not_equal(denom, (0.0))) {
    T t = -ptpl3_distance(ray.origin, plane) / denom;

    if (t >= (0.0)) {
      ipoint = v3_add(ray.origin, v3_scale(ray.direction, t));
    }
  }

  return ipoint;
}

CC_INLINE int rayci2_intersection_point(const DRAY2 ray, const DCIRCLE circle, DPOINT2* out)
{
  int i, j = 0, n;
  DPOINT2 ipoint_[3] = {0};
  n = lici2_intersection_point(ray2_make_line(ray), circle, ipoint_);

  if (n > 0) {
    for (i = 0; i < n; ++i) {
      if (pt2_point_on_ray(ipoint_[i], ray)) {
        out[j++] = ipoint_[i];
      }
    }
  }

  return j;
}

CC_INLINE int raysp3_intersection_point(const DRAY3 ray, const DSPHERE sphere, DPOINT3* out)
{
  int i = 0;
  DVECTOR3 v = v3_sub(ray.origin, sp_make_point(sphere));
  T a, det, diff = v3_dot_product(v, v) - sqr(sphere.radius);

  if (d_less_than_or_equal(diff, (0.0))) {
    T b = v3_dot_product(ray.direction, v);
    T t = -b + sqrt(sqr(b) - diff);
    out[i++] = v3_add(ray.origin, v3_scale(ray.direction, t));
    return i;
  }

  a = v3_dot_product(ray.direction, v);

  if (d_greater_than_or_equal(a, (0.0))) {
    return i;
  }

  det = sqr(a) - diff;

  if (det < (0.0)) {
    return i;
  }
  else if (d_greater_than_or_equal(det, (0.0))) {
    T root = sqrt(det);
    T t1 = -a - root;
    T t2 = -a + root;
    out[i++] = v3_add(ray.origin, v3_scale(ray.direction, t1));
    out[i++] = v3_add(ray.origin, v3_scale(ray.direction, t2));
  }
  else {
    out[i++] = v3_sub(ray.origin, v3_scale(ray.direction, a));
  }

  return i;
}

