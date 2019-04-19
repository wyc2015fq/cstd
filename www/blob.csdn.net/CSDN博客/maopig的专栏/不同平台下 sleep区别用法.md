# 不同平台下 sleep区别用法 - maopig的专栏 - CSDN博客
2011年11月28日 14:49:44[maopig](https://me.csdn.net/maopig)阅读数：1237标签：[平台																[delay																[linux																[mfc																[跨平台																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=跨平台&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=delay&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)
个人分类：[Linux																[网络](https://blog.csdn.net/maopig/article/category/869532)](https://blog.csdn.net/maopig/article/category/850267)

应用程序：
#include <syswait.h>
usleep(n) //n微秒
Sleep（n）//n毫秒
sleep（n）//n秒
驱动程序：
#include <linux/delay.h>
mdelay(n) //微秒milliseconds 其实现
#ifdef notdef
#define mdelay(n) (\
{unsigned long msec=(n); while (msec--) udelay(1000);})
#else
#define mdelay(n) (\
(__builtin_constant_p(n) && (n)<=MAX_UDELAY_MS) ? udelay((n)*1000) : \
({unsigned long msec=(n); while (msec--) udelay(1000);}))
#endif
调用asm/delay.h的udelay,udelay应该是纳秒级的延时
Dos: 
sleep(1); //停留1秒 
delay(100); //停留100毫秒   
Windows: 
Sleep(100); //停留100毫秒 
Linux: 
sleep(1); //停留1秒 
usleep(1000); //停留1毫秒
每一个平台不太一样,最好自己定义一套跨平台的宏进行控制  
#### 秒还是微秒？关于延时函数sleep()
    因为要写一段代码，需要用到sleep（）函数，在我印象中，sleep(10)好像是休眠10微秒，结果却是休眠了10秒（在Linux下）。觉得很奇怪，因为头儿也记得好像是微秒为单位的。所以就查了一下。
    原来linux下的sleep函数原型为：
        unsigned int sleep(unsigned int seconds);
而MFC中的Sleep函数原型为：
        void Sleep(DWORD dwMilliseconds);
也就是说，Linux下（使用的gcc的库），sleep()函数是以秒为单位的，sleep(1);就是休眠1秒。而MFC下的sleep()函数是以微秒为单位的，sleep(1000);才是休眠1秒。原来如此啊。而如果在Linux下也用微妙为单位休眠，可以使用线程休眠函数:void
usleep(unsigned long usec);当然，使用的时候别忘记#include <system.h>哦。
    另外值得一提的是，linux下还有个delay()函数，原型为extern void delay(unsigned int msec);它可以延时msec*4毫秒，也就是如果想延时一秒钟的话，可以这么用 delay(250);
