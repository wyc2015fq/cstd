typedef struct test_gradients_t {
  ctrl_t c[1];
  layout_t lay1[1];
  rbox_t m_rbox[1];
  gamma_t m_profile[1];
  spline_t m_r[1];
  spline_t m_g[1];
  spline_t m_b[1];
  spline_t m_a[1];
  DPOINT m_pt[30];
  int m_idx;
  double m_dx;
  double m_dy;
} test_gradients_t;
static LRESULT test_gradients_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_gradients_t* s = (test_gradients_t*)c->x;
  IRECT rc = c->rc;
  int i;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  LRESULT ret = 0;
  switch (uMsg) {
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
  case WM_CTRLINIT:
    if (s) {
      static const char* m_items0[] = {
#define XBRUSHDEF(name) #name ,
#include "draw/xbrushdef.txt"
#undef XBRUSHDEF
      };
      ctrl_set(spline_proc, s->m_r, s->m_r->c, c, WT_CHILD, NULL);
      ctrl_set(spline_proc, s->m_g, s->m_g->c, c, WT_CHILD, NULL);
      ctrl_set(spline_proc, s->m_b, s->m_b->c, c, WT_CHILD, NULL);
      ctrl_set(spline_proc, s->m_a, s->m_a->c, c, WT_CHILD, NULL);
      rbox_set(s->m_rbox, c, countof(m_items0), m_items0);
      gamma_set(s->m_profile, c, 0.786681, 1.23989, 0.726049, 0.710140);
      layout_set(s->lay1, c, 4, 0,
          " xxx;"
          "x ;"
          "a% ;"
          "a% ;"
          "a% ;"
          "a% ;"
          "a% ;"
          "a% ;",
          s->m_rbox->c,
          s->m_profile->c,
          s->m_r->c,
          s->m_g->c,
          s->m_b->c,
          s->m_a->c
                );
      {
        enum {PP_N = 10};
        double pp[PP_N];
        rand_darr(pp, countof(pp), 0, 1);
        spline_set(s->m_r, countof(pp), pp);
        rand_darr(pp, countof(pp), 0, 1);
        spline_set(s->m_g, countof(pp), pp);
        rand_darr(pp, countof(pp), 0, 1);
        spline_set(s->m_b, countof(pp), pp);
        rand_darr(pp, countof(pp), 0, 1);
        spline_set(s->m_a, countof(pp), pp);
      }
      // AGG gradients with Mach bands compensation
      c->sub = s->lay1->c;
      return 0;
    }
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
    mminfo->ptMinTrackSize.x += 640;
    mminfo->ptMinTrackSize.y += 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_SIZE:
    if (1) {
      ctrl_setrect(c->sub, c->rc);
    }
    break;
  case WM_LBUTTONUP:
    if (1) {
      s->m_idx = -1;
      force_redraw(c);
    }
    break;
  case WM_COMMAND:
    if (1) {
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      for (i = 0; i < 4; i++) {
        if (calc_distance(x, y, s->m_pt[i].x, s->m_pt[i].y) < 10.0) {
          s->m_dx = x - s->m_pt[i].x;
          s->m_dy = y - s->m_pt[i].y;
          s->m_idx = i;
          break;
        }
      }
      if (i == 4) {
        if (point_in_poly(s->m_pt, 4, x, y)) {
          s->m_dx = x - s->m_pt[0].x;
          s->m_dy = y - s->m_pt[0].y;
          s->m_idx = 30;
        }
      }
      force_redraw(c);
    }
    break;
  case WM_MOUSEMOVE:
    if (1) {
      if (s->m_idx == 30) {
        double dx = x - s->m_dx;
        double dy = y - s->m_dy;
        poly_offset(s->m_pt, 4, dx - s->m_pt[0].x, dy - s->m_pt[0].y);
        force_redraw(c);
        return 0;
      }
      if (s->m_idx >= 0) {
        s->m_pt[s->m_idx].x = x - s->m_dx;
        s->m_pt[s->m_idx].y = y - s->m_dy;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int n_drawn = 0;
      const double center_x = 450;
      const double center_y = 180;
      const double rrr = 180;
      //int gamma[aa_scale];
      //IRECT rcClient;
      DPOINT pt[64];
      COLOR color_profile[256]; // color_type is defined in pixel_formats.h
      int i, n;
      brush_t br[1] = {0};
      imdraw_fill(im, _rgb(1, 1, 1));
      for (i = 0; i < 256; i++) {
        double r = s->m_r->m_values[i], g = s->m_g->m_values[i], b = s->m_b->m_values[i], a = s->m_a->m_values[i];
        color_profile[i] = _rgba(r, g, b, a);
      }
      fill_color_array(256, color_profile, _rgb(1, 0, 0), _rgb(0, 0, 1));
      n = vcgen_ellipse(pt, countof(pt), 0, center_x, center_y, rrr, rrr);
      switch (s->m_rbox->hit) {
      case br_solid:
        brush_set_solid(br, color_profile[0]);
        break;
#if 0
      case br_gradient:
        brush_set_gradient(br, center_x - rrr, center_y - rrr, center_x + rrr, center_y + rrr, color_profile[0], color_profile[255]);
        break;
#endif
      case br_lut_gradient:
        brush_set_lut_gradient(br, center_x - rrr, center_y - rrr, center_x + rrr, center_y + rrr, color_profile);
        break;
      case br_lut_radial:
        brush_set_lut_radial(br, center_x, center_y, rrr, rrr, color_profile);
        break;
      case br_lut_swirled:
        brush_set_lut_swirled(br, center_x, center_y, rrr, rrr, color_profile);
        break;
      case br_lut_charicature:
        brush_set_lut_charicature(br, center_x, center_y, rrr, rrr, color_profile);
        break;
      case br_lut_diamond:
        brush_set_lut_diamond(br, center_x, center_y, rrr, rrr, color_profile);
        break;
      case br_gouraud:
        brush_set_gouraud(br, center_x, center_y - rrr / 2., center_x - rrr / 2., center_y + rrr / 2., center_x + rrr / 2., center_y + rrr / 2., _rgb(1, 0, 0), _rgb(0, 1, 0), _rgb(0, 0, 1));
        break;
      default: {
        int asdf = 0;
      }
      break;
      }
      {
        utime_start(_start_time);
        //imdrawaa_fillpoly_lut_radial(im, pt, &n, 1, center_x, center_y, rrr, rrr, color_profile);
        imdrawaa_poly_brush(im, &rc, pt, &n, 1, br);
        printf("%f\n", utime_elapsed(_start_time));
      }
    }
    break;
  }
  return 0;
}

