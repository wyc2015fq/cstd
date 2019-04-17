# windows 7下mingw+msys编译ffmpeg - DoubleLi - 博客园







|# [windows 7下mingw+msys编译ffmpeg](http://www.cnblogs.com/zzugyl/archive/2013/01/18/2867123.html)1-->下载安装MingW，mingw-get-inst-20120426.exe  [http://sourceforge.net/projects/mingw/](http://sourceforge.net/projects/mingw/)安装路径 默认C:\MinGW 。勾选了 <1> C Compiler  <2> C++ Compiler <3>MSYS Basic System <4>MinGW Developer ToolKit其中 <3>就安装了 msys。这样安装需要联网，自动下载mingw项目下一些工具文件。安装完成后，为了方便VS200X以及VS2010调用ffmpeg的动态库，可以通过配置让ffmpeg编译时产生windows下调用dll对应的lib。根据我的VS2010安装目录，用noteplus notepad++ ue等支持Uinux换行的文本工具打开 C:\MinGW\msys\1.0\msys.bat在最最前面加上下面一行call "C:\Program Files\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"将 msys.bat发送到桌面快捷方式，就可以方便打开命令窗口了，一个简单的linux shell。这样 cd /e/ffmpeg/ 就切换到E:/ffmpeg/目录下2--->下载安装yasm，因为编译ffmpeg里面的汇编代码需要yasm.exe，没有这个的话，也能编译通过，可能使用的时候，效率没有那么高。 [http://yasm.tortall.net](http://yasm.tortall.net/)[Win32 VS2010 .zip](http://www.tortall.net/projects/yasm/releases/vsyasm-1.2.0-win32.zip) (for use with VS2010 on 32-bit Windows) Version 1.2.0 Release: October 31, 2011为了减少配置环境变量的麻烦，解压后把 vsyasm.exe 文件名修改成：yasm.exe，并放到系统目录下：C:WINDOWS\system32\下。3--->下载安装ffmpeg。 [http://www.ffmpeg.org](http://www.ffmpeg.org/) Version 1.1 Release:January 7, 20134--->编译X264(项目需求)[http://www.videolan.org/developers/x264.html](http://www.videolan.org/developers/x264.html)  下载 解压 得 x264-snapshot-20130114-2245打开 msys.bat，命令行1.$ cd e/ffmpeg/x264-snapshot-20130114-2245/2.$ ./configure --enable-shared --disable-asm3.$ make4.$ make install这样就把编译结果复制到 C:\MinGW\msys\1.0\local 下 bin include lib这三个文件夹下面。输入命令有个技巧，可以复制后，在msys.bat的命令行窗口标题栏点击右键，以此选择--''编辑--粘贴(P)''。类似DOS，标记后即是复制。如果编译失败或者重新编译，先运行make distclean或者make clean，再执行上面1--2--3--45--->下载SDL。没有SDL编译出来的ffplay.exe不能播放视频。[http://www.libsdl.org/download-1.2.php](http://www.libsdl.org/download-1.2.php) 最后有一个 win32 [SDL-devel-1.2.15-mingw32.tar.gz](http://www.libsdl.org/release/SDL-devel-1.2.15-mingw32.tar.gz) (Mingw32)解压后 bin include lib文件夹内容对应拷贝到C:\MinGW\msys\1.0\local 下 bin include lib这三个文件夹下。使用UltraEdit打开刚刚拷贝的C:\MinGW\msys\1.0\local\bin下的 sdl-config文件    把 prefix=/usr/*******  该成： prefix=c:/mingw其中：c:/mingw 为 mingw的安装路径，请根据你的安装进行修改。为了编译时msys能识别sdl并开启 SDL support yes 进行编译。|
|----|









