# LIRe提供的6种图像特征描述方法的评测 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月18日 16:35:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：30
个人分类：[MPEG7/图像检索](https://blog.csdn.net/leixiaohua1020/article/category/1362935)










Rui Gan等人（看名字来说应该是中国人，机构写的Sun Yat-sen University应该是中山大学，但是很不幸没有找到相应的中文论文）在论文《Using LIRe to Implement Image Retrieval System Based on Multi-Feature Descriptor》中，测试了开源基于内容的图像检索类库LIRe的各种图像特征的性能。在此记录一下以作参考。

这里再提一下LIRe的简介：LIRE（Lucene Image REtrieval）提供一种的简单方式来创建基于图像特性的Lucene索引。利用该索引就能够构建一个基于内容的图像检索(content- based image retrieval，CBIR)系统，来搜索相似的图像。LIRE使用的特性都取自MPEG-7标准： ScalableColor、ColorLayout、EdgeHistogram，目前已经支持其他更多的特性。此外该类库还提供一个搜索该索引的方法。



本文测试了LIRe提供的以下6种特征描述方法：

![](https://img-blog.csdn.net/20131018155335500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

实验以供选择了13个种类，一共100张图片做测试，这些图如下图所示（只是一部分）：

![](https://img-blog.csdn.net/20131018160055156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

测试的步骤不再多说，就是使用LIRe的6种特征描述方法分别建立6个索引，然后分别检索。最后得到的实验结果如图所示：

注：6种特征描述方法分别标以A，B，C，D，E，F，G。其中C为最常见的颜色直方图。

**查准率（Precision）**如下表所示。

![](https://img-blog.csdn.net/20131018160428250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20131018160440312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**查全率（Recall）**如下表所示。

![](https://img-blog.csdn.net/20131018161157031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**查全率和查准率合计如下表所示。**

左边一栏对不同种类的图片分别给出了最适合的特征描述方法。

右边一栏对不同种类的图片分别给出了6种方法结合后的查全率和查准率。

![](https://img-blog.csdn.net/20131018161211875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



论文地址：[http://www.cnki.net/KCMS/detail/detail.aspx?QueryID=2&CurRec=1&recid=&filename=SZZZ201208001534&dbname=IPFDLAST2013&dbcode=IPFD&pr=&urlid=&yx=&uid=WEEvREcwSlJHSldSdnQ1ZCtUQmpGUjNLZ0x6bWI1NVYySGhtNnloM2hQYWZGeVdLajVLemFKOTFiVzhjU2ZnPQ==&v=MTMxODZmUmRMRzRIOVBNcDQ5RlplNE1DQk5LdWhkaG5qOThUbmpxcXhkRWVNT1VLcmlmWnVWdUZpanNVN2pMS0Y0VE5q](http://www.cnki.net/KCMS/detail/detail.aspx?QueryID=2&CurRec=1&recid=&filename=SZZZ201208001534&dbname=IPFDLAST2013&dbcode=IPFD&pr=&urlid=&yx=&uid=WEEvREcwSlJHSldSdnQ1ZCtUQmpGUjNLZ0x6bWI1NVYySGhtNnloM2hQYWZGeVdLajVLemFKOTFiVzhjU2ZnPQ==&v=MTMxODZmUmRMRzRIOVBNcDQ5RlplNE1DQk5LdWhkaG5qOThUbmpxcXhkRWVNT1VLcmlmWnVWdUZpanNVN2pMS0Y0VE5q)



LIRe首页：[http://www.semanticmetadata.net/lire/](http://www.semanticmetadata.net/lire/)

相关下载：

LIRE 源代码：[http://download.csdn.net/detail/leixiaohua1020/6373719](http://download.csdn.net/detail/leixiaohua1020/6373719)

LireDemo （LIRE的例子）：[http://download.csdn.net/detail/leixiaohua1020/6372101](http://download.csdn.net/detail/leixiaohua1020/6372101)






