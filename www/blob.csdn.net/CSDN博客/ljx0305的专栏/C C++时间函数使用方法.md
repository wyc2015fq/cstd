# C/C++时间函数使用方法 - ljx0305的专栏 - CSDN博客
2009年06月22日 09:03:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：535标签：[日历																[struct																[calendar																[timer																[microsoft																[loops](https://so.csdn.net/so/search/s.do?q=loops&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=calendar&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=日历&t=blog)
个人分类：[C++																[C																[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/380566)
1. 时间的定义
C/C++对时间的操作也有许多值得大家注意的地方。最近，在技术群中有很多网友也多次问到过C++语言中对时间的操作、获取和显示等等的问题。下面，在这篇文章中，笔者将主要介绍在C/C++中时间和日期的使用方法. 
         通过学习许多C/C++库，你可以有很多操作、使用时间的方法。但在这之前你需要了解一些“时间”和“日期”的概念，主要有以下几个： 
         Coordinated Universal Time（UTC）：协调世界时，又称为世界标准时间，也就是大家所熟知的格林威治标准时间（Greenwich Mean Time，GMT）。比如，中国内地的时间与UTC的时差为+8，也就是UTC+8。美国是UTC-5。 
         Calendar Time：日历时间，是用“从一个标准时间点到此时的时间经过的秒数”来表示的时间。这个标准时间点对不同的编译器来说会有所不同，但对一个编译系统来说，这个标准时间点是不变的，该编译系统中的时间对应的日历时间都通过该标准时间点来衡量，所以可以说日历时间是“相对时间”，但是无论你在哪一个时区，在同一时刻对同一个标准时间点来说，日历时间都是一样的。 
         epoch：时间点。时间点在标准C/C++中是一个整数，它用此时的时间和标准时间点相差的秒数（即日历时间）来表示。 
         clock tick：时钟计时单元（而不把它叫做时钟滴答次数），一个时钟计时单元的时间长短是由CPU控制的。一个clock tick不是CPU的一个时钟周期，而是C/C++的一个基本计时单位。 
        我们可以使用ANSI标准库中的time.h头文件。这个头文件中定义的时间和日期所使用的方法，无论是在结构定义，还是命名，都具有明显的C语言风格。下面，我将说明在C/C++中怎样使用日期的时间功能。 
2． 计时 
         C/C++中的计时函数是clock()，而与其相关的数据类型是clock_t。在MSDN中，查得对clock函数定义如下： 
clock_t clock( void ); 
         这个函数返回从“开启这个程序进程”到“程序中调用clock()函数”时之间的CPU时钟计时单元（clock tick）数，在MSDN中称之为挂钟时间（wal-clock）。其中clock_t是用来保存时间的数据类型，在time.h文件中，我们可以找到对它的定义： 
#ifndef _CLOCK_T_DEFINED 
typedef long clock_t; 
#define _CLOCK_T_DEFINED 
#endif 
         很明显，clock_t是一个长整形数。在time.h文件中，还定义了一个常量CLOCKS_PER_SEC，它用来表示一秒钟会有多少个时钟计时单元，其定义如下： 
#define CLOCKS_PER_SEC ((clock_t)1000) 
         可以看到每过千分之一秒（1毫秒），调用clock（）函数返回的值就加1。下面举个例子，你可以使用公式clock()/CLOCKS_PER_SEC来计算一个进程自身的运行时间： 
void elapsed_time() 
{ 
     printf("Elapsed time:%u secs./n",clock()/CLOCKS_PER_SEC); 
} 
         当然，你也可以用clock函数来计算你的机器运行一个循环或者处理其它事件到底花了多少时间： 
#include “stdio.h” 
#include “stdlib.h” 
#include “time.h” 
int main( void ) 
{ 
    long     i = 10000000L; 
    clock_t start, finish; 
    double   duration; 
    /* 测量一个事件持续的时间*/ 
    printf( "Time to do %ld empty loops is ", i ); 
    start = clock(); 
    while( i-- )       ; 
    finish = clock(); 
    duration = (double)(finish - start) / CLOCKS_PER_SEC; 
    printf( "%f seconds/n", duration ); 
    system("pause"); 
} 
在笔者的机器上，运行结果如下： 
Time to do 10000000 empty loops is 0.03000 seconds 
         上面我们看到时钟计时单元的长度为1毫秒，那么计时的精度也为1毫秒，那么我们可不可以通过改变CLOCKS_PER_SEC的定义，通过把它定义的大一些，从而使计时精度更高呢？通过尝试，你会发现这样是不行的。在标准C/C++中，最小的计时单位是一毫秒。 
3．与日期和时间相关的数据结构 
         在标准C/C++中，我们可通过tm结构来获得日期和时间，tm结构在time.h中的定义如下： 
#ifndef _TM_DEFINED 
struct tm { 
         int tm_sec;      /* 秒 – 取值区间为[0,59] */ 
         int tm_min;      /* 分 - 取值区间为[0,59] */ 
         int tm_hour;     /* 时 - 取值区间为[0,23] */ 
         int tm_mday;     /* 一个月中的日期 - 取值区间为[1,31] */ 
         int tm_mon;      /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */ 
         int tm_year;     /* 年份，其值等于实际年份减去1900 */ 
         int tm_wday;     /* 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此推 */int tm_yday;     /* 从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 */ 
         int tm_isdst;    /* 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。*/ 
         }; 
#define _TM_DEFINED 
#endif 
         ANSI C标准称使用tm结构的这种时间表示为分解时间(broken-down time)。 
         而日历时间（Calendar Time）是通过time_t数据类型来表示的，用time_t表示的时间（日历时间）是从一个时间点（例如：1970年1月1日0时0分0秒）到此时的秒数。在time.h中，我们也可以看到time_t是一个长整型数： 
#ifndef _TIME_T_DEFINED 
typedef long time_t;          /* 时间值 */ 
#define _TIME_T_DEFINED       /* 避免重复定义 time_t */ 
#endif 
         大家可能会产生疑问：既然time_t实际上是长整型，到未来的某一天，从一个时间点（一般是1970年1月1日0时0分0秒）到那时的秒数（即日历时间）超出了长整形所能表示的数的范围怎么办？对time_t数据类型的值来说，它所表示的时间不能晚于2038年1月18日19时14分07秒。为了能够表示更久远的时间，一些编译器厂商引入了64位甚至更长的整形数来保存日历时间。比如微软在Visual C++中采用了__time64_t数据类型来保存日历时间，并通过_time64()函数来获得日历时间（而不是通过使用32位字的time()函数），这样就可以通过该数据类型保存3001年1月1日0时0分0秒（不包括该时间点）之前的时间。 
         在time.h头文件中，我们还可以看到一些函数，它们都是以time_t为参数类型或返回值类型的函数： 
double difftime(time_t time1, time_t time0); 
time_t mktime(struct tm * timeptr); 
time_t time(time_t * timer); 
char * asctime(const struct tm * timeptr); 
char * ctime(const time_t *timer); 
         此外，time.h还提供了两种不同的函数将日历时间（一个用time_t表示的整数）转换为我们平时看到的把年月日时分秒分开显示的时间格式tm： 
struct tm * gmtime(const time_t *timer); 
struct tm * localtime(const time_t * timer); 
         通过查阅MSDN，我们可以知道Microsoft C/C++ 7.0中时间点的值（time_t对象的值）是从1899年12月31日0时0分0秒到该时间点所经过的秒数，而其它各种版本的Microsoft C/C++和所有不同版本的Visual C++都是计算的从1970年1月1日0时0分0秒到该时间点所经过的秒数。 
4．与日期和时间相关的函数及应用 
         在本节，我将向大家展示怎样利用time.h中声明的函数对时间进行操作。这些操作包括取当前时间、计算时间间隔、以不同的形式显示时间等内容。 
4.1 获得日历时间 
         我们可以通过time()函数来获得日历时间（Calendar Time），其原型为： 
time_t time(time_t * timer); 
         如果你已经声明了参数timer，你可以从参数timer返回现在的日历时间，同时也可以通过返回值返回现在的日历时间，即从一个时间点（例如：1970年1月1日0时0分0秒）到现在此时的秒数。如果参数为空（NUL），函数将只通过返回值返回现在的日历时间，比如下面这个例子用来显示当前的日历时间： 
#include "time.h" 
#include "stdio.h" 
int main(void) 
{ 
     struct tm *ptr; 
     time_t lt; 
     lt =time(NUL); 
     printf("The Calendar Time now is %d/n",lt); 
     return 0; 
} 
运行的结果与当时的时间有关，我当时运行的结果是： 
The Calendar Time now is 1122707619 
         其中1122707619就是我运行程序时的日历时间。即从1970年1月1日0时0分0秒到此时的秒数。 
4.2 获得日期和时间 
         这里说的日期和时间就是我们平时所说的年、月、日、时、分、秒等信息。从第2节我们已经知道这些信息都保存在一个名为tm的结构体中，那么如何将一个日历时间保存为一个tm结构的对象呢？ 
其中可以使用的函数是gmtime()和localtime()，这两个函数的原型为： 
struct tm * gmtime(const time_t *timer);                                           
struct tm * localtime(const time_t * timer); 
         其中gmtime()函数是将日历时间转化为世界标准时间（即格林尼治时间），并返回一个tm结构体来保存这个时间，而localtime()函数是将日历时间转化为本地时间。比如现在用gmtime()函数获得的世界标准时间是2005年7月30日7点18分20秒，那么我用localtime()函数在中国地区获得的本地时间会比世界标准时间晚8个小时，即2005年7月30日15点18分20秒。下面是个例子： 
#include "time.h" 
#include "stdio.h" 
int main(void) 
{ 
     struct tm *local; 
     time_t t; 
     t=time(NUL); 
     local=localtime(&t); 
     printf("Local hour is: %d/n",local->tm_hour); 
     local=gmtime(&t); 
     printf("UTC hour is: %d/n",local->tm_hour); 
     return 0; 
} 
运行结果是： 
Local hour is: 15 
UTC hour is: 7 
4.3 固定的时间格式 
         我们可以通过asctime()函数和ctime()函数将时间以固定的格式显示出来，两者的返回值都是char*型的字符串。返回的时间格式为： 
星期几 月份 日期 时:分:秒 年/n/0 
例如：Wed Jan 02 02:03:55 1980/n/0 
         其中/n是一个换行符，/0是一个空字符，表示字符串结束。下面是两个函数的原型： 
char * asctime(const struct tm * timeptr); 
char * ctime(const time_t *timer); 
         其中asctime()函数是通过tm结构来生成具有固定格式的保存时间信息的字符串，而ctime()是通过日历时间来生成时间字符串。这样的话，asctime（）函数只是把tm结构对象中的各个域填到时间字符串的相应位置就行了，而ctime（）函数需要先参照本地的时间设置，把日历时间转化为本地时间，然后再生成格式化后的字符串。在下面，如果t是一个非空的time_t变量的话，那么： 
printf(ctime(&t)); 
等价于： 
struct tm *ptr; 
ptr=localtime(&t); 
printf(asctime(ptr)); 
         那么，下面这个程序的两条printf语句输出的结果就是不同的了（除非你将本地时区设为世界标准时间所在的时区）： 
#include "time.h" 
#include "stdio.h" 
int main(void) 
{ 
     struct tm *ptr; 
     time_t lt; 
     lt =time(NUL); 
     ptr=gmtime(<); 
     printf(asctime(ptr)); 
     printf(ctime(<)); 
     return 0; 
} 
运行结果： 
Sat Jul 30 08:43:03 2005 
Sat Jul 30 16:43:03 2005 
4.4 自定义时间格式 
         我们可以使用strftime（）函数将时间格式化为我们想要的格式。它的原型如下： 
size_t strftime( 
    char *strDest, 
    size_t maxsize，
const char *format, 
    const struct tm *timeptr  
); 
         我们可以根据format指向字符串中格式命令把timeptr中保存的时间信息放在strDest指向的字符串中，最多向strDest中存放maxsize个字符。该函数返回向strDest指向的字符串中放置的字符数。 
         函数strftime()的操作有些类似于sprintf()：识别以百分号(%)开始的格式命令集合，格式化输出结果放在一个字符串中。格式化命令说明串strDest中各种日期和时间信息的确切表示方法。格式串中的其他字符原样放进串中。格式命令列在下面，它们是区分大小写的。 
%a 星期几的简写  
%A 星期几的全称  
%b 月分的简写  
%B 月份的全称  
%c 标准的日期的时间串  
%C 年份的后两位数字  
%d 十进制表示的每月的第几天  
%D 月/天/年  
%e 在两字符域中，十进制表示的每月的第几天  
%F 年-月-日  
%g 年份的后两位数字，使用基于周的年  
%G 年分，使用基于周的年  
%h 简写的月份名  
%H 24小时制的小时  
%I 12小时制的小时 
%j 十进制表示的每年的第几天  
%m 十进制表示的月份  
%M 十时制表示的分钟数  
%n 新行符  
%p 本地的AM或PM的等价显示  
%r 12小时的时间  
%R 显示小时和分钟：hh:mm  
%S 十进制的秒数  
%t 水平制表符  
%T 显示时分秒：hh:mm:ss  
%u 每周的第几天，星期一为第一天 （值从0到6，星期一为0） 
%U 第年的第几周，把星期日做为第一天（值从0到53） 
%V 每年的第几周，使用基于周的年  
%w 十进制表示的星期几（值从0到6，星期天为0） 
%W 每年的第几周，把星期一做为第一天（值从0到53）  
%x 标准的日期串  
%X 标准的时间串  
%y 不带世纪的十进制年份（值从0到99） 
%Y 带世纪部分的十进制年份  
%z，%Z 时区名称，如果不能得到时区名称则返回空字符。 
%% 百分号 
如果想显示现在是几点了，并以12小时制显示，就象下面这段程序： 
#include “time.h” 
#include “stdio.h” 
int main(void) 
{ 
     struct tm *ptr; 
     time_t lt; 
     char str[80]; 
     lt=time(NUL); 
     ptr=localtime(<); 
     strftime(str,100,"It is now %I %p",ptr); 
     printf(str); 
     return 0; 
} 
其运行结果为： 
It is now 4PM 
而下面的程序则显示当前的完整日期： 
#include <stdio.h> 
#include <time.h> 
void main( void ) 
{ 
         struct tm *newtime; 
         char tmpbuf[128]; 
         time_t lt1; 
         time( <1 ); 
         newtime=localtime(<1); 
         strftime( tmpbuf, 128, "Today is %A, day %d of %B in the year %Y./n", newtime); 
         printf(tmpbuf); 
} 
运行结果： 
Today is Saturday, day 30 of July in the year 2005. 
4.5 计算持续时间的长度 
         有时候在实际应用中要计算一个事件持续的时间长度，比如计算打字速度。在第1节计时部分中，我已经用clock函数举了一个例子。Clock()函数可以精确到毫秒级。同时，我们也可以使用difftime()函数，但它只能精确到秒。该函数的定义如下： 
double difftime(time_t time1, time_t time0); 
         虽然该函数返回的以秒计算的时间间隔是double类型的，但这并不说明该时间具有同double一样的精确度，这是由它的参数觉得的（time_t是以秒为单位计算的）。比如下面一段程序： 
#include "time.h" 
#include "stdio.h" 
#include "stdlib.h" 
int main(void) 
{ 
     time_t start,end; 
     start = time(NUL); 
     system("pause"); 
     end = time(NUL); 
     printf("The pause used %f seconds./n",difftime(end,start));//<- 
     system("pause"); 
     return 0; 
} 
运行结果为： 
请按任意键继续. . . 
The pause used 2.000000 seconds. 
请按任意键继续. . . 
         可以想像，暂停的时间并不那么巧是整整2秒钟。其实，你将上面程序的带有“//<-”注释的一行用下面的一行代码替换： 
printf("The pause used %f seconds./n",end-start); 
其运行结果是一样的。 
4.6 分解时间转化为日历时间 
         这里说的分解时间就是以年、月、日、时、分、秒等分量保存的时间结构，在C/C++中是tm结构。我们可以使用mktime（）函数将用tm结构表示的时间转化为日历时间。其函数原型如下： 
time_t mktime(struct tm * timeptr); 
其返回值就是转化后的日历时间。这样我们就可以先制定一个分解时间，然后对这个时间进行操作了，下面的例子可以计算出1997年7月1日是星期几： 
#include "time.h" 
#include "stdio.h" 
#include "stdlib.h" 
int main(void) 
{ 
     struct tm t; 
     time_t t_of_day; 
     t.tm_year=1997-1900; 
     t.tm_mon=6; 
     t.tm_mday=1; 
     t.tm_hour=0; 
     t.tm_min=0; 
     t.tm_sec=1; 
     t.tm_isdst=0; 
     t_of_day=mktime(&t); 
     printf(ctime(&t_of_day)); 
     return 0; 
} 
运行结果： 
Tue Jul 01 00:00:01 1997 
         现在注意了，有了mktime()函数，是不是我们可以操作现在之前的任何时间呢？你可以通过这种办法算出1945年8月15号是星期几吗？答案是否定的。因为这个时间在1970年1月1日之前，所以在大多数编译器中，这样的程序虽然可以编译通过，但运行时会异常终止。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wuhuiran/archive/2008/01/14/2043147.aspx](http://blog.csdn.net/wuhuiran/archive/2008/01/14/2043147.aspx)
