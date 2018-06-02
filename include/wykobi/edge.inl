
CC_INLINE DSEGMENT2 tr2_edge(const DTRIANGLE2 triangle, int edge_index)
{
  switch (edge_index) {
  case 0 :
    return pt2_make_segment(triangle.p0, triangle.p1);

  case 1 :
    return pt2_make_segment(triangle.p1, triangle.p2);

  case 2 :
    return pt2_make_segment(triangle.p2, triangle.p0);

  default :
    return se2_degenerate();
  }
}

CC_INLINE DSEGMENT3 tr3_edge(const DTRIANGLE3 triangle, int edge_index)
{
  switch (edge_index) {
  case 0 :
    return pt3_make_segment(triangle.p0, triangle.p1);

  case 1 :
    return pt3_make_segment(triangle.p1, triangle.p2);

  case 2 :
    return pt3_make_segment(triangle.p2, triangle.p0);

  default :
    return se3_degenerate();
  }
}

CC_INLINE DSEGMENT2 qu2_edge(const DQUADIX2 quadix, int edge_index)
{
  switch (edge_index) {
  case 0 :
    return pt2_make_segment(quadix.p0, quadix.p1);

  case 1 :
    return pt2_make_segment(quadix.p1, quadix.p2);

  case 2 :
    return pt2_make_segment(quadix.p2, quadix.p3);

  case 3 :
    return pt2_make_segment(quadix.p3, quadix.p0);

  default :
    return se2_degenerate();
  }
}

CC_INLINE DSEGMENT3 qu3_edge(const DQUADIX3 quadix, int edge_index)
{
  switch (edge_index) {
  case 0 :
    return pt3_make_segment(quadix.p0, quadix.p1);

  case 1 :
    return pt3_make_segment(quadix.p1, quadix.p2);

  case 2 :
    return pt3_make_segment(quadix.p2, quadix.p3);

  case 3 :
    return pt3_make_segment(quadix.p3, quadix.p0);

  default :
    return se3_degenerate();
  }
}

CC_INLINE DSEGMENT2 rt2_edge(const DRECTANGLE rectangle, int edge_index)
{
  switch (edge_index) {
  case 0 :
    return d2_make_segment(rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p0.y);

  case 1 :
    return d2_make_segment(rectangle.p1.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y);

  case 2 :
    return d2_make_segment(rectangle.p1.x, rectangle.p1.y, rectangle.p0.x, rectangle.p1.y);

  case 3 :
    return d2_make_segment(rectangle.p0.x, rectangle.p1.y, rectangle.p0.x, rectangle.p0.y);

  default :
    return se2_degenerate();
  }
}

CC_INLINE DSEGMENT2 po2_edge(int n, const DPOINT2* polygon, int edge_index)
{
  if (edge_index >= n) {
    return d2_make_segment((0.0), (0.0), (0.0), (0.0));
  }

  if (edge_index == (n - 1)) {
    return pt2_make_segment(polygon[edge_index], polygon[0]);
  }
  else {
    return pt2_make_segment(polygon[edge_index], polygon[edge_index + 1]);
  }
}

CC_INLINE DSEGMENT3 po3_edge(const DPOLYGON3* polygon, int edge_index)
{
  if (edge_index >= polygon->n) {
    return d3_make_segment((0.0), (0.0), (0.0), (0.0), (0.0), (0.0));
  }

  if (edge_index == (polygon->n - 1)) {
    return pt3_make_segment(polygon->p[edge_index], polygon->p[0]);
  }
  else {
    return pt3_make_segment(polygon->p[edge_index], polygon->p[edge_index + 1]);
  }
}

CC_INLINE DSEGMENT2 tr2_opposing_edge(const DTRIANGLE2 triangle, int corner)
{
  switch (corner) {
  case 0 :
    return tr2_edge(triangle, 1);

  case 1 :
    return tr2_edge(triangle, 2);

  case 2 :
    return tr2_edge(triangle, 0);

  default :
    return se2_degenerate();
  }
}

CC_INLINE DSEGMENT3 tr3_opposing_edge(const DTRIANGLE3 triangle, int corner)
{
  switch (corner) {
  case 0 :
    return tr3_edge(triangle, 1);

  case 1 :
    return tr3_edge(triangle, 2);

  case 2 :
    return tr3_edge(triangle, 0);

  default :
    return se3_degenerate();
  }
}

