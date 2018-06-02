#ifndef _VAR_INL_
#define _VAR_INL_
#include <stdarg.h>
#define myitoa(a, b)  itoa(a, b, 10)
#define myi64toa(a, b)  _i64toa(a, b, 10)
#define myftoa(a, b)  d2str(b, 100, a, 10, 10, 6, 0)
CC_INLINE int var2str(int n, const void* v1, int t1, char* v2, void** pv1, void** pv2)
{
  ASSERT(1 == n);

  switch (t1) {
#define VARTYPEDEF(a, b, c, d, e)   case b: e(*(c*)v1, v2); if (pv1) {*pv1 = (c*)v1+1;} if (pv2) {*pv2 = v2+strlen(v2);} break;
#include "vartypedef.txt"
#undef VARTYPEDEF
  }

  return 0;
}
CC_INLINE int str2var(int n, const char* v1, void* v2, int t2, void** pv1, void** pv2)
{
  int i = 0, l = strlen(v1), j = 0;

  switch (t2) {
#define VARTYPEDEF(a, b, c, d, e)   case b: for (j=0; j<n; ++j) { ((c*)v2)[j] = (c)d(v1, l, i, &i); } if (pv1) {*pv1 = (char*)v1+i;} if (pv2) {*pv2 = (c*)v2+n;} break;
#include "vartypedef.txt"
#undef VARTYPEDEF
  }

  return 0;
}
CC_INLINE int var2var(int n, const void* v1, int t1, void* v2, int t2, void** pv1, void** pv2)
{
  static uchar map[128] = {0};
  static init = 0;
  int tt, i;

  if (!init) {
    init = 1;
#define VARTYPEDEF(a, b, c, d, e)   map[b] = a+1;
#include "vartypedef.txt"
#undef VARTYPEDEF
  }

  if ('s' == t1 && 's' == t2) {
    int l = strlen((const char*)v1);

    if (v2) {
      strcpy((char*)v2, (const char*)v1);
    }

    *pv1 = (char*)v1 + l;
    *pv2 = (char*)v2 + l;
    return 0;
  }
  else if ('s' == t2) {
    return var2str(n, v1, t2, (char*)v2, pv1, pv2);
  }
  else if ('s' == t1) {
    return str2var(n, (const char*)v1, v2, t2, pv1, pv2);
  }

  tt = map[t1] * 10 + map[t2];

  switch (tt) {
#define VARCONFDEF(a, b, c, d, e)  case a: { for (i=0; i<n; ++i) { ((e*)v2)[i] = (e)(((c*)v1)[i]); } if (pv1) {*pv1 = ((c*)v1 + n);} if (pv2) {*pv2 = ((e*)v2 + n);} }break;
#include "varconvdef.txt"
#undef VARCONFDEF

  default:
    ASSERT(0);
    break;
  }

  return 0;
}
CC_INLINE int try_setstr(str_t* s, va_list* pargs)
{
  va_list args = *pargs;
  char* p = va_arg(args, char*);
  char* p1, *p2 = 0;
  int l = 0;
  var2var(1, p, 's', 0, 's', (void**)&p1, (void**)&p2);
  l = p1 - p;
  str_setsize(s, l);
  var2var(1, p, 's', s->s, 's', (void**)&p1, (void**)&p2);
  *pargs = args;
  return 0;
}
CC_INLINE int setvar(int t1, void* v, int t, va_list* pargs)
{
  if ('s' == t) {
    if ('s' == t1) {
      try_setstr((str_t*)v, pargs);
    }
    else {
      char buf[64] = {0};
      var2var(1, (const void*)(*pargs), t1, buf, 's', (void**)pargs, 0);
      str_setstr2((str_t*)v, buf);
    }
  }
  else {
    var2var(1, *pargs, t1, v, t, (void**)pargs, 0);
  }

  return 1;
}
CC_INLINE int try_setvar(const char* p, const char* name, void* v, int t, va_list* pargs)
{
  int l = strlen(name);

  if (p && 0 == memicmp(p, name, l) && ':' == p[l]) {
    setvar(p[l + 1], v, t, pargs);
    return 1;
  }

  return 0;
}
CC_INLINE int getvar(int t1, const void* v, int t, va_list* pargs)
{
  void* p = va_arg(*pargs, void*);

  if ('s' == t) {
    if ('s' == t1) {
      str_t* s = (str_t*)v;
      str_setstr((str_t*)p, s->s, s->l);
    }
    else {
      var2var(1, v, 's', p, t1, 0, 0);
    }
  }
  else {
    var2var(1, v, t, p, t1, 0, 0);
  }

  return 1;
}
CC_INLINE int try_getvar(const char* p, const char* name, const void* v, int t, va_list* pargs)
{
  int l = strlen(name);

  if (0 == memicmp(p, name, l) && ':' == p[l]) {
    getvar(p[l + 1], v, t, pargs);
    return 1;
  }

  return 0;
}
#endif // _VAR_INL_

