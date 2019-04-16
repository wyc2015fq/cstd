# 开源数字媒体资产管理系统：Razuna安装方法 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月26日 12:29:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：33
个人分类：[开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1652801)










Razuna以一个使用Java语言编写的开源的数字媒体资产管理（Digital Asset Management）系统。在这里翻译一下它的安装步骤。

![](https://img-blog.csdn.net/20131125214119437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



Razuna包含以下版本：

**Razuna Stand-alone Server**

它是建立在Tomcat服务器上的。这是迄今为止最快以及最简单的启动和运行方式。推荐新用户使用。

**Razuna Virtual Image**

包含了所需要的所有东西。可以直接部署于AmazonEC2，或VMWare，Parallels，XEN，KVM，VirualIron等

**Razuna WAR/EAR**

WAR包可以成功部署在Jetty，Tomcat以及JBoss这些J2EE服务器上。它实际上和上述razuna服务器相同，但需要手动配置并且主要是面向系统管理员的。如果您有一个现有的已经部署的J2EE服务器，推荐使用该版本。

**源代码 (Github)**

可以获得源代码：[https://github.com/razuna/razuna](https://github.com/razuna/razuna)



在这里翻译一下Razuna安装教程（Windows平台）

Razuna在下列平台测试通过：Windows7，Server2003，2008(R2)，2012。


**安装Java**


您需要安装Java7(1.7)。[下载页面在此](http://www.java.com/en/download/)。

另外系统环境变量需要设置"JAVA_HOME"，通常路径为C:\ProgrammFiles\Java\bin（根据安装位置不同而不同）。



**安装路径**

下文所列程序需要安装在系统的根目录里面。比如C:\ImageMagick或C:\Exiftool。

文件路径中尽量不要包含空格。


**安装Ghostscript**


[http://sourceforge.net/projects/ghostscript/](http://sourceforge.net/projects/ghostscript/)

需要32bit版本的Ghostscript! ImageMagick(内部会调用Ghostscript)需要32bit版本的。


**安装ImageMagick**


从ImageMagick网站[下载最新的Image Magick Windows安装包](http://www.imagemagick.org/script/binary-releases.php#windows)。

推荐使用以下版本：Win32 dynamic at 16bits-per-pixel。


**安装FFMpeg**


[http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)。

使用下列版本:
|**平台**|**地址**|
|----|----|
|32-bit|[http://ffmpeg.zeranoe.com/builds/win32/static/ffmpeg-20130713-git-aac57c7-win32-static.7z](http://ffmpeg.zeranoe.com/builds/win32/static/ffmpeg-20130713-git-aac57c7-win32-static.7z)|
|64-bit|[http://ffmpeg.zeranoe.com/builds/win64/static/ffmpeg-20130713-git-aac57c7-win64-static.7z](http://ffmpeg.zeranoe.com/builds/win64/static/ffmpeg-20130713-git-aac57c7-win64-static.7z)|

解压压缩包并移动至C:\ffmpeg。


**安装Exiftool**


从[Exiftool官方网站](http://www.sno.phy.queensu.ca/~phil/exiftool/)下载最新版本。下载完后，在C盘创建一个名为"exiftool"文件夹，并且把exe文件移动过去。重命名文件名为"exiftool.exe"。


**安装DCraw(optional)**


从该地址下载：[http://www.insflug.org/raw/](http://www.insflug.org/raw/)。或者[直接下载一个](http://www.insflug.org/raw/file_download/8/dcrawMS.exe)dcraw build。下载完后，重命名文件名为"dcraw.exe"。移动exe至c:\dcraw\dcraw.exe。


**安装UFraw**


如果想要安装UFraw需要先安装Gimp。安装方法见此页。


**安装MP4Box**


下载[MP4Box安装包](http://www.videohelp.com/download/MP4BOX_GPAC.Framework.Setup-0.5.0.exe)。并将其安装至C:\MP4Box。


**安装Razuna standalone**


如果您还没有这样做，从[http://razuna.org](http://razuna.org/)下载最新版的Razuna我们建议解压Razuna至C:盘根目录。请确保Razuna路径中不包含空格！Razunastandaloneserver包含了配置过的Tomcat。


**运行Razuna server**


最后的任务是运行Razuna服务器。打开”bin”目录(C:/tomcat/bin/)。使用如下命令启动服务器：
|./startup.bat|
|----|


**导航到Razuna**


访问[http://localhost:8080/razuna](http://localhost:8080/razuna)。将会呈现给您第一次运行的时候的配置页面。



版本页面：
[http://www.razuna.org/download](http://www.razuna.org/download)

安装教程：
[http://wiki.razuna.com/display/ecp/Install+Razuna+on+Windows](http://wiki.razuna.com/display/ecp/Install+Razuna+on+Windows)






