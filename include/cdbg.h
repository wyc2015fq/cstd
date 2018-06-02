
#ifndef _CDBG_H_
#define _CDBG_H_

#ifndef C_DBG
#define tic
#define toc
#define toc_
#define tictoc
#define tictoc0
#else

static char dbg_buf[4096];
CC_INLINE char* dbg_sprintf(const char* fmt, ...) {
  va_list arglist;
  va_start(arglist, fmt);
  cstr_vnprintf(dbg_buf, countof(dbg_buf), fmt, arglist);
  va_end(arglist);
  return dbg_buf;
}

CC_INLINE const char* debugstr_w(const WCHAR* wstr) {
  //int len = sys_wc2mb(CE_GB2312, wstr, wstr_len(wstr), dbg_buf, countof(dbg_buf));
  UNUSED(wstr);
  return dbg_buf;
}

CC_INLINE const char *debugstr_rectf(const FRECT* rc)
{
  if (!rc) return "(null)";
  return dbg_sprintf("(%0.2f,%0.2f,%0.2f,%0.2f)", rc->l, rc->t, RCW(rc), RCH(rc));
}
CC_INLINE const char *debugstr_pointf(const FPOINT* pt)
{
  if (!pt) return "(null)";
  return dbg_sprintf("(%0.2f,%0.2f)", pt->x, pt->y);
}

//static const char* logname(const char*) {}
CC_INLINE char* _logfilename()
{
  static char __logfilename[256] = ".\\log.txt";
  return __logfilename;
}
#define LOGFILENAME _logfilename()
CC_INLINE int fclear(const char* fname)
{
  FILE* pf = fopen(fname, "wb");
  if (pf) {
    fclose(pf);
    return 1;
  }
  return 0;
}
#define logclear()   fclear(LOGFILENAME)
#define logset(_name)   strcpy(LOGFILENAME, _name)
CC_INLINE int* get__logecho()
{
  static int __logecho = 1;
  return &__logecho;
}
CC_INLINE int* get__logtime()
{
  static int __logtime = 1;
  return &__logtime;
}
#define __logecho  (*(get__logecho()))
#define __logtime  (*(get__logtime()))
#define logecho(_ON_OFF)   (__logecho=_ON_OFF)
#define logtime(_ON_OFF)   (__logtime=_ON_OFF)
// pf - stdout?
#ifndef PTTIME
#include <time.h>
//#define STRDATE()  _strdate(_time_buffer)
//#define STRTIME()  _strtime(_time_buffer)
//#define PTTIME STRTIME()
#endif

