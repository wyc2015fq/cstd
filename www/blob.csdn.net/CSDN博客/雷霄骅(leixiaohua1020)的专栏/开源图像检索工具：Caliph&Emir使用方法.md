# 开源图像检索工具：Caliph&Emir使用方法 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月29日 00:25:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：40
个人分类：[MPEG7/图像检索](https://blog.csdn.net/leixiaohua1020/article/category/1362935)










Caliph&Emir是基于MPEG7的软件。它是用Java编写的开源软件。采用了lucene完成索引和检索功能。是研究MPEG7标准，图像检索等等方面不可多得的好工具。

在此介绍一下它们的基本使用方法。深入的研究还需要分析其源代码。Caliph是一个软件，Emir也是一个软件。

官网首页：[http://www.semanticmetadata.net/](http://www.semanticmetadata.net/)

注：查了一下Caliph&Emir的意思，毕竟这个名字还是不太常见的，好像是两个人名连接起来的。Caliph中文是哈里发，是伊斯兰领袖的称号。Emir中文是埃米尔，是对穆斯林统治者的尊称。两个王者啊，都是伊斯兰世界的，好霸气。。。

![](https://img-blog.csdn.net/20131224235147890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20131224235158640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从官方网站上就可以下载可以运行的版本。需要注意的是，Caliph&Emir好像只能在Jdk1.6环境下运行，而不能在Jdk1.7环境下运行。在Jdk1.7版本下会有错误，而这个错误是Jdk1.7本身固有的错误，也不知道这个Bug修复了没有。



**Caliph**

首先来看看Caliph。软件运行后界面如图所示。右边的栏中可以选择图片。在这里我选择了一张飞机场的照片。左边可以编辑图片的各种元数据信息：Who，Where，When，What，Why。

![](https://img-blog.csdn.net/20131224235600250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击选项卡上的Visuals。可以查看图片的特征向量，这些特征提取方法都是基于MPEG7标准的：Edge Histogram，Dominant Color，Color Layout，Scalable Color。比较有特色的是Color Layout还是以可视化的方式显示的。

![](https://img-blog.csdn.net/20131224235605828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

通过语义来描述图片。

![](https://img-blog.csdn.net/20131225000543453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Emir**

Emir软件运行后截图如下图所示。该软件主要用来创建索引和检索。使用前首先要创建一个索引。创建索引需要指定一个文件夹，而后Emir会对该文件夹下的所有图片文件创建索引。

![](https://img-blog.csdn.net/20131225001214140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以通过多种方式检索图片。在这里我使用图像的方式检索图片，即传入一张图片，然后查找图像库中相似的图片。可以使用多种检索方法。

![](https://img-blog.csdn.net/20131225001217921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

检索的结果如下图所示。

![](https://img-blog.csdn.net/20131225001221890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

官网首页：[http://www.semanticmetadata.net/](http://www.semanticmetadata.net/)












