
#ifndef __TIME_HPP__
#define __TIME_HPP__


#include "TimeSpan.hpp"

struct CTime
{
  // return the current system time
  static CTime GetCurrentTime()
  {
    return CTime(::time(NULL));
  }
  time_t m_time;
  DefStruct* def(DefStruct* dc)
  {
	  dc->def("m_time", &m_time);
	  return dc;
  }


  _AFX_INLINE CTime()
  { }
  _AFX_INLINE CTime(time_t time)
  { m_time = time; }
  _AFX_INLINE CTime(const CTime& timeSrc)
  { m_time = timeSrc.m_time; }
  _AFX_INLINE const CTime& operator=(const CTime& timeSrc)
  { m_time = timeSrc.m_time; return *this; }
  _AFX_INLINE const CTime& operator=(time_t t)
  { m_time = t; return *this; }
  _AFX_INLINE time_t GetTime() const
  { return m_time; }
  _AFX_INLINE int GetYear() const
  { return (GetLocalTm(NULL)->tm_year) + 1900; }
  _AFX_INLINE int GetMonth() const
  { return GetLocalTm(NULL)->tm_mon + 1; }
  _AFX_INLINE int GetDay() const
  { return GetLocalTm(NULL)->tm_mday; }
  _AFX_INLINE int GetHour() const
  { return GetLocalTm(NULL)->tm_hour; }
  _AFX_INLINE int GetMinute() const
  { return GetLocalTm(NULL)->tm_min; }
  _AFX_INLINE int GetSecond() const
  { return GetLocalTm(NULL)->tm_sec; }
  _AFX_INLINE int GetDayOfWeek() const
  { return GetLocalTm(NULL)->tm_wday + 1; }
  _AFX_INLINE CTimeSpan operator-(CTime time) const
  { return CTimeSpan(m_time - time.m_time); }
  _AFX_INLINE CTime operator-(CTimeSpan timeSpan) const
  { return CTime(m_time - timeSpan.m_timeSpan); }
  _AFX_INLINE CTime operator+(CTimeSpan timeSpan) const
  { return CTime(m_time + timeSpan.m_timeSpan); }
  _AFX_INLINE const CTime& operator+=(CTimeSpan timeSpan)
  { m_time += timeSpan.m_timeSpan; return *this; }
  _AFX_INLINE const CTime& operator-=(CTimeSpan timeSpan)
  { m_time -= timeSpan.m_timeSpan; return *this; }
  _AFX_INLINE BOOL operator==(CTime time) const
  { return m_time == time.m_time; }
  _AFX_INLINE BOOL operator!=(CTime time) const
  { return m_time != time.m_time; }
  _AFX_INLINE BOOL operator<(CTime time) const
  { return m_time < time.m_time; }
  _AFX_INLINE BOOL operator>(CTime time) const
  { return m_time > time.m_time; }
  _AFX_INLINE BOOL operator<=(CTime time) const
  { return m_time <= time.m_time; }
  _AFX_INLINE BOOL operator>=(CTime time) const
  { return m_time >= time.m_time; }


  CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1)
  {
    struct tm atm;
    atm.tm_sec = nSec;
    atm.tm_min = nMin;
    atm.tm_hour = nHour;
    ASSERT(nDay >= 1 && nDay <= 31);
    atm.tm_mday = nDay;
    ASSERT(nMonth >= 1 && nMonth <= 12);
    atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
    ASSERT(nYear >= 1900);
    atm.tm_year = nYear - 1900;     // tm_year is 1900 based
    atm.tm_isdst = nDST;
    m_time = mktime(&atm);
    ASSERT(m_time != -1);       // indicates an illegal input time
  }

  CTime(WORD wDosDate, WORD wDosTime, int nDST = -1)
  {
    struct tm atm;
    atm.tm_sec = (wDosTime & ~0xFFE0) << 1;
    atm.tm_min = (wDosTime & ~0xF800) >> 5;
    atm.tm_hour = wDosTime >> 11;

    atm.tm_mday = wDosDate & ~0xFFE0;
    atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
    atm.tm_year = (wDosDate >> 9) + 80;
    atm.tm_isdst = nDST;
    m_time = mktime(&atm);
    ASSERT(m_time != -1);       // indicates an illegal input time
  }
  #ifdef _WIN32
  CTime(const SYSTEMTIME& sysTime, int nDST = -1)
  {
    if (sysTime.wYear < 1900)
    {
      time_t time0 = 0L;
      CTime timeT(time0);
      *this = timeT;
    }
    else
    {
      CTime timeT(
        (int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay,
        (int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond,
        nDST);
      *this = timeT;
    }
  }

  CTime(const FILETIME& fileTime, int nDST = -1)
  {
    // first convert file time (UTC time) to local time
    FILETIME localTime;
    if (!FileTimeToLocalFileTime(&fileTime, &localTime))
    {
      m_time = 0;
      return;
    }

    // then convert that time to system time
    SYSTEMTIME sysTime;
    if (!FileTimeToSystemTime(&localTime, &sysTime))
    {
      m_time = 0;
      return;
    }

    // then convert the system time to a time_t (C-runtime local time)
    CTime timeT(sysTime, nDST);
    *this = timeT;
  }
  #endif

  struct tm* GetGmtTm(struct tm* ptm) const
  {
    if (ptm != NULL)
    {
      *ptm = *gmtime(&m_time);
      return ptm;
    }
    else
      return gmtime(&m_time);
  }

  struct tm* GetLocalTm(struct tm* ptm) const
  {
    if (ptm != NULL)
    {
      struct tm* ptmTemp = localtime(&m_time);
      if (ptmTemp == NULL)
        return NULL;    // indicates the m_time was not initialized!

      *ptm = *ptmTemp;
      return ptm;
    }
    else
      return localtime(&m_time);
  }
#ifdef _WIN32
  BOOL GetAsSystemTime(SYSTEMTIME& timeDest) const
  {
    struct tm* ptm = GetLocalTm(NULL);
    if (ptm == NULL)
      return FALSE;

    timeDest.wYear = (WORD) (1900 + ptm->tm_year);
    timeDest.wMonth = (WORD) (1 + ptm->tm_mon);
    timeDest.wDayOfWeek = (WORD) ptm->tm_wday;
    timeDest.wDay = (WORD) ptm->tm_mday;
    timeDest.wHour = (WORD) ptm->tm_hour;
    timeDest.wMinute = (WORD) ptm->tm_min;
    timeDest.wSecond = (WORD) ptm->tm_sec;
    timeDest.wMilliseconds = 0;

    return TRUE;
  }
  #endif

  CString Format(LPCTSTR pFormat) const
  {
    char szBuffer[maxTimeBufferSize];

    struct tm* ptmTemp = localtime(&m_time);
    if (ptmTemp == NULL ||
      !strftime(szBuffer, _countof(szBuffer), pFormat, ptmTemp))
      szBuffer[0] = '\0';
    return szBuffer;
  }

  CString FormatGmt(LPCTSTR pFormat) const
  {
    char szBuffer[maxTimeBufferSize];

    struct tm* ptmTemp = gmtime(&m_time);
    if (ptmTemp == NULL ||
      !strftime(szBuffer, _countof(szBuffer), pFormat, ptmTemp))
      szBuffer[0] = '\0';
    return szBuffer;
  }

  ////////////////////////////////////////////////////////////////////////////
  // out-of-line inlines for binary compatibility
};


LONG AfxTimeToFileTime(const CTime& time, LPFILETIME pFileTime)
{
  SYSTEMTIME sysTime;
  sysTime.wYear = (WORD)time.GetYear();
  sysTime.wMonth = (WORD)time.GetMonth();
  sysTime.wDay = (WORD)time.GetDay();
  sysTime.wHour = (WORD)time.GetHour();
  sysTime.wMinute = (WORD)time.GetMinute();
  sysTime.wSecond = (WORD)time.GetSecond();
  sysTime.wMilliseconds = 0;
  
  // convert system time to local file time
  FILETIME localTime;
  if (!SystemTimeToFileTime((LPSYSTEMTIME)&sysTime, &localTime))
    return ((LONG)::GetLastError());
  
  // convert local file time to UTC file time
  if (!LocalFileTimeToFileTime(&localTime, pFileTime))
    return ((LONG)::GetLastError());
  return 0;
}

StringBuilder& operator <<(StringBuilder& dc, const CTime& time)
{
	char* psz = ctime(&time.m_time);
	if ((psz == NULL) || (time.m_time == 0))
		return dc << "CTime(invalid #" << time.m_time << ")";

	// format it
	psz[24] = '\0';         // nuke newline
	return dc << "CTime(\"" << psz << "\")";
}



#endif // __TIME_HPP__
