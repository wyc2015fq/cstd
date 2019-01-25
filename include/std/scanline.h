
#include "gdi_c.h"
#include "path_c.h"
#include "region_c.h"
#include "iconv_c.h"

#define scanline_free(s)  if (s && s->free) {s->free(s);}
#define addspan_aa(_b, _e, _y, _a)  sl->addspan(sl, _b, _e, _y, _a)
#define addspan_bin(_b, _e, _y)  sl->addspan(sl, _b, _e, _y, aa_mask)
#define addpix_bin(_x, _y)  sl->addspan(sl, _x, _x+1, _y, aa_mask)
#define addpix_aa(_x, _y, _a)  sl->addspan(sl, _x, _x+1, _y, _a)
#define ICC_PUT_POINT(_y, _x)    sl->addspan(sl, _x, _x+1, _y, aa_mask)
#define ICC_PUT_POINT_AA(_y, _x, _a)    sl->addspan(sl, _x, _x+1, _y, _a)
#define BIN_HLINE(_y, _x1, _x2)  addspan_bin(_x1, _x2+1, _y)

#define CC_FILLED -1

#define CC_AA      16
#define XY_SHIFT   16
#define XY_ONE     (1 << XY_SHIFT)

#define DT_R90   0x01000000
#define DT_R180  0x02000000
#define DT_R270  0x03000000


typedef struct scanline_ scanline;
struct scanline_ {
  void (*setsize)(scanline* s, IRECT rc, int n);
  void (*addspan)(scanline* s, int b, int e, int y, int cover);
  void (*addspans)(scanline* s, int b, int e, int y, const uchar* covers);
  void* x;
  img_t* im;
  const IRECT* pclip;
  IRECT clip[8];
  IRECT pre_dirty;
  IRECT cur_dirty;
};
IRECT scanline_clip(scanline* sl, int x, int y, int w, int h) {
  IRECT clip = iRectUnion(sl->pre_dirty, sl->cur_dirty);
  if (sl->pclip) {
    clip = iRectInter(clip, *sl->pclip);
  } else {
    clip = iRECT2(x, y, w, h);
  }
  return iRectClip(&clip, x, y, x+w, y+h);
}
#define scanline_pushcliprect(sl, rc)  scanline_cliprect(sl, &rc)
#define scanline_popcliprect(sl)        scanline_cliprect(sl, NULL)
#define scanline_resetcliprect(sl)        scanline_cliprect(sl, NULL)
void scanline_cliprect(scanline* sl, const IRECT* prc) {
  if (prc) {
    int top = sl->pclip - sl->clip;
    IRECT dirty = iRectUnion(sl->cur_dirty, sl->pre_dirty);
    IRECT clip = iRectInter(*prc, dirty);
    ASSERT(top >= 0 && top < (countof(sl->clip)-1));
    sl->clip[top+1] = iRectInter(sl->clip[top], clip);
    sl->pclip++;
  } else {
    int top = sl->pclip - sl->clip;
    if (top>0 && top<countof(sl->clip)) {
      sl->pclip--;
    } else {
      sl->pclip = sl->clip;
      sl->clip[0] = iRECT(0, 0, sl->im->w, sl->im->h);
    }
  }
}
int scanline_set_recti(scanline* sl, IRECT rect) {
  int y, w = sl->im->w, h = sl->im->h;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  rect = iRectInter(clip, rect);
  sl->setsize(sl, rect, RCH(&rect));
  for (y = rect.t; y < rect.b; ++y) {
    addspan_bin(rect.l, rect.r, y);
  }
  return 0;
}
int scanline_set_poly(scanline* sl, const FPOINT* pt0, const int* lens, int n, BOOL fill_even_odd)
{
  int w = sl->im->w, h = sl->im->h;
  scancell s[1] = {0};
  cell_aa* cells;
  int y;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  IRECT rect = {0};
  int is_irc = is_int_rect(pt0, lens, n, &rect);
  buf_t bf[1] = {0};
  IRECT rc = {0};
  if (is_irc) {
    rect = iRectInter(clip, rect);
    sl->setsize(sl, rect, RCH(&rect));
    for (y = rect.t; y < rect.b; ++y) {
      addspan_bin(rect.l, rect.r, y);
    }
    return 0;
  }
  scancell_set_d(s, pt0, lens, n, &clip, &rc);
  sl->setsize(sl, rc, s->celllen * 2);
  if (NULL == s->cells) {
    return 0;
  }
  cells = s->cells;
  //cellslen = s->celllen;
  //max_y = BOUND(cells[cellslen - 1].y, rc.t, rc.b - 1);
  while (cells->y < rc.t) {
    ++cells;
  }
  for (y = rc.t; y < rc.b; ++y) {
    int cover = 0;
    while (y == cells->y) {
      const cell_aa* cur_cell = cells;
      int x = cur_cell->x, x_end, area = cur_cell->area;
      unsigned char alpha = 0;
      cover += cur_cell->cover;
      //accumulate all cells with the same X
      while (y == cells->y) {
        cur_cell = ++cells;
        if (cur_cell->x != x || cur_cell->y != y) {
          break;
        }
        area += cur_cell->area;
        cover += cur_cell->cover;
      }
      if (area) {
        area = (cover << (poly_subpixel_shift + 1)) - area;
        CALCULATE_ALPHA(alpha, area);
        if (alpha && x >= clip.l && x < clip.r) {
          sl->addspan(sl, x, x + 1, y, alpha);
        }
        x++;
      }
      if (x < cur_cell->x) {
        area = cover << (poly_subpixel_shift + 1);
        CALCULATE_ALPHA(alpha, area);
        if (alpha) {
          x = MAX(x, 0);
          x_end = MIN(cur_cell->x, rc.r);
          if (x<x_end) {
            sl->addspan(sl, x, x_end, y, alpha);
          }
        }
      }
    }
  }
  FREE(s->cells);
  return 0;
}

