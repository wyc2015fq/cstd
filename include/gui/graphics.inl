#if defined(NANA_WINDOWS)
#include <windows.h>
#elif defined(NANA_X11)
#include <X11/Xlib.h>
#endif
ISIZE raw_text_extent_size(graphics_t* g, const char* text, size_t len)
{
  if (NULL == g || NULL == text || 0 == len) {
    return iSIZE(0, 0);
  }
#if defined(NANA_WINDOWS)
  {
    SIZE sz;
    if (GetTextExtentPoint32(g->context, text, (int)(len), &sz)) {
      return iSIZE(sz.cx, sz.cy);
    }
  }
#elif defined(NANA_X11)
#if defined(NANA_USE_XFT)
  string utf8str = to_utf8(wstring(text, len));
  XGlyphInfo ext;
  XftFont* fs = (XftFont*)(g->font->handle);
  ::XftTextExtentsUtf8(open_display(), fs,
      (XftChar8*)((char*)(utf8str.c_str())), utf8str.iSIZE(), &ext);
  return iSIZE(ext.xOff, fs->ascent + fs->descent);
#else
  XRectangle ink;
  XRectangle logic;
  ::XmbTextExtents((XFontSet)(g->font->handle), text, len, &ink, &logic);
  return iSIZE(logic.width, logic.height);
#endif
#endif
  return iSIZE(0, 0);
}
ISIZE text_extent_size(graphics_t* g, const char* text, int len)
{
  if (NULL == g || NULL == text || 0 == len) {
    return iSIZE(0, 0);
  }
  {
    ISIZE extents = raw_text_extent_size(g, text, len);
    const char* end = text + len;
    int tabs = 0;
    for (; text != end; ++text) {
      if (*text == '\t') {
        ++tabs;
      }
    }
    if (tabs) {
      extents.w = (int)(extents.w) - tabs * (int)(g->string.tab_pixels - g->string.whitespace_pixels * g->string.tab_length);
    }
    return extents;
  }
}
void draw_string(graphics_t* g, IPOINT pos, const char* str, int len)
{
#if defined(NANA_WINDOWS)
  TextOutA(g->context, pos.x, pos.y, str, (int)(len));
#elif defined(NANA_X11)
  Display* disp = open_display();
#if defined(NANA_USE_XFT)
  auto fs = (XftFont*)(g->font->handle);
  //Fixed missing array declaration by dareg
  unique_ptr<FT_UInt[]> glyphs_ptr(new FT_UInt[len]);
  auto glyphs = glyphs_ptr.get();
  const auto endstr = str + len;
  for (auto chr = str; chr != endstr; ++chr) {
    (*glyphs++) = XftCharIndex(disp, fs, *chr);
  }
  XftDrawGlyphs(g->xftdraw, &(g->xft_fgcolor), fs, pos.x, pos.y + fs->ascent, glyphs_ptr.get(), len);
#else
  XFontSet fs = (XFontSet)(g->font->handle);
  XFontSetExtents* ext = ::XExtentsOfFontSet(fs);
  XFontStruct** fontstructs;
  char** font_names;
  int ISIZE = ::XFontsOfFontSet(fs, &fontstructs, &font_names);
  unsigned ascent = 0;
  unsigned descent = 0;
  XFontStruct** fontstructs_end = fontstructs + ISIZE;
  for (XFontStruct** i = fontstructs; i < fontstructs_end; ++i) {
    if (ascent < (*i)->ascent) {
      ascent = (*i)->ascent;
    }
    if (descent < (*i)->descent) {
      descent = (*i)->descent;
    }
  }
  XmbDrawString(disp, g->pixmap, (XFontSet)(g->font->handle), g->context, pos.x, pos.y + ascent + descent, str, len);
#endif
#endif
}
void drawable_delete(const graphics_t* p)
{
#if defined(NANA_WINDOWS)
#elif defined(NANA_X11)
  if (p) {
    Display* disp = open_display();
#if defined(NANA_USE_XFT)
    ::XftDrawDestroy(p->xftdraw);
#endif
    ::XFreeGC(disp, p->context);
    ::XFreePixmap(disp, p->pixmap);
  }
#endif
}
#if 0
font::font()
  : impl_(new impl_type)
{
  impl_->font_ptr = default_native_font();
}
font::font(graphics_t* g)
  : impl_(new impl_type)
{
  impl_->font_ptr = g->font;
}
font::font(const font& rhs)
  : impl_(new impl_type)
{
  if (rhs.impl_) {
    impl_->font_ptr = rhs.impl_->font_ptr;
  }
}
font::font(const string& name, unsigned size, bool bold, bool italic, bool underline, bool strike_out)
  : impl_(new impl_type)
{
  make(name, size, bold, italic, underline, strike_out);
}
font::~font()
{
  delete impl_;
}
bool font::empty() const
{
  return ((NULL == impl_) || (NULL == impl_->font_ptr));
}
void font::make(const string& name, unsigned size, bool bold, bool italic, bool underline, bool strike_out)
{
  size = font_size_to_height(size);
  make_raw(name, size, bold ? 700 : 400, italic, underline, strike_out);
}
#endif
void graphics_release(graphics_t* g)
{
  g->handle_ = NULL;
  g->size_.w = g->size_.h = 0;
}
void graphics_set_color(graphics_t* g, COLOR clr)
{
  g->color_ = (clr & 0xFFFFFF);
#if defined(NANA_WINDOWS)
#elif defined(NANA_X11)
#endif
}
#define NANA_RGB(x) (x)
void graphics_set_text_color(graphics_t* g, COLOR clr)
{
  COLOR rgb = (clr & 0xFFFFFF);
  if (g->text_color_ != rgb) {
#if defined(NANA_WINDOWS)
    SetTextColor(g->context, NANA_RGB(rgb));
    g->text_color_ = rgb;
#elif defined(NANA_X11)
    platform_x11* x11 = get_x11();
    COLOR col = g->text_color_;
    switch (screen_depth()) {
    case 16:
      col = ((((col >> 16) & 0xFF) * 31 / 255) << 11) |
          ((((col >> 8) & 0xFF) * 63 / 255) << 5) |
          (col & 0xFF) * 31 / 255;
      break;
    }
    ::XSetForeground(open_display(), g->context, col);
    ::XSetBackground(open_display(), g->context, col);
#if defined(NANA_USE_XFT)
    xft_fgcolor.color.red = ((0xFF0000 & col) >> 16) * 0x101;
    xft_fgcolor.color.green = ((0xFF00 & col) >> 8) * 0x101;
    xft_fgcolor.color.blue = (0xFF & col) * 0x101;
    xft_fgcolor.color.alpha = 0xFFFF;
#endif
#endif
  }
}
void graphics_make(graphics_t* g, int w, int h)
{
  if (g->handle_ == NULL || w != g->size_.w || h != g->size_.h) {
    //The object will be delete while dwptr_ is performing a release.
    //Reuse the old font
#if defined(NANA_WINDOWS)
    HDC hdc = GetDC(0);
    HDC cdc = CreateCompatibleDC(hdc);
    BITMAPINFO bmi;
    HBITMAP bmp;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = -(int)(h);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // four 8-bit components
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = (w * h) << 2;
    bmp = CreateDIBSection(cdc, &bmi, DIB_RGB_COLORS, (void**)(&(g->pixbuf_ptr)), 0, 0);
    if (bmp) {
      DeleteObject((HBITMAP)SelectObject(cdc, bmp));
      //DeleteObject(SelectObject(cdc, g->font->handle));
      g->context = cdc;
      g->pixmap = bmp;
      SetBkMode(cdc, TRANSPARENT);
      //g->brush.set(cdc, g->brush.Solid, 0xFFFFFF);
    }
    else {
      DeleteDC(cdc);
      graphics_release(g);
    }
    ReleaseDC(0, hdc);
#elif defined(NANA_X11)
    platform_x11* x11 = get_x11();
    Display* disp = x11->display_;
    int screen = DefaultScreen(disp);
    Window root = XRootWindow(disp, screen);
    g->pixmap = XCreatePixmap(disp, root, (w ? w : 1), (h ? h : 1), DefaultDepth(disp, screen));
    g->context = XCreateGC(disp, g->pixmap, 0, 0);
#if defined(NANA_USE_XFT)
    g->xftdraw = XftDrawCreate(disp, g->pixmap, spec.screen_visual(), spec.colormap());
#endif
#endif
    if (g) {
      graphics_set_color(g, color_black);
      graphics_set_text_color(g, color_black);
#if defined(NANA_WINDOWS)
      g->bytes_per_line = w * 4;
#else
#endif
      g->size_ = iSIZE(w, h);
      g->string.tab_pixels = raw_text_extent_size(g, "\t", 1).w;
      g->string.whitespace_pixels = raw_text_extent_size(g, " ", 1).w;
    }
  }
}
#if 0
void graphics_typeface(const font& f)
{
  //Keep the font as a shadow, even if the graphics is empty. Setting the font is futile when the size
  //of a widget is zero.
  font_shadow_ = f;
  if (handle_ && (false == f.empty())) {
    handle_->font = f.impl_->font_ptr;
#if defined(NANA_WINDOWS)
    SelectObject(handle_->context, (HFONT)(f.impl_->font_ptr->handle));
#endif
    handle_->string.tab_pixels = raw_text_extent_size(handle_, L"\t", 1).w;
    handle_->string.whitespace_pixels = raw_text_extent_size(handle_, L" ", 1).w;
  }
}
font graphics_typeface() const
{
  //The font may be set when the graphics is still empty.
  //it should return the shadow font when the graphics is empty.
  return (handle_ ? font(handle_) : font_shadow_);
}
size graphics_text_extent_size(const string& text) const
{
  throw_not_utf8(text);
  return text_extent_size(to_wstring(text));
}
::size graphics_text_extent_size(const char* text, size_t len) const
{
  return text_extent_size(string(text, text + len));
}
size graphics_text_extent_size(const wchar_t* text) const
{
  return text_extent_size(text, wcslen(text));
}
size graphics_text_extent_size(const wstring& text) const
{
  return text_extent_size(text.c_str(), static_cast<unsigned>(text.length()));
}
size graphics_text_extent_size(const wchar_t* str, size_t len) const
{
  return text_extent_size(handle_, str, len);
}
size graphics_text_extent_size(const wstring& str, size_t len) const
{
  return text_extent_size(handle_, str.c_str(), len);
}
size graphics_glyph_extent_size(const wchar_t* str, size_t len, size_t begin, size_t end) const
{
  if (len < end) {
    end = len;
  }
  if (NULL == handle_ || NULL == str || 0 == len || begin >= end) return {};
  size sz;
#if defined(NANA_WINDOWS)
  int* dx = new int[len];
  SIZE extents;
  ::GetTextExtentExPoint(handle_->context, str, static_cast<int>(len), 0, 0, dx, &extents);
  sz.w = dx[end - 1] - (begin ? dx[begin - 1] : 0);
  unsigned tab_pixels = handle_->string.tab_length * handle_->string.whitespace_pixels;
  const wchar_t* pend = str + end;
  for (const wchar_t* p = str + begin; p != pend; ++p) {
    if (*p == '\t') {
      sz.w += tab_pixels;
    }
  }
  sz.h = extents.cy;
  delete [] dx;
#elif defined(NANA_X11)
  sz = text_extent_size(str + begin, end - begin);
#endif
  return sz;
}
size graphics_glyph_extent_size(const wstring& str, size_t len, size_t begin, size_t end) const
{
  return glyph_extent_size(str.c_str(), len, begin, end);
}
bool graphics_glyph_pixels(const wchar_t* str, size_t len, unsigned* pxbuf) const
{
  if (NULL == handle_ || NULL == handle_->context || NULL == str || NULL == pxbuf) {
    return false;
  }
  if (len == 0) {
    return true;
  }
  unsigned tab_pixels = handle_->string.tab_length * handle_->string.whitespace_pixels;
#if defined(NANA_WINDOWS)
  int* dx = new int[len];
  SIZE extents;
  int i;
  ::GetTextExtentExPoint(handle_->context, str, static_cast<int>(len), 0, 0, dx, &extents);
  pxbuf[0] = (str[0] == '\t' ? tab_pixels : dx[0]);
  for (i = 1; i < len; ++i) {
    pxbuf[i] = (str[i] == '\t' ? tab_pixels : dx[i] - dx[i - 1]);
  }
  delete [] dx;
#elif defined(NANA_X11) && defined(NANA_USE_XFT)
  Display* disp = open_display();
  XftFont* xft = handle_->font->handle;
  XGlyphInfo extents;
  int i;
  for (i = 0; i < len; ++i) {
    if (str[i] != '\t') {
      FT_UInt glyphs = ::XftCharIndex(disp, xft, str[i]);
      ::XftGlyphExtents(disp, xft, &glyphs, 1, &extents);
      pxbuf[i] = extents.xOff;
    }
    else {
      pxbuf[i] = tab_pixels;
    }
  }
#endif
  return true;
}
size graphics_bidi_extent_size(const wstring& str) const
{
  size sz;
  if (handle_ && handle_->context && str.size()) {
    vector<unicode_bidi::entity> reordered;
    unicode_bidi bidi;
    bidi.linestr(str.c_str(), str.size(), reordered);
for (auto & i: reordered) {
      size t = text_extent_size(i.begin, i.end - i.begin);
      sz.w += t.w;
      if (sz.h < t.h) {
        sz.h = t.h;
      }
    }
  }
  return sz;
}
::size graphics_bidi_extent_size(const string& str) const
{
  return bidi_extent_size(static_cast< wstring>(::charset(str, ::unicode::utf8)));
}
bool graphics_text_metrics(unsigned& ascent, unsigned& descent, unsigned& internal_leading) const
{
  if (handle_) {
#if defined(NANA_WINDOWS)
    ::TEXTMETRIC tm;
    ::GetTextMetrics(handle_->context, &tm);
    ascent = static_cast<unsigned>(tm.tmAscent);
    descent = static_cast<unsigned>(tm.tmDescent);
    internal_leading = static_cast<unsigned>(tm.tmInternalLeading);
    return true;
#elif defined(NANA_X11)
    if (handle_->font) {
#if defined(NANA_USE_XFT)
      XftFont* fs = reinterpret_cast<XftFont*>(handle_->font->handle);
      ascent = fs->ascent;
      descent = fs->descent;
      internal_leading = 0;
#else
      XFontSet fs = reinterpret_cast<XFontSet>(handle_->font->handle);
      XFontSetExtents* ext = ::XExtentsOfFontSet(fs);
      XFontStruct** fontstructs;
      char** font_names;
      int size = ::XFontsOfFontSet(fs, &fontstructs, &font_names);
      ascent = 0;
      descent = 0;
      XFontStruct** fontstructs_end = fontstructs + size;
      for (XFontStruct** i = fontstructs; i < fontstructs_end; ++i) {
        if (ascent < (*i)->ascent) {
          ascent = (*i)->ascent;
        }
        if (descent < (*i)->descent) {
          descent = (*i)->descent;
        }
      }
#endif
      return true;
    }
#endif
  }
  return false;
}
void graphics_line_begin(int x, int y)
{
  if (!handle_) {
    return;
  }
#if defined(NANA_WINDOWS)
  ::MoveToEx(handle_->context, x, y, 0);
#elif defined(NANA_X11)
  handle_->line_begin_pos.x = x;
  handle_->line_begin_pos.y = y;
#endif
}
void graphics_bitblt(int x, int y, const graphics& src)
{
  rectangle r(src.size());
  r.x = x;
  r.y = y;
  bitblt(r, src);
}
void graphics_bitblt(const rectangle& r_dst, native_window_type src)
{
  if (handle_) {
#if defined(NANA_WINDOWS)
    HDC dc = ::GetDC(reinterpret_cast<HWND>(src));
    ::BitBlt(handle_->context, r_dst.x, r_dst.y, r_dst.w, r_dst.h, dc, 0, 0, SRCCOPY);
    ::ReleaseDC(reinterpret_cast<HWND>(src), dc);
#elif defined(NANA_X11)
    ::XCopyArea(open_display(),
        reinterpret_cast<Window>(src), handle_->pixmap, handle_->context,
        0, 0, r_dst.w, r_dst.h, r_dst.x, r_dst.y);
#endif
    if (changed_ == false) {
      changed_ = true;
    }
  }
}
void graphics_bitblt(const rectangle& r_dst, native_window_type src, const point& p_src)
{
  if (handle_) {
#if defined(NANA_WINDOWS)
    HDC dc = ::GetDC(reinterpret_cast<HWND>(src));
    ::BitBlt(handle_->context, r_dst.x, r_dst.y, r_dst.w, r_dst.h, dc, p_src.x, p_src.y, SRCCOPY);
    ::ReleaseDC(reinterpret_cast<HWND>(src), dc);
#elif defined(NANA_X11)
    ::XCopyArea(open_display(),
        reinterpret_cast<Window>(src), handle_->pixmap, handle_->context,
        p_src.x, p_src.y, r_dst.w, r_dst.h, r_dst.x, r_dst.y);
#endif
    if (changed_ == false) {
      changed_ = true;
    }
  }
}
void graphics_bitblt(const rectangle& r_dst, const graphics& src)
{
  if (handle_ && src.handle_) {
#if defined(NANA_WINDOWS)
    ::BitBlt(handle_->context, r_dst.x, r_dst.y, r_dst.w, r_dst.h, src.handle_->context, 0, 0, SRCCOPY);
#elif defined(NANA_X11)
    ::XCopyArea(open_display(),
        src.handle_->pixmap, handle_->pixmap, handle_->context,
        0, 0, r_dst.w, r_dst.h, r_dst.x, r_dst.y);
#endif
    if (changed_ == false) {
      changed_ = true;
    }
  }
}
void graphics_bitblt(const rectangle& r_dst, const graphics& src, const point& p_src)
{
  if (handle_ && src.handle_) {
#if defined(NANA_WINDOWS)
    ::BitBlt(handle_->context, r_dst.x, r_dst.y, r_dst.w, r_dst.h, src.handle_->context, p_src.x, p_src.y, SRCCOPY);
#elif defined(NANA_X11)
    ::XCopyArea(open_display(),
        src.handle_->pixmap, handle_->pixmap, handle_->context,
        p_src.x, p_src.y, r_dst.w, r_dst.h, r_dst.x, r_dst.y);
#endif
    if (changed_ == false) {
      changed_ = true;
    }
  }
}
void graphics_blend(const rectangle& s_r, graphics& dst, const point& d_pos, double fade_rate) const
{
  if (dst.handle_ && handle_ && (dst.handle_ != handle_)) {
    pixel_buffer s_pixbuf;
    s_pixbuf.attach(handle_, ::rectangle { size() });
    s_pixbuf.blend(s_r, dst.handle_, d_pos, fade_rate);
    if (dst.changed_ == false) {
      dst.changed_ = true;
    }
  }
}
void graphics_blur(const rectangle& r, size_t radius)
{
  if (handle_) {
    pixel_buffer pixbuf(handle_, 0, 0);
    pixbuf.blur(r, radius);
    pixbuf.paste(handle_, point {});
  }
}
void graphics_rgb_to_wb()
{
  if (handle_) {
    //Create the color table for perfermance
    float* tablebuf = new float[0x100 * 3];
    float* table_red = tablebuf;
    float* table_green = tablebuf + 0x100;
    float* table_blue = tablebuf + 0x200;
    for (int i = 0; i < 0x100; ++i) {
      table_red[i] = (i * 0.3f);
      table_green[i] = (i * 0.59f);
      table_blue[i] = (i * 0.11f);
    }
    pixel_buffer pixbuf(handle_, 0, 0);
    auto pixels = pixbuf.raw_ptr(0);
    const size sz = paint:: drawable_size(handle_);
    const int rest = sz.w % 4;
    const int length_align4 = sz.w - rest;
    for (unsigned y = 0; y < sz.h; ++y) {
      const auto end = pixels + length_align4;
      for (; pixels < end; pixels += 4) {
        unsigned char gray = static_cast<unsigned char>(table_red[pixels[0].element.red] + table_green[pixels[0].element.green] + table_blue[pixels[0].element.blue] + 0.5f);
        pixels[0].value = gray << 16 | gray << 8 | gray;
        gray = static_cast<unsigned char>(table_red[pixels[1].element.red] + table_green[pixels[1].element.green] + table_blue[pixels[1].element.blue] + 0.5f);
        pixels[1].value = gray << 16 | gray << 8 | gray;
        gray = static_cast<unsigned char>(table_red[pixels[2].element.red] + table_green[pixels[2].element.green] + table_blue[pixels[2].element.blue] + 0.5f);
        pixels[2].value = gray << 16 | gray << 8 | gray;
        gray = static_cast<unsigned char>(table_red[pixels[3].element.red] + table_green[pixels[3].element.green] + table_blue[pixels[3].element.blue] + 0.5f);
        pixels[3].value = gray << 16 | gray << 8 | gray;
      }
      for (int i = 0; i < rest; ++i) {
        unsigned char gray = static_cast<unsigned char>(table_red[pixels[i].element.red] + table_green[pixels[i].element.green] + table_blue[pixels[i].element.blue] + 0.5f);
        pixels[i].element.red = gray;
        pixels[i].element.green = gray;
        pixels[i].element.blue = gray;
      }
      pixels += rest;
    }
    delete [] tablebuf;
    pixbuf.paste(handle_, point {});
    if (changed_ == false) {
      changed_ = true;
    }
  }
}
void graphics_paste(graphics& dst, int x, int y) const
{
  if (handle_ && dst.handle_ && handle_ != dst.handle_) {
#if defined(NANA_WINDOWS)
    ::BitBlt(dst.handle_->context, x, y, size_.w, size_.h, handle_->context, 0, 0, SRCCOPY);
#elif defined(NANA_X11)
    Display* display = open_display();
    ::XCopyArea(display,
        handle_->pixmap, dst.handle_->pixmap, handle_->context,
        0, 0, size_.w, size_.h, x, y);
    ::XFlush(display);
#endif
    dst.changed_ = true;
  }
}
void graphics_paste(native_window_type dst, const rectangle& r, int x, int y) const
{
  paste(dst, r.x, r.y, r.w, r.h, x, y);
}
void graphics_paste(native_window_type dst, int dx, int dy, unsigned w, unsigned h, int sx, int sy) const
{
  if (handle_) {
#if defined(NANA_WINDOWS)
    HDC dc = ::GetDC(reinterpret_cast<HWND>(dst));
    if (dc) {
      ::BitBlt(dc, dx, dy, w, h, handle_->context, sx, sy, SRCCOPY);
      ::ReleaseDC(reinterpret_cast<HWND>(dst), dc);
    }
#elif defined(NANA_X11)
    Display* display = open_display();
    ::XCopyArea(display,
        handle_->pixmap, reinterpret_cast<Window>(dst), handle_->context,
        sx, sy, w, h, dx, dy);
    ::XMapWindow(display, reinterpret_cast<Window>(dst));
    ::XFlush(display);
#endif
  }
}
void graphics_paste(graphics_t* dst, int x, int y) const
{
  if (handle_ && dst && handle_ != dst) {
#if defined (NANA_WINDOWS)
    ::BitBlt(dst->context, x, y, size_.w, size_.h, handle_->context, 0, 0, SRCCOPY);
#elif defined(NANA_X11)
    Display* display = open_display();
    ::XCopyArea(display,
        handle_->pixmap, dst->pixmap, handle_->context,
        0, 0, size_.w, size_.h, x, y);
    ::XFlush(display);
#endif
  }
}
void graphics_paste(const rectangle& r_src, graphics& dst, int x, int y) const
{
  if (handle_ && dst.handle_ && handle_ != dst.handle_) {
#if defined(NANA_WINDOWS)
    ::BitBlt(dst.handle_->context, x, y, r_src.w, r_src.h, handle_->context, r_src.x, r_src.y, SRCCOPY);
#elif defined(NANA_X11)
    Display* display = open_display();
    ::XCopyArea(display,
        handle_->pixmap, dst.handle_->pixmap, handle_->context,
        r_src.x, r_src.y, r_src.w, r_src.h, x, y);
    ::XFlush(display);
#endif
  }
}
void graphics_stretch(const rectangle& src_r, graphics& dst, const rectangle& r) const
{
  if (handle_ && dst.handle_ && (handle_ != dst.handle_)) {
    pixel_buffer pixbuf(handle_, 0, 0);
    pixbuf.stretch(src_r, dst.handle_, r);
  }
}
void graphics_stretch(graphics& dst, const rectangle& r) const
{
  stretch(rectangle(size()), dst, r);
}
void graphics_flush()
{
#if defined(NANA_WINDOWS)
  ::GdiFlush();
#endif
}
void graphics_save_as_file(graphics_t* g, const char* filename)
{
  if (g) {
#if defined(NANA_WINDOWS)
    int iWidth = static_cast<int>(size_.w);
    int iHeight = static_cast<int>(size_.h);
    BITMAPINFO bmpInfo = {};
    bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
    bmpInfo.bmiHeader.biWidth = static_cast<int>(size_.w);
    bmpInfo.bmiHeader.biHeight = static_cast<int>(size_.h);
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    const size_t lineBytes = ((bmpInfo.bmiHeader.biWidth * 3) + 3) & (~3);
    const size_t imageBytes = iHeight * lineBytes;
    HDC hdcMem = ::CreateCompatibleDC(handle_->context);
    BYTE* pData = NULL;
    HBITMAP hBmp = ::CreateDIBSection(hdcMem, &bmpInfo, DIB_RGB_COLORS, reinterpret_cast<void**>(&pData), 0, 0);
    ::SelectObject(hdcMem, hBmp);
    BitBlt(hdcMem, 0, 0, iWidth, iHeight, handle_->context, 0, 0, SRCCOPY);
    BITMAPFILEHEADER bmFileHeader = { 0 };
    bmFileHeader.bfType = 0x4d42; //bmp
    bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmFileHeader.bfSize = bmFileHeader.bfOffBits + static_cast<DWORD>(imageBytes);
    HANDLE hFile = ::CreateFileW(static_cast< wstring>(::charset(file_utf8, ::unicode::utf8)).data(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwWrite = 0;
    ::WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
    ::WriteFile(hFile, &bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
    ::WriteFile(hFile, pData, static_cast<DWORD>(imageBytes), &dwWrite, NULL);
    ::CloseHandle(hFile);
    ::DeleteObject(hBmp);
    ::DeleteDC(hdcMem);
#elif defined(NANA_X11)
    static_cast<void>(file_utf8); //eliminate unused parameter compil warning.
#endif
  }
}
::color graphics_palette(bool for_text) const
{
  if (handle_) {
    return static_cast<color_rgb>(for_text ? handle_->get_text_color() : handle_->get_color());
  }
  return {};
}
graphics& graphics_palette(bool for_text, const ::color& clr)
{
  if (handle_) {
    if (for_text) {
      handle_->set_text_color(clr);
    }
    else {
      handle_->set_color(clr);
    }
  }
  return *this;
}
unsigned graphics_bidi_string(const point& pos, const wchar_t* str, size_t len)
{
  auto moved_pos = pos;
  unicode_bidi bidi;
  vector<unicode_bidi::entity> reordered;
  bidi.linestr(str, len, reordered);
for (auto & i : reordered) {
    string(moved_pos, i.begin, i.end - i.begin);
    moved_pos.x += static_cast<int>(text_extent_size(i.begin, i.end - i.begin).w);
  }
  return static_cast<unsigned>(moved_pos.x - pos.x);
}
unsigned graphics_bidi_string(const point& pos, const char* str, size_t len)
{
  wstring wstr = ::charset(string(str, str + len), ::unicode::utf8);
  return bidi_string(pos, wstr.data(), wstr.size());
}
void graphics_blend(const rectangle& r, const ::color& clr, double fade_rate)
{
  if (handle_) {
    paint:: blend(handle_, r, clr.px_color(), fade_rate);
    if (changed_ == false) {
      changed_ = true;
    }
  }
}
void graphics_set_pixel(int x, int y, const ::color& clr)
{
  if (handle_) {
    handle_->set_color(clr);
    set_pixel(x, y);
  }
}
void graphics_set_pixel(int x, int y)
{
  if (handle_) {
#if defined(NANA_WINDOWS)
    ::SetPixel(handle_->context, x, y, NANA_RGB(handle_->get_color()));
#elif defined(NANA_X11)
    Display* disp = open_display();
    handle_->update_color();
    ::XDrawPoint(disp, handle_->pixmap, handle_->context, x, y);
#endif
    if (changed_ == false) {
      changed_ = true;
    }
  }
}
void graphics_string(const point& pos, const string& text_utf8)
{
  string(pos, to_wstring(text_utf8));
}
void graphics_string(const point& pos, const string& text_utf8, const color& clr)
{
  palette(true, clr);
  string(pos, text_utf8);
}
void graphics_string(point pos, const wchar_t* str, size_t len)
{
  if (handle_ && str && len) {
    auto const end = str + len;
    auto i = find(str, end, '\t');
#if defined(NANA_LINUX) || defined(NANA_MACOS)
    handle_->update_text_color();
#endif
    if (i != end) {
      size_t tab_pixels = handle_->string.tab_length * handle_->string.tab_pixels;
      while (true) {
        len = i - str;
        if (len) {
          //Render a part that does not contains a tab
          draw_string(handle_, pos, str, len);
          pos.x += raw_text_extent_size(handle_, str, len).w;
        }
        str = i;
        while (str != end && (*str == '\t')) {
          ++str;
        }
        if (str != end) {
          //Now i_tab is not a tab, but a non-tab character following the previous tabs
          pos.x += static_cast<int>(tab_pixels * (str - i));
          i = find(str, end, '\t');
        }
        else {
          break;
        }
      }
    }
    else {
      draw_string(handle_, pos, str, len);
    }
    if (changed_ == false) {
      changed_ = true;
    }
  }
}
void graphics_string(const point& pos, const wchar_t* str)
{
  string(pos, str, wcslen(str));
}
void graphics_string(const point& pos, const wstring& str)
{
  string(pos, str.data(), str.size());
}
void graphics_string(const point& pos, const :: wstring& text, const color& clr)
{
  palette(true, clr);
  string(pos, text.data(), text.size());
}
void graphics_line(const point& pos1, const point& pos2)
{
  if (!handle_) {
    return;
  }
#if defined(NANA_WINDOWS)
  handle_->update_pen();
  if (pos1 != pos2) {
    ::MoveToEx(handle_->context, pos1.x, pos1.y, 0);
    ::LineTo(handle_->context, pos2.x, pos2.y);
  }
  ::SetPixel(handle_->context, pos2.x, pos2.y, NANA_RGB(handle_->pen.color));
#elif defined(NANA_X11)
  Display* disp = open_display();
  handle_->update_color();
  ::XDrawLine(disp, handle_->pixmap, handle_->context, pos1.x, pos1.y, pos2.x, pos2.y);
#endif
  if (changed_ == false) {
    changed_ = true;
  }
}
void graphics_line(const point& pos_a, const point& pos_b, const color& clr)
{
  palette(false, clr);
  line(pos_a, pos_b);
}
void graphics_line_to(const point& pos, const color& clr)
{
  if (!handle_) {
    return;
  }
  handle_->set_color(clr);
  line_to(pos);
}
void graphics_line_to(const point& pos)
{
  if (!handle_) {
    return;
  }
#if defined(NANA_WINDOWS)
  handle_->update_pen();
  ::LineTo(handle_->context, pos.x, pos.y);
#elif defined(NANA_X11)
  Display* disp = open_display();
  handle_->update_color();
  ::XDrawLine(disp, handle_->pixmap, handle_->context,
      handle_->line_begin_pos.x, handle_->line_begin_pos.y,
      pos.x, pos.y);
  handle_->line_begin_pos = pos;
#endif
  if (changed_ == false) {
    changed_ = true;
  }
}
void graphics_rectangle(bool solid)
{
  rectangle(::rectangle { size() }, solid);
}
void graphics_rectangle(bool solid, const ::color& clr)
{
  palette(false, clr);
  rectangle(::rectangle { size() }, solid);
}
void graphics_rectangle(const ::rectangle& r, bool solid)
{
  if (r.w && r.h && handle_ && r.right() > 0 && r.bottom() > 0) {
#if defined(NANA_WINDOWS)
    ::RECT native_r = { r.x, r.y, r.right(), r.bottom()};
    handle_->update_brush();
    (solid ? ::FillRect : ::FrameRect)(handle_->context, &native_r, handle_->brush.handle);
#elif defined(NANA_X11)
    Display* disp = open_display();
    handle_->update_color();
    if (solid) {
      ::XFillRectangle(disp, handle_->pixmap, handle_->context, r.x, r.y, r.w, r.h);
    }
    else {
      ::XDrawRectangle(disp, handle_->pixmap, handle_->context, r.x, r.y, r.w - 1, r.h - 1);
    }
#endif
    if (changed_ == false) {
      changed_ = true;
    }
  }
}
void graphics_rectangle(const ::rectangle& r, bool solid, const color& clr)
{
  palette(false, clr);
  rectangle(r, solid);
}
void graphics_frame_rectangle(const ::rectangle& r, const ::color& left_clr, const ::color& top_clr, const ::color& right_clr, const ::color& bottom_clr)
{
  int right = r.right() - 1;
  int bottom = r.bottom() - 1;
  line_begin(r.x, r.y);
  line_to( { right, r.y }, top_clr);
  line_to( { right, bottom }, right_clr);
  line_to( { r.x, bottom }, bottom_clr);
  line_to( { r.x, r.y }, left_clr);
}
void graphics_frame_rectangle(const ::rectangle& r, const color& clr, unsigned gap)
{
  palette(false, clr);
  if (r.w > gap * 2) {
    point left { r.x + static_cast<int>(gap), r.y }, right { r.right() - static_cast<int>(gap) - 1, r.y };
    line(left, right);
    left.y = right.y = r.bottom() - 1;
    line(left, right);
  }
  if (r.h > gap * 2) {
    point top { r.x, r.y + static_cast<int>(gap) }, bottom { r.x, r.bottom() - static_cast<int>(gap) - 1 };
    line(top, bottom);
    top.x = bottom.x = r.right() - 1;
    line(top, bottom);
  }
}
void graphics_gradual_rectangle(const ::rectangle& rct, const ::color& from, const ::color& to, bool vertical)
{
#if defined(NANA_WINDOWS)
  if (pxbuf_.open(handle_)) {
    pxbuf_.gradual_rectangle(rct, from, to, 0.0, vertical);
    pxbuf_.paste(handle_, point {});
  }
#elif defined(NANA_X11)
  if (NULL == handle_) {
    return;
  }
  double deltapx = double(vertical ? rct.h : rct.w);
  double r, g, b;
  const double delta_r = (to.r() - (r = from.r())) / deltapx;
  const double delta_g = (to.g() - (g = from.g())) / deltapx;
  const double delta_b = (to.b() - (b = from.b())) / deltapx;
  unsigned last_color = (int(r) << 16) | (int(g) << 8) | int(b);
  Display* disp = open_display();
  handle_->fgcolor(static_cast<color_rgb>(last_color));
  const int endpos = deltapx + (vertical ? rct.y : rct.x);
  if (endpos > 0) {
    if (vertical) {
      int x1 = rct.x, x2 = rct.right();
      auto y = rct.y;
      for (; y < endpos; ++y) {
        ::XDrawLine(disp, handle_->pixmap, handle_->context, x1, y, x2, y);
        unsigned new_color = (int(r += delta_r) << 16) | (int(g += delta_g) << 8) | int(b += delta_b);
        if (new_color != last_color) {
          last_color = new_color;
          handle_->fgcolor(static_cast<color_rgb>(last_color));
        }
      }
    }
    else {
      int y1 = rct.y, y2 = rct.bottom();
      auto x = rct.x;
      for (; x < endpos; ++x) {
        ::XDrawLine(disp, handle_->pixmap, handle_->context, x, y1, x, y2);
        unsigned new_color = (int(r += delta_r) << 16) | (int(g += delta_g) << 8) | int(b += delta_b);
        if (new_color != last_color) {
          last_color = new_color;
          handle_->fgcolor(static_cast<color_rgb>(last_color));
        }
      }
    }
  }
#endif
  if (changed_ == false) {
    changed_ = true;
  }
}
void graphics_round_rectangle(const ::rectangle& r, unsigned radius_x, unsigned radius_y, const color& clr, bool solid, const color& solid_clr)
{
  if (handle_) {
#if defined(NANA_WINDOWS)
    handle_->set_color(clr);
    if (solid) {
      handle_->update_pen();
      handle_->brush.set(handle_->context, handle_->brush.Solid, solid_clr.px_color().value);
      ::RoundRect(handle_->context, r.x, r.y, r.right(), r.bottom(), static_cast<int>(radius_x * 2), static_cast<int>(radius_y * 2));
    }
    else {
      handle_->update_brush();
      handle_->round_region.set(r, radius_x, radius_y);
      ::FrameRgn(handle_->context, handle_->round_region.handle, handle_->brush.handle, 1, 1);
    }
    if (changed_ == false) {
      changed_ = true;
    }
#elif defined(NANA_X11)
    if (solid && (clr == solid_clr)) {
      rectangle(r, true, clr);
    }
    else {
      rectangle(r, false, clr);
      if (solid) {
        rectangle(::rectangle(r).pare_off(1), true, solid_clr);
      }
    }
    //eliminate unused parameter compiler warning.
    static_cast<void>(radius_x);
    static_cast<void>(radius_y);
#endif
  }
}
//end class graphics
#endif

