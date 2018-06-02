
BOOL point_comparator(int p1, int p2, int anchor, const DPOINT2* pt, T* angle)
{
  if (angle[p1] < angle[p2]) {
    return true;
  }
  else if (angle[p1] > angle[p2]) {
    return false;
  }
  else if (pt2_is_equal(pt[p1], pt[p2])) {
    return false;
  }
  else if (pt2_lay_distance(pt[anchor], pt[p1]) < pt2_lay_distance(pt[anchor], pt[p2])) {
    return true;
  }
  else {
    return false;
  }
}

int ordered_polygon(int n, const DPOINT2* point, DPOINT2* out)
{
  int i, j = 0, miny_index;
  int* it = 0;
  T* angle = 0;

  for (i = 1; i < n; ++i) {
    if (point[i].y < point[j].y) {
      j = i;
    }
    else if (point[i].y == point[j].y) {
      if (point[i].x < point[j].x) {
        j = i;
      }
    }
  }

  miny_index = j;
  it = MALLOC(int, n);
  angle = MALLOC(T, n);
  it[0] = miny_index;

  j = 1;

  for (i = 0; i < n; ++i) {
    if (i != miny_index) {
      it[j++] = i;
    }

    angle[i] = pt2_cartesian_angle_o(point[i], point[miny_index]);
  }

#define GS_POINT_COMPARATOR(a, b) point_comparator(a, b, miny_index, point, angle)
  QSORT((it + 1), it + n, GS_POINT_COMPARATOR, int);
#undef GS_POINT_COMPARATOR

  for (i = 0; i < n; ++i) {
    out[i] = point[it[i]];
  }

  return 0;
}