int scanline_set_spanv(scanline* sl, const spanv_t* sp)
{
  const span_t* span = sp->s;
  const span_t* spane = sp->s + sp->l;
  sl->setsize(sl, sp->rc, sp->l);
  for (; span != spane; ++span) {
    sl->addspan(sl, span->b, span->e, span->y, span->cover);
  }
  return 0;
}
int scanline_set_region(scanline* sl, const region_t* re)
{
  int i=0, y;
  IRECT rc = re->extents;
  IRECT clip = iRectClip(&re->extents, 0, 0, sl->im->w, sl->im->h);
  sl->setsize(sl, clip, RCH(&clip));
  for (i=0; i < re->numRects; ++i) {
    rc = iRectInter(clip, re->rects[i]);
    for (y = rc.t; y<rc.b; ++y) {
      sl->addspan(sl, rc.l, rc.r, y, aa_mask);
    }
  }
  return 0;
}
int scanline_set_rect(scanline* s, double x, double y, double w, double h)
{
  FPOINT pt[4];
  int n = vcgen_rect(pt, 0, x, y, x + w, y + h);
  return scanline_set_poly(s, pt, &n, 1, false);
}
int scanline_set_ellipse(scanline* s, double x, double y, double rx, double ry, double begin_angle, double end_angle) {
  FPOINT pt[64];
  int n = vcgen_ellipse(pt, countof(pt), 0, x, y, rx, ry, begin_angle, end_angle);
  return scanline_set_poly(s, pt, &n, 1, false);
}
int scanline_set_triangle(scanline* s, double x1, double y1, double x2, double y2, double x3, double y3) {
  FPOINT pt[3] = {(float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3};
  int n = 3;
  return scanline_set_poly(s, pt, &n, 1, false);
}
int scanline_set_poly_stroke_impl(scanline* sl, const FPOINT* pt, int ptlen, BOOL closed, PenStyle* penstyle) {
  int n2, pos=0, maxptlen;
  PenStyle penstyle1[1] = {0};
  if (NULL==penstyle) {
    penstyle = penstyle1;
    PenStyle_Init(penstyle);
  }
  maxptlen = 8 + ptlen * 2 * (int)MAX(10, penstyle->lineWidth);
  if (ptlen > 1) {
    int lens[10] = {0};
    FPOINT* pt_buf = NULL;
    MYREALLOC(pt_buf, maxptlen);
    n2 = closed ? 2 : 1;
    vcgen_stroke(pt_buf, NULL, 0, lens, pt, ptlen, closed, penstyle);
    scanline_set_poly(sl, pt_buf, lens, n2, false);
    FREE(pt_buf);
  }
  return 0;
}
int scanline_set_poly_stroke2(scanline* sl, const FPOINT* Points, const BYTE* Types, int Count, PenStyle* penstyle) {
  PATH_LOOP_BEGIN;
  scanline_set_poly_stroke_impl(sl, Points + pos, end-pos, closed, penstyle);
  PATH_LOOP_END;
  return 0;
}
int scanline_set_poly_dash_stroke(scanline* sl, const FPOINT* pt, int ptlen, BOOL closed, PenStyle* penstyle) {
  if (penstyle && penstyle->numdash>1) {
    FPOINT pt0[1000];
    int lens[1000];
    int i, k = 0, n = vcgen_dash(pt, ptlen, closed, penstyle->dashPattern, penstyle->numdash, penstyle->dashPhase, countof(pt0), countof(lens), pt0, lens);
    
    for (i = 0; i < n; i++) {
      scanline_set_poly_stroke_impl(sl, pt0 + k, lens[i], 0, penstyle);
      k += lens[i];
    }
  } else {
    scanline_set_poly_stroke_impl(sl, pt, ptlen, closed, penstyle);
  }
  return 0;
}
int scanline_set_poly_dash_stroke_i(scanline* sl, const IPOINT* pt, int ptlen, BOOL closed, int shift, PenStyle* penstyle) {
  int i, ret=0;
  FPOINT* fpt = NULL;
  double ss = 1./(1<<shift);
  MYREALLOC(fpt, ptlen);
  for (i=0; i<ptlen; ++i) {
    fpt[i].x = pt[i].x*ss;
    fpt[i].y = pt[i].y*ss;
  }
  ret = scanline_set_poly_dash_stroke(sl, fpt, ptlen, closed, penstyle);
  FREE(fpt);
  return ret;
}
int scanline_set_glyph(scanline* sl, const IRECT* pclip, const PrimRectUV* prcuv, int nrcuv, const bitmap_t* tex1) {
  int w = sl->im->w, h = sl->im->h;
  int i, j, y;
  void* buf = NULL;
  int* xoff = NULL;
  int* yoff = NULL;
  uchar* covers = NULL;
  IRECT clip = {0, 0, w, h}, rect;
  if (pclip) {
    clip = iRectInter(clip, *pclip);
  }
  for (i = 0; i < nrcuv; ++i) {
    IRECT drc = iRECT((int)prcuv[i].a.x, (int)prcuv[i].a.y, (int)prcuv[i].c.x, (int)prcuv[i].c.y);
    int dw = RCW(&drc), dh = RCH(&drc);
    ImVec2 uv_a = prcuv[i].uv_a;
    ImVec2 uv_c = prcuv[i].uv_c;
    float uv_w = (uv_c.x - uv_a.x) / dw;
    float uv_h = (uv_c.y - uv_a.y) / dh;
    struct pixfun_t* fun = get_pixfun(tex1->fmt);
    rect = iRectInter(clip, drc);
    multialloc(&buf, &xoff, dw*sizeof(int), &yoff, dh*sizeof(int), &covers, dw, NULL);
    for (j = 0; j < dw; ++j) {
      xoff[j] = (int)((uv_a.x + uv_w * j) * tex1->w);
    }
    for (j = 0; j < dh; ++j) {
      yoff[j] = (int)((uv_a.y + uv_h * j) * tex1->h);
    }
    memset(covers, 255, dw);
    
    sl->setsize(sl, rect, RCH(&rect));
    dw = rect.r - rect.l;
    for (y = rect.t; y < rect.b; ++y) {
      const uchar* psrc = tex1->data + yoff[y - drc.t] * tex1->s;
      fun->getspan8(dw, covers, psrc, 0, NULL);
      sl->addspans(sl, rect.l, rect.r, y, covers);
    }
  }
  FREE(buf);
  return 0;
}
const wchar_t* font_CalcWordWrapPositionW(const font_t* g, float scale, const wchar_t* text, const wchar_t* text_end, float wrap_width)
{
  float line_width = 0.0f;
  float word_width = 0.0f;
  float blank_width = 0.0f;
  const wchar_t* word_end = text;
  const wchar_t* prev_word_end = NULL;
  BOOL inside_word = true;
  const wchar_t* s = text;
  for (; s < text_end; ) {
    unsigned int c = (unsigned int) *s;
    const wchar_t* next_s = s+1;
    float char_width;
    int advance, lsb;
    if (c == 0) {
      break;
    }
    if (c < 32) {
      if (c == L'\n') {
        line_width = word_width = blank_width = 0.0f;
        inside_word = true;
        s = next_s;
        continue;
      }
      if (c == L'\r') {
        s = next_s;
        continue;
      }
    }
    g->fun->GetCodepointHMetrics(g, c, &advance, &lsb);
    char_width = advance*scale;
    if (ImCharIsSpaceW(c)) {
      if (inside_word) {
        line_width += blank_width;
        blank_width = 0.0f;
      }
      blank_width += char_width;
      inside_word = false;
    }
    else {
      word_width += char_width;
      if (inside_word) {
        word_end = next_s;
      }
      else {
        prev_word_end = word_end;
        line_width += word_width + blank_width;
        word_width = blank_width = 0.0f;
      }
      // Allow wrapping after punctuation.
      inside_word = !(c == '.' || c == ',' || c == ';' || c == '!' || c == '?' || c == '\"');
    }
    // We ignore blank width at the end of the line (they can be skipped)
    if (line_width + word_width >= wrap_width) {
      // Words that cannot possibly fit within an entire line will be cut anywhere.
      if (word_width < wrap_width) {
        s = prev_word_end ? prev_word_end : word_end;
      }
      break;
    }
    s = next_s;
  }
  return s;
}
ImVec2 font_CalcTextSizeW(const font_t* g, float size, float max_width, float wrap_width, const wchar_t* text_begin, const wchar_t* text_end, const wchar_t** remaining)
{
  const float font_size = size;
  const float line_height = font_size;
  const float scale = g->fun->ScaleForPixelHeight(g, font_size);
  ImVec2 text_size = fVec2(0, 0);
  float char_width, line_width = 0.0f;
  const BOOL word_wrap_enabled = (wrap_width > 0.0f);
  const wchar_t* word_wrap_eol = NULL;
  const wchar_t* s;
  const wchar_t* prev_s;
  unsigned int c;
  int advance, lsb;
  if (NULL==text_begin) {
    text_begin = L"test";
    return text_size;
  }
  s = text_begin;
  if (!text_end) {
    text_end = text_begin + wcslen(text_begin); // FIXME-OPT: Need to avoid this.
  }
  while (s < text_end) {
    if (word_wrap_enabled) {
      // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
      if (!word_wrap_eol) {
        word_wrap_eol = font_CalcWordWrapPositionW(g, scale, s, text_end, wrap_width - line_width);
        if (word_wrap_eol == s) { // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
          word_wrap_eol++; // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
        }
      }
      if (s >= word_wrap_eol) {
        if (text_size.x < line_width) {
          text_size.x = line_width;
        }
        text_size.y += line_height;
        line_width = 0.0f;
        word_wrap_eol = NULL;
        // Wrapping skips upcoming blanks
        while (s < text_end) {
          const char c = *s;
          if (ImCharIsSpaceW(c)) {
            s++;
          }
          else if (c == '\n') {
            s++;
            break;
          }
          else {
            break;
          }
        }
        continue;
      }
    }
    // Decode and advance source
    prev_s = s;
    c = (unsigned int) * s++;
    if (!c) { break; }
    if (c < 32) {
      if (c == '\n') {
        text_size.x = MAX(text_size.x, line_width);
        text_size.y += line_height;
        line_width = 0.0f;
        continue;
      }
      if (c == '\r') {
        continue;
      }
    }
    g->fun->GetCodepointHMetrics(g, c, &advance, &lsb);
    char_width = advance * scale;
    if (line_width + char_width >= (max_width)) {
      s = prev_s;
      break;
    }
    line_width += char_width;
  }
  if (text_size.x < line_width) {
    text_size.x = line_width;
  }
  if (line_width > 0 || text_size.y == 0.0f) {
    text_size.y += line_height;
  }
  if (remaining) {
    *remaining = s;
  }
  return text_size;
}
int scanline_rendertext(scanline* sl, const font_t* font, float size, ImVec2 pos, FRECT clip_rect, const wchar_t* text_begin, const wchar_t* text_end, float wrap_width, BOOL cpu_fine_clip)
{
  float xpos, ypos;
  float line_height;
  BOOL word_wrap_enabled;
  const wchar_t* word_wrap_eol = NULL;
  const wchar_t* s;
  float font_size = size;
  const float scale = font->fun->ScaleForPixelHeight(font, font_size);
  IPOINT DisplayOffset = {0, 0};
  uchar* bit = NULL;
  if (!text_end) {
    text_end = text_begin + wcslen(text_begin);
  }
  // Align to be pixel perfect
  pos.x = (float)(int)pos.x + DisplayOffset.x;
  pos.y = (float)(int)pos.y + DisplayOffset.y;
  xpos = pos.x;
  ypos = pos.y;
  if (ypos > clip_rect.b) {
    return 0;
  }
  line_height = font_size * scale;
  word_wrap_enabled = (wrap_width > 0.0f);
  // Skip non-visible lines
  s = text_begin;
  if (!word_wrap_enabled && ypos + line_height < clip_rect.t) {
    while (s < text_end && *s != '\n') { // Fast-forward to next line
      s++;
    }
  }
  {
    // Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
    while (s < text_end) {
      unsigned int c;
      int advance, lsb, x0, y0, x1, y1;
      float char_width;
      int XAdvance = 0;
      if (word_wrap_enabled) {
        // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
        if (!word_wrap_eol) {
          word_wrap_eol = font_CalcWordWrapPositionW(font, scale, s, text_end, wrap_width - (xpos - pos.x));
          if (word_wrap_eol == s) { // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
            word_wrap_eol++; // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
          }
        }
        if (s >= word_wrap_eol) {
          xpos = pos.x;
          ypos += line_height;
          word_wrap_eol = NULL;
          // Wrapping skips upcoming blanks
          while (s < text_end) {
            const char c = *s;
            if (ImCharIsSpaceW(c)) {
              s++;
            }
            else if (c == '\n') {
              s++;
              break;
            }
            else {
              break;
            }
          }
          continue;
        }
      }
      // Decode and advance source
      c = (unsigned int) *s++;
      if (c == 0) {
        break;
      }
      if (c < 32) {
        if (c == '\n') {
          xpos = pos.x;
          ypos += line_height;
          if (ypos > clip_rect.b) {
            break;
          }
          if (!word_wrap_enabled && ypos + line_height < clip_rect.t)
            while (s < text_end && *s != '\n') { // Fast-forward to next line
              s++;
            }
          continue;
        }
        if (c == '\r') {
          continue;
        }
      }
      float x_shift = xpos - (float)floor(xpos);
      font->fun->GetCodepointHMetrics(font, c, &advance, &lsb);
      char_width = advance * scale;
      font->fun->GetCodepointBitmapBoxSubpixel(font, c, x_shift, 0, &x0, &y0, &x1, &y1);
      x0 += xpos;
      x1 += xpos;
      y0 += ypos;
      y1 += ypos;
      if (x1<clip_rect.l || y1<clip_rect.t || x0 > clip_rect.r || y0>clip_rect.b) break;
      x0 = BOUND(x0, clip_rect.l, clip_rect.r);
      x1 = BOUND(x1, clip_rect.l, clip_rect.r);
      y0 = BOUND(y0, clip_rect.t, clip_rect.b);
      y1 = BOUND(y1, clip_rect.t, clip_rect.b);

      if (XAdvance) {
        char_width = XAdvance * scale;
        // Arbitrarily assume that both space and tabs are empty glyphs as an optimization
        if (c != ' ' && c != '\t') {
          // We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.t and exit once we pass clip_rect.b
          if (1) {
            // Render a character
            float u1 = 0;
            float v1 = 0;
            float u2 = 1;
            float v2 = 1;
            // CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
            if (cpu_fine_clip) {
              if (x0 < clip_rect.l) {
                u1 = u1 + (1.0f - (x1 - clip_rect.l) / (x1 - x0)) * (u2 - u1);
                x0 = clip_rect.l;
              }
              if (y0 < clip_rect.t) {
                v1 = v1 + (1.0f - (y1 - clip_rect.t) / (y1 - y0)) * (v2 - v1);
                y0 = clip_rect.t;
              }
              if (x1 > clip_rect.r) {
                u2 = u1 + ((clip_rect.r - x0) / (x1 - x0)) * (u2 - u1);
                x1 = clip_rect.r;
              }
              if (y1 > clip_rect.b) {
                v2 = v1 + ((clip_rect.b - y0) / (y1 - y0)) * (v2 - v1);
                y1 = clip_rect.b;
              }
              if (y0 >= y1) {
                xpos += char_width;
                continue;
              }
            }
            // We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug build.
            // Inlined here:
            {
              PrimRectUV uv[1];
              texture_t tex[1] = {0};
              int hh = y1 - y0, ww = x1 - x0;
              bit = (uchar*)realloc(bit, hh*ww);
              memset(bit, 0, hh*ww);
              font->fun->MakeCodepointBitmapSubpixel(font, bit, ww, hh, ww, scale, scale, x_shift, 0, c);
              BMPINIT(tex, hh, ww, bit, ww, 8);
              tex->fmt = bpp2PixFmt(8);
              PrimRectUV_set(uv, fVec2(x1, y0), fVec2(x1, y1), fVec2(u1, v1), fVec2(u2, v2));
              //softgc_rander_text(sg, uv, 1, tex);
              scanline_set_glyph(sl, sl->pclip, uv, 1, tex);
            }
          }
        }
      }
      xpos += char_width;
    }
  }
  if (bit) free(bit);
  // Give back unused vertices
  return 0;
}
CC_INLINE void scanline_set_string(scanline* sl, int opt, const wchar_t* text, const wchar_t* text_end, font_t* font, float size, float x, float y, float w, float h, int formatFlags)
{
  if (text && font) {
    ImVec2 sz;
    FRECT rc = fRECT2(x, y, w, h);
    int textlen = text_end ? (text_end - text) : wcslen(text);
    sz = font_CalcTextSizeW(font, size, w, w, text, text + textlen, NULL);
    fRectAlign(rc, sz.x, sz.y, formatFlags, &rc);
    scanline_rendertext(sl, font, size, fVec2(rc.l, rc.t), rc, text, text + textlen, w, TRUE);
  }
}
CC_INLINE void scanline_set_string(scanline* sl, int opt, const char* text, const char* text_end, font_t* font, float size, float x, float y, float w, float h, int formatFlags)
{
  text_end = text_end ? text_end : (text + strlen(text));
  int len = text_end - text;
  wchar_t* wtext = iconv_a2w(text, len);
  scanline_set_string(sl, opt, wtext, NULL, font, size, x, y, w, h, formatFlags);
  free(wtext);
}
CC_INLINE void scanline_set_string_format(scanline* sl, IRECT rc, font_t* font, int formatFlags, const char* fmt, ...) {
  char* buf = NULL;
  va_list arglist;
  va_start(arglist, fmt);
  avprintf(&buf, 0, fmt, arglist);
  va_end(arglist);
  scanline_set_string(sl, 1, buf, NULL, font, 0, (float)(rc).l, (float)(rc).t, (float)RCW(&rc), (float)RCH(&rc), formatFlags);
  free(buf);
}
typedef struct {
  spanv_t* sp;
  span_t* span;
  span_t* spanb;
  span_t* spane;
} scanline_spanv;
void scanline_spanv_setsize(scanline* s, IRECT rc, int n) {
  scanline_spanv* ss = (scanline_spanv*)(s->x);
  spanv_setsize(ss->sp, rc, n);
  ss->span = ss->spanb = ss->sp->s;
  ss->spane = ss->sp->s + ss->sp->max_l;
}
void scanline_spanv_addspan(scanline* s, int b, int e, int y, int cover) {
  scanline_spanv* ss = (scanline_spanv*)(s->x);
  if (ss->sp->l==0 || y!=ss->span[-1].y) {
    ss->spanb = ss->span;
  }
  ss->span = span_push(ss->span, ss->spanb, ss->spane, b, e, y, cover);
  ss->sp->l = ss->span - ss->sp->s;
}
int spanv_set_poly(spanv_t* sp, img_t* im, const IRECT* pclip, const FPOINT* pt, const int* lens, int n, BOOL fill_even_odd) {
  scanline s[1] = {0};
  scanline_spanv ss[1] = {0};
  ss->sp = sp;
  s->x = ss;
  s->im = im;
  s->pclip = pclip;
  s->setsize = scanline_spanv_setsize;
  s->addspan = scanline_spanv_addspan;
  s->cur_dirty = iRECT(0, 0, im->w, im->h);
  return scanline_set_poly(s, pt, lens, n, fill_even_odd);
}
int spanv_set_rect(spanv_t* sp, img_t* im, const IRECT* pclip, double x, double y, double w, double h)
{
  FPOINT pt[4];
  int n = vcgen_rect(pt, 0, x, y, x + w, y + h);
  return spanv_set_poly(sp, im, pclip, pt, &n, 1, false);
}
int spanv_set_rectR(spanv_t* sp, img_t* im, const IRECT* pclip, IRECT rc)
{
  return spanv_set_rect(sp, im, pclip, rc.l, rc.t, RCW(&rc), RCH(&rc));
}

// 中点画圆法
// d = F（xi + 1, yi – 0.5）= (xi + 1)2 + (yi – 0.5)2 – R2
// 若d < 0，则取P1为下一个点，此时P1的下一个点的判别式为：
// d’ = F（xi + 2, yi – 0.5）= (xi + 2)2 + (yi – 0.5)2 – R2
// 展开后将d带入可得到判别式的递推关系： d’ = d + 2xi + 3
// 若d > 0，则取P2为下一个点，此时P2的下一个点的判别式为：
// d’ = F（xi + 2, yi – 1.5）= (xi + 2)2 + (yi – 1.5)2 – R2
// 展开后将d带入可得到判别式的递推关系：d’ = d + 2(xi - yi) + 5
// d0 = F(1, R – 0.5) = 1 + (R – 0.5)2 – R2 = 1.25 - R
// 中点画椭圆法
// d = F（xi + 1, yi – 0.5）= ((xi + 1)*h/w)2 + (yi – 0.5)2 – h2
// 若d < 0，则取P1为下一个点，此时P1的下一个点的判别式为：
// d’ = F（xi + 2, yi – 0.5）= ((xi + 2)*h/w)2 + (yi – 0.5)2 – h2
// 展开后将d带入可得到判别式的递推关系： d’ = d + (2xi + 3)*(h/w)2
// 若d > 0，则取P2为下一个点，此时P2的下一个点的判别式为：
// d’ = F（xi + 1, yi – 1.5）= ((xi + 1)*h/w)2 + (yi – 1.5)2 – R2
// 展开后将d带入可得到判别式的递推关系：d’ = d + 2(xi - yi) + 5
// d0 = F(1, h – 0.5) = (h/w)2 + (h – 0.5)2 – h2 = (h/w)2 + 0.25 - h

CC_INLINE int draw_circle_map2(double h, double w, int* mapx)
{
  double hw = h / w;
  double hw2 = hw * hw;
  double x = 0, y = h, d = hw2 + 0.25 - h;

  while (y >= 0) {
    if (d < 0) {
      d += (2 * x + 3) * hw2;
      ++x;
    }
    else {
      d += 2 * (x - y) + 5;
      mapx[(int)y] = (int)x;
      --y;
    }
  }

  return 0;
}

CC_INLINE int scanline_bin_ellipse(scanline* sl, int x1, int y1, int x2, int y2, bool fill, int wline)
{
  int xa, xb;
  int x, cx = (x1 + x2), w = (x2 - x1);
  int y, cy = (y1 + y2), h = (y2 - y1);
  int64 h2 = h * h, w2 = w * w;
  int64 hw2 = h2 * w2;
  int64 h3 = (h - wline) * (h - wline), w3 = (w - wline) * (w - wline);
  int64 hw3 = h3 * w3;
  IRECT rc = iRECT(x1, y1, x2, y2);
  sl->setsize(sl, rc, RCH(&rc));
  if (cy & 1) {
    xa = x1, xb = x2;
    
    if (fill) {
      addspan_bin(xa, xb, cy / 2);
    }

    if (wline>0) {
      addspan_bin(xa, xa + wline, cy / 2);
      addspan_bin(x2 - wline, x2, cy / 2);
      xa = x1 + wline, xb = x2 - wline;
    }
  }

  for (y = cy / 2; y-- > y1;) {
    int yy = cy - 1 - y;
    for (x = x1; x1 < x2; ++x, ++x1, --x2) {
      int dy = 2 * y - cy + 1, dx = 2 * x - cx + 1;
      int64 d = dy * dy * w2 + dx * dx * h2;
      if (d <= hw2) {
        break;
      }
    }

    xa = x1, xb = x2;

    if (fill) {
      addspan_bin(xa, xb, y);
      addspan_bin(xa, xb, yy);
    }
    
    if (wline>0) {
      int x3 = x1, x4 = x2;

      for (x = x1; x3 < x4; ++x, ++x3, --x4) {
        int dy = 2 * y - cy + 1, dx = 2 * x - cx + 1;
        int64 d = dy * dy * w2 + dx * dx * h2;

        if (d < hw3) {
          break;
        }
      }
      addspan_bin(x1, x3, y);
      addspan_bin(x4, x2, y);
      addspan_bin(x1, x3, yy);
      addspan_bin(x4, x2, yy);
      xa = x3, xb = x4;
    }

  }

  return 0;
}

CC_INLINE int draw_circle_map3(int r, int* mapx)
{
  int x = 0, y = r, d = 3 - 2 * r;

  while (x <= y) {
    mapx[x] = y;
    mapx[y] = x;

    if (d < 0) {
      d += 4 * x + 6;
    }
    else {
      d += 4 * (x - y) + 10;
      y--;
    }

    x++;
  }

  return 0;
}

CC_INLINE int scanline_bin_circle(scanline* sl, int cx, int cy, int radius, bool fill)
{
  int w = sl->im->w, h = sl->im->h;
  int x, y, inside;
  int err = 0, dx = radius, dy = 0, plus = 1, minus = (radius << 1) - 1;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  x = cx;
  y = cy;
  inside = x >= clip.l + radius && x < clip.r - radius && y >= clip.t + radius && y < clip.b - radius;

  sl->setsize(sl, clip, RCH(&clip));
  while (dx >= dy) {
    int mask;
    int y11 = y - dy, y12 = y + dy, y21 = y - dx, y22 = y + dx;
    int x11 = x - dx, x12 = x + dx, x21 = x - dy, x22 = x + dy;

    if (inside) {
      int tptr0 = y11;
      int tptr1 = y12;

      if (!fill) {
        ICC_PUT_POINT(tptr0, x11);
        ICC_PUT_POINT(tptr1, x11);
        ICC_PUT_POINT(tptr0, x12);
        ICC_PUT_POINT(tptr1, x12);
      }
      else {
        BIN_HLINE(tptr0, x11, x12);
        BIN_HLINE(tptr1, x11, x12);
      }

      tptr0 = y21;
      tptr1 = y22;

      if (!fill) {
        ICC_PUT_POINT(tptr0, x21);
        ICC_PUT_POINT(tptr1, x21);
        ICC_PUT_POINT(tptr0, x22);
        ICC_PUT_POINT(tptr1, x22);
      }
      else {
        BIN_HLINE(tptr0, x21, x22);
        BIN_HLINE(tptr1, x21, x22);
      }
    }
    else if (x11 < clip.r && x12 >= clip.l && y21 < clip.b && y22 >= clip.t) {
      if (fill) {
        x11 = MAX(x11, clip.l);
        x12 = MIN(x12, clip.r - 1);
      }

      if ((unsigned) y11 < (unsigned) h) {
        int tptr = y11;

        if (!fill) {
          if (x11 >= clip.l) {
            ICC_PUT_POINT(tptr, x11);
          }

          if (x12 < clip.r) {
            ICC_PUT_POINT(tptr, x12);
          }
        }
        else {
          BIN_HLINE(tptr, x11, x12);
        }
      }

      if ((unsigned) y12 < (unsigned) h) {
        int tptr = y12;

        if (!fill) {
          if (x11 >= clip.l) {
            ICC_PUT_POINT(tptr, x11);
          }

          if (x12 < clip.r) {
            ICC_PUT_POINT(tptr, x12);
          }
        }
        else {
          BIN_HLINE(tptr, x11, x12);
        }
      }

      if (x21 < clip.r && x22 >= clip.l) {
        if (fill) {
          x21 = MAX(x21, clip.l);
          x22 = MIN(x22, clip.r - 1);
        }

        if (y21 < clip.b && y21 >= clip.t) {
          int tptr = y21;

          if (!fill) {
            if (x21 >= clip.l) {
              ICC_PUT_POINT(tptr, x21);
            }

            if (x22 < clip.r) {
              ICC_PUT_POINT(tptr, x22);
            }
          }
          else {
            BIN_HLINE(tptr, x21, x22);
          }
        }

        if (y22 < clip.b && y22 >= clip.t) {
          int tptr = y22;

          if (!fill) {
            if (x21 >= clip.l) {
              ICC_PUT_POINT(tptr, x21);
            }

            if (x22 < clip.r) {
              ICC_PUT_POINT(tptr, x22);
            }
          }
          else {
            BIN_HLINE(tptr, x21, x22);
          }
        }
      }
    }

    dy++;
    err += plus;
    plus += 2;

    mask = (err <= 0) - 1;

    err -= minus & mask;
    dx += mask;
    minus -= mask & 2;
  }

  return 0;
}

typedef struct PolyEdge {
  int x, dx;
  int y0, y1;
  struct PolyEdge* next;
}
PolyEdge;

CC_INLINE COLOR hsv2rgb(float hue)
{
  int rgb[ 3 ], p, sector;
  static const int sector_data[][ 3 ] = { {0, 2, 1}, {1, 2, 0}, {1, 0, 2}, {2, 0, 1}, {2, 1, 0}, {0, 1, 2} };
  hue *= 0.033333333333333333333333333333333f;
  sector = FLOOR(hue);
  p = ROUND(255 * (hue - sector));
  p ^= sector & 1 ? 255 : 0;

  rgb[ sector_data[ sector ][ 0 ] ] = 255;
  rgb[ sector_data[ sector ][ 1 ] ] = 0;
  rgb[ sector_data[ sector ][ 2 ] ] = p;

  return _RGB(rgb[ 0 ], rgb[ 1 ], rgb[ 1 ]);
}



CC_INLINE int scanline_bin_function(scanline* sl, double(*fun)(double), int wline)
{
  int w = sl->im->w, h = sl->im->h;
  int x, y;
  double y1, y2;
  int start = 0, end = w;

  if (wline < -1 || wline > 255) {
    return 0;
  }

  x = start;
  y1 = ROUND(fun(x));
  x += 1;

  for (; x < end; x += 1) {
    int tptr;
    y2 = ROUND(fun(x));
    y1 = MAX(y1, 0), y1 = MIN(y1, h - 1);
    y2 = MAX(y2, 0), y2 = MIN(y2, h - 1);

    for (y = (int)y1; y < (y1 + y2) / 2; ++y) {
      tptr = y;
      ICC_PUT_POINT(tptr, x - 1);
    }

    for (; y < y2; ++y) {
      tptr = y;
      ICC_PUT_POINT(tptr, x);
    }

    y1 = y2;
  }

  return 0;
}

CC_INLINE int scanline_bin_points(scanline* sl, IPOINT* pt, int n, int wline)
{
  int w = sl->im->w, h = sl->im->h;
  int i, x, y;

  if (wline < -1 || wline > 255) {
    return 0;
  }

  n = MAX(n, 0), n = MIN(n, h - 1);

  for (i = 0; i < n; ++i) {
    int tptr;
    x = pt[i].x, y = pt[i].y;
    y = MAX(y, 0), y = MIN(y, h - 1);
    tptr = y;
    ICC_PUT_POINT(tptr, x - 1);
  }

  return 0;
}

CC_INLINE int DrawPoints(scanline* sl, IPOINT* pt, int n, int wline)
{
  int i, x, y;
  int w = sl->im->w, h = sl->im->h;

  if (wline < -1 || wline > 255) {
    return 0;
  }

  n = MAX(n, 0), n = MIN(n, h - 1);

  for (i = 0; i < n; ++i) {
    x = BOUND(pt[i].x, 0, w-1);
    y = BOUND(pt[i].y, 0, h-1);
    sl->addspan(sl, x, x+1, y, 255);
  }

  return 0;
}

CC_INLINE int ClipLine1(const IRECT* pclip, IPOINT* pt1, IPOINT* pt2)
{
  int result = 0;
  
  int x1, y1, x2, y2;
  int c1, c2;
  int left = pclip->l, top = pclip->t;
  int right = pclip->r - 1, bottom = pclip->b - 1;
  
  if (!pt1 || !pt2) {
    return (1);
  }
  
  if (right < 0 || bottom < 0) {
    return (1);
  }
  
  x1 = pt1->x;
  y1 = pt1->y;
  x2 = pt2->x;
  y2 = pt2->y;
  c1 = (x1 < left) + (x1 > right) * 2 + (y1 < top) * 4 + (y1 > bottom) * 8;
  c2 = (x2 < left) + (x2 > right) * 2 + (y2 < top) * 4 + (y2 > bottom) * 8;
  
  if ((c1 & c2) == 0 && (c1 | c2) != 0) {
    int a;
    
    if (c1 & 12) {
      a = c1 < 8 ? 0 : bottom;
      x1 += (int)(((int64)(a - y1)) * (x2 - x1) / (y2 - y1));
      y1 = a;
      c1 = (x1 < 0) + (x1 > right) * 2;
    }
    
    if (c2 & 12) {
      a = c2 < 8 ? 0 : bottom;
      x2 += (int)(((int64)(a - y2)) * (x2 - x1) / (y2 - y1));
      y2 = a;
      c2 = (x2 < 0) + (x2 > right) * 2;
    }
    
    if ((c1 & c2) == 0 && (c1 | c2) != 0) {
      if (c1) {
        a = c1 == 1 ? 0 : right;
        y1 += (int)(((int64)(a - x1)) * (y2 - y1) / (x2 - x1));
        x1 = a;
        c1 = 0;
      }
      
      if (c2) {
        a = c2 == 1 ? 0 : right;
        y2 += (int)(((int64)(a - x2)) * (y2 - y1) / (x2 - x1));
        x2 = a;
        c2 = 0;
      }
    }
    
    assert((c1 & c2) != 0 || (x1 | y1 | x2 | y2) >= 0);
    
    pt1->x = x1;
    pt1->y = y1;
    pt2->x = x2;
    pt2->y = y2;
  }
  
  result = (c1 | c2) == 0;
  
  if (0 == result) {
    pt1->x = 0;
    pt1->y = 0;
    pt2->x = 0;
    pt2->y = 0;
  }
  
  return result;
}

CC_INLINE int icvDrawLine(scanline* sl, IPOINT pt1, IPOINT pt2) {
  int w = sl->im->w, h = sl->im->h;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  
  if (ClipLine1(&clip, &pt1, &pt2)) {
    int dx = (pt2.x-pt1.x);
    int dy = (pt2.y-pt1.y);
    int absdx = abs(dx);
    int absdy = abs(dy);
    int i, count = MAX(absdx, absdy);
    double x = pt1.x;
    double y = pt1.y;
    double incx = (dx*1.) / count;
    double incy = (dy*1.) / count;
    
    for (i = 0; i < count; ++i, x += incx, y += incy) {
      int ix = (int)x, iy = (int)y;
      sl->addspan(sl, ix, ix+1, iy, 255);
      
    }
  }
  
  return 0;
}

CC_INLINE int icvDrawLine2(scanline* sl, IPOINT pt1, IPOINT pt2)
{
  int w = sl->im->w, h = sl->im->h;
  int dx, dy;
  int ecount;
  int ax, ay;
  int i, j;
  int x_step, y_step;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  int x = 2, y = 2;
  int tptr;

  sl->setsize(sl, clip, RCH(&clip));
  pt1.x -= XY_ONE * 2;
  pt1.y -= XY_ONE * 2;
  pt2.x -= XY_ONE * 2;
  pt2.y -= XY_ONE * 2;
  x = 2, y = 2;

  clip.l = ((clip.l - 5) << XY_SHIFT) + 1;
  clip.t = ((clip.t - 5) << XY_SHIFT) + 1;
  clip.r = ((clip.r - 5) << XY_SHIFT) + 1;
  clip.b = ((clip.b - 5) << XY_SHIFT) + 1;

  if (!ClipLine(&clip, &pt1, &pt2)) {
    return 0;
  }

  dx = pt2.x - pt1.x;
  dy = pt2.y - pt1.y;

  j = dx < 0 ? -1 : 0;
  ax = (dx ^ j) - j;
  i = dy < 0 ? -1 : 0;
  ay = (dy ^ i) - i;

  if (ax > ay) {
    dx = ax;
    dy = (dy ^ j) - j;
    pt1.x ^= pt2.x & j;
    pt2.x ^= pt1.x & j;
    pt1.x ^= pt2.x & j;
    pt1.y ^= pt2.y & j;
    pt2.y ^= pt1.y & j;
    pt1.y ^= pt2.y & j;

    x_step = XY_ONE;
    y_step = (int)(((int64) dy << XY_SHIFT) / (ax | 1));
    ecount = (pt2.x - pt1.x) >> XY_SHIFT;
  }
  else {
    dy = ay;
    dx = (dx ^ i) - i;
    pt1.x ^= pt2.x & i;
    pt2.x ^= pt1.x & i;
    pt1.x ^= pt2.x & i;
    pt1.y ^= pt2.y & i;
    pt2.y ^= pt1.y & i;
    pt1.y ^= pt2.y & i;

    x_step = (int)(((int64) dx << XY_SHIFT) / (ay | 1));
    y_step = XY_ONE;
    ecount = (pt2.y - pt1.y) >> XY_SHIFT;
  }

  pt1.x += (XY_ONE >> 1);
  pt1.y += (XY_ONE >> 1);
  
  ICC_PUT_POINT(y + ((pt2.y + (XY_ONE >> 1)) >> XY_SHIFT), x + ((pt2.x + (XY_ONE >> 1)) >> XY_SHIFT));
  
  if (ax > ay) {
    x += (pt1.x >> XY_SHIFT);
    
    while (ecount >= 0) {
      tptr = y + (pt1.y >> XY_SHIFT);
      ICC_PUT_POINT(tptr, x);
      pt1.y += y_step;
      ++x;
      ecount--;
    }
  }
  else {
    y += (pt1.y >> XY_SHIFT);
    
    while (ecount >= 0) {
      tptr = x + (pt1.x >> XY_SHIFT);
      ICC_PUT_POINT(y, tptr);
      pt1.x += x_step;
      ++y;
      ecount--;
    }
  }
  return 0;
}

CC_INLINE int icvDrawLineAA(scanline* sl, IPOINT pt1, IPOINT pt2)
{
  int w = sl->im->w, h = sl->im->h;
  int dx, dy;
  int ecount, scount = 0;
  int slope;
  int ax, ay;
  int x_step, y_step;
  int i, j, x, y;
  int ep_table[ 9 ];
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  
  static const uchar icvSlopeCorrTable[] = {
    181, 181, 181, 182, 182, 183, 184, 185, 187, 188, 190, 192, 194, 196, 198, 201,
    203, 206, 209, 211, 214, 218, 221, 224, 227, 231, 235, 238, 242, 246, 250, 254
  };

  static const int icvFilterTable[] = {  //
    168, 177, 185, 194, 202, 210, 218, 224, 231, 236, 241, 246, 249, 252, 254, 254,
    254, 254, 252, 249, 246, 241, 236, 231, 224, 218, 210, 202, 194, 185, 177, 168,
    158, 149, 140, 131, 122, 114, 105, 97, 89, 82, 75, 68, 62, 56, 50, 45,
    40, 36, 32, 28, 25, 22, 19, 16, 14, 12, 11, 9, 8, 7, 5, 5
  };

  sl->setsize(sl, clip, RCH(&clip));
  pt1.x -= XY_ONE * 2;
  pt1.y -= XY_ONE * 2;
  pt2.x -= XY_ONE * 2;
  pt2.y -= XY_ONE * 2;
  x = 2, y = 2;

  clip.l = ((clip.l + 5) << XY_SHIFT) + 1;
  clip.t = ((clip.t + 5) << XY_SHIFT) + 1;
  clip.r = ((clip.r - 5) << XY_SHIFT) + 1;
  clip.b = ((clip.b - 5) << XY_SHIFT) + 1;

  if (!ClipLine(&clip, &pt1, &pt2)) {
    return 0;
  }

  dx = pt2.x - pt1.x;
  dy = pt2.y - pt1.y;

  j = dx < 0 ? -1 : 0;
  ax = (dx ^ j) - j;
  i = dy < 0 ? -1 : 0;
  ay = (dy ^ i) - i;

  if (ax > ay) {
    dx = ax;
    dy = (dy ^ j) - j;
    pt1.x ^= pt2.x & j;
    pt2.x ^= pt1.x & j;
    pt1.x ^= pt2.x & j;
    pt1.y ^= pt2.y & j;
    pt2.y ^= pt1.y & j;
    pt1.y ^= pt2.y & j;

    x_step = XY_ONE;
    y_step = (int)(((int64) dy << XY_SHIFT) / (ax | 1));
    pt2.x += XY_ONE;
    ecount = (pt2.x >> XY_SHIFT) - (pt1.x >> XY_SHIFT);
    j = -(pt1.x & (XY_ONE - 1));
    pt1.y += (int)((((int64) y_step) * j) >> XY_SHIFT) + (XY_ONE >> 1);
    slope = (y_step >> (XY_SHIFT - 5)) & 0x3f;
    slope ^= (y_step < 0 ? 0x3f : 0);

    i = (pt1.x >> (XY_SHIFT - 7)) & 0x78;
    j = (pt2.x >> (XY_SHIFT - 7)) & 0x78;
  }
  else {
    dy = ay;
    dx = (dx ^ i) - i;
    pt1.x ^= pt2.x & i;
    pt2.x ^= pt1.x & i;
    pt1.x ^= pt2.x & i;
    pt1.y ^= pt2.y & i;
    pt2.y ^= pt1.y & i;
    pt1.y ^= pt2.y & i;

    x_step = (int)(((int64) dx << XY_SHIFT) / (ay | 1));
    y_step = XY_ONE;
    pt2.y += XY_ONE;
    ecount = (pt2.y >> XY_SHIFT) - (pt1.y >> XY_SHIFT);
    j = -(pt1.y & (XY_ONE - 1));
    pt1.x += (int)((((int64) x_step) * j) >> XY_SHIFT) + (XY_ONE >> 1);
    slope = (x_step >> (XY_SHIFT - 5)) & 0x3f;
    slope ^= (x_step < 0 ? 0x3f : 0);

    i = (pt1.y >> (XY_SHIFT - 7)) & 0x78;
    j = (pt2.y >> (XY_SHIFT - 7)) & 0x78;
  }

  slope = (slope & 0x20) ? 0x100 : icvSlopeCorrTable[ slope ];

  {
    int t0 = slope << 7;
    int t1 = ((0x78 - i) | 4) * slope;
    int t2 = (j | 4) * slope;

    ep_table[ 0 ] = 0;
    ep_table[ 8 ] = slope;
    ep_table[ 1 ] = ep_table[ 3 ] = ((((j - i) & 0x78) | 4) * slope >> 8) & 0x1ff;
    ep_table[ 2 ] = (t1 >> 8) & 0x1ff;
    ep_table[ 4 ] = ((((j - i) + 0x80) | 4) * slope >> 8) & 0x1ff;
    ep_table[ 5 ] = ((t1 + t0) >> 8) & 0x1ff;
    ep_table[ 6 ] = (t2 >> 8) & 0x1ff;
    ep_table[ 7 ] = ((t2 + t0) >> 8) & 0x1ff;
  }

  {
    if (ax > ay) {
      x += (pt1.x >> XY_SHIFT);

      while (ecount >= 0) {
        int tptr = y + ((pt1.y >> XY_SHIFT) - 1);

        int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 + (((ecount >= 2) + 1) & (ecount | 2)) ];
        int a, dist = (pt1.y >> (XY_SHIFT - 5)) & 31;

        a = (ep_corr * icvFilterTable[ dist + 32 ] >> 8) & 0xff;
        ICC_PUT_POINT_AA(tptr, x, a);

        ++tptr;
        a = (ep_corr * icvFilterTable[ dist ] >> 8) & 0xff;
        ICC_PUT_POINT_AA(tptr, x, a);

        ++tptr;
        a = (ep_corr * icvFilterTable[ 63 - dist ] >> 8) & 0xff;
        ICC_PUT_POINT_AA(tptr, x, a);

        pt1.y += y_step;
        ++x;
        scount++;
        ecount--;
      }
    }
    else {
      y += (pt1.y >> XY_SHIFT);

      while (ecount >= 0) {
        int tptr = x + ((pt1.x >> XY_SHIFT) - 1);

        int ep_corr = ep_table[(((scount >= 2) + 1) & (scount | 2)) * 3 + (((ecount >= 2) + 1) & (ecount | 2)) ];
        int a, dist = (pt1.x >> (XY_SHIFT - 5)) & 31;

        a = (ep_corr * icvFilterTable[ dist + 32 ] >> 8) & 0xff;
        ICC_PUT_POINT_AA(y, tptr, a);

        ++tptr;
        a = (ep_corr * icvFilterTable[ dist ] >> 8) & 0xff;
        ICC_PUT_POINT_AA(y, tptr, a);

        ++tptr;
        a = (ep_corr * icvFilterTable[ 63 - dist ] >> 8) & 0xff;
        ICC_PUT_POINT_AA(y, tptr, a);

        pt1.x += x_step;
        ++y;
        scount++;
        ecount--;
      }
    }
  }
  return 0;
}


