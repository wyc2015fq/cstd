
CC_INLINE T ptse2_distance(const DPOINT2 point, const DSEGMENT2 segment)
{
  return pt2_distance(pt2_closest_point_on_segment_from_point(segment, point), point);
}

CC_INLINE T cp2_distance(const DCURVE_POINT2 point1, const DCURVE_POINT2 point2)
{
  return d2_distance(point1.p0.x, point1.p0.y, point2.p0.x, point2.p0.y);
}

CC_INLINE T cp3_distance(const DCURVE_POINT3 point1, const DCURVE_POINT3 point2)
{
  return d3_distance(point1.p0.x, point1.p0.y, point1.p0.z, point2.p0.x, point2.p0.y, point2.p0.z);
}

CC_INLINE T ptse3_distance(const DPOINT3 point, const DSEGMENT3 segment)
{
  return pt3_distance(pt3_closest_point_on_segment_from_point(segment, point), point);
}

CC_INLINE T ptrt2_distance(const DPOINT2 point, const DRECTANGLE rectangle)
{
  return pt2_distance(pt2_closest_point_on_rectangle_from_point(rectangle, point), point);
}

CC_INLINE T pttr2_distance(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  return pt2_distance(pt2_closest_point_on_triangle_from_point(triangle, point), point);
}

CC_INLINE T ptqu2_distance(const DPOINT2 point, const DQUADIX2 quadix)
{
  return sqrt(ptqu2_lay_distance(point, quadix));
}

CC_INLINE T ptray2_distance(const DPOINT2 point, const DRAY2 ray)
{
  return pt2_distance(point, pt2_closest_point_on_ray_from_point(ray, point));
}

CC_INLINE T ptray3_distance(const DPOINT3 point, const DRAY3 ray)
{
  return pt3_distance(point, pt3_closest_point_on_ray_from_point(ray, point));
}

CC_INLINE T ptpl3_distance(const DPOINT3 point, const DPLANE3 plane)
{
  return (plane.normal.x * point.x + plane.normal.y * point.y + plane.normal.z * point.z) - plane.constant;
}

CC_INLINE T d2_distance_segment_to_segment(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  return sqrt(d2_lay_distance_segment_to_segment(x1, y1, x2, y2, x3, y3, x4, y4));
}

CC_INLINE T d3_distance_segment_to_segment(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  return sqrt(d3_lay_distance_segment_to_segment(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4));
}

CC_INLINE T d2_distance_line_to_line(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  return sqrt(d2_lay_distance_line_to_line(x1, y1, x2, y2, x3, y3, x4, y4));
}

CC_INLINE T d3_distance_line_to_line(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4)
{
  return sqrt(d3_lay_distance_line_to_line(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4));
}

CC_INLINE T li2_distance(const DLINE2 line1, const DLINE2 line2)
{
  return d2_distance_line_to_line(line1.p0.x, line1.p0.y,
      line1.p1.x, line1.p1.y,
      line2.p0.x, line2.p0.y,
      line2.p1.x, line2.p1.y);
}

CC_INLINE T lili3_distance(const DLINE3 line1, const DLINE3 line2)
{
  return d3_distance_line_to_line(line1.p0.x, line1.p0.y, line1.p0.z,
      line1.p1.x, line1.p1.y, line1.p1.z,
      line2.p0.x, line2.p0.y, line2.p0.z,
      line2.p1.x, line2.p1.y, line2.p1.z);
}

CC_INLINE T sese2_distance(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return d2_distance_segment_to_segment(segment1.p0.x, segment1.p0.y,
      segment1.p1.x, segment1.p1.y,
      segment2.p0.x, segment2.p0.y,
      segment2.p1.x, segment2.p1.y);
}

CC_INLINE T sese3_distance(const DSEGMENT3 segment1, const DSEGMENT3 segment2)
{
  return d3_distance_segment_to_segment(segment1.p0.x, segment1.p0.y, segment1.p0.z,
      segment1.p1.x, segment1.p1.y, segment1.p1.z,
      segment2.p0.x, segment2.p0.y, segment2.p0.z,
      segment2.p1.x, segment2.p1.y, segment2.p1.z);
}

CC_INLINE T se2_distance(const DSEGMENT2 segment)
{
  return pt2_distance(segment.p0, segment.p1);
}

CC_INLINE T se3_distance(const DSEGMENT3 segment)
{
  return pt3_distance(segment.p0, segment.p1);
}

CC_INLINE T setr2_distance(const DSEGMENT2 segment, const DTRIANGLE2 triangle)
{
  return sqrt(setr2_lay_distance(segment, triangle));
}

CC_INLINE T setr3_distance(const DSEGMENT3 segment, const DTRIANGLE3 triangle)
{
  return sqrt(setr3_lay_distance(segment, triangle));
}

CC_INLINE T sert2_distance(const DSEGMENT2 segment, const DRECTANGLE rectangle)
{
  return min(min(sese2_distance(segment, rt2_edge(rectangle, 0)), sese2_distance(segment, rt2_edge(rectangle, 1))),
      min(sese2_distance(segment, rt2_edge(rectangle, 2)), sese2_distance(segment, rt2_edge(rectangle, 3))));
}

