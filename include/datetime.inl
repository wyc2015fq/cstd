#ifndef _DATETIME_INL_
#define _DATETIME_INL_

#define Int64MinValue _I64_MIN
#define Int64MaxValue _I64_MAX


// TimeSpan represents a duration of time.  A TimeSpan can be negative
// or positive.
//
// TimeSpan is internally represented as a number of milliseconds.  While
// this maps well into units of time such as hours and days, any
// periods longer than that aren't representable in a nice fashion.
// For instance, a month can be between 28 and 31 days, while a year
// can contain 365 or 364 days.  A decade can have between 1 and 3 leapyears,
// depending on when you map the TimeSpan into the calendar.  This is why
// we do not provide Years() or Months().
//
// Note: System.TimeSpan needs to interop with the WinRT structure
// type Windows::Foundation:TimeSpan. These types are currently binary-compatible in
// memory so no custom marshalling is required. If at any point the implementation
// details of this type should change, or new fields added, we need to remember to add
// an appropriate custom ILMarshaler to keep WInRT interop scenarios enabled.
//

static const int64  TicksPerMillisecond =  10000;
static const double MillisecondsPerTick = 1.0 / TicksPerMillisecond;
static const int64 TicksPerSecond = TicksPerMillisecond * 1000;   // 10,000,000
static const double SecondsPerTick =  1.0 / TicksPerSecond;         // 0.0001
static const int64 TicksPerMinute = TicksPerSecond * 60;         // 600,000,000
static const double MinutesPerTick = 1.0 / TicksPerMinute; // 1.6666666666667e-9
static const int64 TicksPerHour = TicksPerMinute * 60;        // 36,000,000,000
static const double HoursPerTick = 1.0 / TicksPerHour; // 2.77777777777777778e-11
static const int64 TicksPerDay = TicksPerHour * 24;          // 864,000,000,000
static const double DaysPerTick = 1.0 / TicksPerDay; // 1.1574074074074074074e-12
static const int MillisPerSecond = 1000;
static const int MillisPerMinute = MillisPerSecond * 60; //     60,000
static const int MillisPerHour = MillisPerMinute * 60;   //  3,600,000
static const int MillisPerDay = MillisPerHour * 24;      // 86,400,000
static const int64 MaxSeconds = Int64MaxValue / TicksPerSecond;
static const int64 MinSeconds = Int64MinValue / TicksPerSecond;
static const int64 MaxMilliSeconds = Int64MaxValue / TicksPerMillisecond;
static const int64 MinMilliSeconds = Int64MinValue / TicksPerMillisecond;
static const int64 TicksPerTenthSecond = TicksPerMillisecond * 100;

static const int64 Zero = 0;

static const int64 TimeSpanMaxValue = Int64MaxValue;
static const int64 TimeSpanMinValue = Int64MinValue;

#if 0
//  so that DateTime doesn't have to call an extra get
// method for some arithmetic operations.
// int64 _ticks;

// TimeSpan() {
//    _ticks = 0;
//}

TimeSpan(int64 ticks) {
  this._ticks = ticks;
}

TimeSpan(int hours, int minutes, int seconds) {
  _ticks = TimeToTicks(hours, minutes, seconds);
}

TimeSpan(int days, int hours, int minutes, int seconds)
: this(days,hours,minutes,seconds,0)
{
}

TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
{
  Int64 totalMilliSeconds = ((Int64)days * 3600 * 24 + (Int64)hours * 3600 + (Int64)minutes * 60 + seconds) * 1000 + milliseconds;
  if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
    throw new ArgumentOutOfRangeException(null, Environment.GetResourceString("Overflow_TimeSpanTooLong"));
  _ticks =  (int64)totalMilliSeconds * TicksPerMillisecond;
}

int64 Ticks {
  get { return _ticks; }
}
#endif
int getDays(int64 _ticks) {
  return (int)(_ticks / TicksPerDay);
}

int getHours(int64 _ticks) {
  return (int)((_ticks / TicksPerHour) % 24);
}

int getMilliseconds(int64 _ticks) {
  return (int)((_ticks / TicksPerMillisecond) % 1000);
}

int getMinutes(int64 _ticks) {
  return (int)((_ticks / TicksPerMinute) % 60);
}

int getSeconds(int64 _ticks) {
  return (int)((_ticks / TicksPerSecond) % 60);
}

double getTotalDays(int64 _ticks) {
  return ((double)_ticks) * DaysPerTick;
}

double getTotalHours(int64 _ticks) {
  return (double)_ticks * HoursPerTick;
}

double getTotalMilliseconds(int64 _ticks) {
  
  double temp = (double)_ticks * MillisecondsPerTick;
  if (temp > MaxMilliSeconds)
    return (double)MaxMilliSeconds;
  
  if (temp < MinMilliSeconds)
    return (double)MinMilliSeconds;
  
  return temp;
  
}

double getTotalMinutes(int64 _ticks) {
  return (double)_ticks * MinutesPerTick;
}

double getTotalSeconds(int64 _ticks) {
  return (double)_ticks * SecondsPerTick;
}

#if 0
TimeSpan Add(TimeSpan ts) {
  int64 result = _ticks + ts._ticks;
  // Overflow if signs of operands was identical and result's
  // sign was opposite.
  // >> 63 gives the sign bit (either 64 1's or 64 0's).
  if ((_ticks >> 63 == ts._ticks >> 63) && (_ticks >> 63 != result >> 63))
    throw new OverflowException(Environment.GetResourceString("Overflow_TimeSpanTooLong"));
  return new TimeSpan(result);
}


// Compares two TimeSpan values, returning an integer that indicates their
// relationship.
//
static int Compare(TimeSpan t1, TimeSpan t2) {
  if (t1._ticks > t2._ticks) return 1;
  if (t1._ticks < t2._ticks) return -1;
  return 0;
}

// Returns a value less than zero if this  object
int CompareTo(Object value) {
  if (value == null) return 1;
  if (!(value is TimeSpan))
    throw new ArgumentException(Environment.GetResourceString("Arg_MustBeTimeSpan"));
  int64 t = ((TimeSpan)value)._ticks;
  if (_ticks > t) return 1;
  if (_ticks < t) return -1;
  return 0;
}

#if GENERICS_WORK
int CompareTo(TimeSpan value) {
  int64 t = value._ticks;
  if (_ticks > t) return 1;
  if (_ticks < t) return -1;
  return 0;
}
#endif

static TimeSpan FromDays(double value) {
  return Interval(value, MillisPerDay);
}

TimeSpan Duration() {
  if (Ticks==TimeSpan.MinValue.Ticks)
    throw new OverflowException(Environment.GetResourceString("Overflow_Duration"));
  Contract.EndContractBlock();
  return new TimeSpan(_ticks >= 0? _ticks: -_ticks);
}

override bool Equals(Object value) {
  if (value is TimeSpan) {
    return _ticks == ((TimeSpan)value)._ticks;
  }
  return false;
}

bool Equals(TimeSpan obj)
{
  return _ticks == obj._ticks;
}

static bool Equals(TimeSpan t1, TimeSpan t2) {
  return t1._ticks == t2._ticks;
}

override int GetHashCode() {
  return (int)_ticks ^ (int)(_ticks >> 32);
}

static TimeSpan FromHours(double value) {
  return Interval(value, MillisPerHour);
}

static TimeSpan Interval(double value, int scale) {
  if (Double.IsNaN(value))
    throw new ArgumentException(Environment.GetResourceString("Arg_CannotBeNaN"));
  Contract.EndContractBlock();
  double tmp = value * scale;
  double millis = tmp + (value >= 0? 0.5: -0.5);
  if ((millis > Int64MaxValue / TicksPerMillisecond) || (millis < Int64MinValue / TicksPerMillisecond))
    throw new OverflowException(Environment.GetResourceString("Overflow_TimeSpanTooLong"));
  return new TimeSpan((int64)millis * TicksPerMillisecond);
}

static TimeSpan FromMilliseconds(double value) {
  return Interval(value, 1);
}

static TimeSpan FromMinutes(double value) {
  return Interval(value, MillisPerMinute);
}

TimeSpan Negate() {
  if (Ticks==TimeSpan.MinValue.Ticks)
    throw new OverflowException(Environment.GetResourceString("Overflow_NegateTwosCompNum"));
  Contract.EndContractBlock();
  return new TimeSpan(-_ticks);
}

static TimeSpan FromSeconds(double value) {
  return Interval(value, MillisPerSecond);
}

