# vs2013编译ffmpeg之十六 libvpx - 枫林港的博客 - CSDN博客





2018年07月08日 23:17:32[枫林港](https://me.csdn.net/u013601132)阅读数：220








## libvpx-1.7.0

最近官网[http://www.webmproject.org/code/build-prerequisites/](http://www.webmproject.org/code/build-prerequisites/) 访问不了，在[http://www.loongnix.org/cgit/libvpx/](http://www.loongnix.org/cgit/libvpx/)上下载了libvpx-1.7.0版本，configure如下：
` ./configure --target=x86-win32-vs12 --enable-vp8 --enable-vp9 --enable-debug --enable-vp9-highbitdepth --enable-postproc --enable-vp9-posproc  --enable-error-concealment --enable-multi-res-encoding --enable-webm-io`
configure打印如下：

```
enabling vp8
  enabling vp9
  enabling debug
  enabling vp9_highbitdepth
  enabling postproc
  enabling vp9_postproc
  enabling error_concealment
  enabling multi_res_encoding
  enabling webm_io
Configuring for target 'x86-win32-vs12'
  enabling x86
vs12 does not support avx512, disabling.....
  disabling avx512
  enabling runtime_cpu_detect
  enabling mmx
  enabling sse
  enabling sse2
  enabling pic
  enabling sse3
  enabling ssse3
  enabling sse4_1
  enabling avx
  enabling avx2
  using msvs
  disabling ccache
  enabling unit_tests
  enabling libyuv
Bypassing toolchain for environment detection.
Creating makefiles for x86-win32-vs12 libs
Creating makefiles for x86-win32-vs12 examples
Creating makefiles for x86-win32-vs12 tools
Creating makefiles for x86-win32-vs12 docs
Creating makefiles for x86-win32-vs12 solution
```

然后执行make，会报错，但生成了sln文件，打开sln进行编译。

### 编译错误一：

```
fatal error C1083: Cannot open include file: 'libyuv/cpu_id.h': No such file or directory
fatal error C1083: Cannot open include file: 'gtest/internal/gtest-internal.h': No such file or directory
```

每个工程设置里面的头文件路径进行修改，将/home/AA/contribute/MSVC/libvpx-v1.7.0改成”.”。这个绝对路径/home/AA/contribute/MSVC/libvpx-v1.7.0是msys能识别的，VS2013识别不了。可能configure/make的时候需要用vs2013的Command Prompt来执行。其他工程里面的头文件路径设置也需要将绝对路径改成相对路径。

### 编译错误二：
`Cannot open include file: 'sys/time.h': No such file or directory`
直接注释掉#include 

### 编译错误三：

```
1>------ Build started: Project: gtest, Configuration: Debug Win32 ------
1>cl : Command line warning D9002: ignoring unknown option '/home\AA\ffmpeg_MSVC\contribute\MSVC\libvpx-1.7.0\third_party\googletest\src\src\gtest-all.cc'
1>cl : Command line error D8003: missing source filename
```

双击工程里面的gtest-all.cc会报找不到此文件的错误，在工程里面删除gtest-all.cc，重新将gtest-all.cc文件（libvpx-1.7.0\third_party\googletest\src\src\gtest-all.cc）放到工程里面。

# libvpx-v1.3.0

官网下载[libvpx-v1.3.0版本](http://www.webmproject.org/code/build-prerequisites/)。 

编译成MSVC版本：

```bash
./configure --target=x86-win32-vs10 --enable-vp8 --enable-vp9
make
```

make 的时候会出错（缺少obj_int_extract.c文件），但是会产生sln和vcxproj，用MSVC2010打开sln文件，从obj_int_extract工程里面删除obj_int_extract.c并重新加载该文件，再编译就不会缺文件了。 

Gtest工程里面的gtest-all.cc的也是一样需要替换，并将其工程设置里面的头文件路径进行修改，将/home/AA/contribute/MSVC/libvpx-v1.3.0改成”.”，改成相对路径方便一点，可以随意拷贝或移动整个目录。

## 编译错误一：
`Command line warning D9002: ignoring unknown option '/home/AA/contribute/MSVC/libvpx-v1.3.0/vp8/encoder/vp8_asm_enc_offsets.c'`
将obj_int_extract.bat里面的：
`cl /I "./" /I "%1" /nologo /c "%1/vp8/encoder/vp8_asm_enc_offsets.c"`
改成：
`cl /I "./" /I "%1" /nologo /c "./vp8/encoder/vp8_asm_enc_offsets.c"`
## 编译错误二：

vpx的工程属性里面，Project properties->Configuration properties->C/C++->Code generation->Runtime Library要设置成Mtd，所有工程都需要设置。否则configure的时候会有下面的错误：
`LIBCMTD.lib(dbgheap.obj) : error LNK2005: __heap_alloc already defined in LIBCMT.lib(malloc.obj)`
## 编译错误三：

在64位win7上用MSVC2013编译libvpx，遇到下面的错误：

```
1>  Assembling vpx_config.asm
1>C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppCommon.targets(170,5): error MSB6006: "cmd.exe" exited with code -1073741515.
```

这个文件的是用yasm编译的，完整的命令在libvpx-v1.3.0\Win32\Debug\vpx\custombuild.command.1.tlog，将这个命令放到MSVC2013的Command Prompt窗口下执行，报缺少libintl-8.dll、libgcc_s_dw2-1.dll、libiconv-2.dll，这3个库在/MinGW/bin下面都有，直接拷贝和yasm放到一起就可以解决问题。

ffmpeg的configure里面的enabled libvpx规则，将其中的-lvpx改成vpxmdd.lib。



