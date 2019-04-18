# Linux 下 C++ 异常处理技巧 - 文章 - 伯乐在线
原文出处： [IBM developerworks](http://www.ibm.com/developerworks/cn/linux/l-cppexcep.html)
# 处理固有语言局限性的四种技术
处理 C++ 中的异常会在语言级别上遇到少许隐含限制，但在某些情况下，您可以绕过它们。学习各种利用异常的方法，您就可以生产更可靠的应用程序。
# 保留异常来源信息
在 C++中，无论何时在处理程序内捕获一个异常，关于该异常来源的信息都是不为人知的。异常的具体来源可以提供许多更好地处理该异常的重要信息，或者提供一些可以附加到错误日志的信息，以便以后进行分析。
为了解决这一问题，可以在抛出异常语句期间，在异常对象的构造函数中生成一个堆栈跟踪。`ExceptionTracer` 是示范这种行为的一个类。
清单 1. 在异常对象构造函数中生成一个堆栈跟踪

```
// Sample Program:
 // Compiler: gcc 3.2.3 20030502
 // Linux: Red Hat
 #include <execinfo.h>
 #include <signal.h>
 #include <exception>
 #include <iostream>
 using namespace std;
 /////////////////////////////////////////////
 class ExceptionTracer
 {
 public:
     ExceptionTracer()
     {
         void * array[25];
         int nSize = backtrace(array, 25);
         char ** symbols = backtrace_symbols(array, nSize);
         for (int i = 0; i < nSize; i++)
         {
             cout << symbols[i] << endl;
         }
         free(symbols);
     }
 };
```
# 管理信号
每当进程执行一个令人讨厌的动作，以致于 Linux™ 内核发出一个信号时，该信号都必须被处理。信号处理程序通常会释放一些重要资源并终止应用程序。在这种情况下，堆栈上的所有对象实例都处于未破坏状态。另一方面，如果这些信号被转换成 C++ 异常，那么您可以优雅地调用其构造函数，并安排多层 catch 块，以便更好地处理这些信号。
清单 2 中定义的 `SignalExceptionClass`，提供了表示内核可能发出信号的 C++ 异常的抽象。`SignalTranslator` 是一个基于 `SignalExceptionClass` 的模板类，它通常用来实现到 C++ 异常的转换。在任何瞬间，只能有一个信号处理程序处理一个活动进程的一个信号。因此，`SignalTranslator` 采用了 singleton 设计模式。整体概念通过用于 SIGSEGV 的 `SegmentationFault` 类和用于 SIGFPE 的 `FloatingPointException` 类得到了展示。
清单 2. 将信号转换成异常

```
template <class SignalExceptionClass> class SignalTranslator
 {
 private:
     class SingleTonTranslator
     {
     public:
         SingleTonTranslator()
         {
             signal(SignalExceptionClass::GetSignalNumber(), SignalHandler);
         }
         static void SignalHandler(int)
         {
             throw SignalExceptionClass();
         }
     };
 public:
     SignalTranslator()
     {
         static SingleTonTranslator s_objTranslator;
     }
 };
 // An example for SIGSEGV
 class SegmentationFault : public ExceptionTracer, public exception
 {
 public:
     static int GetSignalNumber() {return SIGSEGV;}
 };
 SignalTranslator<SegmentationFault> g_objSegmentationFaultTranslator;
 // An example for SIGFPE
 class FloatingPointException : public ExceptionTracer, public exception
 {
 public:
     static int GetSignalNumber() {return SIGFPE;}
 };
 SignalTranslator<FloatingPointException> g_objFloatingPointExceptionTranslator;
```
# 管理构造函数和析构函数中的异常
在全局（静态全局）变量的构造和析构期间，每个 ANSI C++ 都捕获到异常是不可能的。因此，ANSI C++ 不建议在那些其实例可能被定义为全局实例（静态全局实例）的类的构造函数和析构函数中抛出异常。换一种说法就是永远都不要为那些其构造函数和析构函数可能抛出异常的类定义全局（静态全局）实例。不过，如果假定有一个特定编译器和一个特定系统，那么可能可以这样做，幸运的是，对于 Linux 上的 GCC，恰好是这种情况。
使用 `ExceptionHandler` 类可以展示这一点，该类也采用了 singleton 设计模式。其构造函数注册了一个未捕获的处理程序。因为每次只能有一个未捕获的处理程序处理一个活动进程，构造函数应该只被调用一次，因此要采用 singleton 模式。应该在定义有问题的实际全局（静态全局）变量之前定义 `ExceptionHandler` 的全局（静态全局）实例。
清单 3. 处理构造函数中的异常

```
class ExceptionHandler
 {
 private:
     class SingleTonHandler
     {
     public:
         SingleTonHandler()
         {
             set_terminate(Handler);
         }
         static void Handler()
         {
             // Exception from construction/destruction of global variables
             try
             {
                 // re-throw
                 throw;
             }
             catch (SegmentationFault &)
             {
                 cout << "SegmentationFault" << endl;
             }
             catch (FloatingPointException &)
             {
                 cout << "FloatingPointException" << endl;
             }
             catch (...)
             {
                 cout << "Unknown Exception" << endl;
             }
             //if this is a thread performing some core activity
             abort();
             // else if this is a thread used to service requests
             // pthread_exit();
         }
     };
 public:
     ExceptionHandler()
     {
         static SingleTonHandler s_objHandler;
     }
 };
 //////////////////////////////////////////////////////////////////////////
 class A
 {
 public:
     A()
     {
         //int i = 0, j = 1/i;
         *(int *)0 = 0;
     }
 };
 // Before defining any global variable, we define a dummy instance
 // of ExceptionHandler object to make sure that
 // ExceptionHandler::SingleTonHandler::SingleTonHandler() is invoked
 ExceptionHandler g_objExceptionHandler;
 A g_a;
 //////////////////////////////////////////////////////////////////////////
 int main(int argc, char* argv[])
 {
     return 0;
 }
```
# 处理多线程程序中的异常
有时一些异常没有被捕获，这将造成进程异常中止。不过很多时候，进程包含多个线程，其中少数线程执行核心应用程序逻辑，同时，其余线程为外部请求提供服务。如果服务线程因编程错误而没有处理某个异常，则会造成整个应用程序崩溃。这一点可能是不受人们欢迎的，因为它会通过向应用程序传送不合法的请求而助长拒绝服务攻击。为了避免这一点，未捕获处理程序可以决定是请求异常中止调用，还是请求线程退出调用。清单 3 中 `ExceptionHandler::SingleTonHandler::Handler()` 函数的末尾处展示了该处理程序。
# 结束语
我简单地讨论了少许 C++ 编程设计模式，以便更好地执行以下任务：
- 在抛出异常的时候追踪异常的来源。
- 将信号从内核程序转换成 C++ 异常。
- 捕获构造和/或析构全局变量期间抛出的异常。
- 多线程进程中的异常处理。
我希望您能采用这些技巧中的一些来开发无忧代码。
