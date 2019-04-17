# vs2013编译ffmpeg之三十六 zimg - 枫林港的博客 - CSDN博客





2018年07月10日 23:34:17[枫林港](https://me.csdn.net/u013601132)阅读数：180








# zimg

对应ffmpeg configure选项–enable-libzimg。

[https://github.com/sekrit-twc/zimg](https://github.com/sekrit-twc/zimg)

libzimg用于图片缩放、颜色空间转换、采样深度转换。下载的包中有vs工程，但用vs2013编译通不过，也没有搞定，从sln文件看工程是用vs2015创建的。vs2013编译有下面的错误：

> 
colorspace.h(71): `error`C2355: ‘this’ : can only be referenced inside non-static member functions or non-static data member initializers


目前此模块是msys编译的，编译命令：

./autogen.sh; mkdir msys_build; ./configure –prefix=$(pwd)/msys_build; make; make install

ffmpeg-3.3需要zimg-release-2.3以上版本，目前使用2.5.1，ffmpeg-3.3/pc_pcfiles/zimg.pc要修改。

```
prefix=../../contribute/MSVC/zimg-release-2.5.1/msys_build
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: zimg
Description: Scaling, colorspace conversion, and dithering library
Version: 2.5.1

# If building a static library against a C++ runtime other than libstdc++,
# define STL_LIBS when running configure.
Libs: -L${libdir} libzimg.dll.a
#Libs.private: -lstdc++
Cflags: -I${includedir}
```

## gcc升级

gcc需要升级到4.9.3，由于以前的mingw+msys系统是手动安装的，不是用mingw-gui安装的，不能使用mingw-gui来下载和安装gcc。只能用mingw-get来安装，但mingw-get.exe update mingw32-gcc-g++或mingw-get.exe install mingw32-gcc-g++都不能安装gcc4.9.3，只能直接下载安装包：[https://sourceforge.net/projects/mingw/files/MinGW/Base/gcc/Version4/gcc-4.9.3-1/](https://sourceforge.net/projects/mingw/files/MinGW/Base/gcc/Version4/gcc-4.9.3-1/)，下载gcc-4.9.3-1-mingw32-lic.tar.xz、gcc-c++-4.9.3-1-mingw32-bin.tar.xz、gcc-core-4.9.3-1-mingw32-bin.tar.xz，然后手动安装。同时还需要安装mpc：

> 
$ mingw-get install mpc

[http://prdownloads.sourceforge.net/mingw/gmp-5.1.2-1-mingw32-dev.tar.lzma?download](http://prdownloads.sourceforge.net/mingw/gmp-5.1.2-1-mingw32-dev.tar.lzma?download)

39.82 kB / 39.82 kB     |================================================| 100%

[http://prdownloads.sourceforge.net/mingw/mpfr-3.1.2-2-mingw32-dll.tar.lzma?download](http://prdownloads.sourceforge.net/mingw/mpfr-3.1.2-2-mingw32-dll.tar.lzma?download)

160.33 kB / 160.33 kB   |================================================| 100%

[http://prdownloads.sourceforge.net/mingw/mpfr-3.1.2-2-mingw32-dev.tar.lzma?download](http://prdownloads.sourceforge.net/mingw/mpfr-3.1.2-2-mingw32-dev.tar.lzma?download)

18.62 kB / 18.62 kB     |================================================| 100%

[http://prdownloads.sourceforge.net/mingw/libmpc-1.0.2-mingw32-dll-3.tar.xz?download](http://prdownloads.sourceforge.net/mingw/libmpc-1.0.2-mingw32-dll-3.tar.xz?download)

34.07 kB / 34.07 kB     |================================================| 100%

[http://prdownloads.sourceforge.net/mingw/mpc-1.0.2-mingw32-dev.tar.xz?download](http://prdownloads.sourceforge.net/mingw/mpc-1.0.2-mingw32-dev.tar.xz?download)

41.49 kB / 41.49 kB     |================================================| 100%

[http://prdownloads.sourceforge.net/mingw/mpc-1.0.2-mingw32-doc.tar.xz?download](http://prdownloads.sourceforge.net/mingw/mpc-1.0.2-mingw32-doc.tar.xz?download)

4.67 kB / 4.67 kB       |================================================| 100%

[http://prdownloads.sourceforge.net/mingw/mpc-1.0.2-mingw32-info.tar.xz?download](http://prdownloads.sourceforge.net/mingw/mpc-1.0.2-mingw32-info.tar.xz?download)

20.58 kB / 20.58 kB     |================================================| 100%

[http://prdownloads.sourceforge.net/mingw/mpc-1.0.2-mingw32-lic.tar.xz?download](http://prdownloads.sourceforge.net/mingw/mpc-1.0.2-mingw32-lic.tar.xz?download)

2.68 kB / 2.68 kB       |================================================| 100%


## pthread升级

重新用mingw-gui安装了msys后，执行configure的时候发现没有找到pthread库，检查MinGW环境，里面没有pthread头文件和库，直接用mingw-gui来下载，如下图所示，mingw32-pthreads-w32相关的三个都需要安装：

![这里写图片描述](https://img-blog.csdn.net/20180707003715387?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 编译错误

### 编译错误一

gcc 4.6.2报下面的错误：

> 
cc1plus.exe: error: unrecognized command line option ‘-std=c++11’

$ g++  –version

g++.exe (GCC) 4.6.2

Copyright (c) 2011 Free Software Foundation, Inc.


升级到4.9.3就没有这个错误了。

### 编译错误二

20160831时最新的版本是mingw32-pthreads-w32-2.9.1，以前用的是2.8，ffmpeg编译时inc、extra_lib下面的也是2.8版本的。安装了pthread后再configure会报错：

> 
In file included from conftest.c:21:0:

e:\mingw\include\pthread.h:320:8: error: redefinition of ‘struct timespec’

struct timespec {

​        ^

In file included from e:\mingw\include\time.h:53:0,

​                 from e:\mingw\include\pthread.h:219,

​                 from conftest.c:21:

e:\mingw\include\parts\time.h:105:8: note: originally defined here


查看pthread.h里面的定义：

```
#if !defined(HAVE_STRUCT_TIMESPEC)

#define HAVE_STRUCT_TIMESPEC

#if !defined(_TIMESPEC_DEFINED)

#define _TIMESPEC_DEFINED

struct timespec {

        time_t tv_sec;

        long tv_nsec;

};

#endif /* _TIMESPEC_DEFINED */

#endif /* HAVE_STRUCT_TIMESPEC */
```

解决的办法是在configure里面的#include 
`AM_CXXFLAGS = -DNO_INLINE -std=c++11 $(commonflags)`
### 编译错误四

下面这个错误：

> 
./src/zimg/common/alloc.h: In function ‘void* zimg_x_aligned_malloc(size_t, size_t)’:

./src/zimg/common/alloc.h:12:109: error: ‘_aligned_malloc’ was not declared in this scope

inline void *zimg_x_aligned_malloc(size_t size, size_t alignment) { return _aligned_malloc(size, alignment); }

​                                                                                                             ^

./src/zimg/common/alloc.h: In function ‘void zimg_x_aligned_free(void*)’:

./src/zimg/common/alloc.h:13:65: error: ‘_aligned_free’ was not declared in this scope

inline void zimg_x_aligned_free(void *ptr) { _aligned_free(ptr); }

​                                                                 ^

make[1]: *** [src/zimg/api/libzimg_internal_la-zimg.lo] Error 1

make: *** [all-recursive] Error 1


./src/zimg/common/alloc.h里面改成：

```
#ifdef _WIN32

#ifdef MSVCRT_VERSION

#undef MSVCRT_VERSION



#define  MSVCRT_VERSION  0x0700

#endif





  #include <malloc.h>

  inline void *zimg_x_aligned_malloc(size_t size, size_t alignment) { return _aligned_malloc(size, alignment); }

  inline void zimg_x_aligned_free(void *ptr) { _aligned_free(ptr); }
```

##### **编译错误五**

ffmpeg-3.3在configure的时候，报下面的错误：

> 
libzimg.a(libzimg_internal_la-zimg.o) : fatal error LNK1143: 无效或损坏的文件:  没有 COMDAT 0x71 节的符号


开始时使用的是静态库libzimg.a，改成libzimg.dll.a就可以了，ffmpeg运行的时候需要将libzimg-2.dll拷贝到ffmpeg.exe同级目录下。



