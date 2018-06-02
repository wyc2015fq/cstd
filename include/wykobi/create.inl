
CC_INLINE DLINE2 d2_create_line_from_bisector(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  /*
  Bisector angle theorem:
  D: bisector intersect point along (x1y1)(x3,y3)
  |(x1y1)(x2-y2)| : |(x2y2)(x3y3)| == |(x1y1)D| : |D(x3y3)|
  */
  T dist1 = d2_distance(x1, y1, x2, y2);
  T dist2 = d2_distance(x2, y2, x3, y3);
  T ratio = dist2 / (dist1 + dist2);
  return d2_make_line(x2, y2, x3 + ratio * (x1 - x3), y3 + ratio * (y1 - y3));
}

CC_INLINE DSEGMENT2 d2_create_segment_from_bisector(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  T dist1 = d2_distance(x1, y1, x2, y2);
  T dist2 = d2_distance(x2, y2, x3, y3);
  T ratio = dist2 / (dist1 + dist2);
  return d2_make_segment(x2, y2, x3 + ratio * (x1 - x3), y3 + ratio * (y1 - y3));
}

CC_INLINE DRAY2 d2_create_ray_from_bisector(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  DSEGMENT2 segment = d2_create_segment_from_bisector(x1, y1, x2, y2, x3, y3);
  return v2_make_ray(segment.p0, v2_sub(segment.p1, segment.p0));
}

CC_INLINE DLINE3 d3_create_line_from_bisector(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3)
{
  T dist1 = d3_distance(x1, y1, z1, x2, y2, z2);
  T dist2 = d3_distance(x2, y2, z2, x3, y3, z3);
  T ratio = dist2 / (dist1 + dist2);
  return d3_make_line(x2, y2, z2, x3 + ratio * (x1 - x3), y3 + ratio * (y1 - y3), z3 + ratio * (z1 - z3));
}

CC_INLINE DSEGMENT3 d3_create_segment_from_bisector(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3)
{
  T dist1 = d3_distance(x1, y1, z1, x2, y2, z2);
  T dist2 = d3_distance(x2, y2, z2, x3, y3, z3);
  T ratio = dist2 / (dist1 + dist2);
  return d3_make_segment(x2, y2, z2, x3 + ratio * (x1 - x3), y3 + ratio * (y1 - y3), z3 + ratio * (z1 - z3));
}

CC_INLINE DRAY3 d3_create_ray_from_bisector(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3)
{
  DSEGMENT3 segment = d3_create_segment_from_bisector(x1, y1, z1, x2, y2, z2, x3, y3, z3);
  return v3_make_ray(segment.p0, v3_sub(segment.p1, segment.p0));
}

CC_INLINE DLINE2 pt2_create_line_from_bisector(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_create_line_from_bisector(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DSEGMENT2 pt2_create_segment_from_bisector(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_create_segment_from_bisector(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DRAY2 pt2_create_ray_from_bisector(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_create_ray_from_bisector(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DLINE3 pt3_create_line_from_bisector(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_create_line_from_bisector(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z);
}

CC_INLINE DSEGMENT3 pt3_create_segment_from_bisector(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_create_segment_from_bisector(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z);
}

CC_INLINE DRAY3 pt3_create_ray_from_bisector(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_create_ray_from_bisector(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z);
}

CC_INLINE DLINE2 d2_create_perpendicular_bisector(const T x1, const T y1, const T x2, const T y2)
{
  T mx = (x1 + x2) * (0.5);
  T my = (y1 + y2) * (0.5);
  return d2_make_line(mx, my, mx + (y1 - y2), my + (x2 - x1));
}

CC_INLINE DLINE2 pt2_create_perpendicular_bisector(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_create_perpendicular_bisector(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE DLINE2 se2_create_perpendicular_bisector(const DSEGMENT2 segment)
{
  return pt2_create_perpendicular_bisector(segment.p0, segment.p1);
}

CC_INLINE DLINE2 li2_create_perpendicular_line_at_end_point(const DLINE2 line)
{
  return pt2_make_line(line.p1, v2_add(v2_perpendicular(v2_sub(line.p1, line.p0)), line.p1));
}

