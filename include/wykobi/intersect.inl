
CC_INLINE BOOL se2_intersect(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return pt2_intersect(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL se2_intersect_vertical_horizontal(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return (((segment1.p0.y <= segment2.p0.y) && (segment2.p0.y <= segment1.p1.y)) ||
      ((segment1.p1.y <= segment2.p0.y) && (segment2.p0.y <= segment1.p0.y))) &&
      (
          ((segment2.p0.x <= segment1.p0.x) && (segment1.p0.x <= segment2.p1.x)) ||
          ((segment2.p1.x <= segment1.p0.x) && (segment1.p0.x <= segment2.p0.x))
      );
}

CC_INLINE BOOL se2_intersect_vertical_vertical(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return (segment1.p0.x == segment2.p0.x) &&
      (
          ((segment1.p0.y <= segment2.p0.y) && (segment2.p0.y <= segment1.p1.y)) ||
          ((segment1.p0.y <= segment2.p1.y) && (segment2.p1.y <= segment1.p1.y))
      );
}

CC_INLINE BOOL se2_intersect_horizontal_horizontal(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return (segment1.p0.y == segment2.p0.y) &&
      (
          ((segment1.p0.x <= segment2.p0.x) && (segment2.p0.x <= segment1.p1.x)) ||
          ((segment1.p0.x <= segment2.p1.x) && (segment2.p1.x <= segment1.p1.x))
      );
}

CC_INLINE BOOL se2_intersect_at(const DSEGMENT2 segment1, const DSEGMENT2 segment2, DPOINT2* i_point)
{
  return d2_intersect_at(segment1.p0.x, segment1.p0.y, segment1.p1.x, segment1.p1.y,  segment2.p0.x, segment2.p0.y, segment2.p1.x, segment2.p1.y,  i_point);
}

CC_INLINE BOOL se3_intersect(const DSEGMENT3 segment1, const DSEGMENT3 segment2)
{
  return pt3_intersect(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL setr2_intersect(const DSEGMENT2 segment, const DTRIANGLE2 triangle)
{
  return se2_intersect(segment, pt2_make_segment(triangle.p0, triangle.p1)) ||
      se2_intersect(segment, pt2_make_segment(triangle.p1, triangle.p2)) ||
      se2_intersect(segment, pt2_make_segment(triangle.p2, triangle.p0)) ||
      tr2_point_in_triangle(segment.p0, triangle) ||
      tr2_point_in_triangle(segment.p1, triangle);
}

CC_INLINE BOOL sequ2_intersect(const DSEGMENT2 segment, const DQUADIX2 quadix)
{
  return setr2_intersect(segment, pt2_make_triangle(quadix.p0, quadix.p1, quadix.p2)) ||
      setr2_intersect(segment, pt2_make_triangle(quadix.p0, quadix.p2, quadix.p3));
}

CC_INLINE BOOL seli2_intersect(const DSEGMENT2 segment, const DLINE2 line)
{
  return (li2_orientation(line, segment.p0) * li2_orientation(line, segment.p1) <= 0);
}

CC_INLINE BOOL seli3_intersect(const DSEGMENT3 segment, const DLINE3 line)
{
  return se3_intersect(segment, pt3_make_segment(pt3_closest_point_on_line_from_point(line, segment.p0),
      pt3_closest_point_on_line_from_point(line, segment.p1)));
}

CC_INLINE BOOL sebox3_intersect(const DSEGMENT3 segment, const DBOX3 box)
{
  T cx = (box.p0.x + box.p1.x) * (0.5);
  T cy = (box.p0.y + box.p1.y) * (0.5);
  T cz = (box.p0.z + box.p1.z) * (0.5);

  T ex = box.p1.x - cx;
  T ey = box.p1.y - cy;
  T ez = box.p1.z - cz;

  T mx = (segment.p0.x + segment.p1.x) * (0.5) - cx;
  T my = (segment.p0.y + segment.p1.y) * (0.5) - cy;
  T mz = (segment.p0.z + segment.p1.z) * (0.5) - cz;

  T dx = segment.p1.x - mx;
  T dy = segment.p1.y - my;
  T dz = segment.p1.z - mz;
  T adx, ady, adz;

  adx = T_ABS(dx);

  if (T_ABS(mx) > ex + adx) {
    return false;
  }

  ady = T_ABS(dy);

  if (T_ABS(my) > ey + ady) {
    return false;
  }

  adz = T_ABS(dz);

  if (T_ABS(mz) > ez + adz) {
    return false;
  }

  adx += (Epsilon);
  ady += (Epsilon);
  adz += (Epsilon);

  if (T_ABS(my * dz - mz * dy) > (ey * adz + ez * ady)) {
    return false;
  }

  if (T_ABS(mz * dx - mx * dz) > (ex * adz + ez * adx)) {
    return false;
  }

  if (T_ABS(mx * dy - my * dx) > (ex * ady + ey * adx)) {
    return false;
  }

  return true;
}

CC_INLINE BOOL seci2_intersect(const DSEGMENT2 segment, const DCIRCLE circle)
{
  DPOINT2 p = d2_closest_point_on_segment_from_point(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, circle.x, circle.y);
  return (d2_lay_distance(p.x, p.y, circle.x, circle.y) <= (circle.radius * circle.radius));
}

#define SIMPLEX_TO_BEZIER_INTERSECT(PointType, simplex, bezier, steps, bezier_coef_func, create_point_on_bezier_func, make_segment_func, intersect_func) \
  { \
    if (steps>0) { int i; \
      T t = (0.0), dt = (1.0) / ((1.0) * steps - (1.0)); \
      PointType previous_point, coeffs[3]; \
      bezier_coef_func(bezier, coeffs); \
      previous_point = create_point_on_bezier_func(bezier.p0, coeffs, t); \
      t += dt; \
      for (i = 1; i < steps; ++i) { \
        PointType current_point = create_point_on_bezier_func(bezier.p0, coeffs, t); \
        if (intersect_func(make_segment_func(previous_point, current_point), simplex)) { return true; } \
        previous_point = current_point; \
        t += dt; \
      } \
    } \
  }

CC_INLINE BOOL seqb2_intersect(const DSEGMENT2 segment, const DQUADRATIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, segment, bezier, steps, pt2_calculate_qbezier_coefficients, pt2_create_point_on_qbezier, pt2_make_segment, se2_intersect);
  return false;
}

CC_INLINE BOOL secb2_intersect(const DSEGMENT2 segment, const DCUBIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, segment, bezier, steps, pt2_calculate_cbezier_coefficients, pt2_create_point_on_cbezier, pt2_make_segment, se2_intersect);
  return false;
}

CC_INLINE BOOL sesp3_intersect(const DSEGMENT3 segment, const DSPHERE sphere)
{
  T a = se3_lay_distance(segment);
  T b = 2 * ((segment.p1.x - segment.p0.x) * (segment.p0.x - sphere.x) + (segment.p1.y - segment.p0.y) * (segment.p0.y - sphere.y) + (segment.p1.z - segment.p0.z) * (segment.p0.z - sphere.z));
  T c = sqr(sphere.x) + sqr(sphere.y) + sqr(sphere.z) + sqr(segment.p1.x) + sqr(segment.p1.y) + sqr(segment.p1.z) - 2 * (sphere.x * segment.p1.x + sphere.y * segment.p1.y + sphere.z * segment.p1.z) - sqr(sphere.radius);
  //((b * b - 4 * a * c) >= 0)
  return d_greater_than_or_equal((b * b - 4 * a * c), (0.0));
}

CC_INLINE BOOL sepl3_intersect(const DSEGMENT3 segment, const DPLANE3 plane)
{
  T signed_dist1 = v3_dot_product(plane.normal, pt3_make_vector(segment.p0)) - plane.constant;
  T signed_dist2 = v3_dot_product(plane.normal, pt3_make_vector(segment.p1)) - plane.constant;
  signed_dist1 = (d_is_equal(T_ABS(signed_dist1), (0.0))) ? (0.0) : signed_dist1;
  signed_dist2 = (d_is_equal(T_ABS(signed_dist2), (0.0))) ? (0.0) : signed_dist2;
  return d_less_than_or_equal(signed_dist1 * signed_dist2, (0.0));
}

CC_INLINE BOOL seqb3_intersect(const DSEGMENT3 segment, const DQUADRATIC_BEZIER3 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT3, segment, bezier, steps, pt3_calculate_qbezier_coefficients, pt3_create_point_on_qbezier, pt3_make_segment, se3_intersect);
  return false;
}

CC_INLINE BOOL secb3_intersect(const DSEGMENT3 segment, const DCUBIC_BEZIER3 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT3, segment, bezier, steps, pt3_calculate_cbezier_coefficients, pt3_create_point_on_cbezier, pt3_make_segment, se3_intersect);
  return false;
}

