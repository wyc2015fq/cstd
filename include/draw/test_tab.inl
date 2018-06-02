typedef struct test_tab_t {
  UI_engine ui[1];
  uiwindow m_win[1];
  rbox_ctrl m_style[1];
  TabCtrl m_tab[1];
  OutlookTabCtrl m_outlook[1];
  layout_ctrl m_lay[1];
} test_tab_t;
int test_tab_paint_proc(test_tab_t* g, HDDC hDC)
{
  RECT rcClient;
  UIGetWorkArea(g->ui, &rcClient);
  draw_uiwindow(hDC, g->m_win);
  rcClient = g->m_win->rcClient;
  draw_layout_ctrl(hDC, g->m_lay);

  //SelectFontToMemDC("ËÎÌו", (int)g->m_slider[0].m_value, 0, 0);
  //SelectFontToMemDC("ËÎÌו", (int)16, 0, 0);
  //draw_clear(hDC, rgb(1, 0.95, 0.95));
  //draw_fillrectR(hDC1, &win->rcClient, rgb(1, 1, 1));
  if (1) {
    double t;
    draw_rbox_ctrl(hDC, g->m_style);
    TabCtrl_setStyle(g->m_tab, g->m_style->m_cur_item);
    {
      utime_start(_start_time);
      TabCtrl_OnPaint(hDC, g->m_tab);
      OutlookTabCtrlBase_OnPaint(hDC, g->m_outlook);
      t = 1000 * utime_elapsed(_start_time);
    }
  }

  return 0;
}
LRESULT test_tab_frame_proc(test_tab_t* g, event* e)
{
  if (uiwindow_event(g->m_win, e)) {
    return TRUE;
  }

  layout_ctrl_event(g->m_lay, e);
  TabCtrl_event(g->m_tab, e);
  OutlookTabCtrl_event(g->m_outlook, e);
  rbox_ctrl_event(g->m_style, e);
  return 0;
}
int test_tab()
{
  test_tab_t g[1] = {0};
  layitem_t li[] = {
    0, 0, 0, &g->m_style->base,
    'r', 300, 0, &g->m_tab->base,
    'r', 300, 0, &g->m_outlook->base,
  };
  char* m_style_items[] = {
#define TABCTRLSTYLEDEF(x) #x
#include "TabCtrlStyle.txt"
  };
  uiwindow_set(g->m_win, 0, "test_tab", 0, 0, 0, 0, 0);
  layout_ctrl_set(g->m_lay, 4, countof(li), li);
  rbox_ctrl_set(g->m_style, 1, countof(m_style_items), m_style_items);
  TabCtrl_set(g->m_tab);
  OutlookTabCtrl_set(g->m_outlook);
  {
    int n = 0;
    OutlookTabCtrlItem* items = g->m_outlook->m_vtritems;
#undef RESPATH
#define RESPATH(x) "D:\\code\\c\\tab\\OutlookTabCtrl_src\\res\\"x
    GetImageList(RESPATH("TabBig.bmp"), g->m_outlook->m_ImageListBig, _RGB(255, 0, 255));
    GetImageList(RESPATH("TabDisabledBig.bmp"), g->m_outlook->m_ImageListDisabledBig, _RGB(255, 0, 255));
    GetImageList(RESPATH("TabSmall.bmp"), g->m_outlook->m_ImageListSmall, _RGB(255, 0, 255));
    GetImageList(RESPATH("TabDisabledSmall.bmp"), g->m_outlook->m_ImageListDisabledSmall, _RGB(255, 0, 255));
#define ADDITEM(s, a, b) {strcpy(items[n].text, s); items[n].imageBig=a; items[n].imageSmall=b; items[n].visible=TRUE; ++n;};
    ADDITEM("Mail", 0, 0);
    ADDITEM("Calendar", 1, 1);
    ADDITEM("Modeless Dialog Box", -1, -1);
    ADDITEM("Contacts", 2, 2);
    ADDITEM("Tasks", 3, 3);
    ADDITEM("Business Affairs", -1, -1);
    ADDITEM("Notes", 4, 4);
    ADDITEM("Folder List", 5, 5);
    ADDITEM("Shortcuts", 6, 6);
    g->m_outlook->m_vtritemsLen = n;
  }
  UI_set(g->ui, "test_tab", g, test_tab_paint_proc, test_tab_frame_proc, 0, 0);
  waitkey(g->ui, -1);
  FreeMemDC();
  imfree(g->m_outlook->m_ImageListBig);
  imfree(g->m_outlook->m_ImageListDisabledBig);
  imfree(g->m_outlook->m_ImageListSmall);
  imfree(g->m_outlook->m_ImageListDisabledSmall);
  return 0;
}

