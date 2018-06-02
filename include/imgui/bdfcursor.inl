#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#ifdef _WIN32
#define strndup cstr_ndup
#endif // _WIN32
typedef struct {
  char* name;
  int id;
  char* bitmap;
  int hotx;
  int hoty;
} font_info_t;
typedef struct {
  char* name;
  int id;
  int width;
  int height;
  int hotx;
  int hoty;
  char* data;
} cursor_info_t;
typedef struct bdf_file_t {
  int dw, dh;
  int nfont;
  font_info_t* fonts;
} bdf_file_t;
static bool debug = FALSE;
#define HEX(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : (toupper(c) - 'A' + 10))
static int print_font(FILE* pf, bdf_file_t* bdf, font_info_t* fi)
{
  int x, y;
  fprintf(pf, "%s\n", fi->name);
  for (y = 0; y < bdf->dh; y++) {
    for (x = 0; x < bdf->dw; x++) {
      fprintf(pf, fi->bitmap[y * bdf->dw + x] ? "X" : " ");
    }
    fprintf(pf, "\n");
  }
  return 0;
}
static void print_cursor(cursor_info_t* ci)
{
  int x, y;
  for (y = 0; y < ci->height; y++) {
    printf("/* ");
    for (x = 0; x < ci->width; x++) {
      if (ci->hotx == x && ci->hoty == y) {
        printf("o");
      }
      else
        switch (ci->data[y * ci->width + x]) {
        case 0:
          printf(" ");
          break;
        case 1:
          printf(".");
          break;
        case 2:
          printf("X");
          break;
        }
    }
    printf(" */\n");
  }
}
static int read_bdf_font(bdf_file_t* bdf, char* fname)
{
  FILE* f;
  char line[2048];
  int rv = 0;
  bool startchar = FALSE, startbitmap = FALSE;
  char* charname, *p, *bitmap;
  int dx = 0, dy = 0;
  int w, h, x, y, py;
  int id, tmp;
  bdf->dw = 0;
  bdf->dh = 0;
  if (!(f = fopen(fname, "r"))) {
    perror(fname);
    return -1;
  }
  if (fgets(line, sizeof(line), f) && strncasecmp("STARTFONT ", line, 10)) {
    printf("!BDF font file\n");
    fclose(f);
    return -1;
  }
  p = line;
  while (fgets(line, sizeof(line), f)) {
    if (!startchar) {
      if (!strncasecmp("STARTCHAR ", line, 10)) {
        int ll;
        startchar = TRUE;
        ll = strcspn(p + 10, " \r\n");
        charname = strndup(p + 10, ll);
        charname[ll] = 0;
      }
      else if (!strncasecmp("FONTBOUNDINGBOX ", line, 16)) {
        sscanf(p + 16, "%d %d %d %d", &bdf->dw, &bdf->dh, &dx, &dy);
      }
    }
    else {
      if (!strncasecmp("ENDCHAR", line, 7)) {
        font_info_t* nfi;
        if (debug) {
          printf(" %*s*/\n", bdf->dw, "");
        }
        startchar = FALSE;
        startbitmap = FALSE;
        nfi = (font_info_t*)myAllocPush(bdf->fonts, bdf->nfont, 1);
        memset(nfi, '\0', sizeof(font_info_t));
        nfi->name = charname;
        nfi->id = id;
        nfi->bitmap = bitmap;
        nfi->hotx = 0 - dx;
        nfi->hoty = 0 - dy;
      }
      else if (startbitmap) {
        int px, cx;
        uchar mask;
        px = x - dx + py * bdf->dw;
        for (cx = 0; cx < w; cx++) {
          mask = 1 << (3 - (cx % 4));
          bitmap[px + cx] = (mask & HEX(line[cx / 4])) != 0;
          if (debug) {
            printf(bitmap[px + cx] ? "X" : " ");
          }
        }
        py++;
        if (debug) {
          printf(" %*s*/\n/* %*s", bdf->dw - w, "", bdf->dw + dx, "");
        }
      }
      else if (!strncasecmp("BBX ", line, 4)) {
        sscanf(p + 4, "%d %d %d %d", &w, &h, &x, &y);
        if (debug) {
          printf("/* %s: */\n/* %*s", charname, bdf->dw + dx, "");
        }
      }
      else if (!strncasecmp("ENCODING ", line, 9)) {
        if (sscanf(p + 9, "%d %d", &tmp, &id) != 2) {
          id = tmp;
        }
      }
      else if (!strncasecmp("BITMAP", line, 6)) {
        py = y - dy;
        startbitmap = TRUE;
        bitmap = (char*)malloc(bdf->dw * bdf->dh);
        memset(bitmap, '\0', bdf->dw * bdf->dh);
      }
    }
  }
  if (strncasecmp("ENDFONT", line, 7)) {
    rv = -1;
  }
  fclose(f);
  return rv;
}
static int gen_cursor(bdf_file_t* bdf, cursor_info_t* ci, font_info_t* bmap, font_info_t* mask)
{
  int bx = bdf->dw, by = bdf->dh, ex = 0, ey = 0;
  int i, j;
  for (j = 0; j < bdf->dh; j++) {
    bool havep = FALSE;
    for (i = 0; i < bdf->dw; i++) {
      if (bmap->bitmap[j * bdf->dw + i] || mask->bitmap[j * bdf->dw + i]) {
        havep = TRUE;
        bx = MIN(bx, i);
        ex = MAX(i + 1, ex);
      }
    }
    if (havep) {
      by = MIN(by, j);
      ey = MAX(ey, j + 1);
    }
  }
  ci->name = strdup(bmap->name);
  ci->id = bmap->id;
  ci->width = ex - bx;
  ci->height = ey - by;
  ci->hotx = bmap->hotx - bx;
  ci->hoty = ci->height - (bmap->hoty - by);
  ci->data = (char*)malloc(ci->width * ci->height);
  memset(ci->data, '\0', ci->width * ci->height);
  for (j = 0; j < ci->height; j++) {
    for (i = 0; i < ci->width; i++) {
      int ofs = (by + j) * bdf->dw + bx + i;
      ci->data[j * ci->width + i] = mask->bitmap[ofs] * (1 + bmap->bitmap[ofs]);
    }
  }
  return 0;
}
font_info_t* find_font(font_info_t* fonts, int n, const char* name)
{
  int i;
  for (i = 0; i < n; ++i) {
    font_info_t* fi = fonts + i;
    if (0 == strcmp(name, fi->name)) {
      return fi;
    }
  }
  return NULL;
}
static int compose_cursors_from_fonts(bdf_file_t* bdf, cursor_info_t* cursors)
{
  char name[256] = {0};
  int i, j = 0;
  for (i = 0; i < bdf->nfont; ++i) {
    font_info_t* fi = bdf->fonts + i;
    font_info_t* ml;
    strcpy(name, fi->name);
    strcat(name, "_mask");
    if (ml = find_font(bdf->fonts + i, bdf->nfont - i, name)) {
      cursor_info_t* ci = cursors + j++;
      gen_cursor(bdf, ci, fi, ml);
    }
  }
  return j;
}
static char* dump_cursor(cursor_info_t* ci)
{
  static char cdata[8192];
  char* p;
  int i;
  int c;
  bool flushed;
  sprintf(cdata, " { \"%s\", %d, %d, %d, %d, %d, \n \"",
      ci->name, ci->id, ci->width, ci->height, ci->hotx, ci->hoty);
  p = cdata + strlen(cdata);
  for (i = 0; i < ci->width * ci->height; i++) {
    flushed = FALSE;
    if (!(i % 4)) {
      c = 0;
    }
    c = c << 2;
    c += ci->data[i];
    if ((i % 4) == 3) {
      flushed = TRUE;
      sprintf(p, "\\%03o", c);
      p += strlen(p);
    }
    if (i > 0 && !(i % 64)) {
      strcpy(p , "\"\n \"");
      p += strlen(p);
    }
  }
  if (!flushed) {
    sprintf(p, "\\%03o", c);
    p += strlen(p);
  }
  strcpy(p, "\" }");
  return cdata;
}
static int dump_cursors(FILE* f, cursor_info_t* cursors, int ncursor)
{
  int i;
  fprintf(f, "static const struct { const char *name; int type; uchar width; uchar height; uchar hotx; uchar hoty; char *data; } cursors[] = {\n");
  for (i = 0; i < ncursor; ++i) {
    cursor_info_t* ci = cursors + i;
    if (debug) {
      print_cursor(ci);
    }
    fprintf(f, "%s, \n", dump_cursor(ci));
  }
  fprintf(f, " { NULL, 0, 0, 0, 0, 0, NULL },\n};\n");
  return 0;
}
static int print_set(bdf_file_t* bdf, font_info_t* fi)
{
  int x, y;
  for (y = 0; y < bdf->dh; y++) {
    for (x = 0; x < bdf->dw; x++) {
      fi->bitmap[y * bdf->dw + x] = fi->bitmap[y * bdf->dw + x] ? 255 : 0;
    }
  }
  return 0;
}
int set_cursor1(const char* name)
{
  int i;
#include "cursor.txt"
  for (i = 0; i < countof(cursors); ++i) {
    if (0 == strcmp(cursors[i].name, name)) {
      return sys_set_cursor(cursors[i].width, cursors[i].height, cursors[i].hotx, cursors[i].hoty, cursors[i].data);
    }
  }
  return 0;
}
#if 0
#include "img/imgio.inl"
int save_cursor_bmp(bdf_file_t* bdf)
{
  int i;
  char bmpname[256];
  sys_mkdir("cursor_out");
  for (i = 0; i < bdf->nfont; ++i) {
    font_info_t* fi = bdf->fonts + i;
    print_set(bdf, fi);
    _snprintf(bmpname, 256, "cursor_out/%s.bmp", fi->name);
    imwrite4(bmpname, bdf->dh, bdf->dw, (uchar*)fi->bitmap, bdf->dw, 1);
  }
  return 0;
}
#else
int save_cursor_bmp(bdf_file_t* bdf)
{
  UNUSED(bdf);
  return 0;
}
#endif
int test_bdfcursor()
{
  bdf_file_t bdf[1] = {0};
  cursor_info_t* cursors = NULL;
  int i, ncursor = 0;
  char* fn;
  fn = "cursor.bdf";
  sys_chdir("E:/code/cstd/include/imgui");
  debug = 0;
  if (read_bdf_font(bdf, fn)) {
    printf("Error reading font\n");
    return 1;
  }
  MYREALLOC(cursors, bdf->nfont);
  ncursor = compose_cursors_from_fonts(bdf, cursors);
  if (1) {
    FILE* f = stdout;
    f = fopen("cursor.txt", "wb");
    dump_cursors(f, cursors, ncursor);
    fclose(f);
  }
  if (1) {
    save_cursor_bmp(bdf);
  }
  if (0) {
    FILE* f = stdout;
    f = fopen("cursor_log.txt", "wb");
    for (i = 0; i < bdf->nfont; ++i) {
      font_info_t* fi = bdf->fonts + i;
      print_font(f, bdf, fi);
    }
    fclose(f);
  }
  for (i = 0; i < bdf->nfont; ++i) {
    font_info_t* fi = bdf->fonts + i;
    FREE(fi->name);
    FREE(fi->bitmap);
  }
  for (i = 0; i < ncursor; ++i) {
    FREE(cursors[i].name);
    FREE(cursors[i].data);
  }
  FREE(cursors);
  return 0;
}

