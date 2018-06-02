
CC_INLINE BOOL pt2_point_on_segment(const DPOINT2 point, const DSEGMENT2 segment)
{
  return se2_is_point_collinear(segment, point, true);
}

CC_INLINE BOOL pt3_point_on_segment(const DPOINT3 point, const DSEGMENT3 segment)
{
  return se3_is_point_collinear(segment, point, true);
}

CC_INLINE BOOL pt2_point_on_ray(const DPOINT2 point, const DRAY2 ray)
{
  T t = v2_dot_product(ray.direction, v2_sub(point, ray.origin));

  if (d_greater_than_or_equal(t, (0.0))) {
    return pt2_is_equal(point, ray2_generate_point_on_ray(ray, t));
  }
  else {
    return false;
  }
}

CC_INLINE BOOL d2_point_on_ray(const T px, const T py,
    const T ox, const T oy,
    const T dx, const T dy)
{
  return pt2_point_on_ray(d2_make_point(px, py), d2_make_ray(ox, oy, dx, dy));
}

CC_INLINE BOOL pt3_point_on_ray(const DPOINT3 point, const DRAY3 ray)
{
  T t = v3_dot_product(ray.direction, v3_sub(point, ray.origin));

  if (d_greater_than_or_equal(t, (0.0))) {
    return pt3_is_equal(point, ray3_generate_point_on_ray(ray, t));
  }
  else {
    return false;
  }
}

CC_INLINE BOOL d3_point_on_ray(const T px, const T py, const T pz,
    const T ox, const T oy, const T oz,
    const T dx, const T dy, const T dz)
{
  return pt3_point_on_ray(d3_make_point(px, py, pz), d3_make_ray(ox, oy, oz, dx, dy, dz));
}

CC_INLINE BOOL d2_point_on_rectangle(const T px, const T py,
    const T x1, const T y1,
    const T x2, const T y2)
{
  return (((x1 <= px) && (px <= x2)) && ((py == y1) || (py == y2))) ||
      (((y1 <= py) && (py <= y2)) && ((px == x1) || (px == x2)));
}

CC_INLINE BOOL pt2_point_on_rectangle(const DPOINT2 point, const DRECTANGLE rectangle)
{
  return d2_point_on_rectangle(point.x, point.y, rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y);
}

CC_INLINE BOOL d2_point_on_triangle(const T px, const T py,
    const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3)
{
  return d2_is_point_collinear(x1, y1, x2, y2, px, py, true) ||
      d2_is_point_collinear(x2, y2, x3, y3, px, py, true) ||
      d2_is_point_collinear(x3, y3, x1, y1, px, py, true);
}

CC_INLINE BOOL pt2_point_on_triangle(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  return d2_point_on_triangle(point.x, point.y,
      triangle.p0.x, triangle.p0.y,
      triangle.p1.x, triangle.p1.y,
      triangle.p2.x, triangle.p2.y);
}

CC_INLINE BOOL d2_point_on_quadix(const T px, const T py,
    const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3,
    const T x4, const T y4)
{
  return d2_is_point_collinear(x1, y1, x2, y2, px, py, true) ||
      d2_is_point_collinear(x2, y2, x3, y3, px, py, true) ||
      d2_is_point_collinear(x3, y3, x4, y4, px, py, true) ||
      d2_is_point_collinear(x4, y4, x1, y1, px, py, true);
}

CC_INLINE BOOL pt2_point_on_quadix(const DPOINT2 point, const DQUADIX2 quadix)
{
  return d2_point_on_quadix(point.x, point.y,
      quadix.p0.x, quadix.p0.y,
      quadix.p1.x, quadix.p1.y,
      quadix.p2.x, quadix.p2.y,
      quadix.p3.x, quadix.p3.y);
}

CC_INLINE BOOL d2_point_on_circle(const T px, const T py, const T cx, const T cy, const T radius)
{
  return (d2_lay_distance(px, py, cx, cy) == (radius * radius));
}

CC_INLINE BOOL pt2_point_on_circle(const DPOINT2 point, const DCIRCLE circle)
{
  return d2_point_on_circle(point.x, point.y, circle.x, circle.y, circle.radius);
}

CC_INLINE BOOL pt2_point_on_qbezier(const DPOINT2 point, const DQUADRATIC_BEZIER2 bezier, const int steps)
{
  return (d_is_equal(pt2_distance(pt2_closest_point_on_qbezier_from_point(bezier, point, steps), point), (0.0)));
}

CC_INLINE BOOL pt2_point_on_cbezier(const DPOINT2 point, const DCUBIC_BEZIER2 bezier, const int steps)
{
  return (d_is_equal(pt2_distance(pt2_closest_point_on_cbezier_from_point(bezier, point, steps), point), (0.0)));
}

CC_INLINE BOOL pt3_point_on_qbezier(const DPOINT3 point, const DQUADRATIC_BEZIER3 bezier, const int steps)
{
  return (d_is_equal(pt3_distance(pt3_closest_point_on_qbezier_from_point(bezier, point, steps), point), (0.0)));
}

CC_INLINE BOOL pt3_point_on_cbezier(const DPOINT3 point, const DCUBIC_BEZIER3 bezier, const int steps)
{
  return (d_is_equal(pt3_distance(pt3_closest_point_on_cbezier_from_point(bezier, point, steps), point), (0.0)));
}

CC_INLINE BOOL pt2_point_in_convex_polygon(const DPOINT2 point, const DPOLYGON2* polygon)
{
  return d2_point_in_convex_polygon(point.x, point.y, polygon);
}

CC_INLINE BOOL d2_point_on_polygon_edge(const T px, const T py, const DPOLYGON2* polygon)
{
  const DPOINT2* p = polygon->p;
  int i, j = 0, n = polygon->n;

  if (polygon->n < 3) {
    return false;
  }

  j = n - 1;

  for (i = 0; i < polygon->n; ++i) {
    if (d2_is_point_collinear(p[i].x, p[i].y, p[j].x, p[j].y, px, py, true)) {
      return true;
    }

    j = i;
  }

  return false;
}

CC_INLINE BOOL pt2_point_on_polygon_edge(const DPOINT2 point, const DPOLYGON2* polygon)
{
  return d2_point_on_polygon_edge(point.x, point.y, polygon);
}

