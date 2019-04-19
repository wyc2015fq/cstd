# Standford机器学习 聚类算法（clustering）和非监督学习（unsupervised Learning） - fanyun的博客 - CSDN博客
2017年11月26日 21:59:52[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：889
聚类[算法](http://lib.csdn.net/base/datastructure)是一类非监督学习算法，在有监督学习中，学习的目标是要在两类样本中找出他们的分界，训练数据是给定标签的，要么属于正类要么属于负类。而非监督学习，它的目的是在一个没有标签的数据集中找出这个数据集的结构把它自动聚成两类或者多类。
　　本讲主要介绍了最常用了一种聚类算法--K-means聚类算法。如果将数据集分成两类，即k=2，K-means算法过程如下：
1、首先任意选取两个不同的样本作为两类样本的中心
![](https://img-blog.csdn.net/20130711213433140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXNpbmduYW1lc3BhY2Vfc3Rk/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、K-means算法有两部，第一步cluster assignment step，遍历所有样本点，计算出每个样本点离两个中心的距离（这个距离不一定是欧几里得距离，根据不同需要选取）来判断样本点属于哪一个类别（比如某个样本点离红色的中心比离蓝色的中心近，那么就将这个样本点归类到红色这类中，反之亦然），每个样本归类之后就得到如下：
![](https://img-blog.csdn.net/20130711213527828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXNpbmduYW1lc3BhY2Vfc3Rk/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、K-means的第二部是move centroid step，这一步是根据前一步的分类结果，计算出他们的新的类的中心（计算方法可以是对所有样本的每一维取平均，得到每类的中心），计算出新的中心结果如下：
![](https://img-blog.csdn.net/20130711213612968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXNpbmduYW1lc3BhY2Vfc3Rk/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4、然后根据新计算出来的两个中心将每个样本点再归类，不断迭代以上两步，直到所有样本点不再改变（即收敛）
![](https://img-blog.csdn.net/20130711213656828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXNpbmduYW1lc3BhY2Vfc3Rk/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
K-Means正式定义如下，输入要求有一个整数k和m个样本点，然后按照上面讲到的过程一步一步迭代，直到收敛：
![](https://img-blog.csdn.net/20130711213740265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXNpbmduYW1lc3BhY2Vfc3Rk/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Andrew还讲到了一个例子，给不可分的数据集分类，就是一个T-shirt分类的例子，如下图：
![](https://img-blog.csdn.net/20130711213913078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXNpbmduYW1lc3BhY2Vfc3Rk/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
比如现在有这样一系列身高和体重的数据，在生产T-shirt的时候会将T-shirt分成L,M和S三个号，利用K-Means就可以将这三类分出来，这里取k=3。
看了上面，你对K-Means应该有一个直观的概念了，接下去来讲一下关于K-Means的理论，在前面的监督学习中，首先都是建立一个假设，然后用已有的带有标签的数据集来最小化这个cost function，那么，在K-Means中，估值函数可以帮助我们debug，如果估值函数没有预想中的不断减小最后收敛到一个极值，那么算法肯定是写的有问题了。看看我们写的K-means算法有没有正常工作，他的估值函数如下：
![](https://img-blog.csdn.net/20130711213952734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXNpbmduYW1lc3BhY2Vfc3Rk/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
解决算法陷入局部最优问题的方案当然是选取不同的初始值，多次运行kmeans算法，选取最小的cost function J作为最终的结果，这样就能取得一个很好的聚类结果。
一般运行K-means算法时，k（分类数）是人为指定的，但是人根本不知道样本数据中真实的类别是多少，比如上面这个例子中，反映在图上，很明显可以看出它可以聚类成三类，如果把k指定成两类或者其他，那么聚类效果会大打折扣。要区分k最常用的方法是elbow method，它的工作原理如下：
![](https://img-blog.csdn.net/20130711214039171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXNpbmduYW1lc3BhY2Vfc3Rk/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
随着k增大，J会不断变小，k=m时候，j就变成了0。通常选取的k是拐点最明显的那个值。如果学习曲线如上图右边那个曲线，那就说明数据没有很好的区分性，因此可以将数据分成你所需要的任意类。
转自：http://blog.csdn.net/jackie_zhu/article/details/9304507
