#ifndef _INIFILE_H_
#define _INIFILE_H_
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef CC_INLINE
#ifdef __cplusplus
#define CC_INLINE static inline
#else
#define CC_INLINE static
#endif
#endif // CC_INLINE
#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif
typedef struct string_t {
  char* beg, *end;
} string_t;
#define string_t_len(st)  (st.end-st.beg)
typedef struct config_tree {
  string_t name;
  string_t value;
  //struct tree_head tree;
} config_tree;
CC_INLINE int conf_ini_remove_space(const char* src, char* dst0)
{
  char* dst = dst0;
  int is_sp_char = 1;
  for (; *src; ++src) {
    char ch;
    ch = *src;
    if (' ' != ch || !is_sp_char) {
      is_sp_char = ('\n' == ch || '\r' == ch || '[' == ch || ']' == ch || '=' == ch);
      if (is_sp_char) {
        for (; dst != dst0 && ' ' == *(dst - 1);) {
          --dst;
        }
      }
      if ('\r' == ch) {
        ch = '\n';
      }
      if (!(dst0 < dst && '\n' == dst[-1] && '\n' == ch)) {
        *dst++ = ch;
      }
    }
  }
  if (dst0 < dst && '\n' != dst[-1]) {
    *dst++ = '\n';
  }
  *dst = 0;
  return 0;
}
#if 0
char* to_c_str(string_t st, char* buf, int buflen)
{
  int len = MIN(string_t_len(st), buflen);
  memcpy(buf, st.beg, len);
  buf[len] = 0;
  return buf;
}
config_tree* conf_ini_parse(const char* src, char* buf, int buflen)
{
  config_tree* ct, *tct;
  string_t t, a;
  char buf1[1000];
  char buf2[1000];
  BUFUSEBEGIN(buf, buflen);
  ct = BUFMALLOC(config_tree, 1);
  INIT_TREE_HEAD(&ct->tree);
  for (; 1;) {
    if ('#' == *src) {
      src = strchr(src, '\n');
      continue;
    }
    if ((t.beg = ('[' == *src) ? (char*)(src + 1) : NULL) && (t.end = strchr(t.beg, ']'))) {
      printf("[%s]\n", to_c_str(t, buf1, 1000));
      tct = BUFMALLOC(config_tree, 1);
      INIT_TREE_HEAD(&tct->tree);
      tct->name = t;
      tct->value.beg = tct->value.beg = 0;
      tree_add_brother_tail(&tct->tree, &ct->tree);
      for (src = t.end + 1; ('\r' == *src) || ('\n' == *src); ++src) {}
      for (; 1;) {
        if ('#' == *src) {
          src = strchr(src, '\n');
          continue;
        }
        if ('[' != *src
            && (t.beg = (char*)src)
            && (t.end = strchr(t.beg, '='))
            && (a.beg = t.end + 1)
            && (a.end = strchr(a.beg, '\n'))) {
          printf("%s=%s\n", to_c_str(t, buf1, 1000), to_c_str(a, buf2, 1000));
          tct = BUFMALLOC(config_tree, 1);
          INIT_TREE_HEAD(&tct->tree);
          tct->name = t;
          tct->value = a;
          tree_add_child_tail(&tct->tree, &ct->tree);
          for (src = a.end + 1; ('\r' == *src) || ('\n' == *src); ++src) {}
        }
        else {
          break;
        }
      }
    }
    else {
      break;
    }
  }
  BUFUSEEND();
  return 0;
}
#endif
#define touch(_FNAME)  {FILE* pf = fopen(_FNAME, "ab"); if (pf) fclose(pf); }
CC_INLINE void* memmapfile(const char* filename, int iscreate, int* pfilelen)
{
  FILE* pf;
  char* buf = NULL;
  pf = fopen(filename, "rb");
  if (NULL == pf && iscreate) {
    touch(filename);
    pf = fopen(filename, "rb");
  }
  if (pf) {
    int filelen;
    fseek(pf, 0, SEEK_END);
    filelen = ftell(pf);
    fseek(pf, 0, SEEK_SET);
    buf = (char*)malloc(filelen + 2);
    fread(buf, filelen, 1, pf);
    if (pfilelen) {
      *pfilelen = filelen;
    }
    buf[filelen] = 0;
  }
  return buf;
}
CC_INLINE int conf_ini_find_key(const char* src0, const char* sec0, const char* key0, string_t* name, string_t* value)
{
  int srclen = strlen(src0);
  int seclen = strlen(sec0);
  int keylen = strlen(key0);
  char* src = (char*)src0;
  char* sec, *key, *sec1, *sec2;
  for (sec2 = NULL; (sec = strstr(src, sec0)); src = sec + seclen) {
    if (sec > src0 && '[' == sec[-1] && sec[seclen] == ']') {
      sec2 = sec;
      sec1 = strchr(sec, '[');
      if (NULL == sec1) {
        sec1 = src + srclen;
      }
      for (; (key = strstr(sec, key0)) && key < sec1; sec = key + keylen) {
        if (('\n' == key[-1] || '\r' == key[-1]) && key[keylen] == '=') {
          if (name) {
            name->beg = key;
            name->end = key + keylen;
          }
          if (value) {
            value->beg = key + keylen + 1;
            value->end = strchr(name->beg, '\n');
          }
          return 1;
        }
      }
    }
  }
  if (sec2 && (sec = strchr(sec2, ']'))) {
    sec += 2;
    if (name) {
      name->beg = sec;
      name->end = sec;
    }
    if (value) {
      value->beg = sec;
      value->end = sec;
    }
    return 1;
  }
  return 0;
}
CC_INLINE int GetIniString(const char* section, const char* key, char* vlaue, int vlauelen, const char* filename)
{
  char* buf;
  int filelen;
  int len = 0;
  string_t name = {0}, oldvalue = {0};
  buf = (char*)memmapfile(filename, 0, &filelen);
  conf_ini_remove_space(buf, buf);
  if (conf_ini_find_key(buf, section, key, &name, &oldvalue) && string_t_len(name) == (int)strlen(key)) {
    len = MIN(vlauelen - 1, string_t_len(oldvalue));
    memcpy(vlaue, oldvalue.beg, len);
    vlaue[len] = 0;
  }
  free(buf);
  return len;
}
CC_INLINE double GetIniNumber(const char* section, const char* key, double defult, const char* filename)
{
  char buf[64];
  double d = defult;
  if (GetIniString(section, key, buf, 64, filename) > 0) {
    char* end;
    d = strtod(buf, &end);
  }
  return d;
}
CC_INLINE int SetIniString(const char* section, const char* key, const char* vlaue, const char* filename)
{
  char* buf;
  int filelen;
  int vlauelen = strlen(vlaue);
  string_t name, oldvalue;
  buf = (char*)memmapfile(filename, 1, &filelen);
  conf_ini_remove_space(buf, buf);
  if (conf_ini_find_key(buf, section, key, &name, &oldvalue)) {
    //if (string_t_len(oldvalue)!=vlauelen || 0!=memcmp(oldvalue.beg, vlaue, vlauelen))
    {
      FILE* pf = fopen(filename, "wb");
      if (pf) {
        fwrite(buf, name.beg - buf, 1, pf);
        if (string_t_len(name) == 0) {
          oldvalue = name;
          fprintf(pf, "%s=%s\n", key, vlaue);
        }
        else {
          fprintf(pf, "%s=%s", key, vlaue);
        }
        fwrite(oldvalue.end, strlen(oldvalue.end), 1, pf);
        fclose(pf);
      }
    }
  }
  else {
    FILE* pf = fopen(filename, "a");
    if (pf) {
      fprintf(pf, "[%s]\n%s=%s\n", section, key, vlaue);
      fclose(pf);
    }
  }
  free(buf);
  return 1;
}
CC_INLINE int SetIniNumber(const char* section, const char* key, double d, const char* filename)
{
  char buf[64];
  _snprintf(buf, 64, "%30.30lg", d);
  return SetIniString(section, key, buf, filename);
}
#if 0
#ifdef _WIN
#include <windows.h>
#define IniGetInt(filename, lpAppName, lpKeyName, nDefault) \
  GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, filename)
