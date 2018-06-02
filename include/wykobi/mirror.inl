
CC_INLINE DPOINT2 d2_mirror(const T px, const T py, const T x1, const T y1, const T x2, const T y2)
{
  DPOINT2 pt = d2_closest_point_on_line_from_point(x1, y1, x2, y2, px, py);
  pt.x = px + (2.0) * (pt.x - px);
  pt.y = py + (2.0) * (pt.y - py);
  return pt;
}

CC_INLINE DPOINT3 d3_mirror(const T px, const T py, const T pz, const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  DPOINT3 pt = d3_closest_point_on_line_from_point(x1, y1, z1, x2, y2, z2, px, py, pz);
  pt.x = px + (2.0) * (pt.x - px);
  pt.y = py + (2.0) * (pt.y - py);
  pt.z = pz + (2.0) * (pt.z - pz);
  return pt;
}

CC_INLINE DPOINT2 ptli2_mirror(const DPOINT2 point, const DLINE2 mirror_axis)
{
  return d2_mirror(point.x, point.y, mirror_axis.p0.x, mirror_axis.p0.y, mirror_axis.p1.x, mirror_axis.p1.y);
}

CC_INLINE DSEGMENT2 seli2_mirror(const DSEGMENT2 segment, const DLINE2 mirror_axis)
{
  int i;
  DSEGMENT2 segment_;
  DPOINT2* p_ = &segment_.p0;
  const DPOINT2* p = &segment.p0;

  for (i = 0; i < SegmentPointCount; ++i) {
    p_[i] = ptli2_mirror(p[i], mirror_axis);
  }

  return segment_;
}

CC_INLINE DLINE2 lili2_mirror(const DLINE2 line, const DLINE2 mirror_axis)
{
  DLINE2 line_;
  DPOINT2* p_ = &line_.p0;
  const DPOINT2* p = &line.p0;

  int i;

  for (i = 0; i < LinePointCount; ++i) {
    p_[i] = ptli2_mirror(p[i], mirror_axis);
  }

  return line_;
}

CC_INLINE DRECTANGLE rtli2_mirror(const DRECTANGLE rectangle, const DLINE2 mirror_axis)
{
  DRECTANGLE rectangle_;
  DPOINT2* p_ = &rectangle_.p0;
  const DPOINT2* p = &rectangle.p0;

  int i;

  for (i = 0; i < RectanglePointCount; ++i) {
    p_[i] = ptli2_mirror(p[i], mirror_axis);
  }

  return rectangle_;
}

CC_INLINE DTRIANGLE2 trli2_mirror(const DTRIANGLE2 triangle, const DLINE2 mirror_axis)
{
  DTRIANGLE2 triangle_;
  DPOINT2* p_ = &triangle_.p0;
  const DPOINT2* p = &triangle.p0;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    p_[i] = ptli2_mirror(p[i], mirror_axis);
  }

  return triangle_;
}

CC_INLINE DQUADIX2 quli2_mirror(const DQUADIX2 quadix, const DLINE2 mirror_axis)
{
  DQUADIX2 quadix_;
  DPOINT2* p_ = &quadix_.p0;
  const DPOINT2* p = &quadix.p0;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    p_[i] = ptli2_mirror(p[i], mirror_axis);
  }

  return quadix_;
}

CC_INLINE DCIRCLE cili2_mirror(const DCIRCLE circle, const DLINE2 mirror_axis)
{
  DPOINT2 pt = d2_mirror(circle.x, circle.y, mirror_axis.p0.x, mirror_axis.p0.y, mirror_axis.p1.x, mirror_axis.p1.y);
  return pt2_make_circle(pt, circle.radius);

}

CC_INLINE int poli2_mirror(int n, const DPOINT2* polygon, const DLINE2 mirror_axis, DPOINT2* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = ptli2_mirror(polygon[i], mirror_axis);
  }

  return 0;
}

CC_INLINE DPOINT3 ptli3_mirror(const DPOINT3 point, const DLINE3 mirror_axis)
{
  return d3_mirror(point.x, point.y, point.z, mirror_axis.p0.x, mirror_axis.p0.y, mirror_axis.p0.z, mirror_axis.p1.x, mirror_axis.p1.y, mirror_axis.p1.z);
}

CC_INLINE DSEGMENT3 seli3_mirror(const DSEGMENT3 segment, const DLINE3 mirror_axis)
{
  DSEGMENT3 segment_;
  DPOINT3* p_ = &segment_.p0;
  const DPOINT3* p = &segment.p0;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    p_[i] = ptli3_mirror(p[i], mirror_axis);
  }

  return segment_;
}

