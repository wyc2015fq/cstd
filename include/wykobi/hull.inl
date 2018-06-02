
BOOL pt2_gs_point_comparator(int p1, int p2, int anchor, const DPOINT2* pt, const T* angle)
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

int convex_hull_graham_scan(int n, const DPOINT2* point, DPOINT2* out)
{
  int i, j, miny_index = 0;
  int* it = 0;
  T* angle = 0;

  if (n <= 3) {
    MEMCPY(out, point, n);
    return 0;
  }

  j = 0;

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

#define GS_POINT_COMPARATOR(a, b) pt2_gs_point_comparator(a, b, miny_index, point, angle)
  QSORT((it + 1), it + n, GS_POINT_COMPARATOR, int);
#undef GS_POINT_COMPARATOR

  j = 0;
  out[j++] = point[it[0]];
  out[j++] = point[it[1]];

  i = 2;

  while (i != n) {
    if (j > 1) {
      if (pt2_orientation(out[j - 2], out[j - 1], point[it[i]]) == CounterClockwise) {
        out[j++] = point[it[i++]];
      }
      else {
        --j;
      }
    }
    else {
      out[j++] = point[it[i++]];
    }
  }

  FREE(it);
  FREE(angle);
  return j;
}

int convex_hull_jarvis_march(int n, const DPOINT2* point, DPOINT2* out)
{
  DPOINT2* point_list = 0;
  DPOINT2 lowest_point;
  DPOINT2 previous_point;
  DPOINT2 current_point;
  int i, j, previous_it, point_list_n = 0;

  lowest_point = *point;
  point_list = MALLOC(DPOINT2, n);

  for (i = 1; i < n; ++i) {
    if (point[i].y < lowest_point.y) {
      lowest_point = point[i];
    }
  }

  j = 0;
  point_list[j++] = lowest_point;
  previous_point = lowest_point;

  do {
    current_point = (pt2_not_equal(previous_point, *point)) ? *point : *(point + 1);

    for (i = 0; i < n; ++i) {
      if (pt2_orientation(previous_point, current_point, point[i]) == RightHandSide) {
        current_point = point[i];
      }
    }

    if (pt2_not_equal(current_point, lowest_point)) {
      point_list[j++] = current_point;
    }

    previous_point = current_point;
  }
  while (pt2_not_equal(current_point, lowest_point));

  point_list_n = j;
  /* Remove consecutive collinear points */
  previous_it = point_list_n - 1;

  j = 0;

  for (i = 0; i < (point_list_n - 1); ++i) {
    if (pt2_orientation(point_list[previous_it], point_list[i], point_list[i + 1]) != CollinearOrientation) {
      out[j++] = point_list[i];
      previous_it = i;
    }
  }

  if (pt2_orientation(point_list[previous_it], point_list[point_list_n - 1], point_list[0]) != CollinearOrientation) {
    out[j++] = point_list[point_list_n - 1];
  }

  FREE(point_list);
  return j;
}

int convex_hull_melkman(int n, const DPOINT2* point, DPOINT2* out)
{
  int i, j, point_count = n, deq_beg = 0, deq_end = 0, deq_sz = n, deqn = 0;
  DPOINT2* deq;

  if (point_count <= 3) {
    MEMCPY(out, point, n);
  }

  deq = MALLOC(DPOINT2, n);
#define DEQ_SIZE()   ((deq_end+n-deq_beg)%n)
#define DEQ_PUSH_FRONT(x) (deq[deq_beg=(deq_beg+n-1)%n] = x), assert(deq_beg<n)
#define DEQ_PUSH_BACK(x) (deq[deq_end] = x, deq_end=(deq_end+1)%n), assert(DEQ_SIZE()<n)
#define DEQ_POP_FRONT() deq[deq_beg=(deq_beg+1)%n]
#define DEQ_POP_BACK()  deq[deq_end=(deq_end+n-1)%n]
#define DEQ_BEG(I)   deq[(deq_beg+(I))%n]
#define DEQ_END(I)   deq[(deq_end-(I)+n)%n]
  j = 0;

  if (pt2_orientation(point[0], point[1], point[2]) == LeftHandSide) {
    DEQ_PUSH_FRONT((*(point + 2)));
    DEQ_PUSH_FRONT((*(point + 0)));
    DEQ_PUSH_FRONT((*(point + 1)));
    DEQ_PUSH_FRONT((*(point + 2)));
  }
  else {
    DEQ_PUSH_FRONT((*(point + 2)));
    DEQ_PUSH_FRONT((*(point + 1)));
    DEQ_PUSH_FRONT((*(point + 0)));
    DEQ_PUSH_FRONT((*(point + 2)));
  }

  for (i = 3; i != n; ++i) {
    if ((pt2_orientation(DEQ_END(1), DEQ_END(2), point[i]) == LeftHandSide) &&
        (pt2_orientation(DEQ_BEG(1), DEQ_BEG(0), point[i]) == LeftHandSide)) {
      continue;
    }

    while (pt2_orientation(DEQ_END(1), DEQ_END(2), point[i]) != LeftHandSide) {
      DEQ_POP_BACK();
    }

    DEQ_PUSH_BACK(point[i]);

    while (pt2_orientation(DEQ_BEG(1), DEQ_BEG(0), point[i]) != LeftHandSide) {
      DEQ_POP_FRONT();
    }

    DEQ_PUSH_FRONT(point[i]);
  }

  deqn = DEQ_SIZE();

  if (deq_beg < deq_end) {
    MEMCPY(out, deq + deq_beg, deqn - 1);
  }
  else {
    MEMCPY(out, deq + deq_beg, n - deq_beg);
    MEMCPY(out + n - deq_beg, deq, deq_end);
  }

  FREE(deq);

#undef DEQ_PUSH_FRONT
#undef DEQ_PUSH_BACK
#undef DEQ_POP_FRONT
#undef DEQ_POP_BACK
#undef DEQ_SIZE

  return deqn - 1;
}

