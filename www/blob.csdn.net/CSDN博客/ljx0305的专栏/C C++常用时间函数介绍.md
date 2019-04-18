# C/C++常用时间函数介绍 - ljx0305的专栏 - CSDN博客
2013年05月09日 10:49:25[ljx0305](https://me.csdn.net/ljx0305)阅读数：3337
在介绍之前，先介绍两个概念
**Coordinated Universal Time（UTC）：**协调世界时，又称为世界标准时间，也就是大家所熟知的格林威治标准时间（Greenwich Mean Time，GMT）。比如，中国内地的时间与UTC的时差为+8，也就是UTC+8。美国是UTC-5。
**Calendar Time：**日历时间，是用“从一个标准时间点到此时的时间经过的秒数”来表示的时间。这个标准时间点对不同的编译器来说会有所不同，但对一个编译系统来说，这个标准时间点是不变的，该编译系统中的时间对应的日历时间都通过该标准时间点来衡量，所以可以说日历时间是“相对时间”，但是无论你在哪一个时区，在同一时刻对同一个标准时间点来说，日历时间都是一样的。（简单点说，可以理解为CT时间就是UTC时间减去1900-01-01 00:00:00）
与日期和时间相关的数据结构
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- typedef**long****time_t**;         /* 时间值 */
```cpp
typedef long time_t;         /* 时间值 */
```
    大家可能会产生疑问：既然time_t实际上是长整型，到未来的某一天，从一个时间点（一般是1970年1月1日0时0分0秒）到那时的秒数（即日历时间）超出了长整形所能表示的数的范围怎么办？对time_t数据类型的值来说，它所表示的时间不能晚于2038年1月18日19时14分07秒。为了能够表示更久远的时间，一些编译器厂商引入了64位甚至更长的整形数来保存日历时间。比如微软在Visual C++中采用了__time64_t数据类型来保存日历时间，并通过_time64()函数来获得日历时间（而不是通过使用32位字的time()函数），这样就可以通过该数据类型保存3001年1月1日0时0分0秒（不包括该时间点）之前的时间。
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- struct**tm**
- {    
- **int** tm_sec; /* 秒 - 取值区间为[0,59] */
- **int** tm_min; /* 分 - 取值区间为[0,59] */
- **int** tm_hour; /* 时 - 取值区间为[0,23] */
- **int** tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */ i   
-     nt tm_mon; /* 月份(从一月开始，0代表一月)- 取值区间为[0,11] */
- **int** tm_year; /* 年份，其值等于实际年份减去1900 */
- **int** tm_wday; /* 星期–取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */
- **int** tm_yday; /* 从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 */
- **int** tm_isdst; /*夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。
-                     夏令时可以在网上找到答案，在中国，已经不再使用夏令时*/
- };  
```cpp
struct tm 
{ 
    int tm_sec; /* 秒 - 取值区间为[0,59] */ 
    int tm_min; /* 分 - 取值区间为[0,59] */ 
    int tm_hour; /* 时 - 取值区间为[0,23] */ 
    int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */ i
    nt tm_mon; /* 月份(从一月开始，0代表一月)- 取值区间为[0,11] */ 
    int tm_year; /* 年份，其值等于实际年份减去1900 */  
    int tm_wday; /* 星期–取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */ 
    int tm_yday; /* 从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 */ 
    int tm_isdst; /*夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。
                    夏令时可以在网上找到答案，在中国，已经不再使用夏令时*/ 
};
```
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- typedef**long****clock_t**;     从"开启这个程序进程"到"程序中调用clock()函数"时之间的CPU时钟计时单元（clock tick）数  
```cpp
typedef long clock_t;     从"开启这个程序进程"到"程序中调用clock()函数"时之间的CPU时钟计时单元（clock tick）数
```
**clock（取得CPU时钟计时单元（clock tick）数）**函数定义 
clock_t clock( void )
函数说明 
函数返回从"开启这个程序进程"到"程序中调用clock()函数"时之间的CPU时钟计时单元（clock tick）数。其中clock_t是用来保存时间的数据类型
返回值 
从"开启这个程序进程"到"程序中调用clock()函数"时之间的CPU时钟计时单元（clock tick）数
在time.h文件中，定义了一个常量CLOCKS_PER_SEC，它用来表示一秒钟会有多少个时钟计时单元，其定义如下：
#define CLOCKS_PER_SEC ((clock_t)1000)
可以看到每过千分之一秒（1毫秒），调用clock（）函数返回的值就加1。
范例：
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- #include "stdio.h" 
- #include "time.h" 
- 
- **int** main( void )   
- {   
- **long**    i = 10000000L;   
- **clock_t** start, end;   
- **double**  timeUsed;   
-    start = clock();   
- while( i-- )      ;   
-    end = clock();   
-    timeUsed = (**double**)(end - start) / CLOCKS_PER_SEC;   
-    printf( "执行10000000次空循环用时%f秒\n", timeUsed );   
- }  
```cpp
#include "stdio.h"
#include "time.h"
int main( void )
{
   long    i = 10000000L;
   clock_t start, end;
   double  timeUsed;
   start = clock();
   while( i-- )      ;
   end = clock();
   timeUsed = (double)(end - start) / CLOCKS_PER_SEC;
   printf( "执行10000000次空循环用时%f秒\n", timeUsed );
}
```
输出结果
    执行10000000次空循环用时0.031000秒
**time（取得目前的时间）**
函数定义 
time_t time(time_t *t); 
函数说明 
此函数会返回从公元1970年1月1日的0时0分0秒算起到现在所经过的秒数（CT时间）。如果t是非空指针的话，此函数也会将返回值存到t指针所指的内存。 
返回值 
成功则返回秒数，失败则返回((time_t)-1)值，错误原因存于errno中。 
注意：由于返回的是CT时间，所以在做时间判断的时候一定要注意时区
范例
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- #include <stdio.h> 
- #include <time.h> 
- 
- #define DAY_SECONDS (24*60*60) 
- 
- **int** main(**int** argc, **char****argv)
   
- {   
- **time_t** timeNow = time(NULL);   
- 
- **time_t** timeDayNow =  timeNow%DAY_SECONDS;   
- **int** hour = timeDayNow/3600;   
- **int** min = (timeDayNow%3600)/60;   
- **int** sec = timeDayNow%60;   
- 
-     printf("现在时间%d:%d:%d\n", hour, min, sec);    
- 
- return 0;   
- };  
```cpp
#include <stdio.h>
#include <time.h>
#define DAY_SECONDS (24*60*60)
int main(int argc, char**argv)
{
	time_t timeNow = time(NULL);
	time_t timeDayNow =  timeNow%DAY_SECONDS;
	int hour = timeDayNow/3600;
	int min = (timeDayNow%3600)/60;
	int sec = timeDayNow%60;
	printf("现在时间%d:%d:%d\n", hour, min, sec); 
	return 0;
};
```
输出结果会比你当前系统的时间早8个小时，那是因为中国内地的时间与UTC的时差为+8
**localtime（取得当地目前时间和日期）**
函数定义 
struct tm *localtime(const time_t * timep); 
函数说明 
将参数timep所指的time_t结构中的信息转换成真实世界所使用的时间日期表示方法，然后将结果由结构tm返回。结构tm的定义请参考上面。此函数返回的时间日期已经转换成当地时区。
返回值 
返回结构tm代表目前的当地时间。 
**gmtime（取得当地目前时间和日期）**
函数定义 
struct tm *gmtime()(const time_t * timep); 
函数说明 
将日历时间转化为世界标准时间（即格林尼治时间），并返回一个tm结构体来保存这个时间
返回值 
返回结构tm代表目前的当地时间。 
上面这个两个函数的参数和返回值都一样，唯一区别是localtime做了时区处理，返回当前时区的时间，而gmtime返回UTC时间
范例
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- #include <stdio.h> 
- #include <time.h> 
- 
- **int** main(**int** argc, **char****argv)
   
- {   
- const**char** *wday[]={"星期天","星期一","星期二","星期三","星期四","星期五","星期六"}; 
   
- 
- **time_t** timeNow = time(NULL);   
- 
- struct**tm*** p = localtime(&timeNow);    
- 
- struct**tm*** pp;   
- 
-     printf("%d年%d月%d日 %d:%d:%d %s\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec, wday[p->tm_wday]);    
- 
-     pp = gmtime(&timeNow);   
- 
-     printf("%d年%d月%d日 %d:%d:%d %s\n",pp->tm_year+1900,pp->tm_mon+1,pp->tm_mday,pp->tm_hour,pp->tm_min,pp->tm_sec, wday[pp->tm_wday]);    
- 
- return 0;   
- };  
```cpp
#include <stdio.h>
#include <time.h>
int main(int argc, char**argv)
{
	const char *wday[]={"星期天","星期一","星期二","星期三","星期四","星期五","星期六"}; 
	time_t timeNow = time(NULL);
	struct tm* p = localtime(&timeNow); 
	struct tm* pp;
	printf("%d年%d月%d日 %d:%d:%d %s\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec, wday[p->tm_wday]); 
	pp = gmtime(&timeNow);
	printf("%d年%d月%d日 %d:%d:%d %s\n",pp->tm_year+1900,pp->tm_mon+1,pp->tm_mday,pp->tm_hour,pp->tm_min,pp->tm_sec, wday[pp->tm_wday]); 
	return 0;
};
```
输出结果
2010年4月1日 15:16:22 星期四
2010年4月1日 7:16:22 星期四
  其中localtime的结果和当前系统时间相同，gmtime比当前时间少8个小时
  注意：由于localtime和gmtime都使用全局变量存储结果，所以在使用时一定要注意，特别是在多线程中，他们都不是线程安全的函数
范例
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- #include <stdio.h> 
- #include <time.h> 
- 
- **int** main(**int** argc, **char****argv)
   
- {   
- **time_t** timeNow = time(NULL);   
- struct**tm** *p = localtime(&timeNow);    
- 
- struct**tm** *pp;   
-     timeNow += 10000;   
-     pp = localtime(&timeNow);   
- 
-     printf("当前时间：%d年%d月%d日 %d:%d:%d\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec );    
-     printf("相加后时间：%d年%d月%d日 %d:%d:%d\n",pp->tm_year+1900,pp->tm_mon+1,pp->tm_mday,pp->tm_hour,pp->tm_min,pp->tm_sec );    
- 
- return 0;   
- };  
```cpp
#include <stdio.h>
#include <time.h>
int main(int argc, char**argv)
{
	time_t timeNow = time(NULL);
	struct tm *p = localtime(&timeNow); 
	struct tm *pp;
	timeNow += 10000;
	pp = localtime(&timeNow);
	printf("当前时间：%d年%d月%d日 %d:%d:%d\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec ); 
	printf("相加后时间：%d年%d月%d日 %d:%d:%d\n",pp->tm_year+1900,pp->tm_mon+1,pp->tm_mday,pp->tm_hour,pp->tm_min,pp->tm_sec ); 
	return 0;
};
```
输出：
当前时间：2010年4月1日 18:42:46
相加后时间：2010年4月1日 18:42:46
虽然后面的时间加了10000秒，但由于他们共用同样的全局变量，所以两个的时间是一样的
**mktime（将时间结构数据转换成经过的秒数）**
函数定义
time_t mktime(strcut tm * timeptr); 
函数说明 
mktime()用来将参数timeptr所指的tm结构数据转换成从公元1970年1月1日0时0分0 秒算起至今的UTC时间所经过的秒数。 
返回值 
返回经过的秒数。 
注意：这里传入的参数必须是本地时间，不然就乱了
范例
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- #include <stdio.h> 
- #include <time.h> 
- 
- **int** main(**int** argc, **char****argv)
   
- {   
- **time_t** timeNow = time(NULL);   
- 
- struct**tm*** p = localtime(&timeNow);    
- 
- **time_t** timeNow1 = mktime(p);    
- 
- struct**tm*** pp = gmtime(&timeNow);   
- 
- **time_t** timeNow2 = mktime(pp);   
- 
-     printf("%ld\n%ld\n%ld\n", timeNow, timeNow1, timeNow2);   
- 
- return 0;   
- };  
```cpp
#include <stdio.h>
#include <time.h>
int main(int argc, char**argv)
{
	time_t timeNow = time(NULL);
	struct tm* p = localtime(&timeNow); 
	time_t timeNow1 = mktime(p); 
	struct tm* pp = gmtime(&timeNow);
	time_t timeNow2 = mktime(pp);
	printf("%ld\n%ld\n%ld\n", timeNow, timeNow1, timeNow2);
	return 0;
};
```
输出结果
1270106860
1270106860
1270078060
可以发现当传UTC时间给mktime的时候，就还原不回去了，会比当前时间少28800秒，刚好8个小时
**夏令时间**
    夏令时比标准时快一个小时。例如，在夏令时的实施期间，标准时间的上午10点就成了夏令时的上午11点。
　 夏令时，又称“日光节约时制”或“夏时制”，是一种为节约能源而人为规定地方时间的制度，在这一制度实行期间所采用的统一时间称为“夏令时间”。一般在天亮早的夏季人为将时间提前一小时，可以使人早起早睡，减少照明量，以充分利用光照资源，从而节约照明用电。各个采纳夏令时的国家具体规定不同。目前全世界有近110个国家每年要实行夏令时。（各时区多数位于其理想边界之西，导致实际上全年实施夏令时。），中国现在已经不实行夏令时
范例
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- #include <stdio.h> 
- #include <time.h> 
- 
- **int** main(**int** argc, **char****argv)
   
- {   
- **time_t** timeNow = time(NULL);   
- 
- struct**tm** p = *(localtime(&timeNow));    
- struct**tm** pp = p;   
- 
-     printf("当前时间：%ld\n", mktime(&p));   
- 
-     pp.tm_isdst = 1;   
- 
-     printf("启动夏令时后的时间：%ld\n", mktime(&pp));   
- 
- return 0;   
- };  
```cpp
#include <stdio.h>
#include <time.h>
int main(int argc, char**argv)
{
	time_t timeNow = time(NULL);
	struct tm p = *(localtime(&timeNow)); 
	struct tm pp = p;
	printf("当前时间：%ld\n", mktime(&p));
	pp.tm_isdst = 1;
	printf("启动夏令时后的时间：%ld\n", mktime(&pp));
	return 0;
};
```
输出：
当前时间：1270109197
启动夏令时后的时间：1270109197
从这里的输出发现，设置这个值好像没什么用处，同时我试了下调试运行，发现执行了mktime(&pp)后，pp中的tm_isdst又等于0了
暂时没发现tm_isdst变量的实际用途
还有一点要注意，struct tm结构体中字段tm_mday的值是从1开始，所以在构造月初时间的时候一定要把这个字段置为1
Cpp代码 ![复制代码](http://public0821.iteye.com/images/icon_copy.gif)![收藏代码](http://public0821.iteye.com/images/icon_star.png)![](http://public0821.iteye.com/images/spinner.gif)
- #include <stdio.h> 
- #include <time.h> 
- 
- **int** main(**int** argc, **char****argv)
   
- {   
- **time_t** timeNow = time(NULL);   
- struct**tm** *p = localtime(&timeNow); //取得当前时间
- 
- struct**tm** pp;   
-     memset(&pp, '\0', sizeof(pp));   
-     pp.tm_year = p->tm_year;   
-     pp.tm_mon = p->tm_mon;//设置成了当前月 
- 
- **time_t** timePP = mktime(&pp);   
- 
-     printf("时间：%d年%d月%d日 %d:%d:%d\n",pp.tm_year+1900,pp.tm_mon+1,pp.tm_mday,pp.tm_hour,pp.tm_min,pp.tm_sec );    
- 
- return 0;   
- };  
```cpp
#include <stdio.h>
#include <time.h>
int main(int argc, char**argv)
{
	time_t timeNow = time(NULL);
	struct tm *p = localtime(&timeNow); //取得当前时间
	struct tm pp;
	memset(&pp, '\0', sizeof(pp));
	pp.tm_year = p->tm_year;
	pp.tm_mon = p->tm_mon;//设置成了当前月
	
	time_t timePP = mktime(&pp);
	printf("时间：%d年%d月%d日 %d:%d:%d\n",pp.tm_year+1900,pp.tm_mon+1,pp.tm_mday,pp.tm_hour,pp.tm_min,pp.tm_sec ); 
	return 0;
};
```
输出结果：时间:2010年3月31日 0:0:0
我们期待的输出是“2010年4月1日 0:0:0”，但由于pp.tm_mday等于0，所以最后得到的时间timePP是上个月月底，相差了一天，正确的做法是设置pp.tm_mday等于1
引用:[http://public0821.iteye.com/blog/630168](http://public0821.iteye.com/blog/630168)
