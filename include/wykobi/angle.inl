
CC_INLINE T d2_vertex_angle(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3)
{
  /*
  using the cosine identity:
  cos(a) = (b^2 + c^2 - a^2) / (2*b*c)
  a = cos'((b^2 + c^2 - a^2) / (2*b*c))

  where:
  a,b and c : are edges in the triangle
  a : is the angle at the vertex opposite edge 'a'
  aka the edge defined by the vertex <x1y1-x2y2-x3y3>
  */
  /* quantify coordinates */
  T x1_ = x1 - x2;
  T x3_ = x3 - x2;
  T y1_ = y1 - y2;
  T y3_ = y3 - y2;

  /* calculate lay distance */
  T dist = (x1_ * x1_ + y1_ * y1_) * (x3_ * x3_ + y3_ * y3_);

  if (d_is_equal(dist, (0.0))) {
    return (0.0);
  }
  else {
    T input_term = (x1_ * x3_ + y1_ * y3_) / sqrt(dist);

    if (d_is_equal(input_term, (1.0))) {
      return (0.0);
    }
    else if (d_is_equal(input_term, (-1.0))) {
      return (180.0);
    }
    else {
      return (acos(input_term) * _180DivPI);
    }
  }
}

CC_INLINE T pt2_vertex_angle(const DPOINT2 point1,
    const DPOINT2 point2,
    const DPOINT2 point3)
{
  return d2_vertex_angle(point1.x, point1.y,
      point2.x, point2.y,
      point3.x, point3.y);
}

CC_INLINE T vertex_angle(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3)
{
  /*
  Method is the same as the one described in
  the above routine.
  */

  /* Quantify coordinates */
  T x1_ = x1 - x2;
  T x3_ = x3 - x2;
  T y1_ = y1 - y2;
  T y3_ = y3 - y2;
  T z1_ = z1 - z2;
  T z3_ = z3 - z2;

  /* Calculate Ley Distance */
  T dist = (x1_ * x1_ + y1_ * y1_ + z1_ * z1_) * (x3_ * x3_ + y3_ * y3_ + z3_ * z3_);

  if (d_is_equal(dist, (0.0))) {
    return (0.0);
  }
  else {
    return (acos((x1_ * x3_ + y1_ * y3_ + z1_ * z3_) / sqrt(dist)) * _180DivPI);
  }
}

CC_INLINE T pt3_vertex_angle(const DPOINT3 point1,
    const DPOINT3 point2,
    const DPOINT3 point3)
{
  return vertex_angle(point1.x, point1.y, point1.z,
      point2.x, point2.y, point2.z,
      point3.x, point3.y, point3.z);
}

CC_INLINE T d2_oriented_vertex_angle(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3,
    const int orient)
{
  if (d2_orientation(x1, y1, x2, y2, x3, y3) != orient) {
    return (360.0) - d2_vertex_angle(x1, y1, x2, y2, x3, y3);
  }
  else {
    return d2_vertex_angle(x1, y1, x2, y2, x3, y3);
  }
}

CC_INLINE T pt2_oriented_vertex_angle(const DPOINT2 point1,
    const DPOINT2 point2,
    const DPOINT2 point3,
    const int orient)
{
  return d2_oriented_vertex_angle(point1.x, point1.y,
      point2.x, point2.y,
      point3.x, point3.y, orient);
}

CC_INLINE T d2_cartesian_angle(const T x, const T y)
{
  if ((x > (0.0)) && (y > (0.0))) {
    return (atan(y / x) * _180DivPI);
  }
  else if ((x < (0.0)) && (y > (0.0))) {
    return (atan(-x / y) * _180DivPI) + (90.0);
  }
  else if ((x < (0.0)) && (y < (0.0))) {
    return (atan(y / x) * _180DivPI) + (180.0);
  }
  else if ((x > (0.0)) && (y < (0.0))) {
    return (atan(-x / y) * _180DivPI) + (270.0);
  }
  else if ((x == (0.0)) && (y > (0.0))) {
    return (90.0);
  }
  else if ((x < (0.0)) && (y == (0.0))) {
    return (180.0);
  }
  else if ((x == (0.0)) && (y < (0.0))) {
    return (270.0);
  }
  else {
    return (0.0);
  }
}

CC_INLINE T pt2_cartesian_angle(const DPOINT2 point)
{
  return d2_cartesian_angle(point.x, point.y);
}

CC_INLINE T d2_robust_cartesian_angle(const T x, const T y)
{
  if ((x > (0.0)) && (y > (0.0))) {
    return (atan(y / x) * _180DivPI);
  }
  else if ((x < (0.0)) && (y > (0.0))) {
    return (atan(-x / y) * _180DivPI) + (90.0);
  }
  else if ((x < (0.0)) && (y < (0.0))) {
    return (atan(y / x) * _180DivPI) + (180.0);
  }
  else if ((x > (0.0)) && (y < (0.0))) {
    return (atan(-x / y) * _180DivPI) + (270.0);
  }
  else if (d_is_equal(x, (0.0)) && (y > (0.0))) {
    return (90.0);
  }
  else if ((x < (0.0)) && d_is_equal(y, (0.0))) {
    return (180.0);
  }
  else if (d_is_equal(x, (0.0)) && (y < (0.0))) {
    return (270.0);
  }
  else {
    return (0.0);
  }

}

CC_INLINE T pt2_robust_cartesian_angle(const DPOINT2 point)
{
  return d2_robust_cartesian_angle(point.x, point.y);
}

CC_INLINE T d2_cartesian_angle_o(const T x, const T y, const T ox, const T oy)
{
  return d2_cartesian_angle(x - ox, y - oy);
}

CC_INLINE T pt2_cartesian_angle_o(const DPOINT2 point, const DPOINT2 origin)
{
  return d2_cartesian_angle(point.x - origin.x, point.y - origin.y);
}

CC_INLINE T d2_robust_cartesian_angle_o(const T x, const T y, const T ox, const T oy)
{
  return d2_robust_cartesian_angle(x - ox, y - oy);
}

CC_INLINE T pt2_robust_cartesian_angle_o(const DPOINT2 point, const DPOINT2 origin)
{
  return d2_robust_cartesian_angle(point.x - origin.x, point.y - origin.y);
}

