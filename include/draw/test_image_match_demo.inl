typedef struct test_image_match_demo_t {
  UI_engine ui[1];
  uiwindow win[1];
  slider_ctrl m_slider[2];
  button_ctrl m_btn[2];
  progress_ctrl m_progress[1];
  edit_ctrl m_edit[2];
  img_t m_im[10];
  img_t m_imfirst[1];
} test_image_match_demo_t;
int test_image_match_demo_paint_proc(test_image_match_demo_t* g, HDDC hDC)
{
  //SelectFontToMemDC("宋体", (int)g->m_slider[0].m_value, 0, 0);
  SelectFontToMemDC("宋体", (int)16, 0);
  draw_clear(hDC, 0);
  draw_uiwindow(hDC, g->win);
  HDDCSET_RECT(hDC, hDC, &g->win->rcClient);

  //draw_fillrectR(hDC1, &win->rcClient, rgb(1, 1, 1));
  if (1) {
    RECT rc[10] = {0};
    RECT rc2 = g->win->rcClient;
    int i = 0;
    InflateRect(&rc2, -3, -3);
    layout_itR("[3:200]:200,[[0,1,4,5]:20,[2,,,]:30,]", &rc2, 10, 10, rc);

    if (1) {
      drawaa_slider_ctrl(hDC, g->m_slider + 0);
      drawaa_slider_ctrl(hDC, g->m_slider + 1);
      draw_button_ctrl(hDC, g->m_btn);

      if (g->m_imfirst->data) {
        draw_imageR(hDC, &rc[i], g->m_imfirst, 0, 0, 0);
      }

      ++i;
      drawaa_edit_ctrl(hDC, g->m_edit);
      g->m_progress[0].m_value = g->m_slider[1].m_value;
      drawaa_progress_ctrl(hDC, g->m_progress);
    }

    if (0) {
#include "arrinit.inl"
      COLORREF clrs[10] = {0};
      COLORREF clrarr[256] = {0};
      double pos[10] = {0};
      DPOINT pt[100];
      int n;
      fill_color_array2(256, clrarr, 5,
          ARRINIT5(clrs, 0xFF008000, 0xFFFFFF00, 0xFFFF0000, 0xFF0000FF, 0xFFFFA500),
          ARRINIT5(pos, 0, 0.25, 0.5, 0.75, 1));
      n = vcgen_rect(pt, 200, 200, 400, 300);
      drawaa_fillpoly_lut(hDC, pt, &n, 1, 200, 200, 400, 400, clrarr);
    }

    if (1) {
      LPITEMIDLIST itemlist = NULL;
      img_t im[2] = {0};
      RECT rc[1] = {300, 300, 400, 400};
      TCHAR buf[MAX_PATH];
      SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &itemlist);
      SHGetPathFromIDList(itemlist, buf);
      strcpy(buf, "D:\\360Downloads\\Cut_It_win.zip");
      strcpy(buf, "D:\\a.bat");
      GetFileIcon(buf, im, SHGFI_LARGEICON);
      //imsettranscolor(im, _RGB(0,0,0));
      //rc->right=rc->left+im->width;
      //rc->bottom=rc->top+im->height;
      draw_imageR(hDC, rc, im, 0, 0, 0);
    }
  }

  return 0;
}
LRESULT test_image_match_demo_frame_proc(test_image_match_demo_t* g, event* e)
{
  uiwindow* win;
  int hit = 0;
  int ret;
  UI_engine* ui = toUI(e);
  HWND hwnd = ui->hwnd;
  win = g->win;
  ret = uiwindow_event(g->win, e);

  if (ret) {
    return ret;
  }

  slider_ctrl_event(g->m_slider + 0, e);
  slider_ctrl_event(g->m_slider + 1, e);
  progress_ctrl_event(g->m_progress, e);
  button_ctrl_event(g->m_btn, e);
  edit_ctrl_event(g->m_edit, e);

  //slider_ctrl_event(&m_slider+1, event, x, y, flags, param);
  //x -= win->rcClient.left;
  //y -= win->rcClient.top;
  //System_TraceMsg(hwnd, uMsg, wParam, lParam);
  switch (e->msg) {
  case WM_LBUTTONUP: {
  }
  break;

  case WM_LBUTTONDBLCLK: {
    int asdf = 0;
  }
  break;

  case WM_LBUTTONDOWN:
    if (1) {
      //force_redraw(e);
      POINT pt = {e->x, e->y};

      if (PtInRect(&g->m_btn->base.rect, pt)) {
        int asdf = 0;
        char* fn = myGetOpenFileName("*.jpg;*.bmp");
        printf("%s\n", fn);
        GetImage(g->m_imfirst, fn, 0);
        //imsetalpha(g->m_imfirst, 255);
      }
    }

    break;
  }

  return 0;
}
int test_image_match_demo()
{
  test_image_match_demo_t g[1] = {0};
  {
    RECT padd[1] = {30, 40, 30, 40};
    COLORREF trans_color = _RGB(255, 0, 255);
    char* files[10];
    img_t* im = g->m_im;
#define RESPATH1(FN) "D:/pub/bin/imgproc/draw/res/"FN
#define RESPATH(FN) "D:/pub/bin/imgproc/draw/res/SkinSE_QQ2009/skin/"FN

    if (0) {
      ARRINIT4(files, RESPATH("frame_btn_close_normal.bmp"), RESPATH("frame_btn_close_hot.bmp"), RESPATH("frame_btn_close_down.bmp"), RESPATH("frame_btn_close_disable.bmp"));
      GetImages(im, 4, files, trans_color);
      ++im;
      ARRINIT4(files, RESPATH("frame_btn_max_normal.bmp"), RESPATH("frame_btn_max_hot.bmp"), RESPATH("frame_btn_max_down.bmp"), RESPATH("frame_btn_max_disable.bmp"));
      GetImages(im, 4, files, trans_color);
      ++im;
      ARRINIT4(files, RESPATH("frame_btn_min_normal.bmp"), RESPATH("frame_btn_min_hot.bmp"), RESPATH("frame_btn_min_down.bmp"), RESPATH("frame_btn_min_disable.bmp"));
      GetImages(im, 4, files, trans_color);
      ++im;
      GetImage(im, RESPATH("windowBk.bmp"), trans_color);
      SetRect(padd, 30, 40, 30, 40);
      uiwindow_set(g->win, 0, "商标比对系统", im, padd, im - 3, im - 2, im - 1);
      ++im;
      ARRINIT5(files, RESPATH("button_normal.bmp"), RESPATH("button_hot.bmp"), RESPATH("button_focus.bmp"), RESPATH("button_down.bmp"), RESPATH("button_disable.bmp"));
      GetImages(im, 5, files, trans_color);
      SetRect(padd, 4, 4, 4, 4);
      button_ctrl_set(g->m_btn, "选择图片", im, padd);
      ++im;
    }
    else {
      uiwindow_set(g->win, 0, "商标比对系统", 0, 0, 0, 0, 0);
      button_ctrl_set(g->m_btn, "选择图片", 0, 0);
    }

    slider_ctrl_set(g->m_slider + 0, "Pixel size=%1.0f", 17.0, 8.0, 100.0, 23);
    slider_ctrl_set(g->m_slider + 1, "Gamma=%4.3f" , 0.1, 0.0, 1.0, 0);
    progress_ctrl_set(g->m_progress, "%4.1f", 0.2);
  }
  //g->win->hwnd = Win_Create(0, test_image_match_demo_WndProc, g);
  UI_set(g->ui, "test_image_match_demo", g, test_image_match_demo_paint_proc, test_image_match_demo_frame_proc, 0, 0);
  waitkey(g->ui, -1);
  freeims(g->m_im, countof(g->m_im));
  return 0;
}

