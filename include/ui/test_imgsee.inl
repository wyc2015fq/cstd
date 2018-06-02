#include "img/imgcache.inl"
#define IMGSEE_ROW 6
#define IMGSEE_COL 8
#define IMGSEE_PICS (IMGSEE_ROW*IMGSEE_COL)
#define IMGSEE_RANGE_PAGE 2
typedef struct test_imgsee_t {
  ctrl_t c[1];
  int hit;
  int cur;
  int hot;
  imgcache_t sv[1];
  int islist;
} test_imgsee_t;
int test_imgsee_set_cur(test_imgsee_t* s, int cur)
{
  if (s->sv->n > 0) {
    cur = (cur + s->sv->n) % s->sv->n;
    s->cur = cur;
    printf("test_imgsee_set_cur %d\n", cur);
    imgcache_set_range(s->sv, cur, IMGSEE_RANGE_PAGE, IMGSEE_RANGE_PAGE, IMGSEE_PICS);
  }
  return 0;
}
int test_imgsee_set_path(test_imgsee_t* s, const char* path, const char* exts)
{
  imgcache_set_path(s->sv, path, exts);
  test_imgsee_set_cur(s, 0);
  printf("%d\n", s->sv->n);
  return 0;
}
int test_imgsee_imgcache_eventproc(void* ss, int x)
{
  test_imgsee_t* s = (test_imgsee_t*)ss;
  ctrl_t* c = s->c;
  force_redraw(c);
  return 0;
}
int test_imgsee_hittest(test_imgsee_t* s, int x, int y)
{
  IRECT rc1;
  int i, hit = -1;
  if (s->islist) {
    for (i = 0; i < IMGSEE_PICS; ++i) {
      get_grid_rect(s->c->rc, i, IMGSEE_ROW, IMGSEE_COL, &rc1);
      if (iPtInRect(&rc1, x, y)) {
        return i;
      }
    }
  }
  return -1;
}
static LRESULT test_imgsee_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_imgsee_t* s = (test_imgsee_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  switch (uMsg) {
  case WM_CTRLFREE:
    if (1) {
      imgcache_close(s->sv);
      c->ctrlproc = 0;
      return 0;
    }
    break;
  case WM_CTRLINIT:
    if (1) {
      const char* path = NULL;
      const char* exts = ".jpg|.gif";
      s->islist = 1;
      path = "E:/www/mv/aadyw.cc/jpg";
      path = "E:/www/gif/addictedtoeveythingwomen.tumblr.com/ass";
      path = "E:/pub/bin/face/bin/20151123-20151126二幼总";
      path = "E:/www/gif/addictedtoeveythingwomen.tumblr.com/ass/allkindsofass21.tumblr.com";
      path = "E:/www/gif/addictedtoeveythingwomen.tumblr.com/ass";
      path = "C:/www/www.aitaotu.com/jpg";
      path = "E:/www/gif/addictedtoeveythingwomen.tumblr.com/ass/ilovethebooty2.tumblr.com";
      path = "E:/www/gif/bootyoftheday.co/bootyoftheday.co";
      path = "G:/pic/A4u 52CD 全套圖片打包 25.8g";
      path = "G:/pic/360Downloads";
      path = "G:/pic/Chloe Vevrier sets";
      path = "G:/pic/gif";
      path = "G:/pic/jpg";
      path = "E:/www/gif/addictedtoeveythingwomen.tumblr.com/_new";
      path = "E:/www/www1/www.aitaotu.com/jpg";
      exts = ".bmp";
      exts = ".gif";
      exts = ".jpg|.gif|.png";
      s->sv->fun = test_imgsee_imgcache_eventproc;
      s->sv->user = s;
      imgcache_open(s->sv);
      //test_imgsee_set_path(s, path, ".jpg|.gif");
      test_imgsee_set_path(s, path, exts);
      return 0;
    }
    break;
  case WM_TIMER:
    force_redraw(c);
    break;
  case WM_GETMINMAXINFO:
    if (1) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      //c->sub->ctrlproc(c, c->sub, hwnd, uMsg, wParam, lParam);
      mminfo->ptMinTrackSize.x += 420;
      mminfo->ptMinTrackSize.y += 420;
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
    }
    break;
  case WM_COMMAND:
    if (s) {
      if (XCMD_CHANGE == wParam) {
        force_redraw(c);
      }
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      if (s->islist) {
        int cur_page = s->cur / IMGSEE_PICS;
        int beg = cur_page * IMGSEE_PICS;
        int hit = test_imgsee_hittest(s, x, y);
        if (hit >= 0) {
          test_imgsee_set_cur(s, beg + hit);
        }
      }
      s->islist = !s->islist;
      force_redraw(c);
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
      if (s->islist) {
        int cur_page = s->cur / IMGSEE_PICS;
        int beg = cur_page * IMGSEE_PICS;
        int hit = test_imgsee_hittest(s, x, y);
        if (hit >= 0 && hit != s->hot) {
          s->hot = beg + hit;
          force_redraw(c);
        }
      }
    }
    break;
  case WM_MOUSEWHEEL:
    if (s) {
      int tt = (GET_WHEEL_DELTA_WPARAM(wParam) < 0) ? 1 : -1;
      if (s->islist) {
        tt *= IMGSEE_PICS;
      }
      test_imgsee_set_cur(s, s->cur + tt);
      force_redraw(c);
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int i, n_drawn = 0;
      img_t im1[1] = {0};
      IRECT rc1, rc2, rcTxt, rctmp;
      char buf[256];
      imdraw_fill(im, _rgb(0, 0, 0));
      if (s->islist) {
        int cur_page = s->cur / IMGSEE_PICS;
        int beg = cur_page * IMGSEE_PICS;
        int end = MIN(beg + IMGSEE_PICS, s->sv->n);
        for (i = beg; i < end; ++i) {
          if (imgcache_getimthumb(s->sv, i, im1)) {
            get_grid_rect(rc, i - beg, IMGSEE_ROW, IMGSEE_COL, &rc1);
            if (1) {
              iRectScaling(iRECT(0, 0, im1->w, im1->h), RCW(&rc1), RCH(&rc1), &rc2);
              imdraw_image(im, &rc1, &rc1, im1, &rc2);
            }
            else {
              iRectScaling(rc1, im1->w, im1->h, &rc1);
              imdraw_image(im, &rc1, &rc1, im1, 0);
            }
            if (1) {
              rctmp = rc1;
              iRectCutT(&rctmp, -20, 0, &rcTxt);
              imdraw_rect(im, &rcTxt, rcTxt, _rgba(0, 0, 0, 0.4), 0, 0, BDM_ALL);
              _snprintf(buf, 256, "%d %d %d %d", i, s->sv->v[i].h, s->sv->v[i].w, s->sv->v[i].f);
              imdraw_text(im, &rcTxt, rcTxt, buf, -1, c->fo, _rgb(1, 1, 1), 0, DT_CENTER | DT_VCENTER);
            }
            if (i == s->cur) {
              imdraw_rect(im, &rc1, rc1, 0, _RGB(255, 0, 0), 2, BDM_ALL);
            }
            if (i == s->hot) {
              imdraw_rect(im, &rc1, rc1, 0, _RGB(0, 255, 0), 2, BDM_ALL);
            }
          }
        }
      }
      else {
        i = s->cur;
        if (imgcache_getim(s->sv, i, im1)) {
          iRectScaling(rc, im1->w, im1->h, &rc1);
          imdraw_image(im, &rc1, &rc1, im1, 0);
          if (1) {
            rctmp = rc1;
            iRectCutT(&rctmp, -20, 0, &rcTxt);
            imdraw_rect(im, &rcTxt, rcTxt, _rgba(0, 0, 0, 0.4), 0, 0, BDM_ALL);
            _snprintf(buf, 256, "%d %d %d %d", i, s->sv->v[i].h, s->sv->v[i].w, s->sv->v[i].f);
            imdraw_text(im, &rcTxt, rcTxt, buf, -1, c->fo, _rgb(1, 1, 1), 0, DT_CENTER | DT_VCENTER);
          }
        }
      }
      imfree(im1);
      //imdrawui_default_polygon(uidrawer, im, m_poly1);
      //imdrawui_default_polygon(uidrawer, im, m_poly2);
    }
    break;
  }
  return 0;
}

