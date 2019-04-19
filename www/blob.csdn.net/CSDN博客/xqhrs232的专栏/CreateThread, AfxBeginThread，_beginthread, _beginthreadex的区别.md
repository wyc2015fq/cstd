# CreateThread, AfxBeginThread，_beginthread, _beginthreadex的区别 - xqhrs232的专栏 - CSDN博客
2013年05月03日 13:45:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：683
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/miyunhong/article/details/4616982](http://blog.csdn.net/miyunhong/article/details/4616982)
**一、转载自: [http://www.cnblogs.com/chuncn/archive/2009/03/08/1406096.html](http://www.cnblogs.com/chuncn/archive/2009/03/08/1406096.html)**
**CreateThread**是Windows的API函数**(SDK函数**的标准形式,直截了当的创建方式，任何场合都可以使用)，提供操作系统级别的创建线程的操作，且仅限于工作者线程。不调用MFC和RTL的函数时，可以用CreateThread，其它情况不要轻易。在使用的过程中要考虑到进程的同步与互斥的关系（防止死锁）。线程函数定义为：DWORD WINAPI _yourThreadFun(LPVOID pParameter)。但它没有考虑：
（1）C Runtime中需要对多线程进行纪录和初始化，以保证C函数库工作正常（典型的例子是strtok函数）。
（2）MFC也需要知道新线程的创建，也需要做一些初始化工作（当然，如果没用MFC就没事了）。   
**AfxBeginThread**：MFC中线程创建的**MFC函数**，首先创建了相应的CWinThread对象，然后调用CWinThread::CreateThread,   在CWinThread::CreateThread中，完成了对线程对象的初始化工作，然后，调用_beginthreadex(AfxBeginThread相比较更为安全)创建线程。它简化了操作或让线程能够响应消息，即可用于界面线程，也可以用于工作者线程，但要注意不要在一个MFC程序中使用_beginthreadex()或CreateThread()。线程函数定义为：UINT
 _yourThreadFun(LPVOID pParam)
**_beginthreadex**：MS对C Runtime库的扩展**SDK函数**，首先针对C Runtime库做了一些初始化的工作，以保证C Runtime库工作正常。然后，调用CreateThread真正创建线程。  仅使用Runtime Library时，可以用_BegingThread。
uintptr_t _beginthread( 
   void( *start_address )( void * ),
   unsigned stack_size,
   void *arglist 
);
uintptr_t _beginthreadex( 
   void *security,
   unsigned stack_size,
   unsigned ( *start_address )( void * ),
   void *arglist,
   unsigned initflag,
   unsigned *thrdaddr 
);
_beginthreadex(   NULL,   0,   threadProc,   &pagram,   0,   
                                          (unsigned   int   *) idThread   );   
**小节：**实际上，这三个函数之间存在一定的调用关系，第一个纯粹一些，后两个完成自己相应的工作之后，调用前者实现线程的创建。其中CreateThread是由操作系统提供的接口，而AfxBeginThread和_BeginThread则是编译器对它的封装。
**小节：**用_beginthreadex()、_endthreadex函数应该是最佳选择，且都是C Run-time Library中的函数，函数的参数和数据类型都是C Run-time Library中的类型，这样在启动线程时就不需要进行Windows数据类型和C Run-time Library中的数据类型之间的转化，从而，减低了线程启动时的资源消耗和时间的消耗。但使用_beginthread，无法创建带有安全属性的新线程，无法创建暂停的线程，也无法获得 线程ID，_endthread的情况类似，它不带参数，这意味这线程的退出代码必须硬编码为0。
**小节**：MFC也是C++类库（只不过是Microsoft的C++类库，不是标准的C++类库），在MFC中也封装了new和delete两中运算符，所以用到new和delete的地方不一定非要使用_beginthreadex() 函数，用其他两个函数都可以。
_beginthreadex和_beginthread在回调入口函数之前进行一些线程相关的CRT的初始化操作。
CRT的函数库在线程出现之前就已经存在，所以原有的CRT不能真正支持线程，
这也导致了许多CRT的函数在多线程的情况下必须有特殊的支持，不能简单的使用CreateThread就OK。
二、转载自: [http://www.cppblog.com/bidepan2023/archive/2007/10/31/35627.html](http://www.cppblog.com/bidepan2023/archive/2007/10/31/35627.html)
当你打算实现一个多线程(非MFC)程序，你会选择一个单线程的CRT(C运行时库)吗？如果你的回答是NO, 那么会带来另外一个问题，你选择了CreateThread来创建一个线程吗? 大多数人也许会立刻回答YES。可是很不幸，这是错误的选择。
我先来说一下我的结论，待会告诉你为什么。
如果要作多线程(非MFC)程序，在主线程以外的任何线程内
- 使用malloc(),free(),new
- 调用stdio.h或io.h,包括fopen(),open(),getchar(),write(),printf(),errno
- 使用浮点变量和浮点运算函数
- 调用那些使用静态缓冲区的函数如: asctime(),strtok(),rand()等。
你就应该使用多线程的CRT并配合_beginthreadex(该函数只存在于多线程CRT), 其他情况下你可以使用单线程的CRT并配合CreateThread。
因为对产生的线程而言，_beginthreadex比之CreateThread会为上述操作多做额外的簿记工作，比如帮助strtok()为每个线程准备一份缓冲区。
然而多线程程序极少情况不使用上述那些函数(比如内存分配或者io)，所以与其每次都要思考是要使用_beginthreadex还是CreateThread,不如就一棍子敲定_beginthreadex。
当然你也许会借助win32来处理内存分配和Io，这时候你确实可以以单线程crt配合CreateThread，因为io的重任已经从crt转交给了win32。这时通常你应该使用HeapAlloc，HeapFree来处理内存分配，用CreateFile或者GetStdHandle来处理Io。
还有一点比较重要的是_beginthreadex传回的虽然是个unsigned long,其实是个线程Handle(事实上_beginthreadex在内部就是调用了CreateThread)，所以你应该用CloseHandle来结束他。千万不要使用ExitThread()来退出_beginthreadex创建的线程，那样会丧失释放簿记数据的机会，应该使用_endthreadex.
三、转载自:[http://blog.csdn.net/idau7/archive/2007/08/25/1758712.aspx](http://blog.csdn.net/idau7/archive/2007/08/25/1758712.aspx)
这两天刚好手头有点事情跟线程相关, 刚好细细拜读jjh先生所译的win32线程大作, 有点不知所云, 起码是被弄的一愣一愣的, 偏听则暗, 果然如此, 只知其然而不知所以然, 恐怕过两天还是会忘记的. 就当写写心得记录了.
1.         已知windows下可以用如下方法创建线程.
1)        CreadThread(…). API
2)        _beginthread(…). CRT
3)        _beginthreadex(…). CRT
4)        AfxBeginThread(…). MFC
由于AfxBeginThread()同学勾搭MFC, 扬言誓死不分, 开除先.
_beginthreadex()和_beginthread()长的很像, 没什么直接血缘关系, 但都是CRT所提供的线程创建方式. 显著区别在于参数和返回值上.
2.         _beginthread(…)与_beginthreadex(…)区别.
1)        参数区别.
2)        返回值区别
_beginthread()返回-1表示失败, 而_beginthreadex()返回0表示失败
3)        实际过程区别.
3.         _beginthreadex(…)与CreadThread(…)区别.
1)        参数区别.
两者参数基本相同, 只是CreadThread()的参数是windows定义的win32数据类型, 而_beginthreadex()是标准C/C++的数据类型. 需要注意的是, CreateThread()的第三个参数函数指针是unsign long的, 而_beginthreadex()的第三个参数函数指针是unsign int的.
2)        返回值区别.
CreateThread(), 返回的是创建的线程的HANDLE,
_beginthreadex(), 返回的一个是unsigned long. 需通过reinterpret_cast<HANDLE>或(HANDLE)来强制转换.
3)        实际过程区别.
的
       一般不建议直接调用CreateThread(), 除非可以非常确定
