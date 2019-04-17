# vs2013编译ffmpeg之二十四 Openjpeg - 枫林港的博客 - CSDN博客





2018年07月09日 23:30:50[枫林港](https://me.csdn.net/u013601132)阅读数：130








# Openjpeg

对应ffmpeg configure选项–enable-libopenjpeg。

最终用的是openjpeg-1.5.1，高版本的在ffmpeg configure的时候有错误，没有着手解决。openjpeg-1.5.1编译方法和openjpeg-2.0.0类似。

下载源代码openjpeg-2.0.0.tar.gz，解压后在openjpeg-2.0.0目录下执行：
`cmake -G "Visual Studio 10" ./`
会产生MSVC2010的工程文件，打开后直接在MSVC里面编译openjp2工程。

其头文件在openjpeg-2.0/src/lib/openjp2下面，ffmpeg的configure里面的-lopenjpeg要改成-lopenjp2。

## 编译错误一

下面这个错误：
`test.o : error LNK2019: unresolved external symbol _opj_version referenced in function _main`
产生的原因是在openjpeg.h里面如果没有定义OPJ_STATIC就会有：
`#define OPJ_CALLCONV __stdcall`
所以在vs里面要加上OPJ_STATIC这个宏定义。

## 编译错误二

ffmpeg3.3的configure在检测openjpeg的时候报下面的错误：
`fatal error C1083: Cannot open include file: 'openjpeg-1.5/openjpeg.h': No such file or directory`
路径不对，真正的路径是openjpeg-1.5.1，直接修改configure文件，将：
`check_lib openjpeg-1.5/openjpeg.h`
改成：
`check_lib openjpeg-1.5.1/openjpeg.h`
libopenjpeg目录下需要增加这个openjpeg-1.5.1目录并将openjpeg.h拷贝到目录下。从log里面看，虽然configure openjpeg-1.5.1成功了，但config.h里面的HAVE_OPENJPEG_1_5_OPENJPEG_H还是没有定义，搜索ffmpeg-3.3的代码，只是头文件路径不一样，没有啥影响。

##msys编译openjpeg##

下载openjpeg-1.5.1.tar.gz，和前面一样用cmake产生sln文件。

修改工程属性，Configuration Properties–>General–>Configuration Type设置成“Static libraty(.lib)”。

msys+gcc下面编译openjpeg，用下面的configure命令：

```
./configure --enable-static --disable-shared

make CFLAGS="-DWIN32 -DOPJ_STATIC $CFLAGS"
```

否则在msys+gcc下面configure ffmpeg的时候会报下面的错误：
`test.o:test.c:(.text+0xc): undefined reference to `opj_version'`
用nm从libjpeg.a里面查看：
`00000008 T _opj_version@0`
符号表里面多了@0，这个是动态库里面才有的，但不知道为啥在.a里面有这个后缀，所以gcc识别不了。采用make CFLAGS=”-DWIN32 -DOPJ_STATIC $CFLAGS”编译出来的符号表是这样子的：
`00000000 T _opj_version`


