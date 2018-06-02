
CC_INLINE BOOL d2_collinear(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3)
{
  return d_is_equal((x2 - x1) * (y3 - y1) , (x3 - x1) * (y2 - y1));
}

CC_INLINE BOOL d3_collinear(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3)
{
  T dx1 = x2 - x1;
  T dy1 = y2 - y1;
  T dz1 = z2 - z1;
  T dx2 = x3 - x1;
  T dy2 = y3 - y1;
  T dz2 = z3 - z1;
  T cx = (dy1 * dz2) - (dy2 * dz1);
  T cy = (dx2 * dz1) - (dx1 * dz2);
  T cz = (dx1 * dy2) - (dx2 * dy1);

  return d_is_equal(cx * cx + cy * cy + cz * cz, 0.0);
}

CC_INLINE BOOL pt2_collinear(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_collinear(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE BOOL pt3_collinear(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_collinear(point1.x, point1.y, point1.z,
      point2.x, point2.y, point2.z,
      point3.x, point3.y, point3.z);
}

CC_INLINE BOOL d2_robust_collinear(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3)
{
  T leydist1 = d2_lay_distance(x1, y1, x2, y2);
  T leydist2 = d2_lay_distance(x2, y2, x3, y3);
  T leydist3 = d2_lay_distance(x3, y3, x1, y1);

  if (leydist1 >= leydist2)
    if (leydist1 >= leydist3) {
      return d_is_equal(d2_minimum_distance_from_point_to_line(x3, y3, x1, y1, x2, y2), (0.0));
    }
    else {
      return d_is_equal(d2_minimum_distance_from_point_to_line(x2, y2, x3, y3, x1, y1), (0.0));
    }
  else if (leydist2 >= leydist3) {
    return d_is_equal(d2_minimum_distance_from_point_to_line(x1, y1, x2, y2, x3, y3), (0.0));
  }
  else {
    return d_is_equal(d2_minimum_distance_from_point_to_line(x2, y2, x3, y3, x1, y1), (0.0));
  }
}

CC_INLINE BOOL pt2_robust_collinear(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_robust_collinear(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE BOOL d3_robust_collinear(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3)
{
  T leydist1 = d3_lay_distance(x1, y1, z1, x2, y2, z2);
  T leydist2 = d3_lay_distance(x2, y2, z2, x3, y3, z3);
  T leydist3 = d3_lay_distance(x3, y3, z3, x1, y1, z1);

  if (leydist1 >= leydist2)
    if (leydist1 >= leydist3) {
      return d_is_equal(d3_minimum_distance_from_point_to_line(x3, y3, z3, x1, y1, z1, x2, y2, z2), (0.0));
    }
    else {
      return d_is_equal(d3_minimum_distance_from_point_to_line(x2, y2, z2, x3, y3, z3, x1, y1, z1), (0.0));
    }
  else if (leydist2 >= leydist3) {
    return d_is_equal(d3_minimum_distance_from_point_to_line(x1, y1, z1, x2, y2, z2, x3, y3, z3), (0.0));
  }
  else {
    return d_is_equal(d3_minimum_distance_from_point_to_line(x2, y2, z2, x3, y3, z3, x1, y1, z1), (0.0));
  }
}

CC_INLINE BOOL pt3_robust_collinear(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_robust_collinear(point1.x, point1.y, point1.z,
      point2.x, point2.y, point2.z,
      point3.x, point3.y, point3.z);
}

CC_INLINE BOOL li2_robust_collinear(const DLINE2 line, const DPOINT2 point)
{
  return pt2_robust_collinear(line.p0, line.p1, point);
}

CC_INLINE BOOL li3_robust_collinear(const DLINE3 line, const DPOINT3 point)
{
  return pt3_robust_collinear(line.p0, line.p1, point);
}

CC_INLINE bool d2_is_point_collinear(const T x1, const T y1,
    const T x2, const T y2,
    const T px, const T py,
    const bool robust DEFAULT(false))
{
  /*
  This method will return true iff the point (px,py) is collinear
  to points (x1,y1) and (x2,y2) and exists on the line segment A(x1,y1)->B(x2,y2)
  */
  if (((d_less_than_or_equal(x1, px) && d_less_than_or_equal(px, x2)) ||
      (d_less_than_or_equal(x2, px) && d_less_than_or_equal(px, x1))) &&
      ((d_less_than_or_equal(y1, py) && d_less_than_or_equal(py, y2)) ||
          (d_less_than_or_equal(y2, py) && d_less_than_or_equal(py, y1)))) {
    if (robust) {
      return d2_robust_collinear(x1, y1, x2, y2, px, py);
    }
    else {
      return d2_collinear(x1, y1, x2, y2, px, py);
    }
  }

  return false;
}

CC_INLINE BOOL pt2_is_point_collinear(const DPOINT2 point1,
    const DPOINT2 point2,
    const DPOINT2 point3,
    const BOOL robust)
{
  return d2_is_point_collinear(point1.x, point1.y,
      point2.x, point2.y,
      point3.x, point3.y,
      robust);
}

CC_INLINE BOOL se2_is_point_collinear(const DSEGMENT2 segment,
    const DPOINT2 point,
    const BOOL robust)
{
  return pt2_is_point_collinear(segment.p0, segment.p1, point, robust);
}

CC_INLINE BOOL d3_is_point_collinear(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T px, const T py, const T pz,
    const BOOL robust)
{
  /*
  This method will return true iff the point (px,py,pz) is collinear
  to points (x1,y1,z1) and (x2,y2,z2) and exists on the line segment A(x1,y1,z1)->B(x2,y2,z2)
  */
  if (((d_less_than_or_equal(x1, px) && d_less_than_or_equal(px, x2)) ||
      (d_less_than_or_equal(x2, px) && d_less_than_or_equal(px, x1))) &&
      ((d_less_than_or_equal(y1, py) && d_less_than_or_equal(py, y2)) ||
          (d_less_than_or_equal(y2, py) && d_less_than_or_equal(py, y1))) &&
      ((d_less_than_or_equal(z1, pz) && d_less_than_or_equal(pz, z2)) ||
          (d_less_than_or_equal(z2, pz) && d_less_than_or_equal(pz, z1)))) {
    if (robust) {
      return d3_robust_collinear(x1, y1, z1, x2, y2, z2, px, py, pz);
    }
    else {
      return d3_collinear(x1, y1, z1, x2, y2, z2, px, py, pz);
    }
  }

  return false;
}

CC_INLINE BOOL pt3_is_point_collinear(const DPOINT3 point1,
    const DPOINT3 point2,
    const DPOINT3 point3,
    const BOOL robust)
{
  return d3_is_point_collinear(point1.x, point1.y, point1.z,
      point2.x, point2.y, point2.z,
      point3.x, point3.y, point3.z,
      robust);
}

CC_INLINE BOOL se3_is_point_collinear(const DSEGMENT3 segment,
    const DPOINT3 point,
    const BOOL robust)
{
  return pt3_is_point_collinear(segment.p0, segment.p1, point, robust);
}

