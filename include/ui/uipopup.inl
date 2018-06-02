int uipopup_hitrect(const uipopup* s, RECT* out)
{
  RECT rect = s->base.rc;
  int b = 3;
  *out = rect;
  RCOFFSET(out, b, b, -b, -b);
  return 0;
}
int draw_uipopup(const uipopup* s, const event* e)
{
  UIHDC(e);
  if (s->subctrl) {
    draw_3drectR(hDC, &s->base.rc, 0);
    UICALL_DRAW(s->subctrl, e);
  }
  return 0;
}
int uipopup_event(uipopup* s, const event* e)
{
  UIEVT(e);
  int i = 0;
  switch (e->msg) {
  case EVENT_LCLICK:
    if (e->sender == s->subctrl) {
      s->subctrl = 0;
    }
    break;
  case WM_SIZE:
    if (s->subctrl) {
      uipopup_hitrect(s, &s->subctrl->rc);
    }
    break;
  case WM_LBUTTONDOWN:
    if (!PtInRect(prc, e->pt)) {
      s->subctrl = 0;
      force_redraw(e);
    }
    break;
  case EVENT_GETSIZE:
    if (e->wp) {
      SIZE* psz = (SIZE*) e->wp;
      UICALL_EVENT(s->subctrl, e);
      psz->w += 6, psz->h += 6;
      return 1;
    }
    break;
  case EVENT_SETCTRL:
    if (e->wp) {
      SIZE psz[1] = {0};
      RECT rc;
      s->subctrl = (uictrl*)e->wp;
      UISendEvent(e, s, s->subctrl, EVENT_GETSIZE, (UINT)psz, 0);
      psz->w += 6, psz->h += 6;
      rc = iRECT3(e->pt, psz[0]);
      //uipopup_hitrect(s, &rc, &s->subctrl->rect);
    }
    break;
  }
  if (s->subctrl) {
    UICALL_EVENT(s->subctrl, e);
  }
  return 0;
}
int uipopup_set(uipopup* s)
{
  UISETCALL(s, draw_uipopup, uipopup_event);
  return 0;
}