static int IniSetFloat(const char* filename, const char* lpAppName, const char* lpKeyName, double numb)
{
  char buf[100];
  _snprintf(buf, 100, "%g", numb);
  return WritePrivateProfileString(lpAppName, lpKeyName, buf, filename);
}
#define IniGetString(filename, lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize) \
  GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, filename) \
#define IniWriteString(filename, lpAppName, lpKeyName, lpString) \
  WritePrivateProfileString(lpAppName, lpKeyName, lpString, filename)
#define IniGetSection(filename, lpAppName, lpReturnedString, nSize) \
  GetPrivateProfileSection(lpAppName, lpReturnedString, nSize, filename)
#define IniWriteSection(filename, lpAppName, lpString) \
  WritePrivateProfileSection(lpAppName, lpString, filename)
#define IniGetSectionNames(filename, lpReturnedString, nSize) \
  GetPrivateProfileSectionNames(lpReturnedString, nSize, filename)
#define IniGetStruct(filename, lpszSection, lpszKey, lpStruct, uSizeStruct) \
  GetPrivateProfileStruct(filename, lpszSection, lpszKey, lpStruct, uSizeStruct, filename)
#define IniWriteStruct(filename, lpszSection, lpszKey, lpStruct, uSizeStruct) \
  WritePrivateProfileStruct(lpszSection, lpszKey, lpStruct, uSizeStruct, filename)
  #endif // _WIN
  #else
