# C++ Get Current Time 获取当前时间 - Grandyang - 博客园







# [C++ Get Current Time 获取当前时间](https://www.cnblogs.com/grandyang/p/4302489.html)







在做项目中，我们经常需要获取系统的当前时间，那么如何获取呢，参见下面的代码：



```
/* asctime example */
#include <stdio.h>      /* printf */
#include <time.h>       /* time_t, struct tm, time, localtime, asctime */

int main ()
{
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf ( "The current date/time is: %s", asctime (timeinfo) );

  return 0;
}
```



输出结果为：

```
The current date/time is:: Thu Feb 26 11:40:19 2015
```



但是，有些情况下我们想分别获得年月日时分秒的值，而不是像这样得到一个整个的字符串，那么既然时间信息都存在了一个结构体timeinfo中，那么我们就先来看看这个结构体的定义吧：



```
struct tm {
  int tm_sec;   // seconds of minutes from 0 to 61
  int tm_min;   // minutes of hour from 0 to 59
  int tm_hour;  // hours of day from 0 to 24
  int tm_mday;  // day of month from 1 to 31
  int tm_mon;   // month of year from 0 to 11
  int tm_year;  // year since 1900
  int tm_wday;  // days since sunday
  int tm_yday;  // days since January 1st
  int tm_isdst; // hours of daylight savings time
}
```



好了，看完了结构体的定义，我们再来看看asctime()函数是如何取出想要的数据，并加以转换的：



```
char* asctime(const struct tm *timeptr)
{
  static const char wday_name[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  };
  static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[26];
  sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
    wday_name[timeptr->tm_wday],
    mon_name[timeptr->tm_mon],
    timeptr->tm_mday, timeptr->tm_hour,
    timeptr->tm_min, timeptr->tm_sec,
    1900 + timeptr->tm_year);
  return result;
}
```



如果我们直接将timeinfo结构体中的年月日时分秒打印出来，会得到：

```
115 1 26 11 46 37
```



而我们需要的是：

```
2015 2 26 11 46 37
```



仔细观察asctime()函数，我们可以发现年的保存方式是自1900年之后，所以要+1900，月是把名称简写存入了一个数组，数组从0开始，所以要得到月的数字，需要+1，后面的日时分秒没有变化，可以直接使用。



若想获得类似20150226114637这样的年月日时分秒的字符串，可以用下面的函数：



```
std::string getTimeStamp() {
    time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    char year[5], mon[3], mday[3], hour[3], minute[3], sec[3];
    sprintf(year, "%d", timeinfo->tm_year + 1900);
    sprintf(mon, "%d", timeinfo->tm_mon + 1);
    sprintf(mday, "%d", timeinfo->tm_mday);
    sprintf(hour, "%d", timeinfo->tm_hour);
    sprintf(minute, "%d", timeinfo->tm_min);
    sprintf(sec, "%d", timeinfo->tm_sec);

    std::string yearStr = std::string(year);
    std::string monStr = std::string(mon);
    if (monStr.size() == 1) monStr = "0" + monStr;
    std::string mdayStr = std::string(mday);
    if (mdayStr.size() == 1) mdayStr = "0" + mdayStr;
    std::string hourStr = std::string(hour);
    if (hourStr.size() == 1) hourStr = "0" + hourStr;
    std::string minuteStr = std::string(minute);
    if (minuteStr.size() == 1) minuteStr = "0" + minuteStr;
    std::string secStr = std::string(sec);
    if (secStr.size() == 1) secStr = "0" + secStr;
    return yearStr + monStr + mdayStr +\
        hourStr + minuteStr + secStr;
}
```





另外，如果编译器支持C++11的话，也可以使用std::chrono来操作时间，可参见下列实例代码：

```
#include <iostream>
#include <chrono>
#include <ctime>

long fibonacci(int n)
{
    if (n < 3) return 1;
    return fibonacci(n-1) + fibonacci(n-2);
}

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    std::cout << "f(42) = " << fibonacci(42) << '\n';
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
```














