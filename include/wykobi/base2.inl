
CC_INLINE BOOL po2_is_convex_polygon(int n, const DPOINT2* polygon)
{
  if (n < 3) {
    return false;
  }

  {
    int i = 0;
    int j = n - 1;
    int k = n - 2;
    int initial_orientation = 0;

    while ((initial_orientation = pt2_orientation(polygon[k], polygon[j], polygon[i])) == CollinearOrientation) {
      k = j;
      j = i++;

      if (i >= n) {
        return false;
      }
    }

    while (i < n) {
      if (pt2_orientation(polygon[k], polygon[j], polygon[i]) != initial_orientation) {
        return false;
      }

      k = j;
      j = i++;

      if (i >= n) {
        break;
      }
    }
  }

  return true;
}

CC_INLINE int po2_remove_consecutive_collinear_points(int n, const DPOINT2* polygon, DPOINT2* polygon_)
{
  int i, j = 0;
  DPOINT2 previous_point = polygon[n - 1];

  for (i = 0; i < n - 1; ++i) {
    if (pt2_orientation(previous_point, polygon[i], polygon[i + 1]) != CollinearOrientation) {
      polygon_[j++] = (polygon[i]);
      previous_point = polygon[i];
    }
  }

  if (pt2_orientation(previous_point, polygon[n - 1], polygon[0]) != CollinearOrientation) {
    polygon_[j++] = polygon[n - 1];
  }

  return j;
}

CC_INLINE int pt2_remove_consecutive_collinear_points(int n, const DPOINT2* point, DPOINT2* out)
{
  if (n < 3) {
    MEMCPY(out, point, n);
    return 0;
  }

  {
    int prev = 0, it = 1, next = 2, j = 0;

    while (next != n) {
      if (pt2_orientation(point[prev], point[it], point[next]) != CollinearOrientation) {
        out[j++] = point[it];
        prev = it;
      }

      it = next;
      ++next;
    }
  }

  return 0;
}

CC_INLINE BOOL po2_convex_vertex(const int index, int n, const DPOINT2* polygon, int polygon_orientation)
{
  if (0 == index) {
    return (pt2_orientation(polygon[n - 1], polygon[0], polygon[1]) == polygon_orientation);
  }
  else if (index == (n - 1)) {
    return (pt2_orientation(polygon[n - 2], polygon[n - 1], polygon[0]) == polygon_orientation);
  }
  else {
    return (pt2_orientation(polygon[index - 1], polygon[index], polygon[index + 1]) == polygon_orientation);
  }
}

CC_INLINE BOOL po2_collinear_vertex(const int index, int n, const DPOINT2* polygon)
{
  if (0 == index) {
    return pt2_robust_collinear(polygon[n - 1], polygon[0], polygon[1]);
  }
  else if (index == (n - 1)) {
    return pt2_robust_collinear(polygon[n - 2], polygon[n - 1], polygon[0]);
  }
  else {
    return pt2_robust_collinear(polygon[index - 1], polygon[index], polygon[index + 1]);
  }
}

CC_INLINE BOOL po2_vertex_is_ear(const int index, int n, const DPOINT2* polygon)
{
  int i, pred_index, succ_index;
  DTRIANGLE2 triangle;

  if (0 == index) {
    pred_index = n - 1;
    succ_index = 1;
  }
  else if (index == n - 1) {
    pred_index = n - 2;
    succ_index = 0;
  }
  else {
    pred_index = index - 1;
    succ_index = index + 1;
  }

  triangle = pt2_make_triangle(polygon[pred_index], polygon[index], polygon[succ_index]);

  if (pt2_robust_collinear(triangle.p0, triangle.p1, triangle.p2)) {
    return false;
  }

  for (i = 0; i < n; ++i) {
    if ((i != pred_index) && (i != succ_index) && (i != index)) {
      if (tr2_point_in_triangle(polygon[i], triangle)) {
        return false;
      }
    }
  }

  return true;
}

CC_INLINE DTRIANGLE2 po2_vertex_triangle(const int index, int n, const DPOINT2* polygon)
{
  if (0 == index) {
    return pt2_make_triangle(polygon[n - 1], polygon[0], polygon[1]);
  }
  else if (index == (n - 1)) {
    return pt2_make_triangle(polygon[n - 2], polygon[n - 1], polygon[0]);
  }
  else {
    return pt2_make_triangle(polygon[index - 1], polygon[index], polygon[index + 1]);
  }
}

CC_INLINE int po2_polygon_orientation(int n, const DPOINT2* polygon)
{
  T area;
  int index, prev_index;

  if (n < 3) {
    return 0;
  }

  area = (0.0);
  prev_index = n - 1;

  for (index = 0; index < n; ++index) {
    area += (polygon[prev_index].x * polygon[index].y - polygon[index].x * polygon[prev_index].y);
    prev_index = index;
  }

  return ((d_greater_than_or_equal(area, (0.0))) ? CounterClockwise : Clockwise);

  /*
  int anchor = 0;
  for(int i = 1; i < n; ++i)
  {
  if (polygon[i].x > polygon[anchor].x)
  anchor = i;
  else if ((polygon[i].x == polygon[anchor].x) && (polygon[i].y < polygon[anchor].y))
  anchor = i;
  }

  if (0 == anchor)
  {
  return pt2_orientation(*(polygon.end() - 1),*polygon.end(),*polygon.begin());
  }
  else if (anchor == (n - 1))
  {
  return pt2_orientation(*(polygon.end() - 2),*(polygon.end() - 1),*polygon.end());
  }
  else
  {
  return pt2_orientation(polygon[anchor - 2],polygon[anchor - 1],polygon[anchor]);
  }
  */
}

CC_INLINE BOOL tr2_is_equilateral_triangle(const DTRIANGLE2 triangle)
{
  T dist = pt2_distance(triangle.p0, triangle.p1);
  return d_is_equal(pt2_distance(triangle.p1, triangle.p2), dist) &&
      d_is_equal(pt2_distance(triangle.p0, triangle.p2), dist);
}

CC_INLINE BOOL tr3_is_equilateral_triangle(const DTRIANGLE3 triangle)
{
  T dist = pt3_distance(triangle.p0, triangle.p1);
  return d_is_equal(pt3_distance(triangle.p1, triangle.p2), dist) &&
      d_is_equal(pt3_distance(triangle.p0, triangle.p2), dist);
}

CC_INLINE BOOL tr2_is_isosceles_triangle(const DTRIANGLE2 triangle)
{
  T dist1 = pt2_lay_distance(triangle.p0, triangle.p1);
  T dist2 = pt2_lay_distance(triangle.p1, triangle.p2);
  T dist3 = pt2_lay_distance(triangle.p2, triangle.p0);
  return d_is_equal(dist1, dist2) || d_is_equal(dist1, dist3) || d_is_equal(dist2, dist3);
}

CC_INLINE BOOL tr3_is_isosceles_triangle(const DTRIANGLE3 triangle)
{
  T dist1 = pt3_lay_distance(triangle.p0, triangle.p1);
  T dist2 = pt3_lay_distance(triangle.p1, triangle.p2);
  T dist3 = pt3_lay_distance(triangle.p2, triangle.p0);
  return d_is_equal(dist1, dist2) || d_is_equal(dist1, dist3) || d_is_equal(dist2, dist3);
}

CC_INLINE BOOL tr2_is_right_triangle(const DTRIANGLE2 triangle)
{
  T a = se2_lay_distance(tr2_edge(triangle, 0));
  T b = se2_lay_distance(tr2_edge(triangle, 1));
  T c = se2_lay_distance(tr2_edge(triangle, 2));
  return d_is_equal(a + b, c) || d_is_equal(a + c, b) || d_is_equal(b + c, a);
}

CC_INLINE BOOL tr3_is_right_triangle(const DTRIANGLE3 triangle)
{
  T a = se3_lay_distance(tr3_edge(triangle, 0));
  T b = se3_lay_distance(tr3_edge(triangle, 1));
  T c = se3_lay_distance(tr3_edge(triangle, 2));
  return d_is_equal(a + b, c) || d_is_equal(a + c, b) || d_is_equal(b + c, a);
}

CC_INLINE BOOL are_perspective_triangles(const DTRIANGLE2 triangle1, const DTRIANGLE2 triangle2)
{
  int i;

  for (i = 0; i < 3; ++i) {
    if (sese2_robust_parallel(tr2_edge(triangle1, i), tr2_edge(triangle2, i))) {
      return false;
    }
  }

  return pt2_robust_collinear(lili2_intersection_point(se2_make_line(tr2_edge(triangle1, 0)), se2_make_line(tr2_edge(triangle2, 0))),
      lili2_intersection_point(se2_make_line(tr2_edge(triangle1, 1)), se2_make_line(tr2_edge(triangle2, 1))),
      lili2_intersection_point(se2_make_line(tr2_edge(triangle1, 2)), se2_make_line(tr2_edge(triangle2, 2))));
}

CC_INLINE BOOL trtr3_are_perspective_triangles(const DTRIANGLE3 triangle1, const DTRIANGLE3 triangle2)
{
  int i;

  for (i = 0; i < 3; ++i) {
    if (sese3_robust_parallel(tr3_edge(triangle1, i), tr3_edge(triangle2, i))) {
      return false;
    }
  }

  return pt3_robust_collinear(lili3_intersection_point(se3_make_line(tr3_edge(triangle1, 0)), se3_make_line(tr3_edge(triangle2, 0))),
      lili3_intersection_point(se3_make_line(tr3_edge(triangle1, 1)), se3_make_line(tr3_edge(triangle2, 1))),
      lili3_intersection_point(se3_make_line(tr3_edge(triangle1, 2)), se3_make_line(tr3_edge(triangle2, 2))));
}

CC_INLINE DLINE2 trtr2_perspectrix(const DTRIANGLE2 triangle1, const DTRIANGLE2 triangle2)
{
  int i;

  for (i = 0; i < 3; ++i) {
    if (sese2_robust_parallel(tr2_edge(triangle1, i), tr2_edge(triangle2, i))) {
      return li2_degenerate();
    }
  }

  {

    DPOINT2 ipoint0 = lili2_intersection_point(se2_make_line(tr2_edge(triangle1, 0)), se2_make_line(tr2_edge(triangle2, 0)));
    DPOINT2 ipoint1 = lili2_intersection_point(se2_make_line(tr2_edge(triangle1, 1)), se2_make_line(tr2_edge(triangle2, 1)));
    DPOINT2 ipoint2 = lili2_intersection_point(se2_make_line(tr2_edge(triangle1, 2)), se2_make_line(tr2_edge(triangle2, 2)));
    return (pt2_robust_collinear(ipoint0, ipoint1, ipoint2)) ? pt2_make_line(ipoint0, ipoint1) : li2_degenerate();
  }
}

