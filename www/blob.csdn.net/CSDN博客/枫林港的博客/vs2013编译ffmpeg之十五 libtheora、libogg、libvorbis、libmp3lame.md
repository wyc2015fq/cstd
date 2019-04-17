# vs2013编译ffmpeg之十五 libtheora、libogg、libvorbis、libmp3lame - 枫林港的博客 - CSDN博客





2018年07月08日 23:14:04[枫林港](https://me.csdn.net/u013601132)阅读数：364








# libtheora

对应ffmpeg configure选项–enable-libtheora。 

使用的版本是[libtheora-1.2.0alpha1](https://www.theora.org/downloads/)，libtheora-1.2.0alpha1\win32\VS2010下面有工程文件，用MSVC2013打开编译没有出现问题。需要修改工程属性，将libtheora_static工程的c/c++–>代码生成–>运行时库 由“多线程调试 DLL (/MDd)”改成“多线程调试(/MTd)”否则在ffmpeg的configure时加了–enable-libtheora，会导致下面的错：

```
link -DEBUG -LIBPATH:H:/MinGW/msys/1.0/home/AA/contribute/MSVC/SDL-1.2.13/VisualC/SDL/Debug -LIBPATH:H:/MinGW/msys/1.0/home/AA/contribute/MS
VC/faac-1.28/libfaac/Debug -LIBPATH:H:/MinGW/msys/1.0/home/AA/contribute/MSVC/lame-3.99.5/output/Debug -LIBPATH:../../../contribute/MSVC/x264-snapshot-20140315-2245 -LIBPATH:../../
../contribute/MSVC/libtheora-1.1.1/win32/VS2008/Win32/Debug -LIBPATH:../../../contribute/MSVC/openjpeg-2.0.0/bin/Debug -LIBPATH:../../../contribute/MSVC/libogg-1.3.1/win32/VS2010/W
in32/Debug -LIBPATH:../../../contribute/MSVC/libvorbis-1.3.4/win32/VS2010/Win32/Debug -LIBPATH:./extra_lib -nologo -out:test.exe test.o theora_static.lib ogg_static.lib
MSVCRTD.lib(MSVCR100D.dll) : error LNK2005: _malloc already defined in LIBCMT.lib(malloc.obj)
MSVCRTD.lib(MSVCR100D.dll) : error LNK2005: _realloc already defined in LIBCMT.lib(realloc.obj)
MSVCRTD.lib(MSVCR100D.dll) : error LNK2005: _free already defined in LIBCMT.lib(free.obj)
LINK : warning LNK4098: defaultlib 'MSVCRTD' conflicts with use of other libs; use /NODEFAULTLIB:library
test.exe : fatal error LNK1169: one or more multiply defined symbols found
```

另外，ffmpeg的Configure里面，将
`enabled libtheora  && require  libtheora theora/theoraenc.h th_info_init -ltheoraenc -ltheoradec –logg`
改成：
`enabled libtheora  && require  libtheora theora/theoraenc.h th_info_init libtheora_static.lib libogg_static.lib`
由于libtheora-1.1.1在MSVC2010下工程打不开，所以用libtheora-1.2.0alpha1，这个版本可以在MSVC2010下编译通过。

## libogg

libtheora和libvorbis都依赖于libogg。 

官网上下载的[libogg-1.3.1.tar.gz](https://ftp.osuosl.org/pub/xiph/releases/ogg/)，MSVC2013打开libogg-1.3.1\win32\VS2010\ libogg_static.sln编译，编译过程中没有遇到错误。

# libvorbis

对应ffmpeg configure选项–enable-libvorbis。 

下载[libvorbis-1.3.4](https://ftp.osuosl.org/pub/xiph/releases/vorbis/)，MSVC2013打开libvorbis-1.3.4\win32\VS2010\ vorbis_static.sln就可以编译。 

ffmpeg的configure里面将：`enabled libvorbis         && require libvorbis vorbis/vorbisenc.h vorbis_info_init -lvorbisenc -lvorbis –logg`
改成：
`enabled libvorbis  && require  libvorbis vorbis/vorbisenc.h vorbis_info_init libvorbis_static.lib libogg_static.lib`
# libmp3lame

对应ffmpeg configure选项–enable-libmp3lame。 

lame是mp3的编码库，下载lame-3.99.5.tar.gz，用VC2008编译，VC2010编译报错。lame-3.99.5/vc_solution下面有VC工程，直接打开后编译。 

增加mp3支持，ffmpeg的configure里面，将：`enabled libmp3lame && require  "libmp3lame >= 3.98.3" lame/lame.h lame_set_VBR_quality -lmp3lame`
改成：
`enabled libmp3lame && require  "libmp3lame >= 3.98.3" lame/lame.h lame_set_VBR_quality libmp3lame-static.lib libmpghip-static.lib`
下面的错误：
`lame-3.99.5\frontend\gpkplotting.h(25): fatal error C1083: Cannot open include file: 'gtk/gtk.h': No such file or directory`
MSVC2013下，app mp3x工程编译通不过，需要gtk的头文件。不用解决，其他几个可以编译通过就已经生成了库文件。

ffmpeg在configure的时候报下面的错误：
`fatal error C1083: Cannot open include file: 'lame/lame.h': No such file or directory`
Configure里面验证的lame头文件是lame/lame.h，但lame-3.99.5\include没有lame目录，lame目录是lame-3.99.5 make install后生成的，所以直接在lame-3.99.5\include下面建立lame，并把lame.h拷贝到lame目录下。





