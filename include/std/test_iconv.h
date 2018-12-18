
#include "iconv_c.h"
#include "log_c.h"



int test_iconv_case(const char *src_charset, const char *dst_charset, const char* src, size_t srclen, const char* dst, size_t dstlen) {
  int buflen = srclen+dstlen;
  char* buf = (char*)malloc(buflen);
  buflen = iconv_c(src_charset, dst_charset, src, srclen, buf, buflen);
  assert(buflen==dstlen);
  assert(0==memcmp(dst, buf, dstlen));
  free(buf);
  return 0;
}
int test_iconv_all(const char* gbk, int gbklen, const char* uni, int unilen, const char* utf, int utflen) {
#define CODE_CONVERT_DEF(DEF) \
  DEF("utf-8", "UCS-2LE", utf, uni) \
  DEF("UCS-2LE", "gb2312", uni, gbk) \
  DEF("UCS-2LE", "utf-8", uni, utf) \
  DEF("gb2312", "UCS-2LE", gbk, uni) \
  DEF("gb2312", "utf-8", gbk, utf) \
  DEF("utf-8", "gb2312", utf, gbk)
  
#define DEF(a, b, src, dst)  test_iconv_case(a, b, src, src ## len, dst, dst ## len);
  CODE_CONVERT_DEF(DEF);
#undef DEF
#undef CODE_CONVERT_DEF
  return 0;
}

int test_iconv()
{
  test_StringList();
  // Œ“
  char gbk[] = {0xce, 0xd2, 0x00};
  char uni[]= {0x11,0x62, 0x00}; 
  char utf[] = {0xe6, 0x88, 0x91,0x00};
  //_stricoll(gbk, uni);
  test_iconv_all(gbk, 2, uni, 2, utf, 3);
  char buf[8] = {0};
  printf("gbk=%s\n", gbk);
  iconv_c("ucs-2le", "gb2312", uni, 2, buf, 8);
  printf("uni=%s\n", buf);
  iconv_c("utf-8", "gb2312", utf, 3, buf, 8);
  printf("utf=%s\n", buf);
  return 0;
}
