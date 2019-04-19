# usleep函数 - maopig的专栏 - CSDN博客
2012年03月15日 16:12:49[maopig](https://me.csdn.net/maopig)阅读数：5618

　　usleep功能把进程挂起一段时间， 单位是微秒（百万分之一秒）；
　　头文件： unistd.h
　　语法: **void usleep(int micro_seconds);**
　　返回值: 无
　　内容说明：本函数可暂时使程序停止执行。参数 micro_seconds 为要暂停的微秒数(us)。
**注意：这个函数不能工作在** Windows [操作系统](http://baike.baidu.com/view/880.htm)中。参见：usleep() 与sleep()类似，用于延迟挂起进程。进程被挂起放到reday queue。
　　只是一般情况下，延迟时间数量级是秒的时候，尽可能使用sleep()函数。且此函数已被废除，可使用nanosleep。如果延迟时间为几十毫秒（1ms = 1000us），或者更小，尽可能使用usleep()函数。这样才能最佳的利用CPU时间
