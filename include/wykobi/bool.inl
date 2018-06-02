
CC_INLINE BOOL d_is_equal(const T val1, const T val2)
{
  T epsilon = Epsilon;
  T diff = val1 - val2;
  assert(((-epsilon <= diff) && (diff <= epsilon)) == (fabs(diff) <= epsilon));
  return ((-epsilon <= diff) && (diff <= epsilon));
}

CC_INLINE BOOL pt2_is_equal(const DPOINT2 point1, const DPOINT2 point2)
{
  T epsilon = Epsilon;
  return (d_is_equal(point1.x, point2.x) && d_is_equal(point1.y, point2.y));
}

CC_INLINE BOOL pt3_is_equal(const DPOINT3 point1, const DPOINT3 point2)
{
  return (d_is_equal(point1.x, point2.x) &&
      d_is_equal(point1.y, point2.y) &&
      d_is_equal(point1.z, point2.z));
}

CC_INLINE BOOL rt_is_equal(const DRECTANGLE rectangle1, const DRECTANGLE rectangle2)
{
  return (pt2_is_equal(rectangle1.p0, rectangle2.p0) && pt2_is_equal(rectangle1.p1, rectangle2.p1)) ||
      (pt2_is_equal(rectangle1.p0, rectangle2.p1) && pt2_is_equal(rectangle1.p1, rectangle2.p0));
}

CC_INLINE BOOL ci_is_equal(const DCIRCLE circle1, const DCIRCLE circle2)
{
  return d_is_equal(circle1.x, circle2.x) && d_is_equal(circle1.y, circle2.y) && d_is_equal(circle1.radius, circle2.radius);
}

CC_INLINE BOOL box_is_equal(const DBOX3 box1, const DBOX3 box2)
{
  return (pt3_is_equal(box1.p0, box2.p0) && pt3_is_equal(box1.p1, box2.p1)) ||
      (pt3_is_equal(box1.p0, box2.p1) && pt3_is_equal(box1.p1, box2.p0));
}

CC_INLINE BOOL sp_is_equal(const DSPHERE sphere1, const DSPHERE sphere2)
{
  return d_is_equal(sphere1.x, sphere2.x) &&
      d_is_equal(sphere1.y, sphere2.y) &&
      d_is_equal(sphere1.z, sphere2.z) &&
      d_is_equal(sphere1.radius, sphere2.radius);
}

CC_INLINE BOOL d_not_equal(const T val1, const T val2)
{
  T epsilon = Epsilon;
  T diff = val1 - val2;
  assert(((-epsilon > diff) || (diff > epsilon)) == (fabs(val1 - val2) > epsilon));
  return ((-epsilon > diff) || (diff > epsilon));
}

CC_INLINE BOOL pt2_not_equal(const DPOINT2 point1, const DPOINT2 point2)
{
  return (d_not_equal(point1.x, point2.x) || d_not_equal(point1.y, point2.y));
}

CC_INLINE BOOL pt3_not_equal(const DPOINT3 point1, const DPOINT3 point2)
{
  return (d_not_equal(point1.x, point2.x) || d_not_equal(point1.y, point2.y) || d_not_equal(point1.z, point2.z));
}

CC_INLINE BOOL rt_not_equal(const DRECTANGLE rectangle1, const DRECTANGLE rectangle2)
{
  return (!rt_is_equal(rectangle1, rectangle2));
}

CC_INLINE BOOL ci_not_equal(const DCIRCLE circle1, const DCIRCLE circle2)
{
  return (!ci_is_equal(circle1, circle2));
}

CC_INLINE BOOL box_not_equal(const DBOX3 box1, const DBOX3 box2)
{
  return (!box_is_equal(box1, box2));
}

CC_INLINE BOOL sp_not_equal(const DSPHERE sphere1, const DSPHERE sphere2)
{
  return (!sp_is_equal(sphere1, sphere2));
}

CC_INLINE BOOL d_less_than_or_equal(const T val1, const T val2)
{
  return (val1 < val2) || d_is_equal(val1, val2);
}

CC_INLINE BOOL d_greater_than_or_equal(const T val1, const T val2)
{
  return (val1 > val2) || d_is_equal(val1, val2);
}

CC_INLINE BOOL pt2_less(const DPOINT2 point1, const DPOINT2 point2)
{
  if (point1.x < point2.x) {
    return true;
  }
  else if (point1.x > point2.x) {
    return false;
  }
  else if (point1.y < point2.y) {
    return true;
  }
  else {
    return false;
  }
}

CC_INLINE BOOL pt3_less(const DPOINT3 point1, const DPOINT3 point2)
{
  if (point1.x < point2.x) {
    return true;
  }
  else if (point1.x > point2.x) {
    return false;
  }
  else if (point1.y < point2.y) {
    return true;
  }
  else if (point1.y > point2.y) {
    return false;
  }
  else if (point1.z < point2.z) {
    return true;
  }
  else {
    return false;
  }
}

CC_INLINE BOOL pt2_greater(const DPOINT2 point1, const DPOINT2 point2)
{
  if (point1.x > point2.x) {
    return true;
  }
  else if (point1.x < point2.x) {
    return false;
  }
  else if (point1.y > point2.y) {
    return true;
  }
  else {
    return false;
  }
}

CC_INLINE BOOL pt3_greater(const DPOINT3 point1, const DPOINT3 point2)
{
  if (point1.x > point2.x) {
    return true;
  }
  else if (point1.x < point2.x) {
    return false;
  }
  else if (point1.y > point2.y) {
    return true;
  }
  else if (point1.y < point2.y) {
    return false;
  }
  else if (point1.z > point2.z) {
    return true;
  }
  else {
    return false;
  }
}

