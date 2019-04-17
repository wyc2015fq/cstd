# DSD（Dense-Sparse-Dense Training）算法详解 - AI之路 - CSDN博客





2017年09月03日 10:14:47[AI之路](https://me.csdn.net/u014380165)阅读数：4072








论文：DSD： Dense-Sparse-Dense Training for Deep Neural Networks 

论文链接：[https://arxiv.org/pdf/1607.04381.pdf](https://arxiv.org/pdf/1607.04381.pdf)

模型下载地址：[https://songhan.github.io/DSD](https://songhan.github.io/DSD).
**这是Song Han发在ICLR2017上的文章，我们知道Song Han的研究领域主要是模型压缩，模型加速等，但是这篇DSD（Dense-Sparse-Dense）却是关注如何通过改进训练过程提高传统模型的准确率。**个人觉得这是一篇挺有意思的文章，值得一看。

DSD是一种新的训练模型的方式，可以提高预训练模型的准确率。DSD和dropout不一样，虽然都是在训练过程中有prune（剪枝）操作，但是DSD是有一定依据来选择去掉哪些connection，而dropout是随机去掉。另外DSD也不是模型压缩算法，DSD算法训练模型目的是提高准确率，而不是模型压缩。

**那么DSD的效果怎么样呢**？On ImageNet, DSD improved the Top1 accuracy of GoogLeNet by 1.1%, VGG-16 by 4.3%, ResNet-18 by 1.2% and ResNet-50 by 1.1%, respectively.

**DSD算法的训练过程主要分3步**，大致过程可以看这段话：**In the first D (Dense) step, we train a dense network to learn connection weights and importance. In the S (Sparse) step, we regularize the network by pruning the unimportant connections with small weights and retraining the network given the sparsity constraint. In the final D (re-Dense) step, we increase the model capacity by removing the sparsity constraint, re-initialize the pruned parameters from zero and retrain the whole dense network.** 这基本概括了本文的方法，可以参看Figure1。

![这里写图片描述](https://img-blog.csdn.net/20170903101126636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

想要更详细地了解这三步是怎么做的，可以看下面的Algorithm1。第一步是Initial Dense Training，这一步和正常的网络训练一样，只不过多计算了哪些connection是更重要的连接，**而计算的依据就是权重的绝对值（后面会介绍原因）**。第二步是Sparse Training，先将前面一步计算得到的不重要的connection去掉，然后再训练这个sparse network。这一步只有一个参数：sparsity，表示要去掉多少比例的connection，根据这个sparsity以及总的connection数量可以计算出阈值|Wi|，这样就可以进行prune了，而且网络每一层的sparsity都是一样的。作者通过实验得出当sparsity在25%到50%范围内的效果比较好。第三步是Final Dense Training，先将第二步prune的connection重新连接并初始化为0，然后再继续训练整个网络得到最后的结果，这一步的学习率采用第二步的1/10。

![这里写图片描述](https://img-blog.csdn.net/20170903101155763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

接下来两个式子是用来解释为什么用权重的绝对值来衡量该connection该不该去掉，也就是prune的依据。式子1是模型的loss function，那么我们希望网络稀疏后损失的增加要尽可能小，就是式子2要尽可能小，因为式子2中的第一项约等于0，第二项的二次求导计算比较复杂，Wi又有个平方计算，因此采用|Wi|作为prune的依据。

![这里写图片描述](https://img-blog.csdn.net/20170903101213142?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Figure2是DSD算法训练模型时候权重值的分布变化情况**。首先（a）是第一步训练完后的权重分布，可以看到大部分值等于0或接近0；在进行prune后的（b），等于0或接近0的那部分都去掉了，这是因为DSD采用的prune的依据是权重的绝对值小的connection需要去掉。（c）表示prune之后对sparsity网络进行训练，得到的权重分布比（b）要平滑。（d）表示进行第三步的Dense Training时先将之前prune的权重用0初始化。（e）表示最后一步Dense Training。

![这里写图片描述](https://img-blog.csdn.net/20170903101239935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**实验结果：**
**作者将DSD算法应用在CNN，RNN和LSTMs等类型的网络，数据集包括图像分类，speech recognition和caption generation等，发现效果都比原来的要好。**

**Table1是采用DSD算法在各个数据集上训练模型的Top-1 error对比**。baseline就是原来模型的效果。

![这里写图片描述](https://img-blog.csdn.net/20170903101330821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Table4是用DSD训练ResNet网络的结果**。Sparse表示只进行DSD算法的前面两步，也就是最后得到的是个稀疏的模型（类似常见的模型压缩方法中的prune），sparse的效果也还不错。

![这里写图片描述](https://img-blog.csdn.net/20170903101353735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其他更多实验结果可以看论文。

另外**discussion**部分很有意思。 
**escape saddle point。作者认为DSD算法效果较好的一个可能原因是escape saddle point，也就是在训练过程中跳过了鞍点**。如果你了解优化算法，应该知道在优化过程中鞍点和局部最优点是容易影响模型的表现，这些点的梯度接近于0，模型接近收敛。而通过在训练过程中的prune操作得到sparse network并继续训练，有可能跳过这样的点，从而再Dense Training的时候可以收敛到更好的结果。作者也提到这有点simulated annealing（模拟退火）的味道，也就是在优化过程中跳过了一些局部最优值。 
**robust re-initializaiton**。确实现在网络初始化一般就做一次，而DSD却在训练网络的过程中初始化了两次，挺有新意的做法。




