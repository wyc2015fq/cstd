
CC_INLINE DPOINT2 d2_closest_point_on_segment_from_point(const T x1, const T y1, const T x2, const T y2, const T px, const T py)
{
  DPOINT2 pt;
  T c1, c2, ratio;
  T vx = x2 - x1;
  T vy = y2 - y1;
  T wx = px - x1;
  T wy = py - y1;

  c1 = vx * wx + vy * wy;

  if (c1 <= (0.0)) {
    pt.x = x1;
    pt.y = y1;
    return pt;
  }

  c2 = vx * vx + vy * vy;

  if (c2 <= c1) {
    pt.x = x2;
    pt.y = y2;
    return pt;
  }

  ratio = c1 / c2;

  pt.x = x1 + ratio * vx;
  pt.y = y1 + ratio * vy;
  return pt;
}

CC_INLINE DPOINT3 d3_closest_point_on_segment_from_point(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2, const T px, const T py, const T pz)
{
  DPOINT3 pt;
  T c1, c2, ratio;
  T vx = x2 - x1;
  T vy = y2 - y1;
  T vz = z2 - z1;
  T wx = px - x1;
  T wy = py - y1;
  T wz = pz - z1;

  c1 = vx * wx + vy * wy + vz * wz;

  if (c1 <= (0.0)) {
    pt.x = x1;
    pt.y = y1;
    pt.z = z1;
    return pt;
  }

  c2 = vx * vx + vy * vy + vz * vz;

  if (c2 <= c1) {
    pt.x = x2;
    pt.y = y2;
    pt.z = z2;
    return pt;
  }

  ratio = c1 / c2;

  pt.x = x1 + ratio * vx;
  pt.y = y1 + ratio * vy;
  pt.z = z1 + ratio * vz;
}

CC_INLINE DPOINT2 d2_closest_point_on_line_from_point(const T x1, const T y1,
    const T x2, const T y2, const T px, const T py)
{
  DPOINT2 pt;
  T vx = x2 - x1;
  T vy = y2 - y1;
  T wx = px - x1;
  T wy = py - y1;

  T c1 = vx * wx + vy * wy;
  T c2 = vx * vx + vy * vy;

  T ratio = c1 / c2;

  pt.x = x1 + ratio * vx;
  pt.y = y1 + ratio * vy;
  return pt;
}

CC_INLINE DPOINT3 d3_closest_point_on_line_from_point(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2, const T px, const T py, const T pz)
{
  DPOINT3 pt;
  T vx = x2 - x1;
  T vy = y2 - y1;
  T vz = z2 - z1;
  T wx = px - x1;
  T wy = py - y1;
  T wz = pz - z1;

  T c1 = vx * wx + vy * wy + vz * wz;
  T c2 = vx * vx + vy * vy + vz * vz;

  T ratio = c1 / c2;

  pt.x = x1 + ratio * vx;
  pt.y = y1 + ratio * vy;
  pt.z = z1 + ratio * vz;
  return pt;
}

CC_INLINE DPOINT2 d2_order_sensitive_closest_point_on_segment_from_point(const T x1, const T y1, const T x2, const T y2, const T px, const T py)
{
  DPOINT2 pt;

  if (x1 < x2) {
    return d2_closest_point_on_segment_from_point(x1, y1, x2, y2, px, py);
  }
  else if (d_is_equal(x1, x2) && (y1 < y2)) {
    return d2_closest_point_on_segment_from_point(x1, y1, x2, y2, px, py);
  }
  else {
    d2_closest_point_on_segment_from_point(x2, y2, x1, y1, px, py);
  }

  return pt;
}

CC_INLINE DPOINT3 d3_order_sensitive_closest_point_on_segment_from_point(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T px, const T py, const T pz)
{
  DPOINT3 pt;

  if (x1 < x2) {
    pt = d3_closest_point_on_segment_from_point(x1, y1, z1, x2, y2, z2, px, py, pz);
    return pt;
  }
  else if (d_is_equal(x1, x2) && (y1 < y2)) {
    pt = d3_closest_point_on_segment_from_point(x1, y1, z1, x2, y2, z2, px, py, pz);
    return pt;
  }
  else if (d_is_equal(y1, y2) && (z1 < z2)) {
    pt = d3_closest_point_on_segment_from_point(x1, y1, z1, x2, y2, z2, px, py, pz);
    return pt;
  }
  else {
    pt = d3_closest_point_on_segment_from_point(x2, y2, z2, x1, y1, z1, px, py, pz);
  }

  return pt;
}

