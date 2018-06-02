// 'l', 30, 2 表示在2号矩形框左部切出30个宽度的框
typedef struct layitem_t {
  char cmd; // 命令 l, t, r, b
  int d;
  int i;
  int id;
  RECT rc;
} layitem_t;
typedef struct layout {
  layitem_t* li;
  int len;
  int gap;
} layout;
int layout_set(layout* lay, str_t s)
{
  int i;
  str_t s0 = STRtrim(s, " \t\r\n");
  str_t s1 = STRsplit(s0, '\n', " \t\r\n", &s0);
  lay->len = 0;
  sscanf(s1.s, "%d", &lay->gap);
  s = s0;

  for (i = 0; s.l > 0; ++i) {
    STRsplit(s, '\n', " \t\r\n", &s);
    lay->len++;
  }

  s = s0;
  lay->li = (layitem_t*)my_realloc2(lay->li, 0, lay->len * sizeof(layitem_t));

  for (i = 0; s.l > 0; ++i) {
    s1 = STRsplit(s, '\n', " \t\r\n", &s);
    sscanf(s1.s, "%c, %d, %d, %d", &lay->li[i].cmd, &lay->li[i].d, &lay->li[i].i, &lay->li[i].id);
  }

  return 0;
}

int layout_print(const layout* lay)
{
  int i;

  for (i = 0; i < lay->len; ++i) {
    printf("%c %3d %3d\n", lay->li[i].cmd, lay->li[i].d, lay->li[i].i);
  }

  return 0;
}
int layout_additem(layout* lay, int cmd, int d, int i, int id)
{
  int n = lay->len;
  ++lay->len;
  lay->li[n].cmd = cmd, lay->li[n].d = d, lay->li[n].i = i, lay->li[n].id = id;
  return lay->len;
}
int layout_delitem(layout* lay, int k)
{
  int i, j = -1, n = lay->len;
  layitem_t* l = lay->li;
  printf("%d\n", k);

  for (i = n - 1; i > k; --i) {
    if (l[i].i == k) {
      l[i].cmd = l[k].cmd;
      l[i].d = l[k].d;
      l[i].i = l[k].i;
      j = i;
      break;
    }
  }

  if (j >= 0) {
    l[k] = l[j];
    k = j;
  }

  for (i = k + 1; i < n; ++i) {
    if (l[i].i > k) {
      l[i].i--;
    }

    l[i - 1] = l[i];
  }

  lay->len--;
  return k;
}
// out 输出框
// gap 分界线宽度
// gaprc 分界线区域
int layout_rect1(layout* lay, RECT rc, int x, int y, int s)
{
  int i, ret = -1;
  layitem_t* l = lay->li;
  l[0].rc = rc;

  for (i = 1; i < lay->len; ++i) {
    if (RectCut(&l[l[i].i].rc, l[i].cmd, l[i].d, lay->gap, x, y, s, &l[i].rc)) {
      ret = i;
    }
  }

  return ret;
}

int layout_rect(layout* lay, RECT rc, int x, int y, int s, uictrl** out)
{
  int i, ret = layout_rect1(lay, rc, x, y, s);

  for (i = 0; i < lay->len; ++i) {
    if (lay->li[i].id >= 0) {
      out[lay->li[i].id]->rc = lay->li[i].rc;
    }
  }

  return ret;
}

typedef struct layout_ctrl {
  uictrl base;
  layout lay[1];
  int boder_width;
  int hit;
  POINT pt;
  BOOL lmousedown;
} layout_ctrl;

int draw_layout_ctrl(const layout_ctrl* s, const event* e)
{
  UIHDC(e);
  int i, ret = -1, d = 0;

  for (i = 0; i < s->lay->len; ++i) {
    if (s->lay->li[i].id >= 0) {
      UICALL_DRAW(e->wid[s->lay->li[i].id], e);
    }
  }

  i = s->hit;

  if (i >= 0) {
    RECT rc1 = s->lay->li[i].rc;
    COLORREF clrs[2] = {0, _RGB(0, 0, 0)};
    RectNeighbour(&rc1, s->lay->li[i].cmd, s->lay->gap, 0);
    draw_fillrect_colorsR(hDC, &rc1, clrs, 2);
  }

  return 0;
}
int layout_ctrl_call_event(const layout* lay, const event* e, int msg)
{
  int i;
  event e2[1] = {0};
  e2[0] = *e;
  e2->msg = msg;

  for (i = 0; i < lay->len; ++i) {
    if (lay->li[i].id >= 0) {
      if (UICALL_EVENT(e->wid[lay->li[i].id], e2)) {
        return 1;
      }
    }
  }

  return 0;
}