CC_INLINE BOOL litr2_intersect(const DLINE2 line, const DTRIANGLE2 triangle)
{
  int or1, or2;
  or1 = pt2_orientation(line.p0, line.p1, triangle.p0);

  if (0 == or1) {
    return true;
  }

  or2 = pt2_orientation(line.p0, line.p1, triangle.p1);

  if (or2 != or1) {
    return true;
  }

  or2 = pt2_orientation(line.p0, line.p1, triangle.p2);

  return (or2 != or1);
}

CC_INLINE BOOL liqu2_intersect(const DLINE2 line, const DQUADIX2 quadix)
{
  int or1, or2;
  or1 = pt2_orientation(line.p0, line.p1, quadix.p0);

  if (0 == or1) {
    return true;
  }

  or2 = pt2_orientation(line.p0, line.p1, quadix.p1);

  if (or2 != or1) {
    return true;
  }

  or2 = pt2_orientation(line.p0, line.p1, quadix.p2);

  if (or2 != or1) {
    return true;
  }

  or2 = pt2_orientation(line.p0, line.p1, quadix.p3);

  return (or2 != or1);
}

CC_INLINE BOOL line_to_line_intersect(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3,
    const T x4, const T y4)
{
  if (d_not_equal((x1 - x2) * (y3 - y4), (y1 - y2) * (x3 - x4))) {
    return true;
  }
  else if (d2_collinear(x1, y1, x2, y2, x3, y3)) {
    return true;
  }
  else {
    return false;
  }
}

