#ifndef _FONT_INL_
#define _FONT_INL_

//////////////////////////////////////////////////////////////////////////

#define FONT_BIT_INIT(fb, PIX, BX, BY, X, Y, S) ((fb)->bit=PIX, (fb)->bx=BX, (fb)->by=BY, (fb)->x=X, (fb)->y=Y, (fb)->step=S)
typedef struct raster_font_bit_t {
  uchar bx, by;
  char x, y;
  uchar bit[1];
} raster_font_bit_t;
typedef struct raster_font_t {
  uchar sz, h, bl; // sz=每个font_bit_t的大小 cy=字体高
  uchar as, ae;
  uchar hs, he;
  uchar ls, le;
  uchar data[1];
} raster_font_t;
CC_INLINE int sys_font_getbit_memraster(const font_t* fo0, int ch, font_bit_t* bit)
{
  const raster_font_t* fo = (raster_font_t*)fo0->user;
  uchar hi = (ch >> 8) & 0xff;
  uchar lo = ch & 0xff;
  const uchar* data = fo->data;
  raster_font_bit_t* rbit = NULL;
  int XAdvance = fo->h/2;
  if (ch >= fo->as && ch <= fo->ae) {
    rbit = (raster_font_bit_t*)(data + (ch - fo->as) * fo->sz);
    XAdvance = fo->h/2;
  }
  else if (hi >= fo->hs && hi <= fo->he && lo >= fo->ls && lo <= fo->le) {
    int t1 = fo->ae - fo->as + 1;
    int t2 = fo->le - fo->ls + 1;
    int off = (hi - fo->hs) * t2 + (lo - fo->ls) + t1;
    rbit = (raster_font_bit_t*)(data + off * fo->sz);
    XAdvance = fo->h;
  }
  if (rbit) {
    if (bit) {
      int bl = (rbit->bx + 7) >> 3;
      FONT_BIT_INIT(bit, rbit->bit, rbit->bx, rbit->by, rbit->x, rbit->y, bl);
    }
  }
  return XAdvance;
}
static int sys_font_memraster_release(const struct font_t* fo)
{
  //pfree((void*)fo);
  return 0;
}
int font_init_memraster(font_t* fo, const char* name, const raster_font_t* rfo)
{
  memset(fo, 0, sizeof(font_t));
  fo->bl = rfo->bl;
  fo->h = rfo->h;
  fo->w = rfo->h / 2;
  fo->bpp = 1;
  fo->style = 0;
  fo->user = (void*)rfo;
  if (name) {
    strcpy(fo->name, name);
  }
  fo->getbit = sys_font_getbit_memraster;
  fo->release = sys_font_memraster_release;
  return 0;
}
void texfont_free(texfont_t* g)
{
  myFree(g->Glyphs, g->Glyphs_Size);
  //myFree(g->IndexLookup, g->IndexLookup_Size);
  //myFree(g->IndexXAdvance, g->IndexXAdvance_Size);
  FREE_TEX(g->tex);
  memset(g, 0, sizeof(*g));
}
///////////////////////////////////////////////////////////////////////////////////////////
static const font_glyph_t* texfont_FindGlyph(const texfont_t* g, unsigned short c)
{
  if (c < g->IndexLookup_Size) {
    const unsigned short i = g->IndexLookup[c];
    if (i != (unsigned short) - 1) {
      return g->Glyphs + i;
    }
  }
  return g->FallbackGlyph;
}
// Simple word-wrapping for English, not full-featured. Please submit failing cases!
// FIXME: Much possible improvements (don't cut things like "word !", "word!!!" but cut within "word,,,,", more sensible support for punctuations, support for Unicode punctuations, etc.)
// For references, possible wrap point marked with ^
// "aaa bbb, ccc,ddd. eee fff. ggg!"
// ^ ^ ^ ^ ^__ ^ ^
// List of hardcoded separators: .,;!?'"
// Skip extra blanks after a line returns (that includes not counting them in width computation)
// e.g. "Hello world" --> "Hello" "World"
// Cut words that cannot possibly fit within one line.
// e.g.: "The tropical fish" with ~5 characters worth of width --> "The tr" "opical" "fish"
const char* texfont_CalcWordWrapPositionA(const texfont_t* g, float scale, const char* text, const char* text_end, float wrap_width)
{
  float line_width = 0.0f;
  float word_width = 0.0f;
  float blank_width = 0.0f;
  const char* word_end = text;
  const char* prev_word_end = NULL;
  bool inside_word = true;
  const char* s = text;
  while (s < text_end) {
    unsigned int c = (unsigned int) * s;
    const char* next_s;
    float char_width;
    if (c < 0x80) {
      next_s = s + 1;
    }
    else {
      next_s = s + TextCharFromUtf8(&c, s, text_end);
    }
    if (c == 0) {
      break;
    }
    if (c < 32) {
      if (c == '\n') {
        line_width = word_width = blank_width = 0.0f;
        inside_word = true;
        s = next_s;
        continue;
      }
      if (c == '\r') {
        s = next_s;
        continue;
      }
    }
    char_width = ((int)c < g->IndexXAdvance_Size ? g->IndexXAdvance[(int)c] : g->FallbackXAdvance) * scale;
    if (ImCharIsSpace(c)) {
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
ImVec2 texfont_CalcTextSizeA(const texfont_t* g, float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining)
{
  const float line_height = size;
  const float scale = size>0.f ? size / g->size : 1.f;
  ImVec2 text_size = fVec2(0, 0);
  float char_width, line_width = 0.0f;
  const bool word_wrap_enabled = (wrap_width > 0.0f);
  const char* word_wrap_eol = NULL;
  const char* s = text_begin;
  const char* prev_s;
  unsigned int c;
  if (!text_end) {
    text_end = text_begin + strlen(text_begin); // FIXME-OPT: Need to avoid this.
  }
  while (s < text_end) {
    if (word_wrap_enabled) {
      // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
      if (!word_wrap_eol) {
        word_wrap_eol = texfont_CalcWordWrapPositionA(g, scale, s, text_end, wrap_width - line_width);
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
          if (ImCharIsSpace(c)) {
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
    c = (unsigned int) * s;
    if (c < 0x80) {
      s += 1;
    }
    else {
      s += TextCharFromUtf8(&c, s, text_end);
      if (c == 0) {
        break;
      }
    }
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
    char_width = ((int)c < g->IndexXAdvance_Size ? g->IndexXAdvance[(int)c] : g->FallbackXAdvance) * scale;
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
int font_bit_copy(const font_bit_t* bit, uchar* out, int step)
{
  int i, j;
  uchar ss8[] = {0, 255};
  //uint ss32[] = {0, 0xffffffff};
  for (i = 0; i < bit->by; ++i) {
    uchar* ptr = bit->bit + i * bit->step;
    uchar* dst = out + i * step;
    for (j = 0; j < bit->bx; ++j) {
#if 1
      dst[j] = ss8[((ptr[j >> 3] >> (7 - (j & 7))) & 1)];
#else
      uchar t = ptr[j >> 3];
      int k = 7 - (j & 7);
      //dst[j] = ss8[((t >> (k)) & 1)];
#endif
    }
  }
  return 0;
}
#include "geo/packing.inl"
//#include "ui/window.inl"
//#include "font/font.inl"
int TextChar(const char* in_text)
{
  unsigned int c = 0;
  const unsigned char* str = (const unsigned char*)in_text;
  if (!(*str & 0x80)) {
    c = (unsigned int)(*str++);
    return c;
  }
  else {
    c = (*str++) << 8;
    c |= (*str++);
    return c;
  }
}
int rec_num0 = 0;
int packing_raster_font(const font_t* fo, texfont_t* g, img_t* im)
{
  int i, n = 0, max_y;
  font_bit_t bit[1] = {0};
  int rec_num = 0, width = 0;
  double sumarea = 0;
  idrect_t* rec = NULL; //songti_12
  for (n = 0, i = 0; i < (1 << 16); ++i) {
    if (fo->getbit(fo, i, bit)) {
      ++n;
      sumarea += bit->bx * bit->by;
    }
  }
  width = (int)sqrt(sumarea);
  width = next_power(width);
  rec_num = n;
  MYREALLOC(rec, rec_num);
  for (n = 0, i = 0; i < (1 << 16) && n < rec_num; ++i) {
    if (fo->getbit(fo, i, bit)) {
      rec[n].w = bit->bx;
      rec[n].h = bit->by;
      rec[n].i = i;
      ++n;
    }
  }
  rec_num = n;
  rec_num0 = rec_num;
  packing_sortw(rec_num, rec);
  packing_sorth(rec_num, rec);
  max_y = packing_FFD2(width, rec_num, rec);
  max_y = next_power(max_y);
  //MYREALLOC(g->TexPixelsAlpha8, max_y * width);
  g->show_size = g->size = (float)fo->h;
  //g->TexHeight = max_y;
  //g->TexWidth = width;
  //IMINIT(im, max_y, width, g->TexPixelsAlpha8, width, 1, 1);
  imsetsize(im, max_y, width, 1, 1);
  g->Glyphs_Size = rec_num + 10;
  g->IndexLookup_Size = (1 << 16);
  g->IndexXAdvance_Size = (1 << 16);
  g->Glyphs = (font_glyph_t*)pmalloc(sizeof(*g->Glyphs)*g->Glyphs_Size);
  //g->IndexLookup = (ushort*)pmalloc(sizeof(ushort)*g->IndexLookup_Size);
  //g->IndexXAdvance = (float*)pmalloc(sizeof(float)*g->IndexXAdvance_Size);
  //MYREALLOC(g->IndexLookup, g->IndexLookup_Size);
  //MYREALLOC(g->IndexXAdvance, g->IndexXAdvance_Size);
  myAllocPush(g->Glyphs, g->Glyphs_Size, rec_num + 10);
  //myAllocPush(g->IndexLookup, g->IndexLookup_Size, (1 << 16));
  //myAllocPush(g->IndexXAdvance, g->IndexXAdvance_Size, (1 << 16));
  {
    float fx = 1.f / width;
    float fy = 1.f / max_y;
    font_glyph_t* glyph = g->Glyphs;
    for (n = 0; n < (1 << 16); ++n) {
      g->IndexLookup[n] = (unsigned short) - 1;
      g->IndexXAdvance[n] = 0;
    }
    for (n = 0; n < rec_num; ++n) {
      i = rec[n].i;
      if (fo->getbit(fo, i, bit)) {
        int y = fo->bl - bit->y;
        font_bit_copy(bit, im->tt.data + rec[n].x * im->c + rec[n].y * im->s, im->s);
        glyph->Codepoint = i;
        glyph->X0 = (float)bit->x;
        glyph->Y0 = (float)y;
        glyph->X1 = (float)(bit->x + bit->bx);
        glyph->Y1 = (float)(y + bit->by);
        glyph->U0 = rec[n].x * fx;
        glyph->V0 = rec[n].y * fy;
        glyph->U1 = (rec[n].x + bit->bx) * fx;
        glyph->V1 = (rec[n].y + bit->by) * fy;
        g->IndexLookup[i] = n;
        glyph->XAdvance = g->IndexXAdvance[i] = (float)(i < 128 ? (fo->h / 2) : fo->h);
        ++glyph;
      }
    }
    i = ' ';
    g->IndexLookup[i] = glyph - g->Glyphs;
    glyph->XAdvance = g->IndexXAdvance[i] = fo->h / 2.f;
    ++glyph;
    i = '\t';
    g->IndexLookup[i] = glyph - g->Glyphs;
    glyph->XAdvance = g->IndexXAdvance[i] = (fo->h / 2.f) * 8.f;
    ++glyph;
    i = TextChar("　");
    g->IndexLookup[i] = glyph - g->Glyphs;
    glyph->XAdvance = g->IndexXAdvance[i] = (float)fo->h;
    ++glyph;
    g->Glyphs_Size = glyph - g->Glyphs;
  }
#if 0
#endif
  FREE(rec);
  return 0;
}
#if 0
const raster_font_t songti_12[] = {
#include "font/songti_12.txt"
};
int test_packing_raster_font()
{
  texfont_t g[1] = {0};
  img_t im[1] = {0};
  font_t fo[1] = {0};
  font_init_memraster(fo, "songti", songti_12);
  packing_raster_font(fo, g, im);
  imfree(im);
  texfont_free(g);
  return 0;
}
#endif
#endif // _FONT_INL_

