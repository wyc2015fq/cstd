/////////////////////////////////////////////////////////////////////////////
// COleDateTime class HELPER definitions
// Verifies will fail if the needed buffer size is too large
#define MAX_TIME_BUFFER_SIZE    128         // matches that in timecore.cpp
#define MIN_DATE                (-657434L)  // about year 100
#define MAX_DATE                2958465L    // about year 9999
// Half a second, expressed in days
#define HALF_SECOND  (1.0/172800.0)
// One-based array of days in year at month start
static int _afxMonthDays[13] =
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
/////////////////////////////////////////////////////////////////////////////
// COleDateTime class HELPERS - implementation
static BOOL _AfxOleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
    WORD wHour, WORD wMinute, WORD wSecond, DATE* dtDest)
{
  BOOL bLeapYear;
  int nDaysInMonth;
  long nDate;
  double dblTime;
  // Validate year and month (ignore day of week and milliseconds)
  if (wYear > 9999 || wMonth < 1 || wMonth > 12) {
    return FALSE;
  }
  //  Check for leap year and set the number of days in the month
  bLeapYear = ((wYear & 3) == 0) && ((wYear % 100) != 0 || (wYear % 400) == 0);
  nDaysInMonth = _afxMonthDays[wMonth] - _afxMonthDays[wMonth - 1] +
      ((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);
  // Finish validating the date
  if (wDay < 1 || wDay > nDaysInMonth ||
      wHour > 23 || wMinute > 59 ||
      wSecond > 59) {
    return FALSE;
  }
  // Cache the date in days and time in fractional days
  //It is a valid date; make Jan 1, 1AD be 1
  nDate = wYear * 365L + wYear / 4 - wYear / 100 + wYear / 400 +
      _afxMonthDays[wMonth - 1] + wDay;
  //  If leap year and it's before March, subtract 1:
  if (wMonth <= 2 && bLeapYear) {
    --nDate;
  }
  //  Offset so that 12/30/1899 is 0
  nDate -= 693959L;
  dblTime = (((long)wHour * 3600L) +  // hrs in seconds
      ((long)wMinute * 60L) +  // mins in seconds
      ((long)wSecond)) / 86400.;
  *dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);
  return TRUE;
}
static BOOL _AfxTmFromOleDate(DATE dtSrc, struct tm* tmDest)
{
  // The legal range does not actually span year 0 to 9999.
  long nDays;             // Number of days since Dec. 30, 1899
  long nDaysAbsolute;     // Number of days since 1/1/0
  long nSecsInDay;        // Time in seconds since midnight
  long nMinutesInDay;     // Minutes in day
  long n400Years;         // Number of 400 year increments since 1/1/0
  long n400Century;       // Century within 400 year block (0,1,2 or 3)
  long n4Years;           // Number of 4 year increments since 1/1/0
  long n4Day;             // Day within 4 year block
  //  (0 is 1/1/yr1, 1460 is 12/31/yr4)
  long n4Yr;              // Year within 4 year block (0,1,2 or 3)
  BOOL bLeap4 = TRUE;     // TRUE if 4 year block includes leap year
  double dblDate = dtSrc; // tempory serial date
  if (dtSrc > MAX_DATE || dtSrc < MIN_DATE) { // about year 100 to about 9999
    return FALSE;
  }
  // If a valid date, then this conversion should not overflow
  nDays = (long)dblDate;
  // Round to the second
  dblDate += ((dtSrc > 0.0) ? HALF_SECOND : -HALF_SECOND);
  nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899
  dblDate = fabs(dblDate);
  nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);
  // Calculate the day of week (sun=1, mon=2...)
  //   -1 because 1/1/0 is Sat.  +1 because we want 1-based
  tmDest->tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;
  // Leap years every 4 yrs except centuries not multiples of 400.
  n400Years = (long)(nDaysAbsolute / 146097L);
  // Set nDaysAbsolute to day within 400-year block
  nDaysAbsolute %= 146097L;
  // -1 because first century has extra day
  n400Century = (long)((nDaysAbsolute - 1) / 36524L);
  // Non-leap century
  if (n400Century != 0) {
    // Set nDaysAbsolute to day within century
    nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;
    // +1 because 1st 4 year increment has 1460 days
    n4Years = (long)((nDaysAbsolute + 1) / 1461L);
    if (n4Years != 0) {
      n4Day = (long)((nDaysAbsolute + 1) % 1461L);
    }
    else {
      bLeap4 = FALSE;
      n4Day = (long)nDaysAbsolute;
    }
  }
  else {
    // Leap century - not special case!
    n4Years = (long)(nDaysAbsolute / 1461L);
    n4Day = (long)(nDaysAbsolute % 1461L);
  }
  if (bLeap4) {
    // -1 because first year has 366 days
    n4Yr = (n4Day - 1) / 365;
    if (n4Yr != 0) {
      n4Day = (n4Day - 1) % 365;
    }
  }
  else {
    n4Yr = n4Day / 365;
    n4Day %= 365;
  }
  // n4Day is now 0-based day of year. Save 1-based day of year, year number
  tmDest->tm_yday = (int)n4Day + 1;
  tmDest->tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;
  // Handle leap year: before, on, and after Feb. 29.
  if (n4Yr == 0 && bLeap4) {
    // Leap Year
    if (n4Day == 59) {
      /* Feb. 29 */
      tmDest->tm_mon = 2;
      tmDest->tm_mday = 29;
      goto DoTime;
    }
    // Pretend it's not a leap year for month/day comp.
    if (n4Day >= 60) {
      --n4Day;
    }
  }
  // Make n4DaY a 1-based day of non-leap year and compute
  //  month/day for everything but Feb. 29.
  ++n4Day;
  // Month number always >= n/32, so save some loop time */
  for (tmDest->tm_mon = (n4Day >> 5) + 1;
      n4Day > _afxMonthDays[tmDest->tm_mon]; tmDest->tm_mon++);
  tmDest->tm_mday = (int)(n4Day - _afxMonthDays[tmDest->tm_mon - 1]);
