# HEVC（H.265）标准的编码器（x265，DivX265）试用 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年01月30日 11:29:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：47









基于HEVC（H.265）的的应用级别的编码器发展的速度很快。所说的应用级别，就是指速度比较快的，有实际应用价值的编码器。目前可以直接使用的有两个：x265，DivX265。

**DivX265**

DivX265是DivX发布的HEVC编码器，不开源，但是免费，是基于控制台的。

官方网站：[http://labs.divx.com/divx265](http://labs.divx.com/divx265)

自己上传了一个exe（Windows）：[http://download.csdn.net/detail/leixiaohua1020/6890879](http://download.csdn.net/detail/leixiaohua1020/6890879)

官方网站上有个性能测试：

![](https://img-blog.csdn.net/20140130111107968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140130111141953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



使用方法简介：


`DivX265 -i <infile> -o <outfile> -br <bitrate> -s <w>x<h> (for raw infile)`


或: DivX265 -i <infile> -o <outfile> -br <bitrate> (for .avs|.avi infile)

或: DivX265 -h (for help)



**x265**

x265是开源的项目，发展的速度很快，程序接口还在变动。记得之前有人曾经想把它的库libx265集成到ffmpeg中，这样ffmpeg就能编码HEVC了。但是前一阵子我试了一下发现不行，原先写的那个集成用的patch已经不能用了。

x265源代码编译还是比较简单的，试了一下，基本上是“一路顺风”，没有遇到什么错误，直接生成了可执行文件。

官方网站：[http://x265.org/](http://x265.org/)

编译好的版本（Windows）：[http://download.csdn.net/detail/leixiaohua1020/6890893](http://download.csdn.net/detail/leixiaohua1020/6890893)

源代码地址：[https://bitbucket.org/multicoreware/x265](https://bitbucket.org/multicoreware/x265)

其他资源：

[x265.cc](https://x265.cc/)--- Unofficial buildbot。包含了最新版本的exe。

[x265GUI](https://bitbucket.org/ozok/x265gui/downloads) --- by ozok。x265图形界面工具。

[avs4x265](http://kurtnoise.free.fr/x265/) --- by Kurtnoise

[x265.ru](http://x265.ru/)--- by Fllear

使用方法简介：

最简单的命令类似：


`x265 --input-res 480x272 --fps 25 src01_480x272.yuv -o src01_x265.h265`
详细的命令可以查看帮助。



**ffmpeg+x265，DivX265**

ffmpeg和x265，DivX265联合使用可以通过pipe实现。可以直接将ffmpeg的输出指定为x265或DivX265的输入。命令如下所示：


`ffmpeg -i cuc_ieschool.flv -f rawvideo -pix_fmt yuv420p - | DivX265.exe -s 512x288 -br 100 -i - -o cuc_ieschool.hevc`









