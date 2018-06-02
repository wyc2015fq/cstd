typedef struct mdi_t {
  ctrl_t c[1];
  framectrl_t* win;
  int num;
} mdi_t;
int draw_mdi(const mdi_t* s)
{
  int i;
  for (i = 0; i < s->num; ++i) {
    //uictrl* p = &s->win[i].base;
  }
  return 0;
}
int mdi_addwin(mdi_t* s, ctrl_t* c, const char* text, IRECT rc)
{
  int n = s->num;
  s->num++;
  //frame_set(s->win + n, s->c->listener, s->c->sys, text);
  s->win[n].c->rc = rc;
  return 0;
}
static LRESULT mdi_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  mdi_t* s = (mdi_t*)c->x;
  //IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  int i;
  switch (uMsg) {
  case WM_CTRLINIT:
    break;
  case WM_CTRLFREE:
    if (s->win) {
    }
    break;
  case WM_SETFOCUS:
    if (s->num > 0) {
      for (i = 0; i < s->num; ++i) {
      }
    }
    break;
  case WM_SIZE:
    if (s->num > 0) {
      for (i = 0; i < s->num; ++i) {
        //s->win[i].rect[1] = s->base.rc;
        //s->win[i].base.rc = s->win[i].rect[0];
      }
    }
    break;
  }
  return 0;
}

