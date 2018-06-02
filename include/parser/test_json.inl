#include "cstd.h"
#include "str.h"
#include "cfile.h"
enum {JSON_NULL, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT};
//#include "json.inl"
typedef struct dstr_t {
  int d;
  str_t s[1];
} dstr_t;
typedef struct dstrv_t {
  int n;
  dstr_t* v;
} dstrv_t;
int dstrv_free(dstrv_t* sv)
{
  int i;
  if (sv->v) {
    for (i = 0; i < sv->n; ++i) {
      str_free(sv->v[i].s);
    }
    free(sv->v);
  }
  sv->v = NULL;
  sv->n = 0;
  return 0;
}
int dstrv_setsize(dstrv_t* sv, int n)
{
  int i;
  if (sv->v) {
    for (i = n; i < sv->n; ++i) {
      str_free(sv->v[i].s);
    }
  }
  if (sv->n<n) {
    MYREALLOC(sv->v, n);
    MEMSET(sv->v + sv->n, 0, n-sv->n);
  }
  sv->n = n;
  return 0;
}
#define dstrv_push_cstr(sv, s0, l0, d) dstrv_ins_cstr(sv, sv->n, s0, l0, d)
CC_INLINE int dstrv_ins_cstr(dstrv_t* sv, int i, const char* s0, int l0, int d)
{
  int n = sv->n;
  i = (i < 0 || i > n) ? n : i;
  //i = BOUND(i, 0, n);
  if (i >= n) {
    dstrv_setsize(sv, sv->n + 1);
  }
  MEMMOVE(sv->v + i + 1, sv->v + i, n - i);
  str_setstr(sv->v[i].s, s0, l0);
  sv->v[i].d = d;
  return 0;
}
int dstrv_have_sub(const dstrv_t* sv, int i)
{
  int n = 0, d = sv->v[i++].d;
  return (i < sv->n && sv->v[i].d > d);
}
int dstrv_savejson_stream(const dstrv_t* sv, stream_t* f)
{
  int i, j;
  for (i = 0; i < sv->n; ++i) {
    int has = dstrv_have_sub(sv, i);
    if (i > 0 && has) {
      stream_put8(f, '\n');
    }
    if (has) {
      for (j = 0; j < sv->v[i].d; ++j) {
        stream_put8(f, ' ');
      }
      stream_put8(f, '\"');
      stream_write(f, sv->v[i].s->s, sv->v[i].s->l);
      stream_put8(f, '\"');
    }
    else {
      stream_put8(f, ':');
      stream_put8(f, ' ');
      stream_put8(f, '\"');
      stream_write(f, sv->v[i].s->s, sv->v[i].s->l);
      stream_put8(f, '\"');
    }
  }
  return 0;
}
static int dstrv_print(const dstrv_t* sv)
{
#if 1
  int i, j;
  for (i = 0; i < sv->n; ++i) {
    for (j = 0; j < sv->v[i].d; ++j) {
      printf(" ");
    }
    printf("%s\n", sv->v[i].s->s);
  }
#else
  stream_t f[1] = {0};
  fstream_init(f, stdout);
  dstrv_savejson_stream(sv, f);
#endif
  return 0;
}
//val: str|obj
//obj: {(str : val),*}
#define SKIPSPACE1 for (; i < len && ((uchar)buf[i]) <= 32; ++i)
#define is0_9(c) ('0'<=(c) && '9'>=(c))
#define isa_z(c) ('a'<=(c) && 'z'>=(c))
#define isA_Z(c) ('A'<=(c) && 'Z'>=(c))
static int dstrv_parse_str(dstrv_t* sv, int d, const char* buf, int i, int len)
{
  int p0 = 0, p1 = 0;
  p0 = i;
  for (; i < len && '\"' != buf[i] && '\\' != buf[i - 1]; ++i);
  p1 = i++;
  dstrv_push_cstr(sv, buf + p0, p1 - p0, d);
  return i;
}
static int dstrv_parse_str2(dstrv_t* sv, int d, const char* buf, int i, int len)
{
  int p0 = 0, p1 = 0;
  p0 = i;
  for (; i < len && '\n' != buf[i] && ':' != buf[i] && '{' != buf[i] && '[' != buf[i] && '}' != buf[i] && ']' != buf[i] && ',' != buf[i]; ++i);
  p1 = i;
  dstrv_push_cstr(sv, buf + p0, p1 - p0, d);
  return i;
}
static int dstrv_parse_obj(dstrv_t* sv, int d, const char* buf, int i, int len);
static int dstrv_parse_val(dstrv_t* sv, int d, const char* buf, int i, int len, int j)
{
  char c;
  char str[16] = {0};
  int d1 = j < 0 ? d : (d + 1);
  SKIPSPACE1;
  c = buf[i];
  if ('\"' == c) {
    i = dstrv_parse_str(sv, d, buf, i + 1, len);
  }
  else if ('}' == c || ']' == c) {
    return i;
  }
  else if (',' != c && c > 32 && '{' != c && '[' != c) {
    i = dstrv_parse_str2(sv, d, buf, i, len);
  }
  else {
    if (j >= 0) {
      _snprintf(str, 16, "[%d]", j);
      dstrv_push_cstr(sv, str, strlen(str), d);
    }
    if ('[' == c) {
      i = dstrv_parse_obj(sv, d1, buf, i + 1, len);
      SKIPSPACE1;
      ASSERT(']' == buf[i]);
      ++i;
    }
    else if ('{' == c) {
      i = dstrv_parse_obj(sv, d1, buf, i + 1, len);
      SKIPSPACE1;
      ASSERT('}' == buf[i]);
      ++i;
    }
  }
  return i;
}
static int dstrv_parse_obj(dstrv_t* sv, int d, const char* buf, int i, int len)
{
  int j = 0;
  int p0 = 0, p1 = 0;
  for (; i < len; ++j) {
    SKIPSPACE1;
    i = dstrv_parse_val(sv, d, buf, i, len, j);
    SKIPSPACE1;
    if (':' == buf[i]) {
      i = dstrv_parse_val(sv, d + 1, buf, i + 1, len, -1);
    }
    SKIPSPACE1;
    if (',' == buf[i]) {
      ++i;
    }
    else {
      break;
    }
  }
  return i;
}
static int dstrv_setjson(dstrv_t* sv, const char* buf, int len)
{
  int i = dstrv_parse_val(sv, 0, buf, 0, len, -1);
  return i;
}
int dstrv_findname_1(dstrv_t* sv, int i, const char* name, int namelen)
{
  int d = sv->v[i].d;
  for (; i < sv->n && sv->v[i].d >= d; ++i) {
    str_t* s = sv->v[i].s;
    if (d == sv->v[i].d && namelen == s->l && *name == s->s[0] && 0 == cstr_nicmp(s->s, s->l, name, namelen, namelen, 1)) {
      return i;
    }
  }
  return -1;
}
int dstrv_findname(dstrv_t* sv, const char* name)
{
  int j = -1, namelen0;
  const char* pname = name;
  for (; pname && *pname;) {
    const char* name0 = pname;
    pname = strchr(pname, '|');
    if (pname) {
      namelen0 = pname - name0;
      ++pname;
    }
    else {
      namelen0 = strlen(name0);
    }
    j = dstrv_findname_1(sv, j + 1, name0, namelen0);
    if (j < 0) {
      return -1;
    }
  }
  return j;
}
//#include "var.inl"
int test_json()
{
  /* a bunch of json: */
  const char* text1 = "{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\": \"rect\", \n\"width\": 1920, \n\"height\": 1080, \n\"interlace\": false,\"frame rate\": 24\n}\n}";
  const char* text2 = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
  const char* text3 = "[\n [0, -1, 0],\n [1, 0, 0],\n [0, 0, 1]\n ]\n";
  const char* text4 = "{\n \"Image\": {\n \"Width\": 800,\n \"Height\": 600,\n \"Title\": \"View from 15th Floor\",\n \"Thumbnail\": {\n \"Url\": \"http:/*www.example.com/image/481989943\",\n \"Height\": 125,\n \"Width\": \"100\"\n },\n \"IDs\": [116, 943, 234, 38793]\n }\n }";
  const char* text5 = "[\n {\n \"precision\": \"zip\",\n \"Latitude\": 37.7668,\n \"Longitude\": -122.3959,\n \"Address\": \"\",\n \"City\": \"SAN FRANCISCO\",\n \"State\": \"CA\",\n \"Zip\": \"94107\",\n \"Country\": \"US\"\n },\n {\n \"precision\": \"zip\",\n \"Latitude\": 37.371991,\n \"Longitude\": -122.026020,\n \"Address\": \"\",\n \"City\": \"SUNNYVALE\",\n \"State\": \"CA\",\n \"Zip\": \"94085\",\n \"Country\": \"US\"\n }\n ]";
  vstr_t sv[1] = {0};
  str_t s[1] = {0};
  dstrv_t ds[1] = {0};
  int i, j = 0;
  sys_chdir("E:/pub/bin/json");
  if (1) {
    char* aaa[] = {
      "menu|id",
      "menu|popup|menuitem|[2]|value",
      "menu|popup|menuitem|[1]|onclick",
    };
    str_load("test2.json", s);
    dstrv_setjson(ds, s->s, s->l);
    dstrv_print(ds);
    for (i = 0; i < countof(aaa); ++i) {
      j = dstrv_findname(ds, aaa[i]);
      if (j >= 0 && j < ds->n - 1) {
        printf("%s : %s\n", aaa[i], ds->v[j + 1].s->s);
      }
    }
    printf("\n");
  }
  if (0) {
    vstr_load("list.txt", sv);
    for (i = 1; i < sv->n; ++i) {
      printf("<%s\n", sv->v[i].s);
      str_load(sv->v[i].s, s);
      dstrv_setjson(ds, s->s, s->l);
      dstrv_print(ds);
      printf(">\n");
      dstrv_free(ds);
    }
  }
  str_free(s);
  strv_free(sv);
  return 0;
}

