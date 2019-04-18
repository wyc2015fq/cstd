# 搭建mingw-msys环境编译ffmpeg - tifentan的专栏 - CSDN博客

2018年03月23日 10:00:55[露蛇](https://me.csdn.net/tifentan)阅读数：194标签：[msys																[mingw																[ffmpeg](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)](https://so.csdn.net/so/search/s.do?q=mingw&t=blog)](https://so.csdn.net/so/search/s.do?q=msys&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/tifentan/article/category/6983974)


1、下载.mingw-w64-install.exe和msys工具包(msys+7za+wget+svn+git+mercurial+cvs-rev12.7z)。 
[https://sourceforge.net/projects/mingwbuilds/files/external-binary-packages/](https://sourceforge.net/projects/mingwbuilds/files/external-binary-packages/)
[https://sourceforge.net/projects/mingw-w64/files/](https://sourceforge.net/projects/mingw-w64/files/)

2、双击下载的mingw-w64-install.exe安装mingw64,在选择版本界面中要选择x86_64.这样才是64位系统。我安装到D:\mingw-w64下。

3、安装msys,解压msys+7za+wget+svn+git+mercurial+cvs-rev12.7z到D:\mingw-w64\msys目录；

4、配置minGW和msys。

进入msys目录，双击msys.bat.创建mingw目录。

进入etc目录，编辑fstab文件，在文件中添加一行 d:/mingw-w64/x86_64-7.1.0-win32-seh-rt_v5-rev2/mingw64 

/mingw/.保存退出。

关闭窗口。双击msys.bat，输入gcc -v,不出现错误就说明编译环境可用了。

5、下载最新版本的x264和SDL并编译。 

x264编译安装命令如下： 

./configure –prefix=/mingw –host=x86_64-w64-mingw32 –enable-static –enable-shared –disable-asm；make;make install 

SDL 编译安装命令如下： 

./configure –prefix=/mingw –host=x86_64-w64-mingw32 ；make;make install

6、下载FFMpeg.解压到/home/FFMpeg下。 

./configure –prefix=/mingw –enable-gpl –enable-nonfree –enable-postproc –enable-avfilter –enable-runtime-cpudet 

ect –disable-x86asm –enable-libx264 –disable-debug –enable-shared –arch=x86_64 –host-os=win64 –extra-cflags=-I/m 

ingw/include –extra-ldflags=-L/mingw/lib 

make; 

make install 

7.拷贝yasm到msys/bin

