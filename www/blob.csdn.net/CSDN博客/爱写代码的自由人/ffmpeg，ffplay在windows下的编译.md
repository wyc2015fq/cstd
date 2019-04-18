# ffmpeg，ffplay在windows下的编译 - 爱写代码的自由人 - CSDN博客





2018年03月03日 12:05:01[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：662








      由于FFMpeg是基于Linux开发的开源项目，源代码和Windows下最常见的Visual Studio提供的C/C++编译器不兼容，因此它不能使用MSVC++编译。要想使用FFMpeg，最先要解决的问题就是在Windows下配置一个类似Linux的编译环境，将FFMpeg编译为二进制库以后，再利用其进行进一步开发。
　　MSVC++并不严格的遵循C标准，所以整个编译过程必须使用MSys+MinGW系统来完成。如果选择编译动态链接库(dll)，这些库将和其他的动态链接库一样在使用上没什么差别，可以使用MSVC++来链接这些库到自己的程序，只有这种情况下MSVC++是被支持的。
　　FFmpeg在Windows系统下的编译过程如下：

       1.下载 MSYS-1.0.10.exe ， msysDTK-1.0.1.exe，MinGW-5.1.4.exe

       2. 依次安装上述软件：

           注意：安装MinGW时，选择download and install，下两步之后选择Candidate，不要选Current。选择MinGW base tools (autoselected), g++ compiler, MinGW make; 安装路径为c:\mingw。

      3. 升级下MSYS

[](http://downloads.sourceforge.net/mingw/msysCORE-1.0.11-2007.01.19-1.tar.bz2)[http://downloads.sourceforge.net/mingw/msysCORE-1.0.11-2007.01.19-1.tar.bz2](http://downloads.sourceforge.net/mingw/msysCORE-1.0.11-2007.01.19-1.tar.bz2)

解压这个文件，将bin/mount.exe, msys-1.0.dll, ps.exe三个文件copy到c:/msys/bin下面覆盖

3. 升级下MSYS的BASH

[](http://prdownloads.sourceforge.net/mingw/bash-3.1-MSYS-1.0.11-snapshot.tar.bz2)[http://prdownloads.sourceforge.net/mingw/bash-3.1-MSYS-1.0.11-snapshot.tar.bz2](http://prdownloads.sourceforge.net/mingw/bash-3.1-MSYS-1.0.11-snapshot.tar.bz2)

解压这个文件，将bin/bash.exe, sh.exe两个文件copy到c:/msys/bin

6. 配置fstab

copy C:/msys/etc/fstab.sample 成 fstab，确保以下对应：

#Win32_Path    Mount_Point

c:/mingw         /mingw

7. 升级make

删除c:/msys/bin/make.exe

copy c:/mingw/bin/mingw32-make.exe 到 c:/msys/bin/ 并把mingw32-make.exe改成make.exe。

MSYS中输入make -v显示的版本为3.81。 

8. 升级gcc(本人测试的4.3.2编译时有问题，通不过，可以选择4.2.2，4.3.3，或更高的版本)

[http://cdnetworks-kr-2.dl.sourceforge.net/project/tdm-gcc/TDM-GCC%204.3%20series/4.3.3-tdm-1%20SJLJ/gcc-4.3.3-tdm-1-core.tar.gz](http://cdnetworks-kr-2.dl.sourceforge.net/project/tdm-gcc/TDM-GCC%204.3%20series/4.3.3-tdm-1%20SJLJ/gcc-4.3.3-tdm-1-core.tar.gz)[](http://ftp3.ie.freebsd.org/pub/sourceforge/t/td/tdm-gcc/gcc-4.3.2-tdm-1-core.zip)

解压到c:/mingw，覆盖原来的。

MSYS中输入gcc -v显示的版本为4.3.2。

9. 升级W32API

[](http://downloads.sourceforge.net/mingw/w32api-3.13-mingw32-dev.tar.gz?modtime=1228514514&big_mirror=1)[http://downloads.sourceforge.net/mingw/w32api-3.13-mingw32-dev.tar.gz?modtime=1228514514&big_mirror=1](http://downloads.sourceforge.net/mingw/w32api-3.13-mingw32-dev.tar.gz?modtime=1228514514&big_mirror=1)

解压到c:/mingw，覆盖原来的。

10. 添加SDL

[](http://www.libsdl.org/release/SDL-devel-1.2.11-mingw32.tar.gz)[http://www.libsdl.org/release/SDL-devel-1.2.11-mingw32.tar.gz](http://www.libsdl.org/release/SDL-devel-1.2.11-mingw32.tar.gz)

解压该软件包；

将lib下的所有文件复制到C:\msys\mingw\lib；

将include下的所有文件复制到C:\msys\mingw\include；

将bin下的i386-mingw32msvc-sdl-config复制到C:\msys\mingw\bin，并重命名为sdl-config；

将bin下的SDL.dll复制到C:\msys\mingw\bin

[http://sourceforge.net/projects/mingw/files/MSYS%20coreutils/coreutils-5.97-2/](http://sourceforge.net/projects/mingw/files/MSYS%20coreutils/coreutils-5.97-2/)

[http://sourceforge.net/projects/mingw/files/MSYS%20coreutils/coreutils-5.97-2/coreutils-5.97-2-msys-1.0.11-src.tar.lzma/download](http://sourceforge.net/projects/mingw/files/MSYS%20coreutils/coreutils-5.97-2/coreutils-5.97-2-msys-1.0.11-src.tar.lzma/download)

安装 coreutils-5.97-MSYS-1.0.11-snapshot.tar.bz2，下载后解压至msys下，覆盖

           在 MSYS-1.0.10\msys.bat中第一行加入下面这句话

           call "C:\Program Files\Microsoft Visual Studio 8\VC\bin\vcvars32.bat"

           就是你VC的vcvars32.bat所在路径

至此，整体上的编译环境就搭建好了。 

11. 编译FFmpeg

[](http://ffmpeg.mplayerhq.hu/ffmpeg-checkout-snapshot.tar.bz2)[http://ffmpeg.mplayerhq.hu/ffmpeg-checkout-snapshot.tar.bz2](http://ffmpeg.mplayerhq.hu/ffmpeg-checkout-snapshot.tar.bz2)

copy到c:/msys/home/your_name下面。

解压：tar xjvf ffmpeg-checkout-snapshot.tar.bz2。

修改configure的226行和2222行，将pr改成lpr。

（如果安装过coreutils-5.97-MSYS-1.0.11-snapshot.tar.bz2，则不需要更改）

vi一个文件ffmpeg-conf.sh，内容为：

SDL_CFLAGS=/mingw/include

SDL_LIBS=/mingw/lib

./configure --enable-ffplay --enable-memalign-hack

（msys不能进行编辑，自己想办法copy进去吧。(*^__^*) 嘻嘻……）

执行./ffmpeg-conf.sh。

执行make，应该不会有错误的。

执行ffplay filename就可以播放了。

compile method:
configure option

--enable-memalign-hack --disable-decoders --disable-encoders --disable-muxers --disable-demuxers --disable-parsers --disable-protocols --disable-bsfs --disable-devices 
--disable-filters --disable-network --disable-ipv6 --disable-mpegaudio-hp --disable-debug --enable-small --enable-decoder=h264

configure --enable-shared --disable-static --enable-memalign-hack --extra-cflags="-fno-common" --prefix=d:/TEST/ffmpegtest



11. 写在编译完之后

Jeremiah只是对ffmpeg进行了简单的编译，如果需要其他的功能还是去google吧。

编译完了应该会出三个可执行文件：ffmpeg.exe, ffplay.exe, ffserver.exe。

单独使用ffplay，需要将c:/mingw/bin/SDL.dll和ffplay.exe放到相同的目录下。

**提醒一下：我移植了ffplay工程到VC环境中，可以用VC编译和调试，看我的下一篇博文。**



