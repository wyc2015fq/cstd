
int flexbox_config_ctrl(ctrl_t* c, int* pfmt)
{
  static ctrl_t cc[10] = {0};
  IRECT rc = c->rc;
  int fmt = *pfmt;
  int direction = fmt & flex_direction_mask;
  int wrap = (fmt & flex_wrap_mask) >> 2;
  int justify_content = (fmt & flex_justify_content_mask) >> 4;
  int align_items = (fmt & flex_align_items_mask) >> 7;
  int align_content = (fmt & flex_align_content_mask) >> 10;
  int maxh = 160;
  int i = 0;
  if (rbox_ctrl(cc + i++, "direction", "|row|row_reverse|column|column_reverse|", &direction)) {
    printf("direction %d\n", direction);
  }
  if (rbox_ctrl(cc + i++, "wrap", "|nowrap|wrap|wrap_reverse|", &wrap)) {
    printf("wrap %d\n", wrap);
  }
  if (rbox_ctrl(cc + i++, "justify_content", "|start|end|center|space_between|space_around|stretch|", &justify_content)) {
    printf("justify_content %d\n", justify_content);
  }
  if (rbox_ctrl(cc + i++, "align_items", "|istretch|istart|iend|icenter|", &align_items)) {
    printf("align_items %d\n", align_items);
  }
  if (rbox_ctrl(cc + i++, "align_content", "|cstretch|cstart|cend|ccenter|cspace_between|", &align_content)) {
    printf("align_content %d\n", align_content);
  }
  c->sz = flexboxlay(rc, i, cc, 0, flex_column);
  fmt = direction | (wrap << 2) | (justify_content << 4) | (align_items << 7) | (align_content << 10);
  *pfmt = fmt;
  return 0;
}
#include "math/rand.inl"

int ctrl_hittest(int n, ctrl_t* c, int x, int y)
{
  int i;
  for (i = 0; i < n; ++i) {
    if (iPtInRect(&c[i].rc, x, y)) {
      return i;
    }
  }
  return -1;
}
int test_flexctrl(ctrl_t* c)
{
  SYSIO;
  static int n = 20;
  static int fmt = flex_wrap | flex_row | flex_start;
  static int imin[] = {20, 30};
  static int imax[] = {100, 100};
  int gap = 4;
  IRECT rc = c->rc, rcL;
  ctrl_t c0[200] = {0};
  int hot = 0, hotid = -1;
  iRectCutL(&rc, 150, gap, &rcL);
  if (1) {
    static ctrl_t cc[10] = {0};
    int i = 0;
    //slider_int_ctrl(cc+i++, "x min = %d", imin, 0, 100);
    //slider_int_ctrl(cc+i++, "x max = %d", imax, 0, 200);
    slider_int_ctrl(cc + i++, "n = %d", &n, 0, countof(c0));
    //slider_int_ctrl(cc+i++, "gap = %d", gap, 0, 20), gap[1] = gap[0];
    flexbox_config_ctrl(cc + i++, &fmt);
    c->sz = flexboxlay(rcL, i, cc, 0, flex_column);
  }
  {
    rng_t r[1] = {0};
    rng_lcg64_init(r, 10);
    iRandArr(r, n, 2, &c0->sz, sizeof(ctrl_t), imin, imax);
    flexboxlay(rc, n, c0, gap, fmt);
  }
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      hotid = ctrl_hittest(n, c0, x, y);
    }
    switch (io->msg) {
    case MSG_MOUSEMOVE:
      if (hot) {
      }
      break;
    }
  }
  if (1) {
    int i = 0;
    int alpha[2] = {50, 100};
    COLOR clrs[] = {ColorBlue, ColorRed, ColorGreen, ColorOrchid, ColorSalmon};
    n = MIN(n, countof(c0));
    gcFillColor(g, _rgba(1, 0, 0, 0.5));
    gcSolidRect2(g, rc, ColorWhite);
    for (i = 0; i < n; ++i) {
      char buf[256];
      COLOR clr = clrs[i % countof(clrs)];
      gcSolidRect2(g, c0[i].rc, _RGB_A(clr, alpha[i == hotid]));
      _snprintf(buf, 256, "%d %d %d", i, c0[i].sz.w, c0[i].sz.h);
      gcSolidText(g, c0[i].rc, buf, NULL, io->font, 0, TF_CENTER | TF_VCENTER, ColorBlack);
    }
  }
  return 0;
}

