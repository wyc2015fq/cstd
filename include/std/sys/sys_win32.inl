
#include "cstd.h"

#include "str.h"

#if defined C_MSVC
#if _MSC_VER>1200
#include <intrin.h>
#endif
#endif
//#include "cfile.h"
//#define ARCH "win32"
//#define _CRT_SECURE_NO_WARNINGS


int sys_msize(void* p) {
  return p ? (int)_msize(p) : 0;
}
//#pragma warning(disable : 4244)

#ifdef _MSC_VER
#pragma warning(disable : 4305)
#pragma warning(disable : 4996)
#pragma warning(disable : 4819)
#pragma warning(disable : 4018)

// kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")

#endif // _MSC_VER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>


//#include <winsock2.h>
//#undef WINVER
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
//#define WINVER 0x0500 // this is needed for UpdateLayeredWindow
//#define WM_MOUSEWHEEL                   0x020A
//#include <windows.h>

#include <windows.h>
#include <winuser.h>
#include <commctrl.h>
#include <CommDlg.h>


#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

// 高精度计时 (单位为秒, 精确到微秒)
#if 1
int64 sys_utime_frequency()
{
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
  return frequency.QuadPart;
}
int64 sys_utime_counter()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter((LARGE_INTEGER*)&counter);
  return counter.QuadPart;
}
#else
double sys_utime(int64* pcounter) {
  double t;
  int counter = clock();
  t = (counter - *pcounter) * 1.0 / CLOCKS_PER_SEC;
  return t;
}
#endif

#define FILE_ACCESS_MODEDEF(a, b) if (mode&a) {mode1|=b;}

static int filemode_to_sysmode(int mode) {
  int mode1=0;
  FILE_ACCESS_MODEDEF(AS_EXIST, _A_NORMAL);
  FILE_ACCESS_MODEDEF(AS_RDONLY, _A_RDONLY);
  FILE_ACCESS_MODEDEF(AS_EXECUTE, 0);
  FILE_ACCESS_MODEDEF(AS_WRITE, 0);
  FILE_ACCESS_MODEDEF(AS_READ, 0);
  FILE_ACCESS_MODEDEF(AS_SUBDIR, _A_SUBDIR);
  return mode1;
}
int sys_access(const char* filename, int mode) {
  return _access(filename, filemode_to_sysmode(mode));
}
char* sys_get_exe_path(char* buf, int count) {
  GetModuleFileNameA(0, buf, count);
  return buf;
}