CC_INLINE BOOL li2_line_to_line_intersect(const DLINE2 line1, const DLINE2 line2)
{
  return line_to_line_intersect(line1.p0.x, line1.p0.y, line1.p1.x, line1.p1.y,
      line2.p0.x, line2.p0.y, line2.p1.x, line2.p1.y);
}

CC_INLINE BOOL li2_intersect(const DLINE2 line1, const DLINE2 line2)
{
  return li2_line_to_line_intersect(line1, line2);
}

CC_INLINE BOOL lici2_intersect(const DLINE2 line, const DCIRCLE circle)
{
  /*
  It is assumed that an intersection of a circle by a line
  is either a full intersection (2 points), partial intersection
  (1 point), or tangential.
  anything else will result in a false output.
  */
  T x1 = line.p0.x - circle.x;
  T y1 = line.p0.y - circle.y;
  T x2 = line.p1.x - circle.x;
  T y2 = line.p1.y - circle.y;
  return d_greater_than_or_equal(((circle.radius * circle.radius) * d2_lay_distance(x1, y1, x2, y2) - sqr(x1 * y2 - x2 * y1)), (0.0));
}

CC_INLINE BOOL liqb2_intersect(const DLINE2 line, const DQUADRATIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, line, bezier, steps, pt2_calculate_qbezier_coefficients, pt2_create_point_on_qbezier, pt2_make_segment, seli2_intersect);
  return false;
}

CC_INLINE BOOL licb2_intersect(const DLINE2 line, const DCUBIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, line, bezier, steps, pt2_calculate_cbezier_coefficients, pt2_create_point_on_cbezier, pt2_make_segment, seli2_intersect);
  return false;
}

