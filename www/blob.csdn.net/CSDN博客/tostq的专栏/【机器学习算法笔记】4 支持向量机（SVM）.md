# 【机器学习算法笔记】4. 支持向量机（SVM） - tostq的专栏 - CSDN博客





2017年03月29日 10:01:17[tostq](https://me.csdn.net/tostq)阅读数：980
所属专栏：[机器学习算法笔记](https://blog.csdn.net/column/details/15045.html)









# 【机器学习算法笔记】4. 支持向量机（SVM）

给定训练样本，支持向量机建立一个超平面作为决策曲面，使得正例和反例之间的隔离边缘最大化

## 4.1 线性模型

考虑到用于分离超平面的决策曲面： 
![这里写图片描述](https://img-blog.csdn.net/20170329094618305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

x到超平面的距离： 
![这里写图片描述](https://img-blog.csdn.net/20170329094630698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

现在我们考虑到要寻找一个超平面，使得全体x到超平面的距离都大于1：（这里d=1或-1） 
![这里写图片描述](https://img-blog.csdn.net/20170329094642978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（这里可以看出，d取1或-1和d取2或-2是一样，在是用于分割超平面的两半） 

并且满足权值向量最小化代价函数： 
![这里写图片描述](https://img-blog.csdn.net/20170329094706745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

建立拉格朗日函数（这里也可以看出d的值不会影响函数）： 
![这里写图片描述](https://img-blog.csdn.net/20170329094718652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

得到： 
![这里写图片描述](https://img-blog.csdn.net/20170329094732604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以变为： 
![这里写图片描述](https://img-blog.csdn.net/20170329094743682?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

计算出 
![这里写图片描述](https://img-blog.csdn.net/20170329094757948?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.2 不可分模式的最优超平面

上面情况是一个不具有分类误差的分离超平面，我更希望找到一个最优平面，使香分类平均误差最小，这样的分类边缘称为软的。 

我们将分类距离定义为： 
![这里写图片描述](https://img-blog.csdn.net/20170329094828839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最小化代价函数可简化为： 
![这里写图片描述](https://img-blog.csdn.net/20170329094839136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里的参数C用于控制不可分离点数之间的平衡，当C比较大的时，暗示训练样本质量较高，而当C较小时，说明其存在噪声 

所以不可分离情况与可分离情况，主要是在： 
![这里写图片描述](https://img-blog.csdn.net/20170329094854997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

支持向量的个数反映了隐藏空间特征的个数。
## 4.3 SVM的数学解释

SVM有两个方向去看待这个问题，第一个是从拉格朗日方向（也就是前面），第二个方向是直接从目标函数（损失项和正则项角度去看） 

 一般来说，监督学习可以看做最小化下面的目标函数： 
![这里写图片描述](https://img-blog.csdn.net/20170329095307768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170329095319691?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

逻辑回归中Logistic regression中损失函数![这里写图片描述](https://img-blog.csdn.net/20170329095410639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)称为log loss； 

而SVM中的 ![这里写图片描述](https://img-blog.csdn.net/20170329095437676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)称为hinge loss。 

这个方向上考虑SVM，虽然在式子相似，但在理论上是不同的，前者这里的那项C，这是在软间隔中引入的，即在不能线性可分的情况下，允许离群点存在。此时你希望分错的距离越少越好，就增大C，相当于提升了加号后式子的权重，减弱了加号前式子的权重，从而学出来的模型分错的点的距离会小，当C无穷大时，相当于没有错分的，也就是硬间隔。而后者的项C是加在正则项上的。
## 4.4 核方法支持向量机

超平面被写成如下形式： 
![这里写图片描述](https://img-blog.csdn.net/20170329095505474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

此时： 
![这里写图片描述](https://img-blog.csdn.net/20170329095519780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.5 SMO算法求解

首先回到我们前面一直悬而未解的问题，对偶函数最后的优化问题，这个问题可以由SMO算法来快速解决，SMO算法是最快的二次规划优化算法，特别针对线性SVM和数据稀疏时性能更优。 
![这里写图片描述](https://img-blog.csdn.net/20170329095603781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

按照坐标上升的思路，我们首先固定除α1以外的所有参数，然后在α1上求极值。等一下，这个思路有问题，因为如果固定α1以外的所有参数，那么α1将不再是变量（可以由其他值推出），因为问题中规定了 
![这里写图片描述](https://img-blog.csdn.net/20170329095617437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因此，我们需要一次选取两个参数做优化，比如α1和α2，此时α2可以由α1和其他参数表示出来。这样回带到W中，W就只是关于α1的函数了，可解。 

这样，SMO的主要步骤如下： 
![这里写图片描述](https://img-blog.csdn.net/20170329095647476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

意思是，第一步选取一对αi和αj，选取方法使用启发式方法。 

第二步，固定除αi和αj之外的其他参数，确定W极值条件下的αi，αj由αi表示。
## 4.6 RBF的SVM

径向基网络的支持向量机结构： 
![这里写图片描述](https://img-blog.csdn.net/20170329095715117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于径向基网络的支持向量机来说，其径向基函数的数目和中心分别由支持向量的个数和值来决定，其特别是： 

1、有意使特征（隐藏）空间的维数足够大，使得可以在整个空间建立超平面 

2、通过聚类方法找到中心。
## 4.7 SVM的核函数

支持向量机是建立在统计学习理论基础之上的新一代机器学习算法，支持向量机的优势主要体现在解决线性不可分问题，它通过引入核函数，巧妙地解决了在高维空间中的内积运算，从而很好地解决了非线性分类问题。 

构造出一个具有良好性能的SVM，核函数的选择是关键．核函数的选择包括两部分工作：一是核函数类型的选择，二是确定核函数类型后相关参数的选择．因此如何根据具体的数据选择恰当的核函数是SVM应用领域遇到的一个重大难题，也成为科研工作者所关注的焦点，即便如此，却依然没有得到具体的理论或方法来指导核函数的选取．

### 1、经常使用的核函数

核函数的定义并不困难，根据泛函的有关理论，只要一种函数 K ( x i , x j ) 满足Mercer条件，它就对应某一变换空间的内积．对于判断哪些函数是核函数到目前为止也取得了重要的突破，得到Mercer定理和以下常用的核函数类型： 

(1)线性核函数 
![这里写图片描述](https://img-blog.csdn.net/20170329095743891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

(2)多项式核 
![这里写图片描述](https://img-blog.csdn.net/20170329095756297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

(3)径向基核（RBF） 
![这里写图片描述](https://img-blog.csdn.net/20170329095807727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Gauss径向基函数则是局部性强的核函数，其外推能力随着参数 σ 的增大而减弱。多项式形式的核函数具有良好的全局性质。局部性较差。 

(4)小波核 
![这里写图片描述](https://img-blog.csdn.net/20170329095827384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

(5)样条核 
![这里写图片描述](https://img-blog.csdn.net/20170329095840619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

(6)Sigmoid核函数 
![这里写图片描述](https://img-blog.csdn.net/20170329095856416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

采用Sigmoid函数作为核函数时，支持向量机实现的就是一种多层感知器神经网络，应用SVM方法，隐含层节点数目(它确定神经网络的结构)、隐含层节点对输入节点的权值都是在设计(训练)的过程中自动确定的。而且支持向量机的理论基础决定了它最终求得的是全局最优值而不是局部最小值，也保证了它对于未知样本的良好泛化能力而不会出现过学习现象。2、核函数的选择

在选取核函数解决实际问题时，通常采用的方法有：一是利用专家的先验知识预先选定核函数；二是采用Cross-Validation交叉验证方法，即在进行核函数选取时，分别试用不同的核函数，归纳误差最小的核函数就是最好的核函数。三是混合核函数方法，该方法较之前两者是目前选取核函数的主流方法，主要是将不同的核函数结合起来后会产生更好的特性。

关于核函数的一些参考： 
[](http://blog.csdn.net/wsj998689aa/article/details/47027365)http://blog.csdn.net/wsj998689aa/article/details/47027365
[](http://blog.csdn.net/batuwuhanpei/article/details/52354822)http://blog.csdn.net/batuwuhanpei/article/details/52354822









