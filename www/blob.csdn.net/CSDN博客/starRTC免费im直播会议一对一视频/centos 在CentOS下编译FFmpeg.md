# centos 在CentOS下编译FFmpeg - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月17日 15:43:53[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：524
## 所需软件
准备
**yum install   ****autoconf  ****automake  ****gcc** gcc-c++ **git** libtool **make  ****nasm** pkgconfig zlib-devel  -y新版还加了下面的cmake freetype-devel  mercurial 
## 编译yasm
tar -zxvf yasm-1.2.0.tar.gz 
**cd** yasm-1.2.0
.**/**configure --prefix=**/**usr**/****local****make && ****make ****install**~~. ~**/**.bash_profile~~
## 编译fdk_aac
tar -zxvf fdk-aac-0.1.1.tar.gz 
cd fdk-aac-0.1.1
autoreconf -fiv
.**/**configure --prefix=**/**usr**/****local****make &&  ****make  ****install**
## 编译x264
**cd** x264
**//**如果之前安装过FFmpeg,会自动链接lavf,如果不想链接lavf,可添加--disable-lavf
.**/**configure --prefix=**/**usr**/****local **--enable-static  --enable-shared**make  && ****make ****install**
## 编译FFmpeg
**cd ****ffmpeg**PKG_CONFIG_PATH=**/**usr**/**local**/**lib**/**pkgconfig
**export** PKG_CONFIG_PATH
或把上面2行加到vim ~/.bash_profile
source ~/.bash_profile
 echo $PKG_CONFIG_PATH
.**/**configure --prefix=**/**usr**/****local  **--extra-cflags=-I**/**usr**/**local**/**include \
  --extra-ldflags=-L**/**usr**/**local**/**lib   --bindir=**/**usr**/**local**/**bin \
  --extra-libs="-ldl"  --enable-gpl  --enable-nonfree   --enable-libfdk_aac    --enable-libx264  --enable-shared**make  &&  ****make ****install****hash **-r
. ~**/**.bash_profile
接着输入ffmpeg检查是否安装成功，
如果出现如下错误：
error while loading shared libraries: libavdevice.so.54
解决方法：
**vim  ****/**etc**/**ld.so.conf
添加
**/**usr**/**local**/**lib
**/**usr**/**local**/**lib64
后
*# ldconfig*
## 艺搜参考
[http://trac.ffmpeg.org/wiki/CompilationGuide/Centos](http://trac.ffmpeg.org/wiki/CompilationGuide/Centos)
