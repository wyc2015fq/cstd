
static int dPOLYGON_hittest(const DPOLYGON* s, double x, double y, double m_point_radius, int m_hitmark)
{
  int i, npt, ret;
  const DPOINT* pt = s->pt;
  for (i = 0; i < s->n; ++i) {
    npt = s->len[i];
    if (ret = point_in_polygon_hittest(pt, npt, x, y, m_point_radius, m_hitmark)) {
      return ret;
    }
    pt += npt;
  }
  return 0;
}
static int polygon_ctrl_hittest(DPOLYGON* m_polygon, double x, double y, int m_hitmark)
{
  return dPOLYGON_hittest(m_polygon, x, y, 5, m_hitmark);
}
static int gcPolygonText(gc_t* g, int n, const DPOINT* pt, texfont_t* font)
{
  int i;
  char buf[256];
  for (i = 0; i < n; ++i) {
    int x = (int)pt[i].x, y = (int)pt[i].y;
    IRECT rc1 = iRECT(x, y, x + 30, y + 20);
    _snprintf(buf, 256, "%d", i);
    gcSolidText(g, rc1, buf, 0, font, TF_VCENTER, Color_Black);
  }
  return 0;
}
static int polygon_proc(DPOINT* m_polygon, int count, int closed, BOOL isspline, double m_point_radius)
{
  GETGUIIO2();
  IRECT rc = {0};
  int i = 0, j = 0;
  int hot = 0;
  int hotid = 0;
  static IPOINT m_pt;
  void* c = m_polygon;
  int hitmask = POHIT_POINT | POHIT_EDGE | POHIT_FACE;
  if (!calc_rect(100, 100, 0, &rc)) {
    return 0;
  }
  if (ISHOVEREDWIN2()) {
    int x = io->MousePos.x, y = io->MousePos.y;
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      hotid = point_in_polygon_hittest(m_polygon, count, x, y, m_point_radius, hitmask);
    }
    switch (io->lastMsg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        io->HitValue = 0;
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        if (hotid) {
          SETHIT(c);
          io->HitValue = hotid;
          m_pt.x = x, m_pt.y = y;
          FORCE_REDRAW(c);
        }
      }
      break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c) && io->HitValue) {
        int i = io->HitValue & POHIT_MARK;
        double dx, dy;
        DPOINT* pt = m_polygon;
        dx = x - m_pt.x;
        dy = y - m_pt.y;
        if (io->HitValue & POHIT_POINT) {
          i = io->HitValue;
          pt[i].x += dx;
          pt[i].y += dy;
        }
        else if (io->HitValue & POHIT_FACE) {
          for (i = 0; i < count; i++) {
            pt[i].x += dx;
            pt[i].y += dy;
          }
        }
        else if (io->HitValue & POHIT_EDGE) {
          int n1 = i;
          int n2 = NEXT_INDEX(i, count);
          pt[n1].x += dx;
          pt[n1].y += dy;
          pt[n2].x += dx;
          pt[n2].y += dy;
        }
        m_pt.x = x, m_pt.y = y;
        FORCE_REDRAW(c);
      }
      else {
      }
      break;
    }
  }
  if (1) {
    char buf[256];
    COLOR m_color[] = {_rgba(0.0, 0.0, 0.0, 0.2), _rgba(0.0, 0.0, 0.0, 0.4), _rgba(0.0, 0.0, 0.0, 0.6), };
    COLOR m_node_color[] = {_rgba(0.0, 0.5, 0.0, 0.5), _rgba(0.0, 0.7, 0.0, 0.7), _rgba(0.7, 0.2, 0.0, 0.9)};
    COLOR m_text_color = (_rgba(0.0, 0.0, 0.0, 1));
    double m_stroke_width = 1;
    double m_point_radius = 5;
    const DPOINT* pt = m_polygon;
    int hit = ISHIT(c);
    int hotpt = -1;
    int hoted = (hotid & POHIT_EDGE) ? (hit ? 2 : 1) : 0;
    if (hit) {
      hotid = io->HitValue;
    }
    if (hotid & POHIT_POINT) {
      hotpt = hotid & POHIT_MARK;
      if (hit) {
        printf("adsfadf");
      }
    }
    if (isspline) {
      //DPOINT pt2[128];
      //int n = vcgen_bspline(countof(pt2), pt2, count, pt, closed);
      //gcSolidPolygon(ctx, pt2, n, ln, nlen, GCTYPE_DOUBLE, sizeof(DPOINT), 0, m_color[hoted], 2);
    }
    else {
      gcSolidPolygon(ctx, pt, count, closed, GCTYPE_DOUBLE, sizeof(DPOINT), 0, m_color[hoted], 2);
    }
    for (i = 0; i < count; ++i) {
      gcSolidCircle(ctx, pt[i].x, pt[i].y, m_point_radius, m_node_color[(i == hotpt) + !!hit], 0, 0);
      if (1) {
        int x = (int)pt[i].x, y = (int)pt[i].y;
        _snprintf(buf, 256, "%d", i);
        gcSolidText(ctx, iRECT(x, y, x + 30, y + 20), buf, 0, 0, TF_VCENTER, m_text_color);
      }
    }
  }
  return 0;
}

