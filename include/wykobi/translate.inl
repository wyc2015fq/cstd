
CC_INLINE DLINE2 li2_translate(const T dx, const T dy, const DLINE2 line)
{
  DLINE2 line_;
  ptn2_translate(dx, dy, LinePointCount, &line.p0, &line_.p0);
  return line_;
}

CC_INLINE DSEGMENT2 se2_translate(const T dx, const T dy, const DSEGMENT2 segment)
{
  DSEGMENT2 segment_;
  ptn2_translate(dx, dy, SegmentPointCount, &segment.p0, &segment_.p0);
  return segment_;
}

CC_INLINE DTRIANGLE2 tr2_translate(const T dx, const T dy, const DTRIANGLE2 triangle)
{
  DTRIANGLE2 triangle_;
  ptn2_translate(dx, dy, TrianglePointCount, &triangle.p0, &triangle_.p0);
  return triangle_;
}

CC_INLINE DQUADIX2 qu2_translate(const T dx, const T dy, const DQUADIX2 quadix)
{
  DQUADIX2 quadix_;
  ptn2_translate(dx, dy, QuadixPointCount, &quadix.p0, &quadix_.p0);
  return quadix_;
}

CC_INLINE DRECTANGLE rt2_translate(const T dx, const T dy, const DRECTANGLE rectangle)
{
  DRECTANGLE rectangle_;
  ptn2_translate(dx, dy, RectanglePointCount, &rectangle.p0, &rectangle_.p0);
  return rectangle_;
}

CC_INLINE DCIRCLE ci2_translate(const T dx, const T dy, const DCIRCLE circle)
{
  DCIRCLE circle_;
  circle_.x = circle.x + dx;
  circle_.y = circle.y + dy;
  circle_.radius = circle.radius;
  return circle_;
}

CC_INLINE int po2_translate(const T dx, const T dy, int n, const DPOINT2* polygon, DPOINT2* polygon_)
{
  return ptn2_translate(dx, dy, n, polygon, polygon_);
}

CC_INLINE DPOINT2 vpt2_translate(const DVECTOR2 v, const DPOINT2 point)
{
  return pt2_translate(v.x, v.y, point);
}

CC_INLINE DLINE2 vli2_translate(const DVECTOR2 v, const DLINE2 line)
{
  return li2_translate(v.x, v.y, line);
}

CC_INLINE DSEGMENT2 vse2_translate(const DVECTOR2 v, const DSEGMENT2 segment)
{
  return se2_translate(v.x, v.y, segment);
}

CC_INLINE DTRIANGLE2 vtr2_translate(const DVECTOR2 v, const DTRIANGLE2 triangle)
{
  return tr2_translate(v.x, v.y, triangle);
}

CC_INLINE DQUADIX2 vqu2_translate(const DVECTOR2 v, const DQUADIX2 quadix)
{
  return qu2_translate(v.x, v.y, quadix);
}

CC_INLINE DRECTANGLE vrt2_translate(const DVECTOR2 v, const DRECTANGLE rectangle)
{
  return rt2_translate(v.x, v.y, rectangle);
}

CC_INLINE DCIRCLE vci2_translate(const DVECTOR2 v, const DCIRCLE circle)
{
  return ci2_translate(v.x, v.y, circle);
}

CC_INLINE int vpo2_translate(const DVECTOR2 v, int n, const DPOINT2* polygon, DPOINT2* polygon_)
{
  return po2_translate(v.x, v.y, n, polygon, polygon_);
}

CC_INLINE DLINE3 li3_translate(const T dx, const T dy, const T dz, const DLINE3 line)
{
  DLINE3 line_;
  ptn3_translate(dx, dy, dz, LinePointCount, &line.p0, &line_.p0);
  return line_;
}

CC_INLINE DSEGMENT3 se3_translate(const T dx, const T dy, const T dz, const DSEGMENT3 segment)
{
  DSEGMENT3 segment_;
  ptn3_translate(dx, dy, dz, SegmentPointCount, &segment.p0, &segment_.p0);
  return segment_;
}

CC_INLINE DTRIANGLE3 tr3_translate(const T dx, const T dy, const T dz, const DTRIANGLE3 triangle)
{
  DTRIANGLE3 triangle_;
  ptn3_translate(dx, dy, dz, TrianglePointCount, &triangle.p0, &triangle_.p0);
  return triangle_;
}

CC_INLINE DQUADIX3 qu3_translate(const T dx, const T dy, const T dz, const DQUADIX3 quadix)
{
  DQUADIX3 quadix_;
  ptn3_translate(dx, dy, dz, QuadixPointCount, &quadix.p0, &quadix_.p0);
  return quadix_;
}

CC_INLINE DBOX3 box3_translate(const T dx, const T dy, const T dz, const DBOX3 box)
{
  DBOX3 box_;
  ptn3_translate(dx, dy, dz, BoxPointCount, &box.p0, &box_.p0);
  return box_;
}

CC_INLINE DSPHERE sp3_translate(const T dx, const T dy, const T dz, const DSPHERE sphere)
{
  DSPHERE sphere_;
  sphere_.x = sphere.x + dx;
  sphere_.y = sphere.y + dy;
  sphere_.z = sphere.z + dz;
  sphere_.radius = sphere.radius;
  return sphere_;
}

CC_INLINE int po3_translate(const T dx, const T dy, const T dz, int n, const DPOINT3* polygon, DPOINT3* polygon_)
{
  return ptn3_translate(dx, dy, dz, n, polygon, polygon_);
}

CC_INLINE DPOINT3 vpt3_translate(const DVECTOR3 v, const DPOINT3 point)
{
  return pt3_translate(v.x, v.y, v.z, point);
}

CC_INLINE DLINE3 vli3_translate(const DVECTOR3 v, const DLINE3 line)
{
  return li3_translate(v.x, v.y, v.z, line);
}

CC_INLINE DSEGMENT3 vse3_translate(const DVECTOR3 v, const DSEGMENT3 segment)
{
  return se3_translate(v.x, v.y, v.z, segment);
}

CC_INLINE DTRIANGLE3 vtr3_translate(const DVECTOR3 v, const DTRIANGLE3 triangle)
{
  return tr3_translate(v.x, v.y, v.z, triangle);
}

CC_INLINE DQUADIX3 vqu3_translate(const DVECTOR3 v, const DQUADIX3 quadix)
{
  return qu3_translate(v.x, v.y, v.z, quadix);
}

CC_INLINE DBOX3 vbox3_translate(const DVECTOR3 v, const DBOX3 box)
{
  return box3_translate(v.x, v.y, v.z, box);
}

CC_INLINE DSPHERE vsp3_translate(const DVECTOR3 v, const DSPHERE sphere)
{
  return sp3_translate(v.x, v.y, v.z, sphere);
}

CC_INLINE int vpo3_translate(const DVECTOR3 v, int n, const DPOINT3* polygon, DPOINT3* polygon_)
{
  return po3_translate(v.x, v.y, v.z, n, polygon, polygon_);
}

