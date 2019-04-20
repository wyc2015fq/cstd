# linux时间函数 - h13 - 博客园
inux下的时间函数
我们在编程中可能会经常用到时间，比如取得系统的时间（获取系统的年、月、日、时、分、秒，星期等），或者是隔一段时间去做某事，那么我们就用到一些时间函数。
linux下存储时间常见的有两种存储方式，一个是从1970年到现在经过了多少秒，一个是用一个结构来分别存储年月日时分秒的。
time_t 这种类型就是用来存储从1970年到现在经过了多少秒，要想更精确一点，可以用结构struct timeval，它精确到微妙。
structtimeval
{
longtv_sec; /*秒*/
longtv_usec; /*微秒*/
};
而直接存储年月日的是一个结构：
structtm
{
inttm_sec; /*秒，正常范围0-59， 
但允许至61*/
inttm_min; /*分钟，0-59*/
inttm_hour; /*小时， 0-23*/
inttm_mday; /*日，即一个月中的第几天，1-31*/
inttm_mon; /*月， 从一月算起，0-11*/
inttm_year; /*年， 从1900至今已经多少年*/
inttm_wday; /*星期，一周中的第几天， 从星期日算起，0-6*/
inttm_yday; /*从今年1月1日到目前的天数，范围0-365*/
inttm_isdst; /*日光节约时间的旗标*/
};
需要特别注意的是，年份是从1900年起至今多少年，而不是直接存储如2008年，月份从0开始的，0表示一月，星期也是从0开始的， 
0表示星期日，1表示星期一。
下面介绍一下我们常用的时间函数：
#include <time.h>
char *asctime(conststructtm* timeptr);
将结构中的信息转换为真实世界的时间，以字符串的形式显示
char *ctime(consttime_t *timep);
将timep转换为真是世界的时间，以字符串显示，它和asctime不同就在于传入的参数形式不一样
doubledifftime(time_ttime1, time_ttime2);
返回两个时间相差的秒数
intgettimeofday(structtimeval *tv, structtimezone *tz);
返回当前距离1970年的秒数和微妙数，后面的tz是时区，一般不用
structtm* gmtime(consttime_t *timep);
将time_t表示的时间转换为没有经过时区转换的UTC时间，是一个structtm结构指针
stucttm* localtime(consttime_t *timep);
和gmtime类似，但是它是经过时区转换的时间。
time_tmktime(structtm* timeptr);
将structtm 结构的时间转换为从1970年至今的秒数
time_ttime(time_t *t); 
取得从1970年1月1日至今的秒数。
上面是简单的介绍，下面通过实战来看看这些函数的用法：
下载: [gettime1.c](http://1d1a.com/wp-content/plugins/coolcode/coolcode.php?p=37&download=gettime1.c)
- /*gettime1.c*/
- #include <time.h>
- 
- intmain()
- {
- time_ttimep;
- 
- time(&timep); /*获取time_t类型的当前时间*/
- /*用gmtime将time_t类型的时间转换为struct tm类型的时间按，
- 然后再用asctime转换为我们常见的格式 Fri Jan 11 17:25:24 
2008
- */
- printf("%s", asctime(gmtime(&timep)));
- return0;
- }
编译并运行：
$gcc -o gettime1 gettime1.c
$./gettime1
Fri Jan 11 
17:04:08 2008
下面是直接把time_t类型的转换为我们常见的格式:
下载: [gettime2.c](http://1d1a.com/wp-content/plugins/coolcode/coolcode.php?p=37&download=gettime2.c)
- /* gettime2.c*/
- #include <time.h>
- 
- intmain()
- {
- time_ttimep;
- 
- time(&timep); /*获取time_t类型当前时间*/
- /*转换为常见的字符串：Fri 
Jan 11 17:04:08 2008*/
- printf("%s", ctime(&timep));
- return0;
- }
编译并运行：
$gcc -o gettime2 gettime2.c
$./gettime2
Sat Jan 12 
01:25:29 2008
我看了一本书上面说的这两个例子如果先后执行的话，两个的结果除了秒上有差别之外（执行程序需要时间），应该是一样的，可是我这里执行却发现差了很长时间按，一个是周五，一个是周六，后来我用 
date 命令执行了一遍
$date
六 1月 12 01:25:19 CST 2008
我发现date和gettime2比较一致， 我估计可能gettime1并没有经过时区的转换，它们是有差别的。
下载: [gettime3.c](http://1d1a.com/wp-content/plugins/coolcode/coolcode.php?p=37&download=gettime3.c)
- /*gettime3.c */
- #include <time.h>
- 
- intmain()
- {
- char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
- time_ttimep;
- structtm *p;
- 
- time(&timep); /*获得time_t结构的时间，UTC时间*/
- p = gmtime(&timep); /*转换为struct 
tm结构的UTC时间*/
- printf("%d/%d/%d 
", 
1900 + 
p->tm_year, 1 + p->tm_mon, p->tm_mday);
- printf("%s 
%d:%d:%d\n", wday[p->tm_wday], p->tm_hour, 
- p->tm_min, p->tm_sec);
- return0;
- }
编译并运行：
$gcc -o gettime3 gettime3.c
$./gettime3
2008/1/11 Fri 
17:42:54
从这个时间结果上来看，它和gettime1保持一致。
下载: [gettime4.c](http://1d1a.com/wp-content/plugins/coolcode/coolcode.php?p=37&download=gettime4.c)
- /*gettime4.c*/
- #include <time.h>
- 
- intmain()
- {
- char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
- time_ttimep;
- structtm *p;
- 
- time(&timep); /*获得time_t结构的时间，UTC时间*/
- p = localtime(&timep); /*转换为struct 
tm结构的当地时间*/
- printf("%d/%d/%d 
", 
1900 + 
p->tm_year, 1 + p->tm_mon, p->tm_mday);
- printf("%s 
%d:%d:%d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
- return0;
- }
编译并运行：
$gcc -o gettime4 gettime4.c
$./gettime4
2008/1/12 Sat 
1:49:29
从上面的结果我们可以这样说：
time, gmtime, asctime 
所表示的时间都是UTC时间，只是数据类型不一样，而
localtime, ctime 
所表示的时间都是经过时区转换后的时间，它和你用系统命令date所表示的CST时间应该保持一致。
下载: [gettime5.c](http://1d1a.com/wp-content/plugins/coolcode/coolcode.php?p=37&download=gettime5.c)
- /*gettime5.c*/
- #include <time.h>
- 
- intmain()
- {
- time_ttimep;
- structtm *p;
- 
- time(&timep); /*当前time_t类型UTC时间*/
- printf("time():%d\n",timep);
- p = localtime(&timep); /*转换为本地的tm结构的时间按*/
- timep = mktime(p); /*重新转换为time_t类型的UTC时间，这里有一个时区的转换*/
- printf("time()->localtime()->mktime(): %d\n", timep); 
- return0;
- }
编译并运行：
$gcc -o gettime5 
gettime5.c
$./gettime5
time():1200074913
time()->localtime()->mktime(): 
1200074913
这里面把UTC时间按转换为本地时间，然后再把本地时间转换为UTC时间，它们转换的结果保持一致。
下载: [gettime6.c](http://1d1a.com/wp-content/plugins/coolcode/coolcode.php?p=37&download=gettime6.c)
- /*gettime6.c */
- #include <time.h>
- 
- intmain()
- {
- time_ttimep;
- structtm *p;
- 
- time(&timep); /*得到time_t类型的UTC时间*/
- printf("time():%d\n",timep);
- p = gmtime(&timep); /*得到tm结构的UTC时间*/
- timep = mktime(p); /*转换，这里会有时区的转换*/
- printf("time()->gmtime()->mktime(): %d\n", timep); 
- return0;
- }
编译并运行：
$gcc -o gettime6 
gettime6.c
$./gettime6
time():1200075192
time()->gmtime()->mktime(): 
1200046392
从这里面我们可以看出，转换后时间不一致了，计算一下，整整差了8个小时( (1200075192-1200046392)/3600 = 8 
)，说明mktime会把本地时间转换为UTC时间，这里面本来就是UTC时间，于是再弄个时区转换，结果差了8个小时，用的时候应该注意。