CC_INLINE BOOL litr3_intersect(const DLINE3 line, const DTRIANGLE3 triangle)
{
  DVECTOR3 diff = v3_sub(line.p0, triangle.p0);
  DVECTOR3 line_dir = v3_sub(line.p1, line.p0);
  DVECTOR3 edge1 = v3_sub(triangle.p1, triangle.p0);
  DVECTOR3 edge2 = v3_sub(triangle.p2, triangle.p0);
  DVECTOR3 normal = v3_mul(edge1, edge2);

  T denom = v3_dot_product(normal, line_dir);
  T sign = 0.0, val1, val2;

  if (denom > (0.0)) {
    sign = (1.0);
  }
  else if (denom < (0.0)) {
    sign = (-1.0);
    denom = -denom;
  }
  else {
    return false;
  }

  val1 = sign * v3_dot_product(line_dir, v3_mul(diff, edge2));

  if (d_greater_than_or_equal(val1, (0.0))) {
    val2 = sign * v3_dot_product(line_dir, v3_mul(diff, edge1));

    if (d_greater_than_or_equal(val2, (0.0))) {
      if (val1 + val2 <= denom) {
        return true;
      }
    }
  }

  return false;
}

CC_INLINE BOOL lipl3_intersect(const DLINE3 line, const DPLANE3 plane)
{
  return d_not_equal(v3_dot_product(v3_sub(line.p1, line.p0), plane.normal), (0.0));
}

CC_INLINE BOOL lisp3_intersect(const DLINE3 line, const DSPHERE sphere)
{
  T a = sqr(line.p1.x - line.p0.x) + sqr(line.p1.y - line.p0.y) + sqr(line.p1.z - line.p0.z);
  T b = (2.0) * ((line.p1.x - line.p0.x) * (line.p0.x - sphere.x) +
      (line.p1.y - line.p0.y) * (line.p0.y - sphere.y) +
      (line.p1.z - line.p0.z) * (line.p0.z - sphere.z));
  T c = sqr(sphere.x) + sqr(sphere.y) + sqr(sphere.z) +
      sqr(line.p0.x) + sqr(line.p0.y) + sqr(line.p0.z) -
      (2.0) * (sphere.x * line.p0.x + sphere.y * line.p0.y + sphere.z * line.p0.z) - sqr(sphere.radius);
  return d_greater_than_or_equal(b * b - 4 * a * c, (0.0));
}

CC_INLINE BOOL liqb3_intersect(const DLINE3 line, const DQUADRATIC_BEZIER3 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT3, line, bezier, steps, pt3_calculate_qbezier_coefficients, pt3_create_point_on_qbezier, pt3_make_segment, seli3_intersect);
  return false;
}

CC_INLINE BOOL licb3_intersect(const DLINE3 line, const DCUBIC_BEZIER3 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT3, line, bezier, steps, pt3_calculate_cbezier_coefficients, pt3_create_point_on_cbezier, pt3_make_segment, seli3_intersect);
  return false;
}

CC_INLINE BOOL trci2_intersect(const DTRIANGLE2 triangle, const DCIRCLE circle)
{
  return ci2_point_in_circle(pt2_closest_point_on_triangle_from_point(triangle, d2_make_point(circle.x, circle.y)), circle);
}

CC_INLINE BOOL d2_rectangle_to_rectangle_intersect(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  return ((x1 <= x4) && (x2 >= x3) && (y1 <= y4) && (y2 >= y3));
}

CC_INLINE BOOL rtrt2_intersect(const DRECTANGLE rectangle1, const DRECTANGLE rectangle2)
{
  return d2_rectangle_to_rectangle_intersect(rectangle1.p0.x, rectangle1.p0.y,  rectangle1.p1.x, rectangle1.p1.y,  rectangle2.p0.x, rectangle2.p0.y,  rectangle2.p1.x, rectangle2.p1.y);
}

CC_INLINE BOOL sert2_intersect(const DSEGMENT2 segment, const DRECTANGLE rectangle)
{
  if (rtrt2_intersect(rectangle, pt2_make_rectangle(segment.p0, segment.p1))) {
    int orin = pt2_orientation2(segment.p0, segment.p1, rectangle.p0.x, rectangle.p0.y);

    if ((pt2_orientation2(segment.p0, segment.p1, rectangle.p0.x, rectangle.p1.y) == orin) &&
        (pt2_orientation2(segment.p0, segment.p1, rectangle.p1.x, rectangle.p1.y) == orin) &&
        (pt2_orientation2(segment.p0, segment.p1, rectangle.p1.x, rectangle.p0.y) == orin)) {
      return false;
    }
    else {
      return true;
    }
  }
  else {
    return false;
  }
}

