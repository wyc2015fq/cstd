typedef struct obj_t obj_t;
typedef struct var_t var_t;
typedef struct arr_t arr_t;
#define OBJ_ARRAY (1<<31)
#define OBJ_MASK 0xFFFF
typedef enum { OBJ_NULL, OBJ_STR, OBJ_INT1, OBJ_INT4, OBJ_FLOAT4, OBJ_FLOAT8, OBJ_VAR, OBJ_OBJ } obj_type;
#include "canvas.inl"
// struct userclass_t {} userclass_t;
#if 0
struct arr_t {
  unsigned int n;
  void* v;
};
struct obj_t {
  unsigned int t;
  unsigned int v[2];//int;double;obj_t*;
};
#else
struct arr_t {
  unsigned int n;
  union {
    //char* s;
    int* i;
    double* d;
    obj_t* o;
    var_t* va;
    void* v;
  };
};
struct obj_t {
  obj_type t;
  union {
    unsigned int v[2];
    double d;
    int i;
    arr_t arr[1];
  };
};
#endif
typedef struct var_t {
  char* name;
  obj_t o[1];
} var_t;
int obj_subs(obj_t* o)
{
  int isarr = OBJ_ARRAY & o->t;
  int t = OBJ_MASK & o->t;
  arr_t* v = (arr_t*)o->v;
  if (OBJ_VAR == t || OBJ_OBJ == t || isarr) {
    v->n;
  }
  return 0;
}
int obj_depth(obj_t* o)
{
  int isarr = OBJ_ARRAY & o->t;
  int i, t = OBJ_MASK & o->t;
  arr_t* v = (arr_t*)o->v;
  int d = 0;
  if (OBJ_VAR == t) {
    var_t* s = (var_t*)v->v;
    d = 1;
    for (i = 0; i < v->n; ++i) {
      int d1 = d + obj_depth(s[i].o);
      d = MAX(d, d1);
    }
  }
  else if (OBJ_OBJ == t) {
    obj_t* s = (obj_t*)v->v;
    ++d;
    for (i = 0; i < v->n; ++i) {
      int d1 = d + obj_depth(s + i);
      d = MAX(d, d1);
    }
  }
  else if (isarr) {
    d = 1;
  }
  return d;
}
int printf_nextline(int i, int n, int d, int d1)
{
  int j;
  printf(i < (n - 1) ? "," : "");
  if (d > 2) {
    printf("\n");
    d1 -= (i == n - 1);
    for (j = 0; j < d1; ++j) {
      printf(" ");
    }
  }
  return 0;
}
int obj_print_impl(obj_t* o, int d1)
{
  int isarr = OBJ_ARRAY & o->t;
  int i, t = OBJ_MASK & o->t;
  int d2 = d1 + 1;
  arr_t* v = (arr_t*)o->v;
  int n = obj_subs(o);
  int d = obj_depth(o);
  const char* lb = "";
  const char* rb = "";
  if (OBJ_VAR == t) {
    lb = "{";
    rb = "}";
  }
  else if (OBJ_OBJ == t || isarr) {
    lb = "[";
    rb = "]";
  }
  printf(lb);
  printf_nextline(0, 0, d, d1);
  switch (t) {
  case OBJ_VAR: {
    var_t* s = (var_t*)v->v;
    for (i = 0; i < v->n; ++i) {
      printf("\"%s\":", s[i].name);
      obj_print_impl(s[i].o, d2);
      printf_nextline(i, v->n, d, d1);
    }
  }
  break;
  case OBJ_OBJ: {
    obj_t* s = (obj_t*)v->v;
    for (i = 0; i < v->n; ++i) {
      obj_print_impl(s + i, d2);
      printf_nextline(i, v->n, d, d1);
    }
  }
  break;
  case OBJ_STR:
    if (isarr) {
      str_t* s = (str_t*)v->v;
      for (i = 0; i < v->n; ++i) {
        printf("\"%s\"", s[i].s);
        printf_nextline(i, v->n, d, d1);
      }
    }
    else {
      printf("\"%s\"", ((str_t*)o->v)->s);
    }
    break;
  default: {
    switch (t) {
    case OBJ_INT4:
      if (isarr) {
        int* s = (int*)v->v;
        for (i = 0; i < v->n; ++i) {
          printf("%d", s[i]);
          printf_nextline(i, v->n, d, d1);
        }
      }
      else {
        printf("%d", *(int*)o->v);
      }
      break;
    case OBJ_FLOAT8:
      if (isarr) {
        double* s = (double*)v->v;
        for (i = 0; i < v->n; ++i) {
          printf("%g", s[i]);
          printf_nextline(i, v->n, d, d1);
        }
      }
      else {
        printf("%g", *(double*)o->v);
      }
      break;
    default:
      break;
    }
  }
  break;
  }
  printf(rb);
  return 0;
}
int obj_print(obj_t* o)
{
  obj_print_impl(o, 1);
  return 0;
}
int obj_free(obj_t* o)
{
  int isarr = OBJ_ARRAY & o->t;
  int i, t = OBJ_MASK & o->t;
  arr_t* v = (arr_t*)o->v;
  switch (t) {
  case OBJ_VAR: {
    var_t* s = (var_t*)v->v;
    for (i = 0; i < v->n; ++i) {
      FREE(s[i].name);
      obj_free(s[i].o);
    }
    FREE(v->v);
  }
  break;
  case OBJ_OBJ: {
    obj_t* s = (obj_t*)v->v;
    for (i = 0; i < v->n; ++i) {
      obj_free(s + i);
    }
    FREE(v->v);
  }
  break;
  case OBJ_STR:
    if (isarr) {
      str_t* s = (str_t*)v->v;
      for (i = 0; i < v->n; ++i) {
        str_free(s + i);
      }
      FREE(v->v);
      v->v = NULL;
      v->n = 0;
    }
    else {
      str_free((str_t*)o->v);
    }
    break;
  default:
    if (isarr) {
      FREE(v->v);
    }
    break;
  }
  memset(o, 0, sizeof(obj_t));
  return 0;
}
int cstr_setstr(char** pstr, const char* name, int namelen)
{
  char* str = *pstr;
  MYREALLOC(str, namelen + 1);
  memcpy(str, name, namelen);
  str[namelen] = 0;
  *pstr = str;
  return 0;
}
static int parse_str1(const char* buf, int i, int len, int* p)
{
  int p0 = 0, p1 = 0;
  p0 = i;
  for (; i < len && '\"' != buf[i] && '\\' != buf[i - 1]; ++i);
  p1 = i++;
  p[0] = p0;
  p[1] = p1;
  return i;
}
static int parse_str2(const char* buf, int i, int len, int* p)
{
  int p0 = 0, p1 = 0;
  p0 = i;
  for (; i < len && '\n' != buf[i] && ':' != buf[i] && '{' != buf[i] && '[' != buf[i] && '}' != buf[i] && ']' != buf[i] && ',' != buf[i]; ++i) {
  }
  p1 = i;
  p[0] = p0;
  p[1] = p1;
  return i;
}
arr_t* obj_setarr(obj_t* o, int t, int n, int elemsize)
{
  arr_t* v = (arr_t*)o->v;
  o->t = OBJ_ARRAY | t;
  v->n = n;
  myrealloc(&v->v, n * elemsize, __FILE__, __LINE__);
  return v;
}
str_t* obj_setstr(obj_t* o, const char* str, int len)
{
  str_t* s = (str_t*)o->v;
  o->t = OBJ_STR;
  str_setstr(s, str, len);
  return s;
}
double obj_setnum(obj_t* o, const char* str, int len)
{
  double d = atof(str);
  int i = (int)d;
  if (d == (double)i) {
    o->t = OBJ_INT4;
    *(int*)o->v = i;
  }
  else {
    o->t = OBJ_FLOAT8;
    *(double*)o->v = d;
  }
  return d;
}
int debug()
{
  int asdf = 0;
  return 0;
}
#define CHECKERROR(OP) if ((OP)<0) {debug();return -i;}
static int parse_obj(const char* buf, int i, int len, obj_t* o);
static int parse_var(const char* buf, int i, int len, var_t* va)
{
  char c;
  int p[2];
  c = buf[i];
  if ('\"' == c) {
    CHECKERROR(i = parse_str1(buf, i + 1, len, p));
  }
  else if ('}' == c || ']' == c) {
    return i;
  }
  else if (',' != c && c > 32 && '{' != c && '[' != c) {
    CHECKERROR(i = parse_str2(buf, i, len, p));
  }
  SKIPSPACE1;
  if (':' == buf[i]) {
    cstr_setstr(&va->name, buf + p[0], p[1] - p[0]);
    ++i;
    SKIPSPACE1;
    c = buf[i];
  }
  i = parse_obj(buf, i, len, va->o);
  return i;
}
static int parse_vararr(const char* buf, int i, int len, var_t** po, int* pn)
{
  int i1, j = 0;
  var_t* o = *po;
  for (; i < len;) {
    SKIPSPACE1;
    i1 = i;
    MYREALLOC(o, j + 1);
    CHECKERROR(i = parse_var(buf, i, len, o + j));
    if (i > i1) {
      ++j;
    }
    SKIPSPACE1;
    if (',' == buf[i]) {
      ++i;
    }
    else {
      break;
    }
  }
  *po = o;
  *pn = j;
  return i;
}
static int parse_objarr(const char* buf, int i, int len, obj_t** po, int* pn)
{
  int i1, j = 0;
  obj_t* o = *po;
  for (; i < len;) {
    SKIPSPACE1;
    i1 = i;
    MYREALLOC(o, j + 1);
    CHECKERROR(i = parse_obj(buf, i, len, o + j));
    if (i > i1) {
      ++j;
    }
    SKIPSPACE1;
    if (',' == buf[i]) {
      ++i;
    }
    else {
      break;
    }
  }
  *po = o;
  *pn = j;
  return i;
}
static int parse_obj(const char* buf, int i, int len, obj_t* o)
{
  char c;
  int p[2];
  c = buf[i];
  if ('\"' == c) {
    CHECKERROR(i = parse_str1(buf, i + 1, len, p));
    obj_setstr(o, buf + p[0], p[1] - p[0]);
  }
  else if ('}' == c || ']' == c) {
    return i;
  }
  else if (',' != c && c > 32 && '{' != c && '[' != c) {
    CHECKERROR(i = parse_str2(buf, i, len, p));
    if ('.' == c || is0_9(c)) {
      obj_setnum(o, buf + p[0], p[1] - p[0]);
    }
    else {
      obj_setstr(o, buf + p[0], p[1] - p[0]);
    }
  }
  else if ('[' == c) {
    arr_t* v = (arr_t*)o->v;
    CHECKERROR(i = parse_objarr(buf, i + 1, len, (obj_t**)&v->v, &v->n));
    o->t = OBJ_OBJ;
    SKIPSPACE1;
    if (']' != buf[i]) {
      return -i;
    }
    ++i;
  }
  else if ('{' == c) {
    arr_t* v = (arr_t*)o->v;
    CHECKERROR(i = parse_vararr(buf, i + 1, len, (var_t**)&v->v, &v->n));
    o->t = OBJ_VAR;
    SKIPSPACE1;
    if ('}' != buf[i]) {
      return -i;
    }
    ++i;
  }
  else {
    return -i;
  }
  return i;
}
int obj_setjson(obj_t* o, const char* buf, int len)
{
  int i = parse_obj(buf, 0, len, o);
  return i;
}
obj_t* obj_getsubobj_1(obj_t* o, int i)
{
  int isarr = OBJ_ARRAY & o->t;
  int t = OBJ_MASK & o->t;
  arr_t* v = (arr_t*)o->v;
  if (OBJ_OBJ == t) {
    obj_t* s = (obj_t*)v->v;
    return s + i;
  }
  return NULL;
}
var_t* obj_getsubvar_1(obj_t* o, const char* name, int namelen)
{
  int isarr = OBJ_ARRAY & o->t;
  int i, t = OBJ_MASK & o->t;
  arr_t* v = (arr_t*)o->v;
  if (OBJ_VAR == t) {
    var_t* s = (var_t*)v->v;
    for (i = 0; i < v->n; ++i) {
      int l = strlen(s[i].name);
      if (namelen == l && *name == s[i].name[0] && 0 == cstr_nicmp(s[i].name, l, name, namelen, namelen, 1)) {
        return s + i;
      }
    }
  }
  return NULL;
}
obj_t* obj_getsubvar(obj_t* o, const char* name)
{
  int namelen0;
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
    if ('[' == name0[0] && ']' == name0[namelen0 - 1]) {
      int id = atoi(name0 + 1);
      o = obj_getsubobj_1(o, id);
    }
    else {
      var_t* va = obj_getsubvar_1(o, name0, namelen0);
      if (NULL == va) {
        return NULL;
      }
      o = va->o;
    }
  }
  return o;
}
var_t* obj_findsubvar(obj_t* o, const char* name)
{
  return 0;
}
//#include "test/test_memcheck.inl"
int test_var()
{
  /* a bunch of json: */
  const char* text1 = "{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\": \"rect\", \n\"width\": 1920, \n\"height\": 1080, \n\"interlace\": false,\"frame rate\": 24\n}\n}";
  const char* text2 = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
  const char* text3 = "[\n [0, -1, 0],\n [1, 0, 0],\n [0, 0, 1]\n ]\n";
  const char* text4 = "{\n \"Image\": {\n \"Width\": 800,\n \"Height\": 600,\n \"Title\": \"View from 15th Floor\",\n \"Thumbnail\": {\n \"Url\": \"http:/*www.example.com/image/481989943\",\n \"Height\": 125,\n \"Width\": \"100\"\n },\n \"IDs\": [116, 943, 234, 38793]\n }\n }";
  const char* text5 = "[\n {\n \"precision\": \"zip\",\n \"Latitude\": 37.7668,\n \"Longitude\": -122.3959,\n \"Address\": \"\",\n \"City\": \"SAN FRANCISCO\",\n \"State\": \"CA\",\n \"Zip\": \"94107\",\n \"Country\": \"US\"\n },\n {\n \"precision\": \"zip\",\n \"Latitude\": 37.371991,\n \"Longitude\": -122.026020,\n \"Address\": \"\",\n \"City\": \"SUNNYVALE\",\n \"State\": \"CA\",\n \"Zip\": \"94085\",\n \"Country\": \"US\"\n }\n ]";
  vstr_t sv[1] = {0};
  str_t s[1] = {0};
  obj_t o[1] = {0};
  int i, j = 0;
  _chdir("E:/pub/bin/json");
  //test_memcheck();
  if (1) {
    char* aaa[] = {
      "menu|id",
      "menu|popup|menuitem|[0]|onclick",
    };
    str_load("test0.json", s);
    {
      MEM_CHECK_BEGIN();
      obj_setjson(o, s->s, s->l);
      obj_free(o);
      MEM_CHECK_END();
    }
    obj_setjson(o, s->s, s->l);
    obj_print(o);
    printf("\n");
    for (i = 0; i < countof(aaa); ++i) {
      obj_t* o1 = obj_getsubvar(o, aaa[i]);
      if (o1) {
        printf("<%s> : ", aaa[i]);
        obj_print(o1);
        printf("\n");
      }
    }
    printf("\n");
  }
  obj_free(o);
  str_free(s);
  strv_free(sv);
  return 0;
}

