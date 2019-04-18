# VC中基于 Windows 的精确定时 - ljx0305的专栏 - CSDN博客
2010年04月16日 13:37:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：459标签：[windows																[timer																[integer																[null																[任务																[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=integer&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[C++																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/380566)
**VC中基于 Windows 的精确定时**
中国科学院光电技术研究所 [游志宇](mailto:yzyseal@126.com)
[示例工程下载](http://www.vckbase.com/code/winsys/win32/multitimerdemo.rar)
　　在工业生产控制系统中，有许多需要定时完成的操作，如定时显示当前时间，定时刷新屏幕上的进度条，上位 机定时向下位机发送命令和传送数据等。特别是在对控制性能要求较高的实时控制系统和数据采集系统中，就更需要精确定时操作。
　　众所周知，Windows 是基于消息机制的系统，任何事件的执行都是通过发送和接收消息来完成的。 这样就带来了一些问题，如一旦计算机的CPU被某个进程占用，或系统资源紧张时，发送到消息队列 中的消息就暂时被挂起，得不到实时处理。因此，不能简单地通过Windows消息引发一个对定时要求 严格的事件。另外，由于在Windows中已经封装了计算机底层硬件的访问，所以，要想通过直接利用 访问硬件来完成精确定时，也比较困难。所以在实际应用时，应针对具体定时精度的要求，采取相适 应的定时方法。
　　VC中提供了很多关于时间操作的函数，利用它们控制程序能够精确地完成定时和计时操作。本文详细介绍了 VC中基于Windows的精确定时的七种方式，如下图所示：
![](http://www.vckbase.com/document/journal/vckbase37/images/MultiTimerDemoimg.gif)
图一 图像描述 
　　方式一：VC中的WM_TIMER消息映射能进行简单的时间控制。首先调用函数SetTimer()设置定时 间隔，如SetTimer(0,200,NULL)即为设置200ms的时间间隔。然后在应用程序中增加定时响应函数 OnTimer()，并在该函数中添加响应的处理语句，用来完成到达定时时间的操作。这种定时方法非常 简单，可以实现一定的定时功能，但其定时功能如同Sleep()函数的延时功能一样，精度非常低，最小 计时精度仅为30ms，CPU占用低，且定时器消息在多任务操作系统中的优先级很低，不能得到及时响 应，往往不能满足实时控制环境下的应用。只可以用来实现诸如位图的动态显示等对定时精度要求不高的情况。如示例工程中的Timer1。 
　　方式二：VC中使用sleep()函数实现延时，它的单位是ms，如延时2秒，用sleep(2000)。精度非常 低，最小计时精度仅为30ms，用sleep函数的不利处在于延时期间不能处理其他的消息，如果时间太 长，就好象死机一样，CPU占用率非常高，只能用于要求不高的延时程序中。如示例工程中的Timer2。
　　方式三：利用COleDateTime类和COleDateTimeSpan类结合WINDOWS的消息处理过程来实现秒级延时。如示例工程中的Timer3和Timer3_1。以下是实现2秒的延时代码：
      COleDateTime      start_time = COleDateTime::GetCurrentTime();
      COleDateTimeSpan  end_time= COleDateTime::GetCurrentTime()-start_time;
      while(end_time.GetTotalSeconds()< 2) //实现延时2秒
     { 
              MSG   msg;
              GetMessage(&msg,NULL,0,0);
              TranslateMessage(&msg); 
              DispatchMessage(&msg);
              
             //以上四行是实现在延时或定时期间能处理其他的消息，
　　　　　　 //虽然这样可以降低CPU的占有率，
             //但降低了延时或定时精度，实际应用中可以去掉。
             end_time = COleDateTime::GetCurrentTime()-start_time;
      }//这样在延时的时候我们也能够处理其他的消息。      
　　方式四：在精度要求较高的情况下，VC中可以利用GetTickCount()函数，该函数的返回值是  DWORD型，表示以ms为单位的计算机启动后经历的时间间隔。精度比WM_TIMER消息映射高，在较 短的定时中其计时误差为15ms，在较长的定时中其计时误差较低，如果定时时间太长，就好象死机一样，CPU占用率非常高，只能用于要求不高的延时程序中。如示例工程中的Timer4和Timer4_1。下列代码可以实现50ms的精确定时：
       DWORD dwStart = GetTickCount();
       DWORD dwEnd   = dwStart;
       do
       {
          dwEnd = GetTickCount()-dwStart;
       }while(dwEnd <50);
为使GetTickCount()函数在延时或定时期间能处理其他的消息，可以把代码改为：
       DWORD dwStart = GetTickCount();
       DWORD dwEnd   = dwStart;
       do
       {
              MSG   msg;
              GetMessage(&msg,NULL,0,0);
              TranslateMessage(&msg); 
              DispatchMessage(&msg);
              dwEnd = GetTickCount()-dwStart;
       }while(dwEnd <50);
虽然这样可以降低CPU的占有率，并在延时或定时期间也能处理其他的消息，但降低了延时或定时精度。
　　方式五：与GetTickCount()函数类似的多媒体定时器函数DWORD timeGetTime(void)，该函数定时精 度为ms级，返回从Windows启动开始经过的毫秒数。微软公司在其多媒体Windows中提供了精确定时器的底 层API持，利用多媒体定时器可以很精确地读出系统的当前时间，并且能在非常精确的时间间隔内完成一 个事件、函数或过程的调用。不同之处在于调用DWORD timeGetTime(void) 函数之前必须将 Winmm.lib  和 Mmsystem.h 添加到工程中，否则在编译时提示DWORD timeGetTime(void)函数未定义。由于使用该 函数是通过查询的方式进行定时控制的，所以，应该建立定时循环来进行定时事件的控制。如示例工程中的Timer5和Timer5_1。
　　方式六：使用多媒体定时器timeSetEvent()函数，该函数定时精度为ms级。利用该函数可以实现周期性的函数调用。如示例工程中的Timer6和Timer6_1。函数的原型如下：
       MMRESULT timeSetEvent（ UINT uDelay, 
                               UINT uResolution, 
                               LPTIMECALLBACK lpTimeProc, 
                               WORD dwUser, 
                               UINT fuEvent ）
　　该函数设置一个定时回调事件，此事件可以是一个一次性事件或周期性事件。事件一旦被激活，便调用指定的回调函数， 成功后返回事件的标识符代码，否则返回NULL。函数的参数说明如下：
       uDelay：以毫秒指定事件的周期。
       Uresolution：以毫秒指定延时的精度，数值越小定时器事件分辨率越高。缺省值为1ms。
       LpTimeProc：指向一个回调函数。
       DwUser：存放用户提供的回调数据。
       FuEvent：指定定时器事件类型：
       TIME_ONESHOT：uDelay毫秒后只产生一次事件
       TIME_PERIODIC ：每隔uDelay毫秒周期性地产生事件。      
　　具体应用时，可以通过调用timeSetEvent()函数，将需要周期性执行的任务定义在LpTimeProc回调函数 中(如：定时采样、控制等)，从而完成所需处理的事件。需要注意的是，任务处理的时间不能大于周期间隔时间。另外，在定时器使用完毕后， 应及时调用timeKillEvent()将之释放。 
　　方式七：对于精确度要求更高的定时操作，则应该使用QueryPerformanceFrequency()和 QueryPerformanceCounter()函数。这两个函数是VC提供的仅供Windows 95及其后续版本使用的精确时间函数，并要求计算机从硬件上支持精确定时器。如示例工程中的Timer7、Timer7_1、Timer7_2、Timer7_3。
QueryPerformanceFrequency()函数和QueryPerformanceCounter()函数的原型如下：
       BOOL  QueryPerformanceFrequency(LARGE_INTEGER ＊lpFrequency);
       BOOL  QueryPerformanceCounter(LARGE_INTEGER ＊lpCount);
　　数据类型ARGE_INTEGER既可以是一个8字节长的整型数，也可以是两个4字节长的整型数的联合结构， 其具体用法根据编译器是否支持64位而定。该类型的定义如下：
       typedef union _LARGE_INTEGER
       {
           struct
           {
              DWORD LowPart ;// 4字节整型数
              LONG  HighPart;// 4字节整型数
           };
           LONGLONG QuadPart ;// 8字节整型数
           
        }LARGE_INTEGER ;
　　在进行定时之前，先调用QueryPerformanceFrequency()函数获得机器内部定时器的时钟频率， 然后在需要严格定时的事件发生之前和发生之后分别调用QueryPerformanceCounter()函数，利用两次获得的计数之差及时钟频率，计算出事件经 历的精确时间。下列代码实现1ms的精确定时：
       LARGE_INTEGER litmp; 
       LONGLONG QPart1,QPart2;
       double dfMinus, dfFreq, dfTim; 
       QueryPerformanceFrequency(&litmp);
       dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
       QueryPerformanceCounter(&litmp);
       QPart1 = litmp.QuadPart;// 获得初始值
       do
       {
          QueryPerformanceCounter(&litmp);
          QPart2 = litmp.QuadPart;//获得中止值
          dfMinus = (double)(QPart2-QPart1);
          dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
       }while(dfTim<0.001);
　　其定时误差不超过1微秒，精度与CPU等机器配置有关。 下面的程序用来测试函数Sleep(100)的精确持续时间：
       LARGE_INTEGER litmp; 
       LONGLONG QPart1,QPart2;
       double dfMinus, dfFreq, dfTim; 
       QueryPerformanceFrequency(&litmp);
       dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
       QueryPerformanceCounter(&litmp);
       QPart1 = litmp.QuadPart;// 获得初始值
       Sleep(100);
       QueryPerformanceCounter(&litmp);
       QPart2 = litmp.QuadPart;//获得中止值
       dfMinus = (double)(QPart2-QPart1);
       dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒     
　　由于Sleep()函数自身的误差，上述程序每次执行的结果都会有微小误差。下列代码实现1微秒的精确定时：
       LARGE_INTEGER litmp; 
       LONGLONG QPart1,QPart2;
       double dfMinus, dfFreq, dfTim; 
       QueryPerformanceFrequency(&litmp);
       dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
       QueryPerformanceCounter(&litmp);
       QPart1 = litmp.QuadPart;// 获得初始值
       do
       {
          QueryPerformanceCounter(&litmp);
          QPart2 = litmp.QuadPart;//获得中止值
          dfMinus = (double)(QPart2-QPart1);
          dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
       }while(dfTim<0.000001);
其定时误差一般不超过0.5微秒，精度与CPU等机器配置有关。(完)
引用:http://www.vckbase.com/document/viewdoc/?id=1301
