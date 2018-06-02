
//#include "geo/clipper/clipper.inl"
//#include "geo/gpc/gpc.inl"
#include "draw/region.inl"

int spanv_ctrl(ctrl_t* c, const spanv_t* sl, double* m3a, COLOR clr)
{
  SYSIO;
  int i = 0, changed = 0;
  int hot = 0;
  static IPOINT pos = {0};
  if (ISHOVEREDWIN2()) {
    static FPOINT pos2;
    //hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        if (hot) {
          //xSendCmd(XCMD_CLICK);
        }
      }
      break;
    case MSG_LBUTTONDOWN: {
      hot = point_in_spanv(sl, x, y);
      if (hot) {
        SETHIT(c);
        pos = iPOINT(x, y);
        FORCE_REDRAW(c->rc);
        SETFOCUS(c);
      }
    }
    break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        //IPOINT pos = iPOINT(x - pos.x, y - pos.y);
        affine_info_t ai1[] = {GEO_TRANS, pos.x, pos.y};
        V9SET(m3a, 1, 0, 0, 0, 1, 0, 0, 0, 1);
        affine_info_trans(countof(ai1), ai1, m3a);
        changed = 1;
      }
      break;
    }
  }
  return changed;
}
int polygon_impl(ctrl_t* c, FPOINT* pts, int* len, int n, double m_point_radius, int hitmark, FPOINT* pos)
{
  SYSIO;
  int i = 0, changed = 0;
  int hot = 0;
  static FPOINT pt = {0};
  if (ISHOVEREDWIN2()) {
    static FPOINT pos2;
    //hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        if (hot) {
          //xSendCmd(XCMD_CLICK);
        }
      }
      break;
    case MSG_LBUTTONDOWN: {
      hot = point_in_polygon_hittest2(pts, len, n, x, y, m_point_radius, hitmark);
      if (hot) {
        SETHIT(c);
        pt.x = x - pos->x;
        pt.y = y - pos->y;
        FORCE_REDRAW(c->rc);
        SETFOCUS(c);
      }
    }
    break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        pos->x = x - pt.x;
        pos->y = y - pt.y;
        {
          double m3a[9] = {0};
          affine_info_t ai1[] = {GEO_TRANS, pos->x, pos->y};
          V9SET(m3a, 1, 0, 0, 0, 1, 0, 0, 0, 1);
          affine_info_trans(countof(ai1), ai1, m3a);
          changed = 1;
        }
      }
      break;
    }
  }
  return changed;
}
CC_INLINE int point_in_path_hittest(const FPOINT* Points, const BYTE* Types, int Count, double x, double y, double m_point_radius, int hitmark) {
  int ret = 0;
  PATH_LOOP_BEGIN;
  if (ret = point_in_polygon_hittest(Points + pos, end-pos, x, y, m_point_radius, hitmark)) {
    return ret;
  }
  PATH_LOOP_END;
  return 0;
}

