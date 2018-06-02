// http://codegolf.stackexchange.com/questions/35569/tweetable-mathematical-art
#define DIM 1024
#define DM1 (DIM-1)
#define _sq(x) ((x)*(x)) // square
#define _cb(x) abs((x)*(x)*(x)) // absolute value of cube
#define _cr(x) (unsigned char)(pow((x),1.0/3.0)) // cube root
int tweetable_mathematical_art(int style, uchar* img, int step, int cn)
{
  int i, j;
  switch (style) {
  case 0:
    for (j = 0; j < DIM; ++j) {
      uchar* rgb0 = img + j * step;
      for (i = 0; i < DIM; ++i, rgb0 += cn) {
        rgb0[0] = (char)(_sq(cos(atan2(j - 512, i - 512) / 2)) * 255);
        rgb0[1] = (char)(_sq(cos(atan2(j - 512, i - 512) / 2 - 2 * acos(-1) / 3)) * 255);
        rgb0[2] = (char)(_sq(cos(atan2(j - 512, i - 512) / 2 + 2 * acos(-1) / 3)) * 255);
      }
    }
    break;
  case 1:
    for (j = 0; j < DIM; ++j) {
      uchar* rgb0 = img + j * step;
      for (i = 0; i < DIM; ++i, rgb0 += cn) {
        rgb0[0] = (unsigned char)sqrt((double)(_sq(i - DIM / 2) * _sq(j - DIM / 2)) * 2.0);
        rgb0[1] = (unsigned char)sqrt((double)((_sq(i - DIM / 2) | _sq(j - DIM / 2)) * (_sq(i - DIM / 2)&_sq(j - DIM / 2))));
        rgb0[2] = (unsigned char)sqrt((double)(_sq(i - DIM / 2)&_sq(j - DIM / 2)) * 2.0);
      }
    }
    break;
  case 2:
    for (j = 0; j < DIM; ++j) {
      uchar* rgb0 = img + j * step;
      for (i = 0; i < DIM; ++i, rgb0 += cn) {
        rgb0[0] = i && j ? (i % j) & (j % i) : 0;
        rgb0[1] = i && j ? (i % j) + (j % i) : 0;
        rgb0[2] = i && j ? (i % j) | (j % i) : 0;
      }
    }
    break;
  default:
    for (j = 0; j < DIM; ++j) {
      uchar* rgb0 = img + j * step;
      for (i = 0; i < DIM; ++i, rgb0 += cn) {
        rgb0[0] = 0;
        rgb0[1] = 0;
        rgb0[2] = 0;
      }
    }
    break;
  }
  return 0;
}
static LRESULT test_tweetable_mathematical_art_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  typedef struct test_tweetable_mathematical_art_t {
    ctrl_t lb1[1];
    ctrl_t lay1[1];
    ctrl_t nul0[1];
    uchar img[DIM* DIM * 3];
  } test_tweetable_mathematical_art_t;
  test_tweetable_mathematical_art_t* s = (test_tweetable_mathematical_art_t*)c->x;
  //IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  listbox_t* lb1 = 0;
  if (WM_CTRLINIT == uMsg) {
    char* lay =
        " xxx;"
        "x%-%;"
        ;
    ctrl_set(s->nul0, null_proc, 0);
    ctrl_set(s->lb1, listbox_proc, 0);
    ctrl_set(s->lay1, layout_proc, "gap:i;lay:s", 4, lay, s->nul0, s->lb1);
    listbox_set_item_count(s->lb1, 10);
    return 0;
  }
  sub = s->lay1;
  if (sub && sub->ctrlproc && WM_SET != uMsg && WM_SIZE != uMsg && WM_PAINT != uMsg) {
    if (sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam)) {
      //return 0;
    }
  }
  lb1 = (listbox_t*)s->lb1->x;
  switch (uMsg) {
  case WM_COMMAND:
    break;
  case WM_GETMINMAXINFO:
    if (1) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      mminfo->ptMinTrackSize.x = 400;
      mminfo->ptMinTrackSize.y = 300;
      //printf("WM_MOUSEMOVE\n");
    }
    break;
  case WM_SIZE:
    if (1) {
      sub->rc = c->rc;
      sub->redraw = 1;
      sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam);
      force_redraw(c);
    }
    break;
  case WM_TIMER:
    if (sub) {
      force_redraw(c);
    }
    break;
  case WM_MEASUREITEM:
    if (sender == s->lb1) {
      ISIZE* ds = (ISIZE*)lParam;
      ds->h = g_scr->font->h;
      ds->h = 24;
      ds->w = 100;
    }
    break;
  case WM_DRAWITEM:
    if (sender == s->lb1) {
      drawitem_t* ds = (drawitem_t*)lParam;
      char buf[256];
      COLOR clrBk = rgb(1, 1, 1);
      COLOR clrText = rgb(0, 0, 0);
      _snprintf(buf, 256, "item %d", wParam);
      if (ds->state & ODS_SELECTED) {
        clrBk = _RGB(0, 0, 255);
        clrText = rgb(1, 1, 1);
      }
      imdraw_rect(ds->im, 0, ds->itemrc, clrBk, 0, 0, 0);
      imdraw_text(ds->im, ds->cliprc, ds->itemrc, buf, -1, g_scr->font, clrText, 0, TT_VCENTER);
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int n_drawn = 0;
      img_t im1[1] = {0};
      imdraw_fill(im, rgb(1, 0, 0));
      {
        utime_start(_start_time);
        tweetable_mathematical_art(lb1->hit, s->img, DIM * 3, 3);
        printf("%f\n", utime_elapsed(_start_time));
      }
      IMINIT(im1, DIM, DIM, s->img, DIM * 3, 3, 1);
      imdraw_image(im, &s->nul0->rc, &s->nul0->rc, im1, 0);
      sub->redraw = c->redraw;
      sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam);
      c->redraw = 0;
    }
    break;
  }
  return 0;
}

