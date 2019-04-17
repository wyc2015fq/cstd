# vs2013编译ffmpeg之三 bzip2、chromaprint - 枫林港的博客 - CSDN博客





2018年07月07日 22:41:32[枫林港](https://me.csdn.net/u013601132)阅读数：135








# bzip2

从[官网](http://www.bzip.org/download)上下载的版本是bzip2-1.0.6，里面sln工程，MSVC2013打开编译没有遇到错误。 

ffmpeg的configure里面，将： 

disabled bzlib || check_lib2 bzlib.h BZ2_bzlibVersion libbz2.dll || disable bzlib 

改成 

disabled bzlib || check_lib2 bzlib.h BZ2_bzlibVersion libbz2.lib || disable bzlib
# chromaprint

参考： 
[https://github.com/acoustid/chromaprint](https://github.com/acoustid/chromaprint)
[https://acoustid.org/chromaprint](https://acoustid.org/chromaprint)
[https://bitbucket.org/acoustid/chromaprint](https://bitbucket.org/acoustid/chromaprint)

用于音乐指纹的开源库。从官网上下载源代码，用cmake产生sln文件：
```bash
cmake -G "Visual Studio 12 2013" -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON  -DFFMPEG_ROOT=../../../MSVC_ffmpeg/ffmpeg-3.0/vs2013_build  ./
```

出现下面的错误：

```java
CMake Error at CMakeLists.txt:123 (message):
  Neither FFmpeg with avfft.h nor FFTW3 found
```

这个是由于chromaprint依赖与ffmpeg或FFTW3，在chromaprint-1.3.2\cmake\modules\ FindFFmpeg.cmake里面有检测ffmpeg的头文件和库，若里面列出的路径里面没有ffmpeg，则cmake的命令里面就需要指定路径，即-DFFMPEG_ROOT参数。 

为了去掉chromaprint依赖ffmpeg，先不带–enable-chromaprint来configure ffmpeg-3.0，编译出库后，将所有库文件、头文件拷贝到chromaprint-1.3.2\ffmpeg_dependency下，库文件改成avcodec.lib形式。 

cmake命令执行时，“Could NOT find Boost”这个打印不用管。 

fpcalc由于链接了ffmpeg的库，而ffmpeg的库链接了很多第三方库，fpcalc编译通不过，但库已经生成，不用解决。
## 库的命名

cmake对库的前缀和后缀命名都有要求，可以用cmake的`--debug-output --trace`参数将相关打印打出来(debug、trace前面是两个横杠)，可以看到：

```
C:/Program Files/CMake/share/cmake-3.6/Modules/CMakeGenericSystem.cmake(26):  set(CMAKE_LINK_LIBRARY_SUFFIX  )
   Called from: [3] C:/Program Files/CMake/share/cmake-3.6/Modules/CMakeGenericSystem.cmake
                [2] C:/Program Files/CMake/share/cmake-3.6/Modules/CMakeSystemSpecificInformation.cmake
                [1] E:/MinGW/msys/1.0/home/AA/ffmpeg_MSVC/contribute/MSVC/chromaprint-1.3.2/CMakeLists.txt
C:/Program Files/CMake/share/cmake-3.6/Modules/CMakeGenericSystem.cmake(27):  set(CMAKE_STATIC_LIBRARY_PREFIX lib )
   Called from: [3] C:/Program Files/CMake/share/cmake-3.6/Modules/CMakeGenericSystem.cmake
                [2] C:/Program Files/CMake/share/cmake-3.6/Modules/CMakeSystemSpecificInformation.cmake
                [1] E:/MinGW/msys/1.0/home/AA/ffmpeg_MSVC/contribute/MSVC/chromaprint-1.3.2/CMakeLists.txt
```

和：

```
C:/Program Files/CMake/share/cmake-3.6/Modules/Platform/Windows.cmake(11):  set(CMAKE_STATIC_LIBRARY_PREFIX  )
   Called from: [3] C:/Program Files/CMake/share/cmake-3.6/Modules/Platform/Windows.cmake
                [2] C:/Program Files/CMake/share/cmake-3.6/Modules/CMakeSystemSpecificInformation.cmake
                [1] E:/MinGW/msys/1.0/home/AA/ffmpeg_MSVC/contribute/MSVC/chromaprint-1.3.2/CMakeLists.txt
C:/Program Files/CMake/share/cmake-3.6/Modules/Platform/Windows.cmake(12):  set(CMAKE_STATIC_LIBRARY_SUFFIX .lib )
   Called from: [3] C:/Program Files/CMake/share/cmake-3.6/Modules/Platform/Windows.cmake
                [2] C:/Program Files/CMake/share/cmake-3.6/Modules/CMakeSystemSpecificInformation.cmake
                [1] E:/MinGW/msys/1.0/home/AA/ffmpeg_MSVC/contribute/MSVC/chromaprint-1.3.2/CMakeLists.txt
```

CMakeGenericSystem.cmake、Windows.cmake两个文件里面都定义了CMAKE_STATIC_LIBRARY_PREFIX和CMAKE_STATIC_LIBRARY_SUFFIX，但由于Windows.cmake在后，所以用的最终定义是Windows.cmake里面，即库的前缀为空，后缀为.lib。 

那么在检测ffmpeg的时候，库的命名形式就是avcodec.lib。由于自己编译出来的库是libavcodec.a，导致cmake失败，这个问题查了一天。

## 编译

产生了chromaprint.sln文件后，用MSVC2013打开
- chromaprint_p、chromaprint两个工程，将里面的绝对路径改成相对路径，避免后续拷贝到其他硬盘下会编译通不过。
- Runtime Library都改成Mtd。
- 
chromaprint_p工程里面去掉fft_lib_avfft.cpp，这个文件会依赖ffmpeg的libavcodec.a、libavutil.a，这样会相互依赖，ffmpeg  configure的时候会失败，直接去掉fft_lib_avfft.cpp也可以编译通过。

- 
chromaprint_p生成的库名改成chromaprint.lib，ffmpeg configure检测的是这个库名。

- chromaprint工程还需要链接gcc.lib、mingwex.lib，和编译ffmpeg时用的版本是一样的。
- chromaprint工程Image Has Safe Exception Handlers改成NO。
- configure—>enabled chromaprint里面用strlen替换了chromaprint_get_version，是因为检测chromaprint_get_version函数的时候，会链接chromaprint的库，而chromaprint的库会依赖于ffmpeg的库，但ffmpeg的库还没有编译出来，也没有必要在configure的时候链接以前编译的ffmpeg库，直接用strlen代替，通过检测就ok了，后面一起链接生产ffmpeg.exe的时候也不会报错。

ffmpeg-3.0编译的时候遇到下面的错误：

```
libavformat.a(chromaprint.o) : error LNK2001: 无法解析的外部符号 __imp__chromaprint_new
libavformat.a(chromaprint.o) : error LNK2001: 无法解析的外部符号 __imp__chromaprint_free
libavformat.a(chromaprint.o) : error LNK2001: 无法解析的外部符号 __imp__chromaprint_set_option
```

chromaprint-1.3.2\src\ chromaprint.h里面有：

```
#if (defined(_WIN32) || defined(_WIN64))
#   ifdef CHROMAPRINT_NODLL
#       define CHROMAPRINT_API
#   else
#       ifdef CHROMAPRINT_API_EXPORTS
#           define CHROMAPRINT_API __declspec(dllexport)
#       else
#           define CHROMAPRINT_API __declspec(dllimport)
#       endif
#   endif
#else
#   if __GNUC__ >= 4
#       define CHROMAPRINT_API __attribute__ ((visibility("default")))
#   else
#       define CHROMAPRINT_API
#   endif
#endif
```

编译libavformat/ chromaprint.c的时候需要定义CHROMAPRINT_NODLL，这样就会走到#define CHROMAPRINT_API这个分枝，libavformat/chromaprint.o的符号前面就不会有__imp前缀。由于ffmpeg的configure里面是用函数require来检测chromaprint的，require后面的参数里面没有办法带宏定义，所以不能在configure里面加-DCHROMAPRINT_NODLL来解决这个问题，只能在configure的参数里面加。重新configure后，在编译前要先删除libavformat/chromaprint.o，不然不会重新编译libavformat/ chromaprint.c。

`20170428：`

ffmpeg-3.3在configure的时候，enabled chromaprint规则里面只添加strlen还不行，ffmpeg-3.2.2只添加strlen则可以，ffmpeg-3.3的require函数不一样了，configure的时候会失败。解决方法，enabled chromaprint规则：
`enabled chromaprint       && require chromaprint chromaprint.h chromaprint_get_version –lchromaprint`
改成；
`enabled chromaprint       && require chromaprint “chromaprint.h string.h” strlen -lchromaprint`
因为strlen对应的头文件是string.h，所以在规则里面加上，保证configure在编译的时候能通过。规则中两个头文件要用双引号括起来。






