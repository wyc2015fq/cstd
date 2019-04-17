# vs2013编译ffmpeg之三十九 搭建ffmpeg+vs2013调试环境 - 枫林港的博客 - CSDN博客





2018年07月12日 23:43:03[枫林港](https://me.csdn.net/u013601132)阅读数：399








# 建立msvc工程


- [建立msvc工程](#建立msvc工程)- [建立工程](#建立工程)
- [工程设置](#工程设置)- [头文件路径](#头文件路径)
- [Mtd](#mtd)
- [StdAfx.h](#stdafxh)
- [宏定义](#宏定义)
- [库文件路径](#库文件路径)
- [库文件](#库文件)
- [struct pollfd](#struct-pollfd)
- [运行所需的dll](#运行所需的dll)

- [vs2013自定义宏](#vs2013自定义宏)
- [删除vs2013后ffmpeg跑不起来](#删除vs2013后ffmpeg跑不起来)




## 建立工程

先用msys+cl编译出ffmpeg的库，然后建立ffplay、ffmpeg工程，再编译出ffplay.exe、ffmpeg.exe，这样方便调试。

工程都建在msvc目录下。比如ffplay，建立空的console工程，将cmdutils.c、cmdutils_opencl.c、ffplay.c加到工程的Source Files下面。 
![这里写图片描述](https://img-blog.csdn.net/20180707004644605?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
将msys+cl编译ffmpeg的打印保存下来，查看里面链接生成ffplay.exe时的打印，看里面有那些.o被链进来了，那么这些.o对应的.c就需要加到工程里面，这些.c文件一般都在ffmpeg根目录下。

## 工程设置

### 头文件路径

第三方模块的头文件都统一用../../../../contribute/MSVC来做前缀，ffmpeg依赖的库的源代码都放contribute/MSVC下面。如C/C++ –> General –> Additional Include Directories里面需要加上sdl头文件路径，以及其他额外的头文件。

> 
../../inc

../../../../contribute/MSVC/SDL-1.2.13/include

../../../../contribute/MSVC/CUDA_NVIDIA/include


当前目录是工程文件xxx.vcxproj所在的目录。目前ffmpeg的VS2013工程属性里面的路径设置都是相对路径，工程建立好了后，可以直接在不同ffmpeg版本之间拷贝工程文件即msvc目录。

### Mtd

Project properties->Configuration properties->C/C++->Code generation->Runtime Library要设置成`Mtd`。

### StdAfx.h

> 
ffplay.c(3675): fatal error C1010: unexpected end of file while looking for precompiled header. Did you forget to add ‘#include “StdAfx.h”’ to your source?


C/C++ –> Precompiled Headers，选择`Not Using Precompiled headers`，就可以解决这个错误。

### 宏定义

C/C++ –>Preprocessor–>Preprocessor Definitions里面加上下面的宏定义，这些都是从msys+cl编译ffmpeg的编译打印里面弄过来的：

> 
_ISOC99_SOURCE

_FILE_OFFSET_BITS=64

_LARGEFILE_SOURCE

__STRICT_ANSI__

_USE_MATH_DEFINES

inline=__inline

strtoll=_strtoi64

strtod=avpriv_strtod


### 库文件路径

Linker->General->Additional Library Directories里面加上库路径：

> 
../../../vs2013_build/lib

../../../extra_lib

../../../../contribute/MSVC/SDL2-2.0.6/VisualC/Win32/Debug

../../../../contribute/MSVC/faac-1.28/libfaac/Debug

../../../../contribute/MSVC/lame-3.99.5/output/Debug

../../../../contribute/MSVC/x264-master/SMP/build_result/lib/x86

../../../../contribute/MSVC/libtheora-1.2.0alpha1/win32/VS2010/Win32/Debug

../../../../contribute/MSVC/openjpeg-1.5.1/bin/Debug

../../../../contribute/MSVC/libogg-1.3.1/win32/VS2010/Win32/Debug

../../../../contribute/MSVC/libvorbis-1.3.4/win32/VS2010/Win32/Debug

../../../../contribute/MSVC/xvidcore/build/win32/bin

../../../../contribute/MSVC/opus-1.1/win32/VS2010/Win32/Debug

../../../../contribute/MSVC/libvpx-v1.3.0/Win32/Debug

../../../../contribute/MSVC/openal-soft-1.15.1/Debug

../../../../contribute/MSVC/bzip2-1.0.6/Debug

../../../../contribute/MSVC/libass-master_0.13.7_20170609/libass-master/vs2013_build/lib/x86

../../../../contribute/MSVC/libwebp-0.5.1/output/debug-static/x86/lib

../../../../contribute/MSVC/x265_20160531/x265/build/vc13-x86/Debug

../../../../contribute/MSVC/zvbi-0.2.35_MSVC/msvc/zvbi-0.2.35_MSVC/Debug

../../../../contribute/MSVC/freetype-2.5.3/objs/win32/vc2010

../../../../contribute/MSVC/fribidi-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/fontconfig-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/gnutls-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/game-music-emu-master/vs2013_build/lib/x86

../../../../contribute/MSVC/modplug-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/zlib-1.2.8/contrib/vstudio/vc11/x86/ZlibStatDebug

../../../../contribute/MSVC/mfx_dispatch-master_org/vs2013_build/lib/x86

../../../../contribute/MSVC/libilbc-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/soxr-0.1.2-Source/msvc/Debug

../../../../contribute/MSVC/gmp-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/libcdio-paranoia-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/libcdio-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/gsm-1.0-pl13/lib

../../../../contribute/MSVC/opencore-amr-0.1.3/msys_build/lib

../../../../contribute/MSVC/speex-1.2rc2/win32/VS2013/Debug

../../../../contribute/MSVC/libbluray-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/libssh-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/libgcrypt-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/libxml2-2.9.1/lib

../../../../contribute/MSVC/rtmpdump-2.3/vs2013_build

../../../../contribute/MSVC/openssl-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/libbs2b-3.1.0/msys_build/lib

../../../../contribute/MSVC/libcaca-0.99.beta19/win32/Debug/Win32

../../../../contribute/MSVC/openh264-1.5.0/bin/Win32/Debug

../../../../contribute/MSVC/fdk-aac-master_20160630/vs2013_build/lib/x86

../../../../contribute/MSVC/snappy_20160715/snappy/vs2013_build/lib

../../../../contribute/MSVC/schroedinger-1.0.11/msys_build/lib

../../../../contribute/MSVC/shine-3.1.0/msys_build/lib

../../../../contribute/MSVC/WavPack_sources/Debug

../../../../contribute/MSVC/xavs-code_20160829/build/win32/bin

../../../../contribute/MSVC/orc-0.4.25/msys_build/lib

../../../../contribute/MSVC/opencv/build/x86/vc12/staticlib

../../../../contribute/MSVC/vid.stab-master/Debug

../../../../contribute/MSVC/zimg-release-2.2.1/msys_build/lib

../../../../contribute/MSVC/CUDA_NVIDIA/lib/Win32

../../../../contribute/MSVC/twolame-0.3.13/win32/lib

../../../../contribute/MSVC/xz-5.2.2/windows/Debug/Win32/liblzma

../../../../contribute/MSVC/tesseract-3.04.01/vs2013_build/DLL_Debug

../../../../contribute/MSVC/leptonica-1.73/vs2013_build/bin/Debug

../../../../contribute/MSVC/vo-amrwbenc-0.1.3/msys_build/lib

../../../../contribute/MSVC/chromaprint-1.3.2/src/Debug

../../../../contribute/MSVC/libiconv-1.9.2/lib

../../../../contribute/MSVC/libpng-1.6.34\projects\vstudio\Debug


### 库文件

由于用到的库非常多，所以库的路径也非常多，要将所有的库路径都包含进来，否则后面找不到相应的库。Linker->Input->Additional Dependencies里面：

> 
libavdevice.a

libavformat.a

libavcodec.a

libpostproc.a

libavresample.a

libswresample.a

libavfilter.a

libswscale.a

libavutil.a

SDL2.lib

libfdk-aacd.lib

OpenAL32.lib

zvbi.lib

iconv.lib

Advapi32.lib

xvidcore.lib

x265-static.lib

leptonica.lib

libx264d.lib

libwebp_debug.lib

libwebpmux_debug.lib

vpxmdd.lib

libvorbis_static.lib

libogg_static.lib

libtheora_static.lib

opus.lib

celt.lib

silk_common.lib

silk_float.lib

openjpeg.lib

libmp3lame-static.lib

libmpghip-static.lib

freetype253_D.lib

libassd.lib

libfribidid.lib

libfontconfigd.lib

libbz2.lib

libgnutlsd.lib

libgmed.lib

libmodplugd.lib

zlibstat.lib

libmfx.lib

libilbcd.lib

libsoxr.lib

libgmpd.lib

libcdio_paranoiad.lib

libcdiod.lib

libgsm.a

libopencore-amrnb.a

libopencore-amrwb.a

libspeex.lib

libblurayd.lib

libsshd.lib

libgcryptd.lib

librtmp.lib

libssld.lib

libcryptod.lib

libbs2b.a

libcaca.lib

liborc-0.4.a

libschroedinger-1.0.a

libshine.a

libvo-amrwbenc.a

libwavpack.lib

libzimg.dll.a

lzma.lib

OpenCL.lib

opencv_core2413d.lib

opencv_imgproc2413d.lib

snappy.lib

tesseract.lib

twolame.lib

xavs.lib

xml2.lib

vidstab.lib

welsecore.lib

welsdcore.lib

WelsDecPlus.lib

WelsEncPlus.lib

WelsVP.lib

chromaprint.lib

mingwex.lib

psapi.lib

gcc.lib

strmiids.lib

uuid.lib

ws2_32.lib

vfw32.lib

dxguid.lib

Secur32.lib

Shlwapi.lib

WinMM.lib

User32.lib

ole32.lib

intl.lib

pthread.lib

m.lib

OpenGL32.Lib

libpng16.lib


### struct pollfd

ffmpeg-3.3\libavformat\os_support.h里面的struct pollfd及其后面的宏定义，都要用#ifndef POLLRDNORM括起来。这些宏在winsock2.h里面有定义。即改成下面的形式：

```
#ifndef POLLRDNORM

struct pollfd {

    int fd;

    short events;  /* events to look for */

    short revents; /* events that occurred */

};



/* events & revents */

#define POLLIN     0x0001  /* any readable data available */

#define POLLOUT    0x0002  /* file descriptor is writeable */

#define POLLRDNORM POLLIN

#define POLLWRNORM POLLOUT

#define POLLRDBAND 0x0008  /* priority readable data */

#define POLLWRBAND 0x0010  /* priority data can be written */

#define POLLPRI    0x0020  /* high priority readable data */



/* revents only */

#define POLLERR    0x0004  /* errors pending */

#define POLLHUP    0x0080  /* disconnected */

#define POLLNVAL   0x1000  /* invalid file descriptor */

#endif // WIN32 ]

#endif
```

### 运行所需的dll

由于一些第三方代码编译结果是dll，需要将这些dll拷贝到可执行文件如ffplayer.exe同级目录下（ffmpeg-3.3\msvc\MSVC_ffmpeg-3.0\Debug），目前所需的dll如下：

OpenAL32.dll  libfontconfig-1.dll  libglib-2.0-0.dll  libpng16.dll     libzimg-2.dll 

SDL2.dll      libfreetype-6.dll    libiconv-2.dll     libstdc++-6.dll  postproc-54.dll 

intl.dll      libgcc_s_dw2-1.dll   liborc-0.4-0.dll   libxml2-2.dll    pthreadGC2.dll
## vs2013自定义宏

可以利用vs的自定义宏来做路径前缀替换，避免目录路径换了后要修改每个头文件目录的路径或库文件路径。

1、VIEW->Property Manager->在Debug或Release右键添加新项目属性表，或者在View->Other Windows->Property Manage里面，如下图所示的”Add New Project Project Property Sheet”： 
![这里写图片描述](https://img-blog.csdn.net/20180707004835121?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在弹出的界面里面填写props的名字PropertySheet_myMacros.props，路径可以自己选择，默认和当前工程的.vcxproj放在同一个目录下。

2、双击PropertySheet_myMacros.props，在弹出的界面里面点击User Macros，再点”Add Macro”按钮就可以添加了。 
![这里写图片描述](https://img-blog.csdn.net/20180707004849792?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
比如定义了下面的宏： 
![这里写图片描述](https://img-blog.csdn.net/20180707005119587?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

那么头文件路径：
```bash
../../../../contribute/MSVC/SDL2-2.0.6/VisualC/Win32/Debug
```

可以改成：

```bash
$(ThreethPart_Path)/SDL2-2.0.6/VisualC/Win32/Debug
```

3、其他工程直接在其上右键->Add Existing Property Sheet…，然后选择前面创建的PropertySheet_myMacros.props。 
![这里写图片描述](https://img-blog.csdn.net/20180707004911209?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 删除vs2013后ffmpeg跑不起来

电脑上删除VS2013后ffmpeg跑不起来了，需要msvcr120d.dll、vcomp120d.dll，都从安装了VS2013的电脑上拷贝就可以，文件在C:\Windows\SysWOW64下面。 
![这里写图片描述](https://img-blog.csdn.net/2018070700520434?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

“无法定位程序输入点orc_program_add_constant于动态链接库libzimg-2.dll上。”。在另外一台安装了VS2013的电脑上却没有这个问题，另外一台电脑上链接ffmpeg.exe时加-VERBOSE，打印里面也没有orc_program_add_constant符号。直接从没有问题的电脑上拷贝ffmpeg.exe就没有问题了。









