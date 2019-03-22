
#ifndef _STRING_C_H_
#define _STRING_C_H_

#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#include "inttypes_c.h"
#include "stddef_c.h"
#define static_strlen(STR)   (sizeof(STR)-1)

static bool ImCharIsSpace(int c)
{
  return c == ' ' || c == '\t' || c == 0x3000;
}

static bool ImCharIsSpaceW(int c)
{
  return c == L' ' || c == L'\t' || c == 0x3000;
}

#ifdef _TEST
int test_static_strlen()
{
#define CHECK(STR)  assert(static_strlen(STR)==strlen(STR));
  CHECK("test_static_strlen");
  return 0;
}
#endif


#ifdef __linux__
static int64 _atoi64(const char* str)
{
  return strtol(str, NULL, 10 );
}
static char* strupr(char* str)
{
  char* ptr = str;
  while (*ptr != '\0') {
    if (islower(*ptr)) {
      *ptr = toupper(*ptr);
    }
    ptr++;
  }
  return str;
}
static char* strlwr(char* str)
{
  char* ptr = str;
  while (*ptr != '\0') {
    if (isupper(*ptr)) {
      *ptr = tolower(*ptr);
    }
    ptr++;
  }
  return str;
}
static char* strrev(char* str)
{
  char* p1, *p2;
  if (! str || ! *str) {
    return str;
  }
  for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
    *p1 ^= *p2;
    *p2 ^= *p1;
    *p1 ^= *p2;
  }
  return str;
}

#endif

static void memunroll(void* p, int n, int unroll_to)
{
  char* ptr = (char*)p;
  if (1 == n) {
    memset(p, *(unsigned char*)p, unroll_to);
    return ;
  }
  while (n < unroll_to) {
    memcpy(ptr + n, p, MIN(n, unroll_to - n));
    n *= 2;
  }
}
static int xtoa_c(unsigned long val, char* buf, unsigned radix, int is_neg)
{
  char* p;        /* pointer to traverse string */
  char* firstdig;     /* pointer to first digit */
  char temp;       /* temp char */
  unsigned digval;    /* value of digit */
  p = buf;
  if (is_neg) {
    /* negative, so output '-' and negate */
    *p++ = '-';
    val = (unsigned long)(-(long)val);
  }
  firstdig = p;      /* save pointer to first digit */
  do {
    digval = (unsigned)(val % radix);
    val /= radix;    /* get next digit */
    /* convert to ascii and store */
    if (digval > 9) {
      *p++ = (char)(digval - 10 + 'a');  /* a letter */
    } else {
      *p++ = (char)(digval + '0');  /* a digit */
    }
  } while (val > 0);
  /* We now have the digit of the number in the buffer, but in reverse
  order. Thus we reverse them now. */
  int len = (int)(p - buf);
  *p-- = '\0';      /* terminate string; p points to last digit */
  do {
    temp = *p;
    *p = *firstdig;
    *firstdig = temp;  /* swap *p and *firstdig */
    --p;
    ++firstdig;     /* advance to next two digits */
  } while (firstdig < p); /* repeat until halfway */
  return len;
}

/* Actual functions just call conversion helper with neg flag set correctly,
and return pointer to buffer. */

static char* itoa_c(int val, char* buf, int radix)
{
  if (radix == 10 && val < 0) {
    xtoa_c((unsigned long)val, buf, radix, 1);
  } else {
    xtoa_c((unsigned long)(unsigned int)val, buf, radix, 0);
  }
  return buf;
}

static char* ltoa_c(long val, char* buf, int radix)
{
  xtoa_c((unsigned long)val, buf, radix, (radix == 10 && val < 0));
  return buf;
}
static char* ultoa_c(unsigned long val, char* buf, int radix)
{
  xtoa_c(val, buf, radix, 0);
  return buf;
}

#ifndef _NO_INT64

