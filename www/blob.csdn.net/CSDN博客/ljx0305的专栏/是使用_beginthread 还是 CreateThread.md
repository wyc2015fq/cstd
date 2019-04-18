# 是使用_beginthread 还是 CreateThread  - ljx0305的专栏 - CSDN博客
2008年04月07日 14:33:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1289标签：[thread																[allocation																[signal																[function																[microsoft																[dll](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=allocation&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)
CreateThread 是一个Win 32API 函数,_beginthread 是一个CRT(C Run-Time)函数，他们都是实现多线城的创建的函数，而且他们拥有相同的使用方法,相同的参数列表。
但是他们有什么区别呢？
一般来说，从使用角度是没有多大的区别的，CRT函数中除了signal()函数不能在CreateThread创建的线城中使用外，其他的CRT函数都可一正常使用，但是如果在CreateThread创建的线城中使用CRT函数的话，会产生一些Memory Leak.
下面是摘自KB的原话：
## SUMMARY
All C Run-time functions except the signal() function work correctly when used in threads created by the CreateThread() function. However, depending on what CRT functions are called, there may be a small memory leak when threads are terminated. Calling strlen(), for example, does not trigger the allocation of the CRT thread data-block, and calling malloc(), fopen(), _open(), strtok(), ctime(), or localtime() causes allocation of a CRT per-thread data-block, which may cause a memory leak. 
## MORE INFORMATION
The "Programming Techniques" manual supplied with Visual C++ 32-bit Edition states that using CreateThread() in a program that uses Libcmt.lib causes many CRT functions to fail. Actually, the only function that should not be used in a thread created with CreateThread() is the signal() function. 
There are two ways to create threads. One method involves using the CRT _beginthread() or _beginthreadex() (with Visual C++ 2.0 and later); the other method involves using the CreateThread() API. All CRT functions other than the signal() function work correctly in threads created with either _beginthread() or CreateThread(). However, there are some problems involved with using CRT functions in threads created with CreateThread(). 
Threads that are created and terminated with the CreateThread() and ExitThread() Win32 API functions do not have memory that is allocated by the CRT for static data and static buffers cleaned up when the thread terminates. Some examples of this type of memory are static data for errno and _doserrno and the static buffers used by functions such as asctime(), ctime(), localtime(), gmtime(), and mktime(). Using CreateThread() in a program that uses the CRT (for example, links with LIBCMT.LIB) may cause a memory leak of about 70-80 bytes each time a thread is terminated. 
To guarantee that all static data and static buffers allocated by the CRT are cleaned up when the thread terminates, _beginthreadex() and _endthreadex() should be used when creating a thread. The _beginthreadex() function includes the same parameters and functionality as CreateThread(). 
另外有个小小的测验：
 用CreateThread 创建的线城能否被CRT函数 _endthreadex() 关闭？
CreateThread()和_beginthreadex()在Jeffrey的《Windows核心编程》中讲的很清楚，应当尽量避免使用CreateThread()。 
事实上，_beginthreadex()在内部先为线程创建一个线程特有的tiddata结构，然后调用CreateThread()。在某些非线程安全的CRT函数中会请求这个结构。如果直接使用CreateThread()的话，那些函数发现请求的tiddata为NULL，就会在现场为该线程创建该结构，此后调用EndThread()时会引起内存泄漏。_endthreadex()可以释放由CreateThread()创建的线程，实际上，在它的内部会先释放由_beginthreadex()创建的tiddata结构，然后调用EndThread()。 
因此，应当使用_beginthreadex()和_endthreadex()，而避免使用CreateThread()和EndThread()。当然，_beginthread()和_endthread()也是应当避免使用的。 
程序员对于Windows程序中应该用_beginthread还是CreateThread来创建线程，一直有所争论。本文将从对CRT源代码出发探讨这个问题。
I. 起因
今天一个朋友问我程序中究竟应该使用_beginthread还是CreateThread，并且告诉我如果使用不当可能会有内存泄漏。其实我过去对这个问题也是一知半解，为了对朋友负责，专门翻阅了一下VC的运行库（CRT）源代码，终于找到了答案。
II. CRT
CRT(C/C++ Runtime Library)是支持C/C++运行的一系列函数和代码的总称。虽然没有一个很精确的定义，但是可以知道，你的main就是它负责调用的，你平时调用的诸如strlen、strtok、time、atoi之类的函数也是它提供的。我们以Microsoft Visual.NET 2003中所附带的CRT为例。假设你的.NET 2003安装在C:Program FilesMicrosoft Visual Studio .NET 2003中，那么CRT的源代码就在C:Program FilesMicrosoft Visual Studio .NET 2003Vc7crtsrc中。既然有了这些实现的源代码，我们就可以找到一切解释了。
III. _beginthread/_endthread
这个函数究竟做了什么呢？它的代码在thread.c中。阅读代码，可以看到它最终也是通过CreateThread来创建线程的，主要区别在于，它先分配了一个_tiddata，并且调用了_initptd来初始化这个分配了的指针。而这个指针最后会被传递到CRT的线程包装函数_threadstart中，在那里会把这个指针作为一个TLS（Thread Local Storage）保存起来。然后_threadstart会调用我们传入的线程函数，并且在那个函数退出后调用_endthread。这里也可以看到，_threadstart用一个__try/__except块把我们的函数包了起来，并且在发生异常的时候，调用exit退出。（_threadstart和endthread的代码都在thread.c中）
这个_tiddata是一个什么样的结构呢？它在mtdll.h中定义，它的成员被很多CRT函数所用到，譬如int _terrno，这是这个线程中的错误标志；char* _token，strtok以来这个变量记录跨函数调用的信息，...。
那么_endthread又做了些什么呢？除了调用浮点的清除代码以外，它还调用了_freeptd来释放和这个线程相关的tiddata。也就是说，在_beginthread里面分配的这块内存，以及在线程运行过程中其它CRT函数中分配并且记录在这个内存结构中的内存，在这里被释放了。
通过上面的代码，我们可以看到，如果我使用_beginthread函数创建了线程，它会为我创建好CRT函数需要的一切，并且最后无需我操心，就可以把清除工作做得很好，可能唯一需要注意的就是，如果需要提前终止线程，最好是调用_endthread或者是返回，而不要调用ExitThread，因为这可能造成内存释放不完全。同时我们也可以看出，如果我们用CreateThread函数创建了线程，并且不对C运行库进行调用（包括任何间接调用），就不必担心什么问题了。
IV. CreateThread和CRT
或许有人会说，我用CreateThread创建线程以后，我也调用了C运行库函数，并且也使用ExitThread退出了，可是我的程序运行得好好的，既没有因为CRT没有初始化而崩溃，也没有因为忘记调用_endthread而发生内存泄漏，这是为什么呢，让我们继续我们的CRT之旅。
假设我用CreateThread创建了一个线程，我调用strtok函数来进行字符串处理，这个函数肯定是需要某些额外的运行时支持的。strtok的源代码在strtok.c中。从代码可见，在多线程情况下，strtok的第一句有效代码就是_ptiddata ptd = _getptd()，它通过这个来获得当前的ptd。可是我们并没有通过_beginthread来创建ptd，那么一定是_getptd捣鬼了。打开tidtable.c，可以看到_getptd的实现，果然，它先尝试获得当前的ptd，如果不能，就重新创建一个，因此，后续的CRT调用就安全了。可是这块ptd最终又是谁释放的呢？打开dllcrt0.c，可以看到一个DllMain函数。在VC中，CRT既可以作为一个动态链接库和主程序链接，也可以作为一个静态库和主程序链接，这个在Project Setting->Code Generations里面可以选。当CRT作为DLL链接到主程序时，DllMain就是CRT DLL的入口。Windows的DllMain可以由四种原因调用：Process Attach/Process Detach/Thread Attach/Thread Detach，最后一个，也就是当线程函数退出后但是线程还没有销毁前，会在这个线程的上下文中用Thread Detach调用DllMain，这里，CRT做了一个_freeptd(NULL)，也就是说，如果有ptd，就free掉。所以说，恰巧没有发生内存泄漏是因为你用的是动态链接的CRT。
于是我们得出了一个更精确的结论，如果我没有使用那些会使用_getptd的CRT函数，使用CreateThread就是安全的。
V. 使用ptd的函数
那么，究竟那些函数使用了_getptd呢？很多！在CRT目录下搜索_getptd，你会发觉很多意想不到的函数都用到了它，除了strtok、rand这类需要保持状态的，还有所有的字符串相关函数，因为它们要用到ptd中的locale信息；所有的mbcs函数，因为它们要用到ptd中的mbcs信息，...。
VI. 测试代码
下面是一段测试代码（leaker中用到了atoi，它需要ptd）：
#include 
#include
#include 
#include 
volatile bool threadStarted = false;
void leaker()
{
    std::cout << atoi( "0" ) << std::endl;
}
DWORD __stdcall CreateThreadFunc( LPVOID )
{
    leaker();
    threadStarted = false;
    return 0;
}
DWORD __stdcall CreateThreadFuncWithEndThread( LPVOID )
{
    leaker();
    threadStarted = false;
    _endthread();
    return 0;
}
void __cdecl beginThreadFunc( LPVOID )
{
    leaker();
    threadStarted = false;
}
int main()
{
    for(;;)
    {
        while( threadStarted )
            Sleep( 5 );
        threadStarted = true;
//      _beginthread( beginThreadFunc, 0, 0 );//1
        CreateThread( NULL, 0, CreateThreadFunc, 0, 0, 0 );//2
//      CreateThread( NULL, 0, CreateThreadFuncWithEndThread, 0, 0, 0 );//3
    }
    return 0;
}
如果你用VC的多线程+静态链接CRT选项去编译这个程序，并且尝试打开1、2、3之中的一行，你会发觉只有2打开的情况下，程序才会发生内存泄漏（可以在Task Manager里面明显的观察到）。3之所以不会出现内存泄漏是因为主动调用了_endthread。
VII. 总结
如果你使用了DLL方式链接的CRT库，或者你只是一次性创建少量的线程，那么你或许可以采取鸵鸟策略，忽视这个问题。上面一节代码中第3种方法基于对CRT库的了解，但是并不保证这是一个好的方法，因为每一个版本的VC的CRT可能都会有些改变。看来，除非你的头脑清晰到可以记住这一切，或者你可以不厌其烦的每调用一个C函数都查一下CRT代码，否则总是使用_beginthread（或者它的兄弟_beginthreadex）是一个不错的选择。
**[后记]网友condor指出本文的一个错误：在dllcrt0.c中，DllMain的Thread Detach所释放的ptd，其实是dllcrt0.c的DllMain中的Thread Attach所创建的。也就是说，当你用CRT DLL的时候，DllMain对线程做了一切初始化/清除工作。我查看源代码，thread.c中的_threadstart函数，在设置TLS之前做了检查，这其实就是为了避免重复设置导致的内存泄漏。**
引用[http://www.cppblog.com/eday/archive/2006/11/25/15648.html](http://www.cppblog.com/eday/archive/2006/11/25/15648.html)
