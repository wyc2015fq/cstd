#ifndef _CSTR_H_
#define _CSTR_H_

//#define strcasecmp stricmp
//#define strncasecmp strnicmp

#if _MSC_VER > 1300
#define stricmp _stricmp
#endif


#ifdef _WIN32
#define strncasecmp _strnicmp
#define strcasecmp stricmp
#else
#define strnicmp strncasecmp
#define stricmp strcasecmp
#endif

//int my_strncasecmp(const char* a, const char* b, int n) {  return strncasecmp(a, b, n);}
//#define STRNCASECMP2(a, b)  strncasecmp(a, b, sizeof(b)-1)
#define STRNCASECMP2(a, b)  strncasecmp2(a, b)
CC_INLINE int strncasecmp2(const char* s1, const char* s2) {
  size_t l2 = strlen(s2);
  return strncasecmp(s1, s2, l2);
}

#define CELEMTYPE_DEFDEF(CELEMTYPE_DEF) \
    CELEMTYPE_DEF(NULL) \
    CELEMTYPE_DEF(COMMENT) \
    CELEMTYPE_DEF(TEXT) \
    CELEMTYPE_DEF(ELEM) \
    CELEMTYPE_DEF(ATTR) \
    CELEMTYPE_DEF(ATTR_KEY) \
    CELEMTYPE_DEF(ATTR_VALUE) \
    CELEMTYPE_DEF(ARRAY) \
    CELEMTYPE_DEF(OBJECT) \
    CELEMTYPE_DEF(KEY) \
    CELEMTYPE_DEF(STRING) \
    CELEMTYPE_DEF(NUMBER)

typedef enum CElemType {
#define CELEMTYPE_DEF(x)  TT_ ## x,
    CELEMTYPE_DEFDEF(CELEMTYPE_DEF)
#undef CELEMTYPE_DEF
    //XML_MARK_COMMENT = 1 << TT_COMMENT,
    //XML_MARK_TEXT = 1 << TT_TEXT,
    //XML_MARK_ELEMENT = 1 << TT_ELEM,
    //XML_MARK_ATTR_KEY = 1 << TT_ATTR_KEY,
    //XML_MARK_ATTR_VALUE = 1 << TT_ATTR_VALUE,
    //XML_MARK_ALL = 0xff,
} CElemType;

static const char* GetCElemTypeName(CElemType t) {
  static const char* CElemTypeName[] = {
#define CELEMTYPE_DEF(x)  #x ,
  CELEMTYPE_DEFDEF(CELEMTYPE_DEF)
#undef CELEMTYPE_DEF
    };
  return t < countof(CElemTypeName) ? CElemTypeName[t] : "?";
}

typedef struct str_t {
  union {int l; int len;};
  union {char* s; char*str; char* ptr;};
} str_t;
typedef struct {
  int d;
  CElemType t;
  str_t s[1];
} dstr_t;
typedef struct {
  int mark;
  str_t s[1];
  dstr_t* v;
  int l;
} vdstr_t;

#define SKIPSPACE1 for (; i < len && ((uchar)buf[i]) <= 32; ++i)
//#define push_dstr(_s, _l, _d, _t)  if (ds) { ds[*opos].s->s = (char*)(_s), ds[*opos].s->l = _l, ds[*opos].d = _d, ds[*opos].t = _t; } (*opos)++
#define push_dstr(_beg, _l, _d, _t)  if (ds) { ds[*opos].s->s = (char*)(buf + _beg), ds[*opos].s->l = _l, ds[*opos].d = _d, ds[*opos].t = _t; } (*opos)++
#define SKIPSPACE(mm)  for (; mm->pos<mm->l && (' '>=mm->s[mm->pos] && mm->s[mm->pos]>0); mm->pos++) {}

#define STR1(s) STR2(s, strlen(s))
CC_INLINE str_t STR2(const char* s, int l)
{
  str_t s0;
  s0.s = (char*)s, s0.l = l;
  return s0;
}

// http://zh.wikipedia.org/wiki/%E5%AD%97%E7%AC%A6%E9%9B%86
#define LBYTE(X)  ((X)&0xff)
#define HBYTE(X)  LBYTE((X)>>8)
//#include "str/strstr.inl"

#define isupr(ch)      ('A'<=ch && ch<='Z')
#define islow(ch)      ('a'<=ch && ch<='z')
#define isalph(ch)     (isupr(ch)||islow(ch))
#define isnmb(ch)      ('0'<=ch && ch<='9')
#define isalphnmb(ch)  (isalph(ch)||isnmb(ch))
#define isopt(ch)      ('+'==ch||'-'==ch||'*'==ch||'/'==ch||'^'==ch||'%'==ch)

CC_INLINE ushort chr_next(const void* s, int i, int* poff)
{
  ushort chr;
  uchar* us = (uchar*)s;
  int off;
  if (us[i] & 0x80) {
    chr = (us[i] << 8) | us[ i + 1 ];
    off = i + 2;
  }
  else {
    chr = (us[i]);
    off = i + 1;
  }
  if (poff) {
    *poff = off;
  }
  return chr;
}
CC_INLINE ushort chr_prev(const void* s, int i, int* poff)
{
  ushort chr;
  uchar* us = (uchar*)s;
  int off;
  if (i > 1 && (us[i - 1] & 0x80)) {
    off = i - 2;
  }
  else {
    chr = (us[i]);
    off = i - 1;
  }
  if (poff) {
    *poff = off;
  }
  return chr;
}
CC_INLINE int cstr_len(const void* s)
{
  if (s) {
    uchar* us = (uchar*)s;
    while (*us++);
    return us - (uchar*)s - 1;
  }
  return 0;
}
CC_INLINE char* cstr_nredup(char** ps, const char* s, int l)
{
  *ps = 0;
  if (s) {
    int n = l<0 ? cstr_len(s) : l;
    MYREALLOC((*ps), n+1);
    memcpy(*ps, s, n);
    (*ps)[n] = 0;
  }
  return *ps;
}
CC_INLINE char* cstr_redup(char** ps, const char* s) {
  return cstr_nredup(ps, s, -1);
}
CC_INLINE char* cstr_ndup(const char* s, int l)
{
  char* pNew = NULL;
  cstr_nredup(&pNew, s, l);
  return pNew;
}
#define chr_tolower(ch)   (uchar)(('A'<=i && i<='Z') ? ((ch)-'A'+'a') : (ch))
#define chr_toupper(ch)   (uchar)(('a'<=i && i<='a') ? ((ch)-'a'+'A') : (ch))
#define CSTRINITLEN(str, len)   (len = ((len)<0 ? cstr_len((const char*)(str)) : len))
#define CSTRINIT(str, len)    uchar* u ## s = (uchar*)(str); int u ## len = CSTRINITLEN(str, len)
#define tok_have(mask, c)   ((mask[(c >> 3) & (256/8-1)] & (1 << (c & 7))) != 0)
CC_INLINE void tok_mask(uchar* mask, const char* char_set)
{
  memset(mask, 0, 256/8);
  while(*char_set) {
    unsigned c = (unsigned)(*char_set++) & 0xFF;
    mask[c >> 3] |= 1 << (c & 7);
  }
}

CC_INLINE char* cstr_tolower(char* s, int l)
{
  CSTRINIT(s, l);
  int i;
  for (; i<ul; ++i) {
    us[i] = chr_tolower(us[i]);
  }
  return s;
}
CC_INLINE char* cstr_toupper(char* s, int l)
{
  CSTRINIT(s, l);
  int i;
  for (; i<ul; ++i) {
    us[i] = chr_toupper(us[i]);
  }
  return s;
}
CC_INLINE int cstr_trimleft(char* s, int l)
{
  int i;
  for (i = 0; i < l && ' ' == s[i] && '\t' == s[i]; ++i) {}
  if (i > 0) {
    memcpy(s, s + i, l - i);
    s[l - i] = 0;
  }
  return l - i;
}
CC_INLINE int cstr_trimright(char* s, int l)
{
  int i;
  for (i = l; i > 0 && ' ' == s[i - 1] && '\t' == s[i - 1]; --i) {}
  s[i] = 0;
  return i;
}
CC_INLINE int cstr_del(char* p, int l, int i, int n)
{
  CSTRINITLEN(p, l);
  ASSERT((i + n) < l);
  memcpy(p + i, p + i + n, l - (i + n));
  l -= n;
  p[l] = 0;
  return l;
}
CC_INLINE int cstr_ins(const char* p, int l, int i, int n, const char* p0, int l0, char* p1, int l1)
{
  int l2;
  CSTRINITLEN(p, l);
  CSTRINITLEN(p0, l0);
  l2 = memrep(p, l, i, n, p0, l0, p1, l1);
  p1[l2] = 0;
  return l2;
}
CC_INLINE int cstr_ins_c(const char* p, int l, int i, int n, int ch, char* p1, int l1)
{
  char p0[4] = {(char)ch, 0, 0};
  return cstr_ins(p, l, i, n, p0, 1, p1, l1);
}
CC_INLINE int cstr_rev(char* s, int l)
{
  int i;
  char t;
  CSTRINITLEN(s, l);
  for (i = 0; i < l / 2; ++i) {
    SWAP(s[i], s[l - 1 - i], t);
  }
  return 0;
}
CC_INLINE int int2abc(int nNo, char* szTxt)
{
  int i = 0;
  int ndiv = nNo;
  do {
    szTxt[i++] = (char)((ndiv % 26) + 'A');
    ndiv /= 26;
  }
  while (ndiv != 0);
  szTxt[i] = 0;
  cstr_rev(szTxt, i);
  return 0;
}
CC_INLINE BOOL abc2int(const char* lpszAbc, int len, int* pn)
{
  int i, n, nttt;
  char szt[256] = {0};
  memcpy(szt, lpszAbc, len);
  if (0 == len) {
    return FALSE;
  }
  for (i = 0; i < len; i++) {
    if ((szt[i] < 'A') || (szt[i] > 'Z')) {
      return FALSE;
    }
  }
  n = 0;
  nttt = 1;
  for (i = len; i >= 0; --i) {
    n += (int)(szt[i - 1] - 'A') * nttt;
    nttt *= 26;
  }
  *pn = n;
  return TRUE;
}

