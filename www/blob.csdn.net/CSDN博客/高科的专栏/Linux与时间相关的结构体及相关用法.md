
# Linux与时间相关的结构体及相关用法 - 高科的专栏 - CSDN博客

2012年09月11日 15:20:24[高科](https://me.csdn.net/pbymw8iwm)阅读数：5428


**1. Linux下与时间有关的结构体**
struct timeval
{
int tv_sec;
int tv_usec;
};
其中tv_sec是由凌晨开始算起的秒数，tv_usec则是微秒(10E-6 second)。
struct timezone
{
int tv_minuteswest;
int tv_dsttime;
};
tv_minuteswest是格林威治时间往西方的时差，tv_dsttime则是时间的修正方式。
struct timespec
{
long int tv_sec;
long int tv_nsec;
};
tv_nsec是nano second(10E-9 second)。
struct tm
{
int tm_sec;
int tm_min;
int tm_hour;
int tm_mday;
int tm_mon;