TimeSpan Subtract(TimeSpan ts) {
  int64 result = _ticks - ts._ticks;
  // Overflow if signs of operands was different and result's
  // sign was opposite from the first argument's sign.
  // >> 63 gives the sign bit (either 64 1's or 64 0's).
  if ((_ticks >> 63 != ts._ticks >> 63) && (_ticks >> 63 != result >> 63))
    throw new OverflowException(Environment.GetResourceString("Overflow_TimeSpanTooLong"));
  return new TimeSpan(result);
}

static TimeSpan FromTicks(int64 value) {
  return new TimeSpan(value);
}
#endif
static int64 TimeToTicks(int hour, int minute, int second) {
  // totalSeconds is bounded by 2^31 * 2^12 + 2^31 * 2^8 + 2^31,
  // which is less than 2^44, meaning we won't overflow totalSeconds.
  if (!(hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >=0 && second < 60)) {
    ArgumentOutOfRangeException(GetResourceString("ArgumentOutOfRange_BadHourMinuteSecond"));
  }
  int64 totalSeconds = (int64)hour * 3600 + (int64)minute * 60 + (int64)second;
  if (totalSeconds > MaxSeconds || totalSeconds < MinSeconds)
    ArgumentOutOfRangeException(GetResourceString("Overflow_TimeSpanTooLong"));
  return totalSeconds * TicksPerSecond;
}

#if 0
// See System.Globalization.TimeSpanParse and System.Globalization.TimeSpanFormat 
#region ParseAndFormat
static TimeSpan Parse(String s) {
  /* Constructs a TimeSpan from a string.  Leading and trailing white space characters are allowed. */
  return TimeSpanParse.Parse(s, null);
}
static TimeSpan Parse(String input, IFormatProvider formatProvider) {
  return TimeSpanParse.Parse(input, formatProvider);
}
static TimeSpan ParseExact(String input, String format, IFormatProvider formatProvider) {
  return TimeSpanParse.ParseExact(input, format, formatProvider, TimeSpanStyles.None);
}
static TimeSpan ParseExact(String input, String[] formats, IFormatProvider formatProvider) {
  return TimeSpanParse.ParseExactMultiple(input, formats, formatProvider, TimeSpanStyles.None);
}
static TimeSpan ParseExact(String input, String format, IFormatProvider formatProvider, TimeSpanStyles styles) {
  TimeSpanParse.ValidateStyles(styles, "styles");
  return TimeSpanParse.ParseExact(input, format, formatProvider, styles);
}
static TimeSpan ParseExact(String input, String[] formats, IFormatProvider formatProvider, TimeSpanStyles styles) {
  TimeSpanParse.ValidateStyles(styles, "styles");
  return TimeSpanParse.ParseExactMultiple(input, formats, formatProvider, styles);
}
static Boolean TryParse(String s, out TimeSpan result) {
  return TimeSpanParse.TryParse(s, null, out result);
}
static Boolean TryParse(String input, IFormatProvider formatProvider, out TimeSpan result) {
  return TimeSpanParse.TryParse(input, formatProvider, out result);
}
static Boolean TryParseExact(String input, String format, IFormatProvider formatProvider, out TimeSpan result) {
  return TimeSpanParse.TryParseExact(input, format, formatProvider, TimeSpanStyles.None, out result);
}
static Boolean TryParseExact(String input, String[] formats, IFormatProvider formatProvider, out TimeSpan result) {
  return TimeSpanParse.TryParseExactMultiple(input, formats, formatProvider, TimeSpanStyles.None, out result);
}
static Boolean TryParseExact(String input, String format, IFormatProvider formatProvider, TimeSpanStyles styles, out TimeSpan result) {
  TimeSpanParse.ValidateStyles(styles, "styles");
  return TimeSpanParse.TryParseExact(input, format, formatProvider, styles, out result);
}
static Boolean TryParseExact(String input, String[] formats, IFormatProvider formatProvider, TimeSpanStyles styles, out TimeSpan result) {
  TimeSpanParse.ValidateStyles(styles, "styles");
  return TimeSpanParse.TryParseExactMultiple(input, formats, formatProvider, styles, out result);
}
override String ToString() {
  return TimeSpanFormat.Format(this, null, null);
}
String ToString(String format) {
  return TimeSpanFormat.Format(this, format, null);
}
String ToString(String format, IFormatProvider formatProvider) {
  if (LegacyMode) {
    return TimeSpanFormat.Format(this, null, null);
  }
  else {
    return TimeSpanFormat.Format(this, format, formatProvider);
  }
}
#endregion

static TimeSpan operator -(TimeSpan t) {
  if (t._ticks==TimeSpan.MinValue._ticks)
    throw new OverflowException(Environment.GetResourceString("Overflow_NegateTwosCompNum"));
  return new TimeSpan(-t._ticks);
}

static TimeSpan operator -(TimeSpan t1, TimeSpan t2) {
  return t1.Subtract(t2);
}

static TimeSpan operator +(TimeSpan t) {
  return t;
}

static TimeSpan operator +(TimeSpan t1, TimeSpan t2) {
  return t1.Add(t2);
}

static bool operator ==(TimeSpan t1, TimeSpan t2) {
  return t1._ticks == t2._ticks;
}

static bool operator !=(TimeSpan t1, TimeSpan t2) {
  return t1._ticks != t2._ticks;
}

static bool operator <(TimeSpan t1, TimeSpan t2) {
  return t1._ticks < t2._ticks;
}

static bool operator <=(TimeSpan t1, TimeSpan t2) {
  return t1._ticks <= t2._ticks;
}

static bool operator >(TimeSpan t1, TimeSpan t2) {
  return t1._ticks > t2._ticks;
}

static bool operator >=(TimeSpan t1, TimeSpan t2) {
  return t1._ticks >= t2._ticks;
}


//
// In .NET Framework v1.0 - v3.5 System.TimeSpan did not implement IFormattable
//    The composite formatter ignores format specifiers on types that do not implement
//    IFormattable, so the following code would 'just work' by using TimeSpan.ToString()
//    under the hood:
//        String.Format("{0:_someRandomFormatString_}", myTimeSpan);      
//    
// In .NET Framework v4.0 System.TimeSpan implements IFormattable.  This causes the 
//    composite formatter to call TimeSpan.ToString(string format, FormatProvider provider)
//    and pass in "_someRandomFormatString_" for the format parameter.  When the format 
//    parameter is invalid a FormatException is thrown.
//
// The 'NetFx40_TimeSpanLegacyFormatMode' per-AppDomain configuration option and the 'TimeSpan_LegacyFormatMode' 
// process-wide configuration option allows applications to run with the v1.0 - v3.5 legacy behavior.  When
// either switch is specified the format parameter is ignored and the default output is returned.
//
// There are three ways to use the process-wide configuration option:
//
// 1) Config file (MyApp.exe.config)
//        <?xml version ="1.0"?>
//        <configuration>
//         <runtime>
//          <TimeSpan_LegacyFormatMode enabled="true"/>
//         </runtime>
//        </configuration>
// 2) Environment variable
//        set COMPLUS_TimeSpan_LegacyFormatMode=1
// 3) RegistryKey
//        [HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\.NETFramework]
//        "TimeSpan_LegacyFormatMode"=dword:00000001
//

//
// In Silverlight v4, specifying the APP_EARLIER_THAN_SL4.0 quirks mode allows applications to
// run in v2 - v3 legacy behavior.
//
#if !FEATURE_CORECLR
[System.Security.SecuritySafeCritical]
#endif
static bool GetLegacyFormatMode() {
#if !FEATURE_CORECLR
  if (LegacyFormatMode()) // FCALL to check COMPLUS_TimeSpan_LegacyFormatMode
    return true;
  return CompatibilitySwitches.IsNetFx40TimeSpanLegacyFormatMode;
#else
  return CompatibilitySwitches.IsAppEarlierThanSilverlight4;
#endif // !FEATURE_CORECLR
}

static volatile bool _legacyConfigChecked;
static volatile bool _legacyMode;

static bool LegacyMode {
  get {
    if (!_legacyConfigChecked) {
      // no need to lock - idempotent
      _legacyMode = GetLegacyFormatMode();
      _legacyConfigChecked = true;
    }
    return _legacyMode;
  }
}


#endif


