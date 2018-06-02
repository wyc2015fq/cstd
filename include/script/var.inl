enum {
  _VT_NULL,
#define VARENUM_DEF(a, b, c, d) a,
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  _VT_LPSTR,
};
typedef struct var_t {
  int t;
  void* v;
  int locked;
} var_t;
int var_set_impl(var_t* v, int t, const void* s, int n, int m)
{
  ASSERT(v->locked == 0);
  v->t = t;
  if ((n + m) > 0) {
    v->v = realloc(v->v, n + m);
  }
  else {
    free(v->v);
    v->v = NULL;
  }
  if (s) {
    memcpy(v->v, s, n);
  }
  memset((char*)v->v + n, 0, m);
  return 0;
}
int CheckError(int x)
{
  return 0;
}
// ÅÅ³ıÌØÊâ×Ö·ûµÄ¿½±´
int strcpy_nop(char* p, const char* m_pstr, int len)
{
  char* p0 = p;
  const char* m_pstr_end = m_pstr + len;
  int olen;
  while (*m_pstr && m_pstr != m_pstr_end && *m_pstr != '\n') {
    if (*m_pstr == '\\') {
      switch (*++m_pstr) {
      case '"':
        *p++ = '\"';
        m_pstr++;
        break;
      case 'n':
        *p++ = '\n';
        m_pstr++;
        break;
      case 'r':
        *p++ = '\r';
        m_pstr++;
        break;
      case 't':
        *p++ = '\t';
        m_pstr++;
        break;
      default:
        *p++ = *m_pstr++;
        break;
      }
    }
    else {
      *p++ = *m_pstr++;
    }
  }
  olen = p0 - p;
  return olen;
}
char* strdup_nop(const char* m_pstr, int len)
{
  char* p = (char*)malloc(len + 1);
  strcpy_nop(p, m_pstr, len);
  return p;
}
#define var_set_null(v) var_set_impl(v, _VT_NULL, 0, 0, 0)
#define var_set_r4(v, x) { float xx=(float)x; var_set_impl(v, _VT_R4, &xx, 4, 0); }
#define var_set_r8(v, x) { double xx=(double)x; var_set_impl(v, _VT_R8, &xx, 8, 0); }
#define var_set_i4(v, x) { int xx=(int)x; var_set_impl(v, _VT_I4, &xx, 4, 0); }
#define var_set_ui4(v, x) { unsigned int xx=x; var_set_impl(v, _VT_UI4, &xx, 4, 0); }
#define var_set_lpstr(v, s) { var_set_impl(v, _VT_LPSTR, s, strlen(s), 1); }
int var_free(var_t* v)
{
  ASSERT(v->locked == 0);
  if (v->v) {
    FREE(v->v);
  }
  memset(v, 0, sizeof(var_t));
  return 0;
}
int var_copy(var_t* v, const var_t* src)
{
  ASSERT(v->locked == 0);
  if (src->v) {
    int n;
    v->t = src->t;
    n = my_msize(src->v);
    v->v = realloc(v->v, n);
    memcpy(v->v, src->v, n);
  }
  else {
    var_free(v);
    v->t = src->t;
  }
  return 0;
}
int var_tostr(const var_t* v, char* s, int n)
{
  switch (v->t) {
  case _VT_NULL:
    _snprintf(s, n, "%s", "null");
    break;
  case _VT_LPSTR:
    _snprintf(s, n, "%s", v->v);
    break;
#define VARENUM_DEF(a, b, c, d) case a: _snprintf(s, n, c, *(b*)(v->v)); break;
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  }
  return 0;
}
int var_set_fromstr(var_t* v, int t, const char* s)
{
  switch (t) {
  case _VT_NULL:
    var_set_impl(v, t, 0, 0, 0);
    break;
#define VARENUM_DEF(a, b, c, d) case a: {var_set_impl(v, t, 0, sizeof(b), 0); *(b*)(v->v) = (b)d;} break;
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  case _VT_LPSTR:
    var_set_lpstr(v, s);
    break;
  default:
    ASSERT(0);
    break;
  }
  return 0;
}
int var_change_type(const var_t* v, int t, var_t* v2)
{
  char buf[1024];
  var_tostr(v, buf, 1024);
  var_set_fromstr(v2, t, buf);
  return 0;
}
int var_toint(const var_t* v)
{
  int x = 0;
  switch (v->t) {
  case _VT_NULL:
    x = 0;
    break;
#define VARENUM_DEF(a, b, c, d) case a: x = (int)(*(b*)(v->v)); break;
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  case _VT_LPSTR:
    x = atoi((char*)(v->v));
    break;
  default:
    ASSERT(0);
    break;
  }
  return x;
}
double var_todouble(const var_t* v)
{
  double x = 0;
  switch (v->t) {
  case _VT_NULL:
    x = 0;
    break;
#define VARENUM_DEF(a, b, c, d) case a: x = (double)(*(b*)(v->v)); break;
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  case _VT_LPSTR:
    x = atof((char*)(v->v));
    break;
  default:
    ASSERT(0);
    break;
  }
  return x;
}
int var_sub(const var_t* a1, const var_t* a2, var_t* v)
{
  int t = MAX(a1->t, a2->t);
  var_t v1[1] = {0};
  var_t v2[1] = {0};
  var_change_type(a1, v->t, v1);
  var_change_type(a2, v->t, v2);
  switch (t) {
  case _VT_NULL:
    var_set_impl(v, t, 0, 0, 0);
    break;
#define VARENUM_DEF(a, b, c, d) case a: {var_set_impl(v, t, 0, sizeof(b), 0); *(b*)(v->v) = (*(b*)(v1->v)) - (*(b*)(v2->v)); } break;
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  case _VT_LPSTR:
    ASSERT(0);
    break;
  default:
    ASSERT(0);
    break;
  }
  var_free(v1);
  var_free(v2);
  return 0;
}
int var_add(const var_t* a1, const var_t* a2, var_t* v)
{
  int t = MAX(a1->t, a2->t);
  var_t v1[1] = {0};
  var_t v2[1] = {0};
  var_change_type(a1, t, v1);
  var_change_type(a2, t, v2);
  switch (t) {
  case _VT_NULL:
    var_set_impl(v, t, 0, 0, 0);
    break;
#define VARENUM_DEF(a, b, c, d) case a: {var_set_impl(v, t, 0, sizeof(b), 0); *(b*)(v->v) = (*(b*)(v1->v)) + (*(b*)(v2->v)); } break;
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  case _VT_LPSTR: {
    int l1 = strlen((char*)v1->v), l2 = strlen((char*)v2->v);
    var_set_impl(v, t, 0, l1 + l2, 1);
    memcpy((char*)v->v, (char*)v1->v, l1);
    memcpy((char*)v->v + l1, (char*)v2->v, l2);
  }
  break;
  default:
    ASSERT(0);
    break;
  }
  var_free(v1);
  var_free(v2);
  return 0;
}
int var_mul(const var_t* a1, const var_t* a2, var_t* v)
{
  int t = MAX(a1->t, a2->t);
  var_t v1[1] = {0};
  var_t v2[1] = {0};
  var_change_type(a1, v->t, v1);
  var_change_type(a2, v->t, v2);
  switch (t) {
  case _VT_NULL:
    var_set_impl(v, t, 0, 0, 0);
    break;
#define VARENUM_DEF(a, b, c, d) case a: {var_set_impl(v, t, 0, sizeof(b), 0); *(b*)(v->v) = (*(b*)(v1->v)) * (*(b*)(v2->v)); } break;
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  case _VT_LPSTR:
    ASSERT(0);
    break;
  default:
    ASSERT(0);
    break;
  }
  var_free(v1);
  var_free(v2);
  return 0;
}
int var_div(const var_t* a1, const var_t* a2, var_t* v)
{
  int t = MAX(a1->t, a2->t);
  var_t v1[1] = {0};
  var_t v2[1] = {0};
  var_change_type(a1, v->t, v1);
  var_change_type(a2, v->t, v2);
  switch (t) {
  case _VT_NULL:
    var_set_impl(v, t, 0, 0, 0);
    break;
#define VARENUM_DEF(a, b, c, d) case a: {var_set_impl(v, t, 0, sizeof(b), 0); *(b*)(v->v) = (*(b*)(v1->v)) / (*(b*)(v2->v)); } break;
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  case _VT_LPSTR:
    ASSERT(0);
    break;
  default:
    ASSERT(0);
    break;
  }
  var_free(v1);
  var_free(v2);
  return 0;
}
int var_mod(const var_t* a1, const var_t* a2, var_t* v)
{
  int x1 = var_toint(a1);
  int x2 = var_toint(a2);
  int x = x1%x2;
  var_set_i4(v, x);
  return 0;
}
int var_cmp(const var_t* a1, const var_t* a2)
{
  int t = MAX(a1->t, a2->t), x;
  var_t v1[1] = {0};
  var_t v2[1] = {0};
  var_change_type(a1, t, v1);
  var_change_type(a2, t, v2);
  switch (t) {
  case _VT_NULL:
    x = 1;
    break;
#define VARENUM_DEF(a, b, c, d) case a: {x = (*(b*)(v1->v)) < (*(b*)(v2->v)) ? -1 : (*(b*)(v1->v)) > (*(b*)(v2->v)) ? 1 : 0; } break;
#include "VARENUM_DEF.inl"
#undef VARENUM_DEF
  case _VT_LPSTR:
    x = strcmp((char*)v1->v, (char*)v2->v);
    break;
  default:
    ASSERT(0);
    break;
  }
  var_free(v1);
  var_free(v2);
  return x;
}
int var_neg(const var_t* a1, var_t* v)
{
  var_t v1[1] = {0};
  var_set_i4(v1, 0);
  var_sub(v1, a1, v);
  var_free(v1);
  return 0;
}
int var_not(const var_t* v1, var_t* v)
{
  int x = var_toint(v1);
  var_set_i4(v, !x);
  return 0;
}
int var_and(const var_t* a1, const var_t* a2, var_t* v)
{
  int x1 = var_toint(a1);
  int x2 = var_toint(a2);
  int x = x1 && x2;
  var_set_i4(v, x);
  return 0;
}
int var_or(const var_t* a1, const var_t* a2, var_t* v)
{
  int x1 = var_toint(a1);
  int x2 = var_toint(a2);
  int x = x1 || x2;
  var_set_i4(v, x);
  return 0;
}
int var_xor(const var_t* a1, const var_t* a2, var_t* v)
{
  int x1 = var_toint(a1);
  int x2 = var_toint(a2);
  int x = x1 ^ x2;
  var_set_i4(v, x);
  return 0;
}

