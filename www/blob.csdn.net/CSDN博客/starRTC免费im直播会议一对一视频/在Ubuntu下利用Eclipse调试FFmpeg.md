# 在Ubuntu下利用Eclipse调试FFmpeg - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月14日 13:45:53[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：380
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=2)]第一步：准备编译环境
**sudo****apt-get update****sudo ****apt-get **-y **install ****autoconf ****automake** build-essential **git** libass-dev libfaac-dev \
  libgpac-dev libjack-jackd2-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev \
  librtmp-dev libsdl1.2-dev libspeex-dev libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev \
  libx11-dev libxext-dev libxfixes-dev pkg-config texi2html zlib1g-dev
 
**sudo** ldconfig
在home下新建一个目录存放下载的源码：
**mkdir** ~**/**ffmpeg-setup
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=3)]第二步：安装x264和FFmpeg要用到的汇编器yasm
**tar**-zxf yasm-1.2.0.tar.gz
**cd** yasm-1.2.0**/**
.**/**configure --prefix=**/**usr**/****local****make****sudo****make****install**
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=4)]第三步：编译x264
**tar**-jxf x264-20130217.tar.bz2
**cd** x264-snapshot-20130217-2245**/**
.**/**configure --enable-static--enable-shared--prefix=**/**usr**/****local****make****sudo****make****install**
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=5)]第四步：编译fdk-aac
**tar**-zxf fdk-aac-0.1.1.tar.gz
**cd** fdk-aac-0.1.1**/**
autoreconf –fiv
.**/**configure --prefix=**/**usr**/****local****make****sudo****make****install****sudo** ldconfig
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=6)]第五步：配置FFmpeg
**tar**-jxf ffmpeg-1.1.3.tar.bz2
**cd** ffmpeg-1.1.3**/**
.**/**configure --enable-gpl--enable-libass--enable-libfaac--enable-libfdk-aac--enable-libmp3lame \
  --enable-libopencore-amrnb--enable-libopencore-amrwb--enable-libspeex--enable-librtmp--enable-libtheora \
  --enable-libvorbis--enable-x11grab--enable-libx264--enable-nonfree--enable-version3  \
  --disable-optimizations--disable-asm
注意：不需要make
需要加上选项--disable-optimizations --disable-asm,不需要--prefix=/usr/local选项和--enable-shared选项，否则调试时会报错。
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=7)]第六步：安装JRE运行环境(eclipse运行时需要)
**tar**-zxf jre-7u-17-linux-i586.tar.gz
解压后得到jre1.7.0_17目录
**sudo****mkdir**-p**/**usr**/**lib**/**jvm
**sudo****mv** .**/**jre1.7.0_17 **/**usr**/**lib**/**jvm
设置环境变量：
**sudo** gedit ~**/**.bashrc
在最后面加入以下语句后，保存退出
**export**JAVA_HOME=**/**usr**/**lib**/**jvm**/**jre1.7.0_17
**export**CLASSPATH=.:${JAVA_HOME}**/**lib
**export**PATH=${JAVA_HOME}**/**bin:$PATH
接着执行
**source** ~**/**.bashrc
**sudo** update-alternatives --install**/**usr**/**bin**/**java java **/**usr**/**lib**/**jvm**/**jre1.7.0_17**/**bin**/**java 300
最后通过java -version检查是否安装成功。
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=8)]第七步：安装eclipse
直接解压即可；
给eclipse添加桌面快捷方式：右键选择Make Link；然后右键Move to 选择Desktop
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=9)]第八步：利用eclipse编译ffmpeg
启动eclipse后, File - New - Project
选择 "C/C++" 下的 "Makefile Project with Existing Code"
在Import Existing Code对话框中，Toolchain for Indexer Settings中选Linux GCC，并去掉C++勾选。点击“Browse”选中之前已经配置过的FFmpeg源码(到根目录)；
选择Project - Build All编译。
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=10)]第九步：调试FFmpeg
右键Binaries下的ffmpeg_g.exe选择Debug as->Local C/C++ Application，选择gdb/mi
设置调试参数需要先执行一遍此操作；
设置调试参数：
右键点击ffmpeg_g.exe选Debug as-> Debug Configuration，在Arguments选项卡的Program arguments方框里可以填上调试参数。
