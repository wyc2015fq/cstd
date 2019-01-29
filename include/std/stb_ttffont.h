
#ifndef _STD_STB_TTFFONT_H_
#define _STD_STB_TTFFONT_H_

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb/stb_truetype.h"
#include "std/gdi_c.h"
#include "std/fileio_c.h"

struct stb_ttffont_t {
  font_t font[1];
  stbtt_fontinfo info[1];
  uchar* ttf_buffer;
  int len;
};

int stb_ttffont_Release(const struct font_t* fo) {
  if (fo) {
    stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
    if (info->ttf_buffer) free(info->ttf_buffer);
    info->ttf_buffer = NULL;
    info->len = 0;
  }
  return 0;
}
int stb_ttffont_GetShape(const font_t* fo, int unicode_codepoint, ttf_vertex** vertices) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  return stbtt_GetCodepointShape(info->info, unicode_codepoint, (stbtt_vertex**)vertices);
}
float stb_ttffont_ScaleForPixelHeight(const font_t* fo, float height) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  return stbtt_ScaleForPixelHeight(info->info, height);
}
void stb_ttffont_GetMetrics(font_t* fo) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  int ascent, descent, lineGap;
  int x0, y0, x1, y1;
  font_t* font_metrics = fo;
  stbtt_GetFontVMetrics(info->info, &ascent, &descent, &lineGap);
  stbtt_GetFontBoundingBox(info->info, &x0, &y0, &x1, &y1);
  font_metrics->height = ascent - descent;
  font_metrics->ascent = ascent;
  font_metrics->descent = descent;
  font_metrics->lineGap = lineGap;
  font_metrics->ascent = ascent;
  font_metrics->bbox = iRECT(x0, y0, x1, y1);
}
void stb_ttffont_GetGlyphBox(const struct font_t* fo, int codepoint, GlyphBox* glyph_box) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  int advanceWidth, leftSideBearing;
  int x0, y0, x1, y1;
  stbtt_GetCodepointHMetrics(info->info, codepoint, &advanceWidth, &leftSideBearing);
  stbtt_GetCodepointBox(info->info, codepoint, &x0, &y0, &x1, &y1);
  glyph_box->advanceWidth = advanceWidth;
  glyph_box->leftSideBearing = leftSideBearing;
  glyph_box->bbox = iRECT(x0, -y1, x1, -y0);
}
void stb_ttffont_GetGlyphBitmap(const struct font_t* fo, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  stbtt_MakeCodepointBitmapSubpixel(info->info, output, out_w, out_h, out_stride, scale_x, scale_y, shift_x, shift_y, codepoint);
}

font_t* stb_ttffont_init(stb_ttffont_t* font, const char* fontfile) {
  static font_fun_t  stb_ttffont_fun[1] = {
#define FONT_FUN_DEF_DEF(RET, NAME, ARGS)  stb_ttffont_ ## NAME,
  FONT_FUN_DEF(FONT_FUN_DEF_DEF)
#undef FONT_FUN_DEF_DEF
  };

  font->ttf_buffer = (uchar*)loaddata(fontfile, &font->len);
  stbtt_InitFont(font->info, font->ttf_buffer, stbtt_GetFontOffsetForIndex(font->ttf_buffer, 0));
  stb_ttffont_GetMetrics(font->font);
  font->font->fun = stb_ttffont_fun;
  return font->font;
}

#endif // _STD_STB_TTFFONT_H_
