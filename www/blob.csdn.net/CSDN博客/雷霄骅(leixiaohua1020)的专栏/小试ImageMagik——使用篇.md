# 小试ImageMagik——使用篇 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年05月23日 00:04:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：50
个人分类：[ImageMagik](https://blog.csdn.net/leixiaohua1020/article/category/2283347)










ImageMagick的使用和开发的文章：

[小试ImageMagik——使用篇](http://blog.csdn.net/leixiaohua1020/article/details/26633219)

[小试ImageMagik——开发篇](http://blog.csdn.net/leixiaohua1020/article/details/26754089)

======================

前两天帮同学调了一个小程序，是有关ImageMagik的。ImageMagik之前早有耳闻，据说处理图片的功能很强大，几乎都能把PhotoShop给替代了，但是一直没有机会尝试。于是顺便尝试了一下，记录一下它的使用方法和开发方法。

![](https://img-blog.csdn.net/20140523000354921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

首先记录一下它的简介，是从百度百科上看到的：

ImageMagick是一套功能强大、稳定而且开源的工具集和开发包，可以用来读、写和处理超过89种基本格式的图片文件，包括流行的TIFF、JPEG、GIF、PNG等格式。利用ImageMagick，你可以根据web应用程序的需要动态生成图片, 还可以对一个（或一组）图片进行改变大小、旋转、锐化、减色或增加特效等操作，并将操作的结果以相同格式或其它格式保存，对图片的操作，即可以通过命令行进行，也可以用C/C++、Perl、Java、PHP、Python或Ruby编程来完成。



主页：[http://www.imagemagick.org/](http://www.imagemagick.org/)



**安装**

我自己的系统是Windows的，因此从网站上下载安装包之后，一路“Next”就可以安装完毕。需要注意的地方是，如果想要使用ImageMagick进行开发，需要在安装的过程中勾选“Install development headers and libraries for C and C++”

![](https://img-blog.csdn.net/20140522235051109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

安装完成后，目录如下图所示：

![](https://img-blog.csdn.net/20140522235057703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

介绍其中几个重要的文件：

**Lib文件夹：**开发需要使用的静态库文件。包含4个库（前3个分别对应3种接口）：


`CORE_RL_magick_.lib; CORE_RL_Magick++_.lib;CORE_RL_wand_.lib; X11.lib;`


**Include文件夹：**开发需要使用的头文件。包含3个文件夹（分别对应3种接口）：


`magick; Magick++; wand;`




***.dll：**开发和使用过程中需要使用的动态链接库文件。



***.exe：**各种工具，以命令行工具为主。只有一个工具是图形界面的——imdisplay.exe，是个图片浏览器。



**使用**

在这里简单介绍一下自己使用过的几个程序。

**convert.exe**

转换图像格式和大小，模糊，裁剪，驱除污点，图片上画图片，加入新图片，生成缩略图等。

**identify.exe**

描述一个或多个图像文件的格式和特性。

**mogrify.exe**

按规定尺寸制作一个图像，模糊，裁剪等。改写最初的图像文件然后写成一个不同的图像文件。

**composite.exe**

根据多个图片组合生成图片。

**montage.exe**

创建一些分开的缩略图像。可以给这些缩略图像添加修饰，比如边框、图片名称等。

**compare.exe**

在算术上和视觉上评估原始图片和处理后的图片。



**convert.exe使用示例**



转换JPEG格式图像到PNG格式图像



`convert rose.jpg rose.png`




转换JPEG格式图像到PNG格式图像，并缩小至原图片的50%



`convert rose.jpg -resize 50% rose.png`




可以使用很复杂的命令，例如




```
convert -size 320x85 canvas:none-font Bookman-DemiItalic -pointsize 72 \
-draw "text 25,60 'Magick'"-channel RGBA -blur 0x6 -fill darkred -stroke magenta \
-draw "text 20,55 'Magick'"fuzzy-magick.png
```





**identify.exe使用示例**

查看文件格式（简单）



`identify rose.jpg`



输出：



`rose.jpg JPEG 640x480 sRGB 87kb0.050u 0:01`



查看文件格式（详细）



`identify -verbose rose.jpg`



输出：




```
Image: rose.jpg
Format: JPEG (Joint PhotographicExperts Group JFIF format)
Class: DirectClass
Geometry: 70x46+0+0
Resolution: 72x72
Print size: 0.972222x0.638889
Units: PixelsPerInch
Type: TrueColor
Endianess: Undefined
Colorspace: RGB
Depth: 8-bit
Channel depth:
red: 8-bit
green: 8-bit
blue: 8-bit
Channel statistics:
red:
min: 37 (0.145098)
max: 255 (1)
mean: 145.58 (0.5709)
standard deviation: 67.3195(0.263998)
green:
min: 31 (0.121569)
max: 255 (1)
mean: 89.2512 (0.350005)
standard deviation: 52.0488(0.204113)
blue:
min: 17 (0.0666667)
max: 255 (1)
mean: 80.4075 (0.315323)
standard deviation: 54.0052(0.211785)
Rendering intent: Undefined
Interlace: None
Background color: white
Border color: rgb(223,223,223)
Matte color: grey74
Transparent color: black
Page geometry: 70x46+0+0
Dispose: Undefined
Iterations: 0
Compression: JPEG
Quality: 100
Orientation: Undefined
Properties:
date:create:2008-01-08T11:08:52-05:00
date:modify:2005-04-07T12:48:13-04:00
jpeg:colorspace: 2
jpeg:sampling-factor: 2x2,1x1,1x1
signature: dcd00d6303f80f4fa53f991804cb026151c1e851b7a96255e82da87299583ebc
Artifacts:
verbose: true
Tainted: False
Filesize: 3.97266kb
Number pixels: 3.14453kb
Version: ImageMagick 6.8.9-12014-04-11 Q16 http://www.imagemagick.org
```





**mogrify.exe**

调整图像的尺寸至原来的50%



`mogrify -resize 50% rose.jpg`


![](https://img-blog.csdn.net/20140522235108234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




将文件夹下所有JPEG的最大尺寸调整为256x256



`mogrify -resize 256x256 *.jpg`




**composite.exe**

将smile.gif叠加到rose-over.png上



`composite -gravity center smile.gifrose: rose-over.png`


![](https://img-blog.csdn.net/20140522235113203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




**montage.exe**

将两张图片简单合成为一张图片



`montage -background #336699 -geometry+4+4 rose.jpg red-ball.png montage.jpg`


![](https://img-blog.csdn.net/20140522235122531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






在每张图像周围加一些装饰



`montage -label %f -frame 5-background #336699 -geometry +4+4 rose.jpg red-ball.png frame.jpg`



**![](https://img-blog.csdn.net/20140522235127421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

**compare.exe**

比较两张图片的不同



`compare rose.jpg reconstruct.jpgdifference.png`




查看两张图片中红色通道的PSNR



`compare -channel red -metric PSNRrose.jpg reconstruct.jpg difference.png`




查看两张图片的PSNR



`compare -metric PSNR rose.jpgreconstruct.jpg difference.png`



![](https://img-blog.csdn.net/20140522235131000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)











