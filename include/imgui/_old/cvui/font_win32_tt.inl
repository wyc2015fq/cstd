enum glyph_rendering {
  glyph_gray1,
  glyph_gray8,
  glyph_outline,
};
// This code implements the AUTODIN II polynomial
// The variable corresponding to the macro argument "crc" should
// be an unsigned long.
// Oroginal code by Spencer Garrett <srg@quick.com>
//
// generated using the AUTODIN II polynomial
// x^32 + x^26 + x^23 + x^22 + x^16 +
// x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
static const unsigned crc32tab[256] = {
  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
  0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
  0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
  0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
  0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
  0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
  0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
  0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
  0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
  0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
  0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
  0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
  0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
  0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
  0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
  0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
  0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
  0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
  0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
  0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
  0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
  0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
  0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
  0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
  0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
  0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
  0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
  0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
  0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
  0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
  0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
  0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
  0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
  0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
  0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
  0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
  0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
  0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
  0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
  0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
  0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
  0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
  0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
  0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
  0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
  0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
  0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
  0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
  0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
  0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
  0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
  0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
  0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};
static unsigned calc_crc32(const unsigned char* buf, unsigned size)
{
  unsigned crc = (unsigned)~0;
  const unsigned char* p;
  unsigned len = 0;
  unsigned nr = size;
  for (len += nr, p = buf; nr--; ++p) {
    crc = (crc >> 8) ^ crc32tab[(crc ^ *p) & 0xff];
  }
  return ~crc;
}
#if 0
void decompose_win32_glyph_bitmap_mono(const char* gbuf, int w, int h, int x, int y, bool flip_y)
{
  int i;
  int pitch = ((w + 31) >> 5) << 2;
  const int8u* buf = (const int8u*)gbuf;
  sl.reset(x, x + w);
  storage.prepare();
  if (flip_y) {
    buf += pitch * (h - 1);
    y += h;
    pitch = -pitch;
  }
  for (i = 0; i < h; i++) {
    sl.reset_spans();
    bitset_iterator bits(buf, 0);
    int j;
    for (j = 0; j < w; j++) {
      if (bits.bit()) {
        sl.add_cell(x + j, cover_full);
      }
      ++bits;
    }
    buf += pitch;
    if (sl.num_spans()) {
      sl.finalize(y - i - 1);
      storage.render(sl);
    }
  }
}
void decompose_win32_glyph_bitmap_gray8(const char* gbuf,
    int w, int h,
    int x, int y,
    bool flip_y,
    Rasterizer& ras,
    Scanline& sl,
    ScanlineStorage& storage)
{
  int i, j;
  int pitch = ((w + 3) >> 2) << 2;
  const int8u* buf = (const int8u*)gbuf;
  sl.reset(x, x + w);
  storage.prepare();
  if (flip_y) {
    buf += pitch * (h - 1);
    y += h;
    pitch = -pitch;
  }
  for (i = 0; i < h; i++) {
    sl.reset_spans();
    const int8u* p = buf;
    for (j = 0; j < w; j++) {
      if (*p) {
        unsigned v = *p;
        if (v == 64) {
          v = 255;
        }
        else {
          v <<= 2;
        }
        sl.add_cell(x + j, ras.apply_gamma(v));
      }
      ++p;
    }
    buf += pitch;
    if (sl.num_spans()) {
      sl.finalize(y - i - 1);
      storage.render(sl);
    }
  }
}
#endif
CC_INLINE bool decompose_win32_glyph_outline(const char* gbuf, unsigned total_size, BOOL flip_y, const double* mtx, dpath_t* path)
{
  const char* cur_glyph = gbuf;
  const char* end_glyph = gbuf + total_size;
  double x, y;
  typedef double value_type;
  while (cur_glyph < end_glyph) {
    const TTPOLYGONHEADER* th = (TTPOLYGONHEADER*)cur_glyph;
    const char* end_poly = cur_glyph + th->cb;
    const char* cur_poly = cur_glyph + sizeof(TTPOLYGONHEADER);
    x = fx_to_dbl(th->pfxStart.x);
    y = fx_to_dbl(th->pfxStart.y);
    if (flip_y) {
      y = -y;
    }
    dpath_moveto(path, x, y, mtx);
    while (cur_poly < end_poly) {
      const TTPOLYCURVE* pc = (const TTPOLYCURVE*)cur_poly;
      if (pc->wType == TT_PRIM_LINE) {
        int i;
        for (i = 0; i < pc->cpfx; i++) {
          x = fx_to_dbl(pc->apfx[i].x);
          y = fx_to_dbl(pc->apfx[i].y);
          if (flip_y) {
            y = -y;
          }
          dpath_lineto(path, x, y, mtx);
        }
      }
      if (pc->wType == TT_PRIM_QSPLINE) {
        int u;
        for (u = 0; u < pc->cpfx - 1; u++) { // Walk through points in spline
          POINTFX pnt_b = pc->apfx[u]; // B is always the current point
          POINTFX pnt_c = pc->apfx[u + 1];
          if (u < pc->cpfx - 2) { // If not on last spline, compute C
            // midpoint (x,y)
            *(int*)&pnt_c.x = (*(int*)&pnt_b.x + * (int*)&pnt_c.x) / 2;
            *(int*)&pnt_c.y = (*(int*)&pnt_b.y + * (int*)&pnt_c.y) / 2;
          }
          double x2, y2;
          x = fx_to_dbl(pnt_b.x);
          y = fx_to_dbl(pnt_b.y);
          x2 = fx_to_dbl(pnt_c.x);
          y2 = fx_to_dbl(pnt_c.y);
          if (flip_y) {
            y = -y;
            y2 = -y2;
          }
          dpath_curve3(path, x, y, x2, y2, mtx);
        }
      }
      cur_poly += sizeof(WORD) * 2 + sizeof(POINTFX) * pc->cpfx;
    }
    cur_glyph += th->cb;
  }
  dpath_close(path);
  return true;
}
typedef struct glyph_t {
  int format;
  int m_glyph_index;
  dpath_t path[1];
  double m_advance_x;
  double m_advance_y;
} glyph_t;
typedef struct glyphbuf_t {
  GLYPHMETRICS gm[1];
  int format;
  int total_size;
  char gbuf[1];
} glyphbuf_t;
typedef struct fontinfo_t {
  char m_typeface[32];
  int m_height;
  int m_width;
  int m_weight;
  int m_resolution;
  int m_italic;
  int m_char_set;
  int m_pitch_and_family;
} fontinfo_t;
enum { tt_buf_size = 32768 - 32 };
typedef struct font_win32_tt_t {
  fontinfo_t m_fi[1];
  //HDC m_dc;
  HFONT m_font;
  uint m_crc;
  //KERNINGPAIR* m_kerning_pairs;
  //int m_max_kerning_pairs;
  int m_num_kerning_pairs;
  glyphbuf_t** m_glyphcache[256];
} font_win32_tt_t;
// 0 no
// 1 add
// 2 del
// 3 delall
int font_win32_tt_free(font_win32_tt_t* f)
{
  DeleteObject(f->m_font);
  f->m_font = NULL;
  return 0;
}
font_win32_tt_t* font_opt(const fontinfo_t* fi_find, int opt)
{
  static font_win32_tt_t* g_font = NULL;
  static int nfont = 0;
  font_win32_tt_t* ret = NULL;
  int i;
  if (3 == opt) {
    for (i = 0; i < nfont; ++i) {
      font_win32_tt_free(g_font + i);
    }
    FREE(g_font);
    g_font = NULL;
  }
  else if (fi_find) {
    uint crc = calc_crc32((unsigned char*)fi_find, sizeof(fontinfo_t));
    for (i = 0; i < nfont; ++i) {
      if (crc == g_font[i].m_crc && 0 == memcmp(g_font[i].m_fi, fi_find, sizeof(fontinfo_t))) {
        ret = g_font + i;
        break;
      }
    }
    if (NULL == ret) {
      if (1 == opt) {
        int h = fi_find->m_height;
        int w = fi_find->m_width;
        HFONT m_font;
        if (fi_find->m_resolution) {
          h = MulDiv(fi_find->m_height, fi_find->m_resolution, 72);
          w = MulDiv(fi_find->m_width, fi_find->m_resolution, 72);
        }
        m_font = CreateFontA(-h, // height of font
            w, // average character width
            0, // angle of escapement
            0, // base-line orientation angle
            fi_find->m_weight, // font weight
            fi_find->m_italic, // italic attribute option
            0, // underline attribute option
            0, // strikeout attribute option
            fi_find->m_char_set, // character set identifier
            OUT_DEFAULT_PRECIS, // output precision
            CLIP_DEFAULT_PRECIS, // clipping precision
            ANTIALIASED_QUALITY, // output quality
            fi_find->m_pitch_and_family, // pitch and family
            fi_find->m_typeface); // typeface name
        if (m_font) {
          MYREALLOC(g_font, nfont + 1);
          ret = g_font + nfont++;
          *ret->m_fi = *fi_find;
          ret->m_crc = crc;
          ret->m_font = m_font;
        }
      }
    }
    else {
      if (2 == opt) {
        i = ret - g_font;
        font_win32_tt_free(ret);
        MEMCPY(ret, ret + 1, nfont - i - 1);
        nfont--;
      }
    }
  }
  return ret;
}
font_win32_tt_t* font_tt_find(HDC dc, const char* str)
{
  int i, k, l = strlen(str);
  str_t ss[1] = {0};
  fontinfo_t fi[1] = {0};
  memset(fi->m_typeface, 0, sizeof(fi->m_typeface));
  fi->m_weight = (FW_REGULAR); // FW_BOLD
  fi->m_italic = (false);
  fi->m_char_set = (DEFAULT_CHARSET);
  fi->m_pitch_and_family = (FF_DONTCARE);
  for (; ' ' == str[l - 1] || '\t' == str[l - 1];) {
    --l;
  }
  for (i = 0; i < l;) {
    i = cstr_split2(str, i, l, " ", " ", ss);
    k = cstr_splitfind("|normal|italic|oblique|small-caps|bold|bolder|lighter|100|200|300|400|500|600|700|800|900|", -1, ss->s, ss->l, 1, NULL);
    switch (k) {
    case 1:
    case 2:
      fi->m_italic = true;
      break;
    case 4:
      fi->m_weight = FW_BOLD;
      break;
    default:
      if (k > 6) {
        fi->m_weight = atoi(ss->s);
      }
      else if (k < 0) {
        if (is0_9(ss->s[0])) {
          fi->m_height = atof(ss->s);
          //for (j = 0; is0_9(ss->s[j]) || '.' == ss->s[j]; ++j);
          //j = cstr_splitfind("|px|em|pt|inch|", -1, ss->s + j, ss->l - j, 1, NULL);
        }
        else {
          cstr_cpy(fi->m_typeface, 32, ss->s, ss->l);
        }
      }
      break;
    }
  }
  font_win32_tt_t* f = font_opt(fi, 1);
  if (f) {
    SelectObject(dc, f->m_font);
    f->m_num_kerning_pairs = 0;
  }
  return f;
}
glyphbuf_t* find_glyphcache(HDC dc, font_win32_tt_t* f, WORD glyph_code, int format)
{
  glyphbuf_t* gb = NULL;
  int hi = (glyph_code >> 8) & 0xff;
  int lo = (glyph_code) & 0xff;
  MAT2 m_matrix = {0};
  GLYPHMETRICS gm[1];
  //char m_gbuf[tt_buf_size];
  int total_size = 0;
  m_matrix.eM11.value = 1;
  m_matrix.eM22.value = 1;
  if (f->m_glyphcache[hi] && (gb = f->m_glyphcache[hi][lo])) {
    return gb;
  }
  if (NULL == f->m_glyphcache[hi]) {
    MYREALLOC(f->m_glyphcache[hi], 256);
  }
  total_size = GetGlyphOutlineW(dc, glyph_code, format, gm, 0, NULL, &m_matrix);
  if (total_size > 0) {
    myrealloc((void**) & (f->m_glyphcache[hi][lo]), sizeof(glyphbuf_t) + total_size, __FILE__, __LINE__);
    gb = f->m_glyphcache[hi][lo];
    gb->total_size = total_size;
    GetGlyphOutlineW(dc, glyph_code, format, gb->gm, total_size, (void*)gb->gbuf, &m_matrix);
  }
  return gb;
}
bool font_tt_glyph(HDC dc, font_win32_tt_t* f, int m_glyph_rendering, BOOL m_hinting, BOOL m_flip_y, unsigned glyph_code, const double* m_affine, glyph_t* ggo)
{
  glyphbuf_t* gb = NULL;
  int format = GGO_BITMAP;
  switch (m_glyph_rendering) {
  case glyph_gray1:
    format = GGO_BITMAP;
    break;
  case glyph_gray8:
    format = GGO_GRAY8_BITMAP;
    break;
  case glyph_outline:
    format = GGO_NATIVE;
    break;
  }
#ifndef GGO_UNHINTED // For compatibility with old SDKs.
#define GGO_UNHINTED 0x0100
#endif
  if (!m_hinting) {
    format |= GGO_UNHINTED;
  }
  if (!dc || !f->m_font) {
    return 0;
  }
  gb = find_glyphcache(dc, f, glyph_code, format);
  if (NULL == gb) {
    return 0;
  }
  ggo->m_glyph_index = glyph_code;
  ggo->m_advance_x = gb->gm->gmCellIncX;
  ggo->m_advance_y = -gb->gm->gmCellIncY;
  if (m_affine) {
    affine_transform_2x2(m_affine, &ggo->m_advance_x, &ggo->m_advance_y);
  }
  decompose_win32_glyph_outline(gb->gbuf, gb->total_size, m_flip_y, m_affine, ggo->path);
  return true;
}
#define quick_sort_threshold 9
typedef int (*Less)(const KERNINGPAIR* v1, const KERNINGPAIR* v2);
static int pair_less(const KERNINGPAIR* v1, const KERNINGPAIR* v2)
{
  if (v1->wFirst != v2->wFirst) {
    return v1->wFirst < v2->wFirst;
  }
  return v1->wSecond < v2->wSecond;
}
void quick_sort(KERNINGPAIR* arr, int arr_size, Less less)
{
  typedef KERNINGPAIR value_type;
  value_type t;
  value_type* e1;
  value_type* e2;
  int stack[80];
  int* top = stack;
  int limit = arr_size;
  int base = 0;
  if (arr_size < 2) {
    return;
  }
  for (;;) {
    int len = limit - base;
    int i;
    int j;
    int pivot;
    if (len > quick_sort_threshold) {
      // we use base + len/2 as the pivot
      pivot = base + len / 2;
      CV_SWAP(arr[base], arr[pivot], t);
      i = base + 1;
      j = limit - 1;
      // now ensure that *i <= *base <= *j
      e1 = &(arr[j]);
      e2 = &(arr[i]);
      if (less(e1, e2)) {
        CV_SWAP(*e1, *e2, t);
      }
      e1 = &(arr[base]);
      e2 = &(arr[i]);
      if (less(e1, e2)) {
        CV_SWAP(*e1, *e2, t);
      }
      e1 = &(arr[j]);
      e2 = &(arr[base]);
      if (less(e1, e2)) {
        CV_SWAP(*e1, *e2, t);
      }
      for (;;) {
        do {
          i++;
        }
        while (less((arr + i), (arr + base)));
        do {
          j--;
        }
        while (less((arr + base), (arr + j)));
        if (i > j) {
          break;
        }
        CV_SWAP(arr[i], arr[j], t);
      }
      CV_SWAP(arr[base], arr[j], t);
      // now, push the largest sub-array
      if (j - base > limit - i) {
        top[0] = base;
        top[1] = j;
        base = i;
      }
      else {
        top[0] = i;
        top[1] = limit;
        limit = j;
      }
      top += 2;
    }
    else {
      // the sub-array is small, perform insertion sort
      j = base;
      i = j + 1;
      for (; i < limit; j = i, i++) {
        for (; less((arr + j + 1), (arr + j)); j--) {
          CV_SWAP(*(arr + j + 1), *(arr + j), t);
          if (j == base) {
            break;
          }
        }
      }
      if (top > stack) {
        top -= 2;
        base = top[0];
        limit = top[1];
      }
      else {
        break;
      }
    }
  }
}
#if 0
void load_kerning_pairs(HDC m_dc, font_win32_tt_t* f)
{
  int i;
  if (m_dc) {
    if (f->m_kerning_pairs == 0) {
      MYREALLOC(f->m_kerning_pairs, 16384 - 16);
      f->m_max_kerning_pairs = 16384 - 16;
    }
    f->m_num_kerning_pairs = ::GetKerningPairs(m_dc, f->m_max_kerning_pairs, f->m_kerning_pairs);
    if (f->m_num_kerning_pairs) {
      // Check to see if the kerning pairs are sorted and
      // sort them if they are not.
      for (i = 1; i < f->m_num_kerning_pairs; ++i) {
        if (!pair_less(f->m_kerning_pairs + i - 1, f->m_kerning_pairs + i)) {
          quick_sort(f->m_kerning_pairs, f->m_num_kerning_pairs, pair_less);
          break;
        }
      }
    }
  }
}
bool add_kerning(unsigned first, unsigned second, double* x, double* y)
{
  if (m_dc && m_font) {
    if (m_num_kerning_pairs == 0) {
      load_kerning_pairs();
    }
    int end = m_num_kerning_pairs - 1;
    int beg = 0;
    KERNINGPAIR t;
    t.wFirst = (WORD)first;
    t.wSecond = (WORD)second;
    while (beg <= end) {
      int mid = (end + beg) / 2;
      if (m_kerning_pairs[mid].wFirst == t.wFirst &&
          m_kerning_pairs[mid].wSecond == t.wSecond) {
        double dx = m_kerning_pairs[mid].iKernAmount;
        double dy = 0.0;
        if (m_glyph_rendering == glyph_ren_outline ||
            m_glyph_rendering == glyph_ren_agg_mono ||
            m_glyph_rendering == glyph_ren_agg_gray8) {
          m_affine.transform_2x2(&dx, &dy);
        }
        *x += dx;
        *y += dy;
        return true;
      }
      else if (pair_less(t, m_kerning_pairs[mid])) {
        end = mid - 1;
      }
      else {
        beg = mid + 1;
      }
    }
    return false;
  }
  return false;
}
#endif

