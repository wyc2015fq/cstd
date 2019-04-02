# Window线程

2016年12月30日 03:20:22 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：264





# 一、线程创建

Windows线程在创建时会首先创建一个线程内核对象，它是一个较小的数据结构，操作系统通过它来管理线程。新线程可以访问进程内核对象的所有句柄、进程中的所有内存及同一进程中其它线程的栈。

创建有以下几种方式，分别说明

1. CreateThread(...) (操作系统提供的API，尽量不要使用)
2. _beginthread(...)
3. _beginthreadex(...)
4. AfxBeginThread(...) (MFC提供的接口)

首先声明一个线程函数，原型为：

```cpp
DWORD FunThread(LPVOID pParam);
```

## 1. CreateThread()

该函数为操作系统提供，原型如下：



```cpp
HANDLE WINAPI CreateThread(
  _In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes ,
  _In_       SIZE_T dwStackSize ,
  _In_       LPTHREAD_START_ROUTINE lpStartAddress,
  _In_opt_   LPVOID lpParameter,
  _In_       DWORD dwCreationFlags,
  _Out_opt_  LPDWORD lpThreadId 
);
```



说明：

| Header    | Library      | Dll          |
| --------- | ------------ | ------------ |
| WinBase.h | Kernel32.lib | Kernel32.dll |

 

 

参数：

```cpp
　　lpThreadAttributes：指向SECURITY_ATTRIBUTES结构体的指针，记录线程的安全描述。决定子进程能否继承到返回的句柄，如果为NULL，则采用默认安全级别(THREAD_PRIORITY_NORMAL)，同时返回句柄不能继承
　　dwStackSize：指定线程栈大小，当为0时，表示栈使用默认大小
　　lpStartAddress：线程函数指针
　　lpParameter：线程函数参数
　　dwCreationFlags：为0：表示线程创建后立即运行；为CREATE_SUSPEND:创建后挂起，此时可修改线程属性，通过ResumeThread唤醒；
　　lpThreadId：一个指向threadID的指针，若对线程ID关注，则传值，否则置NULL
```

返回值：

　　创建线程的句柄；

　　若创建失败，则返回NULL，可用GetLastError()捕获错误；

MFC中也提供了CreateThread函数，它是CWinThread类的一个方法，如下

```cpp
BOOL CreateThread(
   DWORD dwCreateFlags = 0,
   UINT nStackSize = 0,
   LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL 
);
```

参数含义与返回值含义一致，它的调用方式是：

```cpp
CWinThread thread1;
thread1.CreateThread();
```

需要说明的是dwCreateFlags传值为CREATE_SUSPEND时, 要通过CWinThread::ResumeThread来唤醒

## 2. _beginthread(), _beginthreadex()

原型：



```cpp
unsigned long _beginthread( 
        void( __cdecl *start_address )( void * ), 
        unsigned stack_size, void *arglist 
);

unsigned long _beginthreadex( 
        void *security, 
        unsigned stack_size, 
        unsigned ( __stdcall *start_address )( void * ), 
        void *arglist, 
        unsigned initflag, 
        unsigned *thrdaddr 
);            
```



说明：

| Header    | Library                |
| --------- | ---------------------- |
| process.h | LIBCMT.lib  MSVCRT.lib |

 

 

参数与上面CreateThread含义相同，不在赘述；

二者比较：

1. _beginthread中线程函数调用为_cdecl，且无返回值; _beginthreadex为_stdcall，有返回值；

2. _beginthreadex中initflag相当于CreateThread中的dwCreationFlags，thrdaddr相当于lpThreadId

3. 在实现上_beginthreadex控制了一个_tiddata的线程数据块，里面存放了线程函数地址、参数的很多属性，之后再间接调用CreateThread(...)；

4. _beginthread则参数较少；

## 3. AfxBeginThread()

MFC提供的接口提供了二种不同类型线程的生成，即工作者线程和用户界面线程；可以简单理解用户界面线程包含用户界面，它有自己的消息队列，工作者线程用于计算等；



