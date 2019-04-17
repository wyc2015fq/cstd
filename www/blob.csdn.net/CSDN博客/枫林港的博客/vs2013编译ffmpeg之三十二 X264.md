# vs2013编译ffmpeg之三十二 X264 - 枫林港的博客 - CSDN博客





2018年07月10日 23:32:19[枫林港](https://me.csdn.net/u013601132)阅读数：357








# X264

对应ffmpeg configure选项–enable-libx264。

## vs2013编译

ShiftMediaProject上有vs2013的H264的工程，[https://github.com/ShiftMediaProject/x264](https://github.com/ShiftMediaProject/x264)，这个工程的编译需要vsyasm，vsyasm和yasm是一起的，电脑上有源代码编译的yasm和vsyasm。将vsyasm.exe拷贝到cl.exe同级目录，vsyasm.props、vsyasm.targets、vsyasm.xml拷贝到C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\BuildCustomizations下面，不然编译的时候会报错。这个网站里面也有ffmpeg对应的vs2013的工程。

cl.exe在MSVC的安装目录下有多个，在编译ffmpeg的环境（msys.bat里面启动C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat）里面可以用which cl命令来确定具体是哪个。

## msys编译

官方的x264没有vs编译的方法，只能用msys+gcc编译：

1、从官网[http://www.videolan.org/developers/x264.html](http://www.videolan.org/developers/x264.html)上下载最新的代码。

解压，configure命令：
`configure --prefix=./build --enable-win32thread --enable-shared --enable-static --enable-strip --extra-ldflags=-Wl,--output-def=libx264.def`
2、编译命令：
`make`
3、 编译出来是.a文件，要生成.lib文件才能被VS引用：
`LIB /MACHINE:X86 /DEF:libx264.def `
生成libx264.lib文件。

ffmpeg config错误：找不到libx264

```
ERROR: libx264 not found



If you think configure made a mistake, make sure you are using the latest

version from Git.  If the latest version fails, report the problem to the

ffmpeg-user@ffmpeg.org mailing list or IRC #ffmpeg on irc.freenode.net.

Include the log file "config.log" produced by configure as this will help

solving the problem.
```

查看config.log文件的最后几行，就是测试x264的，将这些打印里面的语句单独拿出来执行，最后发现在检测的时候，链接缺少符号，而缺少的符号在gcc.lib、mingw32.lib 里面。导致这个问题的原因是x264是用mingw+gcc编译的，用到了mingw的库，所以需要在检测x264的地方加上-lgcc –lmingwex。在configure里面将:
`enabled libx264    && require  libx264 x264.h x264_encoder_encode -aa264 &&`
改成：
`enabled libx264    && require  libx264 x264.h x264_encoder_encode -aa264 -lgcc -lmingwex &&`
并拷贝libgcc.a、libmingwex.a到extra_lib下面，并改名成gcc.lib、mingwex.lib，configure的时候不能识别.a文件。在configure的参数里面加上-LIBPATH:./extra_lib。

x264是用/MT编译的，不知道如何修改，直接忽略LIBCMT.lib，，将/NODEFAULTLIB:LIBCMT放在ffmpeg configure的–extra-ldflags参数里面：
`--extra-ldflags="/NODEFAULTLIB:LIBCMT”`
### 编译错误一

编译ffmpeg3.0的时候，出现下面的错误：

```
./compat/windows/mslink -LIBPATH:../../contribute/MSVC/SDL-1.2.13/VisualC/SDL/Debug -LIBPATH:../../contribute/MSVC/faac-1.28/libfaac/Debug -LIBPATH:../../contribute/MSVC/lame-3.99.5/output/Debug -LIBPATH:../../contribute/MSVC/x264-snapshot-20140315-2245 -LIBPATH:../../contribute/MSVC/libtheora-1.1.1/win32/VS2008/Win32/Debug -LIBPATH:../../contribute/MSVC/openjpeg-1.5.1/bin/Debug -LIBPATH:../../contribute/MSVC/libogg-1.3.1/win32/VS2010/Win32/Debug -LIBPATH:../../contribute/MSVC/libvorbis-1.3.4/win32/VS2010/Win32/Debug -LIBPATH:../../contribute/MSVC/xvidcore/build/win32/bin -LIBPATH:../../contribute/MSVC/opus-1.1/win32/VS2010/Win32/Debug -LIBPATH:../../contribute/MSVC/libvpx-v1.3.0/Win32/Debug -LIBPATH:../../contribute/MSVC/openal-soft-1.15.1/Debug -LIBPATH:../../contribute/MSVC/bzip2-1.0.6/Debug -LIBPATH:../../contribute/MSVC/libass-msvc/lib -LIBPATH:../../contribute/MSVC/libwebp-0.4.0/output/debug-static/x86/lib -LIBPATH:../../contribute/MSVC/x265_20150509/x265/build/vc10-x86/Debug -LIBPATH:../../contribute/MSVC/zvbi-0.2.35/src/.libs -LIBPATH:./extra_lib -nologo -LARGEADDRESSAWARE -IH:/MinGW/msys/1.0/win32/include -libpath:H:/MinGW/msys/1.0/win32/lib -out:./ffconf.YHznHFfc.exe ./ffconf.vBclgQqo.o libx264.lib pthread.lib libwebp_debug.lib -libpath:../../contribute/MSVC/libwebp-0.4.0/output/debug-static/x86/lib vpxmdd.lib vpxmdd.lib vpxmdd.lib vpxmdd.lib libvorbis_static.lib libogg_static.lib libtheora_static.lib libogg_static.lib -libpath:../../contribute/MSVC/opus-1.1//win32/VS2010/Win32/Debug opus.lib openjpeg.lib -DOPJ_STATIC libmp3lame-static.lib libmpghip-static.lib -libpath:../../contribute/MSVC/freetype-2.5.3/objs/win32/vc2010 freetype253_D.lib -libpath:../../contribute/MSVC/fontconfig-2.11.1/src/.libs -libpath:../../contribute/MSVC/freetype-2.5.3/objs/win32/vc2010 fontconfig-1.lib freetype253_D.lib libfaac.lib libass.lib -libpath:../../contribute/MSVC/libass-msvc/lib -libpath:../../contribute/MSVC/fribidi-0.19.6/lib/.libs fribidi-0.dll.lib m.lib psapi.lib advapi32.lib shell32.lib

LINK : warning LNK4044: unrecognized option '/IH:/MinGW/msys/1.0/win32/include'; ignored

LINK : warning LNK4044: unrecognized option '/DOPJ_STATIC'; ignored

ffconf.vBclgQqo.o : error LNK2019: unresolved external symbol _x264_encoder_encode referenced in function _check_x264_encoder_encode

./ffconf.YHznHFfc.exe : fatal error LNK1120: 1 unresolved externals
```

ffmpeg-3.0\pc_files下面没有x264.pc文件，所以pkg-config是从/ win32/lib/pkgconfig下面找到的了x264.pc文件，这个是以前安装的。直接在pc_files下面构造x264.pc。

### 编译错误二

ffmpeg3.0，在configure执行检测x264的时候，会出现下面的错误：

```
LINK : warning LNK4044: unrecognized option '/I../../contribute/MSVC/x264-snapshot-20140315-2245/'; ignored

LINK : warning LNK4044: unrecognized option '/DOPJ_STATIC'; ignored

x264.lib(common.o) : warning LNK4217: locally defined symbol __iob imported in function _x264_log_default

x264.lib(osdep.o) : warning LNK4049: locally defined symbol __iob imported

x264.lib(encoder.o) : error LNK2019: unresolved external symbol _log2f referenced in function _x264_validate_parameters

x264.lib(set.o) : error LNK2001: unresolved external symbol _log2f

x264.lib(ratecontrol.o) : error LNK2001: unresolved external symbol _log2f

x264.lib(analyse.o) : error LNK2001: unresolved external symbol _log2f

x264.lib(encoder.o) : error LNK2019: unresolved external symbol _fseeko64 referenced in function _x264_encoder_frame_end.part.5

x264.lib(common.o) : error LNK2001: unresolved external symbol _fseeko64

x264.lib(opencl.o) : error LNK2001: unresolved external symbol _fseeko64

x264.lib(encoder.o) : error LNK2019: unresolved external symbol _snprintf referenced in function _x264_encoder_frame_end.part.5

x264.lib(ratecontrol.o) : error LNK2019: unresolved external symbol ___mingw_vfprintf referenced in function _fprintf

x264.lib(ratecontrol.o) : error LNK2019: unresolved external symbol ___mingw_vsprintf referenced in function _sprintf.constprop.9

x264.lib(ratecontrol.o) : error LNK2019: unresolved external symbol ___fpclassify referenced in function _get_qscale

./ffconf.prWLQDWJ.exe : fatal error LNK1120: 6 unresolved externals
```

解决的办法是加gcc.lib mingwex.lib，在x264.pc里面的Libs规则里面加：
`Libs: -LIBPATH:${libdir} x264.lib pthread.lib gcc.lib mingwex.lib`


