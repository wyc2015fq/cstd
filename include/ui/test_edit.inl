typedef struct test_edit_t {
  ctrl_t c[1];
  slider_t m_alpha[1];
  editctrl_t m_edit[1];
  layout_t lay0[1];
  char buf[256];
} test_edit_t;
static LRESULT test_edit_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_edit_t* s = (test_edit_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  switch (uMsg) {
  case WM_CTRLFREE:
    return 0;
  case WM_CTRLINIT: {
    const char* lay1 =
        " xxx;"
        "a%--;"
        "x%--;"
        ;
    str_t bf[1] = {0};
    const char* ss = "我们asdfasd\n\nfasdfa我们sdfasd\nfasdfasdfasd\nfasdfasdfasd\nfasdf\r\n\r\n\t\r\n";
    str_setsize(bf, 1 << 10);
    randstr_cn(bf->s, (1 << 10) - 1);
    //buf_load("D:/pub/bin/aaa.txt", bf);
    //editctrl_set_buf(s->m_edit, c, 0, bf->s, DT_WRAPLINE);
    strcpy(s->buf, ss);
    editctrl_set_buf(s->m_edit, c, 0, s->buf, 256, DT_WRAPLINE);
    //s->m_edit->c->fo = sys_font_set(c->sys, "simfang|14|");
    slider_set(s->m_alpha, c, "Alpha=%1.0f", 0, 255, 10);
    layout_set(s->lay0, c, 0, 0, lay1, s->m_alpha->c, s->m_edit->c, 0);
    //s->m_edit->c->fo = sys_font_load_ttffile(c->sys, "YaHei.Consolas.1.11b|24|");
    //s->m_edit->c->fo = sys_font_load_ttffile(c->sys, "simfang|24|");
    s->m_edit->c->fo = sys_font_load_win32(c->sys, "宋体|24|");
    ctrl_addsub_tail(c, s->lay0->c);
    return 0;
  }
  break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
    mminfo->ptMinTrackSize.x += 120;
    mminfo->ptMinTrackSize.y += 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_LBUTTONUP:
    if (c->sub) {
    }
    break;
  case WM_COMMAND:
    if (c->sub) {
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      ctrl_setrect(c->sub, c->rc);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      c->redraw = 0;
    }
    break;
  }
  return 0;
}