```cpp
CWinThread* AfxBeginThread(
   AFX_THREADPROC pfnThreadProc,             //线程函数指针，函数原型为UINT _cdecl fnThread(LPVOID pParam);
   LPVOID pParam,                            //线程函数参数
   int nPriority = THREAD_PRIORITY_NORMAL,   //优先级，SetThreadPriority
   UINT nStackSize = 0,                      //栈大小，单位是bytes,为0时表示按默认大小
   DWORD dwCreateFlags = 0,                  //CREATE_SUSPENDED:创建后挂起; 0:创建后立即运行   
   LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL  //指向SECURITY_ATTRIBUTES结构的指针,为Null时表示默认安全属性
); //创建一个工作者线程
CWinThread* AfxBeginThread(
   CRuntimeClass* pThreadClass,                //指向界面类指针,继承自CWinThread
   int nPriority = THREAD_PRIORITY_NORMAL,
   UINT nStackSize = 0,
   DWORD dwCreateFlags = 0,
   LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL 
); //创建一个用户界面线程
```



说明：

| Header   |
| -------- |
| Afxwin.h |

 

 

## 4. 比较

### 1. _beginthread与_beginthreadex

1. 在实现上_beginthreadex控制了一个_tiddata的线程数据块，里面存放了线程函数地址、参数的很多属性，之后再间接调用CreateThread(...)；
2. _beginthread则参数较少，有局限性；

### 2. AfxBeginThread与mfc的CreateThread

1. AfxBeginThread一步创建，之后线程立即运行
2. CWinThread::CreateThread二步创建，但它保存了线程对象，可以在连续的线程创建与运行完成结束之间再使用（Use CreateThread if you want to reuse the thread object between successive creation and termination of thread executions）

# 二、线程终结

终结线程的几个方法：

1. 等待线程函数运行完成自动结束
2. ExitThread()，用于结束线程自身
3. TerminateThread(),所有线程都可以用该方法结束
4. 父进程关闭，子线程随之关闭

只建议使用第一种方法结束线程，其它的方式都对应有缺陷

下面给出几个结束过程中发生事情：

1.资源有序释放（如操作系统分配资源，用到的C++类析构），返回线程退出代码，线程内核对象使用计数-1

2.操作系统相关资源释放；但象C++类并未析构，造成内存泄露；这里如果用_beginthreadex建立线程，而用ExitThread或者_endthread来释放线程，则线程放在堆上的线程数据块_tiddata也未释放，内存泄露；

3.该函数为异步函数，即通知操作系统终结线程后立即返回，而不管系统是否已经真的结束了线程。同时线程栈也不会释放

4.用ExitProgerss, TerminateProcess函数关闭进程后，进程会调用TerminateThread来关闭线程，效果如3，线程的栈没有释放，申请的对象资源也没释放。

## 1. ExitThread()

```cpp
VOID ExitThread(
  DWORD dwExitCode
);
```

说明：

| Header    | Library     |
| --------- | ----------- |
| winbase.h | coredll.lib |

 

 

参数：

```
dwExitCode: 指定线程的退出代码。可以通过GetExitCodeThread来查看一个线程的退出代码
```

返回值：无

说明：在线程结束后，会将线程内核对象中的ExitCode由STILL_ACTIVE转变为传入退出代码；与CreateThread对应

## 2. TerminateThread()

```cpp
BOOL TerminateThread(
  HANDLE hThread,
  DWORD dwExitCode
);
```

说明：

| Header    | Library     |
| --------- | ----------- |
| winbase.h | coredll.lib |

 

 

参数：

```
hThread: 要结束的线程句柄
dwExitCode: 指定线程的退出代码。可以通过GetExitCodeThread来查看一个线程的退出代码
```

返回值：0表示失败，非0表示成功;

## 3. 判断线程是否结束

```cpp
BOOL GetExitCodeThread(
  HANDLE hThread,
  LPDWORD lpExitCode
);
```



```cpp
//判断
bool IsThreadExit(HANDLE hThread)
{
    bool bRet = false;
    DWORD dwExitCode;
    if(GetExitCodeThread(hThread, &dwExitCode))
    {
        if(dwExitCode != STILL_ACTIVE)
            bRet = true;
    }
    else
    {
        //error
        err = GetLastError();   
        throw err;
    }
    return bRet; 
}
```



# 三、注意事项

## 1.在C++多线程编程中，尽量使用_beginthreadex及_endthreadex，而不是其它接口。

### 不使用_beginthread原因：

（1）_beginthread函数参数不够多，某些要求达不到,

不使用_endthread原因：

（1）_endthread函数也是无参的，即线程的退出代码会被硬编码为0；

（2）该函数在调用ExitThread前，会调用CloseHandle，并传入新线程的句柄。类似下面代码会有错误

