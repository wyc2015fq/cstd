
#ifndef _IMGIOEX_INL_
#define _IMGIOEX_INL_

#include "imgio.inl"
#include "codec/base64.inl"

CC_INLINE int imread_base64(const char* base64, int cn_req, img_t* im) {
  int ret = 0;
  int base64_len = strlen(base64);
  int len = base64_len*2;
  char* buf = MALLOC(char, len);
  len = base64_decode(base64, base64_len, buf, len);
  savefile("aaaa.jpg", buf, len);
  ret = imload_mem(buf, len, cn_req, im);
  FREE(buf);
  return ret;
}

#endif // _IMGIOEX_INL_
