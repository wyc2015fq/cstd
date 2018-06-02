#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cstd.h"
//#include "sys\findfile.h"
static char* str2filename1(char* f, char* l)
{
  static char pp[] = "/\\<>?/:|*\"";
  char* p = f;
  for (; f != l; ++f) {
    if (strchr(pp, *f) || (*f > 0 && *f < 10)) {
      *f = '_';
    }
  }
  return p;
}
static char* str2filename2(char* f)
{
  char* l = f + strlen(f);
  str2filename1(f, l);
  SKIP_WHITESPACE(f, l);
  RSKIP_WHITESPACE(f, l);
  *l = '\0';
  return f;
}
static int test_ren1line_0(const char* txt)
{
  FILE* pf = fopen(txt, "rb");
  int i;
  char buf[ 256 ];
  char name[ 256 ];
  char* pn;
  if (pf) {
    const char* ext = strrchr(txt, '.');
    fgets(buf, 256, pf);
    fclose(pf);
    pn = str2filename2(buf);
    if (ext) {
      _snprintf(name, 256, "%s%s", pn, ext);
    }
    else {
      _snprintf(name, 256, "%s", pn);
    }
    if (strcmp(txt, name)) {
      for (i = 0; file_exist(name); ++i) {
        _snprintf(name, 256, "%s_%d%s", pn, i, ext);
      }
      rename(txt, name);
    }
  }
  return 0;
}
int test_ren1line(int argc, char** argv)
{
  const char* f = argc > 1 ? argv[ 1 ] : "*.txt";
  findinfo_t info[1] = {0};
  fileinfo_t fi[1] = {0};
#ifdef _DEBUG
  _chdir("E:/www/txt/mingyan.chazidian.com/htm/°®ÇéÓïÂ¼");
#endif
  for (; find_next_file(info, ".", "*", fi, 0);) {
    const char* fname = fi->name;
    test_ren1line_0(fname);
    printf("%s", fname);
    printf("\r\n");
  }
  return 0;
}

