#include "draw\DockingMarkers.inl"
typedef struct tabs_t {
  int ind;
  int n;
  int d;
  IRECT rc;
} tabs_t;
typedef struct multipane_ctrl_t {
  ctrl_t c[1];
  tab_item* tab;
  int ntab;
  tabs_t* pane;
  int npane;
  int hittab;
  int hititem;
  int hottab;
} multipane_ctrl_t;
int tabs_setrect(tabs_t* tabs, int n, int pos, IRECT rc, int gap)
{
  int i, ind1 = tabs[pos].ind + 1, n2 = 0, sumd = 0;
  tabs[pos].rc = rc;
  for (i = pos + 1; i < n && tabs[i].ind >= ind1; ++i) {
    if (ind1 == tabs[i].ind) {
      ++n2;
      sumd += tabs[i].d;
    }
  }
  if (n2 > 0) {
    IRECT rc1;
    int x = rc.l, y = rc.t, cx = RCW(&rc), cy = RCH(&rc);
    int ishor = ind1 & 1;
    sumd = MAX(sumd, 1);
    if (ishor) {
      cx -= n2 * gap;
    }
    else {
      cy -= n2 * gap;
    }
    if (cx > 0 && cy > 0) {
      for (i = pos + 1; i < n && tabs[i].ind >= ind1; ++i) {
        if (ind1 == tabs[i].ind) {
          int d = tabs[i].d;
          if (ishor) {
            d = cx * d / sumd;
            rc1 = iRECT2(x, y, d, cy);
          }
          else {
            d = cy * d / sumd;
            rc1 = iRECT2(x, y, cx, d);
          }
          tabs_setrect(tabs, n, i, rc1, gap);
          tabs[i].d = d;
        }
      }
    }
  }
  return 0;
}
int tabs_hittest(tabs_t* tabs, int n, int gap, int tt, int x, int y)
{
  int i, hit = -1, ind1 = -1;
  for (i = 0; i < n && tabs[i].ind >= ind1; ++i) {
    if (tabs[i].ind > ind1) {
      IRECT rc1, rc = tabs[i].rc;
      int ishor = ind1 & 1;
      if (ishor) {
        rc1 = iRECT(rc.r - tt, rc.t, rc.r + tt + gap, rc.b);
      }
      else {
        rc1 = iRECT(rc.l, rc.b - tt, rc.r, rc.b + tt + gap);
      }
      if (iPtInRect(&rc1, x, y)) {
        return i * 2 + 1;
      }
      if (iPtInRect(&rc, x, y)) {
        hit = i * 2;
        ind1 = tabs[i].ind;
      }
    }
  }
  return hit;
}
#if 0
int multipane_ctrl_deltabitem(multipane_ctrl_t* s, int itab, int itabitem)
{
  int i;
  for (i = itabitem + 1; i < s->tab[itab].num; ++i) {
    s->tab[itab].items[i - 1] = s->tab[itab].items[i];
  }
  s->tab[itab].num--;
  s->tab[itab].cur = 0;
  if (0 == s->tab[itab].num) {
    int k = layout_delitem(s->lay->lilen, s->lay->li, itab);
    free(s->tab[itab].items);
    s->tab[itab] = s->tab[k];
    for (i = k + 1; i < s->lay->lilen; ++i) {
      s->tab[i - 1] = s->tab[i];
    }
    s->lay->lilen--;
    return itab;
  }
  return -1;
}
int multipane_ctrl_addtabitem(multipane_ctrl_t* s, int itab, tab_item* ti)
{
  int n = s->tab[itab].num;
  s->tab[itab].num++;
  s->tab[itab].items = (tab_item*)realloc(s->tab[itab].items, s->tab[itab].num * sizeof(tab_item));
  s->tab[itab].items[n] = *ti;
  s->tab[itab].cur = n;
  return 0;
}
int multipane_ctrl_movetab(multipane_ctrl_t* s, const RECT* prc, int itab0, int itabitem, int itab, int hit)
{
  tab_item ti = s->tab[itab0].items[itabitem];
  RECT rc[256];
  printf("%d %d\n", itabitem, itab);
  if (0 == hit) {
    multipane_ctrl_addtabitem(s, itab, &ti);
  }
  else if (1 <= hit && 4 >= hit) {
    rc[0] = *prc;
    layout_rect(s->lay->lilen, s->lay->li, s->lay->gap, 0, 0, 0, rc);
    tab_ctrl_set(s->tab + s->lay->lilen);
    multipane_ctrl_addtabitem(s, s->lay->lilen, &ti);
    {
      int aaa[] = {'l', 't', 'r', 'b'};
      int bbb[] = {RCW(&rc[itab]) / 2, RCH(&rc[itab]) / 2, RCW(&rc[itab]) / 2, RCH(&rc[itab]) / 2};
      s->lay->lilen = layout_additem(s->lay->lilen, s->lay->li, aaa[hit - 1], bbb[hit - 1], itab, 0);
    }
  }
  multipane_ctrl_deltabitem(s, itab0, itabitem);
  layout_print(s->lay->lilen, s->lay->li);
  {
    int i, j, k;
    for (j = 0, i = 0; i < s->lay->lilen; ++i) {
      printf("%3d: ", s->tab[i].num);
      for (k = 0; k < s->tab[i].num; ++k) {
        printf("%5s ", s->tab[i].items[k].text);
      }
      printf("\n");
    }
  }
  return 0;
}
int draw_multipane_ctrl_ctrl(const multipane_ctrl_t* s, const event* e)
{
  UIHDC(e);
  RECT rcs[256] = {0};
  RECT rect = *prc;
  int i, n = s->lay->lilen;
  draw_layout_ctrl(s->lay, e, prc);
  rcs[0] = rect;
  layout_rect(s->lay->lilen, s->lay->li, s->lay->gap, 0, 0, 0, rcs);
  for (i = 0; i < n; ++i) {
    draw_tab_ctrl(s->tab + i, e, &rcs[i]);
  }
  if (s->hittab >= 0 && s->hititem >= 0 && s->hottab >= 0) {
    //s->pane->rect = rcs[s->hottab];
    draw_LayoutPane(hDC, s->pane);
  }
  return 0;
}
int multipane_ctrl_hittest(multipane_ctrl_t* s, POINT pt)
{
  int i, n, j = 0;
  n = s->lay->lilen;
  for (i = 0; i < n; ++i) {
    if (s->tab[i].hit >= 0) {
      s->hititem = s->tab[i].hit;
      return i;
    }
  }
  return -1;
}
int multipane_ctrl_hittesttab(multipane_ctrl_t* s, POINT pt, const RECT* prc)
{
  int i;
  RECT rcs[256] = {0};
  rcs[0] = *prc;
  layout_rect(s->lay->lilen, s->lay->li, s->lay->gap, pt.x, pt.y, 0, rcs);
  for (i = 0; i < s->lay->lilen; ++i) {
    if (PtInRect(rcs + i, pt)) {
      return i;
    }
  }
  return -1;
}
int multipane_ctrl_event(multipane_ctrl_t* s, const event* e)
{
  int x = e->x, y = e->y;
  POINT pt = {x, y};
  int i, n;
  int hit = -1;
  int hot = -1;
  RECT rcs[256] = {0};
  n = s->lay->lilen;
  layout_ctrl_event(s->lay, e, prc);
  rcs[0] = *prc;
  layout_rect(s->lay->lilen, s->lay->li, s->lay->gap, 0, 0, 0, rcs);
  for (i = 0; i < n; ++i) {
    tab_ctrl_event(s->tab + i, e, &rcs[i]);
  }
  layoutpane_event(s->pane, e, prc);
  switch (e->msg) {
  case WM_LBUTTONDOWN:
    if (PtInRect(prc, pt)) {
      hit = multipane_ctrl_hittest(s, pt);
      if (s->hittab != hit) {
        s->hittab = hit;
        force_redraw(e);
      }
    }
    break;
  case WM_LBUTTONUP:
    if (PtInRect(prc, pt)) {
      if (s->hittab >= 0 && s->hottab >= 0) {
        hit = LayoutPane_hittest(s->pane, pt);
        if (hit >= 0) {
          multipane_ctrl_movetab(s, prc, s->hittab, s->hititem, s->hottab, hit);
        }
      }
      s->hittab = -1;
      s->hottab = -1;
      force_redraw(e);
    }
    break;
  case WM_MOUSEMOVE:
    if (s->hittab >= 0 && PtInRect(prc, pt)) {
      hot = multipane_ctrl_hittesttab(s, pt, prc);
      if (s->hottab != hot) {
        s->hottab = hot;
        force_redraw(e);
      }
    }
    break;
  case WM_DESTROY:
    break;
  }
  return 0;
}
int multipane_ctrl_set(multipane_ctrl_t* s, tab_item* li, int* pn, int n, layitem_t* la)
{
  int i, j = 0;
  s->hittab = -1;
  s->hottab = -1;
  for (i = 0; i < n; ++i) {
    tab_ctrl_set(s->tab + i, pn[i], 0);
    s->tab[i].items = (tab_item*)realloc(s->tab[i].items, pn[i] * sizeof(tab_item));
    memcpy(s->tab[i].items, li + j, pn[i]*sizeof(tab_item));
    j += pn[i];
  }
  layout_ctrl_set(s->lay, 4, n, la);
  load_MarkersLayout(s->pane, 0);
  return 0;
}
#endif

