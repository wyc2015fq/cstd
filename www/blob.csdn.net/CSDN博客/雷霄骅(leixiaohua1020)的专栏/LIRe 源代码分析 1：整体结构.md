# LIRe 源代码分析 1：整体结构 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月31日 18:52:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：25
个人分类：[MPEG7/图像检索](https://blog.csdn.net/leixiaohua1020/article/category/1362935)










注：此前写了一系列的文章，分析LIRe的源代码，在此列一个列表：

[LIRe 源代码分析 1：整体结构](http://blog.csdn.net/leixiaohua1020/article/details/13668405)
[LIRe 源代码分析 2：基本接口（DocumentBuilder）](http://blog.csdn.net/leixiaohua1020/article/details/13746567)
[LIRe 源代码分析 3：基本接口（ImageSearcher）](http://blog.csdn.net/leixiaohua1020/article/details/13770889)
[LIRe 源代码分析 4：建立索引（DocumentBuilder）[以颜色布局为例]](http://blog.csdn.net/leixiaohua1020/article/details/13774637)
[LIRe 源代码分析 5：提取特征向量[以颜色布局为例]](http://blog.csdn.net/leixiaohua1020/article/details/13775467)
[LIRe 源代码分析 6：检索（ImageSearcher）[以颜色布局为例]](http://blog.csdn.net/leixiaohua1020/article/details/13792905)
[LIRe 源代码分析 7：算法类[以颜色布局为例]](http://blog.csdn.net/leixiaohua1020/article/details/13999995)



LIRE（Lucene Image REtrieval）提供一种的简单方式来创建基于图像特性的Lucene索引。利用该索引就能够构建一个基于内容的图像检索(content- based image retrieval，CBIR)系统，来搜索相似的图像。在这里就不多进行介绍了，已经写过相关的论文：

[LIRE的使用：创建索引](http://blog.csdn.net/leixiaohua1020/article/details/11848717)

[LIRE的使用：搜索相似的图片](http://blog.csdn.net/leixiaohua1020/article/details/11849129)

[LIRe提供的6种图像特征描述方法的评测](http://blog.csdn.net/leixiaohua1020/article/details/12854597)

因为自己开发的媒资检索系统中用到了LIRe，而且可能还要将实验室自己研究的算法加入其中，因此我研究了一下它源代码的大体结构。

想要看LIRe源代码的话，需要将其源代码包添加进来，相关的教程比较多，在这里就不详细解释了。先来看一看它的目录结构吧。

注：开发环境是MyEclipse 9

![](https://img-blog.csdn.net/20131030193626640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

乍一看感觉包的数量实在不少，不急，让我们细细来看。所有的包的前缀都是“net.semanticmetadata.lire”，在这里把该目录当成是“根目录”，根目录中包含的类如上图所示。注：在下面的介绍中就不再提“net.semanticmetadata.lire”了。

根目录主要是一些接口，这些接口可以分为2类：

> 
DocumentBuilder：用于生成索引

ImageSearcher：用于检索


“lire.imageanalysis”里面存储的是lire支持的方法的实现类。每个类以其实现的方法命名。

![](https://img-blog.csdn.net/20131030193631906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这些方法的算法有的位于“lire.imageanalysis”的子包中。

比如CEDD算法的实现类位于“lire.imageanalysis.cedd”中；

ColorLayout算法的实现类位于“lire.imageanalysis.mpeg7”中。



“lire.impl”里面存储的是lire支持的方法的DocumentBuilder和ImageSearcher。命名规则是***DocumentBuilder或者***ImageSearcher（***代表方法名称）

![](https://img-blog.csdn.net/20131030193635187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
























