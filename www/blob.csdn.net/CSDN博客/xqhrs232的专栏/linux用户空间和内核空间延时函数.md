# linux用户空间和内核空间延时函数 - xqhrs232的专栏 - CSDN博客
2011年12月29日 20:42:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1050
原文地址::[http://blog.csdn.net/kevined/article/details/2625095](http://blog.csdn.net/kevined/article/details/2625095)
昨天团队中的一个成员忽然问起linux的延时函数,特整理一下供新人参考:
Linux 下的延时函数分为用户空间延时和内核空间的延时.
**用户空间:**
最常用的延时函数是sleep()，但是其时间延时精确到s，不能满足很多情况下的要求，在linux下，有一个延时函数，可以精确到us级，那就是usleep()
比如要延时一妙，
usleep(1000000);
**内核空间:**
udelay(int n);  延时n微秒 mdelay(int n);  延时n毫秒 ndelay(int n);  延时n纳秒
//=======================================================================
相关网帖
Linux中的延时函数
原文地址::[http://www.linuxidc.com/Linux/2008-06/13407.htm](http://www.linuxidc.com/Linux/2008-06/13407.htm)
应用层：
   #include <unistd.h>
   1、unsigned int sleep(unsigned int seconds); 秒级
   2、int usleep(useconds_t usec);              微秒级：1/10^-6 
   #define _POSIX_C_SOURCE 199309
   #include <time.h>
   3、int nanosleep(const struct timespec *req, struct timespec *rem);
       struct timespec {
                  time_t tv_sec;        /* seconds */
                  long   tv_nsec;       /* nanoseconds */
              };
       // The value of the nanoseconds field must be in the range 0 to 999999999.
 内核层：
   include <linux/delay.h>
   1、void ndelay(unsigned long nsecs);         纳秒级：1/10^-10
   2、void udelay(unsigned long usecs);         微秒级: 1/10^-6
   3、void mdelay(unsigned long msecs);         毫秒级：1/10^-3
本篇文章来源于 Linux公社网站([www.linuxidc.com](http://www.linuxidc.com))  原文链接：[http://www.linuxidc.com/Linux/2008-06/13407.htm](http://www.linuxidc.com/Linux/2008-06/13407.htm)
