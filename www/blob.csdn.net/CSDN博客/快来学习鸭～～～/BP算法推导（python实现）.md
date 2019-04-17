# BP算法推导（python实现） - 快来学习鸭～～～ - CSDN博客





2017年05月25日 14:55:03[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：2798








这里默认了解前向传播，所以不在重述这块，如果对梯度下降有疑惑的，不知道原理的可以看我这篇博客 
[http://blog.csdn.net/sxf1061926959/article/details/66976356](http://blog.csdn.net/sxf1061926959/article/details/66976356)

我用线性回归讲了下梯度下降的原理以及一些理解。本篇主要在BP反向传播的推导，直接开始 
![这里写图片描述](https://img-blog.csdn.net/20170525141437558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

不太会画图，直接手画了一个。
## 隐藏层梯度求解过程

如上图所示，为一个输出层神经元，在计算输出层梯度的时候，我们不用去考虑前一层是如何输入的。所以我们用y来表示，图中的y(n)表示第n个样本在前一层的输出值，这一层的输入值。我们将当前节点定义为j。那么当前节点j的输入值之和为 
![这里写图片描述](https://img-blog.csdn.net/20170525141654399?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里的m是节点j前一层的输入节点的个数，其中包括一个偏置项b。这里的公式都很像，看公式注意下标。然后节点j的输出要经过激活函数，如图所示我们定义激活函数为![这里写图片描述](https://img-blog.csdn.net/20170525141906408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。那么当前节点j的输出值为 
![这里写图片描述](https://img-blog.csdn.net/20170525141955010?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图中还有一个![这里写图片描述](https://img-blog.csdn.net/20170525142050058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，这个表示输入样本的真实标签，这是用来度量网络输出和真实样本之间的误差的。此处定义误差 
![这里写图片描述](https://img-blog.csdn.net/20170525142124848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后为计算梯度，我们需要有一个损失函数，因为反传其实就是在求损失函数对权值的梯度。 

我们这里使用平方误差作为损失函数，所以损失函数为 
![这里写图片描述](https://img-blog.csdn.net/20170525142252590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里的C为全部的输出层神经节点 

好了，把前提说清楚了，就可以开始求梯度了 

我们现在开始求隐藏层权值的梯度 
![这里写图片描述](https://img-blog.csdn.net/20170525142402215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

写得简单点，用上面定义好符号的代替就是![这里写图片描述](https://img-blog.csdn.net/20170525142448509?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后我们按照上面的一个个把梯度求出来，在最后去把他们乘起来。 
![这里写图片描述](https://img-blog.csdn.net/20170525142720575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

继续 
![这里写图片描述](https://img-blog.csdn.net/20170525142757264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里注意下，分子上的求和符号展开后，除了含有![这里写图片描述](https://img-blog.csdn.net/20170525142933156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)的哪项外，其他所求的导数都为0，，因为对当前权值来说，他们都是常数，常数的导数为0.所以我们能得到 
![这里写图片描述](https://img-blog.csdn.net/20170525143112047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

现在把这些都放回原来的公式去 
![这里写图片描述](https://img-blog.csdn.net/20170525143159078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以看出来，当前求的梯度和前一层的输入直接相关。一旦有了梯度，我们要做的就是用这个梯度去更新权值，当然不能直接减，太大了，所以加入一个步长![这里写图片描述](https://img-blog.csdn.net/20170525143254657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，得到 
![这里写图片描述](https://img-blog.csdn.net/20170525143330611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后更新权值 
![这里写图片描述](https://img-blog.csdn.net/20170525143406334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里要注意，![这里写图片描述](https://img-blog.csdn.net/20170525143515711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)里我提前加了一个负号，所以，在后面公式中我用的![这里写图片描述](https://img-blog.csdn.net/20170525143605353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)去更新梯度，但是如果前面没加负号，那么这里应该是减号，因为我们要找的是梯度下降的方向。
## 隐藏层梯度计算

前面计算了输出层的梯度更新，这里我们要计算隐藏层。 

之所以输出层和隐藏层要分开计算，那是因为隐藏层更加复杂，可以想象一下，隐藏层的节点，会连接下一层的所有节点，那么在梯度反传的时候也要从这些连接的节点去获取梯度。所以隐藏层就不能只求一个节点的梯度了，但是原理还是一样的，也是求损失函数对当前权值的梯度，只是计算过程变得比前面一个复杂了一点点。 
![这里写图片描述](https://img-blog.csdn.net/20170525143937716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里我重新画了两个节点，左边这个表示隐藏层节点j，右边那个表示输出层节点k，我们刚才计算的就是右边那个，现在我们来计算前面这个。 

别看我这里只画了一个输出层节点k，但是真实情况不一定只有一个，如果有多个，那么当前的节点j一定会和其有链接，那么和其连接的节点就会有梯度反传。 

所以需要重新计算梯度，和输出层不同的是，输出层只需要去考虑一个输出神经元的损失，而隐藏层需要去考虑全部全部输出层节点的损失，如下 
![这里写图片描述](https://img-blog.csdn.net/20170525144210583?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在求输出层的时候，因为权值只和其中的一个e有关，所以上面的累加展开后，只有一项为非零，其他全是0。但是在这里，隐藏层和上面的每个节点都要连接，所以我们这里要对全部的e求导。 

得到 
![这里写图片描述](https://img-blog.csdn.net/20170525144338347?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后后面的就是差不多的了，一直求梯度求下去、 
![这里写图片描述](https://img-blog.csdn.net/20170525144428145?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里我直接写出来了，步骤和前面的差不多，自己试着推导下就知道了 

我们直接得到最终的梯度更新 
![这里写图片描述](https://img-blog.csdn.net/20170525144521412?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

看着是不是有点复杂，别急 

到这里，梯度更新的推导算是结束了，根据以上公式可以推导出任何一个权值的梯度 

然后我们可以看看这些梯度之间有那些共同点，我把前面推导好的输出层和隐藏层梯度拿下来，如下 
![这里写图片描述](https://img-blog.csdn.net/20170525144722429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上面一个是输出层的，下面一个是隐藏层的，可以发现，每个公式中，最后一项都是所求权值的输入值，导数第二项都是激活函数的导数值，剩下的为传下来的损失。所以在算法实现的时候，往往把前面的损失值和激活函数导数的乘积当作一项整体，这样就可以实现链式计算，方便代码实现。我们把他叫做局部梯度![这里写图片描述](https://img-blog.csdn.net/20170525144933477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，如上面两个公式可以改成如下式子（下标kj的是输出层的，下标ji的是隐藏层的）： 
![这里写图片描述](https://img-blog.csdn.net/20170525145046780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后隐藏层的前半部分式子可以继续合并 
![这里写图片描述](https://img-blog.csdn.net/20170525145229635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最终得到 
![这里写图片描述](https://img-blog.csdn.net/20170525145326027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

所以不管是求哪个权值的梯度，我们都可以化简为如下形式 
![这里写图片描述](https://img-blog.csdn.net/20170525145414487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这样算法实现起来就方便多了
## python实现

主程序 
![这里写图片描述](https://img-blog.csdn.net/20170525203250292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

定义好输入节点、输出节点、隐藏节点大小、以及学习率等
前向传播 
![这里写图片描述](https://img-blog.csdn.net/20170525203545997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

反向更新 
![这里写图片描述](https://img-blog.csdn.net/20170525203708817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[代码github地址](https://github.com/Shicoder/DeepLearning_Demo/tree/master/BackPropagation)

## **参考**

1.neural network and machine learning.









