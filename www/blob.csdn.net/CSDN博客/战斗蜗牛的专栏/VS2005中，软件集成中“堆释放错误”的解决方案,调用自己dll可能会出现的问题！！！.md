# VS2005中，软件集成中“堆释放错误”的解决方案,调用自己dll可能会出现的问题！！！ - 战斗蜗牛的专栏 - CSDN博客





2011年10月13日 18:55:44[vbskj](https://me.csdn.net/vbskj)阅读数：621







一个模块一个堆，一个线程一个栈。

dll里malloc的内存，在exe里free会出错。


CRT（C运行时期库）不是使用进程缺省的堆来实现malloc（new中调用malloc）的，而是使用一个全局句柄 HANDLE _crtheap来分配内存的。这个_crtheap是在XXXCRTStartUp（CRT提供的进口点函数）中创建的。   

由于CRT静态连接，则楼主的DLL里有也有一个CRT，因此也有一个_crtheap。而在dll中的new使用dll中的_crtheap句柄分配 堆，在exe中的delete使用exe中的_crtheap释放堆，当然失败！


解决办法：

1。在DLL中输出一个函数给EXE调用，专门用来释放由DLL分配的内存；

2。用 GlobalAlloc()代替new，用GlobalFree()代替delete；

3。使用单一的堆，分配内存使用 HeapAlloc(GetProcessHeap(),0,size)，释放内存使用HeapFree(GetProcessHeap(),0,p)；

4。 把dll和exe的Settings的C/C++选项卡的Code Generation的Use Run-time liberary改成Debug Multithreaded DLL，在Release版本中改成Multithreaded DLL；这样使用一个CRT了——MSVCRT.DLL。


