# 在Ubuntu下编译FFmpeg - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月14日 13:44:07[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：203
## 第一步：准备编译环境
**sudo****apt-get update****sudo ****apt-get **-y **install ****autoconf ****automake** build-essential **git** libass-dev libfaac-dev \
  libgpac-dev libjack-jackd2-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev \
  librtmp-dev libsdl1.2-dev libspeex-dev libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev \
  libx11-dev libxext-dev libxfixes-dev pkg-config texi2html zlib1g-dev
 
**sudo** ldconfig
在home下新建一个目录用于存放下载的源码：
**mkdir** ~**/**ffmpeg-setup
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E7%BC%96%E8%AF%91FFmpeg&action=edit&section=3)]第二步：安装x264和FFmpeg要用到的汇编器yasm
**tar**-zxf yasm-1.2.0.tar.gz
**cd** yasm-1.2.0**/**
.**/**configure --prefix=**/**usr**/****local****make****sudo****make****install**
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E7%BC%96%E8%AF%91FFmpeg&action=edit&section=4)]第三步：编译x264
**tar**-jxf x264-20130217.tar.bz2
**cd** x264-snapshot-20130217-2245**/**
.**/**configure --enable-static--enable-shared--prefix=**/**usr**/****local****make****sudo****make****install**
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E7%BC%96%E8%AF%91FFmpeg&action=edit&section=5)]第四步：编译fdk-aac
**tar**-zxf fdk-aac-0.1.1.tar.gz
**cd** fdk-aac-0.1.1**/**
autoreconf –fiv
.**/**configure --prefix=**/**usr**/****local****make****sudo****make****install**
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E7%BC%96%E8%AF%91FFmpeg&action=edit&section=6)]第五步：编译FFmpeg
**tar**-jxf ffmpeg-1.1.3.tar.bz2
**cd** ffmpeg-1.1.3**/**
 
.**/**configure --enable-gpl--enable-libass--enable-libfaac--enable-libfdk-aac--enable-libmp3lame \
  --enable-libopencore-amrnb--enable-libopencore-amrwb--enable-libspeex--enable-librtmp--enable-libtheora \
  --enable-libvorbis--enable-x11grab--enable-libx264--enable-nonfree--enable-version3  \
  --enable-shared--prefix=**/**usr**/****local**
去掉--disable-optimizations --disable-asm，并加上-enable-shared --prefix=/usr/local选项
**make****sudo****make****install****sudo** ldconfig
编译完成，输入ffmpeg结果如下：
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E7%BC%96%E8%AF%91FFmpeg&action=edit&section=7)]艺搜参考
[http://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu](http://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu)
