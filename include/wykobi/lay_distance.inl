
CC_INLINE T pt2_lay_distance(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_lay_distance(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE T pt3_lay_distance(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_lay_distance(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
}

CC_INLINE T pttr2_lay_distance(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  return pt2_lay_distance(point, pt2_closest_point_on_triangle_from_point(triangle, point));
}

CC_INLINE T ptqu2_lay_distance(const DPOINT2 point, const DQUADIX2 quadix)
{
  return pt2_lay_distance(point, pt2_closest_point_on_quadix_from_point(quadix, point));
}

CC_INLINE T ptray2_lay_distance(const DPOINT2 point, const DRAY2 ray)
{
  return pt2_lay_distance(point, pt2_closest_point_on_ray_from_point(ray, point));
}

CC_INLINE T ptray3_lay_distance(const DPOINT3 point, const DRAY3 ray)
{
  return pt3_lay_distance(point, pt3_closest_point_on_ray_from_point(ray, point));
}

CC_INLINE T ptpl3_lay_distance(const DPOINT3 point, const DPLANE3 plane)
{
  return sqr((plane.normal.x * point.x + plane.normal.y * point.y + plane.normal.z * point.z) - plane.constant);
}

CC_INLINE T d2_lay_distance_segment_to_segment(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3,
    const T x4, const T y4)
{
  T dx, dy;
  T ux = x2 - x1, uy = y2 - y1;

  T vx = x4 - x3, vy = y4 - y3;

  T wx = x1 - x3, wy = y1 - y3;

  T a = (ux * ux + uy * uy);
  T b = (ux * vx + uy * vy);
  T c = (vx * vx + vy * vy);
  T d = (ux * wx + uy * wy);
  T e = (vx * wx + vy * wy);
  T dt = a * c - b * b;

  T sc = (0.0), sn = (0.0);
  T tc = (0.0), tn = (0.0);
  T sd = dt, td = dt;

  if (d_is_equal(dt, (0.0))) {
    sn = (0.0);
    sd = (1.0);
    tn = e;
    td = c;
  }
  else {
    sn = (b * e - c * d);
    tn = (a * e - b * d);

    if (sn < (0.0)) {
      sn = (0.0);
      tn = e;
      td = c;
    }
    else if (sn > sd) {
      sn = sd;
      tn = e + b;
      td = c;
    }
  }

  if (tn < (0.0)) {
    tn = (0.0);

    if (-d < (0.0)) {
      sn = (0.0);
    }
    else if (-d > a) {
      sn = sd;
    }
    else {
      sn = -d;
      sd = a;
    }
  }
  else if (tn > td) {
    tn = td;

    if ((-d + b) < (0.0)) {
      sn = (0.0);
    }
    else if ((-d + b) > a) {
      sn = sd;
    }
    else {
      sn = (-d + b);
      sd = a;
    }
  }

  if (d_is_equal(sn, (0.0))) {
    sc = (0.0);
  }
  else {
    sc = sn / sd;
  }

  if (d_is_equal(tn, (0.0))) {
    tc = (0.0);
  }
  else {
    tc = tn / td;
  }

  dx = wx + (sc * ux) - (tc * vx);
  dy = wy + (sc * uy) - (tc * vy);
  return dx * dx + dy * dy;
}

CC_INLINE T d3_lay_distance_segment_to_segment(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3,
    const T x4, const T y4, const T z4)
{
  T sc, tc, dx, dy, dz;
  T ux = x2 - x1, uy = y2 - y1, uz = z2 - z1;
  T vx = x4 - x3, vy = y4 - y3, vz = z4 - z3;
  T wx = x1 - x3, wy = y1 - y3, wz = z1 - z3;
  T a = (ux * ux + uy * uy + uz * uz);
  T b = (ux * vx + uy * vy + uz * vz);
  T c = (vx * vx + vy * vy + vz * vz);
  T d = (ux * wx + uy * wy + uz * wz);
  T e = (vx * wx + vy * wy + vz * wz);
  T dt = a * c - b * b;
  T sd = dt, td = dt;
  T sn = (0.0), tn = (0.0);

  if (d_is_equal(dt, (0.0))) {
    sn = (0.0);
    sd = (1.00);
    tn = e;
    td = c;
  }
  else {
    sn = (b * e - c * d);
    tn = (a * e - b * d);

    if (sn < (0.0)) {
      sn = (0.0);
      tn = e;
      td = c;
    }
    else if (sn > sd) {
      sn = sd;
      tn = e + b;
      td = c;
    }
  }

  if (tn < (0.0)) {
    tn = (0.0);

    if (-d < (0.0)) {
      sn = (0.0);
    }
    else if (-d > a) {
      sn = sd;
    }
    else {
      sn = -d;
      sd = a;
    }
  }
  else if (tn > td) {
    tn = td;

    if ((-d + b) < (0.0)) {
      sn = (0.0);
    }
    else if ((-d + b) > a) {
      sn = sd;
    }
    else {
      sn = (-d + b);
      sd = a;
    }
  }

  sc = (0.0), tc = (0.0);

  if (d_is_equal(sn, (0.0))) {
    sc = (0.0);
  }
  else {
    sc = sn / sd;
  }

  if (d_is_equal(tn, (0.0))) {
    tc = (0.0);
  }
  else {
    tc = tn / td;
  }

  dx = wx + (sc * ux) - (tc * vx);
  dy = wy + (sc * uy) - (tc * vy);
  dz = wz + (sc * uz) - (tc * vz);
  return dx * dx + dy * dy + dz * dz;
}

CC_INLINE T d2_lay_distance_line_to_line(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3,
    const T x4, const T y4)
{
  T dx, dy;
  T ux = x2 - x1;
  T uy = y2 - y1;

  T vx = x4 - x3;
  T vy = y4 - y3;

  if (d_not_equal(ux * vy, uy * vx)) {
    return (0.0);
  }

  {
    T wx = x1 - x3;
    T wy = y1 - y3;

    T a = (ux * ux + uy * uy);
    T b = (ux * vx + uy * vy);
    T c = (vx * vx + vy * vy);
    T d = (ux * wx + uy * wy);
    T e = (vx * wx + vy * wy);
    T dt = a * c - b * b;
    T sc = (0.0);
    T tc = (0.0);

    if (d_is_equal(dt, (0.0))) {
      sc = (0.0);

      if (b > c) {
        tc = d / b;
      }
      else {
        tc = e / c;
      }
    }
    else {
      sc = (b * e - c * d) / dt;
      tc = (a * e - b * d) / dt;
    }

    dx = wx + (sc * ux) - (tc * vx);
    dy = wy + (sc * uy) - (tc * vy);
  }

  return dx * dx + dy * dy;
}

CC_INLINE T d3_lay_distance_line_to_line(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3,
    const T x4, const T y4, const T z4)
{
  T ux = x2 - x1;
  T uy = y2 - y1;
  T uz = z2 - z1;

  T vx = x4 - x3;
  T vy = y4 - y3;
  T vz = z4 - z3;

  T wx = x1 - x3;
  T wy = y1 - y3;
  T wz = z1 - z3;

  T a = (ux * ux + uy * uy + uz * uz);
  T b = (ux * vx + uy * vy + uz * vz);
  T c = (vx * vx + vy * vy + vz * vz);
  T d = (ux * wx + uy * wy + uz * wz);
  T e = (vx * wx + vy * wy + vz * wz);
  T dt = a * c - b * b;
  T sc = (0.0);
  T tc = (0.0);
  T dx, dy, dz;

  if (d_is_equal(dt, (0.0))) {
    sc = (0.0);

    if (b > c) {
      tc = d / b;
    }
    else {
      tc = e / c;
    }
  }
  else {
    sc = (b * e - c * d) / dt;
    tc = (a * e - b * d) / dt;
  }

  dx = wx + (sc * ux) - (tc * vx);
  dy = wy + (sc * uy) - (tc * vy);
  dz = wz + (sc * uz) - (tc * vz);
  return dx * dx + dy * dy + dz * dz;
}

CC_INLINE T sese2_lay_distance(const DSEGMENT2 segment1, const DSEGMENT2 segment2)
{
  return d2_lay_distance_segment_to_segment(segment1.p0.x, segment1.p0.y,
      segment1.p1.x, segment1.p1.y,
      segment2.p0.x, segment2.p0.y,
      segment2.p1.x, segment2.p1.y);
}

CC_INLINE T sese3_lay_distance(const DSEGMENT3 segment1, const DSEGMENT3 segment2)
{
  return d3_lay_distance_segment_to_segment(segment1.p0.x, segment1.p0.y, segment1.p0.z,
      segment1.p1.x, segment1.p1.y, segment1.p1.z,
      segment2.p0.x, segment2.p0.y, segment2.p0.z,
      segment2.p1.x, segment2.p1.y, segment2.p1.z);
}

CC_INLINE T lili3_lay_distance(const DLINE3 line1, const DLINE3 line2)
{
  return d3_lay_distance_line_to_line(line1.p0.x, line1.p0.y, line1.p0.z,
      line1.p1.x, line1.p1.y, line1.p1.z,
      line2.p0.x, line2.p0.y, line2.p0.z,
      line2.p1.x, line2.p1.y, line2.p1.z);
}

CC_INLINE T se2_lay_distance(const DSEGMENT2 segment)
{
  return pt2_lay_distance(segment.p0, segment.p1);
}

CC_INLINE T se3_lay_distance(const DSEGMENT3 segment)
{
  return pt3_lay_distance(segment.p0, segment.p1);
}

CC_INLINE T setr2_lay_distance(const DSEGMENT2 segment, const DTRIANGLE2 triangle)
{
  T d1, d2, d3;
  d1 = d2_lay_distance_segment_to_segment(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, triangle.p0.x, triangle.p0.y, triangle.p1.x, triangle.p1.y);
  d2 = d2_lay_distance_segment_to_segment(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y);
  d3 = d2_lay_distance_segment_to_segment(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, triangle.p2.x, triangle.p2.y, triangle.p0.x, triangle.p0.y);
  return min(min(d1, d2), d3);
}

CC_INLINE T setr3_lay_distance(const DSEGMENT3 segment, const DTRIANGLE3 triangle)
{
  T d1, d2, d3;
  d1 = d3_lay_distance_segment_to_segment(segment.p0.x, segment.p0.y, segment.p0.z, segment.p1.x, segment.p1.y, segment.p1.z, triangle.p0.x, triangle.p0.y, triangle.p0.z, triangle.p1.x, triangle.p1.y, triangle.p1.z);
  d2 = d3_lay_distance_segment_to_segment(segment.p0.x, segment.p0.y, segment.p0.z, segment.p1.x, segment.p1.y, segment.p1.z, triangle.p1.x, triangle.p1.y, triangle.p1.z, triangle.p2.x, triangle.p2.y, triangle.p2.z);
  d3 = d3_lay_distance_segment_to_segment(segment.p0.x, segment.p0.y, segment.p0.z, segment.p1.x, segment.p1.y, segment.p1.z, triangle.p2.x, triangle.p2.y, triangle.p2.z, triangle.p0.x, triangle.p0.y, triangle.p0.z);
  return min(min(d1, d2), d3);
}

