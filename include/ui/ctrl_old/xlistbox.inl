typedef struct xlistbox_t {
  int hit, hot;
  vstr_t items[1];
  int item_h;
  int cur;
  IPOINT org;
} xlistbox_t;

int xlistbox_max_item_width(xlistbox_t* s, const font_t* fo)
{
  int i, mw = 100;

  for (i = 0; i < s->items->n; ++i) {
    str_t str = s->items->v[i];
    int dx = font_text_size(fo, str.s, str.l, 0).w;
    mw = MAX(mw, dx);
  }

  return mw;
}

int xlistbox_hittest(xlistbox_t* s, int x, int y)
{
  int i = y / s->item_h;

  if (i >= 0 && i < s->items->n) {
    return i;
  }

  return -1;
}

int xlistbox_set(xlistbox_t* s)
{
  return 0;
}

int xlistbox_msg_proc(MSGPROCARGDEF(xlistbox_t))
{
  IPOINT off = ci[cid].off;

  if (im) {
    if (ci[cid].redraw) {
      IRECT rc0;
      int i, first_show_item, last_show_item;
      int dy = s->item_h, dx = 0;
      imdraw_rect(im, rc, White, 0, 0);
      first_show_item = -off.y / dy;
      last_show_item = 1 + first_show_item + RCH(&rc) / dy;
      first_show_item = MAX(first_show_item, 0);
      last_show_item = MIN(last_show_item, s->items->n);
      XDRAW_BEG(im);
      //draw_fillrectR(im, &rcClient, res->clrBtnWindow);
      //rc = iRectOffset(rca, rcClient.l, rcClient.t, rcClient.l, rcClient.t);
      //rc.t += first_show_item * dy;
      //rc.r = rcClient.r;

      for (i = first_show_item; i < last_show_item; ++i) {
        if (iRectCutT(&rc, dy, 0, &rc0)) {
          str_t str = s->items->v[i];
          COLOR crText = Black;

          if (i == s->cur) {
            imdraw_rect(im, rc0, CLR_HIGHLIGHT, 0, 0);
            crText = White;
          }
          else if (i == s->hot) {
            imdraw_rect(im, rc0, _RGB_A(CLR_HIGHLIGHT, 150), 0, 0);
            crText = White;
          }

          imdraw_text(im, rc0, str.s, str.l, drawer->font, crText, 0, TT_VCENTER);
        }
      }

      XDRAW_END();
    }

    ci[cid].redraw = 0;
  }
  else if (msg) {
    int x = msg->x - rc.l, y = msg->y - rc.t;

    switch (msg->msg) {
    case MSG_INIT:
      if (s) {
        s->hit = -1;
        s->item_h = drawer->font->h + 6;
      }

      break;
      
    case MSG_EXIT:
      if (s) {
        strv_free(s->items);
      }
      break;

    case MSG_SIZE:
      if (s) {
      }

      break;

    case MSG_MOUSEWHEEL:
      if (iPtInRect(&rc, msg->x, msg->y)) {
        s->org.y += GET_WHEEL_DELTA_WPARAM(msg->wp) / 3;
        force_redraw(msg);
      }

      break;

    case MSG_LBUTTONDBLCLK:
    case MSG_LBUTTONDOWN:
      if (iPtInRect(&rc, msg->x, msg->y)) {
        int hit = xlistbox_hittest(s, x - off.x, y - off.y);

        if (s->hit != hit) {
          s->hit = hit;

          if (hit >= 0 && hit < s->items->n) {
            s->cur = hit;
          }

          force_redraw(msg);
        }
      }
      break;
    case MSG_LBUTTONUP:
      if (1) {
        force_redraw(msg);
      }
      break;
    case MSG_MOUSEMOVE:
      if (iPtInRect(&rc, msg->x, msg->y)) {
        int hit = xlistbox_hittest(s, x - off.x, y - off.y);

        if (s->hot != hit) {
          s->hot = hit;
          force_redraw(msg);
        }
      }
      break;
    }
  }

  return 0;
}
