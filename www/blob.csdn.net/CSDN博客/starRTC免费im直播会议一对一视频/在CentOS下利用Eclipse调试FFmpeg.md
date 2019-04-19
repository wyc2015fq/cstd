# 在CentOS下利用Eclipse调试FFmpeg - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月15日 22:25:25[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：395
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)
所属专栏：[](https://blog.csdn.net/column/details/.html)[编解码](https://blog.csdn.net/column/details/12721.html)

## 所需软件
64位软件打包下载链接：[http://pan.baidu.com/s/1i3B08Up](http://pan.baidu.com/s/1i3B08Up) 密码：o50u
[https://yunpan.cn/cBKDSbrGDgBvz](https://yunpan.cn/cBKDSbrGDgBvz)  访问密码 1f55
## 编译FFmpeg
请参考：[在CentOS下编译FFmpeg](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8CentOS%E4%B8%8B%E7%BC%96%E8%AF%91FFmpeg&action=edit&redlink=1)
## 配置FFmpeg
如果要编译出ffplay，需要编译SDL
**yum install** SDL-devel
 
**tar**-jxf ffmpeg-1.1.3.tar.bz2
**cd** ffmpeg-1.1.3**/**PKG_CONFIG_PATH=**/**usr**/**local**/**lib**/**pkgconfig
**export** PKG_CONFIG_PATH
 
.**/**configure  --extra-cflags=-I**/**usr**/**local**/**include  --extra-ldflags=-L**/**usr**/**local**/**lib  \
  --extra-libs="-ldl"--enable-gpl--enable-nonfree --enable-libfdk_aac  --enable-libx264  \
  --disable-optimizations--disable-asm
注意：不需要make
需要加上选项--disable-optimizations --disable-asm,不需要--prefix=/usr/local选项和--enable-shared选项，否则调试时会报错。
## 安装eclipse
运行eclipse需要安装jre
rpm -ivh jre-7u25-linux-x64.rpm
eclipse直接解压即可使用
可能需要安装：
yum install kdelibs3
如果报：没有为可执行文件文件类型安装应用程序
需要加执行权限
启动eclipse后, File - New - Project
选择 "C/C++" 下的 "Makefile Project with Existing Code"
在Import Existing Code对话框中，Toolchain for Indexer Settings中选Linux GCC，并去掉C++勾选。
点击“Browse”选中之前已经配置过的FFmpeg源码(到根目录)；
选择Project - Build All编译。
## 调试FFmpeg
右键Binaries下的ffmpeg_g选择Debug as->Local C/C++ Application，选择gdb/mi
设置调试参数前需要先执行一遍此操作；
### 设置调试参数
右键点击ffmpeg_g.exe选Debug as-> Debug Configuration，在Arguments选项卡的Program arguments方框里可以填上调试参数。
## 常见错误
调试时出现：
Error while launching command: gdb --version
解决：
**yum install****gdb**
## 所需软件
64位软件打包下载链接：[http://pan.baidu.com/s/1i3B08Up](http://pan.baidu.com/s/1i3B08Up) 密码：o50u
[https://yunpan.cn/cBKDSbrGDgBvz](https://yunpan.cn/cBKDSbrGDgBvz)  访问密码 1f55
## 编译FFmpeg
请参考：[在CentOS下编译FFmpeg](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8CentOS%E4%B8%8B%E7%BC%96%E8%AF%91FFmpeg&action=edit&redlink=1)
## 配置FFmpeg
如果要编译出ffplay，需要编译SDL
yum install SDL-devel
 
tar-jxf ffmpeg-1.1.3.tar.bz2
cd ffmpeg-1.1.3/PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH
 
./configure  --extra-cflags=-I/usr/local/include  --extra-ldflags=-L/usr/local/lib  \
  --extra-libs="-ldl"--enable-gpl--enable-nonfree --enable-libfdk_aac  --enable-libx264  \
  --disable-optimizations--disable-asm
注意：不需要make
需要加上选项--disable-optimizations --disable-asm,不需要--prefix=/usr/local选项和--enable-shared选项，否则调试时会报错。
## 安装eclipse
运行eclipse需要安装jre
rpm -ivh jre-7u25-linux-x64.rpm
eclipse直接解压即可使用
可能需要安装：
yum install kdelibs3
如果报：没有为可执行文件文件类型安装应用程序
需要加执行权限
启动eclipse后, File - New - Project
选择 "C/C++" 下的 "Makefile Project with Existing Code"
在Import Existing Code对话框中，Toolchain for Indexer Settings中选Linux GCC，并去掉C++勾选。
点击“Browse”选中之前已经配置过的FFmpeg源码(到根目录)；
选择Project - Build All编译。
## 调试FFmpeg
右键Binaries下的ffmpeg_g选择Debug as->Local C/C++ Application，选择gdb/mi
设置调试参数前需要先执行一遍此操作；
### 设置调试参数
右键点击ffmpeg_g.exe选Debug as-> Debug Configuration，在Arguments选项卡的Program arguments方框里可以填上调试参数。
## 常见错误
调试时出现：
Error while launching command: gdb --version
解决：
yum installgdb
