# WinCE中微秒（us）级的延时函数----带实验数据 - xqhrs232的专栏 - CSDN博客
2014年08月02日 10:40:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：926
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/ok138ok/article/details/4358338](http://blog.csdn.net/ok138ok/article/details/4358338)
相关文章
1、WINCE 微秒、毫秒级延时函数 
 ----[http://blog.163.com/mg29_x/blog/static/60304826200812911148146/](http://blog.163.com/mg29_x/blog/static/60304826200812911148146/)
2、编写wince的底层驱动应该如果实现比较精确的uS、mS级延时呢?----[http://bbs.csdn.net/topics/300188985](http://bbs.csdn.net/topics/300188985)
**WinCE中微秒us级的延时函数**
--作者：赖玉平（auly）[aulyp@163.com](mailto:aulyp@163.com)
    在WINCE 5.0中，做驱动开发，特别是通信协议驱动时，延时动作是非常有必要的，如果想要做延时，像单片机一样用空操作来做的话，是不行的，由于时间片的原因，最多只能达到1US的延时量，如果要实现微秒级的延时，得用到系统API.
    下面是一个经过我验证的函数，在做通信协议时，非常有用。
void Delay_us(int n)
{
    LARGE_INTEGER litmp;
       LONGLONG QPart1,QPart2;
       double dfMinus,dfFreq,dfTim;
       QueryPerformanceFrequency(&litmp);
       dfFreq = (double)litmp.QuadPart;
       QueryPerformanceCounter(&litmp);
       QPart1 = litmp.QuadPart;
       do
       {
       QueryPerformanceCounter(&litmp);
          QPart2 = litmp.QuadPart;
          dfMinus = (double)(QPart2-QPart1);
          dfTim = dfMinus/dfFreq;
       }while(dfTim<0.000001*n);
}
误差：Delay_us(23) =====实际：25us 
            Delay_us(100) ====实际：107us 
//===================================================================================================================================
备注::
1》本人在WINCE6.0+800MH主频的ARM上实验的数据如下
 A、for(j=0;j<100;j++) //336us
  Delay_us(1);
B、Delay_us(100);//102.625us
C、 Delay_us(10);//12.625us
D、Delay_us(5);//7.875us
E、 Delay_us(1);//4.125us
通过看上面的数据，调这个 Delay_us( )函数不要想当然地传多少参数，就会延时多少US的时间出来，最好用示波器量一下某个IO脚拉低拉高之间用这个函数延时出来的时延，
然后保持调用这个函数的参数不变！！！----不是传多少的参数，延时出来的时间就是Delay_us(1)单位时间的简单的参数倍数！！！
其它用for循环写的延时函数也是同样的道理，不要想当然
2》QueryPerformanceCounter好像要依赖BSP中提供的一个硬件定时器？！