//#define memcmp mem_cmp
CC_INLINE int mem_cmp(const void* pbuf1, const void* pbuf2, unsigned count)
{
  const char* buf1 = (const char*)pbuf1;
  const char* buf2 = (const char*)pbuf2;
  if (!count) {
    return(0);
  }
  while (--count && *buf1 == *buf2) {
    buf1++;
    buf2++;
  }
  return(*buf1 - *buf2);
}
CC_INLINE int mem_icmp(const char* s1, const char* s2, int n, int ignore_case)
{
  static uchar map[256] = {0};
  static int inited = 0;
  if (!inited) {
    int i;
    for (i = 0; i < 256; ++i) {
      map[i] = ('A' <= i && i <= 'Z') ? (i - 'A' + 'a') : i;
    }
  }
  if (ignore_case) {
    while (--n && map[(uchar)*s1] == map[(uchar)*s2]) {
      s1++;
      s2++;
    }
    return(map[(uchar) * s1] - map[(uchar) * s2]);
  }
  while (--n && *s1 == *s2) {
    s1++;
    s2++;
  }
  return(*s1 - *s2);
}
//#define strncmp str_ncmp
//#define mystrcmp(s1, l1, s2, l2) (((l1)==(l2))? strncmp((const char*)s1, (const char*)s2, l2):XCMP((l1),(l2)))
//#define mystricmp(s1, l1, s2, l2) (((l1)==(l2))?str_nicmp(s1, s2, l2):XCMP((l1),(l2)))
#define str_cmp(s1, s2, ignore_case) cstr_nicmp_((const char*)(s1).s, (s1).l, (const char*)(s2).s, (s2).l, -1, 0, ignore_case)
#define str_cmp_c(s1, s2) cstr_nicmp_((const char*)(s1).s, (s1).l, (const char*)(s2), -1, -1, 0, 0)
#define cstr_cmp(s1, l1, s2, l2) cstr_nicmp_((const char*)(s1), l1, (const char*)(s2), l2, -1, 0, 0)
#define cstr_nicmp(s1, l1, s2, l2, n, ignore_case)  cstr_nicmp_(s1, l1, s2, l2, n, 0, ignore_case)
#define cstr_ncmp(s1, l1, s2, l2, n)  cstr_nicmp_(s1, l1, s2, l2, n, 0, 0)
#define cstr_icmp(s1, l1, s2, l2, ignore_case)  cstr_nicmp_(s1, l1, s2, l2, -1, 0, ignore_case)
#define cstr_endwith(s1, l1, s2, l2, ignore_case)  (0==cstr_nicmp_(s1, l1, s2, l2, -1, 1, ignore_case))
CC_INLINE int cstr_nicmp_(const char* s1, int l1, const char* s2, int l2, int n, int _isend, int ignore_case)
{
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  if (n < 0) {
    if (_isend) {
      if (l1 < l2) {
        return -1;
      }
      n = l2;
    }
    else {
      if (l1 != l2) {
        return CC_CMP(l1, l2);
      }
      n = l2;
    }
  }
  else {
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
  return mem_icmp(s1, s2, n, ignore_case);
}
CC_INLINE int cstr_next_chunk_alphanum(const char* s, int i, int l, str_t* ps) {
  ps->s = (char*)s + i;
  if ( isnmb(s[i]) ) {
    for (++i;i<l && isnmb(s[i]);++i);
  } else {
    for (++i;i<l && !isnmb(s[i]);++i);
  }
  ps->l = (char*)s + i - ps->s;
  return i;
}
CC_INLINE int cstr_cmp_alphanum(const char* s1, const char* s2, int ignore_case) {
  size_t l1 = strlen(s1);
  size_t l2 = strlen(s2);
  int i1=0, i2=0;
  str_t ps1[1] = {0};
  str_t ps2[1] = {0};
  for (;i1<l1 && i2<l2;) {
    i1 = cstr_next_chunk_alphanum(s1, i1, l1, ps1);
    i2 = cstr_next_chunk_alphanum(s2, i2, l2, ps2);
    if (ps1->l>0 && ps2->l>0) {
      if (isnmb(ps1->s[0]) && isnmb(ps2->s[0])) {
        int t1 = atoi(ps1->s);
        int t2 = atoi(ps2->s);
        if (t1!=t2) {
          return CC_CMP(t1, t2);
        }
      } else {
        int b = str_cmp(*ps1, *ps2, ignore_case);
        if (b) {return b;}
      }
    } else {
      return CC_CMP(ps1->l, ps2->l);
    }
  }
  l1-=i1;
  l2-=i2;
  return CC_CMP(l1, l2);
}
CC_INLINE int cstr_delchr(char* s, int l, int ch) {
  int i,j=0;
  CSTRINITLEN(s, l);
  for (i=0;i<l; ++i) {
    if (s[i]!=ch) {
      s[j++] = s[i];
    }
  }
  return j;
}
CC_INLINE char* cstr_chr(const char* s, int l, int ch)
{
  //int ulen = CSTRINITLEN(s, l);
  const char* e = s + l;
  for (; s < e; ++s) {
    if (*s == ch) {
      return (char*)s;
    }
  }
  return 0;
}
CC_INLINE char* cstr_rchr(const char* s, int l, int ch)
{
  //int ulen = CSTRINITLEN(s, l);
  const char* e = s + l;
  for (; s < e--;) {
    if (*e == ch) {
      return (char*)e;
    }
  }
  return 0;
}
// return new beg
CC_INLINE int cstr_findchr(const char* s, int range_beg, int range_end, int ch)
{
  int i = range_beg;
  CSTRINITLEN(s, range_end);
  ASSERT(range_end<=range_end);
  for (; i < range_end; ++i) {
    if (s[i] == ch) {
      return i;
    }
  }
  return -1;
}
// not find return -1
// founded return next char index
CC_INLINE int cstr_rfindchr(const char* s, int range_beg, int range_end, int ch)
{
  int i;
  CSTRINITLEN(s, range_end);
  ASSERT(range_end<=range_end);
  i = range_end;
  for (; --i>=range_beg;) {
    if (s[i] == ch) {
      return i+1;
    }
  }
  return -1;
}
typedef uchar uchar256[256];
typedef char char256[256];
CC_INLINE int cstr_initmap(uchar* usmap, const char* s, int l, int x, int y) {
  int i;
  uchar* us = (uchar*)s;
  CSTRINITLEN(s, l);
  memset(usmap, x, 256);
  for (i=0; i<l; ++i) {
    usmap[us[i]] = y;
  }
  return 0;
}
// return new beg
CC_INLINE int cstr_findchrs(const char* s, int range_beg, int range_end, const char* chs, BOOL isnot) {
  uchar* us = (uchar*)s;
  uchar256 usmap1 = {0};
  int i = range_beg;
  CSTRINITLEN(s, range_end);
  ASSERT(i<=range_end);
  cstr_initmap(usmap1, chs, -1, isnot, !isnot);
  //i = i<0 ? 0 : i;
  for (; i<range_end;++i) {
    if (usmap1[us[i]]) {
      return i;
    }
  }
  return -1;
}
// return new end
CC_INLINE int cstr_rfindchrs(const char* s, int range_beg, int range_end, const char* chs, BOOL isnot) {
  uchar* us = (uchar*)s;
  uchar256 usmap1;
  int i = range_end;
  CSTRINITLEN(s, range_end);
  ASSERT(i<=range_end);
  cstr_initmap(usmap1, chs, -1, isnot, !isnot);
  for (; --i>=range_beg;) {
    if (usmap1[us[i]]) {
      return i+1;
    }
  }
  return -1;
}
CC_INLINE int cstr_replacechr_cpy(char* s, int l, const char* s1, int l1, int c1, int c2, int onlyone) {
  int i, t=0, j=0, ch;
  if (l<l1) {
    return 0;
  }
  CSTRINITLEN(s1, l1);
  if (onlyone) {
    for (i=0; i<l1; ++i) {
      ch = s1[i];
      if (ch==c1) {
        if (0==t) {
          s[j++] = c2;
        }
        t = 1;
      } else {
        t = 0;
        s[j++] = ch;
      }
    }
  } else {
    for (i=0; i<l1; ++i) {
      ch = s1[i];
      s[i] = (ch==c1) ? c2 : ch;
    }
  }
  s[l1] = 0;
  return l1;
}
CC_INLINE int cstr_replacechrs_cpy(char* s, int l, const char* s1, int l1, const uchar* umap, int c2, int onlyone) {
  uchar* us1 = (uchar*)s1;
  int i, t=0, j=0, ch;
  if (l<l1) {
    return 0;
  }
  CSTRINITLEN(s1, l1);
  if (onlyone) {
    for (i=0; i<l1; ++i) {
      ch = us1[i];
      if (umap[ch]) {
        if (0==t) {
          s[j++] = c2;
        }
        t = 1;
      } else {
        t = 0;
        s[j++] = c2;
      }
    }
  } else {
    for (i=0; i<l1; ++i) {
      ch = s1[i];
      s[i] = umap[ch] ? c2 : ch;
    }
    j = i;
  }
  s[j] = 0;
  return j;
}
CC_INLINE int cstr_countchr(const char* s, int l, int ch)
{
  int i, j = 0;
  CSTRINITLEN(s, l);
  for (i = 0; i < l; ++i) {
    if (s[i] == ch) {
      j++;
    }
  }
  return j;
}
CC_INLINE int cstr_replacechr(char* s, int l, int ch1, int ch2)
{
  int i, j = 0;
  uchar* us = (uchar*)s;
  CSTRINITLEN(s, l);
  for (i = 0; i < l; ++i) {
    if (us[i] == ch1) {
      us[i] = (uchar)ch2;
      j++;
    }
  }
  return j;
}
CC_INLINE char* cstr_chrs(const char* s, int l, const char* chs)
{
  int i;
  const uchar* us = (const uchar*)s;
  const uchar* uchs = (const uchar*)chs;
  //char map[256] = {0};  for (i = 0; uchs[i]; ++i) {    map[uchs[i]] = 1;  }
  CSTRINITLEN(s, l);
  for (i = 0; i < l; ++i) {
    if (uchs[us[i]]) {
      return (char*)(s + i);
    }
  }
  return 0;
}
CC_INLINE char* cstr_rchrs(const char* s, int l, const char* chs)
{
  int i;
  const uchar* us = (const uchar*)s;
  const uchar* uchs = (const uchar*)chs;
  uchar map[256] = {0};
  for (i = 0; uchs[i]; ++i) {
    map[uchs[i]] = 1;
  }
  CSTRINITLEN(s, l);
  for (i = l - 1; i >= 0; --i) {
    if (map[us[i]]) {
      return (char*)(s + i);
    }
  }
  return 0;
}
#define str_find_str(s1, s2, i, ignore_case)  cstr_find((s1).s, (s1).l, (s2).s, (s2).l, i, ignore_case)
CC_INLINE int cstr_find(const char* s1, int l1, const char* s2, int l2, int i, int ignore_case)
{
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  if (l2 > 0) {
    i = i < 0 ? (l1 + i) : i;
    for (; i <= l1 - l2; ++i) {
      if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
        return i;
      }
    }
  }
  return -1;
}
#define str_rfind_str(s1, s2, i, ignore_case)  cstr_rfind((s1).s, (s1).l, (s2).s, (s2).l, i, ignore_case)
CC_INLINE int cstr_rfind(const char* s1, int l1, const char* s2, int l2, int i, int ignore_case)
{
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  if (l2 > 0) {
    i = i ? i : (l1 - l2);
    i = i < 0 ? (l1 + i) : i;
    for (i = l1 - l2; i >= 0; --i) {
      if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
        return i;
      }
    }
  }
  return -1;
}
CC_INLINE int cstr_count(const char* s1, int l1, const char* s2, int l2, int ignore_case)
{
  int i, j = 0;
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  for (i = 0; i <= l1 - l2;) {
    if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
      ++j;
      i += l2;
    }
    else {
      ++i;
    }
  }
  return j;
}
CC_INLINE int cstr_removechr(char* s, int l, int ch)
{
  int i = 0, j = 0;
  CSTRINITLEN(s, l);
  for (; i < l; ++i) {
    if (s[i] != ch) {
      s[j++] = s[i];
    }
  }
  s[j] = 0;
  return i - j;
}
CC_INLINE int cstr_replacestr(char* s1, int l1, int len, const char* s2, int l2, const char* s3, int l3, int ignore_case, int pl1, int* pj) {
  int i, j = 0, m, ret=l1;
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  CSTRINITLEN(s3, l3);
  if (l2 >= l3) {
    for (i = 0, m = 0; i <= l1 - l2;) {
      if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
        memcpy(s1 + m, s3, l3);
        m += l3;
        ++j;
        i += l2;
      }
      else {
        s1[m++] = s1[i++];
      }
    }
    for (; i < l1;) {
      s1[m++] = s1[i++];
    }
    ret = (l1 + j * (l3 - l2));
  } else {
    if (pl1<=0) {
      j = cstr_count(s1, l1, s2, l2, ignore_case);
      ret = l1 + j * (l3 - l2);
      pl1 =  ret;
    }
    if (ret<len && pl1<len) {
      s1[pl1] = 0;
      for (i = l1, m = pl1; i >= l2;) {
        if (s2[0]==s1[i - l2] && 0 == mem_icmp(s1 + i - l2, s2, l2, ignore_case)) {
          m -= l3;
          memcpy(s1 + m, s3, l3);
          i -= l2;
          ++j;
        }
        else {
          s1[--m] = s1[--i];
        }
      }
      for (; i > 0;) {
        s1[--m] = s1[--i];
      }
      ret = pl1;
    }
  }
  if (pj) {
    *pj = j;
  }
  return ret;
}
CC_INLINE int cstr_cpy(char* s0, int len, const char* s2, int l2)
{
  CSTRINITLEN(s2, l2);
  //ASSERT(l2 < len);
  l2 = MIN(l2, len-1);
  memcpy(s0, s2, l2);
  s0[l2] = 0;
  return l2;
}
#define cstr_cat1(s0, len, s1)  cstr_cat(s0, len, s1, -1)
CC_INLINE int cstr_cat(char* s0, int l0, int len, const char* s1, int l1)
{
  CSTRINITLEN(s0, l0);
  CSTRINITLEN(s1, l1);
  if (l0 + l1 < len) {
  memcpy(s0 + l0, s1, l1);
  l0 += l1;
  s0[l0] = 0;
  }
  return l0;
}
CC_INLINE int cstr_cat2(char* s0, int l0, int len, const char* s1, int l1, const char* s2, int l2)
{
  CSTRINITLEN(s0, l0);
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  if ((l0 + l1 + l2) < len) {
  memcpy(s0 + l0, s1, l1);
  l0 += l1;
  memcpy(s0 + l0, s2, l2);
  l0 += l2;
  s0[l0] = 0;
  }
  return l0;
}
CC_INLINE int cstr_cat3(char* s0, int l0, int len, const char* s1, int l1, const char* s2, int l2, const char* s3, int l3)
{
  CSTRINITLEN(s0, l0);
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  CSTRINITLEN(s3, l3);
  if ((l0 + l1 + l2 + l3) < len) {
  memcpy(s0 + l0, s1, l1);
  l0 += l1;
  memcpy(s0 + l0, s2, l2);
  l0 += l2;
  memcpy(s0 + l0, s3, l3);
  l0 += l3;
  s0[l0] = 0;
  }
  return l0;
}
CC_INLINE int cstr_cat_c(char* s0, int l0, int len, int ch)
{
  CSTRINITLEN(s0, l0);
  if (l0 + 1 < len) {
  s0[l0++] = ch;
  s0[l0] = 0;
  }
  return l0;
}
CC_INLINE int cstr_cat_nc(char* s0, int l0, int len, int ch, int n)
{
  CSTRINITLEN(s0, l0);
  if (l0 + n < len) {
  memset(s0+l0, ch, n);
  l0+=n;
  s0[l0] = 0;
  }
  return l0;
}
CC_INLINE int _cstrmid0(const char* s0, int l0, int* beg, int* end)
{
  UNUSED(s0);
  *beg = BOUND(*beg, 0, l0);
  if (*end < 0) {
    *end = l0;
  }
  else {
    *end = BOUND(*end, 0, l0);
  }
  return 0;
}
CC_INLINE int cstr_mid_cpy(const char* s0, int l0, int beg, int end, char* out, int len)
{
  int l2;
  CSTRINITLEN(s0, l0);
  _cstrmid0(s0, l0, &beg, &end);
  l2 = (end-beg);
  if (l2 < len) {
  memcpy(out, s0+beg, l2);
  out[l2] = 0;
  }
  return l2;
}
CC_INLINE str_t __cstrtrim_c(const char* s, int l, const uchar* spmap)
{
  const uchar* us = (const uchar*)s;
  const uchar* ue;
  CSTRINITLEN(s, l);
  ue = us + l;
  for (; us < ue && spmap[ue[-1]]; ue--);
  for (; us < ue && spmap[*us]; us++);
  return STR2((char*)us, ue - us);
}
CC_INLINE str_t _cstrtrim_c(const char* s, int l, const char* sp)
{
  const uchar* us = (const uchar*)s;
  const uchar* ue;
  const uchar* usp = (const uchar*)sp;
  uchar spmap[256] = {0};
  for (;*usp;) {
    spmap[*usp++] = 1;
  }
  CSTRINITLEN(s, l);
  ue = us + l;
  for (; us < ue && spmap[ue[-1]]; ue--);
  for (; us < ue && spmap[*us]; us++);
  return STR2((char*)us, ue - us);
}
CC_INLINE int cstr_trim(char* s1, int l1, const char* sp)
{
  str_t s;
  CSTRINITLEN(s1, l1);
  s = _cstrtrim_c(s1, l1, sp);
  memcpy(s1, s.s, s.l);
  s1[s.l] = 0;
  return s.l;
}
CC_INLINE char* strtrim(char* s, const char* sp)
{
  char* e = s + cstr_len(s);
  for (; s <= e--;) {
    if (strchr(sp, *e) == NULL) {
      e[1] = 0;
      break;
    }
  }
  return s;
}
CC_INLINE int get_delimsmap(uchar* delimsmap, const char* delims) {
  for (; *delims;) {
    delimsmap[(uchar)(*delims++)] = 1;
  }
  return 0;
}

