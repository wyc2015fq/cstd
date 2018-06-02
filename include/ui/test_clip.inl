//#include "make_poly.inl"
//#include "geo/igeo.inl"
//#include ".\geo\clip_weiler_atherton.inl"
//#include ".\geo\ClipPolygon.inl"
//#include ".\geo\ClipPolygon.inl"
//#include "geo\Weiler_Atherton.inl"
#include "geo/clipper.inl"
//#include "geo/iclip2.inl"
#include "get_poly.inl"
typedef struct iclip_pt {
  IPOINT pt;
  struct iclip_pt* next;
  struct iclip_pt* prev;
  int flag;
} iclip_pt;
static int ipush_polygon(iclip_pt* pt, int p1n, const DPOINT* p1)
{
  int i, n = 0;
  for (i = 0; i < p1n; ++i) {
    pt[i].pt.x = (int)p1[i].x;
    pt[i].pt.y = (int)p1[i].y;
    pt[i].pt.x &= ~3;
    pt[i].pt.y &= ~3;
    pt[i].flag = 0;
  }
  for (i = 0; i < p1n; ++i) {
    int iprev = PREV_INDEX(i, p1n);
    if (pt[iprev].pt.x != pt[i].pt.x || pt[iprev].pt.y != pt[i].pt.y) {
      pt[n++] = pt[i];
    }
  }
  p1n = n, n = 0;
  for (i = 0; i < p1n; ++i) {
    int icurr = i;
    int inext = NEXT_INDEX(i, p1n);
    int iprev = PREV_INDEX(i, p1n);
    int d = V2CROSS(&pt[iprev].pt.x, &pt[icurr].pt.x, &pt[inext].pt.x);
    if (d) {
      pt[n++] = pt[i];
    }
  }
  p1n = n, n = 0;
  for (i = 0; i < p1n; ++i) {
    int inext = NEXT_INDEX(i, p1n);
    int iprev = PREV_INDEX(i, p1n);
    pt[i].next = &pt[inext];
    pt[i].prev = &pt[iprev];
  }
  return p1n;
}
// pt插入 p1 p1->next之间
static iclip_pt* iseg_ins(iclip_pt* p1, IPOINT pt, iclip_pt** mem)
{
  iclip_pt* p2 = p1->next;
  iclip_pt* p3 = NULL;
  if (p1->pt.x == pt.x && p1->pt.y == pt.y) {
    p3 = p1;
  }
  else if (p2->pt.x == pt.x && p2->pt.y == pt.y) {
    p3 = p2;
  }
  else {
    p3 = (*mem)++;
    p3->pt = pt;
    p3->flag = 0;
    p3->next = p2;
    p3->prev = p1;
    p1->next = p3;
    p2->prev = p3;
  }
  return p3;
}
// 五点跨立实验 返回1 表示 p1-po-p2 跨立 p3-po-p4
static int test_cross5(const int* p1, const int* p2, const int* p3, const int* p4, const int* po)
{
  int64 d13 = V2CROSS(p1, p3, po);
  int64 d14 = V2CROSS(p1, p4, po);
  int64 d23 = V2CROSS(p2, p3, po);
  int64 d24 = V2CROSS(p2, p4, po);
  int aa = !d13 + !d14 + !d23 + !d24;
  if (aa > 0 && aa < 4) {
    if (3 == aa) {
      int adsf = 0;
    }
  }
  return (d13 * d24 > 0 && d13 * d14 < 0 && d14 * d23 > 0 && d23 * d24 < 0) ? CV_SIGN(d13) : 0;
}
// 交换交点pa 和pb的 next值 ASSERT(pa->pt == pb->pt)
static iclip_pt* iseg_swap_next(iclip_pt* a, iclip_pt* b)
{
  int d;
  iclip_pt* t = NULL;
  if (d = test_cross5(&a->prev->pt.x, &a->next->pt.x, &b->prev->pt.x, &b->next->pt.x, &a->pt.x)) {
    //test_cross5(&a->prev->pt.x, &a->next->pt.x, &b->prev->pt.x, &b->next->pt.x, &a->pt.x);
    //CV_SWAP(a->next, b->next, t);
    t = (d > 0) ? a : b;
    t->flag = 1;
  }
  return t;
}
// p1-po-p3
static int iscrossline(const int* p1, const int* p3, const int* po)
{
  return (p1[0] - po[0]) * (p3[0] - po[0]) <= 0 || (p1[1] - po[1]) * (p3[1] - po[1]) <= 0;
}
// 交比
static int cross_ratio(const int* p1, const int* p2, const int* p3, const int* p4, int* pd1, int* pd2)
{
  int p21[2], p43[2], p31[2], d;
  int d1, d2;
  V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
  d = DET2(p21[0], p43[0], p21[1], p43[1]);
  if (d) {
    V2SUBV(p31, p3, p1);
    d1 = DET2(p31[0], p43[0], p31[1], p43[1]);
    d2 = -DET2(p31[0], p21[0], p31[1], p21[1]);
    if (d < 0) {
      d = -d, d1 = -d1, d2 = -d2;
    }
    *pd1 = d1;
    *pd2 = d2;
  }
  return d;
}
static const int pm[] = {0, 1, 3, 2};
#define XIANGXIAN(x, y) pm[((x)<0)|(((x)<0)<<1)]
static int point_cmp(const int* p1, const int* p2)
{
  int xx1 = XIANGXIAN(p1[0], p1[1]);
  int xx2 = XIANGXIAN(p2[0], p2[1]);
  int d;
  ASSERT(p1[0] != 0 && p1[1] != 0);
  ASSERT(p2[0] != 0 && p2[1] != 0);
  if (xx1 == xx2) {
    d = DET2(p1[0], p2[0], p1[1], p2[1]);
    d = CV_SIGN(d);
  }
  else {
    d = xx2 - xx1;
    d = CV_SIGN(d);
  }
  return d;
}
CC_INLINE int iseg_inter_test3(iclip_pt* a, iclip_pt* b, iclip_pt* c)
{
  const int* p1 = &a->pt.x;
  const int* p2 = &a->next->pt.x;
  const int* p3 = &b->pt.x;
  const int* p4 = &b->next->pt.x;
  c->prev = a;
  c->next = b;
  c->flag = 0;
  if (V2EQ(p1, p3)) {
    const int* po = p1;
    V2SETV(&c->pt.x, p1);
    p1 = &a->prev->pt.x;
    p3 = &b->prev->pt.x;
    {
      int q1[2], q2[2], q3[2], q4[2];
      V2SUBV(q1, p1, po);
      V2SUBV(q2, p2, po);
      V2SUBV(q3, p3, po);
      V2SUBV(q4, p3, po);
    }
  }
  else if ((MAX(p1[0], p2[0]) > MIN(p3[0], p4[0])) // 排斥实验
      && (MAX(p3[0], p4[0]) > MIN(p1[0], p2[0])) // 排斥实验
      && (MAX(p1[1], p2[1]) > MIN(p3[1], p4[1])) // 排斥实验
      && (MAX(p3[1], p4[1]) > MIN(p1[1], p2[1])) // 排斥实验
          ) {
    // 点在线上，跨立实验
    // 如果d1 * d2 < 0，点12在线34的两边，d1==0表示p1在线34上
    int64 d1 = V2CROSS(p1, p4, p3), d2 = V2CROSS(p2, p4, p3);
    // 点34在线12的两边
    int64 d3 = V2CROSS(p3, p2, p1), d4 = V2CROSS(p4, p2, p1);
  }
  return 0;
}
static int polygon_bool_opt(iclip_pt* p1, iclip_pt* p2, iclip_pt* mem, iclip_pt** _ptstak)
{
  iclip_pt* p1_first = p1, *p2_first = p2;
  IPOINT pt;
  iclip_pt* mem0 = mem;
  int k, ncnt = 0;
  iclip_pt** ptstak = _ptstak;
  for (p1 = p1_first; 1;) {
    for (p2 = p2_first; 1;) {
      iclip_pt* p2n = p2->next;
      //k = iseg_inter_test2(&p1->pt.x, &p1->next->pt.x, &p2->pt.x, &p2n->pt.x, &pt.x);
      k = GetCrossPoint(&p1->pt, &p1->next->pt, &p2->pt, &p2->next->pt, &pt);
      if (k) {
        //iseg_inter_test2(&p1->pt.x, &p1->next->pt.x, &p2->pt.x, &p2->next->pt.x, &pt.x);
        GetCrossPoint(&p1->pt, &p1->next->pt, &p2->pt, &p2->next->pt, &pt);
        *ptstak++ = iseg_ins(p1, pt, &mem);
        *ptstak++ = iseg_ins(p2, pt, &mem);
      }
      if ((p2 = p2n) == p2_first) {
        break;
      }
    }
    if ((p1 = p1->next) == p1_first) {
      break;
    }
  }
  ncnt = ptstak - _ptstak;
  for (; ptstak != _ptstak;) {
    ptstak -= 2;
    if (ptstak[0]->flag == 0 || ptstak[1]->flag == 0) {
      iseg_swap_next(ptstak[0], ptstak[1]);
    }
  }
  ptstak = _ptstak + ncnt;
  for (; ptstak != _ptstak;) {
    ptstak -= 2;
    if (ptstak[0]->flag == 1 || ptstak[1]->flag == 1) {
      iclip_pt* t = NULL;
      ptstak[0]->flag <<= 1;
      ptstak[1]->flag <<= 1;
      CV_SWAP(ptstak[0]->next, ptstak[1]->next, t);
    }
  }
  return ncnt;
}
static int iclip_wa(int opt, const DPOLYGON* s1, const DPOLYGON* s2, DPOLYGON* out)
{
  int i, j, k;
  DPOINT* pt1 = 0, *pt2 = 0;
  int npt1 = dPOLYGON_npt(s1);
  int npt2 = dPOLYGON_npt(s2);
  int npt = 10 * (npt1 + npt2);
  iclip_pt* pt = MALLOC(iclip_pt, npt);
  int* len1 = MALLOC(int, s1->n + s2->n);
  int* len2 = len1 + s1->n;
  int n = 0, off = 0, n1, n2, ninter;
  iclip_pt* _ptstak[10000];
  n2 = off;
  for (pt1 = s1->pt, i = 0; i < s1->n; pt1 += s1->len[i], ++i) {
    for (pt2 = s2->pt, j = 0; j < s2->n; pt2 += s2->len[j], ++j) {
      MEMSET(pt, 0, npt);
      n1 = ipush_polygon(pt, s1->len[i], pt1);
      n2 = ipush_polygon(pt + n1, s2->len[j], pt2);
      if (n2 <= 0 || n1 <= 0) {
        continue;
      }
      if (ninter = polygon_bool_opt(pt, pt + n1, pt + n1 + n2, _ptstak)) {
        iclip_pt* pti = pt + n1 + n2;
        for (k = 0; k < ninter; ++k) {
          iclip_pt* pt_first = _ptstak[k];
          if (pt_first->flag) {
            iclip_pt* pt0;
            int npt = 0;
            for (pt0 = pt_first; 1;) {
              pt0->flag = 0;
              ++npt;
              ASSERT(pt0);
              pt0 = pt0->next;
              if (pt_first == pt0) {
                break;
              }
            }
            if (npt) {
              DPOINT* ptlast = out->pt + dPOLYGON_addpts(out, npt, NULL) - npt;
              npt = 0;
              for (pt0 = pt_first; 1;) {
                pt0->flag = 0;
                ptlast[npt].x = pt0->pt.x;
                ptlast[npt].y = pt0->pt.y;
                npt++;
                pt0 = pt0->next;
                if (pt_first == pt0) {
                  break;
                }
              }
            }
          }
        }
        k = 0;
      }
    }
  }
  FREE(pt);
  FREE(len1);
  return 0;
}
typedef struct test_clip_t {
  ctrl_t c[1];
  null_t nul[1];
  rbox_t m_operation[1];
  rbox_t m_polygon[2];
  layout_t lay1[1];
  int hit;
  int m_hit;
  DPOINT pt;
  IPOINT pt1;
  double trans[2][9];
} test_clip_t;
static LRESULT test_clip_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_clip_t* s = (test_clip_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  int i, npt;
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    static const char* m_operation_str[] = {"DIFF", "INT", "XOR", "UNION" };
    static const char* m_polygons_str[] = {
      "gp_0",
      "gp_1",
      "gp_2",
      "gp_3",
      "gp_4",
      "gp_5",
      "gp_arrows",
      "gp_britain",
      "gp_a",
    };
    for (i = 0; i < 2; ++i) {
      rbox_set(s->m_polygon + i, c, countof(m_polygons_str), m_polygons_str);
      M3ID(s->trans[i]);
    }
    rbox_set(s->m_operation, c, countof(m_operation_str), m_operation_str);
    layout_set(s->lay1, c, 4, 0,
        " xxa;"
        "a--%;"
        "a--%;"
        "a--%;"
        "x---;",
        s->m_operation->c,
        s->m_polygon[0].c,
        s->m_polygon[1].c
              );
    s->m_polygon[0].hit = s->m_polygon[1].hit = 2;
    s->m_polygon[0].hit = s->m_polygon[1].hit = 7;
    s->m_polygon[0].hit = s->m_polygon[1].hit = 0;
    s->m_polygon[0].hit = s->m_polygon[1].hit = 1;
    s->m_hit = -1;
    c->sub = s->lay1->c;
    return 0;
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
    mminfo->ptMinTrackSize.x += 620;
    //mminfo->ptMinTrackSize.y += 420;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_SIZE:
    if (s) {
      ctrl_setrect(c->sub, c->rc);
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
      s->m_hit = -1;
    }
    break;
  case WM_COMMAND:
    if (s) {
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      DPOLYGON ss[1] = {0};
      for (i = 0; i < 2; ++i) {
        npt = get_poly(s->m_polygon[i].hit, ss, s->trans[i]);
        if (pt_in_poly(x, y, ss)) {
          s->m_hit = i;
          s->pt.x = x;
          s->pt.y = y;
          break;
        }
        dPOLYGON_free(ss);
      }
      dPOLYGON_free(ss);
    }
    break;
  case WM_TIMER:
    force_redraw(c);
    break;
  case WM_MOUSEMOVE:
    if (s && s->m_hit >= 0) {
      affine_info_t ai1[] = {GEO_TRANS, x - s->pt.x, y - s->pt.y};
      i = s->m_hit;
      affine_info_trans(countof(ai1), ai1, s->trans[i]);
      s->pt.x = x;
      s->pt.y = y;
    }
    s->pt1.x = x;
    s->pt1.y = y;
    force_redraw(c);
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int n_drawn = 0;
      imdraw_fill(im, _rgb(1, 1, 1));
      {
        if (0) {
          int i;
          for (i = 0; i < 20; ++i) {
            //imdraw_circle_aa(IMARG(im), 50 + i * i + i * 2, 200, i, _rgb(1, 0, 0));
            //imdraw_circle(IMARG(im), 50 + i * i + i * 2, 200, i, _rgb(0, 0, 1), _rgb(1, 0, 0), 1 + (int)sli1->m_value);
          }
        }
        if (1) {
          char buf[256];
          DPOLYGON aa[1] = {0};
          DPOLYGON ss[2] = {0};
          COLOR clrs[] = {_rgba(1, 0, 0, 0.2), _rgba(0, 1, 0, 0.2)};
          for (i = 0; i < 2; ++i) {
            npt = get_poly(s->m_polygon[i].hit, ss + i, s->trans[i]);
            imdrawaa_dPOLYGON_solid(im, &rc, ss + i, clrs[i], 0, 0);
          }
          //conv_clipper(aa, ss, cc, m_operation->hit, clipper_non_zero, clipper_non_zero);
          {
            double t;
            font_t* fo = sys_font_load_win32(c->sys, "黑体|24");
            utime_start(_start_time);
            //poly2_clip(s->m_operation->hit, ss+0, ss+1, aa);
            //clip_wa(s->m_operation->hit, ss+0, ss+1, aa);
            iclip_wa(s->m_operation->hit, ss + 0, ss + 1, aa);
            //ipoly_clip_wa(s->m_operation->hit, ss+0, ss+1, aa);
            t = utime_elapsed(_start_time);
            _snprintf(buf, 256, "耗时 %.3f 毫秒", t * 1000);
            imdraw_text(im, &rc, iRECT2(0, 0, 100, 100), buf, -1, fo, _rgb(0, 0, 0), _rgba(1, 0, 0, 0.1), 0);
          }
          //dpoly_clip_wa1(m_operation->hit, ss, cc, aa);
          imdrawaa_dPOLYGON_solid(im, &rc, aa, _rgba(0, 0, 1, 0.2), _rgba(0, 0, 0, 0.5), 1);
          if (aa->len && aa->len[0] < 20 && 0) {
            for (i = 0; i < aa->len[0]; ++i) {
              _snprintf(buf, 256, "%d %d", (int)aa->pt[i].x, (int)aa->pt[i].y);
              imdraw_text(im, &rc, iRECT2((int)aa->pt[i].x, (int)aa->pt[i].y, 100, 100), buf, -1, c->fo, _rgb(0, 0, 0), 0, 0);
            }
          }
          _snprintf(buf, 256, "%d %d", s->pt1.x, s->pt1.y);
          imdraw_text(im, &rc, iRECT2(s->pt1.x + 20, s->pt1.y + 20, 100, 100), buf, -1, c->fo, _rgb(0, 0, 0), 0, 0);
          //test_drawaa(hDC, 0);
          //test_ClipPolygon(hDC, 1);
          dPOLYGON_free(aa);
          dPOLYGON_free(ss + 0);
          dPOLYGON_free(ss + 1);
        }
      }
    }
    break;
  }
  return 0;
}

