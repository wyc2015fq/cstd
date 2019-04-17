# coursera-斯坦福-机器学习-吴恩达-第9周笔记（上）-异常检测 - 博客堂 - CSDN博客





2017年12月20日 22:43:42[最小森林](https://me.csdn.net/u012052268)阅读数：3219
所属专栏：[斯坦福-吴恩达《机器学习》之路](https://blog.csdn.net/column/details/18191.html)









![image](http://images0.cnblogs.com/blog2015/437988/201508/160948453488331.jpg)

# coursera-斯坦福-机器学习-吴恩达-第9周笔记（上）-异常检测



- [coursera-斯坦福-机器学习-吴恩达-第9周笔记上-异常检测](#coursera-斯坦福-机器学习-吴恩达-第9周笔记上-异常检测)- [1异常检测](#1异常检测)- [1引入](#11引入)
- [2高斯正态分布](#12高斯正态分布)
- [3异常检测算法](#13异常检测算法)

- [2建立一个异常检测系统](#2建立一个异常检测系统)- [1评价一个异常检测系统](#21评价一个异常检测系统)
- [2异常检测vs监督学习](#22异常检测vs监督学习)
- [3选择使用哪些features](#23选择使用哪些features)

- [3多元高斯分布](#3多元高斯分布)- [1多元高斯分布数学](#31多元高斯分布数学)
- [2应用多元高斯分布进行异常检测](#32应用多元高斯分布进行异常检测)

- 
- [复习](#4-复习)





## 1异常检测

异常检测(Anomaly detection)问题 是机器学习算法 的一个常见应用， 这种算法的一个有趣之处在于 ：它虽然主要用于 非监督学习问题 ，但从某些角度看 它又类似于一些监督学习问题 。

### 1.1引入

那么 什么是异常检测呢？ 

为了解释这个概念 ，让我举一个例子吧： 假想你是一个 飞机引擎制造商， 当你生产的飞机引擎 从生产线上流出时 你需要进行 QA (质量控制测试)， 而作为这个测试的一部分 你测量了飞机引擎的一些特征变量 ，比如 你可能测量了 引擎运转时产生的热量， 或者引擎的振动等等 。采集这些特征变量 这样一来 你就有了一个数据集 ，从x(1)到x(m)， 如果你生产了m个引擎的话 也许你会将这些数据绘制成图表。

后来有一天 ，你有一个新的飞机引擎 从生产线上流出，而你的新飞机引擎 有特征变量x-test 。所谓的异常检测问题就是 我们希望知道， 这个新的飞机引擎是否有某种异常。

感性来理解一下，如图： 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-20/54867750.jpg)
这些样本点 有很大的概率值 落在 在中心区域。 

而稍微远离中心区域的点概率会小一些 ， 

更远的地方的点 它们的概率将更小， 这外面的点 和这外面的点 将成为异常点 。
而这些圈是可以用一个概率模型P(x)来衡量的，我们的异常检测算法就是找到这样一个概率模型画出这个圈。

异常检测算法有很多用途，比如： 

1. 欺诈检测：把用户的使用习惯设为特征，若很反常有可能为欺诈。 

2. 制造业：产品的质量控制（QA）。 

3. 数据中心的计算监控：监控cpu、内存等的使用情况是否又反常。
### 1.2高斯（正态）分布

异常检测的数学基础：高斯（正态）分布。是数理统计中的知识，话说线性代数与数理统计这两门课真的是有用啊。

所谓样本x服从高斯分布（x~N），就是x出现的概率（或者说发生x事件的概率）满足这样一个公式：$p(x) = \frac{1}{\sqrt{2\pi}\sigma}\exp\left(-\frac{(x-\mu)^2}{2\sigma^2}\right)$

所以这是一个概率公式。他的图像如下：以均值为中心，方差为宽度。

![](http://oqy7bjehk.bkt.clouddn.com/17-12-20/16635067.jpg)

其中均值与方差的计算方法如下：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-20/83336523.jpg)

有了均值与方差的计算方法，就有了**参数估计**这一名词。就是：给你了一些样本值，你可以求出他们的均值与方差，然后用这两个参数估计总体样本的分布。

第二个比较重要的数学知识就是独立分布的概率，等于概率的乘积。 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-20/74782642.jpg)
### 1.3异常检测算法❤❤❤

这一节给出了异常检测的步骤，对于给出的样本$\{x^{(1)},..., x^{(m)}\}$：
- 选出一些重要的特征，比如：$x_j^{(i)}$表示第i个样本的第j个特征，比如发动机温度。
- 计算各个特征的均值与方差：$\displaystyle \mu_j = \frac{1}{m}\sum_{i=1}^m x_j^{(i)},\ \sigma_j^2 = \frac{1}{m}\sum_{i=1}^m(x_j^{(i)} - \mu_j)^2$ 。 这样也就算出了每种特征的分布，也就算出了每种特征出现某值的概率。
- 给出一个新的样本，计算他出现的概率P(x)。P(x)等于各项特征出现概率的乘积。![image](http://oqy7bjehk.bkt.clouddn.com/17-12-20/92514562.jpg)

下面通过图像直观感受：

![image](https://img-blog.csdn.net/20150401203255069?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 左上角为样本点，有X1和X2两个特征。
- 计算分别计算X1和X2的均值和方差。图像如右上角，综合两者的图像为三维左下角，高度为概率。
- 给定2个样本Xtest，计算出现的概率，大于阈值为正常，小于这么小的阈值为异常。

总结：异常检验的数学内涵是数理统的一条法则：

> 
小概率事件一般不会发生。


翻译成通俗解释就是：不可能发生的事情却发生了，必有阴谋！所以判断其为异常值，事后专门对其进行检查。

## 2建立一个异常检测系统

在上一段视频中 我们推导了异常检测算法， 在这段视频中， 我想介绍一下 如何开发一个 关于异常检测的应用 ，来解决一个实际问题 。

### 2.1评价一个异常检测系统

评价异常检测系统最常用的方法就是使用 cross validation（与监督学习一样）：将训练数据分为三部分：Training set is unlabled, cross validation & test set is labled.如图： 
![image](https://img-blog.csdn.net/20150402194442343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于异常检测问题，要检测出的是anomalous的，所以anomalous对应y = 1，我们举个例子，对与飞机发动机故障检测，将10000个好数据分为三部分，20个故障数据分成2部分，如图： 
![image](https://img-blog.csdn.net/20150402194539592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中第二种方式，把CV与test数据弄的一样，吴恩达老师不鼓励这样。

下面总结一下步骤： 
![image](https://img-blog.csdn.net/20150402194514994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)即： 

1. 对训练数据进行寻找估计模型。P(X) 

2. 使用交叉验证和test数据集预测0和1. 

3. 使用召回率和置信度计算F1值评价算法。（为啥不用精度？因为：这是个不均衡数据skewed，前面讲过不能使用classification accuracy。） 

4. 设置epsilon，在交叉验证集上评估算法，然后当我们选择了特征集时，找到epsilon的值，对测试集上的算法进行最终评估。
### 2.2异常检测vs.监督学习

在上一段视频中 ，我们谈到 如何评价一个 异常检测算法 ，我们先是用了一些 带标签的数据 ，以及一些我们知道是异常 或者正常的样本 用 y=1 或 y=0 来表示 。

这就引出了这样一个问题： 我们有了这些带标签的数据 我们有了一些样本 其中一些我们知道是异常的 另外一些是正常的 那我们为什么我们不 直接用监督学习的方法呢？

![image](https://img-blog.csdn.net/20150402202618753?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

ppt已经写得很清楚了，在总结一下：关键的区别就是 ：在异常检测算法中 我们只有一小撮 正样本 因此学习算法不可能 从这些正样本中学出太多东西 。|异常检测|监督学习|
|----|----|
|负向数据很多，可以精确的计算出参数估计P(x)|需要两种数据都多，才能很好的计算出模型。|
|好的数据很集中，但故障可能有很多种，只要将其找出即可|需要大量正样本，才能学习出相关特征，表示出故障的特征进行分类。|

下一页ppt给出了两种算法的一般使用场景： 
![image](https://img-blog.csdn.net/20150402202711743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下面一个练习题，可以看一下。 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-20/73160751.jpg)
### 2.3选择使用哪些features

在此之前 我们讨论了如何 评估一个异常检测算法 。事实上 当你应用异常检测时 对它的效率 影响最大的 因素之一是 你使用什么特征变量 。两种方法： 

1. 参数修改

在我们的异常检测算法中我们做的事情之一就是使用这种正态(高斯)分布来对特征向量建模，但是有些数据并不太符合高斯分布，虽然算法也常常可以正常运行，但效果不好，可以用这种数学方法：

进行数据的不同转换，以使其看起来更加高斯。比如：用 x1 的对数 log(x1) 来替换掉 x1，或者平方根来取代 x3。

![image](https://img-blog.csdn.net/20150402205200516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 误差分析

像之前讲监督学习的误差分析一样，  我们先完整地训练出 一个学习算法， 然后在一组交叉验证集上运行算法 ；然后找出那些预测出错的样本， 然后再看看 我们能否找到一些其他的特征变量 来帮助学习算法， 让它在那些交叉验证时 判断出错的样本中表现更好 。

比如下图：绿色x代表anomaly example， 只有一个feature x1时会区分错误，加一个feature x2时就可以正确区分。

![image](https://img-blog.csdn.net/20150402225750465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 3多元高斯分布

异常检测算法的 一种可能的延伸 —— 多元高斯分布 (multivariate Gaussian distribution) 。它有一些劣势（计算量大），也有一些优势（ 它能捕捉到一些之前的算法检测不出来的异常）。

### 3.1多元高斯分布数学

我们先来看看一个例子： 假设我们的没有标签的数据看起来像这张图一样。 ，我要使用数据中心的监控机的例子 ，我的两个特征变量 x1 是 CPU 的负载和 x2 可能是内存使用量 。

![image](https://img-blog.csdn.net/20150402232942417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

那么普通的高斯分布建模就是粉红色的圆圈，多元高斯分布建模就是蓝色的“斜”椭圆。（其知识为多元正态分布的相关性，其实普通正态分布就是多元正态分布的特例——特征独立不相关。）

其概率表达式为：![image](https://img-blog.csdn.net/20150621144912308)

推导过程如下：
- 
设(X1,X2)是二元正态变量，其密度函数为： ![image](https://img-blog.csdn.net/20150621143111727)

- 
即 (X1,X2)~$N(\mu_1, \mu_2,  \sigma_1^2,\sigma_2^2,\rho)$

其中：ρ是相关系数

- 
令：
- $x = (x_1, x_2)^T$
- $\mu = (\mu_1, \mu_2)^T$
- C=$\begin{pmatrix}  c_{11} & c_{12}  \  c_{21} & c_{22}  \  \end{pmatrix}$=$\begin{pmatrix}        \sigma_1^2 & \rho\sigma_1\sigma_2  \\        \rho\sigma_1\sigma_2&\sigma_2^2  \\        \end{pmatrix}$

- 
于是推出： $f(x_1, x_2) = \frac1{(2\pi)^\frac n2(|C|)^{\frac12}} e^{-\frac12(x-\mu)^TC^(-1)(x-\mu)}$

即：


![image](https://img-blog.csdn.net/20150621144912308)

其中： 

- μ 相当于每个正态分布的对称轴，是一个一维向量 

- Σ是协方差矩阵
下面通过图像来看一下多元正态分布的特点： 

1. 协方差相同，说明特征独立，与普通高斯分布相同： 
![image](https://img-blog.csdn.net/20150402232935587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)- 协方差改变，特征相关，变成椭圆：

![image](https://img-blog.csdn.net/20150402233029451?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 正相关（右上-左下）

![image](https://img-blog.csdn.net/20150402233031669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 负相关（左上-右下） 
![image](https://img-blog.csdn.net/20150402233055444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 3.2应用多元高斯分布进行异常检测

之前看到了一些例子， 通过改变参数 µ 和 Σ 来给不同的概率分布建模。 在这节视频中 ，我们来使用那些想法 ，用它们来开发另一种异常检测算法 。

步骤如下：

## ![image](https://img-blog.csdn.net/20150403000400232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

传统高斯分布与多元高斯分布的关系：当多元高斯分布的协方差为对角矩阵的时候就是不同高斯分布。 
![image](https://img-blog.csdn.net/20150403000413242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
具体区别对比： 
![image](https://img-blog.csdn.net/20150403000344853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)- 多元高斯模型有很多参数，所以这个协方差矩阵σ是一个n乘n的矩阵，大约有n个平方参数，因为它是一个对称矩阵，它实际上更接近于2个参数的n平方，但是这是很多参数，所以你需要确保你有一个相当大的m值，确保你有足够的数据来适应所有这些参数。
- m大于或等于10 n将是一个合理的经验法则，以确保您可以相当好地估计这个协方差矩阵西格玛。
- 在你有非常大的训练集或m非常大而n不是太大的问题中，那么多元高斯模型是值得考虑的，并且可以更好地工作，并且可以节省你不必花费时间手动创建额外的功能，以防异常结果被特征值的异常组合所捕捉。
- 协方差矩阵西格玛不可逆，他们通常是这种情况2，一个如果它不能满足这个米大于n条件; secondcase是如果你有多余features.if你有2个功能是相同的。如果你的x1只是等于x2。或者如果你有多余的功能，比如你的功能X3等同于功能X4，再加上功能X5，那么X3不包含任何额外的信息。

## 4 复习
- 
For which of the following problems would anomaly detection be a suitable algorithm? 

答案AC
- 
[ ] Given a dataset of credit card transactions, identify unusual transactions to flag them as possibly fraudulent.

- 
[ ] Given data from credit card transactions, classify each transaction according to type of purchase (for example: food, transportation, clothing).

- 
[ ] From a large set of primary care patient records, identify individuals who might have unusual health conditions.

- 
[ ] Given an image of a face, determine whether or not it is the face of a particular famous individual.


- 
Suppose you have trained an anomaly detection system that flags anomalies when p(x)is less than ε, and you find on the cross-validation set that it has too many false positives (flagging too many things as anomalies). What should you do? 

答案B
- 
[ ] Increase ε

- 
[ ] Decrease ε


- 
Suppose you are developing an anomaly detection system to catch manufacturing defects in airplane engines.  

You model uses $p(x) = \prod_{j=1}^n p(x_j ; \mu_j, \sigma^2_j)$  You have two features x1 = vibration intensity, and x2 = heat generated. Both x1 and x2take on values between 0 and 1 (and are strictly greater than 0), and for most “normal” engines you expect that x1≈x2. One of the suspected anomalies is that a flawed engine may vibrate very intensely even without generating much heat (large x1, small x2), even though the particular values of x1 and x2 may not fall outside their typical ranges of values. What additional feature x3 should you create to capture these types of anomalies: 

答案B- x3=x21×x22
- x3=x1x2
- x3=(x1+x2)2
- x3=x1×x22
- 
Which of the following are true? Check all that apply. 

答案AD
- 
[ ] If you do not have any labeled data (or if all your data has label y=0), then is is still possible to learn p(x), but it may be harder to evaluate the system or choose a good value of ϵ.

- 
[ ] If you are developing an anomaly detection system, there is no way to make use of labeled data to improve your system.

- 
[ ] If you have a large labeled training set with many positive examples and many negative examples, the anomaly detection algorithm will likely perform just as well as a supervised learning algorithm such as an SVM.

- 
[ ] When choosing features for an anomaly detection system, it is a good idea to look for features that take on unusually large or small values for (mainly the) anomalous examples.


- 
You have a 1-D dataset {x(1),…,x(m)} and you want to detect outliers in the dataset. You first plot the dataset and it looks like this: 

Suppose you fit the gaussian distribution parameters μ1 and σ21 to this dataset. Which of the following values for μ1 and σ21 might you get? 

答案A- 
[ ] μ1=−3,σ21=4

- 
[ ] μ1=−6,σ21=4

- 
[ ] μ1=−3,σ21=2

- 
[ ] μ1=−6,σ21=2























