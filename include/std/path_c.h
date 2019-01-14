
#include "string_c.h"

#define FN_IS_DOTS(_FN)      (((_FN)[0] == '.') && ((_FN)[1] == '\0' || ((_FN)[1] == '.' && (_FN)[2] == '\0')))

static char* GetFilePathCopy(const char* fn, char* szFilePath, int MaxPathLen)
{
  int i;
  i = (int)strlen(fn);
  if (i > 0 && (fn[i] == '\\' || fn[i] == '/')) {
    --i;
  }
  while (i > 0 && (fn[i] != '\\' && fn[i] != '/')) {
    i--;
  }
  if (i >= MaxPathLen) {
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
  for (; pos != fn && (pos[-1] != '\\' && pos[-1] != '/'); --pos) {
    if ('.' == pos[-1]) {
      return pos;
    }
  }
  return (char*)fn + len;
}
static char* GetFileNameExt(const char* fn)
{
  int i = (int)strlen(fn) - 1;
  while (i >= 0 && (fn[i] != '\\' && fn[i] != '/')) {
    i--;
  }
  if (i < 0) {
    return (char*)fn;
  }
  return (char*)(fn + i + 1);
}
static char* ReplaceExt(const char* fn, const char* ext, char* out, int outlen)
{
  const char* oldext = GetFileExt(fn);
  size_t pos = (oldext && *oldext) ? (oldext - fn) : strlen(fn);
  while (*ext && *ext == '.') {
    ++ext;
  }
  IRANGE r = iRANGE(0, outlen);
  int extlen = strlen(ext);
  if (*oldext) {
    strcat2_c(out, &r, fn, pos, ext, extlen);
  } else {
    strcat3_c(out, &r, fn, pos, ".", 1, ext, extlen);
  }
  return out;
}

static char* GetPathFileNameExtN(const char* fn, int lastn)
{
  int i = (int)strlen(fn) - 1, j = 0;
  for (; i >= 0; ) {
    for (; i >= 0 && (fn[i] != '\\' && fn[i] != '/'); --i);
    ++j;
    if (j > lastn) {
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
static char* GetBeforeCopy(const char* fn, char* buf, int buflen, int ch)
{
  int len = (int)strlen(fn);
  int i = len - 1;
  while (i >= 0 && (fn[i] != ch)) {
    i--;
  }
  if (i < 0) {
    i = len;
  }
  if (i >= buflen) {
    return NULL;
  }
  i = MIN(i, buflen - 1);
  strncpy(buf, fn, i);
  buf[i] = 0;
  return buf;
}
static char* GetPathFileNameCopy(const char* fn, char* buf, int buflen)
{
  return GetBeforeCopy(fn, buf, buflen, '.');
}
static char* GetFileNameCopy(const char* fn, char* buf, int buflen)
{
  char* name_ext = GetFileNameExt(fn);
  return GetBeforeCopy(name_ext, buf, buflen, '.');
}