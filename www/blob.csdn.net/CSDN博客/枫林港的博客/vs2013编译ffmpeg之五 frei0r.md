# vs2013编译ffmpeg之五 frei0r - 枫林港的博客 - CSDN博客





2018年07月07日 22:43:50[枫林港](https://me.csdn.net/u013601132)阅读数：173








# frei0r

从[http://frei0r.dyne.org](http://frei0r.dyne.org)下载frei0r-plugins-1.5.0，在其根目录下执行下面的命令，就可以产生sln文件：
`cmake -G "Visual Studio 12 2013" ./  `
整个solution有124个工程，里面的编译错误很多，大部分很容易修改，下面列出比较特别的：

## 1、M_PI_2未定义

error c2065 ’ M_PI_2’ undeclared identifier 

M_PI_2在/mingw/include/math.h里面定义的，直接将这些定义拷贝到frei0r.hpp，因为出错的文件比较多，所以统一放到这个头文件里面。 

## 2、stat重复定义

1>e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\contribute\msvc\frei0r-plugins-1.5.0\src\filter\measure\measure.h(330): error C2039: ‘rms’ : is not a member of ‘stat’ 

1>          C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\wchar.h(517) : see declaration of ‘stat’ 

两个stat的定义重复，直接将measure.h里面的定义改名成frei0r_stat，还有pr0be.c里面的也需要修改。
## 3、map

dither.c(256): error C2057: expected constant expression 

dither.c(256): error C2466: cannot allocate an array of constant size 0 

dither.c(256): error C2133: ‘map’ : unknown size 

map的定义：int map[levels]; 这个MSVC不能识别，改成：int *map = (int *) malloc(sizeof(int)*levels); 然后在函数返回前free(map)。
## 4、类型不匹配

curves.c(307): error C2440: ‘type cast’ : cannot convert from ‘double’ to ‘CHANNELS’ 

inst->channel = (enum CHANNELS)(tmp * 10); 

改成： 

int tmp__1 =  (int)(tmp * 10); 

inst->channel = (enum CHANNELS)(tmp__1); 

整个工程会生成众多.dll，ffmpeg在要使用的时候用dlopen打开对应的库文件。参照msys下编译frei0r的编译结果，在其目录下建立msys_build目录，将生成的库（.dll）都放到./build/lib目录下，头文件frei0r.h放到./build/include下。
# msys编译frei0r

msys下编译frei0r，在frei0r根目录下： 

mkdir msys_build 

cd msys_build 

cmake -G “MSYS Makefiles” -DCMAKE_INSTALL_PREFIX=./  .. 

make; make install 

打开–enable-frei0r ，MSVC2013+ffmpeg-3.0编译ffmpeg的库的时候，会报错，找不到dlfcn.h。直接将/MinGW/include下面的dlfcn.h拷贝到frei0r.h同级目录，dlfcn.h需要修改，dlfcn.h里面不能include _mingw.h，否则ffmpeg在configure的时候会将系统识别成msys，直接注释掉#include <_mingw.h>。删除“**CRT_ALIAS __LIBIMPL**(( FUNCTION = dlopen ))”、**restrict**。将_mingw.h下面这段拷贝到dlfcn.h里面，放在靠前的位置：
```
#ifdef __cplusplus
# define _EXTERN_C       extern "C"
# define _BEGIN_C_DECLS  extern "C" {
# define _END_C_DECLS    }
# define __CRT_INLINE    inline
#else
# define _EXTERN_C       extern
# define _BEGIN_C_DECLS
# define _END_C_DECLS
# if __GNUC_STDC_INLINE__
#  define __CRT_INLINE   extern inline __attribute__((__gnu_inline__))
# else
#  define __CRT_INLINE   extern __inline__
# endif
#endif
```

frei0r未做测试，看libavfilter/vf_frei0r.c里面的代码，frei0r的dll路径都是固定在/usr目录下的，如果要在windows上测试，则需要将这些库的路径安排好，或者直接修改libavfilter/vf_frei0r.c的代码。