CC_INLINE int logprintf(const char* fmt, ...) {
  char buf[4096];
  LOGPRINTFTOBUF(buf, fmt, 1);
  if (__logecho) {
    puts(buf);
    puts("\n");
  }
  return 0;
}
CC_INLINE int logprintf1(const char* fmt, ...)
{
  FILE* pf;
  //static char _time_buffer[ 16 ];
  static char buf[4096];
  pf = fopen(LOGFILENAME, "a");
  if (pf) {
    int len;
    va_list arglist;
    va_start(arglist, fmt);
    if (__logtime) {
      fprintf( pf, "%8s ", _strtime2() );
    }
    len = cstr_vnprintf(buf, countof(buf), fmt, arglist);
    fwrite(buf, len, 1, pf);
    fwrite("\r\n", 2, 1, pf);
    fflush(pf);
    if (__logecho) {
      puts(buf);
    }
    va_end(arglist);
    fclose(pf);
  }
  return 0;
}
CC_INLINE int llogprintf(ELOG_LEVEL level, const char* fmt, ...)
{
  static BOOL showtime = false;
  static BOOL showecho = true;
  static loguser_t user = {0};
  static char filename[64] = {0};
  static const char* levelstr[20] = {"DEBUG", "INFO", "WARNING", "ERROR", "NONE", "", "", "", "", ""};
  char buf[256];
  UNUSED(user);
  switch (level) {
  case ELL_SET_FILE:
    filename[0] = 0;
    if (fmt) {
      strncpy(filename, fmt, 64);
    }
    break;
  case ELL_SET_ECHO:
    showecho = fmt ? true : false;
    break;
  case ELL_SET_TIME:
    showtime = fmt ? true : false;
    break;
  case ELL_SET_USER:
    user = *(loguser_t*)fmt;
    break;
  default:
    if (1) {
      char* p = buf, *e = buf+256;
      va_list arglist;
      va_start(arglist, fmt);
      if (showtime) {
        ctime(0);
        snprintf(p, e-p, "%8s ",  _strtime3());
        p += strlen(buf);
      }
      if (1) {
        snprintf(p, e-p, "%8s: ", levelstr[level]);
        p += strlen(buf);
      }
      cstr_vnprintf(p, e-p, fmt, arglist);
      va_end(arglist);
      cstr_trim(buf, -1, "\r\n");
      if (filename[0]) {
        FILE* pf = fopen(filename, "a");
        if (pf) {
          fprintf(pf, "%s\n", buf);
          fclose(pf);
        }
      }
      if (showecho) {
        printf(fmt, arglist);
      }
    }
    break;
  }
  return 0;
}
//#define LINE printf("%s:%d\n", __FILE__, __LINE__)
////////////////////////////////////////////////////////////////////////////////
#if 1
#define dbg_printf trace_dbg
#else
#define dbg_printf printf
#endif
#define set_dbg_out(pf)   dgb_out(pf, true)
static FILE* dgb_out(FILE* pf, bool set) {
  static FILE* fdgb = NULL;
  if (set) {
    fdgb = pf;
  }
  return fdgb;
}
static int trace_dbg(const char* fmt, ...)
{
  FILE* out = dgb_out(NULL, false);
  char buf[4096];
  int buflen = countof(buf);
  va_list arglist;
  va_start(arglist, fmt);
  //buf = cstr_vsprintf_dup(fmt, arglist);
  buflen = cstr_format_lenv(fmt, arglist);
  //buflen = cstr_format_lenv(fmt, arglist);
  //buf = (char*)pmalloc(buflen);
  _vsnprintf(buf, buflen, fmt, arglist);
  va_end(arglist);
    sys_trace_dbg(buf);
  if (out) {
    fputs(buf, out);
  } else {
    puts(buf);
  }
  return 0;
}
#if 0
void _GetTickCount(int64* ticks)
{
 __asm push edx;
 __asm push ecx;
 __asm mov ecx, ticks;
 __asm _emit 0x0f;
 __asm _emit 0x31;
 __asm mov [ecx], eax;
 __asm mov [ecx+4], edx;
 __asm pop ecx;
 __asm pop edx;
}
int64 sys_utime_frequency()
{
  int64 frequency=0;
  //_GetTickCount();
  return frequency;
}
int64 sys_utime_counter()
{
  int64 counter;
  _GetTickCount(&counter);
  return counter;
}
#endif
////////////////////////////////////////////////////////////////////////////////
#define tic  {static tictoc_t _tictoc[1] = {__FILE__, __LINE__, 0, 0, 0}; uint64 _lastRdtsc = sys_utime_counter()
#define tocf(out) toc_f(out, __FILE__, __LINE__, _lastRdtsc, _tictoc);}
#define toc_      tocf(true)
#define toc       toc_f(false, __FILE__, __LINE__, _lastRdtsc, _tictoc);}

typedef struct tictoc_t_ tictoc_t;