CC_INLINE int icvFillConvexPoly(scanline* sl, const IPOINT* v, int npts, int line_type, int shift)
{
  int w = sl->im->w, h = sl->im->h;
  struct {
    int idx, di;
    int x, dx, ye;
  }
  edge[ 2 ];

  int delta = shift ? 1 << (shift - 1) : 0;
  int i, y, imin = 0, left = 0, right = 1, x1, x2;
  int edges = npts;
  int xmin, xmax, ymin, ymax;
  int size_width = w, size_height = h;
  IPOINT p0;
  int delta1, delta2;

  if (line_type < CC_AA) {
    delta1 = delta2 = XY_ONE >> 1;
  }
  else {
    delta1 = XY_ONE - 1, delta2 = 0;
  }

  p0 = v[ npts - 1 ];
  p0.x <<= XY_SHIFT - shift;
  p0.y <<= XY_SHIFT - shift;

  assert(0 <= shift && shift <= XY_SHIFT);
  xmin = xmax = v[ 0 ].x;
  ymin = ymax = v[ 0 ].y;

  for (i = 0; i < npts; i++) {
    IPOINT p = v[ i ];

    if (p.y < ymin) {
      ymin = p.y;
      imin = i;
    }

    ymax = MAX(ymax, p.y);
    xmax = MAX(xmax, p.x);
    xmin = MIN(xmin, p.x);

    p.x <<= XY_SHIFT - shift;
    p.y <<= XY_SHIFT - shift;

    if (line_type <= 8) {
      icvDrawLine2(sl, p0, p);
    }
    else {
      icvDrawLineAA(sl, p0, p);
    }

    p0 = p;
  }

  xmin = (xmin + delta) >> shift;
  xmax = (xmax + delta) >> shift;
  ymin = (ymin + delta) >> shift;
  ymax = (ymax + delta) >> shift;

  if (npts < 3 || xmax < 0 || ymax < 0 || xmin >= size_width || ymin >= size_height) {
    return 0;
  }

  ymax = MIN(ymax, size_height - 1);
  edge[ 0 ].idx = edge[ 1 ].idx = imin;

  edge[ 0 ].ye = edge[ 1 ].ye = y = ymin;
  edge[ 0 ].di = 1;
  edge[ 1 ].di = npts - 1;

  //img += step * y;

  do {
    if (line_type < CC_AA || y < ymax || y == ymin) {
      for (i = 0; i < 2; i++) {
        if (y >= edge[ i ].ye) {
          int idx = edge[ i ].idx, di = edge[ i ].di;
          int xs = 0, xe, ye, ty = 0;

          for (;;) {
            ty = (v[ idx ].y + delta) >> shift;

            if (ty > y || edges == 0) {
              break;
            }

            xs = v[ idx ].x;
            idx += di;
            idx -= ((idx < npts) - 1) & npts;
            edges--;
          }

          ye = ty;
          xs <<= XY_SHIFT - shift;
          xe = v[ idx ].x << (XY_SHIFT - shift);

          if (y >= ye) {
            return 0;
          }

          edge[ i ].ye = ye;
          edge[ i ].dx = ((xe - xs) * 2 + (ye - y)) / (2 * (ye - y));
          edge[ i ].x = xs;
          edge[ i ].idx = idx;
        }
      }
    }

    if (edge[ left ].x > edge[ right ].x) {
      left ^= 1;
      right ^= 1;
    }

    x1 = edge[ left ].x;
    x2 = edge[ right ].x;

    if (y >= 0) {
      int xx1 = (x1 + delta1) >> XY_SHIFT;
      int xx2 = (x2 + delta2) >> XY_SHIFT;

      if (xx2 >= 0 && xx1 < size_width) {
        if (xx1 < 0) {
          xx1 = 0;
        }

        if (xx2 >= size_width) {
          xx2 = size_width - 1;
        }

        BIN_HLINE(y, xx1, xx2);
      }
    }

    x1 += edge[ left ].dx;
    x2 += edge[ right ].dx;

    edge[ left ].x = x1;
    edge[ right ].x = x2;
    ++y;
  }
  while (++y <= ymax);

  return 0;
}