CC_INLINE DPOINT2 d2_order_sensitive_closest_point_on_line_from_point(const T x1, const T y1, const T x2, const T y2, const T px, const T py)
{
  DPOINT2 pt;

  if (x1 < x2) {
    pt = d2_closest_point_on_line_from_point(x1, y1, x2, y2, px, py);
    return pt;
  }
  else if (d_is_equal(x1, x2) && (y1 < y2)) {
    pt = d2_closest_point_on_line_from_point(x1, y1, x2, y2, px, py);
    return pt;
  }
  else {
    pt = d2_closest_point_on_line_from_point(x2, y2, x1, y1, px, py);
  }

  return pt;
}

CC_INLINE DPOINT3 d3_order_sensitive_closest_point_on_line_from_point(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T px, const T py, const T pz)
{
  DPOINT3 pt;

  if (x1 < x2) {
    pt = d3_closest_point_on_line_from_point(x1, y1, z1, x2, y2, z2, px, py, pz);
    return pt;
  }
  else if (d_is_equal(x1, x2) && (y1 < y2)) {
    pt = d3_closest_point_on_line_from_point(x1, y1, z1, x2, y2, z2, px, py, pz);
    return pt;
  }
  else if (d_is_equal(y1, y2) && (z1 < z2)) {
    pt = d3_closest_point_on_line_from_point(x1, y1, z1, x2, y2, z2, px, py, pz);
    return pt;
  }
  else {
    pt = d3_closest_point_on_segment_from_point(x2, y2, z2, x1, y1, z1, px, py, pz);
  }

  return pt;
}

CC_INLINE DPOINT2 d2_closest_point_on_ray_from_point(const T ox, const T oy, const T dx, const T dy, const T px, const T py)
{
  DPOINT2 pt;
  T t = dx * (px - ox) + dy * (py - oy);

  if (t < (0.0)) {
    pt.x = ox;
    pt.y = oy;
  }
  else {
    pt.x = ox + dx * t;
    pt.y = oy + dy * t;
  }

  return pt;
}

CC_INLINE DPOINT3 d3_closest_point_on_ray_from_point(const T ox, const T oy, const T oz, const T dx, const T dy, const T dz, const T px, const T py, const T pz)
{
  DPOINT3 pt;
  T t = dx * (px - ox) + dy * (py - oy) + dz * (pz - oz);

  if (t < (0.0)) {
    pt.x = ox;
    pt.y = oy;
    pt.z = oz;
  }
  else {
    pt.x = ox + dx * t;
    pt.y = oy + dy * t;
    pt.z = oz + dz * t;
  }

  return pt;
}

CC_INLINE DPOINT2 pt2_closest_point_on_segment_from_point(const DSEGMENT2 segment, const DPOINT2 point)
{
  return d2_closest_point_on_segment_from_point(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, point.x, point.y);
}

CC_INLINE DPOINT3 pt3_closest_point_on_segment_from_point(const DSEGMENT3 segment, const DPOINT3 point)
{
  return d3_closest_point_on_segment_from_point(segment.p0.x, segment.p0.y, segment.p0.z, segment.p1.x, segment.p1.y, segment.p1.z, point.x, point.y, point.z);
}

CC_INLINE DPOINT2 pt2_closest_point_on_line_from_point(const DLINE2 line, const DPOINT2 point)
{
  return d2_closest_point_on_line_from_point(line.p0.x, line.p0.y, line.p1.x, line.p1.y, point.x, point.y);
}

CC_INLINE DPOINT3 pt3_closest_point_on_line_from_point(const DLINE3 line, const DPOINT3 point)
{
  return d3_closest_point_on_line_from_point(line.p0.x, line.p0.y, line.p0.z, line.p1.x, line.p1.y, line.p1.z, point.x, point.y, point.z);
}

CC_INLINE DPOINT2 pt2_closest_point_on_ray_from_point(const DRAY2 ray, const DPOINT2 point)
{
  return d2_closest_point_on_ray_from_point(ray.origin.x, ray.origin.y, ray.direction.x, ray.direction.y, point.x, point.y);
}

