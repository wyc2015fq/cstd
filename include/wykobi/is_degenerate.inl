
CC_INLINE BOOL d2_is_degenerate(const T x1, const T y1, const T x2, const T y2)
{
  return d_is_equal(x1, x2) && d_is_equal(y1, y2);
}

CC_INLINE BOOL se2_is_degenerate(const DSEGMENT2 segment)
{
  return d2_is_degenerate(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y);
}

CC_INLINE BOOL li2_is_degenerate(const DLINE2 line)
{
  return d2_is_degenerate(line.p0.x, line.p0.y, line.p1.x, line.p1.y);
}

CC_INLINE BOOL d3_is_degenerate(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  return d_is_equal(x1, x2) && d_is_equal(y1, y2) && d_is_equal(z1, z2);
}

CC_INLINE BOOL se3_is_degenerate(const DSEGMENT3 segment)
{
  return d3_is_degenerate(segment.p0.x, segment.p0.y, segment.p0.z, segment.p1.x, segment.p1.y, segment.p1.z);
}

CC_INLINE BOOL li3_is_degenerate(const DLINE3 line)
{
  return d3_is_degenerate(line.p0.x, line.p0.y, line.p0.z, line.p1.x, line.p1.y, line.p1.z);
}

#include "edge.inl"

CC_INLINE BOOL tr2_is_degenerate(const DTRIANGLE2 triangle)
{
  BOOL ret = pt2_is_equal(triangle.p0, triangle.p1) ||
      pt2_is_equal(triangle.p0, triangle.p2) ||
      pt2_is_equal(triangle.p1, triangle.p2) ||
      pt2_robust_collinear(triangle.p0, triangle.p1, triangle.p2);

  if (!ret) {
    T d0, d1, d2;
    d0 = pt2_distance(triangle.p0, triangle.p1);
    d1 = pt2_distance(triangle.p1, triangle.p2);
    d2 = pt2_distance(triangle.p2, triangle.p0);
    ret = d_is_equal(d1, d2 + d0) || d_is_equal(d1, d0 + d2) || d_is_equal(d2, d1 + d0);
  }

  return ret;
}

CC_INLINE BOOL tr3_is_degenerate(const DTRIANGLE3 triangle)
{
  BOOL ret =
      (pt3_is_equal(triangle.p0, triangle.p1)) ||
      (pt3_is_equal(triangle.p0, triangle.p2)) ||
      (pt3_is_equal(triangle.p1, triangle.p2)) ||
      pt3_robust_collinear(triangle.p0, triangle.p1, triangle.p2);

  if (!ret) {
    T d0, d1, d2;
    d0 = pt3_distance(triangle.p0, triangle.p1);
    d1 = pt3_distance(triangle.p1, triangle.p2);
    d2 = pt3_distance(triangle.p2, triangle.p0);
    ret = d_is_equal(d1, d2 + d0) || d_is_equal(d1, d0 + d2) || d_is_equal(d2, d1 + d0);
  }

  return ret;
}

CC_INLINE BOOL qu2_is_degenerate(const DQUADIX2 quadix)
{
  return /* stage 1 unique points check */
      d2_is_degenerate(quadix.p0.x, quadix.p0.y, quadix.p1.x, quadix.p1.y) ||
      d2_is_degenerate(quadix.p0.x, quadix.p0.y, quadix.p2.x, quadix.p2.y) ||
      d2_is_degenerate(quadix.p0.x, quadix.p0.y, quadix.p3.x, quadix.p3.y) ||
      d2_is_degenerate(quadix.p1.x, quadix.p1.y, quadix.p2.x, quadix.p2.y) ||
      d2_is_degenerate(quadix.p1.x, quadix.p1.y, quadix.p3.x, quadix.p3.y) ||
      d2_is_degenerate(quadix.p2.x, quadix.p2.y, quadix.p3.x, quadix.p3.y) ||
      /* stage 2 collinearity check */
      pt2_robust_collinear(quadix.p0, quadix.p1, quadix.p2) ||
      pt2_robust_collinear(quadix.p1, quadix.p2, quadix.p3) ||
      pt2_robust_collinear(quadix.p2, quadix.p3, quadix.p0) ||
      pt2_robust_collinear(quadix.p3, quadix.p0, quadix.p1) ||
      pt2_intersect(quadix.p0, quadix.p1, quadix.p2, quadix.p3) ||
      pt2_intersect(quadix.p0, quadix.p3, quadix.p1, quadix.p2) ||
      (! qu2_convex_quadix(quadix));
}

