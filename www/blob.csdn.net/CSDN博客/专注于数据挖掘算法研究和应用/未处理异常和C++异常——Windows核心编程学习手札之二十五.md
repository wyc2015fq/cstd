# 未处理异常和C++异常——Windows核心编程学习手札之二十五 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年09月14日 11:07:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4018








# 未处理异常和C++异常

## ——Windows核心编程学习手札之二十五

当一个异常过滤器返回EXCEPTION_CONTINUE_SEARCH标识符时是告诉系统继续上溯调用树，寻找另外的异常过滤器，但当每个过滤器都返回EXCEPTION_CONTINUE_SEARCH标识符，出现了所谓“未处理异常”。

每个线程开始执行，实际是利用kernel32.dll中的一个函数来调用BaseProcessStart或BaseThreadStart，这两个函数实际是一样的，区别在于一个函数用于进程的主线程（primary thread）：

VOID BaseProcessStart(PPROCESS_START_ROUTINE pfnStartAddr){

__try{

ExitThread((pfnStartAddr)());

}

__except(UnhandledExceptionFilter(GetExceptionInfomation())){

ExitProcess(GetExceptionCode());

}

//note:we never get here

}

另一个函数用于进程的所有辅助线程（Secondary thread）：

VOID BaseThreadStart(PTHREAD_START_ROUTINE pfnStartAddr,PVOID pvParam){

__try{

ExitThread((pfnStartAddr)( pvParam));

}

__except(UnhandledExceptionFilter(GetExceptionInfomation())){

ExitProcess(GetExceptionCode());

}

//note:we never get here

}

这两个函数都包含一个SHE框架，每个函数都有一个try块，并从这个try块里调用主线程或辅助线程的进入点函数。当线程引发一个异常时，所有过滤器都返回EXCEPTION_CONTINUE_SEARCH时，将会自动调用一个由系统提供的特殊过滤器函数：

LONG UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)；

这个函数负责显示一个消息框，指出有一个进程的线程存在未处理的异常，并且能让用户结束或调试这个进程。在消息框的异常描述之后，提供用户两个选择：

1）选择“ok”按钮，将导致UnhandledExceptionFilter返回EXCEPTION_CONTINUE_SEARCH，这将引起全局展开，所有的finally块都要执行，然后BaseProcessStart或BaseThreadStart中的处理程序执行。这两个处理程序都是ExitProcess，退出进程，退出的代码是异常代码，这是进程的线程结束了进程本身不是操作系统，意味这程序员可以控制这种行为并改变它。

2）选择“Cancel”按钮，UnhandledExceptionFilter试图加载一个调试程序，并将这个调试程序挂接在进程上，通过将调试程序附在进程上，可以检查全局、局部和静态变量的状态，设置断点，检查调用树，重新启动进程，以及调试一个进程可以做的任何事情。

注意：上面是在用户方式（user-mode）下的程序开发，对于运行在内核方式（kernel-mode）出现未处理异常，未处理异常是在操作系统中或更可能在设备驱动程序中，而不是在应用程序中，这样的未处理异常表示一个严重的程序错误（bug）！如果一个低级虚拟内存函数产生一个异常，系统查找是否有内核方式异常过滤器准备处理这个异常，如果系统找不到，则异常是未处理的，如果一个未处理异常发生在内核方式，让系统继续运行是不安全的，所以系统在这种情况下不会调用UnhandledExceptionFilter函数，而是显示所谓的蓝屏死机（Blue Screen of Death），显示画屏切换到只包含文本的蓝屏视频方式，并且计算机被停机（balt），显示的文本告诉是哪个设备驱动程序被加载，并且该模块中包含引发未处理异常的代码。

当程序员选择cancel按钮时，告诉UnhandledExceptionFilter函数对进程进行调试，随时将调试程序连接到任何进程的能力称为即时调试（Just-in-time Debugging）。在内部，UnhandledExceptionFilter调用调试程序，需要查看下面的注册表子关键字：

HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/WindowsNT/

CurrentVersion/AeDebug

该关键字里，有一个Debugger的数值，安装Visual Studio时被设置成下面的值：

C:/Program Filese/Microsoft Visual Studio/Common/MSDev98/Bin/msdev.exe

-p %ld–e %ld

在windows98中，这些值不存放在注册表中，而是存放在Win.ini文件中。

这个值告诉系统要将哪个程序（这里是msdev.exe）作为调试程序运行，当然也可以选择其他调试程序。UnhandledExceptionFilter还在命令行里向调试程序传递两个参数，第一个参数是被调试进程ID；第二个参数规定一个可继承的手工复位事件，这个事件是由UnhandledExceptionFilter按无信号状态建立的，厂商必须实现自己的调试程序，这样才能认识指定进程ID和事件句柄的-p和-e选项。

在进程ID和事件句柄都合并到这个串中后，UnhandledExceptionFilter通过调用CreateProcess来执行调试程序，这是，调试程序进程开始运行并检查它的命令行参数，如果存在-p选项，调试程序取得进程ID，并通过调用DebugActiveProcess将自身挂接到该进程上：

BOOL DebugActiveProcess(DWORD dwProcessID)；

一旦调试程序完成自身的挂接，操作系统将被调试者（debuggee）的状态通报给调试程序。在调试程序完全初始化后，要再检查它的命令行，找-e选项，如该项存在，调试程序取得相应事件句柄并调用SetEvent。