CC_INLINE DLINE3 trtr3_perspectrix(const DTRIANGLE3 triangle1, const DTRIANGLE3 triangle2)
{
  int i;

  for (i = 0; i < 3; ++i) {
    if (sese3_robust_parallel(tr3_edge(triangle1, i), tr3_edge(triangle2, i))) {
      return li3_degenerate();
    }
  }

  {
    DPOINT3 ipoint0 = lili3_intersection_point(se3_make_line(tr3_edge(triangle1, 0)), se3_make_line(tr3_edge(triangle2, 0)));
    DPOINT3 ipoint1 = lili3_intersection_point(se3_make_line(tr3_edge(triangle1, 1)), se3_make_line(tr3_edge(triangle2, 1)));
    DPOINT3 ipoint2 = lili3_intersection_point(se3_make_line(tr3_edge(triangle1, 2)), se3_make_line(tr3_edge(triangle2, 2)));
    return (pt3_robust_collinear(ipoint0, ipoint1, ipoint2)) ? pt3_make_line(ipoint0, ipoint1) : li3_degenerate();
  }
}

CC_INLINE DPOINT2 d2_nonsymmetric_mirror(const T px, const T py, const T x1, const T y1, const T x2, const T y2, const T ratio)
{
  DPOINT2 pt = d2_closest_point_on_line_from_point(x1, y1, x2, y2, px, py);
  T general_ratio = (2.0) * ratio;
  pt.x = px + general_ratio * (pt.x - px);
  pt.y = py + general_ratio * (pt.y - py);
  return pt;
}

CC_INLINE DPOINT2 ptli2_nonsymmetric_mirror(const DPOINT2 point, const T ratio, const DLINE2 line)
{
  return d2_nonsymmetric_mirror(point.x, point.y, line.p0.x, line.p0.y, ratio, line.p1.x, line.p1.y);
}

CC_INLINE DSEGMENT2 seli2_nonsymmetric_mirror(const DSEGMENT2 segment, const T ratio, const DLINE2 line)
{
  DSEGMENT2 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = ptli2_nonsymmetric_mirror((&segment.p0)[i], ratio, line);
  }

  return segment_;
}

CC_INLINE DRECTANGLE rtli2_nonsymmetric_mirror(const DRECTANGLE rectangle, const T ratio, const DLINE2 line)
{
  DRECTANGLE rectangle_;

  int i;

  for (i = 0; i < RectanglePointCount; ++i) {
    (&rectangle_.p0)[i] = ptli2_nonsymmetric_mirror((&rectangle.p0)[i], ratio, line);
  }

  return rectangle_;
}

CC_INLINE DTRIANGLE2 trli2_nonsymmetric_mirror(const DTRIANGLE2 triangle, const T ratio, const DLINE2 line)
{
  DTRIANGLE2 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = ptli2_nonsymmetric_mirror((&triangle.p0)[i], ratio, line);
  }

  return triangle_;
}

CC_INLINE DQUADIX2 quli2_nonsymmetric_mirror(const DQUADIX2 quadix, const T ratio, const DLINE2 line)
{
  DQUADIX2 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = ptli2_nonsymmetric_mirror((&quadix.p0)[i], ratio, line);
  }

  return quadix_;
}

CC_INLINE DCIRCLE cili2_nonsymmetric_mirror(const DCIRCLE circle, const T ratio, const DLINE2 line)
{
  DPOINT2 pt = d2_nonsymmetric_mirror(circle.x, circle.y, line.p0.x, line.p0.y, line.p1.x, line.p1.y, ratio);
  return pt2_make_circle(pt, circle.radius);
}

CC_INLINE int poli2_nonsymmetric_mirror(int n, const DPOINT2* polygon, const T ratio, const DLINE2 line, DPOINT2* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = ptli2_nonsymmetric_mirror(polygon[i], ratio, line);
  }

  return 0;
}

CC_INLINE DPOINT3 ptpl3_nonsymmetric_mirror(const DPOINT3 point, const T ratio, const DPLANE3 plane)
{
  DPOINT3 pt = pt3_closest_point_on_plane_from_point(plane, point);
  return v3_add(point, v3_scale(v3_sub(pt, point), (2.0) * ratio));
}

CC_INLINE DSEGMENT3 sepl3_nonsymmetric_mirror(const DSEGMENT3 segment, const T ratio, const DPLANE3 plane)
{
  DSEGMENT3 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = ptpl3_nonsymmetric_mirror((&segment.p0)[i], ratio, plane);
  }

  return segment_;
}

CC_INLINE DBOX3 boxpl3_nonsymmetric_mirror(const DBOX3 box, const T ratio, const DPLANE3 plane)
{
  DBOX3 box_;

  int i;

  for (i = 0; i < BoxPointCount; ++i) {
    (&box_.p0)[i] = ptpl3_nonsymmetric_mirror((&box.p0)[i], ratio, plane);
  }

  return box_;
}

CC_INLINE DTRIANGLE3 trpl3_nonsymmetric_mirror(const DTRIANGLE3 triangle, const T ratio, const DPLANE3 plane)
{
  DTRIANGLE3 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = ptpl3_nonsymmetric_mirror((&triangle.p0)[i], ratio, plane);
  }

  return triangle_;
}

CC_INLINE DQUADIX3 qupl3_nonsymmetric_mirror(const DQUADIX3 quadix, const T ratio, const DPLANE3 plane)
{
  DQUADIX3 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = ptpl3_nonsymmetric_mirror((&quadix.p0)[i], ratio, plane);
  }

  return quadix_;
}

CC_INLINE DSPHERE sppl3_nonsymmetric_mirror(const DSPHERE sphere, const T ratio, const DPLANE3 plane)
{
  DPOINT3 center = sp_make_point(sphere);
  return pt3_make_sphere(ptpl3_nonsymmetric_mirror(center, ratio, plane), sphere.radius);
}

CC_INLINE int popl3_nonsymmetric_mirror(int n, const DPOINT3* polygon, const T ratio, const DPLANE3 plane, DPOINT3* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = ptpl3_nonsymmetric_mirror(polygon[i], ratio, plane);
  }

  return n;
}

CC_INLINE DPOINT2 ci2_invert_point(const DPOINT2 point, const DCIRCLE circle)
{
  return pt2_project_point_t(ci_make_point(circle), point, sqr(circle.radius) / (sqr(point.x - circle.x) + sqr(point.y - circle.y)));
}

CC_INLINE DPOINT3 sp3_invert_point(const DPOINT3 point, const DSPHERE sphere)
{
  return pt3_project_point_t(sp_make_point(sphere), point, sqr(sphere.radius) / (sqr(point.x - sphere.x) + sqr(point.y - sphere.y) + sqr(point.z - sphere.z)));
}

CC_INLINE DPOINT2 ci2_antipodal_point(const DPOINT2 point, const DCIRCLE circle)
{
  return pt2_project_point_t(point, ci_make_point(circle), (2.0));
}

CC_INLINE DPOINT3 sp3_antipodal_point(const DPOINT3 point, const DSPHERE sphere)
{
  return pt3_project_point_t(point, sp_make_point(sphere), (2.0));
}

CC_INLINE T d2_manhattan_distance(const T x1, const T y1, const T x2, const T y2)
{
  return T_ABS(x2 - x1) + T_ABS(y2 - y1);
}

CC_INLINE T d3_manhattan_distance(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  return T_ABS(x2 - x1) + T_ABS(y2 - y1) + T_ABS(z2 - z1);
}

