# 【转自cvchina】online random forest - 三少GG - CSDN博客
2011年04月09日 23:08:00[三少GG](https://me.csdn.net/scut1135)阅读数：2779
[http://www.cvchina.info/2010/05/26/online-random-forest/](http://www.cvchina.info/2010/05/26/online-random-forest/)
优秀网站：[http://www.cvchina.info/tag/machine-learning/](http://www.cvchina.info/tag/machine-learning/)
2010年5月26日[CVUNC](http://www.cvchina.info/author/cvunc/)
一直忽悠cvchina，心有歉意。第一次发文，简单写点online learning的东西。
传统的SVM和adaboost都是batch mode learning. **所谓的batch mode learning， 简单说，就是所有的训练数据都是available的（或则说所有训练数据都已经在内存中）。**这种方法主要有2个缺点：
1）  有时候数据量太大，在内存中放不下，处理起来不方便
2）  由于应用环境限制，有时候无法在训练之前得到所有训练数据
而Online learning， 他的数据是come in sequence，也就是说traning sample 是一个一个来，或则是几个几个来，然后classifer 根据新来的samples进行更新。Online learning是比较困难的，主要原因是你无法知道将来的训练数据是如何的。显然adaboost和svm是行不通的。最近几年也有一些人做online learning的研究，主要方法还是集中在online boosting这一块。推荐2篇不错的文章：
1）  online adaboost [1], 并把他用在object tracking等方面。这篇文章发表于CVPR2006引用率蛮高。这几年的cvpr上的几篇做**tracking**的文章以这个idea为基础。tracking的方法是用最近比较流行的tracking-by-detection的方法。简答的说就是在tracking的时候，observation model这一块是用一个在线训练的分类器。tracking的过程如下图所示（图中还有一步是用跟踪的结果作为训练器的新的输入）：
![](http://www.vision.ee.ethz.ch/boostingTrackers/images/overviewBoost.png)
这篇文章online 训练的时候，用tracking 的结果作为online classifier的positive samples。显然这种数据是有噪声的，最后tracking会drift掉。而且需要指出的是，这种方法没有严格证明，只是模仿batch mode adaboost. 我把这个算法用在uci的训练数据上，效果不是很好。作者的主页是：[http://www.vision.ee.ethz.ch/~hegrabne/](http://www.vision.ee.ethz.ch/~hegrabne/). 这个是他用online learning 做tracking的项目主页：[http://www.vision.ee.ethz.ch/boostingTrackers/](http://www.vision.ee.ethz.ch/boostingTrackers/)。 有现成代码和demo。
2）  去年的一篇论文是关于online random forest[2]。网上有现成的代码。我跑了一下，挺牛逼的，效果没比offline mode差不多。如果你需要做online learning的话十分推荐。
[1].On-line Boosting and Vision. 06CVPR.
[2]. Online random forest. 09ICCV workshop on online machine learning.
PS：code的原始链接失效了，我传了一份到mediafire。[下载链接](http://www.mediafire.com/?lptqanko4occw19)
