# vs2013编译ffmpeg之二十五 Openssl、rtmpdump - 枫林港的博客 - CSDN博客





2018年07月09日 23:34:53[枫林港](https://me.csdn.net/u013601132)阅读数：281








# Openssl

对应ffmpeg configure选项–enable-openssl。

Openssl是从[https://github.com/ShiftMediaProject](https://github.com/ShiftMediaProject)上下载的。

ffmpeg-3.3在configure的时候遇到下面这个错误：

```
libcryptod.lib(cryptlib.obj) : error LNK2019: 无法解析的外部符号 impGetProcessWindowStation@0，该符号在函数 _OPENSSL_isservice 中被引用

libcryptod.lib(cryptlib.obj) : error LNK2019: 无法解析的外部符号 impGetUserObjectInformationW@20，该符号在函数 _OPENSSL_isservice 中被引用

libcryptod.lib(cryptlib.obj) : error LNK2019: 无法解析的外部符号 impMessageBoxW@16，该符号在函数 _OPENSSL_showfatal中被引用

libcryptod.lib(rand_win.obj) : error LNK2019: 无法解析的外部符号 impCreateCompatibleBitmap@12，该符号在函数 _readscreen 中被引用

libcryptod.lib(rand_win.obj) : error LNK2019: 无法解析的外部符号 impDeleteObject@4，该符号在函数 _readscreen 中被引用

libcryptod.lib(rand_win.obj) : error LNK2019: 无法解析的外部符号 impGetDeviceCaps@8，该符号在函数 _readscreen 中被引用

libcryptod.lib(rand_win.obj) : error LNK2019: 无法解析的外部符号 impGetDIBits@28，该符号在函数 _readscreen 中被引用

libcryptod.lib(rand_win.obj) : error LNK2019: 无法解析的外部符号 impGetObjectW@12，该符号在函数 _readscreen 中被引用

libcryptod.lib(rand_win.obj) : error LNK2019: 无法解析的外部符号 impGetDC@4，该符号在函数 _readscreen 中被引用

libcryptod.lib(rand_win.obj) : error LNK2019: 无法解析的外部符号 impReleaseDC@8，该符号在函数 _readscreen 中被引用
```

这些符号在user32.lib Gdi32.lib里面，将这两个库加到openssl.pc的Libs规则里面。

# rtmpdump

对应ffmpeg configure选项–enable-librtmp。

下载rtmpdump-2.3.tgz，其源代码里面没有MSVC工程，由于其依赖openssl，本来想用msys编译rtmpdump，并链接vs2013编译出来的openssl库，但发现二者链到一起比较困难，缺了很多符号。所以直接建立rtmp的MSVC工程。

## 建立librtmp的工程

先建立librtmp的工程，选静态库，将librtmp目录下的5个文件：amf.c、hashswf.c、log.c、parseurl.c、rtmp.c加到工程里面。

rtmp_sys.h里面，将

```
#ifdef _XBOX

#include <xtl.h>

#include <winsockx.h>

#define snprintf _snprintf

#define strcasecmp stricmp

#define strncasecmp strnicmp

#define vsnprintf _vsnprintf



#else /* !_XBOX */

#include <winsock2.h>

#include <ws2tcpip.h>

#endif
```

改成：

```
#ifdef _XBOX

#include <xtl.h>

#include <winsockx.h>

#define snprintf _snprintf

#define strcasecmp stricmp

#define strncasecmp strnicmp

#define vsnprintf _vsnprintf



#else /* !_XBOX */

#include <winsock2.h>

#include <ws2tcpip.h>

#define snprintf _snprintf

#define strcasecmp stricmp

#define strncasecmp strnicmp

#define vsnprintf _vsnprintf

#endif
```

代码默认是支持openssl的，工程里面加上openssl、zlib的头文件路径。将rtmp.c里面与netstackdump文件相关的代码都注释掉，避免后续编译ffmpeg的时候找不到符号。

## 建立rtmpdump的工程

然后建立rtmpdump工程，选console程序： 
![这里写图片描述](https://img-blog.csdn.net/20180706234748768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
加入根目录下的rtmpdump.c、thread.c。设置好头文件路径，宏定义里面加上RTMPDUMP_VERSION=”2.3”，不能定义成数字，不然编译会有问题，运行的时候rtmpdump也会直接退出。依赖的外部库文件里面加入：

```
libcryptod.lib

libssld.lib

librtmp.lib

zlibstat.lib

ws2_32.lib

mingwex.lib

gcc.lib
```

后面3个库是从ffmpeg里面取的，也就是MinGW+msys环境里面的。

## 建立其他工程

同理，建立rtmpgw、rtmpsrv、rtmpsuck等工程，rtmpdump工程里面的代码要编译成C++，不然改起来比较麻烦，如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20180706234802656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 编译错误

MSVC2013+ffmpeg-3.3，编译的时候出现下面的错误：

```
libavformat.a(allformats.o) : error LNK2001: 无法解析的外部符号 _ff_ffrtmpcrypt_protocol

libavformat.a(allformats.o) : error LNK2001: 无法解析的外部符号 _ff_ffrtmphttp_protocol

libavformat.a(allformats.o) : error LNK2001: 无法解析的外部符号 _ff_rtmp_protocol

libavformat.a(allformats.o) : error LNK2001: 无法解析的外部符号 _ff_rtmpe_protocol

libavformat.a(allformats.o) : error LNK2001: 无法解析的外部符号 _ff_rtmps_protocol

libavformat.a(allformats.o) : error LNK2001: 无法解析的外部符号 _ff_rtmpt_protocol

libavformat.a(allformats.o) : error LNK2001: 无法解析的外部符号 _ff_rtmpte_protocol

libavformat.a(allformats.o) : error LNK2001: 无法解析的外部符号 _ff_rtmpts_protocol
```

ffmpeg config.h里面的宏定义是正确的，已经加入了librtmp支持，前面没有加–enable-librtmp编译过ffmpeg。解决的办法是将libavformat下面的.d、.o都删除后重新编译。





