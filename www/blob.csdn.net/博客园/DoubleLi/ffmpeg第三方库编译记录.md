# ffmpeg第三方库编译记录 - DoubleLi - 博客园






最近在研究ffmpeg的编译，之前使用的Ubuntu，需要安装虚拟机，非常麻烦，所以后来改研究在Windows平台编译。

一开始遇到很多挫折，参考了网上很多的帖子，但要么不全要么内容已过期，经过我的反复试验或研究，最终搞定了。在此记录一下，也希望能对编译ffmpeg的朋友们有所帮助。

首先安装Mingw和Msys平台，可以参考我其他几篇转载的帖子，基本都不会有问题

然后从ffmpeg官网下载源码，可以选择最新的[Git](http://lib.csdn.net/base/git)[测试](http://lib.csdn.net/base/softwaretest)版或稳定版（我下载的是ffmpeg-1.1），编译方式都大同小异，如果不需要第三方库的话，编译选项如下（shared和static只能有一个为enable）：
./configure --enable-static --disable-shared --enable-memalign-hack --arch=x86 --target-os=mingw32 --pkg-config=pkg-config --enable-runtime-cpudetect --disable-debug --enable-pthreads
make
make install


下面介绍各种第三方库的编译。为了方便编译，所有库均先解压到[mingw]/msys/1.0/home/[username]根目录，然后在mingw shell中cd命令定位到库文件夹下。





**X264**

[http://www.videolan.org/developers/x264.html，](http://www.videolan.org/developers/x264.html)下载最新版本的[git](http://lib.csdn.net/base/git)

编译安装:
./configure --enable-win32thread
make
make install
cp -iv x264.h x264_config.h /mingw/include  
cp -iv libx264.a /mingw/lib  
cp -iv x264.pc /mingw/lib/pkgconfig 

ffmpeg编译参数：--enable-gpl --enable-libx264

**LAME 3.99.5**

[http://sourceforge.net/projects/lame/files/lame/](http://sourceforge.net/projects/lame/files/lame/)，下载lame-3.99.5.tar.gz

编译安装：

./configure --enable-export=full --enable-static --disable-shared --disable-frontend

make

make install

ffmpeg编译参数：--enable-libmp3lame



**librtmp 2.3**

[http://rtmpdump.mplayerhq.hu/](http://rtmpdump.mplayerhq.hu/)，下载rtmpdump-2.3.tgz(网上有帖子说是rtmpdump-2.3-windows.zip，但我下载了，里面都没有makefile)



librtmp依赖于zlib和openssl，所以要先编译两个库，见下面：

**Zlib 1.2.7**

[http://zlib.net/](http://zlib.net/)，下载zlib-1.2.7.tar.gz

make -f win32/Makefile.gcc
cp -iv zlib1.dll /mingw/bin  
cp -iv zconf.h zlib.h /mingw/include  
cp -iv libz.a /mingw/lib 
cp win32/Makefile.gcc Makefile.gcc

ffmpeg编译参数：--enable-zlib



**OpenSSL 1.0.1**

[http://www.openssl.org/](http://www.openssl.org/)，下载openssl-1.0.1c.tar.gz
./configure mingw --prefix=`pwd`/win32libs -DL_ENDIAN -DOPENSSL_NO_HW
make
make install

编译完成后，把当前目录的win32libs子文件夹下的include和lib分别拷贝到mingw32，以供后面librtmp使用

注意：openssl的编译需要在安装Mingw时包含Perl 5环境，如果没有的话，可到网上下载（我用的是5.16.2版，见[http://www.cpan.org/src/README.html](http://www.cpan.org/src/README.html)），下载后编译参数如下：


./Configure -des -Dprefix=$HOME/localperl
make
make test
make install
ffmpeg编译参数：--enable-nonfree --enable libopenssl



librtmp的编译命令如下：



make SYS=mingw
make SYS=mingw install

注意：编译ffmpeg时会提示“ERROR:   librtmp not found”错误，主要是因为一些library没链接好，不知道是否为ffmpeg的Bug。解决办法是在configure文件中“enabled librtmp”一行最后要添加“ -lwinmm -lwsock32 -lgdi32”

ffmpeg编译参数：--enable-librtmp



**xvidcore 1.3.2**

[http://www.xvid.org](http://www.xvid.org/)，下载xvidcore-1.3.2.tar.gz

编译安装：
cd xvidcore/build/generic  
./configure --prefix=/mingw 

执行到这里后，搜索xvidcore/build/generic 目录下面所有文件(我的版本是platform.inc中有两处)中的 -mno-cygwin 这个选项, 这个选项已经不被GCC支持，全部删除后继续。

make
make install
cp -iv \=build/xvidcore.dll.a /mingw/lib/libxvidcore.dll.a 

ffmpeg编译参数：--enable-libxvid



**libgsm 1.0.13-3**

[http://libgsm.sourcearchive.com/](http://libgsm.sourcearchive.com/)，下载libgsm_1.0.13.orig.tar.gz

编译安装：

make

mkdir /mingw/include/gsm

cp inc/gsm.h /mingw/include/gsm

cp lib/libgsm.a /mingw/lib

ffmpeg编译参数：--enable-libgsm



## libogg



供libvorbis和libtheora用

[http://www.xiph.org/downloads/](http://www.xiph.org/downloads/)，下载libogg-1.1.4.tar.gz

解压，编译安装：

./configure –prefix=/olibs –disable-shared
make
make install



**LIBVorbis 1.3.3**

[http://xiph.org/downloads/](http://xiph.org/downloads/)，下载libvorbis-1.3.3.tar.gz

编译安装：

./configure

make

make install

ffmpeg编译参数：--enable-libvorbis



**LIBTheora 1.1.1**

[http://xiph.org/downloads/](http://xiph.org/downloads/)，下载libtheora-1.1.1.tar.bz2

编译安装：



./configure

make

make install

ffmpeg编译参数：--enable-libtheora



LIBSpeex 1.2rc1

[http://xiph.org/downloads/](http://xiph.org/downloads/)，下载speex-1.2rc1.tar.gz

./configure

make

make install

ffmpeg编译参数：--enable-libspeex





加入以上第三方库以后，在ffmpeg中就可以下面的参数进行编译：

./configure --enable-static --disable-shared --enable-memalign-hack --arch=x86 --target-os=mingw32 --pkg-config=pkg-config --enable-runtime-cpudetect --disable-debug --enable-pthreads --enable-gpl --enable-bzlib --enable-libx264 --enable-libgsm --enable-libmp3lame --enable-libtheora --enable-libvorbis --enable-libxvid --enable-zlib--enable-openssl --enable-librtmp --enable-nonfree --enable-libspeex

make

make install