CC_INLINE T pt2_manhattan_distance(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_manhattan_distance(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE T pt3_manhattan_distance(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_manhattan_distance(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
}

CC_INLINE T ptray2_manhattan_distance(const DPOINT2 point, const DRAY2 ray)
{
  return pt2_manhattan_distance(point, pt2_closest_point_on_ray_from_point(ray, point));
}

CC_INLINE T ptray3_manhattan_distance(const DPOINT3 point, const DRAY3 ray)
{
  return pt3_manhattan_distance(point, pt3_closest_point_on_ray_from_point(ray, point));
}

CC_INLINE T se2_manhattan_distance(const DSEGMENT2 segment)
{
  return pt2_manhattan_distance(segment.p0, segment.p1);
}

CC_INLINE T se3_manhattan_distance(const DSEGMENT3 segment)
{
  return pt3_manhattan_distance(segment.p0, segment.p1);
}

CC_INLINE T cici2_manhattan_distance(const DCIRCLE circle1, const DCIRCLE circle2)
{
  return d2_manhattan_distance(circle1.x, circle1.y, circle2.x, circle2.y);
}

CC_INLINE T d2_chebyshev_distance(const T x1, const T y1, const T x2, const T y2)
{
  return max(T_ABS(x2 - x1), T_ABS(y2 - y1));
}

CC_INLINE T d3_chebyshev_distance(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  return max(max(T_ABS(x2 - x1), T_ABS(y2 - y1)), T_ABS(z2 - z1));
}

CC_INLINE T pt2_chebyshev_distance(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_chebyshev_distance(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE T pt3_chebyshev_distance(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_chebyshev_distance(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
}

CC_INLINE T se2_chebyshev_distance(const DSEGMENT2 segment)
{
  return pt2_chebyshev_distance(segment.p0, segment.p1);
}

CC_INLINE T se3_chebyshev_distance(const DSEGMENT3 segment)
{
  return pt3_chebyshev_distance(segment.p0, segment.p1);
}

CC_INLINE T cici2_chebyshev_distance(const DCIRCLE circle1, const DCIRCLE circle2)
{
  return d2_chebyshev_distance(circle1.x, circle1.y, circle2.x, circle2.y);
}

CC_INLINE T d2_inverse_chebyshev_distance(const T x1, const T y1, const T x2, const T y2)
{
  return min(T_ABS(x2 - x1), T_ABS(y2 - y1));
}

CC_INLINE T d3_inverse_chebyshev_distance(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  return min(min(T_ABS(x2 - x1), T_ABS(y2 - y1)), T_ABS(z2 - z1));
}

CC_INLINE T pt2_inverse_chebyshev_distance(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_inverse_chebyshev_distance(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE T pt3_inverse_chebyshev_distance(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_inverse_chebyshev_distance(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
}

CC_INLINE T se2_inverse_chebyshev_distance(const DSEGMENT2 segment)
{
  return pt2_inverse_chebyshev_distance(segment.p0, segment.p1);
}

CC_INLINE T se3_inverse_chebyshev_distance(const DSEGMENT3 segment)
{
  return pt3_inverse_chebyshev_distance(segment.p0, segment.p1);
}

CC_INLINE T cici2_inverse_chebyshev_distance(const DCIRCLE circle1, const DCIRCLE circle2)
{
  return d2_inverse_chebyshev_distance(circle1.x, circle1.y, circle2.x, circle2.y);
}

CC_INLINE DPOINT2 pt2_minkowski_sum(const DPOINT2 point1, const DPOINT2 point2)
{
  return v2_add(point1, point2);
}

CC_INLINE int rtrt2_minkowski_sum(const DRECTANGLE rectangle1, const DRECTANGLE rectangle2, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < 4; ++i) {
    DPOINT2 point = rt2_rectangle_corner(rectangle1, i);

    for (j = 0; j < 4; ++j) {
      polygon[k++] = pt2_minkowski_sum(point, rt2_rectangle_corner(rectangle2, j));
    }
  }

  return k;
}

CC_INLINE int trtr2_minkowski_sum(const DTRIANGLE2 triangle1, const DTRIANGLE2 triangle2, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < TrianglePointCount; ++i) {
    for (j = 0; j < TrianglePointCount; ++j) {
      polygon[k++] = pt2_minkowski_sum((&triangle1.p0)[i], (&triangle2.p0)[j]);
    }
  }

  return k;
}

CC_INLINE int ququ2_minkowski_sum(const DQUADIX2 quadix1, const DQUADIX2 quadix2, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < QuadixPointCount; ++i) {
    for (j = 0; j < QuadixPointCount; ++j) {
      polygon[k++] = pt2_minkowski_sum((&quadix1.p0)[i], (&quadix2.p0)[j]);
    }
  }

  return k;
}

CC_INLINE int trrt2_minkowski_sum(const DTRIANGLE2 triangle, const DRECTANGLE rectangle, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < TrianglePointCount; ++i) {
    for (j = 0; j < 4; ++j) {
      polygon[k++] = pt2_minkowski_sum((&triangle.p0)[i], rt2_rectangle_corner(rectangle, j));
    }
  }

  return k;
}

CC_INLINE int trqu2_minkowski_sum(const DTRIANGLE2 triangle, const DQUADIX2 quadix, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < TrianglePointCount; ++i) {
    for (j = 0; j < QuadixPointCount; ++j) {
      polygon[k++] = pt2_minkowski_sum((&triangle.p0)[i], (&quadix.p0)[j]);
    }
  }

  return k;
}

CC_INLINE int trci2_minkowski_sum(const DTRIANGLE2 triangle, const DCIRCLE circle, DPOINT2* polygon)
{
  int i, j, k = 0;
  enum {circle_polygon_n = 360};
  DPOINT2 circle_polygon[circle_polygon_n] = {0};
  ci2_make_polygon(circle, circle_polygon_n, circle_polygon);

  for (i = 0; i < TrianglePointCount; ++i) {
    for (j = 0; j < circle_polygon_n; ++j) {
      polygon[k++] = pt2_minkowski_sum((&triangle.p0)[i], circle_polygon[j]);
    }
  }

  return k;
}

CC_INLINE int quci2_minkowski_sum(const DQUADIX2 quadix, const DCIRCLE circle, DPOINT2* polygon)
{
  int i, j, k = 0;
  enum {circle_polygon_n = 360};
  DPOINT2 circle_polygon[circle_polygon_n] = {0};
  ci2_make_polygon(circle, circle_polygon_n, circle_polygon);

  for (i = 0; i < QuadixPointCount; ++i) {
    for (j = 0; j < circle_polygon_n; ++j) {
      polygon[k++] = pt2_minkowski_sum((&quadix.p0)[i], circle_polygon[j]);
    }
  }

  return k;
}

CC_INLINE int qurt2_minkowski_sum(const DQUADIX2 quadix, const DRECTANGLE rectangle, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < QuadixPointCount; ++i) {
    for (j = 0; j < 4; ++j) {
      polygon[k++] = pt2_minkowski_sum((&quadix.p0)[i], rt2_rectangle_corner(rectangle, j));
    }
  }

  return k;
}

CC_INLINE int rtci2_minkowski_sum(const DRECTANGLE rectangle, const DCIRCLE circle, DPOINT2* polygon)
{
  int i, j, k = 0;
  enum {circle_polygon_n = 360};
  DPOINT2 circle_polygon[circle_polygon_n] = {0};
  ci2_make_polygon(circle, circle_polygon_n, circle_polygon);

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < circle_polygon_n; ++j) {
      polygon[k++] = pt2_minkowski_sum(rt2_rectangle_corner(rectangle, i), circle_polygon[j]);
    }
  }

  return k;
}

CC_INLINE int popo2_minkowski_sum(int n1, const DPOINT2* polygon1, int n2, const DPOINT2* polygon2, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < n1; ++i) {
    for (j = 0; j < n2; ++j) {
      polygon[k++] = pt2_minkowski_sum(polygon1[i], polygon2[j]);
    }
  }

  return k;
}

CC_INLINE int cici2_minkowski_sum(const DCIRCLE circle1, const DCIRCLE circle2, DPOINT2* polygon)
{
  enum {circle_polygon_n = 360};
  DPOINT2 circle_polygon1[circle_polygon_n] = {0};
  DPOINT2 circle_polygon2[circle_polygon_n] = {0};
  ci2_make_polygon(circle1, circle_polygon_n, circle_polygon1);
  ci2_make_polygon(circle2, circle_polygon_n, circle_polygon2);
  popo2_minkowski_sum(circle_polygon_n, circle_polygon1, circle_polygon_n, circle_polygon2, polygon);
  return 0;
}

CC_INLINE DPOINT2 pt2_minkowski_difference(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_make_point(point1.x - point2.x, point1.y - point2.y);
}

CC_INLINE int rtrt2_minkowski_difference(const DRECTANGLE rectangle1, const DRECTANGLE rectangle2, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < 4; ++i) {
    DPOINT2 point = rt2_rectangle_corner(rectangle1, i);

    for (j = 0; j < 4; ++j) {
      polygon[k++] = pt2_minkowski_difference(point, rt2_rectangle_corner(rectangle2, j));
    }
  }

  return k;
}

CC_INLINE int trtr2_minkowski_difference(const DTRIANGLE2 triangle1, const DTRIANGLE2 triangle2, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < TrianglePointCount; ++i) {
    for (j = 0; j < TrianglePointCount; ++j) {
      polygon[k++] = pt2_minkowski_difference((&triangle1.p0)[i], (&triangle2.p0)[j]);
    }
  }

  return k;
}

CC_INLINE int ququ2_minkowski_difference(const DQUADIX2 quadix1, const DQUADIX2 quadix2, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < QuadixPointCount; ++i) {
    for (j = 0; j < QuadixPointCount; ++j) {
      polygon[k++] = pt2_minkowski_difference((&quadix1.p0)[i], (&quadix2.p0)[j]);
    }
  }

  return k;
}

CC_INLINE int trrt2_minkowski_difference(const DTRIANGLE2 triangle, const DRECTANGLE rectangle, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < TrianglePointCount; ++i) {
    for (j = 0; j < 4; ++j) {
      polygon[k++] = pt2_minkowski_difference((&triangle.p0)[i], rt2_rectangle_corner(rectangle, j));
    }
  }

  return k;
}

CC_INLINE int trqu2_minkowski_difference(const DTRIANGLE2 triangle, const DQUADIX2 quadix, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < TrianglePointCount; ++i) {
    for (j = 0; j < QuadixPointCount; ++j) {
      polygon[k++] = pt2_minkowski_difference((&triangle.p0)[i], (&quadix.p0)[j]);
    }
  }

  return k;
}

CC_INLINE int trci2_minkowski_difference(const DTRIANGLE2 triangle, const DCIRCLE circle, DPOINT2* polygon)
{
  int i, j, k = 0;
  enum {circle_polygon_n = 360};
  DPOINT2 circle_polygon[circle_polygon_n] = {0};
  ci2_make_polygon(circle, circle_polygon_n, circle_polygon);

  for (i = 0; i < TrianglePointCount; ++i) {
    for (j = 0; j < circle_polygon_n; ++j) {
      polygon[k++] = pt2_minkowski_difference((&triangle.p0)[i], circle_polygon[j]);
    }
  }

  return k;
}

CC_INLINE int quci2_minkowski_difference(const DQUADIX2 quadix, const DCIRCLE circle, DPOINT2* polygon)
{
  int i, j, k = 0;
  enum {circle_polygon_n = 360};
  DPOINT2 circle_polygon[circle_polygon_n] = {0};
  ci2_make_polygon(circle, circle_polygon_n, circle_polygon);

  for (i = 0; i < QuadixPointCount; ++i) {
    for (j = 0; j < circle_polygon_n; ++j) {
      polygon[k++] = pt2_minkowski_difference((&quadix.p0)[i], circle_polygon[j]);
    }
  }

  return 0;
}

CC_INLINE int qurt2_minkowski_difference(const DQUADIX2 quadix, const DRECTANGLE rectangle, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < QuadixPointCount; ++i) {
    for (j = 0; j < 4; ++j) {
      polygon[k++] = pt2_minkowski_difference((&quadix.p0)[i], rt2_rectangle_corner(rectangle, j));
    }
  }

  return k;
}

CC_INLINE int rtci2_minkowski_difference(const DRECTANGLE rectangle, const DCIRCLE circle, DPOINT2* polygon)
{
  int i, j, k = 0;
  enum {circle_polygon_n = 360};
  DPOINT2 circle_polygon[circle_polygon_n] = {0};
  ci2_make_polygon(circle, circle_polygon_n, circle_polygon);

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < circle_polygon_n; ++j) {
      polygon[k++] = pt2_minkowski_difference(rt2_rectangle_corner(rectangle, i), circle_polygon[j]);
    }
  }

  return 0;
}

CC_INLINE int popo2_minkowski_difference(int n1, const DPOINT2* polygon1, int n2, const DPOINT2* polygon2, DPOINT2* polygon)
{
  int i, j, k = 0;

  for (i = 0; i < n1; ++i) {
    for (j = 0; j < n2; ++j) {
      polygon[k++] = pt2_minkowski_difference(polygon1[i], polygon2[j]);
    }
  }

  return k;
}

CC_INLINE int cici2_minkowski_difference(const DCIRCLE circle1, const DCIRCLE circle2, DPOINT2* polygon)
{
  enum {circle_polygon_n = 360};
  DPOINT2 circle_polygon1[circle_polygon_n] = {0};
  DPOINT2 circle_polygon2[circle_polygon_n] = {0};
  ci2_make_polygon(circle1, circle_polygon_n, circle_polygon1);
  ci2_make_polygon(circle2, circle_polygon_n, circle_polygon2);
  return popo2_minkowski_difference(circle_polygon_n, circle_polygon1, circle_polygon_n, circle_polygon2, polygon);
}

