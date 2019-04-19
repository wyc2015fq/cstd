# 入门视频采集与处理（学会分析YUV数据） - maopig的专栏 - CSDN博客
2012年04月19日 11:10:13[maopig](https://me.csdn.net/maopig)阅读数：2097标签：[存储																[matlab																[h.264](https://so.csdn.net/so/search/s.do?q=h.264&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
个人分类：[视频信号处理](https://blog.csdn.net/maopig/article/category/1127356)
标签：[分析](http://blog.51cto.com/tagindex.php?keyword=%B7%D6%CE%F6)[码流](http://blog.51cto.com/tagindex.php?keyword=%C2%EB%C1%F7)[视频采集](http://blog.51cto.com/tagindex.php?keyword=%CA%D3%C6%B5%B2%C9%BC%AF)[RGB](http://blog.51cto.com/tagindex.php?keyword=RGB)[YUV](http://blog.51cto.com/tagindex.php?keyword=YUV)
原创作品，允许转载，转载时请务必以超链接形式标明文章 [原始出处](http://ticktick.blog.51cto.com/823160/555791) 、作者信息和本声明。否则将追究法律责任。[http://ticktick.blog.51cto.com/823160/555791](http://ticktick.blog.51cto.com/823160/555791)
     做视频采集与处理，自然少不了要学会分析YUV数据。因为从采集的角度来说，一般的视频采集芯片输出的码流一般都是YUV数据流的形式，而从视频处理（例如H.264、MPEG视频编解码）的角度来说，也是在原始YUV码流进行编码和解析，所以，了解如何分析YUV数据流对于做视频领域的人而言，至关重要。本文就是根据我的学习和了解，简单地介绍如何分析YUV数据流。
    YUV，分为三个分量，“Y”表示明亮度（Luminance或Luma），也就是灰度值；而“U”和“V” 表示的则是色度（Chrominance或Chroma），作用是描述影像色彩及饱和度，用于指定像素的颜色。
    与我们熟知的RGB类似，YUV也是一种颜色编码方法，主要用于电视系统以及模拟视频领域，它将亮度信息（Y）与色彩信息（UV）分离，没有UV信息一样可以显示完整的图像，只不过是黑白的，这样的设计很好地解决了彩色电视机与黑白电视的兼容问题。并且，YUV不像RGB那样要求三个独立的视频信号同时传输，所以用YUV方式传送占用极少的频宽。
    好了，言归正传，谈谈如何分析YUV码流吧。YUV码流有多种不同的格式，要分析YUV码流，就必须搞清楚你面对的到底是哪一种格式，并且必须搞清楚这种格式的YUV采样和分布情况。下面我将介绍几种常用的YUV码流格式，供大家参考。
**1.  采样方式**
    YUV码流的存储格式其实与其采样的方式密切相关，主流的采样方式有三种，YUV4:4:4，YUV4:2:2，YUV4:2:0，关于其详细原理，可以通过网上其它文章了解，这里我想强调的是如何根据其采样格式来从码流中还原每个像素点的YUV值，因为只有正确地还原了每个像素点的YUV值，才能通过YUV与RGB的转换公式提取出每个像素点的RGB值，然后显示出来。
    用三个图来直观地表示采集的方式吧，以黑点表示采样该像素点的Y分量，以空心圆圈表示采用该像素点的UV分量。
    先记住下面这段话，以后提取每个像素的YUV分量会用到。
- YUV 4:4:4采样，每一个Y对应一组UV分量。 
- 
- YUV 4:2:2采样，每两个Y共用一组UV分量。 
- 
- YUV 4:2:0采样，每四个Y共用一组UV分量。 
**2.  存储方式**
    下面我用图的形式给出常见的YUV码流的存储方式，并在存储方式后面附有取样每个像素点的YUV数据的方法，其中，Cb、Cr的含义等同于U、V。
**（1） YUVY 格式 （属于YUV422）**[](http://img1.51cto.com/attachment/201104/202327880.png)
![](http://img1.51cto.com/attachment/201104/202410157.png)
    YUYV为YUV422采样的存储格式中的一种，相邻的两个Y共用其相邻的两个Cb、Cr，分析，对于像素点Y'00、Y'01 而言，其Cb、Cr的值均为 Cb00、Cr00，其他的像素点的YUV取值依次类推。
**（2） UYVY 格式 （属于YUV422）**
![](http://img1.51cto.com/attachment/201104/202455202.png)
    UYVY格式也是YUV422采样的存储格式中的一种，只不过与YUYV不同的是UV的排列顺序不一样而已，还原其每个像素点的YUV值的方法与上面一样。
**（3） YUV422P（属于YUV422）**
    YUV422P也属于YUV422的一种，它是一种Plane模式，即打包模式，并不是将YUV数据交错存储，而是先存放所有的Y分量，然后存储所有的U（Cb）分量，最后存储所有的V（Cr）分量，如上图所示。其每一个像素点的YUV值提取方法也是遵循YUV422格式的最基本提取方法，即两个Y共用一个UV。比如，对于像素点Y'00、Y'01 而言，其Cb、Cr的值均为 Cb00、Cr00。
**（4）YV12，YU12格式（属于YUV420）**
    YU12和YV12属于YUV420格式，也是一种Plane模式，将Y、U、V分量分别打包，依次存储。其每一个像素点的YUV数据提取遵循YUV420格式的提取方式，即4个Y分量共用一组UV。注意，上图中，Y'00、Y'01、Y'10、Y'11共用Cr00、Cb00，其他依次类推。
**（5）NV12、NV21（属于YUV420）**
    NV12和NV21属于YUV420格式，是一种two-plane模式，即Y和UV分为两个Plane，但是UV（CbCr）为交错存储，而不是分为三个plane。其提取方式与上一种类似，即Y'00、Y'01、Y'10、Y'11共用Cr00、Cb00
**3.  总结**
    几种常见的YUV码流格式就简单地列在上面了，大家在处理YUV码流前，先了解清楚自己的码流到底属于哪一种，然后对应进行处理。
    最后，再回答一个疑问，即分析清楚YUV码流格式了，我们可以做什么？最常用的一点就是，提取出所有的Y分量，然后利用vc或者matlab把你采集的图像的灰度值（Y分量）显示处理，这样你就可以很快地知道你采集的图像是否有问题了。后面我将继续写一些文章讲述如何提取、转换、显示这些YUV原始码流，有兴趣可以继续关注，欢迎留言讨论。
![](https://img-my.csdn.net/uploads/201205/08/1336491065_6775.jpg)
