# C++取得系统时间 - sandro_zhang的专栏 - CSDN博客
2011年12月01日 12:46:51[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：1407
```cpp
string GetTimeStr(){
    char buffer [80] = {0};
    struct tm timeStruct;
    time_t localTime;
    localTime = time(NULL);
    localtime_s(&timeStruct, &localTime);
    strftime( buffer, 80, "%Y-%m-%d %X",&timeStruct);
    return string(buffer);
}
```
显示格式的参数如下：
|specifier|Replaced by|Example|
|----|----|----|
|%a|Abbreviated weekday name *|Thu|
|%A|Full weekday name *|Thursday|
|%b|Abbreviated month name *|Aug|
|%B|Full month name *|August|
|%c|Date and time representation *|Thu Aug 23 14:55:02 2001|
|%d|Day of the month (01-31)|23|
|%H|Hour in 24h format (00-23)|14|
|%I|Hour in 12h format (01-12)|02|
|%j|Day of the year (001-366)|235|
|%m|Month as a decimal number (01-12)|08|
|%M|Minute (00-59)|55|
|%p|AM or PM designation|PM|
|%S|Second (00-61)|02|
|%U|Week number with the first Sunday as the first day of week one (00-53)|33|
|%w|Weekday as a decimal number with Sunday as 0 (0-6)|4|
|%W|Week number with the first Monday as the first day of week one (00-53)|34|
|%x|Date representation *|08/23/01|
|%X|Time representation *|14:55:02|
|%y|Year, last two digits (00-99)|01|
|%Y|Year|2001|
|%Z|Timezone name or abbreviation|CDT|
|%%|A % sign|%|
参考网址：[http://www.cplusplus.com/reference/clibrary/ctime/strftime/](http://www.cplusplus.com/reference/clibrary/ctime/strftime/)