CC_INLINE T seci2_distance(const DSEGMENT2 segment, const DCIRCLE circle)
{
  return ptse2_distance(pt2_closest_point_on_circle_from_segment(circle, segment), segment);
}

CC_INLINE T trtr2_distance(const DTRIANGLE2 triangle1, const DTRIANGLE2 triangle2)
{
  int i;
  const DPOINT2* p1 = &triangle1.p0;
  const DPOINT2* p2 = &triangle2.p0;
  T min_dist = min(pt2_minimum_distance_from_point_to_triangle(triangle1.p0, triangle2),
      pt2_minimum_distance_from_point_to_triangle(triangle2.p0, triangle1));

  for (i = 1; i < TrianglePointCount; ++i) {
    if (d_is_equal(min_dist, (0.0))) {
      return min_dist;
    }

    min_dist = min(min_dist,
        min(pt2_minimum_distance_from_point_to_triangle(p1[i], triangle2),
            pt2_minimum_distance_from_point_to_triangle(p2[i], triangle1)));
  }

  return min_dist;
}

CC_INLINE T trrt2_distance(const DTRIANGLE2 triangle, const DRECTANGLE rectangle)
{
  if (trrt2_intersect(triangle, rectangle)) {
    return (0.0);
  }
  else
    return min(min(setr2_distance(rt2_edge(rectangle, 0), triangle),
        setr2_distance(rt2_edge(rectangle, 1), triangle)),
        min(setr2_distance(rt2_edge(rectangle, 2), triangle),
            setr2_distance(rt2_edge(rectangle, 3), triangle))
              );
}

#include "aabb.inl"

CC_INLINE T rtrt2_distance(const DRECTANGLE rectangle1, const DRECTANGLE rectangle2)
{
  if (rtrt2_intersect(rectangle1, rectangle2)) {
    return (0.0);
  }
  else {
    DRECTANGLE rec1 = rt2_aabb(rectangle1);
    DRECTANGLE rec2 = rt2_aabb(rectangle2);

    if (rec1.p1.y < rec2.p0.y) {
      return sese2_distance(d2_make_segment(rec1.p0.x, rec1.p1.y, rec1.p1.x, rec1.p1.y), d2_make_segment(rec2.p0.x, rec2.p0.y, rec2.p1.x, rec2.p0.y));
    }
    else if (rec1.p0.y > rec2.p1.y) {
      return sese2_distance(d2_make_segment(rec1.p0.x, rec1.p0.y, rec1.p1.x, rec1.p0.y), d2_make_segment(rec2.p0.x, rec2.p1.y, rec2.p1.x, rec2.p1.y));
    }
    else if (rec1.p1.x < rec2.p0.x) {
      return sese2_distance(d2_make_segment(rec1.p1.x, rec1.p0.y, rec1.p1.x, rec1.p1.y), d2_make_segment(rec2.p0.x, rec2.p0.y, rec2.p0.x, rec2.p1.y));
    }
    else if (rec1.p0.x > rec2.p1.x) {
      return sese2_distance(d2_make_segment(rec1.p0.x, rec1.p0.y, rec1.p0.x, rec1.p1.y), d2_make_segment(rec2.p1.x, rec2.p0.y, rec2.p1.x, rec2.p1.y));
    }
    else {
      return (0.0);
    }
  }
}

CC_INLINE T trci2_distance(const DTRIANGLE2 triangle, const DCIRCLE circle)
{
  if (trci2_intersect(triangle, circle)) {
    return (0.0);
  }
  else {
    DPOINT2 point1 = pt2_closest_point_on_triangle_from_point(triangle, ci_make_point(circle));
    DPOINT2 point2 = pt2_closest_point_on_circle_from_point(circle, point1);
    return pt2_distance(point1, point2);
  }
}

CC_INLINE T rtci2_distance(const DRECTANGLE rectangle, const DCIRCLE circle)
{
  if (rtci2_intersect(rectangle, circle)) {
    return (0.0);
  }
  else {
    DPOINT2 point1 = pt2_closest_point_on_rectangle_from_point(rectangle, ci_make_point(circle));
    DPOINT2 point2 = pt2_closest_point_on_circle_from_point(circle, point1);
    return pt2_distance(point1, point2);
  }
}

CC_INLINE T ptci2_distance(const DPOINT2 point, const DCIRCLE circle)
{
  if (ci2_point_in_circle(point, circle)) {
    return (0.0);
  }
  else {
    return pt2_distance(point, pt2_closest_point_on_circle_from_point(circle, point));
  }
}

CC_INLINE T cici2_distance(const DCIRCLE circle1, const DCIRCLE circle2)
{
  T dist = d2_distance(circle1.x, circle1.y, circle2.x, circle2.y);

  if (dist > (circle1.radius + circle2.radius)) {
    return (dist - (circle1.radius + circle2.radius));
  }
  else {
    return (0.0);
  }
}

CC_INLINE T spsp3_distance(const DSPHERE sphere1, const DSPHERE sphere2)
{
  T dist = d3_distance(sphere1.x, sphere1.y, sphere1.z, sphere2.x, sphere2.y, sphere2.z);

  if (dist > (sphere1.radius + sphere2.radius)) {
    return (dist - (sphere1.radius + sphere2.radius));
  }
  else {
    return (0.0);
  }
}

