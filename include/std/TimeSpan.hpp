
#ifndef __TIMESPAN_HPP__
#define __TIMESPAN_HPP__

#include "DumpContext.hpp"
#include "String.hpp"

class CTimeSpan
{
public:
  time_t m_timeSpan;


  _AFX_INLINE CTimeSpan()
  { }

  _AFX_INLINE CTimeSpan(time_t time)
  { m_timeSpan = time; }
  _AFX_INLINE CTimeSpan(LONG lDays, int nHours, int nMins, int nSecs)
  { m_timeSpan = nSecs + 60* (nMins + 60* (nHours + 24* lDays)); }
  _AFX_INLINE CTimeSpan(const CTimeSpan& timeSpanSrc)
  { m_timeSpan = timeSpanSrc.m_timeSpan; }
  _AFX_INLINE const CTimeSpan& operator=(const CTimeSpan& timeSpanSrc)
  { m_timeSpan = timeSpanSrc.m_timeSpan; return *this; }
  _AFX_INLINE LONG GetDays() const
  { return m_timeSpan / (24*3600L); }
  _AFX_INLINE LONG GetTotalHours() const
  { return m_timeSpan/3600; }
  _AFX_INLINE int GetHours() const
  { return (int)(GetTotalHours() - GetDays()*24); }
  _AFX_INLINE LONG GetTotalMinutes() const
  { return m_timeSpan/60; }
  _AFX_INLINE int GetMinutes() const
  { return (int)(GetTotalMinutes() - GetTotalHours()*60); }
  _AFX_INLINE LONG GetTotalSeconds() const
  { return m_timeSpan; }
  _AFX_INLINE int GetSeconds() const
  { return (int)(GetTotalSeconds() - GetTotalMinutes()*60); }
  _AFX_INLINE CTimeSpan operator-(CTimeSpan timeSpan) const
  { return CTimeSpan(m_timeSpan - timeSpan.m_timeSpan); }
  _AFX_INLINE CTimeSpan operator+(CTimeSpan timeSpan) const
  { return CTimeSpan(m_timeSpan + timeSpan.m_timeSpan); }
  _AFX_INLINE const CTimeSpan& operator+=(CTimeSpan timeSpan)
  { m_timeSpan += timeSpan.m_timeSpan; return *this; }
  _AFX_INLINE const CTimeSpan& operator-=(CTimeSpan timeSpan)
  { m_timeSpan -= timeSpan.m_timeSpan; return *this; }
  _AFX_INLINE BOOL operator==(CTimeSpan timeSpan) const
  { return m_timeSpan == timeSpan.m_timeSpan; }
  _AFX_INLINE BOOL operator!=(CTimeSpan timeSpan) const
  { return m_timeSpan != timeSpan.m_timeSpan; }
  _AFX_INLINE BOOL operator<(CTimeSpan timeSpan) const
  { return m_timeSpan < timeSpan.m_timeSpan; }
  _AFX_INLINE BOOL operator>(CTimeSpan timeSpan) const
  { return m_timeSpan > timeSpan.m_timeSpan; }
  _AFX_INLINE BOOL operator<=(CTimeSpan timeSpan) const
  { return m_timeSpan <= timeSpan.m_timeSpan; }
  _AFX_INLINE BOOL operator>=(CTimeSpan timeSpan) const
  { return m_timeSpan >= timeSpan.m_timeSpan; }

#define maxTimeBufferSize       128
  // Verifies will fail if the needed buffer size is too large

  CString Format(LPCTSTR pFormat) const
    // formatting timespans is a little trickier than formatting CTimes
    //  * we are only interested in relative time formats, ie. it is illegal
    //      to format anything dealing with absolute time (i.e. years, months,
    //         day of week, day of year, timezones, ...)
    //  * the only valid formats:
    //      %D - # of days -- NEW !!!
    //      %H - hour in 24 hour format
    //      %M - minute (0-59)
    //      %S - seconds (0-59)
    //      %% - percent sign
  {
    TCHAR szBuffer[maxTimeBufferSize];
    TCHAR ch;
    LPTSTR pch = szBuffer;

    while ((ch = *pFormat++) != '\0')
    {
      ASSERT(pch < &szBuffer[maxTimeBufferSize]);
      if (ch == '%')
      {
        switch (ch = *pFormat++)
        {
        default:
          ASSERT(FALSE);      // probably a bad format character
        case '%':
          *pch++ = ch;
          break;
        case 'D':
          pch += sprintf(pch, ("%ld"), GetDays());
          break;
        case 'H':
          pch += sprintf(pch, ("%02d"), GetHours());
          break;
        case 'M':
          pch += sprintf(pch, ("%02d"), GetMinutes());
          break;
        case 'S':
          pch += sprintf(pch, ("%02d"), GetSeconds());
          break;
        }
      }
      else
      {
        *pch++ = ch;
        if (_istlead(ch))
        {
          ASSERT(pch < &szBuffer[maxTimeBufferSize]);
          *pch++ = *pFormat++;
        }
      }
    }

    *pch = '\0';
    return szBuffer;
  }

};


CDumpContext& operator <<(CDumpContext& dc, CTimeSpan timeSpan)
{
  return dc << "CTimeSpan(" << timeSpan.GetDays() << " days, " <<
    timeSpan.GetHours() << " hours, " <<
    timeSpan.GetMinutes() << " minutes and " <<
    timeSpan.GetSeconds() << " seconds)";
}



#endif // __TIMESPAN_HPP__