CC_INLINE int str_skip(const char* s, int l, int i, const char* trimstr)
{
  char trimstrmap[256] = {0};
  for (; *trimstr;) {
    trimstrmap[(uchar)(*trimstr++)] = 1;
  }
  for (; i < l && trimstrmap[(uchar)s[i]]; ++i) {}
  return i;
}
CC_INLINE int str2i(const char* s, int l, int i, const char* trimstr, int* outi)
{
  int d = 0;
  if (trimstr) {
    i = str_skip(s, l, i, trimstr);
  }
  for (; i < l && isspace(s[i]); ++i);
  for (; i < l && isnmb(s[i]); ++i) {
    d = d * 10 + s[i] - '0';
  }
  if (trimstr) {
    i = str_skip(s, l, i, trimstr);
  }
  if (outi) {
    *outi = i;
  }
  return d;
}
CC_INLINE double str2f(const char* s, int l, int i, const char* trimstr, int* outi)
{
  double d = 0;
  double sign = 1;
  if (trimstr) {
    i = str_skip(s, l, i, trimstr);
  }
  for (; i < l && isspace(s[i]); ++i);
  if ('-'==s[i]) {
    sign = -1;
    ++i;
  } else if ('+'==s[i]) {
    ++i;
  }
  for (; i < l && isnmb(s[i]); ++i) {
    d = d * 10 + s[i] - '0';
  }
  if (i < l && '.' == s[i]) {
    double b = 1.;
    ++i;
    for (; i < l && isnmb(s[i]); ++i) {
      b *= 0.1;
      d += (s[i] - '0') * b;
    }
  }
  if (trimstr) {
    i = str_skip(s, l, i, trimstr);
  }
  if (outi) {
    *outi = i;
  }
  return sign*d;
}
CC_INLINE str_t str2s(const char* s, int l, int i, const char* trimstr, int* outi)
{
  int j;
  str_t s0 = {0};
  char trimstrmap[256] = {0};
  for (; *trimstr;) {
    trimstrmap[(uchar)(*trimstr++)] = 1;
  }
  for (; i < l && trimstrmap[(uchar)s[i]]; ++i) {}
  for (j = i; !trimstrmap[(uchar)s[i]]; ++i) {}
  s0.s = (char*)s + j;
  s0.l = i - j;
  if (outi) {
    //for (; i<l && trimstrmap[(uchar)s[i]]; ++i) {}
    *outi = i;
  }
  return s0;
}
//
CC_INLINE int64 str2i64(const char* s, int l, int i, int base, int* ppos)
{
  static unsigned char basemap[128] = {0};
  int k = 0;
  static int basemapinited = 0;
  const unsigned char* us = (unsigned char*)s;
  int64 x = 0;
  if (!basemapinited) {
    basemapinited = 1;
    memset(basemap, 255, 128);
    for (k = 0; k < 10; ++k) {
      basemap['0' + k] = k;
    }
    for (k = 0; k < 26; ++k) {
      basemap['a' + k] = 10 + k;
      basemap['A' + k] = 10 + k;
    }
  }
  for (; i < l && (us[i] >= 128 || 255 == basemap[us[i]]); ++i);
  for (; i < l; ++i) {
    if (us[i] >= 128 || 255 == basemap[us[i]]) {
      break;
    }
    x = x * base + basemap[us[i]];
  }
  if (ppos) {
    *ppos = i;
  }
  return x;
}
CC_INLINE int rev_byte(int n, char* s)
{
  int i;
  for (i = 0; i < n / 2; ++i) {
    char t = s[i];
    s[i] = s[n - 1 - i], s[n - 1 - i] = t;
  }
  return 0;
}
// Êµï¿½ï¿½ï¿½Ö·ï¿½ï¿½ï¿½ï¿½Ä·ï¿½×ª   today is a good day   ï¿½ï¿½ï¿?day good a is today
CC_INLINE int rev_word(int n, char* s)
{
  int j, i;
  for (i = 0; i < n; ++i) {
    if (' ' != s[i]) {
      for (j = i + 1; j < n && ' ' != s[j]; ++j);
      rev_byte(j - i, s + i);
      i = j;
    }
  }
  rev_byte(n, s);
  return 0;
}
CC_INLINE int chr_hex2int(int h)
{
  static char map[128] = {0};
  static int inited = 0;
  if (!inited) {
    static const char* nu = "0123456789";
    static const char* lo = "abcdef";
    static const char* hi = "ABCDEF";
    int i;
    memset(map, 0, 128);
    for (i = 0; i < 10; ++i) {
      map[(uchar)nu[i]] = i;
    }
    for (i = 0; i < 6; ++i) {
      map[(uchar)lo[i]] = i + 10;
      map[(uchar)hi[i]] = i + 10;
    }
    inited = 1;
  }
  ASSERT(h >= 0 && h < 128);
  return map[h];
}
CC_INLINE unsigned int cstr_hex2int(const char* s, unsigned int l)
{
  unsigned int i, d = 0;
  for (i = 0; i < l && s[i]; ++i) {
    d = (d << 4) + chr_hex2int(s[i]);
  }
  return d;
}
CC_INLINE unsigned int cstr_hex2int2(const char* s, unsigned int l)
{
  unsigned int i, d = 0;
  for (i = 0; i < l; ++i) {
    d |= chr_hex2int(s[i])<<(i*4);
  }
  return d;
}
// split str ; rerutn pos
CC_INLINE int cstr_split(const char* s, int* pi, int l, const char* delims, const char* trimstr, int isend, str_t* ps) {
  uchar delimsmap[256] = {0};
  uchar trimstrmap[256] = {0};
  int i = *pi;
  int i0 = i, i1 = i;
  l = l<0?(int)strlen(s):l;
  get_delimsmap(delimsmap, delims);
  if (trimstr) {
    get_delimsmap(trimstrmap, trimstr);
    if (isend) {
      for (; i > 0 && trimstrmap[(uchar)(s[i-1])]; --i);
    } else {
      for (; i < l && trimstrmap[(uchar)(s[i])]; ++i);
    }
  }
  if (isend) {
    for (i1 = i, i0 = 0; (--i) >= 0 ; ) {
      if (delimsmap[(uchar)(s[i])]) {
        i0 = i+1;
        --i;
        while(delimsmap[(uchar)(s[i])]) {
          --i;
        }
        break;
      }
    }
  } else {
    for (i1 = l, i0 = i; i < l; ++i) {
      if (delimsmap[(uchar)(s[i])]) {
        i1 = i;
        ++i;
        while(delimsmap[(uchar)(s[i])]) {
          ++i;
        }
        break;
      }
    }
  }
  *pi = i;
  if (trimstr) {
    for (; i0 < i1 && trimstrmap[(uchar)(s[i0])]; ++i0);
    for (; i0 < i1 && trimstrmap[(uchar)(s[i1-1])]; --i1);
  }
  ps->s = (char*)s+i0;
  ps->l = i1-i0;
  return i;
}
CC_INLINE int cstr_split_find(const char* s, int* pi, int l, const char* delims, const char* trimstr, char* s1, int l1) {
  int k = 0;
  str_t ps[1] = {0};
  for (; *pi < l; ++k) {
    if (cstr_split(s, pi, l, delims, trimstr, 0, ps)<l && ps->l > 0) {
      if(0==cstr_cmp(s1, l1, ps->s, ps->l)) {
        return k;
      }
    }
  }
  return -1;
}

