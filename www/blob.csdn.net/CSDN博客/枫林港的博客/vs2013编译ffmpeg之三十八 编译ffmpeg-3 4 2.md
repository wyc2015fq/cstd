# vs2013编译ffmpeg之三十八 编译ffmpeg-3.4.2 - 枫林港的博客 - CSDN博客





2018年07月12日 23:40:39[枫林港](https://me.csdn.net/u013601132)阅读数：479








# 编译ffmpeg-3.4.2


- [编译ffmpeg-3.4.2](#编译ffmpeg-342)- [1、libvorbis](#1libvorbis)
- [2、openal](#2openal)
- [3、openssl](#3openssl)
- [4、libcdio](#4libcdio)
- [5、sdl2](#5sdl2)
- [6、DeckLink](#6decklink)- [编译错误一](#编译错误一)
- [编译错误二](#编译错误二)
- [编译错误三](#编译错误三)
- [编译错误四](#编译错误四)





ffmpeg-3.4.2增加了几个目录：

**ffbuild**：configure产生的文件放在这个目录下。

**fftools**：产生可执行文件如ffmpeg.exe/ffplay.exe等的c文件放在这个目录下。

一些特性的变化：

1、去掉了对libschroedinger的支持。

2、增加了–enabled-libxml2支持，configure的时候需要libxml-2.0.pc，同时需要保留libxml2.pc，二者内容一样。configure在检测libass的时候需要用到libxml2.pc。

3、–enable-cuda改成了–enable-cuda_sdk

4、yasm换成了x86asm

 ffmpeg的configure请参考[这里](https://blog.csdn.net/u013601132/article/details/80822596)，下面描述的是ffmpeg-3.4.2与3.3不一样的地方。

## 1、libvorbis

ffmpeg-3.4.2 configure的时候有下面的错误：

> 
No package ‘vorbis’ found

ERROR: vorbis not found using pkg-config


ffmpeg-3.3没有此问题，ffmpeg-3.4.2的enabled libvorbis规则不一样：

```
enabled libvorbis         && require_pkg_config libvorbis vorbis vorbis/codec.h vorbis_info_init &&

                             require_pkg_config libvorbisenc vorbisenc vorbis/vorbisenc.h vorbis_encode_init
```

根据以上规则，需要建立两个.pc文件，vorbisenc.pc、vorbis.pc，放在ffmpeg-3.4.2/pc_file目录下，两个内容一样：

```
# prefix=../../contribute/MSVC/libvorbis-1.3.4

exec_prefix=${prefix}

libdir=${exec_prefix}/win32/VS2010/Win32/Debug

includedir=${prefix}/include



Name: libvorbis

Description: libvorbis-1.3.4

Version: libvorbis-1.3.4

Requires:

Conflicts:

Libs: -L${libdir}  libvorbis_static.lib libogg_static.lib

Cflags: -I${includedir} libvorbis pkg-config source file
```

这样就可以检测通过了。

## 2、openal

ffmpeg-3.4.2 configure的时候，出现下面的错误：

```
OpenAL32.lib(winmm.obj) : error LNK2019: 无法解析的外部符号 impwaveOutGetNumDevs@0，该符号在函数 _ProbePlaybackDevices 中被引用

OpenAL32.lib(winmm.obj) : error LNK2019: 无法解析的外部符号 impwaveOutGetDevCapsA@12，该符号在函数 _ProbePlaybackDevices 中被引用

OpenAL32.lib(winmm.obj) : error LNK2019: 无法解析的外部符号 impwaveOutOpen@24，该符号在函数 _CaptureThreadProc@4 中被引用

OpenAL32.lib(winmm.obj) : error LNK2019: 无法解析的外部符号 impwaveOutClose@4，该符号在函数 _CaptureThreadProc@4 中被引用

OpenAL32.lib(winmm.obj) : error LNK2019: 无法解析的外部符号 impwaveOutPrepareHeader@12，该符号在函数 _CaptureThreadProc@4 中被引用
```

老办法，查看config.log，

```
check_lib openal AL/al.h alGetError -lOpenAL32
check_func_headers AL/al.h alGetError -lOpenAL32
check_ld cc -lOpenAL32
check_cc
BEGIN ./ffconf.NDsShiXx/test.c
    1   #include <AL/al.h>
    2   #include <stdint.h>
    3   long check_alGetError(void) { return (long) alGetError; }
    4   int main(void) { int ret = 0;
    5    ret |= ((intptr_t)check_alGetError) & 0xFFFF;
    6   return ret; }
END ./ffconf.NDsShiXx/test.c
cl ... -c -Fo./ffconf.NDsShiXx/test.o ./ffconf.NDsShiXx/test.c
test.c
./compat/windows/mslink /NODEFAULTLIB:LIBCMT ...
   正在创建库 ./ffconf.NDsShiXx/test.lib 和对象 ./ffconf.NDsShiXx/test.exp
OpenAL32.lib(mmdevapi.obj) : error LNK2019: 无法解析的外部符号 __imp__GetMessageA@16，该符号在函数 _MMDevApiMsgProc@4 中被引用
OpenAL32.lib(winmm.obj) : error LNK2001: 无法解析的外部符号 __imp__GetMessageA@16
OpenAL32.lib(mmdevapi.obj) : error LNK2019: 无法解析的外部符号 __imp__PostThreadMessageA@16，该符号在函数 _alcMMDevApiDeinit 中被引用
OpenAL32.lib(winmm.obj) : error LNK2001: 无法解析的外部符号 __imp__PostThreadMessageA@16
OpenAL32.lib(dsound.obj) : error LNK2019: 无法解析的外部符号 __imp__GetForegroundWindow@0，该符号在函数 _DSoundPlaybackProc 中被引用
OpenAL32.lib(winmm.obj) : error LNK2019: 无法解析的外部符号 __imp__waveOutGetNumDevs@0，该符号在函数 _ProbePlaybackDevices 中被引用
OpenAL32.lib(winmm.obj) : error LNK2019: 无法解析的外部符号 __imp__waveOutGetDevCapsA@12，该符号在函数 _ProbePlaybackDevices 中被引用
```

GetMessageA在user32.lib库里面（这个以前遇到过）。 waveOutGetNumDevs在Winmm.lib里面（网上搜索到的）。

最终在configure里面修改enabled openal的规则，在check_lib里面加上`-lWinMM -lUser32`：

```bash
enabled openal            && { { for al_extralibs in "${OPENAL_LIBS}" "-lopenal" "-lOpenAL32"; do

                               check_lib openal 'AL/al.h' alGetError "${al_extralibs}" && break; done } ||

                               die "ERROR: openal not found"; } &&

                             { check_cpp_condition "AL/al.h" "defined(AL_VERSION_1_1)" ||

                               die "ERROR: openal must be installed and version must be 1.1 or compatible"; }
```

改成：

```bash
enabled openal            && { { for al_extralibs in "${OPENAL_LIBS}" "-lopenal" "-lOpenAL32"; do

                               check_lib openal 'AL/al.h' alGetError "${al_extralibs}" -lWinMM -lUser32 && break; done } ||

                               die "ERROR: openal not found"; } &&

                             { check_cpp_condition "AL/al.h" "defined(AL_VERSION_1_1)" ||

                               die "ERROR: openal must be installed and version must be 1.1 or compatible"; }
```

## 3、openssl

ffmpeg-3.4.2检测openssl的时候遇到下面的错误，config.log最后：

> 
./ffconf.hsMhykWm/test.c(1) : fatal error C1083: 无法打开包括文件: “openssl/ssl.h”: No such file or directory

ERROR: openssl not found


分析如下：

**检测规则**

ffmpeg-3.4.2 configure里面enabled openssl的规则如下：

```
enabled openssl           && { use_pkg_config openssl openssl openssl/ssl.h OPENSSL_init_ssl ||

                               use_pkg_config openssl openssl openssl/ssl.h SSL_library_init ||

                               check_lib openssl openssl/ssl.h SSL_library_init libssld.lib libcryptod.lib ||

                               check_lib openssl openssl/ssl.h SSL_library_init -lssl32 -leay32 ||

                               check_lib openssl openssl/ssl.h SSL_library_init libssld.lib libcryptod.lib -lws2_32 -lgdi32 ||

                               die "ERROR: openssl not found"; }
```

**检测方法**

5条检测规则，前面的失败了则继续检测下一条，这样可以适配不同版本的openssl。

use_pkg_config是用pkg-config命令分析openssl.pc，如第一条规则所示，最终要找到头文件openssl/ssl.h，以及符号OPENSSL_init_ssl。

check_lib没有使用pkg-config命令，直接依赖默认头文件/库文件路径，以及configure执行时所带的头文件/库文件路径，如第三条规则所示，检测头文件openssl/ssl.h、符号 SSL_library_init、库文件libssld.lib libcryptod.lib。

用的是openssl.pc，所用必须在前面两条规则里面检测成功。搜了一下openssl的代码，里面没有OPENSSL_init_ssl，但有SSL_library_init。所以必须在第二条规则这里检测成功。

config.log最后的打印具有迷惑性（无法打开包括文件: “openssl/ssl.h”），真正的错误在第二条检测规则这里，要搞清楚第二条检测规则为何失败了。再看config.log里面第二条规则对应的错误：

> 
libssld.lib(ssl_algs.obj) : warning LNK4075: 忽略“/EDITANDCONTINUE”(由于“/OPT:ICF”规范)

libcryptod.lib(bss_sock.obj) : error LNK2019: 无法解析的外部符号 __imp__closesocket@4，该符号在函数 _sock_free 中被引用

libcryptod.lib(bss_sock.obj) : error LNK2019: 无法解析的外部符号 __imp__recv@16，该符号在函数 _sock_read 中被引用

libcryptod.lib(bss_sock.obj) : error LNK2019: 无法解析的外部符号 __imp__send@16，该符号在函数 _sock_write 中被引用

libcryptod.lib(bss_sock.obj) : error LNK2019: 无法解析的外部符号 __imp__shutdown@8，该符号在函数 _sock_free 中被引用

libcryptod.lib(bss_sock.obj) : error LNK2019: 无法解析的外部符号 __imp__WSASetLastError@4，该符号在函数 _sock_write 中被引用

libcryptod.lib(bss_sock.obj) : error LNK2019: 无法解析的外部符号 __imp__WSAGetLastError@0，该符号在函数 _BIO_sock_should_retry 中被引用

./ffconf.hsMhykWm/test.exe : fatal error LNK1120: 6 个无法解析的外部命令


网上搜了一下，closesocket在ws2_32.lib库中，直接修改openssl.pc文件，在“Libs: ”规则最后加上ws2_32.lib就可以解决这个问题。

```
#openssl.pc



prefix=../../contribute/MSVC/openssl-master_20160630

exec_prefix=${prefix}

libdir=${exec_prefix}/vs2013_build/lib/x86

includedir=${prefix}/vs2013_build/include



Name: GNU openssl

Description: openssl

#Requires: 

Version: 1.0.2h

Libs: -L${libdir} libssld.lib libcryptod.lib user32.lib Gdi32.lib ws2_32.lib

Cflags: -I${includedir}
```

## 4、libcdio

ffmpeg-3.4.2 configure的时候报下面的错误：

> 
libcdio_paranoiad.lib(interface.obj) : warning LNK4075: 忽略“/EDITANDCONTINUE”(由于“/OPT:ICF”规范)

libcdio_paranoiad.lib(utf8.obj) : error LNK2019: 无法解析的外部符号 _libiconv_open，该符号在函数 _cdio_charset_converter_create 中被引用

libcdio_paranoiad.lib(utf8.obj) : error LNK2019: 无法解析的外部符号 _libiconv，该符号在函数 _do_convert 中被引用

libcdio_paranoiad.lib(utf8.obj) : error LNK2019: 无法解析的外部符号 _libiconv_close，该符号在函数 _cdio_charset_converter_destroy 中被引用

./ffconf.sTbrGWaS/test.exe : fatal error LNK1120: 3 个无法解析的外部命令


这三个符号在libiconv里面，直接修改configure，加上`iconv.lib`：

```
if enabled libcdio; then

    check_lib libcdio "cdio/cdda.h cdio/paranoia.h" cdio_cddap_open -lcdio_paranoiad -lcdiod ||

    check_lib libcdio "cdio/paranoia/cdda.h cdio/paranoia/paranoia.h" cdio_cddap_open libcdio_paranoiad.lib  libcdiod.lib ||

    die "ERROR: No usable libcdio/cdparanoia found"

fi
```

改成：

```
if enabled libcdio; then

    check_lib libcdio "cdio/cdda.h cdio/paranoia.h" cdio_cddap_open -lcdio_paranoiad -lcdiod ||

    check_lib libcdio "cdio/paranoia/cdda.h cdio/paranoia/paranoia.h" cdio_cddap_open libcdio_paranoiad.lib  libcdiod.lib iconv.lib ||

    die "ERROR: No usable libcdio/cdparanoia found"

fi
```

iconv.lib所在的路径已经包含在configure命令的参数里面了。

ffmpeg-3.3也是同样的enabled libcdio规则，但不知道为何没有问题。

## 5、sdl2

编译ffmpeg-3.4.2 的时候，sdl版本为2.0.6，从[http://www.libsdl.org/release/](http://www.libsdl.org/release/)选择2.0.6下载，解压后，用VS2013打开SDL2-2.0.6\VisualC\SDL.sln，编译没有报错，编译结果为SDL2-2.0.6\VisualC\Win32\Debug\SDL2.dll。

ffmpeg-3.4.2configure的时候需要加–enable-sdl2，不然configure执行完后CONFIG_SDL2不会为1，ffplay也就不会编译。–enable-sdl在configure参数里面也加了。

ffmpeg-3.4.2 configure的时候，sdl2检测报错：

> 
check_host_cc

BEGIN ./ffconf.wIHBjxlZ.c

​    1    static inline int foo(int a) { return a; }

END ./ffconf.wIHBjxlZ.c

cl -nologo -W4 -wd4244 -wd4127 -wd4018 -wd4389 -wd4146 -wd4057 -wd4204 -wd4706 -wd4305 -wd4152 -wd4324 -we4013 -wd4100 -wd4214 -wd4307 -wd4273 -wd4554 -wd4701 -O3 -c -Fo./ffconf.jwLEKorz.o ./ffconf.wIHBjxlZ.c

cl: 命令行 warning D9002 :忽略未知选项“-O3”

ffconf.wIHBjxlZ.c

./ffconf.wIHBjxlZ.c(1) : warning C4431: 缺少类型说明符 - 假定为 int。注意:  C 不再支持默认 int

./ffconf.wIHBjxlZ.c(1) : error C2054: 在“inline”之后应输入“(”

./ffconf.wIHBjxlZ.c(1) : error C2085: “foo”: 不在形参表中

./ffconf.wIHBjxlZ.c(1) : error C2143: 语法错误 : 缺少“;”(在“{”的前面)

ERROR: sdl2 requested but not found


上面的错误是config.log里面最后的部分。前面这个inline的错误其实可以不用管，很具有迷惑性。真正的错误在最后一行: “ ERROR: sdl2 requested but not found”。搜索config.log里面的sdl2的部分，有打印：

> 
Requested ‘sdl2 >= 2.0.1’ but version of sdl2 is SDL2-devel-2.0.6-VC


版本没有匹配上。ffmpeg-3.4.2 sdl2的检测语句比ffmpeg-3.3的多了版本检测。解决的办法是修改sdl2.pc文件里面的“Version”字段：
`Version: SDL2-devel-2.0.6-VC`
改成：
`Version: 2.0.6`
sdl2.pc：

```
# sdl pkg-config source file

prefix=../../contribute/MSVC/SDL2-2.0.6
exec_prefix=${prefix}
libdir=${exec_prefix}/VisualC/Win32/Debug
includedir=${prefix}/include

Name: sdl2
Description: Simple DirectMedia Layer is a cross-platform multimedia library designed to provide low level access to audio, keyboard, mouse, joystick, 3D hardware via OpenGL, and 2D video framebuffer.
Version: 2.0.6
Requires:
Conflicts:
Libs: -L${libdir}  SDL2.lib SDL2main.lib
Cflags: -I${includedir}
```

## 6、DeckLink

电脑上没有NVIDIA显卡，这个功能最终没有打开。

ffmpeg-3.4.2里面将–enable-cuda改成了–enable-cuda_sdk

ffmpeg-3.3在编译的时候没有遇到下面的问题，是因为ffmpeg-3.3在configure后，CONFIG_DECKLINK_INDEV和CONFIG_DECKLINK_OUTDEV是没有定义的。而ffmpeg-3.4.2里面这两个宏是有定义的。–enable-cuda改成了–enable-cuda_sdk 

### 编译错误一

> 
E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\MSVC_ffmpeg\ffmpeg-3.4.2\compat/w32pthreads.h(52) : error C2371: “pthread_t”: 重定义；不同的基类型

​        E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\MSVC_ffmpeg\ffmpeg-3.4.2\inc\pthread.h(571) : 参见“pthread_t”的声明


参考[这里的解决办法](http://sw.blackmagicdesign.com/DesktopVideo/v10.9.11/Blackmagic_Desktop_Video_Windows_10.9.11.zip?Key-Pair-Id=APKAJTKA3ZJMJRQITVEA&Signature=aOArKm8i4kPDh5d2aX8oaVtia5qaOkSErV94MFY9zHzze11QWqPKeeuogVwI035jGezRTA%20l%20CyTnpEPm7hCWecKmBXFc1p0Wx8feQLqOD3pBdr85BfFaVwdM3hcWoCnKPjcH11eKaZ%206A1rZqPNV4fQ63toSgJbE7QgSUoyKu9A1KdgNsqSNdBYkEjGp4TkqwbjRoSkhEZcLmQ4FxpuDs2D37n3Dea8RN2m3q7wwrRnXccVeL%205jRoRUbOKHranxwSzxXhpFncyH4ur%20luoovye9t%20yu9y6GW8vvh%20xcRu6YsoYITvlapJXZNiNN/ZmlNPfe3P9XK2PJJqauMuztQ==&Expires=1522743646)。

这个是因为decklink_dec.cpp里面#include 

```
#if defined(_VC_CRT_MAJOR_VERSION) && _VC_CRT_MAJOR_VERSION < 14

#pragma comment(linker, "/include:" EXTERN_PREFIX "avpriv_strtod")

#pragma comment(linker, "/include:" EXTERN_PREFIX "avpriv_snprintf")

#endif
```

这两个#pragma comment(linker,）语句，表示在链接的时候要链接avpriv_strtod和avpriv_snprintf两个符号。

在configure检测libzvbi的时候，会检测libzvbi.h，即用require函数检测vbi_decoder_new符号的时候会include libzvbi.h，所以在检测的时候也会包含这两个#pragma comment语句。但avpriv_strtod在compat/strtod.c里面，avpriv_snprintf在compat/msvcrt/snprintf.c里面。configure的时候没法去链接这两个.c里面的符号，因为此时ffmpeg还没有编译，这两个.c文件对应的二进制文件还没有编译出来。

解决的办法，在libzvbi.h中将：
`#include "pthread.h"`
改成：

```
#ifdef CONFIGURE_STAGE

#include "pthread.h"

#else

#include <compat/w32pthreads.h>

#endif
```

同时修改configure中的libzvbi的检测规则：
`enabled libzvbi           && require libzvbi libzvbi.h vbi_decoder_new -lzvbi -lintl -lpthread -lm iconv.lib -lAdvapi32 &&`
改成：
`enabled libzvbi           && require libzvbi libzvbi.h vbi_decoder_new -lzvbi -lintl -lpthread -lm iconv.lib -lAdvapi32 -DCONFIGURE_STAGE &&`
即在configure和编译的时候使用不同的头文件，这个不会影响编译结果，CONFIGURE_STAGE只会在检测的时候用到，编译的时候不会携带该宏定义。

### 编译错误二

decklink_dec.cpp

> 
e:\mingw\msys\1.0\home\AA\ffmpeg_msvc\msvc_ffmpeg\ffmpeg-3.4.2\libavutil\rational.h : warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失

libavdevice/decklink_dec.cpp(532) : error C2695: “decklink_input_callback::VideoInputFormatChanged”:  重写虚函数与“IDeckLinkInputCallback::VideoInputFormatChanged”只是在调用约定上不同

​        E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\decklink\include\DeckLinkAPI.h(576) : 参见“IDeckLinkInputCallback::VideoInputFormatChanged”的声明

libavdevice/decklink_dec.cpp(533) : error C2695: “decklink_input_callback::VideoInputFrameArrived”:  重写虚函数与“IDeckLinkInputCallback::VideoInputFrameArrived”只是在调用约定上不同

​        E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\decklink\include\DeckLinkAPI.h(577) : 参见“IDeckLinkInputCallback::VideoInputFrameArrived”的声明


去掉这两个函数声明前面的`STDMETHODCALLTYPE`。

decklink_enc.cpp也有类似的问题，解决办法一样。

### 编译错误三

decklink_dec.cpp

> 
libavdevice/decklink_dec.cpp(894) : error C2065: “IID_IDeckLinkInput”: 未声明的标识符


在decklink_dec.cpp文件里面加上：

```
#ifdef _WIN32

#include <DeckLinkAPI_i.c>

#endif
```

decklink_enc.cpp也有类似的问题，解决办法一样。

### 编译错误四

decklink_enc.cpp

> 
C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\INCLUDE\xkeycheck.h(250) : fatal error C1189: #error :  The C++ Standard Library forbids macroizing keywords. Enable warning C4005 to find the forbidden macro.


在configure的参数里面加上-D_XKEYCHECK_H，并重新执行configure。

但最后还是用不起来，因为电脑上没有NVIDIA显卡，ffmpeg.exe运行的时候报缺nvcuda.dll。网上下载的nvcuda.dll，有函数在nvcuda.dll里面没有，也跑不起来。最后只能在configure参数里面去掉–enable-cuda_sdk。



