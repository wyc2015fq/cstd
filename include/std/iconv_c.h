
#ifndef _STDC_ICONV_C_H_
#define _STDC_ICONV_C_H_

#include "sys_c.h"
#include "stddef_c.h"
//#include "inttypes_c.h"

enum ICONV_CODEPAGE {
  ICONV_NULL,
  ICONV_GB2312,
  ICONV_UTF8,
  ICONV_UCS2LE,
};

//#define USE_MYCONV
//#define USE_WINCONV



static int isTextUTF8(const void* str0, int length)
{
	int i;
	size_t nBytes = 0;
	const unsigned char* str = (const unsigned char*)str0;
	BOOL bAllAscii = TRUE;
	for (i = 0; i < length; i++) {
		unsigned char chr = *(str + i);
		if ((chr & 0x80) != 0) {
			bAllAscii = FALSE;
		}
		if (nBytes == 0) {
			if (chr >= 0x80) {
				if (chr >= 0xFC && chr <= 0xFD) {
					nBytes = 6;
				}
				else if (chr >= 0xF8) {
					nBytes = 5;
				}
				else if (chr >= 0xF0) {
					nBytes = 4;
				}
				else if (chr >= 0xE0) {
					nBytes = 3;
				}
				else if (chr >= 0xC0) {
					nBytes = 2;
				}
				else {
					return FALSE;
				}
				nBytes--;
			}
		}
		else {
			if ((chr & 0xC0) != 0x80) {
				return FALSE;
			}
			nBytes--;
		}
	}
	if (nBytes > 0) {
		return FALSE;
	}
	if (bAllAscii) {
		return FALSE;
	}
	return TRUE;
}
typedef ICONV_CODEPAGE CodePage;
static CodePage GetCodePage(const void* str, int len)
{
	if (isTextUTF8(str, len)) {
		return ICONV_UTF8;
	}
	return ICONV_GB2312;
}



#ifdef _WIN32
#define USE_WINCONV
#else
#define USE_ICONV
#endif

#ifdef USE_WINCONV
static int codepage_w32(ICONV_CODEPAGE src_charset)
{
#define CVTCODEPAGE_DEF(DEF) \
  DEF(-1, ICONV_UCS2LE) \
  DEF(CP_ACP, ICONV_GB2312) \
  DEF(CP_UTF8, ICONV_UTF8)
#define DEF(a, b)   if (src_charset == b) return a;
  CVTCODEPAGE_DEF(DEF);
#undef DEF
#undef CVTCODEPAGE_DEF
  return -1;
}
static int iconv_win32(ICONV_CODEPAGE src_cp0, ICONV_CODEPAGE dst_cp0, const char* src, size_t srclen, char* dst, size_t dstlen)
{
  int outlen = 0;
  int src_cp = codepage_w32(src_cp0);
  int dst_cp = codepage_w32(dst_cp0);
  if (src_cp == dst_cp) {
    srclen = MIN(srclen, dstlen);
    memcpy(dst, src, srclen);
    return srclen;
  }
  if (src_cp != -1 && dst_cp != -1) {
#if 1
    wchar_t* tmp_ = NULL;
    int count = srclen;
    tmp_ = (wchar_t*)malloc(count * sizeof(wchar_t));
    count = MultiByteToWideChar(src_cp, 0, src, srclen, tmp_, count);
    outlen = WideCharToMultiByte(dst_cp, 0, tmp_, count, dst, dstlen, NULL, NULL);
    if (tmp_) {free(tmp_); tmp_ = NULL;}
#else
    wchar_t* tmp_[256];
    int srci = 0;
    for () {
      count = 256;
      int srclen0 = MIN(srclen - srci, 256);
      count = MultiByteToWideChar(src_cp, 0, src + srci, srclen0, tmp_, count);
      outlen = WideCharToMultiByte(dst_cp, 0, tmp_, count, dst, dstlen, NULL, NULL);
    }
#endif
  } else if (src_cp != -1) {
    outlen = 2 * MultiByteToWideChar(src_cp, 0, src, srclen, (wchar_t*)dst, dstlen / 2);
  } else if (dst_cp != -1) {
    outlen = WideCharToMultiByte(dst_cp, 0, (wchar_t*)src, srclen / 2, dst, dstlen, NULL, NULL);
  }
  dst[outlen] = 0;
  return outlen;
}
#endif

#ifdef USE_ICONV
#include <iconv.h>
#include <wchar.h>

static const char* codepage_linux(ICONV_CODEPAGE src_charset)
{
#define CVTCODEPAGE_DEF(DEF) \
  DEF("UCS-2LE", ICONV_UCS2LE) \
  DEF("gb2312", ICONV_GB2312) \
  DEF("utf-8", ICONV_UTF8)
#define DEF(a, b)   if (src_charset == b) return a;
  CVTCODEPAGE_DEF(DEF);
#undef DEF
#undef CVTCODEPAGE_DEF
  return "utf-8";
}
static int iconv_linux(ICONV_CODEPAGE src_cp0, ICONV_CODEPAGE dst_cp0, const char* src, size_t srclen, char* dst, size_t dstlen)
{
  int outlen = 0;
  char* inbuf = (char*)(src);
  size_t inlen = srclen;
  const char* src_charset = codepage_linux(src_cp0);
  const char* dst_charset = codepage_linux(dst_cp0);
  iconv_t cd = iconv_open(dst_charset, src_charset);//获取�?换句柄，void*类型
  if (cd == 0) {
    return 0;
  }
#if 0
  char outbuf[255];//这里实在不知道需要�?�少�?字节，这�?�?�?�?
  while (1) {
    size_t buflen = 255;
    char* poutbuf = outbuf;
    memset(outbuf, 0, dstlen);
    if (iconv(cd, &inbuf, &inlen, &poutbuf, &buflen) == -1) {
      break;
    }
    int len = 255 - buflen;
    if (len < (dstlen - outlen)) {
      memcpy(dst + outlen, outbuf, len);
      outlen += len;
    } else {
      break;
    }
  }
#else
  size_t buflen = dstlen;
  char* poutbuf = dst;
  if (iconv(cd, &inbuf, &inlen, &poutbuf, &buflen) == -1) {
    printf("error: iconv\n");
  }
  outlen = dstlen - buflen;
#endif
  iconv_close(cd);
  dst[outlen] = 0;
  return outlen;
}
#endif // USE_ICONV

