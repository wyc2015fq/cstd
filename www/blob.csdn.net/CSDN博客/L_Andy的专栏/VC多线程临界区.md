# VC多线程临界区 - L_Andy的专栏 - CSDN博客

2014年05月12日 17:20:54[卡哥](https://me.csdn.net/L_Andy)阅读数：6388


# 在使用多线程时，一般很少有多个线程完全独立的工作。往往是多个线程同时操作一个全局变量来获取程序的运行结果。多个线程同时访问同一个全局变量，如果都是读取操作，则不会出现问题。如果是写操作，则会发生错误。这时候，我们可以通过临界区，为全局变量设置一个保护，保证同时只有一个线程可以访问此变量，其他变量进入等待状态。

      临界区（Critical Section）是一段独占对某些共享资源访问的代码，在任意时刻只允许一个线程对共享资源进行访问。如果有多个线程试图同时访问临界区，那么在有一个线程进入后其他所有试图访问此临界区的线程将被挂起，并一直持续到进入临界区的线程离开。临界区在被释放后，其他线程可以继续抢占，并以此达到用原子方式操作共享资源的目的。

      临界区在使用时以CRITICAL_SECTION结构对象保护共享资源，并分别用 ①InitializeCriticalSection(&cs); //初始化临界区②EnterCriticalSection(&cs);//进入临界区③LeaveCriticalSection(&cs);//离开临界区④DeleteCriticalSection(&cs);//删除临界区 

      一般类程序(举例)

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// 临界区结构对象
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)CRITICAL_SECTION g_cs;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// 共享资源 
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)char g_cArray[10]
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)UINT ThreadProc1(LPVOID pParam)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 进入临界区
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　EnterCriticalSection(&g_cs);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 对共享资源进行写入操作
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　for (int i = 0; i < 10; i++)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)　{
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　　g_cArray[i] = ’a’;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　　Sleep(1);
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)　}
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 离开临界区
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　LeaveCriticalSection(&g_cs);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　return 0;
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)UINT ThreadProc2(LPVOID pParam)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 进入临界区
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　EnterCriticalSection(&g_cs);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 对共享资源进行写入操作
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　for (int i = 0; i < 10; i++)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)　{
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　　g_cArray[10 - i - 1] = ’b’;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　　Sleep(1);
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)　}
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 离开临界区
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　LeaveCriticalSection(&g_cs);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　return 0;
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)void CSampleView::OnCriticalSection() 
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 初始化临界区
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　InitializeCriticalSection(&g_cs);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 启动线程
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　AfxBeginThread(ThreadProc1, NULL);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　AfxBeginThread(ThreadProc2, NULL);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 等待计算完毕
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　Sleep(300);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 报告计算结果
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　CString sResult = CString(g_cArray);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　AfxMessageBox(sResult);
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

      在使用临界区时，一般不允许其运行时间过长，只要进入临界区的线程还没有离开，其他所有试图进入此临界区的线程都会被挂起而进入到等待状态，并会在一定程度上影响。程序的运行性能。尤其需要注意的是不要将等待用户输入或是其他一些外界干预的操作包含到临界区。如果进入了临界区却一直没有释放，同样也会引起其他线程的长时间等待。换句话说，在执行了EnterCriticalSection（）语句进入临界区后无论发生什么，必须确保与之匹配的LeaveCriticalSection（）都能够被执行到。可以通过添加结构化异常处理代码来确保LeaveCriticalSection
 （）语句的执行。虽然临界区同步速度很快，但却只能用来同步本进程内的线程，而不可用来同步多个进程中的线程。

      MFC为临界区提供有一个CCriticalSection类，使用该类进行线程同步处理是非常简单的，只需在线程函数中用CCriticalSection类成员函数 Lock（）和UnLock（）标定出被保护代码片段即可。对于上述代码，可通过CCriticalSection类将其改写如下：

       MFC应用(举例)

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// MFC临界区类对象
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)CCriticalSection g_cs;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// 共享资源 
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)char g_cArray[10];
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)UINT ThreadProc1(LPVOID pParam)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 进入临界区
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　g_cs.Lock();
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 对共享资源进行写入操作
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　for (int i = 0; i < 10; i++)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)　{
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　　g_cArray[i] = ’a’;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　　Sleep(1);
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)　}
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 离开临界区
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　g_cs.Unlock();
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　return 0;
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)UINT ThreadProc2(LPVOID pParam)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 进入临界区
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　g_cs.Lock();
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 对共享资源进行写入操作
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　for (int i = 0; i < 10; i++)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)　{
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　　g_cArray[10 - i - 1] = ’b’;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　　Sleep(1);
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)　}
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 离开临界区
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　g_cs.Unlock();
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　return 0;
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)void CSampleView::OnCriticalSectionMfc() 
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 启动线程
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　AfxBeginThread(ThreadProc1, NULL);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　AfxBeginThread(ThreadProc2, NULL);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 等待计算完毕
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　Sleep(300);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　// 报告计算结果
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　CString sResult = CString(g_cArray);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)　AfxMessageBox(sResult);
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
```