static char* GetAppPath()
{
  int i;
  static char AppPath[256];
  sys_get_exe_path(AppPath, 256);
  i = (int)strlen(AppPath);
  while (i > 0 && AppPath[i] != '\\') {
    i--;
  }
  if (i > 0) {
    AppPath[i] = 0;
  }
  return (AppPath);
}
///////////////////////////////////////////////////////////////
// time_t 转 FILETIME
static FILETIME Timet2FileTime(const time_t t)
{
  LONGLONG nLL = Int32x32To64(t, 10000000) + 116444736000000000;
  FILETIME ft;
  ft.dwLowDateTime = (DWORD) nLL;
  ft.dwHighDateTime = (DWORD)(nLL >> 32);
  return ft;
}
static lutime64_t filetime2timet1(const FILETIME ft)
{
  __int64 i = *(__int64*)&ft;
  return (lutime64_t)((i - 116444736000000000) / 10000000);
}
static void filetime2dosdatetime1(const FILETIME ft, WORD* dosdate, WORD* dostime)
{
  // date: bits 0-4 are day of month 1-31. Bits 5-8 are month 1..12. Bits 9-15 are year-1980
  // time: bits 0-4 are seconds/2, bits 5-10 are minute 0..59. Bits 11-15 are hour 0..23
  SYSTEMTIME st;
  FileTimeToSystemTime(&ft, &st);
  *dosdate = (WORD)(((st.wYear - 1980) & 0x7f) << 9);
  *dosdate |= (WORD)((st.wMonth & 0xf) << 5);
  *dosdate |= (WORD)((st.wDay & 0x1f));
  *dostime = (WORD)((st.wHour & 0x1f) << 11);
  *dostime |= (WORD)((st.wMinute & 0x3f) << 5);
  *dostime |= (WORD)((st.wSecond * 2) & 0x1f);
}
// FILETIME 转 time_t
static time_t FileTimeToTimet(FILETIME ft) {
  time_t pt;
  //LONGLONG nLL;
  ULARGE_INTEGER ui;
  ui.LowPart = ft.dwLowDateTime;
  ui.HighPart = ft.dwHighDateTime;
  //nLL = ((uint64)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
  pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
  return pt;
}
// time_t 转 SYSTEMTIME
static SYSTEMTIME TimetToSystemTime(time_t t)
{
  FILETIME ft;
  SYSTEMTIME pst;
  LONGLONG nLL = Int32x32To64(t, 10000000) + 116444736000000000;
  ft.dwLowDateTime = (DWORD)nLL;
  ft.dwHighDateTime = (DWORD)(nLL >> 32);
  FileTimeToSystemTime(&ft, &pst);
  return pst;
}
// SYSTEMTIME 转 time_t
static time_t SystemTimeToTimet(SYSTEMTIME st)
{
  FILETIME ft;
  SystemTimeToFileTime(&st, &ft);
  return FileTimeToTimet(ft);
}
// time_t 转 SYSTEMTIME
static void tm2SYSTEMTIME(struct tm* ptm, SYSTEMTIME* pst) {
}
static SYSTEMTIME TimetToSystemTime1(time_t t)
{
  struct tm* ptm = localtime(&t);
  SYSTEMTIME pst[1] = {0};
  pst->wYear = 1900 + ptm->tm_year;
  pst->wYear=ptm->tm_year+1900;
  pst->wMonth=ptm->tm_mon+1;
  pst->wDay=ptm->tm_mday;
  pst->wHour=ptm->tm_hour;
  pst->wMinute=ptm->tm_min;
  pst->wSecond=ptm->tm_sec;
  return *pst;
}
// SYSTEMTIME 转 time_t
static time_t SystemTimeToTimet2(const SYSTEMTIME* st)
{
  struct tm temptm = {st->wSecond, st->wMinute, st->wHour, st->wDay, st->wMonth - 1,
    st->wYear - 1900, st->wDayOfWeek, 0, 0 };
  return mktime(&temptm);
}
static FILETIME dosdatetime2filetime1(WORD dosdate, WORD dostime)
{
  FILETIME ft;
  // date: bits 0-4 are day of month 1-31. Bits 5-8 are month 1..12. Bits 9-15 are year-1980
  // time: bits 0-4 are seconds/2, bits 5-10 are minute 0..59. Bits 11-15 are hour 0..23
  SYSTEMTIME st;
  st.wYear = (WORD)(((dosdate >> 9) & 0x7f) + 1980);
  st.wMonth = (WORD)((dosdate >> 5) & 0xf);
  st.wDay = (WORD)(dosdate & 0x1f);
  st.wHour = (WORD)((dostime >> 11) & 0x1f);
  st.wMinute = (WORD)((dostime >> 5) & 0x3f);
  st.wSecond = (WORD)((dostime & 0x1f) * 2);
  st.wMilliseconds = 0;
  SystemTimeToFileTime(&st, &ft);
  return ft;
}
static FILETIME timet2filetime1(const lutime64_t t)
{
  LONGLONG i = Int32x32To64(t, 10000000) + 116444736000000000;
  FILETIME ft;
  ft.dwLowDateTime = (DWORD) i;
  ft.dwHighDateTime = (DWORD)(i >> 32);
  return ft;
}
static SYSTEMTIME systime2winsystime(const sys_time_t* pmyst) {
  SYSTEMTIME st = {0};
  st.wYear = pmyst->year;
  st.wMonth = pmyst->month;
  st.wDay = pmyst->day;
  st.wHour = pmyst->hour;
  st.wMinute = pmyst->minute;
  st.wSecond = pmyst->second;
  st.wMilliseconds = 0;
  return st;
}
static sys_time_t winsystime2systime(const SYSTEMTIME* pst) {
  sys_time_t pmyst[1] = {0};
  pmyst->year = pst->wYear;
  pmyst->month = pst->wMonth;
  pmyst->day = pst->wDay;
  pmyst->hour = pst->wHour;
  pmyst->minute = pst->wMinute;
  pmyst->second = pst->wSecond;
  pmyst->millis = pst->wMilliseconds;
  return *pmyst;
}
static FILETIME systime2filetime(const sys_time_t* ctime) {
  FILETIME ft = {0};
  SYSTEMTIME st = systime2winsystime(ctime);
  SystemTimeToFileTime(&st, &ft);
  return ft;
}
int sys_get_file_attributes(const char* fn) {
  return GetFileAttributesA(fn);
}
int sys_setfiletime(const char* name, const sys_time_t* ctime, const sys_time_t* atime, const sys_time_t* mtime)
{
  // 打开目录的Win32 API调用
  // 必须“写”方式打开
  // 打开现存的目录
    // 只有这样才能打开目录
  HANDLE hDir = CreateFileA( name, GENERIC_READ | GENERIC_WRITE,
    FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if (hDir == INVALID_HANDLE_VALUE)
  {
    return FALSE;  // 打开失败时返回
  } else {
    // creation time目录的创建时间
    FILETIME lpCreationTime = systime2filetime(ctime);
    FILETIME lpLastAccessTime = systime2filetime(atime); // last access time最近一次访问目录的时间
    FILETIME lpLastWriteTime = systime2filetime(mtime);    // last write time最近一次修改目录的时间
    // 修改目录时间的Win32 API函数调用
    BOOL retval = SetFileTime(
      hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime);
    CloseHandle(hDir); // 关闭目录
    return retval;
    // 返回修改成功与否的返回码
  }
}
// time1 - time2
static double systime_diff(const SYSTEMTIME* sTime1, const SYSTEMTIME* sTime2)
{
  ULARGE_INTEGER fTime1;/*FILETIME*/
  ULARGE_INTEGER fTime2;/*FILETIME*/
  __int64 counter = 10 * 1000 * 1000;
  double dft0;
  SystemTimeToFileTime(sTime1, (FILETIME*)&fTime1);
  SystemTimeToFileTime(sTime2, (FILETIME*)&fTime2);
  dft0 = (fTime1.QuadPart - fTime2.QuadPart) * 1. / counter;
  return dft0;
}
// 定时器：为 Windows 实现一个连续更新，高精度的时间供应器
typedef struct reference_point {
  FILETIME        file_time;
  LARGE_INTEGER   counter;
} reference_point;
typedef struct SFTIME {
  reference_point ref_point;
  LARGE_INTEGER   frequency;
  FILETIME        file_time;
  SYSTEMTIME      system_time;
  TIME_ZONE_INFORMATION zone_info;
} SFTIME;
//同步
static void simplistic_synchronize(reference_point* ref_point)
{
  FILETIME       ft0 = { 0, 0 };
  FILETIME*      ft1 = &(ref_point->file_time);
  LARGE_INTEGER* li  = &(ref_point->counter);
  // Spin waiting for a change in system time. Get the matching
  // performace counter value for that time.
  GetSystemTimeAsFileTime(&ft0);
  do {
    GetSystemTimeAsFileTime(ft1);
    QueryPerformanceCounter(li);
  }
  while ((ft0.dwHighDateTime == ft1->dwHighDateTime) && (ft0.dwLowDateTime == ft1->dwLowDateTime));
}
static SFTIME* sftime_init()
{
  static SFTIME sf[1];
  static int sfinited = 0;
  if (0 == sfinited) {
    sfinited = 1;
    QueryPerformanceFrequency(&sf->frequency);
    simplistic_synchronize(&sf->ref_point);
    GetTimeZoneInformation(&sf->zone_info);
  }
  return (sf);
}
static SFTIME* sf_get_time()
{
  SFTIME* sf = sftime_init();
  {
    LARGE_INTEGER* frequency = &sf->frequency;
    reference_point* reference = &sf->ref_point;
    FILETIME* current_time = &sf->file_time;
    LARGE_INTEGER li;
    LARGE_INTEGER ticks_elapsed;
    ULARGE_INTEGER filetime_ticks, filetime_ref_as_ul;
    QueryPerformanceCounter(&li);
    // Calculate performance counter ticks elapsed
    ticks_elapsed.QuadPart = li.QuadPart - reference->counter.QuadPart;
    // Translate to 100-nanosecondsintervals (FILETIME
    // resolution) and add to
    // reference FILETIME to get current FILETIME.
    filetime_ticks.QuadPart = (ULONGLONG)
      ((((double) ticks_elapsed.QuadPart / (double) frequency->QuadPart) * 10000000.0) + 0.5);
    filetime_ref_as_ul.HighPart = reference->file_time.dwHighDateTime;
    filetime_ref_as_ul.LowPart = reference->file_time.dwLowDateTime;
    filetime_ref_as_ul.QuadPart += filetime_ticks.QuadPart;
    // Copy to result
    current_time->dwHighDateTime = filetime_ref_as_ul.HighPart;
    current_time->dwLowDateTime = filetime_ref_as_ul.LowPart;
  }
  FileTimeToSystemTime(&sf->file_time, &sf->system_time);
  SystemTimeToTzSpecificLocalTime(&sf->zone_info, &sf->system_time, &sf->system_time);
  return (sf);
}
int sys_gettime(sys_time_t* st) {
  SFTIME* sf = sf_get_time();
  *st = winsystime2systime(&sf->system_time);
  return 0;
}
///////////////////////////////////////////////////////////////
//#define ACCESS _access
#define UNEIXST_FILE(_FNAME) find_not_eixst_file(_FNAME, 0, 0)

int sys_filestat(const char* file, sys_stat* s) {
  struct _stat st;
  int ret = _stat(file, &st);
  if (s) {
#define COPYMEMBER(_X)  s->_X = st.st_##_X
  COPYMEMBER(dev);
  COPYMEMBER(ino);
  COPYMEMBER(mode);
  COPYMEMBER(nlink);
  COPYMEMBER(uid);
  COPYMEMBER(gid);
  COPYMEMBER(rdev);
  COPYMEMBER(size);
  //COPYMEMBER(blksize);
  //COPYMEMBER(blocks);
  COPYMEMBER(atime);
  COPYMEMBER(mtime);
  COPYMEMBER(ctime);
#undef COPYMEMBER
  }
  return ret;
}

int sys_mkdir(const char* file) {
  return _mkdir(file);
}
char* sys_temp_path(char* _temp_path, int len) {
  GetTempPathA(len, _temp_path);  
  return (char*)_temp_path;
}

char* sys_tmpname(const char* fname)
{
  static char fullname[ 260 ];
  const char* tmppath = getenv("temp");
  strcpy(fullname, tmppath);
  strcat(fullname, "\\");
  if (fname) {
    const char* fname2 = strrchr(fname, '\\');
    fname2 = fname2 ? fname2 + 1 : fname;
    strcat(fullname, fname2);
  }
  else {
    char tmp[ 260 ];
    tmpnam(tmp);
    strcat(fullname, tmp);
  }
  return fullname;
}
static int trace_dbgv(const char* fmt, char* argptr)
{
  enum {_VT_BUF_LEN = 1 << 11};
  char buf[ _VT_BUF_LEN ] = {0};
  _vsnprintf(buf, _VT_BUF_LEN, fmt, argptr);
  OutputDebugStringA(buf);
  return 0;
}
int sys_trace_dbg(const char* fmt, ...)
{
  va_list argptr;
  va_start(argptr, fmt);
  trace_dbgv(fmt, argptr);
  va_end(argptr);
  return 0;
}
//#define printf _trace_dbg
int sys_chdir(const char* file) {
  return _chdir(file);
}
#ifdef TEST
static int test_savedata_inl()
{
  buf_t bf[1] = {0};
  sys_chdir("D:\\code\\c\\Font\\Fonts\\");
  //buf_load("arial.ttf", bf);
  //savedata_inl("arial_ttf.inl", bf->tt.data, bf->len);
  bffree(bf);
  return 0;
}
#endif
static int res_loadres(const char* fname, buf_t* bf)
{
  if (strlen(fname) > 5 && 0 == mem_icmp(fname, "res:", 4, 1)) {
    HINSTANCE hResInstance = GetModuleHandle(0);
    HRSRC res = FindResourceA(hResInstance, (fname), ("BINARY"));
    if (res) {
      HGLOBAL mem = LoadResource(hResInstance, res);
      void* data = LockResource(mem);
      DWORD len = SizeofResource(hResInstance, res);
      bfsetsize(bf, len);
      memcpy(bf->data, data, len);
    }
  }
  return 0;
}


static int win32_attrib_cvt(int attrib) {
  int attrib2=0;
#define WIN32_ATTRIB_CVT(a, b) if (attrib&(a))  attrib2|=(b);
  WIN32_ATTRIB_CVT(_A_RDONLY, AS_RDONLY);
  WIN32_ATTRIB_CVT(_A_SUBDIR, AS_SUBDIR);
  WIN32_ATTRIB_CVT(_A_HIDDEN, AS_HIDDEN);
  return attrib2;
}

int sys_find_next_file(findinfo_t* s, const char* path, const char* filters, fileinfo_t* f, int flag) {
  struct _finddata_t info[1];
  char* name = f->name;
  int maxnamelen = countof(f->name);
  if (NULL==s->dir) {
    char buf[256];
    _snprintf(buf, 256, "%s/%s", path, filters?filters:"*");
    s->dir = (void*)_findfirst(buf, info);
    s->count++;
    f->size = info->size;
    f->ctime = info->time_create;
    f->atime = info->time_access;
    f->wtime = info->time_write;
    f->attrib = win32_attrib_cvt(info->attrib);
    if (flag & (FF_FULLNAME|FF_SUBDIR)) {
      _snprintf(name, maxnamelen, "%s/%s", path, info->name);
    } else {
      strncpy(name, info->name, maxnamelen);
    }
    return ((void*)-1 != s->dir && NULL!=s->dir);
  } else {
    if (_findnext((intptr_t)s->dir, info) == 0) {
      s->count++;
      f->size = info->size;
      f->ctime = info->time_create;
      f->atime = info->time_access;
      f->wtime = info->time_write;
      f->attrib = win32_attrib_cvt(info->attrib);
      if (flag & (FF_FULLNAME|FF_SUBDIR)) {
        _snprintf(name, MAX_PATH, "%s/%s", path, info->name);
      } else {
        strncpy(name, info->name, maxnamelen);
      }
      return TRUE;
    } else {
      _findclose((intptr_t)s->dir);
      s->dir = 0;
    }
  }
  return FALSE;
}

static int codepage_cvt(int cp) {
  int cp_win = CP_ACP;
  switch (cp) {
  case CE_GB2312:
    cp_win = CP_ACP;
    break;
  case CE_UTF8:
    cp_win = CP_UTF8;
    break;
  case CE_UTF7:
    cp_win = CP_UTF7;
    break;
  }
  return cp_win;
}
int sys_wc2mb(int cp, const wchar_t* s, int nwc, void* d, int nmb) {
  int cp_win = codepage_cvt(cp);
  return WideCharToMultiByte(cp_win, 0, s, nwc, (char*)d, nmb, 0, 0);
}
int sys_mb2wc(int cp, const void* pmb0, int nmb, wchar_t* pwc, int nwc) {
  int cp_win = codepage_cvt(cp);
  return MultiByteToWideChar(cp_win, 1, (char*)pmb0, nmb, (wchar_t*)pwc, nwc);
}
static int utf8_printfv(const char* fmt, va_list va) {
  HANDLE m_OutputHandle;
  ULONG beWritten = 0, length=0, len;
  char* msg = NULL;
  WCHAR* wmsg = NULL;
  length = cstr_format_lenv(fmt, va);
  msg = (char*)sys_malloc((length+1)*sizeof(char));
  wmsg = (WCHAR*)sys_malloc((length+1)*sizeof(WCHAR));
  len = _vsnprintf(msg, length+1, fmt, va);
  len = MultiByteToWideChar( CP_UTF8, 0, msg, -1, wmsg, length+1 );
  m_OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  WriteConsoleW(m_OutputHandle, wmsg, len, &beWritten , NULL);
  sys_free(msg);
  sys_free(wmsg);
  return len;
}
static int utf8_printf(const char* fmt, ...) {
  int ret = 0;
  va_list va;
  va_start(va, fmt);
  ret = utf8_printfv(fmt, va);
  va_end(va);
  return ret;
}
static int sys_find_next_file_utf8(findinfo_t* s, const char* path, const char* filters, fileinfo_t* f, int flag) {
  WIN32_FIND_DATAW info[1];
  char* name = f->name;
  int len, maxnamelen = countof(f->name);
  char buf[256];
  if (NULL==s->dir) {
    WCHAR wbuf[256];
    _snprintf(buf, 256, "%s/%s", path, filters?filters:"*");
    len = MultiByteToWideChar( CP_ACP, 0, buf, -1, wbuf, 256 );
    wbuf[len]=0;
    s->dir = (void*)FindFirstFileW(wbuf, info);
    s->count++;
    f->size = info->nFileSizeLow;
    f->ctime = FileTimeToTimet(info->ftCreationTime);
    f->atime = FileTimeToTimet(info->ftLastAccessTime);
    f->wtime = FileTimeToTimet(info->ftLastWriteTime);
    f->attrib = info->dwFileAttributes;
    WideCharToMultiByte(CP_UTF8, 0, info->cFileName, -1, buf, 256, NULL, NULL);
    if (flag & (FF_FULLNAME|FF_SUBDIR)) {
      _snprintf(name, maxnamelen, "%s/%s", path, buf);
    } else {
      strncpy(name, buf, maxnamelen);
    }
    return (NULL != s->dir);
  } else {
    if (FindNextFileW((HANDLE)s->dir, info)) {
      s->count++;
      f->size = info->nFileSizeLow;
      f->ctime = FileTimeToTimet(info->ftCreationTime);
      f->atime = FileTimeToTimet(info->ftLastAccessTime);
      f->wtime = FileTimeToTimet(info->ftLastWriteTime);
      f->attrib = info->dwFileAttributes;
      WideCharToMultiByte(CP_UTF8, 0, info->cFileName, -1, buf, 256, NULL, NULL);
      if (flag & (FF_FULLNAME|FF_SUBDIR)) {
        _snprintf(name, maxnamelen, "%s/%s", path, buf);
      } else {
        strncpy(name, buf, 256);
      }
      return TRUE;
    } else {
      FindClose((HANDLE)s->dir);
      s->dir = 0;
    }
  }
  return FALSE;
}

#define  BMPMARK(A)  (((A)+3)&~3)

// returns TRUE if there is a problem such as ERROR_IO_PENDING.
static BOOL GetBitmapData(HDC dc, int* pwidth, int* pheight, int* channels, int** data)
{
  BITMAP bmp;
  HGDIOBJ h;
  GdiFlush();
  h = GetCurrentObject(dc, OBJ_BITMAP);
  
  if (pwidth) {
    *pwidth = 0;
  }
  if (pheight) {
    *pheight = 0;
  }
  
  
  if (data) {
    * data = 0;
  }
  
  if (h == NULL) {
    return TRUE;
  }
  
  if (GetObject(h, sizeof(bmp), &bmp) == 0) {
    return TRUE;
  }
  
  if (pwidth) {
    *pwidth = abs(bmp.bmWidth);
  }
  if (pheight) {
    *pheight = abs(bmp.bmHeight);
  }
  
  if (channels) {
    * channels = bmp.bmBitsPixel / 8;
  }
  
  if (data) {
    *data = (int*)bmp.bmBits;
  }
  
  return FALSE;
}
static void FillSolidRect(HDC m_hDC, void* lpRect, COLORREF clr)
{
  SetBkColor(m_hDC, clr);
  ExtTextOutA(m_hDC, 0, 0, ETO_OPAQUE, (RECT*)lpRect, NULL, 0, NULL);
}
static int FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin, const RGBQUAD* inpal)
{
  BITMAPINFOHEADER* bmih;
  assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
  bmih = &(bmi->bmiHeader);
  memset(bmih, 0, sizeof(*bmih));
  bmih->biSize = sizeof(BITMAPINFOHEADER);
  bmih->biWidth = width;
  bmih->biHeight = origin ? abs(height) : -abs(height);
  bmih->biPlanes = 1;
  bmih->biBitCount = (unsigned short) bpp;
  bmih->biCompression = BI_RGB;
  
  if (bpp <= 8) {
    RGBQUAD* palette = bmi->bmiColors;
    
    if (inpal) {
      memcpy(palette, inpal, 256 * sizeof(RGBQUAD));
    }
    else {
      int i;
      
      for (i = 0; i < 256; i++) {
        palette[ i ].rgbBlue = palette[ i ].rgbGreen = palette[ i ].rgbRed = (BYTE) i;
        palette[ i ].rgbReserved = 0;
      }
    }
  }
  
  return 0;
}