4.         选择_beginthreadex()和CreateThread()的一般性规则.
不建议使用CreateThread(). 尤其当线程:
l         使用了malloc()/free(), new/delete的线程.
l         调用stdio.h或io.h中声明的任何函数.
l         使用浮点变量.
5.         为什么winCE上只能用CreateThread()?
我也不知道为什么….
参考文献:
1.         MSJ(Mircsofts System Journal) July 1999
[http://www.microsoft.com/msj/0799/Win32/Win320799.aspx](http://www.microsoft.com/msj/0799/Win32/Win320799.aspx)
2.         <Win32多线程程序设计>
3.         <windows核心编程>. 6章.
4.         oRbIt 的专栏. <CreateThread()和_beginthreadex()区别>.
[http://blog.csdn.net/orbit/archive/2005/07/30/440118.aspx](http://blog.csdn.net/orbit/archive/2005/07/30/440118.aspx)
向远处看的专栏. <CreateThread()和_beginthreadex()区别>
[http://blog.csdn.net/xuxinshao/archive/2005/09/14/480797.aspx](http://blog.csdn.net/xuxinshao/archive/2005/09/14/480797.aspx)
搞不清谁的原创了….两位大仙都写原创…那…那肯定有个是引用的啊…要不难道是异灵事件?
5.         kind_li的专栏 kind_li 线程知识点.
[http://blog.csdn.net/kind_li/archive/2003/04/03/10998.aspx](http://blog.csdn.net/kind_li/archive/2003/04/03/10998.aspx)
6.         运行时: 管理进程和线程
[http://www-128.ibm.com/developerworks/cn/linux/sdk/rt/part7/index.html](http://www-128.ibm.com/developerworks/cn/linux/sdk/rt/part7/index.html)
7.         MSDN, _beginthreadex()跟_beginthread()的区别.
[http://msdn2.microsoft.com/en-us/library/kdzttdcb(VS.71).aspx](http://msdn2.microsoft.com/en-us/library/kdzttdcb(VS.71).aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/idau7/archive/2007/08/25/1758712.aspx](http://blog.csdn.net/idau7/archive/2007/08/25/1758712.aspx)
四、转载自 [http://820808.blog.51cto.com/328558/76160](http://820808.blog.51cto.com/328558/76160)
五、来自msdn: 
ms-help://MS.MSDNQTR.v80.chs/MS.MSDN.v80/MS.VisualStudio.v80.chs/dv_vccrt/html/0df64740-a978-4358-a88f-fb0702720091.htm
**[c-sharp]**[view
 plain](http://blog.csdn.net/miyunhong/article/details/4616982#)[copy](http://blog.csdn.net/miyunhong/article/details/4616982#)
- // The following example uses _beginthread and _endthread
- 
- // crt_BEGTHRD.C
- // compile with: /MT /D "_X86_" /c
- // processor: x86
- #include <windows.h>
- #include <process.h>    /* _beginthread, _endthread */
- #include <stddef.h>
- #include <stdlib.h>
- #include <conio.h>
- 
- void Bounce( void *ch );  
- void CheckKey( void *dummy );  
- 
- /* GetRandom returns a random integer between min and max. */
- #define GetRandom( min, max ) ((rand() % (int)(((max) + 1) - (min))) + (min))
- 
- BOOL repeat = TRUE;     /* Global repeat flag and video variable */
- HANDLE hStdOut;         /* Handle for console window */
- CONSOLE_SCREEN_BUFFER_INFO csbi;    /* Console information structure */
- 
- int main()  
- {  
-     CHAR    ch = 'A';  
- 
-     hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );  
- 
- /* Get display screen's text row and column information. */
-    GetConsoleScreenBufferInfo( hStdOut, &csbi );  
- 
- /* Launch CheckKey thread to check for terminating keystroke. */
-     _beginthread( CheckKey, 0, NULL );  
- 
- /* Loop until CheckKey terminates program. */
- while( repeat )  
-     {  
- /* On first loops, launch character threads. */
-         _beginthread( Bounce, 0, (void *) (ch++)  );  
- 
- /* Wait one second between loops. */
-         Sleep( 1000L );  
-     }  
- }  
- 
- /* CheckKey - Thread to wait for a keystroke, then clear repeat flag. */
- void CheckKey( void *dummy )  
- {  
-     _getch();  
-     repeat = 0;    /* _endthread implied */
- 
- }  
- 
- /* Bounce - Thread to create and and control a colored letter that moves
-  * around on the screen.
-  *
-  * Params: ch - the letter to be moved
-  */
- void Bounce( void *ch )  
- {  
- /* Generate letter and color attribute from thread argument. */
- char    blankcell = 0x20;  
- char    blockcell = (char) ch;  
-     BOOL    first = TRUE;  
-    COORD   oldcoord, newcoord;  
-    DWORD   result;  
- 
- 
- /* Seed random number generator and get initial location. */
-     srand( _threadid );  
-     newcoord.X = GetRandom( 0, csbi.dwSize.X - 1 );  
-     newcoord.Y = GetRandom( 0, csbi.dwSize.Y - 1 );  
- while( repeat )  
-     {  
- /* Pause between loops. */
-         Sleep( 100L );  
- 
- /* Blank out our old position on the screen, and draw new letter. */
- if( first )  
-             first = FALSE;  
- else
-          WriteConsoleOutputCharacter( hStdOut, &blankcell, 1, oldcoord, &result );  
-          WriteConsoleOutputCharacter( hStdOut, &blockcell, 1, newcoord, &result );  
- 
- /* Increment the coordinate for next placement of the block. */
-         oldcoord.X = newcoord.X;  
-         oldcoord.Y = newcoord.Y;  
-         newcoord.X += GetRandom( -1, 1 );  
-         newcoord.Y += GetRandom( -1, 1 );  
- 
- /* Correct placement (and beep) if about to go off the screen. */
- if( newcoord.X < 0 )  
-             newcoord.X = 1;  
- elseif( newcoord.X == csbi.dwSize.X )  
-             newcoord.X = csbi.dwSize.X - 2;  
- elseif( newcoord.Y < 0 )  
-             newcoord.Y = 1;  
- elseif( newcoord.Y == csbi.dwSize.Y )  
-             newcoord.Y = csbi.dwSize.Y - 2;  
- 
- /* If not at a screen border, continue, otherwise beep. */
- else
- continue;  
-         Beep( ((char) ch - 'A') * 100, 175 );  
-     }  
- /* _endthread given to terminate */
-     _endthread();  
- }  

**[c-sharp]**[view
 plain](http://blog.csdn.net/miyunhong/article/details/4616982#)[copy](http://blog.csdn.net/miyunhong/article/details/4616982#)
- /*The following sample code demonstrates how you can use the thread handle returned by _beginthreadex with the synchronization API WaitForSingleObject. The main thread waits for the second thread to terminate before it continues. When the second thread calls _endthreadex, it causes its thread object to go to the signaled state. This allows the primary thread to continue running. This cannot be done with _beginthread and _endthread, because _endthread calls CloseHandle, destroying the thread object before it can be set to the signaled state.*/
- 
- 
- // crt_begthrdex.cpp
- // compile with: /MT
- #include <windows.h>
- #include <stdio.h>
- #include <process.h>
- 
- unsigned Counter;   
- unsigned __stdcall SecondThreadFunc( void* pArguments )  
- {  
-     printf( "In second thread.../n" );  
- 
- while ( Counter < 1000000 )  
-         Counter++;  
- 
-     _endthreadex( 0 );  
- return 0;  
- }   
- 
- int main()  
- {   
-     HANDLE hThread;  
-     unsigned threadID;  
- 
-     printf( "Creating second thread.../n" );  
- 
- // Create the second thread.
-     hThread = (HANDLE)_beginthreadex( NULL, 0, &SecondThreadFunc, NULL, 0, &threadID );  
- 
- // Wait until second thread terminates. If you comment out the line
- // below, Counter will not be correct because the thread has not
- // terminated, and Counter most likely has not been incremented to
- // 1000000 yet.
-     WaitForSingleObject( hThread, INFINITE );  
-     printf( "Counter should be 1000000; it is-> %d/n", Counter );  
- // Destroy the thread object.
-     CloseHandle( hThread );  
- }  
msdn :
ms-help://MS.MSDNQTR.v80.chs/MS.MSDN.v80/MS.WIN32COM.v10.en/dllproc/base/creating_threads.htm
**[c-sharp]**[view
 plain](http://blog.csdn.net/miyunhong/article/details/4616982#)[copy](http://blog.csdn.net/miyunhong/article/details/4616982#)
- The CreateThread function creates a new thread for a process. The creating thread must specify the starting address of the code that the new thread is to execute. Typically, the starting address is the name of a function defined in the program code (for more information, see ThreadProc). This function takes a single parameter and returns a DWORD value. A process can have multiple threads simultaneously executing the same function.   
- 
- /*
- The following is a simple example that demonstrates how to create a new thread that executes the locally defined function, ThreadProc. The creating thread uses a dynamically allocated buffer to pass unique information to each instance of the thread function. It is the responsibility of the thread function to free the memory.
- 
- The calling thread uses the WaitForMultipleObjects function to persist until all worker threads have terminated. Note that if you were to close the handle to a worker thread before it terminated, this does not terminate the worker thread. However, the handle will be unavailable for use in subsequent function calls.*/
- 
- 
- #include <windows.h>
- #include <strsafe.h>
- 
- #define MAX_THREADS 3
- #define BUF_SIZE 255
- 
- typedef struct _MyData {  
- int val1;  
- int val2;  
- } MYDATA, *PMYDATA;  
- 
- DWORD WINAPI ThreadProc( LPVOID lpParam )   
- {   
-     HANDLE hStdout;  
-     PMYDATA pData;  
- 
-     TCHAR msgBuf[BUF_SIZE];  
-     size_t cchStringSize;  
-     DWORD dwChars;  
- 
-     hStdout = GetStdHandle(STD_OUTPUT_HANDLE);  
- if( hStdout == INVALID_HANDLE_VALUE )  
- return 1;  
- 
- // Cast the parameter to the correct data type.
- 
-     pData = (PMYDATA)lpParam;  
- 
- // Print the parameter values using thread-safe functions.
- 
-     StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d/n"),   
-         pData->val1, pData->val2);   
-     StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);  
-     WriteConsole(hStdout, msgBuf, cchStringSize, &dwChars, NULL);  
- 
- // Free the memory allocated by the caller for the thread 
- // data structure.
- 
-     HeapFree(GetProcessHeap(), 0, pData);  
- 
- return 0;   
- }   
- 
- void main()  
- {  
-     PMYDATA pData;  
-     DWORD dwThreadId[MAX_THREADS];  
-     HANDLE hThread[MAX_THREADS];   
- int i;  
- 
- // Create MAX_THREADS worker threads.
- 
- for( i=0; i<MAX_THREADS; i++ )  
-     {  
- // Allocate memory for thread data.
- 
-         pData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,  
- sizeof(MYDATA));  
- 
- if( pData == NULL )  
-             ExitProcess(2);  
- 
- // Generate unique data for each thread.
- 
-         pData->val1 = i;  
-         pData->val2 = i+100;  
- 
-         hThread[i] = CreateThread(   
-             NULL,              // default security attributes
-             0,                 // use default stack size  
-             ThreadProc,        // thread function 
-             pData,             // argument to thread function 
-             0,                 // use default creation flags 
-             &dwThreadId[i]);   // returns the thread identifier 
- 
- // Check the return value for success. 
- 
- if (hThread[i] == NULL)   
-         {  
-             ExitProcess(i);  
-         }  
-     }  
- 
- // Wait until all threads have terminated.
- 
-     WaitForMultipleObjects(MAX_THREADS, hThread, TRUE, INFINITE);  
- 
- // Close all thread handles upon completion.
- 
- for(i=0; i<MAX_THREADS; i++)  
-     {  
-         CloseHandle(hThread[i]);  
-     }  
- }  
