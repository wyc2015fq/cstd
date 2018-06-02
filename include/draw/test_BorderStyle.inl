typedef struct test_BorderStyle_t {
  UI_engine ui[1];
  rbox_ctrl m_style[1];
  cbox_ctrl m_cbox[1];
  slider_ctrl m_slider_iter[1];
} test_BorderStyle_t;
const char* BorderStyleName[] = {
  "Sunken",
  "Low",
  "Up",
  "High",
  "Simple",
  "Simple_dot",
  "Flat",
  "Frame",
  "Thin Frame",
  "Light Frame",
  "Thin Light Frame",
  "Etched",
  "Etched 2",
};
int test_BorderStyle_paint_proc(test_BorderStyle_t* g, HDDC hDC)
{
  RECT rcClient;
  UIGetWorkArea(g->ui, &rcClient);
  //SelectFontToMemDC("宋体", (int)g->m_slider[0].m_value, 0, 0);
  //SelectFontToMemDC("宋体", (int)16, 0, 0);
  //draw_clear(hDC, _RGB(255,255,255));
  draw_clear(hDC, _GetSysColor(COLOR_BTNFACE));

  //draw_fillrectR(hDC1, &win->rcClient, rgb(1, 1, 1));
  if (1) {
    RECT rc[10] = {0};
    RECT rc2 = rcClient;
    DWORD clrs[] = {_GetSysColor(COLOR_BTNFACE), 0};
    //draw_fillrect_colorsR(hDC, &rc2, clrs, countof(clrs));
    InflateRect(&rc2, -3, -3);
    layout_itR("[0:100,1],2:200", &rc2, 10, 10, rc);
    //drawaa_rbox_ctrl(hDC, g->m_style, rc[0]);
    //drawaa_rbox_ctrl(hDC, g->m_style, rc[2]);
    skin_win2000_rbox_ctrl(hDC, g->m_style);
    test_draw_border(hDC, rc[0]);
    test_draw_frame_ctrl(hDC, rc[1]);

    if (0) {
      static int aa = 0;

      if (0 == aa) {
        LoadFontFromFile("D:\\TDDOWNLOAD\\font\\黑体-繁 樱桃饭团卡通中文字体.TTF");
      }

      SelectFontToMemDC("黑体-繁", 16, 0);
      draw_textR(hDC, "1234ASDF", -1, &rc[2], DT_CENTER | DT_BOTTOM, _RGB(0, 0, 0));
    }
  }

  return 0;
}
LRESULT test_BorderStyle_frame_proc(test_BorderStyle_t* g, event* e)
{
  rbox_ctrl_event(g->m_style, e);
  slider_ctrl_event(g->m_slider_iter, e);
  return 0;
}
int test_BorderStyle()
{
  test_BorderStyle_t g[1] = {0};
  rbox_ctrl_set(g->m_style, 0, countof(BorderStyleName), BorderStyleName);
  UI_set(g->ui, "test_BorderStyle", g, test_BorderStyle_paint_proc, test_BorderStyle_frame_proc, 0, 0);
  waitkey(g->ui, -1);
  FreeMemDC();
  return 0;
}