CC_INLINE BOOL trrt2_intersect(const DTRIANGLE2 triangle, const DRECTANGLE rectangle)
{
  return sert2_intersect(pt2_make_segment(triangle.p0, triangle.p1), rectangle) ||
      sert2_intersect(pt2_make_segment(triangle.p1, triangle.p2), rectangle) ||
      sert2_intersect(pt2_make_segment(triangle.p2, triangle.p0), rectangle);
}

CC_INLINE BOOL trqb2_intersect(const DTRIANGLE2 triangle, const DQUADRATIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, triangle, bezier, steps, pt2_calculate_qbezier_coefficients, pt2_create_point_on_qbezier, pt2_make_segment, setr2_intersect);
  return false;
}

CC_INLINE BOOL trcb2_intersect(const DTRIANGLE2 triangle, const DCUBIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, triangle, bezier, steps, pt2_calculate_cbezier_coefficients, pt2_create_point_on_cbezier, pt2_make_segment, setr2_intersect);
  return false;
}

CC_INLINE BOOL tr2_intersect(const DTRIANGLE2 triangle1, const DTRIANGLE2 triangle2)
{
  int i;
  const DPOINT2* p0 = &triangle1.p0;

  for (i = 0; i < TrianglePointCount; ++i) {
    if (
        d_is_equal(pt2_minimum_distance_from_point_to_triangle(p0[i], triangle2), (0.0)) ||
        d_is_equal(pt2_minimum_distance_from_point_to_triangle(p0[i], triangle1), (0.0))
    ) {
      return true;
    }
  }

  return false;
}

CC_INLINE BOOL rtqb2_intersect(const DRECTANGLE rectangle, const DQUADRATIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, rectangle, bezier, steps, pt2_calculate_qbezier_coefficients, pt2_create_point_on_qbezier, pt2_make_segment, sert2_intersect);
  return false;
}

CC_INLINE BOOL rtcb2_intersect(const DRECTANGLE rectangle, const DCUBIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, rectangle, bezier, steps, pt2_calculate_cbezier_coefficients, pt2_create_point_on_cbezier, pt2_make_segment, sert2_intersect);
  return false;
}

CC_INLINE BOOL rtci2_intersect(const DRECTANGLE rectangle, const DCIRCLE circle)
{
  return ci2_point_in_circle(pt2_closest_point_on_rectangle_from_point(rectangle, ci_make_point(circle)), circle);
}

CC_INLINE BOOL quqb2_intersect(const DQUADIX2 quadix, const DQUADRATIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, quadix, bezier, steps, pt2_calculate_qbezier_coefficients, pt2_create_point_on_qbezier, pt2_make_segment, sequ2_intersect);
  return false;
}

CC_INLINE BOOL qucb2_intersect(const DQUADIX2 quadix, const DCUBIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, quadix, bezier, steps, pt2_calculate_cbezier_coefficients, pt2_create_point_on_cbezier, pt2_make_segment, sequ2_intersect);
  return false;
}

CC_INLINE BOOL cici2_intersect(const DCIRCLE circle1, const DCIRCLE circle2)
{
  return (d2_lay_distance(circle1.x, circle1.y, circle2.x, circle2.y) <= ((circle1.radius + circle2.radius) * (circle1.radius + circle2.radius)));
}

CC_INLINE BOOL ciqb2_intersect(const DCIRCLE circle, const DQUADRATIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, circle, bezier, steps, pt2_calculate_qbezier_coefficients, pt2_create_point_on_qbezier, pt2_make_segment, seci2_intersect);
  return false;
}

CC_INLINE BOOL cicb2_intersect(const DCIRCLE circle, const DCUBIC_BEZIER2 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT2, circle, bezier, steps, pt2_calculate_cbezier_coefficients, pt2_create_point_on_cbezier, pt2_make_segment, seci2_intersect);
  return false;
}

CC_INLINE BOOL boxsp3_intersect(const DBOX3 box, const DSPHERE sphere)
{
  return sp3_point_in_sphere(pt3_closest_point_on_box_from_point(box, sp_make_point(sphere)), sphere);
}