CC_INLINE int cstr_split_copy(const char* s, int* i, int l, const char* delims, const char* trimstr, char* buf, int buflen) {
  str_t ss[1] = {0};
  cstr_split(s, i, l, delims, trimstr, 0, ss);
  cstr_cpy(buf, buflen, ss->s, ss->l);
  return 0;
}

CC_INLINE int cstr_split2(const char* s, int i, int l, const char* delims, const char* trimstr, str_t* ps) {
  uchar delimsmap[256] = {0};
  int i0 = i, depth0=0, depth1=0, depth2=0;
  get_delimsmap(delimsmap, delims);
  for (; i < l; ++i) {
    if (0==depth0 && 0==depth1 && 0==depth2 && delimsmap[(uchar)(s[i])]) {
      ++i;
      while(delimsmap[(uchar)(s[i])]) {
        ++i;
      }
      break;
    }
    if ('('==s[i]) {
      ++depth0;
    } else if (')'==s[i]) {
      --depth0;
    } else if ('['==s[i]) {
      ++depth1;
    } else if (']'==s[i]) {
      --depth1;
    } else if ('{'==s[i]) {
      ++depth2;
    } else if ('}'==s[i]) {
      --depth2;
    }
  }
  if (trimstr) {
    *ps = _cstrtrim_c(s+i0, i-i0, trimstr);
  }
  return i;
}
// ·µ»Ø¸öÊý
CC_INLINE int cstr_split_c(const char* s, int i, int l, int delim, str_t* items, int maxitems)
{
  int nitem = 0, isbeg = 1;
  CSTRINITLEN(s, l);
  for (; i<l && nitem<maxitems; ++i) {
    if (delim != s[i]) {
      if (isbeg) {
        items[nitem].s = (char*)s+i;
        isbeg = 0;
      }
    }
    else if (!isbeg) {
      items[nitem].l = s+i - items[nitem].s;
      nitem++;
      isbeg = 1;
    }
  }
  return nitem;
}
// s="|str1|str2|str3|" 
// ignore_case  
CC_INLINE int cstr_splitfind(const char* s, int l, const char* s1, int l1, int ignore_case, int* ppos)
{
  int i, j = 0, delims;
  if (NULL == s1 || NULL == s) {
    return -1;
  }
  l = (l > 0) ? l : (int)strlen(s);
  l1 = (l1 > 0) ? l1 : (int)strlen(s1);
  ASSERT(l > 0);
  ASSERT(l1 > 0);
  delims = s[0];
  //first = s1[0];
  for (i = 0; i < l - l1; ++i) {
    if (delims == s[i]) {
      if (delims == s[i + l1 + 1] && 0 == mem_icmp(s + i + 1, s1, l1, ignore_case)) {
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
CC_INLINE char* strdupcat(const char* s1, const char* s2)
{
  int s1len = (int)strlen(s1), s2len = (int)strlen(s2);
  char* s12 = (char*)pmalloc(s1len + s2len + 1);
  memcpy(s12, s1, s1len);
  memcpy(s12 + s1len, s2, s2len + 1);
  return s12;
}
static char* GetFilePathCopy(const char* fn, char* szFilePath, int MaxPathLen) {
  int i;
  i = (int)strlen(fn);
  if (i>0 && (fn[i] == '\\' || fn[i] == '/')) {
    --i;
  }
  while (i > 0 && (fn[i] != '\\' && fn[i] != '/')) {
    i--;
  }
  if (i>=MaxPathLen) {
    return NULL;
  }
  if (i > 0) {
    szFilePath[i] = 0;
  }
  memcpy(szFilePath, fn, i);
  return (szFilePath);
}
static char* _GetFilePath(const char* fn)
{
  static char szFilePath[256] = "";
  return GetFilePathCopy(fn, szFilePath, countof(szFilePath));
}
static char* GetFileExt(const char* fn)
{
  size_t len = strlen(fn);
  char* pos = (char*)fn + len;
  for (;pos!=fn && (pos[-1]!='\\' && pos[-1]!='/');--pos) {
    if ('.'==pos[-1]) {
      return pos;
    }
  }
  return (char*)fn+len;
}
static char* GetFileNameExt(const char* fn)
{
  int i = (int)strlen(fn)-1;
  while (i >= 0 && (fn[i] != '\\' && fn[i] != '/')) {
    i--;
  }
  if (i < 0) {
    return (char*)fn;
  }
  return (char*)(fn + i + 1);
}
static char* ReplaceExt(const char* fn, const char* ext, char* out, int outlen) {
  const char* oldext = GetFileExt(fn);
  size_t pos = (oldext && *oldext) ? (oldext-fn) : strlen(fn);
  while (*ext && *ext=='.') {
    ++ext;
  }
  if (*oldext) {
    cstr_cat2(out, 0, outlen, fn, pos, ext, -1);
  } else {
    cstr_cat3(out, 0, outlen, fn, pos, ".", 1, ext, -1);
  }
  return out;
}

static char* GetPathFileNameExtN(const char* fn, int lastn)
{
  int i = (int)strlen(fn)-1, j=0;
  for (; i>=0; ) {
    for (; i >= 0 && (fn[i] != '\\' && fn[i] != '/'); --i);
    ++j;
    if (j>lastn) {
      break;
    }
    for (; i >= 0 && (fn[i] == '\\' || fn[i] == '/'); --i);
  }
  if (i < 0) {
    return (char*)fn;
  }
  return (char*)(fn + i + 1);
}
// get ch char Before str
static char* GetBeforeCopy(const char* fn, char* buf, int buflen, int ch) {
  int len = (int)strlen(fn);
  int i = len-1;
  while (i >= 0 && (fn[i] != ch)) {
    i--;
  }
  if (i < 0) {
    i = len;
  }
  if (i>=buflen) {
    return NULL;
  }
  i = MIN(i, buflen-1);
  strncpy(buf, fn, i);
  buf[i] = 0;
  return buf;
}
static char* GetPathFileNameCopy(const char* fn, char* buf, int buflen) {
  return GetBeforeCopy(fn, buf, buflen, '.');
}
static char* GetFileNameCopy(const char* fn, char* buf, int buflen) {
  char* name_ext = GetFileNameExt(fn);
  return GetBeforeCopy(name_ext, buf, buflen, '.');
}
#define CSTR_LEFT        1<<1
#define CSTR_PLUS        1<<2
#define CSTR_SPACE       1<<3
#define CSTR_ZEROPAD     1<<4
#define CSTR_SMALL       1<<5
#define CSTR_SIGN        1<<6
#define CSTR_SPECIAL     1<<7
#define CSTR_CAPEXP      1<<8
#define is_digit(c)   ('0'<=(c) && (c)<='9')
CC_INLINE int cstr_atoi(const char* s, int i, int l) {
  int d = 0;
  for (; i<l && is_digit(s[i]); ++i) {
    d = d * 10 + (s[i] - '0');
  }
  return d;
}
CC_INLINE int skip_atoi(const char** pstr)
{
  int d = 0;
  const char* str = *pstr;
  while (is_digit(*str)) {
    d = d * 10 + (*str - '0');
    ++str;
  }
  *pstr = str;
  return d;
}
CC_INLINE int cstr_int2hex(char* str, uint32 ipos, uint32 h, uint32 field_width )
{
  static const char* hex = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (str) {
    uint32 x=h, i=0;
    str += ipos;
    for (x=h;x;++i, x>>=4);
    field_width = MAX(field_width, 1);
    field_width = MAX(field_width, i);
    str[field_width] = 0;
    for (x=h;field_width--;x>>=4) {
      str[field_width] = hex[x&0xf];
    }
  }
  return ipos;
}

#define I2STR_IMPL(str, maxstrlen, n, radix, field_width, flags) { \
    int i = 0, j, fillchr = 0, has_sign, buflen; \
    uchar buf[32] = {0}; \
    const char* intstr = NULL; \
    char ct, sign = 0; \
    fillchr = CSTR_ZEROPAD & flags ? '0' : ' '; \
    intstr = CSTR_SMALL & flags ? "0123456789abcdefghijklmnopqrstuvwxyz" : "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; \
    if (10 == radix) { \
      if (n < 0) { sign = '-', un = -n; } \
      else if (CSTR_PLUS & flags && n) { sign = '+'; } \
    } \
    if (0 == un) { buf[i++] = intstr[0]; } \
    else { while (un) { r = un % radix;  un /= radix; buf[i++] = intstr[r]; } } \
    for (j = 0; j < i / 2; ++j) { CC_SWAP(buf[i - j - 1], buf[j], ct); } \
    buflen = i; has_sign = (sign != 0); \
    fwidth = MAX(field_width, buflen + has_sign); \
    fwidth = MIN(fwidth, maxstrlen - 1); \
    buflen = MIN(buflen, fwidth - has_sign); \
    if (str) { \
      int filllen = fwidth - buflen - has_sign; \
      j = 0; \
      if (CSTR_LEFT & flags) { \
        if (sign) { str[j++] = sign; } \
        memcpy(str + j, buf, buflen), j += buflen; \
        memset(str + j, ' ', filllen); \
      } else { \
        if (CSTR_ZEROPAD & flags) { \
          if (sign) { str[j++] = sign; } \
          memset(str + j, fillchr, fwidth - buflen - has_sign); \
          j += filllen; \
        } else { \
          memset(str + j, fillchr, fwidth - buflen - has_sign); \
          j += filllen; \
          if (sign) { str[j++] = sign; } \
        } \
        memcpy(str + j, buf, buflen); \
      } \
      str[fwidth] = '\0'; \
    } \
  }
CC_INLINE int i2str(char* str, int maxstrlen, int n, uint radix, int field_width, int precision, int flags)
{
  int fwidth = 0;
  int r;
  unsigned int un = n;
  UNUSED(precision);
  I2STR_IMPL(str, maxstrlen, n, radix, field_width, flags);
  return fwidth;
}
CC_INLINE int i2str64(char* str, int maxstrlen, int64 n, uint radix, int field_width, int precision, int flags)
{
  int fwidth = 0;
  int64 r;
  uint64 un = n;
  UNUSED(precision);
  I2STR_IMPL(str, maxstrlen, n, radix, field_width, flags);
  return fwidth;
}

#define IEEE754_FL_E  8
#define IEEE754_FL_T  (31-IEEE754_FL_E)
#define IEEE754_DB_E  11
#define IEEE754_DB_T  (63-IEEE754_DB_E)
#define IEEE754_FL_G_S(x) ((x)>>31)
#define IEEE754_FL_G_F(x) ((x)&((1<<23)-1))
#define IEEE754_FL_G_E(x) ((x>>23)&((1<<8)-1))
CC_INLINE int float_fmt(char* str, int maxstrlen, int sign, int expo, uint64 frac, uint radix, int field_width, int precision, int flags)
{
  char buf[128] = {0};
  const char* intstr = "0123456789abcdefghijklmnopqrstuvwxyz";
  const uint64 fracmax = ((uint64) - 1) / radix;
  const uint radix_2 = radix >> 1;
  uint64 t;
  int i = 0, j, fwidth = field_width;
  int idot = 0, fzeros = 0; // 
  int fillchr = CSTR_ZEROPAD & flags ? '0' : ' ';
  int nzero = 0;
  //field_width = field_width<0?6:field_width;
  if (sign) {
    *str++ = '-';
    --maxstrlen;
  }
  maxstrlen = MIN(maxstrlen, 20);
  //ultoa(frac, buf1, 2);
  while (expo < 0) {
    frac <<= 1;
    expo++;
    while (frac > fracmax) {
      frac = (frac + radix_2) / radix;
      --nzero;
    }
  }
  for (; (expo >= 64) || (0 == (frac >> expo));) {
    while (frac > fracmax) {
      frac >>= 1;
      --expo;
    }
    frac *= radix;
    ++nzero;
  }
  idot = i;
  fwidth = MAX(field_width, idot + precision + 1);
  if (frac || precision) {
    //buf[i++] = '.';
  }
  for (; frac && i < maxstrlen;) {
    while (frac > fracmax) {
      frac >>= 1;
      --expo;
    }
    frac *= radix;
    ++nzero;
    t = (expo < 64) ? (frac >> expo) : 0;
    if (t) {
      int i0 = i, ct;
      for (; t; ++i) {
        int tr = (int)(t % radix);
        buf[i] = intstr[tr];
        t /= radix;
      }
      frac &= ((uint64)1 << expo) - 1;
      //memrev(buf+i0, 1, i-i0);
      ARRREV(i - i0, buf + i0, ct);
    }
    else {
      buf[i++] = '0';
    }
    if ('0' < buf[i] && 0 == fzeros) {
      fzeros = i;
    }
  }
  buf[i] = 0;
  if ((nzero - i) > 2 || nzero < 0) {
    //flags |= CSTR_CAPEXP;
    //precision-=2;    fwidth = MAX(field_width, idot + precision + 1);
  }
  j = i;
  if (1) {
    int u = precision + i - nzero;
    if (i>u) {
      if (buf[u]<'5') {
        buf[u] = 0;
        nzero -= i-u;
        j = u;
      } else {
        int u1 = u;
        if (buf[u]=='9') {
          for(;u>=0 && buf[u]=='9'; --u) {
            buf[u] = '0';
          }
          buf[u]++;
        } else {
          buf[u-1]++;
        }
        buf[u1] = 0;
        nzero -= i-u1;
        j = u1;
      }
    }
  }
  i = j;
  for (; i > 0 && nzero > 0 && '0' == buf[i - 1];) {
    --i;
    --nzero;
  }
  buf[i] = 0;
  if ((CSTR_CAPEXP & flags)) {
    int u = i - nzero - 1;
    memcpy(buf + 2, buf + 1, i - 1);
    buf[1] = '.';
    i = MIN(i, precision + 2);
    buf[i++] = 'e';
    i2str(buf + i, countof(buf) - i, u, 10, 4, 0, CSTR_ZEROPAD | CSTR_PLUS);
    i += 4;
    buf[i] = 0;
    idot = 1;
    fwidth = MAX(field_width, i);
  }
  else {
    int u = i - nzero;
    if (u <= 0) {
      memcpy(buf - u + 2, buf, i);
      memset(buf, '0', -u + 2);
      i += -u + 2;
      u = 1;
    }
    else {
      memcpy(buf + u + 1, buf + u, nzero);
      ++i;
    }
    buf[u] = '.';
    buf[i]=0;
  }
  //fwidth = field_width>0 ? MIN(fwidth, maxstrlen - 1) : 0;
  //precision = MIN(precision, i-idot);
  for(i=0; buf[i]; ++i);
  j = MIN(maxstrlen-1, i);
  fwidth = BOUND(field_width, j, maxstrlen - 1);
  if (CSTR_LEFT & flags) {
    memcpy(str, buf, j);
    memset(str + j, fillchr, fwidth - j);
  }
  else {
    if (sign) {
      --str;
      memset(str, ' ', fwidth - j);
      str[fwidth - j] = '-';
      memcpy(str + fwidth - j + 1, buf, j);
      fwidth++;
    }
    else {
      memset(str, ' ', fwidth - j);
      memcpy(str + fwidth - j, buf, j);
    }
  }
  if (str[fwidth-1]=='.') {
    --fwidth;
  }
  str[fwidth] = '\0';
  return fwidth;
}
CC_INLINE int d2str1(char* str, int maxstrlen, double fnum, uint radix, int field_width, int precision, int flags)
{
  int u;
  const char* intstr = "0123456789abcdefghijklmn0pqrstuvwxyz";
  int fillchr = CSTR_ZEROPAD & flags ? '0' : ' ';
  int i = 0, j = 0, idot = 0;
  char buf[256] = {0}, ct;
  while (fnum > 1) {
    u = (int)fmod(fnum, radix);
    fnum /= radix;
    buf[i++] = intstr[u];
  }
  for (j = 0; j < i / 2; ++j) {
    CC_SWAP(buf[i - j - 1], buf[j], ct);
  }
  idot = i;
  if (precision) {
    buf[i++] = '.';
  }
  field_width = MAX(field_width, idot + precision);
  j = MIN(field_width, i);
  if (i > j) {
    buf[j - 1] += buf[j] > '4';
  }
  field_width = MIN(field_width, maxstrlen-1);
  if (CSTR_LEFT & flags) {
    memcpy(str, buf, j);
    memset(str + j, fillchr, field_width - j);
  }
  else {
    memset(str, ' ', field_width - j);
    memcpy(str + field_width - j, buf, j);
  }
  str[field_width] = '\0';
  return field_width;
}
CC_INLINE int d2str(char* str, int maxstrlen, double fnum, uint radix, int field_width, int precision, int flags)
{
  uint64 u = 0, wf;
  enum { toff = IEEE754_DB_T, maxe = (1 << (IEEE754_DB_E)) - 1, maxe_2 = maxe >> 1 };
  int j = 0, e = toff, e1, ws, we;
  u = db2u64(fnum);
  ws = (int)(u >> 63);
  we = (int)(u >> toff) & maxe;
  wf = u & (((uint64)1 << toff)-1);
  j=0;
  if (0 == wf && 0 == we) {
    if (str && maxstrlen >= 2) {
      if (ws) {
        str[j++] = ws ? '-' : '+';
      }
      str[j++] = '0';
      str[j++] = 0;
    }
  }
  else if (0 == wf && maxe == (int)we) {
    if (str && maxstrlen >= 2) {
      if (ws) {
        str[j++] = ws ? '-' : '+';
      }
      str[j++] = 'i';
      str[j++] = 'n';
      str[j++] = 'f';
      str[j++] = 0;
    }
  }
  else if (0 == wf && maxe == (int)we) {
    if (str && maxstrlen >= 2) {
      if (ws) {
        str[j++] = ws ? '-' : '+';
      }
      str[j++] = '1';
      str[j++] = 0;
    }
  }
  else {
    u = wf | ((uint64)1 << e);
    e1 = maxe_2 - (int)we;
    e += e1;
    j = float_fmt(str, maxstrlen, (int)ws, e, u, radix, field_width, precision, flags);
  }
  return j;
}
CC_INLINE int f2str(char* str, int maxstrlen, float fnum, uint radix, int field_width, int precision, int flags)
{
  typedef union tfl {
    float f;
    int i;
    uint u;
    uchar c[4];
    struct {
      uint f:
    IEEE754_FL_T;
    uint e:
    IEEE754_FL_E;
    uint s: 1;
    } w;
} tfl;
  tfl tf;
  uint u = 0, wf=0;
  enum { toff = IEEE754_FL_T, maxe = (1 << (IEEE754_FL_E)) - 1, maxe_2 = maxe >> 1 };
  int i = 0, j = 0, e = IEEE754_FL_T, e1, ws=0, we=0;
  tf.f = fnum;
  //u = sys_db2u64(fnum);
  u = tf.u;
  ws = (int)(u >> 31);
  we = (int)(u >> toff) & maxe;
  wf = u & ((1 << toff)-1);
  //for(i=0; i<4; ++i) {    printf("%d ", tf.c[i]);  }
  j = 0;
  if (0 == wf && 0 == we) {
    if (str && maxstrlen >= 2) {
      str[j++] = '0';
      str[j] = 0;
    }
  }
  else if (0 == wf && maxe == (int)we) {
    if (str && maxstrlen >= 2) {
      str[j++] = ws ? '-' : '+';
      str[j++] = 'i';
      str[j++] = 'n';
      str[j++] = 'f';
      str[j] = 0;
    }
  }
  else {
    u = wf | (1 << e);
    e1 = maxe_2 - (int)we;
    e += e1;
    j = float_fmt(str, maxstrlen, (int)ws, e, u, radix, field_width, precision, flags);
  }
  return j;
}

/////////////////////////////////////////////////////////////////////////////
// CString formatting
//#define TCHAR_ARG char
//#define WCHAR_ARG wchar_t
//#define CHAR_ARG char
//#define DOUBLE_ARG double
#define FORCE_ANSI 0x10000
#define FORCE_UNICODE 0x20000
#define FORCE_INT64 0x40000
#define _tcsinc(str) (str+1)

static int cstr_format_lenv(const char* lpszFormat, va_list argList)
{
  // make a guess at the maximum length of the resulting string
  int nMaxLen = 0, nItemLen, nWidth, nPrecision, nModifier;
  const char* lpsz;
  for (lpsz = lpszFormat; *lpsz != '\0'; lpsz = _tcsinc(lpsz)) {
    // handle '%' character, but watch out for '%%'
    if (*lpsz != '%' || *(lpsz = _tcsinc(lpsz)) == '%') {
      nMaxLen += (int)strlen(lpsz);
      continue;
    }
    nItemLen = 0;
    // handle '%' character with format
    nWidth = 0;
    for (; *lpsz != '\0'; lpsz = _tcsinc(lpsz)) {
      // check for valid flags
      if (*lpsz == '#') {
        nMaxLen += 2;  // for '0x'
      }
      else if (*lpsz == '*') {
        nWidth = va_arg(argList, int);
      }
      else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
          *lpsz == ' ')
        ;
      else { // hit non-flag character
        break;
      }
    }
    // get width and skip it
    if (nWidth == 0) {
      // width indicated by
      nWidth = atoi(lpsz);
      for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _tcsinc(lpsz))
        ;
    }
    ASSERT(nWidth >= 0);
    nPrecision = 0;
    if (*lpsz == '.') {
      // skip past '.' separator (width.precision)
      lpsz = _tcsinc(lpsz);
      // get precision and skip it
      if (*lpsz == '*') {
        nPrecision = va_arg(argList, int);
        lpsz = _tcsinc(lpsz);
      }
      else {
        nPrecision = atoi(lpsz);
        for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _tcsinc(lpsz))
          ;
      }
      ASSERT(nPrecision >= 0);
    }
    // should be on type modifier or specifier
    nModifier = 0;
    if (strncmp(lpsz, "I64", 3) == 0) {
      lpsz += 3;
      nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
      // __int64 is only available on X86 and ALPHA platforms
      ASSERT(FALSE);
#endif
    }
    else {
      switch (*lpsz) {
        // modifiers that affect size
      case 'h':
        nModifier = FORCE_ANSI;
        lpsz = _tcsinc(lpsz);
        break;
      case 'l':
        nModifier = FORCE_UNICODE;
        lpsz = _tcsinc(lpsz);
        break;
        // modifiers that do not affect size
      case 'F':
      case 'N':
      case 'L':
        lpsz = _tcsinc(lpsz);
        break;
      }
    }
    // now should be on specifier
    switch (*lpsz | nModifier) {
      // single characters
    case 'c':
    case 'C':
      nItemLen = 2;
      (void)va_arg(argList, int);
      break;
    case 'c'|FORCE_ANSI:
    case 'C'|FORCE_ANSI:
      nItemLen = 2;
      (void)va_arg(argList, int);
      break;
    case 'c'|FORCE_UNICODE:
    case 'C'|FORCE_UNICODE:
      nItemLen = 2;
      (void)va_arg(argList, int);
      break;
      // strings
    case 'S':
    case 's': {
      const char* pstrNextArg = va_arg(argList, const char*);
      if (pstrNextArg == NULL) {
        nItemLen = 6;  // "(null)"
      }
      else {
        nItemLen = (int)strlen(pstrNextArg);
        nItemLen = MAX(1, nItemLen);
      }
    }
    break;
    case 's'|FORCE_ANSI:
    case 'S'|FORCE_ANSI: {
      char* pstrNextArg = va_arg(argList, char*);
      if (pstrNextArg == NULL) {
        nItemLen = 6;  // "(null)"
      }
      else {
        nItemLen = (int)strlen(pstrNextArg);
        nItemLen = MAX(1, nItemLen);
      }
    }
    break;
    }
    // adjust nItemLen for strings
    if (nItemLen != 0) {
      if (nPrecision != 0) {
        nItemLen = MIN(nItemLen, nPrecision);
      }
      nItemLen = MAX(nItemLen, nWidth);
    }
    else {
      switch (*lpsz) {
        // integers
      case 'd':
      case 'i':
      case 'u':
      case 'x':
      case 'X':
      case 'o':
        if (nModifier & FORCE_INT64) {
          (void)va_arg(argList, int64);
        }
        else {
          (void)va_arg(argList, int);
        }
        nItemLen = 32;
        nItemLen = MAX(nItemLen, nWidth + nPrecision);
        break;
      case 'e':
      case 'g':
      case 'G':
        va_arg(argList, double);
        nItemLen = 128;
        nItemLen = MAX(nItemLen, nWidth + nPrecision);
        break;
      case 'f': {
        double f;
        char pszTemp[32];
        // 312 == strlen("-1+(309 zeroes).")
        // 309 zeroes == MAX precision of a double
        // 6 == adjustment in case precision is not specified,
        // which means that the precision defaults to 6
        //pszTemp = (char*)pmalloc(MAX(nWidth, 312 + nPrecision + 6));
        f = va_arg(argList, double);
        snprintf(pszTemp, 32, "%*.*f", nWidth, nPrecision + 6, f);
        nItemLen = (int)strlen(pszTemp);
      }
      break;
      case 'p':
        (void)va_arg(argList, void*);
        nItemLen = 32;
        nItemLen = MAX(nItemLen, nWidth + nPrecision);
        break;
        // no output
      case 'n':
        (void)va_arg(argList, int*);
        break;
      default:
        ASSERT(FALSE); // unknown formatting option
      }
    }
    // adjust nMaxLen for output nItemLen
    nMaxLen += nItemLen;
  }
  return nMaxLen;
}

#include <stdarg.h>

static int cstr_format_lenv2(const char* lpszFormat, va_list argList)
{
  // make a guess at the maximum length of the resulting string
  int nMaxLen = 0, nItemLen, nWidth, nPrecision, nModifier;
  const char* lpsz;
  for (lpsz = lpszFormat; *lpsz != '\0'; lpsz = _tcsinc(lpsz)) {
    // handle '%' character, but watch out for '%%'
    if (*lpsz != '%' || *(lpsz = _tcsinc(lpsz)) == '%') {
      nMaxLen += (int)strlen(lpsz);
      continue;
    }
    nItemLen = 0;
    // handle '%' character with format
    nWidth = 0;
    for (; *lpsz != '\0'; lpsz = _tcsinc(lpsz)) {
      // check for valid flags
      if (*lpsz == '#') {
        nMaxLen += 2;  // for '0x'
      }
      else if (*lpsz == '*') {
        nWidth = va_arg(argList, int);
      }
      else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
          *lpsz == ' ')
        ;
      else { // hit non-flag character
        break;
      }
    }
    // get width and skip it
    if (nWidth == 0) {
      // width indicated by
      nWidth = atoi(lpsz);
      for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _tcsinc(lpsz))
        ;
    }
    ASSERT(nWidth >= 0);
    nPrecision = 0;
    if (*lpsz == '.') {
      // skip past '.' separator (width.precision)
      lpsz = _tcsinc(lpsz);
      // get precision and skip it
      if (*lpsz == '*') {
        nPrecision = va_arg(argList, int);
        lpsz = _tcsinc(lpsz);
      }
      else {
        nPrecision = atoi(lpsz);
        for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _tcsinc(lpsz))
          ;
      }
      ASSERT(nPrecision >= 0);
    }
    // should be on type modifier or specifier
    nModifier = 0;
    if (strncmp(lpsz, "I64", 3) == 0) {
      lpsz += 3;
      nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
      // __int64 is only available on X86 and ALPHA platforms
      ASSERT(FALSE);
#endif
    }
    else {
      switch (*lpsz) {
        // modifiers that affect size
      case 'h':
        nModifier = FORCE_ANSI;
        lpsz = _tcsinc(lpsz);
        break;
      case 'l':
        nModifier = FORCE_UNICODE;
        lpsz = _tcsinc(lpsz);
        break;
        // modifiers that do not affect size
      case 'F':
      case 'N':
      case 'L':
        lpsz = _tcsinc(lpsz);
        break;
      }
    }
    // now should be on specifier
    switch (*lpsz | nModifier) {
      // single characters
    case 'c':
    case 'C':
      nItemLen = 2;
      (void)va_arg(argList, int);
      break;
    case 'c'|FORCE_ANSI:
    case 'C'|FORCE_ANSI:
      nItemLen = 2;
      (void)va_arg(argList, int);
      break;
    case 'c'|FORCE_UNICODE:
    case 'C'|FORCE_UNICODE:
      nItemLen = 2;
      (void)va_arg(argList, int);
      break;
      // strings
    case 'S':
    case 's': {
      const char* pstrNextArg = va_arg(argList, const char*);
      if (pstrNextArg == NULL) {
        nItemLen = 6;  // "(null)"
      }
      else {
        nItemLen = (int)strlen(pstrNextArg);
        nItemLen = MAX(1, nItemLen);
      }
    }
    break;
    case 's'|FORCE_ANSI:
    case 'S'|FORCE_ANSI: {
      char* pstrNextArg = va_arg(argList, char*);
      if (pstrNextArg == NULL) {
        nItemLen = 6;  // "(null)"
      }
      else {
        nItemLen = (int)strlen(pstrNextArg);
        nItemLen = MAX(1, nItemLen);
      }
    }
    break;
    }
    // adjust nItemLen for strings
    if (nItemLen != 0) {
      if (nPrecision != 0) {
        nItemLen = MIN(nItemLen, nPrecision);
      }
      nItemLen = MAX(nItemLen, nWidth);
    }
    else {
      switch (*lpsz) {
        // integers
      case 'd':
      case 'i':
      case 'u':
      case 'x':
      case 'X':
      case 'o':
        if (nModifier & FORCE_INT64) {
          (void)va_arg(argList, int64);
        }
        else {
          (void)va_arg(argList, int);
        }
        nItemLen = 32;
        nItemLen = MAX(nItemLen, nWidth + nPrecision);
        break;
      case 'e':
      case 'g':
      case 'G':
        (void)va_arg(argList, double);
        nItemLen = 128;
        nItemLen = MAX(nItemLen, nWidth + nPrecision);
        break;
      case 'f': {
        double f;
        char pszTemp[32];
        // 312 == strlen("-1+(309 zeroes).")
        // 309 zeroes == MAX precision of a double
        // 6 == adjustment in case precision is not specified,
        // which means that the precision defaults to 6
        //pszTemp = (char*)pmalloc(MAX(nWidth, 312 + nPrecision + 6));
        f = va_arg(argList, double);
        snprintf(pszTemp, 32, "%*.*f", nWidth, nPrecision + 6, f);
        nItemLen = (int)strlen(pszTemp);
      }
      break;
      case 'p':
        (void)va_arg(argList, void*);
        nItemLen = 32;
        nItemLen = MAX(nItemLen, nWidth + nPrecision);
        break;
        // no output
      case 'n':
        (void)va_arg(argList, int*);
        break;
      default:
        ASSERT(FALSE); // unknown formatting option
      }
    }
    // adjust nMaxLen for output nItemLen
    nMaxLen += nItemLen;
  }
  return nMaxLen;
}
#undef _tcsinc
#undef TCHAR_ARG
#undef WCHAR_ARG
#undef CHAR_ARG
#undef DOUBLE_ARG
#undef FORCE_ANSI
#undef FORCE_UNICODE
#undef FORCE_INT64

