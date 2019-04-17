# vs2013编译ffmpeg之三十七 其他编译错误 - 枫林港的博客 - CSDN博客





2018年07月10日 23:35:50[枫林港](https://me.csdn.net/u013601132)阅读数：225








# 其他编译错误

## time.c

ffmpeg-3.0编译的时候报下面的错误，usleep在mingwex.lib里面，直接在time.c里面加上extern int usleep();就可以解决这个问题。

> 
libavutil/time.c(84) : error C4013: “usleep”未定义；假设外部返回 int


libavutil/time.c报下面的错误：

> 
“sys/time.h”: No such file or directory


gettimeofday 在mingwex.lib里面，这样在configure后HAVE_GETTIMEOFDAY为1了，而vs里面没有sys/time.h。这个问题的解决办法，将libavutil/time.c里面的：
`#include <sys/time.h>`
改成：
`#include <time.h>`
并加上extern int gettimeofday();

## couldn’t commit memory

> 
make.exe: *** couldn’t commit memory for cygwin heap, Win32 error 0


这个问题是make的时候带了参数V=1导致的，V=1可以看到编译时的整个cl命令，解决编译问题的时候很有用。make的时候不带V=1参数就不会有这个问题。

## –enable-shared

打开–enable-shared后，会出现新的错误。

> 
SDL.lib(SDL_error.obj) : error LNK2005: _SDL_GetError already defined in avdevice.lib(avdevice-57.dll)

SDL.lib(SDL_audio.obj) : error LNK2005: _SDL_OpenAudio already defined in avdevice.lib(avdevice-57.dll)

SDL.lib(SDL_audio.obj) : error LNK2005: _SDL_PauseAudio already defined in avdevice.lib(avdevice-57.dll)

SDL.lib(SDL_audio.obj) : error LNK2005: _SDL_CloseAudio already defined in avdevice.lib(avdevice-57.dll)

SDL.lib(SDL_thread.obj) : error LNK2005: _SDL_CreateThread already defined in avdevice.lib(avdevice-57.dll)

SDL.lib(SDL_thread.obj) : error LNK2005: _SDL_WaitThread already defined in avdevice.lib(avdevice-57.dll)


解决的办法参考[https://trac.ffmpeg.org/ticket/460](https://trac.ffmpeg.org/ticket/460)，即在编译SDL的时候加‘DECLSPEC=’。 
![这里写图片描述](https://img-blog.csdn.net/20180707003922935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## fenv.h

在编译vf_drawtext.c的时候会报错，在这个文件里面会调用feclearexcept，这个函数在/mingw/include/fenv.h里面声明的。configure/make的时候都没有使用/mingw/include作为头文件路径，所以fenv.h找不到。

解决的办法是直接将fenv.h拷贝到ffmpeg-2.6.1\inc目录学，fenv.h里面包含了_mingw.h，直接注释掉，因为_mingw.h的内容已经放到ffmpeg-3.3\inc\getopt.h里面了。

## ___cpu_features

> 
mingwex.lib(fetestexcept.o) : error LNK2001: unresolved external symbol ___cpu_features

mingwex.lib(feclearexcept.o) : error LNK2001: unresolved external symbol ___cpu_features

ffmpeg_g.exe : fatal error LNK1120: 1 unresolved externals


int __cpu_features = 0;定义放在libavfilter下任意会编译到的.c文件中都可以，目前放在libavfilter\avfilter.c里面。

## AF_IPX重定义

ffmpeg-3.3有下面的错误，ffmpeg-3.2.2没有，ffmpeg-3.3在compat下面增加了一些文件导致的。

> 
C:\Program Files (x86)\Windows Kits\8.1\include\shared\ws2def.h(100) : warning C4005: “AF_IPX”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(452) : 参见“AF_IPX”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\shared\ws2def.h(133) : warning C4005: “AF_MAX”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(471) : 参见“AF_MAX”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\shared\ws2def.h(177) : warning C4005: “SO_DONTLINGER”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(394) : 参见“SO_DONTLINGER”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\shared\ws2def.h(556) : warning C4005: “IN_CLASSA”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(279) : 参见“IN_CLASSA”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\shared\ws2def.h(562) : warning C4005: “IN_CLASSB”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(285) : 参见“IN_CLASSB”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\shared\ws2def.h(568) : warning C4005: “IN_CLASSC”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(291) : 参见“IN_CLASSC”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\shared\ws2def.h(579) : warning C4005: “INADDR_ANY”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(296) : 参见“INADDR_ANY”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\shared\ws2def.h(581) : warning C4005: “INADDR_BROADCAST”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(298) : 参见“INADDR_BROADCAST”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock2.h(156) : warning C4005: “FD_CLR”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(89) : 参见“FD_CLR”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock2.h(171) : warning C4005: “FD_SET”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(94) : 参见“FD_SET”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock2.h(517) : warning C4005: “SOMAXCONN”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(536) : 参见“SOMAXCONN”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock2.h(551) : warning C4005: “FD_READ”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(554) : 参见“FD_READ”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock2.h(554) : warning C4005: “FD_WRITE”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(555) : 参见“FD_WRITE”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock2.h(557) : warning C4005: “FD_OOB”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(556) : 参见“FD_OOB”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock2.h(560) : warning C4005: “FD_ACCEPT”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(557) : 参见“FD_ACCEPT”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock2.h(563) : warning C4005: “FD_CONNECT”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(558) : 参见“FD_CONNECT”的前一个定义

C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock2.h(566) : warning C4005: “FD_CLOSE”: 宏重定义

​        C:\Program Files (x86)\Windows Kits\8.1\include\um\winsock.h(559) : 参见“FD_CLOSE”的前一个定义


解决的办法是在ffmpeg-3.3\compat\atomics\win32\stdatomic.h里面，#include 

```
#ifndef WIN32_LEAN_AND_MEAN

#define WIN32_LEAN_AND_MEAN

#endif
```

## release版本链接错误

VS2013+ffmpeg-3.3在编译release版本的时候，报下面的错误：

> 
1>cmdutils.obj : error LNK2001: unresolved external symbol __imp__CommandLineToArgvW@8

1>libavdevice.a(dshow.o) : error LNK2001: unresolved external symbol __imp__OleCreatePropertyFrame@44

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__BitBlt@36

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__CombineRgn@16

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__CreateCompatibleDC@4

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__CreateRectRgn@16

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__DeleteDC@4

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__DeleteObject@4

1>libcryptod.lib(rand_win.obj) : error LNK2001: unresolved external symbol __imp__DeleteObject@4

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__GetDeviceCaps@8

1>libcryptod.lib(rand_win.obj) : error LNK2001: unresolved external symbol __imp__GetDeviceCaps@8

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__GetStockObject@4

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__SelectObject@8

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__CreateDIBSection@24

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__GetDIBColorTable@16

1>libavdevice.a(gdigrab.o) : error LNK2001: unresolved external symbol __imp__GetObjectA@12

1>OpenAL32.lib(alcConfig.obj) : error LNK2001: unresolved external symbol __imp__SHGetSpecialFolderPathA@16

1>libsshd.lib(misc.obj) : error LNK2001: unresolved external symbol __imp__SHGetSpecialFolderPathA@16

1>libblurayd.lib(dirs_win32.obj) : error LNK2001: unresolved external symbol __imp__SHGetFolderPathW@20

1>libcryptod.lib(rand_win.obj) : error LNK2001: unresolved external symbol __imp__CreateCompatibleBitmap@12

1>libcryptod.lib(rand_win.obj) : error LNK2001: unresolved external symbol __imp__GetDIBits@28

1>libcryptod.lib(rand_win.obj) : error LNK2001: unresolved external symbol __imp__GetObjectW@12


这些缺少的符号是windows的，可以在[https://msdn.microsoft.com/en-us/library/windows/desktop/aa906039.aspx](https://msdn.microsoft.com/en-us/library/windows/desktop/aa906039.aspx)里面搜索。加上OleAut32.lib、Ole32.lib、Shell32.lib、Gdi32.lib这几个库就可以解决。