CC_INLINE BOOL sp3_intersect(const DSPHERE sphere1, const DSPHERE sphere2)
{
  return (d3_lay_distance(sphere1.x, sphere1.y, sphere1.z, sphere2.x, sphere2.y, sphere2.z) <= ((sphere1.radius + sphere2.radius) * (sphere1.radius + sphere2.radius)));
}

CC_INLINE BOOL spqb3_intersect(const DSPHERE sphere, const DQUADRATIC_BEZIER3 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT3, sphere, bezier, steps, pt3_calculate_qbezier_coefficients, pt3_create_point_on_qbezier, pt3_make_segment, sesp3_intersect);
  return false;
}

CC_INLINE BOOL spcb3_intersect(const DSPHERE sphere, const DCUBIC_BEZIER3 bezier, const int steps)
{
  SIMPLEX_TO_BEZIER_INTERSECT(DPOINT3, sphere, bezier, steps, pt3_calculate_cbezier_coefficients, pt3_create_point_on_cbezier, pt3_make_segment, sesp3_intersect);
  return false;
}

#include "point_on.inl"

CC_INLINE BOOL ray2_intersect(const DRAY2 ray1, const DRAY2 ray2)
{
  T denom = v2_dot_product(v2_perpendicular(ray1.direction), ray2.direction);

  if (denom != (0.0)) {
    DVECTOR2 diff = v2_sub(ray1.origin, ray2.origin);
    T s = v2_dot_product(v2_perpendicular(ray1.direction), diff) / denom;
    T t = v2_dot_product(v2_perpendicular(ray2.direction), diff) / denom;
    return (d_greater_than_or_equal(t, (0.0)) && d_greater_than_or_equal(s, (0.0)));
  }
  else { // parallel
    return (pt2_point_on_ray(ray2.origin, ray1) || pt2_point_on_ray(ray1.origin, ray2));
  }
}

CC_INLINE BOOL ray3_intersect(const DRAY3 ray1, const DRAY3 ray2)
{
  T denom;

  if (!ray3_coplanar(ray1, ray2)) {
    return false;
  }

  denom = v3_dot_product(v3_perpendicular(ray1.direction), ray2.direction);

  if (denom != (0.0)) {
    DVECTOR3 diff = v3_sub(ray1.origin, ray2.origin);
    T s = v3_dot_product(v3_perpendicular(ray1.direction), diff) / denom;
    T t = v3_dot_product(v3_perpendicular(ray2.direction), diff) / denom;
    return (d_greater_than_or_equal(t, (0.0)) && d_greater_than_or_equal(s, (0.0)));
  }
  else { // parallel
    return (pt3_point_on_ray(ray2.origin, ray1) || pt3_point_on_ray(ray1.origin, ray2));
  }
}

CC_INLINE BOOL rayse2_intersect(const DRAY2 ray, const DSEGMENT2 segment)
{
  DVECTOR2 delta = v2_perpendicular(v2_sub(segment.p1, segment.p0));
  T denom = v2_dot_product(delta, ray.direction);

  if (denom != (0.0)) {
    DVECTOR2 diff = v2_sub(ray.origin, segment.p0);
    T s = v2_dot_product(delta, diff) / denom;
    T t = v2_dot_product(v2_perpendicular(ray.direction), diff) / denom;
    return ((d_greater_than_or_equal(t, (0.0)) && d_less_than_or_equal(t, (1.0))) && d_greater_than_or_equal(s, (0.0)));
  }
  else {
    return pt2_point_on_ray(segment.p0, ray);
  }
}

CC_INLINE BOOL rayse3_intersect(const DRAY3 ray, const DSEGMENT3 segment)
{
  if (pt3_robust_coplanar(segment.p0, segment.p1, ray.origin, ray3_generate_point_on_ray(ray, (1.0)))) {
    return pt3_point_on_segment(pt3_closest_point_on_ray_from_point(ray, segment.p0), segment);
  }
  else {
    return false;
  }
}