CC_INLINE T pt2_lay_distance_from_point_to_circle_center(const DPOINT2 point, const DCIRCLE circle)
{
  return d2_lay_distance(point.x, point.y, circle.x, circle.y);
}

CC_INLINE T pt3_lay_distance_from_point_to_sphere_center(const DPOINT3 point, const DSPHERE sphere)
{
  return d3_lay_distance(point.x, point.y, point.z, sphere.x, sphere.y, sphere.z);
}

CC_INLINE T pt2_distance_from_point_to_circle_center(const DPOINT2 point, const DCIRCLE circle)
{
  return sqrt(pt2_lay_distance_from_point_to_circle_center(point, circle));
}

CC_INLINE T pt3_distance_from_point_to_sphere_center(const DPOINT3 point, const DSPHERE sphere)
{
  return sqrt(pt3_lay_distance_from_point_to_sphere_center(point, sphere));
}

CC_INLINE T rt2_span_length(const DRECTANGLE rect)
{
  return pt2_distance(rect.p0, rect.p1);
}

CC_INLINE T box3_span_length(const DBOX3 box)
{
  return pt3_distance(box.p0, box.p1);
}

CC_INLINE DPOINT2 pt2_project_object(const DPOINT2 point, const T angle, const T distance)
{
  return pt2_project_point_angle(point, angle, distance);
}

CC_INLINE DSEGMENT2 se2_project_object(const DSEGMENT2 segment, const T angle, const T distance)
{
  DSEGMENT2 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt2_project_point_angle((&segment.p0)[i], angle, distance);
  }

  return segment_;
}

CC_INLINE DTRIANGLE2 tr2_project_object(const DTRIANGLE2 triangle, const T angle, const T distance)
{
  DTRIANGLE2 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt2_project_point_angle((&triangle.p0)[i], angle, distance);
  }

  return triangle_;
}

CC_INLINE DQUADIX2 qu2_project_object(const DQUADIX2 quadix, const T angle, const T distance)
{
  DQUADIX2 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt2_project_point_angle((&quadix.p0)[i], angle, distance);
  }

  return quadix_;
}

CC_INLINE DCIRCLE ci2_project_object(const DCIRCLE circle, const T angle, const T distance)
{
  return pt2_make_circle(d2_project_point_angle(circle.x, circle.y, angle, distance), circle.radius);
}

CC_INLINE int po2_project_object(int n, const DPOINT2* polygon, const T angle, const T distance, DPOINT2* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt2_project_point_angle(polygon[i], angle, distance);
  }

  return n;
}

CC_INLINE DSEGMENT2 pt2_project_onto_axis(const DPOINT2 point, const DLINE2 axis)
{
  DPOINT2 point_ = pt2_closest_point_on_line_from_point(axis, point);
  return pt2_make_segment(point_, point_);
}

CC_INLINE DSEGMENT2 tr2_project_onto_axis(const DTRIANGLE2 triangle, const DLINE2 axis)
{
  DPOINT2 point_list[3];
  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    point_list[i] = pt2_closest_point_on_line_from_point(axis, (&triangle.p0)[i]);
  }

  QSORT(point_list, point_list + 3, pt2_less, DPOINT2);
  return pt2_make_segment(point_list[0], point_list[2]);
}

CC_INLINE DSEGMENT2 rt2_project_onto_axis(const DRECTANGLE rectangle, const DLINE2 axis)
{
  DPOINT2 point_list[4];
  int k = 0;
  point_list[k++] = pt2_closest_point_on_line_from_point(axis, d2_make_point(rectangle.p0.x, rectangle.p0.y));
  point_list[k++] = pt2_closest_point_on_line_from_point(axis, d2_make_point(rectangle.p1.x, rectangle.p0.y));
  point_list[k++] = pt2_closest_point_on_line_from_point(axis, d2_make_point(rectangle.p1.x, rectangle.p1.y));
  point_list[k++] = pt2_closest_point_on_line_from_point(axis, d2_make_point(rectangle.p0.x, rectangle.p1.y));
  QSORT(point_list, point_list + 3, pt2_less, DPOINT2);
  return pt2_make_segment(point_list[0], point_list[3]);
}

CC_INLINE DSEGMENT2 qu2_project_onto_axis(const DQUADIX2 quadix, const DLINE2 axis)
{
  DPOINT2 point_list[4];

  int i, k = 0;

  for (i = 0; i < QuadixPointCount; ++i) {
    point_list[k++] = pt2_closest_point_on_line_from_point(axis, (&quadix.p0)[i]);
  }

  QSORT(point_list, point_list + 3, pt2_less, DPOINT2);
  return pt2_make_segment(point_list[0], point_list[3]);
}

CC_INLINE DSEGMENT2 ci2_project_onto_axis(const DCIRCLE circle, const DLINE2 axis)
{
  DVECTOR2 v = v2_normalize(v2_sub(axis.p0, axis.p1));
  DPOINT2 point_list[3];
  int k = 0;
  point_list[k++] = pt2_closest_point_on_line_from_point(axis, d2_make_point(circle.x, circle.y));
  point_list[k++] = pt2_closest_point_on_line_from_point(axis, d2_make_point(point_list[0].x + (v.x * circle.radius), point_list[0].y + (v.y * circle.radius)));
  point_list[k++] = pt2_closest_point_on_line_from_point(axis, d2_make_point(point_list[0].x - (v.x * circle.radius), point_list[0].y - (v.y * circle.radius)));
  QSORT(point_list, point_list + 3, pt2_less, DPOINT2);
  return pt2_make_segment(point_list[0], point_list[k - 1]);
}

CC_INLINE DSEGMENT2 po2_project_onto_axis(int n, const DPOINT2* polygon, const DLINE2 axis)
{
  DSEGMENT2 segment;
  int i, k = 0;
  DPOINT2* point_list = MALLOC(DPOINT2, n);

  for (i = 0; i < n; ++i) {
    point_list[k++] = pt2_closest_point_on_line_from_point(axis, polygon[i]);
  }

  QSORT(point_list, point_list + 3, pt2_less, DPOINT2);
  segment = pt2_make_segment(point_list[0], point_list[k - 1]);
  FREE(point_list);
  return segment;
}

CC_INLINE DSEGMENT3 pt3_project_onto_axis(const DPOINT3 point, const DLINE3 axis)
{
  DPOINT3 point_ = pt3_closest_point_on_line_from_point(axis, point);
  return pt3_make_segment(point_, point_);
}

CC_INLINE DSEGMENT3 tr3_project_onto_axis(const DTRIANGLE3 triangle, const DLINE3 axis)
{
  DPOINT3 point_list[3];

  int i, k = 0;

  for (i = 0; i < TrianglePointCount; ++i) {
    point_list[k++] = pt3_closest_point_on_line_from_point(axis, (&triangle.p0)[i]);
  }

  QSORT(point_list, point_list + 3, pt3_less, DPOINT3);
  return pt3_make_segment(point_list[0], point_list[k - 1]);
}

CC_INLINE DSEGMENT3 box3_project_onto_axis(const DBOX3 box, const DLINE3 axis)
{
  DPOINT3 point_list[8];
  int k = 0;
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(box.p0.x, box.p0.y, box.p0.z));
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(box.p1.x, box.p0.y, box.p0.z));
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(box.p1.x, box.p1.y, box.p0.z));
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(box.p0.x, box.p1.y, box.p0.z));
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(box.p0.x, box.p0.y, box.p1.z));
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(box.p1.x, box.p0.y, box.p1.z));
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(box.p1.x, box.p1.y, box.p1.z));
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(box.p0.x, box.p1.y, box.p1.z));
  QSORT(point_list, point_list + 3, pt3_less, DPOINT3);
  return pt3_make_segment(point_list[0], point_list[k - 1]);
}

CC_INLINE DSEGMENT3 qu3_project_onto_axis(const DQUADIX3 quadix, const DLINE3 axis)
{
  DPOINT3 point_list[4];

  int i, k = 0;

  for (i = 0; i < QuadixPointCount; ++i) {
    point_list[k++] = pt3_closest_point_on_line_from_point(axis, (&quadix.p0)[i]);
  }

  QSORT(point_list, point_list + 3, pt3_less, DPOINT3);
  return pt3_make_segment(point_list[0], point_list[k - 1]);
}

CC_INLINE DSEGMENT3 sp3_project_onto_axis(const DSPHERE sphere, const DLINE3 axis)
{
  DVECTOR3 v = v3_normalize(v3_sub(axis.p0, axis.p1));
  DPOINT3 point_list[3];
  int k = 0;
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(sphere.x, sphere.y, sphere.z));
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(point_list[0].x + (v.x * sphere.radius), point_list[0].y + (v.y * sphere.radius), point_list[0].z + (v.z * sphere.radius)));
  point_list[k++] = pt3_closest_point_on_line_from_point(axis, d3_make_point(point_list[0].x - (v.x * sphere.radius), point_list[0].y - (v.y * sphere.radius), point_list[0].z - (v.z * sphere.radius)));
  QSORT(point_list, point_list + 3, pt3_less, DPOINT3);
  return pt3_make_segment(point_list[0], point_list[k - 1]);
}

CC_INLINE DSEGMENT3 po3_project_onto_axis(int n, const DPOINT3* polygon, const DLINE3 axis)
{
  DSEGMENT3 segment;
  DPOINT3* point_list = MALLOC(DPOINT3, n);

  int i, k = 0;

  for (i = 0; i < n; ++i) {
    point_list[k++] = pt3_closest_point_on_line_from_point(axis, polygon[i]);
  }

  QSORT(point_list, point_list + 3, pt3_less, DPOINT3);
  segment = pt3_make_segment(point_list[0], point_list[k - 1]);
  FREE(point_list);
  return segment;
}

CC_INLINE T d2_qbezier_curve_length(const DQUADRATIC_BEZIER2 bezier, const int point_count)
{
  int i;
  T total_distance = (0.0);
  DPOINT2* curve = MALLOC(DPOINT2, point_count);
  pt2_generate_qbezier(bezier, (curve), point_count);

  for (i = 0; i < (point_count - 1); ++i) {
    total_distance += pt2_distance(curve[i], curve[i + 1]);
  }

  FREE(curve);
  return total_distance;
}