```cpp
DWORD dwExitCode;
HANDLE hThread = _beginthreadex(...);
GetExitCodeThread(hThread, &dwExitCode);
CloseHandle(hThread);
```

### 不使用CreateThread函数原因：

（1）标准C/C++运行库最初并不是为多线程程序而设计的（标准的C运行时库出现在操作系统对线程支持之前），而CreateThread是操作系统接口，调用它时系统不知道是C/C++来调用的，因此为了保证C/C++程序正常运行，要创建一个数据结构与运行库的每个线程关联，_beginthreadex就实现了这样的功能。换言之，在C/C++中用CreateThread创建线程是极度不安全的。

### 不使用ExitThread函数原因：

（1）操作系统相关资源释放；但象C++类并未析构，造成内存泄露；这里如果用_beginthreadex建立线程，而用ExitThread或者_endthread来释放线程，则线程放在堆上的线程数据块_tiddata也未释放，内存泄露；

## 2.C/C++编程中使用CreateThread会发生什么

当线程调用一个需要线程数据块_tiddata的运行库函数时，系统会首先通过线程局部存储（TLS，见下节）来找到线程数据块，若为NULL，C/C++运行库会主调线程分配并初始化一个_tiddata块并与线程关联。但若使用C/C++运行库的signal函数，则整个进程都会终止（因结构化异常处理帧SEH未就绪，RtlUserThreadStart会直接调用ExitProcess来结束进程）；此外，若不通过_endthreadex来结束线程，线程数据块_tiddata不会释放，造成内存泄露。







1.线程内核对象(操作系统接口CreateThread内部实现)

2.线程数据块_tiddata(C/C++运行时库的实现 _beginthreadex与_beginthread)

3.线程结束_endthreadex

下面分别介绍

## 一、线程内核对象

