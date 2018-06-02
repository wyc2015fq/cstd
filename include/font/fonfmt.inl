#include <assert.h>
#define get_byte(x) *(unsigned char*)(fon+x)
#define get_word(x) *(unsigned short*)(fon+x)
#define get_dword(x) *(unsigned long*)(fon+x)
#pragma pack(push)//保存对齐状态
#pragma pack(1)
typedef struct fonchar_t {
  unsigned short width;
  unsigned short height;
  const unsigned char* data;
} fonchar_t;
typedef struct fonfmt_t {
  unsigned short dfVersion;
  unsigned long dfSize;
  char dfCopyright[60];
  unsigned short dfType; //word(0x42)
  unsigned short dfPoints;// word(0x44);
  unsigned short dfVertRes;
  unsigned short dfHorizRes;
  unsigned short dfAscent;// word(0x4a);
  unsigned short dfInternalLeading;
  unsigned short dfExternalLeading;
  unsigned char dfItalic;// byte(0x50);
  unsigned char dfUnderline;// byte(0x51);
  unsigned char dfStrikeOut;// byte(0x52);
  unsigned short dfWeight;// word(0x53);
  unsigned char dfCharSet;// byte(0x55);
  unsigned short dfPixWidth;//
  unsigned short dfPixHeight;// word(0x58);
  unsigned char dfPitchAndFamily;//
  unsigned short dfAvgWidth;//
  unsigned short dfMaxWidth;//
  unsigned char dfFirstChar;//
  unsigned char dfLastChar;//
  unsigned char dfDefaultChar;//
  unsigned char dfBreakChar;//
  unsigned short dfWidthBytes;//
  unsigned long dfDevice;//
  unsigned long dfFace;//0x69
  unsigned long dfBitsPointer;//
  unsigned long dfBitsOffset;//
  unsigned char dfReserved;//
  unsigned long dfFlags;//
  unsigned short dfAspace;//
  unsigned short dfBspace;//
  unsigned short dfCspace;//
  unsigned long dfColorPointer;//
  char dfReserved1[16];
  char dfCharTable[16];
} fonfmt_t;
#pragma pack(pop)//恢复对齐状态
#define ASSERT_TYPEOFF1(_T, _N, _OFF) fprintf(pf, "%d %d", (int)(char*)&((_T)->_N)-(int)(char*)(_T),_OFF)
#define ASSERT_TYPEOFF(_T, _N, _OFF) ASSERT((int)(char*)&((_T)->_N)-(int)(char*)(_T)==_OFF)
#define facename(font) (((const char*)font)+font->dfFace)
#define DEVICENAME(font) (((const char*)font)+font->dfDevice)
int fon_getchar(const fonfmt_t* font, int ch, fonchar_t* fc)
{
  int ctstart, ctsize, maxwidth;
  const char* face = facename(font);
  // This font is a vector font
  if (font->dfType & 1) {
    return 0;
  }
  // Read the char table.
  if (font->dfVersion == 0x200) {
    ctstart = 0x76;
    ctsize = 4;
  }
  else {
    ctstart = 0x94;
    ctsize = 6;
  }
  maxwidth = 0;
  {
    int entry, off, w;
    const unsigned char* fon = (const unsigned char*)(font);
    entry = ctstart + ctsize * (ch - font->dfFirstChar);
    w = get_word(entry);
    fc->width = w;
    fc->height = font->dfPixHeight;
    if (ctsize == 4) {
      off = get_word(entry + 2);
    }
    else {
      off = get_dword(entry + 2);
    }
    fc->data = fon + off;
  }
  return 0;
}
int parse_ne(unsigned char* fon, int len, int neoff)
{
  // Find the resource table.
  int rtable = neoff + get_word(neoff + 0x24);
  // 32h: A shift count that is used to align the logical sector. This
  // count is log2 of the segment sector size. It is typically 4,
  // although the default count is 9.
  int shift = get_word(rtable);
  // Now loop over the rest of the resource table.
  int p = rtable + 2;
  while (1) {
    int i, count, rtype = get_word(p);
    // end of resource table
    if (rtype == 0) {
      break;
    }
    count = get_word(p + 2);
    // type, count, 4 bytes reserved
    p += 8;
    for (i = 0; i < count; i ++) {
      int start = get_word(p) << shift;
      int size = get_word(p + 2) << shift;
      if (start < 0 || size < 0 || start + size > len) {
        printf("Resource overruns file boundaries");
        return FALSE;
      }
      // this is an actual font
      if (rtype == 0x8008) {
        //parse_fnt(fon + start, size, ch, fc);
        return start;
        //echo "font start at start, size: size\n";
      }
      // start, size, flags, name/id, 4 bytes reserved
      p += 12;
    }
  }
  return 0;
}
int loadfon(unsigned char* fon, int len)
{
  fonfmt_t* ff = (fonfmt_t*)fon;
  int neoff;
  if (0 != memcmp(fon, "MZ", 2)) {
    printf("MZ signature not found!");
    return FALSE;
  }
  neoff = get_word(0x3c); // 标志位offset
  if (0 == memcmp(fon + neoff, "NE", 2)) {
    return parse_ne(fon, len, neoff);
  }
  else if (0 == memcmp(fon + neoff, "PE\0\0", 4)) {
    assert(0);
  }
  printf("NE or PE signature not found");
  return 0;
}
int print_allhz()
{
  int i, j;
  for (i = 0xb0; i < 0xf7; ++i) {
    for (j = 0xa1; j < 0xfe; ++j) {
      int ch = (i << 8) | (j & 0xff);
      char chr[3] = {i, j, 0};
      printf("%s\n", chr);
      //font_GetFont(hdc, 1, ch, glpm, buf, 1024);
    }
  }
  return 0;
}
int print_chrbit(FILE* pf, int h, int w, const uchar* bit, int widthbytes)
{
  int j, k;
  for (j = 0; j < h; ++j) {
    for (k = 0; k < w; k ++) {
      int k1 = (widthbytes * 8 - 1 - k) % 8;
      unsigned char v = bit[(k / 8) * h + j];
      fprintf(pf, (v & (1 << k1)) ? "█" : "□");
    }
    fprintf(pf, "\n");
  }
  return 0;
}
int print_chrbit2(FILE* pf, int h, int w, const uchar* bit0, int step)
{
  int j, k;
  for (j = 0; j < h; ++j) {
    const uchar* bit = bit0 + j * step;
    for (k = 0; k < w; k ++) {
      int k1 = (k) % 8;
      uchar v = bit[k / 8];
      int t = v & (1 << (7 - k1));
      if (pf) {
        fprintf(pf, t ? "█" : "□");
      }
      else {
        printf(t ? "█" : "□");
      }
    }
    if (pf) {
      fprintf(pf, "\n");
    }
    else {
      printf("\n");
    }
  }
  return 0;
}
int print_font(const fonfmt_t* font, FILE* pf)
{
  int i;
  ASSERT_TYPEOFF(font, dfType, 0x42);
  ASSERT_TYPEOFF(font, dfPoints, 0x44);
  ASSERT_TYPEOFF(font, dfAscent, 0x4a);
  ASSERT_TYPEOFF(font, dfItalic, 0x50);
  ASSERT_TYPEOFF(font, dfPixHeight, 0x58);
  ASSERT_TYPEOFF(font, dfFace, 0x69);
  ASSERT_TYPEOFF(font, dfFlags, 0x76);
  //ASSERT_TYPEOFF1(font, dfReserved1, 0x76);
  ASSERT_TYPEOFF(font, dfCharTable, 0x94);
  fprintf(pf, "%-10s = %s\n", "facename", facename(font));
#define PRINT_FONTFMT(name, fmt) fprintf(pf, "%-10s = %"#fmt"\n", #name, font->df##name)
  PRINT_FONTFMT(Copyright, s);
  PRINT_FONTFMT(PixHeight, d);
  PRINT_FONTFMT(Ascent, d);
  PRINT_FONTFMT(Points, d);
  PRINT_FONTFMT(Italic, d);
  PRINT_FONTFMT(Underline, d);
  PRINT_FONTFMT(StrikeOut, d);
  PRINT_FONTFMT(Weight, d);
  PRINT_FONTFMT(CharSet, d);
#undef PRINT_FONTFMT
  for (i = font->dfFirstChar; i <= font->dfLastChar; ++i) {
    fonchar_t fc[1] = {0};
    int widthbytes;
    fon_getchar(font, i, fc);
    widthbytes = ((fc->width + 7) / 8);
    fprintf(pf, "char = %c(%d) width = %d\n", i, i, fc->width);
    if (0 == fc->width) {
      continue;
    }
    print_chrbit(pf, fc->height, fc->width, fc->data, widthbytes);
    fprintf(pf, "\n");
  }
  return 0;
}

