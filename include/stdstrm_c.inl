#include <stdio.h>
#include <string.h>
#include <assert.h>
#ifdef STDSTRM_C_INL
#include "macro.h"
#else
#include "../../include/stdstrm.h"
#include "../../include/macro.h"
#endif //inl
static TCHAR* _tcstolower(TCHAR* s)
{
  TCHAR* s0 = s;
  UNUSED(_tcstolower);

  for (; *s; ++s) {
    *s = (TCHAR)_totlower((int) * s);
  }

  return s0;
}
static int my_fread(shandle_t h, data_t d, int s)
{
  int aaa;
  aaa = fread(d, 1, s, (FILE*) h);
  return aaa;
}
static int my_fwrite(shandle_t h, data_t d, int s)
{
  return fwrite(d, 1, s, (FILE*) h);
}
static int my_fseek(shandle_t h, int o, int l)
{
  int aa;
  aa = fseek((FILE*) h, o, l);
  aa = ftell((FILE*) h);
  return aa;
}
// "fileio.h"
static int my_fclose(shandle_t h)
{
  fclose((FILE*) h);
  return 0;
}
static int my_fsize(shandle_t h)
{
  int _cur = ftell((FILE*) h);
  int _SZ;
  //fpos_t pos;
  fseek((FILE*) h, 0, SEEK_END);
  //fgetpos(_PF, &pos);  _SZ = pos;
  _SZ = ftell((FILE*) h);
  fseek((FILE*) h, _cur, SEEK_SET);
  return _SZ;
}
static STRM* NEWSTRM(size_t exsize)
{
  STRM* s;
  UNUSED(exsize);
  s = MALLOC(STRM, 1);
  memset((void*)s, 0, sizeof(STRM));
  return s;
}
static int my_ftell(shandle_t h)
{
  return ftell((FILE*) h);
}
static STRM* sopen_file(const TCHAR* filename, const TCHAR* mode)
{
  STRM* s = 0;
  void* h;
  h = _tfopen(filename, mode);

  if (h) {
    s = NEWSTRM(0);
    s->h = (shandle_t) h;
    s->seek = my_fseek;
    s->read = my_fread;
    s->write = my_fwrite;
    s->close = my_fclose;
    s->size = my_fsize;
    s->tell = my_ftell;
  }

  return s;
}
#define MEMINFOFEILD   unsigned long len, pos, siz; unsigned char* buf
typedef struct MEMINFO {
  MEMINFOFEILD;
}
MEMINFO;
static int my_memread(shandle_t h, data_t d, int s)
{
  MEMINFO* ini = (MEMINFO*) h;

  if (!d) {
    return 0;
  }

  if (s < (int)(ini->len - ini->pos)) {
    s = (int)(ini->len - ini->pos);
  }

  memcpy(d, ini->buf + ini->pos, s);
  ini->pos += s;
  assert(ini->pos <= ini->len);
  assert(ini->buf[ 0 ] == 'B');
  return s;
}
static void my_memrealloc(shandle_t h, int newlen)
{
  MEMINFO* ini = (MEMINFO*) h;

  if (newlen > (int)(ini->siz)) {
    unsigned char* buf;
    ini->siz = next_power(newlen);
    buf = MALLOC(unsigned char, ini->siz);
    memset(buf, 0, ini->siz);

    if (ini->buf) {
      memcpy(buf, ini->buf, ini->len);
      SAFEFREE(ini->buf);
    }

    ini->buf = buf;
  }

  ini->len = newlen;
}
static int my_memwrite(shandle_t h, data_t d, int s)
{
  // static TCHAR int2hex[] = _T( "0123456789abcdef" );
  MEMINFO* ini = (MEMINFO*) h;

  //size_t oldlen = ini->len;
  if (!d) {
    return 0;
  }

  my_memrealloc(h, MAX(ini->pos + s, ini->len));
  memcpy(ini->buf + ini->pos, d, s);
  ini->pos += s;
  assert(ini->pos <= ini->len);
  return s;
}
static int my_memvprintf(shandle_t h, const TCHAR* format, va_list va)
{
  MEMINFO* ini = (MEMINFO*) h;
  unsigned char* buf;
  size_t olelen = ini->len;
  int s = (_tcslen(format) * 10) * sizeof(TCHAR);
  //static TCHAR int2hex[] = _T( "0123456789abcdef" );
  ini->len = MAX(ini->pos + s, ini->len);
  buf = MALLOC(unsigned char, ini->len + 2);
  memcpy(buf, ini->buf, olelen);
  free(ini->buf);
  ini->buf = buf;
  buf += ini->pos;
  ini->pos += _vsntprintf((TCHAR*) buf, s, format, va);
  ini->len = ini->pos;
  return s;
}
static int my_memseek(shandle_t h, int o, int l)
{
  MEMINFO* ini = (MEMINFO*) h;
  assert(l == 0);
  ini->pos = MIN(o, (int) ini->len);
  return ini->pos;
}
// "fileio.h"
static int my_memclose(shandle_t h)
{
  MEMINFO* ini = (MEMINFO*) h;
  free(ini->buf);
  free(ini);
  return 0;
}
static int my_memsize(shandle_t h)
{
  MEMINFO* ini = (MEMINFO*) h;
  return ini->len;
}
static int my_memtell(shandle_t h)
{
  MEMINFO* ini = (MEMINFO*) h;
  return ini->pos;
}
static STRM* sopen_mem_base(void* p, size_t exsize)
{
  STRM* s = NEWSTRM(0);
  MEMINFO* ini;
  ini = (MEMINFO*) malloc(exsize + sizeof(MEMINFO));
  memset(ini, 0, exsize + sizeof(MEMINFO));
  s->h = (shandle_t) ini;
  s->seek = my_memseek;
  s->read = my_memread;
  s->write = my_memwrite;
  s->close = my_memclose;
  s->size = my_memsize;
  s->tell = my_memtell;
  s->vprintf = my_memvprintf;

  if (p) {
    ini->buf = p;
  }

  return s;
}
#ifdef _WIN
#include "inifile.h"
#define _tcsend(_s)  ((TCHAR*)(_s)+_tcslen((TCHAR*)(_s)))
static unsigned char hex2int[ 256 ] = {0};
typedef struct INIINFO {
  MEMINFOFEILD;
  TCHAR* lpAppName;
  TCHAR* lpKeyName;
  TCHAR filename[ 1 ];
}
INIINFO;
// "fileio.h"
static int my_iniclose(shandle_t h)
{
  INIINFO* ini = (INIINFO*) h;
  IniWriteString(ini->filename, ini->lpAppName, ini->lpKeyName, (TCHAR*)(ini->buf));
  return my_memclose(h);
}
static STRM* sopen_win_ini(const TCHAR* filename, const TCHAR* mode)
{
  STRM* s = sopen_mem_base(0, _tcslen(filename) + sizeof(INIINFO) - sizeof(MEMINFO));
  INIINFO* ini;
  TCHAR* ext;
  int ret;
  UNUSED(mode);
  ini = (INIINFO*)(s->h);
  _tcscpy(ini->filename, filename);
  _tcstolower(ini->filename);
  ext = _tcsstr(ini->filename, _T(".ini"));
  ext[ 4 ] = 0;
  ext = _tcsrchr(ext + 5, _T('\\'));
  ext[ 0 ] = 0;
  s->close = my_iniclose;
  ini->lpAppName = _tcsend(ini->filename) + 1;
  ini->lpKeyName = _tcsend(ini->lpAppName) + 1;
  my_memrealloc(s->h, 64 * sizeof(TCHAR));

  for (; (ret = IniGetString(ini->filename, ini->lpAppName, ini->lpKeyName, _T(""), (TCHAR*) ini->buf, ini->len)) == (int)(ini->len);) {
    my_memrealloc(s->h, ini->len * 2);
  }

  ini->len = ret;
  return s;
}
static HKEY gethkey(const TCHAR* regname)
{
  int i = 0;
#define _HKEYNAMEMAPDEF(_KEY)  ( _hkeynamemap[i].hkey = _KEY, _hkeynamemap[i].name = _T(#_KEY), ++i )
  static struct {
    HKEY hkey;
    TCHAR* name;
  }
  _hkeynamemap[10];
  _HKEYNAMEMAPDEF(HKEY_CLASSES_ROOT);
  _HKEYNAMEMAPDEF(HKEY_CURRENT_USER);
  _HKEYNAMEMAPDEF(HKEY_LOCAL_MACHINE);
  _HKEYNAMEMAPDEF(HKEY_USERS);
  _HKEYNAMEMAPDEF(HKEY_PERFORMANCE_DATA);
  //_HKEYNAMEMAPDEF( HKEY_PERFORMANCE_TEXT );
  //_HKEYNAMEMAPDEF( HKEY_PERFORMANCE_NLSTEXT );
#if(WINVER >= 0x0400)
  _HKEYNAMEMAPDEF(HKEY_CURRENT_CONFIG);
  _HKEYNAMEMAPDEF(HKEY_DYN_DATA);
#endif
  _HKEYNAMEMAPDEF(0);

  for (i = 0; i < sizeof(_hkeynamemap) / sizeof(_hkeynamemap[ 0 ]); ++i) {
    if (_tcsstr(regname, _hkeynamemap[ i ].name)) {
      return _hkeynamemap[ i ].hkey;
    }
  }

  assert(0 && "未定义根键！");
  return 0;
}
typedef struct REGINFO {
  MEMINFOFEILD;
  unsigned long type;
  HKEY hKey;
  TCHAR* lpSubKey;
  TCHAR* lpValueName;
  TCHAR filename[ 1 ];
}
REGINFO;
// "fileio.h"
static int my_regclose(shandle_t h)
{
  REGINFO* ini = (REGINFO*) h;
  RegSetValueEx(ini->hKey, ini->lpValueName, 0, ini->type, ini->buf, ini->len);
  RegCloseKey(ini->hKey);
  return my_memclose(h);
}
static STRM* sopen_win_reg(const TCHAR* filename, const TCHAR* mode)
{
  STRM*    s = sopen_mem_base(0, _tcslen(filename) + sizeof(REGINFO) - sizeof(MEMINFO));
  REGINFO* ini;
  TCHAR*   ext;
  HKEY     rootkey;
  int      ret;
  unsigned long type;
  rootkey = gethkey(filename);
  ini = (REGINFO*)(s->h);
  _tcscpy(ini->filename, filename);
  ext = _tcschr(ini->filename, _T('\\'));
  ext[ 0 ] = 0;
  ini->lpSubKey = ext + 1;
  ext = _tcsrchr(ini->lpSubKey, '\\');
  ext[ 0 ] = 0;
  ini->lpValueName = ext + 1;
  s->close = my_regclose;
  ini->type = (mode && _tcspbrk(mode, _T("tT"))) ? REG_SZ : REG_BINARY;
  ret = RegOpenKey(rootkey, ini->lpSubKey, &ini->hKey);

  if (ret != ERROR_SUCCESS) {
    ret = RegCreateKey(rootkey, ini->lpSubKey, &ini->hKey);
  }

  assert(ret == ERROR_SUCCESS);
  ret = RegQueryValueEx(ini->hKey, ini->lpValueName, NULL, &type, 0, &ini->len);
  my_memrealloc(s->h, ini->len + 2);

  if (ret == ERROR_SUCCESS) {
    ret = RegQueryValueEx(ini->hKey, ini->lpValueName, NULL, &type, ini->buf, &ini->len);
  }

  if (REG_SZ == ini->type && ini->len) {
    ini->len--;
  }

  return s;
}
#endif
static STRM* sopen_mem(const TCHAR* filename, const TCHAR* mode)
{
  UNUSED(filename);
  UNUSED(mode);
  return sopen_mem_base((void*)filename, 0);
}
STRM* strmopen(const TCHAR* filename, const TCHAR* mode, const TCHAR* type)
{
  int i = 0;
  static struct {
    TCHAR* type;
    STRM* (*func)(const TCHAR* filename, const TCHAR* mode);
  }
  sopenfuncmap[10];
  UNUSED(strmopen);
  /*
    #define SOPENFUNCMAPDEF(_TYPE)  { _T(#_TYPE), sopen_##_TYPE }
  #define SOPENFUNCMAPDEF(_TYPE, _FUNC)  { _TYPE, _FUNC }
                   SOPENFUNCMAPDEF( file ),
                   SOPENFUNCMAPDEF( mem ),
  #ifdef _WIN
                   SOPENFUNCMAPDEF( win_ini ),
                   SOPENFUNCMAPDEF( win_reg ),
  #endif
  */
#define SOPENFUNCMAPDEF(_TYPE, _FUNC)  ( sopenfuncmap[i].type = _TYPE, sopenfuncmap[i].func = _FUNC, ++i )
  SOPENFUNCMAPDEF(_T("file"), sopen_file);
  SOPENFUNCMAPDEF(_T("mem"), sopen_mem);
#ifdef _WIN
  SOPENFUNCMAPDEF(_T("win_ini"), sopen_win_ini);
  SOPENFUNCMAPDEF(_T("win_reg"), sopen_win_reg);
#endif

  for (i = 0; i < sizeof(sopenfuncmap) / sizeof(sopenfuncmap[ 0 ]); ++i) {
    if (0 == _tcscmp(type, sopenfuncmap[ i ].type)) {
      return sopenfuncmap[ i ].func(filename, mode);
    }
  }

  assert(0 && "未定义的type！");
  return 0;
}

