
#ifndef _STDC_ICONV_C_H_
#define _STDC_ICONV_C_H_

#include "stddef_c.h"

#define USE_MYCONV
//#define USE_WINCONV

#ifdef _WIN32
//#define USE_WINCONV
#endif

#ifdef USE_WINCONV
#include <windows.h>
static int codepage_w32(const char *src_charset) {
#define CVTCODEPAGE_DEF(DEF) \
  DEF(-1, "UCS-2LE") \
  DEF(CP_ACP, "gb2312") \
  DEF(CP_UTF8, "utf-8")
#define DEF(a, b)   if (0==stricmp(src_charset, b)) return a;
  CVTCODEPAGE_DEF(DEF);
#undef DEF
#undef CVTCODEPAGE_DEF
  return -1;
}
static int iconv_win32(const char *src_charset, const char *dst_charset, const char* src, size_t srclen, char* dst, size_t dstlen) {
  int outlen=0;
  int src_codepage = codepage_w32(src_charset);
  int dst_codepage = codepage_w32(dst_charset);
  if (src_codepage==dst_codepage) {
    srclen = MIN(srclen, dstlen);
    memcpy(dst, src, srclen);
    return srclen;
  }
  if (src_codepage!=-1 && dst_codepage!=-1) {
#if 1
    wchar_t* tmp_ = NULL;
    int count = srclen;
    tmp_ = (wchar_t*)malloc(count*sizeof(wchar_t));
    count = MultiByteToWideChar(src_codepage, 0, src, srclen, tmp_, count);
    outlen = WideCharToMultiByte(dst_codepage, 0, tmp_, count, dst, dstlen, NULL, NULL);
    if (tmp_) {free(tmp_);tmp_=NULL;}
#else
    wchar_t* tmp_[256];
    int srci = 0;
    for () {
      count = 256;
      int srclen0 = MIN(srclen-srci, 256);
      count = MultiByteToWideChar(src_codepage, 0, src+srci, srclen0, tmp_, count);
      outlen = WideCharToMultiByte(dst_codepage, 0, tmp_, count, dst, dstlen, NULL, NULL);
    }
#endif
  } else if (src_codepage!=-1) {
    outlen = 2*MultiByteToWideChar(src_codepage, 0, src, srclen, (wchar_t*)dst, dstlen/2);
  } else if (dst_codepage!=-1) {
    outlen = WideCharToMultiByte(dst_codepage, 0, (wchar_t*)src, srclen/2, dst, dstlen, NULL, NULL);
  }
  return outlen;
}
#endif

#ifdef USE_ICONV
#include <iconv.h>
static int iconv_linux(const char *src_charset, const char *dst_charset, const char* src, size_t srclen, char* dst, size_t dstlen) {
  char* inbuf = (char*)(src);
  size_t inlen = srclen;
  iconv_t cd = iconv_open(dst_charset, src_charset);//获取转换句柄，void*类型
  if (cd == 0)
    return 0;
#if 0
  char outbuf[255];//这里实在不知道需要多少个字节，这是个问题
  while (1) {
    size_t buflen = 255;
    char *poutbuf = outbuf;
    memset(outbuf, 0, dstlen);
    if (iconv(cd, &inbuf, &inlen, &poutbuf,&buflen) == -1) {
      break;
    }
    int len = 255-buflen;
    if (len<(dstlen-outlen)) {
      memcpy(dst+outlen, outbuf, len);
      outlen += len;
    } else {
      break;
    }
  }
#else
  size_t buflen = dstlen;
  char *poutbuf = dst;
  if (iconv(cd, &inbuf, &inlen, &poutbuf,&buflen) == -1) {
    printf("error: iconv\n");
  }
  printf("error: ok\n");
  outlen = dstlen-buflen;
#endif
  
  printf("error: ok\n");
  iconv_close(cd);
  return outlen;
  }
#endif // USE_ICONV

#ifdef USE_MYCONV
#include "unicode.h"
enum ICONV_CODEPAGE{
  ICONV_NULL,
    ICONV_GB2312,
    ICONV_UTF8,
    ICONV_UCS2LE,
};
static ICONV_CODEPAGE mycodepage(const char *src_charset) {
#define CVTCODEPAGE_DEF(DEF) \
  DEF(ICONV_UCS2LE, "UCS-2LE") \
  DEF(ICONV_GB2312, "gb2312") \
  DEF(ICONV_UTF8, "utf-8") \
  DEF(ICONV_UTF8, "utf8")
#define DEF(a, b)   if (0==stricmp(src_charset, b)) return a;
  CVTCODEPAGE_DEF(DEF);
#undef DEF
#undef CVTCODEPAGE_DEF
  return ICONV_NULL;
}

static int iconv_my(const char *src_charset, const char *dst_charset, const char* src, size_t srclen, char* dst, size_t dstlen)
{
  int outlen = 0;
  ICONV_CODEPAGE src_codepage = mycodepage(src_charset);
  ICONV_CODEPAGE dst_codepage = mycodepage(dst_charset);
#define CODE_CONVERT_DEF(DEF) \
  DEF(ICONV_UCS2LE, ICONV_GB2312, ucs2le_to_gb) \
  DEF(ICONV_UCS2LE, ICONV_UTF8, ucs2le_to_utf8) \
  DEF(ICONV_GB2312, ICONV_UCS2LE, gb_to_ucs2le) \
  DEF(ICONV_GB2312, ICONV_UTF8, gb_to_utf8) \
  DEF(ICONV_UTF8, ICONV_UCS2LE, utf8_to_ucs2le) \
  DEF(ICONV_UTF8, ICONV_GB2312, utf8_to_gb)
#define DEF(a, b, c)  if (a==src_codepage && b==dst_codepage) {outlen = c(dst, dstlen, src, srclen);}
  CODE_CONVERT_DEF(DEF)
#undef DEF
#undef CODE_CONVERT_DEF
    return outlen;
  }
#endif

//编码转换，src_charset是源编码，dst_charset是目标编码
//src是源编码字符串
int iconv_c(const char *src_charset, const char *dst_charset, const char* src, size_t srclen, char* dst, size_t dstlen)
{
#ifdef USE_MYCONV
  return iconv_my(src_charset, dst_charset, src, srclen, dst, dstlen);
#endif
#ifdef USE_WINCONV
  return iconv_win32(src_charset, dst_charset, src, srclen, dst, dstlen);
#endif
#ifdef USE_ICONV
  return iconv_linux(src_charset, dst_charset, src, srclen, dst, dstlen);
#endif
  return 0;
}

#endif // _STDC_ICONV_C_H_