CC_INLINE BOOL d3_intersect(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3,
    const T x4, const T y4, const T z4)
{
  return (d_less_than_or_equal(d3_lay_distance_segment_to_segment(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4), 0.0));
}

CC_INLINE BOOL pt3_intersect(const DPOINT3 point1,
    const DPOINT3 point2,
    const DPOINT3 point3,
    const DPOINT3 point4)
{
  return d3_intersect(point1.x, point1.y, point1.z,
      point2.x, point2.y, point2.z,
      point3.x, point3.y, point3.z,
      point4.x, point4.y, point4.z);
}

CC_INLINE BOOL qu3_is_degenerate(const DQUADIX3 quadix)
{
  return /* stage 1 unique points check */
      d2_is_degenerate(quadix.p0.x, quadix.p0.y, quadix.p1.x, quadix.p1.y) ||
      d2_is_degenerate(quadix.p0.x, quadix.p0.y, quadix.p2.x, quadix.p2.y) ||
      d2_is_degenerate(quadix.p0.x, quadix.p0.y, quadix.p3.x, quadix.p3.y) ||
      d2_is_degenerate(quadix.p1.x, quadix.p1.y, quadix.p2.x, quadix.p2.y) ||
      d2_is_degenerate(quadix.p1.x, quadix.p1.y, quadix.p3.x, quadix.p3.y) ||
      d2_is_degenerate(quadix.p2.x, quadix.p2.y, quadix.p3.x, quadix.p3.y) ||
      /* stage 2 collinearity check */
      pt3_robust_collinear(quadix.p0, quadix.p1, quadix.p2) ||
      pt3_robust_collinear(quadix.p1, quadix.p2, quadix.p3) ||
      pt3_robust_collinear(quadix.p2, quadix.p3, quadix.p0) ||
      pt3_robust_collinear(quadix.p3, quadix.p0, quadix.p1) ||
      pt3_intersect(quadix.p0, quadix.p1, quadix.p2, quadix.p3) ||
      pt3_intersect(quadix.p0, quadix.p3, quadix.p1, quadix.p2);
}

CC_INLINE BOOL rt_is_degenerate(const DRECTANGLE rectangle)
{
  return pt2_is_equal(rectangle.p0, rectangle.p1);
}

CC_INLINE BOOL ci_is_degenerate(const DCIRCLE circle)
{
  return d_less_than_or_equal(circle.radius, (0.0));
}

CC_INLINE BOOL sp_is_degenerate(const DSPHERE sphere)
{
  return d_less_than_or_equal(sphere.radius, (0.0));
}

#include "angle.inl"

CC_INLINE BOOL is_degenerate(const DCIRCULAR_ARC arc)
{
  return d2_is_degenerate(arc.x1, arc.y1, arc.x2, arc.y2) ||
      d2_is_degenerate(arc.x1, arc.y1, arc.cx, arc.cy) ||
      d2_is_degenerate(arc.x2, arc.y2, arc.cx, arc.cy) ||
      (d2_lay_distance(arc.x1, arc.y1, arc.cx, arc.cy) != d2_lay_distance(arc.x2, arc.y2, arc.cx, arc.cy)) ||
      (d2_lay_distance(arc.x1, arc.y1, arc.cx, arc.cy) != d2_lay_distance(arc.px, arc.py, arc.cx, arc.cy)) ||
      (d2_cartesian_angle(arc.x1 - arc.cx, arc.y1 - arc.cy) != arc.angle1) ||
      (d2_cartesian_angle(arc.x2 - arc.cx, arc.y2 - arc.cy) != arc.angle2) ||
      (d2_cartesian_angle(arc.px - arc.cx, arc.py - arc.cy) != T_ABS(arc.angle1 - arc.angle2)) ||
      (d2_orientation(arc.x1, arc.y1, arc.x2, arc.y2, arc.px, arc.py) != arc.orientation);
}

