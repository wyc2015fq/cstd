
CC_INLINE DPOINT2 d2_project_point_t(const T srcx, const T srcy, const T destx, const T desty, const T t)
{
  DPOINT2 pt;
  pt.x = srcx + t * (destx - srcx);
  pt.y = srcy + t * (desty - srcy);
  return pt;
}

CC_INLINE DPOINT3 d3_project_point_t(const T srcx, const T srcy, const T srcz, const T destx, const T desty, const T destz, const T t)
{
  DPOINT3 pt;
  pt.x = srcx + t * (destx - srcx);
  pt.y = srcy + t * (desty - srcy);
  pt.z = srcz + t * (destz - srcz);
  return pt;
}

CC_INLINE DPOINT2 d2_project_point(const T srcx, const T srcy, const T destx, const T desty, const T dist)
{
  T t = dist / d2_distance(srcx, srcy, destx, desty);
  return d2_project_point_t(srcx, srcy, destx, desty, t);
}

CC_INLINE DPOINT3 d3_project_point(const T srcx, const T srcy, const T srcz, const T destx, const T desty, const T destz, const T dist)
{
  T t = dist / d3_distance(srcx, srcy, srcz, destx, desty, destz);
  return d3_project_point_t(srcx, srcy, srcz, destx, desty, destz, t);
}

CC_INLINE DPOINT2 d2_project_point_angle(const T px, const T py, const T angle, const T distance)
{
  DPOINT2 pt;
  T dx = (0.0);
  T dy = (0.0);

  switch (d1_quadrant(angle)) {
  case 1 : {
    dx = (cos(angle * (PIDiv180))) * distance;
    dy = (sin(angle * (PIDiv180))) * distance;
  }
  break;

  case 2 : {
    dx = (sin((angle - (90.0)) * (PIDiv180))) * distance * (-1.0);
    dy = (cos((angle - (90.0)) * (PIDiv180))) * distance;
  }
  break;

  case 3 : {
    dx = (cos((angle - (180.0)) * (PIDiv180))) * distance * (-1.0);
    dy = (sin((angle - (180.0)) * (PIDiv180))) * distance * (-1.0);
  }
  break;

  case 4 : {
    dx = (sin((angle - (270.0)) * (PIDiv180))) * distance;
    dy = (cos((angle - (270.0)) * (PIDiv180))) * distance * (-1.0);
  }
  break;
  }

  pt.x = px + dx;
  pt.y = py + dy;
  return pt;
}

CC_INLINE DPOINT2 d2_project_point0(const T px, const T py, const T distance)
{
  DPOINT2 pt;
  pt.x = px + distance;
  pt.y = py;
  return pt;
}

CC_INLINE DPOINT2 d2_project_point45(const T px, const T py, const T distance)
{
  DPOINT2 pt;
  pt.x = px + (0.70710678118654752440084436210485) * distance;
  pt.y = py + (0.70710678118654752440084436210485) * distance;
  return pt;
}

CC_INLINE DPOINT2 d2_project_point90(const T px, const T py, const T distance)
{
  DPOINT2 pt;
  pt.x = px;
  pt.y = py + distance;
  return pt;
}

CC_INLINE DPOINT2 d2_project_point135(const T px, const T py, const T distance)
{
  DPOINT2 pt;
  pt.x = px - (0.70710678118654752440084436210485) * distance;
  pt.y = py + (0.70710678118654752440084436210485) * distance;
  return pt;
}

CC_INLINE DPOINT2 d2_project_point180(const T px, const T py, const T distance)
{
  DPOINT2 pt;
  pt.x = px - distance;
  pt.y = py;
  return pt;
}

CC_INLINE DPOINT2 d2_project_point225(const T px, const T py, const T distance)
{
  DPOINT2 pt;
  pt.x = px - (0.70710678118654752440084436210485) * distance;
  pt.y = py - (0.70710678118654752440084436210485) * distance;
  return pt;
}

CC_INLINE DPOINT2 d2_project_point270(const T px, const T py, const T distance)
{
  DPOINT2 pt;
  pt.x = px;
  pt.y = py - distance;
  return pt;
}

CC_INLINE DPOINT2 d2_project_point315(const T px, const T py, const T distance)
{
  DPOINT2 pt;
  pt.x = px + (0.70710678118654752440084436210485) * distance;
  pt.y = py - (0.70710678118654752440084436210485) * distance;
  return pt;
}

CC_INLINE DPOINT2 pt2_project_point_t(const DPOINT2 source_point, const DPOINT2 destination_point, const T t)
{
  return d2_project_point_t(source_point.x, source_point.y, destination_point.x, destination_point.y, t);
}

CC_INLINE DPOINT3 pt3_project_point_t(const DPOINT3 source_point, const DPOINT3 destination_point, const T t)
{
  return d3_project_point_t(source_point.x, source_point.y, source_point.z, destination_point.x, destination_point.y, destination_point.z, t);
}

CC_INLINE DPOINT2 pt2_project_point(const DPOINT2 source_point, const DPOINT2 destination_point, const T distance)
{
  return d2_project_point(source_point.x, source_point.y, destination_point.x, destination_point.y, distance);
}

CC_INLINE DPOINT3 pt3_project_point(const DPOINT3 source_point, const DPOINT3 destination_point, const T distance)
{
  return d3_project_point(source_point.x, source_point.y, source_point.z, destination_point.x, destination_point.y, destination_point.z, distance);
}

CC_INLINE DPOINT2 pt2_project_point_angle(const DPOINT2 point, const T angle, const T distance)
{
  return d2_project_point_angle(point.x, point.y, angle, distance);
}

CC_INLINE DPOINT2 pt2_project_point0(const DPOINT2 point, const T distance)
{
  return d2_project_point0(point.x, point.y, distance);
}

CC_INLINE DPOINT2 pt2_project_point45(const DPOINT2 point, const T distance)
{
  return d2_project_point45(point.x, point.y, distance);
}

CC_INLINE DPOINT2 pt2_project_point90(const DPOINT2 point, const T distance)
{
  return d2_project_point90(point.x, point.y, distance);
}

CC_INLINE DPOINT2 pt2_project_point135(const DPOINT2 point, const T distance)
{
  return d2_project_point135(point.x, point.y, distance);
}

CC_INLINE DPOINT2 pt2_project_point180(const DPOINT2 point, const T distance)
{
  return d2_project_point180(point.x, point.y, distance);
}

CC_INLINE DPOINT2 pt2_project_point225(const DPOINT2 point, const T distance)
{
  return d2_project_point225(point.x, point.y, distance);
}

CC_INLINE DPOINT2 pt2_project_point270(const DPOINT2 point, const T distance)
{
  return d2_project_point270(point.x, point.y, distance);
}

CC_INLINE DPOINT2 pt2_project_point315(const DPOINT2 point, const T distance)
{
  return d2_project_point315(point.x, point.y, distance);
}

