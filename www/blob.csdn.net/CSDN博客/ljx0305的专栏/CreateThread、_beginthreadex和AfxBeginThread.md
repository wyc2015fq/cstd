# CreateThread、_beginthreadex和AfxBeginThread - ljx0305的专栏 - CSDN博客
2009年09月21日 17:39:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3631
瑶瑶按：
创建线程好几个函数可以使用，可是它们有什么区别，适用于什么情况呢？
参考了一些资料，写得都挺好的，这里做一些摘抄和整合。
【参考1】[CreateThread, AfxBeginThread，_beginthread, _beginthreadex的区别](http://www.cnblogs.com/chuncn/archive/2009/03/08/1406096.html) =====================================================================
1、CreateThread——Windows的API函数
2、_beginthreadex——MS对C Runtime库的扩展SDK函数
3、AfxBeginThread——MFC中线程创建的MFC函数
**CreateThread**（API函数：SDK函数的标准形式,直截了当的创建方式，任何场合都可以使用。）
提供操作系统级别的创建线程的操作，且仅限于工作者线程。不调用MFC和RTL的函数时，可以用CreateThread，其它情况不要轻易。在使用的过程中要考虑到进程的同步与互斥的关系（防止死锁）。
线程函数定义为：DWORD WINAPI _yourThreadFun(LPVOID pParameter)。
但它没有考虑：
（1）C Runtime中需要对多线程进行纪录和初始化，以保证C函数库工作正常（典型的例子是strtok函数）。
（2）MFC也需要知道新线程的创建，也需要做一些初始化工作（当然，如果没用MFC就没事了）。  
**_beginthreadex**
MS对C Runtime库的扩展SDK函数，首先针对C Runtime库做了一些初始化的工作，以保证C Runtime库工作正常。然后，调用CreateThread真正创建线程。 仅使用Runtime Library时，可以用_BegingThread。
**AfxBeginThread**MFC中线程创建的MFC函数，首先创建了相应的CWinThread对象，然后调用CWinThread::CreateThread,   在CWinThread::CreateThread中，完成了对线程对象的初始化工作，然后，调用_beginthreadex(AfxBeginThread相比较更为安全)创建线程。它简化了操作或让线程能够响应消息，即可用于界面线程，也可以用于工作者线程，但要注意不要在一个MFC程序中使用_beginthreadex()或CreateThread()。
线程函数定义为：UINT _yourThreadFun(LPVOID pParam)
=====================================================================
【参考2】[CreateThread与_beginthreadex](http://hi.bccn.net/space.php?uid=251633&do=blog&id=12911)
=====================================================================
**CreateThread**
“CreateThread函数是用来创建线程的Windows函数不过，如果你正在编写C/C++代码，决不应该调用CreateThread。相反，应该使用Visual C++运行期库函数_beginthreadex。如果不使用Microsoft的Visual C++编译器，你的编译器供应商有它自己的CreateThred替代函数。不管这个替代函数是什么，你都必须使用。”
**_beginthreadex**
"_beginthreadex函数的参数列表与CreateThread函数的参数列表是相同的，但是参数名和类型并不完全相同。这是因为 Microsoft的C/C++运行期库的开发小组认为， C/C++运行期函数不应该对Windows数据类型有任何依赖。_beginthreadex函数也像CreateThread那样，返回新创建的线程的句柄。因此，如果调用源代码中的CreateThread，就很容易用对_beginthreadex的调用全局取代所有这些调用。不过，由于数据类型并不完全相同，所以必须进行某种转换，使编译器运行得顺利些。" 
"下面是关于_beginthreadex的一些要点： 
1） 每个线程均获得由C/C++运行期库的堆栈分配的自己的tiddata内存结构。（tiddata结构位于Mtdll.h文件中的Visual C++源代码中）。 
2） 传递给_beginthreadex的线程函数的地址保存在tiddata内存块中。传递给该函数的参数也保存在该数据块中。 
3） _beginthreadex确实从内部调用CreateThread，因为这是操作系统了解如何创建新线程的唯一方法。 
4） 当调用CreatetThread时，它被告知通过调用_threadstartex而不是pfnStartAddr来启动执行新线程。还有，传递给线程函数的参数是tiddata结构而不是pvParam的地址。 
5） 如果一切顺利，就会像CreateThread那样返回线程句柄。如果任何操作失败了，便返回NULL。"
**为什么？**
　　“也许你想知道，如果调用CreateThread，而不是调用C/C++运行期库的_beginthreadex来创建新线程，将会发生什么情况。当一个线程调用要求tiddata结构的C/C++运行期库函数时，将会发生下面的一些情况（大多数C/C++运行期库函数都是线程安全函数，不需要该结构）。
　　首先，C/C++运行期库函数试图(通过调用TlsGetValue)获取线程的数据块的地址。如果返回NULL作为tiddata块的地址，调用线程就不拥有与该地址相关的tiddata块。这时，C/C++运行期库函数就在现场为调用线程分配一个tiddata块，并对它进行初始化。然后该 tiddata块（通过TlsSetValue）与线程相关联。此时，只要线程在运行，该tiddata将与线程待在一起。这时，C/C++运行期库函数就可以使用线程的tiddata块，而且将来被调用的所有C/C++运行期函数也能使用tiddata块。 
　　当然，这看来有些奇怪，因为线程运行时几乎没有任何障碍。不过，实际上还是存在一些问题。首先，如果线程使用C/C++运行期库的signal函数，那么整个进程就会终止运行，因为结构化异常处理帧尚未准备好。第二，如果不是调用_endthreadex来终止线程的运行，那么数据块就不会被撤消，内存泄漏就会出现（那么谁还为使用CreateThread函数创建的线程来调用_endthreadex呢？）。 
　　 注意如果程序模块链接到多线程DLL版本的C/C++运行期库，那么当线程终止运行并释放tiddata块（如果已经分配了tiddata块的话）时，该运行期库会收到一个DLL_THREAD_DETACH通知。尽管这可以防止tiddata块的泄漏，但是强烈建议使用_beginthreadex而不是使用Createthread来创建线程。
=====================================================================
【参考3】[关于_beginthreadex和CreateThread的区别](http://hi.baidu.com/%E4%EC%C8%F7%D2%BB%BB%E1/blog/item/22cc88ea72bc77d5d539c996.html)
=====================================================================
　　在 Win32 API 中，创建线程的基本函数是 CreateThread，而 _beginthread(ex) 是C++ 运行库的函数。为什么要有两个呢？因为C++ 运行库里面有一些函数使用了全局量，如果使用 CreateThread 的情况下使用这些C++ 运行库的函数，就会出现不安全的问题。而 _beginthreadex 为这些全局变量做了处理，使得每个线程都有一份独立的“全局”量。
　　所以，如果你的编程只调用 Win32 API/SDK ，就放心用 CreateThread；如果要用到C++ 运行时间库，那么就要使用 _beginthreadex ，并且需要在编译环境中选择 Use MultiThread Lib/DLL。
　　通常他们的解释都是这容易造成内存泄漏。这个解释本身是没有错的，但是解释得不够完全和详细。以至于造成很多新手盲目的信任了那句话，在那里都是用_beginthreadex函数，或者是装作没有看到使用CreateThread函数。曾经有一段时间我也对这个问题很是困惑，不知道到底用那个才是对的。因为我不止一次在很多权威性的代码中看到对CreateThread函数的直接调用。难道是权威错了？？ 抱着怀疑的态度查找了大量的资料和书籍，终于搞明白了这个问题的关键所在，在此做个说明，算是对那句话的一个完善。
　　关于_beginthreadex和CreateThread的区别我就不做说明了，这个很容易找到的。我们只要知道一个问题：_beginthreadex是一个C运行时库的函数，CreateThread是一个系统API函数，_beginthreadex内部调用了CreateThread。只所以所有的书都强调内存泄漏的问题是因为_beginthreadex函数在创建线程的时候分配了一个堆结构并和线程本身关联起来，我们把这个结构叫做tiddata结构，是通过线程本地存储器TLS于线程本身关联起来。我们传入的线程入口函数就保存在这个结构中。tiddata的作用除了保存线程函数入口地址之外，还有一个重要的作用就是:C运行时库中有些函数需要通过这个结构来保存和获取一些数据，比如说errno之类的线程全局变量。这点才是最重要的。
　　当一个线程调用一个要求tiddata结构的运行时库函数的时候，将发生下面的情况：
　　运行时库函数试图TlsGetv　alue获取线程数据块的地址，如果没有获取到，函数就会现场分配一个 tiddata结构，并且和线程相关联，于是问题出现了，如果不通过_endthreadex函数来终结线程的话，这个结构将不会被撤销，内存泄漏就会出现了。但通常情况下，我们都不推荐使用_endthreadex函数来结束线程，因为里面包含了ExitThread调用。
　　找到了内存泄漏的具体原因，我们可以这样说：只要在创建的线程里面不使用一些要求tiddata结构的运行时库函数，我们的内存时安全的。所以，前面说的那句话应该这样说才完善：
　　“绝对不要调用系统自带的CreateThread函数创建新的线程，而应该使用_beginthreadex，除非你在线程中绝不使用需要tiddata结构的运行时库函数”
　　这个需要tiddata结构的函数有点麻烦了，在侯捷的《win32多线程程序设计》一书中这样说到：
　　　如果在除主线程之外的任何线程中进行一下操作，你就应该使用多线程版本的C runtime library,并使用_beginthreadex和_endthreadex：
　　　1 使用malloc()和free()，或是new和delete
　　　2 使用stdio.h或io.h里面声明的任何函数
　　　3 使用浮点变量或浮点运算函数
　　　4 调用任何一个使用了静态缓冲区的runtime函数，比如:asctime(),strtok()或rand()
=====================================================================
【参考4】[_beginthreadex、CreateThread、AfxBeginThread的选择](http://blog.csdn.net/up5p5mm/archive/2008/06/14/2545399.aspx)=====================================================================
1. Create/EndThread是Win32方法开始/结束一个线程
_beginthreadx/_endthreadex是C RunTime方式开始/结束一个线程
AfxBeginThread在MFC中开始/结束一个线程
2.直接在CreateThread API创建的线程中使用sprintf，malloc，strcat等涉及CRT存储堆操作的CRT库函数是很危险的，容易造成线程的意外中止。 在使用_beginthread和_beginthreadex创建的线程中可以安全的使用CRT函数。但是必须在线程结束的时候相应的调用_endthread或_endthreadex
3._beginthread成对调用的_endthread函数内部隐式的调用CloseHandle关闭了线程句柄，而与_beginthreadex成对使用的_endthreadex则没有关闭线程的句柄，需要显示的调用CloseHandle关闭线程句柄，不要使用_beginthread，使用._beginthreadex代替之。
4.尽量不要在一个MFC程序中使用_beginthreadex()或CreateThread()。
5.没有使用到MFC的线程尽量用_beginthreadex启动。
6.如果在一个与LIBCMT.LIB链接的程序中调用C Runtime函数，则必须要用_beginthreadex启动线程
7._beginthreadex启动的线程可以安全的调用任何C Runtime 函数
=====================================================================
【参考5】[CreateThread()、_beginthread()以及_beginthreadex()联系与区别](http://hi.baidu.com/xioazhu/blog/item/1924a01e1ac59416413417ab.html)
=====================================================================
<<Windows核心编程>>中有很详细地介绍：
注意：若要创建一个新线程，绝对不要使用CreateThread，而应使用_beginthreadex.
Why?考虑标准C运行时库的一些变量和函数，如errno，这是一个全局变量。全局变量用于多线程会出什么事，你一定知道的了。故必须存在一种机制，使得每个线程能够引用它自己的errno变量，又不触及另一线程的errno变量._beginthreadex就为每个线程分配自己的tiddata内存结构。该结构保存了许多像errno这样的变量和函数的值、地址（自己看去吧）。
通过线程局部存储将tiddata与线程联系起来。具体实现在Threadex.c中有。   
结束线程使用函数_endthreadex函数，释放掉线程的tiddata数据块。   
CRT的函数库在线程出现之前就已经存在，所以原有的CRT不能真正支持线程，这导致我们在编程的时候有了CRT库的选择，在MSDN中查阅CRT的函数时都有：
　　Libraries   
　　LIBC.LIB   Single   thread   static   library,   retail   version     
　　LIBCMT.LIB   Multithread   static   library,   retail   version     
　　MSVCRT.LIB   Import   library   for   MSVCRT.DLL,   retail   version     
这样的提示！
=====================================================================
引用:http://hi.baidu.com/czlaner/blog/item/8ca6e58b011846dafd1f1009.html