struct tictoc_t_ {
  const char* file;
  int line_beg;
  int line_end;
  int count;
  double _timer;
  tictoc_t* next;
};
#if 0
CC_INLINE tictoc_t* tictoc_stack(tictoc_t* add, bool pop) {
	static tictoc_t* stt[100] = { 0 };
	static int len = 0;
	if (pop) {
		--len;
	}
	else if (add) {
		stt[len++] = add;
	}
	return len>0 stt[len - 1] : NULL;
}
#endif
CC_INLINE tictoc_t** tictoc_opt(tictoc_t* add)  {
  static tictoc_t* gtt = NULL;
  if (add && 0==add->count) {
    //add->next = gtt;
    //gtt = add;
	LIST_PUSHBACK_T(gtt, add, tictoc_t);
  }
  return &gtt;
}
CC_INLINE double toc_f(bool out, const char* file, int line, uint64 _lastRdtsc, tictoc_t* tt) {
  uint64 t = sys_utime_counter();
  double _timer0 = (t - _lastRdtsc)*1./sys_utime_frequency();
  tictoc_opt(tt);
  tt->line_end = line;
  tt->_timer += _timer0;
  tt->count++;
  if (out) {
    dbg_printf("%s(%d):(%d): time = %.3fms\n", (file), tt->line_beg, tt->line_end, _timer0*1000.);
  }
  return _timer0;
}
#define tictoc tictoc_f(true)
#define tictoc0 tictoc_f(false)
CC_INLINE void tictoc_f(bool out) {
  tictoc_t** ptt = tictoc_opt(NULL);
  if (*ptt) {
    //out = out ? out : stdout;
    if (out) { dbg_printf("<<<\n"); }
    for (; *ptt; ) {
      tictoc_t* tt = *ptt;
      *ptt = NULL;
      ptt = &tt->next;
      if (tt->count > 0) {
        if (out) {
          const char* filename = GetFileNameExt(tt->file);
          //fprintf(out, "%s(%d):(%d): time = %.3fms count=%d\n", filename, tt->line_beg, tt->line_end, tt->_timer*1000., tt->count);
          dbg_printf("%s(%d):(%d): time=%.3fms count=%d avg=%.3fms\n", tt->file, tt->line_beg, tt->line_end, tt->_timer*1000., tt->count, tt->_timer*1000. / tt->count);
        }
        tt->_timer = 0;
        tt->count = 0;
      }
    }
    if (out) { dbg_printf(">>>\n"); }
  }
}
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//typedef tm_t tm_t;
//typedef sys_time_t sys_time_t;
#if 0
#define DATESET(date, year, month, day) (date = ((year)<<16) | (((month)&0xff)<<8) | ((day)&0xff))
#define DATEGET(date, year, month, day) (year = (date)>>16, month = ((date)>>8)&0xff, day = (date)&0xff)
#else
#define DATESET(date, year, month, day) (date = ((year)*10000) + ((month)*100) + (day))
#define DATEGET(date, year, month, day) (year = (date)/10000, month = ((date)/100)%100, day = (date)%100)
#endif
CC_INLINE int year_days(int year)
{
  if (((year % 4 == 0) && (year % 100)) || (year % 400 == 0)) {
    return 366;
  }
  return 365;
}
CC_INLINE BOOL is_leap_year(int year)
{
  return ((year % 4 == 0) && (year % 100)) || (year % 400 == 0);
}
CC_INLINE BOOL number_of_leap_year(int year)
{
  return year / 4 - year / 100 + year / 400;
}
CC_INLINE int month_days(int year, int month)
{
  static const int num[] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month != 2) {
    return num[month - 1];
  }
  if (((year % 4 == 0) && (year % 100)) || (year % 400 == 0)) {
    return 29;
  }
  return 28;
}
CC_INLINE int day_in_year(int y, int m, int d)
{
  int i, days = 0;
  m = BOUND(m, 1, 12);
  for (i = 1; i < m; ++i) {
    days += month_days(y, i);
  }
  i = month_days(y, m);
  d = BOUND(d, 1, i);
  return days + d - 1;
}
CC_INLINE int date_to_months(int year, int month)
{
  return year * 12 + month - 1;
}
CC_INLINE int months_to_date(int months, int day)
{
  int year = months / 12;
  int month = months % 12 + 1;
  int date;
  DATESET(date, year, month, day);
  return date;
}
CC_INLINE int date_to_days(int year, int month, int day)
{
  int l = number_of_leap_year(year - 1);
  int y = (year - 1) * 365;
  int m = day_in_year(year, month, day);
  return l + y + m;
}
CC_INLINE int days_to_wday(int days)
{
  return (days + 1) % 7;
}
CC_INLINE int days_to_date(int days)
{
  int date = 0;
  //int ddd = days;
  int year = 0, month = 0, day = 0, d, d_preyear;
  year = days / 366 + 1;
  while (days >= (d = year * 365 + number_of_leap_year(year))) {
    ++year;
    d_preyear = d;
  }
  days -= d_preyear;
  month = 1;
  for (; month <= 12 && days >= (d = month_days(year, month)); ++month) {
    days -= d;
  }
  ASSERT(month < 13);
  day = days + 1;
  DATESET(date, year, month, day);
  return date;
}
CC_INLINE tm_t mylocaltime()
{
  time_t t = time(NULL);
  tm_t* tm_addr = localtime(&t);
  assert(tm_addr);
  return *tm_addr;
}
CC_INLINE bool date_less(const sys_time_t* dt, const sys_time_t* r)
{
  if (dt->year < r->year) {
    return true;
  }
  else if (dt->year == r->year) {
    if (dt->month < r->month) {
      return true;
    }
    else if (dt->month == r->month) {
      return (dt->day < r->day);
    }
  }
  return false;
}
CC_INLINE bool date_grater(const sys_time_t* dt, const sys_time_t* r)
{
  if (dt->year > r->year) {
    return true;
  }
  else if (dt->year == r->year) {
    if (dt->month > r->month) {
      return true;
    }
    else if (dt->month == r->month) {
      return (dt->day > r->day);
    }
  }
  return false;
}
CC_INLINE uint date_add(uint date, int x)
{
  int year, month, day, d;
  DATEGET(date, year, month, day);
  if (x < 0) {
    x = -x;
    while (x) {
      if (day <= x) {
        if (month == 1) {
          month = 12;
          -- year;
        }
        else {
          -- month;
        }
        d = month_days(year, month);
        day += d - x;
      }
      else {
        day -= x;
        break;
      }
    }
  }
  else {
    while (x) {
      int off = month_days(year, month) - day;
      if (off < x) {
        day = 1;
        if (month == 12) {
          month = 1;
          ++ year;
        }
        else {
          ++ month;
        }
        x -= (off + 1);
      }
      else if (off >= x) {
        day += x;
        break;
      }
    }
  }
  DATESET(date, year, month, day);
  return date;
}
//w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1;
//the Jan and Feb of Every year are treated as the 13th/14th month of last year.
CC_INLINE int day_of_week(int year, int month, int day)
{
  int century, w;
  if (month < 3) {
    month += 12;
    --year;
  }
  century = year / 100;
  year = year % 100;
  w = year + (year / 4) + (century / 4) - (2 * century) + (26 * (month + 1) / 10) + day - 1;
  if (w >= 0) {
    return (w % 7);
  }
  return ((1 - (w / 7)) * 7 + w);
}
// Jul 03 2015 ¸ñÊ½×ª time_t
/**
 * @brief getDateFromMacro
 * @param time __DATE__
 * @return
 */
static time_t getDateFromMacro(char const *time) {
    char s_month[5];
    int month, day, year;
    struct tm t = {0};
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time, "%s %d %d", s_month, &day, &year);

    month = (strstr(month_names, s_month)-month_names)/3;

    t.tm_mon = month;
    t.tm_mday = day;
    t.tm_year = year - 1900;
    t.tm_isdst = -1;
    return mktime(&t);
}

#endif
////////////////////////////////////////////////////////////////////////////////
#endif // _CDBG_H_
