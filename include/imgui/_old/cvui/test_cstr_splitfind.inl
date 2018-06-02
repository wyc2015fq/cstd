COLOR findcolor1(const char* s, int l)
{
#define HTML_COLOR_DEF(NAME, CLR) if (0==cstr_icmp(s, l, #NAME , -1, 1)) { return 0xFF##CLR; }
#include "html_color.txt"
#undef HTML_COLOR_DEF
  return -1;
}
COLOR findcolor2(const char* s, int l)
{
  static const char* sss = "|"
#define HTML_COLOR_DEF(NAME, CLR) #NAME"|"
#include "html_color.txt"
#undef HTML_COLOR_DEF
      ;
  static const COLOR ccc[] = {
#define HTML_COLOR_DEF(NAME, CLR) 0xFF##CLR,
#include "html_color.txt"
#undef HTML_COLOR_DEF
  };
  static int ssslen = strlen(sss);
  int k = cstr_splitfind(sss, ssslen, s, l, 1, NULL);
  if (k >= 0) {
    return ccc[k];
  }
  return -1;
}
COLOR findcolor3(const char* s, int l)
{
  typedef struct ss_t {
    char* s;
    int l;
    COLOR color;
  } ss_t;
  static ss_t ss[] = {
#define HTML_COLOR_DEF(NAME, CLR) {#NAME , strlen(#NAME), 0xFF##CLR} ,
#include "html_color.txt"
#undef HTML_COLOR_DEF
  };
  static int inited = 0;
  int i, j, t, n = countof(ss);
  if (!inited) {
#define STRLSS(a, b) (cstr_icmp((a).s, (a).l, (b).s, (b).l, 1)<0)
    QSORT(ss, ss + n, STRLSS, ss_t);
#undef STRLSS
    inited = 1;
  }
  i = 0;
  for (; i < n;) {
    j = (i + n) / 2;
    t = cstr_icmp(s, l, ss[j].s, ss[j].l, 1);
    if (t < 0) {
      n = j;
    }
    else if (t > 0) {
      i = j + 1;
    }
    else {
      return ss[j].color;
    }
  }
  return -1;
}
#include "str/strhash.inl"
COLOR findcolor4(const char* s, int l)
{
  typedef struct ss_t {
    char* s;
    int l;
    COLOR color;
    uint h;
  } ss_t;
  static ss_t ss[] = {
#define HTML_COLOR_DEF(NAME, CLR) {#NAME , strlen(#NAME), 0xFF##CLR, 0} ,
#include "html_color.txt"
#undef HTML_COLOR_DEF
  };
  static int inited = 0;
  int i, n = countof(ss), h = PJWHash(s, l);
  if (!inited) {
    for (i = 0; i < n; ++i) {
      ss[i].h = PJWHash(ss[i].s, ss[i].l);
    }
    inited = 1;
  }
  for (i = 0; i < n; ++i) {
    if (h == ss[i].h && l == ss[i].l && 0 == cstr_icmp(s, l, ss[i].s, ss[i].l, 1)) {
      return ss[i].color;
    }
  }
#undef STRLSS
  return -1;
}
int test_cstr_splitfind()
{
  int i, j;
  typedef struct ss_t {
    char* s;
    int l;
    COLOR color;
  } ss_t;
  static ss_t ss[] = {
#define HTML_COLOR_DEF(NAME, CLR) {#NAME , strlen(#NAME), 0xFF##CLR} ,
#include "html_color.txt"
#undef HTML_COLOR_DEF
  };
  int n = countof(ss);
#define TESTSPLITFIND(fun) {utime_start(_start_time); \
    for (i=0; i<100; ++i) { for (j=0; j<n; ++j) { \
        COLOR clr = fun(ss[j].s, ss[j].l); \
        ASSERT(clr==ss[j].color); } } \
    printf("%f\n", utime_elapsed(_start_time));}
  TESTSPLITFIND(findcolor1);
  TESTSPLITFIND(findcolor2);
  TESTSPLITFIND(findcolor3);
  TESTSPLITFIND(findcolor4);
#undef TESTSPLITFIND
  return 0;
}

