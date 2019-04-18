# 《deep learning》学习笔记（5）——机器学习基础 - Soul Joy Hub - CSDN博客

2017年08月15日 22:31:02[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1267
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/77202136](http://blog.csdn.net/u011239443/article/details/77202136)

# 5.1 学习算法

> 
学习算法：对于某类任务 T 和性能度量P，一个计算机程序被认为可以从经验 E 中学习是指，通过经验 E 改进后，它在任务 T 上由性能度量 P 衡量的性能有所提升。

## 5.1.1 任务 T

> 
样本：我们从某些希望机器学习系统处理的对象或事件中收集到的已经量化的特征（feature）的集合。

常见的机器学习任务: 

  - 分类 

  -   输入缺失分类：当输入向量的每个度量不被保证的时候，分类问题将会变得更有挑战性。为了解决分类任务，学习算法只需要定义一个从输入向量映射到输出类别的函数。当一些输入可能丢失时，学习算法必须学习一组函数，而不是单个分类函数。 

  - 回归 

  -  转录：这类任务中，机器学习系统观测一些相对非结构化表示的数据，并转录信息为离散的文本形式。如语音识别。 

  - 机器翻译：在机器翻译任务中，输入是一种语言的符号序列，计算机程序必须将其转化成另一种语言的符号序列。 

  - 结构化输出：结构化输出任务的输出是向量或者其他包含多个值的数据结构，并且构成输出的这些不同元素间具有重要关系。例如语法分析。 

  -  异常检测：在这类任务中，计算机程序在一组事件或对象中筛选，并标记不正常或非典型的个体。如信用卡欺诈检测。 

  - 合成和采样：在这类任务中，机器学习程序生成一些和训练数据相似的新样本。通过机器学习，合成和采样可能在媒体应用中非常有用，可以避免艺术家大量昂贵或者乏味费时的手动工作。 

  - 缺失值填补：在这类任务中，机器学习算法给定一个新样本 x ∈ R n ，x 中某些元素 x i 缺失。算法必须填补这些缺失值。 

  - 去噪：在这类任务中，机器学习算法的输入是，干净样本 x ∈ R n 经过未知损坏过程后得到的损坏样本 ˜ x ∈ R n 。算法根据损坏后的样本 ˜ x 预测干净的样本x，或者更一般地预测条件概率分布 p(x | ˜ x)。 

  -密度估计或概率质量函数估计：在密度估计问题中，机器学习算法学习函数p model : R n → R，其中 p model (x) 可以解释成样本采样空间的概率密度函数（如果 x 是连续的）或者概率质量函数（如果 x 是离散的）。

## 5.1.2 性能度量 P

> 
准确率：该模型输出正确结果的样本比率。 

  错误率：该模型输出错误结果的样本比率。

## 5.1.3 经验 E

> 
数据集：很多样本组成的集合
- 无监督学习算法（unsupervised learning algorithm）训练含有很多特征的数据集，然后学习出这个数据集上有用的结构性质。
- 监督学习算法（supervised learning algorithm）训练含有很多特征的数据集，不过数据集中的样本都有一个标签（label）或目标（target）。
- 无监督学习涉及到观察随机向量 x 的好几个样本，试图显式或隐式 

  地学习出概率分布 p(x)，或者是该分布一些有意思的性质；
- 监督学习包含观察随机向量 x 及其相关联的值或向量 y，然后从 x 预测 y，通常是估计 p(y | x)。
- 监督学习（supervised learning）源自这样一个视角，教员或者老师提供目标 y 给机器学习系统，指导其应该做什么。
- 在无监督学习中，没有教员或者老师，算法必须学会在没有指导的情况下理解数据。

设计矩阵的每一行包含一个不同的样本。每一列对应不同的特征。例如，Iris 数据集包含 150 个样本，每个样本有 4 个特征。这意味着我们可以将该数据集表示为设计矩阵 X ∈ R 150×4 ，其中 X i,1 表示第 i 个植物的萼片长度，X i,2 表示第 i 个植物的萼片宽度等等。

## 5.1.4 示例：线性回归

> 
线性回归： 

  线性回归解决回归问题。换言之，我们的目标是建立一个系统，将向量 x ∈ R n 作为输入，预测标量 y ∈ R 作为输出。线性回归的输出是其输入的线性函数。令 ˆ y 表示模型预测 y 应该取的值。我们定义输出为 
![](http://upload-images.jianshu.io/upload_images/1621805-6f8c2c3606e2fb10.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

  其中 w ∈ R n 是参数（parameter）向量。 

  我们可以将 w 看作是一组决定每个特征如何影响预测的权重（weight）。

均方误差： 
![](http://upload-images.jianshu.io/upload_images/1621805-fc2437c571cb3c58.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-edcaff1a7b47b9e6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-09be9f3ef6859136.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

线性回归（linear regression）通常用来指稍微复杂一些，附加额外参数（截距项 b）的模型。在这个模型中， 
![](http://upload-images.jianshu.io/upload_images/1621805-d47bcee54fec7389.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

  截距项 b 通常被称为仿射变换的偏置（bias）参数。

# 5.2 容量、过拟合和欠拟合

> 
泛化（generalization）：在先前未观测到的输入上表现良好的能力。

训练误差: 
![](http://upload-images.jianshu.io/upload_images/1621805-49579901a5709422.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

  测试误差： 
![](http://upload-images.jianshu.io/upload_images/1621805-85e7ba4082a34a93.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

训练集和测试集数据通过数据集上被称为数据生成过程（data generating process）的概率分布生成。通常，我们会做一系列被统称为独立同分布假设（i.i.d.assumption）的假设。该假设是说，每个数据集中的样本都是彼此相互独立的（independent），并且训练集和测试集是同分布的（identically distributed），采样自相同的分布。
- 欠拟合是指模型不能在训练集上获得足够低的误差。
- 过拟合是指训练误差和和测试误差之间的差距太大。
- 模型的容量是指其拟合各种函数的能力。容量低的模型可能很难拟合训练集。容量高的模型可能会过拟合，因为记住了不适用于测试集的训练集性质。
- 选择假设空间（hypothesis space）：一种控制训练算法容量的方法是，即学习算法可以选择为解决方案的函数集。

![](http://upload-images.jianshu.io/upload_images/1621805-b3c857dd9ede2713.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
奥卡姆剃刀（Occam’srazor）（c. 1287-1387）：在同样能够解释已知观测现象的假设中，我们应该挑选 ‘‘最简单’’ 的那一个。

VC维：该分类器能够分类的训练样本的最大数目。假设存在 m 个 

  不同 x 点的训练集，分类器可以任意地标记该 m 个不同的 x 点，VC维被定义为 m的最大可能值。

![](http://upload-images.jianshu.io/upload_images/1621805-a5a2f334fce2fa76.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
非参数模型：这种算法的一个示例是最近邻回归（nearest neighbor regression）。最近邻回归模型存储了训练集中所有的X 和 y。当需要为测试点 x 分类时，模型会查询训练集中离该点最近的点，并返回相关的回归目标。换言之，$ y^{\^} = y_i$ 其中 $i = argmin∥X_i,: − x∥^2_2$ 。

贝叶斯误差（Bayes error）：从预先知道的真实分布 p(x,y) 预测而出现的误差。

![](http://upload-images.jianshu.io/upload_images/1621805-a91312e262d3b98f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-2c694ec4f1a444e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 5.2.1 没有免费午餐定理

> 
没有免费午餐定理（no free lunch theorem）：在所有可能的数据生成分布上平均之后，每一个分类算法在未事先观测的点上都有相同的错误率。换言之，在某种意义上，没有一个机器学习算法总是比其他的要好。我们能够设想的最先进的算法和简单地将所有点归为同一类的简单算法有着相同的平均性能（在所有可能的任务上）。

## 5.2.2 正则化

![](http://upload-images.jianshu.io/upload_images/1621805-054f1615bee04670.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

正则化项是$ Ω(w) = w^⊤ w$

![](http://upload-images.jianshu.io/upload_images/1621805-ccd7ae0dc6f6d28f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
正则化：我们修改学习算法，使其降低泛化误差而非训练误差。在我们权重衰减的示例中，通过在最小化的目标中额外增加一项，我们明确地表示了偏好权重较小的线性函数。有很多其他方法隐式或显式地表示对不同解的偏好。

# 5.3 超参数和验证集

> 
超参数：大多数机器学习算法都有超参数，可以设置来控制算法行为。在图5.2所示的多项式回归示例中，有一个超参数：多项式的次数，作为容量超参数。控制权重衰减程度的 λ 是另一个超参数。

验证集（validation set）：用于挑选超参数的数据子集。

## 5.3.1 交叉验证

![](http://upload-images.jianshu.io/upload_images/1621805-8beff504fb056835.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 5.4 估计、偏差和方差

## 5.4.1 点估计

> 
点估计： 

  令 {x (1) ,…,x (m) } 是 m 个独立同分布（i.i.d.）的数据点。点估计（point estimator）或统计量（statistics）是这些数据的任意函数： 
![](http://upload-images.jianshu.io/upload_images/1621805-3628f2ce0a65c3d3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

函数估计：有时我们会关注函数估计（或函数近似）。这时我们试图从输入向量 x 预 

  测变量 y。我们假设有一个函数 f(x) 表示 y 和 x 之间的近似关系。

## 5.4.2 偏差

> 
偏差: 
![](http://upload-images.jianshu.io/upload_images/1621805-c2b3c89564b8fabb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-74f2650a0b86cb7d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-a127ed9fed60e972.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

关于高斯分布方差估计，建议参阅：[http://blog.csdn.net/u011239443/article/details/77198730](http://blog.csdn.net/u011239443/article/details/77198730)

## 5.4.3 方差和标准差

> 
估计量的方差（variance）就是一个方差 
![](http://upload-images.jianshu.io/upload_images/1621805-a6ef3fd4cb822f42.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

  其中随机变量是训练集。另外，方差的平方根被称为标准差（standard error），记作SE( ˆθ)。

![](http://upload-images.jianshu.io/upload_images/1621805-0bcc1203b2a9cc2c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-293c6deb1b455cb7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 5.4.4 权衡偏差和方差以最小化均方误差

> 
均方差：MSE度量着估计和真实参数 θ 之间平方误差的总体期望偏差。MSE估计包含了偏差和方差。 
![](http://upload-images.jianshu.io/upload_images/1621805-4596db1af3986d9a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-3bb341c1d09465c2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 5.4.5 一致性

> 
一致性（consistency）：  
![](http://upload-images.jianshu.io/upload_images/1621805-5ba49d7810df12aa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

  对于任意的 ϵ > 0，当 m → ∞ 时，有$ P(| θ^{\^}_m − θ| >ϵ) → 0$。

# 5.5 最大似然估计

![](http://upload-images.jianshu.io/upload_images/1621805-2bed9cb812d74841.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 5.5.1 条件对数似然和均方误差

![](http://upload-images.jianshu.io/upload_images/1621805-067707f57e6f4546.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
示例：线性回归作为最大似然 : 

  了得到我们之前推导出的相同的线性回归算法，我们定义$p(y | x) = N(y; ˆ y(x;w),σ^2 )$。函数 ˆ y(x;w) 预测高斯的均值。在这个例子中，我们假设方差是用户固定的某个常量 $σ^2$ 。这种函数形式 p(y | x) 会使得最大似然估计得出和之前相同的学习算法。由于假设样本是独立同分布的，条件对数似然（式(5.63)） 

  如下: 
![](http://upload-images.jianshu.io/upload_images/1621805-7b22617e3bd471a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

  我们立刻可以看出最大化关于 w 的对数似然和最小化均方误差会得到相同的参数估计 w。但是对于相同的最优 w，这两个准则有着不同的值。这验证了MSE可以用于最大似然估计。正如我们将看到的，最大似然估计有几个理想的性质。

## 5.5.2 最大似然的性质

> 
统计效率通常用于有参情况（parametric case）的研究中（例如线性回归）。有参情况中我们的目标是估计参数值（假设有可能确定真实参数），而不是函数值。一种度量我们和真实参数相差多少的方法是计算均方误差的期望，即计算 m 个从数据生成分布中出来的训练样本上的估计参数和真实参数之间差值的平方。有参均方误差估计随着 m 的增加而减少，当 m 较大时，Cramér-Rao 下界 (Rao, 1945; Cramér,1946) 表明不存在均方误差低于最大似然估计的一致估计。

最大似然估计优势：1、当样本数目 m → ∞ 时，就收敛率而言是最好的渐近估计。2、良好的统计效率

# 5.6 贝叶斯统计

> 
先验概率分布（prior probability distribution）:在观察到数据前，θ 的已知知识的表示。 
![](http://upload-images.jianshu.io/upload_images/1621805-24c5d7f145a38549.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 5.6.1 最大后验 (MAP) 估计

> 
最大后验 (MAP) 估计：让先验影响点估计的选择来利用贝叶斯方法的优点，而不是简单地回到最大似然估计。 

  MAP估计选择后验概率最大的点（或在 θ 是连续值的更常见情况下，概率密度最大的点）： 
![](http://upload-images.jianshu.io/upload_images/1621805-3fc555e29a5746af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 5.7 监督学习算法

## 5.7.1 概率监督学习

> 
逻辑回归： 

  使用 logistic sigmoid 函数将线性函数的输出压缩进区间 (0,1)。该值可以解释为概率： 
![](http://upload-images.jianshu.io/upload_images/1621805-3dd1cc34449976da.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 5.7.2 支持向量机

> 
支持向量机（support vector machine, SVM）,类似于逻辑回归，这个模型也是基于线性函数 w ⊤ x + b 的。不同于逻辑回归的是，支持向量机不输出概率，只输出类别。当 w ⊤ x + b 为正时，支持向量机预测属于正类。类似地，当 w ⊤ x + b 为负时，支持向量机预测属于负类。

![](http://upload-images.jianshu.io/upload_images/1621805-13a4846e611daaf5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-70f46a619ba46976.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
核技巧十分强大有两个原因： 

  1.它使我们能够使用保证有效收敛的凸优化技术来学习非线性模型（关于 x 的函数）。这是可能的，因为我们可以认为 ϕ 是固定的，仅优化 α，即优化算法可以将决策函数视为不同空间中的线性函数。 

  2.核函数 k 的实现方法通常有比直接构建 ϕ(x) 再算点积高效很多。

![](http://upload-images.jianshu.io/upload_images/1621805-bf0e556a4908b85b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-b1ae66f2d745ce12.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
缺点：计算决策函数的成本关于训练样本的数目是线性的。当数据集很大时，核机器的计算量也会很大。

支持向量（support vector）：判断新样本的类别仅需要计算非零 $α_i$ 对应的训练样本的核函数。这些训练样本被称为支持向量。

## 5.7.3 其他简单的监督学习算法

> 
k-最近邻的高容量使其在训练样本数目大时能够获取较高的精度。然而，它的计算成本很高，另外在训练集较小时泛化能力很差。k-最近邻的一个弱点是它不能学习出哪一个特征比其他更具识别力。

决策树： 
![](http://upload-images.jianshu.io/upload_images/1621805-8e6f41ddf93db6e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-fabbfbf8b1274370.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

  由于决策树通常使用坐标轴相关的拆分，并且每个子节点关联到常数输出，因此有时解决一些对于逻辑回归很简单的问题很费力。

# 5.8 无监督学习算法

## 5.8.1 主成分分析

![](http://upload-images.jianshu.io/upload_images/1621805-9839f39c373e8a14.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-ef4ef0f17000d16f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-023577c8b1eec142.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-c314ea4394035427.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 5.8.2 k-均值聚类

![](http://upload-images.jianshu.io/upload_images/1621805-093e81e7ef5230a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
聚类的缺点： 

  1 没有单一的标准去度量聚类的数据在真实世界中效果如何。 

  2 我们可能希望找到和一个特征相关的聚类，但是得到了一个和任务无关的，同样是合理的不同聚类。

# 5.9 随机梯度下降

![](http://upload-images.jianshu.io/upload_images/1621805-bc6f4dbad7b9cd91.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

其中，ϵ 是学习率。

# 5.10 构建机器学习算法

> 
学习算法都可以被描述为一个相当简单的配方：特定的数据集、代价函数、优化过程和模型。我们可以将不同算法视为出于相同原因解决相关问题的一类方法，而不是一长串各个不同的算法。

# 5.11 促使深度学习发展的挑战

## 5.11.1 维数灾难

> 
维数灾难：当数据的维数很高时，很多机器学习问题变得相当困难。 
![](http://upload-images.jianshu.io/upload_images/1621805-efa360156a944d82.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 5.11.2 局部不变性和平滑正则化

> 
局部不变性先验（local constancy prior）：这个先验表明我们学习的函数不应在小区域内发生很大的变化。 

  局部核（local kernel）：核函数 k(u,v) 在 u = v 时很大，当 u 和 v 距离拉大时而减小。

![](http://upload-images.jianshu.io/upload_images/1621805-e1f27fcf062320c4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
深度学习的核心思想是假设数据由因素 

  或特征组合产生，这些因素或特征可能来自一个层次结构的多个层级。

## 5.11.3 流形学习

> 
流形（manifold）：连接在一起的区域。数学上，它是指一组点，且每个点都有其邻域。机器学习倾向于更松散地定义一组点，只需要考虑少数嵌入在高维空间中的自由度或维数就能很好地近似。

![](http://upload-images.jianshu.io/upload_images/1621805-a418049b04fdf199.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
流形学习（manifold learning）算法：该假设认为 $R^n$ 中大部分区域都是无效的输入，有意义的输入只分布在包含少量数据点的子集构成的一组流形中，而学习函数的输出中，有意义的变化都沿着流形的方向或仅发生在我们切换到另一流形时。

支持流形假设（manifold hypothesis）的观察： 

  1.现实生活中的图像、文本、声音的概率分布都是高度集中的。 
![](http://upload-images.jianshu.io/upload_images/1621805-64407cdcdaef6faa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

  2.我们至少能够非正式地想象这些邻域和变换。在图像中，我们当然会认为有很多可能的变换仍然允许我们描绘出图片空间的流形：我们可以逐渐变暗或变亮光泽、逐步移动或旋转图中对象、逐渐改变对象表面的颜色等等。 
![](http://upload-images.jianshu.io/upload_images/1621805-f070af92582e3c62.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-c94587716649cc4e?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

