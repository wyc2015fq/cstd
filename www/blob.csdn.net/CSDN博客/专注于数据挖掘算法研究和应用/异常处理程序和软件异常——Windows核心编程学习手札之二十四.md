# 异常处理程序和软件异常——Windows核心编程学习手札之二十四 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年09月13日 18:29:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3304








# 异常处理程序和软件异常

## ——Windows核心编程学习手札之二十四

CPU负责捕捉无效内存访问和用0除一个数值这种错误，并相应引发一个异常作为对错误的反应，CPU引发的异常称为硬件异常（hardware exception），操作系统和应用程序引发的异常，称为软件异常（software exception）。当出现一个硬件或软件异常时，操作系统向应用程序提供机会来考察是什么类型的异常被引发，并能够让应用程序自己来处理异常。下面是异常处理程序的文法：

__try{

//guarded body

。。。

}

__except(exception filter){

//Exception handler

。。。

}

每建一个try块，就必须跟随一个finally或一个except块，一个try块之后不能既有finally块又有except块，但可以在try-except块中嵌套try-finally块，反过来也可以。

与结束处理程序不同，异常过滤器（exception filter）和异常处理程序是通过操作系统直接执行的，编译程序在计算异常过滤器表达式和执行异常处理程序方面不做什么事。

当引发异常时，系统将定位到except块的开头，并计算异常过滤器表达式的值，过滤表达式的值只能是EXCEPTION_EXECUTE_HANDLER、EXCEPTION_CONTINUE_SEARCH、EXCEPTION_CONTINUE_EXECUTION三个标识符，定义在Windows的Excpt.h文件中。当try块中没有引发异常，则except块中的代码将永远不会被执行。

### 标识符EXCEPTION_EXECUTE_HANDLER

异常过滤器表达式的值是EXCEPTION_EXECUTE_HANDLER告诉系统执行一个全局展开，然后执行except块中的代码（异常处理程序代码）的跳转。在except块中代码执行完之后，系统考虑这个要被处理的异常并允许应用程序继续执行。这个机制使Windows应用程序可以处理错误并让程序继续运行。

全局展开（global unwind）使所有那些在处理异常的try_except块之后开始执行但未完成的try_finally块恢复执行。通过在finally块里放入一个return语句，可以阻止系统完成一个全局展开，应避免使用。

### 标识符EXCEPTION_CONTINUE_EXECUTION

在异常过滤器里可以直接写上标识符，也可以调用一个函数来返回标识符。过滤器值是标识符EXCEPTION_CONTINUE_EXECUTION时，系统跳回到产生异常的指令，试图再执行一次。当一个线程序试图去存取并不存在的栈存储区时，就会产生一个异常，系统的异常过滤器可以确定这个异常是源于试图存取栈的保留地址空间，异常过滤器调用VirtualAlloc向线程的栈提交更多的存储区，然后过滤器返回EXCEPTION_CONTINUE_EXECUTION，这时，试图存取栈存储区的CPU指令可以成功执行，线程可以继续执行。

### 标识符EXCEPTION_CONTINUE_SEARCH

标识符EXCEPTION_CONTINUE_SEARCH告诉系统去查找前面与一个except块相匹配的try块，并调用跟这个try块的异常处理器。

### 函数GetExceptionCode

一个异常过滤器在确定返回什么值之前，要分析具体情况，指出所发生异常的类别：

代码例子：

__try{

X=0;

Y=4/x;

}

__except( (GetExceptionCode() == EXCEPTION_INT_DIVIDE_BY_ZERO)?

EXCEPTION_EXECUTE_HANDLER:

EXCEPTION_CONTINUE_SEARCH){

//handle divide by zero exception

}

内部函数GetExceptionCode返回一个值，指出所发生异常的种类：

DWORD GetExceptionCode();

这些异常标识符在WinBase.h文件中。该函数只能在一个过滤器中条用（__except之后的括号里），或在一个异常处理程序中被调用。

### 函数GetExceptionInformation

当一个异常发生时，操作系统要向异常的线程的堆栈里压入三个结构：EXCEPTION_RECORD结构、CONTEXT结构和EXCEPTION_POINTERS结构。EXCEPTION_RECORD结构包含有关已发生异常的独立于CPU的信息，CONTEXT结构包含已发生异常的依赖于CPU的信息，EXCEPTION_POINTERS结构只有两个数据成员，二者都是指针，分别指向被压入栈的EXCEPTION_RECORD结构和CONTEXT结构：

Typedef struct _EXCEPTION_POINTERS{

PEXCEPTION_RECORD ExceptionRecord,

PCONTEXT ContextRecord;

}EXCEPTION_POINTERS,*PEXCEPTION_POINTERS；

取得这些信息并在程序中使用这些信息，要调用：

PEXCEPTION_POINTERS GetExceptionInformation();

该函数只能在异常过滤器中调用，因为仅仅在处理异常过滤器（__except后面的括号里）时，上面三个结构才是有效的。

### 软件异常

程序捕获软件异常所采取的方法与捕获硬件异常相同。如何让自己的函数引发软件异常，作为指出失败的方法，可调用RaiseException函数：：

VOID RaiseException(

DWORD dwExceptionCode,

DWORD dwExceptionFlags,

DWORD nNumberOfArguments,

CONST ULONG_PTR *pArguments);

在自己程序中产生自己的软件异常，如向系统的事件日志发送通知消息，每当程序中的一个函数发现某种问题，可以调用RaiseException函数并让某些异常处理程序上溯调用树查看特定的异常，或者将异常写到日志里或弹出一个消息框。建立软件异常来传达程序内部致使错误的信息。



