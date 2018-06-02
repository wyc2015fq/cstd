
CC_INLINE BOOL se2_point_in_rectangle(const DPOINT2 point, const DSEGMENT2 segment)
{
  return d2_point_in_rectangle(point.x, point.y, segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y);
}

CC_INLINE BOOL rt2_point_in_rectangle(const DPOINT2 point, const DRECTANGLE rectangle)
{
  return d2_point_in_rectangle(point.x, point.y, rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y);
}

CC_INLINE BOOL box3_point_in_box(const DPOINT3 point, const DBOX3 box)
{
  return d3_point_in_box(point.x, point.y, point.z, box.p0.x, box.p0.y, box.p0.z, box.p1.x, box.p1.y, box.p1.z);
}

CC_INLINE BOOL se3_point_in_box(const DPOINT3 point, const DSEGMENT3 segment)
{
  return d3_point_in_box(point.x, point.y, point.z, segment.p0.x, segment.p0.y, segment.p0.z, segment.p1.x, segment.p1.y, segment.p1.z);
}

CC_INLINE BOOL tr2_point_in_triangle(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  return d2_point_in_triangle(point.x, point.y, triangle.p0.x, triangle.p0.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y);
}

CC_INLINE BOOL qu2_point_in_quadix(const DPOINT2 point, const DQUADIX2 quadix)
{
  return d2_point_in_quadix(point.x, point.y, quadix.p0.x, quadix.p0.y, quadix.p1.x, quadix.p1.y, quadix.p2.x, quadix.p2.y, quadix.p3.x, quadix.p3.y);
}

CC_INLINE BOOL d2_point_in_circle(const T px, const T py, const T cx, const T cy, const T radius)
{
  return (d2_lay_distance(px, py, cx, cy) <= (radius * radius));
}

CC_INLINE BOOL ci2_point_in_circle(const DPOINT2 point, const DCIRCLE circle)
{
  return d2_point_in_circle(point.x, point.y, circle.x, circle.y, circle.radius);
}

CC_INLINE BOOL d3_point_in_sphere(const T px, const T py, const T pz, const T cx, const T cy, const T cz, const T radius)
{
  return d_less_than_or_equal(d3_lay_distance(px, py, pz, cx, cy, cz), radius * radius);
}

CC_INLINE BOOL sp3_point_in_sphere(const DPOINT3 point, const DSPHERE sphere)
{
  return d3_point_in_sphere(point.x, point.y, point.z, sphere.x, sphere.y, sphere.z, sphere.radius);
}

CC_INLINE BOOL d2_point_in_three_point_circle(const T px, const T py, const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  T dx1 = x1 - px;
  T dx2 = x2 - px;
  T dx3 = x3 - px;
  T dy1 = y2 - py;
  T dy2 = y3 - py;
  T dy3 = y1 - py;
  T a11 = dx3 * dy1 - dx2 * dy2;
  T a12 = dx3 * dy3 - dx1 * dy2;
  T a21 = dx2 * (x2 - x3) + dy1 * (y2 - y3);
  T a22 = dx1 * (x1 - x3) + dy3 * (y1 - y3);

  return d_less_than_or_equal(a11 * a22 - a21 * a12, (0.0));
}

CC_INLINE BOOL pt2_point_in_three_point_circle(const DPOINT2 point, const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_point_in_three_point_circle(point.x, point.y,
      point1.x, point1.y,
      point2.x, point2.y,
      point3.x, point3.y);
}

CC_INLINE BOOL tr2_point_in_triangle_circle(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  return pt2_point_in_three_point_circle(point, triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE BOOL d2_point_in_focus_area(const T px, const T py, const T x1, const T y1, const T x2, const T y2, const T x3, const T y3)
{
  return (-1 == (d2_orientation((x2 + x3) * (0.5), (y2 + y3) * (0.5), x1, y1, px, py) *
      d2_orientation((x1 + x3) * (0.5), (y1 + y3) * (0.5), x2, y2, px, py)));
}

CC_INLINE BOOL pt2_point_in_focus_area(const DPOINT2 point, const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_point_in_focus_area(point.x, point.y, point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE BOOL po2_point_in_polygon(const DPOINT2 point, int n, const DPOINT2* polygon)
{
  return d2_point_in_polygon(point.x, point.y, n, polygon);
}

CC_INLINE BOOL d2_point_in_polygon_winding_number(const T px, const T py, const DPOLYGON2* polygon)
{
  const DPOINT2* p = polygon->p;
  int i, j = 0, n = polygon->n;
  int winding_number = 0;
  DPOINT2 pp = d2_make_point(px, py);
  j = polygon->n - 1;

  for (i = 0; i < polygon->n; ++i) {
    if (p[j].y <= py) {
      if ((p[i].y > py) &&
          (pt2_orientation(p[j], p[i], pp) == LeftHandSide)) {
        winding_number++;
      }
    }
    else {
      if ((p[i].y <= py) &&
          (pt2_orientation(p[j], p[i], pp) == RightHandSide)) {
        winding_number--;
      }
    }

    j = i;
  }

  return (winding_number != 0);
}

CC_INLINE BOOL pt2_point_in_polygon_winding_number(const DPOINT2 point, const DPOLYGON2* polygon)
{
  return d2_point_in_polygon_winding_number(point.x, point.y, polygon);
}