typedef int (*xtoa_t)(char * buf, int len, void * x);

CC_INLINE int cstr_vnprintf(char* buf, int buflen, const char* fmt, va_list args)
{
  int j=0;
  enum {isarr = false};
#define cstr_va_arg(ap,t)   va_arg(ap,t)
#define va_argt(ap,t)    ((uint64)cstr_va_arg(ap,t))
#define va_argt_int(ap,t)   (cstr_va_arg(ap, t) )
#define va_argt_float(ap)   (cstr_va_arg(ap, double))
#define TGETUINT(base) { int64 d = ((64 == bits) ? va_argt_int(args, int64) :  va_argt_int(args, int32) ); \
  j += i2str64(buf + j, buflen - j, d, base, field_width, precision, flags);   }
		
#include "cstr_vnprintf_impl.inl"
#undef TGETUINT
#undef va_argt_int
#undef cstr_va_arg
#undef va_argt_float
  return j; //
}
CC_INLINE char* cstr_vnprintf_arr(char* buf, int buflen, const char* fmt, char* args, int* pcount)
{
  int j=0;
  enum {isarr = true};
#define cstr_va_arg(ap,t)   (*(t *)((ap += sizeof(t)) - sizeof(t)) )
#define va_argt_int(ap,t)   (cstr_va_arg(ap, t) )
#define va_argt_float(ap)   (cstr_va_arg(ap, float) )
#define TGETUINT(base) { int64 d = ((64 == bits) ? va_argt_int(args, int64) : (32 == bits) ? va_argt_int(args, int32) : (16 == bits) ? va_argt_int(args, int16) : va_argt_int(args, int8) ); \
  j += i2str64(buf + j, buflen - j, d, base, field_width, precision, flags);   }
		
