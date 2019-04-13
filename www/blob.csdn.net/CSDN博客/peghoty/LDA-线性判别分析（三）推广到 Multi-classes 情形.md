
# LDA-线性判别分析（三）推广到 Multi-classes 情形 - peghoty - CSDN博客


2013年09月26日 10:48:27[皮果提](https://me.csdn.net/peghoty)阅读数：4243


本来是要调研 Latent Dirichlet Allocation 的那个 LDA 的, 没想到查到很多关于 Linear Discriminant Analysis 这个 LDA 的资料。初步看了看，觉得数学味挺浓，一时引起了很大的兴趣；再看看，就有整理一份资料的冲动了。网上查到的相关文章大都写得不是很详细，而且在概念和记号等方面也比较混乱，因此，在整理本文时，我有意识地牵了一根主线，想让读者读起来有循序渐进的感觉，记号上也力求规范和统一。期间参考了若干文献，以及一些优秀的博客，如 JerryLead、LeftNotEasy、webdancer、xiaodongrush等的博文，在这里对他们的辛勤写作和无私分享表示感谢。文中的数学推导过程写得比较细，方便有需求的读者参考。此外，文中还通过加注的形式放入了一些自己的理解。 当然由于水平有限，错误遗漏之处在所难免, 希望读者朋友可以指出，也欢迎交流。


**目录**

**第 1 节**[预备知识](http://blog.csdn.net/itplus/article/details/12035573)
1.1 分类问题的描述
1.2 拉格朗日乘子法

**第 2 节**[Two-classes 情形的数学推导](http://blog.csdn.net/itplus/article/details/12038357)
2.1 基本思想
2.2 目标函数
2.3 极值求解
2.4 阀值选取

**第 3 节**[推广到 Multi-classes 情形](http://blog.csdn.net/itplus/article/details/12038441)
3.1 降维问题的描述
3.2 目标函数与极值求解
3.3 降维幅度

**第 4 节**[其他几个相关问题](http://blog.csdn.net/itplus/article/details/12038653)
![](https://img-blog.csdn.net/20130926101854140)![](https://img-blog.csdn.net/20130926101901000)![](https://img-blog.csdn.net/20130926101824156)![](https://img-blog.csdn.net/20130926101836296)![](https://img-blog.csdn.net/20130926101845796)


若需要本文完整的 PDF 文档，请点击[《线性判别分析（LDA）浅析》](http://download.csdn.net/detail/peghoty/6322919)进行下载！

相关链接
1.  JerryLead 的博文[《线性判别分析（Linear Discriminant Analysis）（一）》](http://www.cnblogs.com/jerrylead/archive/2011/04/21/2024384.html)
2.  JerryLead 的博文[《线性判别分析（Linear Discriminant Analysis）（二）》](http://www.cnblogs.com/jerrylead/archive/2011/04/21/2024389.html)
3.  LeftNotEasy 的博文[《机器学习中的数学（4）-线性判别分析（LDA），主成分分析（PCA）》](http://www.cnblogs.com/LeftNotEasy/archive/2011/01/08/lda-and-pca-machine-learning.html)
4.  webdancer 的博文[《LDA-linear discriminant analysis》](http://webdancer.is-programmer.com/2013/2/28/lda.37867.html)
5.  xiaodongrush 的博文[《线性判别式分析-LDA-Linear Discriminant Analysis》](http://www.cnblogs.com/pangxiaodong/archive/2011/10/21/2218484.html)
6.  peghoty 的博文[《关于协方差矩阵的理解》](http://blog.csdn.net/itplus/article/details/11452743)
7.  peghoty 的博文[《UFLDL教程学习笔记（四）主成分分析》](http://blog.csdn.net/itplus/article/details/11451327)


作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/12038441](http://blog.csdn.net/itplus/article/details/12038441)
欢迎转载/分享, 但请务必声明文章出处.


