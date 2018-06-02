#include "gpc.inl"
static int poly2polygon(const DPOLYGON* s, gpc_polygon* p)
{
  int c, off = 0;
  p->num_contours = s->n;
  GPC_MALLOC(p->hole, p->num_contours * sizeof(int), "hole flag array creation", int);
  GPC_MALLOC(p->contour, p->num_contours * sizeof(gpc_vertex_list), "contour creation", gpc_vertex_list);
  for (c = 0; c < p->num_contours; c++) {
    p->contour[c].num_vertices = s->len[c];
    GPC_MALLOC(p->contour[c].vertex, p->contour[c].num_vertices
        * sizeof(gpc_vertex), "vertex creation", gpc_vertex);
    MEMCPY(p->contour[c].vertex, s->pt + off, s->len[c]);
    off += s->len[c];
  }
  return 0;
}
static int polygon2poly(const gpc_polygon* p, DPOLYGON* s)
{
  int c, off = 0, npt = 0;
  s->n = p->num_contours;
  REALLOC(int, s->len, s->n);
  for (c = 0; c < p->num_contours; c++) {
    s->len[c] = p->contour[c].num_vertices;
    npt += p->contour[c].num_vertices;
  }
  REALLOC(DPOINT, s->pt, npt);
  for (c = 0; c < p->num_contours; c++) {
    MEMCPY(s->pt + off, p->contour[c].vertex, s->len[c]);
    off += s->len[c];
  }
  return 0;
}
static int dPOLYGON_clip(int opt, const DPOLYGON* s, const DPOLYGON* c, DPOLYGON* r)
{
  gpc_polygon gs[1] = {0}, gc[1] = {0}, gr[1] = {0};
  poly2polygon(s, gs);
  poly2polygon(c, gc);
  gpc_polygon_clip((gpc_op)opt, gs, gc, gr);
  polygon2poly(gr, r);
  gpc_free_polygon(gs);
  gpc_free_polygon(gc);
  gpc_free_polygon(gr);
  return 0;
}
//int tt1=0;
//int tt2=0;
// Bentley-Ottmann扫描线算法求线段交点
static int bentley_ottmann_inter(const DPOINT* pt, const int* ei, int ne, inter_info_t* out)
{
  int* pp = MALLOC(int, 2 * ne);
  char* tt = MALLOC(char, ne);
  int i, j, nt = 0, m = 0;
  DPOINT pt2;
  inter_info_t ii[1] = {0};
  MEMSET(tt, 0, ne);
  for (i = 0; i < 2 * ne; ++i) {
    pp[i] = i;
  }
#define PTXLESS(a, b) (pt[ei[a]].x<pt[ei[b]].x)
  QSORT(pp, pp + 2 * ne, PTXLESS, int);
#undef PTXLESS
  for (i = 0; i < 2 * ne; ++i) {
    int k = pp[i] >> 1;
    int i3 = ei[2 * k], i4 = ei[2 * k + 1];
    if (!tt[k]) {
      for (j = 0; j < ne; ++j) {
        if (tt[j]) {
          int i1 = ei[2 * j], i2 = ei[2 * j + 1];
          if (i1 != i3 && i1 != i4 && i2 != i3 && i2 != i4) {
            int t = !!seg_inter_test(&pt[i1].x, &pt[i2].x, &pt[i3].x, &pt[i4].x, ii);
            pt2 = ii->pt;
            if (out) {
              out[m].pt = pt2;
            }
            m += t;
            //++tt2;
            if (t) {
              //printf("%d %d\n", MIN(j, k), MAX(j, k));
            }
          }
        }
      }
    }
    tt[k] = !tt[k];
  }
  pfree(pp);
  pfree(tt);
  return m;
}
// 线段集合求交
static int segset_inter(const DPOINT* pt, const int* ei, int ne, inter_info_t* out)
{
  int j, k, m = 0;
  DPOINT pt2;
  inter_info_t ii[1] = {0};
  for (j = 0; j < ne; ++j) {
    int i1 = ei[2 * j], i2 = ei[2 * j + 1];
    for (k = 0; k < j; ++k) {
      int i3 = ei[2 * k], i4 = ei[2 * k + 1];
      if (i1 != i3 && i1 != i4 && i2 != i3 && i2 != i4) {
        int t = seg_inter_test(&pt[i1].x, &pt[i2].x, &pt[i3].x, &pt[i4].x, ii);
        pt2 = ii->pt;
        //++tt1;
        if (t) {
          //printf("%d %d\n", MIN(j, k), MAX(j, k));
          if (out) {
            out[m].pt = pt2;
            out[m].flag = t;
          }
        }
        m += !!t;
      }
    }
  }
  return m;
}
static int test_segset_inter()
{
  int ti, i, n = 1000, m1, m2;
  DPOINT* pt = MALLOC(DPOINT, n);
  int* ee = MALLOC(int, 2 * n);
  int tt1 = 0, tt2 = 0;
  for (ti = 0; ti < 100; ++ti) {
    pt_random(n, 1000, 1000, &pt[0].x);
    for (i = 0; i < n; ++i) {
      ee[2 * i + 0] = i;
      ee[2 * i + 1] = NEXT_INDEX(i, n);
    }
    {
      utime_start(_start_time);
      m1 = segset_inter(pt, ee, n, 0);
      printf("%5d %5d %f ", m1, tt1, utime_elapsed(_start_time));
    }
    {
      utime_start(_start_time);
      m2 = bentley_ottmann_inter(pt, ee, n, 0);
      printf("%5d %5d %f\n", m2, tt2, utime_elapsed(_start_time));
    }
    ASSERT(m1 == m2);
  }
  pfree(pt);
  pfree(ee);
  return 0;
}
static int dPOLYGON_info(const DPOLYGON* s, inter_info_t* pp)
{
  int i, ii, k = 0;
  const DPOINT* sp = s->pt;
  for (i = 0; i < s->n; ++i) {
    int sn = s->len[i];
    for (ii = 0; ii < sn; ++ii) {
      if (pp) {
        pp[k].pt = sp[ii];
        //pp[k].next = NEXT_INDEX(ii, sn);
        //pp[k].prev = PREV_INDEX(ii, sn);
      }
      ++k;
    }
    sp += sn;
  }
  return k;
}
static int dPOLYGON_inter(const DPOLYGON* s, const DPOLYGON* c, DPOINT* pt)
{
  int i, j, ii, jj, k = 0;
  const DPOINT* sp = s->pt;
  inter_info_t iii[1] = {0};
  for (i = 0; i < s->n; ++i) {
    int sn = s->len[i];
    for (ii = 0; ii < sn; ++ii) {
      int ni = NEXT_INDEX(ii, sn);
      const DPOINT* cp = c->pt;
      for (j = 0; j < c->n; ++j) {
        int cn = c->len[j];
        for (jj = 0; jj < cn; ++jj) {
          int nj = NEXT_INDEX(jj, cn);
          k += !!seg_inter_test(&sp[ii].x, &sp[ni].x, &cp[jj].x, &cp[nj].x, 0);
          if (pt) {
            line_inter(&sp[ii].x, &sp[ni].x, &cp[jj].x, &cp[nj].x, iii);
            pt[k - 1] = iii->pt;
          }
        }
        cp += cn;
      }
    }
    sp += sn;
  }
  return k;
}
static int dPOLYGON_edge(const DPOLYGON* s, DPOINT* pt, int npt, int* ei, BOOL inv)
{
  int i, ii, k = 0;
  const DPOINT* sp = s->pt;
  for (i = 0; i < s->n; ++i) {
    int sn = s->len[i];
    int npt0 = npt;
    for (ii = 0; ii < sn; ++ii) {
      pt[npt++] = sp[ii];
      ei[2 * k + 0] = npt0 + ii;
      ei[2 * k + 1] = npt0 + (inv ? PREV_INDEX(ii, sn) : NEXT_INDEX(ii, sn));
      ++k;
    }
    sp += sn;
  }
  return k;
}
static int dPOLYGON_edge2(const DPOLYGON* s, inter_info_t* pp, int npp, BOOL inv)
{
  int i, ii, k = 0;
  const DPOINT* sp = s->pt;
  for (i = 0; i < s->n; ++i) {
    int sn = s->len[i];
    int npp0 = npp;
    for (ii = 0; ii < sn; ++ii) {
      pp[npp].pt = sp[ii];
      //pp[npp].next = npp0 + (inv ? PREV_INDEX(ii, sn) : NEXT_INDEX(ii, sn));
      pp[npp].next_inter0 = pp[npp].next_inter1 = npp0 + (inv ? PREV_INDEX(ii, sn) : NEXT_INDEX(ii, sn));
      ++npp;
    }
    sp += sn;
  }
  return npp;
}
//int tt1=0;
//int tt2=0;
// Bentley-Ottmann扫描线算法求线段交点
static int bentley_ottmann_inter2(const DPOINT* pt, const int* ei, int ne1, int ne2, inter_info_t* out, int m)
{
  int ne = ne1 + ne2;
  int* pp = MALLOC(int, 2 * ne);
  char* tt = MALLOC(char, ne);
  int i, j, nt = 0;
  inter_info_t ii[1] = {0};
  MEMSET(tt, 0, ne);
  for (i = 0; i < 2 * ne; ++i) {
    pp[i] = i;
  }
#define PTXLESS(a, b) (pt[ei[a]].x<pt[ei[b]].x)
  QSORT(pp, pp + 2 * ne, PTXLESS, int);
#undef PTXLESS
  for (i = 0; i < 2 * ne; ++i) {
    int k = pp[i] >> 1;
    if (k < ne1) {
      tt[k] = !tt[k];
    }
    else {
      if (!tt[k]) {
        int i3 = ei[2 * k], i4 = ei[2 * k + 1];
        for (j = 0; j < ne1; ++j) {
          if (tt[j]) {
            int i1 = ei[2 * j], i2 = ei[2 * j + 1];
            if (i1 != i3 && i1 != i4 && i2 != i3 && i2 != i4) {
              int t = !!seg_inter_test(&pt[i1].x, &pt[i2].x, &pt[i3].x, &pt[i4].x, ii);
              double* ts = ii->ts;
              DPOINT pt2 = ii->pt;
              //++tt2;
              if (t) {
                if (out) {
                  int t0 = i1, t1 = i3;
                  double ts0 = t < 0 ? ts[0] : ts[1];
                  double ts1 = t < 0 ? ts[1] : ts[0];
                  out[m].ts[0] = ts[0];
                  out[m].ts[1] = ts[1];
                  out[m].pt = pt2;
                  out[m].flag = t;
                  if (11 == m) {
                    int asdf = 0;
                  }
                  if (ts[0] > 0 && ts[0] < 1) {
                  }
                  for (t0 = i1; out[out[t0].next_inter0].flag && out[out[t0].next_inter0].ts[0] < ts[0]; t0 = out[t0].next_inter0) {}
                  out[m].next_inter0 = out[t0].next_inter0, out[t0].next_inter0 = m;
                  if (ts[1] > 0 && ts[1] < 1) {
                  }
                  for (t1 = i3; out[out[t1].next_inter1].flag && out[out[t1].next_inter1].ts[1] < ts[1]; t1 = out[t1].next_inter1) {}
                  out[m].next_inter1 = out[t1].next_inter1, out[t1].next_inter1 = m;
                  //out[m].next = t < 0 ? out[m].next_inter0 : out[m].next_inter1;
                  //out[t < 0 ? t1 : t0].next = m;
                  //out[t0].next=m;
                  //out[t1].next=m;
                  //printf("%d %d %d\n", m, i1, i3);
                  //printf("%f %f\n", ts[0], ts[1]);
                }
                ++m;
              }
            }
          }
        }
      }
    }
  }
  pfree(pp);
  pfree(tt);
  return m;
}
static int segset_inter2(const DPOINT* pt, const int* ei, int ne1, int ne2, inter_info_t* out, int m)
{
  int j, k;
  const int* ei1 = ei, *ei2 = ei + 2 * ne1, m0 = m;
  inter_info_t ii[1] = {0};
  for (j = 0; j < ne1; ++j) {
    int i1 = ei1[2 * j], i2 = ei1[2 * j + 1];
    for (k = 0; k < ne2; ++k) {
      int i3 = ei2[2 * k], i4 = ei2[2 * k + 1];
      //if (i1 != i3 && i1 != i4 && i2 != i3 && i2 != i4)
      seg_inter_test(&pt[i1].x, &pt[i2].x, &pt[i3].x, &pt[i4].x, ii);
#define TESTPT(X) (((X)>0 && (X)<1))
      if (TESTPT(ii->ts[0]) && TESTPT(ii->ts[1])) {
        int asdf = 0;
        //++m;
      }
      //++tt1;
      if (ii->flag) {
        // flag<0 表示 多边形1进入多边形2
        int test;
        int t0 = i1, t1 = i3, mm = m;
#define FEQ(a, b) FISZERO((a)-(b))
        test = (FEQ(ii->ts[0], 0)) | (FEQ(ii->ts[0], 1) << 1) | (FEQ(ii->ts[1], 0) << 2) | (FEQ(ii->ts[1], 1) << 3);
        //ASSERT(ts[0]>=0 && ts[0]<=1.);
        //ASSERT(ts[1]>=0 && ts[1]<=1.);
        //printf("%d %d\n", MIN(j, k), MAX(j, k));
        //if (fabs(ts[0])>=0 && fabs(ts[1])>=0 && fabs(ts[0]-1)>EPS && fabs(ts[1]-1)>EPS)
        if (0 == test) {
          mm = m++;
        }
        else {
          if (1 & test) {
            mm = i1;
          }
          else if (2 & test) {
            mm = i2;
          }
          else if (4 & test) {
            mm = i3;
          }
          else if (8 & test) {
            mm = i4;
          }
        }
        if (out) {
          if (0 == test) {
            out[mm].pt = ii[0].pt;
            out[mm].flag = ii[0].flag;
            out[mm].ts[0] = ii[0].ts[0];
            out[mm].ts[1] = ii[0].ts[1];
            out[mm].d = ii[0].d;
          }
          else {
            if (5 == test || 6 == test || 9 == test || 10 == test) { // 点重合
              int t;
              out[mm].flag += ii[0].flag;
              //out[mm].flag = -1;
              if (5 == test) {
                SWAP(out[i1].next_inter1, out[i3].next_inter0, t);
                out[i3].samp = i1;
              }
              if (10 == test) {
                //out[i3].next_inter1 = out[i1].next_inter0;
              }
            }
            else {
              out[mm].flag = ii[0].flag;
            }
          }
          if (0 == (3 & test)) {
            for (t0 = i1; out[t0].next_inter0 != i2 && out[out[t0].next_inter0].flag && out[out[t0].next_inter0].ts[0] < ii->ts[0]; t0 = out[t0].next_inter0) {
              int asdf = 0;
            }
            if (t0 != mm) {
              out[mm].next_inter0 = out[t0].next_inter0, out[t0].next_inter0 = mm;
            }
          }
          if (0 == (12 & test)) {
            for (t1 = i3; out[t1].next_inter1 != i4 && out[out[t1].next_inter1].flag && out[out[t1].next_inter1].ts[1] < ii->ts[1]; t1 = out[t1].next_inter1) {
              int asdf = 0;
            }
            if (t1 != mm) {
              out[mm].next_inter1 = out[t1].next_inter1, out[t1].next_inter1 = mm;
            }
          }
          //out[m].next = ii->flag < 0 ? out[m].next_inter0 : out[m].next_inter1;
          //out[ii->flag < 0 ? t1 : t0].next = m;
          //out[t0].next=m;
          //out[t1].next=m;
          //printf("%d %d %d\n", m, i1, i3);
          //printf("%f %f\n", ts[0], ts[1]);
          //printf("%3d %3d %3d %3d %3d %15.10f, %15.10f (%15.10f, %15.10f)\n", m, i1, i2, i3, i4, ii->ts[0], ii->ts[1], ii->pt.x, ii->pt.y);
        }
      }
    }
  }
  if (1 == (m - m0) % 2) {
    //ASSERT(0 == (m-m0) % 2);
    //printf("-----------------------\n");
  }
  if (0 && out) {
    int i;
    for (i = 0; i < m; ++i) {
      printf("%3d %3d %3d\n", i, out[i].next_inter0, out[i].next_inter1);
    }
  }
  //printf("=========================\n");
  return m;
}
static int test_segset_inter2()
{
  int ti, i, n = 10, m1;
  DPOINT* pt = MALLOC(DPOINT, n);
  int* ee = MALLOC(int, 2 * n);
  int tt1 = 0, tt2 = 0, n2 = n / 2;
  //int segset_inter2(const DPOINT* pt, const int* ei, int ne1, int ne2, inter_info_t* out, int m);
  for (ti = 0; ti < 100; ++ti) {
    pt_random(n, 3, 3, &pt[0].x);
    for (i = 0; i < n; ++i) {
      ee[2 * i + 0] = i;
      ee[2 * i + 1] = NEXT_INDEX(i, n2) + (n2) * (i / n2);
    }
    {
      //utime_start(_start_time);
      m1 = segset_inter2(pt, ee, n2, n2, 0, 0);
      //printf("%5d %5d %f ", m1, tt1, utime_elapsed(_start_time));
    }
  }
  pfree(pt);
  pfree(ee);
  return 0;
}
static int inter_info_find_contour(const inter_info_t* pp, int np, const char* bl)
{
  int i;
  for (i = 0; i < np; ++i) {
    if (bl[i]) {
      return i;
    }
  }
  return -1;
}
enum { GEO_INT = 0, GEO_A_B = 1, GEO_B_A = 2, GEO_UNION = 3, };
// Weiler－Atherton任意多边形裁剪算法
// 从被裁剪多边形的一个入点开始，碰到入点，沿着被裁剪多边形按顺时针方向搜集顶点序列；
// 当遇到出点时，则沿着裁剪窗口按顺时针方向搜集顶点序列。如此交替地沿着两个多边形的边线行进，直到回到起始点。
static int setnext(int* next, int* prev, int n, int a, int b, int c, int d)
{
  int m = n + 1;
  next[n] = a;
  next[c] = n;
  next[m] = b;
  next[d] = m;
  prev[n] = c;
  prev[b] = m;
  prev[m] = d;
  prev[a] = n;
  return 0;
}
// next_offset下一点的偏移量
static int push_polygon(int n, DPOINT* pt, int* next, int* prev, int p1n, const DPOINT* p1, int next_offset)
{
  int i;
  for (i = 0; i < p1n; ++i) {
    pt[n + i] = p1[i];
    next[n + i] = n + (i + next_offset + p1n) % p1n;
    prev[n + i] = n + (i - next_offset + p1n) % p1n;
  }
  return n + p1n;
}
static int polygon_boolopt(int npt1, int npt2, DPOINT* pt, int next_offset1, int next_offset2, int* next, int* prev, char* flag)
{
  int i, j, k = 0, n1, n2;
  int cc[4];
  double* ts;
  inter_info_t ii[1] = {0};
  inter_info_t ii1[1] = {0};
  inter_info_t ii2[1] = {0};
  n1 = n2 = npt1 + npt2;
  ts = MALLOC(double, 10 * n2);
  memset(flag, 0, n1);
  for (i = 0; i < 10 * n1; ++i) {
    ts[i] = -1;
  }
  for (i = 0; i < npt1; ++i) {
    for (j = npt1; j < n1; ++j) {
      int i0 = i;
      int i1 = (i + next_offset1 + npt1) % npt1;
      int i3 = (i1 + next_offset1 + npt1) % npt1;
      int i2 = (i - next_offset1 + npt1) % npt1;
      int j0 = j;
      int j1 = npt1 + (j - npt1 + next_offset2 + npt2) % npt2;
      int j3 = npt1 + (j1 - npt1 + next_offset2 + npt2) % npt2;
      int j2 = npt1 + (j - npt1 - next_offset2 + npt2) % npt2;
      int u = 0, k = seg_inter_test2(&pt[i].x, &pt[i1].x, &pt[j].x, &pt[j1].x, ii);
      int R0 = (k >> 1) & 15, R1 = (k >> 5) & 1, R2 = (k >> 6) & 15, R = k & 1;
      if (i == 1 && j == 4) {
        int asdf = 0;
      }
      if (k) {
        double d = ii->d;
        flag[n2] = flag[n2 + 1] = 0;
        if (R) {
          if (R0) {
            // 点在点上
            if (1 == R0) {
              ii1->ts[0] = ii2->ts[0] = -1;
              line_inter(&pt[i2].x, &pt[i1].x, &pt[j0].x, &pt[j1].x, ii1);
              line_inter(&pt[i2].x, &pt[i1].x, &pt[j0].x, &pt[j2].x, ii2);
              if ((ii1->ts[0] > 0 && ii1->ts[0] < 1) || (ii2->ts[0] > 0 && ii2->ts[0] < 1)) {
                V4SET(cc, next[i0], next[j0], i0, j0);
                flag[n2] = flag[n2 + 1] = u = (ii1->ts[0] > 0 && ii1->ts[0] < 1) ? 1 : -1;
                //printf("点在点上\n");
              }
            }
          }
          else if (R1) {
            // 跨立实验
            if (R2) {
              double d1, d2, d3, d4, d5;
              // 点在线上
              switch (R2) {
              case 1:
                d1 = V2CROSS(&pt[i1].x, &pt[j0].x, &pt[j1].x);
                d2 = V2CROSS(&pt[i2].x, &pt[j0].x, &pt[j1].x);
                d3 = d1 * d2;
                if (d3 < 0 || FEQ(d3, 0)) {
                  for (; next[j0] != j1 && (ts[next[j0]] < ii->ts[1]); j0 = next[j0]) {
                    int asdf = 0;
                    ASSERT(next[j0] >= n1);
                    ASSERT(ts[next[j0]] >= 0);
                  }
                  V4SET(cc, next[i0], next[j0], i0, j0);
                  flag[n2] = u = d < 0 ? -1 : 1;
                  if (FEQ(d3, 0)) {
                    d4 = V2CROSS(&pt[i2].x, &pt[i1].x, &pt[i0].x);
                    d5 = V2CROSS(&pt[j0].x, &pt[i1].x, &pt[i0].x);
                    flag[n2 + 1] = d4 * d5 < 0 ? 0 : u;
                  }
                  else {
                    flag[n2 + 1] = u;
                  }
                  //printf("点在线上1\n");
                }
                break;
              case 2:
                d1 = V2CROSS(&pt[i0].x, &pt[j0].x, &pt[j1].x);
                d2 = V2CROSS(&pt[i3].x, &pt[j0].x, &pt[j1].x);
                d3 = d1 * d2;
                if (FEQ(d3, 0)) {
                  for (; next[j0] != j1 && (ts[next[j0]] < ii->ts[1]); j0 = next[j0]) {
                    int asdf = 0;
                    ASSERT(next[j0] >= n1);
                    ASSERT(ts[next[j0]] >= 0);
                  }
                  d4 = V2CROSS(&pt[j1].x, &pt[i0].x, &pt[i1].x);
                  d5 = V2CROSS(&pt[i3].x, &pt[i0].x, &pt[i1].x);
                  V4SET(cc, i1, next[j0], prev[i1], j0);
                  flag[n2 + 1] = u = d < 0 ? -1 : 1;
                  flag[n2] = d4 * d5 < 0 ? 0 : u;
                  //printf("点在线上1\n");
                }
                break;
              case 4:
                d1 = V2CROSS(&pt[j1].x, &pt[i0].x, &pt[i1].x);
                d2 = V2CROSS(&pt[j2].x, &pt[i0].x, &pt[i1].x);
                d3 = d1 * d2;
                if (d3 < 0 || FEQ(d3, 0)) {
                  for (; next[i0] != i1 && (ts[next[i0]] < ii->ts[0]); i0 = next[i0]) {
                    int asdf = 0;
                    ASSERT(next[i0] >= n1);
                    ASSERT(ts[next[i0]] >= 0);
                  }
                  V4SET(cc, next[i0], next[j0], i0, j0);
                  flag[n2 + 1] = u = d < 0 ? -1 : 1;
                  if (FEQ(d3, 0)) {
                    d4 = V2CROSS(&pt[j2].x, &pt[j1].x, &pt[j0].x);
                    d5 = V2CROSS(&pt[i0].x, &pt[j1].x, &pt[j0].x);
                    flag[n2] = d4 * d5 < 0 ? 0 : u;
                  }
                  else {
                    flag[n2] = u;
                  }
                  //printf("点在线上2\n");
                }
                break;
              case 8:
                d1 = V2CROSS(&pt[j0].x, &pt[i0].x, &pt[i1].x);
                d2 = V2CROSS(&pt[j3].x, &pt[i0].x, &pt[i1].x);
                d3 = d1 * d2;
                if (FEQ(d3, 0)) {
                  for (; next[i0] != i1 && (ts[next[i0]] < ii->ts[0]); i0 = next[i0]) {
                    int asdf = 0;
                    ASSERT(next[i0] >= n1);
                    ASSERT(ts[next[i0]] >= 0);
                  }
                  d4 = V2CROSS(&pt[i1].x, &pt[j0].x, &pt[j1].x);
                  d5 = V2CROSS(&pt[j3].x, &pt[j0].x, &pt[j1].x);
                  V4SET(cc, next[i0], j1, i0, prev[j1]);
                  flag[n2] = u = d < 0 ? -1 : 1;
                  flag[n2 + 1] = d4 * d5 < 0 ? 0 : u;
                  //printf("点在线上2\n");
                }
                break;
              }
            }
            else {
              d = ii->d;
              if (!FEQ(d, 0)) {
                for (; next[i0] != i1 && (ts[next[i0]] < ii->ts[0]); i0 = next[i0]) {
                  int asdf = 0;
                  ASSERT(next[i0] >= n1);
                  ASSERT(ts[next[i0]] >= 0);
                }
                for (; next[j0] != j1 && (ts[next[j0]] < ii->ts[1]); j0 = next[j0]) {
                  int asdf = 0;
                  ASSERT(next[j0] >= n1);
                  ASSERT(ts[next[j0]] >= 0);
                }
                V4SET(cc, next[i0], next[j0], i0, j0);
                flag[n2] = flag[n2 + 1] = u = d < 0 ? -1 : 1;
              }
            }
          }
        }
      }
      if (u) {
        ASSERT(cc[0] < npt1 || cc[0] >= n1);
        ASSERT(cc[2] < npt1 || cc[2] >= n1);
        ASSERT(cc[1] >= npt1);
        ASSERT(cc[3] >= npt1);
        setnext(next, prev, n2, cc[0], cc[1], cc[2], cc[3]);
        pt[n2 + 1] = pt[n2] = ii->pt;
        V2SETV(ts + n2, ii->ts);
        if (ii->ts[0] < 0 || ii->ts[1] < 0) {
          int asdf = 0;
        }
        ASSERT(ii->ts[0] >= 0 && ii->ts[1] >= 0);
        //flag[n2] = flag[n2 + 1] = u;
        n2 += 2;
      }
    }
  }
  for (i = 0; i < n1; ++i) {
    ASSERT(0 == flag[i]);
  }
  for (i = n1; i < n2; ++i) {
    //ASSERT(-1 == flag[i] || 1 == flag[i]);
  }
  for (i = 0; i < npt1; ++i) {
    int i0 = i;
    int i1 = (i + next_offset1 + npt1) % npt1;
    int i2 = (i - next_offset1 + npt1) % npt1;
    for (; next[i0] != i1; i0 = next[i0]) {
      int asdf = 0;
      ASSERT(next[i0] >= n1);
      ASSERT(ts[next[i0]] >= 0);
    }
  }
  for (j = npt1; j < n1; ++j) {
    int j0 = j;
    int j1 = npt1 + (j - npt1 + next_offset2 + npt2) % npt2;
    int j2 = npt1 + (j - npt1 - next_offset2 + npt2) % npt2;
    for (; next[j0] != j1; j0 = next[j0]) {
      int asdf = 0;
      ASSERT(next[j0] >= n1);
      ASSERT(ts[next[j0]] >= 0);
    }
  }
  pfree(ts);
  return n2;
}
static int wa_findflag(int n, int n1, const char* flag)
{
  int i;
  for (i = 0; i < n; ++i) {
    if (!((i - n1) & 1) && flag[i] < 0) {
      return i;
    }
  }
  return 0;
}
static int clip_wa(int opt, const DPOLYGON* s1, const DPOLYGON* s2, DPOLYGON* out)
{
  int i, j, k, k0, off1 = 0, off2 = 0;
  int npt1 = dPOLYGON_npt(s1);
  int npt2 = dPOLYGON_npt(s2);
  int npt = 10 * (npt1 + npt2);
  char* flag = MALLOC(char, npt);
  DPOINT* pt = MALLOC(DPOINT, npt);
  DPOINT* pt2 = MALLOC(DPOINT, npt);
  int* next = MALLOC(int, npt);
  int* prev = MALLOC(int, npt);
  int nextoff1 = opt & 1 ? -1 : 1;
  int nextoff2 = opt & 2 ? -1 : 1;
  for (off1 = 0, i = 0; i < s1->n; ++i) {
    for (off2 = 0, j = 0; j < s2->n; ++j) {
      int n = 0;
      int n1 = s1->len[i] + s2->len[j];
      n = push_polygon(n, pt, next, prev, s1->len[i], s1->pt + off1, nextoff1);
      n = push_polygon(n, pt, next, prev, s2->len[j], s2->pt + off2, nextoff2);
      memset(flag, 0, npt);
      //int polygon_boolopt(int npt1, int npt2, DPOINT* pt, int* next, int* prev, BOOL dointer, char* flag);
      n = polygon_boolopt(s1->len[i], s2->len[j], pt, nextoff1, nextoff2, next, prev, flag);
      ASSERT(n < npt);
      if (n < 100) {
        for (k = 0; k < n; ++k) {
          printf("%3d %3d %3d %3.f %3.f\n", k, next[k], flag[k], pt[k].x, pt[k].y);
          if (k > n1 && k & 1) {
            //draw_ptmark(hDC11, 1, pt + k, k);
          }
        }
        printf("=============================\n");
      }
      //if (0)
      for (; k0 = wa_findflag(n, n1, flag);) {
        int h = 0;
        int k1 = n1 + ((k0 - n1) ^ 1);
        pt2[h++] = pt[k0];
        flag[k0] = 0;
        flag[k1] = 0;
        //printf("%2d\n", k0);
        for (k = next[k0]; k != k0 && k != k1;) {
          //printf("%2d\n", k);
          pt2[h++] = pt[k];
          if (11 == k) {
            int asdf = 0;
          }
          if (flag[k]) {
            int k1;
            ASSERT(k >= n1);
            // k = n1 + ((k - n1) ^ 1);
            if (flag[k] < 0) {
              k1 = n1 + ((k - n1) & (~1));
            }
            else {
              k1 = n1 + ((k - n1) | 1);
            }
            flag[k] = 0;
            k = k1;
          }
          flag[k] = 0;
          if (k >= n1) {
            flag[n1 + ((k - n1) ^ 1)] = 0;
          }
          k = next[k];
          ASSERT(h < npt);
        }
        //printf("-------------\n");
        dPOLYGON_addpts(out, h, pt2);
      }
      off2 += s2->len[j];
    }
    off1 += s1->len[i];
  }
  pfree(next);
  pfree(pt);
  pfree(pt2);
  pfree(flag);
  pfree(prev);
  return 0;
}
//#include "aaa.inl"