#include "cstr_vnprintf_impl.inl"
#undef TGETUINT
#undef va_argt_int
#undef cstr_va_arg
#undef va_argt_float
  if (pcount) {
    *pcount = j;
  }
  return args; //
}
CC_INLINE char* _format(const char* fmt, ...) {
  static char buf_format[4096];
  va_list ap;
  va_start(ap, fmt);
  _vsnprintf(buf_format, countof(buf_format), fmt, ap);
  va_end(ap);
  return buf_format;
}
CC_INLINE char* cstr_vsprintf_dup2(const char* fmt, va_list arglist) {
  int buflen = cstr_format_lenv2(fmt, arglist)+10;
  char* buf = (char*)pmalloc(buflen);
  int len = _vsnprintf(buf, buflen, fmt, arglist);
  buf[len] = 0;
  return buf;
}
CC_INLINE char* cstr_vsprintf2(const char* fmt, ...)
{
  char* buf = NULL;
  va_list arglist;
  va_start(arglist, fmt);
  buf = cstr_vsprintf_dup2(fmt, arglist);
  va_end(arglist);
  return buf;
}
CC_INLINE char* cstr_vsprintf_dup(const char* fmt, va_list arglist) {
  int buflen = cstr_format_lenv(fmt, arglist);
  char* buf = (char*)pmalloc(buflen);
  //int len = cstr_vnprintf(buf, buflen, fmt, arglist, &arglist);
  //buf[len] = 0;
  _vsnprintf(buf, buflen, fmt, arglist);
  return buf;
}
CC_INLINE int cstr_nprintf(char* buf, int buflen, const char* fmt, ...)
{
  int len = 0;
  va_list arglist;
  va_start(arglist, fmt);
  len = cstr_vnprintf(buf, buflen, fmt, arglist);
  va_end(arglist);
  return len;
}
CC_INLINE int cstr_cat_nprintf(char* buf, int buflen, const char* fmt, ...) {
  int len = 0;
  va_list arglist;
  int l = (int)strlen(buf);
  va_start(arglist, fmt);
  len = cstr_vnprintf(buf+l, buflen-l, fmt, arglist);
  va_end(arglist);
  return len;
}
CC_INLINE int cstr_cat_int32(char* buf, int buflen, int n) {
  int l = (int)strlen(buf);
  l+=i2str(buf+l, buflen-l, n, 10, 0, 0, 0);
  return l;
}
#define str_printf_arr(fmt, n, arr, bufsz)  {char* buf = MALLOC(char, bufsz); MEMSET(buf, 0, bufsz); str_nprintf_arr(buf, bufsz, fmt, n, arr); printf(buf); FREE(buf);}
#define fprintf_arr(fn, fmt, n, arr, bufsz)  {FILE* pf = fopen(fn, "wb"); char* buf = MALLOC(char, bufsz); MEMSET(buf, 0, bufsz); str_nprintf_arr(buf, bufsz, fmt, n, arr); fprintf(pf, "%s", buf); FREE(buf); fclose(pf);}
CC_INLINE int cstr_nprintf_arr(char* buf, int buflen, const char* fmt, int n, void* arr)
{
  int i, len = 0, len1=0;
  char* arglist = (char*)arr;
  for (i = 0; i < n && buflen > len + 1; ++i) {
    ASSERT(len < buflen);
    arglist = cstr_vnprintf_arr(buf + len, buflen - len, fmt, arglist, &len1);
    len += len1;
  }
  return len;
}
CC_INLINE int cstr_nprintf_mat(char* buf, int buflen, const char* fmt, const char* endline, int m, int n, void* mat, int step)
{
  int j, len = 0;
  int endlinelen = cstr_len(endline);
  for (j = 0; j < m; ++j) {
    len += cstr_nprintf_arr(buf + len, buflen - len, fmt, n, (uchar*)mat + j * step);
    memcpy(buf + len, endline, endlinelen);
    len += endlinelen;
    buf[len] = 0;
  }
  return 0;
}
CC_INLINE int str_printf_mat(const char* fmt, int m, int n, void* mat, int step, int bufsz)
{
  char* buf = MALLOC(char, bufsz);
  MEMSET(buf, 0, bufsz);
  cstr_nprintf_mat(buf, bufsz, fmt, "\n", m, n, mat, step);
  puts(buf);
  FREE(buf);
  return 0;
}
#undef CSTR_LEFT
#undef CSTR_PLUS
#undef CSTR_SPACE
#undef CSTR_ZEROPAD
#undef CSTR_SMALL
#undef CSTR_SIGN
#undef CSTR_SPECIAL
#undef CSTR_CAPEXP

