#define IV2EQ(a, b) ((a)[0]==(b)[0] && (a)[1]==(b)[1])
int iseg_inter_test2(const int* p1, const int* p2, const int* p3, const int* p4, iINTER* ii)
{
  int R0 = 0, R1 = 0, R2 = 0, R = 1 //
      && (MAX(p1[0], p2[0]) >= MIN(p3[0], p4[0])) // 排斥实验
      && (MAX(p3[0], p4[0]) >= MIN(p1[0], p2[0])) // 排斥实验
      && (MAX(p1[1], p2[1]) >= MIN(p3[1], p4[1])) // 排斥实验
      && (MAX(p3[1], p4[1]) >= MIN(p1[1], p2[1])); // 排斥实验
  if (R) {
    // 点在点上
    R0 = MAKE4BIT(IV2EQ(p1, p3), IV2EQ(p1, p4), IV2EQ(p2, p3), IV2EQ(p2, p4));
    ii->ts[0] = ii->ts[1] = 0;
    if (R0 & 1) {
      V2SETV(&ii->pt.x, p1);
    }
    else if (R0 & 2) {
      V2SETV(&ii->pt.x, p1);
    }
    else if (R0 & 2) {
      V2SETV(&ii->pt.x, p2);
    }
    else if (R0 & 4) {
      V2SETV(&ii->pt.x, p2);
    }
    else if (!R0) {
      // 点在线上，跨立实验
      // 如果d1 * d2 < 0，点12在线34的两边，d1==0表示p1在线34上
      int d1 = V2CROSS(p1, p4, p3), d2 = V2CROSS(p2, p4, p3);
      // 点34在线12的两边
      int d3 = V2CROSS(p3, p2, p1), d4 = V2CROSS(p4, p2, p1);
      R1 = (d3 * d4 <= 0) && (d1 * d2 <= 0);
      R2 = MAKE4BIT(FEQ(d1, 0), FEQ(d2, 0), FEQ(d3, 0), FEQ(d4, 0));
      if (R2 & 1) {
        iline_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p1);
        ii->ts[0] = 0;
      }
      else if (R2 & 2) {
        iline_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p2);
        ii->ts[0] = 1;
      }
      else if (R2 & 4) {
        iline_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p3);
        ii->ts[1] = 0;
      }
      else if (R2 & 8) {
        iline_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p4);
        ii->ts[1] = 1;
      }
      else if (!R2) {
        iline_inter(p1, p2, p3, p4, ii);
      }
    }
  }
  return R | (R0 << 1) | (R1 << 5) | (R2 << 6);
}
#define ARRREV(_N, _A, t) {int _i=0; for (;_i<(_N)/2; ++_i) {t=(_A)[_i], (_A)[_i]=(_A)[_N-1-_i], (_A)[_N-1-_i]=t;} }
int ipoly_inter(const ipoly_t* s, const ipoly_t* c, IPOINT* pt)
{
  int i, j, ii, jj, k = 0;
  const IPOINT* sp;
  iINTER iii[1] = {0};
  sp = s->pt;
  for (i = 0; i < s->n; ++i) {
    int sn = s->len[i];
    for (ii = 0; ii < sn; ++ii) {
      int ni = NEXT_INDEX(ii, sn);
      const IPOINT* cp = c->pt;
      for (j = 0; j < c->n; ++j) {
        int cn = c->len[j];
        for (jj = 0; jj < cn; ++jj) {
          int nj = NEXT_INDEX(jj, cn);
          k += !!iseg_inter_test(&sp[ii].x, &sp[ni].x, &cp[jj].x, &cp[nj].x, 0);
          if (pt) {
            iline_inter(&sp[ii].x, &sp[ni].x, &cp[jj].x, &cp[nj].x, iii);
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
// next_offset下一点的偏移量
int ipolygon_push(int n, IPOINT* pt, int* next, int* prev, int p1n, const IPOINT* p1, int next_offset)
{
  int i;
  for (i = 0; i < p1n; ++i) {
    pt[n + i] = p1[i];
    next[n + i] = n + (i + next_offset + p1n) % p1n;
    prev[n + i] = n + (i - next_offset + p1n) % p1n;
  }
  return n + p1n;
}
int setnext(int* next, int* prev, int n, int a, int b, int c, int d)
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
int ipolygon_boolopt(int npt1, int npt2, IPOINT* pt, int next_offset1, int next_offset2, int* next, int* prev, char* flag)
{
  int i, j, k = 0, n1, n2;
  int cc[4];
  int* ts;
  iINTER ii[1] = {0};
  iINTER ii1[1] = {0};
  iINTER ii2[1] = {0};
  n1 = n2 = npt1 + npt2;
  ts = MALLOC(int, 10 * n2);
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
      int u = 0, k = iseg_inter_test2(&pt[i].x, &pt[i1].x, &pt[j].x, &pt[j1].x, ii);
      int R0 = (k >> 1) & 15, R1 = (k >> 5) & 1, R2 = (k >> 6) & 15, R = k & 1;
      if (i == 1 && j == 4) {
        int asdf = 0;
      }
      if (k) {
        int d = ii->d;
        flag[n2] = flag[n2 + 1] = 0;
        if (R) {
          if (R0) {
            // 点在点上
            if (1 == R0) {
              ii1->ts[0] = ii2->ts[0] = -1;
              iline_inter(&pt[i2].x, &pt[i1].x, &pt[j0].x, &pt[j1].x, ii1);
              iline_inter(&pt[i2].x, &pt[i1].x, &pt[j0].x, &pt[j2].x, ii2);
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
              int d1, d2, d3, d4, d5;
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
        //ASSERT(ii->ts[0] >= 0 && ii->ts[1] >= 0);
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
int wa_findflag(int n, int n1, const char* flag)
{
  int i;
  for (i = 0; i < n; ++i) {
    if (!((i - n1) & 1) && flag[i] < 0) {
      return i;
    }
  }
  return 0;
}
int ipoly_clip_wa(int opt, const ipoly_t* s1, const ipoly_t* s2, ipoly_t* out);
int dpoly_clip_wa(int opt, const poly2d_t* s1, const poly2d_t* s2, poly2d_t* out)
{
  ipoly_t is1[1] = {0};
  ipoly_t is2[1] = {0};
  ipoly_t is3[1] = {0};
  double t = 1;
  poly2d_to_ipoly(s1, is1, t);
  poly2d_to_ipoly(s2, is2, t);
  ipoly_remove_samp(is1);
  ipoly_remove_samp(is2);
  ipoly_clip_wa(opt, is1, is2, is3);
  ipoly_to_poly2d(is3, out, 1. / t);
  ipoly_free(is1);
  ipoly_free(is2);
  ipoly_free(is3);
  return 0;
}
int ipoly_clip_wa(int opt, const ipoly_t* s1, const ipoly_t* s2, ipoly_t* out)
{
  int i, j, k, k0, off1 = 0, off2 = 0;
  int npt1 = ipoly_npt(s1);
  int npt2 = ipoly_npt(s2);
  int npt = 10 * (npt1 + npt2);
  char* flag = MALLOC(char, npt);
  IPOINT* pt = MALLOC(IPOINT, npt);
  IPOINT* pt2 = MALLOC(IPOINT, npt);
  int* next = MALLOC(int, npt);
  int* prev = MALLOC(int, npt);
  int nextoff1 = opt & 1 ? -1 : 1;
  int nextoff2 = opt & 2 ? -1 : 1;
  for (off1 = 0, i = 0; i < s1->n; ++i) {
    for (off2 = 0, j = 0; j < s2->n; ++j) {
      int n = 0;
      int n1 = s1->len[i] + s2->len[j];
      n = ipolygon_push(n, pt, next, prev, s1->len[i], s1->pt + off1, nextoff1);
      n = ipolygon_push(n, pt, next, prev, s2->len[j], s2->pt + off2, nextoff2);
      memset(flag, 0, npt);
      //int polygon_boolopt(int npt1, int npt2, IPOINT* pt, int* next, int* prev, BOOL dointer, char* flag);
      n = ipolygon_boolopt(s1->len[i], s2->len[j], pt, nextoff1, nextoff2, next, prev, flag);
      ASSERT(n < npt);
      if (n < 100) {
        for (k = 0; k < n; ++k) {
          //printf("%3d %3d %3d %3.f %3.f\n", k, next[k], flag[k], pt[k].x, pt[k].y);
          if (k > n1 && k & 1) {
            //draw_ptmark(hDC11, 1, pt + k, k);
          }
        }
        //printf("=============================\n");
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
        ipoly_addpts(out, h, pt2);
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