CC_INLINE BOOL rayrt2_intersect(const DRAY2 ray, const DRECTANGLE rectangle)
{
  T tmin = 0.0;
  T tmax = 1.0;
  T t;

  if (d_not_equal(ray.direction.x, (0.0))) {
    T recip_dirx = (1.0) / ray.direction.x;

    if (ray.direction.x > (0.0)) {
      if ((t = (rectangle.p1.x - ray.origin.x) * recip_dirx) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (rectangle.p0.x - ray.origin.x) * recip_dirx) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
    else {
      if ((t = (rectangle.p0.x - ray.origin.x) * recip_dirx) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (rectangle.p1.x - ray.origin.x) * recip_dirx) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
  }
  else if ((ray.origin.x < rectangle.p0.x) || (ray.origin.x > rectangle.p1.x)) {
    return false;
  }

  if (d_not_equal(ray.direction.y, (0.0))) {
    T recip_diry = (1.0) / ray.direction.y;

    if (ray.direction.y > (0.0)) {
      if ((t = (rectangle.p1.y - ray.origin.y) * recip_diry) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (rectangle.p0.y - ray.origin.y) * recip_diry) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
    else {
      if ((t = (rectangle.p0.y - ray.origin.y) * recip_diry) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (rectangle.p1.y - ray.origin.y) * recip_diry) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
  }
  else if ((ray.origin.y < rectangle.p0.y) || (ray.origin.y > rectangle.p1.y)) {
    return false;
  }

  return (tmin < tmax);
}

CC_INLINE BOOL raybox3_intersect(const DRAY3 ray, const DBOX3 box)
{
  T tmin = 0.0;
  T tmax = 1.0;
  T t;

  if (d_not_equal(ray.direction.x, (0.0))) {
    T recip_dirx = (1.0) / ray.direction.x;

    if (ray.direction.x > (0.0)) {
      if ((t = (box.p1.x - ray.origin.x) * recip_dirx) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (box.p0.x - ray.origin.x) * recip_dirx) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
    else {
      if ((t = (box.p0.x - ray.origin.x) * recip_dirx) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (box.p1.x - ray.origin.x) * recip_dirx) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
  }
  else if ((ray.origin.x < box.p0.x) || (ray.origin.x > box.p1.x)) {
    return false;
  }

  if (d_not_equal(ray.direction.y, (0.0))) {
    T recip_diry = (1.0) / ray.direction.y;

    if (ray.direction.y > (0.0)) {
      if ((t = (box.p1.y - ray.origin.y) * recip_diry) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (box.p0.y - ray.origin.y) * recip_diry) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
    else {
      if ((t = (box.p0.y - ray.origin.y) * recip_diry) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (box.p1.y - ray.origin.y) * recip_diry) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
  }
  else if ((ray.origin.y < box.p0.y) || (ray.origin.y > box.p1.y)) {
    return false;
  }

  if (d_not_equal(ray.direction.z, (0.0))) {
    T recip_dirz = (1.0) / ray.direction.z;

    if (ray.direction.z > (0.0)) {
      if ((t = (box.p1.z - ray.origin.z) * recip_dirz) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (box.p0.z - ray.origin.z) * recip_dirz) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
    else {
      if ((t = (box.p0.z - ray.origin.z) * recip_dirz) < tmin) {
        return false;
      }

      tmax = min(t, tmax);

      if ((t = (box.p1.z - ray.origin.z) * recip_dirz) > tmax) {
        return false;
      }

      tmin = max(t, tmin);
    }
  }
  else if ((ray.origin.z < box.p0.z) || (ray.origin.z > box.p1.z)) {
    return false;
  }

  return (tmin < tmax);
}

CC_INLINE BOOL raytr2_intersect(const DRAY2 ray, const DTRIANGLE2 triangle)
{
  if (tr2_point_in_triangle(ray.origin, triangle)) {
    return true;
  }
  else
    return rayse2_intersect(ray, tr2_make_segment(triangle, 0)) ||
        rayse2_intersect(ray, tr2_make_segment(triangle, 1)) ||
        rayse2_intersect(ray, tr2_make_segment(triangle, 2));
}

CC_INLINE BOOL raytr3_intersect(const DRAY3 ray, const DTRIANGLE3 triangle)
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
    return false;
  }
  else {
    T inv_det = (1.0) / det;

    T tvec_x = ray.origin.x - triangle.p0.x;
    T tvec_y = ray.origin.y - triangle.p0.y;
    T tvec_z = ray.origin.z - triangle.p0.z;

    T u = (tvec_x * pvec_x + tvec_y * pvec_y + tvec_z * pvec_z) * inv_det;

    if (u < 0.0 || u > 1.0) {
      return false;
    }
    else {
      T qvec_x = tvec_y * edge1_z - tvec_z * edge1_y;
      T qvec_y = tvec_z * edge1_x - tvec_x * edge1_z;
      T qvec_z = tvec_x * edge1_y - tvec_y * edge1_x;

      T v = (ray.direction.x * qvec_x + ray.direction.y * qvec_y + ray.direction.z * qvec_z) * inv_det;

      if ((v < 0.0) || ((u + v) > 1.0)) {
        return false;
      }
    }
  }

  return true;
}

CC_INLINE BOOL rayqu2_intersect(const DRAY2 ray, const DQUADIX2 quadix)
{
  if (qu2_point_in_quadix(ray.origin, quadix)) {
    return true;
  }
  else
    return rayse2_intersect(ray, qu2_make_segment(quadix, 0)) ||
        rayse2_intersect(ray, qu2_make_segment(quadix, 1)) ||
        rayse2_intersect(ray, qu2_make_segment(quadix, 2)) ||
        rayse2_intersect(ray, qu2_make_segment(quadix, 3));
}

CC_INLINE BOOL rayci2_intersect(const DRAY2 ray, const DCIRCLE circle)
{
  T dx = ray.origin.x - circle.x;
  T dy = ray.origin.y - circle.y;
  T b, c = (dx * dx) + (dy * dy) - (circle.radius * circle.radius);

  if (d_less_than_or_equal(c, (0.0))) {
    return true;
  }

  b = dx * ray.direction.x + dy * ray.direction.y;

  if (d_greater_than_or_equal(b, (0.0))) {
    return false;
  }

  return d_greater_than_or_equal(b * b, c);
}

CC_INLINE BOOL raysp3_intersect(const DRAY3 ray, const DSPHERE sphere)
{
  T dx = ray.origin.x - sphere.x;
  T dy = ray.origin.y - sphere.y;
  T dz = ray.origin.z - sphere.z;
  T b, c = (dx * dx) + (dy * dy) + (dz * dz) - (sphere.radius * sphere.radius);

  if (d_less_than_or_equal(c, (0.0))) {
    return true;
  }

  b = dx * ray.direction.x + dy * ray.direction.y + dz * ray.direction.z;

  if (d_greater_than_or_equal(b, (0.0))) {
    return false;
  }

  return d_greater_than_or_equal(b * b, c);
}

#include "distance.inl"

CC_INLINE BOOL raypl3_intersect(const DRAY3 ray, const DPLANE3 plane)
{
  T denom = v3_dot_product(ray.direction, plane.normal);

  if (d_not_equal(denom, (0.0))) {
    return ((-ptpl3_distance(ray.origin, plane) / denom) >= (0.0));
  }
  else {
    return false;
  }
}

CC_INLINE BOOL raypo2_intersect(const DRAY2 ray, const DPOLYGON2* polygon)
{
  int i, j = polygon->n - 1;

  if (polygon->n < 3) {
    return false;
  }

  for (i = 0; i < polygon->n; ++i) {
    if (rayse2_intersect(ray, pt2_make_segment(polygon->p[i], polygon->p[j]))) {
      return true;
    }

    j = i;
  }

  return false;
}

CC_INLINE BOOL plpl3_intersect(const DPLANE3 plane1, const DPLANE3 plane2)
{
  return d_is_equal(v3_dot_product(plane1.normal, plane2.normal), (1.0));
}

CC_INLINE BOOL plsp3_intersect(const DPLANE3 plane, const DSPHERE sphere)
{
  return d_less_than_or_equal(ptpl3_distance(sp_make_point(sphere), plane), sphere.radius);
}

CC_INLINE BOOL plli3_intersect(const DPLANE3 plane, const DLINE3 line)
{
  return d_not_equal(v3_dot_product(plane.normal, v3_sub(line.p1, line.p0)), (0.0));
}

