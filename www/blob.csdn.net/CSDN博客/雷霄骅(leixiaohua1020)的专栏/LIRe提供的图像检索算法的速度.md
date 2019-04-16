# LIRe提供的图像检索算法的速度 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月22日 23:19:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：24
个人分类：[MPEG7/图像检索](https://blog.csdn.net/leixiaohua1020/article/category/1362935)










本文翻译了LIRe的作者Mathias Lux发表的论文《LIRe: Lucene Image Retrieval - An Extensible Java CBIR Library》。主要介绍了LIRe的功能。节选了算法性能部分的内容。



在LIRe中主要实现的图像特征有：
 1. RGB和HSV空间的颜色直方图；
 2. MPEG-7的颜色特征，包括Scalable color，Color layout和Edge Histogram；
 3. Tamura纹理特征，包括粗糙度（Coarseness），对比度（Contrast）和方向度（Directionality）；
 4. 颜色和边缘的方向性描述符（Color and edge directivity descriptor, CEDD）；
 5. 模糊颜色和纹理直方图（Fuzzy color and texture histogram, FCTH）；
 6. 颜色关联图（Auto color correlation）；
 7. 尺度不变特征变换（Scale-invariant feature transform , SIFT）。



这些算法的性能表如下图所示。测试使用的计算机配置是：IntelCore 2 Quad 4核 CPU 2.4 GHz，2 GB RAM内存，
 Windows XP ， Java 1.6 u6 。使用单线程的方式。

![](https://img-blog.csdn.net/20131122142620328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以看出Auto color correlation以及Tamura还是比较耗时的。其次是FCTH以及CEDD。剩下的几种算法还是比较节约时间的。