// This value type represents a date and time.  Every DateTime 
// object has a  field (Ticks) of type Int64 that stores the 
// date and time as the number of 100 nanosecond intervals since 
// 12:00 AM January 1, year 1 A.D. in the proleptic Gregorian Calendar.
//
// Starting from V2.0, DateTime also stored some context about its time
// zone in the form of a 3-state value representing Unspecified, Utc or
// Local. This is stored in the two top bits of the 64-bit numeric value
// with the remainder of the bits storing the tick count. This information 
// is only used during time zone conversions and is not part of the 
// identity of the DateTime. Thus, operations like Compare and Equals
// ignore this state. This is to stay compatible with earlier behavior
// and performance characteristics and to avoid forcing  people into dealing 
// with the effects of daylight savings. Note, that this has little effect
// on how the DateTime works except in a context where its specific time
// zone is needed, such as during conversions and some parsing and formatting
// cases.
//
// There is also 4th state stored that is a special type of Local value that
// is used to avoid data loss when round-tripping between local and UTC time.
// See below for more information on this 4th state, although it is 
// effectively hidden from most users, who just see the 3-state DateTimeKind
// enumeration.
//
// For compatability, DateTime does not serialize the Kind data when used in
// binary serialization.
// 
// For a description of various calendar issues, look at
// 
// Calendar Studies web site, at 
// http://serendipity.nofadz.com/hermetic/cal_stud.htm.
// 
// 
// Number of 100ns ticks per time unit
#if 0
static const int64 TicksPerMillisecond = 10000;
static const int64 TicksPerSecond = TicksPerMillisecond * 1000;
static const int64 TicksPerMinute = TicksPerSecond * 60;
static const int64 TicksPerHour = TicksPerMinute * 60;
static const int64 TicksPerDay = TicksPerHour * 24;

// Number of milliseconds per time unit
static const int MillisPerSecond = 1000;
static const int MillisPerMinute = MillisPerSecond * 60;
static const int MillisPerHour = MillisPerMinute * 60;
static const int MillisPerDay = MillisPerHour * 24;
#endif
// Number of days in a non-leap year
static const int DaysPerYear = 365;
// Number of days in 4 years
static const int DaysPer4Years = DaysPerYear * 4 + 1;       // 1461
// Number of days in 100 years
static const int DaysPer100Years = DaysPer4Years * 25 - 1;  // 36524
// Number of days in 400 years
static const int DaysPer400Years = DaysPer100Years * 4 + 1; // 146097

// Number of days from 1/1/0001 to 12/31/1600
static const int DaysTo1601 = DaysPer400Years * 4;          // 584388
// Number of days from 1/1/0001 to 12/30/1899
static const int DaysTo1899 = DaysPer400Years * 4 + DaysPer100Years * 3 - 367;
// Number of days from 1/1/0001 to 12/31/1969
static const int DaysTo1970 = DaysPer400Years * 4 + DaysPer100Years * 3 + DaysPer4Years * 17 + DaysPerYear; // 719,162
// Number of days from 1/1/0001 to 12/31/9999
static const int DaysTo10000 = DaysPer400Years * 25 - 366;  // 3652059

const int64 MinTicks = 0;
const int64 MaxTicks = DaysTo10000 * TicksPerDay - 1;
const int64 MaxMillis = (int64)DaysTo10000 * MillisPerDay;

const int64 FileTimeOffset = DaysTo1601 * TicksPerDay;
const int64 DoubleDateOffset = DaysTo1899 * TicksPerDay;
// The minimum OA date is 0100/01/01 (Note it's year 100).
// The maximum OA date is 9999/12/31
const int64 OADateMinAsTicks = (DaysPer100Years - DaysPerYear) * TicksPerDay;
// All OA dates must be greater than (not >=) OADateMinAsDouble
const double OADateMinAsDouble = -657435.0;
// All OA dates must be less than (not <=) OADateMaxAsDouble
const double OADateMaxAsDouble = 2958466.0;

static const int DatePartYear = 0;
static const int DatePartDayOfYear = 1;
static const int DatePartMonth = 2;
static const int DatePartDay = 3;

static const int DaysToMonth365[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
static const int DaysToMonth366[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};

// This enum is used to indentify DateTime instances in cases when they are known to be in local time, 
// UTC time or if this information has not been specified or is not applicable.
typedef int32 Int32;
typedef uint32 UInt32;
typedef int64 Int64;
typedef uint64 UInt64;
const UInt64 TicksMask             = 0x3FFFFFFFFFFFFFFF;
const UInt64 FlagsMask             = 0xC000000000000000;
const UInt64 LocalMask             = 0x8000000000000000;
const Int64 TicksCeiling           = 0x4000000000000000;
const UInt64 KindUnspecified       = 0x0000000000000000;
const UInt64 KindUtc               = 0x4000000000000000;
const UInt64 KindLocal             = 0x8000000000000000;
const UInt64 KindLocalAmbiguousDst = 0xC000000000000000;
const Int32 KindShift = 62;

enum DateTimeKind
{
  Unspecified = 0,
    Utc = 1,
    Local = 2,
};

const char* TicksField            = "ticks";
const char* DateDataField         = "dateData";

// The data is stored as an unsigned 64-bit integeter
//   Bits 01-62: The value of 100-nanosecond ticks where 0 represents 1/1/0001 12:00am, up until the value
//               12/31/9999 23:59:59.9999999
//   Bits 63-64: A four-state value that describes the DateTimeKind value of the date time, with a 2nd
//               value for the rare case where the date time is local, but is in an overlapped daylight
//               savings time hour and it is in daylight savings time. This allows distinction of these
//               otherwise ambiguous local times and prevents data loss when round tripping from Local to
//               UTC time.
// UInt64 dateData;

// Constructs a DateTime from a tick count. The ticks
// argument specifies the date as the number of 100-nanosecond intervals
// that have elapsed since 1/1/0001 12:00am.
//
UInt64 DateTime21(int64 ticks, DateTimeKind kind) {
  if (ticks < MinTicks || ticks > MaxTicks) {
    ArgumentOutOfRangeException("ticks" || GetResourceString("ArgumentOutOfRange_DateTimeBadTicks"));
  }
  if (kind < Unspecified || kind > Local) {
    ArgumentException(GetResourceString("Argument_InvalidDateTimeKind") || "kind");
  }
  return ((UInt64)ticks | ((UInt64)kind << KindShift));
}
#define DateTime2(ticks, kind)   ((UInt64)(ticks) | ((UInt64)(kind) << KindShift))
// typedef UInt64 DateTime;
static const UInt64 DateTimeMinValue = DateTime2(MinTicks, Unspecified);
static const UInt64 DateTimeMaxValue = DateTime2(MaxTicks, Unspecified);

typedef bool Boolean;
UInt64 DateTime3(int64 ticks, DateTimeKind kind, Boolean isAmbiguousDst) {
  if (ticks < MinTicks || ticks > MaxTicks) {
    ArgumentOutOfRangeException("ticks" || GetResourceString("ArgumentOutOfRange_DateTimeBadTicks"));
  }
  //Contract.Requires(kind == DateTimeKind.Local, "Internal Constructor is for local times only");
  return ((UInt64)ticks | (isAmbiguousDst ? KindLocalAmbiguousDst : KindLocal));
}
// Constructs a DateTime from a given year, month, and day. The
// time-of-day of the resulting DateTime is always midnight.
//
#if 0
DateTime(int year, int month, int day) {
  this.dateData = (UInt64) DateToTicks(year, month, day);
}
// Constructs a DateTime from a given year, month, and day for
// the specified calendar. The
// time-of-day of the resulting DateTime is always midnight.
//
DateTime(int year, int month, int day, Calendar calendar) 
: this(year, month, day, 0, 0, 0, calendar) {
}

// Constructs a DateTime from a given year, month, day, hour,
// minute, and second.
//
DateTime(int year, int month, int day, int hour, int minute, int second) {
  this.dateData = (UInt64)(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second));
}

DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind) {
  if (kind < DateTimeKind.Unspecified || kind > DateTimeKind.Local) {
    ArgumentException(GetResourceString("Argument_InvalidDateTimeKind"), "kind");
  }
  
  Int64 ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
  this.dateData = ((UInt64)ticks | ((UInt64)kind << KindShift));
}

// Constructs a DateTime from a given year, month, day, hour,
// minute, and second for the specified calendar.
//
DateTime(int year, int month, int day, int hour, int minute, int second, Calendar calendar) {
  if (calendar == null)
    ArgumentNullException("calendar");
  
  this.dateData = (UInt64)calendar.ToDateTime(year, month, day, hour, minute, second, 0).Ticks;
}

