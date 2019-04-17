# vs2013编译ffmpeg之二十七 schroedinger - 枫林港的博客 - CSDN博客





2018年07月09日 23:38:27[枫林港](https://me.csdn.net/u013601132)阅读数：62








# schroedinger

`注：ffmpeg-3.4.2中去掉了对libschroedinger的支持。`

schroedinger是VC-2的实现。

## orc

schroedinger-1.0.11依赖于orc先编译orc-0.4.25，下载地址：[https://gstreamer.freedesktop.org/src/orc/](https://gstreamer.freedesktop.org/src/orc/)。解压后在orc-0.4.25目录下执行：
`mkdir msys_build; ./configure --prefix=$(pwd)/msys_build; make; make install`
编译的结果就在orc-0.4.25/ msys_build目录下。运行的时候需要将liborc-0.4-0.dll拷贝到ffmpeg.exe同级目录下，schroedinger依赖于这个库。

## 编译schroedinger

从官网上下载[schroedinger-1.0.11.tar.gz](https://sourceforge.net/projects/schrodinger/?source=directory)，解压后在schroedinger-1.0.11目录下执行：

```bash
mkdir msys_build; export PKG_CONFIG_PATH=../orc-0.4.25/msys_build/lib/pkgconfig/; ./configure --prefix=$(pwd)/msys_build; make; make install
```

### 编译错误一

configure文件需要修改，不然编译会报下面的错误：

```
make[2]: Entering directory `/home/AA/ffmpeg_MSVC/contribute/MSVC/schroedinger-1.0.11/schroedinger'

../orc-0.4.25/msys_build/bin/orcc.exe  --inline --implementation -o tmp-orc.c ./schroorc.orc

make[2]: ../orc-0.4.25/msys_build/bin/orcc.exe: Command not found

make[2]: *** [tmp-orc.c] Error 127

make[2]: Leaving directory `/home/AA/ffmpeg_MSVC/contribute/MSVC/schroedinger-1.0.11/schroedinger'

make[1]: *** [all-recursive] Error 1

make[1]: Leaving directory `/home/AA/ffmpeg_MSVC/contribute/MSVC/schroedinger-1.0.11'

make: *** [all] Error 2
```

出错的目录是schroedinger-1.0.11/schroedinger，orcc.exe的路径不对，查orc的.pc文件orc-0.4.25\msys_build\lib\pkgconfig\orc-0.4.pc，里面的路径是绝对路径：
`prefix=/home/AA/ffmpeg_MSVC/contribute/MSVC/orc-0.4.25/msys_build`
路径是正确的，可能是pkg-config的时候出的问题。从schroedinger-1.0.11/config.log找到检测ORC时的打印：

```
configure:16425: checking for ORC

configure:16433: PKG_CONFIG --exists --print-errors "orc-0.4 >= ORC_VER"

configure:16436: $? = 0

configure:16451: PKG_CONFIG --exists --print-errors "orc-0.4 >= ORC_VER"

configure:16454: $? = 0

configure:16490: result: yes
```

从configure的第16425行开始检测ORC，分别在16433、16451执行了pkg-config命令：

```
configure:16425: checking for ORC

configure:16433: PKG_CONFIG --exists --print-errors "orc-0.4 >= ORC_VER"

configure:16436: $? = 0

configure:16451: PKG_CONFIG --exists --print-errors "orc-0.4 >= ORC_VER"
```

将这个pkg-config命令单独拿出来执行，$PKG_CONFIG就是pkg-config，ORC_VER在configure里面有定义，值为0.4.16，则单独的命令为：
`export PKG_CONFIG_PATH=../orc-0.4.25/msys_build/lib/pkgconfig/; pkg-config --debug --exists --print-errors "orc-0.4 >= 0.4.16"`
pkg-config要加`--debug`参数，不然没有打印。pkg-config的打印：

```
Reading 'orc-0.4' from file '../orc-0.4.25/msys_build/lib/pkgconfig/\orc-0.4.pc'

Parsing package file '../orc-0.4.25/msys_build/lib/pkgconfig/\orc-0.4.pc'

  line>prefix=/home/AA/ffmpeg_MSVC/contribute/MSVC/orc-0.4.25/msys_build

 Variable declaration, 'prefix' overridden with '../orc-0.4.25/msys_build'

  line>exec_prefix=${prefix}

 Variable declaration, 'exec_prefix' has value '../orc-0.4.25/msys_build'
```

从打印里面可以看出prefix变量被../orc-0.4.25/msys_build覆盖。所以路径都不正确了。上网搜了一下“Variable declaration, ‘prefix’ overridden with”，[https://github.com/peterbraden/node-opencv/issues/58](https://github.com/peterbraden/node-opencv/issues/58)，pkg-config可以加参数`--dont-define-prefix`来指示不需要覆盖prefix变量。从schroedinger-1.0.11/Makefile里面可以看出，ORCC、ORC_CFLAGS、ORC_LIBS三个变量的值里面的路径被覆盖了。

在configure里面找到这三个变量赋值的地方，分别是：

```
ORCC=$PKG_CONFIG --variable=orcc orc-0.4

pkg_cv_ORC_CFLAGS=$PKG_CONFIG --cflags "orc-0.4 >= $ORC_VER" 2>/dev/null

pkg_cv_ORC_LIBS=$PKG_CONFIG --libs "orc-0.4 >= $ORC_VER" 2>/dev/null
```

改成：

```
ORCC=$PKG_CONFIG --debug --dont-define-prefix --variable=orcc orc-0.4

pkg_cv_ORC_CFLAGS=$PKG_CONFIG --debug --dont-define-prefix --cflags "orc-0.4 >= $ORC_VER" 2>/dev/null

pkg_cv_ORC_LIBS=$PKG_CONFIG --debug --dont-define-prefix --libs "orc-0.4 >= $ORC_VER" 2>/dev/null
```

重新configure再make就可以解决问题。prefix为何被覆盖，原因不清楚。

### 编译错误二

schroedinger-1.0.11编译到testsuite目录的时候会报错，缺_orc_code_orc_interleave2_s16、_orc_code_orc_deinterleave2_s16这两个符号，直接在wavelet_max_gain.c里面加上这两个符号的定义就可以解决：

```
OrcCode *_orc_code_orc_interleave2_s16;

OrcCode *_orc_code_orc_deinterleave2_s16;
```

## 编译错误三

在编译ffmpeg-3.3的时候会报下面的错误：

```
libschroedingerenc.c

C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\INCLUDE\math.h(515) : error C2059: 语法错误:“(”
```

这个是加了schroedinger后才出现的，math.h 515行是rint函数的定义，在schroedinger源代码下面直接搜索rint，发现rint已经有定义：

```
if defined(_MSC_VER)

define rint(x) (floor((x) + 0.5))

#endif
```

直接将其注释掉就可以了，并重新编译schroedinger。



