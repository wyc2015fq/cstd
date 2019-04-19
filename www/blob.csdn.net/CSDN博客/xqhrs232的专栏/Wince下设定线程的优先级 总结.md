# Wince下设定线程的优先级 总结 - xqhrs232的专栏 - CSDN博客
2013年11月20日 18:49:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1316
原文地址::[http://www.67bar.com/archives/400](http://www.67bar.com/archives/400)
**友情提示：**当在调试程序时，遇到了莫名其妙的问题，但是又不能发现问题之所在，那可以尝试一下调整线程的优先级，有的时候很好使啊，亲身体会的。
wince下设定线程的优先级一般有以下两种方式：
1. 通过ceSetThreadPriority函数
BOOL CeSetThreadPriority(
  HANDLE hThread,
  int nPriority
);
第一个参数是线程句柄，第二个值介于 0-255 之间，用于描述所需的优先级。选择使用哪个线程优先级非常关键，而能够以图表表现应用程序优先级的使用，也有助于确保适当的性能。优先级从
 0 至 247 的线程（0 表示最高优先级）是实时线程优先级，需要调用 CeSetThreadPriority 来访问。一般线程优先级介于 248-255 之间，要使用 SetThreadPriority 进行访问。下表提供了 Windows CE .NET 标准优先级实现的快速指南。
表 3：实时线程优先级：CeSetThreadPriority
|优先级|组件|
|----|----|
|0-19|开放 – 高于驱动程序的实时|
|20|Permedia 垂直折返|
|21-98|开放 – 高于驱动程序的实时|
|99|电源管理恢复线程|
|100-108|USB OHCI UHCI、串行|
|109-129|Irsir1、NDIS、触摸板|
|130|KITL|
|131|VMini|
|132|CxPort|
|133-144|开放 – 设备驱动程序|
|145|PS2 键盘|
|146-147|开放 – 设备驱动程序|
|148|IRComm|
|149|开放 – 设备驱动程序|
|150|TAPI|
|151-152|开放 – 设备驱动程序|
|153-247|开放 – 低于驱动程序的实时|
表 4：一般线程优先级：SetThreadPriority
|优先级|组件|
|----|----|
|248|电源管理|
|249|WaveDev、TVIA5000、鼠标、PnP、电源|
|250|WaveAPI|
|251|电源管理器电池线程|
|252-255|开放|
一般来说，最先需要决定的是要确定关键线程是否需要驱动程序。如果关键线程需要驱动程序才能正常工作，而将它的优先级设定为高于驱动程序的优先级，则很难获得好的性能。总之，时间关键型应用程序需要放在“高于驱动程序类别的实时”类别中，优先级范围为 0-98。
|优先级范围|分配对象|
|----|----|
|0-96|高于驱动程序的程序|
|97-152|基于Windows CE的驱动程序|
|153-247|低于驱动程序的程序|
|248-255|普通的应用程序|
THREAD_PRIORITY_TIME_CRITICAL
         Indicates 3 points above normal priority. 
THREAD_PRIORITY_HIGHEST
                    Indicates 2 points above normal priority. 
THREAD_PRIORITY_ABOVE_NORMAL
      Indicates 1 point above normal priority. 
THREAD_PRIORITY_NORMAL
                    Indicates normal priority. 
THREAD_PRIORITY_BELOW_NORMAL
      Indicates 1 point below normal priority. 
THREAD_PRIORITY_LOWEST
                    Indicates 2 points below normal priority. 
THREAD_PRIORITY_ABOVE_IDLE
             Indicates 3 points below normal priority. 
THREAD_PRIORITY_IDLE
                           Indicates 4 points below normal priority. 
获取线程优先级：
int CeGetThreadPriority(
  HANDLE hThread
);
2. 通过SetThreadPriority函数，其设定优先级的范围为：248-255，这个要注意了
BOOL SetThreadPriority(
HANDLE hThread, // handle to the thread
int nPriority // thread priority level
);
功能说明
设置指定线程的优先级
参数说明
||线程优先级等级|标志|优先级值|
|----|----|----|----|
|1|idle （最低）|THREAD_PRIORITY_IDLE|如果进程优先级为realtime则调整为16，其它情况为1|
|2|LOWEST 低|THREAD_PRIORITY_LOWEST|-2（在原有基础上-2）|
|3|BELOW 低于标准|THREAD_PRIORITY_BELOW_NORMAL|-1（在原有基础上-1）|
|4|NORMAL（标准）|THREAD_PRIORITY_NORMAL|不变(取进程优先级值)|
|5|ABOVE 高于标准|THREAD_PRIORITY_ABOVE_NORMAL|+1（在原有基础上+1）|
|6|HIGHEST （高）|THREAD_PRIORITY_HIGHEST|+2（在原有基础上+2）|
|7|CRITICAL（最高)|THREAD_PRIORITY_TIME_CRITICAL|如果进程优先级为realtime则调整为31，其它情况为15|
