# 基于BoF（Bag-of-feature)图像分类 - tostq的专栏 - CSDN博客





2016年03月04日 11:09:34[tostq](https://me.csdn.net/tostq)阅读数：4265








       图像分类一直是图像处理问题的热点，近年来基于BoF（bag-of-feature）特征袋方法倍受关注，特征袋方法（BoF)是词袋方法（BoW）一种改进。本文简要的介绍了基本BoF图像分类的思路和其目前的一些改进方法。




**字典学习过程（train a codebook）**

（1）特征提取：利用SIFT等局部描述子（SIFT方法最为常用，OpponentSIFT在各类SIFT改进方式综合表现最为优秀）提取图像的特征点，这个过程一般会生成非常多的特征点

（2）特征聚类：由于一般提取的特征点实在太多（每张训练图片上都会提取到成千上万的特征点），不太适合分析和操作，所以一般会利用K-Means聚类等方法将所以特征点分成K类，这里每个类，我们就称为一个词（codeword）

（3）形成字典：所有的类（词codeword）就构成字典（codebook）




**图像投影（这个过程也叫Feature Pooling）**

（1）特征提取：利用SIFT提取测试图像中所有的特征点

（2）查找对应词：就是找每个特征点对应的词

（3）构成图像特征词表：将字典内每个词出现频率构成图像特征向量（显然是K维向量）




**分类：**

       现在每张图像我们都可以得到一个K维特征向量（反映了字典各词的出现频率），现在就是传统的分类问题了，最常用的方法是SVM：）




**然而传统的BoF图像分类问题也存在问题：**

（1）SIFT局部描述子能较好的描述图像纹理信息（描述方向梯度），但是也有可能造成其他方面信息的丢失，所以一般解决方法是利用多描述子混合的思路。

（2）这种一个特征点对应于一个词的方法显然忽略图像空间上下文（Spatial Context Modeling），这很容易造成词语义的模糊，所以我们需要构建空间结构，更好的Feature Pooling :)

       最为成功的尝试就是SPM（Spatial Pyramid Matching 空间金字塔匹配），这个方法就是将图像分成多个尺度小块，这里图像每个尺度内的小分块都会得到一个特征词表，所以特征词表串联起来就得到整个图像特征词表，这样做好处是保留了图像空间信息，另外一种思路是在方向梯度空间内操作。

      另一种方法是引入词组（phrases），在由词（low-level）到主题（high-level）中引入中间层（mid-level），比如LLC引入特征点的位置约束。

![](https://img-blog.csdn.net/20160304110618534?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


       下面这种GPP的方法也很类似，其将特征点周围最近的K个特征点共同组成了一个词组(Phrase），此时整个Feature Pooling过程变为word-phrase-theme

![](https://img-blog.csdn.net/20160304110634050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        在BoF图像分类问题上，这方面（Better coding + pooling）的研究是重点，上面关于SPM和LLC，我只是简要说了一些目的和原理，不过网上有很多关于SPM及稀疏编码的文章可以参考，特别是这篇http://blog.csdn.net/jwh_bupt/article/details/9837555，转载也挺多的。

（3）图像中一般都存在同主题无关的内容，会影响分类的结果，改进方法是检测兴趣区域（detection of regions-of-interest(ROI)）并给不同的权重。