DoTime:
  if (nSecsInDay == 0) {
    tmDest->tm_hour = tmDest->tm_min = tmDest->tm_sec = 0;
  }
  else {
    tmDest->tm_sec = (int)nSecsInDay % 60L;
    nMinutesInDay = nSecsInDay / 60L;
    tmDest->tm_min = (int)nMinutesInDay % 60;
    tmDest->tm_hour = (int)nMinutesInDay / 60;
  }
  return TRUE;
}
static void _AfxTmConvertToStandardFormat(struct tm* tmSrc)
{
  // Convert afx internal tm to format expected by runtimes (_tcsftime, etc)
  tmSrc->tm_year -= 1900;  // year is based on 1900
  tmSrc->tm_mon -= 1;      // month of year is 0-based
  tmSrc->tm_wday -= 1;     // day of week is 0-based
  tmSrc->tm_yday -= 1;     // day of year is 0-based
}
static double _AfxDoubleFromDate(DATE dt)
{
  double temp;
  // No problem if positive
  if (dt >= 0) {
    return dt;
  }
  // If negative, must convert since negative dates not continuous
  // (examples: -1.25 to -.75, -1.50 to -.50, -1.75 to -.25)
  temp = ceil(dt);
  return temp - (dt - temp);
}
static DATE _AfxDateFromDouble(double dbl)
{
  double temp;
  // No problem if positive
  if (dbl >= 0) {
    return dbl;
  }
  // If negative, must convert since negative dates not continuous
  // (examples: -.75 to -1.25, -.50 to -1.50, -.25 to -1.75)
  temp = floor(dbl); // dbl is now whole part
  return temp + (temp - dbl);
}
/////////////////////////////////////////////////////////////////////////////
// COleDateTime class

