
# 宽度学习（Broad Learning System） - 颹蕭蕭 - CSDN博客


2018年09月27日 19:58:01[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：4843标签：[宽度学习																](https://so.csdn.net/so/search/s.do?q=宽度学习&t=blog)[BL																](https://so.csdn.net/so/search/s.do?q=BL&t=blog)[BLS																](https://so.csdn.net/so/search/s.do?q=BLS&t=blog)[机器学习																](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)[陈俊龙																](https://so.csdn.net/so/search/s.do?q=陈俊龙&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=BLS&t=blog)个人分类：[机器学习																](https://blog.csdn.net/itnerd/article/category/8068411)
[
																								](https://so.csdn.net/so/search/s.do?q=BLS&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=BL&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=BL&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=宽度学习&t=blog)

### 一、宽度学习的前世今生
**宽度学习系统（BLS）**一词的提出源于澳门大学科技学院院长[陈俊龙](http://www.fst.umac.mo/en/staff/pchen.html)和其学生于2018年1月发表在*IEEE TRANSACTIONS ON NEURAL NETWORKS AND LEARNING SYSTEMS，VOL. 29, NO. 1*的一篇文章，题目叫[《Broad Learning System: An Effective and Efficient Incremental Learning System Without the Need for Deep Architecture 》](http://www.broadlearning.ai/broad-learning-system-an-effective-and-efficient-incremental-learning-system-without-the-need-for-deep-architecture/)。文章的主旨十分明显，就是提出了一种可以和深度学习媲美的宽度学习框架。
**为什么要提出宽度学习？**众所周知，深度学习中最让人头疼之处在于其数量庞大的待优化参数，通常需要耗费大量的时间和机器资源来进行优化。
宽度学习的前身实际上是已经被人们研究了很久的*随机向量函数连接网络**random vector functional-link neural network (RVFLNN)*，如图所示：
![在这里插入图片描述](https://img-blog.csdn.net/20180927191714677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
咋一看这网络结构没有什么奇特之处，其实也对，就是在单层前馈网络（SLFN）中增加了从输入层到输出层的直接连接。网络的第一层也叫**输入层**，第二层改名了，叫做**增强层**，第三层是**输出层**。具体来看，网络中有三种连接，分别是
[
](https://img-blog.csdn.net/20180927191714677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)（输入层 => 增强层）加权后有非线性变换
（增强层 => 输出层）只有线性变换
（输入层 => 输出层）只有线性变换
[
](https://img-blog.csdn.net/20180927191714677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)所以在RVFLNN中只有*增强层*是真正意义上的神经网络单元，因为只有它带了*激活函数*，网络的其他部分均是线性的。下面我们将这个网络结构扭一扭：
![在这里插入图片描述](https://img-blog.csdn.net/20180927193220429?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
当我们把增强层和输入层排成一行时，将它们视为一体，那网络就成了由**A**（输入层+增强层）到**Y**的线性变换了！线性变换对应的权重矩阵**W**就是*输入层加增强层*到*输出层*之间的线性连接！！
[
](https://img-blog.csdn.net/20180927193220429?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)这时你可能要问：那输入层到增强层之间的连接怎么处理/优化？我们的回答是：**不管它！！！**我们给这些连接随机初始化，固定不变！
[
](https://img-blog.csdn.net/20180927193220429?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)如果我们固定输入层到增强层之间的权重，那么对整个网络的训练就是求出 A 到 Y 之间的变换 W，而 W 的确定非常简单：
$$
W = A^{-1}Y
$$
输入**X**已知，就可以求出增强层**A**；训练数据的标签已知，就知道了**Y**。接下来的学习就是一步到位的事情了。
**为什么可以这样做？**
深度学习费了老劲把网络层数一加再加，就是为了增加模型的复杂度，能更好地逼近我们希望学习到的非线性函数，但是不是非线性层数越多越好呢？理论早就证明*单层前馈网络*（SLFN）已经可以作为*函数近似器*了，可见增加层数并不是必要的。RVFLNN也被证明可以用来逼近紧集上的任何连续函数，其非线性近似能力就体现在增强层的非线性激活函数上，只要增强层单元数量足够多，要多非线性有多非线性！
### 二、宽度学习系统（BLS）
![在这里插入图片描述](https://img-blog.csdn.net/20180927201045629?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
之前介绍的是RVFLNN，现在来看BLS，它对输入层做了一点改进，就是不直接用原始数据作为输入层，而是先对数据做了一些变换，相当于特征提取，将变化后的特征作为原RVFLNN的输入层，这样做的意义不是很大，只不过想告诉你：宽度学习可以利用别的模型提取到的特征来训练，即可以可别的机器学习算法组装。现在我们不把第一层叫做输入层，而是叫它**特征层**。
[
](https://img-blog.csdn.net/20180927201045629?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)当给定了特征**Z**，直接计算增强层**H**，将特征层和增强层合并成**A=[Z|H]**，竖线表示合并成一行。由于训练数据的标签**Y**已知，计算权重$W = A^{-1}Y$即可。实际计算时，使用*岭回归*来求解权值矩阵，即通过下面的优化问题来解W（其中$\sigma_1=\sigma_2=v=u=2$）：
![在这里插入图片描述](https://img-blog.csdn.net/20180927203148652?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解得
![在这里插入图片描述](https://img-blog.csdn.net/20180927203601182?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
以上过程是一步到位，即当数据固定，模型结构固定，可以直接找到最优的参数**W**。
[
](https://img-blog.csdn.net/20180927203601182?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)然而在大数据时代，数据固定是不可能的，数据会源源不断地来。模型固定也是不现实的，因为时不时需要调整数据的维数，比如增加新的特征。这样一来，就有了针对以上网络的增量学习算法。注意，宽度学习的核心在其增量学习算法，因为当数据量上亿时，相当于矩阵**Z**或**X**有上亿行，每次更新权重都对一个上一行的矩阵求*伪逆*是不现实的！
[
](https://img-blog.csdn.net/20180927203601182?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)增量学习的核心就是，利用上一次的计算结果，和新加入的数据，只需少量计算就能得进而得到更新的权重。
![在这里插入图片描述](https://img-blog.csdn.net/20180927210141349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
例如：当我们发现初始设计的模型拟合能力不够，需要增加增强节点数量来减小损失函数。这时，我们给矩阵 A 增加一列 a，表示新增的增强节点，得到[A|a]，这时要计算新的权值矩阵，就需要求$[A|a]^{-1}$，于是问题就转化成分块矩阵的广义逆问题，得到了$[A|a]^{-1}$，则更新的权重为$W_{new} = [A|a]^{-1}Y$，具体解形式如下，可以看到，$W_{n+1}$中用到了更新之前的权值矩阵$W_{n}$，因而有效地减少了更新权重的计算量。
![在这里插入图片描述](https://img-blog.csdn.net/20180927210201706?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第一次计算权值是用的是 岭回归 算法，因为有迭代过程，可能计算时间稍长。但是第二次、三次……计算时都只涉及矩阵的乘法，所以权值矩阵的更新是非常迅速。相比深度学习的反复训练，时常陷入局部最优无法自拔，宽度学习的优势非常明显。
[
            ](https://img-blog.csdn.net/20180927210201706?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[
  ](https://img-blog.csdn.net/20180927210141349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)