static int cvShowDC1(HDC dc,  int height, int width, const unsigned char* arr, int step, int cn, const RGBQUAD* inpal)
{
  int i, j, h, w;
  int channels = 0;
  int* dst_ptr = 0;
  const int channels0 = 3;
  //int origin = 0;
  //HBITMAP image = NULL;
  
  // if there is something wrong with these system calls, we cannot display image...
  if (GetBitmapData(dc, &w, &h, &channels, &dst_ptr)) {
    //return 0;
  }
  
  if (w != width || h != height || channels != channels0) {
    unsigned char buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
    BITMAPINFO* binfo = (BITMAPINFO*) buffer;
    int cnbit[] = {1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    
    if (inpal) {
      LPLOGPALETTE lpPal = (LPLOGPALETTE)buffer;  // 指向逻辑调色板的指针
      HPALETTE hPal = NULL; // 调色板的句柄
      int i;
      lpPal->palVersion = 0x300;   // 设置版本号
      lpPal->palNumEntries = (WORD) 256;   // 设置颜色数目
      
      for (i = 0; i < (int) 256; i++) {
        lpPal->palPalEntry[ i ].peRed = inpal[ i ].rgbRed;
        lpPal->palPalEntry[ i ].peGreen = inpal[ i ].rgbGreen;
        lpPal->palPalEntry[ i ].peBlue = inpal[ i ].rgbBlue;
        lpPal->palPalEntry[ i ].peFlags = 0;
      }
      
      hPal = CreatePalette(lpPal);
      DeleteObject(SelectPalette(dc, hPal, TRUE));
    }
    
    //DeleteObject(SelectObject(dc, image));
    //image = 0;
    
    w = width;
    h = height;
    FillBitmapInfo(binfo, w, h, cnbit[cn] * 8, 1, inpal);
    SelectObject(dc, CreateDIBSection(dc, binfo, DIB_RGB_COLORS, (void**)&dst_ptr, 0, 0));
  }
  
  if (8 == cn) {
    int dststep = (w + 3) & -4, mini = 0, maxi = 0;
    unsigned char* dst = (unsigned char*)dst_ptr;
    const double* darr = ((const double*)arr);
    for (i = 0; i < h*w; ++i) {
      if (darr[i]<darr[mini]) {
        mini = i;
      } else if (darr[i]>darr[maxi]) {
        maxi = i;
      }
    }
    
    for (i = 0; i < h; ++i) {
      unsigned char* dst1 = dst + (h - 1 - i) * dststep;
      const double* srcd = (const double*)(arr + i * step);
      
      for (j = 0; j < w; ++j) {
        int t = (int)((srcd[j] - darr[mini]) * 256 / (darr[maxi] - darr[mini]));
        if (t>255) {
          t = 255;
        }else if (t<0) {
          t = 0;
        }
        dst1[j] = t;
      }
    }
  }
  else if (2 == cn) {
    int dststep = (w + 3) & -4, mini = 0, maxi = 0;
    unsigned char* dst = (unsigned char*)dst_ptr;
    const short* darr = ((const short*)arr);
    for (i = 0; i < h*w; ++i) {
      if (darr[i]<darr[mini]) {
        mini = i;
      } else if (darr[i]>darr[maxi]) {
        maxi = i;
      }
    }
    
    for (i = 0; i < h; ++i) {
      unsigned char* dst1 = dst + (h - 1 - i) * dststep;
      const double* srcd = (const double*)(arr + i * step);
      
      for (j = 0; j < w; ++j) {
        int t = (int)((srcd[j] - darr[mini]) * 256 / (darr[maxi] - darr[mini]));
        if (t>255) {
          t = 255;
        }else if (t<0) {
          t = 0;
        }
        dst1[j] = t;
      }
    }
  }
  else {
    int dststep = (w * cn + 3) & -4;
    unsigned char* dst = (unsigned char*)dst_ptr;
    dst += (h - 1) * dststep;
    
    //IM2IM( h, w, arr, step, cn, dst, -dststep, channels );
    for (i = 0; i < h; ++i) {
      memcpy(dst + i * (-dststep), arr + i * step, w * cn);
    }
    
    if (1 == cn) {
      int s = 0;
      //SKIP_MAX_ELEMENT(height, width, arr, step, cn, s);
      for (i = 0; i < h; ++i) {
        const unsigned char* srcd = (const unsigned char*)(arr + i * step);
        
        for (j = 0; j < w; ++j) {
          if (srcd[j]>s) {
            s = srcd[j];
          }
        }
      }
      dst = (unsigned char*)dst_ptr;
      
      if (1 == arr[s]) {
        for (i = 0; i < h * dststep; ++i) {
          dst[i] = dst[i] ? 255 : 0;
        }
      }
    }
  }

  //BitBlt(hDC, 0, 0, width, height, hDC, x, y, SRCCOPY);
  return 0;
}

#define OUTERRMSG()  printf("%s\n", GetErrorMsg(0))
char* sys_errmsg(int dw)
{
  // Retrieve the system error message for the last-error code
  static char buf[256];
  char* lpMsgBuf = NULL;
  
  if (0 == dw) {
    dw = GetLastError();
  }
  
  FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &lpMsgBuf, 0, NULL);
  snprintf(buf, 256, "err: %ld %s", dw, lpMsgBuf);
  return buf;
}

int sys_set_clipboard_text(const char* str, int nStrLen)
{
  HWND hWnd = NULL;
  if (nStrLen <= 0) {
    return 0;
  }
  
  if (OpenClipboard(hWnd)) {
    char* pchData;
    HGLOBAL hClipboardData;
    EmptyClipboard();
    hClipboardData = GlobalAlloc(GMEM_DDESHARE, nStrLen + 1);
    pchData = (char*)GlobalLock(hClipboardData);
    memmove(pchData, str, nStrLen * sizeof(char));
    GlobalUnlock(hClipboardData);
    SetClipboardData(CF_TEXT, hClipboardData);
    CloseClipboard();
  }
  
  return 0;
}
int sys_get_clipboard_text(char* str, int nMaxStrLen)
{
  HWND hWnd = NULL;
  int nStrLen = 0;
  
  if (OpenClipboard(hWnd)) {
    if (IsClipboardFormatAvailable(CF_TEXT) || IsClipboardFormatAvailable(CF_OEMTEXT)) {
      HANDLE hClipboardData = GetClipboardData(CF_TEXT);
      char* pchData = (char*)GlobalLock(hClipboardData);
      nStrLen = (int)strlen(pchData);
      if (str) {
        strncpy(str, pchData, nMaxStrLen);
      }
      //str = strdup(pchData);
      GlobalUnlock(hClipboardData);
    }
    
    CloseClipboard();
  }
  
  return nStrLen;
}

char* sys_get_clipboard_text_static()
{
  HWND hWnd = NULL;
  int nStrLen = 0;
  char* str = NULL;
  //int nMaxStrLen
  
  if (OpenClipboard(hWnd)) {
    if (IsClipboardFormatAvailable(CF_TEXT) || IsClipboardFormatAvailable(CF_OEMTEXT)) {
      HANDLE hClipboardData = GetClipboardData(CF_TEXT);
      char* pchData = (char*)GlobalLock(hClipboardData);
      nStrLen = (int)strlen(pchData);
      str = (char*)sys_realloc(str, nStrLen + 1);
      memcpy(str, pchData, nStrLen);
      str[nStrLen] = 0;
      GlobalUnlock(hClipboardData);
    }
    
    CloseClipboard();
  }
  
  return str;
}

#ifndef WM_CHANGEUISTATE
#define WM_CHANGEUISTATE  0x0127
#define WM_UPDATEUISTATE  0x0128
#define WM_QUERYUISTATE   0x0129
#define WM_MOUSEWHEEL     0x020A
#define WM_APPCOMMAND 0x0319
#define WM_NCXBUTTONDBLCLK 0x00AD
#define WM_NCXBUTTONDOWN 0x00AB
#define WM_NCXBUTTONUP 0x00AC
#define WM_XBUTTONDBLCLK 0x020D
#define WM_XBUTTONDOWN 0x020B
#define WM_XBUTTONUP 0x020C
#endif



static int win32_set_cursor(HCURSOR rv0) {
  static HCURSOR rv = NULL;
  if (rv0) {
    if (rv) {
      DestroyCursor(rv);
    }
    rv = rv0;
    SetCursor(rv);
  }
  return 0;
}
int sys_set_cursor(int width, int height, int hotx, int hoty, void* pix)
{
  int j, x, y, ofs;
  int w, h;
  uchar* and_plane, *xor_plane;
  uchar* data0 = (uchar*)pix;
  w = GetSystemMetrics(SM_CXCURSOR);
  h = GetSystemMetrics(SM_CYCURSOR);
  and_plane = (uchar*)sys_malloc((w / 8) * h);
  memset(and_plane, 0xff, (w / 8) * h);
  xor_plane = (uchar*)sys_malloc((w / 8) * h);
  memset(xor_plane, 0, (w / 8) * h);
  if (pix) {
#define SET_BIT(v,b) (v |= (1 << b))
#define RESET_BIT(v,b) (v &= ~(1 << b))
    for (j = 0, y = 0; y < height && y < h ; y++) {
      ofs = (y * w) / 8;
      j = y * width;
      for (x = 0; x < width && x < w ; x++, j++) {
        int pofs = ofs + x / 8;
        uchar data = (data0[j / 4] & (0xc0 >> (2 * (j % 4)))) >> (2 * (3 - (j % 4)));
        int bit = 7 - (j % width) % 8;
        if (data) {
          RESET_BIT(and_plane[pofs], bit);
          if (data == 1) {
            SET_BIT(xor_plane[pofs], bit);
          }
        }
      }
    }
#undef SET_BIT
#undef RESET_BIT
    {
      HINSTANCE hmodule = NULL;
      HCURSOR rv0 = CreateCursor(hmodule, hotx, hoty, w, h, and_plane, xor_plane);
      if (rv0) {
        win32_set_cursor(rv0);
      }
      else {
        printf("CreateCursor failed");
      }
    }
  }
  sys_free(and_plane);
  sys_free(xor_plane);
  return 0;
}
static void _gdk_win32_display_get_default_cursor_size(uint* width, uint* height)
{
  if (width) {
    *width = GetSystemMetrics(SM_CXCURSOR);
  }
  if (height) {
    *height = GetSystemMetrics(SM_CYCURSOR);
  }
}
static void _gdk_win32_display_get_maximal_cursor_size(uint* width, uint* height)
{
  if (width) {
    *width = GetSystemMetrics(SM_CXCURSOR);
  }
  if (height) {
    *height = GetSystemMetrics(SM_CYCURSOR);
  }
}
/* Convert a pixbuf to an HICON (or HCURSOR). Supports alpha under
 * Windows XP, thresholds alpha otherwise. Also used from
 * gdkwindow-win32.c for creating application icons.
 */
