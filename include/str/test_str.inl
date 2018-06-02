//#include "debug.h"

#include "cstd.h"
#include "str.h"
#include "test_inifile.inl"
//#include "test_strstr.inl"
#if 0
#include "test_mscript.inl"
#include "test_charenc.inl"
#include "test_charsetchecker.inl"
#include "test_str2f.inl"
#include "test_printf.inl"
#include "test_regex.inl"
#include "test_regexp.inl"
#include "test_FloatToStr.inl"
#include "test_sprintf.inl"
#include "test_mem_cpy.inl"
#endif
//#include "test_redft.inl"
// ×Ö·ûµÄÈ«½Ç¡¢°ë½Ç×ª»»£º
int fullToHalf(char* sFullStr, char* sHalfStr)
{
  int iFullStrIndex = 0;
  int iHalfStrIndex = 0;
  int iFullStrLen = strlen(sFullStr);
  printf("sFullStr[%s]\n", sFullStr);
  for (; iFullStrIndex < iFullStrLen; iFullStrIndex++, iHalfStrIndex++) {
    if (sFullStr[iFullStrIndex] == 0xA3) {
      sHalfStr[iHalfStrIndex] = sFullStr[++iFullStrIndex] - 0x80;
    }
    else {
      sHalfStr[iHalfStrIndex] = sFullStr[iFullStrIndex];
    }
  }
  printf("sHalfStr:[%s]\n", sHalfStr);
  return 0;
}
int halfToFull(char* sFullStr, char* sHalfStr)
{
  int iFullStrIndex = 0;
  int iHalfStrIndex = 0;
  int iHalfStrlen = strlen(sHalfStr);
  printf("sHalfStr[%s]\n", sHalfStr);
  for (; iHalfStrIndex < iHalfStrlen; iHalfStrIndex++, iFullStrIndex++) {
    if (sHalfStr[iHalfStrIndex] < 0x80) {
      sFullStr[iFullStrIndex] = (char)0xA3;
      sFullStr[++iFullStrIndex] = sHalfStr[iHalfStrIndex] + 0x80;
    }
    else {
      sFullStr[iFullStrIndex] = sHalfStr[iHalfStrIndex];
    }
  }
  printf("sFullStr:[%s]\n", sFullStr);
  return 0;
}

typedef struct {
  const char* cap[20];
  const char* s;
  const char* s_end;
  const char* p;
  const char* pstar;
} rmatchcap_t;

CC_INLINE int rmatch(rmatchcap_t* r, const char *p, const char *s)
{
  if (*p==0) {
    r->s = s;
    return TRUE;
  }
  if (*p == '*') {
    if (rmatch(r, p+1,s)) { // '*' match end
      r->cap[(p-r->p)*2+1] = s;
      return TRUE;
    } else if (s<r->s_end && rmatch(r, p,s+1)) { // '*' matched next char
      r->cap[(p-r->p)*2] = s;
      return TRUE;
    }
    return FALSE;
  }
  if (*p == *s || *p == '?') {
    return (s<r->s_end && rmatch(r, p+1,s+1));
  }
  return FALSE;
}
CC_INLINE int rmatch1(rmatchcap_t* r, const char *p, const char *s, int l) {
  r->p = p;
  r->s_end = s;
  return rmatch(r, p, s, l);
}

int test_rmatch() {
  char* s;
  int len, ret;
  rmatchcap_t r[1] = {0};
  s = "abc(34, 123) asdfa";
  len = strlen(s);
  ret = rmatch1(r, "*(*,*)", s, len);
  return 0;
}

CC_INLINE int test_str()
{
  str_t aa = {0}, bb = {0}, s2 = {0}, s3 = {0};
  str_setstr2(&aa, "sdasdfasdfasdfaasd sd");
  str_setstr2(&s2, "sd");
  str_setstr2(&s3, "tttsd");
  str_setstr1(&bb, aa);
  str_replace_str(&aa, s2, s3, 0);
  str_replace_str(&aa, s3, s2, 0);
  ASSERT(0 == str_cmp(aa, bb, 0));
  str_format(&aa, 0, "asdf, fasdf %13.5f %s %d", 5.66, " ff ff", 34);
  str_free(&aa);
  str_free(&bb);
  str_free(&s2);
  str_free(&s3);
  return 0;
}