CC_INLINE DPOINT3 pt3_closest_point_on_ray_from_point(const DRAY3 ray, const DPOINT3 point)
{
  return d3_closest_point_on_ray_from_point(ray.origin.x, ray.origin.y, ray.origin.z, ray.direction.x, ray.direction.y, ray.direction.z, point.x, point.y, point.z);
}

CC_INLINE DPOINT2 d2_closest_point_on_triangle_from_point(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T px, const T py)
{
  DPOINT2 pt;
  int or1, or2, or3;
  or1 = d2_orientation((x2 + x3) * (0.5), (y2 + y3) * (0.5), x1, y1, px, py);
  or2 = d2_orientation((x1 + x3) * (0.5), (y1 + y3) * (0.5), x2, y2, px, py);

  if (d2_differing_orientation(x1, y1, x2, y2, px, py, x3, y3) && ((or1 * or2) == -1)) {
    pt = d2_closest_point_on_segment_from_point(x1, y1, x2, y2, px, py);
    return pt;
  }

  or3 = d2_orientation((x1 + x2) * (0.5), (y1 + y2) * (0.5), x3, y3, px, py);

  if (d2_differing_orientation(x2, y2, x3, y3, px, py, x1, y1) && ((or2 * or3) == -1)) {
    pt = d2_closest_point_on_segment_from_point(x2, y2, x3, y3, px, py);
    return pt;
  }

  if (d2_differing_orientation(x3, y3, x1, y1, px, py, x2, y2) && ((or3 * or1) == -1)) {
    pt = d2_closest_point_on_segment_from_point(x3, y3, x1, y1, px, py);
    return pt;
  }

  pt.x = px;
  pt.y = py;
  return pt;
}

CC_INLINE DPOINT2 pt2_closest_point_on_triangle_from_point(const DTRIANGLE2 triangle, const DPOINT2 point)
{
  return d2_closest_point_on_triangle_from_point(triangle.p0.x, triangle.p0.y,  triangle.p1.x, triangle.p1.y,  triangle.p2.x, triangle.p2.y,  point.x, point.y);
}

CC_INLINE DPOINT3 pt3_closest_point_on_triangle_from_point(const DTRIANGLE3 triangle, const DPOINT3 point)
{
  DVECTOR3 n;
  T u, v, w, va, vb, vc, snom, sdenom, tnom, tdenom, unom, udenom;
  DVECTOR3 ab = v3_sub(triangle.p1, triangle.p0);
  DVECTOR3 ac = v3_sub(triangle.p2, triangle.p0);
  DVECTOR3 bc = v3_sub(triangle.p2, triangle.p1);

  snom = v3_dot_product(v3_sub(point, triangle.p0), ab);
  sdenom = v3_dot_product(v3_sub(point, triangle.p1), v3_sub(triangle.p0, triangle.p1));

  tnom = v3_dot_product(v3_sub(point, triangle.p0), ac);
  tdenom = v3_dot_product(v3_sub(point, triangle.p2), v3_sub(triangle.p0, triangle.p2));

  if (snom <= (0.0) && tnom <= (0.0)) {
    return triangle.p0;
  }

  unom = v3_dot_product(v3_sub(point, triangle.p1), bc);
  udenom = v3_dot_product(v3_sub(point, triangle.p2), v3_sub(triangle.p1, triangle.p2));

  if ((sdenom <= (0.0)) && (unom <= (0.0))) {
    return triangle.p1;
  }

  if ((tdenom <= (0.0)) && (udenom <= (0.0))) {
    return triangle.p2;
  }

  n = v3_mul(v3_sub(triangle.p1, triangle.p0), v3_sub(triangle.p2, triangle.p0));
  vc = v3_dot_product(n, v3_mul(v3_sub(triangle.p0, point), v3_sub(triangle.p1, point)));

  if ((vc <= (0.0)) && (snom >= (0.0)) && (sdenom >= (0.0))) {
    return v3_add(triangle.p0, v3_scale(ab, snom / (snom + sdenom)));
  }

  va = v3_dot_product(n, v3_mul(v3_sub(triangle.p1, point), v3_sub(triangle.p2, point)));

  if (va <= (0.0) && unom >= (0.0) && udenom >= (0.0)) {
    return v3_add(triangle.p1, v3_scale(bc, unom / (unom + udenom)));
  }

  vb = v3_dot_product(n, v3_mul(v3_sub(triangle.p2, point), v3_sub(triangle.p0, point)));

  if ((vb <= (0.0)) && (tnom >= (0.0)) && (tdenom >= (0.0))) {
    return v3_add(triangle.p0, v3_scale(ac, tnom / (tnom + tdenom)));
  }

  u = va / (va + vb + vc);
  v = vb / (va + vb + vc);
  w = (1.0) - u - v;
  return v3_add3(v3_scale(triangle.p0, u), v3_scale(triangle.p1, v), v3_scale(triangle.p2, w));
}

