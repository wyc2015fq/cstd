#include "aaa.inl"
#define x11_color(clr) (clr)
static void x11_draw_text(ImGuiHost* host, IRECT rc, const char* text, int len, XFont* font, COLOR fg)
{
  XWindow* s = (XWindow*)host->Window;
  ImGuiX11* surf = (ImGuiX11*)host->Driver;
  ImGuiX11* g = (ImGuiX11*)host->Driver;
  int tx, ty;
  len = len < 0 ? strlen(text) : len;
  if (!text || !font || !len) {
    return;
  }
  tx = (int)rc.l;
  ty = (int)rc.t + font->ascent;
  XSetForeground(s->dpy, surf->gc, fg);
  if (font->set) {
    XmbDrawString(s->dpy, surf->drawable, font->set, surf->gc, tx, ty, (const char*)text, (int)len);
  }
  else {
    XDrawString(s->dpy, surf->drawable, surf->gc, tx, ty, (const char*)text, (int)len);
  }
}
static void x11_fill_polygon(ImGuiHost* host, BOOL fill, BOOL line, const dpath_t* path)
{
  XWindow* s = (XWindow*)host->Window;
  ImGuiX11* surf = (ImGuiX11*)host->Driver;
  ImGuiX11* g = (ImGuiX11*)host->Driver;
  int i = 0, j = 0, k, count = 0;
  if (fill) {
    count = path->c > 0 ? path->ln[0] : 0;
    XPoint* xpnts = (XPoint*)malloc(count * sizeof(XPoint));
    XSetForeground(s->dpy, surf->gc, g->brush);
    for (i = 0; i < count; ++i) {
      xpnts[i].x = (int)path->pt[i].x;
      xpnts[i].y = (int)path->pt[i].y;
    }
    XFillPolygon(s->dpy, surf->drawable, surf->gc, xpnts, count, Convex, CoordModeOrigin);
    free(xpnts);
  }
  if (line) {
    DPOINT* pnts = path->pt;
    XSetForeground(s->dpy, surf->gc, g->pen);
    XSetLineAttributes(s->dpy, surf->gc, (int)g->lineWidth, LineSolid, CapButt, JoinMiter);
    for (k = 0; k < path->c; ++k) {
      count = path->ln[k];
      j = count - 1;
      for (i = 0; i < count; ++i) {
        XDrawLine(s->dpy, surf->drawable, surf->gc, pnts[j].x, pnts[j].y, pnts[i].x, pnts[i].y);
        j = i;
      }
      pnts += count;
    }
    XSetLineAttributes(s->dpy, surf->gc, 1, LineSolid, CapButt, JoinMiter);
  }
}
IM_STATIC ISIZE x11_measureTextSize(XFont* font, const char* text, int len)
{
  XRectangle r;
  ISIZE sz = {0};
  int w = 0;
  if (!font || !text) {
    return sz;
  }
  if (font->set) {
    XmbTextExtents(font->set, (const char*)text, len, NULL, &r);
    w = (float)r.width;
  }
  else {
    int w = XTextWidth(font->xfont, (const char*)text, len);
  }
  sz = iSIZE(w, font->height);
  return sz;
}
int x11_Rander(ImGuiHost* host, const ImDrawContent* ctx)
{
  int i;
  XWindow* s = (XWindow*)host->Window;
  ImGuiX11* surf = (ImGuiX11*)host->Driver;
  ImGuiX11* g = (ImGuiX11*)host->Driver;
  const char* va = ctx->buf;
  const char* vb = ctx->buf + ctx->len;
  XClearWindow(s->dpy, s->win);
  XSetForeground(s->dpy, surf->gc, x11_color(host->clrbg));
  XFillRectangle(s->dpy, surf->drawable, surf->gc, 0, 0, surf->w, surf->h);
  for (; va < vb;) {
    int type = VA_ARG(char);
    switch (type) {
    case ImDrawCmd_styleColor: {
      char fill = VA_ARG(char);
      ImColor clr = VA_ARG(ImColor);
      clr = x11_color(clr);
      if (fill) {
        g->brush = clr;
      }
      else {
        g->pen = clr;
      }
    }
    break;
    case ImDrawCmd_styleGradient: {
      char fill = VA_ARG(char);
      FPOINT pt1, pt2;
      int count;
      COLOR blend[100];
      float positions[100];
      ImColor clr = 0;
      pt1.x = VA_ARG(float);
      pt1.y = VA_ARG(float);
      pt2.x = VA_ARG(float);
      pt2.y = VA_ARG(float);
      count = VA_ARG(int);
      ASSERT(count < 100);
      for (i = 0; i < count; ++i) {
        positions[i] = VA_ARG(float);
        blend[i] = VA_ARG(ImColor);
        clr = blend[i] = x11_color(blend[i]);
      }
      if (fill) {
        g->brush = clr;
      }
      else {
        g->pen = clr;
      }
    }
    break;
    case ImDrawCmd_stylePattern:
      ASSERT(0);
      break;
    case ImDrawCmd_lineWidth: {
      float width = VA_ARG(float);
      g->lineWidth = width;
    }
    break;
    case ImDrawCmd_lineCap: {
      char lineCap1 = VA_ARG(char);
      g->lineCap = lineCap1;
    }
    break;
    case ImDrawCmd_lineJoin: {
      char lineJoin1 = VA_ARG(char);
      g->lineJoin = lineJoin1;
    }
    break;
    case ImDrawCmd_miterLimit: {
      float miterLimit = VA_ARG(float);
      g->miterLimit = miterLimit;
    }
    break;
#define VA_ARG_EQ(T, A) T A = VA_ARG(T)
    case ImDrawCmd_rect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      dpath_rect(g->path, x, y, w, h);
    }
    break;
    case ImDrawCmd_fillRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      XSetForeground(s->dpy, surf->gc, g->brush);
      XFillRectangle(s->dpy, surf->drawable, surf->gc, (int)x, (int)y, (int)w, (int)h);
    }
    break;
    case ImDrawCmd_strokeRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      XSetForeground(s->dpy, surf->gc, g->pen);
      XSetLineAttributes(s->dpy, surf->gc, g->lineWidth, LineSolid, CapButt, JoinMiter);
      XFillRectangle(s->dpy, surf->drawable, surf->gc, (int)x, (int)y, (int)w, (int)h);
    }
    break;
    case ImDrawCmd_clearRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
    }
    break;
    case ImDrawCmd_resetPath:
      dpath_reset(g->path);
      break;
    case ImDrawCmd_beginPath:
      dpath_reset(g->path);
      break;
    case ImDrawCmd_closePath:
      dpath_close(g->path);
      break;
    case ImDrawCmd_clip:
      ASSERT(0);
      break;
    case ImDrawCmd_clipRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      if (w < 0 || h < 0) {
        *g->clip = iRECT2(0, 0, g->w, g->h);
      }
      else {
        *g->clip = iRECT2((int)x, (int)y, (int)w, (int)h);
      }
    }
    break;
    case ImDrawCmd_fill:
      x11_fill_polygon(host, 1, 0, g->path);
      break;
    case ImDrawCmd_stroke:
      x11_fill_polygon(host, 0, 1, g->path);
      break;
    case ImDrawCmd_moveTo: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      dpath_lineto(g->path, x, y);
    }
    break;
    case ImDrawCmd_lineTo: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      dpath_lineto(g->path, x, y);
    }
    break;
    case ImDrawCmd_arcTo: {
      VA_ARG_EQ(float, x1);
      VA_ARG_EQ(float, y1);
      VA_ARG_EQ(float, x2);
      VA_ARG_EQ(float, y2);
      VA_ARG_EQ(float, radius);
      ASSERT(0);
    }
    break;
    case ImDrawCmd_arc: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, radius);
      VA_ARG_EQ(float, startAngle);
      VA_ARG_EQ(float, endAngle);
      startAngle *= RADIAN;
      endAngle *= RADIAN;
      //if (anticlockwise) { //endAngle += 2 * (float)PI; }
      dpath_arc(g->path, x, y, radius, startAngle, endAngle);
      dpath_lineto(g->path, x + radius * cos(endAngle), y + radius * sin(endAngle));
    }
    break;
    case ImDrawCmd_quadraticCurveTo: {
      VA_ARG_EQ(float, cpx);
      VA_ARG_EQ(float, cpy);
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      ASSERT(0);
      //g->x = x, g->y = y;
    }
    break;
    case ImDrawCmd_bezierCurveTo: {
      VA_ARG_EQ(float, cpx1);
      VA_ARG_EQ(float, cpy1);
      VA_ARG_EQ(float, cpx2);
      VA_ARG_EQ(float, cpy2);
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      ASSERT(0);
      //g->x = x, g->y = y;
    }
    break;
    case ImDrawCmd_scale: {
      VA_ARG_EQ(float, scalewidth);
      VA_ARG_EQ(float, scaleheight);
      ASSERT(0);
    }
    break;
    case ImDrawCmd_rotate: {
      VA_ARG_EQ(float, angle);
      ASSERT(0);
    }
    break;
    case ImDrawCmd_translate: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      ASSERT(0);
    }
    break;
    case ImDrawCmd_transform: {
      VA_ARG_EQ(float, a);
      VA_ARG_EQ(float, b);
      VA_ARG_EQ(float, c);
      VA_ARG_EQ(float, d);
      VA_ARG_EQ(float, e);
      VA_ARG_EQ(float, f);
      ASSERT(0);
    }
    break;
    case ImDrawCmd_setTransform: {
      VA_ARG_EQ(float, a);
      VA_ARG_EQ(float, b);
      VA_ARG_EQ(float, c);
      VA_ARG_EQ(float, d);
      VA_ARG_EQ(float, e);
      VA_ARG_EQ(float, f);
      ASSERT(0);
    }
    break;
    case ImDrawCmd_textAlign: {
      VA_ARG_EQ(char, textAlign);
      g->textAlign = textAlign;
    }
    break;
    case ImDrawCmd_text: {
      VA_ARG_EQ(char, fill);
      VA_ARG_EQ(int, textlen);
      const char* text = (va += textlen) - textlen;
      VA_ARG_EQ(int, font_id);
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      VA_ARG_EQ(int, formatFlags);
      XFont* font = NULL;
      imuiGetFontByID(font_id, &font, 0);
      if (font) {
        IRECT rc = iRECT2((int)x, (int)y, (int)w, (int)h);
        ISIZE sz = x11_measureTextSize(font, text, textlen);
        iRectAlign(rc, sz.w, sz.h, formatFlags, &rc);
        x11_draw_text(host, rc, text, textlen, font, g->brush);
      }
    }
    break;
    case ImDrawCmd_primRectUV: {
      VA_ARG_EQ(int, npr);
  VA_ARG_EQ(texture_t*, tex);
    }
    break;
    default:
      ASSERT(0);
      break;
    }
  }
  XCopyArea(s->dpy, surf->drawable, s->win, surf->gc, 0, 0, surf->w, surf->h, 0, 0);
  XFlush(s->dpy);
  return 0;
}
int x11_handle_event(ImGui* io, ImGuiHost* host, XEvent* evt)
{
  XWindow* s = (XWindow*)host->Window;
  ImGuiX11* surf = (ImGuiX11*)host->Driver;
  Display* dpy = s->dpy;
  int screen = s->screen;
  Window win = s->win;
  switch (evt->type) {
  case Expose:
  case ConfigureNotify: {
    /* Window resize handler */
    int w, h;
    XWindowAttributes attr;
    XGetWindowAttributes(dpy, win, &attr);
    w = (int)attr.width;
    h = (int)attr.height;
    if (surf->w != w || surf->h != h) {
      surf->w = w;
      surf->h = h;
      if (surf->drawable) {
        XFreePixmap(s->dpy, surf->drawable);
      }
      surf->drawable = XCreatePixmap(s->dpy, s->root, w, h, (unsigned int)DefaultDepth(s->dpy, s->screen));
    }
    return 1;
  }
  break;
  }
  return 0;
}
int x11_CreateFont(ImGuiHost* host, ImFont* f, const char* name, int size)
{
  int n;
  char* def, **missing;
  XWindow* s = (XWindow*)host->Window;
  ImGuiX11* surf = (ImGuiX11*)host->Driver;
  Display* dpy = s->dpy;
  float height = size;
  XFont* font = (XFont*)calloc(1, sizeof(XFont));
  font->set = XCreateFontSet(dpy, name, &missing, &n, &def);
  if (missing) {
    while (n--) {
      fprintf(stderr, "missing fontset: %s\n", missing[n]);
    }
    XFreeStringList(missing);
  }
  if (font->set) {
    XFontStruct** xfonts = NULL;
    char** font_names = NULL;
    XExtentsOfFontSet(font->set);
    n = XFontsOfFontSet(font->set, &xfonts, &font_names);
    while (n--) {
      font->ascent = MAX(font->ascent, (*xfonts)->ascent);
      font->descent = MAX(font->descent, (*xfonts)->descent);
      xfonts++;
    }
  }
  else {
    if (!(font->xfont = XLoadQueryFont(dpy, name)) && !(font->xfont = XLoadQueryFont(dpy, "fixed"))) {
      free(font);
      return 0;
    }
    font->ascent = font->xfont->ascent;
    font->descent = font->xfont->descent;
  }
  font->height = font->ascent + font->descent;
  f->font = font;
  f->size = height;
  return font != NULL;
}
IM_STATIC int x11_measureText(ImGuiHost* host, const char* text, const char* text_end, ImFontID font_id, int formatFlags, IRECT* rc)
{
  XFont* font = NULL;
  imuiGetFontByID(font_id, &font, NULL);
  int len = text_end ? (text_end - text) : strlen(text);
  int w = 0;
  XRectangle r;
  if (!font || !text) {
    return 0;
  }
  if (font->set) {
    XmbTextExtents(font->set, (const char*)text, len, NULL, &r);
    w = (float)r.width;
  }
  else {
    int w = XTextWidth(font->xfont, (const char*)text, len);
  }
  *rc = iRECT2(rc->l, rc->t, w, font->height);
  return w;
}
IM_STATIC int x11_DeleteFont(ImGuiHost* host, ImFont* f)
{
  XWindow* s = (XWindow*)host->Window;
  ImGuiX11* surf = (ImGuiX11*)host->Driver;
  Display* dpy = s->dpy;
  XFont* font = (XFont*)(f->font);
  if (!font) {
    return 0;
  }
  if (font->set) {
    XFreeFontSet(dpy, font->set);
  }
  else {
    XFreeFont(dpy, font->xfont);
  }
  free(font);
  f->font = NULL;
}
int x11_UnInit(ImGuiHost* host)
{
  XWindow* s = (XWindow*)host->Window;
  ImGuiX11* surf = (ImGuiX11*)host->Driver;
  XFreePixmap(s->dpy, surf->drawable);
  XFreeGC(s->dpy, surf->gc);
  memset(surf, 0, sizeof(*surf));
  XUnmapWindow(s->dpy, s->win);
  XFreeColormap(s->dpy, s->cmap);
  XDestroyWindow(s->dpy, s->win);
  XCloseDisplay(s->dpy);
  memset(s, 0, sizeof(*s));
}
IM_STATIC ImFont* x11_Font_opt(ImGuiHost* host, int opt, ImFontID id, const char* name, int size)
{
  static ImFont fontx[100] = {0};
  return ImGuiFont_opt(opt, host, fontx, 100, id, name, size, x11_CreateFont, x11_DeleteFont);
}
int x11_Init(ImGuiHost* host)
{
  XWindow* s = (XWindow*)host->Window;
  Display* dpy = s->dpy;
  int screen = s->screen;
  Window root = s->root;
  ImGuiX11* surf = (ImGuiX11*)calloc(1, sizeof(ImGuiX11));
  int w = s->width, h = s->height;
  if (!setlocale(LC_ALL, "")) {
    return 0;
  }
  if (!XSupportsLocale()) {
    return 0;
  }
  if (!XSetLocaleModifiers("@im=none")) {
    return 0;
  }
  /* create invisible cursor */
  if (0) {
    static XColor dummy;
    char data[1] = {0};
    Pixmap blank = XCreateBitmapFromData(dpy, root, data, 1, 1);
    if (blank == None) {
      return 0;
    }
    //cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
    XFreePixmap(dpy, blank);
  }
  surf->w = w;
  surf->h = h;
  surf->gc = XCreateGC(s->dpy, s->root, 0, NULL);
  XSetLineAttributes(s->dpy, surf->gc, 1, LineSolid, CapButt, JoinMiter);
  surf->drawable = XCreatePixmap(s->dpy, s->root, w, h, (unsigned int)DefaultDepth(s->dpy, s->screen));
  host->Driver = surf;
  host->DrvUnInit = x11_UnInit;
  host->DrvRander = x11_Rander;
  s->handle_event = x11_handle_event;
  host->DrvFont_opt = x11_Font_opt;
  host->DrvMeasureText = x11_measureText;
  return 0;
}