#define WIN32_GDI_FAILED(str) printf("%s failed", str)
static HBITMAP create_alpha_bitmap(int size, uchar** outdata)
{
  BITMAPV5HEADER bi;
  HDC hdc;
  HBITMAP hBitmap;
  ZeroMemory(&bi, sizeof(BITMAPV5HEADER));
  bi.bV5Size = sizeof(BITMAPV5HEADER);
  bi.bV5Height = bi.bV5Width = size;
  bi.bV5Planes = 1;
  bi.bV5BitCount = 32;
  bi.bV5Compression = BI_BITFIELDS;
  /* The following mask specification specifies a supported 32 BPP
  * alpha format for Windows XP (BGRA format).
  */
  bi.bV5RedMask = 0x00FF0000;
  bi.bV5GreenMask = 0x0000FF00;
  bi.bV5BlueMask = 0x000000FF;
  bi.bV5AlphaMask = 0xFF000000;
  /* Create the DIB section with an alpha channel. */
  hdc = GetDC(NULL);
  if (!hdc) {
    WIN32_GDI_FAILED("GetDC");
    return NULL;
  }
  hBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS,
      (PVOID*) outdata, NULL, (DWORD)0);
  if (hBitmap == NULL) {
    WIN32_GDI_FAILED("CreateDIBSection");
  }
  ReleaseDC(NULL, hdc);
  return hBitmap;
}
static HBITMAP create_color_bitmap(int size, uchar** outdata, int bits)
{
  struct {
    BITMAPV4HEADER bmiHeader;
    RGBQUAD bmiColors[2];
  } bmi;
  HDC hdc;
  HBITMAP hBitmap;
  ZeroMemory(&bmi, sizeof(bmi));
  bmi.bmiHeader.bV4Size = sizeof(BITMAPV4HEADER);
  bmi.bmiHeader.bV4Height = bmi.bmiHeader.bV4Width = size;
  bmi.bmiHeader.bV4Planes = 1;
  bmi.bmiHeader.bV4BitCount = bits;
  bmi.bmiHeader.bV4V4Compression = BI_RGB;
  /* when bits is 1, these will be used.
  * bmiColors[0] already zeroed from ZeroMemory()
  */
  bmi.bmiColors[1].rgbBlue = 0xFF;
  bmi.bmiColors[1].rgbGreen = 0xFF;
  bmi.bmiColors[1].rgbRed = 0xFF;
  hdc = GetDC(NULL);
  if (!hdc) {
    WIN32_GDI_FAILED("GetDC");
    return NULL;
  }
  hBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bmi, DIB_RGB_COLORS,
      (PVOID*) outdata, NULL, (DWORD)0);
  if (hBitmap == NULL) {
    WIN32_GDI_FAILED("CreateDIBSection");
  }
  ReleaseDC(NULL, hdc);
  return hBitmap;
}
static bool pixbuf_to_hbitmaps_alpha_winxp(const img_t* pixbuf, HBITMAP* color, HBITMAP* mask)
{
  /* Based on code from
  * http://www.dotnet247.com/247reference/msgs/13/66301.aspx
  */
  HBITMAP hColorBitmap, hMaskBitmap;
  uchar* indata, *inrow;
  uchar* colordata, *colorrow, *maskdata, *maskbyte;
  int width, height, size, i, i_offset, j, j_offset, rowstride;
  uint maskstride, mask_bit;
  width = pixbuf->w; /* width of icon */
  height = pixbuf->h; /* height of icon */
  /* The bitmaps are created square */
  size = MAX(width, height);
  hColorBitmap = create_alpha_bitmap(size, &colordata);
  if (!hColorBitmap) {
    return FALSE;
  }
  hMaskBitmap = create_color_bitmap(size, &maskdata, 1);
  if (!hMaskBitmap) {
    DeleteObject(hColorBitmap);
    return FALSE;
  }
  /* MSDN says mask rows are aligned to "LONG" boundaries */
  maskstride = (((size + 31) & ~31) >> 3);
  indata = pixbuf->tt.data;
  rowstride = pixbuf->s;
  if (width > height) {
    i_offset = 0;
    j_offset = (width - height) / 2;
  }
  else {
    i_offset = (height - width) / 2;
    j_offset = 0;
  }
  for (j = 0; j < height; j++) {
    colorrow = colordata + 4 * (j + j_offset) * size + 4 * i_offset;
    maskbyte = maskdata + (j + j_offset) * maskstride + i_offset / 8;
    mask_bit = (0x80 >> (i_offset % 8));
    inrow = indata + (height - j - 1) * rowstride;
    for (i = 0; i < width; i++) {
      colorrow[4 * i + 0] = inrow[4 * i + 2];
      colorrow[4 * i + 1] = inrow[4 * i + 1];
      colorrow[4 * i + 2] = inrow[4 * i + 0];
      colorrow[4 * i + 3] = inrow[4 * i + 3];
      if (inrow[4 * i + 3] == 0) {
        maskbyte[0] |= mask_bit; /* turn ON bit */
      }
      else {
        maskbyte[0] &= ~mask_bit; /* turn OFF bit */
      }
      mask_bit >>= 1;
      if (mask_bit == 0) {
        mask_bit = 0x80;
        maskbyte++;
      }
    }
  }
  *color = hColorBitmap;
  *mask = hMaskBitmap;
  return TRUE;
}
static bool pixbuf_to_hbitmaps_normal(const img_t* pixbuf, HBITMAP* color, HBITMAP* mask)
{
  /* Based on code from
  * http://www.dotnet247.com/247reference/msgs/13/66301.aspx
  */
  HBITMAP hColorBitmap, hMaskBitmap;
  uchar* indata, *inrow;
  uchar* colordata, *colorrow, *maskdata, *maskbyte;
  int width, height, size, i, i_offset, j, j_offset, rowstride, nc, bmstride;
  bool has_alpha;
  uint maskstride, mask_bit;
  width = pixbuf->w; /* width of icon */
  height = pixbuf->h; /* height of icon */
  /* The bitmaps are created square */
  size = MAX(width, height);
  hColorBitmap = create_color_bitmap(size, &colordata, 24);
  if (!hColorBitmap) {
    return FALSE;
  }
  hMaskBitmap = create_color_bitmap(size, &maskdata, 1);
  if (!hMaskBitmap) {
    DeleteObject(hColorBitmap);
    return FALSE;
  }
  /* rows are always aligned on 4-byte boundarys */
  bmstride = size * 3;
  if (bmstride % 4 != 0) {
    bmstride += 4 - (bmstride % 4);
  }
  /* MSDN says mask rows are aligned to "LONG" boundaries */
  maskstride = (((size + 31) & ~31) >> 3);
  indata = pixbuf->tt.data;
  rowstride = pixbuf->s;
  nc = pixbuf->c;
  has_alpha = pixbuf->c > 3;
  if (width > height) {
    i_offset = 0;
    j_offset = (width - height) / 2;
  }
  else {
    i_offset = (height - width) / 2;
    j_offset = 0;
  }
  for (j = 0; j < height; j++) {
    colorrow = colordata + (j + j_offset) * bmstride + 3 * i_offset;
    maskbyte = maskdata + (j + j_offset) * maskstride + i_offset / 8;
    mask_bit = (0x80 >> (i_offset % 8));
    inrow = indata + (height - j - 1) * rowstride;
    for (i = 0; i < width; i++) {
      if (has_alpha && inrow[nc * i + 3] < 128) {
        colorrow[3 * i + 0] = colorrow[3 * i + 1] = colorrow[3 * i + 2] = 0;
        maskbyte[0] |= mask_bit; /* turn ON bit */
      }
      else {
        colorrow[3 * i + 0] = inrow[nc * i + 2];
        colorrow[3 * i + 1] = inrow[nc * i + 1];
        colorrow[3 * i + 2] = inrow[nc * i + 0];
        maskbyte[0] &= ~mask_bit; /* turn OFF bit */
      }
      mask_bit >>= 1;
      if (mask_bit == 0) {
        mask_bit = 0x80;
        maskbyte++;
      }
    }
  }
  *color = hColorBitmap;
  *mask = hMaskBitmap;
  return TRUE;
}
static bool _gdk_win32_pixbuf_to_hicon_supports_alpha(void)
{
  static bool is_win_xp = FALSE, is_win_xp_checked = FALSE;
  if (!is_win_xp_checked) {
    OSVERSIONINFO version;
    is_win_xp_checked = TRUE;
    memset(&version, 0, sizeof(version));
    version.dwOSVersionInfoSize = sizeof(version);
    is_win_xp = GetVersionEx(&version)
        && version.dwPlatformId == VER_PLATFORM_WIN32_NT
        && (version.dwMajorVersion > 5
            || (version.dwMajorVersion == 5 && version.dwMinorVersion >= 1));
  }
  return is_win_xp;
}
static HICON pixbuf_to_hicon(const img_t* pixbuf, bool is_icon, int x, int y)
{
  ICONINFO ii;
  HICON icon;
  bool success;
  if (pixbuf == NULL) {
    return NULL;
  }
  if (_gdk_win32_pixbuf_to_hicon_supports_alpha() && (pixbuf->c > 3)) {
    success = pixbuf_to_hbitmaps_alpha_winxp(pixbuf, &ii.hbmColor, &ii.hbmMask);
  }
  else {
    success = pixbuf_to_hbitmaps_normal(pixbuf, &ii.hbmColor, &ii.hbmMask);
  }
  if (!success) {
    return NULL;
  }
  ii.fIcon = is_icon;
  ii.xHotspot = x;
  ii.yHotspot = y;
  icon = CreateIconIndirect(&ii);
  DeleteObject(ii.hbmColor);
  DeleteObject(ii.hbmMask);
  return icon;
}
static HICON _gdk_win32_pixbuf_to_hicon(img_t* pixbuf)
{
  return pixbuf_to_hicon(pixbuf, TRUE, 0, 0);
}
int sys_set_cursor_image(int h, int w, void* data, int step, int cn, int x_hotspot, int y_hotspot)
{
  if (data) {
    img_t pixbuf[1] = {0};
    HCURSOR rv;
    IMINIT(pixbuf, h, w, data, step, cn, 1);
    rv = pixbuf_to_hicon(pixbuf, FALSE, x_hotspot, y_hotspot);
    return win32_set_cursor(rv);
  } else {
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    return 1;
  }
}

static LRESULT TraceMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  const char* str = "";
  switch (msg) {
  case WM_NCPAINT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
    return 0;
  }
  switch (msg) {
#define MSG2STR(a, b, c) case a: str=#a","c;break;
#include "sys_win32_msg2string.txt"
#undef MSG2STR
  default:
    str = 0;
    break;
  }
  if (str) {
#ifdef _WIN64
	  printf("w=%016I64x l=%016I64x %s\n", wparam, lparam, str);
#else
	  printf("w=%08x l=%08x %s\n", wparam, (uint)lparam, str);
#endif
    //printf("%s\n", str);
  }
  else {
#ifdef _WIN64
	  printf("m=%08x w=%016I64x l=%016I64x %s\n", msg, wparam, lparam, "未知");
#else
	  printf("m=%08x w=%08x l=%08x %s\n", msg, wparam, lparam, "未知");
#endif
    //printf("%d\n", msg.message);
    //printf("%x\n", 0x8000);
  }
  return 0;
}

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#define CLEARTYPE_QUALITY 5
COLORREF WINAPI SetDCBrushColor(IN HDC, IN COLORREF);
COLORREF WINAPI SetDCPenColor(IN HDC, IN COLORREF);
#define DC_BRUSH 18
#define DC_PEN 19
#endif // WM_MOUSEWHEEL