int path_impl(ctrl_t* c, FPOINT* Points, const BYTE* Types, int Count, double m_point_radius, int hitmark, FPOINT* pos)
{
  SYSIO;
  int i = 0, changed = 0;
  int hot = 0;
  static FPOINT pt = {0};
  if (ISHOVEREDWIN2()) {
    static FPOINT pos2;
    //hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        if (hot) {
          //xSendCmd(XCMD_CLICK);
        }
      }
      break;
    case MSG_LBUTTONDOWN: {
      hot = point_in_path_hittest(Points, Types, Count, x, y, m_point_radius, hitmark);
      if (hot) {
        SETHIT(c);
        pt.x = x - pos->x;
        pt.y = y - pos->y;
        FORCE_REDRAW(c->rc);
        SETFOCUS(c);
      }
                          }
      break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        pos->x = x - pt.x;
        pos->y = y - pt.y;
        {
          double m3a[9] = {0};
          affine_info_t ai1[] = {GEO_TRANS, pos->x, pos->y};
          V9SET(m3a, 1, 0, 0, 0, 1, 0, 0, 0, 1);
          affine_info_trans(countof(ai1), ai1, m3a);
          changed = 1;
        }
      }
      break;
    }
  }
  return changed;
}
int test_scanline_boolean2(ctrl_t* c)
{
  SYSIO;SOFTGC_ARG;
  static Path poly[3] = {0};
  static FPOINT pos[2] = {0};
  static ctrl_t cc[10] = {0};
  static int inited = 0;
  IRECT rc = c->rc, rcLeft;
  int w = RCW(&rc), h = RCH(&rc);
  static spanv_t sp[3] = {0};
  static region_t re[3] = {0};
  spanv_t* sp1 = sp + 1;
  spanv_t* sp2 = sp + 2;
  region_t* re1 = re + 1;
  region_t* re2 = re + 2;
  static double t1 = 0;
  static double times[20];
  static int ntime=0;
  static int itime=0;
  int i;
  COLOR clrs[] = {_rgba(1, 0, 0, 0.5), _rgba(0, 1, 0, 0.5)};
  int left_w = 200;
  static int m_operation = RGN_OR;
  static int m_fill_rule = 1;
  static int m_scanline_type = 1;
  static int poly_id[2] = {gp_Path_1, gp_Britain};
  static int do_aa = 0;
  double t2 = 0;
  utime_start(_start_time);
  iRectCutL(&rc, left_w, 0, &rcLeft);
  if (!inited) {
    inited = 1;
  }
  utime_restart(_start_time);
  for (i = 0; i < 2; ++i) {
    int bord = 100;
    int npt = get_poly((poly_id_t)poly_id[i], poly + i);
    pt_center(npt, poly[i].Points, pos[i].x + rc.l + bord, pos[i].y + rc.t + bord, w - 2 * bord, h - 2 * bord, 1, -1);
    path_impl(c + i, poly[i].Points, poly[i].Types, poly[i].Count, 0, POHIT_FACE, pos + i);
    brush_set_solid(sg->brush, clrs[i]);
#if 0
    tic;
    if (do_aa) {
      spanv_set_path(sp + i, im, NULL, poly[i].Points, poly[i].Types, poly[i].Count, false);
      scanline_set_spanv(sg->sl, sp + i);
    }
    else {
      region_set_path(re + i, poly[i].Points, poly[i].Types, poly[i].Count, 0);
      scanline_set_region(sg->sl, re + i);
    }
    toc;
#endif
    //imdrawaa_poly(im, NULL, poly[i].Points, poly[i].len, poly[i].n, 0, solid_fun, clrs[i]);
    //imdrawaa_poly(im, NULL, poly[i].Points, poly[i].len, poly[i].n, 0, solid_fun, _rgb(1,0,0));
  }
  {
    boolopt opt = (boolopt)m_operation;
    tic;
    if (do_aa) {
      spanv_boolopt(sp2, sp, sp1, opt);
    }
    else {
      region_boolopt(re2, re, re1, opt);
    }
    toc;
#if 0
    if (0) {
      Clipper clpr;
      Paths ppg0, ppg1, ppg2;
      double scale = 1<<16;
      DPOLYGON2Paths(poly+0, ppg0, scale);
      DPOLYGON2Paths(poly+1, ppg1, scale);
      
      tic;
      clpr.AddPaths(ppg0, ptSubject, true);
      clpr.AddPaths(ppg1, ptClip, true);
      int ret = clpr.Execute(ctIntersection, ppg2, pftEvenOdd, pftEvenOdd);
      toc;
      Paths2DPOLYGON(ppg2, poly+2, scale);
    }
#endif
    //spanv_and(sp, sp1, sp2);
  }
  {
    static ctrl_t cc[10] = {0};
    static const char* s_operation = "|"
#define BOOLOPT_DEF(name) #name "|"
        BOOLOPT_DEF_DEF(BOOLOPT_DEF)
#undef BOOLOPT_DEF
        ;
    i = 0;
    flexboxlay(rcLeft, countof(cc), cc, 2, flex_column);
    //printf("time1 %5.2fms %5.f %5.f %5.f\n", t1 * 1000, sp->l * 100. / sp->max_l, sp1->l * 100. / sp1->max_l, sp2->l * 100. / sp2->max_l);
    fmtlabel_ctrl(cc + i++, "time1 %5.2fms %5.f %5.f %5.f", t1 * 1000,
        sp->l * 100. / sp->max_l, sp1->l * 100. / sp1->max_l, sp2->l * 100. / sp2->max_l);
    cbox_ctrl(cc + i++, "do_aa", &do_aa);
    rbox_ctrl(cc + i++, "operation", s_operation, &m_operation);
    //rbox_ctrl(cc+i++, "fill_rule", "|Even-Odd|Non Zero|", &m_fill_rule);
    //rbox_ctrl(cc+i++, "scanline_type", "|scanline_p|scanline_u|scanline_bin|", &m_scanline_type);
    rbox_ctrl(cc + i++, "A", s_get_poly, poly_id + 0);
    rbox_ctrl(cc + i++, "B", s_get_poly, poly_id + 1);
#if 0
    {
      ctrl_t* minishow = cc + i++;
      IRECT rcs[2];
      //const char* strs[] = {"A", "B", "C"};
      iRectMatrix(minishow->rc, 1, 2, 0, 0, 0, 2, rcs);
      for (i = 0; i < 2; ++i) {
        int npt = dPOLYGON_npt(poly + i);
        IRECT rc1 = iRectIn(rcs[i], -2);
        pt_center(npt, poly[i].Points, rc1.l, rc1.t, RCW(&rc1), RCH(&rc1), 1, 1);
        imdrawaa_poly_solid(im, flip_y, NULL, poly[i].Points, poly[i].len, poly[i].n, _A_RGB(255, clrs[i]));
      }
      minishow->sz = iSIZE(left_w, left_w / 2);
    }
#endif
  }
  //solid_fillspan(im, sp2, _rgba(0, 0, 1, 0.5));
#if 0
  if (1) {
    if (do_aa) {
      //spanv_hatch(sp2, im, flip_y, 0, 0, _rgba(0, 0, 1, 0.5), _rgba(0, 1, 1, 0.5), g_hatch_datas[HatchStyleHorizontalBrick]);
      spanv_solid(sp + i, im, flip_y, _rgba(0.2, 0.4, 0.9, 0.9));
    }
    else {
      region_hatch(re2, im, flip_y, 0, 0, _rgba(0, 0, 1, 0.5), _rgba(0, 1, 1, 0.5), g_hatch_datas[HatchStyleHorizontalBrick]);
    }
  } else {
    i = 2;
    imdrawaa_poly_solid(im, flip_y, NULL, poly[i].pt, poly[i].len, poly[i].n, _rgba(0.2, 0.4, 0.9, 0.9));
  }
  t1 = utime_elapsed(_start_time);
#endif
  spanv_resets(sp, countof(sp));
  //tictoc;
  //spanv_frees(sp, countof(sp));
  return 0;
}


