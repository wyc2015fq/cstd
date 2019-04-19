# AfxBeginThread和CreateThread具体区别(转载) - xqhrs232的专栏 - CSDN博客
2010年01月08日 16:41:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：972标签：[mfc																[winapi																[编译器																[windows																[library																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
具体说来，CreateThread这个 函数是windows提供给用户的 API函数，是SDK的标准形式，在使用的过程中要考虑到进程的同步与互斥的关系，进程间的同步互斥等一系列会导致操作系统死锁的因素，用起来比较繁琐一些，初学的人在用到的时候可能会产生不可预料的错误，建议多使用AfxBeginThread，是编译器对原来的CreateThread函数的封装，用与MFC编程（当然，只要修改了项目属性，console和win32项目都能调用）而_beginthread是C的运行库函数。               
 在使用AfxBeginThread时，线程函数的定义为：UINT   _yourThreadFun(LPVOID   pParam)参数必须如此                在使用CreateThread时，线程的函数定义为：  DWORD  WINAPI  _yourThreadFun(LPVOID pParameter)。
        两个的实质都是一样的，不过AfxBeginThread返回一个CWinThread的指针，就是说他会new一个CWinThread对象，而且这个对象是自动删除的（在线程运行结束时），给我们带来的不便就是无法获得它的状态，因为随时都有可能这个指针指向的是一个已经无效的内存区域，所以使用时（如果需要了解它的运行状况的话）首先CREATE_SUSPENDED让他挂起，然后m_bAutoDelete=FALSE，接着才ResumeThread，最后不要了delete那个指针。     CreatThread就方便多了，它返回的是一个句柄，如果你不使用CloseHandle的话就可以通过他安全的了解线程状态，最后不要的时候CloseHandle，Windows才会释放资源，所以我一般使用CreatThread，方便。
如果用MFC编程，不要用CreateThread，如果只是使用Runtime Library，用_BegingThread，总之，不要轻易使用CreateThread。这是因为在MFC和RTL中的函数有可能会用到些它们所封装的公用变量，也就是说AfxBeginThread和_BeginThread都有自己的启动代码是CreateThread所没有的。在用CreateThread所创建的线程中使用MFC的类和RTL函数就有可能出现问题。如果你是用汇编编写win32程序并且在线程函数中也不调用MFC和RTL的函数，那用CreateThread就没问题，或者你虽然是用C写线程函数，但你很小心没调用RTL函数也不会有问题。
　　CreateThread是由操作系统提供的接口，而AfxBeginThread和_BeginThread则是编译器对它的封装。
　　在可能的情况下，不要调用_beginthread,而应该调用_beginthreadex。以及对应的_endthreadex。这都是C++运行期函数。但是使用_beginthread，无法创建带有安全属性的新线程，无法创建暂停的线程，也无法获得线程ID，_endthread的情况类似，它不带参数，
　　这意味这线程的退出代码必须硬编码为0。这两个函数在_beginthreadex和_endthreadex中进行调用。CreateThread不要进行直接调用。
　　写的有点乱，呵呵