int sys_tick_count() {
  return GetTickCount();
}

typedef struct {
  short KeyMap[512];
} sys_win32_t;
static sys_win32_t g_sys1[1] = {0};

static int sys_initio() {
  int i;
  static int inited=0;
  if (!inited) {
    SYSIO;
    short* KeyMap = g_sys1->KeyMap;
    inited = 1;
    mouse->dbclick_time = 0.3;
    (void)g;
    (void)x;
    (void)y;
    //SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
    //strcpy(io->def_font_name, lf.lfFaceName);
    //io->def_font_size = lf.lfHeight;
    for (i = 0; i < countof(g_sys1->KeyMap); ++i) {
      KeyMap[i] = i;
    }
    //KeyMap[VK_TAB] = KEY_TAB;
    KeyMap[VK_LEFT] = KEY_LEFT;
    KeyMap[VK_RIGHT] = KEY_RIGHT;
    KeyMap[VK_UP] = KEY_UP;
    KeyMap[VK_DOWN] = KEY_DOWN;
    KeyMap[VK_PRIOR] = KEY_PAGEUP;
    KeyMap[VK_NEXT] = KEY_PAGEDOWN;
    KeyMap[VK_HOME] = KEY_HOME;
    KeyMap[VK_END] = KEY_END;
    KeyMap[VK_DELETE] = KEY_DELETE;
    KeyMap[VK_BACK] = KEY_BACK;
    //KeyMap[VK_RETURN] = KEY_ENTER;
    KeyMap[VK_ESCAPE] = KEY_ESCAPE;
    //KeyMap[VK_CONTROL] = KEY_CTRL;
    //KeyMap[VK_SHIFT] = KEY_SHIFT;
    //KeyMap[VK_MENU] = KEY_ALT;
    //KeyMap[VK_MENU] = KEY_ALT;
  }
  return 0;
}
static LRESULT WINAPI imuiWndProc_impl(window_host_t* host, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  sysio_t* io = sys_getio();
  sysmouse_t* mouse = io->mouse;
  int x = mouse->pos->x, y = mouse->pos->y;
  sys_initio();
  io->msg = MSG_NOP;
  io->cur_host = host;
  //TraceMsg(hWnd, uMsg, wParam, lParam);
  switch (uMsg) {
  case WM_LBUTTONDOWN:
    mouse->code = MOUSE_LBUTTON;
    mouse->down[0] = true;
    mouse->pos[1] = mouse->pos[0];
    io->msg = MSG_LBUTTONDOWN;
    {
      int64 t1 = mouse->clicked_time[0];
      int64 t2 = sys_utime_counter();
      double tt = ((t2 - t1) * 1.0 / sys_utime_frequency());
      mouse->double_clicked[0] = tt < mouse->dbclick_time;
      mouse->clicked_time[0] = t2;
    }
    SetCapture(hWnd);
    return 1;
  case WM_LBUTTONUP:
    mouse->code = MOUSE_LBUTTON;
    mouse->double_clicked[MOUSE_LBUTTON] = false;
    mouse->down[MOUSE_LBUTTON] = false;
    ReleaseCapture();
    io->msg = MSG_LBUTTONUP;
    return 1;
  case WM_RBUTTONDOWN:
    mouse->code = MOUSE_RBUTTON;
    mouse->down[1] = true;
    io->msg = MSG_RBUTTONDOWN;
    return 1;
  case WM_RBUTTONUP:
    mouse->code = MOUSE_RBUTTON;
    mouse->down[1] = false;
    io->msg = MSG_RBUTTONUP;
    return 1;
  case WM_MBUTTONDOWN:
    mouse->code = MOUSE_MBUTTON;
    mouse->down[2] = true;
    io->msg = MSG_MBUTTONDOWN;
    return 1;
  case WM_MBUTTONUP:
    mouse->code = MOUSE_MBUTTON;
    mouse->down[2] = false;
    io->msg = MSG_MBUTTONUP;
    return 1;
  case WM_MOUSEWHEEL:
    io->msg = MSG_MOUSEWHEEL;
    mouse->wheelDelta = ((short)HIWORD(wParam)) > 0 ? +1.0f : -1.0f;
    return 1;
  case WM_MOUSEMOVE:
    io->msg = MSG_MOUSEMOVE;
    mouse->pos->x = (signed short)(lParam);
    mouse->pos->y = (signed short)(lParam >> 16);
    return 1;
  case WM_KEYDOWN:
    io->msg = MSG_KEYDOWN;
    io->keydown[io->last_key = g_sys1->KeyMap[wParam & 511]] = 1;
    return 1;
  case WM_KEYUP:
    io->msg = MSG_KEYUP;
    io->keydown[io->last_key = g_sys1->KeyMap[wParam & 511]] = 0;
    return true;
  case WM_CHAR:
    //io->KeyDownTime[0] = sys_utime(&io->KeyDownTime[0]) > io->KeyRepeatDelay;
    if (KEY_COUNT < g_sys1->KeyMap[wParam & 511]) {
      io->msg = MSG_CHAR;
      io->last_char = g_sys1->KeyMap[wParam & 511];
      return true;
    }
  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU) { // Disable ALT application menu
    return 1;
    }
    break;
  case WM_DESTROY:
    io->msg = MSG_CLOSE;
    PostQuitMessage(0);
    return 1;
  case WM_SETCURSOR:
    if (io->cursor
      // && res_set_cursor(io->cursor)
      ) {
      return 1;
    }
    break;
  case WM_SIZE:
    if (host && wParam != SIZE_MINIMIZED) {
      render_drv_t* drv = host->drv;
      if (drv->SetSize) {
        int w = LOWORD(lParam), h = HIWORD(lParam);
        drv->SetSize(drv, w, h);
        sys_get_window_rect(host->x, &host->rect, &host->root->rc);
      }
    return 1;
    }
    break;
  //case WM_ERASEBKGND:
    //InvalidateRect(hWnd, 0, 0);
    //return 1;
  case WM_PAINT:
    if (host) {
      //render_drv_t* drv = host->drv;
      //return 1;
      //drv->Render(drv, NULL, drv->clrbg);
      //return 1;
    }
    break;
  }
  return 0;
}

#ifndef SYSUI_DEFED
#define SYSUI_DEFED

static int sys_dc_get_image(window_dc_t* hdc, img_t* im)
{
  BITMAP bmp = {0};
  HGDIOBJ hbmp;
  int h, w, step, cn;
  GdiFlush();
  hbmp = GetCurrentObject((HDC)hdc, OBJ_BITMAP);
  
  if (hbmp == NULL) {
    return TRUE;
  }
  
  if (GetObject(hbmp, sizeof(bmp), &bmp) == 0) {
    return TRUE;
  }
  
  h = abs(bmp.bmHeight), w = abs(bmp.bmWidth), cn = bmp.bmBitsPixel / 8;
  step = (w * cn + 3) & -4;
  
  if (h * w * cn > 0) {
    IMINIT(im, h, w, bmp.bmBits, step, cn, 1);
  }
  
  return FALSE;
}
CC_INLINE int sys_dc_set_size(window_dc_t** hdc, int h, int w, int cn)
{
  if ((h|w|cn)==0) {
    if (*hdc) {
      DeleteDC((HDC)*hdc);
      *hdc = NULL;
    }
  } else {
    uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
    BITMAPINFO* binfo = (BITMAPINFO*) buffer;
    HBITMAP hBmp;
    uchar* dst_ptr = 0;
    img_t im[1] = {0};
    if (NULL == *hdc) {
      *hdc = (window_dc_t*)CreateCompatibleDC(0);
    }
    
    sys_dc_get_image(*hdc, im);
    //if (im->h!=h || im->w!=w || im->c!=cn)
    {
      FillBitmapInfo(binfo, w, h, cn * 8, 1, 0);
      hBmp = CreateDIBSection((HDC)*hdc, binfo, DIB_RGB_COLORS, (void**)&dst_ptr, 0, 0);
      DeleteObject(SelectObject((HDC)*hdc, hBmp));
    }
  }
  return 0;
}

int sys_waitio(int delay)
{
  SYSIO;
  int time0 = GetTickCount();
  static MSG msg[1] = {0};
  io->msg = MSG_NOP;
  //io->time = sys_utime_counter();
  if (!(mouse->down[0] || mouse->down[1] || mouse->down[2])) {
    io->hitid = NULL;
  }
  memset(msg, 0, sizeof(*msg));
  if (delay > 0) {
    while (abs((int)(GetTickCount() - time0)) < delay) {
      if (PeekMessage(msg, 0, 0, 0, PM_REMOVE) == FALSE) {
        Sleep(1);
      }
      else {
        TranslateMessage(msg);
        DispatchMessage(msg);
        break;
      }
    }
  }
  else {
    GetMessage(msg, 0, 0, 0);
    TranslateMessage(msg);
    DispatchMessage(msg);
  }
  //TraceMsg(msg->hwnd, msg->message, msg->lParam, msg->wParam);
  //if (!g->g_FontTexture) { imuiImplDX9_CreateDeviceObjects(); }
  {
    // Setup display size (every frame to accommodate for window resizing)
    static int g_TicksPerSecond = 0;
    static int pre_time = 0;
    int current_time;
    // Setup time step
    // Read keyboard modifiers inputs
    io->keyctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    io->keyshift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    io->keyalt = (GetKeyState(VK_MENU) & 0x8000) != 0;
    //io->keydown[KEY_SUPER] = false;
    // io->keydown : filled by WM_KEYDOWN/WM_KEYUP events
    // mouse->pos : filled by WM_MOUSEMOVE events
    // mouse->down : filled by WM_*BUTTON* events
    // mouse->wheel : filled by WM_MOUSEWHEEL events
    // Hide OS mouse cursor if ImGui is drawing it
    //SetCursor(io->MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));
    // Start the frame
    //imuiNewFrame();
    current_time = GetTickCount();
    //io->delta_time = (double)(current_time - g_Time) / g_TicksPerSecond;
    io->tick = (io->time>0 && (current_time - io->time) > delay);
    if (io->msg==MSG_NOP && current_time-pre_time > delay) {
      io->msg = MSG_TIMER;
      pre_time = current_time;
    }
    io->time = current_time;
  }
  return 1;
  //return msg->message != WM_QUIT;
}
#include <winuser.h>
static LRESULT WINAPI imuiWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  sysio_t* io = sys_getio();
  WindowMsg msg = io->msg;
  window_host_t* host = (window_host_t*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
  int ret = 0;
  io->msg = MSG_NOP;
  io->msg_host = host;
  ret = imuiWndProc_impl(host, hWnd, uMsg, wParam, lParam);
  if (host && host->event_fun) {
    host->event_fun(io);
  }
  if (ret) {
    return 1;
  }
  io->msg = msg;
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
#define IMGUIWIN32_CLASSNAME "imguiwindow"

static WNDCLASSEXA* getWndClass()
{
  static WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, imuiWndProc, 0L, 0L, NULL, NULL, NULL, NULL, NULL, IMGUIWIN32_CLASSNAME, NULL };
  if (!wc.hInstance) {
    //LOGFONT lf;
    sys_initio();
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    if (!RegisterClassExA(&wc)) {
      return 0;
    }
  }
  return &wc;
}
static int sys_ui_exit()
{
  UnregisterClassA(IMGUIWIN32_CLASSNAME, getWndClass()->hInstance);
  return 0;
}
window_native_t* sys_create_window(int flag, const char* name, int x, int y, int w, int h, window_host_t* host) {
  DWORD Style = WS_OVERLAPPEDWINDOW;
  DWORD ExStyle = 0;
  RECT rc = {x, y, x+w, y+h};
  HWND hWnd = NULL;
  WNDCLASSEXA* pwc;
  host->flip_y = true;
  pwc = getWndClass();
  if (1) {
    AdjustWindowRectEx(&rc, Style, FALSE, ExStyle);
    x = rc.left, y = rc.top, w = rc.right-rc.left, h = rc.bottom - rc.top;
    //SetWindowPos(NULL, 0, 0, rcClient.Width(), rcClient.Height(), SWP_NOMOVE | SWP_NOZORDER);
  }
  hWnd = CreateWindowExA(ExStyle, IMGUIWIN32_CLASSNAME, name, Style, x, y, w, h, NULL, NULL, pwc->hInstance, NULL);
  if (!hWnd) {
    const char* ss = sys_errmsg(0);
    printf("err: %s\n", ss);
    return 0;
  }
  SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)host);
  ShowWindow(hWnd, SW_SHOWDEFAULT);
  UpdateWindow(hWnd);
  return (window_native_t*)hWnd;
}
int sys_set_window_rect(window_native_t* w, IRECT* pwindow, IRECT* pclient) {
  IRECT rc = {0};
  assert(w);
  if (pclient) {
    rc = *pclient;
    AdjustWindowRectEx((RECT*)&rc, GetWindowLong((HWND)w, GWL_STYLE), FALSE, GetWindowLong((HWND)w, GWL_EXSTYLE));
  } else {
    rc = *pwindow;
  }
  MoveWindow((HWND)w, rc.l, rc.t, RCW(&rc), RCH(&rc), FALSE);
  return 0;
}
static int sys_get_window_rect(window_native_t* w, IRECT* pwindow, IRECT* pclient) {
  if (pwindow) {
    GetWindowRect((HWND)w, (RECT*)pwindow);
  }
  if (pclient) {
    GetClientRect((HWND)w, (RECT*)pclient);
  }
  return 0;
}
int sys_set_window_text(window_native_t* w, const char* text) {
  return SetWindowTextA((HWND)w, text);
}
int sys_window_bitblt(window_native_t* dst_win, int x, int y, int w, int h, window_dc_t* src_dc, int src_x, int src_y) {
  HDC hDC = GetDC((HWND)dst_win);
  BitBlt(hDC, x, y, w, h, (HDC)src_dc, src_x, src_y, SRCCOPY);
  ReleaseDC((HWND)dst_win, hDC);
  return 0;
}
static int sys_del_dc(window_dc_t* dc) {
  return DeleteDC((HDC)dc);
}
/////////////////////////////////////////////////////////////////////////////////////////

