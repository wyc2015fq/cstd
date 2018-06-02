
CC_INLINE DPOINT2 d2_make_point(const T x, const T y)
{
  DPOINT2 point;
  point.x = x;
  point.y = y;
  return point;
}

CC_INLINE DPOINT3 d3_make_point(const T x, const T y, const T z)
{
  DPOINT3 point;
  point.x = x;
  point.y = y;
  point.z = z;
  return point;
}

CC_INLINE DPOINT2 pt2_make_point(const DPOINT3 point)
{
  return d2_make_point(point.x, point.y);
}

CC_INLINE DPOINT3 pt3_make_point(const DPOINT2 point, const T z)
{
  return d3_make_point(point.x, point.y, z);
}

CC_INLINE DPOINT2 ci_make_point(const DCIRCLE circle)
{
  return d2_make_point(circle.x, circle.y);
}

CC_INLINE DPOINT3 sp_make_point(const DSPHERE sphere)
{
  return d3_make_point(sphere.x, sphere.y, sphere.z);
}

CC_INLINE DVECTOR2 d2_make_vector(const T x, const T y)
{
  DVECTOR2 v;
  v.x = x;
  v.y = y;
  return v;
}

CC_INLINE DVECTOR3 d3_make_vector(const T x, const T y, const T z)
{
  DVECTOR3 v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

CC_INLINE DVECTOR2 v2_make_vector(const DVECTOR3 v)
{
  return d2_make_vector(v.x, v.y);
}

CC_INLINE DVECTOR3 v3_make_vector(const DVECTOR2 v, const T z)
{
  return d3_make_vector(v.x, v.y, z);
}

CC_INLINE DVECTOR2 pt2_make_vector(const DPOINT2 point)
{
  return d2_make_vector(point.x, point.y);
}

CC_INLINE DVECTOR3 pt3_make_vector(const DPOINT3 point)
{
  return d3_make_vector(point.x, point.y, point.z);
}

CC_INLINE DCURVE_POINT2 d2_make_curve_point(const T x, const T y, const T t)
{
  DCURVE_POINT2 point;
  point.p0.x = x;
  point.p0.y = y;
  point.t = t;
  return point;
}

CC_INLINE T v2_vector_norm(const DVECTOR2 v)
{
  return sqrt((v.x * v.x) + (v.y * v.y));
}

CC_INLINE T v3_vector_norm(const DVECTOR3 v)
{
  return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

CC_INLINE DVECTOR2 v2_normalize(const DVECTOR2 v)
{
  T norm = v2_vector_norm(v);
  return d2_make_vector(v.x / norm, v.y / norm);
}

CC_INLINE DVECTOR3 v3_normalize(const DVECTOR3 v)
{
  T norm = v3_vector_norm(v);
  return d3_make_vector(v.x / norm, v.y / norm, v.z / norm);
}

CC_INLINE DRAY2 d2_make_ray(const T ox, const T oy, const T dir_x, const T dir_y)
{
  DRAY2 _ray;
  _ray.origin.x = ox;
  _ray.origin.y = oy;
  _ray.direction.x = dir_x;
  _ray.direction.y = dir_y;
  _ray.direction = v2_normalize(_ray.direction);
  return _ray;
}

CC_INLINE DRAY3 d3_make_ray(const T ox, const T oy, const T oz, const T dir_x, const T dir_y, const T dir_z)
{
  DRAY3 _ray;
  _ray.origin.x = ox;
  _ray.origin.y = oy;
  _ray.origin.z = oz;
  _ray.direction.x = dir_x;
  _ray.direction.y = dir_y;
  _ray.direction.z = dir_z;
  _ray.direction = v3_normalize(_ray.direction);
  return _ray;
}

CC_INLINE DRAY2 v2_make_ray(const DPOINT2 origin, const DVECTOR2 direction)
{
  return d2_make_ray(origin.x, origin.y, direction.x, direction.y);
}

CC_INLINE DRAY3 v3_make_ray(const DPOINT3 origin, const DVECTOR3 direction)
{
  return d3_make_ray(origin.x, origin.y, origin.z, direction.x, direction.y, direction.z);
}

CC_INLINE DRAY2 pt2_make_ray(const DPOINT2 origin, const T bearing)
{
  return v2_make_ray(origin, pt2_make_vector(pt2_project_point_angle(d2_make_point((0.0), (0.0)), bearing, (1.0))));
}

CC_INLINE DCURVE_POINT3 d3_make_curve_point(const T x, const T y, const T z, const T t)
{
  DCURVE_POINT3 point;
  point.p0.x = x;
  point.p0.y = y;
  point.p0.z = z;
  point.t = t;
  return point;
}

CC_INLINE DCURVE_POINT2 pt2_make_curve_point(const DPOINT2 point, const T t)
{
  return d2_make_curve_point(point.x, point.y, t);
}

CC_INLINE DCURVE_POINT3 pt3_make_curve_point(const DPOINT3 point, const T t)
{
  return d3_make_curve_point(point.x, point.y, point.z, t);
}

CC_INLINE DSEGMENT2 d2_make_segment(const T x1, const T y1, const T x2, const T y2)
{
  DSEGMENT2 segment;
  segment.p0 = d2_make_point(x1, y1);
  segment.p1 = d2_make_point(x2, y2);
  return segment;
}

CC_INLINE DSEGMENT3 d3_make_segment(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  DSEGMENT3 segment;
  segment.p0 = d3_make_point(x1, y1, z1);
  segment.p1 = d3_make_point(x2, y2, z2);
  return segment;
}

CC_INLINE DSEGMENT2 pt2_make_segment(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_make_segment(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE DSEGMENT3 pt3_make_segment(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_make_segment(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
}

CC_INLINE DSEGMENT2 li2_make_segment(const DLINE2 line)
{
  return pt2_make_segment(line.p0, line.p1);
}

CC_INLINE DSEGMENT3 li3_make_segment(const DLINE3 line)
{
  return pt3_make_segment(line.p0, line.p1);
}

CC_INLINE DSEGMENT2 ptn2_make_segment(const DPOINT2* p, int n, int i)
{
  return pt2_make_segment(p[i], p[(i + 1) % 3]);
}

CC_INLINE DSEGMENT3 ptn3_make_segment(const DPOINT3* p, int n, int i)
{
  return pt3_make_segment(p[i], p[(i + 1) % 3]);
}

CC_INLINE DSEGMENT2 tr2_make_segment(const DTRIANGLE2 triangle, int i)
{
  const DPOINT2* p = &triangle.p0;
  return pt2_make_segment(p[i], p[(i + 1) % 3]);
}

CC_INLINE DSEGMENT3 tr3_make_segment(const DTRIANGLE3 triangle, int i)
{
  const DPOINT3* p = &triangle.p0;
  return pt3_make_segment(p[i], p[(i + 1) % 3]);
}

CC_INLINE DSEGMENT2 qu2_make_segment(const DQUADIX2 quadix, int i)
{
  const DPOINT2* p = &quadix.p0;
  return pt2_make_segment(p[i], p[(i + 1) % 4]);
}

CC_INLINE DSEGMENT3 qu3_make_segment(const DQUADIX3 quadix, int i)
{
  const DPOINT3* p = &quadix.p0;
  return pt3_make_segment(p[i], p[(i + 1) % 4]);
}

CC_INLINE DLINE2 d2_make_line(const T x1, const T y1, const T x2, const T y2)
{
  DLINE2 line;
  line.p0 = d2_make_point(x1, y1);
  line.p1 = d2_make_point(x2, y2);
  return line;
}

CC_INLINE DLINE3 d3_make_line(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  DLINE3 line;
  line.p0 = d3_make_point(x1, y1, z1);
  line.p1 = d3_make_point(x2, y2, z2);
  return line;
}

CC_INLINE DLINE2 pt2_make_line(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_make_line(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE DLINE3 pt3_make_line(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_make_line(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
}

CC_INLINE DLINE2 se2_make_line(const DSEGMENT2 segment)
{
  return pt2_make_line(segment.p0, segment.p1);
}

CC_INLINE DLINE3 se3_make_line(const DSEGMENT3 segment)
{
  return pt3_make_line(segment.p0, segment.p1);
}

CC_INLINE unsigned int pt2_quadrant(const DPOINT2 point)
{
  return d2_quadrant(point.x, point.y);
}

CC_INLINE DPOINT2 pt2_degenerate()
{
  return d2_make_point(InfinityT, InfinityT);
}

CC_INLINE DPOINT3 pt3_degenerate()
{
  return d3_make_point(InfinityT, InfinityT, InfinityT);
}

CC_INLINE DVECTOR2 v2_degenerate()
{
  return d2_make_vector(InfinityT, InfinityT);
}

CC_INLINE DVECTOR3 v3_degenerate()
{
  return d3_make_vector(InfinityT, InfinityT, InfinityT);
}

CC_INLINE DPOINT2 ray2_generate_point_on_ray(const DRAY2 ray, const T t)
{
  if (t < (0.0)) {
    return pt2_degenerate();
  }

  return d2_make_point(ray.origin.x + t * ray.direction.x,
      ray.origin.y + t * ray.direction.y);
}

CC_INLINE DPOINT3 ray3_generate_point_on_ray(const DRAY3 ray, const T t)
{
  if (t < (0.0)) {
    return pt3_degenerate();
  }

  return d3_make_point(ray.origin.x + t * ray.direction.x,
      ray.origin.y + t * ray.direction.y,
      ray.origin.z + t * ray.direction.z);

}

CC_INLINE DLINE2 ray2_make_line(const DRAY2 ray)
{
  return pt2_make_line(ray.origin, ray2_generate_point_on_ray(ray, (1.0)));
}

CC_INLINE DLINE3 ray3_make_line(const DRAY3 ray)
{
  return pt3_make_line(ray.origin, ray3_generate_point_on_ray(ray, (1.0)));
}

CC_INLINE DRECTANGLE d2_make_rectangle(const T x1, const T y1, const T x2, const T y2)
{
  DRECTANGLE rectangle_;
  rectangle_.p0 = d2_make_point(x1, y1);
  rectangle_.p1 = d2_make_point(x2, y2);

  if (rectangle_.p1.x < rectangle_.p0.x) {
    T_SWAP(T, rectangle_.p0.x, rectangle_.p1.x);
  }

  if (rectangle_.p1.y < rectangle_.p0.y) {
    T_SWAP(T, rectangle_.p0.y, rectangle_.p1.y);
  }

  return rectangle_;
}

CC_INLINE DRECTANGLE pt2_make_rectangle(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_make_rectangle(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE DBOX3 d3_make_box(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  DBOX3 box_;
  box_.p0 = d3_make_point(x1, y1, z1);
  box_.p1 = d3_make_point(x2, y2, z2);

  if (box_.p1.x < box_.p0.x) {
    T_SWAP(T, box_.p0.x, box_.p1.x);
  }

  if (box_.p1.y < box_.p0.y) {
    T_SWAP(T, box_.p0.y, box_.p1.y);
  }

  if (box_.p1.z < box_.p0.z) {
    T_SWAP(T, box_.p0.z, box_.p1.z);
  }

  return box_;
}

CC_INLINE DBOX3 pt3_make_box(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_make_box(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
}

CC_INLINE DTRIANGLE2 d2_make_triangle(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3)
{
  DTRIANGLE2 triangle_;
  triangle_.p0 = d2_make_point(x1, y1);
  triangle_.p1 = d2_make_point(x2, y2);
  triangle_.p2 = d2_make_point(x3, y3);
  return triangle_;
}

CC_INLINE DTRIANGLE3 d3_make_triangle(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3)
{
  DTRIANGLE3 triangle_;
  triangle_.p0 = d3_make_point(x1, y1, z1);
  triangle_.p1 = d3_make_point(x2, y2, z2);
  triangle_.p2 = d3_make_point(x3, y3, z3);
  return triangle_;
}

CC_INLINE DTRIANGLE2 pt2_make_triangle(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_make_triangle(point1.x, point1.y,
      point2.x, point2.y,
      point3.x, point3.y);
}

CC_INLINE DTRIANGLE3 pt3_make_triangle(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_make_triangle(point1.x, point1.y, point1.z,
      point2.x, point2.y, point2.z,
      point3.x, point3.y, point3.z);
}

CC_INLINE DQUADIX2 d2_make_quadix(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3,
    const T x4, const T y4)
{
  DQUADIX2 quadix_;
  quadix_.p0 = d2_make_point(x1, y1);
  quadix_.p1 = d2_make_point(x2, y2);
  quadix_.p2 = d2_make_point(x3, y3);
  quadix_.p3 = d2_make_point(x4, y4);
  return quadix_;
}

CC_INLINE DQUADIX3 d3_make_quadix(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3,
    const T x4, const T y4, const T z4)
{
  DQUADIX3 quadix_;
  quadix_.p0 = d3_make_point(x1, y1, z1);
  quadix_.p1 = d3_make_point(x2, y2, z2);
  quadix_.p2 = d3_make_point(x3, y3, z3);
  quadix_.p3 = d3_make_point(x4, y4, z4);
  return quadix_;
}

CC_INLINE DQUADIX2 pt2_make_quadix(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d2_make_quadix(point1.x, point1.y,
      point2.x, point2.y,
      point3.x, point3.y,
      point4.x, point4.y);
}

CC_INLINE DQUADIX3 pt3_make_quadix(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3, const DPOINT3 point4)
{
  return d3_make_quadix(point1.x, point1.y, point1.z,
      point2.x, point2.y, point2.z,
      point3.x, point3.y, point3.z,
      point4.x, point4.y, point4.z);
}

CC_INLINE DQUADIX2 d22_make_quadix(const T x1, const T y1, const T x2, const T y2)
{
  DQUADIX2 quadix_;
  quadix_.p0 = d2_make_point(x1, y1);
  quadix_.p1 = d2_make_point(x2, y1);
  quadix_.p2 = d2_make_point(x2, y2);
  quadix_.p3 = d2_make_point(x1, y2);
  return quadix_;
}

CC_INLINE DQUADIX2 rt2_make_quadix(const DRECTANGLE rectangle)
{
  return d22_make_quadix(rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y);
}

CC_INLINE DCIRCLE d2_make_circle(const T x, const T y, const T radius)
{
  DCIRCLE circle_;
  circle_.x = x;
  circle_.y = y;
  circle_.radius = radius;
  return circle_;
}

CC_INLINE DCIRCLE pt2_make_circle(const DPOINT2 point, const T radius)
{
  return d2_make_circle(point.x, point.y, radius);
}

CC_INLINE DCIRCLE pt22_make_circle(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_make_circle((point1.x + point2.x) * (0.5), (point1.y + point2.y) * (0.5),  d2_distance(point1.x, point1.y, point2.x, point2.y) * (0.5));
}

CC_INLINE DCIRCLE d2_circumcircle(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  DPOINT2 c = d2_circumcenter(x1, y1, x2, y2, x3, y3);
  return d2_make_circle(c.x, c.y, d2_distance(c.x, c.y, x1, y1));
}

CC_INLINE DCIRCLE pt2_circumcircle(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_circumcircle(point1.x, point1.y,  point2.x, point2.y,  point3.x, point3.y);
}

CC_INLINE DCIRCLE tr2_circumcircle(const DTRIANGLE2 triangle)
{
  return pt2_circumcircle(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DCIRCLE pt23_make_circle(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_circumcircle(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE DCIRCLE tr2_make_circle(const DTRIANGLE2 triangle)
{
  return pt23_make_circle(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE DSPHERE d3_make_sphere(const T x, const T y, const T z, const T radius)
{
  DSPHERE sphere_;
  sphere_.x = x;
  sphere_.y = y;
  sphere_.z = z;
  sphere_.radius = radius;
  return sphere_;
}

CC_INLINE DSPHERE pt3_make_sphere(const DPOINT3 point, const T radius)
{
  return d3_make_sphere(point.x, point.y, point.z, radius);
}

CC_INLINE DSPHERE pt32_make_sphere(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_make_sphere((point1.x + point2.x) * (0.5), (point1.y + point2.y) * (0.5), (point1.z + point2.z) * (0.5),
      d3_distance(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z) * (0.5));
}

CC_INLINE DPLANE3 d3_make_plane(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3)
{
  DPLANE3 plane_;
  DVECTOR3 v1 = d3_make_vector(x2 - x1, y2 - y1, z2 - z1);
  DVECTOR3 v2 = d3_make_vector(x3 - x1, y3 - y1, z3 - z1);
  plane_.normal = v3_normalize(v3_mul(v1, v2));
  plane_.constant = -v3_dot_product(plane_.normal, d3_make_vector(x1, y1, z1));
  return plane_;
}

CC_INLINE DPLANE3 pt3_make_plane(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return d3_make_plane(point1.x, point1.y, point1.z,  point2.x, point2.y, point2.z,  point3.x, point3.y, point3.z);
}

CC_INLINE DPLANE3 tr3_make_plane(const DTRIANGLE3 triangle)
{
  return pt3_make_plane(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE int pt2_make_polygon(const DPOINT2* pt, int n, DPOINT2* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt[i];
  }

  return n;
}

CC_INLINE int pt3_make_polygon(const DPOINT3* pt, int n, DPOINT3* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt[i];
  }

  return n;
}

CC_INLINE int tr2_make_polygon(const DTRIANGLE2 triangle, DPOINT2* polygon_)
{
  return pt2_make_polygon(&triangle.p0, TrianglePointCount, polygon_);
}

CC_INLINE int qu2_make_polygon(const DQUADIX2 quadix, DPOINT2* polygon_)
{
  return pt2_make_polygon(&quadix.p0, QuadixPointCount, polygon_);
}

CC_INLINE int rt_make_polygon(const DRECTANGLE rectangle, DPOINT2* polygon_)
{
  polygon_[0] = (d2_make_point(rectangle.p0.x, rectangle.p0.y));
  polygon_[1] = (d2_make_point(rectangle.p1.x, rectangle.p0.y));
  polygon_[2] = (d2_make_point(rectangle.p1.x, rectangle.p1.y));
  polygon_[3] = (d2_make_point(rectangle.p0.x, rectangle.p1.y));
  return 4;
}

CC_INLINE int ci2_make_polygon(const DCIRCLE circle, int point_count /* 360*/, DPOINT2* polygon)
{
  int i;
  T angle = (360.0 / (1.0 * point_count));

  for (i = 0; i < point_count; ++i) {
    polygon[i] = d2_rotate_o(angle * (1.0 * i), circle.x + circle.radius, circle.y, circle.x, circle.y);
  }

  return 0;
}

