
// Note: Clip boundries must be convex.

typedef struct half_plane_edge {
  T a, b, c;
} half_plane_edge;

half_plane_edge pt2_make_half_plane_edge(const DPOINT2 point1, const DPOINT2 point2)
{
  half_plane_edge s;
  s.a = (point2.y - point1.y), s.b = (point1.x - point2.x), s.c = (-s.a * point1.x - s.b * point1.y);
  return s;
}

BOOL pt2_inside_half_plane(half_plane_edge s, const DPOINT2 point)
{
  return ((0.0) < (s.a * point.x + s.b * point.y + s.c));
}

DPOINT2 pthp2_intersection_point(half_plane_edge s, const DPOINT2 point1, const DPOINT2 point2)
{
  T d = point2.y - point1.y;
  T e = point1.x - point2.x;
  T f = -d * point1.x - e * point1.y;
  T ratio = (1.0) / (e * s.a - s.b * d);
  return d2_make_point((s.b * f - e * s.c) * ratio, (d * s.c - s.a * f) * ratio);
}

int clip_against_edge(const half_plane_edge edge, int input_poly_n, const DPOINT2* input_poly, DPOINT2* clipped_poly)
{
  int it, j;
  DPOINT2 previous_point;

  if (input_poly_n < 2) {
    input_poly_n = 0;
    input_poly_n = 0;
    return 0;
  }

  previous_point = input_poly[input_poly_n - 1];

  j = 0;

  for (it = 0; it != input_poly_n; ++it) {
    DPOINT2 current_point = input_poly[it];
    BOOL current_point_in = pt2_inside_half_plane(edge, current_point);
    BOOL previous_point_in = pt2_inside_half_plane(edge, previous_point);

    if (current_point_in && previous_point_in) {
      clipped_poly[j++] = (current_point);
    }
    else if (!current_point_in && previous_point_in) {
      clipped_poly[j++] = (pthp2_intersection_point(edge, current_point, previous_point));
    }
    else if (current_point_in && !previous_point_in) {
      clipped_poly[j++] = (pthp2_intersection_point(edge, previous_point, current_point));
      clipped_poly[j++] = (current_point);
    }

    previous_point = current_point;
  }

  return j;
}

int clip(int edge_list_size, const half_plane_edge* edge_list, int n, const DPOINT2* begin, DPOINT2* out)
{
  DPOINT2* clip_poly1;
  DPOINT2* clip_poly2;
  DPOINT2* clip_poly_out;
  int i, nn = n;

  if (n < 3) {
    return 0;
  }

  if (edge_list_size < 3) {
    return 0;
  }

  clip_poly1 = MALLOC(DPOINT2, 2 * n);
  clip_poly2 = MALLOC(DPOINT2, 2 * n);
  MEMCPY(clip_poly1, begin, n);
  clip_poly_out = clip_poly1;

  for (i = 0; i < edge_list_size; ++i) {
    nn = clip_against_edge(edge_list[i], nn, clip_poly1, clip_poly2);
    CV_SWAP(clip_poly1, clip_poly2, clip_poly_out);
    clip_poly_out = clip_poly1;
  }

  MEMCPY(out, clip_poly_out, nn);
  FREE(clip_poly1);
  FREE(clip_poly2);
  return nn;
}

int po2_sutherland_hodgman_polygon_clipper(int clip_boundry_n, const DPOINT2* clip_boundry, int input_polygon_n, const DPOINT2* input_polygon, DPOINT2* clipped_polygon)
{
  int i, j, nn = 0, k = 0;
  half_plane_edge* half_edge_list = 0;

  if (input_polygon_n < 3) {
    return 0;
  }

  half_edge_list = MALLOC(half_plane_edge, clip_boundry_n);

  if (pt2_orientation(clip_boundry[0], clip_boundry[1], clip_boundry[2]) == LeftHandSide) {
    j = 0;

    for (i = clip_boundry_n - 1; (0 <= i) && (i < clip_boundry_n); i--) {
      half_edge_list[k++] = pt2_make_half_plane_edge(clip_boundry[j], clip_boundry[i]);
      j = i;
    }
  }
  else {
    j = clip_boundry_n - 1;

    for (i = 0; i < clip_boundry_n; ++i) {
      half_edge_list[k++] = pt2_make_half_plane_edge(clip_boundry[j], clip_boundry[i]);
      j = i;
    }
  }

  nn = clip(k, half_edge_list, input_polygon_n, input_polygon, clipped_polygon);
  FREE(half_edge_list);
  return nn;
}

int rt2_sutherland_hodgman_polygon_clipper(const DRECTANGLE clip_boundry, int input_polygon_n, const DPOINT2* input_polygon, DPOINT2* clipped_polygon)
{
  DPOINT2 pt[4];
  int n;

  if (input_polygon_n < 3) {
    return 0;
  }

  n = rt_make_polygon(clip_boundry, pt);
  return po2_sutherland_hodgman_polygon_clipper(n, pt, input_polygon_n, input_polygon, clipped_polygon);
}