另外，不必在调试进程之前等待异常的出现，可以随时将一个调试程序连接在任何进程上，只需运行MSDEV –p PID，其中PID是要调试的进程的ID，实际在Windows2000的Task Manager里选择一个进程，并选择debug菜单，就将引起Task Manager去查看前面的注册表子关键字，调用CreateProcess，并传递所选定的进程ID作为参数，在这里，Task Manage为事件句柄传送0值。

如果异常发生时，不想在屏幕上显示消息框，有下面几个方法：

1）强制进程终止运行

为防止UnhandledExceptionFilter显示异常消息框，可以调用SetErrorMode函数，并向它传递一个SEM_NOGPFAULTERRORBOX标识符：

UNINT SetErrorMode(UINT fuErrorMode)；

当调用UnhandledExceptionFilter函数来处理异常时，看到已经设置了这个标志，就立即返回EXCEPT_EXECUTE_HANDLER，这将导致全局展开并执行BaseProcessStart或BaseThreadStart中的处理程序，该处理程序结束进程。

2）包装一个线程函数

在主线程进入点函数（main、wmain、WinMain、wWinMain）的整个内容安排一个try-except块，保证异常过滤器的结果值总是EXCEPT_EXECUTE_HANDLER，这样就保证异常总能得到处理，防止系统再调用UnhandledExceptionFilter函数。缺点是只能捕捉主线程中发生的异常，而其他辅助线程则不能覆盖。

3）包装所有线程函数

Windows提供了SetUnhandledExceptionFilter函数可以按SHE格式包装所有线程函数。

PTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(

PTOP_LEVEL_EXCEPTION_FILTER pTopLevelExceptionFilter);

在进程调用这个函数后，进程的任何线程发生一个未处理异常，就会导致调用程序自己的异常过滤器，需要将这个过滤器地址作为参数传递给SetUnhandledExceptionFilter。过滤器函数原型是：

LONG UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)；

3）自动调用调试程序

在设置调试程序的注册表同一个关键字里，有个Auto值，该值用来规定UnhandledExceptionFilter是应该显示消息框还是仅启动调试程序，如果Auto设置为1，UnhandledExceptionFilter就不显示消息框向用户报告异常，而是立即调用调试程序，如设置为0，就按照上面所说的。

UnhandledExceptionFilter函数的内部执行情况：

1）如果发生一个存取违规并且是由于试图写内存（相对于读内存）引起的，系统要查看是不是要修改一个exe模块或dll模块中的资源，默认下资源是只读的，试图修改资源会引起存取异常。然而16位Windows允许修改资源，从兼容性考虑，32位和64位也应允许修改资源，所以当想要修改资源时，UnhandledExceptionFilter调用VirtualProtect，将资源页上的保护改成PAGE_READWRITE，并返回EXCEPTION_CONTINUE_EXECUTION；

2）如果已经调用了SetUnhandledExceptionFilter指定自己的过滤器，UnhandledExceptionFilter就调用指定的过滤器，如果自己的过滤器返回EXCEPTION_EXECUTE_HANDLER或EXECEPTION_CONTINUE_EXECUTION，UnhandledExceptionFilter就将这个值返回给系统，如果没有设置自己的未处理异常过滤器，或自己的未处理异常过滤器返回EXCEPTION_CONTINUE_SEARCH，则进入第3步；

3）如果进程是在调试程序下运行，就返回EXCEPTION_CONTINUE_SEARCH。调试程序显示一个消息框并运行调试进程（IsDebuggerPresent函数用来确定一个进程是否正在被调试）；

4）如果进程中一个线程以SEM_NOGPFAULTERRORBOX标志为参数调用SetErrorCode，UnhandledExceptionFilter就返回EXCEPTION_EXECUTE_HANDLER；

5）如果进程在一个作业job里并且作业的限制信息设定了JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION标志，则UnhandledExceptionFilter返回EXCEPTION_EXECUTE_HANDLER；

6）UnhandledExceptionFilter查阅注册表并取出Auto值，如果是1，到第7步，如是0，则向用户显示一个消息框；

7）UnhandledExceptionFilter产生调试程序，首先调用CreateEvent建立一个无信号的、手工复位的事件，这个事件的句柄可继承，然后从注册表中取出Debugger值，调用sprintf把它粘帖到进程ID（通过调用GetCurrentProcessID函数得到）和事件句柄里。STARTUPINFO的lpDesktop成员也设置成”Winsta0//Default”，这样调试程序就出现在交互式桌面上。

8）当调试程序完成初始化，就设置事件句柄，这将唤醒UnhandledExceptionFilter中的线程，这样进程就在调试程序下运行，UnhandledExceptionFilter返回EXCEPTION_CONTINUE_SEARCH。

SHE（结构化异常处理）可用于任何编程语言的操作系统设施，而C++异常只能用于编写C++代码。如果是编写C++程序，应使用C++异常处理而不是结构化异常处理，理由是C++异常处理是语言的一部分，编译器知道C++类对象是什么，也就是说编译器能够自动生成代码来调用C++对象析构函数，保证对象的清除。

不过，Microsoft Visual C++编译器也利用了操作系统的SHE实现了C++异常处理，所以当建立一个C++的try块时，编译器就生成一个SHE的__try块，一个C++的catch测试变成一个SHE异常过滤器，并且catch中的代码变成SHE的__except块中的代码，实际上，C++的throw语句，编译器生成windows对应的RaiseException函数的调用，用于throw语句的变量传递给RaiseException作为附加的参数，如上，C++异常在内部是由SHE实现的。

正常情况下，C++异常处理不能使程序从硬件异常中恢复，硬件违规就是存取违规或零作除数这种异常，但microsoft已经对其编译器增加了这种支持能力。