#endif // SYSUI_DEFED

///////////////////////////////////////////////////////////////////////////////////////////
int sys_cpfile(const char* lpExistingFileName, const char* lpNewFileName, int bFailIfExists) {
  return CopyFileA( lpExistingFileName, lpNewFileName, bFailIfExists );
}
int sys_rmfile(const char* lpFileName) {
  return DeleteFileA( lpFileName );
}
int sys_rmdir(const char* lpFileName) {
  return RemoveDirectoryA( lpFileName );
}
int sys_mvfile(const char* lpExistingFileName, const char* lpNewFileName) {
  return MoveFileA( lpExistingFileName, lpNewFileName );
}

///////////////////////////////////////////////////////////////////////////////////////////
//删除文件或者文件夹
#include <ShellAPI.h>
int sys_fileopt(FileOptFlag opt, const char* strFrom, const char* strTo)
{
  int bResult = 0;
  char* pFrom = NULL;
  char* pTo = NULL;
  SHFILEOPSTRUCTA FileOp={0};
  cstr_redup(&pFrom, strFrom);
  cstr_redup(&pTo, strTo);
  FileOp.fFlags = //FOF_ALLOWUNDO |   //允许放回回收站
    FOF_NOCONFIRMATION |  //不出现确认对话框
    FOF_NOERRORUI; //不出现错误对话框
  FileOp.pFrom = pFrom;
  FileOp.pTo = pTo;      //一定要是NULL
  switch (opt) {
  case FOF_MOVE:
    FileOp.wFunc = FO_MOVE;      //移动
    break;
  case FOF_COPY:
    FileOp.wFunc = FO_COPY;      //拷贝
    break;
  case FOF_DELETE:
    FileOp.wFunc = FO_DELETE;    //删除
    break;
  case FOF_RENAME:
    FileOp.wFunc = FO_RENAME;    //重命名
    break;
  }
  bResult = SHFileOperationA(&FileOp);
  if (pFrom) {
    pfree(pFrom);
  }
  if (pTo) {
    pfree(pTo);
  }
  return bResult;
}

/////////////////////////////////////////////////////////////////////////////////////////
// thread
int sys_sleep(int ms) {
  Sleep(ms);
  return 0;
}


int sys_spin_lock(volatile long *address) {
  int ret;
  do {
    while (*address) {YIELDING;};
    
#if defined(_MSC_VER) && !defined(__clang__)
    // use intrinsic instead of inline assembly
    ret = InterlockedExchange((volatile LONG *)address, 1);
    // inline assembly
    /*__asm {
    mov eax, address
    mov ebx, 1
    xchg [eax], ebx
    mov ret, ebx
  }*/
#else
#endif
    
  } while (ret);
  return ret;
}

int sys_spin_unlock(volatile long *address) {
  MB;
  *address = 0;
  return 0;
}

int sys_lock_init(lock_t* c)
{
  CRITICAL_SECTION* cs = (CRITICAL_SECTION*)sys_malloc(sizeof(CRITICAL_SECTION));
  c->x = cs;
  InitializeCriticalSection(cs);
  return 0;
}
int sys_lock_destroy(lock_t* c)
{
  CRITICAL_SECTION* cs = (CRITICAL_SECTION*)c->x;
  DeleteCriticalSection(cs);
  sys_free(cs);
  c->x = NULL;
  return 0;
}
int sys_lock_lock(lock_t* c)
{
  CRITICAL_SECTION* cs = (CRITICAL_SECTION*)c->x;
  EnterCriticalSection(cs);
  return 0;
}
int sys_lock_unlock(lock_t* c)
{
  CRITICAL_SECTION* cs = (CRITICAL_SECTION*)c->x;
  LeaveCriticalSection(cs);
  return 0;
}
int sys_event_init(event_t* c, bool manual_reset, bool init_state) {
  c->x = CreateEventA(NULL, manual_reset, init_state, NULL);
  return 0;
}
int sys_event_destroy(event_t* c) {
  CloseHandle((HANDLE)c->x);
  c->x = NULL;
  return 0;
}
int sys_event_wait(event_t* c, int dwMillises) {
  DWORD ret = WaitForSingleObject((HANDLE)c->x, dwMillises);
  if (ret == WAIT_OBJECT_0) {  
    return 0;
  }
  if (ret == WAIT_TIMEOUT) {
    return 1;
  }  
  return -1;  
}
int sys_event_signal(event_t* c) {
  return !SetEvent((HANDLE)c->x);
}
int sys_event_reset(event_t* c) {
  int ret = ResetEvent((HANDLE)c->x);
  //ResetEvent 返回非零表示成功
  if (ret) { return 0; }
  return -1;
}
//typedef WINAPI void* (*thread_cb)(void* pParam);
//创建一个匿名互斥对象
int sys_mutex_init(mutex_t* c, const char* name)
{
  c->x = CreateMutex(NULL, FALSE, NULL);
  return 0;
}
//销毁互斥对象，释放资源
int sys_mutex_destroy(mutex_t* c)
{
  CloseHandle((HANDLE)c->x);
  c->x = 0;
  return 0;
}
//确保拥有互斥对象的线程对被保护资源的独自访问
int sys_mutex_lock(mutex_t* c)
{
  DWORD d = WaitForSingleObject((HANDLE)c->x, INFINITE);
  return 0;
}
//释放当前线程拥有的互斥对象，以使其它线程可以拥有互斥对象，对被保护资源进行访问
int sys_mutex_unlock(mutex_t* c)
{
  ReleaseMutex((HANDLE)c->x);
  return 0;
}
static DWORD WINAPI ThreadProxy(LPVOID args)
{
  thread_t* th = (thread_t*)(args);
  DWORD ret = 0;
  if (th && th->run) {
    ret = th->run(th->arg);
  }
  return ret;
}

//typedef void* uintptr_t;

int sys_thread_create(thread_t* th) {
  HANDLE hthread = CreateThread((LPSECURITY_ATTRIBUTES)(th->security), th->stack_size, ThreadProxy, th, th->initflag, (unsigned long *)&th->threadID);
  th->x = hthread;
  return 0;
}
static int sys_thread_creates(int n, thread_t* th)
{
  int i;
  for (i = 0; i < n; ++i) {
    sys_thread_create(th+i);
  }
  return 0;
}
int sys_thread_join(thread_t* th)
{
  int i = 0, ret = 1;
  if (WaitForSingleObject((HANDLE)th->x, INFINITE) != WAIT_OBJECT_0) {
    TerminateThread((HANDLE)th->x, 0);
    memset(th, 0, sizeof(*th));
    ret = 0;
  }
  return ret;
}
static int sys_thread_wait(int count, thread_t* th)  
{  
  int waitingThreadsCount = count;  
  int index = 0, i;
  DWORD res = 0;
  HANDLE handles[MAXIMUM_WAIT_OBJECTS];
  while(waitingThreadsCount > 0)
  {
    int len = MIN(waitingThreadsCount, MAXIMUM_WAIT_OBJECTS);
    for (i=0; i<len; ++i) {
      handles[i] = (HANDLE)th[index].x;
    }
    res = WaitForMultipleObjects(len, handles, TRUE, INFINITE);  
    if(res == WAIT_TIMEOUT || res == WAIT_FAILED)  
    {  
      printf("1. Wait Failed.");  
      return res;  
    }
    waitingThreadsCount -= MAXIMUM_WAIT_OBJECTS;  
    index += MAXIMUM_WAIT_OBJECTS;
  }
  return res;  
}
static int sys_thread_joins(int n, thread_t* th)
{
  int i = 0, ret = 1;
  if (1==n) {
    if (WaitForSingleObject((HANDLE)th->x, INFINITE) != WAIT_OBJECT_0) {
      TerminateThread((HANDLE)th->x, 0);
      memset(th, 0, sizeof(*th));
      ret = 0;
    }
  } else {
    if (sys_thread_wait(n, th) != WAIT_OBJECT_0) {
      for (i = 0; i < n; ++i) {
        TerminateThread((HANDLE)th[i].x, 0);
      }
      memset(th, 0, n*sizeof(*th));
      ret = 0;
    }
  }
  return ret;
}
static int sys_thread_detach(int n, thread_t* th)
{
  int i, ret=1;
  for (i = 0; i < n; ++i) {
    int ret1 = (TRUE == CloseHandle((HANDLE)th[i].x));
    ret = ret && ret1;
  }
  memset(th, 0, n*sizeof(thread_t));
  return ret;
}
static int sys_thread_self(thread_t* th) {
  // GetCurrentThread();
  return 0;
}
int sys_thread_id() {
  DWORD tid = GetCurrentThreadId();
  return tid;
}
int sys_getpid() {
  DWORD pid = GetCurrentProcessId();
  return pid;
}

/////////////////////////////////////////////////////////////////////////////////////////
#define ttf_buf_size  (32768-32)
typedef struct {
  HDC m_dc;
  HFONT m_cur_font;
  HFONT m_old_font;
  char m_gbuf[ttf_buf_size];
  TEXTMETRICW textmetric;
  float ascent;
  float IndexXAdvance[1<<16];
} ttffont_win32_t;

