# 在Windows下利用Eclipse调试FFmpeg - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月18日 17:20:59[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：851
所属专栏：[编解码](https://blog.csdn.net/column/details/12721.html)

## 目录
 [[隐藏](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#)] 
- [1 环境与软件](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.8E.AF.E5.A2.83.E4.B8.8E.E8.BD.AF.E4.BB.B6)
- [2 第一步：安装MinGW](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.AC.AC.E4.B8.80.E6.AD.A5.EF.BC.9A.E5.AE.89.E8.A3.85MinGW)
- [3 第二步：配置编译环境](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.AC.AC.E4.BA.8C.E6.AD.A5.EF.BC.9A.E9.85.8D.E7.BD.AE.E7.BC.96.E8.AF.91.E7.8E.AF.E5.A2.83)
- [4 第三步：配置SDL](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.AC.AC.E4.B8.89.E6.AD.A5.EF.BC.9A.E9.85.8D.E7.BD.AESDL)
- [5 第四步：编译](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.AC.AC.E5.9B.9B.E6.AD.A5.EF.BC.9A.E7.BC.96.E8.AF.91)- [5.1 编译faac](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.BC.96.E8.AF.91faac)
- [5.2 编译fdk-aac](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.BC.96.E8.AF.91fdk-aac)
- [5.3 编译x264](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.BC.96.E8.AF.91x264)
- [5.4 配置ffmpeg](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E9.85.8D.E7.BD.AEffmpeg)
- [6 第五步：利用eclipse编译ffmpeg](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.AC.AC.E4.BA.94.E6.AD.A5.EF.BC.9A.E5.88.A9.E7.94.A8eclipse.E7.BC.96.E8.AF.91ffmpeg)
- [7 第六步：调试FFmpeg](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg#.E7.AC.AC.E5.85.AD.E6.AD.A5.EF.BC.9A.E8.B0.83.E8.AF.95FFmpeg)
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=1)]环境与软件
win7 32位
打包下载链接：[http://pan.baidu.com/s/1c0deSSg](http://pan.baidu.com/s/1c0deSSg) 密码：cp1g
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=2)]第一步：安装MinGW
安装mingw-get-inst-20120426.exe(安装时选中c++编译器和Mingw Developer Toolkit，其它默认)
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=3)]第二步：配置编译环境
将yasm-1.2.0-win32.exe更名为yasm.exe后直接放在 /usr/local/bin（windows路径为C:\MinGW\msys\1.0\local\bin）
将下面3个压缩包里bin目录中的文件解压后放在/usr/local/bin下
glib_2.28.8-1_win32.zip：GLib (Run-time)
gettext-runtime_0.18.1.1-2_win32.zip：gettext-runtime (Run-time)
pkg-config_0.26-1_win32.zip：pkg-config (tool)
配置好后，效果如下：
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=4)]第三步：配置SDL
为了编译出ffplay，还需要配置SDL
下载编译好的SDL库文件SDL-devel-1.2.15-mingw32.rar并解压，
接下来无论是下载的还是自己编译的，都需要修改bin目录下的 sdl-config文件：
把 prefix=/usr/local/cross-tools/i686-w64-mingw32该成： prefix=c:/MinGW
并去掉2处-mwindows
解压后，将bin，include和lib目录下的文件分别拷贝到/usr/local对应目录下（注：直接拷贝include目录下的SDL文件夹），
为了编译时ffmpeg能识别SDL并开启 SDL support，需要将bin，include和lib目录下的文件再拷贝一份到C:\MinGW\的对应目录中。
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=5)]第四步：编译
下一步开始编译,不编译shared版本
注：软件安装时统一加上--prefix=/usr/local
### [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=6)]编译faac
.**/**configure --prefix="/usr/local"--enable-static--disable-shared --with-mp4v2=no
**make****make****install**
### [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=7)]编译fdk-aac
configure FFmpeg时需要加上--enable-libfdk_aac
.**/**configure --prefix="/usr/local"--disable-shared**make****make****install**
### [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=8)]编译x264
.**/**configure --prefix=**/**usr**/****local**--enable-static--enable-shared**make****make****install**
### [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=9)]配置ffmpeg
如果未配置SDL，需要添加--disable-ffplay，下面内容可以全部复制后粘贴到命令行中。
PKG_CONFIG_PATH="/usr/local/lib/pkgconfig"LDFLAGS="-L/usr/local/lib"CFLAGS="-I/usr/local/include" .**/**configure --enable-gpl \
  --disable-optimizations--disable-asm--enable-libfaac --enable-libfdk_aac \
  --enable-libx264--enable-nonfree--enable-static--disable-shared
去掉-enable-shared --prefix=/usr/local选项，并加上--disable-optimizations --disable-asm选项
不用make
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=10)]第五步：利用eclipse编译ffmpeg
eclipse运行需要先安装JRE；
打开eclipse：
File-New-C project,选择Makefile project下的Empty Project.右侧选Mingw GCC
再拷贝上面configure之后的ffmpeg源码到工程根目录下，F5刷新后选择Project-Build All.
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Windows%E4%B8%8B%E5%88%A9%E7%94%A8Eclipse%E8%B0%83%E8%AF%95FFmpeg&action=edit&section=11)]第六步：调试FFmpeg
右键Binaries下的ffmpeg_g.exe选择Debug as->Local C/C++ Application，选择gdb/mi
设置调试参数需要先执行一遍上面的操作，然后右击ffmpeg_g.exe选Debug as-> Debug Configuration，
在Arguments选项卡的Program arguments方框里可以输入调试参数。
如果调试时ffmpeg导常终止，不能进入Main函数中，如下图所示：
请将x264动态链接库(C:\MinGW\msys\1.0\local\bin\libx264-129.dll)拷贝到工程根目录下。
再次调试结果如下：