CC_INLINE DPOINT3 d3_closest_point_on_triangle_from_point(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3,
    const T px, const T py, const T pz)
{
  return pt3_closest_point_on_triangle_from_point(d3_make_triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3), d3_make_point(px, py, pz));
}

CC_INLINE DPOINT2 d2_closest_point_on_rectangle_from_point(const T x1, const T y1, const T x2, const T y2, const T px, const T py)
{
  DPOINT2 pt;

  if (px < min(x1, x2)) {
    pt.x = min(x1, x2);
  }
  else if (px > max(x1, x2)) {
    pt.x = max(x1, x2);
  }
  else {
    pt.x = px;
  }

  if (py < min(y1, y2)) {
    pt.y = min(y1, y2);
  }
  else if (py > max(y1, y2)) {
    pt.y = max(y1, y2);
  }
  else {
    pt.y = py;
  }

  return pt;
}

CC_INLINE DPOINT2 pt2_closest_point_on_rectangle_from_point(const DRECTANGLE rectangle, const DPOINT2 point)
{
  return d2_closest_point_on_rectangle_from_point(rectangle.p0.x, rectangle.p0.y, rectangle.p1.x, rectangle.p1.y, point.x, point.y);
}

CC_INLINE DPOINT3 d3_closest_point_on_box_from_point(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2, const T px, const T py, const T pz)
{
  DPOINT3 pt;

  if (px < min(x1, x2)) {
    pt.x = min(x1, x2);
  }
  else if (px > max(x1, x2)) {
    pt.x = max(x1, x2);
  }
  else {
    pt.x = px;
  }

  if (py < min(y1, y2)) {
    pt.y = min(y1, y2);
  }
  else if (py > max(y1, y2)) {
    pt.y = max(y1, y2);
  }
  else {
    pt.y = py;
  }

  if (pz < min(z1, z2)) {
    pt.z = min(z1, z2);
  }
  else if (pz > max(z1, z2)) {
    pt.z = max(z1, z2);
  }
  else {
    pt.z = pz;
  }

  return pt;
}

CC_INLINE DPOINT3 pt3_closest_point_on_box_from_point(const DBOX3 box, const DPOINT3 point)
{
  return d3_closest_point_on_box_from_point(box.p0.x, box.p0.y, box.p0.z, box.p1.x, box.p1.y, box.p1.z, point.x, point.y, point.z);
}

CC_INLINE DPOINT2 d2_closest_point_on_quadix_from_point(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4, const T px, const T py)
{
  DPOINT2 pt, t;
  T temp_dist, min_dist;

  pt.x = px;
  pt.y = py;

  if (d2_point_in_quadix(px, py, x1, y1, x2, y2, x3, y3, x4, y4)) {
    return pt;
  }

  pt = d2_closest_point_on_segment_from_point(x1, y1, x2, y2, px, py);
  min_dist = d2_distance(pt.x, pt.y, px, py);

  t = d2_closest_point_on_segment_from_point(x2, y2, x3, y3, px, py);
  temp_dist = d2_distance(t.x, t.y, px, py);

  if (min_dist > temp_dist) {
    min_dist = temp_dist;
    pt.x = t.x;
    pt.y = t.y;
  }

  t = d2_closest_point_on_segment_from_point(x3, y3, x4, y4, px, py);
  temp_dist = d2_distance(t.x, t.y, px, py);

  if (min_dist > temp_dist) {
    min_dist = temp_dist;
    pt.x = t.x;
    pt.y = t.y;
  }

  t = d2_closest_point_on_segment_from_point(x4, y4, x1, y1, px, py);
  temp_dist = d2_distance(t.x, t.y, px, py);

  if (min_dist > temp_dist) {
    pt.x = t.x;
    pt.y = t.y;
  }

  return pt;
}