CC_INLINE int FillConvexPoly(scanline* sl, IPOINT* v, int npts)
{
  int line_type = 0; // CC_AA
  icvFillConvexPoly(sl, v, npts, line_type, 0);
  return 0;
}

CC_INLINE int icvCollectPolyEdges(scanline* sl, const IPOINT* v, int v_total, PolyEdge* edges, int line_type, int shift, IPOINT offset)
{
  int w = sl->im->w, h = sl->im->h;
  int i, count = v_total, cnt = 0;
  int delta = offset.y + (shift ? 1 << (shift - 1) : 0);
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  int elem_type = 1;

  for (i = 0; i < count; i++) {
    IPOINT pt0, pt1, t0, t1;
    PolyEdge edge;
    // CC_READ_EDGE( pt0, pt1, v );
    pt0 = v[ i ], pt1 = v[(i + 1) % count ];

    if (elem_type) {
      pt0.x = (pt0.x + offset.x) << (XY_SHIFT - shift);
      pt0.y = (pt0.y + delta) >> shift;
      pt1.x = (pt1.x + offset.x) << (XY_SHIFT - shift);
      pt1.y = (pt1.y + delta) >> shift;
    }
    else {
      suf32_t x, y;
      assert(shift == 0);

      x.i = pt0.x;
      y.i = pt0.y;
      pt0.x = ROUND((x.f + offset.x) * XY_ONE);
      pt0.y = ROUND(y.f + offset.y);
      x.i = pt1.x;
      y.i = pt1.y;
      pt1.x = ROUND((x.f + offset.x) * XY_ONE);
      pt1.y = ROUND(y.f + offset.y);
    }

    if (line_type < CC_AA) {
      t0.y = pt0.y;
      t1.y = pt1.y;
      t0.x = (pt0.x + (XY_ONE >> 1)) >> XY_SHIFT;
      t1.x = (pt1.x + (XY_ONE >> 1)) >> XY_SHIFT;
      icvDrawLine(sl, t0, t1);
    }
    else {
      t0.x = pt0.x;
      t1.x = pt1.x;
      t0.y = pt0.y << XY_SHIFT;
      t1.y = pt1.y << XY_SHIFT;
      icvDrawLineAA(sl, t0, t1);
    }

    if (pt0.y == pt1.y) {
      continue;
    }

    if (pt0.y > pt1.y) {
      CC_SWAP(pt0, pt1, t0);
    }

    clip.t = MIN(clip.t, pt0.y);
    clip.b = MAX(clip.b, pt1.y);

    if (pt0.x < pt1.x) {
      clip.l = MIN(clip.l, pt0.x);
      clip.r = MAX(clip.r, pt1.x);
    }
    else {
      clip.l = MIN(clip.l, pt1.x);
      clip.r = MAX(clip.r, pt0.x);
    }

    edge.y0 = pt0.y;
    edge.y1 = pt1.y;
    edge.x = pt0.x;
    edge.dx = (pt1.x - pt0.x) / (pt1.y - pt0.y);
    assert(edge.y0 < edge.y1);

    // CC_WRITE_SEQ_ELEM( edge, writer );
    edges[ cnt++ ] = edge;
  }

  //*rect = clip;
  //cvEndWriteSeq( &writer );
  return cnt;
}

static int iCmpEdges(const int* _e1, const int* _e2, int* userdata)
{
  PolyEdge* e1 = (PolyEdge*) _e1, *e2 = (PolyEdge*) _e2;
  return e1->y0 - e2->y0 ? e1->y0 - e2->y0 :
      e1->x - e2->x ? e1->x - e2->x : e1->dx - e2->dx;
}

CC_INLINE int scanline_dpoints(scanline* sl, const DPOINT* pts, int n)
{
  int i;
  int w = sl->im->w, h = sl->im->h;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  sl->setsize(sl, clip, RCH(&clip));
  
  for (i = 0; i < n; ++i) {
    int x = (int)pts[i].x;
    int y = (int)pts[i].y;
    
    if (x >= clip.l && x < clip.r && y >= clip.t && y < clip.b) {
      sl->addspan(sl, x, x+1, y, 255);
    }
  }
  
  return 0;
}

