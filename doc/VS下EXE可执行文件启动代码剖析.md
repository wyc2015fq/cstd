# VS下EXE可执行文件启动代码剖析

2016年12月28日 22:19:50 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：737



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/53914526



# （1）main/WinMain函数

​    当我们在学C/C++语言编程的时候，老师总是跟我们说程序是从Main函数开始的，然而当我第一次用OD分析一个可以执行文件EXE的时候，找这个Main函数就花了我好长时间，后来才知道在Windows下不同的编译器都会在生产最后的可执行PE文件的时候，在我们写的启动函数之前加入一些编译器附带的启动代码，来做一些初始化工作，比如初始化全局变量，运行全局类对象的构造函数，初始化C运行时库等等。对于有（知其然知其所以然）强迫症的我，为此在网上爬了不少帖，也自己调试分析了好几天VS的启动代码，所以贴出来备忘。

​    在程序进入main/WinMain函数之前，通过在Visual Studio中调试，通过栈回溯可以找到位于crt0.c中的_tmainCRTStartup函数，这个函数负责进行一些初始化操作，因为C运行库代码又有两个版本，如果是静态编译的话代码位于crt0.c之中，如果是动态编译的话代码位于crtexe.c之中，这里可以通过项目属性的“配置属性”——“C/C++”——“代码生成”——“运行库”的MT和MD进行设置。

根据工程的类型的不同（Win32工程和Console工程），以及工程编码的不同（Unicode与多字节），实际的入口函数会有四种不同的可能，_tmainCRTStartup被设置为一个红，根据工程的设置，实际的名字选取其中的一种：



```cpp
static
int
__tmainCRTStartup(
         void
         );
 
#ifdef _WINMAIN_
 
#ifdef WPRFLAG
int wWinMainCRTStartup(
#else  /* WPRFLAG */
int WinMainCRTStartup(
#endif  /* WPRFLAG */
#else  /* _WINMAIN_ */
#ifdef WPRFLAG
int wmainCRTStartup(
#else  /* WPRFLAG */
int mainCRTStartup(
#endif  /* WPRFLAG */
#endif  /* _WINMAIN_ */
        void
        )
{
        /*
         * The /GS security cookie must be initialized before any exception
         * handling targetting the current image is registered.  No function
         * using exception handling can be called in the current image until
         * after __security_init_cookie has been called.
         */
        __security_init_cookie();
 
        return __tmainCRTStartup();
}
```