// Constructs a DateTime from a given year, month, day, hour,
// minute, and second.
//
DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond) {
  if (millisecond < 0 || millisecond >= MillisPerSecond) {
    ArgumentOutOfRangeException("millisecond" || GetResourceString("ArgumentOutOfRange_Range", 0, MillisPerSecond - 1));
  }
  
  Int64 ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
  ticks += millisecond * TicksPerMillisecond;
  if (ticks < MinTicks || ticks > MaxTicks)
    ArgumentException(GetResourceString("Arg_DateTimeRange"));
  this.dateData = (UInt64)ticks;
}

DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind) {
  if (millisecond < 0 || millisecond >= MillisPerSecond) {
    ArgumentOutOfRangeException("millisecond" || GetResourceString("ArgumentOutOfRange_Range", 0, MillisPerSecond - 1));
  }
  if (kind < DateTimeKind.Unspecified || kind > DateTimeKind.Local) {
    ArgumentException(GetResourceString("Argument_InvalidDateTimeKind"), "kind");
  }
  
  Int64 ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
  ticks += millisecond * TicksPerMillisecond;
  if (ticks < MinTicks || ticks > MaxTicks)
    ArgumentException(GetResourceString("Arg_DateTimeRange"));
  this.dateData = ((UInt64)ticks | ((UInt64)kind << KindShift));
}

// Constructs a DateTime from a given year, month, day, hour,
// minute, and second for the specified calendar.
//
DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, Calendar calendar) {
  if (calendar == null)
    ArgumentNullException("calendar");
  if (millisecond < 0 || millisecond >= MillisPerSecond) {
    ArgumentOutOfRangeException("millisecond" || GetResourceString("ArgumentOutOfRange_Range", 0, MillisPerSecond - 1));
  }
  
  Int64 ticks = calendar.ToDateTime(year, month, day, hour, minute, second, 0).Ticks;
  ticks += millisecond * TicksPerMillisecond;
  if (ticks < MinTicks || ticks > MaxTicks)
    ArgumentException(GetResourceString("Arg_DateTimeRange"));
  this.dateData = (UInt64)ticks;
}

DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, Calendar calendar, DateTimeKind kind) {
  if (calendar == null)
    ArgumentNullException("calendar");
  if (millisecond < 0 || millisecond >= MillisPerSecond) {
    ArgumentOutOfRangeException("millisecond" || GetResourceString("ArgumentOutOfRange_Range", 0, MillisPerSecond - 1));
  }            
  if (kind < DateTimeKind.Unspecified || kind > DateTimeKind.Local) {
    ArgumentException(GetResourceString("Argument_InvalidDateTimeKind"), "kind");
  }
  
  Int64 ticks = calendar.ToDateTime(year, month, day, hour, minute, second, 0).Ticks;
  ticks += millisecond * TicksPerMillisecond;
  if (ticks < MinTicks || ticks > MaxTicks)
    ArgumentException(GetResourceString("Arg_DateTimeRange"));
  this.dateData = ((UInt64)ticks | ((UInt64)kind << KindShift));
}

DateTime(SerializationInfo info, StreamingContext context) {
  if (info==null)
    ArgumentNullException("info");
  
  
  Boolean foundTicks = false;
  Boolean foundDateData = false;
  Int64 serializedTicks = 0;
  UInt64 serializedDateData = 0;
  
  
  // Get the data
  SerializationInfoEnumerator enumerator = info.GetEnumerator();
  while( enumerator.MoveNext()) {
    switch( enumerator.Name) {
    case TicksField:
      serializedTicks = Convert.ToInt64(enumerator.Value, CultureInfo.InvariantCulture);
      foundTicks = true;
      break;
    case DateDataField:
      serializedDateData = Convert.ToUInt64(enumerator.Value, CultureInfo.InvariantCulture);
      foundDateData = true;
      break;
    default:
      // Ignore other fields for forward compatability.
      break;
    }
  }
  if (foundDateData) {
    this.dateData = serializedDateData;
  }
  else if (foundTicks) {
    this.dateData = (UInt64)serializedTicks;
  }
  else {
    SerializationException(GetResourceString("Serialization_MissingDateTimeData"));                
  }
  Int64 ticks = InternalTicks;
  if (ticks < MinTicks || ticks > MaxTicks) {
    SerializationException(GetResourceString("Serialization_DateTimeTicksOutOfRange"));
  }                        
}

// Returns the DateTime resulting from adding the given
// TimeSpan to this DateTime.
//
DateTime Add(TimeSpan value) {
  return AddTicks(value._ticks);
}

// Returns the DateTime resulting from adding a fractional number of
// time units to this DateTime.
DateTime Add(double value, int scale) {
  int64 millis = (int64)(value * scale + (value >= 0? 0.5: -0.5));
  if (millis <= -MaxMillis || millis >= MaxMillis) 
    ArgumentOutOfRangeException("value" || GetResourceString("ArgumentOutOfRange_AddValue"));
  return AddTicks(millis * TicksPerMillisecond);
}

// Returns the DateTime resulting from adding a fractional number of
// days to this DateTime. The result is computed by rounding the
// fractional number of days given by value to the nearest
// millisecond, and adding that interval to this DateTime. The
// value argument is permitted to be negative.
//
DateTime AddDays(double value) {
  return Add(value, MillisPerDay);
}

// Returns the DateTime resulting from adding a fractional number of
// hours to this DateTime. The result is computed by rounding the
// fractional number of hours given by value to the nearest
// millisecond, and adding that interval to this DateTime. The
// value argument is permitted to be negative.
//
DateTime AddHours(double value) {
  return Add(value, MillisPerHour);
}

// Returns the DateTime resulting from the given number of
// milliseconds to this DateTime. The result is computed by rounding
// the number of milliseconds given by value to the nearest integer,
// and adding that interval to this DateTime. The value
// argument is permitted to be negative.
//
DateTime AddMilliseconds(double value) {
  return Add(value, 1);
}

// Returns the DateTime resulting from adding a fractional number of
// minutes to this DateTime. The result is computed by rounding the
// fractional number of minutes given by value to the nearest
// millisecond, and adding that interval to this DateTime. The
// value argument is permitted to be negative.
//
DateTime AddMinutes(double value) {
  return Add(value, MillisPerMinute);
}

#endif
Int64 getInternalTicks(UInt64 dateData) {
  return (Int64)(dateData & TicksMask);
}

UInt64 getInternalKind(UInt64 dateData) {
  return (dateData & FlagsMask);
}        

// Returns a given date part of this DateTime. This method is used
// to compute the year, day-of-year, month, or day part.
int GetDatePart(UInt64 dateData, int part) {
  Int64 ticks = getInternalTicks(dateData);
  // n = number of days since 1/1/0001
  int n = (int)(ticks / TicksPerDay);
  // y400 = number of whole 400-year periods since 1/1/0001
  int y400 = n / DaysPer400Years;
  // n = day number within 400-year period
  n -= y400 * DaysPer400Years;
  // y100 = number of whole 100-year periods within 400-year period
  int y100 = n / DaysPer100Years;
  // Last 100-year period has an extra day, so decrement result if 4
  if (y100 == 4) y100 = 3;
  // n = day number within 100-year period
  n -= y100 * DaysPer100Years;
  // y4 = number of whole 4-year periods within 100-year period
  int y4 = n / DaysPer4Years;
  // n = day number within 4-year period
  n -= y4 * DaysPer4Years;
  // y1 = number of whole years within 4-year period
  int y1 = n / DaysPerYear;
  // Last year has an extra day, so decrement result if 4
  if (y1 == 4) y1 = 3;
  // If year was requested, compute and return it
  if (part == DatePartYear) {
    return y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
  }
  // n = day number within year
  n -= y1 * DaysPerYear;
  // If day-of-year was requested, return it
  if (part == DatePartDayOfYear) return n + 1;
  // Leap year calculation looks different from IsLeapYear since y1, y4,
  // and y100 are relative to year 1, not year 0
  bool leapYear = y1 == 3 && (y4 != 24 || y100 == 3);
  const int* days = leapYear? DaysToMonth366: DaysToMonth365;
  // All months have less than 32 days, so n >> 5 is a good conservative
  // estimate for the month
  int m = (n >> 5) + 1;
  // m = 1-based month number
  while (n >= days[m]) m++;
  // If month was requested, return it
  if (part == DatePartMonth) return m;
  // Return 1-based day-of-month
  return n - days[m - 1] + 1;
}

