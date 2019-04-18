# windows编译ffmpeg - tifentan的专栏 - CSDN博客

2017年10月11日 11:00:25[露蛇](https://me.csdn.net/tifentan)阅读数：161


这方法很烂，建议使用mingw完整的环境： 
[https://blog.csdn.net/tifentan/article/details/79663499](https://blog.csdn.net/tifentan/article/details/79663499)

系统环境：Windows 7 64位 

　　需要安装的软件和工具： 

　　Visual Studio 2015 With Update 3 

　　MSYS2 

　　YASM 

　　一、VS2015 

　　安装VS2015时，选择Git。 

　　二、msys2 

　　1、到这里下载： 
[http://msys2.github.io](http://msys2.github.io). 选择第二个:msys2-x86_64-20160921.exe 

　　下载后，安装默认安装到C:\ msys64 

　　2.安装后，到msys2窗口执行，安装编译程序： pacman -S make gcc diffutils pkg-config 

　　3. 重命名C:\msys64\usr\bin\link.exe 为C: \msys64\usr\bin\link.bak, 避免和MSVC 的link.exe抵触。 

　　三、YASM 

　　1. YASM下载地址：[http://yasm.tortall.net/Download.html](http://yasm.tortall.net/Download.html)，下载其64位版本Win64 .exe (64 位 Windows 通用)。 

　　2、下载后，将下载回来的yasm-1.3.0-win64.exe 改名为yasm.exe，并放置于 MSYS2 安装目录中。就是放置到c:\msys64\usr\bin\ 中。 

　　四，ffmpeg 

下载最新版本ffmpeg,本文是在2016.10.21下载，如果速度太慢，可以翻墙后再执行： 

　　进入windows命令行，在C:\FFMPEG目录下执行： 

　　git clone git://source.ffmpeg.org/ffmpeg.git 

　　四，编译ffmpeg 

　　1、使用VC2015编辑C:\msys64\msys2_shell.cmd,将其中的： 

　　rem set MSYS2_PATH_TYPE=inherit 

　　“rem”注释删除掉，成为： 

　　set MSYS2_PATH_TYPE=inherit 

　　2、到开始菜单，执行“Visual Studio 2015-> VS2015 x64 本机工具命令提示符”(编译32位选x86),在命令窗口下，CD \msys64,执行：msys2_shell.cmd -mingw64 

　　3、在启动的 MSYS2 终端中分别运行一下命令观察各便于工具组件是否被正确找到： 
![这里写图片描述](https://img-blog.csdn.net/20161021094457866?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

　　5、修改msys2窗口代码页为GBK，避免编译时显示乱码： 

　　4、编译ffmpeg 

　　在msys2窗口: 

　　cd / 

　　cd /ffmpeg 

　　cd /ffmgeg 

　　进入下载ffmpeg目录中 

`./configure  --toolchain=msvc  --arch=x86  --enable-yasm  --enable-asm --enable-shared  --disable-static`

　　make 

　　make install 

　　这样以后，编译生产的执行文件ffmpeg.exe和DLL文件在目录C:\msys64\usr\local\bin中，开发头文件在C:\msys64\usr\local\include中。

