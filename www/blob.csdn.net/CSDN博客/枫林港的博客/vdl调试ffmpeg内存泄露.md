# vdl调试ffmpeg内存泄露 - 枫林港的博客 - CSDN博客





置顶2018年07月24日 23:03:57[枫林港](https://me.csdn.net/u013601132)阅读数：194








# vld调试memory leaks

vs2013调试ffmpeg时，在程序退出的时候报：

> 
Detected memory leaks! 

  Dumping objects -> 

  {25351} normal block at 0x01BBE098, 200 bytes long. 

   Data: <                > 80 B2 BB 01 00 E0 BB 01 CD CD CD CD CD CD CD CD  

  {25345} normal block at 0x01BBB040, 500 bytes long. 

   Data: < E:\tmp\stream\Be> 45 3A 5C 74 6D 70 5C 73 74 72 65 61 6D 5C 42 65  

  {25296} normal block at 0x01BBDD10, 500 bytes long. 

   Data: < E :\ t m p\ s > 45 00 3A 00 5C 00 74 00 6D 00 70 00 5C 00 73 00  

  Object dump complete.
由于几个地址一直在变化，无法用_CrtSetBreakAlloc()方法来解决。后来网上搜到vld可以检测。官网：[http://vld.codeplex.com/](http://vld.codeplex.com/)，以及：[https://kinddragon.github.io/vld/](https://kinddragon.github.io/vld/)，当前最新版本为2.5.1，下载vld-2.5.1-setup.exe并安装，安装后其dll库的路径已经放到环境变量Path里面了。

使用方法
- 
在某个源代码文件里面（.c文件或.h文件都可以）`#include <vld.h>`，相对于其他头文件来说，include的顺序无所谓，但必须放到`#include "stdafx.h"`（如果有的话）后面。如果要检测某个dll是否有内存泄露，dll的源代码里面也需要`#include <vld.h>`

- 
vld-2.5.1-setup.exe安装好后，vld的头文件路径和库文件路径在vs2013里面都已经设置好了，新建一个工程，在工程设置里面可以看到相关的路径。vld_x86.dll或vld_x64.dll路径也已经设置好了，具体是怎么找到这个dll的没有搞清楚，反正程序运行的时候没有提示缺vld的dll，程序的同级目录下也没有vld的dll。

- 
将其配置文件vld.ini（C:\Program Files (x86)\Visual Leak Detector\vld.ini）拷贝到被调试的可执行文件的同级目录。里面有两个比较重要的配置项：


```
ReportFile =.\memory_leak_report.txt 
ReportTo = both
```

ReportTo需要设置成both，否则不会生成memory_leak_report.txt，打印只会在MSVC的output窗口显示。 

 4.  程序编译成debug版本，release版本中vld不会起作用。 

 5. 被调试的程序退出后，就可以看到打印了。对于malloc后没有释放这种泄露很有效，可以精确到代码行。 

 6. 如果被调试程序启动的时候报找不到vld_x86.dll，则将vld_x86.dll（C:\Program Files (x86)\Visual Leak Detector\bin\Win32\）拷贝到被调试的可执行文件的同级目录。 

 7.  如果报下面的错误： 
![这里写图片描述](https://img-blog.csdn.net/20180724004028294?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
则将vld_x86.dll、Microsoft.DTfW.DHL.manifest、dbghelp.dll（3个文件都在C:\Program Files (x86)\Visual Leak Detector\bin\Win32\下面）拷贝到被调试的可执行文件的同级目录。 

   8. 头文件vld.h
`'#include <vld.h>' should appear before '#include <afxwin.h>' in file stdafx.h`
V2.5没有问题，安装了V2.5.1后出现了这个问题。解决办法是，直接将`#include <vld.h>`放到stdafx.h里面，并放到`#include <afxwin.h>`之前。vld.h不能放在stdafx.h前面，否则的话模块不会起作用，程序运行完毕关闭后，没有是否有内存泄露的打印。

## ffmpeg内存泄露

`下面所描述的内存泄露，在ffmpeg程序的生命周期中只会泄露一次，程序退出后就会被系统回收，不介意的话可以不用管。但是，如果程序调用ffmpeg库的相关接口，并且是多次调用的话，那么可能会造成多次泄露，建议还是解决一下。`

ff_gnutls_init/ff_gnutls_deinit这两个函数分别在avformat_network_init/avformat_network_deinit里面调用的。vld的打印：

```
---------- Block 963 at 0x004CAD28: 63 bytes ----------
CRT Alloc ID: 966
Leak Hash: 0xEB1F8F19, Count: 1, Total 63 bytes
Call Stack (TID 9268):
ntdll.dll!RtlAllocateHeap()
f:\dd\vctools\crt\crtw32\misc\dbgheap.c (2621): SDL2_test1.exe!_aligned_malloc() + 0x13 bytes
    e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\msvc_ffmpeg\ffmpeg-3.3\libavutil\mem.c (90): SDL2_test1.exe!av_malloc() + 0xB bytes
    e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\msvc_ffmpeg\ffmpeg-3.3\libavcodec\utils.c (78): SDL2_test1.exe!default_lockmgr_cb() + 0x7 bytes
    e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\msvc_ffmpeg\ffmpeg-3.3\libavcodec\utils.c (3947): SDL2_test1.exe!avpriv_lock_avformat() + 0xD bytes
    e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\msvc_ffmpeg\ffmpeg-3.3\libavformat\tls_gnutls.c (59): SDL2_test1.exe!ff_gnutls_init()
    e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\msvc_ffmpeg\ffmpeg-3.3\libavformat\network.c (40): SDL2_test1.exe!ff_tls_init()
    e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\msvc_ffmpeg\ffmpeg-3.3\libavformat\utils.c (4725): SDL2_test1.exe!avformat_network_init() + 0x5 bytes
    e:\mingw\msys\1.0\home\aa\mfc\sdl2_test\sdl2_test1\sdl2_test1\sdl2_test1dlg.cpp (274): SDL2_test1.exe!CSDL2_test1Dlg::mainPlayer_thread()
    kernel32.dll!BaseThreadInitThunk() + 0x12 bytes
    ntdll.dll!RtlInitializeExceptionChain() + 0x63 bytes
    ntdll.dll!RtlInitializeExceptionChain() + 0x36 bytes
  Data:
    CD CD CD CD    CD CD CD CD    CD CD CD CD    CD CD CD CD     ........ ........
    28 AD 4C 00    ED ED ED ED    88 AD 4C 00    FF FF FF FF     (.L..... ..L.....
    00 00 00 00    00 00 00 00    00 00 00 00    00 00 00 00     ........ ........
    CD CD CD CD    CD CD CD CD    CD CD CD CD    CD CD CD        ........ ........
```

从ffmpeg的代码来分析，代码在libavcodec/utils里面：

```
static int default_lockmgr_cb(void **arg, enum AVLockOp op)
{
    void * volatile * mutex = arg;
    int err;

    switch (op) {
    case AV_LOCK_CREATE:
        return 0;
    case AV_LOCK_OBTAIN:
        if (!*mutex) {
            pthread_mutex_t *tmp = av_malloc(sizeof(pthread_mutex_t));
            if (!tmp)
                return AVERROR(ENOMEM);
            if ((err = pthread_mutex_init(tmp, NULL))) {
                av_free(tmp);
                return AVERROR(err);
            }
            if (avpriv_atomic_ptr_cas(mutex, NULL, tmp)) {
                pthread_mutex_destroy(tmp);
                av_free(tmp);
            }
        }

        if ((err = pthread_mutex_lock(*mutex)))
            return AVERROR(err);

        return 0;
    case AV_LOCK_RELEASE:
        if ((err = pthread_mutex_unlock(*mutex)))
            return AVERROR(err);
        return 0;
    case AV_LOCK_DESTROY:
        if (*mutex)
            pthread_mutex_destroy(*mutex);
        av_free(*mutex);
        avpriv_atomic_ptr_cas(mutex, *mutex, NULL);
        return 0;
    }
    return 1;
}

static int(*lockmgr_cb)(void **mutex, enum AVLockOp op) = default_lockmgr_cb;
int avpriv_lock_avformat(void)
{
    if (lockmgr_cb) {
        if ((*lockmgr_cb)(&avformat_mutex, AV_LOCK_OBTAIN))
            return -1;
    }
    return 0;
}

int avpriv_unlock_avformat(void)
{
    if (lockmgr_cb) {
        if ((*lockmgr_cb)(&avformat_mutex, AV_LOCK_RELEASE))
            return -1;
    }
    return 0;
}

void ff_gnutls_init(void)
{
    avpriv_lock_avformat();
#if HAVE_THREADS && GNUTLS_VERSION_NUMBER < 0x020b00
    if (gcry_control(GCRYCTL_ANY_INITIALIZATION_P) == 0)
        gcry_control(GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
#endif
    gnutls_global_init();
    avpriv_unlock_avformat();
}

void ff_gnutls_deinit(void)
{
    avpriv_lock_avformat();
    gnutls_global_deinit();
    avpriv_unlock_avformat();
}
```

HAVE_W32THREADS是有定义的。ff_gnutls_init发送AV_LOCK_OBTAIN消息给default_lockmgr_cb，default_lockmgr_cb调用av_malloc分配mutex。从default_lockmgr_cb的代码看，AV_LOCK_DESTROY消息才会释放mutex。但代码里面没有谁发送AV_LOCK_DESTROY消息。

所以解决的办法是直接在ff_gnutls_deinit里面发送AV_LOCK_DESTROY消息：

```
int avpriv_lock_destroy_avformat(void)
{
    if (lockmgr_cb) {
        if ((*lockmgr_cb)(&avformat_mutex, AV_LOCK_DESTROY))
            return -1;
        else
            avformat_mutex = NULL;
    }
    return 0;
}
void ff_gnutls_deinit(void)
{
    avpriv_lock_avformat();
    gnutls_global_deinit();
    avpriv_unlock_avformat();
    avpriv_lock_destroy_avformat();
}
```

增加destroy接口avpriv_lock_destroy_avformat，不能直接将AV_LOCK_DESTROY放到avpriv_unlock_avformat里面，每次重新创建mutex和没有锁一样，根本就没有锁的作用。

看上去分配mutex和加锁都放在AV_LOCK_OBTAIN并不合理，不知道为何会这么设计。

ffmpg.c、ffplay.c、ffprobe.c里面，只有ffplay.c有调用av_lockmgr_register注册lockmgr_cb函数。所以ffmpg.c、ffprobe.c都没有这个内存泄露问题。

## gnutils内存泄露

VLD报gnutils有内存泄露，这个不是ffmpeg的代码。

> 
​    f:\dd\vctools\crt\crtw32\misc\dbgmalloc.c (56): SDL2_test1.exe!malloc() + 0x15 bytes

​    e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\contribute\msvc\gnutls-master_20160630\lib\gnutls_global.c (223): SDL2_test1.exe!gnutls_global_init() + 0x10 bytes

​    e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\msvc_ffmpeg\ffmpeg-3.3\libavformat\tls_gnutls.c (60): SDL2_test1.exe!ff_gnutls_init()

​    kernel32.dll!BaseThreadInitThunk() + 0x12 bytes

​    ntdll.dll!RtlInitializeExceptionChain() + 0x63 bytes

​    ntdll.dll!RtlInitializeExceptionChain() + 0x36 bytes

Data:

​    20 EE DA 03    FF FF FF FF    00 00 00 00    00 00 00 00     …….. ……..

​    00 00 00 00    00 00 00 00                                   …….. ……..


原因和ffmpeg的类似，GNUTLS_STATIC_MUTEX_LOCK里面分配并获取了锁，但没有接口可以释放。直接修改gnutls_global_deinit：

```
# define GNUTLS_STATIC_MUTEX_DESTROY(mutex) \
        if(*mutex) { free(*mutex); *mutex = NULL; }

void gnutls_global_deinit(void)
{
    _gnutls_global_deinit(0);
    GNUTLS_STATIC_MUTEX_DESTROY(&global_init_mutex);
}
```