CC_INLINE DPOINT2 pt2_closest_point_on_quadix_from_point(const DQUADIX2 quadix, const DPOINT2 point)
{
  return d2_closest_point_on_quadix_from_point(quadix.p0.x, quadix.p0.y, quadix.p1.x, quadix.p1.y,
      quadix.p2.x, quadix.p2.y, quadix.p3.x, quadix.p3.y, point.x, point.y);
}

CC_INLINE DPOINT2 pt2_closest_point_on_circle_from_point(const DCIRCLE circle, const DPOINT2 point)
{
  DPOINT2 point_;
  T dx, dy, ratio;
  dx = point.x - circle.x;
  dy = point.y - circle.y;

  if ((sqr(dx) + sqr(dy)) <= sqr(circle.radius)) {
    return point;
  }

  ratio = circle.radius / sqrt(dx * dx + dy * dy);
  point_.x = circle.x + ratio * dx;
  point_.y = circle.y + ratio * dy;
  return point_;
}

CC_INLINE DPOINT3 pt3_closest_point_on_sphere_from_point(const DSPHERE sphere, const DPOINT3 point)
{
  DPOINT3 point_;
  T dx = point.x - sphere.x;
  T dy = point.y - sphere.y;
  T dz = point.z - sphere.z;
  T ratio = sphere.radius / sqrt(dx * dx + dy * dy);
  point_.x = sphere.x + ratio * dx;
  point_.y = sphere.y + ratio * dy;
  point_.z = sphere.z + ratio * dz;
  return point_;
}

CC_INLINE DPOINT2 pt2_closest_point_on_aabbb_from_point(const DRECTANGLE rectangle, const DPOINT2 point)
{
  return pt2_closest_point_on_rectangle_from_point(rectangle, point);
}

CC_INLINE DPOINT2 pt2_closest_point_on_circle_from_segment(const DCIRCLE circle, const DSEGMENT2 segment)
{
  DPOINT2 point, n;
  T ratio;
  n = d2_closest_point_on_segment_from_point(segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y, circle.x, circle.y);
  ratio = circle.radius / d2_distance(circle.x, circle.y, n.x, n.y);
  point.x = circle.x + ratio * (n.x - circle.x);
  point.y = circle.y + ratio * (n.y - circle.y);
  return point;
}

CC_INLINE DPOINT3 pt3_closest_point_on_sphere_from_segment(const DSPHERE sphere,
    const DSEGMENT3 segment)
{
  DPOINT3 n, point;
  T ratio;
  n = d3_closest_point_on_segment_from_point(segment.p0.x, segment.p0.y, segment.p0.z,
      segment.p1.x, segment.p1.y, segment.p1.z, sphere.x, sphere.y, sphere.z);
  ratio = sphere.radius / d2_distance(sphere.x, sphere.y, n.x, n.y);
  point.x = sphere.x + ratio * (n.x - sphere.x);
  point.y = sphere.y + ratio * (n.y - sphere.y);
  point.z = sphere.z + ratio * (n.z - sphere.z);
  return point;
}

CC_INLINE DPOINT3 pt3_closest_point_on_plane_from_point(const DPLANE3 plane,
    const DPOINT3 point)
{
  T mu = plane.normal.x * point.x +
      plane.normal.y * point.y +
      plane.normal.z * point.z - plane.constant;

  if (d_is_equal(mu, (0.0))) {
    return point;
  }
  else
    return d3_make_point(point.x - mu * plane.normal.x,
        point.y - mu * plane.normal.y,
        point.z - mu * plane.normal.z);
}