/* stdcall is faster and smaller... Might as well use it for the helper. */
static void x64toa_c(uint64 val, char* buf, unsigned radix, int is_neg)
{
  char* p;        /* pointer to traverse string */
  char* firstdig;     /* pointer to first digit */
  char temp;       /* temp char */
  unsigned digval;    /* value of digit */
  p = buf;
  if (is_neg) {
    *p++ = '-';     /* negative, so output '-' and negate */
    val = (uint64)(-(int64)val);
  }
  firstdig = p;      /* save pointer to first digit */
  do {
    digval = (unsigned)(val % radix);
    val /= radix;    /* get next digit */
    /* convert to ascii and store */
    if (digval > 9) {
      *p++ = (char)(digval - 10 + 'a');  /* a letter */
    } else {
      *p++ = (char)(digval + '0');  /* a digit */
    }
  } while (val > 0);
  /* We now have the digit of the number in the buffer, but in reverse
  order. Thus we reverse them now. */
  *p-- = '\0';      /* terminate string; p points to last digit */
  do {
    temp = *p;
    *p = *firstdig;
    *firstdig = temp;  /* swap *p and *firstdig */
    --p;
    ++firstdig;     /* advance to next two digits */
  } while (firstdig < p); /* repeat until halfway */
}

/* Actual functions just call conversion helper with neg flag set correctly,
and return pointer to buffer. */
static char* i64toa_c(int64 val, char* buf, int radix)
{
  x64toa_c((uint64)val, buf, radix, (radix == 10 && val < 0));
  return buf;
}
static char* ui64toa_c(uint64 val, char* buf, int radix)
{
  x64toa_c(val, buf, radix, 0);
  return buf;
}

#endif /* _NO_INT64 */

static int atoi_c(const char* nptr)
{
  int c;              /* current char */
  int total;      /* current total */
  int sign;           /* if '-', then negative, otherwise positive */
  /* skip whitespace */
  while (isspace((int)(unsigned char)*nptr)) {
    ++nptr;
  }
  c = (int)(unsigned char) * nptr++;
  sign = c;           /* save sign indication */
  if (c == '-' || c == '+') {
    c = (int)(unsigned char) * nptr++;  /* skip sign */
  }
  total = 0;
  while (isdigit(c)) {
    total = 10 * total + (c - '0');     /* accumulate digit */
    c = (int)(unsigned char) * nptr++;  /* get next char */
  }
  if (sign == '-') {
    return -total;
  } else {
    return total;  /* return result, negated if necessary */
  }
}
static int64 atoi64_c(const char* nptr)
{
  int c;              /* current char */
  int64 total;      /* current total */
  int sign;           /* if '-', then negative, otherwise positive */
  /* skip whitespace */
  while (isspace((int)(unsigned char)*nptr)) {
    ++nptr;
  }
  c = (int)(unsigned char) * nptr++;
  sign = c;           /* save sign indication */
  if (c == '-' || c == '+') {
    c = (int)(unsigned char) * nptr++;  /* skip sign */
  }
  total = 0;
  while (isdigit(c)) {
    total = 10 * total + (c - '0');     /* accumulate digit */
    c = (int)(unsigned char) * nptr++;  /* get next char */
  }
  if (sign == '-') {
    return -total;
  } else {
    return total;  /* return result, negated if necessary */
  }
}

