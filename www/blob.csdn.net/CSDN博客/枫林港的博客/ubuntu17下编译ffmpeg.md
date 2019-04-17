# ubuntu17下编译ffmpeg - 枫林港的博客 - CSDN博客





2018年07月24日 23:02:25[枫林港](https://me.csdn.net/u013601132)阅读数：153








# ubuntu下编译ffmpeg

ubuntu镜像服务器上的ffmpeg版本是2.4.3的，下面描述ffmpeg-3.3.2的编译情况。


- [ubuntu下编译ffmpeg](#ubuntu下编译ffmpeg)- [1. 安装第三方库](#1-安装第三方库)- [1>. Ubuntu14.04](#1-ubuntu1404)
- [2>. Ubuntu17](#2-ubuntu17)

- [2. 安装shine](#2-安装shine)
- [3. 编译lame](#3-编译lame)
- [4. 编译x264](#4-编译x264)- [1) 编译nasm](#1-编译nasm)
- [2) 编译x264](#2-编译x264)

- [5. 编译x265](#5-编译x265)
- [6. libopencv](#6-libopencv)
- [7. openjpeg](#7-openjpeg)
- [8. 编译ffmpeg](#8-编译ffmpeg)
- [9. 对比测试](#9-对比测试)




## 1. 安装第三方库

### 1>. Ubuntu14.04

> 
@ubuntu:~/study/ffmpeg/contribute/nasm-2.13.01$ `uname -a`

Linux ubuntu 4.4.0-31-generic #50~14.04.1-Ubuntu SMP Wed Jul 13 01:07:32 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux

@ubuntu:~/study/ffmpeg/contribute/nasm-2.13.01$ `gcc --version`

gcc (Ubuntu/Linaro 4.4.7-8ubuntu1) 4.4.7

Copyright (C) 2010 Free Software Foundation, Inc.

This is free software; see the source for copying conditions.  There is NO

warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


在执行`sudo apt-get install`之前执行一下`sudo apt-get update; sudo apt-get upgrade`。下面列的都是基于后面ffmpeg configure命令所需要安装的第三方库：

```bash
sudo apt-get install yasm dos2unix

sudo apt-get install build-essential  libtool libpcre3 libpcre3-dev libpcrecpp0 libssl-dev zlib1g-dev  libxml2 libxml2-dev libxml2-utils  libxslt1-dbg libxslt1-dev  libxslt1.1   libgd2-xpm libgd2-xpm-dev  libgeoip-dev

sudo apt-get install frei0r-plugins-dev frei0r-plugins libgnutls-dev  gnutls-bin ladspa-sdk libiec61883-0 libiec61883-dev libavc1394-0 libavc1394-dev libass-dev libbluray-dev libbs2b-dev  libbs2b0 libflite1 flite flite1-dev libgme-dev libgme0 libgsm1  libgsm1-dev libmodplug1  libmodplug-dev libopencv-core2.4 libopencv-dev libopencv-core-dev libopenjpeg2 libopenjpeg-dev libopus0  libopus-dev librtmp0  librtmp-dev libschroedinger-1.0-0 libschroedinger-dev  libshine2 libshine-dev libshine3  libsoxr0  libsoxr-lsr0  libsoxr-dev libssh-4 libssh-dev libspeex1 libspeex-dev libspeexdsp1 libspeexdsp-dev libtheora0 libtheora-dev libtwolame0 libtwolame-dev 

sudo apt-get install libvorbis0a libvorbis-dev libvpx1 libvpx-dev libwavpack1 libwavpack-dev libwebp2 libwebp-dev libxvidcore4 libxvidcore-dev libzvbi0 libzvbi-dev libzmq3 libzmq3-dev  libopenal1 libopenal-dev libopencv-core2.4 libopencv-dev libopencv-gpu2.4 libopencv-gpu-dev libopencv-gpu2.3 cmake cmake-doc cmake-curses-gui cmake-qt-gui

sudo apt-get install libcdio13 libcdio-dev libcdio-cdda1 libcdio-cdda-dev libcdparanoia0 libcdparanoia-dev libcdio-paranoia1 libcdio-paranoia-dev libcaca0 libcaca-dev libfreetype6 libfreetype6-dev libfribidi0 libfribidi-dev libpulse0 libpulse-dev

sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev freeglut3
```

### 2>. Ubuntu17

ubuntu 17 里面没有libgd2-xpm、schroedinger、libshine2，libopenjpeg2等要换成`libopenjp2-7`，`libopencv-core2.4`要自己下载代码编译。libvpx1换成`libvpx4`。

```bash
sudo apt-get install yasm libpcrecpp0

sudo apt-get install build-essential  libtool libpcre3 libpcre3-dev libssl-dev zlib1g-dev  libxml2 libxml2-dev libxml2-utils  libxslt1-dbg libxslt1-dev  libxslt1.1 libgd2-xpm-dev  libgeoip-dev

sudo apt-get install frei0r-plugins-dev frei0r-plugins ladspa-sdk libiec61883-0 libiec61883-dev libavc1394-0 libavc1394-dev libass-dev libbluray-dev libbs2b-dev  libbs2b0 libflite1 flite flite1-dev libgme-dev libgme0 libgsm1  libgsm1-dev libmodplug1  libmodplug-dev libopenjp2-7 libopenjp2-7-dev libopus0  libopus-dev  libshine-dev libshine3  libsoxr0  libsoxr-lsr0  libsoxr-dev libssh-4 libssh-dev libspeex1 libspeex-dev libspeexdsp1 libspeexdsp-dev libtheora0 libtheora-dev libtwolame0 libtwolame-dev 

sudo apt-get install librtmp0 librtmp-dev    //有安装顺序，前面的装完了才能安装



sudo apt-get install libvorbis0a libvorbis-dev libvpx4 libvpx-dev libwavpack1 libwavpack-dev libwebp6 libwebp-dev libxvidcore4 libxvidcore-dev libzvbi0 libzvbi-dev libzmq5 libzmq3-dev  libopenal1 libopenal-dev cmake cmake-doc cmake-curses-gui cmake-qt-gui



sudo apt-get install libcdio13 libcdio-dev libcdio-cdda1 libcdio-cdda-dev libcdparanoia0 libcdparanoia-dev libcdio-paranoia1 libcdio-paranoia-dev libcaca0 libcaca-dev libfreetype6 libfreetype6-dev libfribidi0 libfribidi-dev libpulse0 libpulse-dev

sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev freeglut3 dos2unix libsdl2-dev libsdl2-2.0-0
```

ubuntu 17里面没有schroedinger，要添加ubuntu 16的镜像源，/etc/apt/sources.list里面加上:

> 
deb [http://cz.archive.ubuntu.com/ubuntu](http://cz.archive.ubuntu.com/ubuntu) xenial main universe   


然后更新一下：

```bash
sudo apt-get update
sudo apt-get install libschroedinger-1.0-0 libschroedinger-dev
```

## 2. 安装shine

ffmpeg需要shine3，镜像服务器上没有shine3，直接下载二进制文件安装。在：

[https://launchpad.net/ubuntu/zesty/amd64/libshine3/3.1.0-5](https://launchpad.net/ubuntu/zesty/amd64/libshine3/3.1.0-5)

[https://launchpad.net/ubuntu/artful/amd64/libshine-dev/3.1.0-5](https://launchpad.net/ubuntu/artful/amd64/libshine-dev/3.1.0-5)

下载libshine3_3.1.0-5_amd64.deb、libshine-dev_3.1.0-5_amd64.deb，用`sudo dpkg -i`命令安装。

## 3. 编译lame

ffmpeg-3.3.2 要求 libmp3lame >= 3.98.3，用apt-get安装的ubuntu镜像服务器上的lame版本太低，需要自己编译。从[https://sourceforge.net/projects/lame/files/lame/3.99/](https://sourceforge.net/projects/lame/files/lame/3.99/)下载lame-3.99.5.tar.gz，解压后编译：

```bash
./configure --enable-nasm; make; sudo make install
```

## 4. 编译x264

从[http://www.videolan.org/developers/x264.html](http://www.videolan.org/developers/x264.html)下载最新的版本x264-snapshot。

### 1) 编译nasm

ubuntu镜像服务器上的nasm版本太低，先编译nasm，从[http://www.linuxfromscratch.org/blfs/view/svn/general/nasm.html](http://www.linuxfromscratch.org/blfs/view/svn/general/nasm.html)下载最新的版本nasm-2.13.01.tar.bz2：

```bash
./configure --enable-sections --enable-lto; make; sudo make install
```

不要使用`--enable-werror`选项，此选项将警告当成错误，有几个地方会编译不过。

在某个ubuntu服务器上，make报下面的错误：

> 
~/nasm-2.13.01/asm/parser.c:448: undefined reference to `_Static_assert’


直接注释掉出错的这行：nasm_static_assert(P_none == 0);

nasm_static_assert宏定义为static_assert，是编译时的断言，不知道为何这个版本的ubuntu版本的gcc直接将这个看成了函数调用，有可能是gcc版本差异造成的。

### 2) 编译x264

```bash
./configure  --enable-shared --enable-static; make; sudo make install
```

configure打印：

> 
ubuntu14:~/ffmpeg/contribute/x264-snapshot$ ./configure –enable-shared –enable-static

Warning: AV_PIX_FMT_FLAG_RGB is missing from libavutil, update for swscale support

platform:      X86_64

byte order:    little-endian

system:        LINUX

cli:           yes

libx264:       internal

shared:        no

static:        no

asm:           yes

interlaced:    yes

avs:           avxsynth

lavf:          no

ffms:          no

mp4:           no

gpl:           yes

thread:        posix

opencl:        yes

filters:       crop select_every 

lto:           no

debug:         no

gprof:         no

strip:         no

PIC:           no

bit depth:     8

chroma format: all


## 5. 编译x265

在windows下用msys下载：

```bash
hg clone http://hg.videolan.org/x265
```

然后拷贝到ubuntu下。编译x265需要安装cmake，前面已经安装了。ubuntu下编译：

```bash
cd build\linux; ./ make-Makefiles.bash
```

随后会弹出字符图形界面进行配置，用回车打开了ENABLE_AGGRESSIVE_CHECKS、ENABLE_DYNAMIC_HDR10两项，然后按g键可以保存退出。

```bash
make; sudo make install
```

但这样编译出来的x265跑不起来：

> 
ubuntu14: x265_master/build/linux$ `x265 --version`

x265: error while loading shared libraries: libx265.so.121: cannot open shared object file: No such file or directory


用ldd查看库依赖：

> 
ubuntu14: /x265_master/build/linux$ ldd `which x265`

​        linux-vdso.so.1 =>  (0x00007fff6c05b000)

​        libx265.so.121 => not found

​        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f12b53f8000)

​        libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f12b50f3000)

​        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f12b4edd000)

​        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f12b4b15000)

​        /lib64/ld-linux-x86-64.so.2 (0x00007f12b5646000)

​        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f12b480e000)


查看x265编译时make install的打印有– Installing: /usr/local/lib/libx265.so.121，所以此库在/usr/local/lib/下，但这个路径没有在库的搜索路径列表里面，所以找不到。如何找到库的搜索路径，可以利用LD_DEBUG 命令，LD_DEBUG 是 glibc 中的 loader 为了方便自身调试而设置的一个环境变量。通过设置这个环境变量，可以方便的看到 loader 的加载过程。

> 
ubuntu14: x265_master/build/linux$ `LD_DEBUG=help ls`

Valid options for the LD_DEBUG environment variable are:

libs        display library search paths

reloc       display relocation processing

files       display progress for input file

symbols     display symbol table processing

bindings    display information about symbol binding

versions    display version dependencies

scopes      display scope information

all         all previous options combined

statistics  display relocation statistics

unused      determined unused DSOs

help        display this help message and exit

To direct the debugging output into a file instead of standard output

a filename can be specified using the LD_DEBUG_OUTPUT environment variable.

AA@ubuntu14:~/ffmpeg/contribute/x265_master/build/linux$ LD_DEBUG=libs x265 -v

​     13598:     find library=libx265.so.121 [0]; searching

​     13598:      search cache=/etc/ld.so.cache

​     13598:      search path=/lib/x86_64-linux-gnu/tls/x86_64:/lib/x86_64-linux-gnu/tls:/lib/x86_64-linux-gnu/x86_64:/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu/tls/x86_64:/usr/lib/x86_64-linux-gnu/tls:/usr/lib/x86_64-linux-gnu/x86_64:/usr/lib/x86_64-linux-gnu:/lib/tls/x86_64:/lib/tls:/lib/x86_64:/lib:/usr/lib/tls/x86_64:/usr/lib/tls:/usr/lib/x86_64:/usr/lib            (system search path)

​     13598:       trying file=/lib/x86_64-linux-gnu/tls/x86_64/libx265.so.121

​     13598:       trying file=/lib/x86_64-linux-gnu/tls/libx265.so.121

​     13598:       trying file=/lib/x86_64-linux-gnu/x86_64/libx265.so.121

​     13598:       trying file=/lib/x86_64-linux-gnu/libx265.so.121

​     13598:       trying file=/usr/lib/x86_64-linux-gnu/tls/x86_64/libx265.so.121

​     13598:       trying file=/usr/lib/x86_64-linux-gnu/tls/libx265.so.121

​     13598:       trying file=/usr/lib/x86_64-linux-gnu/x86_64/libx265.so.121

​     13598:       trying file=/usr/lib/x86_64-linux-gnu/libx265.so.121

​     13598:       trying file=/lib/tls/x86_64/libx265.so.121

​     13598:       trying file=/lib/tls/libx265.so.121

​     13598:       trying file=/lib/x86_64/libx265.so.121

​     13598:       trying file=/lib/libx265.so.121

​     13598:       trying file=/usr/lib/tls/x86_64/libx265.so.121

​     13598:       trying file=/usr/lib/tls/libx265.so.121

​     13598:       trying file=/usr/lib/x86_64/libx265.so.121

​     13598:       trying file=/usr/lib/libx265.so.121

​     13598:

x265: error while loading shared libraries: libx265.so.121: cannot open shared object file: No such file or directory

ubuntu14:~/ffmpeg/contribute/x265_master/build/linux$


解决的办法是用ldconfig命令将/usr/local/lib加到库的搜索路径列表里面：

> 
ubuntu14: x265_master/build/linux$`cd /etc/ld.so.conf.d`

ubuntu14:/etc/ld.so.conf.d$ `ll`

drwxr-xr-x   2 root root  4096  68 10:44 ./

drwxr-xr-x 163 root root 12288  68 14:35 ../

-rw-rw-r–   1 root root    38  34  2014 fakeroot-x86_64-linux-gnu.conf

lrwxrwxrwx   1 root root    40  52 12:04 i386-linux-gnu_GL.conf -> /etc/alternatives/i386-linux-gnu_gl_conf

-rw-r–r–   1 root root   108  32 03:40 i686-linux-gnu.conf

-rw-r–r–   1 root root    44  80  2009 libc.conf

-rw-r–r–   1 root root    68  42  2014 x86_64-linux-gnu.conf

lrwxrwxrwx   1 root root    43  52 10:57 x86_64-linux-gnu_EGL.conf -> /etc/alternatives/x86_64-linux-gnu_egl_conf

lrwxrwxrwx   1 root root    42  52 10:57 x86_64-linux-gnu_GL.conf -> /etc/alternatives/x86_64-linux-gnu_gl_conf

-rw-r–r–   1 root root    56  32 03:22 zz_i386-biarch-compat.conf

-rw-r–r–   1 root root    58  32 03:23 zz_x32-biarch-compat.conf

AA@ubuntu14:/etc/ld.so.conf.d$ `sudo vi usr_local_lib.conf`


vi编辑usr_local_lib.conf，文件内容为/usr/local/lib。最后执行一下`sudo ldconfig`。

> 
AA@ubuntu14:/etc/ld.so.conf.d$`cat usr_local_lib.conf`

/usr/local/lib

AA@ubuntu14:/etc/ld.so.conf.d$`sudo ldconfig`


这样x265就可以执行了。

## 6. libopencv

ubuntu17 默认安装的libopencv版本是3.4（[https://github.com/opencv/opencv/pull/6586](https://github.com/opencv/opencv/pull/6586) 有workaround），此版本不支持C语言，ffmpeg-3.3.2在configure的时候报下面的错误：

> 
undefined reference to `cvRound’


只能用2.4.13.5（23 Dec 2017）版本的，自己编译，在opencv-2.4.13.5目录下：

```bash
mkdir build; cd build/; cmake ..; make -j 2; sudo make install
```

## 7. openjpeg

ubuntu 17上默认安装的openjpeg版本是2.2的，ffmpeg-3.3.2只支持openjpeg-2.1，在configure的时候会在检测openjpeg的时候失败。解决的办法是直接将/usr/include 下面的openjpeg-2.2改成openjpeg-2.1。

若是修改configure里面的openjpeg检测规则，将openjpeg-2.1改成openjpeg-2.2，这样会有问题，configure后的config.h里面HAVE_OPENJPEG_2_1_OPENJPEG_H定义成0了，最终编译的时候会报错。

## 8. 编译ffmpeg

从官网上下载ffmpeg-3.3.2，configure命令参考ubuntu镜像服务器上的ffmpeg的，configure命令如下：

```bash
./configure --prefix=$(pwd)/install_dir --extra-version='1ubuntu1~trusty6' --build-suffix=-ffmpeg --toolchain=hardened --extra-cflags= --extra-cxxflags= --libdir=/usr/lib/x86_64-linux-gnu --shlibdir=/usr/lib/x86_64-linux-gnu --incdir=/usr/include/x86_64-linux-gnu --enable-gpl --enable-shared --disable-stripping --enable-avresample --enable-avisynth --enable-fontconfig --enable-gnutls --enable-ladspa --enable-libass --enable-libbluray --enable-libbs2b --enable-libcaca --enable-libcdio --enable-libflite --enable-libfreetype --enable-libfribidi --enable-libgme --enable-libgsm --enable-libmodplug --enable-libmp3lame --enable-libopenjpeg --enable-libopus --enable-libpulse --enable-librtmp --enable-libschroedinger --enable-libshine --enable-libspeex --enable-libssh --enable-libtheora --enable-libtwolame --enable-libvorbis --enable-libvpx --enable-libwavpack --enable-libwebp --enable-opengl --enable-libxvid --enable-libx265 --enable-libdc1394 --enable-libiec61883 --enable-libzvbi --enable-libzmq --enable-frei0r --enable-libx264 --enable-libsoxr --enable-openal --enable-libopencv
```

然后编译：

```bash
make; sudo make install
```

将ffmpeg可执行文件的路径加入环境变量：
`vi .bashrc`
在最后加上：

```bash
export PATH="PATH:(pwd)/study/ffmpeg/install_dir/bin"
```

$(pwd)不能用~代替，路径必须生成绝对路径，否则找不到nasm。

## 9. **对比测试**

由于电脑上用ffmpeg重编码速度不快，所以想用服务器来跑ffmpeg。两个机器配置：

***Pc：***

> 
Intel i5-6500 CPU@3.2GHZ  8GB DDR4  win7 64位


转码命令：

```bash
ffmpeg -i  input.mp4 -c:v libx264  -c:a aac -cutoff 15000 -ab 95k -strict -2 -async 1 output.mp4
```

X264用ffmpeg默认的参数：

> 
[libx264 @ 06901AA0] using SAR=1/1

[libx264 @ 06901AA0] using cpu capabilities: MMX2 SSE2Fast SSSE3 SSE4.2 AVX FMA3 AVX2 LZCNT BMI2 UnalignedStack

[libx264 @ 06901AA0] profile High, level 3.1

[libx264 @ 06901AA0] 264 - core 148 r2699 - H.264/MPEG-4 AVC codec - Copyleft 2003-2016 - [http://www.videolan.org/x264.html](http://www.videolan.org/x264.html) - options: cabac=1 ref=3 deblock=1:0:0 analyse=0x3:0x113 me=hex subme=7 psy=1 psy_rd=1.00:0.00 mixed_ref=1 me_range=16 chroma_me=1 trellis=1 8x8dct=1 cqm=0 deadzone=21,11 fast_pskip=1 chroma_qp_offset=-2 threads=6 lookahead_threads=1 sliced_threads=0 nr=0 decimate=1 interlaced=0 bluray_compat=0 constrained_intra=0 bframes=3 b_pyramid=2 b_adapt=1 b_bias=0 direct=1 weightb=1 open_gop=0 weightp=2 keyint=250 keyint_min=25 scenecut=40 intra_refresh=0 rc_lookahead=40 rc=crf mbtree=1 crf=23.0 qcomp=0.60 qpmin=0 qpmax=69 qpstep=4 ip_ratio=1.40 aq=1:1.00


Pc机编码速度47～50帧/秒。

***服务器：***

> 
Ubuntu 14.04.1 Intel(R) Xeon(R) CPU E7-4850 v2 @ 2.30GHz  96核  198G内存


转码命令：

```bash
ffmpeg -i  input.mp4 -threads 16 -c:v libx264  -c:a aac -cutoff 15000 -ab 95k -strict -2 -async 1 output.mp4
```

X264用ffmpeg默认的参数：

> 
[libx264 @ 0x562456fd6080] using SAR=1/1

[libx264 @ 0x562456fd6080] using cpu capabilities: MMX2 SSE2Fast SSSE3 SSE4.2 AVX

[libx264 @ 0x562456fd6080] profile High, level 3.1

[libx264 @ 0x562456fd6080] 264 - core 152 - H.264/MPEG-4 AVC codec - Copyleft 2003-2017 - [http://www.videolan.org/x264.html](http://www.videolan.org/x264.html) - options: cabac=1 ref=3 deblock=1:0:0 analyse=0x3:0x113 me=hex subme=7 psy=1 psy_rd=1.00:0.00 mixed_ref=1 me_range=16 chroma_me=1 trellis=1 8x8dct=1 cqm=0 deadzone=21,11 fast_pskip=1 chroma_qp_offset=-2 threads=16 lookahead_threads=2 sliced_threads=0 nr=0 decimate=1 interlaced=0 bluray_compat=0 constrained_intra=0 bframes=3 b_pyramid=2 b_adapt=1 b_bias=0 direct=1 weightb=1 open_gop=0 weightp=2 keyint=250 keyint_min=25 scenecut=40 intra_refresh=0 rc_lookahead=40 rc=crf mbtree=1 crf=23.0 qcomp=0.60 qpmin=0 qpmax=69 qpstep=4 ip_ratio=1.40 aq=1:1.00


服务器编码54帧/秒。

从测试结果来看，两个机器编码速度差异不大，ffmpeg都是用release版本。有可能是ubuntu上的x264多线程没有真正打开，或打开的方法不对。



