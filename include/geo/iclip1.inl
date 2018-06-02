int iseg_inter_test1(const int* p1, const int* p2, const int* p3, const int* p4, int* pp)
{
  int max012, min012, max034, min034;
  int max112, min112, max134, min134;
  int ret = 0, R = 1 //
      && ((max012 = MAX(p1[0], p2[0])) >= (min034 = MIN(p3[0], p4[0]))) // 排斥实验
      && ((max034 = MAX(p3[0], p4[0])) >= (min012 = MIN(p1[0], p2[0]))) // 排斥实验
      && ((max112 = MAX(p1[1], p2[1])) >= (min134 = MIN(p3[1], p4[1]))) // 排斥实验
      && ((max134 = MAX(p3[1], p4[1])) >= (min112 = MIN(p1[1], p2[1]))) // 排斥实验
      ;
  if (R) {
    int d1, d2, d3, d4;
    ret |= IV2EQ(p1, p3) ? IPT_A_ON_C : 0;
    ret |= IV2EQ(p1, p4) ? IPT_A_ON_D : 0;
    ret |= IV2EQ(p2, p3) ? IPT_B_ON_C : 0;
    ret |= IV2EQ(p2, p4) ? IPT_B_ON_D : 0;
    d1 = V2CROSS(p1, p3, p4);
    d2 = V2CROSS(p2, p3, p4);
    d3 = V2CROSS(p3, p1, p2);
    d4 = V2CROSS(p4, p1, p2);
    ret |= (0 == d1 && min034 <= p1[0] && p1[0] <= min034 && min134 <= p1[1] && p1[1] <= min134) ? IPT_A_ON_CD : 0;
    ret |= (0 == d2 && min034 <= p2[0] && p2[0] <= min034 && min134 <= p2[1] && p2[1] <= min134) ? IPT_B_ON_CD : 0;
    ret |= (0 == d3 && min012 <= p3[0] && p3[0] <= min012 && min112 <= p3[1] && p3[1] <= min112) ? IPT_C_ON_AB : 0;
    ret |= (0 == d4 && min012 <= p4[0] && p4[0] <= min012 && min112 <= p4[1] && p4[1] <= min112) ? IPT_D_ON_AB : 0;
    if (!ret) {
      int p21[2], p43[2], p31[2], d, d0, d1 = 0;
      V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
      //p21*ts[0]-p43*ts[1]==p31;
      d = DET2(p21[0], p43[0], p21[1], p43[1]);
      if (d) {
        ret |= IPT_INTER;
        V2SUBV(p31, p3, p1);
        d0 = DET2(p31[0], p43[0], p31[1], p43[1]);
        pp[0] = p1[0] + p21[0] * d0 * 1. / d;
        pp[1] = p1[1] + p21[1] * d0 * 1. / d;
      }
      else {
        ret |= IPT_PARALLEL;
      }
    }
  }
  if (ret & (IPT_A_ON_C | IPT_A_ON_D | IPT_A_ON_CD)) {
    pp[0] = p1[0];
    pp[1] = p1[1];
  }
  else if (ret & (IPT_B_ON_C | IPT_B_ON_D | IPT_B_ON_CD)) {
    pp[0] = p2[0];
    pp[1] = p2[1];
  }
  else if (ret & (IPT_C_ON_AB)) {
    pp[0] = p3[0];
    pp[1] = p3[1];
  }
  else if (ret & (IPT_D_ON_AB)) {
    pp[0] = p2[0];
    pp[1] = p2[1];
  }
  return ret;
}
#define PTEQ(pt1, pt2) ((pt1).x==(pt2).x && (pt1).y==(pt2).y)
int ipt_cross(IPOINT p1, IPOINT p2, IPOINT p3)
{
  int ret = V2CROSS(&p1.x, &p2.x, &p3.x);
  return ret;
}
// 点列pt 绕点po 顺时针排序
int pt_sort(int n, IPOINT* pt, IPOINT po)
{
#define PTLESS(a, b) (ipt_cross(a, b, po)>=0)
  QSORT(pt, pt + n, PTLESS, IPOINT);
#undef PTLESS
  return 0;
}
int pt_sort_index(int n, const IPOINT* pt, IPOINT po, int* index)
{
#define PTLESS(a, b) (ipt_cross(pt[a], pt[b], po)>=0)
  QSORT(index, index + n, PTLESS, int);
#undef PTLESS
  return 0;
}
int test_pt_sort()
{
  IPOINT po = {0}, pt[] = {
    100, 100,
    -100, -100,
    100, -100,
    -100, 100,
  };
  int index[countof(pt)] = {0};
  int i = 0;
  for (i = 0; i < countof(pt); ++i) {
    index[i] = i;
  }
  //pt_sort(countof(pt), pt, po);
  pt_sort_index(countof(pt), pt, po, index);
  return 0;
}
typedef struct iINTER1 {
  IPOINT pt;
  int k1, k2;
  int j1, j2;
} iINTER1;
int test111(const iINTER1* ii, const IPOINT* pt0)
{
  int index[4];
  index[0] = ii->k1;
  index[1] = ii->j1;
  index[2] = ii->k2;
  index[3] = ii->j2;
  return 0;
}
int ipolygon_boolopt1(int* next, int* prev, int head1, int head2, int npt, IPOINT* pt, int maxnpt)
{
  int i1, i2, j1, j2, k, i = 0, n = 0, k1, k2;
  iINTER1 ii[100];
  for (i1 = head1, j1 = -1; j1 != head1; i1 = j1) {
    j1 = next[i1];
    k1 = prev[i1];
    for (i2 = head2, j2 = -1; j2 != head2; i2 = j2) {
      IPOINT pp = {0};
      j2 = next[i2];
      k2 = prev[i2];
      k = iseg_inter_test1(&pt[i1].x, &pt[j1].x, &pt[i2].x, &pt[j2].x, &pp.x);
      if (k & (IPT_A_ON_C | IPT_A_ON_CD | IPT_C_ON_AB | IPT_INTER)) {
        ASSERT(n < maxnpt);
        ii[n].pt = pp;
        if (k & IPT_A_ON_C) {
          ii[n].k1 = k1;
          ii[n].k2 = k2;
          ii[n].j1 = j1;
          ii[n].j2 = j2;
        }
        else if (k & IPT_A_ON_CD) {
          ii[n].k1 = k1;
          ii[n].k2 = i2;
          ii[n].j1 = j1;
          ii[n].j2 = j2;
        }
        else if (k & IPT_C_ON_AB) {
          ii[n].k1 = i1;
          ii[n].k2 = k2;
          ii[n].j1 = j1;
          ii[n].j2 = j2;
        }
        else if (k & IPT_INTER) {
          ii[n].k1 = i1;
          ii[n].k2 = i2;
          ii[n].j1 = j1;
          ii[n].j2 = j2;
        }
        ++n;
      }
    }
  }
  for (i = 0; i < n; ++i) {
    SWAP(next[i + 0], next[i + 1], k);
  }
  return n;
}
int ipolygon_remove_samp(int n, const IPOINT* p1, IPOINT* p2)
{
  int i = 0, j = 0;
  if (n <= 0) {
    return 0;
  }
  p2[j++] = p1[i++];
  for (; i < n; ++i) {
    if (!PTEQ(p1[i], p2[j - 1])) {
      p2[j++] = p1[i];
    }
  }
  if (PTEQ(p2[0], p2[j - 1])) {
    --j;
  }
  return j;
}
int ipoly_remove_samp(ipoly_t* s)
{
  int i, j = 0, n1, n2;
  IPOINT* p1 = s->pt;
  IPOINT* p2 = s->pt;
  for (i = 0; i < s->n; ++i) {
    n1 = s->len[i];
    n2 = ipolygon_remove_samp(n1, p1, p2);
    p1 += n1;
    p2 += n2;
    if (n2 > 0) {
      s->len[j++] = n2;
    }
  }
  s->n = j;
  return 0;
}
// next_offset下一点的偏移量
int ipolygon_push1(int n, IPOINT* pt, int* next, int* prev, int p1n, const IPOINT* p1, int next_offset)
{
  int i;
  for (i = 0; i < p1n; ++i) {
    pt[n + i] = p1[i];
    next[n + i] = n + (i + next_offset + p1n) % p1n;
    prev[n + i] = n + (i - next_offset + p1n) % p1n;
  }
  return n + p1n;
}
int ipoly_clip_wa1(int opt, const ipoly_t* s1, const ipoly_t* s2, ipoly_t* out)
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
      n = ipolygon_push1(n, pt, next, prev, s1->len[i], s1->pt + off1, nextoff1);
      n = ipolygon_push1(n, pt, next, prev, s2->len[j], s2->pt + off2, nextoff2);
      //int polygon_boolopt(int npt1, int npt2, IPOINT* pt, int* next, int* prev, BOOL dointer, char* flag);
      n = ipolygon_boolopt1(next, prev, 0, s1->len[i], n1, pt, npt);
      ASSERT(n < npt);
      //if (0)
      for (; k0 = n1;) {
        int h = 0;
        pt2[h++] = pt[k0];
        //printf("%2d\n", k0);
        for (k = next[k0]; k != k0;) {
          pt2[h++] = pt[k];
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
int dpoly_clip_wa1(int opt, const poly2d_t* s1, const poly2d_t* s2, poly2d_t* out)
{
  ipoly_t is1[1] = {0};
  ipoly_t is2[1] = {0};
  ipoly_t is3[1] = {0};
  double t = 1;
  poly2d_to_ipoly(s1, is1, t);
  poly2d_to_ipoly(s2, is2, t);
  ipoly_remove_samp(is1);
  ipoly_remove_samp(is2);
  ipoly_clip_wa1(opt, is1, is2, is3);
  ipoly_to_poly2d(is3, out, 1. / t);
  ipoly_free(is1);
  ipoly_free(is2);
  ipoly_free(is3);
  return 0;
}