#define IniGetInt(filename, lpAppName, lpKeyName, nDefault) GetIniNumber( lpAppName, lpKeyName, nDefault, filename )
#define IniSetFloat(filename, lpAppName, lpKeyName, numb) SetIniNumber( lpAppName, lpKeyName, numb, filename )
#define IniGetString(filename, lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize) GetIniString( lpAppName, lpKeyName, lpReturnedString, nSize, filename )
#define IniWriteString(filename, lpAppName, lpKeyName, lpString) SetIniString(lpAppName, lpKeyName, lpString, filename)
#define IniGetSection(filename, lpAppName, lpReturnedString, nSize)  ASSERT(0)
#define IniWriteSection(filename, lpAppName, lpString)   ASSERT(0)
#define IniGetSectionNames(filename, lpReturnedString, nSize) ASSERT(0)
#define IniGetStruct(filename, lpszSection, lpszKey, lpStruct, uSizeStruct) ASSERT(0)
#define IniWriteStruct(filename, lpszSection, lpszKey, lpStruct, uSizeStruct) ASSERT(0)
  #endif
#define ATOF(STR, FLT)  sscanf(STR, "%lf", &(FLT))
#define IniGetFloat(filename, lpAppName, lpKeyName, fReturned)                \
  do {                                                                          \
    char buf[64]; double dTemp=fReturned;                                       \
    IniGetString(filename, lpAppName, lpKeyName, "", buf, 64);                  \
    ATOF(buf, dTemp); fReturned = dTemp;                                        \
  } while(0)
    static char __buf[ 64 ];
#define IniGetFloat2(filename, lpAppName, lpKeyName)    \
  (IniGetString(filename, lpAppName, lpKeyName, "0", __buf, 64), atof(__buf))
  static char* _inifilename()
  {
    static char __inifilename[1024] = ".\\test.ini";
    return __inifilename;
  }
  static char* _iniappname()
  {
    static char __iniappname[256] = "test";
    return __iniappname;
  }
#define _INIFILENAME _inifilename()
#define _INIAPPNAME _iniappname()
#define ininame(_name)   strcpy(_INIFILENAME, _name)
#define iniappname(_appname)   strcpy(_INIAPPNAME, _appname)
#define inigetint(lpKeyName)   (int)IniGetInt(_INIFILENAME, _INIAPPNAME, lpKeyName, 0)
  static char* inigetstr(const char* lpKeyName)
  {
    static char inigetstrbuf[256] = "";
    IniGetString(_INIFILENAME, _INIAPPNAME, lpKeyName, "", inigetstrbuf, 256);
    return inigetstrbuf;
  }
  static int inisetstr(const char* lpKeyName, const char* lpString)
  {
    return IniWriteString(_INIFILENAME, _INIAPPNAME, lpKeyName, lpString);
  }
  static int inisetnumber(const char* lpKeyName, double numb)
  {
    char buf[100];
    _snprintf(buf, 100, "%g", numb);
    return IniWriteString(_INIFILENAME, _INIAPPNAME, lpKeyName, buf);
  }
  static int inisetint(const char* lpKeyName, int numb)
  {
    char buf[100];
    _snprintf(buf, 100, "%d", numb);
    return IniWriteString(_INIFILENAME, _INIAPPNAME, lpKeyName, buf);
  }
  double inigetfloat(const char* lpKeyName)
  {
    char* str = inigetstr(lpKeyName);
    double fReturned = 0;
    ATOF(str, fReturned);
    return fReturned;
  }
  #endif // _INIFILE_H_