typedef struct {
  int is;
} regex_result_t;

CC_INLINE int cstr_regex_match(regex_result_t* re, int ire, int nre, const char* p, int ip, int plen, const char* s, int is, int slen);
CC_INLINE int cstr_regex_match_star(regex_result_t* re, int ire, int nre, const char* p, int ip, int plen, const char* s, int is, int slen, int pa, int pb) {
  regex_result_t r1[1] = {{0}};
  int ir1 = cstr_regex_match(r1, 0, countof(r1), p, pa, pb, s, is, slen);
	UNUSED(nre);
  if (ir1>0 && is<r1[0].is) {
    //regex_result_t* re1 = re + ire;
    int is1 = r1[0].is;
    ire = cstr_regex_match_star(re, ire, 1, p, ip, plen, s, is1, slen, pa, pb);
  }
  ire = cstr_regex_match(re, ire, 1, p, ip, plen, s, is, slen);
  return ire;
}
CC_INLINE int cstr_regex_match_star_range(regex_result_t* re, int ire, int nre, const char* p, int ip, int plen, const char* s, int is, int slen, const uchar* umap) {
	UNUSED(nre);
  if (is<slen && umap[(uchar)s[is]]) {
    ire = cstr_regex_match_star_range(re, ire, 1, p, ip, plen, s, is+1, slen, umap);
  }
  ire = cstr_regex_match(re, ire, 1, p, ip, plen, s, is, slen);
  return ire;
}
CC_INLINE int cstr_regex_match_level1(regex_result_t* re, int ire, int nre, const char* p, int ip, int plen, const char* s, int is, int slen) {
  if (ip==plen) {
    ASSERT(ire<10);
    re[0].is = MAX(re[0].is, is);
    ire = 1;
  } else if ('('==p[ip]) {
    int p1 = ip+1;
    regex_result_t r1[1] = {{0}};
    int ind = 1, ir1;
    for (;ind && p1<plen;++p1) {
      char ch = p[p1];
      if ('\\'==ch && (p1+1)<plen) {
        ++p1;
      } else if ('('==ch) {
        ++ind;
      } else if (')'==ch) {
        --ind;
      }
    }
    if (p1<plen && '*'==p[p1]) {
      ire = cstr_regex_match_star(re, ire, 1, p, p1+1, plen, s, is, slen, ip+1, p1-1);
    } else {
      ir1 = cstr_regex_match(r1, 0, countof(r1), p, ip+1, p1-1, s, is, slen);
      if (ir1>0 && is<r1[0].is) {
        //regex_result_t* re1 = re + ire;
        int is1 = r1[0].is;
        ire = cstr_regex_match_level1(re, ire, 1, p, p1, plen, s, is1, slen);
      }
    }
  } else if ('.' == p[ip]) {
    if (ip<plen && '*'==p[ip+1]) {
      int p1 = ip+2;
      if (is<slen) {
        ire = cstr_regex_match_level1(re, ire, nre, p, p1, plen, s, is+1, slen);
        ire = cstr_regex_match_level1(re, ire, nre, p, ip, plen, s, is+1, slen);
      }
      ire = cstr_regex_match_level1(re, ire, nre, p, p1, plen, s, is, slen);
    } else {
      ire = cstr_regex_match_level1(re, ire, nre, p, ip+1, plen, s, is+1, slen);
    }
  } else if ('\\' == p[ip] && (ip+1)<plen && is<slen) {
    if (p[ip+1] == s[is]) {
      ire = cstr_regex_match_level1(re, ire, nre, p, ip+2, plen, s, is+1, slen);
    }
  } else if ('[' == p[ip] && (ip+1)<plen) {
    int p1 = ip+1;
    int ind = 1, val = '^'!=p[p1];
    uchar map[256] = {0};
    p1 += !val;
    memset(map, !val, countof(map));
    for (;ind && p1<plen;++p1) {
      char ch = p[p1];
      if ('\\'==ch && (p1+1)<plen) {
        ++p1;
      } else if ('['==ch) {
        ++ind;
      } else if (']'==ch) {
        --ind;
      } else if (1==ind) {
        if (p1+2<plen && '-'==p[p1+1] && ']'!=p[p1+2]) {
          int len = (uchar)p[p1+2] - (uchar)p[p1] + 1;
          if (len>0) {
            memset(map+(uchar)p[p1], val, len);
          }
          p1+=2;
        } else {
          map[(uchar)p[p1]] = val;
        }
      }
    }
    if (p1<plen && '*'==p[p1]) {
      ire = cstr_regex_match_star_range(re, ire, 1, p, p1+1, plen, s, is, slen, map);
    } else {
      if (is<slen && map[(uchar)s[is]]) {
        ire = cstr_regex_match_level1(re, ire, 1, p, p1, plen, s, is+1, slen);
      }
    }
  } else if ('*' == p[ip]) {
    ire = cstr_regex_match_level1(re, ire, nre, p, ip+1, plen, s, is+1, slen);
    ire = cstr_regex_match_level1(re, ire, nre, p, ip+1, plen, s, is, slen);
    ire = cstr_regex_match_level1(re, ire, nre, p, ip, plen, s, is+1, slen);
  } else if ('?' == p[ip]) {
    ire = cstr_regex_match_level1(re, ire, nre, p, ip+1, plen, s, is+1, slen);
  } else if ((ip+1)<plen && '*'==p[ip+1]) {
    int p1 = ip+2;
    if (is<slen && p[ip] == s[is]) {
      ire = cstr_regex_match_level1(re, ire, nre, p, ip, plen, s, is+1, slen);
      ire = cstr_regex_match_level1(re, ire, nre, p, p1, plen, s, is+1, slen);
    }
    ire = cstr_regex_match_level1(re, ire, nre, p, p1, plen, s, is, slen);
  } else if (is==slen) {
    return ire;
  } else if (p[ip] == s[is]) {
    ire = cstr_regex_match_level1(re, ire, nre, p, ip+1, plen, s, is+1, slen);
  }
  return ire;
}
CC_INLINE int cstr_regex_match(regex_result_t* re, int ire, int nre, const char* p, int ip, int plen, const char* s, int is, int slen)
{
  int p1 = ip, ind = 1;
  for (;ind && p1<plen;++p1) {
    char ch = p[p1];
    if ('\\'==ch && (p1+1)<plen) {
      ++p1;
    } else if ('('==ch) {
      ++ind;
    } else if (')'==ch) {
      --ind;
    } else if (1==ind && '|'==ch) {
      ire = cstr_regex_match(re, ire, nre, p, ip, p1, s, is, slen);
      ip = p1+1;
    }
  }
  ire = cstr_regex_match_level1(re, ire, nre, p, ip, plen, s, is, slen);
  return ire;
}
// Pattern Matching
// RMatch: recursively match s against pattern p, minplen
// is the min length string that can match p and
// numstars is the number of *'s in p
CC_INLINE BOOL RMatch(const char *s, const char *p, int slen, int minplen,int numstars)
{
  if (slen==0 && minplen==0)
    return TRUE;
  if (numstars==0 && minplen!=slen)
    return FALSE;
  if (minplen>slen)
    return FALSE;
  if (*p == '*') {
    return RMatch(s+1,p+1,slen-1,minplen,numstars-1) ||
      RMatch(s,p+1,slen,minplen,numstars-1) ||
      RMatch(s+1,p,slen-1,minplen,numstars);
  }
  if (*p == *s || *p == '?') {
    return RMatch(s+1,p+1,slen-1,minplen-1,numstars);
  } else {
    return FALSE;  
  }
}

