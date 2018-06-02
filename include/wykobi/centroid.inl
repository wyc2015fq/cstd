
CC_INLINE DPOINT2 d22_centroid(const T x1, const T y1, const T x2, const T y2)
{
  DPOINT2 pt;
  pt.x = (x1 + x2) * (0.5);
  pt.y = (y1 + y2) * (0.5);
  return pt;
}

CC_INLINE DPOINT2 pt22_centroid(const DPOINT2 point1, const DPOINT2 point2)
{
  return d22_centroid(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE DPOINT2 se22_centroid(const DSEGMENT2 segment)
{
  return pt2_segment_mid_point(segment.p0, segment.p1);
}

CC_INLINE DPOINT2 d23_centroid(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  DPOINT2 pt, mid1, mid2;
  mid1 = d2_segment_mid_point(x2, y2, x3, y3);
  mid2 = d2_segment_mid_point(x1, y1, x3, y3);
  d2_intersect_at(x1, y1, mid1.x, mid1.y, x2, y2, mid2.x, mid2.y, &pt);
  return pt;
}

CC_INLINE DPOINT3 d33_centroid(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3)
{
  DPOINT3 mid1, mid2;
  mid1 = d3_segment_mid_point(x2, y2, z2, x3, y3, z3);
  mid2 = d3_segment_mid_point(x1, y1, z1, x3, y3, z3);
  return d3_intersection_point(x1, y1, z1, mid1.x, mid1.y, mid1.z, x2, y2, z2, mid2.x, mid2.y, mid2.z);
}

CC_INLINE DPOINT2 d24_centroid(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4)
{
  T x = 0.0, y = 0.0;
  T asum = 0.0;
  T term = 0.0;

  term = ((x4 * y1) - (y4 * x1));
  asum = asum + term;
  x = x + (x4 + x1) * term;
  y = y + (y4 + y1) * term;

  term = ((x1 * y2) - (y1 * x2));
  asum = asum + term;
  x = x + (x1 + x2) * term;
  y = y + (y1 + y2) * term;

  term = ((x2 * y3) - (y2 * x3));
  asum = asum + term;
  x = x + (x2 + x3) * term;
  y = y + (y2 + y3) * term;

  term = ((x3 * y4) - (y3 * x4));
  asum = asum + term;
  x = x + (x3 + x4) * term;
  y = y + (y3 + y4) * term;

  if (asum != (0.0)) {
    x = x / ((3.0) * asum);
    y = y / ((3.0) * asum);
  }

  return d2_make_point(x, y);
}

CC_INLINE DPOINT2 tr2_centroid(const DTRIANGLE2 triangle)
{
  return d23_centroid(triangle.p0.x, triangle.p0.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y);
}

CC_INLINE DPOINT3 tr3_centroid(const DTRIANGLE3 triangle)
{
  return d33_centroid(triangle.p0.x, triangle.p0.y, triangle.p0.z, triangle.p1.x, triangle.p1.y, triangle.p1.z, triangle.p2.x, triangle.p2.y, triangle.p2.z);
}

CC_INLINE DPOINT2 qu2_centroid(const DQUADIX2 quadix)
{
  return d24_centroid(quadix.p0.x, quadix.p0.y, quadix.p1.x, quadix.p1.y, quadix.p2.x, quadix.p2.y, quadix.p3.x, quadix.p3.y);
}

CC_INLINE DPOINT2 rt2_centroid(const DRECTANGLE rectangle)
{
  DPOINT2 pt;
  pt.x = (rectangle.p0.x + rectangle.p1.x) * (0.5);
  pt.y = (rectangle.p0.y + rectangle.p1.y) * (0.5);
  return pt;
}

CC_INLINE DPOINT3 box3_centroid(const DBOX3 box)
{
  DPOINT3 pt;
  pt.x = (box.p0.x + box.p1.x) * (0.5);
  pt.y = (box.p0.y + box.p1.y) * (0.5);
  pt.z = (box.p0.z + box.p1.z) * (0.5);
  return pt;
}

CC_INLINE DPOINT2 po2_centroid(int n, const DPOINT2* polygon)
{
  int i, j;
  T asum, x = (0.0), y = (0.0);

  if (n < 3) {
    return d2_make_point(x, y);
  }

  asum = (0.0);
  j = n - 1;

  for (i = 0; i < n; ++i) {
    T term = ((polygon[j].x * polygon[i].y) - (polygon[j].y * polygon[i].x));
    asum += term;
    x += ((polygon[j].x + polygon[i].x) * term);
    y += ((polygon[j].y + polygon[i].y) * term);
    j = i;
  }

  if (d_not_equal(asum, (0.0))) {
    x /= ((3.0) * asum);
    y /= ((3.0) * asum);
  }

  return d2_make_point(x, y);
}

CC_INLINE DPOINT2 pt23_centroid(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d23_centroid(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DPOINT2 pt24_centroid(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d24_centroid(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, point4.x, point4.y);
}