// Checks whether a given year is a leap year. This method returns true if
// year is a leap year, or false if not.
//
static bool IsLeapYear(int year) {
  if (year < 1 || year > 9999) {
    ArgumentOutOfRangeException("year" || GetResourceString("ArgumentOutOfRange_Year"));
  }
  return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

// Returns the tick count corresponding to the given year, month, and day.
// Will check the if the parameters are valid.
static int64 DateToTicks(int year, int month, int day) {     
  if (year >= 1 && year <= 9999 && month >= 1 && month <= 12) {
    const int* days = IsLeapYear(year)? DaysToMonth366 : DaysToMonth365;
    if (day >= 1 && day <= days[month] - days[month - 1]) {
      int y = year - 1;
      int n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;
      return n * TicksPerDay;
    }
  }
  ArgumentOutOfRangeException(GetResourceString("ArgumentOutOfRange_BadYearMonthDay"));
  return 0;
}

// Returns the number of days in the month given by the year and
// month arguments.
//
static int DaysInMonth(int year, int month) {
  if (month < 1 || month > 12) ArgumentOutOfRangeException("month" || GetResourceString("ArgumentOutOfRange_Month"));
  
  // IsLeapYear checks the year argument
  const int* days = IsLeapYear(year)? DaysToMonth366: DaysToMonth365;
  return days[month] - days[month - 1];
}

// Converts an OLE Date to a tick count.
// This function is duplicated in COMDateTime.cpp
static int64 DoubleDateToTicks(double value) {
  // The check done this way will take care of NaN
  if (!(value < OADateMaxAsDouble) || !(value > OADateMinAsDouble))
    ArgumentException(GetResourceString("Arg_OleAutDateInvalid"));
  
  // Conversion to int64 will not cause an overflow here, as at this point the "value" is in between OADateMinAsDouble and OADateMaxAsDouble
  int64 millis = (int64)(value * MillisPerDay + (value >= 0? 0.5: -0.5));
  // The interesting thing here is when you have a value like 12.5 it all positive 12 days and 12 hours from 01/01/1899
  // However if you a value of -12.25 it is minus 12 days but still positive 6 hours, almost as though you meant -11.75 all negative
  // This line below fixes up the millis in the negative case
  if (millis < 0) {
    millis -= (millis % MillisPerDay) * 2;
  }
  
  millis += DoubleDateOffset / TicksPerMillisecond;
  
  if (millis < 0 || millis >= MaxMillis) ArgumentException(GetResourceString("Arg_OleAutDateScale"));
  return millis * TicksPerMillisecond;
}

// Returns the DateTime resulting from adding the given number of
// months to this DateTime. The result is computed by incrementing
// (or decrementing) the year and month parts of this DateTime by
// months months, and, if required, adjusting the day part of the
// resulting date downwards to the last day of the resulting month in the
// resulting year. The time-of-day part of the result is the same as the
// time-of-day part of this DateTime.
//
// In more precise terms, considering this DateTime to be of the
// form y / m / d + t, where y is the
// year, m is the month, d is the day, and t is the
// time-of-day, the result is y1 / m1 / d1 + t,
// where y1 and m1 are computed by adding months months
// to y and m, and d1 is the largest value less than
// or equal to d that denotes a valid day in month m1 of year
// y1.
UInt64 AddMonths(UInt64 dateData, int months) {
  if (months < -120000 || months > 120000) {
    ArgumentOutOfRangeException("months" || GetResourceString("ArgumentOutOfRange_DateTimeBadMonths"));
  }
  //
  int y = GetDatePart(dateData, DatePartYear);
  int m = GetDatePart(dateData, DatePartMonth);
  int d = GetDatePart(dateData, DatePartDay);
  int i = m - 1 + months;
  if (i >= 0) {
    m = i % 12 + 1;
    y = y + i / 12;
  }
  else {
    m = 12 + (i + 1) % 12;
    y = y + (i - 11) / 12;
  }
  if (y < 1 || y > 9999) {
    ArgumentOutOfRangeException("months" || GetResourceString("ArgumentOutOfRange_DateArithmetic"));
  }
  int days = DaysInMonth(y, m);
  if (d > days) d = days;
  return ((UInt64)(DateToTicks(y, m, d) + getInternalTicks(dateData) % TicksPerDay) | getInternalKind(dateData));
}
#if 0
// Returns the DateTime resulting from adding a fractional number of
// seconds to this DateTime. The result is computed by rounding the
// fractional number of seconds given by value to the nearest
// millisecond, and adding that interval to this DateTime. The
// value argument is permitted to be negative.
//
DateTime AddSeconds(double value) {
  return Add(value, MillisPerSecond);
}

// Returns the DateTime resulting from adding the given number of
// 100-nanosecond ticks to this DateTime. The value argument
// is permitted to be negative.
//
DateTime AddTicks(int64 value) {
  int64 ticks = InternalTicks;
  if (value > MaxTicks - ticks || value < MinTicks - ticks) {
    ArgumentOutOfRangeException("value" || GetResourceString("ArgumentOutOfRange_DateArithmetic"));
  }
  return new DateTime((UInt64)(ticks + value) | InternalKind);
}

// Returns the DateTime resulting from adding the given number of
// years to this DateTime. The result is computed by incrementing
// (or decrementing) the year part of this DateTime by value
// years. If the month and day of this DateTime is 2/29, and if the
// resulting year is not a leap year, the month and day of the resulting
// DateTime becomes 2/28. Otherwise, the month, day, and time-of-day
// parts of the result are the same as those of this DateTime.
//
DateTime AddYears(int value) {
  if (value < -10000 || value > 10000) ArgumentOutOfRangeException("years" || GetResourceString("ArgumentOutOfRange_DateTimeBadYears"));
  
  return AddMonths(value * 12);
}

// Compares two DateTime values, returning an integer that indicates
// their relationship.
//
static int Compare(DateTime t1, DateTime t2) {            
  Int64 ticks1 = t1.InternalTicks;
  Int64 ticks2 = t2.InternalTicks;
  if (ticks1 > ticks2) return 1;
  if (ticks1 < ticks2) return -1;
  return 0;
}

// Compares this DateTime to a given object. This method provides an
// implementation of the IComparable interface. The object
// argument must be another DateTime, or otherwise an exception
// occurs.  Null is considered less than any instance.
//
// Returns a value less than zero if this  object
int CompareTo(Object value) {
  if (value == null) return 1;
  if (!(value is DateTime)) {
    ArgumentException(GetResourceString("Arg_MustBeDateTime"));
  }
  
  int64 valueTicks = ((DateTime)value).InternalTicks;
  int64 ticks = InternalTicks;
  if (ticks > valueTicks) return 1;
  if (ticks < valueTicks) return -1;
  return 0;
}

int CompareTo(DateTime value) {
  int64 valueTicks = value.InternalTicks;
  int64 ticks = InternalTicks;
  if (ticks > valueTicks) return 1;
  if (ticks < valueTicks) return -1;
  return 0;
}

// Checks if this DateTime is equal to a given object. Returns
// true if the given object is a boxed DateTime and its value
// is equal to the value of this DateTime. Returns false
// otherwise.
//
override bool Equals(Object value) {
  if (value is DateTime) {
    return InternalTicks == ((DateTime)value).InternalTicks;
  }
  return false;
}

bool Equals(DateTime value) {
  return InternalTicks == value.InternalTicks;
}

// Compares two DateTime values for equality. Returns true if
// the two DateTime values are equal, or false if they are
// not equal.
//
static bool Equals(DateTime t1, DateTime t2) {
  return t1.InternalTicks == t2.InternalTicks;
}

static DateTime FromBinary(Int64 dateData) {        
  if ((dateData & (unchecked( (Int64) LocalMask))) != 0) {
    // Local times need to be adjusted as you move from one time zone to another, 
    // just as they are when serializing in text. As such the format for local times
    // changes to store the ticks of the UTC time, but with flags that look like a 
    // local date.
    Int64 ticks = dateData & (unchecked((Int64)TicksMask));
    // Negative ticks are stored in the top part of the range and should be converted back into a negative number
    if (ticks > TicksCeiling - TicksPerDay) {
      ticks = ticks - TicksCeiling;
    }
    // Convert the ticks back to local. If the UTC ticks are out of range, we need to default to
    // the UTC offset from MinValue and MaxValue to be consistent with Parse. 
    Boolean isAmbiguousLocalDst = false;
    Int64 offsetTicks;
    if (ticks < MinTicks) {
      offsetTicks = TimeZoneInfo.GetLocalUtcOffset(DateTime.MinValue, TimeZoneInfoOptions.NoThrowOnInvalidTime).Ticks;
    }
    else if (ticks > MaxTicks) {
      offsetTicks = TimeZoneInfo.GetLocalUtcOffset(DateTime.MaxValue, TimeZoneInfoOptions.NoThrowOnInvalidTime).Ticks;
    }
    else {                    
      // Because the ticks conversion between UTC and local is lossy, we need to capture whether the 
      // time is in a repeated hour so that it can be passed to the DateTime constructor.
      DateTime utcDt = new DateTime(ticks, DateTimeKind.Utc);
      Boolean isDaylightSavings = false;
      offsetTicks = TimeZoneInfo.GetUtcOffsetFromUtc(utcDt, TimeZoneInfo.Local, out isDaylightSavings, out isAmbiguousLocalDst).Ticks;
    }        
    ticks += offsetTicks;
    // Another behaviour of parsing is to cause small times to wrap around, so that they can be used
    // to compare times of day
    if (ticks < 0) {
      ticks += TicksPerDay;
    }                
    if (ticks < MinTicks || ticks > MaxTicks) {
      ArgumentException(GetResourceString("Argument_DateTimeBadBinaryData"), "dateData");
    }
    return new DateTime(ticks, DateTimeKind.Local, isAmbiguousLocalDst);                
  }
  else {
    return DateTime.FromBinaryRaw(dateData);
  }
}        

// A version of ToBinary that uses the real representation and does not adjust local times. This is needed for
// scenarios where the serialized data must maintain compatability
static DateTime FromBinaryRaw(Int64 dateData) {        
  Int64 ticks = dateData & (Int64)TicksMask;
  if (ticks < MinTicks || ticks > MaxTicks)
    ArgumentException(GetResourceString("Argument_DateTimeBadBinaryData"), "dateData");
  return new DateTime((UInt64)dateData);
}        

// Creates a DateTime from a Windows filetime. A Windows filetime is
// a int64 representing the date and time as the number of
// 100-nanosecond intervals that have elapsed since 1/1/1601 12:00am.
//
static DateTime FromFileTime(int64 fileTime) {
  return FromFileTimeUtc(fileTime).ToLocalTime();
}

static DateTime FromFileTimeUtc(int64 fileTime) {
  if (fileTime < 0 || fileTime > MaxTicks - FileTimeOffset) {
    ArgumentOutOfRangeException("fileTime" || GetResourceString("ArgumentOutOfRange_FileTimeInvalid"));
  }
  
  
  // This is the ticks in Universal time for this fileTime.
  int64 universalTicks = fileTime + FileTimeOffset;            
  return new DateTime(universalTicks, DateTimeKind.Utc);
}

// Creates a DateTime from an OLE Automation Date.
//
static DateTime FromOADate(double d) {
  return new DateTime(DoubleDateToTicks(d), DateTimeKind.Unspecified);
}        

#if FEATURE_SERIALIZATION
[System.Security.SecurityCritical /*auto-generated_required*/]
void ISerializable.GetObjectData(SerializationInfo info, StreamingContext context) {
  if (info==null) {
    ArgumentNullException("info");
  }
  
  
  // Serialize both the old and the new format
  info.AddValue(TicksField, InternalTicks);
  info.AddValue(DateDataField, dateData);
}        
#endif

Boolean IsDaylightSavingTime() {
  if (Kind == DateTimeKind.Utc) {
    return false;
  }
  return TimeZoneInfo.Local.IsDaylightSavingTime(this, TimeZoneInfoOptions.NoThrowOnInvalidTime);
}        

static DateTime SpecifyKind(DateTime value, DateTimeKind kind) {
  return new DateTime(value.InternalTicks, kind);
}            

Int64 ToBinary() {
  if (Kind == DateTimeKind.Local) {
    // Local times need to be adjusted as you move from one time zone to another, 
    // just as they are when serializing in text. As such the format for local times
    // changes to store the ticks of the UTC time, but with flags that look like a 
    // local date.
    
    // To match serialization in text we need to be able to handle cases where
    // the UTC value would be out of range. Unused parts of the ticks range are
    // used for this, so that values just past max value are stored just past the
    // end of the maximum range, and values just below minimum value are stored
    // at the end of the ticks area, just below 2^62.
    TimeSpan offset = TimeZoneInfo.GetLocalUtcOffset(this, TimeZoneInfoOptions.NoThrowOnInvalidTime);
    Int64 ticks = Ticks;
    Int64 storedTicks = ticks - offset.Ticks;
    if (storedTicks < 0) {
      storedTicks = TicksCeiling + storedTicks;
    }
    return storedTicks | (unchecked((Int64) LocalMask));
  }
  else {
    return (Int64)dateData;
  }
}        

// Return the underlying data, without adjust local times to the right time zone. Needed if performance
// or compatability are important.
Int64 ToBinaryRaw() {
  return (Int64)dateData;
}        

// Returns the date part of this DateTime. The resulting value
// corresponds to this DateTime with the time-of-day part set to
// zero (midnight).
//
DateTime Date {
  get { 
    Int64 ticks = InternalTicks;
    return new DateTime((UInt64)(ticks - ticks % TicksPerDay) | InternalKind);
  }
}

// Returns the day-of-month part of this DateTime. The returned
// value is an integer between 1 and 31.
//
int Day {
  get {
    Contract.Ensures(Contract.Result<int>() >= 1);
    Contract.Ensures(Contract.Result<int>() <= 31);
    return GetDatePart(DatePartDay);
  }
}

// Returns the day-of-week part of this DateTime. The returned value
// is an integer between 0 and 6, where 0 indicates Sunday, 1 indicates
// Monday, 2 indicates Tuesday, 3 indicates Wednesday, 4 indicates
// Thursday, 5 indicates Friday, and 6 indicates Saturday.
//
DayOfWeek DayOfWeek {
  get {
    Contract.Ensures(Contract.Result<DayOfWeek>() >= DayOfWeek.Sunday);
    Contract.Ensures(Contract.Result<DayOfWeek>() <= DayOfWeek.Saturday);
    return (DayOfWeek)((InternalTicks / TicksPerDay + 1) % 7);
  }
}

// Returns the day-of-year part of this DateTime. The returned value
// is an integer between 1 and 366.
//
int DayOfYear {
  get {
    Contract.Ensures(Contract.Result<int>() >= 1);
    Contract.Ensures(Contract.Result<int>() <= 366);  // leap year
    return GetDatePart(DatePartDayOfYear);
  }
}

// Returns the hash code for this DateTime.
//
override int GetHashCode() {
  Int64 ticks = InternalTicks;
  return unchecked((int)ticks) ^ (int)(ticks >> 32);
}

// Returns the hour part of this DateTime. The returned value is an
// integer between 0 and 23.
//
int Hour {
  get {
    Contract.Ensures(Contract.Result<int>() >= 0);
    Contract.Ensures(Contract.Result<int>() < 24);
    return (int)((InternalTicks / TicksPerHour) % 24); 
  }
}

Boolean IsAmbiguousDaylightSavingTime() {
  return (InternalKind == KindLocalAmbiguousDst);
}                

[Pure]
DateTimeKind Kind {
  get {
    switch (InternalKind) {
    case KindUnspecified:
      return DateTimeKind.Unspecified;
    case KindUtc:
      return DateTimeKind.Utc;
    default:
      return DateTimeKind.Local;                    
    }
  }
}        

// Returns the millisecond part of this DateTime. The returned value
// is an integer between 0 and 999.
//
int Millisecond {
  get {
    Contract.Ensures(Contract.Result<int>() >= 0);
    Contract.Ensures(Contract.Result<int>() < 1000);
    return (int)((InternalTicks/ TicksPerMillisecond) % 1000); 
  }
}

// Returns the minute part of this DateTime. The returned value is
// an integer between 0 and 59.
//
int Minute {
  get {
    Contract.Ensures(Contract.Result<int>() >= 0);
    Contract.Ensures(Contract.Result<int>() < 60);
    return (int)((InternalTicks / TicksPerMinute) % 60); 
  }
}

// Returns the month part of this DateTime. The returned value is an
// integer between 1 and 12.
//
int Month {
  get {
    Contract.Ensures(Contract.Result<int>() >= 1);
    return GetDatePart(DatePartMonth);
  }
}

// Returns a DateTime representing the current date and time. The
// resolution of the returned value depends on the system timer. For
// Windows NT 3.5 and later the timer resolution is approximately 10ms,
// for Windows NT 3.1 it is approximately 16ms, and for Windows 95 and 98
// it is approximately 55ms.
//
static DateTime Now {
  get {
    Contract.Ensures(Contract.Result<DateTime>().Kind == DateTimeKind.Local);
    
    DateTime utc = UtcNow; 
    Boolean isAmbiguousLocalDst = false;
    Int64 offset = TimeZoneInfo.GetDateTimeNowUtcOffsetFromUtc(utc, out isAmbiguousLocalDst).Ticks;
    int64 tick = utc.Ticks + offset;
    if (tick>DateTime.MaxTicks) {
      return new DateTime(DateTime.MaxTicks, DateTimeKind.Local);
    }
    if (tick<DateTime.MinTicks) {
      return new DateTime(DateTime.MinTicks, DateTimeKind.Local);
    }
    return new DateTime(tick, DateTimeKind.Local, isAmbiguousLocalDst);  
  }
}

static DateTime UtcNow {
  [System.Security.SecuritySafeCritical]  // auto-generated
    get {
    Contract.Ensures(Contract.Result<DateTime>().Kind == DateTimeKind.Utc);
    // following code is tuned for speed. Don't change it without running benchmark.
    int64 ticks = 0;
    ticks = GetSystemTimeAsFileTime();
    
#if FEATURE_LEGACYNETCF
    // Windows Phone 7.0/7.1 return the ticks up to millisecond, not up to the 100th nanosecond.
    if (CompatibilitySwitches.IsAppEarlierThanWindowsPhone8)
    {
      int64 ticksms = ticks / TicksPerMillisecond;
      ticks = ticksms * TicksPerMillisecond;
    }
#endif
    return new DateTime( ((UInt64)(ticks + FileTimeOffset)) | KindUtc);
  }
}


[System.Security.SecurityCritical]  // auto-generated
[MethodImplAttribute(MethodImplOptions.InternalCall)]
static extern int64 GetSystemTimeAsFileTime();



// Returns the second part of this DateTime. The returned value is
// an integer between 0 and 59.
//
int Second {
  get {
    Contract.Ensures(Contract.Result<int>() >= 0);
    Contract.Ensures(Contract.Result<int>() < 60);
    return (int)((InternalTicks / TicksPerSecond) % 60); 
  }
}    

// Returns the tick count for this DateTime. The returned value is
// the number of 100-nanosecond intervals that have elapsed since 1/1/0001
// 12:00am.
//
int64 Ticks {
  get { 
    return InternalTicks; 
  }
}

// Returns the time-of-day part of this DateTime. The returned value
// is a TimeSpan that indicates the time elapsed since midnight.
//
TimeSpan TimeOfDay {
  get { 
    return new TimeSpan(InternalTicks % TicksPerDay); 
  }
}

// Returns a DateTime representing the current date. The date part
// of the returned value is the current date, and the time-of-day part of
// the returned value is zero (midnight).
//
static DateTime Today {
  get {
    return DateTime.Now.Date;
  }
}

// Returns the year part of this DateTime. The returned value is an
// integer between 1 and 9999.
//
int Year {
  get {
    Contract.Ensures(Contract.Result<int>() >= 1 && Contract.Result<int>() <= 9999);
    return GetDatePart(DatePartYear);
  }
}

// Constructs a DateTime from a string. The string must specify a
// date and optionally a time in a culture-specific or universal format.
// Leading and trailing whitespace characters are allowed.
// 
static DateTime Parse(String s) {
  return (DateTimeParse.Parse(s, DateTimeFormatInfo.CurrentInfo, DateTimeStyles.None));
}

// Constructs a DateTime from a string. The string must specify a
// date and optionally a time in a culture-specific or universal format.
// Leading and trailing whitespace characters are allowed.
// 
static DateTime Parse(String s, IFormatProvider provider) {
  return (DateTimeParse.Parse(s, DateTimeFormatInfo.GetInstance(provider), DateTimeStyles.None));
}

static DateTime Parse(String s, IFormatProvider provider, DateTimeStyles styles) {
  DateTimeFormatInfo.ValidateStyles(styles, "styles");
  return (DateTimeParse.Parse(s, DateTimeFormatInfo.GetInstance(provider), styles));
}

// Constructs a DateTime from a string. The string must specify a
// date and optionally a time in a culture-specific or universal format.
// Leading and trailing whitespace characters are allowed.
// 
static DateTime ParseExact(String s, String format, IFormatProvider provider) {
  return (DateTimeParse.ParseExact(s, format, DateTimeFormatInfo.GetInstance(provider), DateTimeStyles.None));
}

// Constructs a DateTime from a string. The string must specify a
// date and optionally a time in a culture-specific or universal format.
// Leading and trailing whitespace characters are allowed.
// 
static DateTime ParseExact(String s, String format, IFormatProvider provider, DateTimeStyles style) {
  DateTimeFormatInfo.ValidateStyles(style, "style");
  return (DateTimeParse.ParseExact(s, format, DateTimeFormatInfo.GetInstance(provider), style));
}    

static DateTime ParseExact(String s, String[] formats, IFormatProvider provider, DateTimeStyles style) {
  DateTimeFormatInfo.ValidateStyles(style, "style");
  return DateTimeParse.ParseExactMultiple(s, formats, DateTimeFormatInfo.GetInstance(provider), style);
}

TimeSpan Subtract(DateTime value) {
  return new TimeSpan(InternalTicks - value.InternalTicks);
}

DateTime Subtract(TimeSpan value) {
  int64 ticks = InternalTicks;            
  int64 valueTicks = value._ticks;
  if (ticks - MinTicks < valueTicks || ticks - MaxTicks > valueTicks) {
    ArgumentOutOfRangeException("value" || GetResourceString("ArgumentOutOfRange_DateArithmetic"));
  }
  return new DateTime((UInt64)(ticks - valueTicks) | InternalKind);
}

// This function is duplicated in COMDateTime.cpp
static double TicksToOADate(int64 value) {
  if (value == 0)
    return 0.0;  // Returns OleAut's zero'ed date value.
  if (value < TicksPerDay) // This is a fix for VB. They want the default day to be 1/1/0001 rathar then 12/30/1899.
    value += DoubleDateOffset; // We could have moved this fix down but we would like to keep the bounds check.
  if (value < OADateMinAsTicks)
    OverflowException(GetResourceString("Arg_OleAutDateInvalid"));
  // Currently, our max date == OA's max date (12/31/9999), so we don't 
  // need an overflow check in that direction.
  int64 millis = (value  - DoubleDateOffset) / TicksPerMillisecond;
  if (millis < 0) {
    int64 frac = millis % MillisPerDay;
    if (frac != 0) millis -= (MillisPerDay + frac) * 2;
  }
  return (double)millis / MillisPerDay;
}

// Converts the DateTime instance into an OLE Automation compatible
// double date.
double ToOADate() {
  return TicksToOADate(InternalTicks);
}

int64 ToFileTime() {
  // Treats the input as local if it is not specified
  return ToUniversalTime().ToFileTimeUtc();
}

int64 ToFileTimeUtc() {
  // Treats the input as universal if it is not specified
  int64 ticks = ((InternalKind & LocalMask) != 0) ? ToUniversalTime().InternalTicks : this.InternalTicks;
  ticks -= FileTimeOffset;
  if (ticks < 0) {
    ArgumentOutOfRangeException(null || GetResourceString("ArgumentOutOfRange_FileTimeInvalid"));
  }
  return ticks;
}

DateTime ToLocalTime()
{
  return ToLocalTime(false);
}

DateTime ToLocalTime(bool throwOnOverflow)
{
  if (Kind == DateTimeKind.Local) {
    return this;
  }
  Boolean isDaylightSavings = false;
  Boolean isAmbiguousLocalDst = false;
  Int64 offset = TimeZoneInfo.GetUtcOffsetFromUtc(this, TimeZoneInfo.Local, out isDaylightSavings, out isAmbiguousLocalDst).Ticks;
  int64 tick = Ticks + offset;
  if (tick > DateTime.MaxTicks)
  {
    if (throwOnOverflow)
      ArgumentException(GetResourceString("Arg_ArgumentOutOfRangeException"));
    else
      return new DateTime(DateTime.MaxTicks, DateTimeKind.Local);
  }
  if (tick < DateTime.MinTicks)
  {
    if (throwOnOverflow)
      ArgumentException(GetResourceString("Arg_ArgumentOutOfRangeException"));
    else
      return new DateTime(DateTime.MinTicks, DateTimeKind.Local);
  }
  return new DateTime(tick, DateTimeKind.Local, isAmbiguousLocalDst);  
}

String ToLongDateString() {
  Contract.Ensures(Contract.Result<String>() != null);
  return DateTimeFormat.Format(this, "D", DateTimeFormatInfo.CurrentInfo);
}

String ToLongTimeString() {
  Contract.Ensures(Contract.Result<String>() != null);
  return DateTimeFormat.Format(this, "T", DateTimeFormatInfo.CurrentInfo);
}

String ToShortDateString() {
  Contract.Ensures(Contract.Result<String>() != null);
  return DateTimeFormat.Format(this, "d", DateTimeFormatInfo.CurrentInfo);
}

String ToShortTimeString() {
  Contract.Ensures(Contract.Result<String>() != null);
  return DateTimeFormat.Format(this, "t", DateTimeFormatInfo.CurrentInfo);
}

override String ToString() {
  Contract.Ensures(Contract.Result<String>() != null);
  return DateTimeFormat.Format(this, null, DateTimeFormatInfo.CurrentInfo);
}

String ToString(String format) {
  Contract.Ensures(Contract.Result<String>() != null);
  return DateTimeFormat.Format(this, format, DateTimeFormatInfo.CurrentInfo);
}

String ToString(IFormatProvider provider) {
  Contract.Ensures(Contract.Result<String>() != null);
  return DateTimeFormat.Format(this, null, DateTimeFormatInfo.GetInstance(provider));
}

String ToString(String format, IFormatProvider provider) {
  Contract.Ensures(Contract.Result<String>() != null);
  return DateTimeFormat.Format(this, format, DateTimeFormatInfo.GetInstance(provider));
}

DateTime ToUniversalTime() {
  return TimeZoneInfo.ConvertTimeToUtc(this, TimeZoneInfoOptions.NoThrowOnInvalidTime);
}

static Boolean TryParse(String s, out DateTime result) {
  return DateTimeParse.TryParse(s, DateTimeFormatInfo.CurrentInfo, DateTimeStyles.None, out result);
}

static Boolean TryParse(String s, IFormatProvider provider, DateTimeStyles styles, out DateTime result) {
  DateTimeFormatInfo.ValidateStyles(styles, "styles");
  return DateTimeParse.TryParse(s, DateTimeFormatInfo.GetInstance(provider), styles, out result);
}    

static Boolean TryParseExact(String s, String format, IFormatProvider provider, DateTimeStyles style, out DateTime result) {
  DateTimeFormatInfo.ValidateStyles(style, "style");
  return DateTimeParse.TryParseExact(s, format, DateTimeFormatInfo.GetInstance(provider), style, out result);
}    

static Boolean TryParseExact(String s, String[] formats, IFormatProvider provider, DateTimeStyles style, out DateTime result) {
  DateTimeFormatInfo.ValidateStyles(style, "style");
  return DateTimeParse.TryParseExactMultiple(s, formats, DateTimeFormatInfo.GetInstance(provider), style, out result);
}

static DateTime operator +(DateTime d, TimeSpan t) {
  int64 ticks = d.InternalTicks;
  int64 valueTicks = t._ticks;
  if (valueTicks > MaxTicks - ticks || valueTicks < MinTicks - ticks) {
    ArgumentOutOfRangeException("t" || GetResourceString("ArgumentOutOfRange_DateArithmetic"));
  }
  return new DateTime((UInt64)(ticks + valueTicks) | d.InternalKind);
}

static DateTime operator -(DateTime d, TimeSpan t) {
  int64 ticks = d.InternalTicks;            
  int64 valueTicks = t._ticks;
  if (ticks - MinTicks < valueTicks || ticks - MaxTicks > valueTicks) {
    ArgumentOutOfRangeException("t" || GetResourceString("ArgumentOutOfRange_DateArithmetic"));
  }
  return new DateTime((UInt64)(ticks - valueTicks) | d.InternalKind);
}

static TimeSpan operator -(DateTime d1, DateTime d2) {
  return new TimeSpan(d1.InternalTicks - d2.InternalTicks);
}

static bool operator ==(DateTime d1, DateTime d2) {
  return d1.InternalTicks == d2.InternalTicks;
}

static bool operator !=(DateTime d1, DateTime d2) {
  return d1.InternalTicks != d2.InternalTicks;
}

static bool operator <(DateTime t1, DateTime t2) {
  return t1.InternalTicks < t2.InternalTicks;
}

static bool operator <=(DateTime t1, DateTime t2) {
  return t1.InternalTicks <= t2.InternalTicks;
}

static bool operator >(DateTime t1, DateTime t2) {
  return t1.InternalTicks > t2.InternalTicks;
}

static bool operator >=(DateTime t1, DateTime t2) {
  return t1.InternalTicks >= t2.InternalTicks;
}


// Returns a string array containing all of the known date and time options for the 
// current culture.  The strings returned are properly formatted date and 
// time strings for the current instance of DateTime.
String[] GetDateTimeFormats()
{
  Contract.Ensures(Contract.Result<String[]>() != null);
  return (GetDateTimeFormats(CultureInfo.CurrentCulture));
}

// Returns a string array containing all of the known date and time options for the 
// using the information provided by IFormatProvider.  The strings returned are properly formatted date and 
// time strings for the current instance of DateTime.
String[] GetDateTimeFormats(IFormatProvider provider)
{
  Contract.Ensures(Contract.Result<String[]>() != null);
  return (DateTimeFormat.GetAllDateTimes(this, DateTimeFormatInfo.GetInstance(provider)));
}


// Returns a string array containing all of the date and time options for the 
// given format format and current culture.  The strings returned are properly formatted date and 
// time strings for the current instance of DateTime.
String[] GetDateTimeFormats(char format)
{
  Contract.Ensures(Contract.Result<String[]>() != null);
  return (GetDateTimeFormats(format, CultureInfo.CurrentCulture));
}

// Returns a string array containing all of the date and time options for the 
// given format format and given culture.  The strings returned are properly formatted date and 
// time strings for the current instance of DateTime.
String[] GetDateTimeFormats(char format, IFormatProvider provider)
{
  Contract.Ensures(Contract.Result<String[]>() != null);
  return (DateTimeFormat.GetAllDateTimes(this, format, DateTimeFormatInfo.GetInstance(provider)));
}

//
// IConvertible implementation
// 

TypeCode GetTypeCode() {
  return TypeCode.DateTime;
}


/// <internalonly/>
bool IConvertible.ToBoolean(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "Boolean"));
}

