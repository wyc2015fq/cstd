# C++ tm 和 time_t 有关总结 - 建建的博客 - CSDN博客
2017年05月24日 17:46:50[纪建](https://me.csdn.net/u013898698)阅读数：583
【分解时间 & 日历时间】
在标准C/C++中，我们可通过tm结构来获得日期和时间，struct tm结构体在time.h中的定义如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- #ifndef_TM_DEFINED  
- structtm {    
- int tm_sec; /* 秒 – 取值区间为[0,59] */
- int tm_min; /* 分 - 取值区间为[0,59] */
- int tm_hour; /* 时 - 取值区间为[0,23] */
- int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */
- int tm_mon; /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
- int tm_year; /* 年份，其值等于实际年份减去1900 */
- int tm_wday; /* 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */
- int tm_yday; /* 从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 */
- int tm_isdst; /* 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。*/
- };    
- #define_TM_DEFINED  
- #endif  
![](http://static.blog.csdn.net/images/save_snippets.png)
ANSI C标准称使用 tm 结构的这种时间表示为分解时间(broken-down time)。
而日历时间（Calendar Time）是通过time_t数据类型来表示的，用time_t表示的时间（日历时间）是从一个时间点（例如：1970年1月1日0时0分0秒）到此时的秒数。在time.h中，我们也可以看到time_t是一个长整型数：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- #ifndef _TIME_T_DEFINED
- typedeflongtime_t;    /* 时间值 */
- #define _TIME_T_DEFINED
- #endif
        那么问题来了：既然time_t实际上是长整型，到未来的某一天，从一个时间点（一般是1970年1月1日0时0分0秒）到那时的秒数（即日历时间）超出了长整形所能表示的数的范围怎么办？对time_t数据类型的值来说，它所表示的时间不能晚于2038年1月18日19时14分07秒。
        为了能够表示更久远的时间，一些编译器厂商引入了64位甚至更长的整形数来保存日历时间。比如微软在Visual C++中采用了__time64_t数据类型来保存日历时间，并通过_time64()函数来获得日历时间（而不是通过使用32位字的time()函数），这样就可以通过该数据类型保存3001年1月1日0时0分0秒（不包括该时间点）之前的时间。
【常用函数】
在time.h头文件中，我们还可以看到一些函数，它们都是以time_t为参数类型或返回值类型的函数：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- double difftime(time_t time1, time_t time0);  
- time_t mktime(structtm * timeptr);  
- time_t time(time_t * timer);  
- char * asctime(conststructtm * timeptr);  
- char * ctime(consttime_t *timer);  
此外，time.h还提供了两种不同的函数，将日历时间转换为我们平时看到的时间格式（年月日时分秒分开显示的）
 tm：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- structtm * gmtime(consttime_t *timer);  
- structtm * localtime(consttime_t * timer);  
其中gmtime()是将日历时间转化为世界标准时间（即格林尼治时间），并返回一个tm结构体来保存这个时间。而localtime()是将日历时间转化为本地时间。比如：用gmtime()函数获得的世界标准时间是2016年7月30日7点18分20秒，那么用localtime()函数在中国地区获得的本地时间会比时间标准时间晚8个小时，即2016年7月30日15点18分20秒。示例代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- #include "time.h"
- #include "stdio.h"
- 
- int main(void)  
- {  
- time_t t = time(NULL);//参数为NULL,返回现在的日历时间
- structtm *local = localtime(&t);  
-     printf("Local hour is: %d\n",local->tm_hour);  
- 
-     local = gmtime(&t);  
-     printf("UTC hour is: %d\n",local->tm_hour);  
- 
- return 0;  
- }  
运行结果：
Local hour is: 15
UTC hour is: 7
【固定的时间格式】
可以通过asctime()函数和ctime()函数将时间以固定的格式显示出来，两者的返回值都是char*型的字符串。返回的时间格式为：
星期几 月份 日期 时:分:秒 年\n\0
例如：Wed Jan 02 02:03:55 1980\n\0
函数原型如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- char * asctime(conststructtm * timeptr);  
- char * ctime(consttime_t *timer);  
其中asctime()函数，是通过tm结构来生成具有固定格式的保存时间信息的字符串，而ctime()是通过日历时间来生成时间字符串。这样的话，asctime（）函数只是把tm结构对象中的各个域填到时间字符串的相应位置就行了，而ctime（）函数需要先参照本地的时间设置，把日历时间转化为本地时间，然后再生成格式化后的字符串。示例代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- #include "time.h"
- #include "stdio.h"
- 
- int main(void)  
- {  
- time_t tt = time(NULL);  
- structtm *ptr = gmtime(&tt);  
-     printf(asctime(ptr));  
-     printf(ctime(&tt));  
- 
- return 0;  
- }  
运行结果：
Tue Aug 16 07:21:59 2016
Tue Aug 16 15:21:59 2016
【自定义时间格式】
我们可以使用strftime()函数将时间格式化为我们想要的格式。它的原型如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- size_t strftime(  
- char *strDest,  
- size_t maxsize,  
- constchar *format,  
- conststructtm *timeptr   
-                 );  
可以根据format指向字符串中格式命令，把timeptr中保存的时间信息放在strDest指向的字符串中，最多向strDest中存放maxsize个字符。该函数返回向strDest指向的字符串中放置的字符个数。格式命令列如下，它们是区分大小写的！！
**[plain]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- %a 星期几的简写   
- %A 星期几的全称   
- %b 月分的简写   
- %B 月份的全称   
- %c 标准的日期的时间串   
- %C 年份的后两位数字   
- %d 十进制表示的每月的第几天   
- %D 月/天/年   
- %e 在两字符域中，十进制表示的每月的第几天   
- %F 年-月-日   
- %g 年份的后两位数字，使用基于周的年   
- %G 年分，使用基于周的年   
- %h 简写的月份名   
- %H 24小时制的小时   
- %I 12小时制的小时  
- %j 十进制表示的每年的第几天   
- %m 十进制表示的月份   
- %M 十时制表示的分钟数   
- %n 新行符   
- %p 本地的AM或PM的等价显示   
- %r 12小时的时间   
- %R 显示小时和分钟：hh:mm   
- %S 十进制的秒数   
- %t 水平制表符   
- %T 显示时分秒：hh:mm:ss   
- %u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）  
- %U 第年的第几周，把星期日做为第一天（值从0到53）  
- %V 每年的第几周，使用基于周的年   
- %w 十进制表示的星期几（值从0到6，星期天为0）  
- %W 每年的第几周，把星期一做为第一天（值从0到53）   
- %x 标准的日期串   
- %X 标准的时间串   
- %y 不带世纪的十进制年份（值从0到99）  
- %Y 带世纪部分的十制年份   
- %z，%Z 时区名称，如果不能得到时区名称则返回空字符。  
- %% 百分号  
strftime 的示例代码：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- #include "time.h"
- #include "stdio.h"
- 
- int main(void)  
- {  
- //显示现在是几点了，并以12小时制显示
- char str[80];  
- time_t _t = time(NULL);  
- structtm *ptm = localtime(&_t);  
-     strftime(str, 100, "It is now %I %p\n",ptm);  
-     printf(str);  
- 
- //显示当前的完整日期
- char tmpbuf[128];     
- time_t It = time( NULL );  
- structtm *newtime = localtime(&It);  
-     strftime(tmpbuf, 128, "Today is %A, day %d of %B in the year %Y.\n", newtime);  
-     printf(tmpbuf);  
- 
- return 0;  
- }  
输出结果：
It is now 03 PM
Today is Tuesday, day 16 of August in the year 2016.
【应用举例】
使用tm和time_t，计算两个日期之间的间隔天数：
**[cpp]**[view
 plain](http://blog.csdn.net/hellokandy/article/details/52218092#)[copy](http://blog.csdn.net/hellokandy/article/details/52218092#)
[print](http://blog.csdn.net/hellokandy/article/details/52218092#)[?](http://blog.csdn.net/hellokandy/article/details/52218092#)
- #include <stdio.h>
- #include <time.h>
- #include <cstdlib>
- 
- class DaysCounter  
- {  
- public:  
-     DaysCounter(){}  
-     ~DaysCounter(){}  
- 
- staticint get_date_span(int date1, int date2)  
-     {  
- time_t t_of_day1 = time_maker(date1);  
- time_t t_of_day2 = time_maker(date2);  
- double diff = difftime(t_of_day1, t_of_day2);  
- if(diff < 0)  
-             diff = 0-diff;  
- 
- int count_gap_day = (int)diff/86400;//60*60*24=86400  
- return count_gap_day;  
-     }  
- 
- statictime_t time_maker(int date)  
-     {  
- /*struct*/tm _tm;  
-         _tm.tm_year = date/10000-1900;  
-         _tm.tm_mon = (date-(_tm.tm_year+1900)*10000)/100-1;  
-         _tm.tm_mday = date-(_tm.tm_year+1900)*10000-(_tm.tm_mon+1)*100;  
-         _tm.tm_hour = 0;  
-         _tm.tm_min = 0;  
-         _tm.tm_sec = 1;  
-         _tm.tm_isdst = 0;  
- 
- return mktime(&_tm);  
-     }  
- 
- int current_time(time_t& current_time)  
-     {  
- time_t _t = time(NULL);  
- structtm * timeinfo = localtime(&_t);  
- int year = timeinfo->tm_year + 1900;  
- int month = timeinfo->tm_mon + 1;  
- int day = timeinfo->tm_mday;  
- int date_current = year*10000 + month*100 + day;  
- return date_current;  
-     }     
- };  
- 
- int main()  
-  {  
- int days = DaysCounter::get_date_span(20150810, 20150815);  
-     printf("days : %d\n", days);  
- 
-     system("pause");  
- return 0;  
-  }  