CC_INLINE DPOINT2 pt2_closest_point_on_qbezier_from_point(const DQUADRATIC_BEZIER2 bezier,
    const DPOINT2 point,
    const int steps)
{
  int i;
  typedef DPOINT2 point_type;
  T smallest_distance = +InfinityT;
  point_type closest_point = pt2_degenerate();
  point_type* point_list = MALLOC(point_type, steps);
  pt2_generate_qbezier(bezier, point_list, steps);

  for (i = 0; i < (steps - 1); ++i) {
    point_type current_point = pt2_closest_point_on_segment_from_point(pt2_make_segment(point_list[i], point_list[i + 1]), point);
    T current_distance = pt2_distance(current_point, point);

    if (current_distance < smallest_distance) {
      closest_point = current_point;
      smallest_distance = current_distance;
    }
  }

  FREE(point_list);
  return closest_point;
}

CC_INLINE DPOINT2 pt2_closest_point_on_cbezier_from_point(const DCUBIC_BEZIER2 bezier,
    const DPOINT2 point,
    const int steps)
{
  int i;
  typedef DPOINT2 point_type;
  T smallest_distance = +InfinityT;
  point_type closest_point = pt2_degenerate();
  point_type* point_list = MALLOC(point_type, steps);
  pt2_generate_cbezier(bezier, (point_list), steps);

  for (i = 0; i < (steps - 1); ++i) {
    point_type current_point = pt2_closest_point_on_segment_from_point(pt2_make_segment(point_list[i], point_list[i + 1]), point);
    T current_distance = pt2_distance(current_point, point);

    if (current_distance < smallest_distance) {
      closest_point = current_point;
      smallest_distance = current_distance;
    }
  }

  FREE(point_list);
  return closest_point;
}

CC_INLINE DPOINT3 pt3_closest_point_on_qbezier_from_point(const DQUADRATIC_BEZIER3 bezier,
    const DPOINT3 point,
    const int steps)
{
  int i;
  typedef DPOINT3 point_type;
  T smallest_distance = +InfinityT;
  point_type closest_point = pt3_degenerate();
  point_type* point_list = MALLOC(point_type, steps);
  pt3_generate_qbezier(bezier, (point_list), steps);

  for (i = 0; i < (steps - 1); ++i) {
    point_type current_point = pt3_closest_point_on_segment_from_point(pt3_make_segment(point_list[i], point_list[i + 1]), point);
    T current_distance = pt3_distance(current_point, point);

    if (current_distance < smallest_distance) {
      closest_point = current_point;
      smallest_distance = current_distance;
    }
  }

  FREE(point_list);
  return closest_point;
}

CC_INLINE DPOINT3 pt3_closest_point_on_cbezier_from_point(const DCUBIC_BEZIER3 bezier,
    const DPOINT3 point,
    const int steps)
{
  int i;
  typedef DPOINT3 point_type;
  T smallest_distance = +InfinityT;
  point_type closest_point = pt3_degenerate();
  point_type* point_list = MALLOC(point_type, steps);
  pt3_generate_cbezier(bezier, (point_list), steps);

  for (i = 0; i < (steps - 1); ++i) {
    point_type current_point = pt3_closest_point_on_segment_from_point(pt3_make_segment(point_list[i], point_list[i + 1]), point);
    T current_distance = pt3_distance(current_point, point);

    if (current_distance < smallest_distance) {
      closest_point = current_point;
      smallest_distance = current_distance;
    }
  }

  FREE(point_list);
  return closest_point;
}

CC_INLINE DPOINT2 pt2_closest_point_on_circle_from_circle(const DCIRCLE circle1, const DCIRCLE circle2)
{
  return pt2_closest_point_on_circle_from_point(circle1, pt2_closest_point_on_circle_from_point(circle2, ci_make_point(circle1)));
}

CC_INLINE DPOINT3 pt3_closest_point_on_sphere_from_sphere(const DSPHERE sphere1, const DSPHERE sphere2)
{
  return pt3_closest_point_on_sphere_from_point(sphere1, pt3_closest_point_on_sphere_from_point(sphere2, sp_make_point(sphere1)));
}

CC_INLINE DPOINT2 pt2_closest_point_on_polygon_from_point(int n, const DPOINT2* polygon, const DPOINT2 point)
{
  const DPOINT2* p = polygon;
  int i, j = 0;

  if (n < 3) {
    return pt2_degenerate();
  }

  if (po2_point_in_polygon(point, n, polygon)) {
    return point;
  }

  {
    T min_dist = +InfinityT;
    DPOINT2 closest_point = pt2_degenerate();
    j = n - 1;

    for (i = 0; i < n; ++i) {
      DPOINT2 curr_point = pt2_closest_point_on_segment_from_point(pt2_make_segment(p[i], p[j]), point);
      T curr_dist = distance(point, curr_point);

      if (curr_dist < min_dist) {
        closest_point = curr_point;
        min_dist = curr_dist;
      }

      j = i;
    }

    return closest_point;
  }
}

