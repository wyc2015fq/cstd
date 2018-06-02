
CC_INLINE void se2_generate_random_object(const T x1, const T y1, const T x2, const T y2, DSEGMENT2* segment)
{
  T dx = T_ABS(x2 - x1);
  T dy = T_ABS(y2 - y1);

  do {
    segment->p0.x = x1 + d1_generate_random_value(dx);
    segment->p0.y = y1 + d1_generate_random_value(dy);

    segment->p1.x = x1 + d1_generate_random_value(dx);
    segment->p1.y = y1 + d1_generate_random_value(dy);
  }
  while (se2_is_degenerate(*segment));
}

CC_INLINE void rt2_generate_random_object(const T x1, const T y1, const T x2, const T y2, DRECTANGLE* rectangle)
{
  T dx = T_ABS(x2 - x1);
  T dy = T_ABS(y2 - y1);

  do {
    rectangle->p0.x = x1 + d1_generate_random_value(dx);
    rectangle->p0.y = y1 + d1_generate_random_value(dy);

    rectangle->p1.x = x1 + d1_generate_random_value(dx);
    rectangle->p1.y = y1 + d1_generate_random_value(dy);
  }
  while (rt_is_degenerate(*rectangle));

  if (rectangle->p1.x < rectangle->p0.x) {
    T t = rectangle->p1.x;
    rectangle->p1.x = rectangle->p0.x;
    rectangle->p0.x = t;
  }

  if (rectangle->p1.y < rectangle->p0.y) {
    T t = rectangle->p1.y;
    rectangle->p1.y = rectangle->p0.y;
    rectangle->p0.y = t;
  }
}

CC_INLINE void tr2_generate_random_object(const T x1, const T y1, const T x2, const T y2, DTRIANGLE2* triangle)
{
  T dx = T_ABS(x2 - x1);
  T dy = T_ABS(y2 - y1);

  do {
    triangle->p0.x = x1 + d1_generate_random_value(dx);
    triangle->p0.y = y1 + d1_generate_random_value(dy);
    triangle->p1.x = x1 + d1_generate_random_value(dx);
    triangle->p1.y = y1 + d1_generate_random_value(dy);
    triangle->p2.x = x1 + d1_generate_random_value(dx);
    triangle->p2.y = y1 + d1_generate_random_value(dy);
  }
  while (tr2_is_degenerate(*triangle));
}

CC_INLINE void qu2_generate_random_object(const T x1, const T y1, const T x2, const T y2, DQUADIX2* quadix)
{
  T dx = T_ABS(x2 - x1);
  T dy = T_ABS(y2 - y1);

  do {
    quadix->p0.x = x1 + d1_generate_random_value(dx);
    quadix->p0.y = y1 + d1_generate_random_value(dy);
    quadix->p1.x = x1 + d1_generate_random_value(dx);
    quadix->p1.y = y1 + d1_generate_random_value(dy);
    quadix->p2.x = x1 + d1_generate_random_value(dx);
    quadix->p2.y = y1 + d1_generate_random_value(dy);
    quadix->p3.x = x1 + d1_generate_random_value(dx);
    quadix->p3.y = y1 + d1_generate_random_value(dy);
  }
  while (qu2_is_degenerate(*quadix));
}

CC_INLINE void ci_generate_random_object(const T x1, const T y1, const T x2, const T y2, DCIRCLE* circle)
{
  T dx = T_ABS(x2 - x1);
  T dy = T_ABS(y2 - y1);
  circle->radius = d1_generate_random_value(min(dx, dy) * (0.5));
  circle->x = x1 + circle->radius + d1_generate_random_value(dx - ((2.0) * circle->radius));
  circle->y = y1 + circle->radius + d1_generate_random_value(dy - ((2.0) * circle->radius));
}

CC_INLINE void box3_generate_random_object(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, DBOX3* box)
{
  DBOX3 region = d3_make_box(x1, y1, z1, x2, y2, z2);
  box->p0 = box3_generate_random_point(region);
  box->p1 = box3_generate_random_point(region);
}

