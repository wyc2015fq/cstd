
CC_INLINE int li2_orientation(const DLINE2 line,  const DPOINT2 point)
{
  return pt2_orientation(line.p0, line.p1, point);
}

CC_INLINE int se2_orientation(const DSEGMENT2 segment,  const DPOINT2 point)
{
  return pt2_orientation(segment.p0, segment.p1, point);
}

CC_INLINE int tr2_orientation(const DTRIANGLE2 triangle)
{
  return pt2_orientation(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE int pt3_orientation(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4)
{
  return d3_orientation(point1.x, point1.y, point1.z,  point2.x, point2.y, point2.z,  point3.x, point3.y, point3.z,  point4.x, point4.y, point4.z);
}

CC_INLINE int tr3_orientation(const DTRIANGLE3 triangle, const DPOINT3 point)
{
  return pt3_orientation(triangle.p0, triangle.p1, triangle.p2, point);
}

CC_INLINE BOOL d2_differing_orientation(const T x1, const T y1, const T x2, const T y2, const T p1x, const T p1y, const T p2x, const T p2y)
{
  /* Collinear orientation is not considered */
  return ((d2_orientation(x1, y1, x2, y2, p1x, p1y) * d2_orientation(x1, y1, x2, y2, p2x, p2y)) == -1);
}

CC_INLINE BOOL pt2_differing_orientation(const DPOINT2 p1, const DPOINT2 p2, const DPOINT2 q1, const DPOINT2 q2)
{
  return d2_differing_orientation(p1.x, p1.y, p2.x, p2.y, q1.x, q1.y, q2.x, q2.y);
}

CC_INLINE int d_in_circle(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T px, const T py)
{
  T dx1 = x1 - px;
  T dy1 = y1 - py;
  T dx2 = x2 - px;
  T dy2 = y2 - py;
  T dx3 = x3 - px;
  T dy3 = y3 - py;

  T det1 = dx1 * dy2 - dx2 * dy1;
  T det2 = dx2 * dy3 - dx3 * dy2;
  T det3 = dx3 * dy1 - dx1 * dy3;
  T lift1 = dx1 * dx1 + dy1 * dy1;
  T lift2 = dx2 * dx2 + dy2 * dy2;
  T lift3 = dx3 * dx3 + dy3 * dy3;

  T result = lift1 * det2 + lift2 * det3 + lift3 * det1;

  if (d_is_equal(result, (0.0))) {
    return Cocircular;
  }
  else if (result > (0.0)) {
    return PointInside;
  }
  else {
    return PointOutside;
  }
}

CC_INLINE int pt2_in_circle(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d_in_circle(point1.x, point1.y,
      point2.x, point2.y,
      point3.x, point3.y,
      point4.x, point4.y);
}

CC_INLINE int tr2_in_circle(const DTRIANGLE2 triangle, const DPOINT2 point)
{
  return pt2_in_circle(triangle.p0, triangle.p1, triangle.p2, point);
}

CC_INLINE int d3_in_sphere(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4, const T px, const T py, const T pz)
{
  T dx1 = x1 - px;
  T dx2 = x2 - px;
  T dx3 = x3 - px;
  T dx4 = x4 - px;
  T dy1 = y1 - py;
  T dy2 = y2 - py;
  T dy3 = y3 - py;
  T dy4 = y4 - py;
  T dz1 = z1 - pz;
  T dz2 = z2 - pz;
  T dz3 = z3 - pz;
  T dz4 = z4 - pz;

  T ab = dx1 * dy2 - dx2 * dy1;
  T bc = dx2 * dy3 - dx3 * dy2;
  T cd = dx3 * dy4 - dx4 * dy3;
  T da = dx4 * dy1 - dx1 * dy4;
  T ac = dx1 * dy3 - dx3 * dy1;
  T bd = dx2 * dy4 - dx4 * dy2;

  T abc = dz1 * bc - dz2 * ac + dz3 * ab;
  T bcd = dz2 * cd - dz3 * bd + dz4 * bc;
  T cda = dz3 * da + dz4 * ac + dz1 * cd;
  T dab = dz4 * ab + dz1 * bd + dz2 * da;

  T alift = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
  T blift = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;
  T clift = dx3 * dx3 + dy3 * dy3 + dz3 * dz3;
  T dlift = dx4 * dx4 + dy4 * dy4 + dz4 * dz4;

  T result = (dlift * abc - clift * dab) + (blift * cda - alift * bcd);

  if (d_is_equal(result, (0.0))) {
    return Cospherical;
  }
  else if (result > (0.0)) {
    return PointInside;
  }
  else {
    return PointOutside;
  }
}

CC_INLINE int pt3_in_sphere(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4, const DPOINT3 point5)
{
  return d3_in_sphere(point1.x, point1.y, point1.z,  point2.x, point2.y, point2.z,  point3.x, point3.y, point3.z,  point4.x, point4.y, point4.z,  point5.x, point5.y, point5.z);
}

CC_INLINE int qu3_in_sphere(const DQUADIX3 quadix, const DPOINT3 point)
{
  return pt3_in_sphere(quadix.p0, quadix.p1, quadix.p2, quadix.p3, point);
}

CC_INLINE T d2_signed_area(const T x1, const T y1, const T x2, const T y2, const T px, const T py)
{
  return (x2 - x1) * (py - y1) - (px - x1) * (y2 - y1);
}

CC_INLINE T pt2_signed_area(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_signed_area(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE T se2_signed_area(const DSEGMENT2 segment, const DPOINT2 point)
{
  return pt2_signed_area(segment.p0, segment.p1, point);
}

CC_INLINE T d3_signed_volume(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T px, const T py, const T pz)
{
  T px1 = x1 - px;
  T px2 = x2 - px;
  T px3 = x3 - px;

  T py1 = y1 - py;
  T py2 = y2 - py;
  T py3 = y3 - py;

  T pz1 = z1 - pz;
  T pz2 = z2 - pz;
  T pz3 = z3 - pz;

  return px1 * (py2 * pz3 - pz2 * py3) +  px2 * (py3 * pz1 - pz3 * py1) +  px3 * (py1 * pz2 - pz1 * py2);
}

CC_INLINE T pt3_signed_volume(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4)
{
  return d3_signed_volume(point1.x, point1.y, point1.z,  point2.x, point2.y, point2.z,  point3.x, point3.y, point3.z,  point4.x, point4.y, point4.z);
}

CC_INLINE T tr3_signed_volume(const DTRIANGLE3 triangle, const DPOINT3 point)
{
  return pt3_signed_volume(triangle.p0, triangle.p1, triangle.p2, point);
}

#include "project_point.inl"
#include "make.inl"
#include "degenerate.inl"
#include "generate.inl"

#include "point_in.inl"
#include "closest_point.inl"
#include "collinear.inl"

#include "lay_distance.inl"
#include "is_degenerate.inl"
#include "generate_random_object.inl"

CC_INLINE BOOL pt3_robust_coplanar(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4)
{
  return d_less_than_or_equal(ptpl3_lay_distance(point4, pt3_make_plane(point1, point2, point3)), 0.0);
}

CC_INLINE BOOL ray3_coplanar(const DRAY3 ray1, const DRAY3 ray2)
{
  DPOINT3 pnt1 = ray3_generate_point_on_ray(ray1, (1.0));
  DPOINT3 pnt2 = ray3_generate_point_on_ray(ray2, (1.0));

  if (pt3_robust_collinear(ray1.origin, pnt1, pnt2) && pt3_robust_collinear(ray2.origin, pnt2, pnt1)) {
    return true;
  }
  else {
    return pt3_robust_coplanar(ray1.origin, ray2.origin, pnt1, pnt2);
  }
}

CC_INLINE BOOL se3_coplanar(const DSEGMENT3 segment1, const DSEGMENT3 segment2)
{
  if (pt3_robust_collinear(segment1.p0, segment1.p1, segment2.p0) &&
      pt3_robust_collinear(segment1.p0, segment1.p1, segment2.p1)) {
    return true;
  }
  else {
    return pt3_robust_coplanar(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
  }
}

CC_INLINE BOOL li3_coplanar(const DLINE3 line1, const DLINE3 line2)
{
  if (pt3_robust_collinear(line1.p0, line1.p1, line2.p0) &&
      pt3_robust_collinear(line1.p0, line1.p1, line2.p1)) {
    return true;
  }
  else {
    return pt3_robust_coplanar(line1.p0, line1.p1, line2.p0, line2.p1);
  }
}

CC_INLINE BOOL tr3_coplanar(const DTRIANGLE3 triangle1, const DTRIANGLE3 triangle2)
{
  return pt3_robust_coplanar(triangle1.p0, triangle1.p1, triangle1.p2, triangle2.p0) &&
      pt3_robust_coplanar(triangle1.p0, triangle1.p1, triangle1.p2, triangle2.p1) &&
      pt3_robust_coplanar(triangle1.p0, triangle1.p1, triangle1.p2, triangle2.p2);
}

CC_INLINE BOOL qu3_coplanar(const DQUADIX3 quadix1, const DQUADIX3 quadix2)
{
  return pt3_robust_coplanar(quadix1.p0, quadix1.p1, quadix1.p2, quadix2.p0) &&
      pt3_robust_coplanar(quadix1.p0, quadix1.p1, quadix1.p2, quadix2.p1) &&
      pt3_robust_coplanar(quadix1.p0, quadix1.p1, quadix1.p2, quadix2.p2) &&
      pt3_robust_coplanar(quadix1.p0, quadix1.p1, quadix1.p2, quadix2.p3);
}

CC_INLINE BOOL d2_cocircular(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  DCIRCLE circle = d2_circumcircle(x1, y1, x2, y2, x3, y3);
  return d_is_equal(d2_distance(x4, y4, circle.x, circle.y), circle.radius);
}

CC_INLINE BOOL pt2_cocircular(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d2_cocircular(point1.x, point1.y,  point2.x, point2.y,  point3.x, point3.y,  point4.x, point4.y);
}

CC_INLINE BOOL tr2_cocircular(const DTRIANGLE2 triangle, const DPOINT2 point)
{
  return pt2_cocircular(triangle.p0, triangle.p1, triangle.p2, point);
}

CC_INLINE BOOL ci_cocircular(const DCIRCLE circle, const DPOINT2 point)
{
  return d_is_equal(d2_distance(point.x, point.y, circle.x, circle.y), circle.radius);
}

CC_INLINE BOOL d2_is_skinny_triangle(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  /*
  L. Paul Chew, Guaranteed-Quality Triangular Meshes,
  Technical Report TR-89-983,
  Department of Computer Science, Cornell University, 1989.
  Skinny only if circumradius-to-shortest tr2_edge ratio > 1.
  */
  DCIRCLE cir_circle = d2_circumcircle(x1, y1, x2, y2, x3, y3);
  T d0 = d2_lay_distance(x1, y1, x2, y2);
  T d1 = d2_lay_distance(x1, y1, x3, y3);
  T d2 = d2_lay_distance(x2, y2, x3, y3);
  T shortest_length = sqrt(MIN3(d1, d1, d2));
  return ((cir_circle.radius / shortest_length) > (1.0));
}

CC_INLINE BOOL pt2_is_skinny_triangle(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_is_skinny_triangle(point1.x, point1.y,  point2.x, point2.y,  point3.x, point3.y);
}

CC_INLINE BOOL tr2_is_skinny_triangle(const DTRIANGLE2 triangle)
{
  return pt2_is_skinny_triangle(triangle.p0, triangle.p1, triangle.p2);
}

#include "intersect.inl"

CC_INLINE BOOL d2_simple_intersect(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  return (((d2_orientation(x1, y1, x2, y2, x3, y3) * d2_orientation(x1, y1, x2, y2, x4, y4)) <= 0) &&
      ((d2_orientation(x3, y3, x4, y4, x1, y1) * d2_orientation(x3, y3, x4, y4, x2, y2)) <= 0));
}

CC_INLINE BOOL pt2_simple_intersect(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d2_simple_intersect(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, point4.x, point4.y);
}

CC_INLINE BOOL se2_simple_intersect(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return pt2_simple_intersect(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

#include "intersection_point.inl"

CC_INLINE T normalize_angle(const T angle)
{
  T n_angle = angle;

  if (n_angle > (360.0)) {
    n_angle = n_angle - ((int)(n_angle / (360.0)) * (360.0));
  }
  else if (n_angle < (0.0)) {
    while (n_angle < (0.0)) {
      n_angle = n_angle + (360.0);
    }
  }

  return n_angle;
}

CC_INLINE T vertical_mirror(const T angle)
{
  if (d_is_equal(angle, (0.0)) || d_is_equal(angle, (180.0)) || d_is_equal(angle, (360.0))) {
    return angle;
  }

  return ((360.0) - angle);
}

CC_INLINE T horizontal_mirror(const T angle)
{
  if (angle <= (180.0)) {
    return ((180.0) - angle);
  }
  else {
    return ((540.0) - angle);
  }
}

CC_INLINE BOOL d2_parallel(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  return d_is_equal(((y1 - y2) * (x3 - x4)), ((y3 - y4) * (x1 - x2)));
}

CC_INLINE BOOL pt2_parallel(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d2_parallel(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, point4.x, point4.y);
}

CC_INLINE BOOL se2_parallel(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return pt2_parallel(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL li2_parallel(const DLINE2 line1, const DLINE2 line2)
{
  return pt2_parallel(line1.p0, line1.p1, line2.p0, line2.p1);
}

CC_INLINE BOOL d3_parallel(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  /*
  Theory:
  If the gradients in the following planes x-y, y-z, z-x are equal
  then it can be said that the segments are parallel in 3d.
  Worst case scenario: 6 multiplications and 9 subtractions
  */

  T dx1 = x1 - x2;
  T dx2 = x3 - x4;

  T dy1 = y1 - y2;
  T dy2 = y3 - y4;

  T dz1 = z1 - z2;
  T dz2 = z3 - z4;

  if (d_not_equal((dy1 * dx2), (dy2 * dx1))) {
    return false;
  }

  if (d_not_equal((dz1 * dy2), (dz2 * dy1))) {
    return false;
  }

  if (d_not_equal((dx1 * dz2), (dx2 * dz1))) {
    return false;
  }

  return true;
}

CC_INLINE BOOL pt3_parallel(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4)
{
  return d3_parallel(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z, point4.x, point4.y, point4.z);
}

CC_INLINE BOOL se3_parallel(const DSEGMENT3 segment1, const DSEGMENT3 segment2)
{
  return pt3_parallel(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL li3_parallel(const DLINE3 line1, const DLINE3 line2)
{
  return pt3_parallel(line1.p0, line1.p1,
      line2.p0, line2.p1);
}

CC_INLINE BOOL d2_robust_parallel(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  DPOINT2 p1, p2;
  p1 = d2_closest_point_on_line_from_point(x1, y1, x2, y2, x3, y3);
  p2 = d2_closest_point_on_line_from_point(x1, y1, x2, y2, x4, y4);
  return d_is_equal(d2_distance(x3, y3, p1.x, p1.y), d2_distance(x4, y4, p2.x, p2.y));
}

CC_INLINE BOOL pt2_robust_parallel(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d2_robust_parallel(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, point4.x, point4.y);
}

CC_INLINE BOOL sese2_robust_parallel(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return pt2_robust_parallel(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL lili2_robust_parallel(const DLINE2 line1, const DLINE2 line2)
{
  return pt2_robust_parallel(line1.p0, line1.p1,
      line2.p0, line2.p1);
}

CC_INLINE BOOL lise2_robust_parallel(const DLINE2 line, const DSEGMENT2 segment)
{
  return pt2_robust_parallel(line.p0, line.p1, segment.p0, segment.p1);
}

CC_INLINE BOOL d3_robust_parallel(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  DPOINT3 p1, p2;
  p1 = d3_closest_point_on_line_from_point(x1, y1, z1, x2, y2, z2, x3, y3, z3);
  p2 = d3_closest_point_on_line_from_point(x1, y1, z1, x2, y2, z2, x4, y4, z4);
  return d_is_equal(d3_distance(x3, y3, z3, p1.x, p1.y, p1.z), d3_distance(x4, y4, z4, p2.x, p2.y, p2.z));
}

CC_INLINE BOOL pt3_robust_parallel(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4)
{
  return d3_robust_parallel(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z, point4.x, point4.y, point4.z);
}

CC_INLINE BOOL sese3_robust_parallel(const DSEGMENT3 segment1, const DSEGMENT3 segment2)
{
  return pt3_robust_parallel(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL lili3_robust_parallel(const DLINE3 line1, const DLINE3 line2)
{
  return pt3_robust_parallel(line1.p0, line1.p1,
      line2.p0, line2.p1);
}

CC_INLINE BOOL lise3_robust_parallel(const DLINE3 line, const DSEGMENT3 segment)
{
  return pt3_robust_parallel(line.p0, line.p1, segment.p0, segment.p1);
}

CC_INLINE BOOL d2_perpendicular(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  return d_is_equal((-1.0) * (y2 - y1) * (y4 - y3), (x4 - x3) * (x2 - x1));
}

CC_INLINE BOOL pt2_perpendicular(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d2_perpendicular(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, point4.x, point4.y);
}
CC_INLINE BOOL se2_perpendicular(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return pt2_perpendicular(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL lili2_perpendicular(const DLINE2 line1, const DLINE2 line2)
{
  return pt2_perpendicular(line1.p0, line1.p1, line2.p0, line2.p1);
}

CC_INLINE BOOL lise2_perpendicular(const DLINE2 line, const DSEGMENT2 segment)
{
  return pt2_perpendicular(line.p0, line.p1, segment.p0, segment.p1);
}

CC_INLINE BOOL d3_perpendicular(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  /*
  The dot product of the vector forms of the segments will
  be 0 if the segments are perpendicular
  */

  T dx1 = x1 - x2;
  T dx2 = x3 - x4;

  T dy1 = y1 - y2;
  T dy2 = y3 - y4;

  T dz1 = z1 - z2;
  T dz2 = z3 - z4;

  return d_is_equal((dx1 * dx2) + (dy1 * dy2) + (dz1 * dz2), (0.0));
}

CC_INLINE BOOL pt3_perpendicular(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4)
{
  return d3_perpendicular(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z, point4.x, point4.y, point4.z);
}

CC_INLINE BOOL sese3_perpendicular(const DSEGMENT3 segment1, const DSEGMENT3 segment2)
{
  return pt3_perpendicular(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL lili3_perpendicular(const DLINE3 line1, const DLINE3 line2)
{
  return pt3_perpendicular(line1.p0, line1.p1, line2.p0, line2.p1);
}

CC_INLINE BOOL d2_robust_perpendicular(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  DPOINT2 p1, p2;
  p1 = d2_closest_point_on_line_from_point(x1, y1, x2, y2, x3, y3);
  p2 = d2_closest_point_on_line_from_point(x1, y1, x2, y2, x4, y4);
  return d_is_equal(d2_distance(p1.x, p1.y, p2.x, p2.y), (0.0));
}

CC_INLINE BOOL pt2_robust_perpendicular(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d2_robust_perpendicular(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, point4.x, point4.y);
}

CC_INLINE BOOL sese2_robust_perpendicular(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return pt2_robust_perpendicular(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL lili2_robust_perpendicular(const DLINE2 line1, const DLINE2 line2)
{
  return pt2_robust_perpendicular(line1.p0, line1.p1, line2.p0, line2.p1);
}

CC_INLINE BOOL lise2_robust_perpendicular(const DLINE2 line, const DSEGMENT2 segment)
{
  return pt2_robust_perpendicular(line.p0, line.p1, segment.p0, segment.p1);
}

CC_INLINE BOOL d3_robust_perpendicular(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  DPOINT3 p1, p2;
  p1 = d3_closest_point_on_line_from_point(x1, y1, z1, x2, y2, z2, x3, y3, z3);
  p2 = d3_closest_point_on_line_from_point(x1, y1, z1, x2, y2, z2, x4, y4, z4);
  return d_is_equal(d3_distance(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z), (0.0));
}

CC_INLINE BOOL pt3_robust_perpendicular(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4)
{
  return d3_robust_perpendicular(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z, point4.x, point4.y, point4.z);
}

CC_INLINE BOOL sese3_robust_perpendicular(const DSEGMENT3 segment1, const DSEGMENT3 segment2)
{
  return pt3_robust_perpendicular(segment1.p0, segment1.p1, segment2.p0, segment2.p1);
}

CC_INLINE BOOL lili3_robust_perpendicular(const DLINE3 line1, const DLINE3 line2)
{
  return pt3_robust_perpendicular(line1.p0, line1.p1, line2.p0, line2.p1);
}

CC_INLINE BOOL rtrt2_rectangle_to_rectangle_intersect(const DRECTANGLE rectangle1, const DRECTANGLE rectangle2)
{
  return d2_rectangle_to_rectangle_intersect(rectangle1.p0.x, rectangle1.p0.y, rectangle1.p1.x, rectangle1.p1.y, rectangle2.p0.x, rectangle2.p0.y, rectangle2.p1.x, rectangle2.p1.y);
}

CC_INLINE BOOL d3_box_to_box_intersect(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  return ((x1 <= x4) && (x2 >= x3) && (y1 <= y4) && (y2 >= y3) && (z1 <= z4) && (z2 >= z3));
}

CC_INLINE BOOL boxbox3_box_to_box_intersect(const DBOX3 box1, const DBOX3 box2)
{
  return d3_box_to_box_intersect(box1.p0.x, box1.p0.y, box1.p0.z, box1.p1.x, box1.p1.y, box1.p1.z, box2.p0.x, box2.p0.y, box2.p0.z, box2.p1.x, box2.p1.y, box2.p1.z);
}

CC_INLINE BOOL d2_rectangle_within_rectangle(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  return (d2_point_in_rectangle(x1, y1, x3, y3, x4, y4) && d2_point_in_rectangle(x2, y2, x3, y3, x4, y4));
}

CC_INLINE BOOL rtrt2_rectangle_within_rectangle(const DRECTANGLE rectangle1, const DRECTANGLE rectangle2)
{
  return d2_rectangle_within_rectangle(rectangle1.p0.x, rectangle1.p0.y, rectangle1.p1.x, rectangle1.p1.y, rectangle2.p0.x, rectangle2.p0.y, rectangle2.p1.x, rectangle2.p1.y);
}

CC_INLINE BOOL d3_box_within_box(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  return (d3_point_in_box(x1, y1, z1, x3, y3, z3, x4, y4, z4) && d3_point_in_box(x2, y2, z2, x3, y3, z3, x4, y4, z4));
}

CC_INLINE BOOL boxbox3_box_within_box(const DBOX3 box1, const DBOX3 box2)
{
  return d3_box_within_box(box1.p0.x, box1.p0.y, box1.p0.z, box1.p1.x, box1.p1.y, box1.p1.z, box2.p0.x, box2.p0.y, box2.p0.z, box2.p1.x, box2.p1.y, box2.p1.z);
}

CC_INLINE BOOL d2_circle_within_rectangle(const T x, const T y, const T radius, const T x1, const T y1, const T x2, const T y2)
{
  return rtrt2_rectangle_within_rectangle(ci2_aabb(d2_make_circle(x, y, radius)), d2_make_rectangle(x1, y1, x2, y2));
}

CC_INLINE BOOL cirt2_circle_within_rectangle(const DCIRCLE circle, const DRECTANGLE rectangle)
{
  return d2_circle_within_rectangle(circle.x, circle.y, circle.radius, rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y);
}

CC_INLINE BOOL d2_triangle_within_rectangle(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4, const T x5, const T y5)
{
  return d2_point_in_rectangle(x1, y1, x4, y4, x5, y5) && d2_point_in_rectangle(x2, y2, x4, y4, x5, y5) && d2_point_in_rectangle(x3, y3, x4, y4, x5, y5);
}

CC_INLINE BOOL trrt2_triangle_within_rectangle(const DTRIANGLE2 triangle, const DRECTANGLE rectangle)
{
  return d2_triangle_within_rectangle(triangle.p0.x, triangle.p0.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y);
}

CC_INLINE BOOL d2_segment_within_rectangle(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  return d2_point_in_rectangle(x1, y1, x3, y3, x4, y4) && d2_point_in_rectangle(x2, y2, x3, y3, x4, y4);
}

CC_INLINE BOOL sert2_segment_within_rectangle(const DSEGMENT2 segment, const DRECTANGLE rectangle)
{
  return d2_segment_within_rectangle(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y);
}

CC_INLINE BOOL d2_quadix_within_rectangle(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4, const T x5, const T y5, const T x6, const T y6)
{
  return d2_point_in_rectangle(x1, y1, x5, y5, x6, y6) && d2_point_in_rectangle(x2, y2, x5, y5, x6, y6) && d2_point_in_rectangle(x3, y3, x5, y5, x6, y6) && d2_point_in_rectangle(x4, y4, x5, y5, x6, y6);
}

CC_INLINE BOOL qurt2_quadix_within_rectangle(const DQUADIX2 quadix, const DRECTANGLE rectangle)
{
  return d2_quadix_within_rectangle(quadix.p0.x, quadix.p0.y, quadix.p1.x, quadix.p1.y, quadix.p2.x, quadix.p2.y, quadix.p3.x, quadix.p3.y, rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y);
}

CC_INLINE BOOL port2_polygon_within_rectangle(const DPOLYGON2* polygon, const DRECTANGLE rectangle)
{
  int i;

  for (i = 0; i < polygon->n; ++i) {
    if (!rt2_point_in_rectangle(polygon->p[i], rectangle)) {
      return false;
    }
  }

  return true;
}

CC_INLINE BOOL d3_sphere_within_box(const T x, const T y, const T z, const T radius, const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  return boxbox3_box_within_box(sp3_aabb(d3_make_sphere(x, y, z, radius)), d3_make_box(x1, y1, z1, x2, y2, z2));
}

CC_INLINE BOOL spbox3_sphere_within_box(const DSPHERE sphere, const DBOX3 box)
{
  return d3_sphere_within_box(sphere.x, sphere.y, sphere.z, sphere.radius, box.p0.x, box.p0.y, box.p0.z, box.p1.x, box.p1.y, box.p1.z);
}

CC_INLINE BOOL d3_triangle_within_box(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4, const T x5, const T y5, const T z5)
{
  return d3_point_in_box(x1, y1, z1, x4, y4, z4, x5, y5, z5) && d3_point_in_box(x2, y2, z2, x4, y4, z4, x5, y5, z5) && d3_point_in_box(x3, y3, z3, x4, y4, z4, x5, y5, z5);
}

CC_INLINE BOOL trbox3_triangle_within_box(const DTRIANGLE3 triangle, const DBOX3 box)
{
  return d3_triangle_within_box(triangle.p0.x, triangle.p0.y, triangle.p0.z, triangle.p1.x, triangle.p1.y, triangle.p1.z, triangle.p2.x, triangle.p2.y, triangle.p2.z, box.p0.x, box.p0.y, box.p0.z, box.p1.x, box.p1.y, box.p1.z);
}

CC_INLINE BOOL d3_segment_within_box(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  return d3_point_in_box(x1, y1, z1, x3, y3, z3, x4, y4, z4) && d3_point_in_box(x2, y2, z2, x3, y3, z3, x4, y4, z4);
}

CC_INLINE BOOL sebox3_segment_within_box(const DSEGMENT3 segment, const DBOX3 box)
{
  return d3_segment_within_box(segment.p0.x, segment.p0.y, segment.p0.z, segment.p1.x, segment.p1.y, segment.p1.z, box.p0.x, box.p0.y, box.p0.z, box.p1.x, box.p1.y, box.p1.z);
}

CC_INLINE BOOL d3_quadix_within_box(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4, const T x5, const T y5, const T z5, const T x6, const T y6, const T z6)
{
  return d3_point_in_box(x1, y1, z1, x5, y5, z5, x6, y6, z6) && d3_point_in_box(x2, y2, z2, x5, y5, z5, x6, y6, z6) && d3_point_in_box(x3, y3, z3, x5, y5, z5, x6, y6, z6) && d3_point_in_box(x4, y4, z4, x5, y5, z5, x6, y6, z6);
}

CC_INLINE BOOL qubox3_quadix_within_box(const DQUADIX3 quadix, const DBOX3 box)
{
  return d3_quadix_within_box(quadix.p0.x, quadix.p0.y, quadix.p0.z, quadix.p1.x, quadix.p1.y, quadix.p1.z, quadix.p2.x, quadix.p2.y, quadix.p2.z, quadix.p3.x, quadix.p3.y, quadix.p3.z, box.p0.x, box.p0.y, box.p0.z, box.p1.x, box.p1.y, box.p1.z);
}

CC_INLINE BOOL pobox3_polygon_within_box(const DPOLYGON3* polygon, const DBOX3 box)
{
  int i;

  for (i = 0; i < polygon->n; ++i) {
    if (!box3_point_in_box(polygon->p[i], box)) {
      return false;
    }
  }

  return true;
}

CC_INLINE BOOL cici2_circle_in_circle(const DCIRCLE circle1, const DCIRCLE circle2)
{
  return d_greater_than_or_equal(circle2.radius - (d2_distance(circle1.x, circle1.y, circle2.x, circle2.y) + circle1.radius), (0.0));
}

#include "translate.inl"

CC_INLINE BOOL seci2_is_tangent(const DSEGMENT2 segment, const DCIRCLE circle)
{
  DSEGMENT2 tmp_segment = se2_translate(-circle.x, -circle.y, segment);
  T rsqr = sqr(circle.radius);
  T drsqr = se2_lay_distance(tmp_segment);
  T dsqr = sqr(tmp_segment.p0.x * tmp_segment.p1.y - tmp_segment.p1.x * tmp_segment.p0.y);
  return d_is_equal((rsqr * drsqr - dsqr), (0.0));
}

CC_INLINE BOOL d2_point_of_reflection(const T sx1, const T sy1, const T sx2, const T sy2, const T p1x, const T p1y, const T p2x, const T p2y, DPOINT2* rp)
{
  if (
      (!d2_collinear(sx1, sy1, sx2, sy2, p1x, p1y)) && (!d2_collinear(sx1, sy1, sx2, sy2, p2x, p2y)) &&
      (d2_orientation(sx1, sy1, sx2, sy2, p1x, p1y) == d2_orientation(sx1, sy1, sx2, sy2, p2x, p2y))
  ) {
    DPOINT2 p0, p1p, p2p;
    p1p = d2_closest_point_on_line_from_point(sx1, sy1, sx2, sy2, p1x, p1y);
    p2p = d2_closest_point_on_line_from_point(sx1, sy1, sx2, sy2, p2x, p2y);
    d2_intersect_at(p1x, p1y, p2p.x, p2p.y, p2x, p2y, p1p.x, p1p.y, &p0);
    *rp = d2_closest_point_on_line_from_point(sx1, sy1, sx2, sy2, p0.x, p0.y);

    if (d2_is_point_collinear(sx1, sy1, sx2, sy2, rp->x, rp->y, false)) {
      return true;
    }
  }

  return false;
}

CC_INLINE BOOL pt2_point_of_reflection(const DSEGMENT2 segment, const DPOINT2 point1, const DPOINT2 point2, DPOINT2* reflection_point)
{
  return d2_point_of_reflection(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, point1.x, point1.y, point2.x, point2.y, reflection_point);
}

CC_INLINE DSEGMENT2 se2_reverse_segment(const DSEGMENT2 segment)
{
  return pt2_make_segment(segment.p1, segment.p0);
}

CC_INLINE DSEGMENT3 se3_reverse_segment(const DSEGMENT3 segment)
{
  return pt3_make_segment(segment.p1, segment.p0);
}

CC_INLINE DPOINT2 rt2_rectangle_corner(const DRECTANGLE rectangle, const int corner_index)
{
  switch (corner_index) {
  case 0 :
    return rectangle.p0;

  case 1 :
    return d2_make_point(rectangle.p1.x, rectangle.p0.y);

  case 2 :
    return d2_make_point(rectangle.p1.x, rectangle.p1.y);

  case 3 :
    return d2_make_point(rectangle.p0.x, rectangle.p1.y);

  default :
    return pt2_degenerate();
  }
}

CC_INLINE DPOINT3 box3_box_corner(const DBOX3 box, const int corner_index)
{
  switch (corner_index) {
  case 0 :
    return box.p0;

  case 1 :
    return d3_make_point(box.p1.x, box.p0.y, box.p0.z);

  case 2 :
    return d3_make_point(box.p1.x, box.p1.y, box.p0.z);

  case 3 :
    return d3_make_point(box.p0.x, box.p1.y, box.p0.z);

  case 4 :
    return box.p1;

  case 5 :
    return d3_make_point(box.p1.x, box.p0.y, box.p1.z);

  case 6 :
    return d3_make_point(box.p1.x, box.p1.y, box.p1.z);

  case 7 :
    return d3_make_point(box.p0.x, box.p1.y, box.p1.z);

  default :
    return pt3_degenerate();
  }
}

#include "create.inl"

CC_INLINE DLINE2 tr2_triangle_bisector(const DTRIANGLE2 triangle, const int bisector)
{
  switch (bisector) {
  case 0 :
    return pt2_create_line_from_bisector(triangle.p2, triangle.p0, triangle.p1);

  case 1 :
    return pt2_create_line_from_bisector(triangle.p0, triangle.p1, triangle.p2);

  case 2 :
    return pt2_create_line_from_bisector(triangle.p0, triangle.p2, triangle.p1);

  default:
    return li2_degenerate();
  }
}

CC_INLINE DLINE3 tr3_triangle_bisector(const DTRIANGLE3 triangle, const int bisector)
{
  switch (bisector) {
  case 0 :
    return pt3_create_line_from_bisector(triangle.p2, triangle.p0, triangle.p1);

  case 1 :
    return pt3_create_line_from_bisector(triangle.p0, triangle.p1, triangle.p2);

  case 2 :
    return pt3_create_line_from_bisector(triangle.p0, triangle.p2, triangle.p1);

  default:
    return li3_degenerate();
  }
}

CC_INLINE DLINE2 tr2_triangle_external_bisector(const DTRIANGLE2 triangle, const int corner, const int opposing_corner)
{
  const DPOINT2* p = &triangle.p0;
  return pt2_create_line_from_bisector(pt2_project_point_t(p[corner], p[opposing_corner], (2.0)), p[opposing_corner], p[(opposing_corner + 1) % 3]);
}

CC_INLINE DLINE3 tr3_triangle_external_bisector(const DTRIANGLE3 triangle, const int corner, const int opposing_corner)
{
  const DPOINT3* p = &triangle.p0;
  return pt3_create_line_from_bisector(pt3_project_point_t(p[corner], p[opposing_corner], (2.0)),  p[opposing_corner],  p[(opposing_corner + 1) % 3]);
}

CC_INLINE DPOINT2 d2_segment_mid_point(const T x1, const T y1, const T x2, const T y2)
{
  DPOINT2 mid;
  mid.x = (x1 + x2) * (0.5);
  mid.y = (y1 + y2) * (0.5);
  return mid;
}

CC_INLINE DPOINT2 pt2_segment_mid_point(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_segment_mid_point(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE DPOINT2 se2_segment_mid_point(const DSEGMENT2 segment)
{
  return pt2_segment_mid_point(segment.p0, segment.p1);
}

CC_INLINE DPOINT3 d3_segment_mid_point(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  DPOINT3 mid;
  mid.x = (x1 + x2) * (0.5);
  mid.y = (y1 + y2) * (0.5);
  mid.z = (z1 + z2) * (0.5);
  return mid;
}

CC_INLINE DPOINT3 pt3_segment_mid_point(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_segment_mid_point(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
}

CC_INLINE DPOINT3 se3_segment_mid_point(const DSEGMENT3 segment)
{
  return pt3_segment_mid_point(segment.p0, segment.p1);
}

CC_INLINE DLINE2 tr2_triangle_median(const DTRIANGLE2 triangle, const int median)
{
  switch (median) {
  case 0 :
    return pt2_make_line(triangle.p0, pt2_segment_mid_point(triangle.p1, triangle.p2));

  case 1 :
    return pt2_make_line(triangle.p1, pt2_segment_mid_point(triangle.p0, triangle.p2));

  case 2 :
    return pt2_make_line(triangle.p2, pt2_segment_mid_point(triangle.p0, triangle.p1));

  default:
    return li2_degenerate();
  }
}

CC_INLINE DLINE3 tr3_triangle_median(const DTRIANGLE3 triangle, const int median)
{
  switch (median) {
  case 0 :
    return pt3_make_line(triangle.p0, pt3_segment_mid_point(triangle.p1, triangle.p2));

  case 1 :
    return pt3_make_line(triangle.p1, pt3_segment_mid_point(triangle.p0, triangle.p2));

  case 2 :
    return pt3_make_line(triangle.p2, pt3_segment_mid_point(triangle.p0, triangle.p1));

  default:
    return li3_degenerate();
  }
}

#include "mirror.inl"

CC_INLINE DLINE2 tr2_triangle_symmedian(const DTRIANGLE2 triangle, const int symmedian)
{
  if ((0 <= symmedian) && (symmedian < 3)) {
    return lili2_mirror(tr2_triangle_median(triangle, symmedian), tr2_triangle_bisector(triangle, symmedian));
  }
  else {
    return li2_degenerate();
  }
}

CC_INLINE DLINE3 tr3_triangle_symmedian(const DTRIANGLE3 triangle, const int symmedian)
{
  if ((0 <= symmedian) && (symmedian < 3)) {
    return lili3_mirror(tr3_triangle_median(triangle, symmedian), tr3_triangle_bisector(triangle, symmedian));
  }
  else {
    return li3_degenerate();
  }
}

#include "centroid.inl"

CC_INLINE DPOINT2 tr2_orthocenter(const DTRIANGLE2 triangle)
{
  return lili2_intersection_point(pt2_make_line(triangle.p0, pt2_closest_point_on_line_from_point(se2_make_line(tr2_opposing_edge(triangle, 0)), triangle.p0)),
      pt2_make_line(triangle.p1, pt2_closest_point_on_line_from_point(se2_make_line(tr2_opposing_edge(triangle, 1)), triangle.p0)));
}

CC_INLINE DPOINT3 tr3_orthocenter(const DTRIANGLE3 triangle)
{
  return lili3_intersection_point(pt3_make_line(triangle.p0, pt3_closest_point_on_line_from_point(se3_make_line(tr3_opposing_edge(triangle, 0)), triangle.p0)),
      pt3_make_line(triangle.p1, pt3_closest_point_on_line_from_point(se3_make_line(tr3_opposing_edge(triangle, 1)), triangle.p0)));
}

CC_INLINE DLINE2 tr2_euler_line(const DTRIANGLE2 triangle)
{
  return pt2_make_line(tr2_centroid(triangle), tr2_orthocenter(triangle));
}

CC_INLINE DLINE3 tr3_euler_line(const DTRIANGLE3 triangle)
{
  return pt3_make_line(tr3_centroid(triangle), tr3_orthocenter(triangle));
}

CC_INLINE DLINE2 pt2_create_parallel_line_on_point(const DLINE2 line, const DPOINT2 point)
{
  return pt2_make_line(point, v2_add(point, v2_sub(line.p1, line.p0)));
}

CC_INLINE DLINE3 pt3_create_parallel_line_on_point(const DLINE3 line, const DPOINT3 point)
{
  return pt3_make_line(point, v3_add(point, v3_sub(line.p1, line.p0)));
}

CC_INLINE DSEGMENT2 pt2_create_parallel_segment_on_point(const DLINE2 line, const DPOINT2 point)
{
  return pt2_make_segment(point, v2_add(point, v2_sub(line.p1, line.p0)));
}

CC_INLINE DSEGMENT3 pt3_create_parallel_segment_on_point(const DLINE3 line, const DPOINT3 point)
{
  return pt3_make_segment(point, v3_add(point, v3_sub(line.p1, line.p0)));
}

CC_INLINE DPOINT2 tr2_exmedian_point(const DTRIANGLE2 triangle, const int corner)
{
  const DPOINT2* p = &triangle.p0;
  DLINE2 line1 = pt2_create_parallel_line_on_point(pt2_make_line(p[corner], p[(corner + 1) % 3]), p[(corner + 2) % 3]);
  DLINE2 line2 = pt2_create_parallel_line_on_point(pt2_make_line(p[corner], p[(corner + 2) % 3]), p[(corner + 1) % 3]);
  return lili2_intersection_point(line1, line2);
}

CC_INLINE DPOINT3 tr3_exmedian_point(const DTRIANGLE3 triangle, const int corner)
{
  const DPOINT3* p = &triangle.p0;
  DLINE3 line1 = pt3_create_parallel_line_on_point(pt3_make_line(p[corner], p[(corner + 1) % 3]), p[(corner + 2) % 3]);
  DLINE3 line2 = pt3_create_parallel_line_on_point(pt3_make_line(p[corner], p[(corner + 2) % 3]), p[(corner + 1) % 3]);
  return lili3_intersection_point(line1, line2);
}

CC_INLINE DCIRCLE d2_inscribed_circle(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  /* using heron's s = ur */
  T side12 = d2_distance(x1, y1, x2, y2);
  T side23 = d2_distance(x2, y2, x3, y3);
  T side31 = d2_distance(x3, y3, x1, y1);
  T perimeter = (1.0) / (side12 + side23 + side31);
  return d2_make_circle((side23 * x1 + side31 * x2 + side12 * x3) * perimeter,
      (side23 * y1 + side31 * y2 + side12 * y3) * perimeter,
      (0.5) * sqrt((-side12 + side23 + side31) * (side12 - side23 + side31) * (side12 + side23 - side31) * perimeter));
}

CC_INLINE DCIRCLE pt2_inscribed_circle(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_inscribed_circle(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DCIRCLE tr2_inscribed_circle(const DTRIANGLE2 triangle)
{
  return pt2_inscribed_circle(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DSPHERE d3_inscribed_sphere(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3)
{
  DPOINT3 int_point = lili3_intersection_point(d3_create_line_from_bisector(x1, y1, z1, x2, y2, z2, x3, y3, z3),
      d3_create_line_from_bisector(x2, y2, z2, x3, y3, z3, x1, y1, z1));
  return pt3_make_sphere(int_point, pt3_distance(int_point, pt3_closest_point_on_line_from_point(d3_make_line(x1, y1, z1, x2, y2, z2), int_point)));
}

CC_INLINE DSPHERE pt3_inscribed_sphere(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_inscribed_sphere(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z);
}

CC_INLINE DSPHERE tr3_inscribed_sphere(const DTRIANGLE3 triangle)
{
  return pt3_inscribed_sphere(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DCIRCLE d2_nine_point_circle(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  DPOINT2 h1 = d2_closest_point_on_line_from_point(x2, y2, x3, y3, x1, y1);
  DPOINT2 h2 = d2_closest_point_on_line_from_point(x1, y1, x3, y3, x2, y2);
  DPOINT2 h3 = d2_closest_point_on_line_from_point(x1, y1, x2, y2, x3, y3);
  return d2_circumcircle(h1.x, h1.y, h2.x, h2.y, h3.x, h3.y);
}

CC_INLINE DCIRCLE pt2_nine_point_circle(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_nine_point_circle(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DCIRCLE tr2_nine_point_circle(const DTRIANGLE2 triangle)
{
  return pt2_nine_point_circle(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DPOINT2 tr2_feuerbach_point(const DTRIANGLE2 triangle)
{
  DPOINT2 ipoint[2];
  ipoint[0] = pt2_degenerate(), ipoint[1] = pt2_degenerate();
  cici2_intersection_point(tr2_inscribed_circle(triangle), tr2_nine_point_circle(triangle), ipoint);
  return ipoint[0];
}

CC_INLINE DLINE2 tr2_confined_triangle_median(const DTRIANGLE2 triangle, const DPOINT2 point, const int median)
{
  switch (median) {
  case 0 :
    return pt2_make_line(triangle.p0, seli2_intersection_point(tr2_edge(triangle, 1), pt2_make_line(triangle.p0, point)));

  case 1 :
    return pt2_make_line(triangle.p1, seli2_intersection_point(tr2_edge(triangle, 2), pt2_make_line(triangle.p1, point)));

  case 2 :
    return pt2_make_line(triangle.p2, seli2_intersection_point(tr2_edge(triangle, 0), pt2_make_line(triangle.p2, point)));

  default:
    return li2_degenerate();
  }
}

CC_INLINE DLINE3 confined_triangle_median(const DTRIANGLE3 triangle, const DPOINT3 point, const int median)
{
  switch (median) {
  case 0 :
    return pt3_make_line(triangle.p0, seli3_intersection_point(tr3_edge(triangle, 1), pt3_make_line(triangle.p0, point)));

  case 1 :
    return pt3_make_line(triangle.p1, seli3_intersection_point(tr3_edge(triangle, 2), pt3_make_line(triangle.p1, point)));

  case 2 :
    return pt3_make_line(triangle.p2, seli3_intersection_point(tr3_edge(triangle, 0), pt3_make_line(triangle.p2, point)));

  default:
    return li3_degenerate();
  }
}

CC_INLINE DPOINT2 pt2_isogonal_conjugate(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  return lili2_intersection_point(lili2_mirror(pt2_make_line(triangle.p0, point), tr2_triangle_median(triangle, 0)),
      lili2_mirror(pt2_make_line(triangle.p1, point), tr2_triangle_median(triangle, 1)));
}

CC_INLINE DPOINT3 pt3_isogonal_conjugate(const DPOINT3 point, const DTRIANGLE3 triangle)
{
  return lili3_intersection_point(lili3_mirror(pt3_make_line(triangle.p0, point), tr3_triangle_median(triangle, 0)),
      lili3_mirror(pt3_make_line(triangle.p1, point), tr3_triangle_median(triangle, 1)));
}

CC_INLINE DPOINT2 pt2_cyclocevian_conjugate(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  DPOINT2 a_prime = lili2_intersection_point(pt2_make_line(triangle.p0, point), se2_make_line(tr2_edge(triangle, 1)));
  DPOINT2 b_prime = lili2_intersection_point(pt2_make_line(triangle.p1, point), se2_make_line(tr2_edge(triangle, 2)));
  DPOINT2 c_prime = lili2_intersection_point(pt2_make_line(triangle.p2, point), se2_make_line(tr2_edge(triangle, 0)));
  DCIRCLE circle = pt2_circumcircle(a_prime, b_prime, c_prime);
  DPOINT2 point_set1[4];
  DPOINT2 point_set2[4];
  int n1 = seci2_intersection_point(tr2_edge(triangle, 1), circle, (point_set1));
  int n2 = seci2_intersection_point(tr2_edge(triangle, 2), circle, (point_set2));
  DLINE2 line_prime_a = li2_degenerate();
  DLINE2 line_prime_b = li2_degenerate();

  if (n1 == 2) {
    if (pt2_is_equal(point_set1[0], a_prime)) {
      line_prime_a = pt2_make_line(triangle.p0, point_set1[1]);
    }
    else {
      line_prime_a = pt2_make_line(triangle.p0, point_set1[0]);
    }
  }
  else if (n1 == 1) {
    line_prime_a = pt2_make_line(triangle.p0, point_set1[0]);
  }

  if (n2 == 2) {
    if (pt2_is_equal(point_set2[0], b_prime)) {
      line_prime_b = pt2_make_line(triangle.p0, point_set2[1]);
    }
    else {
      line_prime_b = pt2_make_line(triangle.p0, point_set2[0]);
    }
  }
  else if (n2 == 1) {
    line_prime_b = pt2_make_line(triangle.p0, point_set2[0]);
  }

  return lili2_intersection_point(line_prime_a, line_prime_b);
}

CC_INLINE DPOINT2 tr2_symmedian_point(const DTRIANGLE2 triangle)
{
  return pt2_isogonal_conjugate(tr2_centroid(triangle), triangle);
}

CC_INLINE DPOINT3 tr3_symmedian_point(const DTRIANGLE3 triangle)
{
  return pt3_isogonal_conjugate(tr3_centroid(triangle), triangle);
}

CC_INLINE DPOINT2 d2_create_equilateral_triangle_p3(const T x1, const T y1, const T x2, const T y2)
{
  DPOINT2 pt;
  const T sin60 = (0.86602540378443864676372317075294);
  const T cos60 = (0.50000000000000000000000000000000);
  /* translate for x1,y1 to be origin */
  T tx = x2 - x1;
  T ty = y2 - y1;
  /* rotate 60 degrees and translate back */
  pt.x = ((tx * cos60) - (ty * sin60)) + x1;
  pt.y = ((ty * cos60) + (tx * sin60)) + y1;
  return pt;
}

CC_INLINE DPOINT2 pt2_create_equilateral_triangle_p3(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_create_equilateral_triangle_p3(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE DTRIANGLE2 d2_create_equilateral_triangle(const T x1, const T y1, const T x2, const T y2)
{
  DTRIANGLE2 triangle_;
  triangle_.p0.x = x1;
  triangle_.p0.y = y1;
  triangle_.p1.x = x2;
  triangle_.p1.y = y2;
  triangle_.p2 = d2_create_equilateral_triangle_p3(x1, y1, x2, y2);
  return triangle_;
}

CC_INLINE DTRIANGLE2 pt2_create_equilateral_triangle(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_create_equilateral_triangle(point1.x, point1.y, point2.x, point2.y);
}

#include "center_at_location.inl"

CC_INLINE DTRIANGLE2 d2_create_equilateral_triangle_l(const T cx, const T cy, const T side_length)
{
  return tr2_center_at_location(d2_create_equilateral_triangle(-side_length * (0.5), (0.0), side_length * (0.5), (0.0)), cx, cy);
}

CC_INLINE DTRIANGLE2 pt2_create_equilateral_triangle_l(const DPOINT2 center_point, const T side_length)
{
  return d2_create_equilateral_triangle_l(center_point.x, center_point.y, side_length);
}

CC_INLINE DTRIANGLE2 pt2_create_triangle(const DPOINT2 point1, const DPOINT2 point2, const T angle1, const T angle2)
{
  T bearing, theta_a, theta_b;
  DRAY2 ray1, ray2;

  if (d_greater_than_or_equal(angle1 + angle2, (180.0))) {
    return tr2_degenerate();
  }

  bearing = d2_cartesian_angle(point2.x - point1.x, point2.y - point1.y);
  theta_a = (0.0), theta_b = (0.0);

  switch (d1_quadrant(bearing)) {
  case 1 : {
    theta_a = normalize_angle(bearing - angle1);
    theta_b = normalize_angle((180.0) + bearing + angle2);
  }
  break;

  case 2 : {
    theta_a = normalize_angle(bearing - angle1);
    theta_b = normalize_angle((180.0) + bearing + angle2);
  }
  break;

  case 3 : {
    theta_a = normalize_angle(bearing - angle1);
    theta_b = normalize_angle(bearing - (180.0) + angle2);
  }
  break;

  case 4 : {
    theta_a = normalize_angle(bearing - angle1);
    theta_b = normalize_angle((180.0) - ((360.0) - bearing) + angle2);
  }
  break;
  }

  ray1 = pt2_make_ray(point1, theta_a);
  ray2 = pt2_make_ray(point2, theta_b);

  return pt2_make_triangle(rayray2_intersection_point(ray1, ray2), point1, point2);
}

CC_INLINE DTRIANGLE2 se2_create_triangle(const DSEGMENT2 segment, const T angle1, const T angle2)
{
  return pt2_create_triangle(segment.p0, segment.p1, angle1, angle2);
}

CC_INLINE DTRIANGLE2 pt2_create_isosceles_triangle(const DPOINT2 point1, const DPOINT2 point2, const T angle)
{
  return pt2_create_triangle(point1, point2, angle, angle);
}

CC_INLINE DTRIANGLE2 se2_create_isosceles_triangle(const DSEGMENT2 segment, const T angle)
{
  return pt2_create_isosceles_triangle(segment.p0, segment.p1, angle);
}

CC_INLINE DTRIANGLE2 create_morley_triangle(const DTRIANGLE2 triangle)
{
  DTRIANGLE2 tri = triangle;

  if (pt2_orientation(tri.p0, tri.p1, tri.p2) == LeftHandSide) {
    T_SWAP(DPOINT2, tri.p0, tri.p1);
  }

  {
    T angle1 = pt2_vertex_angle(tri.p2, tri.p0, tri.p1) * (1.0 / 3.0);
    T angle2 = pt2_vertex_angle(tri.p0, tri.p1, tri.p2) * (1.0 / 3.0);
    T angle3 = pt2_vertex_angle(tri.p1, tri.p2, tri.p0) * (1.0 / 3.0);
    DTRIANGLE2 triangle1 = se2_create_triangle(tr2_edge(tri, 0), angle1, angle2);
    DTRIANGLE2 triangle2 = se2_create_triangle(tr2_edge(tri, 1), angle2, angle3);
    DTRIANGLE2 triangle3 = se2_create_triangle(tr2_edge(tri, 2), angle3, angle1);
    return pt2_make_triangle(triangle1.p0, triangle2.p0, triangle3.p0);
  }
}

CC_INLINE DTRIANGLE2 tr2_create_cevian_triangle(const DTRIANGLE2 triangle, const DPOINT2 point)
{
  return pt2_make_triangle(sese2_intersection_point(pt2_make_segment(triangle.p0, point), tr2_edge(triangle, 1)),
      sese2_intersection_point(pt2_make_segment(triangle.p1, point), tr2_edge(triangle, 2)),
      sese2_intersection_point(pt2_make_segment(triangle.p2, point), tr2_edge(triangle, 0)));
}

CC_INLINE DTRIANGLE3 tr3_create_cevian_triangle(const DTRIANGLE3 triangle, const DPOINT3 point)
{
  return pt3_make_triangle(sese3_intersection_point(pt3_make_segment(triangle.p0, point), tr3_edge(triangle, 1)),
      sese3_intersection_point(pt3_make_segment(triangle.p1, point), tr3_edge(triangle, 2)),
      sese3_intersection_point(pt3_make_segment(triangle.p2, point), tr3_edge(triangle, 0)));
}

CC_INLINE DTRIANGLE2 create_anticevian_triangle(const DTRIANGLE2 triangle, const DPOINT2 point)
{
  int i;
  DPOINT2 point_list[3];
  const DPOINT2* p = &triangle.p0;
  pt2_degenerate();

  for (i = 0; i < 3; ++i) {
    DLINE2 opp_edge = se2_make_line(tr2_opposing_edge(triangle, i));
    DLINE2 cevian_edge = pt2_make_line(p[i], point);
    DLINE2 orthic_edge = pt2_make_line(p[i], pt2_closest_point_on_line_from_point(opp_edge, p[i]));

    //if (!robust_collinear(orthic_edge,point))
    {
      point_list[i] = lili2_intersection_point(cevian_edge, pt2_make_line((&orthic_edge.p0)[i % 2], ptli2_mirror(point, opp_edge)));
    }
    //else
    // point_list[i] = triangle[i];
  }

  /* Note: Buggy - DO NOT USE !*/
  return pt2_make_triangle(point_list[0], point_list[1], point_list[2]);
}

CC_INLINE DTRIANGLE3 tr3_create_anticevian_triangle(const DTRIANGLE3 triangle, const DPOINT3 point)
{
  return pt3_make_triangle(
      lili3_intersection_point(pt3_make_line(triangle.p0, point),
          pt3_make_line(pt3_closest_point_on_line_from_point(se3_make_line(tr3_opposing_edge(triangle, 0)), triangle.p0),
              ptli3_mirror(point, se3_make_line(tr3_opposing_edge(triangle, 0))))),
      lili3_intersection_point(pt3_make_line(triangle.p1, point),
          pt3_make_line(pt3_closest_point_on_line_from_point(se3_make_line(tr3_opposing_edge(triangle, 1)), triangle.p1),
              ptli3_mirror(point, se3_make_line(tr3_opposing_edge(triangle, 1))))),
      lili3_intersection_point(pt3_make_line(triangle.p2, point),
          pt3_make_line(pt3_closest_point_on_line_from_point(se3_make_line(tr3_opposing_edge(triangle, 2)), triangle.p2),
              ptli3_mirror(point, se3_make_line(tr3_opposing_edge(triangle, 2))))));
}

CC_INLINE DTRIANGLE2 tr2_create_anticomplementary_triangle(const DTRIANGLE2 triangle)
{
  return pt2_make_triangle(tr2_exmedian_point(triangle, 0),  tr2_exmedian_point(triangle, 1),  tr2_exmedian_point(triangle, 2));
}

CC_INLINE DTRIANGLE3 tr3_create_anticomplementary_triangle(const DTRIANGLE3 triangle)
{
  return pt3_make_triangle(tr3_exmedian_point(triangle, 0),  tr3_exmedian_point(triangle, 1),  tr3_exmedian_point(triangle, 2));
}

CC_INLINE DTRIANGLE2 tr2_create_inner_napoleon_triangle(const DTRIANGLE2 triangle)
{
  if (tr2_orientation(triangle) == RightHandSide) {
    return pt2_make_triangle(tr2_centroid(pt2_create_equilateral_triangle(triangle.p1, triangle.p0)),
        tr2_centroid(pt2_create_equilateral_triangle(triangle.p2, triangle.p1)),
        tr2_centroid(pt2_create_equilateral_triangle(triangle.p0, triangle.p2)));
  }
  else {
    return pt2_make_triangle(tr2_centroid(pt2_create_equilateral_triangle(triangle.p0, triangle.p1)),
        tr2_centroid(pt2_create_equilateral_triangle(triangle.p1, triangle.p2)),
        tr2_centroid(pt2_create_equilateral_triangle(triangle.p2, triangle.p0)));
  }
}

CC_INLINE DTRIANGLE2 tr2_create_outer_napoleon_triangle(const DTRIANGLE2 triangle)
{
  if (tr2_orientation(triangle) == RightHandSide) {
    return pt2_make_triangle(tr2_centroid(pt2_create_equilateral_triangle(triangle.p0, triangle.p1)),
        tr2_centroid(pt2_create_equilateral_triangle(triangle.p1, triangle.p2)),
        tr2_centroid(pt2_create_equilateral_triangle(triangle.p2, triangle.p0)));
  }
  else {
    return pt2_make_triangle(tr2_centroid(pt2_create_equilateral_triangle(triangle.p1, triangle.p0)),
        tr2_centroid(pt2_create_equilateral_triangle(triangle.p2, triangle.p1)),
        tr2_centroid(pt2_create_equilateral_triangle(triangle.p0, triangle.p2)));
  }
}

CC_INLINE int d2_create_equilateral_quadix_p34(const T x1, const T y1, const T x2, const T y2, DPOINT2* p3, DPOINT2* p4)
{
  T tx = x2 - x1;
  T ty = y2 - y1;
  p4->x = x1 - ty;
  p4->y = y1 + tx;
  p3->x = x2 - ty;
  p3->y = y2 + tx;
  return 0;
}

CC_INLINE int pt2_create_equilateral_quadix_p34(const DPOINT2 point1, const DPOINT2 point2, DPOINT2* point3, DPOINT2* point4)
{
  return d2_create_equilateral_quadix_p34(point1.x, point1.y, point2.x, point2.y, point3, point4);
}

CC_INLINE DQUADIX2 d2_create_equilateral_quadix(const T x1, const T y1, const T x2, const T y2)
{
  DQUADIX2 quadix_;
  quadix_.p0.x = x1;
  quadix_.p0.y = y1;
  quadix_.p1.x = x2;
  quadix_.p1.y = y2;
  d2_create_equilateral_quadix_p34(x1, y1, x2, y2, &quadix_.p2, &quadix_.p3);
  return quadix_;
}

CC_INLINE DQUADIX2 pt2_create_equilateral_quadix(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_create_equilateral_quadix(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE DQUADIX2 se2_create_equilateral_quadix(const DSEGMENT2 segment)
{
  return pt2_create_equilateral_quadix(segment.p0, segment.p1);
}

CC_INLINE DQUADIX2 d2_create_equilateral_quadix_l(const T cx, const T cy, const T side_length)
{
  return qu2_center_at_location(d2_create_equilateral_quadix(-side_length * (0.5), (0.0), side_length * (0.5), (0.0)), cx, cy);
}

CC_INLINE DQUADIX2 pt2_create_equilateral_quadix_l(const DPOINT2 center_point, const T side_length)
{
  return d2_create_equilateral_quadix_l(center_point.x, center_point.y, side_length);
}

CC_INLINE DTRIANGLE2 tr2_create_inner_vecten_triangle(const DTRIANGLE2 triangle)
{
  if (tr2_orientation(triangle) == RightHandSide) {
    return pt2_make_triangle(qu2_centroid(se2_create_equilateral_quadix(se2_reverse_segment(tr2_edge(triangle, 0)))),
        qu2_centroid(se2_create_equilateral_quadix(se2_reverse_segment(tr2_edge(triangle, 1)))),
        qu2_centroid(se2_create_equilateral_quadix(se2_reverse_segment(tr2_edge(triangle, 2)))));
  }
  else {
    return pt2_make_triangle(qu2_centroid(se2_create_equilateral_quadix(tr2_edge(triangle, 0))),
        qu2_centroid(se2_create_equilateral_quadix(tr2_edge(triangle, 1))),
        qu2_centroid(se2_create_equilateral_quadix(tr2_edge(triangle, 2))));
  }
}

CC_INLINE DTRIANGLE2 tr2_create_outer_vecten_triangle(const DTRIANGLE2 triangle)
{
  if (tr2_orientation(triangle) == RightHandSide) {
    return pt2_make_triangle(qu2_centroid(se2_create_equilateral_quadix(tr2_edge(triangle, 0))),
        qu2_centroid(se2_create_equilateral_quadix(tr2_edge(triangle, 1))),
        qu2_centroid(se2_create_equilateral_quadix(tr2_edge(triangle, 2))));
  }
  else {
    return pt2_make_triangle(qu2_centroid(se2_create_equilateral_quadix(se2_reverse_segment(tr2_edge(triangle, 0)))),
        qu2_centroid(se2_create_equilateral_quadix(se2_reverse_segment(tr2_edge(triangle, 1)))),
        qu2_centroid(se2_create_equilateral_quadix(se2_reverse_segment(tr2_edge(triangle, 2)))));
  }
}

CC_INLINE DTRIANGLE2 tr2_create_medial_triangle(const DTRIANGLE2 triangle)
{
  return pt2_make_triangle(pt2_segment_mid_point(triangle.p0, triangle.p1),  pt2_segment_mid_point(triangle.p1, triangle.p2),  pt2_segment_mid_point(triangle.p2, triangle.p0));
}

CC_INLINE DTRIANGLE3 tr3_create_medial_triangle(const DTRIANGLE3 triangle)
{
  return pt3_make_triangle(pt3_segment_mid_point(triangle.p0, triangle.p1),  pt3_segment_mid_point(triangle.p1, triangle.p2),  pt3_segment_mid_point(triangle.p2, triangle.p0));
}

CC_INLINE DPOINT2 d2_incenter(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  DPOINT2 pt;
  /* Using Heron's s=ur */
  T side12 = d2_distance(x1, y1, x2, y2);
  T side23 = d2_distance(x2, y2, x3, y3);
  T side31 = d2_distance(x3, y3, x1, y1);
  T perim = (1.0) / (side12 + side23 + side31);
  pt.x = (side23 * x1 + side31 * x2 + side12 * x3) * perim;
  pt.y = (side23 * y1 + side31 * y2 + side12 * y3) * perim;
  return pt;
}

CC_INLINE DPOINT3 d3_incenter(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3)
{
  DPOINT3 pt;
  /* Using Heron's s=ur */
  T side12 = d3_distance(x1, y1, z1, x2, y2, z2);
  T side23 = d3_distance(x2, y2, z2, x3, y3, z3);
  T side31 = d3_distance(x3, y3, z3, x1, y1, z1);
  T perim = (1.0) / (side12 + side23 + side31);
  pt.x = (side23 * x1 + side31 * x2 + side12 * x3) * perim;
  pt.y = (side23 * y1 + side31 * y2 + side12 * y3) * perim;
  pt.z = (side23 * z1 + side31 * z2 + side12 * z3) * perim;
  return pt;
}

CC_INLINE DPOINT2 pt2_incenter(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_incenter(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DPOINT3 pt3_incenter(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_incenter(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z);
}

CC_INLINE DPOINT2 tr2_incenter(const DTRIANGLE2 triangle)
{
  return pt2_incenter(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DPOINT3 tr3_incenter(const DTRIANGLE3 triangle)
{
  return pt3_incenter(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DTRIANGLE2 tr2_create_contact_triangle(const DTRIANGLE2 triangle)
{
  DPOINT2 center = tr2_incenter(triangle);
  return pt2_make_triangle(pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 0)), center),
      pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 1)), center),
      pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 2)), center));
}

CC_INLINE DTRIANGLE3 tr3_create_contact_triangle(const DTRIANGLE3 triangle)
{
  DPOINT3 center = tr3_incenter(triangle);
  return pt3_make_triangle(pt3_closest_point_on_line_from_point(se3_make_line(tr3_edge(triangle, 0)), center),
      pt3_closest_point_on_line_from_point(se3_make_line(tr3_edge(triangle, 1)), center),
      pt3_closest_point_on_line_from_point(se3_make_line(tr3_edge(triangle, 2)), center));
}

CC_INLINE DTRIANGLE2 tr2_create_symmedial_triangle(const DTRIANGLE2 triangle, const DPOINT2 point)
{
  return pt2_make_triangle(lili2_intersection_point(pt2_make_line(triangle.p0, point), se2_make_line(tr2_edge(triangle, 1))),
      lili2_intersection_point(pt2_make_line(triangle.p1, point), se2_make_line(tr2_edge(triangle, 2))),
      lili2_intersection_point(pt2_make_line(triangle.p2, point), se2_make_line(tr2_edge(triangle, 0))));
}

CC_INLINE DTRIANGLE2 tr2_create_orthic_triangle(const DTRIANGLE2 triangle)
{
  return pt2_make_triangle(pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 0)), triangle.p2),
      pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 1)), triangle.p0),
      pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 2)), triangle.p1));
}

CC_INLINE DTRIANGLE3 tr3_create_orthic_triangle(const DTRIANGLE3 triangle)
{
  return pt3_make_triangle(pt3_closest_point_on_line_from_point(se3_make_line(tr3_edge(triangle, 0)), triangle.p2),
      pt3_closest_point_on_line_from_point(se3_make_line(tr3_edge(triangle, 1)), triangle.p0),
      pt3_closest_point_on_line_from_point(se3_make_line(tr3_edge(triangle, 2)), triangle.p1));
}

CC_INLINE DTRIANGLE2 tr2_create_pedal_triangle(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  return pt2_make_triangle(pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 0)), point),
      pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 1)), point),
      pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 2)), point));
}

CC_INLINE DTRIANGLE3 tr3_create_pedal_triangle(const DPOINT3 point, const DTRIANGLE3 triangle)
{
  return pt3_make_triangle(pt3_closest_point_on_line_from_point(se3_make_line(tr3_edge(triangle, 0)), point),
      pt3_closest_point_on_line_from_point(se3_make_line(tr3_edge(triangle, 1)), point),
      pt3_closest_point_on_line_from_point(se3_make_line(tr3_edge(triangle, 2)), point));
}

CC_INLINE DTRIANGLE2 tr2_create_antipedal_triangle(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  DLINE2 line1 = li2_create_perpendicular_line_at_end_point(pt2_make_line(point, pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 0)), point)));
  DLINE2 line2 = li2_create_perpendicular_line_at_end_point(pt2_make_line(point, pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 1)), point)));
  DLINE2 line3 = li2_create_perpendicular_line_at_end_point(pt2_make_line(point, pt2_closest_point_on_line_from_point(se2_make_line(tr2_edge(triangle, 2)), point)));

  return pt2_make_triangle(lili2_intersection_point(line1, line2),
      lili2_intersection_point(line1, line3),
      lili2_intersection_point(line2, line3));
}

CC_INLINE DPOINT2 tr2_excenter(const DTRIANGLE2 triangle, const int corner)
{
  return lili2_intersection_point(tr2_triangle_bisector(triangle, corner), tr2_triangle_external_bisector(triangle, corner, (corner + 1) % 3));
}

CC_INLINE DPOINT3 tr3_excenter(const DTRIANGLE3 triangle, const int corner)
{
  return lili3_intersection_point(tr3_triangle_bisector(triangle, corner), tr3_triangle_external_bisector(triangle, corner, (corner + 1) % 3));
}

CC_INLINE DTRIANGLE2 tr2_create_excentral_triangle(const DTRIANGLE2 triangle)
{
  return pt2_make_triangle(tr2_excenter(triangle, 0), tr2_excenter(triangle, 1), tr2_excenter(triangle, 2));
}

CC_INLINE DTRIANGLE3 tr3_create_excentral_triangle(const DTRIANGLE3 triangle)
{
  return pt3_make_triangle(tr3_excenter(triangle, 0), tr3_excenter(triangle, 1), tr3_excenter(triangle, 2));
}

CC_INLINE DTRIANGLE2 tr2_create_incentral_triangle(const DTRIANGLE2 triangle)
{
  return tr2_create_cevian_triangle(triangle, tr2_incenter(triangle));
}

CC_INLINE DTRIANGLE3 tr3_create_incentral_triangle(const DTRIANGLE3 triangle)
{
  return tr3_create_cevian_triangle(triangle, tr3_incenter(triangle));
}

CC_INLINE DTRIANGLE2 tr2_create_extouch_triangle(const DTRIANGLE2 triangle)
{
  DTRIANGLE2 triangle_ = tr2_create_excentral_triangle(triangle);
  triangle_.p0 = pt2_closest_point_on_segment_from_point(tr2_opposing_edge(triangle, 0), triangle_.p0);
  triangle_.p1 = pt2_closest_point_on_segment_from_point(tr2_opposing_edge(triangle, 1), triangle_.p1);
  triangle_.p2 = pt2_closest_point_on_segment_from_point(tr2_opposing_edge(triangle, 2), triangle_.p2);
  return triangle_;
}

CC_INLINE DTRIANGLE3 tr3_create_extouch_triangle(const DTRIANGLE3 triangle)
{
  DTRIANGLE3 triangle_ = tr3_create_excentral_triangle(triangle);
  triangle_.p0 = pt3_closest_point_on_segment_from_point(tr3_opposing_edge(triangle, 0), triangle_.p0);
  triangle_.p1 = pt3_closest_point_on_segment_from_point(tr3_opposing_edge(triangle, 1), triangle_.p1);
  triangle_.p2 = pt3_closest_point_on_segment_from_point(tr3_opposing_edge(triangle, 2), triangle_.p2);
  return triangle_;
}

CC_INLINE DCIRCLE tr2_excircle(const DTRIANGLE2 triangle, const int i)
{
  if (i < 3) {
    DPOINT2 center = tr2_excenter(triangle, i);
    T radius = pt2_minimum_distance_from_point_to_segment(center, tr2_opposing_edge(triangle, i));
    return pt2_make_circle(center, radius);
  }
  else {
    return ci_degenerate();
  }
}

CC_INLINE DTRIANGLE2 tr2_create_feuerbach_triangle(const DTRIANGLE2 triangle)
{
  DCIRCLE circle = tr2_nine_point_circle(triangle);
  return pt2_make_triangle(pt2_project_point(ci_make_point(circle), ci_make_point(tr2_excircle(triangle, 0)), circle.radius),
      pt2_project_point(ci_make_point(circle), ci_make_point(tr2_excircle(triangle, 1)), circle.radius),
      pt2_project_point(ci_make_point(circle), ci_make_point(tr2_excircle(triangle, 2)), circle.radius));
}

CC_INLINE DTRIANGLE2 tr2_create_circumcevian_triangle(const DTRIANGLE2 triangle, const DPOINT2 point)
{
  DCIRCLE circum_circle = tr2_make_circle(triangle);
  DTRIANGLE2 triangle_ = tr2_degenerate();
  DPOINT2 ipoint[4];
  int ipoint_size = lici2_intersection_point(pt2_make_line(triangle.p0, point), circum_circle, (ipoint));

  if (0 == ipoint_size || (2 != ipoint_size)) {
    return triangle_;
  }

  triangle_.p0 = (pt2_is_equal(ipoint[0], triangle.p0)) ? ipoint[1] : ipoint[0];
  ipoint_size = lici2_intersection_point(pt2_make_line(triangle.p1, point), circum_circle, (ipoint));

  if (0 == ipoint_size || (2 != ipoint_size)) {
    return triangle_;
  }

  triangle_.p1 = (pt2_is_equal(ipoint[0], triangle.p1)) ? ipoint[1] : ipoint[0];
  ipoint_size = lici2_intersection_point(pt2_make_line(triangle.p2, point), circum_circle, (ipoint));

  if (0 == ipoint_size || (2 != ipoint_size)) {
    return triangle_;
  }

  triangle_.p2 = (pt2_is_equal(ipoint[0], triangle.p2)) ? ipoint[1] : ipoint[0];
  return triangle_;
}

CC_INLINE DTRIANGLE2 tr2_create_circummedial_triangle(const DTRIANGLE2 triangle)
{
  DCIRCLE circum_circle = tr2_make_circle(triangle);
  DTRIANGLE2 triangle_ = tr2_degenerate();
  DPOINT2 point_ = tr2_centroid(triangle);
  DPOINT2 ipoint[4];
  int ipoint_size;
  ipoint_size = lici2_intersection_point(pt2_make_line(triangle.p0, point_), circum_circle, (ipoint));

  if (0 == ipoint_size || (2 != ipoint_size)) {
    return triangle_;
  }

  triangle_.p0 = (pt2_is_equal(ipoint[0], triangle.p0)) ? ipoint[1] : ipoint[0];
  lici2_intersection_point(pt2_make_line(triangle.p1, point_), circum_circle, (ipoint));

  if (0 == ipoint_size || (2 != ipoint_size)) {
    return triangle_;
  }

  triangle_.p1 = (pt2_is_equal(ipoint[0], triangle.p1)) ? ipoint[1] : ipoint[0];
  lici2_intersection_point(pt2_make_line(triangle.p2, point_), circum_circle, (ipoint));

  if (0 == ipoint_size || (2 != ipoint_size)) {
    return triangle_;
  }

  triangle_.p2 = (pt2_is_equal(ipoint[0], triangle.p2)) ? ipoint[1] : ipoint[0];
  return triangle_;
}

CC_INLINE DPOINT3 d3_circumcenter(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3)
{
  DPLANE3 tri_plane = d3_make_plane(x1, y1, z1, x2, y2, z2, x3, y3, z3);
  DVECTOR3 vec1 = d3_make_vector(x1 - x2, y1 - y2, z1 - z2);
  DVECTOR3 vec2 = d3_make_vector(x3 - x2, y3 - y2, z3 - z2);
  DPOINT3 p1 = d3_make_point((x1 + x2) * (0.5), (y1 + y2) * (0.5), (z1 + z2) * (0.5));
  DPOINT3 p2 = d3_make_point((x3 + x2) * (0.5), (y3 + y2) * (0.5), (z3 + z2) * (0.5));
  DPOINT3 p1_ = v3_add(p1, v3_mul(vec1, tri_plane.normal));
  DPOINT3 p2_ = v3_add(p2, v3_mul(vec2, tri_plane.normal));
  return d3_intersection_point_line_to_line(p1.x, p1.y, p1.z, p1_.x, p1_.y, p1_.z, p2.x, p2.y, p2.z, p2_.x, p2_.y, p2_.z);
}

CC_INLINE DPOINT2 pt2_circumcenter(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_circumcenter(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DPOINT3 pt3_circumcenter(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_circumcenter(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z);
}

CC_INLINE DPOINT2 tr2_circumcenter(const DTRIANGLE2 triangle)
{
  return pt2_circumcenter(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DPOINT3 tr3_circumcenter(const DTRIANGLE3 triangle)
{
  return pt3_circumcenter(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DTRIANGLE2 tr2_create_first_brocard_triangle(const DTRIANGLE2 triangle)
{
  int i;
  DPOINT2 o = tr2_circumcenter(triangle);
  DPOINT2 k = tr2_symmedian_point(triangle);
  DCIRCLE circle = pt22_make_circle(o, k);
  DPOINT2 closest_point;
  DTRIANGLE2 result;
  DPOINT2 int_pts[4];
  int int_pts_size = 0;

  for (i = 0; i < 3; ++i) {
    closest_point = pt2_closest_point_on_segment_from_point(tr2_edge(triangle, i), o);
    int_pts_size = seci2_intersection_point(pt2_make_segment(o, closest_point), circle, (int_pts));
    (&result.p0)[i] = (pt2_is_equal(o, int_pts[0]) ? int_pts[1] : int_pts[0]);
  }

  return result;
}

CC_INLINE int pt2_create_right_triangle(const DPOINT2 p1, const DPOINT2 p2, DPOINT2* c)
{
  T distance = pt2_distance(p1, p2);
  DPOINT2 mid = pt2_segment_mid_point(p1, p2);
  DVECTOR2 v = v2_scale(v2_normalize(v2_perpendicular(v2_sub(p1, p2))), distance / (2.0));
  c[0] = v2_add(mid, v);
  c[1] = v2_add(mid, v);
  return 2;
}

CC_INLINE DPOINT2 d2_torricelli_point(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  /*
  Proven by Bonaventura Francesco Cavalieri in the book
  "Exercitationes geometricae sex" 1647. The theory goes,
  if the triangle has an angle of 120 degrees or more the
  toricelli point lies at the vertex of the large angle.
  Otherwise the point at which the simpson lines intersect
  is said to be the optimal solution.
  To find an intersection in 2D, all that is needed is 2
  lines (segments), hence not all three of the simpson
  lines are calculated.
  */
  DPOINT2 pt = {0};

  if (d_greater_than_or_equal(d2_vertex_angle(x1, y1, x2, y2, x3, y3), (120.0))) {
    pt.x = x2;
    pt.y = y2;
  }
  else if (d_greater_than_or_equal(d2_vertex_angle(x3, y3, x1, y1, x2, y2), (120.0))) {
    pt.x = x1;
    pt.y = y1;
  }
  else if (d_greater_than_or_equal(d2_vertex_angle(x2, y2, x3, y3, x1, y1), (120.0))) {
    pt.x = x3;
    pt.y = y3;
  }
  else {
    DPOINT2 oet1, oet2;

    if (d2_orientation(x1, y1, x2, y2, x3, y3) == RightHandSide) {
      oet1 = d2_create_equilateral_triangle_p3(x1, y1, x2, y2);
      oet2 = d2_create_equilateral_triangle_p3(x2, y2, x3, y3);
    }
    else {
      oet1 = d2_create_equilateral_triangle_p3(x2, y2, x1, y1);
      oet2 = d2_create_equilateral_triangle_p3(x3, y3, x2, y2);
    }

    pt = d2_intersection_point(oet1.x, oet1.y, x3, y3, oet2.x, oet2.y, x1, y1);
  }

  return pt;
}

CC_INLINE DPOINT2 pt2_torricelli_point(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_torricelli_point(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DPOINT2 tr2_torricelli_point(const DTRIANGLE2 triangle)
{
  return pt2_torricelli_point(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DSPHERE d3_circumsphere(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3)
{
  DPOINT3 c = d3_circumcenter(x1, y1, z1, x2, y2, z2, x3, y3, z3);
  return d3_make_sphere(c.x, c.y, c.z, d3_distance(c.x, c.y, c.z, x1, y1, z2));
}

CC_INLINE DSPHERE pt3_circumsphere(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_circumsphere(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z);
}

CC_INLINE DSPHERE tr3_circumsphere(const DTRIANGLE3 triangle)
{
  return pt3_circumsphere(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DCIRCLE tr2_mandart_circle(const DTRIANGLE2 triangle)
{
  return tr2_circumcircle(tr2_create_extouch_triangle(triangle));
}

CC_INLINE DCIRCLE tr2_brocard_circle(const DTRIANGLE2 triangle)
{
  return pt22_make_circle(tr2_circumcenter(triangle), tr2_symmedian_point(triangle));
}

CC_INLINE DCIRCLE ci2_invert_circle_across_circle(const DCIRCLE circle1, const DCIRCLE circle2)
{
  T t = sqr(circle2.radius) / (sqr(circle2.x - circle1.x) + sqr(circle2.y - circle1.y) - sqr(circle1.radius));
  return pt2_make_circle(pt2_project_point_t(ci_make_point(circle2), ci_make_point(circle1), t), t * circle1.radius);
}

CC_INLINE DSPHERE sp3_invert_sphere_across_sphere(const DSPHERE sphere1, const DSPHERE sphere2)
{
  T t = sqr(sphere2.radius) / (sqr(sphere2.x - sphere1.x) + sqr(sphere2.y - sphere1.y) + sqr(sphere2.z - sphere1.z) - sqr(sphere1.radius));
  return pt3_make_sphere(pt3_project_point_t(sp_make_point(sphere2), sp_make_point(sphere1), t), t * sphere1.radius);
}

CC_INLINE int ci_circle_tangent_points(const DCIRCLE circle, const DPOINT2 point, DPOINT2* out)
{
  DVECTOR2 v = v2_sub(point, ci_make_point(circle));
  T sqr_length = sqr(v.x) + sqr(v.y);
  T radius_sqr = sqr(circle.radius);

  if (d_greater_than_or_equal(sqr_length, radius_sqr)) {
    T ratio = (1.0) / sqr_length;
    T delta_dist = sqrt(T_ABS(sqr_length - radius_sqr));
    out[0].x = circle.x + circle.radius * (circle.radius * v.x - v.y * delta_dist) * ratio;
    out[0].y = circle.y + circle.radius * (circle.radius * v.y + v.x * delta_dist) * ratio;
    out[1].x = circle.x + circle.radius * (circle.radius * v.x + v.y * delta_dist) * ratio;
    out[1].y = circle.y + circle.radius * (circle.radius * v.y - v.x * delta_dist) * ratio;
  }
  else {
    out[0] = pt2_degenerate();
    out[1] = pt2_degenerate();
  }

  return 2;
}

CC_INLINE DLINE2 ci2_tangent_line(const DCIRCLE circle, const DPOINT2 point)
{
  return pt2_make_line(point, v2_add(point, v2_perpendicular(v2_sub(point, ci_make_point(circle)))));
}

#include "base2.inl"

