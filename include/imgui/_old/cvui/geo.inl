struct circle_t {
  struct DPOINT center;
  double r;
};
int double_equals(double const a, double const b)
{
  static const double ZERO = 1e-9;
  return fabs(a - b) < ZERO;
}
double distance_sqr(struct DPOINT const* a, struct DPOINT const* b)
{
  return (a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y);
}
double distance(struct DPOINT const* a, struct DPOINT const* b)
{
  return sqrt(distance_sqr(a, b));
}
int circle_insect(struct circle_t circles[], struct DPOINT points[])
{
  double d, a, b, c, p, q, r;
  double cos_value[2], sin_value[2];
  if (double_equals(circles[0].center.x, circles[1].center.x)
      && double_equals(circles[0].center.y, circles[1].center.y)
      && double_equals(circles[0].r, circles[1].r)) {
    return -1;
  }
  d = distance(&circles[0].center, &circles[1].center);
  if (d > circles[0].r + circles[1].r
      || d < fabs(circles[0].r - circles[1].r)) {
    return 0;
  }
  a = 2.0 * circles[0].r * (circles[0].center.x - circles[1].center.x);
  b = 2.0 * circles[0].r * (circles[0].center.y - circles[1].center.y);
  c = circles[1].r * circles[1].r - circles[0].r * circles[0].r
      - distance_sqr(&circles[0].center, &circles[1].center);
  p = a * a + b * b;
  q = -2.0 * a * c;
  if (double_equals(d, circles[0].r + circles[1].r)
      || double_equals(d, fabs(circles[0].r - circles[1].r))) {
    cos_value[0] = -q / p / 2.0;
    sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);
    points[0].x = circles[0].r * cos_value[0] + circles[0].center.x;
    points[0].y = circles[0].r * sin_value[0] + circles[0].center.y;
    if (!double_equals(distance_sqr(&points[0], &circles[1].center),
        circles[1].r * circles[1].r)) {
      points[0].y = circles[0].center.y - circles[0].r * sin_value[0];
    }
    return 1;
  }
  r = c * c - b * b;
  cos_value[0] = (sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
  cos_value[1] = (-sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
  sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);
  sin_value[1] = sqrt(1 - cos_value[1] * cos_value[1]);
  points[0].x = circles[0].r * cos_value[0] + circles[0].center.x;
  points[1].x = circles[0].r * cos_value[1] + circles[0].center.x;
  points[0].y = circles[0].r * sin_value[0] + circles[0].center.y;
  points[1].y = circles[0].r * sin_value[1] + circles[0].center.y;
  if (!double_equals(distance_sqr(&points[0], &circles[1].center),
      circles[1].r * circles[1].r)) {
    points[0].y = circles[0].center.y - circles[0].r * sin_value[0];
  }
  if (!double_equals(distance_sqr(&points[1], &circles[1].center),
      circles[1].r * circles[1].r)) {
    points[1].y = circles[0].center.y - circles[0].r * sin_value[1];
  }
  if (double_equals(points[0].y, points[1].y)
      && double_equals(points[0].x, points[1].x)) {
    if (points[0].y > 0) {
      points[1].y = -points[1].y;
    }
    else {
      points[0].y = -points[0].y;
    }
  }
  return 2;
}
int test_circle_insect()
{
  struct circle_t circles[2];
  struct DPOINT points[2];
  printf("请输入两圆x，y，半径(以逗号分开)：");
  while (EOF != scanf("%lf,%lf,%lf,%lf,%lf,%lf",
      &circles[0].center.x, &circles[0].center.y, &circles[0].r,
      &circles[1].center.x, &circles[1].center.y, &circles[1].r)) {
    switch (circle_insect(circles, points)) {
    case -1:
      printf("THE CIRCLES ARE THE SAME/n");
      break;
    case 0:
      printf("NO INTERSECTION/n");
      break;
    case 1:
      printf("(%.3lf %.3lf)\n", points[0].x, points[0].y);
      break;
    case 2:
      printf("(%.3lf %.3lf) (%.3lf %.3lf)\n",
          points[0].x, points[0].y,
          points[1].x, points[1].y);
    }
  }
  return 0;
}

