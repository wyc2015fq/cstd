# 线程函数的设计以及MsgWaitForMultipleObjects函数的使用要点 - ljx0305的专栏 - CSDN博客
2010年01月11日 22:28:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：662标签：[object																[null																[工作																[timer																[mfc																[thread](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)
个人分类：[MFC																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403913)
使用多线程技术可以显著地提高程序性能，本文就讲讲在程序中如何使用工作线程，以及工作线程与主线程通讯的问题。
一创建线程
使用MFC提供的全局函数AfxBeginThread()即可创建一个工作线程。线程函数的标准形式为 UINT MyFunProc(LPVOID );此函数既可以是全局函数，也可以是类的静态成员函数。之所以必须是静态成员函数，是由于类的非静态成员函数，编译器在编译时会自动加上一个this指针参数，如果将函数设置为静态的成员函数，则可以消除this指针参数。如果想在线程函数中任意调用类的成员变量（此处指的是数据成员，而不是控件关联的成员变量），则可以将类的指针作为参数传递给线程函数，然后经由该指针，就可以调用类的成员变量了。
//线程函数，类的静态成员函数
UINT CThreadTest::TH_SetProgress(LPVOID lpVoid)
{
CThreadTest *pTest=(CThreadTest *)lpVoid;
pTest->SetProgress();
return 0;
}
//类的成员函数，此函数执行实际的线程函数操作，却可以自如的调用成员数据
void CThreadTest::SetProgress()
{
int nCount=0;
while (1)
{
m_progress.SetPos(nCount); //设置进度条进度
//this->SendMessage(WM_SETPROGRESSPOS,nCount,0);//也可以采用这种方式设置
nCount++;
if (g_exitThread)
{
return;
}
Sleep(200);
}
}
二线程函数体的设计
有过多线程设计经验的人都有体会，多线程设计最重要的就是要处理好线程间的同步和通讯问题。如解决不好这个问题，会给程序带来潜藏的隐患。线程的同步可以利用临界区、事件、互斥体和信号量来实现，线程间的通讯可利用全局变量和发消息的形式实现。其中事件和临界区是使用得比较多的工具。请看下面的线程函数体：
UINT AnalyseProc(LPVOID lVOID)
{
if(WAIT_OBJECT_0== WaitForSingleObject(m_eventStartAnalyse.m_hThread,INFINITE))
{
while (WAIT_OBJECT_0 == WaitForSingleObject(m_eventExitAnalyse.m_hThread,0))
{
DWORD dRet=WaitForSingleObject(m_eventPause.m_hThread,0);
if (dRet == WAIT_OBJECT_0)
{
//暂停分析
Sleep(10);
}
else if (dRet == WAIT_TIMEOUT)
{
//继续分析
// 
}
}
}
return 0;
}
上面的线程函数用到了三个事件变量eventStartAnalyse、eventExitAnalyse和eventPause，分别用来控制线程函数的启动、退出以及暂停。再配以WaitForSingleObject函数，就可以自如的控制线程函数的执行，这是在线程函数体内应用事件变量的典型方式，也是推荐的方式。
无论是工作线程还是用户界面线程，都有消息队列，都可以接收别的线程发过来的消息也可以给别的线程发送消息。给工作线程发消息使用的函数是PostThreadMessage()。此函数的第一个参数是接收消息的线程的ID。此函数是异步执行的，机制和PostMessage一样，就是把消息抛出后就立即返回，不理会消息是否被处理完了。
这里还有着重强调一点，线程消息队列是操作系统帮我们维护的一种资源，所以它的容量也是有限制的。笔者曾经做过实验，在5~6秒事件内调用PostThreadMessage往线程消息队列里发送5万多条消息，可是由于线程函数处理消息的速度远慢于发送速度，结果导致线程消息队列里已经堆满了消息，而发送端还在发消息，最终导致消息队列溢出，很多消息都丢失了。所以，如果你要在短时间内往线程消息队列里发送很多条消息，那就要判断一下PostThreadMessage函数的返回值。当消息队列已经溢出时，此函数返回一个错误值。根据返回值，你就可以控制是否继续发送。
工作线程给主线程发消息使用的是SendMessage和PoseMessage函数。这两个函数的区别在于SendMessage函数是阻塞方式，而PoseMessage函数是非阻塞方式。如果不是严格要求工作线程与主线程必须同步执行，则推荐使用PoseMessage。不要在线程函数体内操作MFC控件，因为每个线程都有自己的线程模块状态映射表，在一个线程中操作另一个线程中创建的MFC对象，会带来意想不到的问题。更不要在线程函数里，直接调用UpdataData()函数更新用户界面，这会导致程序直接crash。而应该通过发送消息给主线程的方式，在主线程的消息响应函数里操作控件。上面提到的SetProgress函数和AnalyseProc函数均为线程函数，但它们都不能接收别的线程发过来的消息，虽然它们都可以给主线程发消息。它们要想能够接收别的线程发过来的消息，则必须调用GetMessage或PeekMessage函数。这两个函数的主要区别在于：
GetMessage函数可以从消息队列中抓取消息，当抓取到消息后，GetMessage函数会将此条消息从消息队列中删除。而且，如果消息队列中没有消息，则GetMessage函数不会返回，CPU转而回去执行别的线程，释放控制权。GetMessage返回的条件是抓取的消息是WM_QUIT。
PeekMessage函数也可以从消息队列中抓取消息，如果它的最后一个参数设置为PM_NOREMOVE，则不从消息队列中删除此条消息，此条消息会一直保留在消息队列中。如果它的最后一个参数是PM_REMOVE，则会删除此条消息。如果消息队列中没有消息，则PeekMessage函数会立刻返回，而不是像GetMessage一样就那样等在那儿。PeekMessage函数就像是窥探一下消息队列，看看有没有消息，有的话就处理，没有就离开了。这一点也是两个函数的最大不同。下面的代码演示了在线程函数中使用这两个函数的三种方式，这三种方法可以达到同样的效果：
void CThreadTest::SetSlider()
{
// 在线程函数里启动一个时钟，每50毫秒发送一个WM_TIMER消息
int nTimerID=::SetTimer(NULL,1,50,NULL);
int nSliderPos=0;
MSG msg;
while (1)
{
//方式一使用GetMessage函数
/*if (::GetMessage(&msg,NULL,0,0))
{
switch(msg.message)
{
case WM_TIMER:
{
nSliderPos++;
::SendMessage(this->m_hWnd,WM_SETSLIDERPOS,nSliderPos,0);
}
break;
case WM_QUIT_THREAD: //自定义消息
{
::KillTimer(NULL,1);
return;
}
break;
default:
break;
}
}
 */
//方式二使用PeekMessage函数
/*if (::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
{
switch(msg.message)
{
case WM_TIMER:
{
nSliderPos++;
::SendMessage(this->m_hWnd,WM_SETSLIDERPOS,nSliderPos,0);
}
break;
case WM_QUIT_THREAD: //自定义消息
{
::KillTimer(NULL,1);
return;
}
break;
default:
break;
}
}
else
{
//必须有此操作，要不然当没有消息到来时，线程函数相当于陷
//入空循环，cpu的占有率会飙升
Sleep(20);
}
*/
//方式三同时使用PeekMessage和GetMessage函数
if (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
{
if(::GetMessage(&msg,NULL,0,0))
{
switch(msg.message)
{
case WM_TIMER:
{
nSliderPos++; ::SendMessage(this->m_hWnd,WM_SETSLIDERPOS,nSliderPos,0);
}
break;
case WM_QUIT_THREAD: //自定义消息
{
::KillTimer(NULL,1);
return;
}
break;
default:
break;
}
}
}
else
{
Sleep(20);
}
}
}
前面已经介绍过了，不建议线程函数里用SendMessage给主线程发消息，因为这个函数是同步操作，就是如果SendMessage函数不执行完，是不会返回的，这样线程函数就无法继续执行。有时这种操作容易导致工作线程和主线程死锁，这个我们后面会谈到，会介绍一种解决方法。
三线程的退出
线程的退出有多种方式，比如可以调用TerminateThread()函数强制线程退出，但不推荐这种方式，因为这样做会导致线程中的资源来不及释放。最好的也是推荐的方式，是让线程函数自己退出。就像上面介绍的SetProgress()函数中，用全局变量g_exitThread使线程退出。
而AnalyseProc用WAIT_OBJECT_0 ==WaitForSingleObject(m_eventExitAnalyse.m_hThread,0)这种方式来退出线程，还有在SetSlider函数中利用发送自定义消息WM_QUIT_THREAD的方式令线程退出。这些都是可以使用的方法。
当主线程要退出时，为了能保证线程的资源能全部地释放，主线程必须等待工作线程退出。线程对象和进程对象一样，也是内核对象，而且线程对象的特点是当线程退出时，线程内核对象会自动变为有信号状态，能够唤醒所有正在等待它的线程。我们通常都习惯于使用WaitForSingleObject等函数来等待某个内核对象变为有信号状态，但是我想说的是，在主线程中不要使用WaitForSingleObject和WaitForMultipleObjects两个函数等待线程退出，其原因就是有导致程序死锁的隐患，特别是线程函数里调用了SendMessage或是直接操作了MFC对象，更易出现此种现象。下面的函数是一个在主线程中用来等待SetProgress()线程函数退出的函数：
//退出线程
void CThreadTest::OnButton2() 
{
g_exitThread=TRUE; //设置全局变量为真，令线程退出
#if 1
WaitForSingleObject(m_pThread1->m_hThread,INFINITE); //无限等待
#else
DWORD dRet;
MSG msg;
while (1)
{
dRet=::MsgWaitForMultipleObjects(1,&m_pThread1->m_hThread,FALSE,INFINITE,QS_ALLINPUT);
if (dRet == WAIT_OBJECT_0+1)
{
while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}
}
else
{
break;
}
}
#endif
}
在上面的函数中我用#if #else #endif这组预编译指令控制函数的执行代码，如果我令#if 1，则执行WaitForSingleObject函数，如果我令#if 0，则执行DWORD dRet路径。首先令#if  1，测试会发现，程序死锁了。原因是当程序执行到WaitForSingleObject函数时，主线程挂起，等待线程函数退出，此时CPU切换到线程函数体内执行，如果执行到if (g_exitThread)处，则线程函数顺利退出，可如果执行到m_progress.SetPos(nCount)处，由于SetPos函数是在主线程中完成的操作，Windows是基于消息的操作系统，很多操作都是靠发消息完成的，由于主线程已经挂起，所以没有机会去消息队列中抓取消息并处理它，结果导致SetPos函数不会返回，工作线程也被挂起，典型的死锁。如果不用m_progress.SetPos，而改用this->SendMessage(…),其结果是一样的。此时如果用了PostMessage，则工作线程会顺利退出，因为PostMessage是异步执行的。由此可见，在主线程中用WaitForSingleObject等待工作线程退出是有很大隐患的。
为解决这一问题，微软特提供了一个MsgWaitForMultipleObjects函数，该函数的特点是它不但可以等待内核对象，还可以等消息。也就是当有消息到来时，该函数也一样可以返回，并处理消息，这样就给了工作线程退出的机会。
**DWORD MsgWaitForMultipleObjects(****DWORD***nCount***,** //要等待的*内核*对象数目**LPHANDLE***pHandles***,** //要等待的*内核*对象句柄数组指针**BOOL***fWaitAll***,** //是等待全部对象还是单个对象**DWORD***dwMilliseconds***,//****等待时间****DWORD***dwWakeMask***);//****等待的消息类型**下面就详解一下该函数的参数使用方法：**DWORD***nCount**：要等待的内核对象的数目。如果等待两个线程退出，则nCount=2；***LPHANDLE***pHandles*：要等待的*内核*对象句柄数组指针。如果只要等待一个线程退出，则直接设置该线程句柄的指针即可：MsgWaitForMultipleObjects(1,&m_pThread->m_hThread,…)如果要等待两个线程退出，则使用方法为:HANDLE hArray[2]={m_pThread1->m_hThread ,m_pThread2->m_hThread }；MsgWaitForMultipleObjects(2,hArray,…)**BOOL***fWaitAll**： *TRUE-表示只有要等待的线程全部退出后，此函数才返回，FALSE-表示要等待的线程中任意一个退出了，或是有消息到达了，此函数均会返回。在上面的OnButton2()函数中，我要等待一个线程退出，将fWaitAll设置为FALSE，目的是无论是线程真的退出了，还是有消息到达了，该函数都能返回。如果将该fWaitAll设置为TRUE，那么函数返回的唯一条件是线程退出了，即便是有消息到来了，该函数也一样不会返回。**DWORD***dwMilliseconds**：等待的事件，单位是毫秒。可以设置为*INFINITE，无穷等待**DWORD***dwWakeMask**：等待的消息类型，通常可以设置为*QS_ALLINPUT。此宏表示的是可以等待任意类型的消息。当然，也可以指定等待的消息类型。#define QS_ALLINPUT(QS_INPUT| /QS_POSTMESSAGE| /QS_TIMER| /QS_PAINT| /QS_HOTKEY| /QS_SENDMESSAGE)
返回值：DWORD dRet 通过函数返回值，可以得到一些有效信息。函数返回值依*fWaitAll**设置的不同而有所不同。*下面是函数返回值的几种常见类型：
**dRet =** 0xFFFFFFFF**：**表示函数调用失败，可用GetLastError()得到具体的出错信息;
**dRet =**WAIT_OBJECT_0+*nCount**：表示有消息到达了；*
**如果***fWaitAll**设置为TRUE*
**dRet =** WAIT_OBJECT_0,表示所有等待的核心对象都激发了，或是线程都退出了；
**如果***fWaitAll**设置为FALSE*
**dRet =** WAIT_OBJECT_0 ~ WAIT_OBJECT_0+*nCount-1**：表示等待的内核对象被激发了，index=***dRet - **WAIT_OBJECT_0，表示hArray[]数组中索引为*index**的那个对象被激发了。*
*当函数由于消息到来而返回，则需要用户主动去消息队列中将消息抓取出来，然后派发出去，这样该消息就会被处理了。其具体的操作就是：*
while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}
下面再看一个用这个函数等待两个线程退出的例子：
//关闭线程1和2
void CThreadTest::OnButton6() 
{
…
…
DWORD dRet=-2;
HANDLE hArray[2]; 
hArray[0]=m_pThread1->m_hThread;
hArray[1]=m_pThread2->m_hThread;
MSG msg;
int nExitThreadCount=0;//标记已经有几个线程退出了
BOOL bWaitAll=FALSE; 
int nWaitCount=2;//初始等待的线程数目
while (1)
{
dRet=MsgWaitForMultipleObjects(nWaitCount,hArray,bWaitAll,INFINITE,QS_ALLINPUT);
if (dRet == WAIT_OBJECT_0+ nWaitCount)
{
TRACE("收到消息,函数返回值为%d /n",dRet);
while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}
}
else if (dRet >= WAIT_OBJECT_0 && dRet < WAIT_OBJECT_0+ nWaitCount)
{
nExitThreadCount++;
if (nExitThreadCount == 1)
{
TRACE("一个线程退出了/n");
int nIndex=dRet-WAIT_OBJECT_0;
hArray[nIndex]=hArray[nWaitCount-1];
hArray[nWaitCount-1]=NULL;
nWaitCount--;
}
else 
{
TRACE("两个线程都退出了/n");
break;
}
}
else
{
DWORD dErrCode=GetLastError();
…
break;
}
}
}
在上面这个例子中，我将bWaitAll设置为FALSE，目的是当我要等待的两个线程中由一个退出了，或是有消息到来了，此函数都可以退出。如果我将此参数设置为TRUE，那么，当且仅当我要等待的两个线程均退出了，这个函数才会返回，这种使用方法有是程序陷入死锁的危险，故应避免。无论是等待一个还是多个线程，只需将此参数设置为FALSE即可，然后通过函数返回值判断究竟是那个返回了，还是消息到达了即可。这一要点前面已有陈述，此处再强调一遍。
通过函数返回值可以得知究竟哪个线程退出了，当要等待的两个线程中的一个已经退出后，则应该从新设置等待函数的参数，对等待的句柄数组进行整理。
{
int nIndex=dRet-WAIT_OBJECT_0;
hArray[nIndex]=hArray[nWaitCount-1];
hArray[nWaitCount-1]=NULL;
nWaitCount--;
}
这组语句就是用来从新设置参数的，其过程就是将等待的总数目减一，并将刚退出的线程的句柄设置为NULL,移到数组的最末位置。
上面介绍了线程函数的设计以及在主线程中等待工作线程退出的方法，着重介绍了MsgWaitForMultipleObjects函数的使用要点，希望对大家有所帮助，也希望大家能提宝贵意见，补我之不足，愿与大家共同进步。
引用:http://www.cppblog.com/fwxjj/archive/2009/04/01/78519.html