CC_INLINE T d3_qbezier_curve_length(const DQUADRATIC_BEZIER3 bezier, const int point_count)
{
  int i;
  T total_distance = (0.0);
  DPOINT3* curve = MALLOC(DPOINT3, point_count);
  pt3_generate_qbezier(bezier, (curve), point_count);

  for (i = 0; i < (point_count - 1); ++i) {
    total_distance += pt3_distance(curve[i], curve[i + 1]);
  }

  FREE(curve);
  return total_distance;
}

CC_INLINE T d2_cbezier_curve_length(const DCUBIC_BEZIER2 bezier, const int point_count)
{
  int i;
  T total_distance = (0.0);
  DPOINT2* curve = MALLOC(DPOINT2, point_count);
  pt2_generate_cbezier(bezier, (curve), point_count);

  for (i = 0; i < (point_count - 1); ++i) {
    total_distance += pt2_distance(curve[i], curve[i + 1]);
  }

  FREE(curve);
  return total_distance;
}

CC_INLINE T d3_cbezier_curve_length(const DCUBIC_BEZIER3 bezier, const int point_count)
{
  int i;
  T total_distance = (0.0);
  DPOINT3* curve = MALLOC(DPOINT3, point_count);
  pt3_generate_cbezier(bezier, (curve), point_count);

  for (i = 0; i < (point_count - 1); ++i) {
    total_distance += pt3_distance(curve[i], curve[i + 1]);
  }

  FREE(curve);
  return total_distance;
}

CC_INLINE DTRIANGLE2 d2_qbezier_convex_hull(const DQUADRATIC_BEZIER2 bezier)
{
  return pt2_make_triangle(bezier.p0, bezier.p1, bezier.p2);
}

CC_INLINE DQUADIX2 d2_cbezier_convex_hull(const DCUBIC_BEZIER2 bezier)
{
  if (pt2_orientation(bezier.p0, bezier.p2, bezier.p1) != pt2_orientation(bezier.p0, bezier.p2, bezier.p3)) {
    return pt2_make_quadix(bezier.p0, bezier.p1, bezier.p2, bezier.p3);
  }
  else if (pt2_orientation(bezier.p0, bezier.p3, bezier.p1) != pt2_orientation(bezier.p0, bezier.p3, bezier.p2)) {
    return pt2_make_quadix(bezier.p0, bezier.p1, bezier.p3, bezier.p2);
  }
  else {
    return pt2_make_quadix(bezier.p0, bezier.p2, bezier.p1, bezier.p3);
  }
}

CC_INLINE DSEGMENT2 d2_shorten_segment(T x1, T y1, T x2, T y2, const T amount)
{
  T dx, dy, dist_ratio, segment_length = d2_distance(x1, y1, x2, y2);

  if (segment_length < amount) {
    DPOINT2 pt = d2_segment_mid_point(x1, y1, x2, y2);
    x2 = x1 = pt.x;
    y2 = y1 = pt.y;
  }
  else {
    dist_ratio = amount / (2 * segment_length);
    dx = x2 - x1;
    dy = y2 - y1;

    x1 = x1 + dist_ratio * dx;
    y1 = y1 + dist_ratio * dy;
    x2 = x2 - dist_ratio * dx;
    y2 = y2 - dist_ratio * dy;
  }

  return d2_make_segment(x1, y1, x2, y2);
}

CC_INLINE DSEGMENT3 d3_shorten_segment(T x1, T y1, T z1, T x2, T y2, T z2, const T amount)
{
  T segment_length = d3_distance(x1, y1, z1, x2, y2, z2);

  if (segment_length < amount) {
    DPOINT3 pt = d3_segment_mid_point(x1, y1, z1, x2, y2, z2);
    x2 = x1 = pt.x;
    y2 = y1 = pt.y;
    z2 = y1 = pt.z;
  }
  else {
    T dist_ratio = amount / (2 * segment_length);
    T dx = x2 - x1;
    T dy = y2 - y1;
    x1 = x1 + dist_ratio * dx;
    y1 = y1 + dist_ratio * dy;
    x2 = x2 - dist_ratio * dx;
    y2 = y2 - dist_ratio * dy;
  }

  return d3_make_segment(x1, y1, z1, x2, y2, z2);
}

CC_INLINE DSEGMENT2 se2_shorten_segment(const DSEGMENT2 segment, const T amount)
{
  return d2_shorten_segment(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, amount);
}

CC_INLINE DSEGMENT3 shorten_segment(const DSEGMENT3 segment, const T amount)
{
  return d3_shorten_segment(segment.p0.x, segment.p0.y, segment.p0.z, segment.p1.x, segment.p1.y, segment.p1.z, amount);
}

CC_INLINE DSEGMENT2 d2_lengthen_segment(T x1, T y1, T x2, T y2, const T amount)
{
  DPOINT2 c = d2_segment_mid_point(x1, y1, x2, y2);
  T segment_length = d2_distance(x1, y1, x2, y2);
  T ratio = (amount + segment_length) / segment_length;
  x1 = c.x + ratio * (x1 - c.x);
  y1 = c.y + ratio * (y1 - c.y);
  x2 = c.x + ratio * (x2 - c.x);
  y2 = c.y + ratio * (y2 - c.y);
  return d2_make_segment(x1, y1, x2, y2);
}

CC_INLINE DSEGMENT3 d3_lengthen_segment(T x1, T y1, T z1, T x2, T y2, T z2, const T amount)
{
  DPOINT3 c = d3_segment_mid_point(x1, y1, z1, x2, y2, z2);
  T segment_length = d3_distance(x1, y1, z1, x2, y2, z2);
  T ratio = (amount + segment_length) / segment_length;
  x1 = c.x + ratio * (x1 - c.x);
  y1 = c.y + ratio * (y1 - c.y);
  z1 = c.y + ratio * (z1 - c.y);
  x2 = c.x + ratio * (x2 - c.x);
  y2 = c.y + ratio * (y2 - c.y);
  z2 = c.y + ratio * (z2 - c.y);
  return d3_make_segment(x1, y1, z1, x2, y2, z2);
}

CC_INLINE DSEGMENT2 se2_lengthen_segment(const DSEGMENT2 segment, const T amount)
{
  return d2_lengthen_segment(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, amount);
}

CC_INLINE DSEGMENT3 se3_lengthen_segment(const DSEGMENT3 segment, const T amount)
{
  return d3_lengthen_segment(segment.p0.x, segment.p0.y, segment.p0.z, segment.p1.x, segment.p1.y, segment.p1.z, amount);
}

CC_INLINE int pt2_out_code(const DPOINT2 point, const DRECTANGLE rectangle)
{
  int result = 0;

  if (point.y < rectangle.p0.y) {
    result |= CLIP_TOP;
  }
  else if (point.y > rectangle.p1.y) {
    result |= CLIP_BOTTOM;
  }

  if (point.x < rectangle.p0.x) {
    result |= CLIP_LEFT;
  }
  else if (point.x > rectangle.p1.x) {
    result |= CLIP_RIGHT;
  }

  return result;
}

CC_INLINE BOOL d2_clip(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4, DPOINT2* c)
{
  if (d2_rectangle_to_rectangle_intersect(x1, y1, x2, y2, x3, y3, x4, y4)) {
    if (x1 < x3) {
      c[0].x = x3;
    }
    else {
      c[0].x = x1;
    }

    if (x2 > x4) {
      c[1].x = x4;
    }
    else {
      c[1].x = x2;
    }

    if (y1 < y3) {
      c[0].y = y3;
    }
    else {
      c[0].y = y1;
    }

    if (y2 > y4) {
      c[1].y = y4;
    }
    else {
      c[1].y = y2;
    }

    return true;
  }
  else {
    return false;
  }
}

CC_INLINE BOOL d3_clip(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T x3, const T y3, const T z3, const T x4, const T y4, const T z4, DPOINT3* c)
{
  if (d3_box_to_box_intersect(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4)) {
    if (x1 < x3) {
      c[0].x = x3;
    }
    else {
      c[0].x = x1;
    }

    if (x2 > x4) {
      c[1].x = x4;
    }
    else {
      c[1].x = x2;
    }

    if (y1 < y3) {
      c[0].y = y3;
    }
    else {
      c[0].y = y1;
    }

    if (y2 > y4) {
      c[1].y = y4;
    }
    else {
      c[1].y = y2;
    }

    if (z1 < z3) {
      c[0].z = z3;
    }
    else {
      c[0].z = z1;
    }

    if (z2 > z4) {
      c[1].z = z4;
    }
    else {
      c[1].z = z2;
    }

    return true;
  }
  else {
    return false;
  }
}

CC_INLINE BOOL sert2_clip(const DSEGMENT2 src_segment, const DRECTANGLE rectangle, DSEGMENT2* csegment)
{
  BOOL result = false;
  T x = (0.0), y = (0.0);
  T dx, dy;
  int outcode0, outcode1;
  int outcodeout = 0;

  *csegment = src_segment;

  outcode0 = pt2_out_code(csegment->p0, rectangle);
  outcode1 = pt2_out_code(csegment->p1, rectangle);

  while ((outcode0 != 0) || (outcode1 != 0)) {
    if ((outcode0 & outcode1) != 0) {
      return result;
    }
    else {
      if (outcode0 != 0) {
        outcodeout = outcode0;
      }
      else {
        outcodeout = outcode1;
      }

      dx = (csegment->p1.x - csegment->p0.x);
      dy = (csegment->p1.y - csegment->p0.y);

      if ((outcodeout & CLIP_TOP) == CLIP_TOP) {
        x = csegment->p0.x + dx * (rectangle.p0.y - csegment->p0.y) / dy;
        y = rectangle.p0.y;
      }
      else if ((outcodeout & CLIP_BOTTOM) == CLIP_BOTTOM) {
        x = csegment->p0.x + dx * (rectangle.p1.y - csegment->p0.y) / dy;
        y = rectangle.p1.y;
      }
      else if ((outcodeout & CLIP_RIGHT) == CLIP_RIGHT) {
        y = csegment->p0.y + dy * (rectangle.p1.x - csegment->p0.x) / dx;
        x = rectangle.p1.x;
      }
      else if ((outcodeout & CLIP_LEFT) == CLIP_LEFT) {
        y = csegment->p0.y + dy * (rectangle.p0.x - csegment->p0.x) / dx;
        x = rectangle.p0.x;
      }

      if (outcodeout == outcode0) {
        csegment->p0.x = x;
        csegment->p0.y = y;
        outcode0 = pt2_out_code(csegment->p0, rectangle);
      }
      else {
        csegment->p1.x = x;
        csegment->p1.y = y;
        outcode1 = pt2_out_code(csegment->p1, rectangle);
      }
    }
  }

  return true;
}