CC_INLINE int strlen_c(const void* s)
{
  if (s) {
    uchar* us = (uchar*)s;
    while (*us++);
    return (int)(us - (uchar*)s - 1);
  }
  return 0;
}
//#define strlen_c(s)  (int)strlen(s)
//IRANGE { int s, l; };
//CC_INLINE IRANGE iRANGE(int s, int e) {  IRANGE r;  r.s = s, r.e = e;  return r;}
CC_INLINE IRANGE iRANGE_c(const char* s)
{
  IRANGE r;
  r.s = 0, r.e = (int)strlen(s);
  return r;
}
CC_INLINE uchar* get_delims_set(uchar* delims_set, const char* delims)
{
  for (; *delims;) {
    delims_set[(uchar)(*delims++)] = 1;
  }
  return delims_set;
}
CC_INLINE int memcmp_c(const char* s1, const char* s2, int n)
{
  while (--n && *s1 == *s2) {
    s1++;
    s2++;
  }
  return(*s1 - *s2);
}
CC_INLINE int memicmp_c(const char* s1, const char* s2, int n, int ignore_case)
{
  if (ignore_case) {
    static uchar map[256] = { 0 };
    static int inited = 0;
    if (!inited) {
      int i;
      for (i = 0; i < 256; ++i) {
        map[i] = ('A' <= i && i <= 'Z') ? (i - 'A' + 'a') : i;
      }
    }
    while (--n && map[(uchar)*s1] == map[(uchar)*s2]) {
      s1++;
      s2++;
    }
    return(map[(uchar) * s1] - map[(uchar) * s2]);
  }
  return memcmp_c(s1, s2, n);
}
#define CSTRINITLEN(str, len)   (len = ((len)<0 ? strlen_c((const char*)(str)) : len))
// _isend 锟斤拷末尾锟斤拷始锟饺斤拷
CC_INLINE int strnicmp_c(const char* s1, int l1, const char* s2, int l2, int n, int _isend, int ignore_case)
{
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  if (n < 0) {
    if (_isend) {
      if (l1 < l2) {
        return -1;
      }
      n = l2;
    } else {
      if (l1 != l2) {
        return CC_CMP(l1, l2);
      }
      n = l2;
    }
  } else {
    n = MIN(n, l1);
    n = MIN(n, l2);
  }
  if (!n) {
    return(0);
  }
  if (_isend) {
    s1 += l1 - n;
    s2 += l2 - n;
  }
  return memicmp_c(s1, s2, n, ignore_case);
}
CC_INLINE int stricmp_c(const char* s1, int l1, const char* s2, int l2, int ignore_case)
{
  return strnicmp_c(s1, l1, s2, l2, -1, 0, ignore_case);
}
CC_INLINE int strcmp_c(const char* s1, int l1, const char* s2, int l2)
{
  return strnicmp_c(s1, l1, s2, l2, -1, 0, 0);
}
#if 0
CC_INLINE int str_icmp(const char* s1, const char* s2, int ignore_case)
{
  return strnicmp_c(s1, -1, s2, -1, -1, 0, ignore_case);
}
CC_INLINE int str_cmp(const char* s1, const char* s2)
{
  return strnicmp_c(s1, -1, s2, -1, -1, 0, 0);
}
#endif
static int endwith(const char* s1, const char* s2, int ignore_case)
{
  return (0 == strnicmp_c(s1, -1, s2, -1, -1, 1, ignore_case));
}
CC_INLINE int findchr_c(const char* s, int i, int l, int ch)
{
  for (; i < l; ++i) {
    if (s[i] == ch) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int rfindchr_c(const char* s, int i, int l, int ch)
{
  for (; i < l--; ) {
    if (s[l] == ch) {
      return l;
    }
  }
  return -1;
}
CC_INLINE int findchrs_c(const char* s, int i, int l, const uchar* chs_set)
{
  //i = i<0 ? 0 : i;
  for (; i < l; ++i) {
    if (chs_set[(uchar)s[i]]) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int rfindchrs_c(const char* s, int i, int l, const uchar* chs_set)
{
  //i = i<0 ? 0 : i;
  for (; i < l--; ) {
    if (chs_set[(uchar)s[l]]) {
      return l;
    }
  }
  return -1;
}
static int find_last_of(const char* s, const char* chrs)
{
  int l = (int)strlen(s);
  uchar chrs_set[256] = { 0 };
  get_delims_set(chrs_set, chrs);
  return rfindchrs_c(s, 0, l, chrs_set);
}
CC_INLINE int findstr_c(const char* s, int i, int l, const char* s2, int l2, int ignore_case)
{
  l -= l2;
  for (; i < l; ++i) {
    if (0 == memicmp_c(s + i, s2, l2, ignore_case)) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int findstr(const char* s, const char* s2, int ignore_case)
{
  int l = (int)strlen(s);
  int l2 = (int)strlen(s2);
  return findstr_c(s, 0, l, s2, l2, ignore_case);
}
CC_INLINE int rfindstr_c(const char* s, int i, int l, const char* s2, int l2, int ignore_case)
{
  l -= l2;
  for (; i < l; --l) {
    if (0 == memicmp_c(s + l, s2, l2, ignore_case)) {
      return l;
    }
  }
  return -1;
}
CC_INLINE int strcount_c(const char* s1, int l1, const char* s2, int l2, int ignore_case)
{
  int i, j = 0;
  for (i = 0; i <= l1 - l2;) {
    if (0 == memicmp_c(s1 + i, s2, l2, ignore_case)) {
      ++j;
      i += l2;
    } else {
      ++i;
    }
  }
  return j;
}
CC_INLINE int delchr_c(char* s, int i, int l, int ch)
{
  int j = i;
  CSTRINITLEN(s, l);
  for (; i < l; ++i) {
    if (s[i] != ch) {
      s[j++] = s[i];
    }
  }
  return j;
}

CC_INLINE int replacestr_c(char* s1, IRANGE* r, const char* s2, int l2, const char* s3, int l3, int ignore_case)
{
  int l1 = r->s;
  int i, j = 0, m;
  l2 = l2 < 0 ? strlen(s2) : l2;
  l3 = l3 < 0 ? strlen(s3) : l3;
  if (l2 >= l3) {
    for (i = 0, m = 0; i <= l1 - l2;) {
      if (0 == memicmp_c(s1 + i, s2, l2, ignore_case)) {
        memcpy(s1 + m, s3, l3);
        m += l3;
        ++j;
        i += l2;
      } else {
        s1[m++] = s1[i++];
      }
    }
    for (; i < l1;) {
      s1[m++] = s1[i++];
    }
    r->s = (l1 + j * (l3 - l2));
  } else {
    //int pl1 = -1;
    //if (pl1 <= 0)
    {
      j = strcount_c(s1, l1, s2, l2, ignore_case);
      r->s = l1 + j * (l3 - l2);
    }
    if (r->s < r->e) {
      s1[r->s] = 0;
      for (i = l1, m = r->s; i >= l2;) {
        if (s2[0] == s1[i - l2] && 0 == memicmp_c(s1 + i - l2, s2, l2, ignore_case)) {
          m -= l3;
          memcpy(s1 + m, s3, l3);
          i -= l2;
          ++j;
        } else {
          s1[--m] = s1[--i];
        }
      }
      for (; i > 0;) {
        s1[--m] = s1[--i];
      }
    }
  }
  return j;
}

CC_INLINE int replace_str(char* s1, int l, const char* s2, int l2, const char* s3, int l3, int ignore_case) {
  l = l < 0 ? strlen(s1) : l;
  IRANGE r = iRANGE(l, l);
  replacestr_c(s1, &r, s2, l2, s3, l3, ignore_case);
  return r.e;
}
static void trim_c(const char* s, IRANGE* r, const uchar* trims_set)
{
  for (; r->s < r->e && trims_set[(uchar)s[r->e - 1]]; r->e--);
  for (; r->s < r->e && trims_set[(uchar)s[r->s]]; r->s++);
}
static void trim(const char* s, IRANGE* r, const char* trims)
{
  uchar trims_set[256] = { 0 };
  get_delims_set(trims_set, trims);
  trim_c(s, r, trims_set);
}

static const void strim(char** ps, int* plen, const char* trims)
{
  IRANGE r = iRANGE(0, *plen);
  trim(*ps, &r, trims);
  *ps = (*ps) + r.s;
  *plen = r.e-r.s;
}

static int split_c_(const char* s, IRANGE* r, IRANGE* out, int maxout, const uchar* delims_set, const uchar* trims_set, int minLen)
{
  int out_count = 0;
  for (; r->s < r->e && out_count < maxout;) {
    IRANGE ret = *r;
    for (; r->s < r->e; ++r->s) {
      if (delims_set[(uchar)s[r->s]]) {
        ret.e = r->s;
        ++r->s;
        break;
      }
    }
    if (trims_set) {
      trim_c(s, &ret, trims_set);
    }
    // save
    if (out) {
      if ((ret.e - ret.s) >= minLen) {
        out[out_count++] = ret;
      }
    } else {
      out_count++;
    }
  }
  return out_count;
}
static char* strsep_c(char** p, const char* delims)
{
  uchar delims_set[256] = { 0 };
  char* ret = NULL;
  get_delims_set(delims_set, delims);
  IRANGE r = iRANGE_c(*p), out;
  if (split_c_(*p, &r, &out, 1, delims_set, NULL, 0)) {
    ret = *p + out.s;;
    (*p)[out.e] = 0;
    *p = *p + r.s;
  } else {
    *p = ret = NULL;
  }
  if (r.s == r.e) {
    *p = NULL;
  }
  return ret;
}
static char* strcat_c(char* s, IRANGE* r, const char* s1, int l1)
{
  int l = r->e - r->s;
  //int l1 = r1.e - r1.s;
  assert(l > l1);
  memcpy(s + r->s, s1, l1);
  r->s += l1;
  s[r->s] = 0;
  return s;
}
static char* strcat2_c(char* s, IRANGE* r, const char* s1, int l1, const char* s2, int l2)
{
  int l = r->e - r->s;
  //int l1 = r1.e - r1.s;
  assert(l > (l1 + l2));
  memcpy(s + r->s, s1, l1);
  r->s += l1;
  memcpy(s + r->s, s2, l2);
  r->s += l2;
  s[r->s] = 0;
  return s;
}
static char* strcat3_c(char* s, IRANGE* r, const char* s1, int l1, const char* s2, int l2, const char* s3, int l3)
{
  int l = r->e - r->s;
  //int l1 = r1.e - r1.s;
  assert(l > (l1 + l2 + l3));
  memcpy(s + r->s, s1, l1);
  r->s += l1;
  memcpy(s + r->s, s2, l2);
  r->s += l2;
  memcpy(s + r->s, s3, l3);
  r->s += l3;
  s[r->s] = 0;
  return s;
}
static int strcpy_c(char* s, int rs, int re, const char* s1, int l1)
{
  int l = re - rs;
  //int l1 = r1.e - r1.s;
  assert(l > l1);
  memcpy(s + rs, s1, l1);
  rs += l1;
  s[rs] = 0;
  return rs;
}
static char* strcpy_c(char* s, IRANGE* r, const char* s1, int l1)
{
  r->s = strcpy_c(s, r->s, r->e, s1, l1);
  return s;
}
static char* strins_c(char* s, int i, IRANGE* r, const char* s1, int l1)
{
  int l = r->e - r->s;
  //int l1 = r1.e - r1.s;
  assert(i <= r->s);
  assert(l > l1);
  if (i < r->s) {
    memmove(s + i + l1, s + i, r->s - i);
  }
  memcpy(s + i, s1, l1);
  r->s += l1;
  s[r->s] = 0;
  return s;
}
static char* strdel_c(char* s, IRANGE* r, IRANGE r1)
{
  //int l = r->e - r->s;
  int l1 = r1.e - r1.s;
  assert(r1.s < r1.e);
  assert(r1.e <= r->s);
  memmove(s + r1.s, s + r1.e, r->s - r1.e);
  r->s -= l1;
  s[r->s] = 0;
  return s;
}
#define GET_DELIMS_SET(nameset, str)  uchar nameset[256] = {0};uchar* p##nameset = get_delims_set(nameset, str)
static int split_c(const char* s, IRANGE* r, IRANGE* out, int maxout, const char* delims, const char* trims, int minLen)
{
  uchar delims_set[256] = { 0 };
  uchar trims_set[256] = { 0 };
  get_delims_set(delims_set, delims);
  if (trims) {
    get_delims_set(trims_set, " \r\n\t");
  }
  return split_c_(s, r, out, maxout, delims_set, trims ? trims_set : NULL, minLen);
}
static int split_count_c(const char* s, int i, int l, const uchar* delims_set, const uchar* trims_set, int minlen)
{
  IRANGE r = iRANGE(i, l);
  return split_c_(s, &r, NULL, MAX_INT, delims_set, trims_set, minlen);
}
// s="|str1|str2|str3|"
// ignore_case
CC_INLINE int splitfind_c(const char* s, int l, const char* s1, int l1, int ignore_case, int* ppos)
{
  int i, j = 0, delims;
  if (NULL == s1 || NULL == s) {
    return -1;
  }
  CSTRINITLEN(s, l);
  CSTRINITLEN(s1, l1);
  ASSERT(l > 0);
  ASSERT(l1 > 0);
  delims = s[0];
  //first = s1[0];
  for (i = 0; i < l - l1; ++i) {
    if (delims == s[i]) {
      if (delims == s[i + l1 + 1] && 0 == memicmp_c(s + i + 1, s1, l1, ignore_case)) {
        if (ppos) {
          *ppos = i + 1;
        }
        return j;
      }
      ++j;
    }
  }
  return -1;
}
////////////////////////////////////////////////////

static char* strend(const char* str)
{
  return (char*)str + strlen(str);
}
static void path_split(const char* s, IRANGE* path, IRANGE* filename, IRANGE* filenameext, IRANGE* ext)
{
  const char* p;
  const char* e;
  const char* ends = strend(s);
  p = strrchr(s, '\\');
  e = strrchr(s, '/');
  p = MAX(p + 1, e + 1);
  p = MAX(p, s);
  e = (e = strrchr(p, '.')) ? e : strend(p);
  if (path) { *path = iRANGE(s - s, p - s); }
  if (filename) { *filename = iRANGE(p - s, e - s); }
  if (filenameext) { *filenameext = iRANGE(p - s, ends - s); }
  if (ext) { *ext = iRANGE(e - s, ends - s); }
}
static char* path_split_filename(const char* fullpath, char* buf, int len)
{
  IRANGE ret;
  path_split(fullpath, NULL, &ret, NULL, NULL);
  strncpy(buf, fullpath + ret.s, MIN(len, (ret.e-ret.s)));
  return buf;
}
static char* path_split_filenameext(const char* fullpath, char* buf, int len)
{
  IRANGE ret;
  path_split(fullpath, NULL, NULL, &ret, NULL);
  strncpy(buf, fullpath + ret.s, MIN(len, (ret.e-ret.s)));
  return buf;
}
static char* path_split_ext(const char* fullpath, char* buf, int len)
{
  IRANGE ret;
  path_split(fullpath, NULL, NULL, NULL, &ret);
  strncpy(buf, fullpath + ret.s, MIN(len, (ret.e-ret.s)));
  return buf;
}
//////////////////////////////////////////////////////////////
static char* avprintf(char** buf, int i, char const* const _Format, va_list  _ArgList) {
  int bsize = 32;
  int len = bsize;
  for (; len >= bsize; ) {
    bsize = len + 10;
    *buf = (char*)realloc((*buf), i + bsize + 1);
    len = vsnprintf((*buf) + i, bsize, _Format, _ArgList);
    assert(len >= 0 && "Check format string for errors");
  }
  (*buf)[bsize - 1] = 0;
  return *buf;
}
static char* aprintf(char** buf, int i, const char* fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  avprintf(buf, i, fmt, va);
  va_end(va);
  return *buf;
}

#ifdef _WIN32
static wchar_t* avwprintf(wchar_t** buf, int i, wchar_t const* const _Format, va_list  _ArgList) {
  int bsize = 32;
  int len = bsize;
  for (; len >= bsize; ) {
    bsize = len + 10;
    *buf = (wchar_t*)realloc((*buf), (i + bsize + 1) * sizeof(wchar_t));
    len = _vsnwprintf((*buf) + i, bsize, _Format, _ArgList);
    assert(len >= 0 && "Check format string for errors");
  }
  (*buf)[bsize - 1] = 0;
}
static wchar_t* awprintf(wchar_t** buf, int i, const wchar_t* fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  avwprintf(buf, i, fmt, va);
  va_end(va);
  return *buf;
}
#endif
#endif // _STRING_C_H_