/// <internalonly/>
char IConvertible.ToChar(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "Char"));
}

/// <internalonly/>
sbyte IConvertible.ToSByte(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "SByte"));
}

/// <internalonly/>
byte IConvertible.ToByte(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "Byte"));
}

/// <internalonly/>
short IConvertible.ToInt16(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "Int16"));
}

/// <internalonly/>
ushort IConvertible.ToUInt16(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "UInt16"));
}

/// <internalonly/>
int IConvertible.ToInt32(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "Int32"));
}

/// <internalonly/>
uint IConvertible.ToUInt32(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "UInt32"));
}

/// <internalonly/>
int64 IConvertible.ToInt64(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "Int64"));
}

/// <internalonly/>
ulong IConvertible.ToUInt64(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "UInt64"));
}

/// <internalonly/>
float IConvertible.ToSingle(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "Single"));
}

/// <internalonly/>
double IConvertible.ToDouble(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "Double"));
}

/// <internalonly/>
Decimal IConvertible.ToDecimal(IFormatProvider provider) {
  InvalidCastException(GetResourceString("InvalidCast_FromTo", "DateTime", "Decimal"));
}

/// <internalonly/>
DateTime IConvertible.ToDateTime(IFormatProvider provider) {
  return this;
}

/// <internalonly/>
Object IConvertible.ToType(Type type, IFormatProvider provider) {
  return Convert.DefaultToType((IConvertible)this, type, provider);
}

// Tries to construct a DateTime from a given year, month, day, hour,
// minute, second and millisecond.
//
static Boolean TryCreate(int year, int month, int day, int hour, int minute, int second, int millisecond, out DateTime result) {
  result = DateTime.MinValue;
  if (year < 1 || year > 9999 || month < 1 || month > 12) {
    return false;
  }
  const int* days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
  if (day < 1 || day > days[month] - days[month - 1]) {
    return false;
  }                
  if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60 || second < 0 || second >= 60) {
    return false;
  }                
  if (millisecond < 0 || millisecond >= MillisPerSecond) {
    return false;
  }
  int64 ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
  
  ticks += millisecond * TicksPerMillisecond;
  if (ticks < MinTicks || ticks > MaxTicks) {
    return false;
  }
  result = new DateTime(ticks, DateTimeKind.Unspecified);
  return true;
}        

#endif


#endif // _DATETIME_INL_