/* helper macros and functions for sequence/contour processing */
CC_INLINE int icvFillEdgeCollection(scanline* sl, PolyEdge* edges, int total)
{
  int w = sl->im->w, h = sl->im->h;
  PolyEdge tmp;
  int i, y;
  PolyEdge* e;
  int y_max = MAX_INT;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  IRECT* pclip = &clip;

  memset(&tmp, 0, sizeof(tmp));

  /* check parameters */
  if (total < 2 || RCH(pclip) < 0 || pclip->t >= h || RCW(pclip) < 0 || pclip->l >= w) {
    return 0;
  }

#define CMPEDGES(E1, E2) ((E1).y0 - (E2).y0 ? (E1).y0 - (E2).y0 : (E1).x - (E2).x ? (E1).x - (E2).x : (E1).dx - (E2).dx)

#define CMPEDGES_LESS(e1, e2) (CMPEDGES(e1, e2)< 0)
  QSORT(edges, edges + total, CMPEDGES_LESS, PolyEdge);
  //cvStartReadSeq( (CvSeq*)edges, &reader );
#undef CMPEDGES_LESS
#ifdef _DEBUG

  e = &tmp;
  tmp.y0 = INT_MIN;
#endif

  for (i = 0; i < total; i++) {
    PolyEdge* e1 = edges + i;

#ifdef _DEBUG

    assert(e1->y0 < e1->y1 && (i == 0 || CMPEDGES(*e, *e1) <= 0));
    e = e1;
#endif

    y_max = MAX(y_max, e1->y1);
  }

  /* start drawing */
  tmp.y0 = INT_MAX;
  // cvSeqPush( (CvSeq*)edges, &tmp );
  edges[ total++ ] = tmp;

  i = 0;
  tmp.next = 0;
  // cvStartReadSeq( (CvSeq*)edges, &reader );
  e = edges;
  y_max = MIN(y_max, h);

  for (y = e->y0; y < y_max; y++) {
    PolyEdge* last, *prelast, *keep_prelast;
    int sort_flag = 0;
    int draw = 0;
    int clipline = y < 0;

    prelast = &tmp;
    last = tmp.next;

    while (last || e->y0 == y) {
      if (last && last->y1 == y) {
        /* exlude edge if y reachs its lower point */
        prelast->next = last->next;
        last = last->next;
        continue;
      }

      keep_prelast = prelast;

      if (last && (e->y0 > y || last->x < e->x)) {
        /* go to the next edge in active list */
        prelast = last;
        last = last->next;
      }
      else if (i < total) {
        /* insert new edge into active list if y reachs its upper point */
        prelast->next = e;
        e->next = last;
        prelast = e;
        // CC_NEXT_SEQ_ELEM( edges->elem_size, reader );
        //e = (PolyEdge*)(reader.img);
        e++;
        i++;
      }
      else {
        break;
      }

      if (draw) {
        if (!clipline) {
          /* convert x's from fixed-point to image coordinates */
          int x1 = keep_prelast->x;
          int x2 = prelast->x;

          if (x1 > x2) {
            int t = x1;

            x1 = x2;
            x2 = t;
          }

          x1 = (x1 + XY_ONE - 1) >> XY_SHIFT;
          x2 = x2 >> XY_SHIFT;

          /* clip and draw the line */
          if (x1 < w && x2 >= 0) {
            if (x1 < 0) {
              x1 = 0;
            }

            if (x2 >= w) {
              x2 = w - 1;
            }

            //BIN_HLINE(timg, x1, x2, cn);
            sl->addspan(sl, x1, x2+1, y, 255);
          }
        }

        keep_prelast->x += keep_prelast->dx;
        prelast->x += prelast->dx;
      }

      draw ^= 1;
    }

    /* sort edges (bubble sort on list) */
    keep_prelast = 0;

    do {
      prelast = &tmp;
      last = tmp.next;

      while (last != keep_prelast && last->next != 0) {
        PolyEdge* te = last->next;

        /* swap edges */
        if (last->x > te->x) {
          prelast->next = te;
          last->next = te->next;
          te->next = last;
          prelast = te;
          sort_flag = 1;
        }
        else {
          prelast = last;
          last = te;
        }
      }

      keep_prelast = prelast;
    }
    while (sort_flag && keep_prelast != tmp.next && keep_prelast != &tmp);
  }

  return 0;
}

CC_INLINE int Ellipse2Poly(double cx, double cy, int axes_height, int axes_width, int angle, int arc_start, int arc_end, IPOINT* pts, int delta)
{
  float alpha, beta;
  double size_a = axes_width, size_b = axes_height;
  IPOINT* pts_origin = pts;
  int i;

  while (angle < 0) {
    angle += 360;
  }

  while (angle > 360) {
    angle -= 360;
  }

  if (arc_start > arc_end) {
    i = arc_start;
    arc_start = arc_end;
    arc_end = i;
  }

  while (arc_start < 0) {
    arc_start += 360;
    arc_end += 360;
  }

  while (arc_end > 360) {
    arc_end -= 360;
    arc_start -= 360;
  }

  if (arc_end - arc_start > 360) {
    arc_start = 0;
    arc_end = 360;
  }

  iSinCos(angle, &alpha, &beta);

  for (i = arc_start; i < arc_end + delta; i += delta) {
    double x, y;
    angle = i;

    if (angle > arc_end) {
      angle = arc_end;
    }

    if (angle < 0) {
      angle += 360;
    }

    x = size_a * table_fsin[ 450 - angle ];
    y = size_b * table_fsin[ angle ];
    pts->x = ROUND(cx + x * alpha - y * beta);
    pts->y = ROUND(cy - x * beta - y * alpha);
    pts += i == arc_start || pts->x != pts[ -1 ].x || pts->y != pts[ -1 ].y;
  }

  i = (int)(pts - pts_origin);

  for (; i < 2; i++) {
    pts_origin[ i ] = pts_origin[ i - 1 ];
  }

  return i;
}

#define NJINZHI(_N, _F, n)                                                    \
  do {                                                                          \
    int _I;                                                                     \
    for (_I = 0; _I < _N; ++_I) {                                               \
      ++(*(_F + _I));                                                           \
      if (!(*(_F + _I)/n)) {                                                    \
        break;                                                                  \
      }                                                                         \
      *(_F + _I) = (*(_F + _I))%n;                                              \
    }                                                                           \
  } while(0)

CC_INLINE int icvEllipseEx(scanline* sl, int cx, int cy, int axes_height, int axes_width, int angle, int arc_start, int arc_end, int wline, int line_type, int shift);

CC_INLINE int icvDrawCircle(scanline* sl, int cx, int cy, int radius, int fill)
{
  int x, y, inside;
  int err = 0, dx = radius, dy = 0, plus = 1, minus = (radius << 1) - 1;
  int w = sl->im->w, h = sl->im->h;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  x = cx;
  y = cy;
  inside = x >= clip.l + radius && x < clip.r - radius && y >= clip.t + radius && y < clip.b - radius;
  while (dx >= dy) {
    int mask;
    int y11 = y - dy, y12 = y + dy, y21 = y - dx, y22 = y + dx;
    int x11 = x - dx, x12 = x + dx, x21 = x - dy, x22 = x + dy;

    if (inside) {

      if (!fill) {
        sl->addspan(sl, x11, x11+1, y11, 255);
        sl->addspan(sl, x11, x11+1, y12, 255);
        sl->addspan(sl, x12, x12+1, y11, 255);
        sl->addspan(sl, x12, x12+1, y12, 255);
      }
      else {
        sl->addspan(sl, x11, x12+1, y11, 255);
        sl->addspan(sl, x11, x12+1, y12, 255);
      }

      if (!fill) {
        sl->addspan(sl, x21, x21+1, y21, 255);
        sl->addspan(sl, x21, x21+1, y22, 255);
        sl->addspan(sl, x22, x22+1, y21, 255);
        sl->addspan(sl, x22, x22+1, y22, 255);
      }
      else {
        sl->addspan(sl, x21, x22+1, y21, 255);
        sl->addspan(sl, x21, x22+1, y22, 255);
      }
    }
    else if (x11 < clip.r && x12 >= clip.l && y21 < clip.b && y22 >= clip.t) {
      if (fill) {
        x11 = MAX(x11, clip.l);
        x12 = MIN(x12, clip.r - 1);
      }

      if ((unsigned) y11 < (unsigned) h) {
        if (!fill) {
          if (x11 >= clip.l) {
            sl->addspan(sl, x11, x11+1, y11, 255);
          }

          if (x12 < clip.r) {
            sl->addspan(sl, x12, x12+1, y11, 255);
          }
        }
        else {
          sl->addspan(sl, x11, x12+1, y11, 255);
        }
      }

      if ((unsigned) y12 < (unsigned) h) {
        if (!fill) {
          if (x11 >= clip.l) {
            sl->addspan(sl, x11, x11+1, y12, 255);
          }

          if (x12 < clip.r) {
            sl->addspan(sl, x12, x12+1, y12, 255);
          }
        }
        else {
          sl->addspan(sl, x11, x12+1, y12, 255);
        }
      }

      if (x21 < clip.r && x22 >= clip.l) {
        if (fill) {
          x21 = MAX(x21, clip.l);
          x22 = MIN(x22, clip.r - 1);
        }

        if (y21 < clip.b && y21 >= clip.t) {

          if (!fill) {
            if (x21 >= clip.l) {
              sl->addspan(sl, x21, x21+1, y21, 255);
            }

            if (x22 < clip.r) {
              sl->addspan(sl, x22, x22+1, y21, 255);
            }
          }
          else {
            sl->addspan(sl, x21, x22+1, y21, 255);
          }
        }

        if (y22 < clip.b && y22 >= clip.t) {

          if (!fill) {
            if (x21 >= clip.l) {
            sl->addspan(sl, x21, x21+1, y22, 255);
            }

            if (x22 < clip.r) {
            sl->addspan(sl, x22, x22+1, y22, 255);
            }
          }
          else {
            sl->addspan(sl, x21, x22+1, y22, 255);
          }
        }
      }
    }

    dy++;
    err += plus;
    plus += 2;

    mask = (err <= 0) - 1;

    err -= minus & mask;
    dx += mask;
    minus -= mask & 2;
  }

  return 0;
}

// flags 是否 round cap
CC_INLINE int ThickLine(scanline* sl, IPOINT p0, IPOINT p1, int wline, int line_type, int flags, int shift)
{
  static const double INV_XY_ONE = 1. / XY_ONE;

  p0.x <<= XY_SHIFT - shift;
  p0.y <<= XY_SHIFT - shift;
  p1.x <<= XY_SHIFT - shift;
  p1.y <<= XY_SHIFT - shift;

  if (wline <= 1) {
    if (line_type < CC_AA) {
      if (line_type == 1 || line_type == 4 || shift == 0) {
        p0.x = (p0.x + (XY_ONE >> 1)) >> XY_SHIFT;
        p0.y = (p0.y + (XY_ONE >> 1)) >> XY_SHIFT;
        p1.x = (p1.x + (XY_ONE >> 1)) >> XY_SHIFT;
        p1.y = (p1.y + (XY_ONE >> 1)) >> XY_SHIFT;
        icvDrawLine(sl, p0, p1);
      }
      else {
        icvDrawLine2(sl, p0, p1);
      }
    }
    else {
      icvDrawLineAA(sl, p0, p1);
    }
  }
  else {
    IPOINT pt[ 4 ];
    IPOINT dp = {0, 0};
    double dx = (p0.x - p1.x) * INV_XY_ONE, dy = (p1.y - p0.y) * INV_XY_ONE;
    double r = dx * dx + dy * dy;
    int i;
    wline <<= XY_SHIFT - 1;

    if (fabs(r) > DBL_EPSILON) {     // DBL_EPSILON defined in "float.h"
      r = wline * ((float)(1. / sqrt(r)));
      dp.x = ROUND(dy * r);
      dp.y = ROUND(dx * r);
    }

    pt[ 0 ].x = p0.x + dp.x;
    pt[ 0 ].y = p0.y + dp.y;
    pt[ 1 ].x = p0.x - dp.x;
    pt[ 1 ].y = p0.y - dp.y;
    pt[ 2 ].x = p1.x - dp.x;
    pt[ 2 ].y = p1.y - dp.y;
    pt[ 3 ].x = p1.x + dp.x;
    pt[ 3 ].y = p1.y + dp.y;

    icvFillConvexPoly(sl, pt, 4, line_type, XY_SHIFT);

    for (i = 0; i < 2; i++) {
      if (flags & (i + 1)) {
        if (line_type < CC_AA) {
          int cx, cy;
          cx = (p0.x + (XY_ONE >> 1)) >> XY_SHIFT;
          cy = (p0.y + (XY_ONE >> 1)) >> XY_SHIFT;
          icvDrawCircle(sl, cx, cy, wline >> XY_SHIFT, 1);
        }
        else {
          icvEllipseEx(sl, p0.x, p0.y, wline, wline, 0, 0, 360, -1, line_type, shift);
        }
      }

      p0 = p1;
    }
  }

  return 0;
}

CC_INLINE int icvPolyLines(scanline* sl, const IPOINT* v, int count, int is_closed, int wline, int shift)
{
  if (count > 0) {
    int line_type = 0;
    int i = is_closed ? count - 1 : 0;
    int flags = 2 + !is_closed;
    IPOINT p0;
    assert(wline >= 0);
    assert(v && count >= 0);
    line_type = CC_AA;
    line_type = 0;

    p0 = v[ i ];

    for (i = !is_closed; i < count; i++) {
      ThickLine(sl, p0, v[ i ], wline, line_type, flags, shift);
      p0 = v[ i ];
      flags = 2;
    }
  }

  return 0;
}

CC_INLINE int icvEllipseEx(scanline* sl, int cx, int cy, int axes_height, int axes_width, int angle, int arc_start, int arc_end, int wline, int line_type, int shift)
{
  int w = sl->im->w, h = sl->im->h;
  IPOINT pts[ 1 << 8 ];
  int count, delta;

  if (axes_width < 0 || axes_height < 0) {
    return 0;
  }

  delta = (MAX(axes_width, axes_height) + (XY_ONE >> 1)) >> XY_SHIFT;
  delta = delta < 3 ? 90 : delta < 10 ? 30 : delta < 15 ? 18 : 5;

  count = Ellipse2Poly(cx, cy, axes_height, axes_width, angle, arc_start, arc_end, pts, delta);

  if (wline >= 0) {
    icvPolyLines(sl, pts, count, 0, wline, shift);
  }
  else if (arc_end - arc_start >= 360) {
    icvFillConvexPoly(sl, pts, count, line_type, XY_SHIFT);
  }
  else {
    IPOINT off = {0, 0};
    PolyEdge edges[1 << 12];
    int len = icvCollectPolyEdges(sl, pts, count, edges, line_type, XY_SHIFT, off);
    icvFillEdgeCollection(sl, edges, len);
  }

  return 0;
}

CC_INLINE int scanline_bin_line(scanline* sl, IPOINT pt1, IPOINT pt2, int wline)
{
  int line_type = 0;

  if ((unsigned) wline > 255) {
    return 0;
  }

  ThickLine(sl, pt1, pt2, wline, line_type, 3, 0);
  return 0;
}
CC_INLINE int scanline_bin_line2(scanline* sl, int x1, int y1, int x2, int y2, int wline)
{
  IPOINT pt1 = iPOINT(x1, y1);
  IPOINT pt2 = iPOINT(x2, y2);
  return scanline_bin_line(sl, pt1, pt2, wline);
}