CC_INLINE DLINE3 lili3_mirror(const DLINE3 line, const DLINE3 mirror_axis)
{
  DLINE3 line_;
  DPOINT3* p_ = &line_.p0;
  const DPOINT3* p = &line.p0;

  int i;

  for (i = 0; i < LinePointCount; ++i) {
    p_[i] = ptli3_mirror(p[i], mirror_axis);
  }

  return line_;
}

CC_INLINE DBOX3 boxli3_mirror(const DBOX3 box, const DLINE3 mirror_axis)
{
  DBOX3 box_;
  DPOINT3* p_ = &box_.p0;
  const DPOINT3* p = &box.p0;

  int i;

  for (i = 0; i < BoxPointCount; ++i) {
    p_[i] = ptli3_mirror(p[i], mirror_axis);
  }

  return box_;
}

CC_INLINE DTRIANGLE3 trli3_mirror(const DTRIANGLE3 triangle, const DLINE3 mirror_axis)
{
  DTRIANGLE3 triangle_;
  DPOINT3* p_ = &triangle_.p0;
  const DPOINT3* p = &triangle.p0;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    p_[i] = ptli3_mirror(p[i], mirror_axis);
  }

  return triangle_;
}

CC_INLINE DQUADIX3 quli3_mirror(const DQUADIX3 quadix, const DLINE3 mirror_axis)
{
  DQUADIX3 quadix_;
  DPOINT3* p_ = &quadix_.p0;
  const DPOINT3* p = &quadix.p0;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    p_[i] = ptli3_mirror(p[i], mirror_axis);
  }

  return quadix_;
}

CC_INLINE DSPHERE spli3_mirror(const DSPHERE sphere, const DLINE3 mirror_axis)
{
  DPOINT3 pt = d3_mirror(sphere.x, sphere.y, sphere.z, mirror_axis.p0.x, mirror_axis.p0.y, mirror_axis.p0.z, mirror_axis.p1.x, mirror_axis.p1.y, mirror_axis.p1.z);
  return pt3_make_sphere(pt, sphere.radius);
}

CC_INLINE int poli3_mirror(int n, const DPOINT3* polygon, const DLINE3 mirror_axis, DPOINT3* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = ptli3_mirror(polygon[i], mirror_axis);
  }

  return 0;
}

CC_INLINE DPOINT3 ptpl3_mirror(const DPOINT3 point, const DPLANE3 plane)
{
  DPOINT3 pt = pt3_closest_point_on_plane_from_point(plane, point);
  return v3_sub(v3_scale(pt, 2.0), point);
}

CC_INLINE DSEGMENT3 sepl3_mirror(const DSEGMENT3 segment, const DPLANE3 plane)
{
  DSEGMENT3 segment_;
  DPOINT3* p_ = &segment_.p0;
  const DPOINT3* p = &segment.p0;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    p_[i] = ptpl3_mirror(p[i], plane);
  }

  return segment_;
}

CC_INLINE DLINE3 lipl3_mirror(const DLINE3 line, const DPLANE3 plane)
{
  DLINE3 line_;
  DPOINT3* p_ = &line_.p0;
  const DPOINT3* p = &line.p0;

  int i;

  for (i = 0; i < LinePointCount; ++i) {
    p_[i] = ptpl3_mirror(p[i], plane);
  }

  return line_;
}

CC_INLINE DBOX3 boxpl3_mirror(const DBOX3 box, const DPLANE3 plane)
{
  DBOX3 box_;
  DPOINT3* p_ = &box_.p0;
  const DPOINT3* p = &box.p0;

  int i;

  for (i = 0; i < BoxPointCount; ++i) {
    p_[i] = ptpl3_mirror(p[i], plane);
  }

  return box_;
}

CC_INLINE DTRIANGLE3 trpl3_mirror(const DTRIANGLE3 triangle, const DPLANE3 plane)
{
  DTRIANGLE3 triangle_;
  DPOINT3* p_ = &triangle_.p0;
  const DPOINT3* p = &triangle.p0;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    p_[i] = ptpl3_mirror(p[i], plane);
  }

  return triangle_;
}

CC_INLINE DQUADIX3 qupl3_mirror(const DQUADIX3 quadix, const DPLANE3 plane)
{
  DQUADIX3 quadix_;
  DPOINT3* p_ = &quadix_.p0;
  const DPOINT3* p = &quadix.p0;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    p_[i] = ptpl3_mirror(p[i], plane);
  }

  return quadix_;
}

CC_INLINE DSPHERE sppl3_mirror(const DSPHERE sphere, const DPLANE3 plane)
{
  DPOINT3 center = sp_make_point(sphere);
  return pt3_make_sphere(ptpl3_mirror(center, plane), sphere.radius);
}

CC_INLINE int popl3_mirror(int n, const DPOINT3* polygon, const DPLANE3 plane, DPOINT3* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = ptpl3_mirror(polygon[i], plane);
  }

  return 0;
}

