
CC_INLINE DRECTANGLE se2_aabb(const DSEGMENT2 segment)
{
  DRECTANGLE rectangle_;

  if (segment.p0.x < segment.p1.x) {
    rectangle_.p0.x = segment.p0.x;
    rectangle_.p1.x = segment.p1.x;
  }
  else {
    rectangle_.p0.x = segment.p1.x;
    rectangle_.p1.x = segment.p0.x;
  }

  if (segment.p0.y < segment.p1.y) {
    rectangle_.p0.y = segment.p0.y;
    rectangle_.p1.y = segment.p1.y;
  }
  else {
    rectangle_.p0.y = segment.p1.y;
    rectangle_.p1.y = segment.p0.y;
  }

  return rectangle_;
}

CC_INLINE DRECTANGLE tr2_aabb(const DTRIANGLE2 triangle)
{
  int i;
  DRECTANGLE rectangle_;
  const DPOINT2* p = &triangle.p0;
  rectangle_.p0.x = triangle.p0.x;
  rectangle_.p0.y = triangle.p0.y;
  rectangle_.p1.x = triangle.p0.x;
  rectangle_.p1.y = triangle.p0.y;

  for (i = 1; i < TrianglePointCount; ++i) {
    if (p[i].x < rectangle_.p0.x) {
      rectangle_.p0.x = p[i].x;
    }
    else if (p[i].x > rectangle_.p1.x) {
      rectangle_.p1.x = p[i].x;
    }

    if (p[i].y < rectangle_.p0.y) {
      rectangle_.p0.y = p[i].y;
    }
    else if (p[i].y > rectangle_.p1.y) {
      rectangle_.p1.y = p[i].y;
    }
  }

  return rectangle_;
}

CC_INLINE DRECTANGLE rt2_aabb(const DRECTANGLE rectangle)
{
  DRECTANGLE rectangle_;
  rectangle_.p0.x = min(rectangle.p0.x, rectangle.p1.x);
  rectangle_.p0.y = min(rectangle.p0.y, rectangle.p1.y);
  rectangle_.p1.x = max(rectangle.p0.x, rectangle.p1.x);
  rectangle_.p1.y = max(rectangle.p0.y, rectangle.p1.y);
  return rectangle_;
}

CC_INLINE DRECTANGLE qu2_aabb(const DQUADIX2 quadix)
{
  int i;
  DRECTANGLE rectangle_;
  const DPOINT2* p = &quadix.p0;
  rectangle_.p0.x = quadix.p0.x;
  rectangle_.p0.y = quadix.p0.y;
  rectangle_.p1.x = quadix.p0.x;
  rectangle_.p1.y = quadix.p0.y;

  for (i = 1; i < QuadixPointCount; ++i) {
    if (p[i].x < rectangle_.p0.x) {
      rectangle_.p0.x = p[i].x;
    }
    else if (p[i].x > rectangle_.p1.x) {
      rectangle_.p1.x = p[i].x;
    }

    if (p[i].y < rectangle_.p0.y) {
      rectangle_.p0.y = p[i].y;
    }
    else if (p[i].y > rectangle_.p1.y) {
      rectangle_.p1.y = p[i].y;
    }
  }

  return rectangle_;
}

CC_INLINE DRECTANGLE ci2_aabb(const DCIRCLE circle)
{
  return d2_make_rectangle(circle.x - circle.radius, circle.y - circle.radius,
      circle.x + circle.radius, circle.y + circle.radius);
}

CC_INLINE DRECTANGLE po2_aabb(const DPOLYGON2* polygon)
{
  int i;
  DRECTANGLE rectangle_;

  if (polygon->n < 3) {
    return d2_make_rectangle((0.0), (0.0), (0.0), (0.0));
  }

  rectangle_.p0.x = polygon->p[0].x;
  rectangle_.p0.y = polygon->p[0].y;
  rectangle_.p1.x = polygon->p[0].x;
  rectangle_.p1.y = polygon->p[0].y;

  for (i = 0; i < polygon->n; ++i) {
    if (polygon->p[i].x < rectangle_.p0.x) {
      rectangle_.p0.x = polygon->p[i].x;
    }
    else if (polygon->p[i].x > rectangle_.p1.x) {
      rectangle_.p1.x = polygon->p[i].x;
    }

    if (polygon->p[i].y < rectangle_.p0.y) {
      rectangle_.p0.y = polygon->p[i].y;
    }
    else if (polygon->p[i].y > rectangle_.p1.y) {
      rectangle_.p1.y = polygon->p[i].y;
    }
  }

  return rectangle_;
}

CC_INLINE DBOX3 se3_aabb(const DSEGMENT3 segment)
{
  DBOX3 box_;

  if (segment.p0.x < segment.p1.x) {
    box_.p0.x = segment.p0.x;
    box_.p1.x = segment.p1.x;
  }
  else {
    box_.p0.x = segment.p1.x;
    box_.p1.x = segment.p0.x;
  }

  if (segment.p0.y < segment.p1.y) {
    box_.p0.y = segment.p0.y;
    box_.p1.y = segment.p1.y;
  }
  else {
    box_.p0.y = segment.p1.y;
    box_.p1.y = segment.p0.y;
  }

  if (segment.p0.z < segment.p1.z) {
    box_.p0.z = segment.p0.z;
    box_.p1.z = segment.p1.z;
  }
  else {
    box_.p0.z = segment.p1.z;
    box_.p1.z = segment.p0.z;
  }

  return box_;

}

CC_INLINE DBOX3 ptn3_aabb(const DPOINT3* p, int n)
{
  DBOX3 box_ = {0};

  if (n > 0) {
    int i;
    box_ = pt3_make_box(p[0], p[0]);

    for (i = 1; i < n; ++i) {
      if (p[i].x < box_.p0.x) {
        box_.p0.x = p[i].x;
      }
      else if (p[i].x > box_.p1.x) {
        box_.p1.x = p[i].x;
      }

      if (p[i].y < box_.p0.y) {
        box_.p0.y = p[i].y;
      }
      else if (p[i].y > box_.p1.y) {
        box_.p1.y = p[i].y;
      }

      if (p[i].z < box_.p0.z) {
        box_.p0.z = p[i].z;
      }
      else if (p[i].z > box_.p1.z) {
        box_.p1.z = p[i].z;
      }
    }
  }

  return box_;
}
CC_INLINE DBOX3 tr3_aabb(const DTRIANGLE3 triangle)
{
  return ptn3_aabb(&triangle.p0, TrianglePointCount);
}

CC_INLINE DBOX3 aabb(const DBOX3 box)
{
  DBOX3 box_;
  box_.p0.x = min(box.p0.x, box.p1.x);
  box_.p0.y = min(box.p0.y, box.p1.y);
  box_.p0.z = min(box.p0.z, box.p1.z);
  box_.p1.x = max(box.p0.x, box.p1.x);
  box_.p1.y = max(box.p0.y, box.p1.y);
  box_.p1.z = max(box.p0.z, box.p1.z);
  return box_;
}

CC_INLINE DBOX3 qu3_aabb(const DQUADIX3 quadix)
{
  return ptn3_aabb(&quadix.p0, QuadixPointCount);
}

CC_INLINE DBOX3 sp3_aabb(const DSPHERE sphere)
{
  return d3_make_box(sphere.x - sphere.radius, sphere.y - sphere.radius, sphere.z - sphere.radius,
      sphere.x + sphere.radius, sphere.y + sphere.radius, sphere.z + sphere.radius);
}

CC_INLINE DBOX3 po3_aabb(const DPOLYGON3* polygon)
{
  return ptn3_aabb(polygon->p, polygon->n);
}

