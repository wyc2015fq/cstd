# Mplayer 的编译 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月20日 15:52:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：38
个人分类：[Mplayer](https://blog.csdn.net/leixiaohua1020/article/category/2286593)










由于项目要用到mplayer，所以要对mplayer进行编译，首先我的平台如下：

系统：windows xp2

安装好mingw+msys，目录为c:/mingw，其中装的gcc是4.3.2版本的。

好了，开始我们windows下的编译之旅。

首先，感谢网络上面的资料，各位前辈辛辛苦苦写的blog和上传的资料。大家都为开源而努力吧~~！本文遵循GPL/LGPL协议，欢迎自由转载，但请注明作者出处。

关于directx的头文件，我是直接将我机子上安装的dx的include中的所以头文件都复制到了c:/mingw/include中。大家可以单独下载dx7或者dx9，最好去网上搜搜资源。我打开了很多网页，里面有下载链接，不过好像很多都挂掉了，回头有时间自己上传一个吧。

开始下载安装一些MPlayer编译所依赖的软件和lib等东东。

注：这些库也可以不编译安装，并不影响MPlayer的使用，只是会少一些功能。

1、freetype（OSD字幕用）

软件主页：[http://www.gnu.org/software/libiconv/](http://www.gnu.org/software/libiconv/)
 下载版本为：libiconv-1.13.1.tar.gz

解压后吧libiconv-1.13.1文件放到c盘下，然后通过msys的终端，cd c:/libiconv-1.13.1。

接着输入： ./configure --prefix=c:/mingw --disable-shared

注：配置代码命令，其中--prefix=c:/mingw为指定目录命令， --disable-shared为取消共享。回车，等待执行完毕后，输入：
 make
 注：编译命令。回车，等待执行完毕后，输入：
 make install
 注：编译安装命令。等待执行完毕后，如果一切顺利，没有报错。那么到此就算完成了一个软件的安装。与下面的各个步骤大同小异，下面我就写的简单一些了。



2、freetype2
 软件主页：[http://sourceforge.net/project/showfiles.php?group_id=3157](http://sourceforge.net/project/showfiles.php?group_id=3157)
 需要文件：freetype-2.1.9.tar.bz2
 同上一样，解压，然后

 cdfreetype-2.1.9

 ./configure --prefix=c:/mingw

 make

 make install



3.zlib

软件主页：[http://www.zlib.net/](http://www.zlib.net/)

下载文件：zlib-1.2.5.tar.gz2

cd zlib-1.2.5

./configure --prefix=c:/mingw

make

make install

ps:网上有的没有./configure，应该要有，不然后面编译的时候会出现找不到zlib.h的error。另外，网上有的教程是针对zlib-1.2.3版本的，这个可以在csdn上搜索到，如果1.2.5本版不行的话，可以用那个。



4.libpng

软件主页：[http://sourceforge.net/project/showfiles.php?group_id=5624](http://sourceforge.net/project/showfiles.php?group_id=5624)
 所需文件：libpng-1.2.8-config.tar.gz
 解压
 cd libpng-1.2.8-config
 ./configure --prefix=c:/mingw --disable-shared
 make
 make install


5、libjpeg 

软件主页：[http://www.ijg.org/](http://www.ijg.org/)
 所需文件：jpegsrc.v8b.tar.gz
 解压

 cd jpeg-8b

 ./configure --prefix=c:/mingw --enable-static
 cp .libs/libjpeg.a c:/mingw/lib/
 cp jpeglib.h jconfig.h jmorecfg.h c:/mingw/include/



 6、libregif
 软件主页：[http://oss.netfarm.it/mplayer-win32.php](http://oss.netfarm.it/mplayer-win32.php)
 所需文件：libregif-4.1.7-ac.tar.bz2（附：在[Misc Files]标签下）
 解压

 cd libregif-4.1.7

 ./configure --prefix=c:/mingw

 make

 make install


7、nasm
 软件主页：[http://sourceforge.net/projects/nasm/files/](http://sourceforge.net/projects/nasm/files/)

> 
所需文件：nasm-2.07.tar.bz2
 解压
 cd nasm-2.07


> 
./configure --prefix=c:/mingw
 make
mkdir c:/mingw/man/man1(这句要加~~，不过可能你的已经存在这个目录了，那就没事了。)
 make install




8、lame
 软件主页：[http://lame.sourceforge.net/download/download.html](http://lame.sourceforge.net/download/download.html)
 所需文件：lame3.98.4.tar.gz
 解压
 cd lame-3.98.4
 ./configure --prefix=c:/mingw --disable-shared --disable-decoder
 make
 make install



9、xvid
 软件主页：[http://www.xvid.org/downloads.html](http://www.xvid.org/downloads.html)
 所需文件：xvidcore-1.2.2.tar.bz2
 cd xvidcore-1.2.2/build/generic
 ./configure --prefix=c:/mingw --disable-shared
 make 
 make install
 mv c:/mingw/lib/xvidcore.a c:/mingw/lib/libxvidcore.a



10.svn

软件主页：[http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=11149&expandFolder=11149&folderID=0](http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=11149&expandFolder=11149&folderID=0)

打开subversion-》Releases-》Windows Binaries

下载文件：[svn-win32-1.4.6.zip](http://blog.csdn.net/moodytong/article/details/files/documents/15/47778/svn-win32-1.4.6.zip) （最后一个）

解压后将 /bin 文件夹内的所有文件复制到C:/mingw/bin文件夹内。



11、live555
下载地址：[http://www.live555.com/liveMedia/public/](http://www.live555.com/liveMedia/public/)
 所需文件：[live555-latest.tar.gz](http://blog.csdn.net/moodytong/article/details/live555-latest.tar.gz)

 解压
 cd live
 genMakefiles mingw
 make 



mplayer的初步编译：

下载版本为Player-1.0rc3.tar.bz

cd c/mplayer/1.0rc3

./configure --prefix=/usr --enable-static --enable-qtx --enable-real --enable-freetype --enable-png --enable-jpeg --enable-gif --enable-tga --disable-mencoder --enable-runtime-cpudetection --disable-ssse3 --extra-libs="-mwindows"

make

make install

成功之后：



![](https://img-blog.csdn.net/20131120154455281)



其它可参考文档：

[http://blog.csdn.net/li_007/archive/2008/01/21/2057382.aspx](http://blog.csdn.net/li_007/archive/2008/01/21/2057382.aspx)

[http://kb.cnblogs.com/a/1426019/](http://kb.cnblogs.com/a/1426019/)

[http://www.gaia-gis.it/spatialite-2.4.0/mingw_how_to.html](http://www.gaia-gis.it/spatialite-2.4.0/mingw_how_to.html)

[http://www.linuxdiyf.com/bbs/thread-22266-1-1.html](http://www.linuxdiyf.com/bbs/thread-22266-1-1.html)



转载自：[http://blog.csdn.net/moodytong/article/details/5895632](http://blog.csdn.net/moodytong/article/details/5895632)








