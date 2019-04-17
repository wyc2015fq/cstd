# Deeply-Supervised Nets - 家家的专栏 - CSDN博客





2014年12月21日 17:10:41[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3292








# Deeply-Supervised Nets

[Chen-Yu Lee](http://arxiv.org/find/stat/1/au:+Lee_C/0/1/0/all/0/1), [Saining
 Xie](http://arxiv.org/find/stat/1/au:+Xie_S/0/1/0/all/0/1), [Patrick Gallagher](http://arxiv.org/find/stat/1/au:+Gallagher_P/0/1/0/all/0/1), [Zhengyou
 Zhang](http://arxiv.org/find/stat/1/au:+Zhang_Z/0/1/0/all/0/1), [Zhuowen Tu](http://arxiv.org/find/stat/1/au:+Tu_Z/0/1/0/all/0/1)


Our proposed deeply-supervised nets (DSN) method simultaneously minimizes classification error while making the learning process of hidden layers direct and transparent. We make an attempt to**boost**the classification performance by studying a
 new formulation in deep networks. Three aspects in convolutional neural networks (CNN) style architectures are being looked at: (1) transparency of the intermediate layers to the overall classification; (2) discriminativeness and robustness of learned features,
 especially in the early layers; (3) effectiveness in training due to the presence of the exploding and vanishing gradients. We introduce "companion objective" to the individual hidden layers, in addition to the overall objective at the output layer (a different
 strategy to layer-wise pre-training). We extend techniques from stochastic gradient methods to analyze our algorithm. The advantage of our method is evident and our experimental result on benchmark datasets shows significant performance
 gain over existing methods (e.g. all state-of-the-art results on MNIST, CIFAR-10, CIFAR-100, and SVHN).

示意图：

![](https://img-blog.csdn.net/20141221170830786)

prototxt解析:

![](https://img-blog.csdn.net/20141221170938537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



其中cccp layer是1*1kernel convolution层。。

paper下载：http://arxiv.org/abs/1409.5185


code 和prototxt文件下载：https://github.com/s9xie/DSN






