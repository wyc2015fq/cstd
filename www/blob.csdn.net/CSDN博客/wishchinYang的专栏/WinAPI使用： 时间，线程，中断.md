# WinAPI使用： 时间，线程，中断 - wishchinYang的专栏 - CSDN博客
2013年12月25日 20:54:10[wishchin](https://me.csdn.net/wishchin)阅读数：638
个人分类：[C++编程](https://blog.csdn.net/wishchin/article/category/1508333)
（1）：C/C++获取当前系统时间：http://www.cnblogs.com/mfryf/archive/2012/02/13/2349360.html
不过当计算算法耗时的时候，不要忘记second，不能只要用Milliseconds来减，不然后出现负值，若是算法耗时太长就得用minutes啦。再不然，就hours……
//方案— 优点：仅使用C标准库；缺点：只能精确到秒级
#include <time.h> 
#include <stdio.h> 
int>
    char tmp[64]; 
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天 %z",localtime(&t) ); 
    puts( tmp ); 
    return 0; 
}
size_t strftime(char *strDest, size_t maxsize, const char *format, const struct tm *timeptr);
根据格式字符串生成字符串。
struct tm *localtime(const time_t *timer);
取得当地时间，localtime获取的结果由结构tm返回
返回的字符串可以依下列的格式而定：
%a 星期几的缩写。Eg:Tue 
%A 星期几的全名。 Eg: Tuesday
%b 月份名称的缩写。 
%B 月份名称的全名。 
%c 本地端日期时间较佳表示字符串。 
%d 用数字表示本月的第几天 (范围为 00 至 31)。日期
%H 用 24 小时制数字表示小时数 (范围为 00 至 23)。 
%I 用 12 小时制数字表示小时数 (范围为 01 至 12)。 
%j 以数字表示当年度的第几天 (范围为 001 至 366)。 
%m 月份的数字 (范围由 1 至 12)。
%M 分钟。 
%p 以 ''AM'' 或 ''PM'' 表示本地端时间。 
%S 秒数。 
%U 数字表示为本年度的第几周，第一个星期由第一个周日开始。 
%W 数字表示为本年度的第几周，第一个星期由第一个周一开始。 
%w 用数字表示本周的第几天 ( 0 为周日)。 
%x 不含时间的日期表示法。 
%X 不含日期的时间表示法。 Eg: 15:26:30
%y 二位数字表示年份 (范围由 00 至 99)。 
%Y 完整的年份数字表示，即四位数。 Eg:2008
%Z(%z) 时区或名称缩写。Eg:中国标准时间 
%% % 字符。
//方案二 优点：能精确到毫秒级；缺点：使用了win API 
#include <windows.h> 
#include <stdio.h> 
int main( void ) 
{ 
SYSTEMTIME sys; 
GetLocalTime( &sys ); 
printf( "%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek); 
return 0;
}
//方案三,优点：利用系统函数, 还能修改系统时间
//此文件必须是c++文件
#include<stdlib.h>
#include<iostream>
using namespace std;
void main()
{
    system("time");
}
//方案四,将当前时间折算为秒级，再通过相应的时间换算即可
//此文件必须是c++文件
#include<iostream>
#include<ctime>
using>
cout<<now_time;
return 0;
}
（2）：程序获取系统权限，占据单个线程：
（3）：C/C++ 直接使用中断：
