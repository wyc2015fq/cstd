# 最简单的视频编码器：编译（libx264，libx265，libvpx） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年12月22日 00:35:27[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：31277










=====================================================


最简单的视频编码器系列文章列表：
[最简单的视频编码器：编译](http://blog.csdn.net/leixiaohua1020/article/details/42069383)

[最简单的视频编码器：基于libx264（编码YUV为H.264）](http://blog.csdn.net/leixiaohua1020/article/details/42078645)


[最简单的视频编码器：基于libx265（编码YUV为H.265）](http://blog.csdn.net/leixiaohua1020/article/details/42079101)


[最简单的视频编码器：libvpx（编码YUV为VP8）](http://blog.csdn.net/leixiaohua1020/article/details/42079217)


=====================================================


最近研究了一下几种常见的视频编码器：x264，x265，vpx。本文简单记录一下它们的编译方法。

x264，x265，vpx这三个开源的视频编码器可以说是当今“最火”的视频编码器。x264现在占据着H.264视频编码器的半壁江山；x265则是目前实现H.265标准最好的开源视频编码器，并且在未来可能接替x264；而vpx则是Google推出的开源视频编码器，它提出的VP9编码标准的性能也不错。在记录完这篇文章后，计划分别针对x264，x265，vpx写三篇文章记录它们的使用方法。


## X264 

![](https://img-blog.csdn.net/20141222001638961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





网站：[http://www.videolan.org/developers/x264.html](http://www.videolan.org/developers/x264.html)

源代码获取地址：
`git clone git://git.videolan.org/x264.git`
注意编译前需要安装yasm。


**Windows下最简单编译步骤（MinGW）**

1.下载安装MinGW，运行后切换至源代码目录
2.输入 "./configure"
3.输入"make"
4.输入"make install"安装（可选）


**Linux下最简单编译步骤：**

1.切换至源代码目录
2.输入 "./configure"
3.输入"make"

4.输入"make install"安装（可选）




编译后生成：


> x264.exe：可以直接使用的编码H.264命令行程序

Libx264.a：可以用于程序开发的编码H.264的类库




**Windows下编译获得lib类库步骤（生成libx264.lib）**
1.       生成“*.def”文件。输入“./configure”命令的时候，将“./configure”后面加上参数，改为：
`./configure --enable-shared --extra-ldflags=-Wl,--output-def=libx264.def`这样可以生成“libx264.dll”，“libx264.def”文件。
2.       使用VC自带的lib生成工具“LIB”，生成“libx264.lib”文件。使用“Visual Studio command prompt”，切换至x264源代码目录，输入以下命令：
`LIB /DEF:libx264.def`即可生成libx264.lib用于在VC下开发。


## X265 

![](https://img-blog.csdn.net/20141222001712033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




网站1：[http://www.videolan.org/developers/x265.html](http://www.videolan.org/developers/x265.html)

网站2：[https://bitbucket.org/multicoreware/x265](https://bitbucket.org/multicoreware/x265)
源代码获取地址：
`hg clone http://hg.videolan.org/x265`
注意编译前需要安装：
(1)yasm。
(2)CMake。

**Windows下最简单编译步骤（VC2010）**
1.使用CMake生成x265.sln 文件。具体是在其代码根目录的“build”文件夹中，找到想要生成的“.sln”的版本。

![](https://img-blog.csdn.net/20141222001735714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

以本机为例，打开“vc10-x86”，运行其中的“make-solutions.bat”，即可生成VC2010的32位的解决方案文件。中间会弹出一个CMake的配置对话框，如图所示。可以直接先单击“Configure”，然后单击“Generate”即可。

![](https://img-blog.csdn.net/20141222001802546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2. 用VC打开生成的x265.sln，即可编译程序。可以右键解决方案，选择“生成解决方案”批量编译其中所有的工程。

![](https://img-blog.csdn.net/20141222001821156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





编译后生成：

x265.exe：可以直接使用的编码H.265的命令行程序

libx265.dll，libx265.lib：可以用于程序开发的编码H.265的类库（lib和dll分开）

x265-static.lib：：可以用于程序开发的编码H.265的类库（单独一个lib）


## VPX

![](https://img-blog.csdn.net/20141222001850891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




网站：[http://www.webmproject.org/](http://www.webmproject.org/)

源代码获取地址：


`git clone https://chromium.googlesource.com/webm/libvpx`


上述地址有可能访问不了，可以使用下面的镜像地址：


`git clone https://github.com/webmproject/libvpx`


注意编译前需要安装yasm。


**Windows下最简单编译步骤（MinGW）**
1.下载安装MinGW，运行后切换至源代码目录
2.输入 "./configure"
3.输入"make"

**Linux下最简单编译步骤**
1.切换至源代码目录
2.输入 "./configure"
3.输入"make"



编译后生成：
vpxdec.exe：可以直接使用的解码VP8，VP9命令行程序
vpxenc.exe：可以直接使用的编码VP8，VP9命令行程序
libvpx.a：可以用于程序开发的编码VP8，VP9的类库





**Windows下编译获得各个项目VS2010解决方案的步骤**

1.       configure的时候设置“—target”为“x86-win32-vs10”
`./configure --target=x86-win32-vs10`
2.       用VC打开生成的vpx.sln，即可用VS浏览整个解决方案。可以右键解决方案，选择“生成解决方案”批量编译其中所有的工程。

![](https://img-blog.csdn.net/20141222001914852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