#define fx_to_dbl(_p)  ((_p).value + (_p).fract / 65536.0)
#define dbl_to_u8(_p)  BOUND((int)(_p), -127, 127)


static int decompose_win32_glyph_outline(const char* gbuf, unsigned total_size, bool flip_y, char* obuf, int olen, float ascent)
{
  const char* cur_glyph = gbuf;
  const char* end_glyph = gbuf + total_size;
  double x, y;
  char* obuf0 = obuf;
  char* obuf1 = obuf+olen;
  //double minx=10000, miny=10000;
  double maxx=0, maxy=0;
  
  while(cur_glyph < end_glyph) {
    const TTPOLYGONHEADER* th = (TTPOLYGONHEADER*)cur_glyph;
    ttpolygon* oh = (ttpolygon*)obuf;
    const char* end_poly = cur_glyph + th->cb;
    const char* cur_poly = cur_glyph + sizeof(TTPOLYGONHEADER);
    char* opoly = obuf + sizeof(ttpolygon);
    x = fx_to_dbl(th->pfxStart.x);
    y = fx_to_dbl(th->pfxStart.y);
    y = flip_y ? (ascent-y) : y;
    oh->start.x = dbl_to_u8(x);
    oh->start.y = dbl_to_u8(y);
    //if(flip_y) y = -y;

    while(cur_poly < end_poly) {
      const TTPOLYCURVE* pc = (const TTPOLYCURVE*)cur_poly;
      ttpoly* op = (ttpoly*)opoly;
      int i, type = TT_PRIM_LINE;
      op->npt = pc->cpfx;
      ASSERT(pc->cpfx<256);
      if (pc->wType == TT_PRIM_LINE) {
        type = TT_LINE;
      }
      if (pc->wType == TT_PRIM_CSPLINE) {
        type = TT_CSPLINE;
      }
      if (pc->wType == TT_PRIM_QSPLINE) {
        type = TT_QSPLINE;
      }
      op->type = type;
      for (i = 0; i < pc->cpfx; i++) {
        x = fx_to_dbl(pc->apfx[i].x);
        y = fx_to_dbl(pc->apfx[i].y);
        y = flip_y ? (ascent-y) : y;
        //path.line_to(value_type(dbl_to_int26p6(x)), value_type(dbl_to_int26p6(y)));
        op->pt[i].x = dbl_to_u8(x);
        op->pt[i].y = dbl_to_u8(y);
        //minx = MIN(minx, x);
        //miny = MIN(maxx, y);
        //maxx = MAX(maxx, x);
        //maxy = MAX(maxx, y);
      }
      cur_poly += sizeof(WORD) * 2 + sizeof(POINTFX) * pc->cpfx;
      opoly += sizeof(uchar) * 2 + sizeof(pointi8) * pc->cpfx;
    }
    cur_glyph += th->cb;
    oh->cb = opoly - obuf;
    obuf = opoly;
    ASSERT(obuf<obuf1);
  }
  return obuf - obuf0;
}

static int sys_font_get_glyph_w32(const ttffont_t* f, uint code, char* obuf, int olen) {
  ttffont_win32_t* s = (ttffont_win32_t*)(f->x);
  GLYPHMETRICS gm;
  int total_size, format = GGO_NATIVE;
  static const MAT2 m_matrix = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
  total_size = GetGlyphOutlineW(s->m_dc, code, format, &gm, ttf_buf_size, obuf ? (void*)s->m_gbuf : NULL, &m_matrix);
  if (total_size>0) {
    s->IndexXAdvance[code&0xff] = gm.gmCellIncX;
  }
  if (obuf) {
    total_size = decompose_win32_glyph_outline(s->m_gbuf, total_size, true, obuf, olen, s->ascent);
  }
  return total_size;
}
static int sys_font_del_w32(ttffont_t* f) {
  ttffont_win32_t* s = (ttffont_win32_t*)(f->x);
  SelectObject(s->m_dc, s->m_old_font);
  DeleteObject(s->m_cur_font);
  ReleaseDC(NULL, s->m_dc);
  sys_free(s);
  memset(f, 0, sizeof(*f));
  return 0;
}
int sys_font_set(ttffont_t* f, const char* fontname, int style) {
  int m_height = 128;
  int m_width = 0;
  int m_italic = 0;
  int m_weight = FW_REGULAR;
  int m_char_set = DEFAULT_CHARSET;
  int m_pitch_and_family = FF_DONTCARE;
  int m_hinting = 0;
  HFONT m_cur_font = NULL;
  LOGFONTA lf = {0};
  if (f->x) {
    if (0==strcmp(f->name, fontname)) {
      return 1;
    }
    ttffont_del(f);
  }
  strncpy(lf.lfFaceName, fontname, 32);
  lf.lfHeight = -m_height;
  lf.lfWeight = style & FontStyleBold ? FW_BOLD : FW_REGULAR;
  lf.lfItalic = style & FontStyleItalic;
  lf.lfUnderline = style & FontStyleUnderline;
  lf.lfStrikeOut = style & FontStyleStrikeout;
  
  m_cur_font = CreateFontIndirectA(&lf);
  if (m_cur_font) {
    ttffont_win32_t* s = (ttffont_win32_t*)sys_malloc(sizeof(ttffont_win32_t));
    s->m_dc = GetDC(NULL);
    s->m_old_font = (HFONT)SelectObject(s->m_dc, m_cur_font);
    GetTextMetricsW(s->m_dc, &s->textmetric);
    s->m_cur_font = m_cur_font;
    strncpy(f->name, fontname, 32);
    f->info->FallbackXAdvance = f->info->size = m_height;
    s->ascent = s->textmetric.tmAscent * f->info->size/s->textmetric.tmHeight;
    f->x = s;
    f->get_glyph = sys_font_get_glyph_w32;
    f->del = sys_font_del_w32;
    if (1) {
      int i, len = 0;
      int total_size, format = GGO_NATIVE;
      static const MAT2 m_matrix = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
      f->info->IndexXAdvance = s->IndexXAdvance;
      f->info->IndexXAdvance_Size = countof(s->IndexXAdvance);
      for (i=0; i<256; ++i) {
        s->IndexXAdvance[i] = m_height/2;
      }
      for (; i<f->info->IndexXAdvance_Size; ++i) {
        s->IndexXAdvance[i] = m_height;
      }
      for (i=32; i<128; ++i) {
        GLYPHMETRICS gm;
        s->IndexXAdvance[0] = 0;
        total_size = GetGlyphOutlineW(s->m_dc, i, format, &gm, ttf_buf_size, NULL, &m_matrix);
        if (total_size>0) {
          s->IndexXAdvance[i] = gm.gmCellIncX;
        }
      }
    }
    return 1;
  }
  return m_cur_font!=NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////
int sys_debug_puts(const char* str) {
  OutputDebugStringA(str);
  return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
int sys_msgbox(const char* fmt, ...) {
  char dbg_buf[256];
  va_list arglist;
  va_start(arglist, fmt);
  cstr_vnprintf(dbg_buf, countof(dbg_buf), fmt, arglist);
  va_end(arglist);
  return MessageBoxA(NULL, dbg_buf, NULL, MB_OK);
}
static int MyAssert(const char* pStatement, const char* pFileName, unsigned int LineNumber)
{
  char Message[1024];
  sprintf(Message, "%s(%d) : assert: %s",  pFileName, LineNumber, pStatement);
  printf("%s\n", Message);
  sys_msgbox(Message);
  return 0;
}
//#define ASSERT(exp)  (void)( (exp) || (MyAssert(#exp, __FILE__, __LINE__), 0) )

/////////////////////////////////////////////////////////////////////////////////////////

static BOOL bitmap2im(HBITMAP hBmp, img_t* im)
{
  int nChannels, n;
  int depth;
  BITMAP bmp;
  GetObject(hBmp, sizeof(BITMAP), &bmp);
  nChannels = bmp.bmBitsPixel/8;
  depth = MIN(8, bmp.bmBitsPixel);
  imsetsize(im, bmp.bmHeight, bmp.bmWidth, nChannels, 1);
  n = GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidthBytes, im->tt.data);
  //imsetalpha(im, 255);
  
#if 0
  if (1 == depth) {
    int i, k = bmp.bmWidthBytes;
    Color32 colorTable[2] = {0};
    char* bit = memdup(im->data, n);
    colorTable[0].argb = 0xffffffff;

    for (i = 0; i < bmp.bmHeight; ++i) {
      rgbColorToGray_line(bit + i * k, depth, colorTable, im->data + i * im->step, bmp.bmWidth);
    }

    free(bit);
  }
  else {
    if (32 != bmp.bmBitsPixel) {
      int i, k = bmp.bmWidthBytes;
      char* bit = memdup(im->data, n);

      for (i = 0; i < bmp.bmHeight; ++i) {
        rgbColorToColor32_line(bit + i * k, bmp.bmBitsPixel, 0, (Color32*)(im->data + i * im->step), bmp.bmWidth);
      }

      free(bit);
    }
  }
#endif

  return 0;
}
//考贝屏幕 lpRect 代表选定区域
static int imcopyscreen(const IRECT* lpRect, img_t* im)
{
  HDC hScrDC, hMemDC;
  // 屏幕和内存设备描述表
  HBITMAP hBitmap, hOldBitmap;
  // 位图句柄
  int nX, nY, nX2, nY2;
  // 选定区域坐标
  int nWidth, nHeight;
  int m_xScreen, m_yScreen;

  //获取屏幕分辩率
  m_xScreen = GetSystemMetrics(SM_CXSCREEN);
  m_yScreen = GetSystemMetrics(SM_CYSCREEN);

  if (lpRect) {
    // 确保选定区域不为空矩形
    if (iRectIsEmpty(lpRect)) {
      return 0;
    }

    // 获得选定区域坐标，确保选定区域是可见的
    nX = MAX(0, lpRect->l);
    nY = MAX(0, lpRect->t);
    nX2 = MIN(m_xScreen, lpRect->r);
    nY2 = MIN(m_yScreen, lpRect->b);
  }
  else {
    nX = 0, nY = 0, nX2 = m_xScreen, nY2 = m_yScreen;
  }

  nWidth = nX2 - nX;
  nHeight = nY2 - nY;
  
  //为屏幕创建设备描述表
  hScrDC = CreateDCA("DISPLAY", NULL, NULL, NULL);
  
  //为屏幕设备描述表创建兼容的内存设备描述表
  hMemDC = CreateCompatibleDC(hScrDC);

  // 创建一个与屏幕设备描述表兼容的位图
  hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
  // 把新位图选到内存设备描述表中
  hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
  // 把屏幕设备描述表拷贝到内存设备描述表中
// #define SRCCOPY             (DWORD)0x00CC0020
  BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);
  
  hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
  //得到屏幕位图的句柄
  //清除
  DeleteDC(hScrDC);
  DeleteDC(hMemDC);
  // 返回位图句柄
  bitmap2im(hBitmap, im);
  DeleteObject(hBitmap);
  return 1;
}


int FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin, const RGBQUAD* inpal);

