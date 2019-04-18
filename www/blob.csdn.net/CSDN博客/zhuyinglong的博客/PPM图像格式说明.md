# PPM图像格式说明 - zhuyinglong的博客 - CSDN博客





2017年09月24日 18:12:33[初见时的微笑](https://me.csdn.net/zhuyinglong2010)阅读数：308








**声明：下文所提到的概念说明均摘自于网络，加上个人理解总结而成。**



PPM图像格式(Portable Pixelmap)是一种linux图片格式，可用ACDSEE批量转变。它是一种简单的图像格式，仅包含格式、图像宽高、位数信息和图像数据。图像数据的保存格式可以用ASCII码，也可用二进制。与PPM格式类似的还有PBM,PGM两种图片格式。
PBM 是位图（BitMap），仅有黑与白，没有灰。

PGM 是灰度图（GrayMap）。

PPM 是真彩色图（PixMap），通过RGB三种颜色显现的图像。

以上三种格式图像文件的开头都通过2个字节「magic number」来表明文件格式的类型（PBM, PGM, PPM）以及编码方式（ASCII 或 Binary），「magic number」分别为P1、P2、P3、P4、P5、P6。对应关系如下表：

![](https://img-blog.csdn.net/20170924183303828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面以PPM格式为例，对以上三种格式进行说明。PPM图像格式分为两部分，分别为头部分和图像数据部分。

1、头部分：

该部分由3部分组成，通过换行或空格进行分割。

第1部分：P3或P6，指明PPM的编码格式，一般PPM的标准是空格。

第2部分：图像的宽度和高度，通过ASCII表示，

第3部分：最大像素值，0-255字节表示。

2、图像数据部分：

图像数据的存储方式有ASCII与Binary两种方式。ASCII格式：按RGB的顺序排列，RGB中间用空格隔开，图片每一行用回车隔开。Binary格式：用24位代表每一个像素，红绿蓝分别占用8位。不用空格分隔，占用空间容量更少。

例子：

P6\n

       width height\n

　　255\n

　　rgbrgb…
其中，P6表示PPM以Binary格式进行存储；\n表示换行符；width和height表示图像的宽高，用空格隔开；255表示每个颜色分量的最大值；rgb数据从左到右，从上到下排放。


