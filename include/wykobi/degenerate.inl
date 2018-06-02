
CC_INLINE DRAY2 ray2_degenerate()
{
  return v2_make_ray(pt2_degenerate(), v2_degenerate());
}

CC_INLINE DRAY3 ray3_degenerate()
{
  return v3_make_ray(pt3_degenerate(), v3_degenerate());
}

CC_INLINE DLINE2 li2_degenerate()
{
  return pt2_make_line(pt2_degenerate(), pt2_degenerate());
}

CC_INLINE DLINE3 li3_degenerate()
{
  return pt3_make_line(pt3_degenerate(), pt3_degenerate());
}

CC_INLINE DSEGMENT2 se2_degenerate()
{
  return pt2_make_segment(pt2_degenerate(), pt2_degenerate());
}

CC_INLINE DSEGMENT3 se3_degenerate()
{
  return pt3_make_segment(pt3_degenerate(), pt3_degenerate());
}

CC_INLINE DTRIANGLE2 tr2_degenerate()
{
  return pt2_make_triangle(pt2_degenerate(),
      pt2_degenerate(),
      pt2_degenerate());
}

CC_INLINE DTRIANGLE3 tr3_degenerate()
{
  return pt3_make_triangle(pt3_degenerate(),
      pt3_degenerate(),
      pt3_degenerate());
}

CC_INLINE DQUADIX2 qu2_degenerate()
{
  return pt2_make_quadix(pt2_degenerate(),
      pt2_degenerate(),
      pt2_degenerate(),
      pt2_degenerate());
}

CC_INLINE DQUADIX3 qu3_degenerate()
{
  return pt3_make_quadix(pt3_degenerate(),
      pt3_degenerate(),
      pt3_degenerate(),
      pt3_degenerate());
}

CC_INLINE DRECTANGLE rt_degenerate()
{
  return pt2_make_rectangle(pt2_degenerate(), pt2_degenerate());
}

CC_INLINE DCIRCLE ci_degenerate()
{
  return pt2_make_circle(pt2_degenerate(), InfinityT);
}

CC_INLINE DSPHERE sp_degenerate()
{
  return pt3_make_sphere(pt3_degenerate(), InfinityT);
}

CC_INLINE DPOINT2 pt2_positive_infinite()
{
  return d2_make_point(+InfinityT, +InfinityT);
}

CC_INLINE DPOINT2 pt2_negative_infinite()
{
  return d2_make_point(-InfinityT, -InfinityT);
}

CC_INLINE DPOINT3 pt3_positive_infinite()
{
  return d3_make_point(+InfinityT, +InfinityT, +InfinityT);
}

CC_INLINE DPOINT3 pt3_negative_infinite()
{
  return d3_make_point(-InfinityT, -InfinityT, -InfinityT);
}

