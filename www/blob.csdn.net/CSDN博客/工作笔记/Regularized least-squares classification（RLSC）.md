# Regularized least-squares classification（RLSC） - 工作笔记 - CSDN博客





2016年08月17日 11:41:18[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6225








原文：

http://blog.csdn.net/ikerpeng/article/details/40147733

在机器学习或者是模式识别当中有一种重要的分类器叫做：SVM 。这个被广泛的应用于各个领域。但是其计算的复杂度以及训练的速度是制约其在实时的计算机应用的主要原因。因此也很很多的算法被提出来，如SMO，Kernel的方法。




但是这里要提到的 [Regularized
 least-squares classification](http://www.researchgate.net/publication/2833962_Regularized_Least-Squares_Classification/file/32bfe50e3fcab4eb89.pdf) 是一个和他有着同样的效果的分类器。比较而言计算却比较的简单（We see that a Regularized Least-Squares Classification problem can be solved by solving a single system of linear

equations.）。接下来将对其进行介绍。




首先我们知道最终要求得的结果是： f（x）= <W, x> +b 

策略函数：

我们还是要来看一下他的策略：结构风险最小化Function。

![](https://img-blog.csdn.net/20141016144521218)


通过使用kernel的方法将x 投影到希尔伯特空间（只需要隐式的表示）得到的结果是：

![](https://img-blog.csdn.net/20141217103131763)（ 这里的alpha就是下面的c啦！）


带入到目标函数以及简化，我们得到最终要求的函数 f*(x):

![](https://img-blog.csdn.net/20141217103834187)


这就是最终要求的结果。后面额的核函数我们可以采用一些常用的核函数处理掉（比如说：高斯核（Gaussian Kernel）等）。那如何来解决ci勒？




如何解c：




大家都知道在SVM当中采用的是合页损失函数（hinge loss Function）。但是很显然这里是平方损失函数：

![](https://img-blog.csdn.net/20141016150305558)

同时我们对于上面的f*（x）带入到最开始的策略函数当中：

![](https://img-blog.csdn.net/20141016150524866)


于是我们再来通过求导，令导数等于0，解出这个方程：

![](https://img-blog.csdn.net/20141016152921223)


有没有发现异常的简单啊！




这里要注意的是： K 是一个n*n的方阵，对于训练来讲，没两个样本（投影到高维空间后）都要做内积才能够得到K。




但其实作者也说了，可以通过一个线性的问题来解决并不意味着它的时间复杂度和空间复杂度就小了：训练一个Kernel的时间还是需要很长的。同时也需要较大的空间来存储下这样的核：K。但是仍旧是可以通过一些的方法来明显的提高它的性能的。

结果的比较：

文章中提到：It took 10,045 seconds (on a Pentium IV running at 1.5 GhZ) to train 20 one-vs-all SVMs, and only 1,309 seconds to train the equivalent RLSC classifiers. At test time, both SVM and RLSC yield
 a linear hyperplane, so testing times are equivalent. 足见速度可以达到SVM的速度的9倍左右，而精度却可以达到相当的程度。

我们再来看一个测试的比较，下表表示的是两种算法在两个数据集上面的错误率：

![](https://img-blog.csdn.net/20141016165057906)


最上面的数据代表训练的样本数，下面书错误率。可以看得出来RLS的方法还是很好的。



