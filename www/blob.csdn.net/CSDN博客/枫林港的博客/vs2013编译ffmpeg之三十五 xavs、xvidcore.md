# vs2013编译ffmpeg之三十五  xavs、xvidcore - 枫林港的博客 - CSDN博客





2018年07月12日 23:54:17[枫林港](https://me.csdn.net/u013601132)阅读数：53








## xavs

对应ffmpeg configure选项–enable-libxavs。

[http://xavs.sourceforge.net/](http://xavs.sourceforge.net/)

[http://xavs.sourceforge.net/xavs_ffmpeg.html](http://xavs.sourceforge.net/xavs_ffmpeg.html)

下载代码命令：
`svn checkout svn://svn.code.sf.net/p/xavs/code/trunk xavs-code`
中国AVS标准的编解码库，代码里面自带MSVC工程。在vs2013下编译有下面的错误：
`xavs-code_20160829\common\common.h(73): fatal error C1083: Cannot open include file: 'pthread.h': No such file or directory`
直接用mingw或msys环境里面的pthread.h，同时需要sched.h。也需要将pthread.lib加到工程里面。pthread.lib是拷贝了\MinGW\lib\libpthread.a，只是改了名字。

下面的错误：
`error LNK2026: module unsafe for SAFESEH image`
解决的办法：link->advance->Image Has Safe Exception Handlers设置为No (/SAFESEH:NO)。

为了不修改ffmpeg的configure文件，xavs工程里面，libavs输出的库名字改成xavs.lib，原始的是libxavs.lib。

# xvidcore

对应ffmpeg configure选项–enable-libxvid。

下载xvidcore-1.3.2.tar.gz，xvidcore\build\win32下面有MSVC2010的sln工程，打开后编译发现缺少nasm。

下载nasm-2.11.02-win32.zip，64位系统下载nasm-2.11.02-win64.zip，将其中的nasm.exe放到C:\Program Files\Microsoft Visual Studio 10.0\VC\bin下面，然后就可以编译了。

VS2010打开后，生成的库文件是Debug/libxvidcore.lib(VS2010生成的bin/xvidcore.lib太小，看上去是用于dll的)，而VS2013生成的是bin/xvidcore.lib。这个可以通过修改工程配置来改变，libxvidcore工程属性->General->Configuration Type设置成Static library (.lib)，即生成静态库。

链接生成.exe文件的时候会有下面的错误：
`2>LINK : fatal error LNK1104: cannot open file 'xvidcore.dll.a'`
将xvid_decraw、xvid_encraw两个工程属性里面，link->input->Additional Dependencies里面，xvidcore.dll.a改成xvidcore.lib，在xvidcore\build\win32\bin下面生成的是xvidcore.lib。

msys+gcc编译xvidcore的时候要将configure里面的-mno-cygwin都去掉，因为gcc不认识这个选项。



