# CreateThread与_beginthread的区别 - ljx0305的专栏 - CSDN博客
2009年10月29日 11:34:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：517标签：[library																[microsoft																[多线程																[服务器软件																[编译器																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器软件&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)
个人分类：[C++																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/380566)
在写c++代码时，一直牢记着一句话：决不应该调用CreateThread。相反，应该使用Visual   C++运行期库函数_beginthreadex。
好像CreateThread函数就是老虎，既然这样为什么微软要开发这个函数呢？
从网上找到的相关资料，现在汇总一下，在此对相关人员进行感谢！
摘自《windows   核心编程》：
CreateThread函数是用来创建线程的Windows函数。不过，如果你正在编写C/C++代码，决不应该调用CreateThread。相反，应该使用Visual   C++运行期库函数_beginthreadex。如果不使用Microsoft的Visual   C++编译器，你的编译器供应商有它自己的CreateThred替代函数。
若要使多线程C和C++程序能够正确地运行，必须创建一个数据结构，并将它与使用C/C++运行期库函数的每个线程关联起来。当你调用C/C++运行期库时，这些函数必须知道查看调用线程的数据块，这样就不会对别的线程产生不良影响。
   1.每个线程均获得由C/C++运行期库的堆栈分配的自己的tiddata内存结构。
2.传递给_beginthreadex的线程函数的地址保存在tiddata内存块中。传递给该函数的参数也保存在该数据块中。
3._beginthreadex确实从内部调用CreateThread，因为这是操作系统了解如何创建新线程的唯一方法。
4.当调用CreatetThread时，它被告知通过调用_threadstartex而不是pfnStartAddr来启动执行新线程。还有，传递给线程函数的参数是tiddata结构而不是pvParam的地址。
5.如果一切顺利，就会像CreateThread那样返回线程句柄。如果任何操作失败了，便返回 NULL。
     _beginthreadex和_beginthread函数的区别。_beginthread函数的参数比较少，因此比特性全面的_beginthreadex函数受到更大的限制。
例如，如果使用_beginthread，就无法创建带有安全属性的新线程，无法创建暂停的线程，也无法获得线程的ID值。
下面摘录Csdn中的Holly（）的帖子进行解释，再次表示感谢。
来源：[[url]http://topic.csdn.net/t/20000926/10/31810.html[/url]](http://topic.csdn.net/t/20000926/10/31810.html)
Holly（）：
oldworm提供了很好的使用的例子，而且也运用了编译控制！
我来解释一下理论上的区别：
 CreateThread、_beginthread和_beginthreadex都是用来启动线程的，但大家看到oldworm没有提供_beginthread的方式，原因简单，_beginthread是_beginthreadex的功能子集，虽然_beginthread内部是调用_beginthreadex但他屏蔽了象安全特性这样的功能，所以_beginthread与CreateThread不是同等级别，_beginthreadex和CreateThread在功能上完全可替代，我们就来比较一下_beginthreadex与CreateThread!   
  CRT的函数库在线程出现之前就已经存在，所以原有的CRT不能真正支持线程，这导致我们在编程的时候有了CRT库的选择，在MSDN中查阅CRT的函数时都有：
  Libraries   
  LIBC.LIB   Single   thread   static   library,   retail   version     
  LIBCMT.LIB   Multithread   static   library,   retail   version     
  MSVCRT.LIB   Import   library   for   MSVCRT.DLL,   retail   version     
这样的提示！
对于线程的支持是后来的事！
这也导致了许多CRT的函数在多线程的情况下必须有特殊的支持，不能简单的使用CreateThread就OK。
大多的CRT函数都可以在CreateThread线程中使用，看资料说只有signal()函数不可以，会导致进程终止！但可以用并不是说没有问题！
有些CRT的函数象malloc(),   fopen(),   _open(),   strtok(),   ctime(),   或localtime()等函数需要专门的线程局部存储的数据块，这个数据块通常需要在创建线程的时候就建立，如果使用CreateThread，这个数据块就没有建立，然后会怎样呢？在这样的线程中还是可以使用这些函数而且没有出错，实际上函数发现这个数据块的指针为空时，会自己建立一个，然后将其与线程联系在一起，这意味着如果你用CreateThread来创建线程，然后使用这样的函数，会有一块内存在不知不觉中创建，遗憾的是，这些函数并不将其删除，而CreateThread和ExitThread也无法知道这件事，于是就会有Memory   Leak，在线程频繁启动的软件中(比如某些服务器软件)，迟早会让系统的内存资源耗尽！
  _beginthreadex(内部也调用CreateThread)和_endthreadex就对这个内存块做了处理，所以没有问题！(不会有人故意用CreateThread创建然后用_endthreadex终止吧，而且线程的终止最好不要显式的调用终止函数，自然退出最好！)   
谈到Handle的问题，_beginthread的对应函数_endthread自动的调用了CloseHandle，而_beginthreadex的对应函数_endthreadex则没有，所以CloseHandle无论如何都是要调用的不过_endthread可以帮你执行自己不必写，其他两种就需要自己写！(Jeffrey   Richter强烈推荐尽量不用显式的终止函数，用自然退出的方式，自然退出当然就一定要自己写CloseHandle)
本文出自 “[YOUNG ADULT ,GOGOGO!](http://820808.blog.51cto.com/)” 博客，请务必保留此出处[http://820808.blog.51cto.com/328558/76160](http://820808.blog.51cto.com/328558/76160)
引用:http://820808.blog.51cto.com/328558/76160
