# Linux系统调用--gettimeofday/settimeofday函数详解 - xqhrs232的专栏 - CSDN博客
2011年05月31日 13:38:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3893
原文地址::[http://freewind886.blog.163.com/blog/static/661924642009316113825767/](http://freewind886.blog.163.com/blog/static/661924642009316113825767/)
【gettimeofday/settimeofday系统调用】 
功能描述：
gettimeofday获取当前时间和时区信息。settimeofday设置当前时间和时区信息。只有超级用户可以调用settimeofday，如果存在为NULL的参数，表示不改变某一项信息。
用法： 
#include <sys/time.h>
#include <time.h>
int gettimeofday(struct timeval *tv, struct timezone *tz);
int settimeofday(const struct timeval *tv , const struct timezone *tz);
参数： 
tv：对于gettimeofday，指向存放返回的时间信息的缓冲区；对于settimeofday，指向需要设置的时间信息缓冲区。原型如下
struct timeval {
    time_t      tv_sec;     /* 秒 */
    suseconds_t tv_usec;    /* 微妙 */
};
tz：时区信息，一般不会被使用。原型如下
struct timezone {
    int tz_minuteswest;     /* minutes west of Greenwich */
    int tz_dsttime;         /* type of DST correction */
};
如果tv或tz某一项为NULL，表示对相关的信息不感兴趣。
操作timeval结构体的宏有：
#define timerisset(tvp)/
        ((tvp)->tv_sec || (tvp)->tv_usec)
#define timercmp(tvp, uvp, cmp)/
        ((tvp)->tv_sec cmp (uvp)->tv_sec ||/
        (tvp)->tv_sec == (uvp)->tv_sec &&/
        (tvp)->tv_usec cmp (uvp)->tv_usec)
#define timerclear(tvp)/
        ((tvp)->tv_sec = (tvp)->tv_usec = 0)
返回说明： 
成功执行时，返回0。失败返回-1，errno被设为以下的某个值 
EFAULT：tv或tz其中某一项指向的空间不可访问
EINVAL：时区格式无效
EPERM：权限不足，调用进程不允许使用settimeofday设置当前时间和时区值。
