# YCbCr空间中进行色彩调整的方法 - 工作笔记 - CSDN博客





2014年05月30日 11:12:41[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6386
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









今天看到一篇对我很有帮助的文档，给大家转转。

原文地址：[http://blog.sina.com.cn/s/blog_53a06e87010087dh.html](http://blog.sina.com.cn/s/blog_53a06e87010087dh.html)

YCbCr以极坐标表示，则Chroma和Hue的计算公式如下：

Chroma = Sqrt(Cb^2+Cr^2)

Hue    = Arctg(Cr/Cb)



下面我们给出了6种颜色的C与H的值：



            R    G    B    Y   Cb    Cr    C     H 

Red    255  0    0     76   84  235 250  72

Blue   0      0    255  29 235  106 257    24

Green 0    255 0     150 42    18  45     24

Yellow 255 255 0    226  0   150  150  270


Cyan 0 255 255      179 172 0    172 360


Magenta 255 0 255 105  214 235 319 50



那么现在我们就有了最基本的调整关系公式了。

在YCbCr空间中，一般情况下我们进行调整都是利用一个调整矩阵来实现的：

| Y'  |   |k1  k2  k3|    | Y|

|Cb'| =|k4  k5  k6| * |Cb|

|Cr' |   |k7  k8  k9|    |Cr| 

这里我们主要调整的就是这个矩阵的各个参数了，我们一般调整的物理参数主要有：Contrast,Cb_Gain,Cr_Gain,Hue这几种参数，那么这几个参数是如何作用的呢？下面一个一个来说吧：

Contrast：这个参数只需要调整第一行即可，一般的调整范围在（0～4）之间；

Cb_Gain：需要调整的是就是第二行了，范围大致也在（0～4）；

Cr_Gain：这个与Cb_Gain类似，调整的是第三行，范围（0～4）；

Hue：这个的调整是比较麻烦的，因为这个是在极坐标系内处理的，而从公式看，它又与Cb和Cr都有关系，但与Y无关。一般来说tan(Hue)=k6/k5=-k8/k9。调整的时候由于还要考虑到chroma的归一问题，所以还要进行如下限制：

k5^2+k6^2=1（不考虑cb_gain的时候）

k8^2+k9^2=1（不考虑cr_gain的时候）

这时候在上面的两个限制前提下，再加上tan(Hue)=k6/k5=-k8/k9，我们就可以按照Hue(0~360)来一度一度的进行调节了。




﻿﻿

﻿﻿