CC_INLINE BOOL setr2_clip(const DSEGMENT2 src_segment, const DTRIANGLE2 triangle, DSEGMENT2* csegment)
{
  DPOINT2* p = &csegment->p0;
  int pos = 0;

  if (!setr2_intersect(src_segment, triangle)) {
    return false;
  }

  *csegment = src_segment;

  if (se2_intersect_at(src_segment, tr2_edge(triangle, 0), p + pos)) {
    pos++;
  }

  if (se2_intersect_at(src_segment, tr2_edge(triangle, 1), p + pos)) {
    pos++;
  }

  if ((pos < 2) && se2_intersect_at(src_segment, tr2_edge(triangle, 2), p + pos)) {
    pos++;
  }

  if (pos == 1) {
    if (tr2_point_in_triangle(src_segment.p0, triangle)) {
      p[pos] = src_segment.p0;
    }
    else {
      p[pos] = src_segment.p1;
    }
  }

  return true;
}

CC_INLINE BOOL sequ2_clip(const DSEGMENT2 src_segment, const DQUADIX2 quadix, DSEGMENT2* csegment)
{
  DPOINT2* p = &csegment->p0;
  int pos = 0;

  if (!sequ2_intersect(src_segment, quadix)) {
    return false;
  }

  *csegment = src_segment;

  if (se2_intersect_at(src_segment, qu2_edge(quadix, 0), p + pos)) {
    pos++;
  }

  if (se2_intersect_at(src_segment, qu2_edge(quadix, 1), p + pos)) {
    pos++;
  }

  if ((pos < 2) && (se2_intersect_at(src_segment, qu2_edge(quadix, 2), p + pos))) {
    pos++;
  }

  if ((pos < 2) && (se2_intersect_at(src_segment, qu2_edge(quadix, 3), p + pos))) {
    pos++;
  }

  if (pos == 1) {
    if (qu2_point_in_quadix(src_segment.p0, quadix)) {
      p[pos] = src_segment.p0;
    }
    else {
      p[pos] = src_segment.p1;
    }
  }

  return true;
}

CC_INLINE BOOL seci2_clip(const DSEGMENT2 src_segment, const DCIRCLE circle, DSEGMENT2* csegment)
{
  DPOINT2 int_point[4];
  int int_point_n = seci2_intersection_point(src_segment, circle, (int_point));

  if (int_point_n == 2) {
    csegment->p0 = int_point[0];
    csegment->p1 = int_point[1];
    return true;
  }
  else {
    return false;
  }
}

CC_INLINE BOOL rtrt2_clip(const DRECTANGLE rectangle1, const DRECTANGLE rectangle2, DRECTANGLE* crectangle)
{
  return d2_clip(rectangle1.p0.x, rectangle1.p0.y, rectangle1.p1.x, rectangle1.p1.y, rectangle2.p0.x, rectangle2.p0.y, rectangle2.p1.x, rectangle2.p1.y, &crectangle->p0);
}

CC_INLINE BOOL boxbox3_clip(const DBOX3 box1, const DBOX3 box2, DBOX3* cbox)
{
  return d3_clip(box1.p0.x, box1.p0.y, box1.p0.z, box1.p1.x, box1.p1.y, box1.p1.z, box2.p0.x, box2.p0.y, box2.p0.z, box2.p1.x, box2.p1.y, box2.p1.z, &cbox->p0);
}

CC_INLINE T pt2_area(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return (0.5) * (
      (point1.x * (point2.y - point3.y)) +
      (point2.x * (point3.y - point1.y)) +
      (point3.x * (point1.y - point2.y))
      );
}

CC_INLINE T pt3_area(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  T dx1 = point2.x - point1.x;
  T dy1 = point2.y - point1.y;
  T dz1 = point2.z - point1.z;

  T dx2 = point3.x - point1.x;
  T dy2 = point3.y - point1.y;
  T dz2 = point3.z - point1.z;

  T cx = dy1 * dz2 - dy2 * dz1;
  T cy = dx2 * dz1 - dx1 * dz2;
  T cz = dx1 * dy2 - dx2 * dy1;

  return (sqrt(cx * cx + cy * cy + cz * cz) * (0.5));
}

CC_INLINE T tr2_area(const DTRIANGLE2 triangle)
{
  return (0.5) * (
      (triangle.p0.x * (triangle.p1.y - triangle.p2.y)) +
      (triangle.p1.x * (triangle.p2.y - triangle.p0.y)) +
      (triangle.p2.x * (triangle.p0.y - triangle.p1.y))
      );
}

CC_INLINE T tr3_area(const DTRIANGLE3 triangle)
{
  T dx1 = triangle.p1.x - triangle.p0.x;
  T dy1 = triangle.p1.y - triangle.p0.y;
  T dz1 = triangle.p1.z - triangle.p0.z;

  T dx2 = triangle.p2.x - triangle.p0.x;
  T dy2 = triangle.p2.y - triangle.p0.y;
  T dz2 = triangle.p2.z - triangle.p0.z;

  T cx = dy1 * dz2 - dy2 * dz1;
  T cy = dx2 * dz1 - dx1 * dz2;
  T cz = dx1 * dy2 - dx2 * dy1;

  return (sqrt(cx * cx + cy * cy + cz * cz) * (0.5));
}

CC_INLINE T qu2_area(const DQUADIX2 quadix)
{
  return (0.5) * (
      (quadix.p0.x * (quadix.p1.y - quadix.p3.y)) +
      (quadix.p1.x * (quadix.p2.y - quadix.p0.y)) +
      (quadix.p2.x * (quadix.p3.y - quadix.p1.y)) +
      (quadix.p3.x * (quadix.p0.y - quadix.p2.y))
      );
}

CC_INLINE T qu3_area(const DQUADIX3 quadix)
{
  return (
      tr3_area(pt3_make_triangle(quadix.p0, quadix.p1, quadix.p2)) +
      tr3_area(pt3_make_triangle(quadix.p2, quadix.p3, quadix.p0))
      );
}

CC_INLINE T rt2_area(const DRECTANGLE rectangle)
{
  return T_ABS(rectangle.p1.x - rectangle.p0.x) * T_ABS(rectangle.p1.y - rectangle.p0.y);
}

CC_INLINE T ci2_area(const DCIRCLE circle)
{
  return (PI) * circle.radius * circle.radius;
}

CC_INLINE T po2_area(int n, const DPOINT2* polygon)
{
  T result = (0.0);

  if (n < 3) {
    return (0.0);
  }

  {

    int i, j = n - 1;

    for (i = 0; i < n; ++i) {
      result += ((polygon[j].x * polygon[i].y) - (polygon[j].y * polygon[i].x));
      j = i;
    }

  }

  return T_ABS(result * (0.5));
}

CC_INLINE T pt2_perimeter(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return pt2_distance(point1, point2) + pt2_distance(point2, point3) + pt2_distance(point3, point1);
}

CC_INLINE T pt3_perimeter(const DPOINT3 point1, const DPOINT3 point2, const DPOINT3 point3)
{
  return pt3_distance(point1, point2) + pt3_distance(point2, point3) + pt3_distance(point3, point1);
}

CC_INLINE T tr2_perimeter(const DTRIANGLE2 triangle)
{
  return pt2_perimeter(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE T tr3_perimeter(const DTRIANGLE3 triangle)
{
  return pt3_perimeter(triangle.p0, triangle.p1, triangle.p2);
}

CC_INLINE T qu2_perimeter(const DQUADIX2 quadix)
{
  return pt2_distance(quadix.p0, quadix.p1) +
      pt2_distance(quadix.p1, quadix.p2) +
      pt2_distance(quadix.p2, quadix.p3) +
      pt2_distance(quadix.p3, quadix.p0);
}

CC_INLINE T qu3_perimeter(const DQUADIX3 quadix)
{
  return pt3_distance(quadix.p0, quadix.p1) +
      pt3_distance(quadix.p1, quadix.p2) +
      pt3_distance(quadix.p2, quadix.p3) +
      pt3_distance(quadix.p3, quadix.p0);
}

CC_INLINE T rt2_perimeter(const DRECTANGLE rectangle)
{
  return (2.0) * (T_ABS(rectangle.p1.x - rectangle.p0.x) + T_ABS(rectangle.p1.y - rectangle.p0.y));
}

CC_INLINE T ci2_perimeter(const DCIRCLE circle)
{
  return (2.0) * (PI) * circle.radius;
}

CC_INLINE T po2_perimeter(int n, const DPOINT2* polygon)
{
  T total_perimeter = (0.0);

  if (n < 3) {
    return (0.0);
  }

  {
    int i, j = n - 1;

    for (i = 0; i < n; ++i) {
      total_perimeter += pt2_distance(polygon[j], polygon[i]);
      j = i;
    }
  }

  return total_perimeter;
}

CC_INLINE DSEGMENT2 se2_rotate(const T rotation_angle, const DSEGMENT2 segment)
{
  DSEGMENT2 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt2_rotate(rotation_angle, (&segment.p0)[i]);
  }

  return segment_;
}

CC_INLINE DSEGMENT2 se2_rotate_o(const T rotation_angle, const DSEGMENT2 segment, const DPOINT2 opoint)
{
  DSEGMENT2 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt2_rotate_o(rotation_angle, (&segment.p0)[i], opoint);
  }

  return segment_;
}

CC_INLINE DTRIANGLE2 tr2_rotate(const T rotation_angle, const DTRIANGLE2 triangle)
{
  DTRIANGLE2 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt2_rotate(rotation_angle, (&triangle.p0)[i]);
  }

  return triangle_;
}

CC_INLINE DTRIANGLE2 tr2_rotate_o(const T rotation_angle, const DTRIANGLE2 triangle, const DPOINT2 opoint)
{
  DTRIANGLE2 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt2_rotate_o(rotation_angle, (&triangle.p0)[i], opoint);
  }

  return triangle_;
}

CC_INLINE DQUADIX2 qu2_rotate(const T rotation_angle, const DQUADIX2 quadix)
{
  DQUADIX2 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt2_rotate(rotation_angle, (&quadix.p0)[i]);
  }

  return quadix_;
}

CC_INLINE DQUADIX2 qu2_rotate_o(const T rotation_angle, const DQUADIX2 quadix, const DPOINT2 opoint)
{
  DQUADIX2 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt2_rotate_o(rotation_angle, (&quadix.p0)[i], opoint);
  }

  return quadix_;
}

CC_INLINE int po2_rotate(const T rotation_angle, int n, const DPOINT2* polygon, DPOINT2* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt2_rotate(rotation_angle, polygon[i]);
  }

  return 0;
}

CC_INLINE int po2_rotate_o(const T rotation_angle, int n, const DPOINT2* polygon, const DPOINT2 opoint, DPOINT2* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt2_rotate_o(rotation_angle, polygon[i], opoint);
  }

  return 0;
}

