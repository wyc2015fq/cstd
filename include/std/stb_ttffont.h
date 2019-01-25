
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
int stb_ttffont_GetCodepointShape(const font_t* fo, int unicode_codepoint, ttf_vertex** vertices) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  return stbtt_GetCodepointShape(info->info, unicode_codepoint, (stbtt_vertex**)vertices);
}
float stb_ttffont_ScaleForPixelHeight(const font_t* fo, float height) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  return stbtt_ScaleForPixelHeight(info->info, height);
}
void stb_ttffont_GetFontVMetrics(const font_t* fo, int* ascent, int* descent, int* lineGap) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  stbtt_GetFontVMetrics(info->info, ascent, descent, lineGap);
}
void stb_ttffont_GetCodepointHMetrics(const struct font_t* fo, int codepoint, int* advanceWidth, int* leftSideBearing) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  stbtt_GetCodepointHMetrics(info->info, codepoint, advanceWidth, leftSideBearing)
}
void stb_ttffont_GetCodepointBitmapBoxSubpixel(const font_t* font, int codepoint, float shift_x, float shift_y, int* ix0, int* iy0, int* ix1, int* iy1) {
  stb_ttffont_t* info = container_of(fo, stb_ttffont_t, font);
  stbtt_GetCodepointBitmapBoxSubpixel(info->info, codepoint, shift_x, shift_y, ix0, iy0, ix1, iy1);
}
void stb_ttffont_MakeCodepointBitmapSubpixel(const font_t* fo, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint);

font_t* stb_ttffont_init(stb_ttffont_t* font, const char* fontfile) {
  static font_fun_t  stb_ttffont_fun[1] = {
#define FONT_FUN_DEF_DEF(RET, NAME, ARGS)  stb_ttffont_ ## NAME,
  FONT_FUN_DEF(FONT_FUN_DEF_DEF)
#undef FONT_FUN_DEF_DEF
  };

  font->ttf_buffer = (uchar*)loaddata(fontfile, &font->len);
  stbtt_InitFont(font->info, font->ttf_buffer, stbtt_GetFontOffsetForIndex(font->ttf_buffer, 0));
  return font->font;
}

#endif // _STD_STB_TTFFONT_H_
