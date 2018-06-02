
DCIRCLE minimum_ball_with_2_points(int n, const DPOINT2* point, const DPOINT2 q1, const DPOINT2 q2)
{
  int i;
  DCIRCLE circle = pt22_make_circle(q1, q2);
  //random_shuffle(point, end);

  for (i = 1; i < n; ++i) {
    if (!ci2_point_in_circle(point[i], circle)) {
      circle = pt23_make_circle(q1, q2, point[i]);
    }
  }

  return circle;
}

DCIRCLE minimum_ball_with_1_point(int n, const DPOINT2* point, const DPOINT2 q)
{
  int i;
  DCIRCLE circle = pt22_make_circle(q, *point);
  //random_shuffle(point, end);

  for (i = 1; i < n; ++i) {
    if (!ci2_point_in_circle(point[i], circle)) {
      circle = minimum_ball_with_2_points(i - 1, point, q, point[i]);
    }
  }

  return circle;
}

DCIRCLE randomized_minimum_bounding_ball(int n, const DPOINT2* point)
{
  DCIRCLE circle = {0};
  int i;

  switch (n) {
  case 0 : {
    return circle;
  }
  break;

  case 1 : {
    return pt2_make_circle(*point, (0.0));
  }
  break;

  case 2 : {
    return pt22_make_circle(*point, *(point + 1));
  }
  break;

  case 3 : {
    return pt23_make_circle(*point, *(point + 1), *(point + 2));
  }
  break;
  }

  //random_shuffle(point_list.point(), point_list.end());
  circle = pt22_make_circle(point[0], point[1]);

  for (i = 2; i < n; ++i) {
    if (!ci2_point_in_circle(point[i], circle)) {
      circle = minimum_ball_with_1_point(i - 1, point, point[i]);
    }
  }

  return circle;
}

DCIRCLE randomized_minimum_bounding_ball_with_ch_filter(int n, const DPOINT2* point)
{
  DPOINT2* convex_hull = MALLOC(DPOINT2, n);
  int cn2 = convex_hull_graham_scan(n, point, convex_hull);
  DCIRCLE circle = randomized_minimum_bounding_ball(cn2, convex_hull);
  FREE(convex_hull);
  return circle;
}

DCIRCLE naive_minimum_bounding_ball(int npt, const DPOINT2* point)
{
  DCIRCLE circle = {0};
  int it1, it2, it3, n;

  switch (npt) {
  case 0 : {
    return circle;
  }
  break;

  case 1 : {
    return pt2_make_circle(*point, (0.0));
  }
  break;

  case 2 : {
    return pt22_make_circle(*point, *(point + 1));
  }
  break;

  case 3 : {
    return pt23_make_circle(*point, *(point + 1), *(point + 2));
  }
  break;
  }

  circle = ci_degenerate();

  // Expected complexity O(n^4)
  for (it1 = 0; it1 != npt; ++it1) {
    for (it2 = it1 + 1; it2 != npt; ++it2) {
      for (it3 = it2 + 1; it3 != npt; ++it3) {
        DCIRCLE current_circle = pt23_make_circle(point[it1], point[it2], point[it3]);
        BOOL contains_all_points = true;

        for (n = 0; n != npt; ++n) {
          if ((n != it1) && (n != it2) && (n != it3) && (!ci2_point_in_circle(point[n], current_circle))) {
            contains_all_points = false;
            break;
          }
        }

        if (contains_all_points && (current_circle.radius < circle.radius)) {
          circle = current_circle;
        }
      }
    }
  }

  return circle;
}

DCIRCLE naive_minimum_bounding_ball_with_ch_filter(int n, const DPOINT2* point)
{
  DCIRCLE circle;
  DPOINT2* convex_hull = MALLOC(DPOINT2, n);
  int n2 = convex_hull_graham_scan(n, point, convex_hull);
  circle = naive_minimum_bounding_ball(n2, convex_hull);
  FREE(convex_hull);
  return circle;
}

