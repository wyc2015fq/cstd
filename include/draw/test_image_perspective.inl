#include "rand_text.inl"
typedef struct test_image_perspective_t {
  UI_engine ui[1];
  uiwindow m_win[1];
  rbox_ctrl m_trans_type[1];
  tab_ctrl m_tab[1];
  slider_ctrl m_sss[1];
  polygon_ctrl m_quad[1];
  img_t im[1];
  layout_ctrl m_lay[1];
  button_ctrl m_btn1[1];
  tree_ctrl m_tree[1];
} test_image_perspective_t;
int test_image_perspective_event_proc(test_image_perspective_t* g, event* e)
{
  tree_ctrl* lb = g->m_tree;
  int icon;
  img_t im[1] = {0};
  UINT uFlag;
  uFlag = SHGFI_LARGEICON;
  uFlag = SHGFI_SMALLICON;

  if (e->msg == EVENT_INIT) {
    const char* root = "C:\\";
    icon = GetFileIcon(root, im, uFlag);
    tree_ctrl_insert(lb, 0, "C:", 0, icon, im, UIS_HASCHILD);
    tree_ctrl_insert(lb, 1, "D:", 0, icon, im, UIS_HASCHILD);
  }

  if (g->m_tree == e->sender && e->msg == EVENT_LCLICK) {
    int m = lb->m_num_items;
    char buf[256];
    char buf2[256];
    int hit = lb->hit;
    int ind = lb->m_items[hit].ind + 1;
    int chnum = tree_ctrl_get_chi_num(lb, hit);

    if (lb->m_items[hit].m_state & UIS_HASCHILD && 0 == chnum) {
      char* pp = tree_ctrl_get_full_path(lb, hit, "\\");
      chnum = 0;
      printf("%s\n", pp);
      _snprintf(buf, 256, "%s\\*.*", pp);
      begin_find(info, buf);

      if (is_directory(info)) {
        if (filename[0] != '.') {
          _snprintf(buf2, 256, "%s\\%s", pp, filename);
          icon = GetFileIcon(buf2, im, uFlag);
          tree_ctrl_insert(lb, hit + 1, filename, ind, icon, im, UIS_HASCHILD);
          ++chnum;
        }
      }
      else if (1) {
        _snprintf(buf2, 256, "%s\\%s", pp, filename);
        icon = GetFileIcon(buf2, im, uFlag);
        tree_ctrl_insert(lb, hit + 1, filename, ind, icon, im, 0);
        ++chnum;
      }

      end_find(info);
#define TREE_ITEM_LESS(a, b) (((a).m_state>(b).m_state) || ((a).m_state==(b).m_state && stricmp((a).m_text, (b).m_text)<0))
      QSORT((lb->m_items + hit + 1), (lb->m_items + hit + 1 + chnum), TREE_ITEM_LESS, tree_item);
#undef TREE_ITEM_LESS
      free(pp);

      if (0 == chnum) {
        lb->m_items[hit].m_state &= ~UIS_HASCHILD;
      }
    }
  }

  imfree(im);

  if (g->m_btn1 == e->sender && e->msg == EVENT_LCLICK) {
  }

  if (e->msg == EVENT_EXIT) {
    imfree(g->im);
  }

  return 0;
}
int test_image_perspective_paint_proc(test_image_perspective_t* g, HDDC hDC)
{
  RECT rcClient;
  UIGetWorkArea(g->ui, &rcClient);
  //SelectFontToMemDC("ËÎÌå", (int)g->m_slider[0].m_value, 0, 0);
  //SelectFontToMemDC("ËÎÌå", (int)16, 0, 0);
  draw_uiwindow(hDC, g->m_win);
  rcClient = g->m_win->rcClient;

  //draw_clear(hDC, _RGB(255, 255, 255));
  //draw_clear(hDC, _GetSysColor(COLOR_BTNFACE));
  //draw_fillrectR(hDC1, &win->rcClient, rgb(1, 1, 1));
  if (1) {
    RECT rc2 = rcClient;
    DWORD clrs[] = {_GetSysColor(COLOR_BTNFACE), 0};
    //draw_fillrect_colorsR(hDC, &rc2, clrs, countof(clrs));
    InflateRect(&rc2, -3, -3);

    if (0) {
      double t;
      char buf[256];
      {
        utime_start(_start_time);
        trans_perspective p[1];
        trans_perspective_init(p);
        //HDDCSET_CLIP(hDC, rc);
        draw_clear(hDC, _RGB(255, 255, 255));
        trans_perspective_quad_to_rect(p, (double*)g->m_quad->m_polygon, 0, 0, g->im->width, g->im->height);
        drawaa_fillpoly_image_perspective(hDC, g->m_quad->m_polygon, &g->m_quad->m_num_points, 1, g->im, p);
        t = utime_elapsed(_start_time);
      }
      _snprintf(buf, 256, "%.3f", 1000 * t);
      draw_textR(hDC, buf, -1, &rc2, 0, rgb(0, 0, 0));
      HDDCSET_CLIP(hDC, 0);
    }

    //drawaa_polygon_ctrl(hDC, g->m_quad, rc[0]);
    //drawaa_scroll_ctrl(hDC, g->m_scroll, rc[1]);
    {
      RECT rc = g->m_tab->base.rect;
      COLORREF clr[256];
      fill_color_array(30, clr, rgb(1, 0, 0), rgb(0, 0, 1));
      InflateRect(&rc, -20, -20);
      draw_groupbox(hDC, rc, "asdfasdf");
      //draw_rect_implR(hDC, &rc, BDM_LEFT|BDM_TOP|BDM_BOTTOM, clr, 30);
    }

    //drawaa_slider_ctrl(hDC, g->m_sss3, rc[6]);
    //OnDrawTab(hDC, rc[6], 0, STYLE_3D_ROUNDED, 1, rgb(1, 0, 0), 0);
    if (0) {
      int r1 = round(g->m_sss->m_value);
      //OnDrawMenuBorder(hDC, rc[7]);
      //OnDrawBarGripper(hDC, rc[7], 1, 1);
      //draw_hline(hDC, 100, 100, r1, rgb(1, 0, 0));
      //draw_line(hDC, 3, 3, 7, 7, rgb(1, 0, 0));
      //draw_line(hDC, 17, 7, 13, 3, rgb(1, 0, 0));
      {
        DPOINT pts[100] = {1, 0, 1, -1, -1, -1, -1, 0, 0, 1};
        int len = 5;
        poly_offset(pts, len, 2, 2);
        poly_scale(pts, len, 100);
        poly_offset(pts, len, 2, 2);
        //len = vcgen_rect(pts, 100, 100, 100 + r1, 100 + r1) - 1;
        draw_fillpoly(hDC, pts, len, rgb(1, 0, 0));
        //draw_lines(hDC, pts, len, rgb(0, 0, 0), 1);
      }
      //DrawMenuImages(hDC, IdClose, rc[7], 0);
      //draw_ellipseR(hDC, &rc[7], rgb(1, 0, 0), r3);
      //DrawGradientRing(hDC, rc[7], rgb(1, 0, 0), rgb(0, 1, 0), rgb(0, 0, 0), 180, 20, rgb(0,0,1));
      //draw_ellipse(hDC, 0, 0, r1, r2, rgb(1, 0, 0), 0);
    }

    //draw_jiahao(hDC, rc[5], ((int)g->m_sss->m_value), 0xff, rgb(0, 0, 0), _GetSysColor(COLOR_3DSHADOW));
    //HighlightRect(hDC, rc[1], 10*((int)g->m_sss->m_value), rgb(0, 0, 0), 20, rgb(1, 0, 0));
    //GrayRect(hDC, rc[1], 10*((int)g->m_sss->m_value), rgb(0, 0, 0), 20, rgb(1, 0, 0));
    //MirrorRect(hDC, rc[1], 1);
  }

  return 0;
}
int test_image_perspective_frame_proc(test_image_perspective_t* g, event* e)
{
  uiwindow_event(g->m_win, e);
  return 0;
}
int test_image_perspective()
{
  test_image_perspective_t g[1] = {0};
  RECT rc[10];
  layitem_t li[] = {
    0, 0, 0, &g->m_tab->base,
    'r', 100, 0, &g->m_tree->base,
    't', 100, 1, &g->m_trans_type->base,
    't', 20, 1, &g->m_sss->base,
    't', 20, 1, &g->m_btn1->base,
  };
  const char* m_trans_type_items[] = {
    ("Affine Parallelogram"),
    ("Bilinear"),
    ("Perspective"),
  };
  DPOINT pt[4];
  {
    tab_item li[] = {
      -1, "Affine", 0,
      -1, "Bilinear", 0,
      -1, "Perspective", 0,
    };
    tab_ctrl_set(g->m_tab, countof(li), li);
  }
  uiwindow_set(g->m_win, &g->m_lay->base, "test_image_perspective", 0, 0, 0, 0, 0);
  slider_ctrl_set(g->m_sss, "sss=%.f", 3, 0, 200, 0);
  layout_ctrl_set(g->m_lay, 4, countof(li), li);
  tree_ctrl_set(g->m_tree, 0, 0);
  rbox_ctrl_set(g->m_trans_type, 2, countof(m_trans_type_items), m_trans_type_items);
  SetRect(rc, 100, 100, 500, 500);
  vcgen_rectR(pt, rc);
  button_ctrl_set(g->m_btn1, "asdf", 0, 0);
  polygon_ctrl_set(g->m_quad, countof(pt), pt, 1);
  GetImage(g->im, "D:/code/c/Í¼ÐÎÍ¼Ïñ/Anti-Grain Geometry/agg-2.5/examples/win32_api/image_perspective/spheres.bmp", 0);
  UI_set(g->ui, "test_image_perspective", g, test_image_perspective_paint_proc, test_image_perspective_frame_proc, test_image_perspective_event_proc, 0);
  waitkey(g->ui, -1);
  FreeMemDC();
  return 0;
}