[![线程内核对象](http://images.cnblogs.com/cnblogs_com/t427/201211/201211271506455785.jpg)](http://images.cnblogs.com/cnblogs_com/t427/201211/201211271506451848.jpg)

线程创建时，会先创建一个线程内核对象（分配在进程的地址空间上），如上图，存储上下文context(一个数据结构)及一些统计信息，具体包括：

1.寄存器SP：指向栈中线程函数指针的地址

2.寄存器IP：指向装载的NTDLL.dll里RtlUserThreadStart函数地址

3.Usage Count：引用计数，初始化为2

4.Suspend Count：挂起数，初始化为1。

5.ExitCode：退出代码，线程在运行时为STILL_ACTIVE(且初始化为该值)

6.Signaled：初始化为未触发状态

### RtlUserThreadStart(...)

函数原型如下：

[![RtlUserThreadStart](http://images.cnblogs.com/cnblogs_com/t427/201211/201211271506465502.jpg)](http://images.cnblogs.com/cnblogs_com/t427/201211/201211271506468850.jpg)

RtlUserThreadStart函数是线程真正起始执行的地方，因为新线程的指令指针是指向这个函数 。RtlUserThreadStart函数原型使你认为它收到二个参数，但那不是真的，这只是意味着它被调用自另一个函数。新的线程只不过在这里生效和开始执行。 
RtlUserThreadStart函数之所以认为被调用自另一个函数，是因为它获得了二个参数。但是获得这些参数的途径是由于操作系统把这些值直接写入了线程的堆栈（通常参数被传递到函数的方法）。 
注意，一些CPU架构用CPU寄存器来传递参数，而不是堆栈。对于这些架构，系统在同意线程执行RtlUserThreadStart函数之前会对相应的寄存器进行正确的初始化。 
(在32位Windows中用的是BaseThreadStart函数而不是64位Windows中的RtlUserThreadStart函数，BaseThreadStart函数来出自Kernel32.dll组件，而RtlUserThreadStart函数函数来出自于NTDLL.dll组件)

RtlUserThreadStart函数是线程真正开始执行的地方，在函数中

（1）设置了一个围绕线程函数的结构化异常处理SEH帧

（2）线程函数返回时，调用ExitThread，并将线程函数返回值作为参数传递进去。线程内核对象的使用计数递减，而后线程停止执行。

（3）执行期间若发生未被处理的异常，则调用异常处理块中的ExitProgress()关闭进程

当一个程序运行时，会生成一个主线程，之后RtlUserThreadStart开始执行，调用C/C++运行库的代码，后者初始化继而访问你的程序入口函数（_tmain,_tWinMain等）；入口函数返回时，C/C++运行时启动代码会调用ExitProcess来结束进程。

因此使用CreateThread生成线程应有二步

（1）生成线程内核对象并初始化

（2）由内核对象指向的RtlUserThreadStart运行线程函数

## 二、线程数据块_tiddata

线程数据块是_beginthreadex函数维护的一个数据结构，存储了线程相关的一些信息。我们先来看_beginthreadex的源码（VS2008的存储在C:\Program Files\Microsoft Visual Studio 9.0\VC\crt\src\threadex.c中）：

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```cpp
_MCRTIMP uintptr_t __cdecl _beginthreadex (
        void *security,
        unsigned stacksize,
        unsigned (__CLR_OR_STD_CALL * initialcode) (void *),
        void * argument,
        unsigned createflag,
        unsigned *thrdaddr
        )
{
        _ptiddata ptd;                  /* pointer to per-thread data */
        uintptr_t thdl;                 /* thread handle */
        unsigned long err = 0L;     /* Return from GetLastError() */
        unsigned dummyid;               /* dummy returned thread ID */

        /* validation section */
        _VALIDATE_RETURN(initialcode != NULL, EINVAL, 0);

        /* Initialize FlsGetValue function pointer */
        __set_flsgetvalue();

        /*
         * Allocate and initialize a per-thread data structure for the to-
         * be-created thread.
         */
        if ( (ptd = (_ptiddata)_calloc_crt(1, sizeof(struct _tiddata))) == NULL )
                goto error_return;

        /*
         * Initialize the per-thread data
         */

        _initptd(ptd, _getptd()->ptlocinfo);

        ptd->_initaddr = (void *) initialcode;
        ptd->_initarg = argument;
        ptd->_thandle = (uintptr_t)(-1);

#if defined (_M_CEE) || defined (MRTDLL)
        if(!_getdomain(&(ptd->__initDomain)))
        {
            goto error_return;
        }
#endif  /* defined (_M_CEE) || defined (MRTDLL) */

        /*
         * Make sure non-NULL thrdaddr is passed to CreateThread
         */
        if ( thrdaddr == NULL )
                thrdaddr = &dummyid;

        /*
         * Create the new thread using the parameters supplied by the caller.
         */
        if ( (thdl = (uintptr_t)
              CreateThread( (LPSECURITY_ATTRIBUTES)security,
                            stacksize,
                            _threadstartex,
                            (LPVOID)ptd,
                            createflag,
                            (LPDWORD)thrdaddr))
             == (uintptr_t)0 )
        {
                err = GetLastError();
                goto error_return;
        }

        /*
         * Good return
         */
        return(thdl);

        /*
         * Error return
         */
error_return:
        /*
         * Either ptd is NULL, or it points to the no-longer-necessary block
         * calloc-ed for the _tiddata struct which should now be freed up.
         */
        _free_crt(ptd);

        /*
         * Map the error, if necessary.
         *
         * Note: this routine returns 0 for failure, just like the Win32
         * API CreateThread, but _beginthread() returns -1 for failure.
         */
        if ( err != 0L )
                _dosmaperr(err);

        return( (uintptr_t)0 );
}
```



其中被标红加粗的二部分是重点，即首先初始化了一个线程数据块（_ptiddata ptd），将线程函数地址及参数设置到线程数据块内，该块是分配在堆上的。之后调用CreateThread函数创建线程，要注意传入该函数的参数，即要运行的函数_threadstartex（注意不是线程函数）, 其参数是线程数据块(LPVOID)ptd

 _threadstartex的功能是

1.将新建线程与内存数据块关联（__fls_setvalue，该函数是操作系统函数，即所谓的线程局部存储（Thread Local Storage, TLS））

2.调用_callthreadstartex来执行及终结真正的线程函数



```cpp
static unsigned long WINAPI _threadstartex (
        void * ptd
        )
{
        _ptiddata _ptd;                  /* pointer to per-thread data */

        /* Initialize FlsGetValue function pointer */
        __set_flsgetvalue();

        /*
         * Check if ptd is initialised during THREAD_ATTACH call to dll mains
         */
        if ( ( _ptd = (_ptiddata)__fls_getvalue(__get_flsindex())) == NULL)
        {
            /*
             * Stash the pointer to the per-thread data stucture in TLS
             */
            if ( !__fls_setvalue(__get_flsindex(), ptd) )
                ExitThread(GetLastError());
            /*
             * Set the thread ID field -- parent thread cannot set it after
             * CreateThread() returns since the child thread might have run
             * to completion and already freed its per-thread data block!
             */
            ((_ptiddata) ptd)->_tid = GetCurrentThreadId();
        }
        else
        {
            _ptd->_initaddr = ((_ptiddata) ptd)->_initaddr;
            _ptd->_initarg =  ((_ptiddata) ptd)->_initarg;
            _ptd->_thandle =  ((_ptiddata) ptd)->_thandle;
#if defined (_M_CEE) || defined (MRTDLL)
            _ptd->__initDomain=((_ptiddata) ptd)->__initDomain;
#endif  /* defined (_M_CEE) || defined (MRTDLL) */
            _freefls(ptd);
            ptd = _ptd;
        }


        /*
         * Call fp initialization, if necessary
         */
#ifndef MRTDLL
#ifdef CRTDLL
        _fpclear();
#else  /* CRTDLL */
        if (_FPmtinit != NULL &&
            _IsNonwritableInCurrentImage((PBYTE)&_FPmtinit))
        {
            (*_FPmtinit)();
        }
#endif  /* CRTDLL */
#endif  /* MRTDLL */

#if defined (_M_CEE) || defined (MRTDLL)
        DWORD domain=0;
        if(!_getdomain(&domain))
        {
            ExitThread(0);
        }
        if(domain!=_ptd->__initDomain)
        {
            /* need to transition to caller's domain and startup there*/
            ::msclr::call_in_appdomain(_ptd->__initDomain, _callthreadstartex);

            return 0L;
        }
#endif  /* defined (_M_CEE) || defined (MRTDLL) */

        _callthreadstartex();

        /*
         * Never executed!
         */
        return(0L);
}
```





```cpp
static void _callthreadstartex(void)
{
    _ptiddata ptd;           /* pointer to thread's _tiddata struct */

    /* must always exist at this point */
    ptd = _getptd();

    /*
        * Guard call to user code with a _try - _except statement to
        * implement runtime errors and signal support
        */
    __try {
            _endthreadex (
                ( (unsigned (__CLR_OR_STD_CALL *)(void *))(((_ptiddata)ptd)->_initaddr) ) ( ((_ptiddata)ptd)->_initarg ) ) ;
    }
    __except ( _XcptFilter(GetExceptionCode(), GetExceptionInformation()) )
    {
            /*
                * Should never reach here
                */
            _exit( GetExceptionCode() );

    } /* end of _try - _except */

}
```



_callthreadstartex函数功能如下：

1. 如上标红地方运行真正线程函数

2. 将真正线程函数运行完的返回值作为返回代码传递给_endthreadex结束该线程

至此，_beginthreadex就运行完毕了。

这里_callthreadstartex调用_endthreadex直接删除线程，而不是回退到_threadstartex，再到RtlUserThreadStart, 若直接返回的话，线程数据块并未删除，会造成内存泄露。

总结下_beginthreadex的运行过程

1.先生成并初始化_tiddata内存块，将线程函数地址及参数传递进去

2.调用CreateThread生成线程，运用RtlUserThreadStart函数运行线程函数（但要运行的函数为_threadstartex，参数为线程数据块地址）

3._threadstartex将通过线程局部存储（TLS）将线程数据块与运行线程绑定

4._threadstartex调用_callthreadstartex，运行真正的线程函数，当真正线程函数正确返回后用_endthreadex结束；若出错，返回0；

下面附上_tiddata的具体内容，可以参考下（VS2008，C:\Program Files\Microsoft Visual Studio 9.0\VC\crt\src\mtdll.h）。



```cpp
struct _tiddata {
    unsigned long   _tid;       /* thread ID */


    uintptr_t _thandle;         /* thread handle */

    int     _terrno;            /* errno value */
    unsigned long   _tdoserrno; /* _doserrno value */
    unsigned int    _fpds;      /* Floating Point data segment */
    unsigned long   _holdrand;  /* rand() seed value */
    char *      _token;         /* ptr to strtok() token */
    wchar_t *   _wtoken;        /* ptr to wcstok() token */
    unsigned char * _mtoken;    /* ptr to _mbstok() token */

    /* following pointers get malloc'd at runtime */
    char *      _errmsg;        /* ptr to strerror()/_strerror() buff */
    wchar_t *   _werrmsg;       /* ptr to _wcserror()/__wcserror() buff */
    char *      _namebuf0;      /* ptr to tmpnam() buffer */
    wchar_t *   _wnamebuf0;     /* ptr to _wtmpnam() buffer */
    char *      _namebuf1;      /* ptr to tmpfile() buffer */
    wchar_t *   _wnamebuf1;     /* ptr to _wtmpfile() buffer */
    char *      _asctimebuf;    /* ptr to asctime() buffer */
    wchar_t *   _wasctimebuf;   /* ptr to _wasctime() buffer */
    void *      _gmtimebuf;     /* ptr to gmtime() structure */
    char *      _cvtbuf;        /* ptr to ecvt()/fcvt buffer */
    unsigned char _con_ch_buf[MB_LEN_MAX];
                                /* ptr to putch() buffer */
    unsigned short _ch_buf_used;   /* if the _con_ch_buf is used */

    /* following fields are needed by _beginthread code */
    void *      _initaddr;      /* initial user thread address */
    void *      _initarg;       /* initial user thread argument */

    /* following three fields are needed to support signal handling and
     * runtime errors */
    void *      _pxcptacttab;   /* ptr to exception-action table */
    void *      _tpxcptinfoptrs; /* ptr to exception info pointers */
    int         _tfpecode;      /* float point exception code */

    /* pointer to the copy of the multibyte character information used by
     * the thread */
    pthreadmbcinfo  ptmbcinfo;

    /* pointer to the copy of the locale informaton used by the thead */
    pthreadlocinfo  ptlocinfo;
    int         _ownlocale;     /* if 1, this thread owns its own locale */

    /* following field is needed by NLG routines */
    unsigned long   _NLG_dwCode;

    /*
     * Per-Thread data needed by C++ Exception Handling
     */
    void *      _terminate;     /* terminate() routine */
    void *      _unexpected;    /* unexpected() routine */
    void *      _translator;    /* S.E. translator */
    void *      _purecall;      /* called when pure virtual happens */
    void *      _curexception;  /* current exception */
    void *      _curcontext;    /* current exception context */
    int         _ProcessingThrow; /* for uncaught_exception */
    void *              _curexcspec;    /* for handling exceptions thrown from std::unexpected */
#if defined (_M_IA64) || defined (_M_AMD64)
    void *      _pExitContext;
    void *      _pUnwindContext;
    void *      _pFrameInfoChain;
    unsigned __int64    _ImageBase;
#if defined (_M_IA64)
    unsigned __int64    _TargetGp;
#endif  /* defined (_M_IA64) */
    unsigned __int64    _ThrowImageBase;
    void *      _pForeignException;
#elif defined (_M_IX86)
    void *      _pFrameInfoChain;
#endif  /* defined (_M_IX86) */
    _setloc_struct _setloc_data;

#ifdef _M_IX86
    void *      _encode_ptr;    /* EncodePointer() routine */
    void *      _decode_ptr;    /* DecodePointer() routine */
#endif  /* _M_IX86 */

    void *      _reserved1;     /* nothing */
    void *      _reserved2;     /* nothing */
    void *      _reserved3;     /* nothing */

    int _cxxReThrow;        /* Set to True if it's a rethrown C++ Exception */

    unsigned long __initDomain;     /* initial domain used by _beginthread[ex] for managed function */

};

typedef struct _tiddata * _ptiddata;
```



## 三、线程终结

上文文中_callthreadstartex函数用_endthreadex来终结线程，源码如下：



```cpp
void __cdecl _endthreadex (
        unsigned retcode
        )
{
        _ptiddata ptd;           /* pointer to thread's _tiddata struct */

        /*
         * Call fp termination, if necessary
         */
#ifdef CRTDLL
        _fpclear();
#else  /* CRTDLL */
        if (_FPmtterm != NULL &&
            _IsNonwritableInCurrentImage((PBYTE)&_FPmtterm))
        {
            (*_FPmtterm)();
        }
#endif  /* CRTDLL */

        ptd = _getptd_noexit();

        if (ptd) {
            /*
             * Free up the _tiddata structure & its subordinate buffers
             *      _freeptd() will also clear the value for this thread
             *      of the FLS variable __flsindex.
             */
            _freeptd(ptd);
        }

        /*
         * Terminate the thread
         */
        ExitThread(retcode);

}
```



所以，_endthreadex的功能如下：

1.删除与该线程相关的线程数据块

2.调用ExitThread(与CreateThread相对)终结并传递退出代码