DCIRCLE pt2_ritter_minimum_bounding_ball(int npt, const DPOINT2* point)
{
  DCIRCLE circle = {0};
  DPOINT2 dia1, dia2;
  DPOINT2 min_x, min_y, max_x, max_y;
  T span_x, span_y, max_span;
  T radius_sqr;
  int it;

  switch (npt) {
  case 0 : {
    return circle;
  }
  break;

  case 1 : {
    return pt2_make_circle(*point, (0.0));
  }
  break;

  case 2 : {
    return pt22_make_circle(*point, *(point + 1));
  }
  break;

  case 3 : {
    return pt23_make_circle(*point, *(point + 1), *(point + 2));
  }
  break;
  }

  min_x = pt2_positive_infinite();
  min_y = pt2_positive_infinite();
  max_x = pt2_negative_infinite();
  max_y = pt2_negative_infinite();

  for (it = 0; it != npt; ++it) {
    DPOINT2 current_point = point[it];

    if (current_point.x < min_x.x) {
      min_x = current_point;
    }

    if (current_point.x > max_x.x) {
      max_x = current_point;
    }

    if (current_point.y < min_y.y) {
      min_y = current_point;
    }

    if (current_point.y > max_y.y) {
      max_y = current_point;
    }
  }

  span_x = distance(max_x, min_x);
  span_y = distance(max_y, min_y);
  max_span = -InfinityT;
  dia1 = pt2_negative_infinite();
  dia2 = pt2_negative_infinite();

  if (span_x > span_y) {
    max_span = span_x;
    dia1 = min_x;
    dia2 = max_x;
  }
  else {
    max_span = span_y;
    dia1 = min_y;
    dia2 = max_y;
  }

  circle = pt22_make_circle(dia1, dia2);
  radius_sqr = sqr(circle.radius);

  for (it = 0; it != npt; ++it) {
    DPOINT2 current_point = point[it];
    T lay_dist = pt2_lay_distance_from_point_to_circle_center(current_point, circle);

    if (lay_dist > radius_sqr) {
      T ratio, difference, dist = sqrt(lay_dist);
      circle.radius = (circle.radius + dist) * (0.5);
      radius_sqr = sqr(circle.radius);
      difference = dist - circle.radius;
      ratio = (1.0) / dist;
      circle = d2_make_circle((circle.radius * circle.x + difference * current_point.x) * ratio,
          (circle.radius * circle.y + difference * current_point.y) * ratio, circle.radius);
    }
  }

  return circle;
}

DCIRCLE ritter_minimum_bounding_ball_with_ch_filter(int npt, const DPOINT2* point)
{
  DCIRCLE circle;
  DPOINT2* convex_hull = MALLOC(DPOINT2, npt);
  int n = convex_hull_graham_scan(npt, point, convex_hull);
  circle = pt2_ritter_minimum_bounding_ball(n, convex_hull);
  FREE(convex_hull);
  return circle;
}

DSPHERE pt3_ritter_minimum_bounding_ball(int npt, const DPOINT3* point)
{
  DSPHERE sphere = {0};
  int it;
  DPOINT3 min_x, min_y, min_z;
  DPOINT3 max_x, max_y, max_z;
  DPOINT3 dia1, dia2;
  T span_x, span_y, span_z;
  T radius_sqr, max_span;

  switch (npt) {
  case 0 : {
    return sphere;
  }
  break;

  case 1 : {
    return pt3_make_sphere(*point, (0.0));
  }
  break;

  case 2 : {
    return pt32_make_sphere(*point, *(point + 1));
  }
  break;
  }

  min_x = pt3_positive_infinite();
  min_y = pt3_positive_infinite();
  min_z = pt3_positive_infinite();
  max_x = pt3_negative_infinite();
  max_y = pt3_negative_infinite();
  max_z = pt3_negative_infinite();

  for (it = 0; it != npt; ++it) {
    DPOINT3 current_point = point[it];

    if (current_point.x < min_x.x) {
      min_x = current_point;
    }

    if (current_point.x > max_x.x) {
      max_x = current_point;
    }

    if (current_point.y < min_y.y) {
      min_y = current_point;
    }

    if (current_point.y > max_y.y) {
      max_y = current_point;
    }

    if (current_point.z < min_z.z) {
      min_z = current_point;
    }

    if (current_point.z > max_z.z) {
      max_z = current_point;
    }
  }

  span_x = pt3_distance(max_x, min_x);
  span_y = pt3_distance(max_y, min_y);
  span_z = pt3_distance(max_z, min_z);
  max_span = span_x;

  dia1 = min_x;
  dia2 = max_x;

  if (span_y > max_span) {
    max_span = span_y;
    dia1 = min_y;
    dia2 = max_y;
  }

  if (span_z > max_span) {
    max_span = span_z;
    dia1 = min_z;
    dia2 = max_z;
  }

  sphere = pt32_make_sphere(dia1, dia2);
  radius_sqr = sqr(sphere.radius);

  for (it = 0; it != npt; ++it) {
    DPOINT3 current_point = point[it];
    T lay_dist = pt3_lay_distance_from_point_to_sphere_center(current_point, sphere);

    if (lay_dist > radius_sqr) {
      T ratio, difference, dist = sqrt(lay_dist);
      sphere.radius = (sphere.radius + dist) * (0.5);
      radius_sqr = sqr(sphere.radius);
      difference = dist - sphere.radius;
      ratio = (1.0) / dist;
      sphere = d3_make_sphere((sphere.radius * sphere.x + difference * current_point.x) * ratio,
          (sphere.radius * sphere.y + difference * current_point.y) * ratio,
          (sphere.radius * sphere.z + difference * current_point.z) * ratio,
          sphere.radius);
    }
  }

  return sphere;
}