static ICONV_CODEPAGE mycodepage(const char* src_charset)
{
#define CVTCODEPAGE_DEF(DEF) \
  DEF(ICONV_UCS2LE, "UCS-2LE") \
  DEF(ICONV_GB2312, "gb2312") \
  DEF(ICONV_UTF8, "utf-8") \
  DEF(ICONV_UTF8, "utf8")
#define DEF(a, b)   if (0==_stricmp(src_charset, b)) return a;
  CVTCODEPAGE_DEF(DEF);
#undef DEF
#undef CVTCODEPAGE_DEF
  return ICONV_NULL;
}

#ifdef USE_MYCONV
#include "unicode.h"

static int iconv_my(ICONV_CODEPAGE src_cp, ICONV_CODEPAGE dst_cp, const char* src, size_t srclen, char* dst, size_t dstlen)
{
  int outlen = 0;
#define CODE_CONVERT_DEF(DEF) \
  DEF(ICONV_UCS2LE, ICONV_GB2312, ucs2le_to_gb) \
  DEF(ICONV_UCS2LE, ICONV_UTF8, ucs2le_to_utf8) \
  DEF(ICONV_GB2312, ICONV_UCS2LE, gb_to_ucs2le) \
  DEF(ICONV_GB2312, ICONV_UTF8, gb_to_utf8) \
  DEF(ICONV_UTF8, ICONV_UCS2LE, utf8_to_ucs2le) \
  DEF(ICONV_UTF8, ICONV_GB2312, utf8_to_gb)
#define DEF(a, b, c)  if (a==src_cp && b==dst_cp) {outlen = c(dst, dstlen, src, srclen);}
  CODE_CONVERT_DEF(DEF);
#undef DEF
#undef CODE_CONVERT_DEF
  dst[outlen] = 0;
  return outlen;
}
#endif

static int ustrlen(const char* str, ICONV_CODEPAGE cp)
{
  switch (cp) {
  case ICONV_UCS2LE:
    return 2 * wcslen((const wchar_t*)str);
  }
  return strlen(str);
}

static int iconv_c(ICONV_CODEPAGE src_cp, ICONV_CODEPAGE dst_cp, const char* src, int srclen, char* dst, int dstlen)
{
  srclen = srclen < 0 ? ustrlen(src, src_cp) : srclen;
#ifdef USE_MYCONV
  return iconv_my(src_cp, dst_cp, src, srclen, dst, dstlen);
#endif
#ifdef USE_WINCONV
  return iconv_win32(src_cp, dst_cp, src, srclen, dst, dstlen);
#endif
#ifdef USE_ICONV
  return iconv_linux(src_cp, dst_cp, src, srclen, dst, dstlen);
#endif
  return 0;
}

//编码�?�?，src_charset�?源编码，dst_charset�?�?标编�?
//src�?源编码字符串
static int iconv_cs(const char* src_charset, const char* dst_charset, const char* src, int srclen, char* dst, int dstlen)
{
  ICONV_CODEPAGE src_cp = mycodepage(src_charset);
  ICONV_CODEPAGE dst_cp = mycodepage(dst_charset);
  return iconv_c(src_cp, dst_cp, src, srclen, dst, dstlen);
}

/////////////////////////////////////////////////////////////////////////////
static int iconv_wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
{
  if (count == 0 && mbstr != NULL) {
    return 0;
  }
  int result = iconv_cs("UCS-2LE", "gb2312", (const char*)wcstr, count * 2, mbstr, count * 4);
  if (result > 0) {
    mbstr[result] = 0;
  }
  return result;
}

static int iconv_mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
{
  if (count == 0 && wcstr != NULL) {
    return 0;
  }
  int result = iconv_cs("gb2312", "UCS-2LE", mbstr, count, (char*)wcstr, count * 2);
  if (result > 0) {
    wcstr[result / 2] = 0;
  }
  return result;
}
static wchar_t* iconv_a2w(const char* a, size_t count) {
  count = count <= 0 ? strlen(a) : count;
  wchar_t* w = (wchar_t*)malloc(count * sizeof(wchar_t));
  iconv_mbstowcsz(w, a, count);
  return w;
}
static char* iconv_w2a(const wchar_t* w, size_t count) {
  count = count <= 0 ? wcslen(w) : count;
  char* a = (char*)malloc(count * sizeof(char));
  iconv_wcstombsz(a, w, count);
  return a;
}

#endif // _STDC_ICONV_C_H_
