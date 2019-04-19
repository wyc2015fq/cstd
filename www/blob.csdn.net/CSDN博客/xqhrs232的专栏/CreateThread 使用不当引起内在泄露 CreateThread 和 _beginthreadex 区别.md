# CreateThread 使用不当引起内在泄露? CreateThread 和 _beginthreadex 区别 - xqhrs232的专栏 - CSDN博客
2013年05月03日 13:36:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：632
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/lgxqf/archive/2009/02/10/1387480.html](http://www.cnblogs.com/lgxqf/archive/2009/02/10/1387480.html)
在 Win32 API 中，创建线程的基本函数是 CreateThread，而 _beginthread(ex) 是
C++ 运行库的函数。为什么要有两个呢？因为C++ 运行库里面有一些函数使用了全局
量，如果使用 CreateThread 的情况下使用这些C++ 运行库的函数，就会出现不安全
的问题。而 _beginthreadex 为这些全局变量做了处理，使得每个线程都有一份独立
的“全局”量。
所以，如果你的编程只调用 Win32 API/SDK ，就放心用 CreateThread；如果要用到
C++ 运行时间库，那么就要使用 _beginthreadex ，并且需要在编译环境中选择 Use
MultiThread Lib/DLL。
C++ 运行期库有两个创建线程的函数，另一个是 _beginthread，它们两者的区别请
自己去看MSDN：
通常他们的解释都是这容易造成内存泄漏。这个解释本身是没有错的，但是解释得不够完全和详细。以至于造成很多新手盲目的信任了那句话，在那里都是用_beginthreadex函数，或者是装作没有看到使用CreateThread函数。曾经有一段时间我也对这个问题很是困惑，不知道到底用那个才是对的。因为我不止一次在很多权威性的代码中看到对CreateThread函数的直接调用。难道是权威错了？？抱着怀疑的态度查找了大量的资料和书籍，终于搞明白了这个问题的关键所在，在此做个说明，算是对那句话的一个完善。
关于_beginthreadex和CreateThread的区别我就不做说明了，这个很容易找到的。我们只要知道一个问题：_beginthreadex是一个C运行时库的函数，CreateThread是一个系统API函数，_beginthreadex内部调用了CreateThread。只所以所有的书都强调内存泄漏的问题是因为_beginthreadex函数在创建线程的时候分配了一个堆结构并和线程本身关联起来，我们把这个结构叫做tiddata结构，是通过线程本地存储器TLS于线程本身关联起来。我们传入的线程入口函数就保存在这个结构中。tiddata的作用除了保存线程函数入口地址之外，还有一个重要的作用就是:C运行时库中有些函数需要通过这个结构来保存和获取一些数据，比如说errno之类的线程全局变量。这点才是最重要的。
当一个线程调用一个要求tiddata结构的运行时库函数的时候，将发生下面的情况：
运行时库函数试图TlsGetvalue获取线程数据块的地址，如果没有获取到，函数就会现场分配一个 tiddata结构，并且和线程相关联，于是问题出现了，如果不通过_endthreadex函数来终结线程的话，这个结构将不会被撤销，内存泄漏就会出现了。但通常情况下，我们都不推荐使用_endthreadex函数来结束线程，因为里面包含了ExitThread调用。
找到了内存泄漏的具体原因，我们可以这样说：只要在创建的线程里面不使用一些要求tiddata结构的运行时库函数，我们的内存时安全的。所以，前面说的那句话应该这样说才完善：
“绝对不要调用系统自带的CreateThread函数创建新的线程，而应该使用_beginthreadex，除非你在线程中绝不使用需要tiddata结构的运行时库函数”
这个需要tiddata结构的函数有点麻烦了，在侯捷的《win32多线程程序设计》一书中这样说到：
”如果在除主线程之外的任何线程中进行一下操作，你就应该使用多线程版本的C runtime library,并使用_beginthreadex和_endthreadex：
1 使用malloc()和free()，或是new和delete
2 使用stdio.h或io.h里面声明的任何函数
3 使用浮点变量或浮点运算函数
4 调用任何一个使用了静态缓冲区的runtime函数，比如:asctime(),strtok()或rand()
Addition info please refer to
[http://support.microsoft.com/kb/104641/en-us](http://support.microsoft.com/kb/104641/en-us)
[http://msdn.microsoft.com/en-us/library/ms682453(VS.85).aspx](http://msdn.microsoft.com/en-us/library/ms682453%28VS.85%29.aspx)
