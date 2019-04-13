
# 【Learning Notes】基于 boosting 原理训练深层残差神经网络 - 丁丁的博客 - CSDN博客


2017年06月17日 00:28:00[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：1385


Huang et al.[Learning Deep ResNet Blocks Sequetially using Boosting Theory](https://arxiv.org/abs/1706.04964).
文章指出一种基于 boosting（提升）原理，逐层训练深度残差神经网络的方法，并对性能及泛化能力给出了理论上的证明。
# 1. 背景
## 1.1 Boosting
Boosting[1] 是一种训练[Ensemble](https://en.wikipedia.org/wiki/Ensemble_learning)模型的经典方法，其中一种具体实现[GBDT](https://en.wikipedia.org/wiki/Gradient_boosting)更是广泛应用在各类问题上。介绍boost的文章很多，这里不再赘述。简单而言，boosting 方法是通过特定的准则，逐个训练一系列弱分类，这些弱分类加权构成一个强分类器（图1）。
![这里写图片描述](https://img-blog.csdn.net/20170616232839520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170616232839520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图1 Boosting 方法原理图【****[src](http://www.cnblogs.com/LeftNotEasy/archive/2011/01/02/machine-learning-boosting-and-gradient-boosting.html)****】**
[

](https://img-blog.csdn.net/20170616232839520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 1.2 残差网络
[
](https://img-blog.csdn.net/20170616232839520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)残差网络[2]目前是图像分类等任务上最好的模型，也被应用到语音识别等领域。其中核心是 skip connect 或者说  shortcut（图2）。这种结构使梯度更易容向后传导，因此，使训练更深的网络变得可行。
[
](https://img-blog.csdn.net/20170616232839520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20170616233926380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170616233926380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图2. 残差网络基本block[2]**
[
](https://img-blog.csdn.net/20170616233926380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 在之前的
> [博文](http://blog.csdn.net/jackytintin/article/details/53242476)
> 中，我们知道，一些学者将残差网络视一种特殊的 Ensemble 模型[3,4]。论文作者之一是
> [Robert Schapire](http://rob.schapire.net/)
> （刚注意到已经加入微软研究院），AdaBoost的提出者（和 Yoav Freund一起）。Ensemble 的观点基本算是主流观点（之一）了。
[

](https://img-blog.csdn.net/20170616233926380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 2. 训练方法
[

](https://img-blog.csdn.net/20170616233926380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.1 框架
[
](https://img-blog.csdn.net/20170616233926380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20170628131557785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170628131557785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图3. BoostResNet 框架**
[
](https://img-blog.csdn.net/20170628131557785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)残差网络
[
](https://img-blog.csdn.net/20170628131557785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
$$
g_{t+1}(\mathbf{x}) = f(g_t(\mathbf{x}) ) + g_t(\mathbf{x})
$$
[
](https://img-blog.csdn.net/20170628131557785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)hypothesis module
o_t(\mathbf{x}) = softmax(\mathbf{W}^T_t \cdot g_t(\mathbf{x})) \in R^C
其中C为分类任务的类别数。
即这是一个线性分类器（Logistic Regression）。
weak module classifier

h_t(\mathbf{x}) = \alpha_{t+1} o_{t+1}(\mathbf{x}) - \alpha_t o_t(\mathbf{x}) \in R^C
其中\alpha为标量，也即h是相邻两层 hypothesis 的线性组合。第一层没有更低层，因此，可以视为有一个虚拟的低层，\alpha_0=0并且、o_0(x)=0。
将残差网络显示表示为 ensemble
令残差网络的最后输出为F(x)，并接合上述定义，显然有：
F(x) = o_T(x) = \frac{1}{\alpha_T} \alpha_T \cdot o(x) = \frac{1}{\alpha_T}  \sum_{t=0}^T (\alpha_{t} \cdot o_t(x) - \alpha_{t-1} \cdot o_{t-1}(x)) = \frac{1}{\alpha_T}  \sum_{t=0}^T h_t(x)
这里用到了裂项求和的技巧（[telescoping sum](http://mathworld.wolfram.com/TelescopingSum.html)），因此作者称提出的算法为 telescoping sum boosting.
我们只需要逐级（residual block）训练残差网络，效果上便等同于训练了一系列弱分类的 enemble。其中，除了训练残差网络的权值外，还要训练一些辅助的参数——各层的\alpha及W（训练完成后即可丢弃）。
2.2  Telescoping Sum Boosting（裂项求和提升）
文章正文以二分类问题为例展开，我们更关心多分类问题，相关算法在附录部分。文章给出的伪代码说明相当清楚，直接复制如下：
![这里写图片描述](https://img-blog.csdn.net/20170616223757438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中，\gamma_t是一个标量；C_t是一个 m 乘 C （样本数乘类别数）的矩阵，C_t(i, j)表示其中第i行第j列的元素。
需要特别说明的是，s_t(x, l)表示s_t(x)的第l个元素(h(x, l)、o_t(x,l)同理，此处符号用的略随意:-)；而s_t(x) = \sum_{\tau=1}^t h_\tau(x) =  \alpha_t \cdot o_t(x)。
![这里写图片描述](https://img-blog.csdn.net/20170616224013988?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
与算法3中类似，f(g(x_i), l)表示f(g(x_i))的第l个元素，g(x_i, y_i)表示g(x_i)的第i个元素。
显然 Algorithm 4 给的最小化问题可以用 SGD 优化，也可以数值的方法求解（[1] 4.3 节）。
3. 理论
理论分部没有详细看。大体上，作者证明了 BoostResNet 保留了 boost 算法是优点：1）误差随网络深度（即弱分类器数量）指数减小；2）抗过拟合性，模型复杂度承网络深度线性增长。详细可参见论文。
4. 讨论
BoostResNet 最大的特点是逐层训练，这样有一系列好处：
减少内存占用（Memory Efficient），使得训练大型的深层网络成为可能。（目前我们也只能在 CIFAR 上训练千层的残差网络，过过干瘾）
减少计算量（Computationally Efficient），每一级都只训练一个浅层模型。
因为只需要训练浅层模型，在优化方法上可以有更多的选择（非 SGD 方法）。
另外，网络层数可以依据训练情况动态的确定。
一些疑问
文章应该和逐层训练的残差网络（固定或不固定前面各层的权值）进行比较多，而不是仅仅比较所谓的 e2eResNet（端到端残差网络）。
作者这 1.1 节最后也提到，训练框架不限于 ResNet，甚至不限于神经网络。不知道用来训练普通深度模型效果会怎样，必争 layer-wise pretraining 现在已经显得有点过时了。
References
Schapire & Freund. Boosting: Foundations and Algorithms. MIT.
He et al.[Deep Residual Learning for Image Recognition](https://arxiv.org/abs/1512.03385).
Veit et al.[Residual Networks Behave Like Ensembles of Relatively Shallow Networks](https://arxiv.org/abs/1605.06431).
Xie et al.[Aggregated Residual Transformations for Deep Neural Networks](https://arxiv.org/abs/1611.05431).

