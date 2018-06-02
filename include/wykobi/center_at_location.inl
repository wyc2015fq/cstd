
CC_INLINE DSEGMENT2 se2_center_at_location(const DSEGMENT2 segment, const T x, const T y)
{
  DPOINT2 c = se2_segment_mid_point(segment);
  return se2_translate(x - c.x, y - c.y, segment);
}

CC_INLINE DSEGMENT3 se3_center_at_location(const DSEGMENT3 segment, const T x, const T y, const T z)
{
  DPOINT3 c = se3_segment_mid_point(segment);
  return se3_translate(x - c.x, y - c.y, z - c.z, segment);
}

CC_INLINE DTRIANGLE2 tr2_center_at_location(const DTRIANGLE2 triangle, const T x, const T y)
{
  DPOINT2 c = tr2_centroid(triangle);
  return tr2_translate(x - c.x, y - c.y, triangle);
}

CC_INLINE DRECTANGLE rt2_center_at_location(const DRECTANGLE rectangle, const T x, const T y)
{
  DPOINT2 c = rt2_centroid(rectangle);
  return rt2_translate(x - c.x, y - c.y, rectangle);
}

CC_INLINE DBOX3 box3_center_at_location(const DBOX3 box, const T x, const T y, const T z)
{
  DPOINT3 c = box3_centroid(box);
  return box3_translate(x - c.x, y - c.y, z - c.z, box);
}

CC_INLINE DQUADIX2 qu2_center_at_location(const DQUADIX2 quadix, const T x, const T y)
{
  DPOINT2 c = qu2_centroid(quadix);
  return qu2_translate(x - c.x, y - c.y, quadix);
}

CC_INLINE DCIRCLE ci2_center_at_location(const DCIRCLE circle, const T x, const T y)
{
  DCIRCLE circle_;
  circle_.x = x;
  circle_.y = y;
  circle_.radius = circle.radius;
  return circle_;
}

CC_INLINE int po2_center_at_location(int n, const DPOINT2* polygon, const T x, const T y, DPOINT2* polygon_)
{
  DPOINT2 c = po2_centroid(n, polygon);
  return po2_translate(x - c.x, y - c.y, n, polygon, polygon_);
}

CC_INLINE DSEGMENT2 sept2_center_at_location(const DSEGMENT2 segment, const DPOINT2 center_point)
{
  return se2_center_at_location(segment, center_point.x, center_point.y);
}

CC_INLINE DSEGMENT3 sept3_center_at_location(const DSEGMENT3 segment, const DPOINT3 center_point)
{
  return se3_center_at_location(segment, center_point.x, center_point.y, center_point.z);
}

CC_INLINE DTRIANGLE2 trpt2_center_at_location(const DTRIANGLE2 triangle, const DPOINT2 center_point)
{
  return tr2_center_at_location(triangle, center_point.x, center_point.y);
}

CC_INLINE DRECTANGLE rtpt2_center_at_location(const DRECTANGLE rectangle, const DPOINT2 center_point)
{
  return rt2_center_at_location(rectangle, center_point.x, center_point.y);
}

CC_INLINE DBOX3 boxpt3_center_at_location(const DBOX3 box, const DPOINT3 center_point)
{
  return box3_center_at_location(box, center_point.x, center_point.y, center_point.z);
}

CC_INLINE DQUADIX2 qupt2_center_at_location(const DQUADIX2 quadix, const DPOINT2 center_point)
{
  return qu2_center_at_location(quadix, center_point.x, center_point.y);
}

CC_INLINE DCIRCLE cipt2_center_at_location(const DCIRCLE circle, const DPOINT2 center_point)
{
  return ci2_center_at_location(circle, center_point.x, center_point.y);
}

CC_INLINE int popt2_center_at_location(int n, const DPOINT2* polygon, const DPOINT2 center_point, DPOINT2* polygon_)
{
  return po2_center_at_location(n, polygon, center_point.x, center_point.y, polygon_);
}