// imdrow
#define DrawRectangle2(sl, rt, lw)    scanline_bin_rect_i(sl, 0, (rt).x, (rt).y, (rt).x+(rt).w, (rt).y+(rt).h, 0, lw)
#define DrawRectangle3(sl, rt, lw)    scanline_bin_rect_i(sl, 0, (rt).left, (rt).top, (rt).right, (rt).bottom, lw)
#define DrawRectangles(sl, rts, rtsn, lw)   {int _i=0;for (;_i<(rtsn); ++_i) scanline_bin_rect_i(sl, 0, (rts)[_i].x, (rts)[_i].y, (rts)[_i].x+(rts)[_i].w, (rts)[_i].y+(rts)[_i].h, lw, 8, 0);}

CC_INLINE int scanline_bin_circle1(scanline* sl, int cx, int cy, int radius, int wline)
{
  int shift = 0;
  int line_type = 0;
  
  if (radius < 0) {
    return 0;
  }
  
  if (wline < -1 || wline > 255) {
    return 0;
  }
  
  if (wline > 1 || line_type >= CC_AA) {
    int axes_height, axes_width;
    cx <<= XY_SHIFT - shift;
    cy <<= XY_SHIFT - shift;
    radius <<= XY_SHIFT - shift;
    axes_height = axes_width = radius;
    icvEllipseEx(sl, cx, cy, axes_height, axes_width, 0, 0, 360, wline, line_type, XY_SHIFT - shift);
  }
  else {
    icvDrawCircle(sl, cx, cy, radius, wline < 0);
  }
  
  return 0;
}

CC_INLINE int scanline_bin_ellipse1(scanline* sl, int cx, int cy, int axes_width, int axes_height, double angle, double start_angle, double end_angle, int wline)
{
  int shift = 0;
  int _angle = ROUND(angle);
  int _start_angle = ROUND(start_angle);
  int _end_angle = ROUND(end_angle);
  cx <<= XY_SHIFT - shift;
  cy <<= XY_SHIFT - shift;
  axes_width <<= XY_SHIFT - shift;
  axes_height <<= XY_SHIFT - shift;
  
  if (axes_width < 0 || axes_height < 0) {
    return 0;
  }
  
  if (wline < -1 || wline > 255) {
    return 0;
  }
  
  icvEllipseEx(sl, cx, cy, axes_height, axes_width, _angle, _start_angle, _end_angle, wline, CC_AA, XY_SHIFT);
  return 0;
}

CC_INLINE int scanline_bin_fillpoly(scanline* sl, const IPOINT* pts, int* npts, int contours, int shift)
{
  int w = sl->im->w, h = sl->im->h;
  int i;
  PolyEdge* edges = 0;
  int len = 0, cnt = 0;
  IPOINT off = {0, 0};
  int line_type = 0;
  
  if (contours <= 0) {
    CC_ERROR(CC_StsBadArg, "CC_StsBadArg");
  }
  
  if (!pts) {
    CC_ERROR(CC_StsNullPtr, "CC_StsNullPtr");
  }
  
  for (i = 0; i < contours; i++) {
    if (npts[ i ] < 0) {
      CC_ERROR(CC_StsOutOfRange, "CC_StsOutOfRange");
    }
    
    len += npts[ i ];
  }
  
  edges = MALLOC(PolyEdge, len * 2);
  
  for (i = 0; i < contours; i++) {
    cnt += icvCollectPolyEdges(sl, pts, npts[ i ], edges + cnt, line_type, shift, off);
    pts += npts[i];
  }
  
  icvFillEdgeCollection(sl, edges, cnt);
  FREE(edges);
  return 0;
}

CC_INLINE int scanline_bin_polyline(scanline* sl, const IPOINT* pts, const int* npts, int contours, int wline)
{
  int i;
  
  for (i = 0; i < contours; i++) {
    icvPolyLines(sl, pts, npts[ i ], 1, wline, 0);
    pts += npts[ i ];
  }
  
  return 0;
}

CC_INLINE int scanline_bin_polyline(scanline* sl, const IPOINT* pts, const int* npts, int contours, COLOR crLine, int wline)
{
  int i;
  
  for (i = 0; i < contours; i++) {
    icvPolyLines(sl, pts, npts[ i ], 1, wline, 0);
    pts += npts[ i ];
  }
  
  return 0;
}
CC_INLINE int scanline_bin_poly(scanline* sl, const IPOINT* pt, int n, int closed, COLOR crFill, COLOR crLine, int wline)
{
  if (crFill) {
    scanline_bin_fillpoly(sl, pt, &n, 1, 0);
  }
  
  if (crLine) {
    scanline_bin_polyline(sl, pt, &n, 1, wline);
  }
  
  return 0;
}

CC_INLINE int scanline_bin_ksqx(scanline* sl, IPOINT p1, IPOINT p2, float m_limit, int wline)
{
  int w = sl->im->w, h = sl->im->h;
  float dx, dy, ds; //2点间距离
  float dx1, dy1, ds1;
  float tempdx1, tempdy1;
  float tempdx2, tempdy2;
  float tempdx3, tempdy3;
  float middx, middy;
  IPOINT t1, t2, t3, midp;
  
  dx = (float)(p2.x - p1.x);
  dy = (float)(p2.y - p1.y);
  ds = (float) sqrt(dx * dx + dy * dy);
  
  if (ds <= m_limit) {
    return scanline_bin_line(sl, p1, p2, wline);
  }
  
  tempdx1 = (ds / 3) * (dx / ds);
  tempdy1 = (ds / 3) * (dy / ds);
  tempdx2 = (ds * 2 / 3) * (dx / ds);
  tempdy2 = (ds * 2 / 3) * (dy / ds);
  
  t1.x = (int)(p1.x + tempdx1);
  t1.y = (int)(p1.y + tempdy1);
  t2.x = (int)(p1.x + tempdx2);
  t2.y = (int)(p1.y + tempdy2);
  
  middx = ds * (float) 0.5 * (float)(dx / ds);
  middy = ds * (float) 0.5 * (float)(dy / ds);
  
  midp.x = (int)(p1.x + middx);
  midp.y = (int)(p1.y + middy);
  
  dx1 = -dy;
  dy1 = dx;
  ds1 = (float) sqrt(dx1 * dx1 + dy1 * dy1);      //or ds1 = ds
  tempdx3 = (ds * (float)(sqrt(3.0) / 6)) * (dx1 / ds1);
  tempdy3 = (ds * (float)(sqrt(3.0) / 6)) * (dy1 / ds1);
  t3.x = (int)(midp.x + tempdx3);
  t3.y = (int)(midp.y + tempdy3);
  
  scanline_bin_ksqx(sl, p1, t1, m_limit, wline);
  scanline_bin_ksqx(sl, t1, t3, m_limit, wline);
  scanline_bin_ksqx(sl, t3, t2, m_limit, wline);
  scanline_bin_ksqx(sl, t2, p2, m_limit, wline);
  return 0;
}

#define CC_ADJUST_EDGE_COUNT( count, seq )   ((count) -= ((count) == (seq)->total && !CC_IS_SEQ_CLOSED(seq)))

CC_INLINE int scanline_bin_ploy(scanline* sl, int n, const double* xishu, double cx, double cy, int wline)
{
  int w = sl->im->w, h = sl->im->h;
  enum {ptnum = 20};
  IPOINT pt[ptnum];
  int i, j;
  int shift = 10;
  double y1, x;
  
  for (j = 0; j < ptnum; ++j) {
    x = j * (w - 1) / (ptnum - 1);
    
    for (y1 = 0, i = 0; i < n; ++i) {
      y1 = y1 * (x - cx) + xishu[i];
    }
    
    y1 += cy;
    pt[j].x = (int)(x * (1 << shift));
    pt[j].y = (int)(y1 * (1 << shift));
  }
  
  icvPolyLines(sl, pt, ptnum, 0, wline, shift);
  return 0;
}


#ifndef WIN32
#define COLOR_SCROLLBAR         0
#define COLOR_BACKGROUND        1
#define COLOR_ACTIVECAPTION     2
#define COLOR_INACTIVECAPTION   3
#define COLOR_MENU              4
#define COLOR_WINDOW            5
#define COLOR_WINDOWFRAME       6
#define COLOR_MENUTEXT          7
#define COLOR_WINDOWTEXT        8
#define COLOR_CAPTIONTEXT       9
#define COLOR_ACTIVEBORDER      10
#define COLOR_INACTIVEBORDER    11
#define COLOR_APPWORKSPACE      12
#define COLOR_HIGHLIGHT         13
#define COLOR_HIGHLIGHTTEXT     14
#define COLOR_BTNFACE           15
#define COLOR_BTNSHADOW         16
#define COLOR_GRAYTEXT          17
#define COLOR_BTNTEXT           18
#define COLOR_INACTIVECAPTIONTEXT 19
#define COLOR_BTNHIGHLIGHT      20

#define COLOR_3DDKSHADOW        21
#define COLOR_3DLIGHT           22
#define COLOR_INFOTEXT          23
#define COLOR_INFOBK            24

#define COLOR_HOTLIGHT          26
#define COLOR_GRADIENTACTIVECAPTION 27
#define COLOR_GRADIENTINACTIVECAPTION 28

#define COLOR_MENUHILIGHT       29
#define COLOR_MENUBAR           30


#define COLOR_DESKTOP           COLOR_BACKGROUND
#define COLOR_3DFACE            COLOR_BTNFACE
#define COLOR_3DSHADOW          COLOR_BTNSHADOW
#define COLOR_3DHIGHLIGHT       COLOR_BTNHIGHLIGHT
#define COLOR_3DHILIGHT         COLOR_BTNHIGHLIGHT
#define COLOR_BTNHILIGHT        COLOR_BTNHIGHLIGHT


/* WM_PRINT flags */
#define PRF_CHECKVISIBLE    0x00000001L
#define PRF_NONCLIENT       0x00000002L
#define PRF_CLIENT          0x00000004L
#define PRF_ERASEBKGND      0x00000008L
#define PRF_CHILDREN        0x00000010L
#define PRF_OWNED           0x00000020L

/* 3D border styles */
#define BDR_RAISEDOUTER 0x0001
#define BDR_SUNKENOUTER 0x0002
#define BDR_RAISEDINNER 0x0004
#define BDR_SUNKENINNER 0x0008


#define BDR_OUTER       (BDR_RAISEDOUTER | BDR_SUNKENOUTER)
#define BDR_INNER       (BDR_RAISEDINNER | BDR_SUNKENINNER)
#define BDR_RAISED      (BDR_RAISEDOUTER | BDR_RAISEDINNER)
#define BDR_SUNKEN      (BDR_SUNKENOUTER | BDR_SUNKENINNER)


#define EDGE_RAISED     (BDR_RAISEDOUTER | BDR_RAISEDINNER)
#define EDGE_SUNKEN     (BDR_SUNKENOUTER | BDR_SUNKENINNER)
#define EDGE_ETCHED     (BDR_SUNKENOUTER | BDR_RAISEDINNER)
#define EDGE_BUMP       (BDR_RAISEDOUTER | BDR_SUNKENINNER)


/* Border flags */
#define BF_LEFT         0x0001
#define BF_TOP          0x0002
#define BF_RIGHT        0x0004
#define BF_BOTTOM       0x0008

#define BF_TOPLEFT      (BF_TOP | BF_LEFT)
#define BF_TOPRIGHT     (BF_TOP | BF_RIGHT)
#define BF_BOTTOMLEFT   (BF_BOTTOM | BF_LEFT)
#define BF_BOTTOMRIGHT  (BF_BOTTOM | BF_RIGHT)
#define BF_RECT         (BF_LEFT | BF_TOP | BF_RIGHT | BF_BOTTOM)

#define BF_DIAGONAL     0x0010

// For diagonal lines, the BF_RECT flags specify the end point of the
// vector bounded by the rectangle parameter.
#define BF_DIAGONAL_ENDTOPRIGHT     (BF_DIAGONAL | BF_TOP | BF_RIGHT)
#define BF_DIAGONAL_ENDTOPLEFT      (BF_DIAGONAL | BF_TOP | BF_LEFT)
#define BF_DIAGONAL_ENDBOTTOMLEFT   (BF_DIAGONAL | BF_BOTTOM | BF_LEFT)
#define BF_DIAGONAL_ENDBOTTOMRIGHT  (BF_DIAGONAL | BF_BOTTOM | BF_RIGHT)


#define BF_MIDDLE       0x0800  /* Fill in the middle */
#define BF_SOFT         0x1000  /* For softer buttons */
#define BF_ADJUST       0x2000  /* Calculate the space left over */
#define BF_FLAT         0x4000  /* For flat rather than 3D borders */
#define BF_MONO         0x8000  /* For monochrome borders */


#endif

static const COLOR SysColors[] = {
  _RGB(212, 208, 200), /* COLOR_SCROLLBAR  */
  _RGB(58, 110, 165),  /* COLOR_BACKGROUND  */
  _RGB(10, 36, 106),   /* COLOR_ACTIVECAPTION  */
  _RGB(128, 128, 128), /* COLOR_INACTIVECAPTION  */
  _RGB(212, 208, 200), /* COLOR_MENU  */
  _RGB(255, 255, 255), /* COLOR_WINDOW  */
  _RGB(0, 0, 0),       /* COLOR_WINDOWFRAME  */
  _RGB(0, 0, 0),       /* COLOR_MENUTEXT  */
  _RGB(0, 0, 0),       /* COLOR_WINDOWTEXT  */
  _RGB(255, 255, 255), /* COLOR_CAPTIONTEXT  */
  _RGB(212, 208, 200), /* COLOR_ACTIVEBORDER  */
  _RGB(212, 208, 200), /* COLOR_INACTIVEBORDER  */
  _RGB(128, 128, 128), /* COLOR_APPWORKSPACE  */
  _RGB(10, 36, 106),   /* COLOR_HIGHLIGHT  */
  _RGB(255, 255, 255), /* COLOR_HIGHLIGHTTEXT  */
  _RGB(212, 208, 200), /* COLOR_BTNFACE  */
  _RGB(128, 128, 128), /* COLOR_BTNSHADOW  */
  _RGB(128, 128, 128), /* COLOR_GRAYTEXT  */
  _RGB(0, 0, 0),       /* COLOR_BTNTEXT  */
  _RGB(212, 208, 200), /* COLOR_INACTIVECAPTIONTEXT  */
  _RGB(255, 255, 255), /* COLOR_BTNHIGHLIGHT  */
  _RGB(64, 64, 64),    /* COLOR_3DDKSHADOW  */
  _RGB(212, 208, 200), /* COLOR_3DLIGHT  */
  _RGB(0, 0, 0),       /* COLOR_INFOTEXT  */
  _RGB(255, 255, 225), /* COLOR_INFOBK  */
  _RGB(181, 181, 181), /* COLOR_UNKNOWN  */
  _RGB(0, 0, 128),     /* COLOR_HOTLIGHT  */
  _RGB(166, 202, 240), /* COLOR_GRADIENTACTIVECAPTION  */
  _RGB(192, 192, 192), /* COLOR_GRADIENTINACTIVECAPTION  */
  _RGB(49, 106, 197),  /* COLOR_MENUHILIGHT  */
  _RGB(236, 233, 216)  /* COLOR_MENUBAR  */
};
#define NUM_SYSCOLORS 31

