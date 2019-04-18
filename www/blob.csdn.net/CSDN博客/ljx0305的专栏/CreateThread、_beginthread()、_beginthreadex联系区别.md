# CreateThread、_beginthread()、_beginthreadex联系区别  - ljx0305的专栏 - CSDN博客
2009年10月29日 14:00:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：609标签：[thread																[library																[多线程																[服务器软件																[winapi																[signal](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器软件&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)
个人分类：[VC																[C++](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/401790)
CreateThread、_beginthread()、_beginthreadex联系区别
联系：
CreateThread、_beginthread和_beginthreadex都是用来启动线程的。
区别：
_beginthread是_beginthreadex的功能子集，_beginthreadex是微软的C/C++运行时库函数，CreateThread是操作系统的函数。虽然_beginthread内部是调用_beginthreadex但他屏蔽了象安全特性这样的功能，所以_beginthread与CreateThread不是同等级别，_beginthreadex和CreateThread在功能上完全可替代，我们就来比较一下_beginthreadex与CreateThread!  
<<Windows核心编程>>中有很详细地介绍：
注意：若要创建一个新线程，绝对不要使用CreateThread，而应使用_beginthreadex.   
  Why?考虑标准C运行时库的一些变量和函数，如errno，这是一个全局变量。全局变量用于   
  多线程会出什么事，你一定知道的了。故必须存在一种机制，使得每个线程能够引用它自己的   
  errno变量，又不触及另一线程的errno变量._beginthreadex就为每个线程分配自己的   
  tiddata内存结构。该结构保存了许多像errno这样的变量和函数的值、地址（自己看去吧）。   
  通过线程局部存储将tiddata与线程联系起来。具体实现在Threadex.c中有。   
  结束线程使用函数_endthreadex函数，释放掉线程的tiddata数据块。   
  CRT的函数库在线程出现之前就已经存在，所以原有的CRT不能真正支持线程，这导致我们在编程的时候有了CRT库的选择，在MSDN中查阅CRT的函数时都有：   
  Libraries   
  LIBC.LIB   Single   thread   static   library,   retail   version     
  LIBCMT.LIB   Multithread   static   library,   retail   version     
  MSVCRT.LIB   Import   library   for   MSVCRT.DLL,   retail   version     
  这样的提示！   
  对于线程的支持是后来的事！   
  这也导致了许多CRT的函数在多线程的情况下必须有特殊的支持，不能简单的使用CreateThread就OK。   
  大多的CRT函数都可以在CreateThread线程中使用，看资料说只有signal()函数不可以，会导致进程终止！但可以用并不是说没有问题！   
  有些CRT的函数象malloc(),   fopen(),   _open(),   strtok(),   ctime(),   或localtime()等函数需要专门的线程局部存储的数据块，这个数据块通常需要在创建线程的时候就建立，如果使用CreateThread，这个数据块就没有建立，然后会怎样呢？在这样的线程中还是可以使用这些函数而且没有出错，实际上函数发现这个数据块的指针为空时，会自己建立一个，然后将其与线程联系在一起，这意味着如果你用CreateThread来创建线程，然后使用这样的函数，会有一块内存在不知不觉中创建，遗憾的是，这些函数并不将其删除，而CreateThread和ExitThread也无法知道这件事，于是就会有Memory   Leak，在线程频繁启动的软件中(比如某些服务器软件)，迟早会让系统的内存资源耗尽！   
  _beginthreadex(内部也调用CreateThread)和_endthreadex就对这个内存块做了处理，所以没有问题！(不会有人故意用CreateThread创建然后用_endthreadex终止吧，而且线程的终止最好不要显式的调用终止函数，自然退出最好！)   
  谈到Handle的问题，_beginthread的对应函数_endthread自动的调用了CloseHandle，而_beginthreadex的对应函数_endthreadex则没有，所以CloseHandle无论如何都是要调用的不过_endthread可以帮你执行自己不必写，其他两种就需要自己写！(Jeffrey   Richter强烈推荐尽量不用显式的终止函数，用自然退出的方式，自然退出当然就一定要自己写CloseHandle) 
举例：
//   create/destroy   
  BOOL   CXThread::create   (   void   *   pThreadData   )   
  {   
  //   get   ptr   to   data   
  m_pThreadData   =   pThreadData;   
  //   create   events   
  if   (   !createThreadEvents()   )   
  return   FALSE;   
  //   create   thread   
  #if   defined(UseWin32Thread)   
  m_hThread   =     
  CreateThread(NULL,0,   CXThread::threadProc,   (LPVOID)   this,   0,   &m_idThread   );   
  #else   
  m_hThread   =   (HANDLE)   
  _beginthreadex(   NULL,   0,   CXThread::threadProc,   (LPVOID)   this,   0,   
                                          (unsigned   int   *)   &m_idThread   );   
  #endif   
  if   (   m_hThread   ==   0   )   
  return   FALSE;   
  //   success   so   start   thread   
  //::ResumeThread(   m_hThread   );   
  return   TRUE;   
  }   
  //   thread   proc   
  #if   defined(UseWin32Thread)   
  DWORD   WINAPI CXThread::threadProc   (   LPVOID   parameter   )   
  #else   
  unsigned   _stdcall   CXThread::threadProc   (   LPVOID   parameter   )   
  #endif   
  {   
  if   (   !parameter   )   
  return   XTHREAD_NORMAL;   
  //   start   thread   
  CXThread   *   pThread   =   (CXThread   *)   parameter;   
  int   ret   =   pThread->run();   
  //   exit   the   thread   
  #if   defined(UseWin32Thread)   
  ExitThread(   XTHREAD_NORMAL);   
  #else   
  _endthreadex(   XTHREAD_NORMAL);   
  #endif   
  return   ret;   
  }  
引用:http://blog.chinaunix.net/u3/103892/showart_2048099.html