_tmainCRTStartup实际上是__tmainCRTStartup的一个包装函数，在调用后者之前，对cookie进行了初始化操作，如果设置了/GS选项的话，在函数调用过程中，建立栈帧的时候会设置一个cookie，函数返回之前会校验cookie是否一致，简单的判断是否发出缓冲区溢出。 [Windows下程序入口函数与_security_init_cookie](http://blog.csdn.net/wangpengk7788/article/details/53891857)这里有关系cookie的详细分析

先来看下使用静态C运行库（/MT /MTd）时的代码

位于crt0.c

```cpp
int
__tmainCRTStartup(
         void
         )
{
        int initret;
        int mainret=0;
        int managedapp;
#ifdef _WINMAIN_
        _TUCHAR *lpszCommandLine;
        STARTUPINFOW StartupInfo;
 
        GetStartupInfoW( &StartupInfo );
#endif  /* _WINMAIN_ */
 
#ifdef _M_IX86
        // 对于32位程序，设置为如果检测到堆被破坏则自动结束进程
        // 64位程序默认就设置了这个行为
        if (!_NoHeapEnableTerminationOnCorruption)
        {
            HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
        }
#endif  /* _M_IX86 */
 
        // 检测PE头中的标志，判断是否为.net托管程序
        managedapp = check_managed_app();
        // ======================================================
        // 堆初始化操作
        // 对于32位程序而言，_heap_init通过CreateHeap创建一个堆
        // ======================================================
        if ( !_heap_init() )                /* initialize heap */
            fast_error_exit(_RT_HEAPINIT);  /* write message and die */
        // 初始化多线程环境，暂时不做分析
        if( !_mtinit() )                    /* initialize multi-thread */
            fast_error_exit(_RT_THREAD);    /* write message and die */
 
        _CrtSetCheckCount(TRUE);
 
#ifdef _RTC
        _RTC_Initialize();
#endif  /* _RTC */
 
        __try {
            // I/O初始化，暂时不做分析
            if ( _ioinit() < 0 )            /* initialize lowio */
                _amsg_exit(_RT_LOWIOINIT);
            // 获取命令行参数
            /* get wide cmd line info */
            _tcmdln = (_TSCHAR *)GetCommandLineT();
            // 获取环境变量参数
            _tenvptr = (_TSCHAR *)GetEnvironmentStringsT();
            // 解析并设置命令行参数
            if ( _tsetargv() < 0 )
                _amsg_exit(_RT_SPACEARG);
            // 解析并设置环境变量参数
            if ( _tsetenvp() < 0 )
                _amsg_exit(_RT_SPACEENV);
            // 初始化全局数据和浮点寄存器
            initret = _cinit(TRUE);                  /* do C data initialize */
            if (initret != 0)
                _amsg_exit(initret);
            // 进入(w)WinMain或者(w)main函数
#ifdef _WINMAIN_
            lpszCommandLine = _twincmdln();
            mainret = _tWinMain( (HINSTANCE)&__ImageBase,
                                 NULL,
                                 lpszCommandLine,
                                 StartupInfo.dwFlags & STARTF_USESHOWWINDOW
                                      ? StartupInfo.wShowWindow
                                      : SW_SHOWDEFAULT
                                );
#else  /* _WINMAIN_ */
            _tinitenv = _tenviron;
            mainret = _tmain(__argc, _targv, _tenviron);
#endif  /* _WINMAIN_ */
 
            if ( !managedapp )
                exit(mainret);
 
            _cexit();
 
        }
        // 异常处理
        __except ( _XcptFilter(GetExceptionCode(), GetExceptionInformation()) )
        {
            /*
             * Should never reach here
             */
 
            mainret = GetExceptionCode();
 
            if ( !managedapp )
                _exit(mainret);
 
            _c_exit();
 
        } /* end of try - except */
 
        return mainret;
} 
```



先来看下_Heap_Init

```cpp
int __cdecl _heap_init (void)
{
        ULONG HeapType = 2;
 
        //  Initialize the "big-block" heap first.
        if ( (_crtheap = HeapCreate(0, BYTES_PER_PAGE, 0)) == NULL )  _crtheap  全局堆句柄  mallco 函数使用该句柄
            return 0;
 
#ifdef _WIN64
        // Enable the Low Fragmentation Heap by default on Windows XP and
        // Windows Server 2003.  It's the 8 byte overhead heap, and has
        // generally better performance charateristics than standard heap,
        // particularly for apps that perform lots of small allocations.
 
        if (LOBYTE(GetVersion()) < 6)
        {
            HeapSetInformation(_crtheap, HeapCompatibilityInformation,
                               &HeapType, sizeof(HeapType));
        }
#endif  /* _WIN64 */
        return 1;
}
```

这个函数用HeapCreate创建一个私有堆供标准C函数malloc使用来分配内存，它的第三个参数为0表示堆可以自动增长。因为是静态链接C运行库，所以在C运行库代码是嵌入到应用程序里的，因此_crtheap为应用程序私有，如果该应该程序调用一个同样使用静态运行库的DLL，那么该DLL同样会创建属于该DLL的_crtheap并调用HeapCreate为其获取句柄标识。

来看来malloc的实现位于 smallheap.c文件中

```cpp
void * __cdecl malloc (
        size_t size
        )
{
        return _nh_malloc( size, _newmode );
}
void __cdecl free (
        void * pblock
        )
{
        if ( pblock == NULL )
            return;
 
        HeapFree(_crtheap, 0, pblock);
}
```

_nh_malloc同样位于这个文件中

```cpp
void * __cdecl _nh_malloc (
        size_t size,
        int nhFlag
        )
{
        void * retp;
 
        for (;;) {
 
            retp = HeapAlloc( _crtheap, 0, size );  //使用全局句柄来申请空间
 
            /*
             * if successful allocation, return pointer to memory
             * if new handling turned off altogether, return NULL
             */
 
            if (retp || nhFlag == 0)
                return retp;
 
            /* call installed new handler */
            if (!_callnewh(size))
                return NULL;
 
            /* new handler was successful -- try to allocate again */
        }
}
```



在这个C文件中也有一个初始化堆的函数

```cpp
int __cdecl _heap_init (
        void
        )
{
        if ( (_crtheap = HeapCreate( 0, BYTES_PER_PAGE, 0 )) == NULL )
            return 0;
 
        return 1;
}
```

通过翻代码可以找到它在dllcrt0.c中和crtlib.c被调用



dllcrt0.c

```cpp
BOOL WINAPI _CRT_INIT(       DLL使用静态运行库时被调用
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
        /*
         * Start-up code only gets executed when the process is initialized
         */
 
        if ( dwReason == DLL_PROCESS_ATTACH )
        {
            if ( !_heap_init() )    /* initialize heap */
                return FALSE;  
```





crtlib.c



```cpp
__CRTDLL_INIT(               //使用动态运行库时被调用
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
        if ( dwReason == DLL_PROCESS_ATTACH ) {
            if ( !_heap_init() )    /* initialize heap */
```





上面使用静态C运行库的DLL程序的启动代码的一部分，和使用动态运行库msvcrxx.dll时被调用的一部分，因此不难理解，当我们调用一个使用静态C运行库的DLL，或者一个使用动态C运行库的DLL， 他们分别创建和使用自己的私有堆，并用各自的_crtheap全局句柄来标识。如果这个DLL有个导出的函数，该函数使用malloc申请了一块内存并返回指针。

当我们编写一个使用静态运行库的EXE来调用这个DLL导出的该函数的时候，用Free来释放已申请的内存，会导致程序崩溃就不难理解了，相应的如果EXE用动态运行库来编写，那么DLL使用静态库时报错，使用动态运行库时候工作正常，因为它们调用的都是msvcrxx.DLL中的malloc 和 free,自然引用的是同一个_crtheap全局句柄，该句柄在mscrcxx.dll中。





由此我们不难看到在编写Windows程序的时候如果大量使用C标准库函数，应该使用动态运行库来避免一些不必要的错误。

使用C++的 new关键字来为对象分配空间的时候同样是使用_crtheap为标识 使用HeapAlloc来申请堆内存的。因此应该在同一个模块中使用NEW 和Delete来申请和释放堆对象





# （2）_mtinit函数



紧接上文，来看看另外一个函数_mtinit,这个函数定义在tidtable.c文件中



```cpp
/****
*_mtinit() - Init multi-thread data bases
*
*Purpose:
*       (1) Call _mtinitlocks to create/open all lock semaphores.
*       (2) Allocate a TLS index to hold pointers to per-thread data
*           structure.
*
*       NOTES:
*       (1) Only to be called ONCE at startup
*       (2) Must be called BEFORE any mthread requests are made
*
*Entry:
*       <NONE>
*Exit:
*       returns FALSE on failure
*
*Uses:
*       <any registers may be modified at init time>
*
*Exceptions:
*
*******************************************************************************/
int __cdecl _mtinit (
        void
        )
{
        _ptiddata ptd; /_tiddata结构体的指针
 
#ifdef _M_IX86
        /*
         * Initialize fiber local storage function pointers.
         */
 
        HINSTANCE hKernel32 = GetModuleHandleW(L"KERNEL32.DLL");
        if (hKernel32 == NULL) {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }
 
        gpFlsAlloc = (PFLS_ALLOC_FUNCTION)GetProcAddress(hKernel32,
                                                            "FlsAlloc");
 
        gpFlsGetValue = (PFLS_GETVALUE_FUNCTION)GetProcAddress(hKernel32,
                                                                "FlsGetValue");
 
        gpFlsSetValue = (PFLS_SETVALUE_FUNCTION)GetProcAddress(hKernel32,
                                                                "FlsSetValue");
 
        gpFlsFree = (PFLS_FREE_FUNCTION)GetProcAddress(hKernel32,
                                                        "FlsFree");
        if (!gpFlsAlloc || !gpFlsGetValue || !gpFlsSetValue || !gpFlsFree) {
            gpFlsAlloc = (PFLS_ALLOC_FUNCTION)__crtTlsAlloc;
 
            gpFlsGetValue = (PFLS_GETVALUE_FUNCTION)TlsGetValue;
 
            gpFlsSetValue = (PFLS_SETVALUE_FUNCTION)TlsSetValue;
            gpFlsFree = (PFLS_FREE_FUNCTION)TlsFree;   
        }
                                                              //填充这几个全局函数指针 	Fls系列是纤程的局部储存函数，区别于线程局部储存Tls系列
        /*
         * Allocate and initialize a TLS index to store FlsGetValue pointer
         * so that the FLS_* macros can work transparently
         */
 
        if ( (__getvalueindex = TlsAlloc()) == TLS_OUT_OF_INDEXES ||
             !TlsSetValue(__getvalueindex, (LPVOID)gpFlsGetValue) ) {
            return FALSE;                                                   //初始化全局线程局部标识__getvalueindex，用来标识FlsGetValue函数的指针
 
        }
#endif  /* _M_IX86 */
        _init_pointers();       /* initialize global function pointers */  初始化一些全局函数的指针
 
#ifdef _M_IX86
        /*
         * Encode the fiber local storage function pointers
         */
 
        gpFlsAlloc = (PFLS_ALLOC_FUNCTION) EncodePointer(gpFlsAlloc);
        gpFlsGetValue = (PFLS_GETVALUE_FUNCTION) EncodePointer(gpFlsGetValue);
        gpFlsSetValue = (PFLS_SETVALUE_FUNCTION) EncodePointer(gpFlsSetValue);
        gpFlsFree = (PFLS_FREE_FUNCTION) EncodePointer(gpFlsFree);    //对函数指针加密
#endif  /* _M_IX86 */
 
        /*
         * Initialize the mthread lock data base
         */
 
        if ( !_mtinitlocks() ) {
            _mtterm();
            return FALSE;       /* fail to load DLL */    初始化多线程同步时用到的一个结构
        }
 
        /*
         * Allocate a TLS index to maintain pointers to per-thread data
         */
        if ( (__flsindex = FLS_ALLOC(&_freefls)) == FLS_OUT_OF_INDEXES ) {
            _mtterm();
            return FALSE;       /* fail to load DLL */   //__flsindex是一个全局的线程局部储存标识，标识每个线程的tiddate
        }
 
        /*
         * Create a per-thread data structure for this (i.e., the startup)
         * thread.
         */
        if ( ((ptd = _calloc_crt(1, sizeof(struct _tiddata))) == NULL) ||
             !FLS_SETVALUE(__flsindex, (LPVOID)ptd) )              //为tiddate结构分配对空间          
        {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }
 
        /*
         * Initialize the per-thread data
         */
 
        _initptd(ptd,NULL);   //初始化tiddate结构
 
        ptd->_tid = GetCurrentThreadId();
        ptd->_thandle = (uintptr_t)(-1);
 
        return TRUE;
}
```

从该函数的注释看这个函数主要做了两方面的工作



```cpp
*       (1) Call _mtinitlocks to create/open all lock semaphores.
*       (2) Allocate a TLS index to hold pointers to per-thread data
*           structure.
```

1、调用_mtinitlocks建立一个临界区对象表，该对象表保存一组CRITICAL_SECTION对象的指针，这些指针的一部分在调用该函数的时候初始化，他们被初始化为指向一个静态的CRITICAL_SECTION全局数组，这些临界区对象用来保证多线程环境下调用C/C++标准库函数时候能够多线程互斥，从而保证这些库函数对一些全局静态数据的访问能够在多线程环境下稳定工作，下文详细分析。



2、分配一个线程局部索引标记，为每个线程存储一个_tiddate结构体，该结构体同样是为C/C++标准库函数在多线程环境下工作提供支持，一些函数的实现中，用每个线程的_tiddate结构中的数据来当做全局变量工作。





首先  _ptiddata ptd;这里是一个结构体的指针 typedef struct _tiddata * _ptiddata;  

_tiddata是一个在crt中很重要的结构体，每个程序的主线程以及使用_beginthreadex来启动的线程都默认拥有这样一个结构体，通过线程本地储存来保存它的指针

他定义在mtdll.h文件中



```cpp
/* Structure for each thread's data */
 
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
 
    void *      _reserved1;     /* nothing */
    void *      _reserved2;     /* nothing */
    void *      _reserved3;     /* nothing */
#ifdef _M_IX86
    void *      _reserved4;     /* nothing */
    void *      _reserved5;     /* nothing */
#endif  /* _M_IX86 */
 
    int _cxxReThrow;        /* Set to True if it's a rethrown C++ Exception */
 
    unsigned long __initDomain;     /* initial domain used by _beginthread[ex] for managed function */
};
 
typedef struct _tiddata * _ptiddata;
```

都有英文注释，不翻译了



这个结构体的阐述放到后面









下面开始一点一点的来看_mtinit 做了哪些工作



首先 初始化纤程本地存储 相关的几个函数指针

gpFlsAlloc   gpFlsGetValue  gpFlsSetValue  gpFlsFree    因为这一系列函数在VISTA 和Server 2003及之后才支持，因此定义了在宏#ifdef _M_IX86内，在32位系统中从kernel32.dll中动态获取函数地址，并且加密该函数指针



```cpp
        gpFlsAlloc = (PFLS_ALLOC_FUNCTION) EncodePointer(gpFlsAlloc);
        gpFlsGetValue = (PFLS_GETVALUE_FUNCTION) EncodePointer(gpFlsGetValue);
        gpFlsSetValue = (PFLS_SETVALUE_FUNCTION) EncodePointer(gpFlsSetValue);
        gpFlsFree = (PFLS_FREE_FUNCTION) EncodePointer(gpFlsFree); 
```

在mtdll.c文件中有这些函数指针怎么使用的宏定义





```cpp
#ifdef _M_IX86
 
extern PFLS_ALLOC_FUNCTION gpFlsAlloc;
extern PFLS_GETVALUE_FUNCTION gpFlsGetValue;
extern PFLS_SETVALUE_FUNCTION gpFlsSetValue;
extern PFLS_FREE_FUNCTION gpFlsFree;
 
#define FLS_ALLOC(callback)  (((PFLS_ALLOC_FUNCTION) DecodePointer(gpFlsAlloc))(callback))
#define FLS_GETVALUE    ((PFLS_GETVALUE_FUNCTION)TlsGetValue(__getvalueindex))
#define FLS_SETVALUE(index, value) (((PFLS_SETVALUE_FUNCTION) DecodePointer(gpFlsSetValue))(index, value))
#define FLS_FREE(index) (((PFLS_FREE_FUNCTION) DecodePointer(gpFlsFree))(index))
 
#else  /* _M_IX86 */
 
#define FLS_ALLOC(callback) FlsAlloc(callback)
#define FLS_GETVALUE(index) FlsGetValue(index)
#define FLS_SETVALUE(index, value) FlsSetValue(index, value)
#define FLS_FREE(index)     FlsFree(index)
```

当32位编译模式的时候 解密函数指针并调用，在64位模式时候直接调用Kernel32.dll导出的接口





然后是

```cpp
 _init_pointers();       /* initialize global function pointers */  初始化一些全局函数的指针
```



该函数定义在crt0dat.c文件中

```cpp
void __cdecl _init_pointers() {
        void *enull = _encoded_null();
 
        _initp_heap_handler(enull);
        _initp_misc_invarg(enull);
        _initp_misc_purevirt(enull);
        _initp_misc_rand_s(enull);
        _initp_misc_winsig(enull);
        _initp_eh_hooks(enull);
}
_CRTIMP void * __cdecl _encoded_null()
{
    return EncodePointer(NULL);    //返回一个对NULL的加密表示空的函数指针
}
```



1、_initp_heap_handler(enull);  定义在handler.cpp文件中



```cpp
/* pointer to old-style C++ new handler */
_PNH _pnhHeap;                                   //typedef int (__clrcall * _PNH)( size_t );一个函数指针
 
/***
*_initp_heap_handler()
*
*Purpose:
*
*******************************************************************************/
 
extern "C"
void __cdecl _initp_heap_handler(void *enull)
{
        _pnhHeap = (_PNH) enull;
}
```

_pnhHeap 是一个函数指针，这里将它设置成一个空的加密指针，该函数指针可以用_set_new_handler来设置它的值，这个函数用作NEW失败的时候调用，可以由我们来指定完成一些工作，关于这个的详细过程可以参考

对new的几种形式的一些认识









2、_initp_misc_invarg(enull);      定义在invarg.c文件中



```cpp
/* global variable which stores the user handler */
 
_invalid_parameter_handler __pInvalidArgHandler;
       //typedef void (__cdecl *_invalid_parameter_handler)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
 
extern "C"
void _initp_misc_invarg(void* enull)
{
    __pInvalidArgHandler = (_invalid_parameter_handler) enull;
}
```



__pInvalidArgHandler函数指针，这里初始化为空加密，它的作用是当传递给一个CRT函数无效的参数时被调用，这个函数指针可以由 _set_invalid_parameter_handler 来设置，



3、 _initp_misc_purevirt(enull);  在inithelp.c文件中定义

```cpp
_purecall_handler __pPurecall= NULL;
 
/***
*void _initp_misc_purevirt(void) -
*
*Purpose:
*       Initialize the __pPurecall function pointer
*
*Entry:
*       The per-process encoded value for the null pointer.
*
*Exit:
*       Never returns
*
*Exceptions:
*
*******************************************************************************/
 
//extern "C"
void __cdecl _initp_misc_purevirt(void* enull)
{
    __pPurecall = (_purecall_handler) enull;
}
```



__pPurecall全局函数指针，它的作用是当调用一个纯虚函数时候被触发，这个指针可以通过_set_purecall_handler来设置成我们自己的函数过程



4、 _initp_misc_rand_s(enull); 在Rand_s.c文件中定义



```cpp
typedef BOOL (APIENTRY *PGENRANDOM)( PVOID, ULONG );
 
static PGENRANDOM g_pfnRtlGenRandom;
 
void __cdecl _initp_misc_rand_s(void* enull)
{
    g_pfnRtlGenRandom = (PGENRANDOM) enull;
}
```





g_pfnRtlGenRandom 全局函数指针，它的作用是返回一个无符号的随机整数，这个指针在rand_s函数中被替换为ADVAPI32.DLL导出的RtlGenRandom函数，参考MSDN 中rand_s函数的使用

5、_initp_misc_winsig(enull); 定义在Winsig.c文件中



```cpp
/*
 * variables holding action codes (and code pointers) for SIGINT, SIGBRK,
 * SIGABRT and SIGTERM.
 *
 * note that the disposition (i.e., action to be taken upon receipt) of
 * these signals is defined on a per-process basis (not per-thread)!!
 */
 
static _PHNDLR ctrlc_action       = SIG_DFL;    /* SIGINT   */
static _PHNDLR ctrlbreak_action   = SIG_DFL;    /* SIGBREAK */
static _PHNDLR abort_action       = SIG_DFL;    /* SIGABRT  */
static _PHNDLR term_action        = SIG_DFL;    /* SIGTERM  */
 
 
void __cdecl _initp_misc_winsig(void* enull)
{
    ctrlc_action       = (_PHNDLR) enull;    /* SIGINT   */
    ctrlbreak_action   = (_PHNDLR) enull;    /* SIGBREAK */
    abort_action       = (_PHNDLR) enull;    /* SIGABRT  */
    term_action        = (_PHNDLR) enull;    /* SIGTERM  */
}
```

_initp_misc_winsig 初始化这四个异常函数指针，这个四个函数指针在相应的 信号处理例程发生异常时候被调用





6、_initp_eh_hooks 没有找这个函数的实现

紧接着是 

```cpp
        if ( !_mtinitlocks() ) {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }
```

_mtinitlocks 这个函数的实现在 Mlock.c文件中

```cpp
/***
*_mtinitlocks() - Initialize multi-thread lock scheme
*
*Purpose:
*       Perform whatever initialization is required for the multi-thread
*       locking (synchronization) scheme. This routine should be called
*       exactly once, during startup, and this must be before any requests
*       are made to assert locks.
*
*       NOTES: In Win32, the multi-thread locks are created individually,
*       each upon its first use. That is when any particular lock is asserted
*       for the first time, the underlying critical section is then allocated,
*       initialized and (finally) entered. This allocation and initialization
*       is protected under _LOCKTAB_LOCK. It is _mtinitlocks' job to set up
*       _LOCKTAB_LOCK.
*
*       All other named (non-FILE) locks are also preallocated in _mtinitlocks.
*       That is because a failure to allocate a lock on its first use in _lock
*       triggers a fatal error, which cannot be permitted since that can bring
*       down a long-lived app without warning.
*
*Entry:
*       <none>
*
*Exit:
*       returns FALSE on failure
*
*Exceptions:
*
*******************************************************************************/
 
int __cdecl _mtinitlocks (
        void
        )
{
        int locknum;
        int idxPrealloc = 0;
 
        /*
         * Scan _locktable[] and allocate all entries marked lkPrealloc.
         */
        for ( locknum = 0 ; locknum < _TOTAL_LOCKS ; locknum++ ) {
            if ( _locktable[locknum].kind == lkPrealloc ) {
                _locktable[locknum].lock = &lclcritsects[idxPrealloc++];
                if ( !InitializeCriticalSectionAndSpinCount( _locktable[locknum].lock,
                                                             _CRT_SPINCOUNT ))
                {
                    _locktable[locknum].lock = NULL;
                    return FALSE;
                }
            }
        }
 
 
        return TRUE;
}
```

这里引用到了两个全局的静态数组_locktable和 lclcritsects,







```cpp
#define NUM_STD_FILE_LOCKS     3
 
/*
 *  _DEBUG_LOCK is preallocated in _DEBUG & not in Retail
 */
 
#ifdef _DEBUG
#define NUM_NON_PREALLOC_LOCKS 4
#else  /* _DEBUG */
#define NUM_NON_PREALLOC_LOCKS 5
#endif  /* _DEBUG */
 
#define NUM_PREALLOC_LOCKS \
        ( _STREAM_LOCKS + NUM_STD_FILE_LOCKS - NUM_NON_PREALLOC_LOCKS )
static CRITICAL_SECTION lclcritsects[NUM_PREALLOC_LOCKS];
 
/*
 * Lock Table
 * This table contains a pointer to the critical section management structure
 * for each lock.
 *
 * Locks marked lkPrealloc have their critical sections statically allocated
 * and initialized at startup in _mtinitlocks.  Locks marked lkNormal must
 * be allocated when first used, via a call to _mtinitlocknum.
 */
static struct {
        PCRITICAL_SECTION lock;
        enum { lkNormal = 0, lkPrealloc, lkDeleted } kind;
} _locktable[_TOTAL_LOCKS] = {
        { NULL, lkPrealloc }, /* 0  == _SIGNAL_LOCK      */
        { NULL, lkPrealloc }, /* 1  == _IOB_SCAN_LOCK    */
        { NULL, lkNormal   }, /* 2  == _TMPNAM_LOCK      - not preallocated */
        { NULL, lkPrealloc }, /* 3  == _CONIO_LOCK       */
        { NULL, lkPrealloc }, /* 4  == _HEAP_LOCK        */
        { NULL, lkNormal   }, /* 5  == _UNDNAME_LOCK     - not preallocated */
        { NULL, lkPrealloc }, /* 6  == _TIME_LOCK        */
        { NULL, lkPrealloc }, /* 7  == _ENV_LOCK         */
        { NULL, lkPrealloc }, /* 8  == _EXIT_LOCK1       */
        { NULL, lkNormal   }, /* 9  == _POPEN_LOCK       - not preallocated */
        { NULL, lkPrealloc }, /* 10 == _LOCKTAB_LOCK     */
        { NULL, lkNormal   }, /* 11 == _OSFHND_LOCK      - not preallocated */
        { NULL, lkPrealloc }, /* 12 == _SETLOCALE_LOCK   */
        { NULL, lkPrealloc }, /* 13 == _MB_CP_LOCK       */
        { NULL, lkPrealloc }, /* 14 == _TYPEINFO_LOCK    */
#ifdef _DEBUG
        { NULL, lkPrealloc }, /* 15 == _DEBUG_LOCK       */
#else  /* _DEBUG */
        { NULL, lkNormal },   /* 15 == _DEBUG_LOCK       */
#endif  /* _DEBUG */
 
        { NULL, lkPrealloc }, /* 16 == _STREAM_LOCKS+0 - stdin  */
        { NULL, lkPrealloc }, /* 17 == _STREAM_LOCKS+1 - stdout */
        { NULL, lkPrealloc }, /* 18 == _STREAM_LOCKS+2 - stderr */
/*      { NULL, lkNormal   }, /* ... */
};
```

可以看到_locktable 是个结构体数组，该类型的结构体有一个临界区对象指针PCRITICAL_SECTION lock;，和一个枚举型变量enum { lkNormal = 0, lkPrealloc, lkDeleted } kind构成。lclcritsects则是一个CRITICAL_SECTION临界区对象数组，该数组的大小为NUM_PREALLOC_LOCKS







```cpp
#define NUM_PREALLOC_LOCKS \
        ( _STREAM_LOCKS + NUM_STD_FILE_LOCKS - NUM_NON_PREALLOC_LOCKS )
 
#define _STREAM_LOCKS   16      /* Table of stream locks            */
#define NUM_STD_FILE_LOCKS     3
 
 
#ifdef _DEBUG
#define NUM_NON_PREALLOC_LOCKS 4
#else  /* _DEBUG */
#define NUM_NON_PREALLOC_LOCKS 5
#endif  /* _DEBUG */
```



在Debug模式下是15 在release下是14  刚好和 _locktable 数组中 枚举量为lkPrealloc的元素数目相等。根据结构体的说 lkNormal类型的在第一次使用时为其分配内存，而lkPrealloc则在程序启动的时候为其分配内存并初始化

```cpp
        for ( locknum = 0 ; locknum < _TOTAL_LOCKS ; locknum++ ) {
            if ( _locktable[locknum].kind == lkPrealloc ) {
                _locktable[locknum].lock = &lclcritsects[idxPrealloc++];      //将类型为lpPrealloc类型的，让指针指向clcritsects数组对应的元素
                if ( !InitializeCriticalSectionAndSpinCount( _locktable[locknum].lock,//  用InitializeCriticalSectionAndSpinCount初始化该临界区对象
                                                             _CRT_SPINCOUNT ))    //_CRT_SPINCOUNT 的值为4000
                {
                    _locktable[locknum].lock = NULL;
                    return FALSE;
                }
            }
        }
```

InitializeCriticalSectionAndSpinCount  跟InitializeCriticalSection的区别在于，前者在尝试获取临街区代码控制权时有个数量的循环尝试过程，每次占用很少的CPU时钟周期，在循环完毕后才进入线程休眠状态，而后者如果获取不到权限直接进入休眠，线程休眠耗用的CPU时钟周期很大，因此在很轻量级的互斥中，如对单个全局变量的访问时，选择前者更为效率。





这个18临界区对象都有各自的用处，他们通过_lock和_unlock两个接口函数来使用



```cpp
***
* _lock - Acquire a multi-thread lock
*
*Purpose:
*       Acquire a multi-thread lock.  If the lock has not already been
*       allocated, do so, but that is an internal CRT error, since all locks
*       should be allocated before first being acquired, either in
*       _mtinitlocks or individually in _mtinitlocknum.
*
*       Note that it is legal for a thread to aquire _EXIT_LOCK1
*       multiple times.
*
*Entry:
*       locknum = number of the lock to aquire
*
*Exit:
*
*Exceptions:
*       A failure to allocate a new lock results in a fatal _RT_LOCK error.
*
*******************************************************************************/
 
void __cdecl _lock (
        int locknum
        )
{
 
        /*
         * Create/open the lock, if necessary
         */
        if ( _locktable[locknum].lock == NULL ) {
 
            if ( !_mtinitlocknum(locknum) )
                _amsg_exit( _RT_LOCK );
        }
 
        /*
         * Enter the critical section.
         */
 
        EnterCriticalSection( _locktable[locknum].lock ); 
}
 
 
/***
* _unlock - Release multi-thread lock
*
*Purpose:
*       Note that it is legal for a thread to aquire _EXIT_LOCK1
*       multiple times.
*
*Entry:
*       locknum = number of the lock to release
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/
 
void __cdecl _unlock (
        int locknum
        )
{
        /*
         * leave the critical section.
         */
        LeaveCriticalSection( _locktable[locknum].lock );
}
#define _mlock(l)               _lock(l)
```

在Mtdll.h文件中定义了 访问每个临界区对象对应的宏

```cpp
#define _SIGNAL_LOCK    0       /* lock for signal()                */
#define _IOB_SCAN_LOCK  1       /* _iob[] table lock                */
#define _TMPNAM_LOCK    2       /* lock global tempnam variables    */
#define _CONIO_LOCK     3       /* lock for conio routines          */
#define _HEAP_LOCK      4       /* lock for heap allocator routines */
#define _UNDNAME_LOCK   5       /* lock for unDName() routine       */
#define _TIME_LOCK      6       /* lock for time functions          */
#define _ENV_LOCK       7       /* lock for environment variables   */
#define _EXIT_LOCK1     8       /* lock #1 for exit code            */
#define _POPEN_LOCK     9       /* lock for _popen/_pclose database */
#define _LOCKTAB_LOCK   10      /* lock to protect semaphore lock table */
#define _OSFHND_LOCK    11      /* lock to protect _osfhnd array    */
#define _SETLOCALE_LOCK 12      /* lock for locale handles, etc.    */
#define _MB_CP_LOCK     13      /* lock for multibyte code page     */
#define _TYPEINFO_LOCK  14      /* lock for type_info access        */
#define _DEBUG_LOCK     15      /* lock for debug global structs    */
 
#define _STREAM_LOCKS   16      /* Table of stream locks            */
```

这些临界区对象，为C运行库函数的多线程支持提供了，防止代码重入对一些静态及全家数据的访问的破坏。



再往下是

```cpp
       if ( (__flsindex = FLS_ALLOC(&_freefls)) == FLS_OUT_OF_INDEXES ) {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }
```



这里FLS_ALLOC就是前面提到的宏  在32位下 调用全家函数指针gpFlsAlloc  在64位下调用FlsAlloc, 这个函数在MSDN中是这样说明的

```
DWORD WINAPI FlsAlloc(
  __in  PFLS_CALLBACK_FUNCTION lpCallback
);
```





- *lpCallback* [in]

  A pointer to the application-defined callback function of type **PFLS_CALLBACK_FUNCTION**. This parameter is optional. For more information, see[**FlsCallback**](http://127.0.0.1:47873/help/1-6108/ms.help?method=page&id=D05A6550-7FEC-44E6-9B38-DFAFFF7895C8&product=VS&productVersion=100&topicVersion=85&locale=ZH-CN&topicLocale=EN-US). 

这里_freefls就是一个回调函数指针，这个回调函数在FlsFree相应的索引标记时被调用





```cpp
_CRTIMP void
WINAPI
_freefls (
    void *data
    )
 
{
 
    _ptiddata ptd;
    pthreadlocinfo ptloci;
    pthreadmbcinfo ptmbci;
 
    /*
     * Free up the _tiddata structure & its malloc-ed buffers.
     */
 
    ptd = data;
    if (ptd != NULL) {
        if(ptd->_errmsg)
            _free_crt((void *)ptd->_errmsg);
 
        if(ptd->_namebuf0)
            _free_crt((void *)ptd->_namebuf0);
 
        if(ptd->_namebuf1)
            _free_crt((void *)ptd->_namebuf1);
 
        if(ptd->_asctimebuf)
            _free_crt((void *)ptd->_asctimebuf);
 
        if(ptd->_wasctimebuf)
            _free_crt((void *)ptd->_wasctimebuf);
 
        if(ptd->_gmtimebuf)
            _free_crt((void *)ptd->_gmtimebuf);
 
        if(ptd->_cvtbuf)
            _free_crt((void *)ptd->_cvtbuf);
 
        if (ptd->_pxcptacttab != _XcptActTab)
            _free_crt((void *)ptd->_pxcptacttab);
 
        _mlock(_MB_CP_LOCK);
        __try {
            if ( ((ptmbci = ptd->ptmbcinfo) != NULL) &&
                 (InterlockedDecrement(&(ptmbci->refcount)) == 0) &&
                 (ptmbci != &__initialmbcinfo) )
                _free_crt(ptmbci);
        }
        __finally {
            _munlock(_MB_CP_LOCK);
        }
 
        _mlock(_SETLOCALE_LOCK);
 
        __try {
            if ( (ptloci = ptd->ptlocinfo) != NULL )
            {
                __removelocaleref(ptloci);
                if ( (ptloci != __ptlocinfo) &&
                     (ptloci != &__initiallocinfo) &&
                     (ptloci->refcount == 0) )
                    __freetlocinfo(ptloci);
            }
        }
        __finally {
            _munlock(_SETLOCALE_LOCK);
        }
 
        _free_crt((void *)ptd);
    }
    return;
```

看以看到  这里_freefls 用来清理释放为线程分配的_tiddate结构。

```cpp
__flsindex = FLS_ALLOC(&_freefls)) == FLS_OUT_OF_INDEXES
```

 



这里FLS_OUT_OF_INDEXES 是一个宏标识 没有分配到索引号，而__flsindex  是一个全局变量来表示FlsAlloc分配的索引号，每个线程的_tiddate结构，在堆中分配内存，然后初始化后通过/!FLS_SETVALUE(__flsindex, (LPVOID)ptd) )将结构体指针保存到 局部储存中去。





再往下是：

```cpp
        if ( ((ptd = _calloc_crt(1, sizeof(struct _tiddata))) == NULL) ||
             !FLS_SETVALUE(__flsindex, (LPVOID)ptd) )
        {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }
```



_calloc_crt  这里为_tiddata结构在堆中分配内存，紧接着_initptd(ptd,NULL); 初始化分配到的结构体空间







```cpp
/***
*void _initptd(_ptiddata ptd, pthreadlocinfo) - initialize a per-thread data structure
*
*Purpose:
*       This routine handles all of the per-thread initialization
*       which is common to _beginthread, _beginthreadex, _mtinit
*       and _getptd.
*
*Entry:
*       pointer to a per-thread data block
*
*Exit:
*       the common fields in that block are initialized
*
*Exceptions:
*
*******************************************************************************/
 
_CRTIMP void __cdecl _initptd (
        _ptiddata ptd,
        pthreadlocinfo ptloci
        )
{
#ifdef _M_IX86
    HINSTANCE hKernel32 = GetModuleHandleW(L"KERNEL32.DLL");
#endif  /* _M_IX86 */
 
    ptd->_pxcptacttab = (void *)_XcptActTab;  //一个异常表，用于控制台信号实现
    ptd->_terrno = 0;                          //每个线程有自己的错误返回码 
    ptd->_holdrand = 1L;
 
    // It is necessary to always have GLOBAL_LOCALE_BIT set in perthread data
    // because when doing bitwise or, we won't get __UPDATE_LOCALE to work when
    // global per thread locale is set.
    ptd->_ownlocale = _GLOBAL_LOCALE_BIT;
 
    // Initialize _setloc_data. These are the only valuse that need to be
    // initialized.
    ptd->_setloc_data._cachein[0]='C';
    ptd->_setloc_data._cacheout[0]='C'; //本地化相关
    ptd->ptmbcinfo = &__initialmbcinfo; //字符相关
 
    _mlock(_MB_CP_LOCK);
    __try
    {
        InterlockedIncrement(&(ptd->ptmbcinfo->refcount));
    }
    __finally
    {
        _munlock(_MB_CP_LOCK);
    }
 
        // We need to make sure that ptd->ptlocinfo in never NULL, this saves us
    // perf counts when UPDATING locale.
    _mlock(_SETLOCALE_LOCK);
    __try {
        ptd->ptlocinfo = ptloci;
        /*
         * Note that and caller to _initptd could have passed __ptlocinfo, but
         * that will be a bug as between the call to _initptd and __addlocaleref
         * the global locale may have changed and ptloci may be pointing to invalid
         * memory. Thus if the wants to set the locale to global, NULL should
         * be passed.
         */
        if (ptd->ptlocinfo == NULL)
            ptd->ptlocinfo = __ptlocinfo;
        __addlocaleref(ptd->ptlocinfo);
    }
    __finally {
        _munlock(_SETLOCALE_LOCK);
    }
}
```

最后


```cpp
ptd->_tid = GetCurrentThreadId();   // 设置线程ID
ptd->_thandle = (uintptr_t)(-1);    // 线程句柄设成-1
```







# （3）_ioinit 函数



在完成了对多线程支持的初始化后 
```cpp
            if ( _ioinit() < 0 )            /* initialize lowio */
                _amsg_exit(_RT_LOWIOINIT);
```


根据注释来看这个函数的功能是初始化IO，IO（或I/O）的全称是Input/Output，即输入和输出。对于计算机来说，I/O代表了计算机与外界的交互，交互的对象可以是人或其他设备

而对于程序来说，I/O涵盖的范围还要宽广一些。一个程序的I/O指代了程序与外界的交互，包括文件、管道、网络、命令行、信号等。更广义地讲，I/O指代任何操作系统理解为"文件"的事务。许多操作系统，包括Linux和Windows，都将各种具有输入和输出概念的实体--包括设备、磁盘文件、命令行等--统称为文件，因此这里所说的文件是一个广义的概念。
对于一个任意类型的文件，操作系统会提供一组操作函数，这包括打开文件、读文件、写文件、移动文件指针等，相信有编程经验的读者对此都不会陌生。有过C编程经验的读者应该知道，C语言文件操作是通过一个FILE结构的指针来进行的。fopen函数返回一个FILE结构的指针，而其他的函数如fwrite使用这个指针操作文件。使用文件的最简单代码如下：

```cpp
#include <stdio.h>
int main(int argc,char** argv)
{
FILE* f = fopen( "test.dat", "wb" );
if( f == NULL )
Return -1;
fwrite( "123", 3, 1, f );
fclose(f);
```

在操作系统层面上，文件操作也有类似于FILE的一个概念，在Linux里，这叫做文件描述符（File Descriptor），而在Windows里，叫做句柄（Handle）（以下在没有歧义的时候统称为句柄）。用户通过某个函数打开文件以获得句柄，此后用户操纵文件皆通过该句柄进行。

设计这么一个句柄的原因在于句柄可以防止用户随意读写操作系统内核的文件对象。无论是Linux还是Windows，文件句柄总是和内核的文件对象相关联的，但如何关联细节用户并不可见。内核可以通过句柄来计算出内核里文件对象的地址，但此能力并不对用户开放。

下面举一个实际的例子，在Linux中，值为0、1、2的fd分别代表标准输入、标准输出和标准错误输出。在程序中打开文件得到的fd从3开始增长。fd具体是什么呢？在内核中，每一个进程都有一个私有的"打开文件表"，这个表是一个指针数组，每一个元素都指向一个内核的打开文件对象。而fd，就是这个表的下标。当用户打开一个文件时，内核会在内部生成一个打开文件对象，并在这个表里找到一个空项，让这一项指向生成的打开文件对象，并返回这一项的下标作为fd。由于这个表处于内核，并且用户无法访问到，因此用户即使拥有fd，也无法得到打开文件对象的地址，只能够通过系统提供的函数来操作。

在C语言里，操纵文件的渠道则是FILE结构，不难想象，C语言中的FILE结构必定和fd有一对一的关系，每个FILE结构都会记录自己唯一对应的fd。

FILE、fd、打开文件表和打开文件对象的关系如图所示。



![img](https://img-blog.csdn.net/20170102203310941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



图FILE结构、fd和内核对象


图1中，内核指针p指向该进程的打开文件表，所以只要有fd，就可以用fd+p来得到打开文件表的某一项地址。stdin、stdout、stderr均是FILE结构的指针。
对于Windows中的句柄，与Linux中的fd大同小异，不过Windows的句柄并不是打开文件表的下标，而是其下标经过某种线性变换之后的结果。


在大致了解了I/O为何物之后，我们就能知道I/O初始化的职责是什么了。首先I/O初始化函数需要在用户空间中建立stdin、stdout、stderr及其对应的FILE结构，使得程序进入main之后可以直接使用printf、scanf等函数。



首先让我们来看看MSVC中，FILE结构的定义（FILE结构实际定义在C语言标准中并未指出，因此不同的版本可能有不同的实现）：







```cpp
struct _iobuf {
        char *_ptr;   //当前文件指针
        int   _cnt;
        char *_base;
        int   _flag;    //访问模式
        int   _file;    //一个索引值，用来对应保存在全局数组中的已分配的文件句柄
        int   _charbuf; //缓冲区
        int   _bufsiz;  //缓冲区大小
        char *_tmpfname;
        };
typedef struct _iobuf FILE;
```

FILE被定义为了一个名为_iobuf的结构体，VC的C运行库是怎么通过这个结构体工作的呢，不如来一步步来看fopen函数是怎么实现的吧



在open.c文件中 fopen的实体是_tfopen



```cpp
/***
*FILE *fopen(file, mode) - open a file
*
*Purpose:
*       Opens the file specified as a stream.  mode determines file mode:
*       "r": read       "w": write      "a": append
*       "r+": read/write                "w+": open empty for read/write
*       "a+": read/append
*       Append "t" or "b" for text and binary mode
*
*Entry:
*       char *file - file name to open
*       char *mode - mode of file access
*
*Exit:
*       returns pointer to stream
*       returns NULL if fails
*
*Exceptions:
*
*******************************************************************************/
 
FILE * __cdecl _tfopen (
        const _TSCHAR *file,
        const _TSCHAR *mode
        )
{
        return( _tfsopen(file, mode, _SH_DENYNO) );
}
```

他只是一个中转 实现在_tfsopen中





```cpp
*******************************************************************************/
 
FILE * __cdecl _tfsopen (
        const _TSCHAR *file,
        const _TSCHAR *mode
        ,int shflag
        )
{
    REG1 FILE *stream=NULL;
    REG2 FILE *retval=NULL;      
 
    _VALIDATE_RETURN((file != NULL), EINVAL, NULL);
    _VALIDATE_RETURN((mode != NULL), EINVAL, NULL);
    _VALIDATE_RETURN((*mode != _T('\0')), EINVAL, NULL);  //检查参数   _VALIDATE_RETURN检查他的第一个表达式结构 如果为0直接返回错误代码
 
 
    /* Get a free stream */
    /* [NOTE: _getstream() returns a locked stream.] */
 
    if ((stream = _getstream()) == NULL)  //在全局流对象数组中找到闲置的，并返回其指针
    {
        errno = EMFILE;
        return(NULL);
    }
 
    __try {
        /* We deliberately don't hard-validate for emptry strings here. All other invalid
        path strings are treated as runtime errors by the inner code in _open and openfile.
        This is also the appropriate treatment here. Since fopen is the primary access point
        for file strings it might be subjected to direct user input and thus must be robust to
        that rather than aborting. The CRT and OS do not provide any other path validator (because
        WIN32 doesn't allow such things to exist in full generality).
        */
 
        if(*file==_T('\0')) //简单的判断文件路径是不是为空，把严格的检查留给后面要调用的运行库函数，和WindowsAPI函数
        {
            errno=EINVAL;
            return NULL;
        }
 
        /* open the stream */
#ifdef _UNICODE
        retval = _wopenfile(file,mode,shflag,stream);
#else  /* _UNICODE */
        retval = _openfile(file,mode,shflag,stream);
#endif  /* _UNICODE */
 
    }
    __finally {
        _unlock_str(stream);
    }
 
    return(retval);
}
```

这里_tfsopen  通过_getstream() 获取了一个FILE指针用作openfile的参数，然后简单的文件打开路径是否为空，然后调用相应版本的openfile函数



_getstream() 函数需要介绍下，它在Stream.c文件中





这个函数用到了两个全局变量 __piob和_nstream





```cpp
/*
 * Pointer to array of FILE * or _FILEX * structures.
 */
void ** __piob;
```





```cpp
#ifdef CRTDLL
int _nstream = _NSTREAM_;
#else  /* CRTDLL */
int _nstream;
#endif  /* CRTDLL */
#define _NSTREAM_   512

```







__piob是一个指针数组，用来保存已存在的流的指针（_FILEX结构体指针），_nstream标识已存在流的数量





```cpp
typedef struct {
        FILE f;
        CRITICAL_SECTION lock; 
        }   _FILEX;
```







```cpp
/***
*FILE *_getstream() - 查找一个未使用的流
*
*目的:
*       查找一个未使用的流并使其对调用者有效。 此函数打算只在库内使用。
*
*进入:
*       无参数. 扫描 __piob[]
*
*退出:
*       返回指向空闲流的指针, 或者 NULL 如果都已被使用.
        一个流成为被指定的状态，如果调用者决定使用 r w r/w 中的任意模式。
*
*       [多线程主意: 如果找到一个空闲流, 它被已锁住的状态返回.  解锁这个流是调用者的责任.]
*
*Exceptions:
*
*******************************************************************************/
 
FILE* _getstream ()
{
    FILE* retval = NULL;                                                           //定义返回值
    int i;
 
    /* Get the iob[] scan lock */
    _mlock(_IOB_SCAN_LOCK);                                                        //<mtdll.h> void _lock(int); 请求线程锁 _IOB_SCAN_LOCK锁ID 用于扫描存储流的表
    __try {
 
        /*
        * 通过 __piob 表循环查找一个空闲流, 或一个NULL结果.
        */
        for ( i = 0 ; i < _nstream ; i++ ) {                                       //<internal.h> _nstream(_NSTREAM_  512) 打开文件的最大数量
 
            if ( __piob[i] != NULL ) {                                             //<internal.h> void** __piob; 指向保存FILE资源地址的表的表头指针 推测：__iob[]的类型为 FILE* __iob[_nstream];
                /*
                * 判断如果流未在使用中，则返回此流.
                */
                if ( !inuse( (FILE *)__piob[i] ) && !str_locked( (FILE *)__piob[i] ) ) {
                    //<file2.h> #define inuse(s) ((s)->_flag & (_IOREAD | _IOWTITE | _IORW))
                    //<file2.h> #define str_locked ((s)->_flag & (_IOLOCKED))
                    //若流的当前状态不是 只读 只写 可读可写 锁定 则表达式为真
                    /*
                    * Allocate the FILE lock, in case it hasn't already been
                    * allocated (only necessary for the first _IOB_ENTRIES
                    * locks, not including stdin/stdout/stderr).  Return
                    * failure if lock can't be allocated.
                    */
                    if ( i > 2 && i < _IOB_ENTRIES )                               //流的下标大于2，小于_IOB_ENTRIES(20)
                        if ( !_mtinitlocknum( _STREAM_LOCKS + i ) )                //<mtdll.h> int _mtinitlocknum(int); 给出空闲ID号，分配一个新的，非预先分配的线程锁。失败返回0。
//#define _STREAM_LOCKS   16      /* 流的线程锁起始ID */
//#define _LAST_STREAM_LOCK  (_STREAM_LOCKS+_IOB_ENTRIES-1) /* 最后一个线程锁ID */
//#define _TOTAL_LOCKS        (_LAST_STREAM_LOCK+1)
                            break;
 
                    _lock_str2(i, __piob[i]);                                      //<mtdll.h> void _lock_file2(int, void*); 用新分配的线程锁_STREAM_LOCKS + i锁定文件__piob[i],在他的内部调用的是_lock。
 
                    if ( inuse( (FILE *)__piob[i] ) ) {
                        _unlock_str2(i, __piob[i]);                                //<mtdll.h> void _unlock_file2(int, void*); 解锁文件，内部调用_unlock。
                        continue;                                                  //若文件已经在使用中，则接着扫描。（不明其用意）
                    }
                    retval = (FILE *)__piob[i];                                    //赋值给返回值
                    break;
                }
            }
            else {                                                                 //预先准备的流资源数量为20，超过20执行下面的操作。
                /*
                * 将_piob[i]设置为新分配的_FILEX资源, 并返回指向它的指针.
                * 下面的操作进入windows底层。C标准规定同时打开文件的数量至少为8，
                * 如果打开超过20就要依靠window底层API调用，那么就没有可移植性了。
                */
                if ( (__piob[i] = _malloc_crt( sizeof(_FILEX) )) != NULL ) {
 
                    if ( !__crtInitCritSecAndSpinCount(
                        &(((_FILEX *)__piob[i])->lock), _CRT_SPINCOUNT ))
                    {
                        /*
                        * Failed to initialize the critical section because
                        * of lack of memory, clean up and return failure.
                        */
                        _free_crt( __piob[i] );
                        __piob[i] = NULL;
                        break;
                    }
 
                    EnterCriticalSection( &(((_FILEX *)__piob[i])->lock) );
                    retval = (FILE *)__piob[i];
                    retval->_flag = 0;
                }
 
                break;
            }
        }
 
        /*
        * 配置这个返回流的信息。.
        */
        if ( retval != NULL ) {
            /* 确保 _IOLOCKED 是预制的并且其他状态位为0 */
            retval->_flag &= _IOLOCKED;
            retval->_cnt = 0;
            retval->_tmpfname = retval->_ptr = retval->_base = NULL;
            retval->_file = -1;                                                        //文件描述符，此流未与任何的文件建立联系。
        }
 
    }
    __finally {
        _munlock(_IOB_SCAN_LOCK);                                                      //<mtdll.h> void _unlock(int); 释放线程锁 _IOB_SCAN_LOCK锁ID
    }
 
    return(retval);                                                                    //返回流资源的地址或NULL。
}
```

简单理解 从下标为0开始循环遍历__piob数组，如果该下标下的指针不为空表示已分配的，就检查其是不是在使用，是不是锁定状态，


```cpp
#define inuse(s)        ((s)->_flag & (_IOREAD|_IOWRT|_IORW))   
#define str_locked(s)   ((s)->_flag & (_IOLOCKED))
```


如果是继续循环，如果不是则



如果下标大2小于20 （_IOB_ENTRIES被定义为20，下标为0、1、2默认为std:cin   std::cou   std::cerro 保留   2-20 为STD标准库保留   大于20的给用户使用），初始化

_locktable (参见上文_mtinitlocks分析)中相应的临界区对象。

然后  _lock_str2(i, __piob[i]);      为该流对象加锁并 赋给 retval返回



如果__piob数组该下标指针为空，堆中分配一个_FILEX的空间，并初始化他的临界区对象，并加锁，然后赋给retval并返回









紧接着假定是在非UNICODE模式 ，则调用_openfile(file,mode,shflag,stream)，将获取到的加锁的流做为第四个实参



__topenfile  位于_open.c文件中





```cpp
#define __topenfile    _openfile
FILE * __cdecl __topenfile (
        const _TSCHAR *filename,
        REG3 const _TSCHAR *mode,
        int shflag,
        FILE *str
        )
{
        REG2 int modeflag;
        int streamflag = _commode;
        int commodeset = 0;
        int scanset    = 0;
        int whileflag;
        int filedes;
        REG1 FILE *stream;
        BOOL encodingFlag = FALSE;
 
        _ASSERTE(filename != NULL);
        _ASSERTE(mode != NULL);
        _ASSERTE(str != NULL);
 
        /* Parse the user's specification string as set flags in
               (1) modeflag - system call flags word
               (2) streamflag - stream handle flags word. */
 
        /* Skip leading spaces */
        while (*mode == _T(' '))
        {
            ++mode;
        }
 
        /* First mode character must be 'r', 'w', or 'a'. */
 
        switch (*mode) {
        case _T('r'):
                modeflag = _O_RDONLY;
                streamflag |= _IOREAD;
                break;
        case _T('w'):
                modeflag = _O_WRONLY | _O_CREAT | _O_TRUNC;
                streamflag |= _IOWRT;
                break;
        case _T('a'):
                modeflag = _O_WRONLY | _O_CREAT | _O_APPEND;
                streamflag |= _IOWRT;
                break;
        default:
                _VALIDATE_RETURN(("Invalid file open mode",0), EINVAL, NULL);
        }
                                                                                //根据fopen传过来的打开模式，设置 流的标志和文件标识
        /* There can be up to three more optional mode characters:
           (1) A single '+' character,
           (2) One of 't' and 'b' and
           (3) One of 'c' and 'n'.
        */
 
        whileflag=1;
 
        while(*++mode && whileflag)
                switch(*mode) {
 
                case _T(' '):
                    /* skip spaces */
                    break;
 
                case _T('+'):
                        if (modeflag & _O_RDWR)
                                whileflag=0;
                        else {
                                modeflag |= _O_RDWR;
                                modeflag &= ~(_O_RDONLY | _O_WRONLY);
                                streamflag |= _IORW;
                                streamflag &= ~(_IOREAD | _IOWRT);
                        }
                        break;
 
                case _T('b'):
                        if (modeflag & (_O_TEXT | _O_BINARY))
                                whileflag=0;
                        else
                                modeflag |= _O_BINARY;
                        break;
 
                case _T('t'):
                        if (modeflag & (_O_TEXT | _O_BINARY))
                                whileflag=0;
                        else
                                modeflag |= _O_TEXT;
                        break;
 
                case _T('c'):
                        if (commodeset)
                                whileflag=0;
                        else {
                                commodeset = 1;
                                streamflag |= _IOCOMMIT;
                        }
                        break;
 
                case _T('n'):
                        if (commodeset)
                                whileflag=0;
                        else {
                                commodeset = 1;
                                streamflag &= ~_IOCOMMIT;
                        }
                        break;
 
                case _T('S'):
                        if (scanset)
                                whileflag=0;
                        else {
                                scanset = 1;
                                modeflag |= _O_SEQUENTIAL;
                        }
                        break;
 
                case _T('R'):
                        if (scanset)
                                whileflag=0;
                        else {
                                scanset = 1;
                                modeflag |= _O_RANDOM;
                        }
                        break;
 
                case _T('T'):
                        if (modeflag & _O_SHORT_LIVED)
                                whileflag=0;
                        else
                                modeflag |= _O_SHORT_LIVED;
                        break;
 
                case _T('D'):
                        if (modeflag & _O_TEMPORARY)
                                whileflag=0;
                        else
                                modeflag |= _O_TEMPORARY;
                        break;
                case _T('N'):
                        modeflag |= _O_NOINHERIT;
                        break;
 
                case _T(','):
                        encodingFlag = TRUE;      //如果有，则可以能为其他编码模式
                        whileflag = 0;
                        break;
 
 
                default:
                        _VALIDATE_RETURN(("Invalid file open mode",0), EINVAL, NULL);
                }                                                                          //处理组合模式
        if (encodingFlag)
        {
            static const _TSCHAR ccsField[] = _T("ccs");
            static const _TSCHAR utf8encoding[] = _T("UTF-8");
            static const _TSCHAR utf16encoding[] = _T("UTF-16LE");
            static const _TSCHAR unicodeencoding[] = _T("UNICODE");
 
            /* Skip spaces */
            while (*mode == _T(' '))
            {
                ++mode;
            }
 
            /*
             * The length that we want to compare is numbers of elements in
             * csField -1 since this number also contains NULL terminator
             */
            if (_tcsncmp(ccsField, mode, (_countof(ccsField))-1) != 0)
                _VALIDATE_RETURN(("Invalid file open mode",0), EINVAL, NULL);
 
            mode += _countof(ccsField)-1;
 
            /* Skip spaces */
            while (*mode == _T(' '))
            {
                ++mode;
            }
 
            /* Look for '=' */
            if (*mode != _T('='))
            {
                _VALIDATE_RETURN(("Invalid file open mode",0), EINVAL, NULL);
            }
            ++mode;
 
            /* Skip spaces */
            while (*mode == _T(' '))
            {
                ++mode;
            }
 
            if (_tcsnicmp(mode, utf8encoding, _countof(utf8encoding) - 1) == 0){
                mode += _countof(utf8encoding)-1;
                modeflag |= _O_U8TEXT;
            }
            else if (_tcsnicmp(mode, utf16encoding, _countof(utf16encoding) - 1) == 0) {
                mode += _countof(utf16encoding)-1;
                modeflag |= _O_U16TEXT;
            }
            else if (_tcsnicmp(mode, unicodeencoding, _countof(unicodeencoding) - 1) == 0) {
                mode += _countof(unicodeencoding)-1;
                modeflag |= _O_WTEXT;
            }
            else
                _VALIDATE_RETURN(("Invalid file open mode",0), EINVAL, NULL);
 
        }                                 //处理其他编码模式
 
        /* Skip trailing spaces */
        while (*mode == _T(' '))
        {
            ++mode;
        }
 
        _VALIDATE_RETURN( (*mode == _T('\0')), EINVAL, NULL);
 
        /* Try to open the file.  Note that if neither 't' nor 'b' is
           specified, _sopen will use the default. */
 
        if (_tsopen_s(&filedes, filename, modeflag, shflag, _S_IREAD | _S_IWRITE) != 0)
                return(NULL);                                        调用_tsopen_s打开文件
 
        /* Set up the stream data base. */
#ifndef CRTDLL
        _cflush++;  /* force library pre-termination procedure */
#endif  /* CRTDLL */
        /* Init pointers */
        stream = str;
 
        stream->_flag = streamflag;
        stream->_cnt = 0;
        stream->_tmpfname = stream->_base = stream->_ptr = NULL;
 
        stream->_file = filedes;    //设置传进来的流指针参数，并返回
 
        return(stream);
}
```

这个函数比较长，但主要工作是处理fopen传进来的字符串类型的打开模式，解析字符串，设置流模式和 文件模式，最终调用_tsopen_s 去打来文件，然后用传进来的流指针设置模式并作为返回值





接着往下挖 _tsopen_s 位于 open.c文件中





```cpp
errno_t __cdecl _tsopen_s (
        int * pfh,
        const _TSCHAR *path,
        int oflag,
        int shflag,
        int pmode
        )
{
    /* Last parameter passed as 1 because we want to validate
     * pmode from the secure open_s */
    return _tsopen_helper(path, oflag, shflag, pmode, pfh, 1);
}
```



又是一个中转，将参数完封不动的传给了_tsopen_helper    但是参数顺序发生了变化，多了最后一个参数1 先不管它







```cpp
errno_t __cdecl _tsopen_helper (
        const _TSCHAR *path,
        int oflag,
        int shflag,
        int pmode,
        int * pfh,
        int bSecure
        )
{
        errno_t retval;
        int unlock_flag = 0;
 
        _VALIDATE_RETURN_ERRCODE( (pfh != NULL), EINVAL);
        *pfh = -1;
        _VALIDATE_RETURN_ERRCODE( (path != NULL), EINVAL);
 
        if(bSecure)
            _VALIDATE_RETURN_ERRCODE(((pmode & (~(_S_IREAD | _S_IWRITE))) == 0), EINVAL);  检查是不是之前传_tsopen_s进来的_S_IREAD | _S_IWRITE模式
 
 
        __try {
            retval = _tsopen_nolock( &unlock_flag,
                                 pfh,
                                 path,
                                 oflag,
                                 shflag,
                                 pmode,
                                 bSecure );
        }
        __finally {
            if ( unlock_flag )
            {
                if (retval)
                {
                    _osfile(*pfh) &= ~FOPEN;
                }
                _unlock_fh(*pfh);
            }
        }
 
        /* in error case, ensure *pfh is -1 */
        if (retval != 0)
        {
            *pfh = -1;
        }
 
        return retval;
}
```

这里依然是一个中转 建立了也该SEH异常结构 ，并调用_tsopen_nolock  又增加了一个参数int unlock_flag = 0;





_tsopen_nolock 这个函数比较长分割开来看比较合适





```cpp
static errno_t __cdecl _tsopen_nolock (
        int *punlock_flag,
        int *pfh,
        const _TSCHAR *path,
        int oflag,
        int shflag,
        int pmode,
        int bSecure
        )
{
        int filepos;                    /* length of file - 1 */
        _TSCHAR ch;                     /* character at end of file */
        char fileflags;                 /* _osfile flags */
        int fmode = 0;
 
        HANDLE osfh;                    /* OS handle of opened file */
        DWORD fileaccess;               /* OS file access (requested) */
        DWORD fileshare;                /* OS file sharing mode */
        DWORD filecreate;               /* OS method of opening/creating */
        DWORD fileattrib;               /* OS file attribute flags */
        DWORD isdev;                    /* device indicator in low byte */
        SECURITY_ATTRIBUTES SecurityAttributes;
        char tmode = __IOINFO_TM_ANSI;  /* textmode - ANSI/UTF-8/UTF-16 */
        errno_t retvalue = 0;
 
        SecurityAttributes.nLength = sizeof( SecurityAttributes );
        SecurityAttributes.lpSecurityDescriptor = NULL;
 
        if (oflag & _O_NOINHERIT) {
            SecurityAttributes.bInheritHandle = FALSE;
            fileflags = FNOINHERIT;
        }
        else {
            SecurityAttributes.bInheritHandle = TRUE;
            fileflags = 0;
        }
```






```cpp
        HANDLE osfh;                    /* OS handle of opened file */
        DWORD fileaccess;               /* OS file access (requested) */
        DWORD fileshare;                /* OS file sharing mode */
        DWORD filecreate;               /* OS method of opening/creating */
        DWORD fileattrib;               /* OS file attribute flags */
        DWORD isdev;                    /* device indicator in low byte */
```


这几个分别对应了  Windows API   函数的 CreateFile的参数   ，已经可以猜到这个函数最终要调用CreateFile系统函数来完成打开文件的操作，呵呵

定义了一个 SECURITY_ATTRIBUTES 结构体  ，并根据传进来的ofalg  设置是不是运行句柄继承





```cpp
    _ERRCHECK(_get_fmode(&fmode));
 
        /* figure out binary/text mode */
        if ((oflag & _O_BINARY) == 0)
            if (oflag & (_O_TEXT | _O_WTEXT | _O_U16TEXT | _O_U8TEXT))
                fileflags |= FTEXT;
            else if (fmode != _O_BINARY)   /* check default mode */
                fileflags |= FTEXT;
 
        /*
         * decode the access flags
         */
        switch( oflag & (_O_RDONLY | _O_WRONLY | _O_RDWR) ) {
 
            case _O_RDONLY:         /* read access */
                    fileaccess = GENERIC_READ;
                    break;
            case _O_WRONLY:         /* write access */
                    /* giving it read access as well
                     * because in append (a, not a+), we need
                     * to read the BOM to determine the encoding
                     * (ie. ANSI, UTF8, UTF16)
                     */
                    if ((oflag & _O_APPEND)
                            && (oflag & (_O_WTEXT | _O_U16TEXT | _O_U8TEXT)) != 0)
                    {
                        fileaccess = GENERIC_READ | GENERIC_WRITE;
                    }
                    else
                    {
                        fileaccess = GENERIC_WRITE;
                    }
                    break;
            case _O_RDWR:           /* read and write access */
                    fileaccess = GENERIC_READ | GENERIC_WRITE;
                    break;
            default:                /* error, bad oflag */
                    _doserrno = 0L; /* not an OS error */
                    *pfh = -1;
                    _VALIDATE_RETURN_ERRCODE(( "Invalid open flag" , 0 ), EINVAL);
 
        }
```

熟悉的GENERIC_READ | GENERIC_WRITE有木有，根据oflag设置File的访问模式







```cpp
  /*
         * decode sharing flags
         */
        switch ( shflag ) {
 
            case _SH_DENYRW:        /* exclusive access */
                fileshare = 0L;
                break;
 
            case _SH_DENYWR:        /* share read access */
                fileshare = FILE_SHARE_READ;
                break;
 
            case _SH_DENYRD:        /* share write access */
                fileshare = FILE_SHARE_WRITE;
                break;
 
            case _SH_DENYNO:        /* share read and write access */
                fileshare = FILE_SHARE_READ | FILE_SHARE_WRITE;
                break;
 
            case _SH_SECURE:       /* share read access only if read-only */
                if (fileaccess == GENERIC_READ)
                    fileshare = FILE_SHARE_READ;
                else
                    fileshare = 0L;
                break;
 
            default:                /* error, bad shflag */
                _doserrno = 0L; /* not an OS error */
                *pfh = -1;
                _VALIDATE_RETURN_ERRCODE(( "Invalid sharing flag" , 0 ), EINVAL);
        }
```

根据shflag设置文件共享模式





```cpp
    switch ( oflag & (_O_CREAT | _O_EXCL | _O_TRUNC) ) {
            case 0:
            case _O_EXCL:                   // ignore EXCL w/o CREAT
                filecreate = OPEN_EXISTING;
                break;
 
            case _O_CREAT:
                filecreate = OPEN_ALWAYS;
                break;
 
            case _O_CREAT | _O_EXCL:
            case _O_CREAT | _O_TRUNC | _O_EXCL:
                filecreate = CREATE_NEW;
                break;
 
            case _O_TRUNC:
            case _O_TRUNC | _O_EXCL:        // ignore EXCL w/o CREAT
                filecreate = TRUNCATE_EXISTING;
                break;
 
            case _O_CREAT | _O_TRUNC:
                filecreate = CREATE_ALWAYS;
                break;
 
            default:
                // this can't happen ... all cases are covered
                _doserrno = 0L;
                *pfh = -1;
                _VALIDATE_RETURN_ERRCODE(( "Invalid open flag" , 0 ), EINVAL);
        }
```

获取文件的创建模式







```cpp
     /*
         * decode file attribute flags if _O_CREAT was specified
         */
        fileattrib = FILE_ATTRIBUTE_NORMAL;     /* default */
 
        if ( oflag & _O_CREAT ) {
 
            if ( !((pmode & ~_umaskval) & _S_IWRITE) )
                fileattrib = FILE_ATTRIBUTE_READONLY;
        }
 
        /*
         * Set temporary file (delete-on-close) attribute if requested.
         */
        if ( oflag & _O_TEMPORARY ) {
            fileattrib |= FILE_FLAG_DELETE_ON_CLOSE;
            fileaccess |= DELETE;
            fileshare |= FILE_SHARE_DELETE;
        }
 
        /*
         * Set temporary file (delay-flush-to-disk) attribute if requested.
         */
        if ( oflag & _O_SHORT_LIVED )
            fileattrib |= FILE_ATTRIBUTE_TEMPORARY;
 
        /*
         * Set sequential or random access attribute if requested.
         */
        if ( oflag & _O_SEQUENTIAL )
            fileattrib |= FILE_FLAG_SEQUENTIAL_SCAN;
        else if ( oflag & _O_RANDOM )
            fileattrib |= FILE_FLAG_RANDOM_ACCESS;
```



获取要创建的文件的属性







```cpp
     /*
         * get an available handle.
         *
         * multi-thread note: the returned handle is locked!
         */
        if ( (*pfh = _alloc_osfhnd()) == -1 ) {
            _doserrno = 0L;         /* not an OS error */
            *pfh = -1;
            errno = EMFILE;
            return errno;          /* return error to caller */
        }
 
        /* Beyond this do not set *pfh = -1 on errors for MT.
            Because the caller needs to release the lock on the
            handle */
 
        *punlock_flag = 1;
 
        /*
         * try to open/create the file
         */
        if ( (osfh = CreateFile( (LPTSTR)path,
                                 fileaccess,
                                 fileshare,
                                 &SecurityAttributes,
                                 filecreate,
                                 fileattrib,
                                 NULL ))
             == (HANDLE)(-1) )
        {
            if ((fileaccess & (GENERIC_READ | GENERIC_WRITE)) == (GENERIC_READ | GENERIC_WRITE) &&
                    (oflag & _O_WRONLY))
            {
                /*
                 * We just failed on CreateFile(), because we might be trying
                 * open something for read while it cannot be read (eg. pipes or devices).
                 * So try again with GENERIC_WRITE and we will have to use the default
                 * encoding.  We won't be able to determine the encoding from reading
                 * the BOM.
                 */
                fileaccess &= ~GENERIC_READ;
                if ( (osfh = CreateFile( (LPTSTR)path,
                                         fileaccess,
                                         fileshare,
                                         &SecurityAttributes,
                                         filecreate,
                                         fileattrib,
                                         NULL ))
                     == (HANDLE)(-1) )
                {
                    /*
                     * OS call to open/create file failed! map the error, release
                     * the lock, and return -1. note that it's not necessary to
                     * call _free_osfhnd (it hasn't been used yet), but we do need
                     * to clear the FOPEN that was set by _alloc_osfhnd.
                     */
                    _osfile(*pfh) &= ~FOPEN;
                    _dosmaperr(GetLastError());
                    retvalue = errno;
                    goto exit;
                }
            }
            else
            {
                /*
                 * OS call to open/create file failed! map the error, release
                 * the lock, and return -1. note that it's not necessary to
                 * call _free_osfhnd (it hasn't been used yet), but we do need
                 * to clear the FOPEN that was set by _alloc_osfhnd.
                 */
                _osfile(*pfh) &= ~FOPEN;
                _dosmaperr(GetLastError());
                retvalue = errno;
                goto exit;
            }
        }
```

 首先来看  if ( (*pfh = _alloc_osfhnd()) == -1 )   phf是在_topenfile中被传进来的一个名为 filedes的int型变量的指针，在_topenfile函数结尾



```cpp
 stream->_file = filedes;    //设置传进来的流指针参数，并返回
```



因此 FILE* f = fopen( "test.dat", "wb" );  f->_file   最终保存了这个_alloc_osfhnd() 返回的值，那么这个值到底起什么作用呢，来看看_alloc_osfhnd()都做了些什么







先来介绍两个全局变量 

1、_CRTIMP ioinfo * __pioinfo[IOINFO_ARRAYS];      ioinfo结构的指针数组，实际大小为64*32，它的实际内存模型是一个二维指针数组，每列32个指针，共64行,但是在内存分布上并不是连续的，事实上 __pioinfo是64个在内存中连续存放的指针，每个指针指向的内存块是由32个ioinfo结构体的大小组成，因此每次的分配粒度必须是32*sizeof(ioinfo)

ioinfo  结构被定义为

```cpp
typedef struct {
        intptr_t osfhnd;    /* underlying OS file HANDLE */      用来存放对应的WINDOWS API返回的句柄值
        char osfile;        /* attributes of file (e.g., open in text mode?) */  属性值
        char pipech;        /* one char buffer for handles opened on pipes */    一个字节，管道操作使用
        int lockinitflag;                                                         //标识是否为加锁状态
        CRITICAL_SECTION lock;                                                   //临界区对象，加锁用
#ifndef _SAFECRT_IMPL
        /* Not used in the safecrt downlevel. We do not define them, so we cannot use them accidentally */
        char textmode : 7;     /* __IOINFO_TM_ANSI or __IOINFO_TM_UTF8 or __IOINFO_TM_UTF16LE */
        char unicode : 1;      /* Was the file opened as unicode? */
        char pipech2[2];       /* 2 more peak ahead chars for UNICODE mode */
        __int64 startpos;      /* File position that matches buffer start */
        BOOL utf8translations; /* Buffer contains translations other than CRLF*/
        char dbcsBuffer;       /* Buffer for the lead byte of dbcs when converting from dbcs to unicode */
        BOOL dbcsBufferUsed;   /* Bool for the lead byte buffer is used or not */
#endif  /* _SAFECRT_IMPL */
    }   ioinfo;
```



2、int _nhandle;     标识 __pioinfo中已被分配内存空间的ioinfo结构的数目







```cpp
int __cdecl _alloc_osfhnd(
        void
        )
{
        int fh = -1;    /* file handle */
        int i;
        ioinfo *pio;
        int failed=FALSE;
 
        if (!_mtinitlocknum(_OSFHND_LOCK))      //如果_locktable[_OSFHND_LOCK] 没有初始化则进行初始化
            return -1;
 
        _mlock(_OSFHND_LOCK);   /* lock the __pioinfo[] array */   _locktable[_OSFHND_LOCK]用这个临界区对象防止线程重入，它被专门用来对__pioinfo[]访问时的
 线程同步
 
         __TRY
 
            for ( i = 0 ; i < IOINFO_ARRAYS ; i++ ) {           遍历这个二维数组
 
                if ( __pioinfo[i] != NULL ) {                    如果第一维不为空
 
                    for ( pio = __pioinfo[i] ;
                          pio < __pioinfo[i] + IOINFO_ARRAY_ELTS ; //IOINFO_ARRAY_ELTS =32      遍历第二维
                          pio++ )
                    {
                        if ( (pio->osfile & FOPEN) == 0 ) {                //如果该指针指向的ioinfo的标志是还未打开的,表示它处在空闲状态
                            if ( pio->lockinitflag == 0 ) {
                                _mlock( _LOCKTAB_LOCK );                    //如果没有初始化它的临界区对象
                                __TRY
                                    if ( pio->lockinitflag == 0 ) {
                                        if ( !InitializeCriticalSectionAndSpinCount( &(pio->lock), _CRT_SPINCOUNT ))
                                        {
     
                                            failed=TRUE;
                                        }
                                        else
                                        {
                                            pio->lockinitflag++;
                                        }
                                    }
                                __FINALLY                              //为该ioinfo结构初始化临界区对象
                                    _munlock( _LOCKTAB_LOCK );
                                __END_TRY_FINALLY
                            }
 
                            if(!failed) //如果找到空闲的已分配的ioinfo
                            {
                                EnterCriticalSection( &(pio->lock) );       //为该ioinfo加锁
 
 
                                if ( (pio->osfile & FOPEN) != 0 ) {         //如果这时其他线程占用了该ioinfo解锁
                                        LeaveCriticalSection( &(pio->lock) );
                                        continue;
                                }
                            }
 
                            if(!failed)
                            {
                                pio->osfile = FOPEN;    //给找到的空闲的ioinfo设置成已打开
                                pio->osfhnd = (intptr_t)INVALID_HANDLE_VALUE;   //句柄设置为无效句柄
                                fh = i * IOINFO_ARRAY_ELTS + (int)(pio - __pioinfo[i]); //fh 的低5位 设置成列号 5位之前表示行号
                                break;  跳出第二次循环
                            }
                        }
                    }
 
                    if ( fh != -1 )
                        break;        //如果fh不为-1  表示找到了空间info结构并进行了设置  不在循环
                }
                else {                                   
 
                    if ( (pio = _calloc_crt( IOINFO_ARRAY_ELTS, sizeof(ioinfo) ))        申请一行
                        != NULL )
                    {
                        __pioinfo[i] = pio;               //把改行加入二维数组
                        _nhandle += IOINFO_ARRAY_ELTS;
 
                        for ( ; pio < __pioinfo[i] + IOINFO_ARRAY_ELTS ; pio++ ) {
                            pio->osfile = 0;
                            pio->osfhnd = (intptr_t)INVALID_HANDLE_VALUE;
                            pio->pipech = 10;
                            pio->lockinitflag = 0;                      //为该行每列初始化
                        }
 
                        fh = i * IOINFO_ARRAY_ELTS;        //如果没找到空闲的则分配增加一个行 ，并把行地址给fn
                        _osfile(fh) = FOPEN;
                        if ( !__lock_fhandle( fh ) ) {
 
                            fh = -1;
                        }
                    }
 
                    break;
                }
            }
        __FINALLY
            _munlock(_OSFHND_LOCK); /* unlock the __pioinfo[] table */        解锁 释放对__pioinfo 的控制权给其他线程
        __END_TRY_FINALLY
 
        return( fh );
}
```

不难看出运行库 维护着一个全局的二维指针数组__pioinfo，用来存放info结构体，该结构体可以用来存已经打开的Window内核对象句柄，及访问属性，并且配有一个临界区对象用来做线程的同步，因此可以猜想到  fopen最终调用CreateFile  并把返回的HANDLE存放在_alloc_osfhnd  申请到ioinfo结构中，_alloc_osfhnd返回一个int类型的值，该值得低5位是__pioinfo二维数组的列号 5位以上标识行号，因此我们可以用返回的这个int值 来索引到ioinfo结构指针在该数组中的位置



在internal.h中有用来索引的宏定义



```cpp
#define IOINFO_L2E          5
 
/*
 * Definition of IOINFO_ARRAY_ELTS, the number of elements in ioinfo array
 */
#define IOINFO_ARRAY_ELTS   (1 << IOINFO_L2E)
 
#define _pioinfo(i) ( __pioinfo[(i) >> IOINFO_L2E] + ((i) & (IOINFO_ARRAY_ELTS - \
                              1)) )
#define _osfhnd(i)  ( _pioinfo(i)->osfhnd )
 
#define _osfile(i)  ( _pioinfo(i)->osfile )
 
#define _pipech(i)  ( _pioinfo(i)->pipech )
 
.......................
```







回到fopen的分析 



```cpp
if ( (*pfh = _alloc_osfhnd()) == -1 ) 
*pfh  保存了返回的这个索引值，接着CreateFile  将返回的句柄保存到了局部变量osfh中              

  if ( (osfh = CreateFile( (LPTSTR)path,
                                         fileaccess,
                                         fileshare,
                                         &SecurityAttributes,
                                         filecreate,
                                         fileattrib,
                                         NULL ))
                     == (HANDLE)(-1) )
```



那么它是在哪里通过索引找到申请的ioinfo结构,并把句柄放在ioinflo中去的呢，因为 _tsopen_nolock函数在CreateFile之后要有很长的代码用来设置文件指针和一些标志，这里不一一贴出了，在CreateFile后 不处使用了一个函数，完成了上面所说的设置





```cpp
        /*
         * the file is open. now, set the info in _osfhnd array
         */
        _set_osfhnd(*pfh, (intptr_t)osfh); //将句柄和索引值作为参数
```





```cpp
int __cdecl _set_osfhnd (
        int fh,
        intptr_t value
        )
{
        if ( fh >= 0 && ((unsigned)fh < (unsigned)_nhandle) &&
             (_osfhnd(fh) == (intptr_t)INVALID_HANDLE_VALUE)   //如果fn有效 即大于0小于2048 ，且索引到的ioinfo结构的osfhnd为无效句柄,
           ) {                                             //_nhandle =64*32  __pioinfo的最大容量
            if ( __app_type == _CONSOLE_APP ) {  ///如果是控制台程序
                switch (fh) {
                case 0:
                    SetStdHandle( STD_INPUT_HANDLE, (HANDLE)value );
                    break;
                case 1:
                    SetStdHandle( STD_OUTPUT_HANDLE, (HANDLE)value );
                    break;
                case 2:
                    SetStdHandle( STD_ERROR_HANDLE, (HANDLE)value );
                    break;
                }
            }// 0、1、2对应 cin cout cerro
 
            _osfhnd(fh) = value;   //将HANDLE保存到对应的ioinfo中去
            return(0);
        } else {
            errno = EBADF;      /* bad handle */
            _doserrno = 0L;     /* not an OS error */
            return -1;
        }
```







总结

但从fopen流程的分析可以窥见 VS这套对C标准库IO的实现，其实就是对系统API的封装，主要是通过两个全局结构体指针数组完成的。

1、__pioinfo   ioinfo类型的指针数组

ioinfo  结构被定义为

```cpp
typedef struct {
        intptr_t osfhnd;    /* underlying OS file HANDLE */      用来存放对应的WINDOWS API返回的句柄值
        char osfile;        /* attributes of file (e.g., open in text mode?) */  属性值
        char pipech;        /* one char buffer for handles opened on pipes */    一个字节，管道操作使用
        int lockinitflag;                                                         //标识是否为加锁状态
        CRITICAL_SECTION lock;                                                   //临界区对象，加锁用
#ifndef _SAFECRT_IMPL
        /* Not used in the safecrt downlevel. We do not define them, so we cannot use them accidentally */
        char textmode : 7;     /* __IOINFO_TM_ANSI or __IOINFO_TM_UTF8 or __IOINFO_TM_UTF16LE */
        char unicode : 1;      /* Was the file opened as unicode? */
        char pipech2[2];       /* 2 more peak ahead chars for UNICODE mode */
        __int64 startpos;      /* File position that matches buffer start */
        BOOL utf8translations; /* Buffer contains translations other than CRLF*/
        char dbcsBuffer;       /* Buffer for the lead byte of dbcs when converting from dbcs to unicode */
        BOOL dbcsBufferUsed;   /* Bool for the lead byte buffer is used or not */
#endif  /* _SAFECRT_IMPL */
    }   ioinfo;
```



2、__piob   _FILEX类型指针数组

```cpp
struct {
        FILE f;
        CRITICAL_SECTION lock;
        }   _FILEX;

struct _iobuf {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;       //
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
        };
typedef struct _iobuf FILE;
```



我们用fopen获取到的FILE是保存在__piob中的，FILE中的_file是一个索引值标识了 ioinfo在__pioinfo中的位置，ioinfo中保存了最终的API返回的句柄

这两个全局数组中的指针是可以复用的，在申请一个指针的时候，先遍历已存在的有效指针指向的结构是不是为已使用的，如果发现不在使用中（空闲）的就返回该空闲指针，只有当所有已分配的指针都处于使用中的时候，才在堆中构建结构，把构建好的结构的指针放入数组中去。因此可以推定在调用fclose时并不释放已经申请的FILE和ioinfo，指针将他们的使用状态改为空闲。我们还可以推定fwrite和fread时通过FILE中的_file做索引，找到ioinfo在数组中的指针，并获取保证在里面的HANDLE然后调用WriteFile或ReadFile



这一对系统API的保证看起来是如此的复杂



最终回到正题吧 启动代码中的_ioinit函数，它位于Ioinit,c文件中



```cpp
/***
*_ioinit() -
*
*Purpose:
*       Allocates and initializes initial array(s) of ioinfo structs. Then,
        申请和初始化ioinfo结构体类型指针的数组，即__pioinfo
*       obtains and processes information on inherited file handles from the
*       parent process (e.g., cmd.exe).
*       获取和处理从父进程继承来的文件句柄
*       Obtains the StartupInfo structure from the OS. The inherited file
*       handle information is pointed to by the lpReserved2 field. The format
*       of the information is as follows:
         在StartupInfo结构中，继承来的文件句柄信息由这个结构的lpReserved2标识，这块被lpReserved2指向的内存结构如下
*
*           bytes 0 thru 3          - integer value, say N, which is the
*                                     number of handles information is passed     第0 -- 3个字节标识了有N个句柄被继承
*                                     about
*
*           bytes 4 thru N+3        - the N values for osfile                      第4 -- 3+N 个字节标识了文件句柄的属性 对应ioinfo结构的osfile
*
*           bytes N+4 thru 5*N+3    - N double-words, the N OS HANDLE values       第4+N -- 5*N+3个字节存放了 继承来的句柄值
*                                     being passed
*
*       Next, osfhnd and osfile for the first three ioinfo structs,
*       corrsponding to handles 0, 1 and 2, are initialized as follows:
*       //__pioinfo数组的前三个ioinfo结构体，即下标为0、1、2的ioinfo，它们的 osfhnd和osfile 按照如下的方式进行初始化
*           If the value in osfhnd is INVALID_HANDLE_VALUE, then try to
*           obtain a HANDLE by calling GetStdHandle, and call GetFileType to
*           help set osfile. Otherwise, assume _osfhndl and _osfile are
*           valid, but force it to text mode (standard input/output/error
*           are to always start out in text mode).
*           //如果osfhnd 的值是INVALID_HANDLE_VALUE ，那么用GetStdHandle来获取一个HANDLE值，然后用GetFileType去获取HANDLE的类型，设置osfile,
            如果 osfhndl和osfile是有效的，说明它们是继承来的，设置text模式
*       Notes:
*           1. In general, not all of the passed info from the parent process
*              will describe open handles! If, for example, only C handle 1
*              (STDOUT) and C handle 6 are open in the parent, info for C
*              handles 0 thru 6 is passed to the the child.
*              //一般来说 并不是所有从父进程继承来的句柄都是打开的，例如 仅仅是第一个和第六个句柄是打开的在它们的服进程中，那么0-6都会被传到子进程中
*           2. Care is taken not to 'overflow' the arrays of ioinfo structs.
*              注意不在对这个ioinfo结构体数组访问越界
*           3. See exec\dospawn.c for the encoding of the file handle info
*              to be passed to a child process.
*             可以参照dospawn,c文件中在传给子进程句柄的时候是如何进程编码的。
*Entry:
*       No parameters: reads the STARTUPINFO structure.
*
*Exit:
*       0 on success, -1 if error encountered
*
*Exceptions:
*
*******************************************************************************/
```



传过来的句柄信息在内存中这样分布的

![img](https://img-blog.csdn.net/20170103032738997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







```cpp
int __cdecl _ioinit (
        void
        )
{
        STARTUPINFOW StartupInfo;
        int cfi_len;
        int fh;
        int i;
        ioinfo *pio;
        char *posfile;
        UNALIGNED intptr_t *posfhnd;
        intptr_t stdfh;
        DWORD htype;
 
        GetStartupInfoW( &StartupInfo );
 
        /*
         * Allocate and initialize the first array of ioinfo structs. This
         * array is pointed to by __pioinfo[0]
         */
        if ( (pio = _calloc_crt( IOINFO_ARRAY_ELTS, sizeof(ioinfo) ))  申请32个ioinfo结构的空间
             == NULL )
        {
            return -1;
        }
 
        __pioinfo[0] = pio;                //  将申请到的空间做为二维数组的第一行
        _nhandle = IOINFO_ARRAY_ELTS;
 
        for ( ; pio < __pioinfo[0] + IOINFO_ARRAY_ELTS ; pio++ ) {
            pio->osfile = 0;
            pio->osfhnd = (intptr_t)INVALID_HANDLE_VALUE;
            pio->pipech = 10;                   /* linefeed/newline char */
            pio->lockinitflag = 0;              /* uninitialized lock */
            pio->textmode = 0;
            pio->unicode = 0;
            pio->pipech2[0] = 10;
            pio->pipech2[1] = 10;
            pio->dbcsBufferUsed = FALSE;
            pio->dbcsBuffer = '\0';
        }       //对该行的每一列的ioinfo初始化
 
        /*
         * Process inherited file handle information, if any
         */
 
        if ( (StartupInfo.cbReserved2 != 0) &&
             (StartupInfo.lpReserved2 != NULL) )              如果有从父进程穿过来句柄
        {
            /*
             * Get the number of handles inherited.
             */
            cfi_len = *(UNALIGNED int *)(StartupInfo.lpReserved2);  //lpReserved2指向的内存区的从前4个字节获取 传过来的句柄数
 
            /*
             * Set pointers to the start of the passed file info and OS
             * HANDLE values.
             */
            posfile = (char *)(StartupInfo.lpReserved2) + sizeof( int );  //posfile指向句柄属性数组，每个属性占一个字节
            posfhnd = (UNALIGNED intptr_t *)(posfile + cfi_len);           //posfhnd 指向句柄数组
 
            /*
             * Ensure cfi_len does not exceed the number of supported
             * handles!
             */
             cfi_len = __min( cfi_len, _NHANDLE_ ); //检测传过来的句柄数 是不是大于__pioinfo的最大容量，如果是用最大容量来防止越界
              for ( i = 1 ; _nhandle < cfi_len ; i++ ) {
 
 
                /*
                 * Allocate another array of ioinfo structs
                 */
                if ( (pio = _calloc_crt( IOINFO_ARRAY_ELTS, sizeof(ioinfo) ))
                    == NULL )
                {
                    /*
                     * No room for another array of ioinfo structs, reduce
                     * the number of inherited handles we process.
                     */
                    cfi_len = _nhandle;
                    break;
                }
 
 
                /*
                 * Update __pioinfo[] and _nhandle
                 */
                __pioinfo[i] = pio;
                _nhandle += IOINFO_ARRAY_ELTS;
 
 
                for ( ; pio < __pioinfo[i] + IOINFO_ARRAY_ELTS ; pio++ ) {
                    pio->osfile = 0;
                    pio->osfhnd = (intptr_t)INVALID_HANDLE_VALUE;
                    pio->pipech = 10;
                    pio->lockinitflag = 0;
                    pio->textmode = 0;
                    pio->pipech2[0] = 10;
                    pio->pipech2[1] = 10;
                    pio->dbcsBufferUsed = FALSE;
                    pio->dbcsBuffer = '\0';
                }
            }
 
 
            /*
             * Validate and copy the passed file information
             */
            for ( fh = 0 ; fh < cfi_len ; fh++, posfile++, posfhnd++ ) {
                /*
                 * Copy the passed file info iff it appears to describe
                 * an open, valid file or device.
                 *
                 * Note that GetFileType cannot be called for pipe handles
                 * since it may 'hang' if there is blocked read pending on
                 * the pipe in the parent.
                 */
                if ( (*posfhnd != (intptr_t)INVALID_HANDLE_VALUE) &&
                     (*posfhnd != _NO_CONSOLE_FILENO) &&
                     (*posfile & FOPEN) &&
                     ((*posfile & FPIPE) ||
                      (GetFileType( (HANDLE)*posfhnd ) != FILE_TYPE_UNKNOWN)) )
                {
                    pio = _pioinfo( fh );
                    pio->osfhnd = *posfhnd;
                    pio->osfile = *posfile;
                    /* Allocate the lock for this handle. */
                    if ( !InitializeCriticalSectionAndSpinCount( &pio->lock,
                                                                 _CRT_SPINCOUNT ))
                        return -1;
                    pio->lockinitflag++;
                }
            }
        }
 
 
        /*
         * If valid HANDLE-s for standard input, output and error were not
         * inherited, try to obtain them directly from the OS. Also, set the
         * appropriate bits in the osfile fields.
         */
        for ( fh = 0 ; fh < 3 ; fh++ ) {
 
 
            pio = __pioinfo[0] + fh;
 
 
            if ( (pio->osfhnd == (intptr_t)INVALID_HANDLE_VALUE) ||
                 (pio->osfhnd == _NO_CONSOLE_FILENO)) {
                /*
                 * mark the handle as open in text mode.
                 */
                pio->osfile = (char)(FOPEN | FTEXT);
 
 
                if ( ((stdfh = (intptr_t)GetStdHandle( stdhndl(fh) )) != (intptr_t)INVALID_HANDLE_VALUE) &&
                     (stdfh!=((intptr_t)NULL)) &&
                     ((htype = GetFileType( (HANDLE)stdfh )) != FILE_TYPE_UNKNOWN) )
                {
                    /*
                                        /*
                     * obtained a valid HANDLE from GetStdHandle
                     */
                    pio->osfhnd = stdfh;   将获取到的句柄保存
 
                    /*
                     * finish setting osfile: determine if it is a character
                     * device or pipe.
                     */
                    if ( (htype & 0xFF) == FILE_TYPE_CHAR )
                        pio->osfile |= FDEV;    类型为标准设备
                    else if ( (htype & 0xFF) == FILE_TYPE_PIPE )
                        pio->osfile |= FPIPE;   类型为管道
 
                    /* Allocate the lock for this handle. */
                    if ( !InitializeCriticalSectionAndSpinCount( &pio->lock,
                                                                 _CRT_SPINCOUNT ))    初始化临界区对象
                        return -1;
                    pio->lockinitflag++;
                }
                else {
      
                    pio->osfile |= FDEV;
                    pio->osfhnd = _NO_CONSOLE_FILENO;    //else则为非控制台程序
                }
            }
            else  {
                /*
                 * handle was passed to us by parent process. make
                 * sure it is text mode.
                 */
                pio->osfile |= FTEXT;     句柄已经由父进程传进来了
            }
        }
 
        /*
         * Set the number of supported HANDLE-s to _nhandle
         */
        (void)SetHandleCount( (unsigned)_nhandle );
 
        return 0;
}
```





到最后这里理解_ioinit  已经很容易了



1、为数组的第一行分配空间并初始化这个32个ioinfo结构

2、为了保存父进程传过来的句柄，分配足够的ioinfo结构，进行句柄和属性的复制

3、初始化标准输入输出的句柄及用来保存句柄的结构









# （4）_cinit 函数

2017年01月03日 22:32:21 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：457



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/53997347

接着上文 在初始化IO之后的代码是这样的



```cpp
        __try {
 
            if ( _ioinit() < 0 )            /* initialize lowio */
                _amsg_exit(_RT_LOWIOINIT);
 
            /* get wide cmd line info */
            _tcmdln = (_TSCHAR *)GetCommandLineT();
 
            /* get wide environ info */
            _tenvptr = (_TSCHAR *)GetEnvironmentStringsT();
 
            if ( _tsetargv() < 0 )
                _amsg_exit(_RT_SPACEARG);
            if ( _tsetenvp() < 0 )
                _amsg_exit(_RT_SPACEENV);
 
            initret = _cinit(TRUE);                  /* do C data initialize */
            if (initret != 0)
                _amsg_exit(initret);
 
#ifdef _WINMAIN_
 
            lpszCommandLine = _twincmdln();
            mainret = _tWinMain( (HINSTANCE)&__ImageBase,
                                 NULL,
                                 lpszCommandLine,
                                 StartupInfo.dwFlags & STARTF_USESHOWWINDOW
                                      ? StartupInfo.wShowWindow
                                      : SW_SHOWDEFAULT
                                );
#else  /* _WINMAIN_ */
            _tinitenv = _tenviron;
            mainret = _tmain(__argc, _targv, _tenviron);
#endif  /* _WINMAIN_ */
 
            if ( !managedapp )
                exit(mainret);
 
            _cexit();
 
        }
        __except ( _XcptFilter(GetExceptionCode(), GetExceptionInformation()) )
```

_tsetargv()  和_tsetenvp() 前者获取命令行参数的字符串，并解析分割对argc和argv进行初始化，后者解析获取到的环境变量字符串_tenvptr把结果存放到全局变量_tenviron 



这里就不贴代码了，因为都是些解析字符串的操作没什么可看的。





这里仅分析_setargv 中最为核心的代码



```cpp
_TSCHAR *p;
_TSCHAR *cmdstart;                  /* start of command line to parse */
int numargs, numchars;
 
//MAX_PATH 是 260 ，很有趣 ，文件的完整路径最大255 + "." + 后缀(比如exe) + " \0"
static _TSCHAR _pgmname[ MAX_PATH ];
 
 
/* 
 * __initmbctable 只能被调用一次，因此会设置一个__mbctype_initialized标记。
 * __initmbctable 内部会调用_setmbcp 函数去创建一个新的multibyte code page，
 * 随后置__mbctype_initialized=1
 */
if ( __mbctype_initialized == 0 )
        __initmbctable();
 
 
/* 将当前进程的exe完整路径复制到_pgmname数组中
 * 注意GetModuleFileName 是拿不到 程序启动参数args的，它获得仅仅是程序的完整路径而已
 */
GetModuleFileName( NULL, _pgmname, sizeof( _pgmname ) / sizeof(_TSCHAR));
_pgmptr = _pgmname;
 
/* 如果之前解析出来的_acmdln为空，则采用_pgmptr */
cmdstart = (*_acmdln == NULCHAR) ? _pgmptr : _acmdln;
 
/* 计算出 numargs 和 numchars 的大小  */
parse_cmdline(cmdstart, NULL, NULL, &numargs, &numchars);
 
/* 
 * 为argv 分配所需的空间 
 * 先是numargs 个指针，前numargs-1 指向路径与参数，最后一个是NULL
 * 紧接着是numchars 个字符，用来存放numargs-1 个指针所指的内容
 */
p = _malloc_crt(numargs * sizeof(_TSCHAR *) + numchars * sizeof(_TSCHAR));
if (p == NULL)
        _amsg_exit(_RT_SPACEARG);
 
/* 为指针P所指向的内存空间里填充argv  */
parse_cmdline(cmdstart, (char **)p, p + numargs * sizeof(char *), &numargs, &numchars);
 
/* 至此，argc 与  argv 已经全部现形 */
__argc = numargs - 1;
__argv = (char **)p; 
```





紧接着就是initret = _cinit(TRUE);   



cinit这个函数所完成的操作在所有VS生产的应用程序的启动代码中都有，无论是WIN32的  还是MFC的  或者是DLL类型  ，不管是使用动态运行库 还是静态运行库。

因为它主要工作是调用_initterm_e和_initterm 来完成C/C++中  全局变量和全局对象的初始化，调用全局对象的构造函数。



不妨先看看这两个函数的实现

首先看下_initterm



```cpp
typedef void (__cdecl *_PVFV)(void);
typedef int  (__cdecl *_PIFV)(void);
```





```cpp
#ifdef CRTDLL
void __cdecl _initterm (
#else  /* CRTDLL */
static void __cdecl _initterm (
#endif  /* CRTDLL */
        _PVFV * pfbegin,
        _PVFV * pfend
        )
{
        /*
         * walk the table of function pointers from the bottom up, until
         * the end is encountered.  Do not skip the first entry.  The initial
         * value of pfbegin points to the first valid entry.  Do not try to
         * execute what pfend points to.  Only entries before pfend are valid.
         */  沿着函数指针表从下到上，直到遇到结束标志，并不跳过表的第一项，pfbegin指向了第一个有效的表项，不要指向pfend指向的函数，只有在pfend之前的有效
        while ( pfbegin < pfend )
        {
            /*
             * if current table entry is non-NULL, call thru it.
             */
            if ( *pfbegin != NULL )     //如何改函数指针不为空 执行它
                (**pfbegin)();
            ++pfbegin;
        }
```

 _initterm_e  跟_initterm 差不多  只是它要执行的函数表中的每一个函数是有返回值得，如果返回值不为0就不再继续，并把该返回值 返回

```cpp
int __cdecl _initterm_e (
        _PIFV * pfbegin,
        _PIFV * pfend
        )
{
        int ret = 0;
 
        /*
         * walk the table of function pointers from the bottom up, until
         * the end is encountered.  Do not skip the first entry.  The initial
         * value of pfbegin points to the first valid entry.  Do not try to
         * execute what pfend points to.  Only entries before pfend are valid.
         */
        while ( pfbegin < pfend  && ret == 0)
        {
            /*
             * if current table entry is non-NULL, call thru it.
             */
            if ( *pfbegin != NULL )
                ret = (**pfbegin)();
            ++pfbegin;
        }
 
        return ret;
}
```





从代码上来这两个函数的作用很简单，参数pfbegin和pfend是两个指向函数指针的指针，他们标识了一个指向函数指针的指针组成的数组，pfbegin是数组的第一元素，pfend是超出数组末端的指针，函数的工作就是简单的遍历数组中的每一个元素，获取对应的函数地址，执行函数（函数都是无参的）



在cinit()中是这样调用两个函数的





```cpp
initret = _initterm_e( __xi_a, __xi_z );
```





```cpp
 _initterm( __xc_a, __xc_z );
```

这里引用到了四个全局变量 ，它们的定义是这样的

```cpp
_CRTALLOC(".CRT$XIA") _PVFV __xi_a[] = { NULL };
 
_CRTALLOC(".CRT$XIZ") _PVFV __xi_z[] = { NULL };
 
_CRTALLOC(".CRT$XCA") _PVFV __xc_a[] = { NULL };
 
_CRTALLOC(".CRT$XCZ") _PVFV __xc_z[] = { NULL };
 
#define _CRTALLOC(x) __declspec(allocate(x))
#pragma comment(linker, "/merge:.CRT=.rdata") //
```



可以看出这四个变量分别在数据段`.CRT$XIA`、`.CRT$XIZ`、`.CRT$XCA`、`.CRT$XCZ`中。当连接器布局代码时，它按根据的名称，按照字母排序的规则，排列所有段。这样在段`.CRT$XIA`中的变量出现在段`.CRT$XIZ`所有变量之前，从而形成一个表。对于`.CRT$XCA`、`.CRT$XCZ`数据段同理。最后这四个数据段被合并到.data数据段中。

再看看这些变量的类型，typedef void (__cdecl *_PVFV)(void); 所以这些变量组成了2个初始化函数指表







在MSDN中关于这两个表的描述是这样的





Initializing a Global Object
Consider the following code:
以带有颜色区分的格式查看复制到剪贴板打印

```cpp
int func(void)
{
    return 3;
}

int gi = func();

int main()
{
    return gi;
}
int func(void)
{
    return 3;
}

int gi = func();

int main()
{
    return gi;
}
```

According to the C/C++ standard, func() must be called beforemain() is executed. But who calls it?
One way to determine this is to set a breakpoint in func(), debug the application, and examine the stack. This is possible because the CRT source code is included with Visual Studio.
When you browse the functions on the stack, you will find that the CRT is looping through a list of function pointers and calling each one as it encounters them. These functions are either similar tofunc() or constructors for class instances.
The CRT obtains the list of function pointers from the Visual C++ compiler. When the compiler sees a global initializer, it generates a dynamic initializer in the.CRT$XCU section (where CRT is the section name andXCU is the group name). To obtain a list of those dynamic initializers run the commanddumpbin /all main.obj, and then search the .CRT$XCU section (when main.cpp is compiled as a C++ file, not a C file). It will be similar to the following:

According to the C/C++ standard, func() must be called beforemain() is executed. But who calls it?
One way to determine this is to set a breakpoint in func(), debug the application, and examine the stack. This is possible because the CRT source code is included with Visual Studio.
When you browse the functions on the stack, you will find that the CRT is looping through a list of function pointers and calling each one as it encounters them. These functions are either similar tofunc() or constructors for class instances.
The CRT obtains the list of function pointers from the Visual C++ compiler. When the compiler sees a global initializer, it generates a dynamic initializer in the.CRT$XCU section (where CRT is the section name andXCU is the group name). To obtain a list of those dynamic initializers run the commanddumpbin /all main.obj, and then search the .CRT$XCU section (when main.cpp is compiled as a C++ file, not a C file). It will be similar to the following:





```
SECTION HEADER #6
.CRT$XCU name
       0 physical address
       0 virtual address
       4 size of raw data
     1F2 file pointer to raw data (000001F2 to 000001F5)
     1F6 file pointer to relocation table
       0 file pointer to line numbers
       1 number of relocations
       0 number of line numbers
40300040 flags
         Initialized Data
         4 byte align
         Read Only


RAW DATA #6
  00000000: 00 00 00 00                                      ....


RELOCATIONS #6
                                                Symbol    Symbol
 Offset    Type              Applied To         Index     Name
--------  ----------------  -----------------  --------  ------
 00000000  DIR32                      00000000         C  ??__Egi@@YAXXZ (void __cdecl `dynamic initializer for 'gi''(void))
SECTION HEADER #6
.CRT$XCU name
       0 physical address
       0 virtual address
       4 size of raw data
     1F2 file pointer to raw data (000001F2 to 000001F5)
     1F6 file pointer to relocation table
       0 file pointer to line numbers
       1 number of relocations
       0 number of line numbers
40300040 flags
         Initialized Data
         4 byte align
         Read Only


RAW DATA #6
  00000000: 00 00 00 00                                      ....

RELOCATIONS #6
                                                Symbol    Symbol
 Offset    Type              Applied To         Index     Name

--------  ----------------  -----------------  --------  ------
 00000000  DIR32                      00000000         C  ??__Egi@@YAXXZ (void __cdecl `dynamic initializer for 'gi''(void))
The CRT defines two pointers:
__xc_a in `.CRT$XCA`
__xc_z in `.CRT$XCZ`
Both groups do not have any other symbols defined except __xc_a and__xc_z.
Now, when the linker reads various .CRT groups, it combines them in one section and orders them alphabetically. This means that the user-defined global initializers (which the Visual C++ compiler puts in`.CRT$XCU`) will always come after .CRT$XCA and before`.CRT$XCZ`.
The section will resemble the following:
以带有颜色区分的格式查看复制到剪贴板打印

.CRT$XCA
            __xc_a
.CRT$XCU
            Pointer to Global Initializer 1
            Pointer to Global Initializer 2
.CRT$XCZ
            __xc_z
.CRT$XCA
            __xc_a
.CRT$XCU
            Pointer to Global Initializer 1
            Pointer to Global Initializer 2
.CRT$XCZ
            __xc_z

So, the CRT library uses both __xc_a and __xc_z to determine the start and end of the global initializers list because of the way in which they are laid out in memory after the image is loaded. 
```





根据微软的解释，当VC编译器看到一个全局的对象，就动态的构造一个该对象的初始化器，并把初始化函数的指针放入.CRT段的XCU组中，当链接器生成应用程序的时候是按照自如顺序组织段内结构的，因此当是使用C++编译链接程序时
这些函数指针被放到了__xc_a和__xc_z 之间 


__xc_a和__xc_z 都被定义成了空的，因此__xc_a实际指向了这组函数指针的第一个元素   __xc_z指向了数组超出数组末尾的未知数据
同样的道理当使用C模式来编译程序的时候  函数指针数组被放到__xi_a和__xi_z 之间

这样当我们调用
initret = _initterm_e( __xi_a, __xi_z );
 _initterm( __xc_a, __xc_z );
的时候就可以  所有的在编译和链接后放入`.CRT$XCA`和`.CRT​$XCZ`     ` .CRT​$XIA`和`.CRT​$XIZ`之间的函数指针都会被调用





下面是完整的_cinit函数代码







```cpp
int __cdecl _cinit (
        int initFloatingPrecision
        )
{
        int initret;
 
        /*
         * initialize floating point package, if present
         */
#ifdef CRTDLL
        _fpmath(initFloatingPrecision);      //初始化浮点运算
#else  /* CRTDLL */
        if (_FPinit != NULL &&
            _IsNonwritableInCurrentImage((PBYTE)&_FPinit))
        {
            (*_FPinit)(initFloatingPrecision);
        }
        _initp_misc_cfltcvt_tab();
#endif  /* CRTDLL */
 
        /*
         * do initializations
         */
        initret = _initterm_e( __xi_a, __xi_z );
        if ( initret != 0 )
            return initret;
 
#ifdef _RTC
        atexit(_RTC_Terminate);
#endif  /* _RTC */
        /*
         * do C++ initializations
         */
        _initterm( __xc_a, __xc_z );
 
#ifndef CRTDLL
        /*
         * If we have any dynamically initialized __declspec(thread)
         * variables, then invoke their initialization for the thread on
         * which the DLL is being loaded, by calling __dyn_tls_init through
         * a callback defined in tlsdyn.obj.  We can't rely on the OS
         * calling __dyn_tls_init with DLL_PROCESS_ATTACH because, on
         * Win2K3 and before, that call happens before the CRT is
         * initialized.
         */
        if (__dyn_tls_init_callback != NULL &&                                             //关于静态TLS的检查设置
            _IsNonwritableInCurrentImage((PBYTE)&__dyn_tls_init_callback))
        {
            __dyn_tls_init_callback(NULL, DLL_THREAD_ATTACH, NULL);
        }
#endif  /* CRTDLL */
 
        return 0;
}
```





在完成了了所有的初始化工作之后就用 _WINMAIN_ 调用我们的main()函数了，这之后我们的代码才获得了执行权，









# （5）使用动态运行库的EXE



前面看到在使用静态运行库的EXE的启动代码做了很多的工作，下面再来看看使用动态库的EXE的启动代码



使用/mtd选项生产一个EXE，在我们的main函数开头下断，查看调用堆栈可以看到程序在被系统加载完成之后，我们程序的入口由Crtexe.c文件中的__tmainCRTStartup开始

下面是代码





```cpp
static
int
__tmainCRTStartup(
         void
         );
 
#ifdef _WINMAIN_
 
#ifdef WPRFLAG
int wWinMainCRTStartup(
#else  /* WPRFLAG */
int WinMainCRTStartup(
#endif  /* WPRFLAG */
#else  /* _WINMAIN_ */
#ifdef WPRFLAG
int wmainCRTStartup(
#else  /* WPRFLAG */
int mainCRTStartup(
#endif  /* WPRFLAG */
#endif  /* _WINMAIN_ */
        void
        )
{
 
        __security_init_cookie();
 
        return __tmainCRTStartup();
}
```

跟使用静态运行库时一样的   同样先是创建了一个cookier然后 四种情况下调用的是同一个函数__tmainCRTStartup





直接上代码  在代码中注释，相比使用静态库，使用动态库的启动代码只完成了很有限的工作。





```cpp
__declspec(noinline)
int
__tmainCRTStartup(
        void
        )
{
#ifdef _WINMAIN_
        _TUCHAR *lpszCommandLine;
        STARTUPINFOW StartupInfo;
        BOOL inDoubleQuote=FALSE;
 
        GetStartupInfoW( &StartupInfo );
#endif  /* _WINMAIN_ */
 
#ifdef _M_IX86
        /*
         * Enable app termination when heap corruption is detected on
         * Windows Vista and above. This is a no-op on down-level OS's
         * and enabled by default for 64-bit processes.
         */
                           //在XPSP3 VISTA 及之后的版本中增加了一个Heap管理特性，当Heap管理器发现进程中有任何使用中的Heap出现错误的时候，系统终止进程
        if (!_NoHeapEnableTerminationOnCorruption) //如果该全局标准为0表示 需要开启该特性
        {
            HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);   //设置开启
        }
#endif  /* _M_IX86 */
 
        /*
        __try   //
        {
  
            void *lock_free=0;
            void *fiberid=((PNT_TIB)NtCurrentTeb())->StackBase;  获取栈基址
            int nested=FALSE;
            while((lock_free=InterlockedCompareExchangePointer((volatile PVOID *)&__native_startup_lock, fiberid, 0))!=0)
            {                                                     //如果__native_startup_lock为0 把fiberid的值给它
                if(lock_free==fiberid)
                {
                    nested=TRUE;
                    break;
                }
                Sleep(1000);
            }
            if (__native_startup_state == __initializing)  //在混合模式的情况下如果其他本地地方在初始化本地代码，那么出错，不能同时初始化
            {
                _amsg_exit( _RT_CRT_INIT_CONFLICT);
            }
            else if (__native_startup_state == __uninitialized)
            {
                __native_startup_state = __initializing;
#ifndef _SYSCRT
                if (_initterm_e( __xi_a, __xi_z ) != 0)
                {
                    return 255;                                         //初始化C全局对象
                }
#else  /* _SYSCRT */
                _initterm((_PVFV *)(void *)__xi_a, (_PVFV *)(void *)__xi_z);  
 
#endif  /* _SYSCRT */ 
            }
            else
            {
                has_cctor = 1;
            }
 
            /*
            * do C++ constructors (initializers) specific to this EXE
            */
            if (__native_startup_state == __initializing) 
            {
                _initterm( __xc_a, __xc_z );                       //初始化C++全局对象
                __native_startup_state = __initialized;
            }
            _ASSERTE(__native_startup_state == __initialized);
            if(!nested)
            {
                /* For X86, the definition of InterlockedExchangePointer wrongly causes warning C4312 */
#pragma warning(push)
#pragma warning(disable:4312)
                InterlockedExchangePointer((volatile PVOID *)&__native_startup_lock, 0);
#pragma warning(pop)
            }
 
 
             */
            if (__dyn_tls_init_callback != NULL &&
                _IsNonwritableInCurrentImage((PBYTE)&__dyn_tls_init_callback))
            {
                __dyn_tls_init_callback(NULL, DLL_THREAD_ATTACH, NULL);            //初始化TLS相关的
            }
 
            /* Enable buffer count checking if linking against static lib */
            _CrtSetCheckCount(TRUE);
 
#ifdef _WINMAIN_
            /*
             * Skip past program name (first token in command line).
             * Check for and handle quoted program name.
             */
#ifdef WPRFLAG
            lpszCommandLine = (wchar_t *)_wcmdln;
#else  /* WPRFLAG */
            lpszCommandLine = (unsigned char *)_acmdln;
#endif  /* WPRFLAG */
 
            while (*lpszCommandLine > SPACECHAR ||
                   (*lpszCommandLine&&inDoubleQuote)) {
                /*
                 * Flip the count from 1 to 0 or 0 to 1 if current character
                 * is DOUBLEQUOTE
                 */
                if (*lpszCommandLine==DQUOTECHAR) inDoubleQuote=!inDoubleQuote;
#ifdef _MBCS
                if (_ismbblead(*lpszCommandLine)) {
                    if (lpszCommandLine) {
                        lpszCommandLine++;
                    }
                }
#endif  /* _MBCS */
                ++lpszCommandLine;
            }
 
            /*
             * Skip past any white space preceeding the second token.
             */
            while (*lpszCommandLine && (*lpszCommandLine <= SPACECHAR)) {
                lpszCommandLine++;
            }                                                //去掉命令行参数中程序自身路径的部分
 
#ifdef WPRFLAG
            mainret = wWinMain(
#else  /* WPRFLAG */
            mainret = WinMain(
#endif  /* WPRFLAG */
                       (HINSTANCE)&__ImageBase,
                       NULL,
                       lpszCommandLine,
                       StartupInfo.dwFlags & STARTF_USESHOWWINDOW
                        ? StartupInfo.wShowWindow
                        : SW_SHOWDEFAULT                               //调用我们的main函数
                      );
#else  /* _WINMAIN_ */
 
#ifdef WPRFLAG
            __winitenv = envp;
            mainret = wmain(argc, argv, envp);
#else  /* WPRFLAG */
            __initenv = envp;
            mainret = main(argc, argv, envp);
#endif  /* WPRFLAG */
 
#endif  /* _WINMAIN_ */
 
            /*
             * Note that if the exe is managed app, we don't really need to
             * call exit or _c_exit. .cctor should be able to take care of
             * this.
             */
            if ( !managedapp )
                exit(mainret);
 
            if (has_cctor == 0)
                _cexit();         //退出代码，
 
        }
        __except ( _XcptFilter(GetExceptionCode(), GetExceptionInformation()) )
        {
            /*                                                    //发生异常调用退出函数 终止进程
             * Should never reach here
             */
 
            mainret = GetExceptionCode();
 
            /*
             * Note that if the exe is managed app, we don't really need to
             * call exit or _c_exit. .cctor should be able to take care of
             * this.
             */
            if ( !managedapp )
                _exit(mainret);
 
            if (has_cctor == 0)
                _cexit();
        } /* end of try - except */
 
        return mainret;
}
```

使用动态库的EXE的启动代码  除了 初始化全局对象外，并没有做其他实质性的初始化工作，然后就调用了main函数 将执行权限交给了我们

因此可以推断对CRT的初始化工作，运行库的DLL文件中，当Msvcrtxx.DLL被加载到后，DLL的启动代码完成了之前我们分析的，使用静态库的exe的启动代码所做的工作。



于是我在VC\crt\src  下搜索包含_ioinit的文件，发现crtlib.c文件中的_CRTDLL_INIT 函数调用了这个初始化io的函数

__CRTDLL_INIT 开始地方下断  果然在这里

![img](https://img-blog.csdn.net/20170104003202211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

代码



```cpp
BOOL WINAPI
_CRTDLL_INIT(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        /*
         * The /GS security cookie must be initialized before any exception
         * handling targetting the current image is registered.  No function
         * using exception handling can be called in the current image until
         * after __security_init_cookie has been called.
         */
        __security_init_cookie();
    }
 
    return __CRTDLL_INIT(hDllHandle, dwReason, lpreserved);
```

很明显就是这个运行库DLL的启动代码了   跟EXE的如出一辙



```cpp
__declspec(noinline)
BOOL __cdecl
__CRTDLL_INIT(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
        if ( dwReason == DLL_PROCESS_ATTACH ) {
            if ( !_heap_init() )    /* initialize heap */   //分配堆
 
                return FALSE;
 
            if(!_mtinit())          /* initialize multi-thread */  多线程支持
            {
                _heap_term();       /* heap is now invalid! */
 
                return FALSE;       /* fail DLL load on failure */
            }
 
            if (_ioinit() < 0) {    /* inherit file info */  初始IO
 
                _mtterm();
 
                _heap_term();       /* heap is now invalid! */
 
                return FALSE;       /* fail DLL load on failure */
            }
 
            _aenvptr = (char *)__crtGetEnvironmentStringsA();   获取环境变量
 
            _acmdln = GetCommandLineA();
            _wcmdln = GetCommandLineW();
 
#ifdef _MBCS
            __initmbctable();
#endif  /* _MBCS */
 
 
            if (_setenvp() < 0 ||   /* get environ info */
                _cinit(FALSE) != 0)  /* do C data initialize */
            {
                _ioterm();          /* shut down lowio */
                _mtterm();          /* free TLS index, call _mtdeletelocks() */
                _heap_term();       /* heap is now invalid! */
                return FALSE;       /* fail DLL load on failure */
            }
            proc_attached++;
 
        }
        else if ( dwReason == DLL_PROCESS_DETACH ) {
 
           ..................................   清理工作省略
        }
 
        else if ( dwReason == DLL_THREAD_ATTACH )
        {...........................................清理工作省略
 
          }
        return TRUE;
}
```

一切都是那么的熟悉有木有，所有的初始化工作这个DLL都给做了，EXE安心的调用它提供的库函数就OK了