/* EXPORT->DoMatch: return TRUE if s matches pattern p */
CC_INLINE BOOL DoMatch(const char *s, const char *p)
{
   int slen, minplen, numstars;
   const char *q;
   char c;
   
   slen = (int)strlen(s);
   minplen = 0; numstars = 0; q = p;
   while ((c=*q++))
      if (c == '*') ++numstars; else ++minplen;
   return RMatch(s,p,slen,minplen,numstars);
}

/* SpRMatch: recursively match s against pattern p, minplen
   is the min length string that can match p and
   numstars is the number of *'s in p 
	   spkr is next character of the spkr name */
CC_INLINE BOOL SpRMatch(char *s,char *p,char *spkr, int slen,int minplen,int numstars)
{
   BOOL match;
   
   if (slen==0 && minplen==0)
      match=TRUE;
   else if ((numstars==0 && minplen!=slen) || minplen>slen)
      match=FALSE;
   else if (*p == '*') {
      match=(SpRMatch(s+1,p,spkr,slen-1,minplen,numstars) ||
	     SpRMatch(s,p+1,spkr,slen,minplen,numstars-1) ||
	     SpRMatch(s+1,p+1,spkr,slen-1,minplen,numstars-1));
   }
   else if (*p == '%') {
      *spkr=*s,spkr[1]=0;
      match=SpRMatch(s+1,p+1,spkr+1,slen-1,minplen-1,numstars);
      if (!match) *spkr=0;
   }
   else if (*p == *s || *p == '?')
      match=SpRMatch(s+1,p+1,spkr,slen-1,minplen-1,numstars);
   else
      match=FALSE;
   
   return(match);
}
#define MAXSTRLEN 256
/* EXPORT->MaskMatch: return spkr if s matches pattern p */
CC_INLINE BOOL MaskMatch(char *mask, char *spkr, char *str)
{
  int spkrlen, slen, minplen, numstars;
  char *q,c;
  
  if (mask == NULL || str==NULL) return(FALSE);
  slen = (int)strlen(str);
  spkrlen = minplen = numstars = 0;
  q = mask;
  while ((c=*q++)) {
    if (c == '*') ++numstars; else ++minplen;
    if (c == '%') ++spkrlen;
  }
  if (spkrlen>=MAXSTRLEN) {
    printf("MaskMatch: Speaker name too long %d vs %d",spkrlen,MAXSTRLEN);
  }
  spkr[0]=0;
  if (SpRMatch(str,mask,spkr,slen,minplen,numstars)) {
    return(TRUE);
  }
  spkr[0]=0;
  return(FALSE);
}
CC_INLINE int cstr_shortstr(const char* s, int l, char* s2, int l2) {
  CSTRINITLEN(s, l);
  ASSERT(l2>0);
  if (l>l2) {
    int t1 = l2/2, t2 = l2-t1;
    memcpy(s2, s, t1);
    memcpy(s2+t1, s+l-t2, t2);
    if (l2>3) {
      s2[t1-1] = s2[t1] = s2[t1+1] = '.';
    }
    l = l2;
  } else {
    memcpy(s2, s, l);
  }
  return l;
}
CC_INLINE char* cstr_shortstr2(const char* s, int l, char* s2, int l2) {
  l2 = cstr_shortstr(s, l, s2, l2-1);
  s2[l2] = 0;
  return s2;
}

static int cstr_getline(str_t s, int* pi, str_t* ps) {
  int i = *pi, ret=0;
  ps->s = s.s+i;
  for ( ; i<s.l && s.s[i]!='\r' && s.s[i]!='\n'; ++i);
  ps->l = s.s+i-ps->s;
  for (; i<s.l && (s.s[i]=='\r' || s.s[i]=='\n'); ++i);
  ret = i>*pi;
  *pi = i;
  return ret;
}
//#define isalnum_(c)   (isalnum(c) || '_'==(c) || '-'==(c))
//#define isspace_(c)   (' '==(c) || '\t'==(c))
static int cstr_getstr(str_t s, int* pi, str_t* ps, char* str, int len, const char* delims) {
  int i = *pi, ret=0;
  str_t s2;
  uchar delimsmap[256] = {0};
  delims = delims ? delims : " \t";
  get_delimsmap(delimsmap, delims);
  ps = ps ? ps : &s2;
  for (; i<s.l && delimsmap[(uchar)s.s[i]]; ++i) {}
  ps->s = s.s+i;
  for (; i<s.l && !delimsmap[(uchar)s.s[i]]; ++i);
  ps->l = s.s+i-ps->s;
  if (str) {
    cstr_cpy(str, len, ps->s, ps->l);
  }
  ret = i>*pi;
  *pi = i;
  return ret;
}
static int cstr_getkv(str_t s, int* pi, str_t* pk, str_t* pv) {
  int ret=0;
  if (cstr_getstr(s, pi, pk, NULL, 0, " \t=") && s.s[*pi]=='=') {
    *pi += 1;
    ret = cstr_getstr(s, pi, pv, NULL, 0, " \t=");
  }
  return ret;
}
static int cstr_getint(str_t s, int* pi, int* pint, int def) {
  int i = *pi, ret=0;
  for (; i<s.l && !isdigit(s.s[i]); ++i) {}
  if (isdigit(s.s[i])) {
    char* s1 = (char*)s.s+i;
    def = strtol(s1, &s1, 10);
    i += s1 - (s.s+i);
  }
  ret = i>*pi;
  *pi = i;
  *pint = def;
  return ret;
}
static int cstr_getdouble(str_t s, int* pi, double* pd, double def) {
  int i = *pi, ret=0;
  for (; i<s.l && !isdigit(s.s[i]); ++i) {}
  if (isdigit(s.s[i])) {
    char* s1 = (char*)s.s+i;
    def = strtod(s1, &s1);
    i += s1 - (s.s+i);
  }
  ret = i>*pi;
  *pi = i;
  *pd = def;
  return ret;
}
static int cstr_getfloat(str_t s, int* pi, float* pf, float def) {
  double d = *pf;
  int ret = cstr_getdouble(s, pi, &d, def);
  *pf = d;
  return ret;
}

#define cc_isprint(c)     ((uchar)(c) >= (uchar)' ')
#define cc_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

CC_INLINE bool cc_isalnum(char c)
{
    return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

CC_INLINE bool cc_isalpha(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

CC_INLINE bool cc_isdigit(char c)
{
    return '0' <= c && c <= '9';
}

CC_INLINE bool cc_isspace(char c)
{
    return (9 <= c && c <= 13) || c == ' ';
}

CC_INLINE char* cc_skip_BOM(char* ptr)
{
    if((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      return ptr + 3;
    }
    return ptr;
}

static char* icc_itoa( int _val, char* buffer, int radix )
{
    char* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);
    radix = 10;

    *ptr = '\0';
    do
    {
        unsigned r = val / radix;
        *--ptr = (char)(val - (r*radix) + '0');
        val = r;
    }
    while( val != 0 );

    if( _val < 0 )
        *--ptr = '-';

    return ptr;
}

CC_INLINE bool cc_strcasecmp(const char * s1, const char * s2)
{
    size_t len1, len2, i;
    if ( s1 == 0 && s2 == 0 )
        return true;
    else if ( s1 == 0 || s2 == 0 )
        return false;

    len1 = strlen(s1);
    len2 = strlen(s2);
    if ( len1 != len2 )
        return false;

    for ( i = 0U; i < len1; i++ )
        if ( tolower( (int)(s1[i]) ) != tolower( (int)(s2[i]) ) )
            return false;

    return true;
}

#endif // _CSTR_H_
