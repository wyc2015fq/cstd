# vc++ 精确获取时间（QueryPerformanceCounter） - 建建的博客 - CSDN博客
2018年05月10日 10:20:12[纪建](https://me.csdn.net/u013898698)阅读数：104

对于精确度要求高的定时操作，则应该使用QueryPerformanceFrequency()和 QueryPerformanceCounter()函数。数据类型ARGE_INTEGER既可以是一个8字节长的整型数，也可以是两个4字节长的整型数的联合结构， 其具体用法根据编译器是否支持64位而定。定时过程如下：
在进行定时之前，先调用QueryPerformanceFrequency()函数获得机器内部定时器的时钟频率， 然后在需要严格定时的事件发生之前和发生之后分别调用QueryPerformanceCounter()函数，利用两次获得的计数之差及时钟频率，计算出事件经 历的精确时间。
LARGE_INTEGER tima,timb;
QueryPerformanceCounter(&tima);
在 Windows Server 2003 和 WindowsXP 中使用 QueryPerformanceCounter 函数的程序可能执行不当
QueryPerformanceCounter 來精確計算執行時間
QueryPerformanceCounter 來精確計算執行時間
// 這個程式展示了如何使用QueryPerformanceCounter 來精確計算執行時間
//代码
- LARGE_INTEGER m_liPerfFreq={0};
- //获取每秒多少CPU Performance Tick
-  QueryPerformanceFrequency(&m_liPerfFreq); 
- 
-  LARGE_INTEGER m_liPerfStart={0};
-  QueryPerformanceCounter(&m_liPerfStart);
- 
- for(int i=0; i< 100; i++)
-   cout << i << endl;
- 
-  LARGE_INTEGER liPerfNow={0};
- // 计算CPU运行到现在的时间
-  QueryPerformanceCounter(&liPerfNow);
- 
- int time=( ((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000)/m_liPerfFreq.QuadPart);
- 
- char buffer[100];
-  sprintf(buffer,"執行時間 %d millisecond ",time);
- 
-  cout<<buffer<<endl;
- 
- 
QueryPerformanceCounter()这个函数返回高精确度性能计数器的值,它可以以微妙为单位计时.但是QueryPerformanceCounter()确切的精确计时的最小单位是与系统有关的,所以,必须要查询系统以得到QueryPerformanceCounter()返回的嘀哒声的频率.
QueryPerformanceFrequency()提供了这个频率值,返回每秒嘀哒声的个数.
计算确切的时间是从第一次调用QueryPerformanceCounter()开始的
假设得到的LARGE_INTEGER为nStartCounter,过一段时间后再次调用该函数结束的,
设得到nStopCounter.
两者之差除以QueryPerformanceFrequency()的频率就是开始到结束之间的秒数.由于计时函数本身要耗费很少的时间,要减去一个很少的时间开销.但一般都把这个开销忽略.公式如下:   
                         nStopCounter-nStartCounter 
ElapsedTime=------------------------------------ - overhead 
frequency 
double time=(nStopCounter.QuadPart-nStartCounter.QuadPart)/frequency.QuadPart
这两个函数是VC提供的仅供Windows 95及其后续版本使用的精确时间函数，并要求计算机从硬件上支持精确定时器。
QueryPerformanceFrequency()函数和QueryPerformanceCounter()函数的原型如下：
       BOOL  QueryPerformanceFrequency(LARGE_INTEGER ＊lpFrequency);
       BOOL  QueryPerformanceCounter(LARGE_INTEGER ＊lpCount);
　　数据类型ARGE_INTEGER既可以是一个8字节长的整型数，也可以是两个4字节长的整型数的联合结构， 其具体用法根据编译器是否支持64位而定。该类型的定义如下：
       typedef union _LARGE_INTEGER
       {
           struct
           {
              DWORD LowPart ;// 4字节整型数
              LONG  HighPart;// 4字节整型数
           };
           LONGLONG QuadPart ;// 8字节整型数
           
        }LARGE_INTEGER ;
　　在进行定时之前，先调用QueryPerformanceFrequency()函数获得机器内部定时器的时钟频率， 然后在需要严格定时的事件发生之前和发生之后分别调用QueryPerformanceCounter()函数，利用两次获得的计数之差及时钟频率，计算出事件经 历的精确时间。下列代码实现1ms的精确定时：
-        LARGE_INTEGER litmp; 
-        LONGLONG QPart1,QPart2;
- double dfMinus, dfFreq, dfTim; 
-        QueryPerformanceFrequency(&litmp);
-        dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
-        QueryPerformanceCounter(&litmp);
-        QPart1 = litmp.QuadPart;// 获得初始值
- do
-        {
-           QueryPerformanceCounter(&litmp);
-           QPart2 = litmp.QuadPart;//获得中止值
-           dfMinus = (double)(QPart2-QPart1);
-           dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
-        }while(dfTim<0.001);
　　其定时误差不超过1微秒，精度与CPU等机器配置有关。 下面的程序用来测试函数Sleep(100)的精确持续时间：
-        LARGE_INTEGER litmp; 
-        LONGLONG QPart1,QPart2;
- double dfMinus, dfFreq, dfTim; 
-        QueryPerformanceFrequency(&litmp);
-        dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
-        QueryPerformanceCounter(&litmp);
-        QPart1 = litmp.QuadPart;// 获得初始值
-        Sleep(100);
-        QueryPerformanceCounter(&litmp);
-        QPart2 = litmp.QuadPart;//获得中止值
-        dfMinus = (double)(QPart2-QPart1);
-        dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒    
　　由于Sleep()函数自身的误差，上述程序每次执行的结果都会有微小误差。下列代码实现1微秒的精确定时：
-        LARGE_INTEGER litmp; 
-        LONGLONG QPart1,QPart2;
- double dfMinus, dfFreq, dfTim; 
-        QueryPerformanceFrequency(&litmp);
-        dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
-        QueryPerformanceCounter(&litmp);
-        QPart1 = litmp.QuadPart;// 获得初始值
- do
-        {
-           QueryPerformanceCounter(&litmp);
-           QPart2 = litmp.QuadPart;//获得中止值
-           dfMinus = (double)(QPart2-QPart1);
-           dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
-        }while(dfTim<0.000001);
其定时误差一般不超过0.5微秒，精度与CPU等机器配置有关。