static COLOR myGetSysColor1(int nIndex)
{
  if (nIndex >= 0 && nIndex < NUM_SYSCOLORS) {
    return SysColors[nIndex];
  }

  return 0;
}

static const WORD wPattern_AA55[8] = { 0xaaaa, 0x5555, 0xaaaa, 0x5555,
    0xaaaa, 0x5555, 0xaaaa, 0x5555
                                     };

#define COLOR_SCROLLBAR         0
#define COLOR_BACKGROUND        1
#define COLOR_ACTIVECAPTION     2
#define COLOR_INACTIVECAPTION   3
#define COLOR_MENU              4
#define COLOR_WINDOW            5
#define COLOR_WINDOWFRAME       6
#define COLOR_MENUTEXT          7
#define COLOR_WINDOWTEXT        8
#define COLOR_CAPTIONTEXT       9
#define COLOR_ACTIVEBORDER      10
#define COLOR_INACTIVEBORDER    11
#define COLOR_APPWORKSPACE      12
#define COLOR_HIGHLIGHT         13
#define COLOR_HIGHLIGHTTEXT     14
#define COLOR_BTNFACE           15
#define COLOR_BTNSHADOW         16
#define COLOR_GRAYTEXT          17
#define COLOR_BTNTEXT           18
#define COLOR_INACTIVECAPTIONTEXT 19
#define COLOR_BTNHIGHLIGHT      20

#define COLOR_3DDKSHADOW        21
#define COLOR_3DLIGHT           22
#define COLOR_INFOTEXT          23
#define COLOR_INFOBK            24

#define COLOR_HOTLIGHT                  26
#define COLOR_GRADIENTACTIVECAPTION     27
#define COLOR_GRADIENTINACTIVECAPTION   28

#define COLOR_DESKTOP           COLOR_BACKGROUND
#define COLOR_3DFACE            COLOR_BTNFACE
#define COLOR_3DSHADOW          COLOR_BTNSHADOW
#define COLOR_3DHIGHLIGHT       COLOR_BTNHIGHLIGHT
#define COLOR_3DHILIGHT         COLOR_BTNHIGHLIGHT
#define COLOR_BTNHILIGHT        COLOR_BTNHIGHLIGHT

#define COLOR_3DDKSHADOW        21
#define COLOR_3DLIGHT           22
#define COLOR_INFOTEXT          23
#define COLOR_INFOBK            24

#define COLOR_HOTLIGHT                  26
#define COLOR_GRADIENTACTIVECAPTION     27
#define COLOR_GRADIENTINACTIVECAPTION   28


/* These tables are used in:
 * UITOOLS_DrawDiagEdge()
 * UITOOLS_DrawRectEdge()
 */
static const signed char LTInnerNormal[] = {
  -1,           -1,                 -1,                 -1,
  -1,           COLOR_BTNHIGHLIGHT, COLOR_BTNHIGHLIGHT, -1,
  -1,           COLOR_3DDKSHADOW,   COLOR_3DDKSHADOW,   -1,
  -1,           -1,                 -1,                 -1
};

static const signed char LTOuterNormal[] = {
  -1,                 COLOR_3DLIGHT,     COLOR_BTNSHADOW, -1,
  COLOR_BTNHIGHLIGHT, COLOR_3DLIGHT,     COLOR_BTNSHADOW, -1,
  COLOR_3DDKSHADOW,   COLOR_3DLIGHT,     COLOR_BTNSHADOW, -1,
  -1,                 COLOR_3DLIGHT,     COLOR_BTNSHADOW, -1
};

static const signed char RBInnerNormal[] = {
  -1,           -1,                -1,              -1,
  -1,           COLOR_BTNSHADOW,   COLOR_BTNSHADOW, -1,
  -1,           COLOR_3DLIGHT,     COLOR_3DLIGHT,   -1,
  -1,           -1,                -1,              -1
};

static const signed char RBOuterNormal[] = {
  -1,              COLOR_3DDKSHADOW,  COLOR_BTNHIGHLIGHT, -1,
  COLOR_BTNSHADOW, COLOR_3DDKSHADOW,  COLOR_BTNHIGHLIGHT, -1,
  COLOR_3DLIGHT,   COLOR_3DDKSHADOW,  COLOR_BTNHIGHLIGHT, -1,
  -1,              COLOR_3DDKSHADOW,  COLOR_BTNHIGHLIGHT, -1
};

static const signed char LTInnerSoft[] = {
  -1,                  -1,                -1,              -1,
  -1,                  COLOR_3DLIGHT,     COLOR_3DLIGHT,   -1,
  -1,                  COLOR_BTNSHADOW,   COLOR_BTNSHADOW, -1,
  -1,                  -1,                -1,              -1
};

static const signed char LTOuterSoft[] = {
  -1,              COLOR_BTNHIGHLIGHT, COLOR_3DDKSHADOW, -1,
  COLOR_3DLIGHT,   COLOR_BTNHIGHLIGHT, COLOR_3DDKSHADOW, -1,
  COLOR_BTNSHADOW, COLOR_BTNHIGHLIGHT, COLOR_3DDKSHADOW, -1,
  -1,              COLOR_BTNHIGHLIGHT, COLOR_3DDKSHADOW, -1
};

#define RBInnerSoft RBInnerNormal   /* These are the same */
#define RBOuterSoft RBOuterNormal

static const signed char LTRBOuterMono[] = {
  -1,           COLOR_WINDOWFRAME, COLOR_WINDOWFRAME, COLOR_WINDOWFRAME,
  COLOR_WINDOW, COLOR_WINDOWFRAME, COLOR_WINDOWFRAME, COLOR_WINDOWFRAME,
  COLOR_WINDOW, COLOR_WINDOWFRAME, COLOR_WINDOWFRAME, COLOR_WINDOWFRAME,
  COLOR_WINDOW, COLOR_WINDOWFRAME, COLOR_WINDOWFRAME, COLOR_WINDOWFRAME,
};

static const signed char LTRBInnerMono[] = {
  -1, -1,           -1,           -1,
  -1, COLOR_WINDOW, COLOR_WINDOW, COLOR_WINDOW,
  -1, COLOR_WINDOW, COLOR_WINDOW, COLOR_WINDOW,
  -1, COLOR_WINDOW, COLOR_WINDOW, COLOR_WINDOW,
};

static const signed char LTRBOuterFlat[] = {
  -1,                COLOR_BTNSHADOW, COLOR_BTNSHADOW, COLOR_BTNSHADOW,
  COLOR_BTNFACE,     COLOR_BTNSHADOW, COLOR_BTNSHADOW, COLOR_BTNSHADOW,
  COLOR_BTNFACE,     COLOR_BTNSHADOW, COLOR_BTNSHADOW, COLOR_BTNSHADOW,
  COLOR_BTNFACE,     COLOR_BTNSHADOW, COLOR_BTNSHADOW, COLOR_BTNSHADOW,
};

static const signed char LTRBInnerFlat[] = {
  -1, -1,              -1,              -1,
  -1, COLOR_BTNFACE,     COLOR_BTNFACE,     COLOR_BTNFACE,
  -1, COLOR_BTNFACE,     COLOR_BTNFACE,     COLOR_BTNFACE,
  -1, COLOR_BTNFACE,     COLOR_BTNFACE,     COLOR_BTNFACE,
};


/* WM_PRINT flags */
#define PRF_CHECKVISIBLE    0x00000001L
#define PRF_NONCLIENT       0x00000002L
#define PRF_CLIENT          0x00000004L
#define PRF_ERASEBKGND      0x00000008L
#define PRF_CHILDREN        0x00000010L
#define PRF_OWNED           0x00000020L

/* 3D border styles */
#define BDR_RAISEDOUTER 0x0001
#define BDR_SUNKENOUTER 0x0002
#define BDR_RAISEDINNER 0x0004
#define BDR_SUNKENINNER 0x0008

#ifndef BDR_OUTER
#define BDR_OUTER       0x0003
#define BDR_INNER       0x000c
#endif // BDR_OUTER

#define EDGE_RAISED     (BDR_RAISEDOUTER | BDR_RAISEDINNER)
#define EDGE_SUNKEN     (BDR_SUNKENOUTER | BDR_SUNKENINNER)
#define EDGE_ETCHED     (BDR_SUNKENOUTER | BDR_RAISEDINNER)
#define EDGE_BUMP       (BDR_RAISEDOUTER | BDR_SUNKENINNER)

/* Border flags */
#define BF_LEFT         0x0001
#define BF_TOP          0x0002
#define BF_RIGHT        0x0004
#define BF_BOTTOM       0x0008

#define BF_TOPLEFT      (BF_TOP | BF_LEFT)
#define BF_TOPRIGHT     (BF_TOP | BF_RIGHT)
#define BF_BOTTOMLEFT   (BF_BOTTOM | BF_LEFT)
#define BF_BOTTOMRIGHT  (BF_BOTTOM | BF_RIGHT)
#define BF_RECT         (BF_LEFT | BF_TOP | BF_RIGHT | BF_BOTTOM)

#define BF_DIAGONAL     0x0010

// For diagonal lines, the BF_RECT flags specify the end point of the
// vector bounded by the rectangle parameter.
#define BF_DIAGONAL_ENDTOPRIGHT     (BF_DIAGONAL | BF_TOP | BF_RIGHT)
#define BF_DIAGONAL_ENDTOPLEFT      (BF_DIAGONAL | BF_TOP | BF_LEFT)
#define BF_DIAGONAL_ENDBOTTOMLEFT   (BF_DIAGONAL | BF_BOTTOM | BF_LEFT)
#define BF_DIAGONAL_ENDBOTTOMRIGHT  (BF_DIAGONAL | BF_BOTTOM | BF_RIGHT)


#define BF_MIDDLE       0x0800  /* Fill in the middle */
#define BF_SOFT         0x1000  /* For softer buttons */
#define BF_ADJUST       0x2000  /* Calculate the space left over */
#define BF_FLAT         0x4000  /* For flat rather than 3D borders */
#define BF_MONO         0x8000  /* For monochrome borders */

CC_INLINE int imdraw_fillrectX(scanline* sl, IRECT rc, BOOL invx, const COLOR* clrX)
{
  return 0;
}

CC_INLINE int imdraw_fillrectY(scanline* sl, IRECT rc, BOOL invy, const COLOR* clrY)
{
  return 0;
}

CC_INLINE int imdraw_fillrectXY(scanline* sl, IRECT rc, BOOL invx, const COLOR* clrX, BOOL invy, const COLOR* clrY)
{
  return 0;
}

CC_INLINE int scanline_bin_border(scanline* sl, IRECT rc, COLOR clrFill, int nL, const COLOR* clrL, int nT, const COLOR* clrT, int nR, const COLOR* clrR, int nB, const COLOR* clrB)
{
#if 0
  int w = sl->im->w, h = sl->im->h;
  int l = rc.l, t = rc.t, r = rc.r, b = rc.b;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  pclip = &clip;
  nL = BOUND(nL, 0, w);
  nT = BOUND(nT, 0, h);
  nR = BOUND(nR, 0, w);
  nB = BOUND(nB, 0, h);
  imdraw_fillrectX(im, pclip, iRECT(l, t + nT, l + nL, b - nB), 0, clrL);
  imdraw_fillrectX(im, pclip, iRECT(r - nR, t + nT, r, b - nB), 1, clrR);
  imdraw_fillrectY(im, pclip, iRECT(l + nL, t, r - nR, t + nT), 0, clrT);
  imdraw_fillrectY(im, pclip, iRECT(l + nL, b - nB, r - nR, b), 1, clrB);
  imdraw_fillrectXY(im, pclip, iRECT(l, t, l + nL, t + nT), 0, clrL, 0, clrT);
  imdraw_fillrectXY(im, pclip, iRECT(r - nR, t, r, t + nT), 1, clrR, 0, clrT);
  imdraw_fillrectXY(im, pclip, iRECT(l, b - nB, l + nL, b), 0, clrL, 1, clrB);
  imdraw_fillrectXY(im, pclip, iRECT(r - nR, b - nB, r, b), 1, clrR, 1, clrB);
  
  if (clrFill) {
    imdraw_fillrect(im, pclip, iRECT(l + nL, t + nT, r - nR, b - nB), clrFill);
  }
#endif
  
  return 0;
}