CC_INLINE DPOINT3 d3_rotate(const T rx, const T ry, const T rz, const T x, const T y, const T z)
{
  DPOINT3 pt;
  T xradang = rx * (PIDiv180);
  T yradang = ry * (PIDiv180);
  T zradang = rz * (PIDiv180);

  T sinx = sin(xradang);
  T siny = sin(yradang);
  T sinz = sin(zradang);

  T cosx = cos(xradang);
  T cosy = cos(yradang);
  T cosz = cos(zradang);

  T tempy = y * cosy - z * siny;
  T tempz = y * siny + z * cosy;
  T tempx = x * cosx - tempz * sinx;

  pt.z = x * sinx + tempz * cosx;
  pt.x = tempx * cosz - tempy * sinz;
  pt.y = tempx * sinz + tempy * cosz;
  return pt;
}

CC_INLINE DPOINT3 d3_rotate_o(const T rx, const T ry, const T rz, const T x, const T y, const T z, const T ox, const T oy, const T oz)
{
  DPOINT3 pt = d3_rotate(rx, ry, rz, x - ox, y - oy, z - oz);
  pt.x += ox;
  pt.y += oy;
  pt.z += oz;
  return pt;
}

CC_INLINE DPOINT3 pt3_rotate(const T rx, const T ry, const T rz, const DPOINT3 point)
{
  return d3_rotate(rx, ry, rz, point.x, point.y, point.z);
}

CC_INLINE DPOINT3 pt3_rotate_o(const T rx, const T ry, const T rz, const DPOINT3 point, const DPOINT3 opoint)
{
  return d3_rotate_o(rx, ry, rz, point.x, point.y, point.z, opoint.x, opoint.y, opoint.z);
}

CC_INLINE DSEGMENT3 se3_rotate(const T rx, const T ry, const T rz, const DSEGMENT3 segment)
{
  DSEGMENT3 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt3_rotate(rx, ry, rz, (&segment.p0)[i]);
  }

  return segment_;
}

CC_INLINE DSEGMENT3 se3_rotate_o(const T rx, const T ry, const T rz, const DSEGMENT3 segment, const DPOINT3 opoint)
{
  DSEGMENT3 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt3_rotate_o(rx, ry, rz, (&segment.p0)[i], opoint);
  }

  return segment_;
}

CC_INLINE DTRIANGLE3 tr3_rotate(const T rx, const T ry, const T rz, const DTRIANGLE3 triangle)
{
  DTRIANGLE3 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt3_rotate(rx, ry, rz, (&triangle.p0)[i]);
  }

  return triangle_;
}

CC_INLINE DTRIANGLE3 tr3_rotate_o(const T rx, const T ry, const T rz, const DTRIANGLE3 triangle, const DPOINT3 opoint)
{
  DTRIANGLE3 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt3_rotate_o(rx, ry, rz, (&triangle.p0)[i], opoint);
  }

  return triangle_;
}

CC_INLINE DQUADIX3 qu3_rotate(const T rx, const T ry, const T rz, const DQUADIX3 quadix)
{
  DQUADIX3 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt3_rotate(rx, ry, rz, (&quadix.p0)[i]);
  }

  return quadix_;
}

CC_INLINE DQUADIX3 qu3_rotate_o(const T rx, const T ry, const T rz, const DQUADIX3 quadix, const DPOINT3 opoint)
{
  DQUADIX3 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt3_rotate_o(rx, ry, rz, (&quadix.p0)[i], opoint);
  }

  return quadix_;
}

CC_INLINE int po3_rotate(const T rx, const T ry, const T rz, int n, const DPOINT3* polygon, DPOINT3* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt3_rotate(rx, ry, rz, polygon[i]);
  }

  return n;
}

CC_INLINE int po3_rotate_o(const T rx, const T ry, const T rz, int n, const DPOINT3* polygon, const DPOINT3 opoint, DPOINT3* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt3_rotate_o(rx, ry, rz, polygon[i], opoint);
  }

  return n;
}

CC_INLINE DPOINT2 d2_fast_rotate(const int rotation_angle, const T x, const T y)
{
  DPOINT2 pt;
  int rot_ang = rotation_angle % 360;

  if (rot_ang < 0) {
    rot_ang = 360 + rot_ang;
  }

  {
    T sin_val = (_sin_table[(uint)(rot_ang)]);
    T cos_val = (_cos_table[(uint)(rot_ang)]);
    pt.x = (x * cos_val) - (y * sin_val);
    pt.y = (y * cos_val) + (x * sin_val);
  }

  return pt;
}

CC_INLINE DPOINT2 d2_fast_rotate_o(const int rotation_angle, const T x, const T y, const T ox, const T oy)
{
  DPOINT2 pt = d2_fast_rotate(rotation_angle, x - ox, y - oy);
  pt.x += ox;
  pt.y += oy;
  return pt;
}

CC_INLINE DPOINT2 pt2_fast_rotate(const int rotation_angle, const DPOINT2 point)
{
  return d2_fast_rotate(rotation_angle, point.x, point.y);
}

CC_INLINE DPOINT2 pt2_fast_rotate_o(const int rotation_angle, const DPOINT2 point, const DPOINT2 opoint)
{
  return d2_fast_rotate_o(rotation_angle, point.x, point.y, opoint.x, opoint.y);
}

CC_INLINE DSEGMENT2 se2_fast_rotate(const int rotation_angle, const DSEGMENT2 segment)
{
  DSEGMENT2 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt2_fast_rotate(rotation_angle, (&segment.p0)[i]);
  }

  return segment_;
}

CC_INLINE DSEGMENT2 se2_fast_rotate_o(const int rotation_angle, const DSEGMENT2 segment, const DPOINT2 opoint)
{
  DSEGMENT2 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt2_fast_rotate_o(rotation_angle, (&segment.p0)[i], opoint);
  }

  return segment_;
}

CC_INLINE DTRIANGLE2 tr2_fast_rotate(const int rotation_angle, const DTRIANGLE2 triangle)
{
  DTRIANGLE2 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt2_fast_rotate(rotation_angle, (&triangle.p0)[i]);
  }

  return triangle_;
}

CC_INLINE DTRIANGLE2 tr2_fast_rotate_o(const int rotation_angle, const DTRIANGLE2 triangle, const DPOINT2 opoint)
{
  DTRIANGLE2 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt2_fast_rotate_o(rotation_angle, (&triangle.p0)[i], opoint);
  }

  return triangle_;
}

CC_INLINE DQUADIX2 qu2_fast_rotate(const int rotation_angle, const DQUADIX2 quadix)
{
  DQUADIX2 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt2_fast_rotate(rotation_angle, (&quadix.p0)[i]);
  }

  return quadix_;
}

CC_INLINE DQUADIX2 qu2_fast_rotate_o(const int rotation_angle, const DQUADIX2 quadix, const DPOINT2 opoint)
{
  DQUADIX2 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt2_fast_rotate_o(rotation_angle, (&quadix.p0)[i], opoint);
  }

  return quadix_;
}

CC_INLINE int po2_fast_rotate(const int rotation_angle, int n, const DPOINT2* polygon, DPOINT2* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt2_fast_rotate(rotation_angle, polygon[i]);
  }

  return 0;
}

CC_INLINE int po2_fast_rotate_o(const int rotation_angle, int n, const DPOINT2* polygon, const DPOINT2 opoint, DPOINT2* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt2_fast_rotate_o(rotation_angle, polygon[i], opoint);
  }

  return 0;
}

CC_INLINE DPOINT3 d3_fast_rotate(const int rx, const int ry, const int rz, const T x, const T y, const T z)
{
  DPOINT3 pt;
  int rx_ = rx % 360;
  int ry_ = ry % 360;
  int rz_ = rz % 360;

  if (rx_ < 0) {
    rx_ += 360;
  }

  if (ry_ < 0) {
    ry_ += 360;
  }

  if (rz_ < 0) {
    rz_ += 360;
  }

  {

    T sin_x = (_sin_table[(uint)(rx_)]);
    T sin_y = (_sin_table[(uint)(ry_)]);
    T sin_z = (_sin_table[(uint)(rz_)]);

    T cos_x = (_cos_table[(uint)(rx_)]);
    T cos_y = (_cos_table[(uint)(ry_)]);
    T cos_z = (_cos_table[(uint)(rz_)]);

    T tmp_y = y * cos_y - z * sin_y;
    T tmp_z = y * sin_y + z * cos_y;
    T tmp_x = x * cos_x - tmp_z * sin_x;

    pt.z = x * sin_x + tmp_z * cos_x;
    pt.x = tmp_x * cos_z - tmp_y * sin_z;
    pt.y = tmp_x * sin_z + tmp_y * cos_z;
  }

  return pt;
}

CC_INLINE DPOINT3 d3_fast_rotate_o(const int rx, const int ry, const int rz, const T x, const T y, const T z, const T ox, const T oy, const T oz)
{
  DPOINT3 pt = d3_fast_rotate(rx, ry, rz, x - ox, y - oy, z - oz);
  pt.x += ox;
  pt.y += oy;
  pt.z += oz;
  return pt;
}

CC_INLINE DPOINT3 pt3_fast_rotate(const int rx, const int ry, const int rz, const DPOINT3 point)
{
  return d3_fast_rotate(rx, ry, rz, point.x, point.y, point.z);
}

CC_INLINE DPOINT3 pt3_fast_rotate_o(const int rx, const int ry, const int rz, const DPOINT3 point, const DPOINT3 opoint)
{
  return d3_fast_rotate_o(rx, ry, rz, point.x, point.y, point.z, opoint.x, opoint.y, opoint.z);
}

CC_INLINE DSEGMENT3 se3_fast_rotate(const int rx, const int ry, const int rz, const DSEGMENT3 segment)
{
  DSEGMENT3 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt3_fast_rotate(rx, ry, rz, (&segment.p0)[i]);
  }

  return segment_;
}

CC_INLINE DSEGMENT3 se3_fast_rotate_o(const int rx, const int ry, const int rz, const DSEGMENT3 segment, const DPOINT3 opoint)
{
  DSEGMENT3 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt3_fast_rotate_o(rx, ry, rz, (&segment.p0)[i], opoint);
  }

  return segment_;
}

CC_INLINE DTRIANGLE3 tr3_fast_rotate(const int rx, const int ry, const int rz, const DTRIANGLE3 triangle)
{
  DTRIANGLE3 triangle_;
  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt3_fast_rotate(rx, ry, rz, (&triangle.p0)[i]);
  }

  return triangle_;
}

