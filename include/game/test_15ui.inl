#include "numgame.inl"
typedef struct {
  int h, w, n;
  numnode_t* a;
  uchar abuf[256];
} test_numui_t;
int test_numui_hittest(test_numui_t* s, int x, int y, int cx, int cy)
{
  int i, j, hit = -1;
  for (i = 0; i < s->h; ++i) {
    for (j = 0; j < s->w; ++j) {
      IRECT rc1 = iRECT2(j * cx, i * cy, cx, cy);
      if (iPtInRect(&rc1, x, y)) {
        return i * s->w + j;
      }
    }
  }
  return hit;
}
int test_numui(test_numui_t* s)
{
  //button_ctrl
  GETGUIIO2();
  void* c = s;
  int i, j, hot;
  int cx = 100, cy = cx;
  IRECT rc = {0};
  uchar* data;
  numnode_t* a;
  a = (numnode_t*)s->abuf;
  data = (uchar*)(a + 1);
  if (1) {
    if (!calc_rect(100, 100, 0, &rc)) {
      return 0;
    }
  }
  if (!s->a) {
    s->a = a;
    s->n = s->h * s->w;
    for (i = 0; i < s->n; ++i) {
      j = (i + 1) % (s->n);
      data[i] = j;
      if (0 == j) {
        a->y = i / s->w;
        a->x = i % s->w;
      }
    }
  }
  if (ISHOVEREDWIN2()) {
    int x = io->MousePos.x, y = io->MousePos.y;
    hot = iPtInRect(&rc, x, y);
    switch (io->lastMsg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        if (hot) {
        }
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        SETFOCUS(c);
        io->HitValue = test_numui_hittest(s, x, y, cx, cy);
        if (io->HitValue >= 0) {
          int x0 = io->HitValue % s->w, y0 = io->HitValue / s->w;
          for (i = 0; i < 4; ++i) {
            int x1 = a->x + dir[i][0];
            int y1 = a->y + dir[i][1];
            if (x0 == x1 && y0 == y1) {
              numnode_move1(s->h, s->w, a, a, i);
            }
          }
        }
      }
      break;
    case MSG_KEYDOWN:
      switch (io->lastChar) {
      case KEY_LEFT:
        numnode_move1(s->h, s->w, a, a, 0);
        break;
      case KEY_RIGHT:
        numnode_move1(s->h, s->w, a, a, 1);
        break;
      case KEY_UP:
        numnode_move1(s->h, s->w, a, a, 2);
        break;
      case KEY_DOWN:
        numnode_move1(s->h, s->w, a, a, 3);
        break;
      }
      break;
    }
  }
  if (1) {
    COLOR clrFace = myGetSysColor(CLR_BTNFACE);
    COLOR clrText = Color_Black;
    COLOR clrFace2 = RGBMUL(clrFace, 0.9);
    char buf[16];
    for (i = 0; i < s->h; ++i) {
      for (j = 0; j < s->w; ++j) {
        IRECT rc1 = iRECT2(j * cx, i * cy, cx, cy);
        int id = i * s->w + j;
        gcEdgeRectSys(ctx, rc1, (ISHIT(c) && io->HitValue == (id)) ? BDS_SUNKEN : BDS_HIGH, 15, data[id] ? clrFace : clrFace2);
        if (hot) {
          gcEdgeRect1(ctx, rc1, _rgba(1, 1, 1, 0.2), 0, 0);
        }
        _snprintf(buf, 16, "%d", data[id]);
        //gcSolidText(ctx, rc1, buf, 0, 0, TF_CENTER|TF_VCENTER, clrText);
        //clrText = _rgba(0,0,0,0.3);
        //gcTextv(ctx, buf, rc1, CV_FONT_HERSHEY_COMPLEX, 30, 0, 3, clrText, TF_CENTER|TF_VCENTER);
        if (0) {
          DPOINT pt[3] = {10, 10, 10, 100, 100, 10};
          gcSolidPolygon(ctx, pt, 3, 1, GCTYPE_DOUBLE, sizeof(DPOINT), clrText, 0, 0);
        }
        if (1) {
          dpath_t path[1] = {0};
          dpath_reset(path);
          gsv_text(path, buf, rc1, 30, 0, TF_CENTER | TF_VCENTER);
          //int i;for (i=0; i<path->n; ++i) { printf("%f %f\n", path->pt[i].x, path->pt[i].y); }
          clrText = Color_Black;
          gcSolidPolygons(ctx, path->pt, path->n, path->ln, path->flag, path->c, GCTYPE_DOUBLE, sizeof(DPOINT), 0, clrText, 2);
          DPOINT* pt = path->pt;
          int i;
          for (i = 0; i < path->n; ++i) {
            //gcSolidCircle(ctx, pt[i].x, pt[i].y, 3, clrText, 0, 0);
          }
          dpath_free(path);
        }
      }
    }
  }
  return 0;
}

