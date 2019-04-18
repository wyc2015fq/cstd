# 图像检索中为什么仍用BOW和LSH - wishchinYang的专栏 - CSDN博客
2014年06月30日 12:32:47[wishchin](https://me.csdn.net/wishchin)阅读数：932
个人分类：[图像检索](https://blog.csdn.net/wishchin/article/category/1673923)
原文链接：[http://blog.csdn.net/jwh_bupt/article/details/27713453](http://blog.csdn.net/jwh_bupt/article/details/27713453)
       去年年底的时候在一篇[博客中](http://blog.csdn.net/jwh_bupt/article/details/17540561)，用ANN的框架解释了BOW模型[1]，并与LSH[2]等哈希方法做了比较，当时得出了结论，BOW就是一种经过学习的Hash函数。去年再早些时候，又简单介绍过LLC[3]等稀疏的表示模型，当时的相关论文几乎一致地得出结论，这些稀疏表示的方法在图像识别方面的性能一致地好于BOW的效果。后来我就逐渐产生两个疑问：
1）BOW在检索时好于LSH，那么为什么不在任何时候都用BOW代替LSH呢？
2）既然ScSPM，LLC等新提出的方法一致地好于BOW，那能否直接用这些稀疏模型代替BOW来表示图像的特征？
       粗略想了一下，心中逐渐对这两个问题有了答案。这篇博文我就试图在检索问题上，谈一谈Bag-of-words模型与LSH存在的必要性。
**一、回顾LSH      **
         LSH方法本身已经在很多文章中有过介绍，大家可以参考[这里](http://blog.csdn.net/icvpr/article/details/12342159)和[这里](http://www.strongczq.com/2012/04/locality-sensitive-hashinglsh%E4%B9%8B%E9%9A%8F%E6%9C%BA%E6%8A%95%E5%BD%B1%E6%B3%95.html)。其主要思想就是在特征空间中对所有点进行多次随机投影（相当于对特征空间的随机划分），越相近的点，随机投影后的值就越有可能相同。通常投影后的值是个binary
 code（0或者1），那么点xi经过N次随机投影后就可以得到一个N维的二值向量qi，qi就是xi经过LSH编码后的值。
         问题是LSH是一种随机投影（见图1），上篇博客中也提到这样随机其实没有充分利用到样本的实际分布信息，因此N需要取一个十分大的数才能取得好的效果。因此，[2]中作者理所当然地就想到对LSH的投影函数进行学习（用BoostSSC和RBM来做学习），效果可以见图3。经过学习的LSH就可以通过更少的投影函数取得更好的区分性。这就和BOW的作用有点像了（都是通过学习对原始的特征空间进行划分），只不过BOW对特征空间的划分是非线性的（见图2），而LSH则是线性的。
![](https://img-blog.csdn.net/20140605171909609)
图1
![](https://img-blog.csdn.net/20140605171926125)
图2
![](https://img-blog.csdn.net/20140605171916765)
图3
**二、LSH VS BOW：检索的时候对什么特征做编码？**
    （ 以下对LSH的介绍将不区分是否利用BoostSSC和RBM来做学习）。LSH一般是对图像的全局特征做LSH。比如图像的GIST，HOG，HSV等全局的特征。可以说，LSH是将一个特征编码成另外一个特征。这有一点降维的味道。经过N次随机投影后，特征被降维为一个长度为N的二值特征了。
BOW一般是对图像的局部特征做编码，比如SIFT，MSER等。BOW是将一组特征（局部特征）编码成一个特征（全局特征），带有一种aggregation的性质。这是它与LSH最大的不同之处。
**三、LSH VS BOW：检索和排序的过程有何不同？**
先来说说LSH。假设两个样本x1和y1经过LSH编码后得到q1和q2，那么两个样本之间的相似度可以这么计算：
## ![](http://latex.codecogs.com/gif.latex?sim%28x_%7B1%7D%2Cx_%7B2%7D%29%3A%3Dsim%28q_%7B1%7D%2Cq_%7B2%7D%29%20%3D%20%5Csum_%7Bi%3D1%7D%5E%7BN%7Dq_%7B1%7D%28i%29q_%7B2%7D%28i%29)    （1）
这就是LSH编码后两个样本之间的[汉明距离](http://en.wikipedia.org/wiki/Hamming_distance)。假设我们有一个dataset，把dataset里面的图片记做di。有一个查询图片query，记做q。假设已经对dataset和query的所有图片经过LSH编码了，会有两种方式进行图片检索:
a) 建立一张哈希表，di编码后的code做为哈希值（键值）。每个di都有唯一的一个键值。query编码后，在这张哈希表上进行查找，凡是与query不超过D个bits不同的codes，就认为是与query近邻的，也就把这些键值下的图片检索出来。这种做法十分快速（几乎不用做任何计算），缺点在于Hash table将会非常大，大小是。
b) 如果N大于30（这时(a)中的hash table太大了），通常采用exhaustive search，即按照（1）式计算q到di的hamming距离，并做排序。因为是binary code，所以速度会非常快（12M图片不用1秒钟就能得到结果）。
再来说说BOW如何做检索（这个大家都很熟悉了）。假设已经通过BOW得到了图像的全局特征向量，通常通过计算两个向量的直方图距离确定两个向量的相似度，然后进行排序。因为BOW特征是比较稀疏的，所以可以利用[倒排索引](http://diducoder.com/mass-data-topic-8-inverted-index.html)提高检索速度。
**四、BOW能否代替LSH**
       BOW是从一组特征到一个特征之间的映射。你可能会说，当“一组特征”就是一个特征（也就是全局特征）的时候，BOW不也能用来对全局特征做编码么？这样做是不好的，因为这时BOW并不和LSH等效。为什么呢？一幅图像只能提取出一个GIST向量，经过BOW编码后，整个向量将会只有在1个bin上的取值为1，而在其他bin上的取值为0。于是乎，两幅图像之间的相似度要么为0，要么为1。想像在一个真实的图像检索系统中，dataset中的相似度要么是0要么是1，相似的图片相似度都是1，被两级化了，几乎无法衡量相似的程度了。所以说BOW还是比较适合和局部特征搭配起来用。其实LSH的索引a）方法也很类似，Hash值（codes）一样的图像之间是比较不出相似程度的。确实也如此，但是LSH和BOW相比仍然有处不同，便是经过LSH编码后不会像BOW那样极端（整个向量只有1个值为1，其它值为0）。所以通过1）式计算出的相似度依然能够反映两特征原始的相似度。所以在比较全局特征的时候，还是LSH比较好用些。
**五、LSH能否代替BOW**
        BOW在处理局部特征的时候，相当于两幅图像之间做点点匹配。如果把LSH编码的所有可能级联成一维向量的话，我觉得在一定程度上是起到了BOW相似的作用的。
**六、LLC能否代替BOW**
         不完全可以吧。尽管在识别问题上，LLC性能是比BOW好，但是由于HKM[4]和AKM[5]的提出，BOW的码书可以训练到非常大（可以达到1000000维）。而LLC之类的学习方法就没那么幸运了，说到天上去也就几万维吧。尽管相同维数下BOW性能不那么好，但是放到100万维上，优势就体现出来了。所以在检索问题上，BOW依然如此流行。
---------------------------- 
参考文献：
[1]Video Google: A Text Retrieval Approach to Object Matching in Videos
[2]Small Codes and Large Image Databases for Recognition
[3]Locality-constrained Linear Coding for Image Classification
[4]Scalable Recognition with a Vocabulary Tree
[5]Object retrieval with large vocabularies and fast spatial matching
作者的另外一篇文章：[CVPR14与图像视频检索相关的论文](http://blog.csdn.net/jwh_bupt/article/details/23039357)：[http://blog.csdn.net/jwh_bupt/article/details/23039357](http://blog.csdn.net/jwh_bupt/article/details/23039357)
