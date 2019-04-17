# vs2013编译ffmpeg之二十 openal - 枫林港的博客 - CSDN博客





2018年07月09日 00:17:07[枫林港](https://me.csdn.net/u013601132)阅读数：76








# openal

对应ffmpeg configure选项–enable-openal。

[http://www.openal.org/](http://www.openal.org/)上没有找到源代码（只有预先编译好的包），在[www.openal-soft.org](http://www.openal-soft.org)上下载到了源代码，版本openal-soft-1.15.1。这样有可能有问题，这个功能没有验证过。

## 编译错误一

在链接的时候出现了下面的错误：

```
libavdevice.a(openal-dec.o) : error LNK2019: unresolved external symbol impalcGetString referenced in function _al_get_error

libavdevice.a(openal-dec.o) : error LNK2019: unresolved external symbol impalcGetError referenced in function _al_get_error

libavdevice.a(openal-dec.o) : error LNK2019: unresolved external symbol impalcCaptureStart referenced in function _read_header

libavdevice.a(openal-dec.o) : error LNK2019: unresolved external symbol impalcCaptureCloseDevice referenced in function _read_header

libavdevice.a(openal-dec.o) : error LNK2019: unresolved external symbol impalcCaptureOpenDevice referenced in function _read_header

libavdevice.a(openal-dec.o) : error LNK2019: unresolved external symbol impalcCaptureSamples referenced in function _read_packet

libavdevice.a(openal-dec.o) : error LNK2019: unresolved external symbol impalcGetIntegerv referenced in function _read_packet

libavdevice.a(openal-dec.o) : error LNK2019: unresolved external symbol impalcCaptureStop referenced in function _read_close
```

从openal-dec.o的符号表里面发现有(用nm命令可以导出符号表)：

U __imp__alcGetString

而从openal-dec.c里面看：
`error_msg_ret = (const char) alcGetString(device, error);`
调用的时候并没有**imp**前缀。在Openal源代码里面的al.h/alc.h里面有：

```
#ifndef ALC_API

 #if defined(AL_LIBTYPE_STATIC)

  #define ALC_API

 #elif defined(_WIN32)

  #define ALC_API __declspec(dllimport)

 #else

  #define ALC_API extern

 #endif

#endif
```

在openal-dec.c中al.h/alc.h前面，加上#define AL_LIBTYPE_STATIC就不会有问题了。其他的库用nm倒出来的符号表，对外的接口里面每个符号都有个对应的加了**imp**前缀的符号。

## 编译错误二

编译openal-soft-1.15.1的openal-info.exe的时候，会有下面的打印：

```
2>openal-info.obj : error LNK2001: unresolved external symbol impalcCloseDevice

2>openal-info.obj : error LNK2001: unresolved external symbol impalcDestroyContext

2>openal-info.obj : error LNK2001: unresolved external symbol impalcMakeContextCurrent

2>openal-info.obj : error LNK2001: unresolved external symbol impalcCreateContext

2>openal-info.obj : error LNK2001: unresolved external symbol impalcOpenDevice

2>openal-info.obj : error LNK2001: unresolved external symbol impalcGetString

2>openal-info.obj : error LNK2001: unresolved external symbol impalcIsExtensionPresent

2>openal-info.obj : error LNK2001: unresolved external symbol impalcGetIntegerv

2>openal-info.obj : error LNK2001: unresolved external symbol impalcGetError

2>openal-info.obj : error LNK2001: unresolved external symbol impalGetString

2>openal-info.obj : error LNK2001: unresolved external symbol impalGetError

2>openal-info.obj : error LNK2001: unresolved external symbol impalGetEnumValue
```

也是同样的解决办法，即在openal-info.c最前面加上#define AL_LIBTYPE_STATIC。

##编译错误三##

编译ffmpeg_g的时候，下面这个错误：

```
OpenAL32.lib(null.obj) : error LNK2001: unresolved external symbol imptimeGetTime@0

SDL.lib(SDL_systimer.obj) : error LNK2019: unresolved external symbol imptimeBeginPeriod@4 referenced in function _SDL_StartTicks

SDL.lib(SDL_systimer.obj) : error LNK2019: unresolved external symbol imptimeSetEvent@20 referenced in function _SDL_SYS_TimerInit

SDL.lib(SDL_systimer.obj) : error LNK2019: unresolved external symbol imptimeEndPeriod@4 referenced in function _SDL_SYS_TimerQuit

SDL.lib(SDL_systimer.obj) : error LNK2019: unresolved external symbol imptimeKillEvent@4 referenced in function _SDL_SYS_TimerQuit

SDL.lib(SDL_dx5video.obj) : error LNK2001: unresolved external symbol _GUID_POV
```

将WinMM.Lib Gdi32.lib dxguid.lib链进来就可以了。

```bash
enabled openal            && { { for al_libs in "${OPENAL_LIBS}" "-lopenal" "-lOpenAL32"; do

                               check_lib 'AL/al.h' alGetError "${al_libs}"  && break; done } ||
```

改成：

```bash
enabled openal            && { { for al_libs in "${OPENAL_LIBS}" "-lopenal" "-lOpenAL32"; do

                               check_lib 'AL/al.h' alGetError "${al_libs}" -lWinMM -lUser32 -lole32 && break; done } ||
```

加了dxguid.lib后有了新问题，因为dxguid.lib是用/MT编译的，其他的库和ffmpeg都是用/MTd编译的，编译ffmpeg_g.exe的时候会报错：
`LIBCMT.lib(invarg.obj) : error LNK2005: __initp_misc_invarg already defined in LIBCMTD.lib(invarg.obj)`
因为没有dxguid.lib的源代码，所以只能加/NODEFAULTLIB:LIBCMTD.lib来解决。

## 编译错误四

vs2013+ffmpeg-3.3，前面的openal是用vs2010编译的，在vs2013下需要重新执行cmake，编译的时候出现出现下面的错误：
`./CMakeFiles/CMakeError.log:26:LINK : fatal error LNK1146: 没有用选项“/L/usr/win32/lib:”指定的参数 [K:\MinGW\msys\1.0\home\AA\contribute\MSVC\openal-soft-1.15.1\CMakeFiles\CMakeTmp\cmTC_550b6.vcxproj]`
这个额外的选项在Linker选项里面，直接删除就可以了： 
![这里写图片描述](https://img-blog.csdn.net/20180706233643486?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



