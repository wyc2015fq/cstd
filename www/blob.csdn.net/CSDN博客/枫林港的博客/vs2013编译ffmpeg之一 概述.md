# vs2013编译ffmpeg之一  概述 - 枫林港的博客 - CSDN博客





置顶2018年07月07日 22:37:45[枫林港](https://me.csdn.net/u013601132)阅读数：440










- [一、 编译环境](#一-编译环境)
- [二、目录结构](#二目录结构)
- [三、Configure命令](#三configure命令)- [configure参数说明](#configure参数说明)

- [四、Configure调试](#四configure调试)- [1、加打印的方法](#1加打印的方法)
- [2、条件执行](#2条件执行)
- [3、脚本打印行号](#3脚本打印行号)
- [4、跟踪脚本执行](#4跟踪脚本执行)
- [5、enable模块的检测函数](#5enable模块的检测函数)- [check_header/check_header_objcc](#checkheadercheckheaderobjcc)
- [check_lib/require](#checklibrequire)
- [require_pkg_config](#requirepkgconfig)
- [dumpbin](#dumpbin)

- [6、pkg-config](#6pkg-config)- [手动构造.pc文件](#手动构造pc文件)


- [五、yasm](#五yasm)
- [六、头文件路径](#六头文件路径)
- [七、遇到的错误](#七遇到的错误)- [1、LIBCMTD](#1libcmtd)
- [2、缺少snprintf](#2缺少snprintf)
- [3、缺少avpriv_snprintf](#3缺少avprivsnprintf)
- [4、缺少useconds_t](#4缺少usecondst)
- [5、__inline__](#5inline)
- [6、mingwex.lib](#6mingwexlib)

- [八、ShiftMediaProject](#八shiftmediaproject)




编译ffmpeg主要是出于兴趣，想研究一下ffmpeg和某些音视频codec格式。用Visual Studio编译主要是单步调试方便。虽然对gdb很熟悉，也经常用gdb调试，但gdb命令行或集成了gdb的IDE如codeblock、eclipse等，都远不如Visual Studio方便。gdb界面太不友好，而且需要对代码比较熟悉才好调试。用codeblock、eclipse调试ffmpeg遇到过很多问题，与IDE自身有关系。最终选择了Visual Studio，从使用效果来看，效率很高。
ffmpeg编译分为两步：

第一步：configure，主要的目的是产生config.mak，用于后面的编译； 

第二步：make，执行编译过程，产生可执行文件。如果enable的特性很少，编译则很简单。自己编译时enable的特性比较多，后面的描述会比较长。
有关编译ffmpeg依赖的第三方库，请参考本博客的**vs2013编译ffmpeg**系列文章

# 一、 编译环境

vs编译ffmpeg的官方指导文档请参考[http://ffmpeg.org/platform.html#Native-Windows-compilation-using-MinGW-or-MinGW_002dw64](http://ffmpeg.org/platform.html#Native-Windows-compilation-using-MinGW-or-MinGW_002dw64)。 
编译环境是win7 64位 + mingw/msys（32位环境） + vs2013，ffmpeg的版本是3.3，后面的描述基本上是基于这个环境。 

mingw/msys一直是用的32位版本。在XP上编译ffmpeg也使用了很长一段时间，最开始研究ffmpeg的时候还没有msys2，由于没有相关的需求，这个环境一直都没有换。ffmpeg的configure文件是个bash脚本，须在mingw/msys下面执行。好像没有看到谁用cygwin来配合vs编译ffmpeg。 

msys.bat 比较靠前的位置需要加个语句：
```bash
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
```

这个命令的作用是准备vs2013的编译环境，call了这个脚本后，msys才能找到vs2013的cl编译器，vs2013编译时用到的头文件路径、库文件路径也已经准备好了。

vs的版本包括vs2008、vs2010、vs2012、vs2013都用过。**`不要试图用vs2008、vs2010、vs2012来编译ffmpeg，因为vs2012及以前的版本不支持c99语法，vs2013才开始支持。`** ffmpeg里面有很多代码都是用c99语法规范编写的，vs2012及以前的版本编译时会报错。曾用vs2010编译ffmpeg用了很长时间，编译的时候需要将代码先转换成c99的代码，再用vs的cl.exe来编译转换后的代码，搞起来比较麻烦，而且会有意想不到的问题。用vs2013来编译ffmpeg问题则少多了。 

编译过很多ffmpeg的版本，包括1.0、2.x、3.0、3.2.2、3.3、3.4.2等，3.3用的时间比较长，后面都用这个版本来描述，可能会有部分描述是针对3.3以前的版本。

# 二、目录结构

由于引用的第三方模块比较多，改动也有点多，所以目录结构要安排好才不会乱。后面的描述中的路径可以参考下面的目录结构描述。顶层目录是ffmpeg_MSVC，如下图所示，里面有两个目录，contribute下面是ffmpeg所依赖的第三方源代码，有80个左右的目录，有的第三方源代码依赖其他模块，所以目录比较多。MSVC_ffmpeg下面是不同版本的ffmpeg源代码。 
![这里写图片描述](https://img-blog.csdn.net/2018062623595742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面的这些目录都用vs2013编译过： 
![这里写图片描述](https://img-blog.csdn.net/20180627000546885?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

contribute下面分了两个子目录，MSVC下面的代码都是ffmpeg所依赖的第三方模块的源代码。msys下面是用msys+gcc编译ffmpeg时依赖的第三方模块的源代码，主要作用是在遇到错误用于对比。 
![这里写图片描述](https://img-blog.csdn.net/20180627000641842?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

ffmpeg-3.3源代码目录： 
![这里写图片描述](https://img-blog.csdn.net/20180627000826213?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

源代码目录下面增加了几个目录： 
**extra_lib**：ffmpeg所依赖的msys/mingw的库，有的库可能是编译老版本ffmpeg所需的，但没有去掉。 
**inc**：msys/mingw的头文件，或者是第三方模块的头文件。有时候重装了msys或在不同电脑上拷贝这个编译环境，msys/mingw的头文件可能会不一样，为了避免依赖msys/mingw的头文件，将用到的头文件直接放到ffmpeg里面。最开始没有意识到ffmpeg在configure、make的时候会引用msys/mingw的头文件，出现了很多问题，后来单独建了这个inc文件夹，不再依赖msys/mingw的头文件，问题就少多了。 
**msvc**：vs2013的工程所在目录，ffmpeg.exe等建了vs工程。 
**pc_files**：ffmpeg在configure的时候会用到pkg-config，有的第三方模块用vs2013编译后没有产生.pc文件，需要自己手动构造，所以将需要用pkg-config检测的第三方模块对应的.pc文件放到pc_files目录下，这样也可以避免在configure的时候到源代码目录下去搜索.pc文件。目前用到的.pc文件有32个。 
![这里写图片描述](https://img-blog.csdn.net/20180627001649435?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180627001700900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 三、Configure命令

目前（20180620）所用到的configure命令如下，放在bash脚本文件里面执行的：

```bash
#!/bin/bash
binPrefix_D=vs2013_build 

export PKG_CONFIG_PATH="$(pwd)/pc_files "
echo PKG_CONFIG_PATH=$PKG_CONFIG_PATH
echo "==================== Start to build debug version ===================="
echo "1. configure ffmpeg first."

    ./configure --toolchain=msvc --prefix=./$binPrefix_D --extra-cflags="-Dsnprintf=_snprintf  -MTd -Z7 -I ./inc \
    -I ../../contribute/MSVC/faac-1.28/include \
    -I ../../contribute/MSVC/lame-3.99.5/include \
    -I ../../contribute/MSVC/x264-master/SMP/build_result/include \
    -I ../../contribute/MSVC/libtheora-1.2.0alpha1/include \
    -I ../../contribute/MSVC/openjpeg-1.5.1/libopenjpeg \
    -I ../../contribute/MSVC/libogg-1.3.1/include \
    -I ../../contribute/MSVC/libvorbis-1.3.4/include \
    -I ../../contribute/MSVC/xvidcore/src \
    -I ../../contribute/MSVC/opus-1.1/include \
    -I ../../contribute/MSVC/libvpx-v1.3.0 \
    -I ../../contribute/MSVC/openal-soft-1.15.1/include \
    -I ../../contribute/MSVC/bzip2-1.0.6 \
    -I ../../contribute/MSVC/x265_20160531/x265/source \
    -I ../../contribute/MSVC/x265_20160531/x265/build/vc13-x86 \
    -I ../../contribute/MSVC/libiconv-1.9.2/include \
    -I ../../contribute/MSVC/libilbc-master_20160630/vs2013_build/include \
    -I ../../contribute/MSVC/game-music-emu-master/vs2013_build/include  \
    -I ../../contribute/MSVC/soxr-0.1.2-Source/src  \
    -I ../../contribute/MSVC/gmp-master_20160630/vs2013_build/include  \
    -I ../../contribute/MSVC/zlib-1.2.8  \
    -I ../../contribute/MSVC/libcdio-paranoia-master_20160630/vs2013_build/include  \
    -I ../../contribute/MSVC/libcdio-master_20160630/vs2013_build/include  \
    -I ../../contribute/MSVC/libcdio-master_20160630/SMP  \
    -I ../../contribute/MSVC/frei0r-plugins-1.5.0/vs2013_build/include  \
    -I ../../contribute/MSVC/gsm-1.0-pl13/inc  \
    -I ../../contribute/MSVC/opencore-amr-0.1.3/msys_build/include  \
    -I ../../contribute/MSVC/snappy_20160715/snappy/vs2013_build/include \
    -I ../../contribute/MSVC/twolame-0.3.13/libtwolame  \
    -I ../../contribute/MSVC/vo-amrwbenc-0.1.3/msys_build/include  \
    -I ../../contribute/MSVC/WavPack_sources/Debug/include  \
    -I ../../contribute/MSVC/xavs-code_20160829  \
    -I ../../contribute/MSVC/decklink/include  \
    -I ../../contribute/MSVC/xz-5.2.2/src/liblzma/api  \
    -I ../../contribute/MSVC/nvenc_5.0.1_sdk/Samples/common/inc  \
    -I ../../contribute/MSVC/chromaprint-1.3.2/src  -DCHROMAPRINT_NODLL \
    -I ../../contribute/MSVC/flite-2.0.0-release/msys_build/include  \
    -I ../../contribute/MSVC/libgcrypt-master_20160630/vs2013_build/include  \
    -I ../../contribute/MSVC/libgpg-error-master_20160630/vs2013_build/include  \
    -I ../../contribute/MSVC/CUDA_NVIDIA/include  \
    -I ../../contribute/MSVC/libxml2-2.9.1/include  \
    -I ../../contribute/MSVC/libcelt/msvc  \
    " \
    --extra-ldflags="/NODEFAULTLIB:LIBCMT \
    -LIBPATH:../../contribute/MSVC/lame-3.99.5/output/Debug \
    -LIBPATH:../../contribute/MSVC/x264-master/SMP/build_result/lib/x86 \
    -LIBPATH:../../contribute/MSVC/libtheora-1.2.0alpha1/win32/VS2010/Win32/Debug \
    -LIBPATH:../../contribute/MSVC/openjpeg-1.5.1/bin/Debug \
    -LIBPATH:../../contribute/MSVC/libogg-1.3.1/win32/VS2010/Win32/Debug \
    -LIBPATH:../../contribute/MSVC/libvorbis-1.3.4/win32/VS2010/Win32/Debug \
    -LIBPATH:../../contribute/MSVC/xvidcore/build/win32/bin \
    -LIBPATH:../../contribute/MSVC/opus-1.1/win32/VS2010/Win32/Debug \
    -LIBPATH:../../contribute/MSVC/libvpx-v1.3.0/Win32/Debug \
    -LIBPATH:../../contribute/MSVC/openal-soft-1.15.1/Debug \
    -LIBPATH:../../contribute/MSVC/bzip2-1.0.6/Debug \
    -LIBPATH:../../contribute/MSVC/x265_20160531/x265/build/vc13-x86/Debug \
    -LIBPATH:../../contribute/MSVC/zvbi-0.2.35/src/.libs \
    -LIBPATH:../../contribute/MSVC/libiconv-1.9.2/lib  \
    -LIBPATH:../../contribute/MSVC/libilbc-master_20160630/vs2013_build/lib/x86 \
    -LIBPATH:../../contribute/MSVC/game-music-emu-master/vs2013_build/lib/x86 \
    -LIBPATH:../../contribute/MSVC/soxr-0.1.2-Source/msvc/Debug  \
    -LIBPATH:../../contribute/MSVC/gmp-master_20160630/vs2013_build/lib/x86  \
    -LIBPATH:../../contribute/MSVC/zlib-1.2.8/contrib/vstudio/vc11/x86/ZlibStatDebug  \
    -LIBPATH:../../contribute/MSVC/libcdio-paranoia-master_20160630/vs2013_build/lib/x86  \
    -LIBPATH:../../contribute/MSVC/libcdio-master_20160630/vs2013_build/lib/x86  \
    -LIBPATH:../../contribute/MSVC/frei0r-plugins-1.5.0/vs2013_build/lib/  \
    -LIBPATH:../../contribute/MSVC/gsm-1.0-pl13/lib  \
    -LIBPATH:../../contribute/MSVC/opencore-amr-0.1.3/msys_build/lib  \
    -LIBPATH:../../contribute/MSVC/snappy_20160715/snappy/vs2013_build/lib \
    -LIBPATH:../../contribute/MSVC/twolame-0.3.13/win32/lib  \
    -LIBPATH:../../contribute/MSVC/vo-amrwbenc-0.1.3/msys_build/lib  \
    -LIBPATH:../../contribute/MSVC/WavPack_sources/Debug  \
    -LIBPATH:../../contribute/MSVC/xavs-code_20160829/build/win32/bin  \
    -LIBPATH:../../contribute/MSVC/xz-5.2.2/windows/Debug/Win32/liblzma  \
    -LIBPATH:../../contribute/MSVC/chromaprint-1.3.2/src/Debug  \
    -LIBPATH:../../contribute/MSVC/flite-2.0.0-release/msys_build/lib  \
    -LIBPATH:../../contribute/MSVC/libgcrypt-master_20160630/vs2013_build/lib/x86  \
    -LIBPATH:../../contribute/MSVC/CUDA_NVIDIA/lib/Win32  \
    -LIBPATH:../../contribute/MSVC/libxml2-2.9.1/lib  \
    -LIBPATH:../../contribute/MSVC/libcelt/Debug  \
    -LIBPATH:./extra_lib \
    gcc.lib mingwex.lib \
     " \
     --enable-debug \
                      --enable-gpl --enable-version3 --enable-nonfree --enable-libmp3lame  --enable-libx264 --enable-libtheora      \
     --enable-libopenjpeg --enable-libvorbis --enable-libxvid --enable-libopus --enable-libvpx --enable-openal                      \
     --enable-runtime-cpudetect --enable-postproc --enable-avresample --enable-bzlib --enable-sdl                                   \
     --enable-libass --enable-libwebp --enable-libx265 --enable-libzvbi --enable-libfreetype --enable-fontconfig                    \
     --enable-libspeex --enable-avisynth --enable-libilbc --enable-libfribidi --enable-libgme --enable-libmodplug --enable-libsoxr  \
     --enable-gnutls --disable-schannel --enable-gmp --enable-libssh --enable-libcdio --enable-libbluray --enable-opengl            \
     --enable-libmfx --enable-libopenh264 --enable-dxva2 --enable-zlib --enable-frei0r --enable-libbs2b --enable-libgsm             \
     --enable-libcaca --enable-libopencore-amrnb  --enable-libopencore-amrwb  --enable-librtmp --enable-openssl --enable-libsnappy  \
     --enable-libschroedinger --enable-libtwolame --enable-libvidstab --enable-libvo-amrwbenc --enable-libwavpack --enable-libxavs  \
     --enable-libxavs --enable-libzimg --enable-libfdk-aac --enable-decklink --enable-nvenc --enable-gcrypt --enable-chromaprint    \
     --enable-libshine --enable-libtesseract --enable-libopencv --enable-opencl --enable-cuda --enable-cuvid
```

## configure参数说明

`export PKG_CONFIG_PATH**="$(pwd)/pc_files "`：定义.pc文件的搜索路径，pack-config会用到（请参考后面的内容）。Ffmpeg所依赖的第三方模块的.pc文件都放在当前目录下的pc_files文件夹里面。 
`--toolchain=msvc`：指定用vs的编译器，configure里面搜索msvc可以找到有关cl.exe编译器的检测代码。 
`--prefix=./$binPrefix_D`：make install的时候会用到这个路径，make完后执行make install可以将库文件、头文件拷贝到这个目录下。 
`--extra-cflags="”`：configure会将这里面的标志放到config.mk的CFLAGS变量里面，make的时候会用到CFLAGS定义的标志。 
`--extra-ldflags="”`：configure会将这里面的标志放到config.mk的LDFLAGS变量里面，链接时会用到LDFLAGS定义的标志。 
`-Dsnprintf=_snprintf`：以前编译的时候遇到过缺snprintf符号的问题，这个标志一直加在参数里面了。 
`-MTd`：最开始编译ffmpeg的时候选择了这个参数，后面一直使用。需要注意的是，ffmpeg的库用这个标志编译后，引用ffmpeg库的程序、ffmpeg库依赖的第三方程序都需要用这个标志编译。vs2013的工程属性里面有设置：Project properties->Configuration properties->C/C++->Code generation->Runtime Library改成Mtd。 
`-Z7`：调试信息的格式。这个好像也需要统一设置，即ffmpeg库、引用ffmpeg库的程序、ffmpeg库依赖的第三方程序都要用-Z7编译。 
`-I`：（大写的i，include中的i）后面是头文件路径。 

头文件路径没有什么特别的，多个头文件路径格式如下： 

```
--extra-cflags=" -I ./inc \ 

    -I ../../contribute/MSVC/faac-1.28/include ”
```

`/NODEFAULTLIB:LIBCMT`：经常遇到libcmtd和libcmt两个库相冲突的问题，直接在extra-cflags加上这个标志，忽略libcmt库里面的符号。 
`gcc.lib mingwex.lib`：msys+gcc编译的第三方库依赖于这两个库，有很多第三方软件只能用msys+gcc编译，如chromaprint等。
`-LIBPATH:path`：第三方库文件所在的路径，链接的时候会搜索这些路径，直到找到每一个依赖的库和符号。 

库文件路径格式如下： 

```
--extra-ldflags="-LIBPATH:../../contribute/MSVC/lame-3.99.5/output/Debug \ 

    -LIBPATH:../../contribute/MSVC/x264-master/SMP/build_result/lib/x86”
```


上面这个格式configure执行没有问题，也可以得出正确的库文件路径。但根据configure里面的msvc_common_flags函数的” -L*)”规则，下面这种写法才是正确的： 

```
--extra-ldflags="-L../../contribute/MSVC/lame-3.99.5/output/Debug \ 

    -L../../contribute/MSVC/x264-master/SMP/build_result/lib/x86”
```


有时间试一下这个写法。.pc文件里面用的是这个-L的写法。 
`--enable-xxx`：可以用configure –help查看各个–enable-xxx的具体含义。
configure文件本身的分析请参考[雷神的博客](https://blog.csdn.net/leixiaohua1020/article/details/44587465?locationNum=2&fps=1)。

# 四、Configure调试

大部分软件的configure执行分3个阶段： 

1、  检测cpu类型、操作系统类型、编译器所支持的参数、头文件等。 

2、  分析configure所带的enable参数，检测所依赖的头文件和库文件。 

3、  产生makefile。 

经常遇到configure相关的问题，所以configure调试的方法比较重要。
## 1、加打印的方法

configure是个bash脚本，通常bash脚本加打印的方法： 

echo “cflags_filter=$cflags_filter” 
configure执行时的详细打印在config.log里面，遇到错误的时候可以打开这个文件查看具体是什么错误。
## 2、条件执行

下面的语句会输出第二个echo的打印，用这个方法可以跳过某些不想执行的语句。因为configure执行时间很长，在查问题的时候可以跳过某些语句，加快问题查找。

```bash
if [ true = true ] ; then
    echo "==============true============"
else
    echo "==============false============"
fi
```

“[”和”]”前后都需要空格。脚本会走第一个echo分支，将要略过的语句放在else分支里面。

## 3、脚本打印行号

echo -n “line $LINENO ” 

加在configure里面，以检测执行到哪行了。这个语句对makefile没有作用。

## 4、跟踪脚本执行

sh -x  configure 

这个是shell脚本通用的方法。“-x”选项使shell在执行脚本的过程中把它实际执行的每一个命令行显示出来，并且在行首显示一个”+”号。 “+”号后面显示的是经过了变量替换之后的命令行的内容，有助于分析实际执行的是什么命令。

## 5、enable模块的检测函数

大部分问题都出在–enable-xxx这个阶段。检测所依赖的第三方库或头文件主要是下面这三类函数：

### check_header/check_header_objcc

检测头文件，这两个函数在configure里面都可以找到其定义。 

比如configure参数里面加了–enable-cuda，则对应的检测规则是：

```bash
enabled cuda              && check_header cuda.h # this is not a dependency
```

“#”号后面是注释。从config.log里面可以找到其检测时的打印：

```bash
check_header cuda.h
check_cpp
BEGIN ./ffconf.MdbZVmyu.c
    1   #include <cuda.h>
    2   int x;
END ./ffconf.MdbZVmyu.c
cl -D_ISOC99_SOURCE -D_FILE_OFFSET_BITS=64 …
```

即生成临时文件ffconf.MdbZVmyu.c（临时文件一般放在ffmpeg根目录下），里面只包含两条代码：

```
#include <cuda.h>
int x;
```

然后用cl编译ffconf.MdbZVmyu.c，编译没有报错就检测通过了。cl语句比较长，上面的cl语句没有全部截出来。

### check_lib/require

同时检测头文件和库文件，require最终也是调用了check_lib函数。 

比如–enable-gmp对应的检测规则：

```bash
enabled gmp               && require gmp gmp.h mpz_export libgmpd.lib
```

从config.log里面可以找到其检测时的打印：

```bash
require gmp gmp.h mpz_export libgmpd.lib
check_lib gmp.h mpz_export libgmpd.lib
check_func_headers gmp.h mpz_export libgmpd.lib
check_ld cc libgmpd.lib
check_cc libgmpd.lib
BEGIN ./ffconf.MdbZVmyu.c
    1   #include <gmp.h>
    2   #include <stdint.h>
    3   long check_mpz_export(void) { return (long) mpz_export; }
    4   int main(void) { int ret = 0;
    5    ret |= ((intptr_t)check_mpz_export) & 0xFFFF;
    6   return ret; }
END ./ffconf.MdbZVmyu.c
cl -D_ISOC99_SOURCE -D_FILE_OFFSET_BITS=64 …
./compat/windows/mslink /NODEFAULTLIB:LIBCMT …
```

即用cl检测gmp.h头文件是否存在、检测check_ mpz_export符号是否有定义，最后用./compat/windows/mslink进行链接，检测libgmpd.lib是否存在、check_mpz_export是否有具体的实现。mslink是个脚本，里面调用的是vs2013的链接器link。

### require_pkg_config

这个函数通过pkg-config命令来检测依赖的第三方库。 

如–enable-libass对应的检测规则如下：

```bash
enabled libass            && require_pkg_config libass ass/ass.h ass_library_init
```

从config.log里面可以找到其检测时的打印：

```bash
use_pkg_config libass ass/ass.h ass_library_init
pkg-config --debug --exists --print-errors libass
Option --debug seen
Option --exists seen
Option --print-errors seen
Path position of 'libass' is 1
Adding 'libass' to list of known packages, returning as package 'libass'
check_func_headers ass/ass.h ass_library_init …
check_ld cc …
check_cc …
BEGIN ./ffconf.MdbZVmyu.c
    1   #include <ass/ass.h>
    2   #include <stdint.h>
    3   long check_ass_library_init(void) { return (long) ass_library_init; }
    4   int main(void) { int ret = 0;
    5    ret |= ((intptr_t)check_ass_library_init) & 0xFFFF;
    6   return ret; }
END ./ffconf.MdbZVmyu.c
cl -D_ISOC99_SOURCE -D_FILE_OFFSET_BITS=64 …
./compat/windows/mslink /NODEFAULTLIB:LIBCMT …
```

pack-config加了–debug参数（打印比较多，上面没有全部贴出来），即将configure里面的check_pkg_config()语句：

```bash
check_cmd $pkg_config --exists --print-errors $pkgandversion || return
```

改成：

```bash
check_cmd $pkg_config --debug --exists --print-errors $pkgandversion || return
```

### dumpbin

dumpbin可以查符号表，对报缺符号的错误很有用：

> 
dumpbin -SYMBOLS  x265-static.lib    #将x265-static.lib的符号都倒出来。


也可以查看dll依赖：

> 
dumpbin -DEPENDENTS  ffmpeg.exe     #可以看到ffmpeg.exe所依赖的dll文件。


## 6、pkg-config

configure里面对一些模块的检测都是用pkg-config来实现的，很多错误是在执行pkg-config的时候出现的，或者是由于其返回的模块的头文件或库的路径有问题而导致configure失败。 

pkg-config的调试手段： 
**echo PKG_CONFIG_PATH=$PKG_CONFIG_PATH **此命令可以检查pkg-config当前的搜索路径，pkg-config会根据PKG_CONFIG_PATH所列的路径来搜索.pc文件。Windows下的mingw环境里面没有这个环境变量，如果ffmpeg在configure的时候要使用.pc文件，则需要手动指定.pc文件的路径。Linux下安装了pkg-config后会设置这个环境变量，执行这个echo语句的时候会看到相应的路径。 
**pkg-config加`--debug`参数可以看出其搜索路径**，configure在检查时执行类似于`pkg-config --exists --print-errors x264`的语句，在configure.log里面可以看到。当出问题的时候，直接将此命令加上`--debug`参数看pkg-configd 执行过程和结果。 
**pkg-config –debug –exists –print-errors libwebp >= 0.2.0**要改成`pkg-config --debug --exists --print-errors "libwebp >= 0.2.0"`才能在msys里面执行，libwebp >= 0.2.0要加双引号。 
**PKG_CONFIG_PATH环境变量里面**，路径之间用冒号分割，冒号前后不能有空格。Windows+msys、linux下都是这个规则。 
**export PKG_CONFIG_PATH的语句写法要注意**，如果当前PKG_CONFIG_PATH变量为空，则直接写成：
```bash
export PKG_CONFIG_PATH=$(pwd)/pc_files
```

如果当前PKG_CONFIG_PATH变量不为空，则可以写成：

```bash
export PKG_CONFIG_PATH=$(pwd)/pc_files:$PKG_CONFIG_PATH
```

否则的话，当PKG_CONFIG_PATH为空时，”:”会作为路径的一部分，pkg-config执行的报下面的错误：
`Cannot open directory '/home/AA/MSVC_ffmpeg/ffmpeg-3.3/pc_files:' in package search path: No such file or directory`
路径的最后多了个冒号，不知道msys里面为何pkg-config识别不了，pkg-config版本是0.27.1。linux系统下的0.23版本则没有此问题。 

在msys下，PKG_CONFIG_PATH环境变量里面，多个路径用”:”分割， export语句格式如下：

```bash
export PKG_CONFIG_PATH="$(pwd)/pc_files1:$(pwd)/pc_files2"
```

**`--dont-define-prefix`**来指示不需要覆盖prefix变量。请参考后面的[schroedinger编译](https://blog.csdn.net/u013601132/article/details/80947479)。

### 手动构造.pc文件

一些直接用MSVC编译的库没有产生.pc文件，所以需要手动构造.pc文件。其实很简单，比如libwebp手动构造的.pc文件：

```
prefix=../../contribute/MSVC/libwebp-0.5.1
exec_prefix=${prefix}
libdir=${exec_prefix}/output/debug-static/x86/lib
includedir=${prefix}/src

Name: libwebp
Description: Libwebp
Version: 0.5.1
Libs: -L${libdir} libwebp_debug.lib 
Cflags: -I${includedir}
```

主要有用的是Libs、Cflags两个变量，为后续编译指明头文件和库文件所在的位置。在执行ffmpeg的configure之前先将.pc文件所在的路径export出来，在ffmpeg-3.3目录下面执行：

```bash
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$(pwd)/pc_files
```

注意，冒号前后不能有空格。 

为了方便，直接将所有的.pc拷贝到ffmpeg-3.3下面的pc_files目录下。 

单独验证libwebp的时候可以用下面的命令：`pkg-config  --debug --exists --print-errors libwebp`
# 五、yasm

汇编代码的编译需要用到yasm，采用的版本是yasm-1.2.0，官网上下载的，编译好了后各vs版本都可以用。在msys下编译，解压后的编译方法： 

./configure; make 

中间没有遇到错误。 
`yasm、vsyasm、libgcc_s_dw2-1.dll、libiconv-2.dll、libintl-8.dll`，放到**C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin**下面。后三个文件在/MinGW/bin下面。 
`vsyasm.props、vsyasm.targets、vsyasm.xml`放到**C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\BuildCustomizations**下面，这三个文件在yasm的源代码下面。
**编译x265时的补丁**

yasm一直用的是1.2.0版本（x265 2.6之后的版本用的是nasm，为了编译速度），此版本的yasm编译x265的时候会异常退出，原因是里面有个hash表定义的太小，需要打补丁才能编译x265。即yasm-1.3.0\modules\preprocs\nasm\nasm-pp.c里面：
`#define NHASH 31`
改成：
`#define NHASH 4096`
重新编译yasm并生成yasm.exe，并替换掉C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin下面的yasm.exe。 

yasm-1.3.0已经解决了这个问题。

# 六、头文件路径

~ \MinGW\msys\1.0\win32、~ \MinGW\include目录最好都注释掉，否则configure的时候引用的头文件会在这些路径下，所有的头文件都用VC自带的、contribute\MSVC下面的第三方库、ffmpeg-3.3\inc下面的。 

同时要确保环境变量里面没有mingw、msys的头文件路径，即在msys的终端下用export命令确认INCLUDE变量后面没有相关路径，不然也会出错。 

曾经遇到过’rint’ undefined的错误：``ffplay.c(700) : error C4013: 'rint' undefined; assuming extern returning int
或者：
``vf_hqdn3d.o_converted.c
libavfilter/vf_hqdn3d.c(174) : error C4013: 'lrint' undefined; assuming extern returning int
libavfilter/vf_hqdn3d.c(244) : warning C4133: 'function' : incompatible types - from'const AVPixelFormat [20]' to 'const int *'
这个是因为config.h里面有#define HAVE_LRINT 1，configure通过/mingw/include下面的头文件找到了rint。真正的rint在C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\ math.h里面。如果没有检测到rint的话，就有ffmpeg源代码下面的libm.h里面定义的rint。

# 七、遇到的错误

## 1、LIBCMTD

关于多线程库，都统一用libcmtd，即编译的时候加上/MTd，包括所有的外部库、ffmpeg的源代码等。 

增加了``--enable-libfaac后可能会出现下面的错误：
``LIBCMTD.lib(dbghook.obj) : error LNK2005: __crt_debugger_hook already defined in LIBCMT.lib(dbghook.obj)
LIBCMTD.lib(isctype.obj) : error LNK2005: __isctype_l already defined in LIBCMT.lib(isctype.obj)
LIBCMTD.lib(isctype.obj) : error LNK2005: __isctype already defined in LIBCMT.lib(isctype.obj)
LINK : warning LNK4098: defaultlib 'LIBCMTD' conflicts with use of other libs; use /NODEFAULTLIB:library
ffprobe_g.exe : fatal error LNK1169: one or more multiply defined symbols found
make: *** [ffprobe_g.exe] Error1
在configure的``--extra-cflags参数里面加上/NODEFAULTLIB:LIBCMT

下面的错误：
``LIBCMTD.lib(isctype.obj) : error LNK2005: __isctype_l already defined in LIBCMT.lib(isctype.obj)
LIBCMTD.lib(isctype.obj) : error LNK2005: __isctype already defined in LIBCMT.lib(isctype.obj)
LINK : warning LNK4098: defaultlib 'LIBCMTD' conflicts with use of other libs; use /NODEFAULTLIB:library
在configure的``--extra-cflags参数里面加上/NODEFAULTLIB:LIBCMT。

下面这个错误：
``LINK : error LNK2001: unresolved external symbol _mainCRTStartup
silk_common.lib(NLSF_VQ_weights_laroia.obj) : error LNK2001: unresolved external symbol __RTC_CheckEsp
若库用-MTd编译的，但是源代码没有用此标志，则会报这个错误。源代码和库文件都要采用一致的标志位编译。 

Project properties->Configuration properties->C/C++->Code generation->Runtime Library改成Mtd 
![这里写图片描述](https://img-blog.csdn.net/20180628003236711?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)2、缺少snprintf

如果编译的时候报缺少snprintf的错误，则在configure的时候，将``--extra-cflags里面加上-Dsnprintf=_snprintf。3、缺少avpriv_snprintf``error LNK2001: unresolved external symbol _avpriv_snprintff

后来发现这个问题是configure的时候引用了/mingw/include里面的头文件，ffmpeg-2.2的解决办法： 

avpriv_snprintf在compat\msvcrt\snprintf.c里面，在configure里面搜索msvcrt或snprintf.o，这个在probe_libc里面有相关的语句。在这个函数后面加个打印，将libc_type的值打出来，发现其值是mingw32，正确的值应该是msvcrt，这个比较关键，有段时间这个值错了，导致出现了很多问题。查看probe_libc里面mingw32的检测条件是：
```bash

```
elif check_${pfx}cpp_condition _mingw.h "defined __MINGW_VERSION"  ||
check_${pfx}cpp_condition _mingw.h "defined __MINGW32_VERSION"; then

即存在头文件_mingw.h且头文件里面定义了__MINGW_VERSION或定义__MINGW32_VERSION。所以在configure的参数里面的头文件路径里面包含了_mingw.h。后来发现ffmpeg-2.2 /inc目录下面有_mingw.h，去掉这个文件重新configure就可以。但在编译libavcodec/libxvid.c的时候需要getopt.h 和_mingw.h，编译的时候又手动将_mingw.h改回来。

``--enable-fontconfig后，configure在检查fontconfig的时候需要_mingw.h，所以直接把_mingw.h里面的定义东东放到getopt.h里面，并确保头文件搜索路径里面没有_mingw.h。4、缺少useconds_t

configure的时候加上``--enable-libx264后，就必须加上unistd.h，否则编译avio.c的时候会报错，编译的时候也没有显示具体是什么错误。unistd.h里面用到了useconds_t，这个本来在下面的头文件里面定义的：``#include <sys/types.h>

x264没有适配vs的编译器，但能在msys环境下编译，真正的types.h应该是H:\MinGW\include\sys\types.h，但vs的安装目录下也有types.h，在C:\Program Files\Microsoft Visual Studio 12.0\VC\include\sys下面。由于搜索头文件时，系统头文件路径（即vs安装目录下的）在其他头文件路径的前面，所以编译的时候会用vs下面的。所以这个问题的解决办法是直接将定义：``typedefunsignedint useconds_t; 

放到ffmpeg-3.3/inc/unistd.h里面。5、__inline__

vs的cl编译器不认识__inline__，直接将unistd.h里面的ftruncate相关定义注释掉。 

MSVC2013之前的版本都不支持inline关键字，编译的时候需要加定义-Dinline=__inline。MSVC2013版本inline成了关键字，但好像和gcc的inline作用不一样，下面这个错误需要将代码当作C++来编译，default配置编译会有问题。``error C2054: expected '(' to follow 'inline'

![这里写图片描述](https://img-blog.csdn.net/20180628010101679?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)6、mingwex.lib

configure会检查很多数学函数如roundf、sinf等，这些函数在mingwex.lib里面，所以将mingwex.lib加到``--extra-ldflags里面，在configure的时候带上``--extra-ldflags参数。如果没有检查到这些函数，则会用ffmpeg自带的。 

加上mingwex.lib后编译的时候会有下面的错误：``h:\mingw\msys\1.0\home\aa\ffmpeg_msvc\msvc_ffmpeg\ffmpeg-3.0\libavutil\x86/intmath.h(58) : error C4013: '_tzcnt_u32' undefined; assuming extern returning int

在mingw、msys的库里面没有找到tzcnt_u32，MSVC下没有搜索。解决的办法是直接在config.h里面将HAVE_FAST_CLZ改成0，让程序使用ffmpeg自带的相关函数。每次configure都需要手动修改一下，没有遇到这个错误就不用管。

mingwex.lib是拷贝了/MinGW/lib目录下的，原名libmingwex.a，直接改名的，没有用工具转换。 

MSVC2013+ffmpeg-3.0，报下面的错误：``LIBCMTD.lib(strtoq.obj) : error LNK2005: _strtoull 已经在 mingwex.lib(strtoumax.o) 中定义
LIBCMTD.lib(strtoq.obj) : error LNK2005: _strtoumax 已经在 mingwex.lib(strtoumax.o) 中定义

解决的办法是将strtoumax.o从mingwex.lib里面去掉，先用ar命令将/MinGW/bin/libmingwex.a里面的strtoumax.o去掉，再重命名libmingwex.a为mingwex.lib，MSVC也可以直接链接.a文件。
```bash

```
ar -d libmingwex.a strtoumax.o

如果_strtof也报重复定义，也是同样的处理方法。 

用MSVC2013编译还需要去掉strtoimax.o，configure在检测strtoll的时候报重定义。LIBCMTD.lib里面有strtoll的实现，ffmpeg configure如果没有检测到strtoll则定义strtoll=_strtoi64（-D strtoll=_strtoi64）。八、ShiftMediaProject

官网：[](https://github.com/ShiftMediaProject)https://github.com/ShiftMediaProject

这个上面提供ffmpeg及其依赖的第三方库下载，都是源代码，而且里面都有vs的工程。这上面本来有很多项目的（至少有5页），2017年后删除了很多。 

没有用这上面的编译方法，因为想更方便地enable ffmpeg的特性。有部分ffmpeg依赖的第三方库是用的这个上面的。