static int SelectImagePal(HDC hdc, int height, int width, const unsigned char* arr, int step, int cn, const RGBQUAD* inpal)
{
  void* dst_ptr = 0;
  int origin = 0;
  //CvMat stub, dst;
  BOOL changed_size = FALSE; // philipg
  HBITMAP hBmp;
  uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
  BITMAPINFO* binfo = (BITMAPINFO*) buffer;

  if (inpal) {
    LOGPALETTE* lpPal = (LOGPALETTE*)buffer; // 指向逻辑调色板的指针
    HPALETTE hPal = NULL; // 调色板的句柄
    int i;
    lpPal->palVersion = 0x300; // 设置版本号
    lpPal->palNumEntries = (WORD) 256; // 设置颜色数目

    for (i = 0; i < (int) 256; i++) {
      lpPal->palPalEntry[ i ].peRed = inpal[ i ].rgbRed;
      lpPal->palPalEntry[ i ].peGreen = inpal[ i ].rgbGreen;
      lpPal->palPalEntry[ i ].peBlue = inpal[ i ].rgbBlue;
      lpPal->palPalEntry[ i ].peFlags = 0;
    }

    hPal = CreatePalette(lpPal);
    DeleteObject(SelectPalette(hdc, hPal, TRUE));
  }

  changed_size = TRUE;
  FillBitmapInfo(binfo, width, height, cn * 8, 1, inpal);
  hBmp = CreateDIBSection(hdc, binfo, DIB_RGB_COLORS, &dst_ptr, 0, 0);

  if (dst_ptr) {
    int i, dststep = (width * cn + 3) & -4;
    unsigned char* dst = (unsigned char*)dst_ptr + (height - 1) * dststep;

    //IM2IM( size.h, size.w, arr, step, cn, dst, -dststep, cn );
    for (i = 0; i < height; ++i) {
      memcpy(dst + i * (-dststep), arr + i * step, width * cn);
    }

    if (1 == cn) {
      int s = 0;
      SKIP_MAX_ELEMENT(height, width, arr, step, cn, s);
      dst = (unsigned char*)dst_ptr;

      if (1 == arr[s]) {
        int i;

        for (i = 0; i < height * dststep; ++i) {
          dst[i] = dst[i] ? 255 : 0;
        }
      }
    }
  }

  DeleteObject(SelectObject(hdc, hBmp));
  DeleteObject(hBmp);
  return 0;
}
static HBITMAP mkHBITMAP(int height, int width, const unsigned char* arr, int step, int cn)
{
  HBITMAP hBmp;
  uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
  BITMAPINFO* binfo = (BITMAPINFO*) buffer;
  HDC hdc = GetDC(0);
  void* dst_ptr = 0;
  FillBitmapInfo(binfo, width, height, cn * 8, 1, 0);
  hBmp = CreateDIBSection(hdc, binfo, DIB_RGB_COLORS, &dst_ptr, 0, 0);
  memcpy(dst_ptr, arr, height * width * cn);
  ReleaseDC(0, hdc);
  return hBmp;
}
#if 0
static HBITMAP HBITMAP_resize(HBITMAP hBmp, double ss)
{
  img_t im[1] = {0};
  img_t im2[1] = {0};
  int h, w;
  bitmap2im(hBmp, im);
  h = (int)(ss * im->h);
  w = (int)(ss * im->w);
  imresize(im, h, w, im2);
  hBmp = mkHBITMAP(h, w, im2->tt.data, im2->s, im2->c);
  imfree(im);
  imfree(im2);
  return hBmp;
}
static int test_HBITMAP()
{
  int h = 100, w = 100, al = w * 4;
  unsigned char arr[100 * 100 * 4] = {0};
  HBITMAP hBmp = 0;
  HBITMAP hBmp2 = 0;
  img_t im[1] = {0};
  memset(arr, 255, 100 * 100 * 2);
  hBmp = mkHBITMAP(h, w, arr, al, 4);
  hBmp2 = HBITMAP_resize(hBmp, 2);
  bitmap2im(hBmp2, im);
  //imshow(im);
  //cvWaitKey(-1);
  return 0;
}
BOOL WINAPI GetIconInfo( HICON hIcon, ICONINFO* piconinfo);
static int icon2im(HICON hIcon, img_t* imColor)
{
  if (hIcon) {
    ICONINFO csII;
    int bRetValue = GetIconInfo(hIcon, &csII);
    bitmap2im(csII.hbmColor, imColor);

    if (1) {
      img_t imMask[1] = {0};
      bitmap2im(csII.hbmMask, imMask);
      imsetmask(imColor, imMask);
      imfree(imMask);
    }
  }

  return 0;
}

static int imagelist2im(HIMAGELIST hImageList, img_t* im)
{
  int n = ImageList_GetImageCount(hImageList);
  COLORREF c = ImageList_GetBkColor(hImageList);
  //HBITMAP bmp = ConvertIconToBitmap(ico);
  int i, cx, cy;
  img_t im2[1] = {0};
  ImageList_GetIconSize(hImageList, &cx, &cy);
  imsetsize(im, cy * n, cx, 4, 1);

  for (i = 0; i < n; ++i) {
    HICON hIcon = ImageList_GetIcon(hImageList, i, 0);
    icon2im(hIcon, im2);
    memcpy(im->tt.data + i * cy * im->s, im2->tt.data, cy * im->s);
    DestroyIcon(hIcon);
  }

  return 0;
}

typedef void* LPITEMIDLIST;
HRESULT WINAPI SHGetSpecialFolderLocation(HWND hwndOwner, int nFolder, LPITEMIDLIST * ppidl);

static int test_SHGetFileInfo()
{
  LPITEMIDLIST itemlist = NULL;
  SHFILEINFO sfi;
  img_t im[2] = {0};
  char buf[MAX_PATH];
  SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &itemlist);
  ZeroMemory(&sfi, sizeof(SHFILEINFO));
  SHGetPathFromIDList(itemlist, buf);
  SHGetFileInfo(buf, 0, &sfi, sizeof(sfi), SHGFI_DISPLAYNAME | SHGFI_ICON | SHGFI_SMALLICON);
  icon2im(sfi.hIcon, im);
  imfree(im);
  return 0;
}
// 返回索引号
// uFlag == SHGFI_LARGEICON | SHGFI_SMALLICON
static int GetFileIcon(const char* fn, img_t* im, UINT uFlag)
{
  SHFILEINFO sfi[1] = {0};
  SHGetFileInfo(fn, 0, sfi, sizeof(SHFILEINFO), SHGFI_TYPENAME | SHGFI_DISPLAYNAME | SHGFI_ICON | uFlag);
  icon2im(sfi->hIcon, im);
  return sfi->iIcon;
}
#endif


int sys_reg_load_data(const char* keyname, const char* dataname, void* pdata, int len)
{
  HKEY hkey;
  char szKey[ 1024 ];
  DWORD dwType = 0;
  DWORD dwSize = len;
  strcat(szKey, keyname);
  
  if (RegOpenKeyExA(HKEY_CURRENT_USER, szKey, 0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS) {
    RegQueryValueExA(hkey, dataname, NULL, &dwType, (BYTE*)pdata, &dwSize);
    RegCloseKey(hkey);
  }
  
  return dwSize;
}

int sys_reg_save_data(const char* keyname, const char* dataname, const void* pdata, int len)
{
  static const DWORD MAX_RECORD_COUNT = 100;
  HKEY hkey;
  char szKey[ 1024 ];
  char rootKey[ 1024 ];
  strcat(szKey, keyname);
  
  if (RegOpenKeyExA(HKEY_CURRENT_USER, szKey, 0, KEY_READ, &hkey) != ERROR_SUCCESS) {
    HKEY hroot;
    DWORD count = 0;
    FILETIME oldestTime = { UINT_MAX, UINT_MAX };
    char oldestKey[ 1024 ];
    char currentKey[ 1024 ];
    strcpy(rootKey, keyname);
    rootKey[ strlen(rootKey) - 1 ] = '\0';
    
    if (RegCreateKeyExA(HKEY_CURRENT_USER, rootKey, 0, NULL, 0, KEY_READ + KEY_WRITE, 0, &hroot, NULL) != ERROR_SUCCESS)
      //RegOpenKeyEx( HKEY_CURRENT_USER,rootKey,0,KEY_READ,&hroot) != ERROR_SUCCESS )
    {
      return 0;
    }
    
    for (;;) {
      DWORD csize = sizeof(currentKey);
      FILETIME accesstime = { 0, 0 };
      LONG code = RegEnumKeyExA(hroot, count, currentKey, &csize, NULL, NULL, NULL, &accesstime);
      
      if (code != ERROR_SUCCESS && code != ERROR_MORE_DATA) {
        break;
      }
      
      count++;
      
      if (oldestTime.dwHighDateTime > accesstime.dwHighDateTime ||
        (oldestTime.dwHighDateTime == accesstime.dwHighDateTime &&
        oldestTime.dwLowDateTime > accesstime.dwLowDateTime)) {
        oldestTime = accesstime;
        strcpy(oldestKey, currentKey);
      }
    }
    
    if (count >= MAX_RECORD_COUNT) {
      RegDeleteKeyA(hroot, oldestKey);
    }
    
    RegCloseKey(hroot);
    
    if (RegCreateKeyExA(HKEY_CURRENT_USER, szKey, 0, NULL, 0, KEY_WRITE, 0, &hkey, NULL) != ERROR_SUCCESS) {
      return 0;
    }
  }
  else {
    RegCloseKey(hkey);
    
    if (RegOpenKeyExA(HKEY_CURRENT_USER, szKey, 0, KEY_WRITE, &hkey) != ERROR_SUCCESS) {
      return 0;
    }
  }
  
  RegSetValueExA(hkey, dataname, 0, REG_DWORD, (BYTE*)pdata, len);
  RegCloseKey(hkey);
  return 0;
}
#include <ShlObj.h>
char* sys_browse_folder(const char* title, char* buf, int len)
{
  BROWSEINFOA bi = {0};
  LPITEMIDLIST pIDList = NULL;
  //初始化入口参数bi
  bi.hwndOwner = NULL;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = buf;    //此参数如为NULL则不能显示对话框  
  bi.lpszTitle = title ? title : "选择路径";
  bi.ulFlags = BIF_RETURNONLYFSDIRS;
  bi.lpfn = NULL;
  bi.iImage = 0;
  
  pIDList = SHBrowseForFolderA(&bi);  //调用显示选择对话框  
  if(pIDList) {
    SHGetPathFromIDListA(pIDList,buf);           //取得文件夹路径到buf里
  }
  return buf;  
}
char* sys_open_filename(const char* szFilter, char* szFile, int len)
{
  OPENFILENAMEA ofn = {0}; // 公共对话框结构。
  szFile[0] = len;
  // 初始化选择文件对话框。
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = szFile;
  ofn.lpstrFile[ 0 ] = ('\0');
  ofn.nMaxFile = len;
  ofn.lpstrFilter = szFilter ? szFilter : ("All\0*.*\0图片\0*.bmp\0");
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  // 显示打开选择文件对话框
  if (GetOpenFileNameA(&ofn)) {
    //显示选择的文件。
    //OutputDebugString(szFile);
    //OutputDebugString(_T("\r\n"));
  }
  return szFile;
}
#if 0
static void ngx_gettimeofday(struct timeval *tp)  
{  
    uint64_t  intervals;  
    FILETIME  ft;  
  
    GetSystemTimeAsFileTime(&ft);  
  
    /* 
     * A file time is a 64-bit value that represents the number 
     * of 100-nanosecond intervals that have elapsed since 
     * January 1, 1601 12:00 A.M. UTC. 
     * 
     * Between January 1, 1970 (Epoch) and January 1, 1601 there were 
     * 134744 days, 
     * 11644473600 seconds or 
     * 11644473600,000,000,0 100-nanosecond intervals. 
     * 
     * See also MSKB Q167296. 
     */  
  
    intervals = ((uint64_t) ft.dwHighDateTime << 32) | ft.dwLowDateTime;  
    intervals -= 116444736000000000;  
  
    tp->tv_sec = (long) (intervals / 10000000);  
    tp->tv_usec = (long) ((intervals % 10000000) / 10);  
}
#endif

#include "sys_win32_gui.inl"
//#include "sys_win32_mmsystem.inl"
#ifdef USE_SERIAL
#include "sys_win32_serial_port.inl"
#endif