// Ported from WINE20020904
// Same as DrawEdge invoked without BF_DIAGONAL
// 
// 23-Nov-1997: Changed by Bertho Stultiens
// 
// Well, I started testing this and found out that there are a few things
// that weren't quite as win95. The following rewrite should reproduce
// win95 results completely.
// The colorselection is table-driven to avoid awfull if-statements.
// The table below show the color settings.
// 
// Pen selection table for uFlags = 0
// 
// uType |  LTI  |  LTO  |  RBI  |  RBO
// ------+-------+-------+-------+-------
//  0000 |   x   |   x   |   x   |   x
//  0001 |   x   |  22   |   x   |  21
//  0010 |   x   |  16   |   x   |  20
//  0011 |   x   |   x   |   x   |   x
// ------+-------+-------+-------+-------
//  0100 |   x   |  20   |   x   |  16
//  0101 |  20   |  22   |  16   |  21
//  0110 |  20   |  16   |  16   |  20
//  0111 |   x   |   x   |   x   |   x
// ------+-------+-------+-------+-------
//  1000 |   x   |  21   |   x   |  22
//  1001 |  21   |  22   |  22   |  21
//  1010 |  21   |  16   |  22   |  20
//  1011 |   x   |   x   |   x   |   x
// ------+-------+-------+-------+-------
//  1100 |   x   |   x   |   x   |   x
//  1101 |   x   | x (22)|   x   | x (21)
//  1110 |   x   | x (16)|   x   | x (20)
//  1111 |   x   |   x   |   x   |   x
// 
// Pen selection table for uFlags = BF_SOFT
// 
// uType |  LTI  |  LTO  |  RBI  |  RBO
// ------+-------+-------+-------+-------
//  0000 |   x   |   x   |   x   |   x
//  0001 |   x   |  20   |   x   |  21
//  0010 |   x   |  21   |   x   |  20
//  0011 |   x   |   x   |   x   |   x
// ------+-------+-------+-------+-------
//  0100 |   x   |  22   |   x   |  16
//  0101 |  22   |  20   |  16   |  21
//  0110 |  22   |  21   |  16   |  20
//  0111 |   x   |   x   |   x   |   x
// ------+-------+-------+-------+-------
//  1000 |   x   |  16   |   x   |  22
//  1001 |  16   |  20   |  22   |  21
//  1010 |  16   |  21   |  22   |  20
//  1011 |   x   |   x   |   x   |   x
// ------+-------+-------+-------+-------
//  1100 |   x   |   x   |   x   |   x
//  1101 |   x   | x (20)|   x   | x (21)
//  1110 |   x   | x (21)|   x   | x (20)
//  1111 |   x   |   x   |   x   |   x
// 
// x = don't care; (n) = is what win95 actually uses
// LTI = left Top Inner line
// LTO = left Top Outer line
// RBI = Right Bottom Inner line
// RBO = Right Bottom Outer line
// 15 = COLOR_BTNFACE
// 16 = COLOR_BTNSHADOW
// 20 = COLOR_BTNHIGHLIGHT
// 21 = COLOR_3DDKSHADOW
// 22 = COLOR_3DLIGHT
static BOOL IntDrawRectEdge(scanline* sl, IRECT rc, UINT uType, UINT uFlags)
{
  signed char LTInnerI, LTOuterI;
  signed char RBInnerI, RBOuterI;
  IRECT InnerRect = rc;
  COLOR crFill = 0;
  COLOR crLT[4] = {0};
  COLOR crRB[4] = {0};
  int LBpenplus = 0;
  int LTpenplus = 0;
  int RTpenplus = 0;
  int RBpenplus = 0;
  BOOL retval = !(((uType & BDR_INNER) == BDR_INNER
      || (uType & BDR_OUTER) == BDR_OUTER)
      && !(uFlags & (BF_FLAT | BF_MONO)));
  /* Init some vars */

  /* Determine the colors of the edges */
  if (uFlags & BF_MONO) {
    LTInnerI = RBInnerI = LTRBInnerMono[uType & (BDR_INNER | BDR_OUTER)];
    LTOuterI = RBOuterI = LTRBOuterMono[uType & (BDR_INNER | BDR_OUTER)];
  }
  else if (uFlags & BF_FLAT) {
    LTInnerI = RBInnerI = LTRBInnerFlat[uType & (BDR_INNER | BDR_OUTER)];
    LTOuterI = RBOuterI = LTRBOuterFlat[uType & (BDR_INNER | BDR_OUTER)];

    /* Bertho Stultiens states above that this function exactly matches win95
     * In win98 BF_FLAT rectangles have an inner border same color as the
    * middle (COLOR_BTNFACE). I believe it's the same for win95 but since
    * I don't know I go with Bertho and just sets it for win98 until proven
    * otherwise.
    *                                          Dennis Bj枚rklund, 10 June, 99
    */
    if (LTInnerI != -1) {
      LTInnerI = RBInnerI = COLOR_BTNFACE;
    }
  }
  else if (uFlags & BF_SOFT) {
    LTInnerI = LTInnerSoft[uType & (BDR_INNER | BDR_OUTER)];
    LTOuterI = LTOuterSoft[uType & (BDR_INNER | BDR_OUTER)];
    RBInnerI = RBInnerSoft[uType & (BDR_INNER | BDR_OUTER)];
    RBOuterI = RBOuterSoft[uType & (BDR_INNER | BDR_OUTER)];
  }
  else {
    LTInnerI = LTInnerNormal[uType & (BDR_INNER | BDR_OUTER)];
    LTOuterI = LTOuterNormal[uType & (BDR_INNER | BDR_OUTER)];
    RBInnerI = RBInnerNormal[uType & (BDR_INNER | BDR_OUTER)];
    RBOuterI = RBOuterNormal[uType & (BDR_INNER | BDR_OUTER)];
  }

  if ((uFlags & BF_BOTTOMLEFT) == BF_BOTTOMLEFT) {
    LBpenplus = 1;
  }

  if ((uFlags & BF_TOPRIGHT) == BF_TOPRIGHT) {
    RTpenplus = 1;
  }

  if ((uFlags & BF_BOTTOMRIGHT) == BF_BOTTOMRIGHT) {
    RBpenplus = 1;
  }

  if ((uFlags & BF_TOPLEFT) == BF_TOPLEFT) {
    LTpenplus = 1;
  }

  if ((uFlags & BF_MIDDLE) && retval) {
    crFill = myGetSysColor1(uFlags & BF_MONO ? COLOR_WINDOW : COLOR_BTNFACE);
  }

  //CC_INLINE int scanline_bin_border(scanline* sl, IRECT rc, COLOR clrFill, int nL, const COLOR* clrL, int nT, const COLOR* clrT, int nR, const COLOR* clrR, int nB, const COLOR* clrB)
  /* Draw the outer edge */
  crLT[0] = (myGetSysColor1(LTOuterI));
  crRB[0] = (myGetSysColor1(RBOuterI));
  crLT[1] = (myGetSysColor1(LTInnerI));
  crRB[1] = (myGetSysColor1(RBInnerI));
  {
    int nLT = !!crLT[0] + !!crLT[1];
    int nRB = !!crRB[0] + !!crRB[1];
    scanline_bin_border(sl, InnerRect, crFill, (uFlags & BF_LEFT) ? nLT : 0, crLT, (uFlags & BF_TOP) ? nLT : 0, crLT, (uFlags & BF_RIGHT) ? nRB : 0, crRB, (uFlags & BF_BOTTOM) ? nRB : 0, crRB);
  }

  if (((uFlags & BF_MIDDLE) && retval) || (uFlags & BF_ADJUST)) {
    int add = (LTRBInnerMono[uType & (BDR_INNER | BDR_OUTER)] != -1 ? 1 : 0)
        + (LTRBOuterMono[uType & (BDR_INNER | BDR_OUTER)] != -1 ? 1 : 0);

    if (uFlags & BF_LEFT) {
      InnerRect.l += add;
    }

    if (uFlags & BF_RIGHT) {
      InnerRect.r -= add;
    }

    if (uFlags & BF_TOP) {
      InnerRect.t += add;
    }

    if (uFlags & BF_BOTTOM) {
      InnerRect.b -= add;
    }

    if (uFlags & BF_ADJUST) {
      rc = InnerRect;
    }
  }

  return retval;
}

static BOOL scanline_bin_edge(scanline* sl, IRECT rc, UINT edge, UINT flags)
{
  if (flags & BF_DIAGONAL) {
    //return IntDrawDiagEdge(hDC, rc, edge, flags);
  }
  else {
    return IntDrawRectEdge(sl, rc, edge, flags);
  }

  return FALSE;
}

CC_INLINE int scanline_bin_xrects(scanline* sl, const IRect* r, int n)
{
  //画出矩形框
  int i;

  for (i = 0; i < n; ++i) {
    IRECT rc = iRECT(r[i].x, r[i].y, r[i].x + r[i].w,  r[i].y + r[i].h);
    scanline_set_recti(sl, rc);
  }

  return 0;
}

// 矢量字体
CC_INLINE int PutOneChar_(int h, int w, uchar* A, int al, int ai, int cn, const uchar* B, int bl, int x, int y, int bpp, COLOR clr)
{
  int i, j;
  uchar* ptr = A;
  const uchar* bit;
  uchar* pclr = (uchar*)&clr;
  bit = B + y * bl;

  switch (bpp) {
  case 1:
    for (i = 0; i < h; ++i, ptr += al, bit += bl) {
      for (j = 0; j < w; ++j) {
        int k = j + x;

        if ((bit[k >> 3] >> (7 - (k & 7))) & 1) {
          memcpy(ptr + j * ai, pclr, cn);
        }
      }
    }

    break;

  case 8:
    for (i = 0; i < h; ++i, ptr += al, bit += bl) {
      for (j = 0; j < w; ++j) {
        int k, _a = bit[j + x];

        if (_a) {
          uchar* pix = ptr + j * ai;
          //_a = (_a * 6) >> 2;
          //_a = MIN(_a, 255);

          for (k = 0; k < ai; ++k) {
            SETBLENP(pix, pclr, k, _a);
          }
        }
      }
    }

    break;
  }

  return 0;
}

// 矢量字体
CC_INLINE int PutOneChar(scanline* sl, IRECT rect, const uchar* B, int bl, int xoff, int yoff, int bpp)
{
  int y, x;
  int w = sl->im->w, h = sl->im->h;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);
  
  switch (bpp) {
  case 1:
    for (y = clip.t; y < clip.b; ++y) {
      const uchar* bit = B + (y-clip.t+yoff) * bl;
      for (x = clip.l; x < clip.r; ++x) {
        int k = x-clip.l + xoff;
        
        if ((bit[k >> 3] >> (7 - (k & 7))) & 1) {
          addpix_bin(x, y);
        }
      }
    }
    
    break;
    
  case 8:
    for (y = clip.t; y < clip.b; ++y) {
      const uchar* bit = B + (y-clip.t+yoff) * bl;
      for (x = clip.l; x < clip.r; ++x) {
        int _a = bit[x-clip.l + xoff];
        
        if (_a) {
          addpix_aa(x, y, _a);
        }
      }
    }
    
    break;
  }
  
  return 0;
}

CC_INLINE int PutOneChar2(scanline* sl, IRECT rc, const int* mat, COLOR clr, int bh, int bw, const uchar* B, int bl, int bpp)
{
  int x = 0, y;
  int w = sl->im->w, h = sl->im->h;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);

  for (y = clip.t; y < clip.b; ++y) {
    const uchar* bit = B + y * bl;
    for (x = clip.l; x < clip.r; ++x) {
      int _a;
      _a = bit[x];

      if (_a) {
        addpix_aa(x, y, _a);
      }
    }
  }

  return 0;
}


#define MENUIMAGES_W 9
#define MENUIMAGES_H 9
#define CBCGPMenuImages_Size() iSIZE(MENUIMAGES_W, MENUIMAGES_H)
typedef enum {
  IdArowDown,
  IdArowRight,
  IdCheck,
  IdMinimize,
  IdRestore,
  IdClose,
  IdMaximize,
  IdArowUp,
  IdArowShowAll,
  IdArowLeft,
  IdCloseSmall,
  IdMoreButtons,
  IdRadio,
  IdArowDownLarge,
  IdArowRightLarge,
  IdPinHorz,
  IdPinVert,
  IdArowLeftLarge,
  IdArowFirst,
  IdArowLast,
  IdArowRightTab3d,
  IdArowLeftTab3d,
  IdArowRightDsbldTab3d,
  IdArowLeftDsbldTab3d,
  IdArowUpLarge,
  IdArowPageLeft,
  IdArowPageRight,
  IdArowBack,
  IdArowForward,
  IdCustomizeArowDown,
  IdCustomizeArowLeft,
  IdCustomizeMoreButtonsHorz,
  IdCustomizeMoreButtonsVert,
  IdCustomizeArowDownBold,
  IdCloseBold,
  IdLaunchArrow,
} IMAGES_IDS;
enum IMAGE_STATE {
  ImageBlack,
  ImageGray,
  ImageLtGray,
  ImageWhite,
  ImageDkGray,
  ImageBlack2,
};

CC_INLINE int scanline_bin_bit(scanline* sl, IRECT rc, int fmt, int cx, int cy, const uchar* bit, int bl, int tx, int ty, int bpp)
{
  IRECT rc2 = {0};
  int x, y;
  int w = sl->im->w, h = sl->im->h;
  iRectAlign(rc, cx, cy, fmt, &rc2);
  PutOneChar(sl, rc2, bit, bl, tx + x - rc2.l, ty + y - rc2.t, bpp);
  return 0;
}

#define CBCGPMenuImages_Size()  iSIZE(MENUIMAGES_W, MENUIMAGES_H)

#if 0
CC_INLINE int scanline_bin_menuimg(scanline* sl, IRECT rc, int id)
{
  static const uchar menuimg_9x324[] = {
#include "menuimg_9x324.txt"
  };
  enum { cx = MENUIMAGES_W, cy = MENUIMAGES_H, MAXid = countof(menuimg_9x324) * 8 / (cx * cy), bl = countof(menuimg_9x324) * 8 / (cy)};
  scanline_bin_bit(sl, rc, TF_VCENTER | TF_CENTER, cx, cy, menuimg_9x324 + (id * cx) / 8, (bl + 7) >> 3, (id * cx) & 7, 0, 1);
  return 0;
}
#endif

CC_INLINE int scanline_bin_dpoint(scanline* sl, const DPOINT* pts, int n, COLOR crFg)
{
  int i;
  int w = sl->im->w, h = sl->im->h;
  IRECT clip = scanline_clip(sl, 0, 0, w, h);

  for (i = 0; i < n; ++i) {
    int x = (int)pts[i].x;
    int y = (int)pts[i].y;

    if (x >= clip.l && x < clip.r && y >= clip.t && y < clip.b) {
      addpix_bin(x, y);
    }
  }

  return 0;
}

#if 0
// 矢量字体
//#include "cv/imgproc/drawing.inl"
CC_INLINE int scanline_bin_text_v(scanline* sl, const char* text, int org_x, int org_y, enum CC_FONT_FACE fontFace, double fontScale, PenStyle* penstyle)
{
  int bottomLeftOrigin = 0;
  IPOINT pts[ 1 << 10 ];
  int i;
  const int* ascii = getFontData(fontFace);
  const char** faces = g_HersheyGlyphs;
  int base_line = -(ascii[0] & 15);
  int hscale = cRound(fontScale*XY_ONE), vscale = hscale;
  //CcFont default_font;

  if (bottomLeftOrigin) {
    vscale = -vscale;
  }

  int view_x = org_x << XY_SHIFT;
  int view_y = (org_y << XY_SHIFT) + base_line * vscale;
  
  for (i = 0; text[ i ] != '\0'; i++) {
    int c = (unsigned char) text[ i ];
    const char* ptr;
    IPOINT p;

    if (c > 128 || c < ' ') {
      c = '?';
    }

    ptr = faces[ ascii[(c - ' ') + 1 ] ];
    p.x = (unsigned char) ptr[ 0 ] - 'R';
    p.y = (unsigned char) ptr[ 1 ] - 'R';
    int dx = p.y * hscale;
    view_x -= p.x * hscale;
    int npts = 0;

    for (ptr += 2;;) {
      if (*ptr == ' ' || !*ptr) {
        scanline_set_poly_dash_stroke_i(sl, pts, npts, 0, XY_SHIFT, penstyle);
        if( !*ptr++ )
          break;
        npts = (0);
      }
      else {
        p.x = (unsigned char) ptr[ 0 ] - 'R';
        p.y = (unsigned char) ptr[ 1 ] - 'R';
        ptr += 2;
        pts[npts++] = cPoint(p.x*hscale + view_x, p.y*vscale + view_y);
      }
    }

    view_x += dx;
  }

  return 0;
}
#endif

#undef BDR_OUTER
#undef BDR_INNER
#undef BDR_RAISED
#undef BDR_SUNKEN