CC_INLINE DTRIANGLE3 fast_rotate_o(const int rx, const int ry, const int rz, const DTRIANGLE3 triangle, const DPOINT3 opoint)
{
  DTRIANGLE3 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt3_fast_rotate_o(rx, ry, rz, (&triangle.p0)[i], opoint);
  }

  return triangle_;
}

CC_INLINE DQUADIX3 qu3_fast_rotate(const int rx, const int ry, const int rz, const DQUADIX3 quadix)
{
  DQUADIX3 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt3_fast_rotate(rx, ry, rz, (&quadix.p0)[i]);
  }

  return quadix_;
}

CC_INLINE DQUADIX3 qu3_fast_rotate_o(const int rx, const int ry, const int rz, const DQUADIX3 quadix, const DPOINT3 opoint)
{
  DQUADIX3 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt3_fast_rotate_o(rx, ry, rz, (&quadix.p0)[i], opoint);
  }

  return quadix_;
}

CC_INLINE int po3_fast_rotate(const int rx, const int ry, const int rz, int n, const DPOINT3* polygon, DPOINT3* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt3_fast_rotate(rx, ry, rz, polygon[i]);
  }

  return 0;
}

CC_INLINE int po3_fast_rotate_o(const int rx, const int ry, const int rz, int n, const DPOINT3* polygon, const DPOINT3 opoint, DPOINT3* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt3_fast_rotate_o(rx, ry, rz, polygon[i], opoint);
  }

  return 0;
}

CC_INLINE DPOINT2 pt2_scale(const T dx, const T dy, const DPOINT2 point)
{
  DPOINT2 point_;
  point_.x = point.x * dx;
  point_.y = point.y * dy;
  return point_;
}

CC_INLINE DLINE2 li2_scale(const T dx, const T dy, const DLINE2 line)
{
  DLINE2 line_;

  int i;

  for (i = 0; i < LinePointCount; ++i) {
    (&line_.p0)[i] = pt2_scale(dx, dy, (&line.p0)[i]);
  }

  return line_;
}

CC_INLINE DSEGMENT2 se2_scale(const T dx, const T dy, const DSEGMENT2 segment)
{
  DSEGMENT2 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt2_scale(dx, dy, (&segment.p0)[i]);
  }

  return segment_;
}

CC_INLINE DTRIANGLE2 tr2_scale(const T dx, const T dy, const DTRIANGLE2 triangle)
{
  DTRIANGLE2 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt2_scale(dx, dy, (&triangle.p0)[i]);
  }

  return triangle_;
}

CC_INLINE DQUADIX2 qu2_scale(const T dx, const T dy, const DQUADIX2 quadix)
{
  DQUADIX2 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt2_scale(dx, dy, (&quadix.p0)[i]);
  }

  return quadix_;
}

CC_INLINE DRECTANGLE rt2_scale(const T dx, const T dy, const DRECTANGLE rectangle)
{
  DRECTANGLE rectangle_;

  int i;

  for (i = 0; i < RectanglePointCount; ++i) {
    (&rectangle_.p0)[i] = pt2_scale(dx, dy, (&rectangle.p0)[i]);
  }

  return rectangle_;
}

CC_INLINE DCIRCLE ci2_scale(const T dr, const DCIRCLE circle)
{
  DCIRCLE circle_;
  circle_.x = circle.x;
  circle_.y = circle.y;
  circle_.radius = circle.radius * dr;
  return circle_;
}

CC_INLINE int po2_scale(const T dx, const T dy, int n, const DPOINT2* polygon, DPOINT2* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt2_scale(dx, dy, polygon[i]);
  }

  return n;
}

CC_INLINE DPOINT3 pt3_scale(const T dx, const T dy, const T dz, const DPOINT3 point)
{
  DPOINT3 point_;
  point_.x = point.x * dx;
  point_.y = point.y * dy;
  point_.z = point.z * dz;
  return point_;
}

CC_INLINE DLINE3 li3_scale(const T dx, const T dy, const T dz, const DLINE3 line)
{
  DLINE3 line_;

  int i;

  for (i = 0; i < LinePointCount; ++i) {
    (&line_.p0)[i] = pt3_scale(dx, dy, dz, (&line.p0)[i]);
  }

  return line_;
}

CC_INLINE DSEGMENT3 se3_scale(const T dx, const T dy, const T dz, const DSEGMENT3 segment)
{
  DSEGMENT3 segment_;

  int i;

  for (i = 0; i < SegmentPointCount; ++i) {
    (&segment_.p0)[i] = pt3_scale(dx, dy, dz, (&segment.p0)[i]);
  }

  return segment_;
}

CC_INLINE DTRIANGLE3 tr3_scale(const T dx, const T dy, const T dz, const DTRIANGLE3 triangle)
{
  DTRIANGLE3 triangle_;

  int i;

  for (i = 0; i < TrianglePointCount; ++i) {
    (&triangle_.p0)[i] = pt3_scale(dx, dy, dz, (&triangle.p0)[i]);
  }

  return triangle_;
}

CC_INLINE DQUADIX3 qu3_scale(const T dx, const T dy, const T dz, const DQUADIX3 quadix)
{
  DQUADIX3 quadix_;

  int i;

  for (i = 0; i < QuadixPointCount; ++i) {
    (&quadix_.p0)[i] = pt3_scale(dx, dy, dz, (&quadix.p0)[i]);
  }

  return quadix_;
}

CC_INLINE DBOX3 box3_scale(const T dx, const T dy, const T dz, const DBOX3 box)
{
  DBOX3 box_;

  int i;

  for (i = 0; i < BoxPointCount; ++i) {
    (&box_.p0)[i] = pt3_scale(dx, dy, dz, (&box.p0)[i]);
  }

  return box_;
}

CC_INLINE DSPHERE sp3_scale(const T dr, const DSPHERE sphere)
{
  DSPHERE sphere_;
  sphere_.x = sphere.x;
  sphere_.y = sphere.y;
  sphere_.z = sphere.z;
  sphere_.radius = sphere.radius * dr;
  return sphere_;
}

CC_INLINE int po3_scale(const T dx, const T dy, const T dz, int n, const DPOINT3* polygon, DPOINT3* polygon_)
{
  int i;

  for (i = 0; i < n; ++i) {
    polygon_[i] = pt3_scale(dx, dy, dz, polygon[i]);
  }

  return 0;
}

CC_INLINE DRECTANGLE rt2_update_rectangle(const DRECTANGLE rectangle, DPOINT2 point)
{
  if (!rt2_point_in_rectangle(point, rectangle)) {
    return d2_make_rectangle(MIN3(rectangle.p0.x, rectangle.p1.x, point.x),
        MIN3(rectangle.p0.y, rectangle.p1.y, point.y),
        MAX3(rectangle.p0.x, rectangle.p1.x, point.x),
        MAX3(rectangle.p0.y, rectangle.p1.y, point.y));
  }
  else {
    return rectangle;
  }
}

CC_INLINE DBOX3 box3_update_box(const DBOX3 box, DPOINT3 point)
{
  if (!box3_point_in_box(point, box)) {
    return d3_make_box(MIN3(box.p0.x, box.p1.x, point.x),
        MIN3(box.p0.y, box.p1.y, point.y),
        MIN3(box.p0.z, box.p1.z, point.z),
        MAX3(box.p0.x, box.p1.x, point.x),
        MAX3(box.p0.y, box.p1.y, point.y),
        MAX3(box.p0.z, box.p1.z, point.z));
  }
  else {
    return box;
  }
}

CC_INLINE DCIRCLE ci2_update_circle(const DCIRCLE circle, DPOINT2 point)
{
  DVECTOR2 d = v2_sub(point, d2_make_point(circle.x, circle.y));
  T dist2 = v2_dot_product(d, d);

  if (dist2 > sqr(circle.radius)) {
    T dist = sqrt(dist2);
    T new_radius = (circle.radius + dist) * (0.5);
    T ratio = (new_radius - circle.radius) / dist;
    return d2_make_circle(circle.x + (d.x * ratio), circle.y + (d.y * ratio), new_radius);
  }
  else {
    return circle;
  }
}

CC_INLINE DSPHERE sp3_update_sphere(const DSPHERE sphere, DPOINT3 point)
{
  DVECTOR3 d = v3_sub(point, d3_make_point(sphere.x, sphere.y, sphere.z));
  T dist2 = v3_dot_product(d, d);

  if (dist2 > sqr(sphere.radius)) {
    T dist = sqrt(dist2);
    T new_radius = (sphere.radius + dist) * (0.5);
    T ratio = (new_radius - sphere.radius) / dist;
    return d3_make_sphere(sphere.x + (d.x * ratio), sphere.y + (d.y * ratio), sphere.z + (d.z * ratio), new_radius);
  }
  else {
    return sphere;
  }
}

CC_INLINE DTRIANGLE2 tr2_right_shift(const DTRIANGLE2 triangle, const int shift)
{
  switch (shift % 3) {
  case 0 :
    return triangle;

  case 1 :
    return pt2_make_triangle(triangle.p2, triangle.p0, triangle.p1);

  case 2 :
    return pt2_make_triangle(triangle.p1, triangle.p2, triangle.p0);

  default :
    return triangle;
  }
}

CC_INLINE DTRIANGLE3 right_shift(const DTRIANGLE3 triangle, const int shift)
{
  switch (shift % 3) {
  case 0 :
    return triangle;

  case 1 :
    return pt3_make_triangle(triangle.p2, triangle.p0, triangle.p1);

  case 2 :
    return pt3_make_triangle(triangle.p1, triangle.p2, triangle.p0);

  default :
    return triangle;
  }
}

CC_INLINE DQUADIX2 qu2_right_shift(const DQUADIX2 quadix, const int shift)
{
  switch (shift % 4) {
  case 0 :
    return quadix;

  case 1 :
    return pt2_make_quadix(quadix.p3, quadix.p0, quadix.p1, quadix.p2);

  case 2 :
    return pt2_make_quadix(quadix.p2, quadix.p3, quadix.p0, quadix.p1);

  case 3 :
    return pt2_make_quadix(quadix.p1, quadix.p2, quadix.p3, quadix.p0);

  default :
    return quadix;
  }
}

CC_INLINE DQUADIX3 qu3_right_shift(const DQUADIX3 quadix, const int shift)
{
  switch (shift % 4) {
  case 0 :
    return quadix;

  case 1 :
    return pt3_make_quadix(quadix.p3, quadix.p0, quadix.p1, quadix.p2);

  case 2 :
    return pt3_make_quadix(quadix.p2, quadix.p3, quadix.p0, quadix.p1);

  case 3 :
    return pt3_make_quadix(quadix.p1, quadix.p2, quadix.p3, quadix.p0);

  default :
    return quadix;
  }
}

