# c++中 string 转 __time64_t - writeeee的专栏 - CSDN博客
2015年08月20日 19:22:53[writeeee](https://me.csdn.net/writeeee)阅读数：1993
```
COleDateTime datetime;      
        datetime.ParseDateTime(startTime);
        struct tm tmTitmTime.tm_sec=datetime.GetSecond();
        tmTime.tm_min=datetime.GetMinute();
        tmTime.tm_hour=datetime.GetHour();
        tmTime.tm_mday=datetime.GetDay();
        tmTime.tm_mon=datetime.GetMonth();
        tmTime.tm_year=datetime.GetYear();
        __time64_t tmpStartTime= mktime(tmTime);
```