CC_INLINE T d2_minimum_distance_from_point_to_segment(const T px, const T py, const T x1, const T y1, const T x2, const T y2)
{
  DPOINT2 n = d2_closest_point_on_segment_from_point(x1, y1, x2, y2, px, py);
  return d2_distance(px, py, n.x, n.y);
}

CC_INLINE T d3_minimum_distance_from_point_to_segment(const T px, const T py, const T pz,
    const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2)
{
  DPOINT3 n = d3_closest_point_on_segment_from_point(x1, y1, z1, x2, y2, z2, px, py, pz);
  return d3_distance(px, py, pz, n.x, n.y, n.z);
}

CC_INLINE T pt2_minimum_distance_from_point_to_segment(const DPOINT2 point, const DSEGMENT2 segment)
{
  return d2_minimum_distance_from_point_to_segment(point.x, point.y, segment.p0.x, segment.p0.y, segment.p1.x, segment.p1.y);
}

CC_INLINE T pt3_minimum_distance_from_point_to_segment(const DPOINT3 point, const DSEGMENT3 segment)
{
  return d3_minimum_distance_from_point_to_segment(point.x, point.y, point.z,
      segment.p0.x, segment.p0.y, segment.p0.z,
      segment.p1.x, segment.p1.y, segment.p1.z);
}

CC_INLINE T d2_minimum_distance_from_point_to_line(const T px, const T py, const T x1, const T y1, const T x2, const T y2)
{
  DPOINT2 n = d2_closest_point_on_line_from_point(x1, y1, x2, y2, px, py);
  return d2_distance(px, py, n.x, n.y);
}

CC_INLINE T d3_minimum_distance_from_point_to_line(const T px, const T py, const T pz,
    const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2)
{
  DPOINT3 n = d3_closest_point_on_line_from_point(x1, y1, z1, x2, y2, z2, px, py, pz);
  return d3_distance(px, py, pz, n.x, n.y, n.z);
}

CC_INLINE T pt2_minimum_distance_from_point_to_line(const DPOINT2 point, const DLINE2 line)
{
  return d2_minimum_distance_from_point_to_line(line.p0.x, line.p0.y,
      line.p1.x, line.p1.y,
      point.x, point.y);
}

CC_INLINE T pt3_minimum_distance_from_point_to_line(const DPOINT3 point, const DLINE3 line)
{
  return d3_minimum_distance_from_point_to_line(line.p0.x, line.p0.y, line.p0.z,
      line.p1.x, line.p1.y, line.p1.z,
      point.x, point.y, point.z);
}

CC_INLINE T d2_minimum_distance_from_point_to_triangle(const T px, const T py,
    const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3)
{
  DPOINT2 n = d2_closest_point_on_triangle_from_point(x1, y1, x2, y2, x3, y3, px, py);
  return d2_distance(px, py, n.x, n.y);
}

CC_INLINE T pt2_minimum_distance_from_point_to_triangle(const DPOINT2 point, const DTRIANGLE2 triangle)
{
  return d2_minimum_distance_from_point_to_triangle(point.x, point.y,
      triangle.p0.x, triangle.p0.y,
      triangle.p1.x, triangle.p1.y,
      triangle.p2.x, triangle.p2.y);
}

CC_INLINE T d2_minimum_distance_from_point_to_rectangle(const T px, const T py,
    const T x1, const T y1,
    const T x2, const T y2)
{
  DPOINT2 n = d2_closest_point_on_rectangle_from_point(x1, y1, x2, y2, px, py);
  return d2_distance(px, py, n.x, n.y);
}

CC_INLINE T pt2_minimum_distance_from_point_to_rectangle(const DPOINT2 point, const DRECTANGLE rectangle)
{
  return d2_minimum_distance_from_point_to_rectangle(rectangle.p0.x, rectangle.p0.y,
      rectangle.p1.x, rectangle.p1.y,
      point.x, point.y);
}