int layout_ctrl_event(layout_ctrl* s, const event* e)
{
  UIEVT(e);
  int hit, cmd;

  switch (e->msg) {
  case WM_SIZE:
    if (1) {
      layout_rect(s->lay, s->base.rc, e->x, e->y, 5, e->wid);
    }

    break;

  case EVENT_INIT:
    UISETCALL(s, draw_layout_ctrl, layout_ctrl_event);
    s->hit = -1;
    s->pt = iPOINT(-1, -1);
    return TRUE;
    break;

  case EVENT_EXIT:
    free(s->lay->li);
    break;

  case EVENT_SETATTRIBUTE:
    if (e->wp) {
      str_t* str = (str_t*)e->wp;

      if (0 == mystricmp(str->s, str->l, "data", 4)) {
        int ret = layout_set(s->lay, str[1]);
      }

      return TRUE;
    }

    break;

  case WM_LBUTTONUP:
    if (1) {
      s->hit = -1;
      s->lmousedown = FALSE;
      layout_rect(s->lay, s->base.rc, e->x, e->y, 5, e->wid);
      layout_ctrl_call_event(s->lay, e, WM_SIZE);
    }

    break;

  case WM_MOUSEMOVE:
    if (PtInRect(prc, e->pt)) {
      if (s->hit < 0) {
        hit = layout_rect(s->lay, s->base.rc, e->x, e->y, 5, e->wid);

        if (hit >= 0 && !s->lmousedown) {
          cmd = s->lay->li[hit].cmd;

          if ('l' == cmd || 'r' == cmd) {
            SetCursor(LoadCursor(NULL, IDC_SIZEWE));
          }

          if ('t' == cmd || 'b' == cmd) {
            SetCursor(LoadCursor(NULL, IDC_SIZENS));
          }

          s->pt = e->pt;
          force_redraw(e);
        }
        else {
          SetCursor(LoadCursor(NULL, IDC_ARROW));
        }
      }
      else {
        hit = s->hit;
        cmd = s->lay->li[hit].cmd;

        if ('l' == cmd) {
          s->lay->li[hit].d -= s->pt.x - e->x;
        }

        if ('r' == cmd) {
          s->lay->li[hit].d += s->pt.x - e->x;
        }

        if ('t' == cmd) {
          s->lay->li[hit].d -= s->pt.y - e->y;
        }

        if ('b' == cmd) {
          s->lay->li[hit].d += s->pt.y - e->y;
        }

        s->pt = e->pt;
        layout_rect(s->lay, s->base.rc, e->x, e->y, 5, e->wid);
        layout_ctrl_call_event(s->lay, e, WM_SIZE);
        force_redraw(e);
        return TRUE;
      }
    }

    break;

  case WM_LBUTTONDOWN:
    if (1) {
      s->lmousedown = TRUE;
      hit = layout_rect(s->lay, s->base.rc, e->x, e->y, 5, e->wid);

      if (hit >= 0) {
        cmd = s->lay->li[hit].cmd;

        if ('l' == cmd || 'r' == cmd) {
          SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        }

        if ('t' == cmd || 'b' == cmd) {
          SetCursor(LoadCursor(NULL, IDC_SIZENS));
        }

        s->hit = hit;
        s->pt = e->pt;
        force_redraw(e);
        return TRUE;
      }
    }

    break;
  }

  layout_ctrl_call_event(s->lay, e, e->msg);
  return FALSE;
}
int layout_ctrl_set(layout_ctrl* s, int gap)
{
  UISETCALL(s, draw_layout_ctrl, layout_ctrl_event);
  s->hit = -1;
  return 0;
}
typedef struct matrix_layout_ctrl {
  uictrl base;
  int* li;
  int len;
  int gap;
  int boder_width;
  int hit;
  POINT pt;
  BOOL lmousedown;
  int row, col;
} matrix_layout_ctrl;

int matrix_layout_ctrl_hitrect(const matrix_layout_ctrl* s, uictrl** out)
{
  RECT rc[256] = {0};
  int i;
  matrix_rect(s->base.rc, s->row, s->col, s->gap, s->gap, s->len, rc);

  for (i = 0; i < s->len; ++i) {
    out[s->li[i]]->rc = rc[i];
  }

  return 0;
}
int drawaa_matrix_layout_ctrl(const matrix_layout_ctrl* s, const event* e)
{
  UIHDC(e);
  int i;

  for (i = 0; i < s->len; ++i) {
    UICALL_DRAW(e->wid[s->li[i]], e);
  }

  return 0;
}
int matrix_layout_ctrl_event(matrix_layout_ctrl* s, const event* e)
{
  UIHDC(e);
  int i;

  for (i = 0; i < s->len; ++i) {
    UICALL_EVENT(e->wid[s->li[i]], e);
  }

  return 0;
}
int matrix_layout_ctrl_set(matrix_layout_ctrl* s, int row, int col, int gap, int n, int* li)
{
  UISETCALL(s, drawaa_matrix_layout_ctrl, matrix_layout_ctrl_event);
  s->gap = gap, s->li = li, s->len = n;
  s->hit = -1;
  s->row = row;
  s->col = col;

  if (row <= 0) {
    s->row = (n + col - 1) / col;
  }

  return 0;
}
typedef struct test_image_t {
  uictrl base;
  UI_engine ui[1];
  img_t m_img[256];
  RECT rc[256];
  int row, col;
  int num;
} test_ui_t;
test_ui_t g_test_ui[1] = {0};
int test_ui_paint_proc(const test_ui_t* g, const event* e)
{
  UIHDC(e);
  int i;
  const img_t* im = g->m_img;
  draw_clear(hDC, _GetSysColor(COLOR_BTNFACE));

  for (i = 0; i < countof(g->m_img); ++i) {
    if (im[i].s > 0 && im[i].h > 0) {
      RECT rc0 = g->rc[i];
      RectScaling(rc0, im[i].w, im[i].h, &rc0);
      draw_imageR(hDC, &rc0, im + i, 0, 0, 0);
    }
  }

  return 0;
}
int test_ui_frame_proc(test_ui_t* g, const event* e)
{
  switch (e->msg) {
  case EVENT_EXIT:
    if (e->sender == g) {
      freeims(g->m_img, countof(g->m_img));
      PostQuitMessage(0);
      return 1;
    }

    break;

  case WM_SIZE:
    matrix_rect(g->base.rc, g->row, g->col, 4, 4, g->num, g->rc);
    break;

  case EVENT_INIT:
    if (e->sender == g) {
      return 1;
    }

    break;
  }

  return 0;
}

