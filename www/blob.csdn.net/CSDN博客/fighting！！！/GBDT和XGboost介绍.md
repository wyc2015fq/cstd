# GBDT和XGboost介绍 - fighting！！！ - CSDN博客
2019年01月29日 20:33:25[dujiahei](https://me.csdn.net/dujiahei)阅读数：145
转自：[https://blog.csdn.net/legendavid/article/details/78904353](https://blog.csdn.net/legendavid/article/details/78904353)
## 前言
GBDT（Gradient Boosting Decision Tree）是一种基于迭代所构造的决策树算法，它又可以简称为MART（Multiple Additive Regression Tree）或GBRT（Gradient Boosting Regression Tree）。虽然名字上又是Gradient又是Boosting的，但它的原理还是很浅显易懂（当然详细的推导还是有一些难度）。简单来讲，这种算法在实际问题中将生成多棵决策树，并将所有树的结果进行汇总来得到最终答案。也就是说，该算法将决策树与集成思想进行了有效的结合。
前面我们已经学过，集成思想主要分为两大流派，Boosting一族通过将弱学习器提升为强学习器的集成方法来提高预测精度（典型算法为AdaBoost，详见[Learn R | AdaBoost of Data Mining](https://zhuanlan.zhihu.com/p/24790439?refer=The-Art-of-Data)）；而另一类则为Bagging，即通过自助采样的方法生成众多并行式的分类器，通过“少数服从多数”的原则来确定最终的结果（典型算法为随机森林，详见[Learn R | Random Forest of Data Mining](https://zhuanlan.zhihu.com/p/24349592?refer=The-Art-of-Data)）。
今天所要学习的GBDT同样是属于Boosting大家庭中的一员，自算法的诞生之初，它就和SVM一起被认为是泛化能力（generalization）较强的算法。近些年来更因为被用于构建搜索排序的机器学习模型而引起广泛的关注。
> 除此之外，GBDT还是目前竞赛中最为常用的一种机器学习算法，因为它不仅可以适用于多种场景，而且相比较于其他算法还有着出众的准确率，如此优异的性能也让GBDT收获了机器学习领域的“屠龙刀”这一赞誉。
那么，如此优秀的算法在实际中是如何进行工作的呢？背后的基本原理又是什么？接下来，我们就来走进算法内部，揭开这把“屠龙宝刀”的神秘面纱。
## 一、GBDT之DT——回归树
GBDT主要由三个概念组成：Regression Decistion Tree、Gradient Boosting与Shrinkage，只有弄清楚这三个概念，我们才能明白算法的基本原理。首先来学习第一个概念：Regression Decistion Tree，即回归决策树。
提到决策树，相信很多人会潜意识的想到最常见的分类决策树（ID3、C4.5、CART等等），但要把GBDT中的DT也理解为分类决策树那就[大错特错](https://www.baidu.com/s?wd=%E5%A4%A7%E9%94%99%E7%89%B9%E9%94%99&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)了。实际上，决策树不仅可以用于分类，还可用于回归，它的作用在于数值预测，例如明天的温度、用户的年龄等等，而且对基于回归树所得到的数值进行加减是有意义的（例如10岁+5岁-3岁=12岁），这是区别于分类树的一个显著特征（毕竟男+女=是男是女?，这样的运算是毫无道理的）。GBDT在运行时就使用到了回归树的这个性质，它将累加所有树的结果作为最终结果。所以，GBDT中的所有决策树都是回归树，而非分类树。
接着，我们对问题进行进一步细分，来分析具体的一棵回归树的运行流程。
作为对比，简要回顾下分类树的运行过程：以ID3为例，穷举每一个属性特征的信息增益值，每一次都选取使信息增益最大的特征进行分枝，直到分类完成或达到预设的终止条件，实现决策树的递归构建。
回归树的运行流程与分类树基本类似，但有以下两点不同之处：
- 第一，回归树的每个节点得到的是一个预测值而非分类树式的样本计数，假设在某一棵树的某一节点使用了年龄进行分枝（并假设在该节点上人数![>1](http://www.zhihu.com/equation?tex=%3E1)），那么这个预测值就是属于这个节点的所有人年龄的平均值。
- 第二，在分枝节点的选取上，回归树并没有选用最大熵值来作为划分标准，而是使用了最小化均方差，即![\frac{\sum_{i=1}^{n}{} (x_i-\bar{x} )^2}{n}](http://www.zhihu.com/equation?tex=%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%7B%7D+%28x_i-%5Cbar%7Bx%7D+%29%5E2%7D%7Bn%7D+)。这很好理解，被预测出错的次数越多，错的越离谱，均方差就越大，通过最小化均方差也就能够找到最靠谱的分枝依据。
> 一般来讲，回归树的分枝不太可能实现每个叶子节点上的属性值都唯一，更多的是达到我们预设的终止条件即可（例如叶子个数上限），这样势必会存在多个属性取值，那么该节点处的预测值自然就为基于这些样本所得到的平均值了。
## 二、GBDT之GB——梯度提升
在简单了解了回归树后，继续来看第二个概念：梯度提升（Gradient Boosting）。
首先需要明确，GB本身是一种理念而非一个具体的算法，其基本思想为：沿着梯度方向，构造一系列的弱分类器函数，并以一定权重组合起来，形成最终决策的强分类器（由于梯度提升的具体内容与数学推导有一些复杂，而且即使不了解这块知识的[来龙去脉](https://www.baidu.com/s?wd=%E6%9D%A5%E9%BE%99%E5%8E%BB%E8%84%89&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)也不妨碍对算法本身的理解，出于这样的考虑，本文将不涉及'Gradient'，梯度提升的内容也将放在下一篇文章中详细介绍）。
那么这一系列的弱分类器是怎么样形成的呢？这就是GBDT的核心所在：每一棵树所学习的是之前所有树结论和的残差，这个残差就是一个加预测值后能得真实值的累加量。
> 举一个简单的例子，同样使用年龄进行分枝，假设我们A的真实年龄是18岁，但第一棵树的预测年龄是12岁，即残差为6岁。那么在第二棵树里我们把A的年龄设为6岁去学习，如果第二棵树真的能把A分到6岁的叶子节点，那累加两棵树的结论就是A的真实年龄；如果第二棵树的结论是5岁，则A仍然存在1岁的残差，第三棵树里A的年龄就变成1岁……以此类推学习下去，这就是梯度提升在GBDT算法中的直观意义。
## 三、GBDT算法的简单应用
接下来还是通过训练一个用于预测年龄的模型来展现算法的运行流程（本节的内容与图片引用自博客文章[GBDT（MART） 迭代决策树入门教程 | 简介](http://link.zhihu.com/?target=http%3A//blog.csdn.net/w28971023/article/details/8240756)）
首先，训练集有4个人A、B、C、D，他们的年龄分别是14、16、24、26。其中A，B分别是高一和高三学生；C，D分别是应届毕业生和工作两年的员工，可用于分枝的特征包括上网时长、购物金额、上网时段和对百度知道的使用方式等。如果是用一棵传统的回归决策树来训练，会得到如下图所示结果：
![](https://pic4.zhimg.com/50/v2-6e5ea0e0134fc8b9855c0d5e9361602f_hd.jpg)但如果是用GBDT来做这件事，由于数据太少，我们限定叶子节点做多有两个，即每棵树都只有一个分枝，并且限定只学两棵树。我们会得到如下图所示结果：
![](https://pic3.zhimg.com/50/v2-616ff0c92bae55a4801bb43a0cbdb8eb_hd.jpg)
第一棵树的分枝与之前一样，也是使用购物金额进行区分，两拨人各自用年龄均值作为预测值，得到残差值-1、1、-1、1，然后拿这些残差值替换初始值去训练生成第二棵回归树，如果新的预测值和残差相等，则只需把第二棵树的结论累加到第一棵树上就能得到真实年龄了。
第二棵树只有两个值1和-1，直接可分成两个节点。此时所有人的残差都是0，即每个人都得到了真实的预测值。
将两棵回归树预测结果进行汇总，解释如下：
- A：14岁高一学生；购物较少；经常问学长问题；预测年龄A = 15 – 1 = 14
- B：16岁高三学生；购物较少；经常被学弟问问题；预测年龄B = 15 + 1 = 16
- C：24岁应届毕业生；购物较多，经常问师兄问题；预测年龄C = 25 – 1 = 24
- D：26岁工作两年员工；购物较多，经常被师弟问问题；预测年龄D = 25 + 1 = 26
对比初始的回归树与GBDT所生成的回归树，可以发现，最终的结果是相同的，那我们为什么还要使用GBDT呢？
答案就是对模型过拟合的考虑。过拟合是指为了让训练集精度更高，学到了很多“仅在训练集上成立的规律”，导致换一个数据集后，当前规律的预测精度就不足以使人满意了。毕竟，在训练精度和实际精度（或测试精度）之间，后者才是我们想要真正得到的。
> 在上面这个例子中，初始的回归树为达到100%精度使用了3个特征（上网时长、时段、网购金额），但观察发现，分枝“上网时长>1.1h”很显然过拟合了，不排除恰好A上网1.5h, B上网1小时，所以用上网时间是不是>1.1小时来判断所有人的年龄很显然是有悖常识的。
而在GBDT中，两棵回归树仅使用了两个特征（购物金额与对百度知道的使用方式）就实现了100%的预测精度，其分枝依据更合乎逻辑（当然这里是相比较于上网时长特征而言），算法在运行中也体现了“如无必要，勿增实体”的奥卡姆剃刀原理。
## 四、GBDT之Shrinkage——缩减
> Shrinkage是GBDT的第三个基本概念，中文含义为“缩减”。它的基本思想就是：每次走一小步逐渐逼近结果的效果，要比每次迈一大步很快逼近结果的方式更容易避免过拟合。换句话说缩减思想不完全信任每一个棵残差树，它认为每棵树只学到了真理的一小部分，累加的时候只累加一小部分，只有通过多学几棵树才能弥补不足。
Shrinkage仍然以残差作为学习目标，但由于它采用的是逐步逼近目标的方式，导致各个树的残差是渐变的而不是陡变的。之所以这样做也是基于模型过拟合的考虑（更为详细的内容可参考文末给出的参考资料）。
GBDT的基本内容大致介绍完毕，在下一文中，我们将从梯度的角度对算法进行推导与学习。
## 一、GBDT的梯度提升过程
在上一文中，我们详细讲述了GBDT算法的基本概念，并通过一个简单的小例子阐述它在实际使用中的运行流程，可以说我们对算法已经有了一个初步的认识。不过还留有一个问题尚待解决，那就是如何理解GBDT中的Gradient（梯度）？
[众所周知](https://www.baidu.com/s?wd=%E4%BC%97%E6%89%80%E5%91%A8%E7%9F%A5&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，基于Boosting的集成学习是通过迭代得到一系列的弱学习器，进而通过不同的组合策略得到相应的强学习器。在GBDT的迭代中，假设前一轮得到的强学习器为![f_{t-1}(x)](http://www.zhihu.com/equation?tex=f_%7Bt-1%7D%28x%29)，对应的损失函数则为![L(y,f_{t-1}(x))](http://www.zhihu.com/equation?tex=L%28y%2Cf_%7Bt-1%7D%28x%29%29)。因此新一轮迭代的目的就是找到一个弱学习器![h_t(x)](http://www.zhihu.com/equation?tex=h_t%28x%29)，使得损失函数![L(y,f_{t-1}(x)+h_t(x))](http://www.zhihu.com/equation?tex=L%28y%2Cf_%7Bt-1%7D%28x%29%2Bh_t%28x%29%29)达到最小。
因此问题的关键就在于对损失函数的度量，这也正是难点所在，毕竟损失函数多种多样，怎么样才能找到一种通用的拟合方法呢？
![](https://pic3.zhimg.com/50/v2-e28f3b474f4086211e46cdefa6239b0a_hd.jpg)
（上图为常用的损失函数及其梯度，图片来源：[The Elements of Statistical Learning](http://link.zhihu.com/?target=http%3A//statweb.stanford.edu/~tibs/ElemStatLearn/)，下同）
针对这一问题，机器学习界的大牛Freidman提出了梯度提升算法：利用最速下降的近似方法，即利用损失函数的负梯度在当前模型的值，作为回归问题中提升树算法的残差的近似值，拟合一个回归树。
这样的话，第![t](http://www.zhihu.com/equation?tex=t)轮的第![i](http://www.zhihu.com/equation?tex=i)个样本的负梯度表示为：![r_{ti}=-\left[ \frac{\partial L(y,f(x_i))}{\partial f(x_i)} \right]_{f(x)=f_{t-1}(x)}](http://www.zhihu.com/equation?tex=r_%7Bti%7D%3D-%5Cleft%5B+%5Cfrac%7B%5Cpartial+L%28y%2Cf%28x_i%29%29%7D%7B%5Cpartial+f%28x_i%29%7D+%5Cright%5D_%7Bf%28x%29%3Df_%7Bt-1%7D%28x%29%7D++)，算法的完整流程如下：
![](https://pic1.zhimg.com/50/v2-bec5b3cb12594fb1245461264cad0bbe_hd.jpg)
接下来对上图中的算法步骤进行详细解释：
- 初始化弱学习器![f_0(x)](http://www.zhihu.com/equation?tex=f_0%28x%29)，得到使损失函数极小化的一个常数值，此时树仅有一个根节点
- 计算损失函数的负梯度值，以此作为残差的估计
	- 针对选取的不同的损失函数（平方、绝对值、Huber），对应图1中不同的梯度值；
- 算法中嵌套两层循环，分别为迭代轮数![m](http://www.zhihu.com/equation?tex=m)和样本![i](http://www.zhihu.com/equation?tex=i)
- 利用计算得到的![(x_i,r_{ti})](http://www.zhihu.com/equation?tex=%28x_i%2Cr_%7Bti%7D%29)拟合一棵CART回归树，得到第![m](http://www.zhihu.com/equation?tex=m)轮的回归树，对应的叶子节点区域为![R_{jm},j=1,2,...,J_m](http://www.zhihu.com/equation?tex=R_%7Bjm%7D%2Cj%3D1%2C2%2C...%2CJ_m)（![J_m](http://www.zhihu.com/equation?tex=J_m)为回归树![t](http://www.zhihu.com/equation?tex=t)的叶子节点的个数）
- 接着，对叶子区域计算最佳拟合值![\gamma _{jm}](http://www.zhihu.com/equation?tex=%5Cgamma+_%7Bjm%7D)（损失函数极小化）并更新强学习器![f_m(x)](http://www.zhihu.com/equation?tex=f_m%28x%29)
- 最后，在迭代结束后输出最终模型![\widehat{f}(x)](http://www.zhihu.com/equation?tex=%5Cwidehat%7Bf%7D%28x%29)
## 二、GBDT小结
至此，GBDT的内容就基本讲完了，对这个算法，一方面我们可以从残差的角度来理解，每一棵回归树都是在学习之前的树的残差；另一方面也可以从梯度的角度掌握算法，即每一棵回归树通过梯度下降法学习之前的树的梯度下降值。
这样看来，这两种理解角度从总体流程和输入输出上没有区别的，它们都是迭代回归树，都是累加每棵树结果作为最终结果，每棵树都在学习前面树尚存的不足。而不同之处就在于每一步迭代时的求解方法的不同，前者使用残差（残差是全局最优值），后者使用梯度（梯度是局部最优方向），简单一点来讲就是前者每一步都在试图向最终结果的方向优化，后者则每一步试图让当前结果更好一点。
> 看起来前者更科学一点，毕竟有绝对最优方向不学，为什么舍近求远去估计一个局部最优方向呢？原因在于灵活性。前者最大问题是，由于它依赖残差，损失函数一般固定为反映残差的均方差，因此很难处理纯回归问题之外的问题。而后者求解方法为梯度下降，只要可求导的损失函数都可以使用。
最后小结一下GBDT算法的优缺点。
优点：
- 预测精度高
- 适合低维数据
- 能处理非线性数据
缺点：
- 并行麻烦（因为上下两棵树有联系）
- 如果数据维度较高时会加大算法的计算复杂度
## 三、GBDT算法的R实现
在R中，我们可以使用gbm包中的相关函数来实现算法，首先进行安装与载入。
```
> install.packages('gbm')
> library(gbm)
```
```
# gbm()函数的调用公式及主要参数如下：
> gbm(formula = formula(data),distribution = "bernoulli",data = list(),
+ n.trees = 100,shrinkage = 0.001,...,
+ bag.fraction = 0.5,interaction.depth = 1)
# distribution:损失函数的形式,可选择 "gaussian"(squared error),"laplace" (absolute loss),"bernoulli"(logistic regression for 0-1 outcomes), "huberized"(huberized hinge loss for 0-1 outcomes)等
# n.trees:迭代次数
# shrinkage:学习速率,我们都知道步子迈得太大容易扯着蛋，所以学习速率是越小越好，但是步子太小的话，步数就得增加，也就是训练的迭代次数需要加大才能使模型达到最优，这样训练所需时间和计算资源也相应加大了,gbm作者的经验法则是设置参数在0.01-0.001之间
# bag.fraction:再抽样比率
# 除此之外,函数中还有其他参数,可自行查阅相关文档
```
接下来选用TH.data包中的bodyfat数据集进行实证分析，这个数据集记录了71名健康女性的身体数据，包括年龄、腰围、臀围等变量，用于身体脂肪（DEXfat）的预测分析。
```
> library(TH.data)
> data(bodyfat)
> dim(bodyfat)
[1] 71 10
> head(bodyfat)
   age DEXfat waistcirc hipcirc elbowbreadth kneebreadth anthro3a anthro3b
47  57  41.68     100.0   112.0          7.1         9.4     4.42     4.95 
48  65  43.29      99.5   116.5          6.5         8.9     4.63     5.01 
49  59  35.41      96.0   108.5          6.2         8.9     4.12     4.74     
50  58  22.79      72.0    96.5          6.1         9.2     4.03     4.48     
51  60  36.42      89.5   100.5          7.1        10.0     4.24     4.68     
52  61  24.13      83.5    97.0          6.5         8.8     3.55     4.06       
   anthro3c anthro4
47     4.50    6.13
48     4.48    6.37
49     4.60    5.82
50     3.91    5.66
51     4.15    5.91
52     3.64    5.14
```
```
# 模型构建
> gbdt_model <- gbm(DEXfat~.,distribution = 'gaussian',data=bodyfat,
+ n.trees=1000,shrinkage = 0.01)
```
```
# 使用gbm.pref()确定最佳迭代次数
> best.iter <- gbm.perf(gbdt_model)
> best.iter
[1] 399
```
![](https://pic1.zhimg.com/50/v2-3bd703e4528f34e3e90d521b274d514f_hd.jpg)
```
# 查看各变量的重要程度
> summary.gbm(gbdt_model,best.iter)
                      var   rel.inf
hipcirc           hipcirc 36.276643
waistcirc       waistcirc 31.447718
anthro3c         anthro3c  6.561716
anthro4           anthro4  6.447923
anthro3b         anthro3b  6.423434
anthro3a         anthro3a  5.068798
kneebreadth   kneebreadth  3.834679
age                   age  2.050727
elbowbreadth elbowbreadth  1.888361
```
![](https://pic1.zhimg.com/50/v2-e47507f0ac6355076147ce72353234b9_hd.jpg)
```
# 查看各变量的边际效应,用数值来自定义变量查询
> par(mfrow=c(1,3))
> plot.gbm(gbdt_model,3,best.iter)
> plot.gbm(gbdt_model,4,best.iter)
> plot.gbm(gbdt_model,5,best.iter)
```
![](https://pic1.zhimg.com/50/v2-0666b8eed307a9442aaab2736edb86ee_hd.jpg)
```
# 使用建立好的模型进行DEXfat值的预测
> gbdt_fit <- predict(gbdt_model,bodyfat,best.iter)
> head(gbdt_fit)
[1] 43.78154 43.51006 36.23172 23.54703 36.71636 22.47789
# 计算预测结果与真实结果的方差值
> print(sum((bodyfat$DEXfat-gbdt_fit)^2))
[1] 924.0749
```
这就是GBDT算法在R中的一个实例应用，由于我们的目的在于获取所需的预测值，所以无法直接通过对比预测值与实际值的一致程度（即预测精度）来判断模型的好坏，而是选用了方差作为衡量模型优劣的标准。如果我们想要查看一个模型在新的数据集上的表现如何，可以考虑生成训练数据集与测试数据集，操作如下：
```
> index <- sample(2,nrow(bodyfat),replace = TRUE,prob=c(0.7,0.3))
> traindata <- bodyfat[index==1,]
> testdata <- bodyfat[index==2,]
```
这样的话，生成模型选用traindata，进行预测可选用testdata。
最后需要注意的是，在bodyfat的案例中，我们一开始选用了回归中最常见的平方损失，但要知道不同的损失函数对最终的预测精度也会起到不同的影响，其他参数也是同理。所以，如果要对模型进行进一步改进的话，参数选择这块还是大有文章可做的。
## 一、XGBoost简介
在GBDT的学习过程中，不少博客都提到了该算法的升级版——XGBoost，并对它赞赏有加。所以，这一块的知识（包括算法的基本内容，数学推导与案例实现）将使用两篇文章的篇幅来进行学习掌握。
经过前面的学习，我们已经知道，GBDT是一种基于集成思想下的Boosting学习器，并采用梯度提升的方法进行每一轮的迭代最终组建出强学习器，这样的话算法的运行往往要生成一定数量的树才能达到令我们满意的准确率。当数据集大且较为复杂时，运行一次极有可能需要几千次的迭代运算，这将对我们使用算法造成巨大的计算瓶颈。
针对这一问题，华盛顿大学的陈天奇博士开发出了XGBoost（eXtreme Gradient Boosting），它是Gradient Boosting Machine的一个c++实现，并在原有的基础上加以改进，从而极大地提升了模型训练速度和预测精度。可以说，XGBoost是Gradient Boosting的高效实现。
> XGBoost最大的特点在于它能够自动利用CPU的多线程进行并行计算，同时在算法上加以改进提高了精度。在Kaggle的希格斯子信号识别竞赛中，XGBoost因为出众的效率与较高的预测准确度在比赛论坛中引起了参赛选手的广泛关注，在1700多支队伍的激烈竞争中占有一席之地。随着它在Kaggle社区知名度的提高，在其他的比赛中也有队伍借助XGBoost夺得第一。
接下来学习XGBoost算法的数学推导过程。
## 二、目标函数：损失与正则
在监督学习中，我们通常会构造一个目标函数和一个预测函数，使用训练样本对目标函数最小化学习到相关的参数，然后用预测函数和训练样本得到的参数来对未知的样本进行分类的标注或者数值的预测。在XGBoost中，目标函数的形式为：![Obj(\Theta) = L(\theta) + \Omega(\Theta)](http://www.zhihu.com/equation?tex=Obj%28%5CTheta%29+%3D+L%28%5Ctheta%29+%2B+%5COmega%28%5CTheta%29)
- ![L(\theta)](http://www.zhihu.com/equation?tex=L%28%5Ctheta%29)：损失函数，常用损失函数有：
	- 平方损失：![L(\theta) = \sum_i (y_i-\hat{y}_i)^2](http://www.zhihu.com/equation?tex=L%28%5Ctheta%29+%3D+%5Csum_i+%28y_i-%5Chat%7By%7D_i%29%5E2)
- Logistic损失：![L(\theta) = \sum_i[ y_i\ln (1+e^{-\hat{y}_i}) + (1-y_i)\ln (1+e^{\hat{y}_i})]](http://www.zhihu.com/equation?tex=L%28%5Ctheta%29+%3D+%5Csum_i%5B+y_i%5Cln+%281%2Be%5E%7B-%5Chat%7By%7D_i%7D%29+%2B+%281-y_i%29%5Cln+%281%2Be%5E%7B%5Chat%7By%7D_i%7D%29%5D)
- ![\Omega(\Theta)](http://www.zhihu.com/equation?tex=%5COmega%28%5CTheta%29)：正则化项，之所以要引入它是因为我们的目标是希望生成的模型能准确的预测新的样本（即应用于测试数据集），而不是简单的拟合训练集的结果（这样会导致过拟合）。所以需要在保证模型“简单”的基础上最小化训练误差，这样得到的参数才具有好的泛化性能。而正则项就是用于惩罚复杂模型，避免预测模型过分拟合训练数据，常用的正则有![L_1](http://www.zhihu.com/equation?tex=L_1)正则与![L_2](http://www.zhihu.com/equation?tex=L_2)正则。
![](https://pic3.zhimg.com/50/v2-3a395f4aa0ec27706c30ccd356ae984f_hd.jpg)
上图所展示的就是损失函数与正则化项在模型中的应用（图片来源：[Introduction to Boosted Trees](http://link.zhihu.com/?target=http%3A//xgboost.readthedocs.io/en/latest/model.html)）。观察发现，如果目标函数中的损失函数权重过高，那么模型的预测精度则不尽人意，反之如果正则项的权重过高，所生成的模型则会出现过拟合情况，难以对新的数据集做出有效预测。只有平衡好两者之间的关系，控制好模型复杂度，并在此基础上对参数进行求解，生成的模型才会“简单有效”（这也是机器学习中的偏差方差均衡）。
## 三、XGBoost的推导过程
1. 目标函数的迭代与泰勒展开
由于之前已经学习过树的生成及集成方法，这里不再赘述。首先，我们可以把某一次迭代后集成的模型表示为：![\hat{y}_i = \sum_{k=1}^K f_k(x_i), f_k \in \mathcal{F}](http://www.zhihu.com/equation?tex=%5Chat%7By%7D_i+%3D+%5Csum_%7Bk%3D1%7D%5EK+f_k%28x_i%29%2C+f_k+%5Cin+%5Cmathcal%7BF%7D)（![\hat{y}_i](http://www.zhihu.com/equation?tex=%5Chat%7By%7D_i+)也就是上文中的![f_m(x)](http://www.zhihu.com/equation?tex=f_m%28x%29)）
相对应的目标函数：![\text{Obj}(\theta) = \sum_i^n l(y_i, \hat{y}_i) + \sum_{k=1}^K \Omega(f_k)](http://www.zhihu.com/equation?tex=%5Ctext%7BObj%7D%28%5Ctheta%29+%3D+%5Csum_i%5En+l%28y_i%2C+%5Chat%7By%7D_i%29+%2B+%5Csum_%7Bk%3D1%7D%5EK+%5COmega%28f_k%29)
将这两个公式进行扩展，应用在前![t](http://www.zhihu.com/equation?tex=t)轮的模型迭代中，具体表示为：
![\begin{split}\hat{y}_i^{(0)} &= 0\\\hat{y}_i^{(1)} &= f_1(x_i) = \hat{y}_i^{(0)} + f_1(x_i)\\\hat{y}_i^{(2)} &= f_1(x_i) + f_2(x_i)= \hat{y}_i^{(1)} + f_2(x_i)\\&\dots\\\hat{y}_i^{(t)} &= \sum_{k=1}^t f_k(x_i)= \hat{y}_i^{(t-1)} + f_t(x_i)\end{split}](http://www.zhihu.com/equation?tex=%5Cbegin%7Bsplit%7D%5Chat%7By%7D_i%5E%7B%280%29%7D+%26%3D+0%5C%5C%0A%5Chat%7By%7D_i%5E%7B%281%29%7D+%26%3D+f_1%28x_i%29+%3D+%5Chat%7By%7D_i%5E%7B%280%29%7D+%2B+f_1%28x_i%29%5C%5C%0A%5Chat%7By%7D_i%5E%7B%282%29%7D+%26%3D+f_1%28x_i%29+%2B+f_2%28x_i%29%3D+%5Chat%7By%7D_i%5E%7B%281%29%7D+%2B+f_2%28x_i%29%5C%5C%0A%26%5Cdots%5C%5C%0A%5Chat%7By%7D_i%5E%7B%28t%29%7D+%26%3D+%5Csum_%7Bk%3D1%7D%5Et+f_k%28x_i%29%3D+%5Chat%7By%7D_i%5E%7B%28t-1%29%7D+%2B+f_t%28x_i%29%0A%5Cend%7Bsplit%7D)
![\hat{y}_i^{(t-1)}](http://www.zhihu.com/equation?tex=%5Chat%7By%7D_i%5E%7B%28t-1%29%7D)就是前![t-1](http://www.zhihu.com/equation?tex=t-1)轮的模型预测，![f_t(x_i)](http://www.zhihu.com/equation?tex=f_t%28x_i%29)为新![t](http://www.zhihu.com/equation?tex=t)轮加入的预测函数。
这里自然就涉及一个问题：如何选择在每一轮中加入的![f(x_i)](http://www.zhihu.com/equation?tex=f%28x_i%29)呢？答案很直接，选取的![f(x_i)](http://www.zhihu.com/equation?tex=f%28x_i%29)必须使得我们的目标函数尽量最大地降低（这里应用到了Boosting的基本思想，即当前的基学习器重点关注以前所有学习器犯错误的那些数据样本，以此来达到提升的效果）。先对目标函数进行改写，表示如下：
![\begin{split}\text{Obj}^{(t)} & = \sum_{i=1}^n l(y_i, \hat{y}_i^{(t)}) + \sum_{i=1}^t\Omega(f_i) \\          & = \sum_{i=1}^n l(y_i, \hat{y}_i^{(t-1)} + f_t(x_i)) + \Omega(f_t) + constant\end{split}](http://www.zhihu.com/equation?tex=%5Cbegin%7Bsplit%7D%5Ctext%7BObj%7D%5E%7B%28t%29%7D+%26+%3D+%5Csum_%7Bi%3D1%7D%5En+l%28y_i%2C+%5Chat%7By%7D_i%5E%7B%28t%29%7D%29+%2B+%5Csum_%7Bi%3D1%7D%5Et%5COmega%28f_i%29+%5C%5C%0A++++++++++%26+%3D+%5Csum_%7Bi%3D1%7D%5En+l%28y_i%2C+%5Chat%7By%7D_i%5E%7B%28t-1%29%7D+%2B+f_t%28x_i%29%29+%2B+%5COmega%28f_t%29+%2B+constant%0A%5Cend%7Bsplit%7D)
如果我们考虑使用平方误差作为损失函数，公式可改写为：
![\begin{split}\text{Obj}^{(t)} & = \sum_{i=1}^n (y_i - (\hat{y}_i^{(t-1)} + f_t(x_i)))^2 + \sum_{i=1}^t\Omega(f_i) \\          & = \sum_{i=1}^n [2(\hat{y}_i^{(t-1)} - y_i)f_t(x_i) + f_t(x_i)^2] + \Omega(f_t) + constant\end{split}](http://www.zhihu.com/equation?tex=%5Cbegin%7Bsplit%7D%5Ctext%7BObj%7D%5E%7B%28t%29%7D+%26+%3D+%5Csum_%7Bi%3D1%7D%5En+%28y_i+-+%28%5Chat%7By%7D_i%5E%7B%28t-1%29%7D+%2B+f_t%28x_i%29%29%29%5E2+%2B+%5Csum_%7Bi%3D1%7D%5Et%5COmega%28f_i%29+%5C%5C%0A++++++++++%26+%3D+%5Csum_%7Bi%3D1%7D%5En+%5B2%28%5Chat%7By%7D_i%5E%7B%28t-1%29%7D+-+y_i%29f_t%28x_i%29+%2B+f_t%28x_i%29%5E2%5D+%2B+%5COmega%28f_t%29+%2B+constant%0A%5Cend%7Bsplit%7D)
更加一般的，对于不是平方误差的情况，我们可以采用如下的泰勒展开近似来定义一个近似的目标函数，方便我们进行这一步的计算。
> 泰勒展开：![f(x+\Delta  x)\simeq f(x)+f^{'}(x)\Delta x+\frac{1}{2} f^{''}(x)\Delta x^2](http://www.zhihu.com/equation?tex=f%28x%2B%5CDelta++x%29%5Csimeq+f%28x%29%2Bf%5E%7B%27%7D%28x%29%5CDelta+x%2B%5Cfrac%7B1%7D%7B2%7D+f%5E%7B%27%27%7D%28x%29%5CDelta+x%5E2)
![\text{Obj}^{(t)} = \sum_{i=1}^n [l(y_i, \hat{y}_i^{(t-1)}) + g_i f_t(x_i) + \frac{1}{2} h_i f_t^2(x_i)] + \Omega(f_t) + constant](http://www.zhihu.com/equation?tex=%5Ctext%7BObj%7D%5E%7B%28t%29%7D+%3D+%5Csum_%7Bi%3D1%7D%5En+%5Bl%28y_i%2C+%5Chat%7By%7D_i%5E%7B%28t-1%29%7D%29+%2B+g_i+f_t%28x_i%29+%2B+%5Cfrac%7B1%7D%7B2%7D+h_i+f_t%5E2%28x_i%29%5D+%2B+%5COmega%28f_t%29+%2B+constant)
其中![g_i= \partial_{\hat{y}_i^{(t-1)}} l(y_i, \hat{y}_i^{(t-1)})](http://www.zhihu.com/equation?tex=g_i%3D+%5Cpartial_%7B%5Chat%7By%7D_i%5E%7B%28t-1%29%7D%7D+l%28y_i%2C+%5Chat%7By%7D_i%5E%7B%28t-1%29%7D%29)，![h_i = \partial_{\hat{y}_i^{(t-1)}}^2 l(y_i, \hat{y}_i^{(t-1)})](http://www.zhihu.com/equation?tex=h_i+%3D+%5Cpartial_%7B%5Chat%7By%7D_i%5E%7B%28t-1%29%7D%7D%5E2+l%28y_i%2C+%5Chat%7By%7D_i%5E%7B%28t-1%29%7D%29)
> 如果移除掉常数项，我们会发现这个目标函数有一个非常明显的特点，它只依赖于每个数据点的在误差函数上的一阶导数和二阶导数（![\sum_{i=1}^n [g_i f_t(x_i) + \frac{1}{2} h_i f_t^2(x_i)] + \Omega(f_t)](http://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5En+%5Bg_i+f_t%28x_i%29+%2B+%5Cfrac%7B1%7D%7B2%7D+h_i+f_t%5E2%28x_i%29%5D+%2B+%5COmega%28f_t%29)）。有人可能会问，这个公式似乎比我们之前学过的决策树学习难懂。为什么要花这么多力气来做推导呢？
这是因为，这样做会使我们可以很清楚地理解整个目标是什么，并且一步一步推导出如何进行树的学习。这一个抽象的形式对于实现机器学习工具也是非常有帮助的。因为它包含所有可以求导的目标函数，也就是说有了这个形式，我们写出来的代码可以用来求解包括回归，分类和排序的各种问题，正式的推导可以使得机器学习的工具更加一般化。
2. 决策树的复杂度
接着来讨论如何定义树的复杂度。我们先对于![f](http://www.zhihu.com/equation?tex=f)的定义做一下细化，把树拆分成结构部分![q](http://www.zhihu.com/equation?tex=q)和叶子权重部分![w](http://www.zhihu.com/equation?tex=w)。其中结构函数![q](http://www.zhihu.com/equation?tex=q)把输入映射到叶子的索引号上面去，而![w](http://www.zhihu.com/equation?tex=w)给定了每个索引号对应的叶子分数是什么。具体公式为：![f_t(x) = w_{q(x)}, w \in R^T, q:R^d\rightarrow \{1,2,\cdots,T\}](http://www.zhihu.com/equation?tex=f_t%28x%29+%3D+w_%7Bq%28x%29%7D%2C+w+%5Cin+R%5ET%2C+q%3AR%5Ed%5Crightarrow+%5C%7B1%2C2%2C%5Ccdots%2CT%5C%7D+)
当我们给定上述定义后，那么一棵树的复杂度就为![\Omega(f) = \gamma T + \frac{1}{2}\lambda \sum_{j=1}^T w_j^2](http://www.zhihu.com/equation?tex=%5COmega%28f%29+%3D+%5Cgamma+T+%2B+%5Cfrac%7B1%7D%7B2%7D%5Clambda+%5Csum_%7Bj%3D1%7D%5ET+w_j%5E2)
> 这个复杂度包含了一棵树里面节点的个数（左侧），以及每个树叶子节点上面输出分数的![L_2](http://www.zhihu.com/equation?tex=L_2)模平方（右侧）。当然这不是唯一的一种定义方式，不过这一定义方式学习出的树效果一般都比较不错。
简单提及一下![\gamma](http://www.zhihu.com/equation?tex=%5Cgamma+)和![\lambda](http://www.zhihu.com/equation?tex=%5Clambda+)两个系数的作用，![\gamma](http://www.zhihu.com/equation?tex=%5Cgamma+)作为叶子节点的系数，使XGBoost在优化目标函数的同时相当于做了预剪枝；![\lambda](http://www.zhihu.com/equation?tex=%5Clambda+)作为![L_2](http://www.zhihu.com/equation?tex=L_2)平方模的系数也是要起到防止过拟合的作用。
这里举一个小例子加深对复杂度的理解（图片来源：[Introduction to Boosted Trees](http://link.zhihu.com/?target=http%3A//xgboost.readthedocs.io/en/latest/model.html)，下同）
![](https://pic3.zhimg.com/50/v2-a41a4d0cb6f3c3046cadfc4f8e51c71e_hd.jpg)
上图为实际生成的一棵决策树，底部的数字代表决策树的预测值，那么这棵树的复杂度自然就为：![\Omega =\gamma 3+\frac{1}{2} \lambda \sum_{j=1}^{T}{(4+0.01+1)}](http://www.zhihu.com/equation?tex=%5COmega+%3D%5Cgamma+3%2B%5Cfrac%7B1%7D%7B2%7D+%5Clambda+%5Csum_%7Bj%3D1%7D%5E%7BT%7D%7B%284%2B0.01%2B1%29%7D+)
3. 目标函数的最小化
接下来就是非常关键的一步，在这种新的定义下，我们可以把目标函数进行如下改写，其中![I](http://www.zhihu.com/equation?tex=I)被定义为每个叶子上面样本集合![I_j = \{i|q(x_i)=j\}](http://www.zhihu.com/equation?tex=I_j+%3D+%5C%7Bi%7Cq%28x_i%29%3Dj%5C%7D)
![\begin{split}Obj^{(t)} &\approx \sum_{i=1}^n [g_i w_{q(x_i)} + \frac{1}{2} h_i w_{q(x_i)}^2] + \gamma T + \frac{1}{2}\lambda \sum_{j=1}^T w_j^2\\&= \sum^T_{j=1} [(\sum_{i\in I_j} g_i) w_j + \frac{1}{2} (\sum_{i\in I_j} h_i + \lambda) w_j^2 ] + \gamma T\end{split}](http://www.zhihu.com/equation?tex=%5Cbegin%7Bsplit%7DObj%5E%7B%28t%29%7D+%26%5Capprox+%5Csum_%7Bi%3D1%7D%5En+%5Bg_i+w_%7Bq%28x_i%29%7D+%2B+%5Cfrac%7B1%7D%7B2%7D+h_i+w_%7Bq%28x_i%29%7D%5E2%5D+%2B+%5Cgamma+T+%2B+%5Cfrac%7B1%7D%7B2%7D%5Clambda+%5Csum_%7Bj%3D1%7D%5ET+w_j%5E2%5C%5C%0A%26%3D+%5Csum%5ET_%7Bj%3D1%7D+%5B%28%5Csum_%7Bi%5Cin+I_j%7D+g_i%29+w_j+%2B+%5Cfrac%7B1%7D%7B2%7D+%28%5Csum_%7Bi%5Cin+I_j%7D+h_i+%2B+%5Clambda%29+w_j%5E2+%5D+%2B+%5Cgamma+T%0A%5Cend%7Bsplit%7D)
分别定义![G_j = \sum_{i\in I_j} g_i](http://www.zhihu.com/equation?tex=G_j+%3D+%5Csum_%7Bi%5Cin+I_j%7D+g_i)与![H_j = \sum_{i\in I_j} h_i](http://www.zhihu.com/equation?tex=H_j+%3D+%5Csum_%7Bi%5Cin+I_j%7D+h_i)，上式简化为![{Obj}^{(t)} = \sum^T_{j=1} [G_jw_j + \frac{1}{2} (H_j+\lambda) w_j^2] +\gamma T](http://www.zhihu.com/equation?tex=%7BObj%7D%5E%7B%28t%29%7D+%3D+%5Csum%5ET_%7Bj%3D1%7D+%5BG_jw_j+%2B+%5Cfrac%7B1%7D%7B2%7D+%28H_j%2B%5Clambda%29+w_j%5E2%5D+%2B%5Cgamma+T)
由此，我们将目标函数转换为一个一元二次方程求最小值的问题（在此式中，变量为![w_j](http://www.zhihu.com/equation?tex=w_j)，函数本质上是关于![w_j](http://www.zhihu.com/equation?tex=w_j)的二次函数），略去求解步骤，最终结果如下所示：
![w_j^\ast = -\frac{G_j}{H_j+\lambda}](http://www.zhihu.com/equation?tex=w_j%5E%5Cast+%3D+-%5Cfrac%7BG_j%7D%7BH_j%2B%5Clambda%7D)，![\begin{split}\\{Obj}^\ast = -\frac{1}{2} \sum_{j=1}^T \frac{G_j^2}{H_j+\lambda} + \gamma T\end{split}](http://www.zhihu.com/equation?tex=%5Cbegin%7Bsplit%7D%5C%5C%0A%7BObj%7D%5E%5Cast+%3D+-%5Cfrac%7B1%7D%7B2%7D+%5Csum_%7Bj%3D1%7D%5ET+%5Cfrac%7BG_j%5E2%7D%7BH_j%2B%5Clambda%7D+%2B+%5Cgamma+T%0A%5Cend%7Bsplit%7D)
乍一看目标函数的计算与树的结构函数![q](http://www.zhihu.com/equation?tex=q)没有什么关系，但是如果我们仔细回看目标函数的构成，就会发现其中![G_j](http://www.zhihu.com/equation?tex=G_j)和![H_j](http://www.zhihu.com/equation?tex=H_j)的取值都是由第![j](http://www.zhihu.com/equation?tex=j)个树叶上数据样本所决定的。而第![j](http://www.zhihu.com/equation?tex=j)个树叶上所具有的数据样本则是由树结构函数![q](http://www.zhihu.com/equation?tex=q)决定的。也就是说，一旦树的结构![q](http://www.zhihu.com/equation?tex=q)确定，那么相应的目标函数就能够根据上式计算出来。那么树的生成问题也就转换为找到一个最优的树结构![q](http://www.zhihu.com/equation?tex=q)，使得它具有最小的目标函数。
> 计算求得的![Obj](http://www.zhihu.com/equation?tex=Obj)代表了当指定一个树的结构的时候，目标函数上面最多减少多少。所有我们可以把它叫做结构分数(structure score)。
![](https://pic1.zhimg.com/50/v2-376ce10a7aae9b7758cce58e2c5ee02a_hd.jpg)
上图为结构分数的一次实际应用，根据决策树的预测结果得到各样本的梯度数据，然后计算出实际的结构分数。正如图中所言，分数越小，代表树的结构越优。
4. 枚举树的结构——贪心法
在前面分析的基础上，当寻找到最优的树结构时，我们可以不断地枚举不同树的结构，利用这个打分函数来寻找出一个最优结构的树，加入到我们的模型中，然后再重复这样的操作。不过枚举所有树结构这个操作不太可行，在这里XGBoost采用了常用的贪心法，即每一次尝试去对已有的叶子加入一个分割。对于一个具体的分割方案，我们可以获得的增益可以由如下公式计算得到：
![Gain = \frac{1}{2} \left[\frac{G_L^2}{H_L+\lambda}+\frac{G_R^2}{H_R+\lambda}-\frac{(G_L+G_R)^2}{H_L+H_R+\lambda}\right] - \gamma](http://www.zhihu.com/equation?tex=Gain+%3D+%5Cfrac%7B1%7D%7B2%7D+%5Cleft%5B%5Cfrac%7BG_L%5E2%7D%7BH_L%2B%5Clambda%7D%2B%5Cfrac%7BG_R%5E2%7D%7BH_R%2B%5Clambda%7D-%5Cfrac%7B%28G_L%2BG_R%29%5E2%7D%7BH_L%2BH_R%2B%5Clambda%7D%5Cright%5D+-+%5Cgamma)
其中![\frac{G_L^2}{H_L+\lambda}](http://www.zhihu.com/equation?tex=%5Cfrac%7BG_L%5E2%7D%7BH_L%2B%5Clambda%7D)代表左子树分数，![\frac{G_R^2}{H_R+\lambda}](http://www.zhihu.com/equation?tex=%5Cfrac%7BG_R%5E2%7D%7BH_R%2B%5Clambda%7D)代表右子树分数，![\frac{(G_L+G_R)^2}{H_L+H_R+\lambda}](http://www.zhihu.com/equation?tex=%5Cfrac%7B%28G_L%2BG_R%29%5E2%7D%7BH_L%2BH_R%2B%5Clambda%7D)代表不分割时我们可以获得的分数，![\gamma](http://www.zhihu.com/equation?tex=%5Cgamma+)代表加入新叶子节点引入的复杂度代价。
对于每次扩展，我们还是要枚举所有可能的分割方案，那么如何高效地枚举所有的分割呢？假设需要枚举所有![x<a](http://www.zhihu.com/equation?tex=x%3Ca)这样的条件，那么对于某个特定的分割![a](http://www.zhihu.com/equation?tex=a)我们要计算![a](http://www.zhihu.com/equation?tex=a)左边和右边的导数和，在实际应用中如下图所示：
![](https://pic3.zhimg.com/50/v2-b1391820150b72cdca54b376f73c22b8_hd.jpg)
我们可以发现对于所有的![a](http://www.zhihu.com/equation?tex=a)，我们只要做一遍从左到右的扫描就可以枚举出所有分割的梯度与![G_L](http://www.zhihu.com/equation?tex=G_L)和![G_R](http://www.zhihu.com/equation?tex=G_R)。然后用上面的公式计算每个分割方案的分数就可以了。
> 但需要注意是：引入的分割不一定会使得情况变好，因为在引入分割的同时也引入新叶子的惩罚项。所以通常需要设定一个阈值，如果引入的分割带来的增益小于一个阀值的时候，我们可以剪掉这个分割。此外在XGBoost的具体实践中，通常会设置树的深度来控制树的复杂度，避免单个树过于复杂带来的过拟合问题。
到这里为止，XGBoost的数学推导就简要介绍完毕。在下一文中，我们将去了解该算法相对于GBDT的优异特性以及学习XGBoost的R实现。
本文是GBDT算法的第四篇，在完成XGBoost的基本介绍与数学推导后，接下来学习XGBoost区别于GBDT的一些独特之处以及算法的R实现。
## 一、XGBoost的优良特性
同样是梯度提升，同样是集成学习，那么XGBoost比GBDT要好在哪里呢？结合前面的推导过程与相关博客文章（见文末参考资料），可大致总结为以下几点：
- GBDT是以CART为基分类器，但XGBoost在此基础上还支持线性分类器，此时XGBoost相当于带![L_1](https://www.zhihu.com/equation?tex=L_1)和![L_2](https://www.zhihu.com/equation?tex=L_2)正则化项的Logistics回归（分类问题）或者线性回归（回归问题）
- XGBoost在目标函数里加入了正则项，用于控制模型的复杂度。正则项里包含了树的叶子节点个数和每棵树叶子节点上面输出分数的![L_2](https://www.zhihu.com/equation?tex=L_2)模平方。从偏差方差权衡的角度来讲，正则项降低了模型的variance，使学习出来的模型更加简单，防止过拟合
- 传统的GBDT在优化时只用到一阶导数，XGBoost则对目标函数进行了二阶泰勒展开，同时用到了一阶和二阶导数。（顺便提一下，XGBoost工具支持自定义代价函数，只要函数可一阶和二阶求导）
- 树节点在进行分裂时，我们需要计算每个特征的每个分割点对应的增益，即用贪心法枚举所有可能的分割点。当数据无法一次载入内存或者在分布式情况下，贪心算法效率就会变得很低，所以XGBoost采用了一种近似的算法。大致的思想是根据百分位法列举几个可能成为分割点的候选者，然后从候选者中根据上面求分割点的公式计算找出最佳的分割点
- Shrinkage（缩减），相当于学习速率（XGBoost中的eta）。XGBoost在进行完一次迭代后，会将叶子节点的权重乘上该系数，主要是为了削弱每棵树的影响，让后面有更大的学习空间。实际应用中，一般把eta设置得小一点，然后迭代次数设置得大一点。（当然普通的GBDT实现也有学习速率）
- 特征列排序后以块的形式存储在内存中，在迭代中可以重复使用；虽然boosting算法迭代必须串行，但是在处理每个特征列时可以做到并行
- 列抽样（column subsampling）：XGBoost借鉴了随机森林的做法，支持列抽样，不仅能降低过拟合，还能减少计算，这也是XGBoost异于传统GBDT的一个特性
- 除此之外，XGBoost还考虑了当数据量比较大，内存不够时怎么有效的使用磁盘，主要是结合多线程、数据压缩、分片的方法，尽可能的提高算法效率
## 二、xgboost包安装与数据准备
在R中，xgboost包用于算法的实现，首先进行安装
```
# xgboost包在安装时需要把R升级到3.3.0以上的版本,否则安装不成功
> install.packages('xgboost')
# 也可使用devtools包安装github版本
> devtools::install_github('dmlc/xgboost', subdir='R-package')
> library(xgboost)
```
在包中有一组蘑菇数据集可供使用，我们的目标是预测蘑菇是否可以食用（分类任务），此数据集已被分割成训练数据与测试数据。
```
> data(agaricus.train, package='xgboost')
> data(agaricus.test, package='xgboost')
> train <- agaricus.train 
> test <- agaricus.test
```
```
# 整个数据集是由data和label组成的list
> class(train)
[1] "list"
# 查看数据维度
> dim(train$data)
[1] 6513  126
> dim(test$data)
[1] 1611  126
```
```
# 在此数据集中，data是一个dgCMatrix类的稀疏矩阵,label是一个由{0,1}构成的数值型向量
> str(train)
List of 2
 $ data :Formal class 'dgCMatrix' [package "Matrix"] with 6 slots
  .. ..@ i       : int [1:143286] 2 6 8 11 18 20 21 24 28 32 ...
  .. ..@ p       : int [1:127] 0 369 372 3306 5845 6489 6513 8380 8384 10991 ...
  .. ..@ Dim     : int [1:2] 6513 126
  .. ..@ Dimnames:List of 2
  .. .. ..$ : NULL
  .. .. ..$ : chr [1:126] "cap-shape=bell" "cap-shape=conical" "cap-shape=convex" "cap-shape=flat" ...
  .. ..@ x       : num [1:143286] 1 1 1 1 1 1 1 1 1 1 ...
  .. ..@ factors : list()
 $ label: num [1:6513] 1 0 0 1 0 0 0 1 0 0 ...
```
## 三、构建模型和预测实现
xgboost包提供了两个函数用于模型构建，分别是xgboost()与xgb.train()，前者可以满足对算法参数的基本设置，而后者的话在此基础上可以实现一些更为高级的功能。
```
# data与label分别指定数据与标签
# max.deph：树的深度,默认值为6,在此数据集中的分类问题比较简单，设置为2即可
# nthread：并行运算的CPU的线程数,设置为2;
# nround：生成树的棵数
# objective = "binary:logistic"：设置逻辑回归二分类模型
> xgboost_model <- xgboost(data = train$data, label = train$label, max.depth = 2, eta = 1, nthread = 2, nround = 2, objective = "binary:logistic")
# 得到两次迭代的训练误差
[1]	train-error:0.046522 
[2]	train-error:0.022263
```
xgboost函数可调用的参数众多，在此不在详细展开介绍，可参阅博客文章[[译]快速上手：在R中使用XGBoost算法](https://link.zhihu.com/?target=https%3A//segmentfault.com/a/1190000004421821%23articleHeader7)中的"在xgboost中使用参数"一节，该文章将这些参数归为通用、辅助和任务参数三大类，对我们掌握算法与调参有着很大帮助。
```
# 设置verbose参数,可以显示内部的学习过程
> xgboost_model <- xgboost(data = train$data, label = train$label, 
+ max.depth = 2, eta = 1, nthread = 2, nround = 2, verbose = 2,
+ objective = "binary:logistic")
[13:56:36] amalgamation/../src/tree/updater_prune.cc:74: tree pruning end, 1 roots, 6 extra nodes, 0 pruned nodes, max_depth=2
[1]	train-error:0.046522 
[13:56:36] amalgamation/../src/tree/updater_prune.cc:74: tree pruning end, 1 roots, 4 extra nodes, 0 pruned nodes, max_depth=2
[2]	train-error:0.022263
```
```
# 将建立好的模型用于预测新的数据集
> xgboost_pred <- predict(xgboost_model, test$data)
> head(xgboost_pred)
[1] 0.28583017 0.92392391 0.28583017 0.28583017 0.05169873 0.92392391
# 以上给出的是每一个样本的预测概率值,进一步转化后可得到具体的预测分类
> prediction <- as.numeric(xgboost_pred > 0.5)
> head(prediction)
[1] 0 1 0 0 0 1
> model_accuracy <- table(prediction,test$label)
> model_accuracy
          
prediction   0   1
         0 813  13
         1  22 763
> model_accuracy_1 <- sum(diag(model_accuracy))/sum(model_accuracy)
> model_accuracy_1
[1] 0.9782744
```
## 四、XGBoost的高级功能
xgb.train()函数可以实现一些高级功能，帮助我们对模型进行进一步的优化。
```
# 在使用函数前需要将数据集进行转换为xgb.Dmatrix格式
> dtrain <- xgb.DMatrix(data = train$data, label=train$label)
> dtest <- xgb.DMatrix(data = test$data, label=test$label)
```
```
# 使用watchlist参数,可同时得到训练数据与测试数据的误差
> watchlist <- list(train=dtrain, test=dtest)
> xgboost_model <- xgb.train(data=dtrain, max.depth=2, eta=1, nthread = 2,
+ nround = 3,objective = "binary:logistic",watchlist = watchlist)
[1]	train-error:0.046522	test-error:0.042831 
[2]	train-error:0.022263	test-error:0.021726
[3]	train-error:0.007063	test-error:0.006207
```
```
# 自定义损失函数,可同时观察两种损失函数的表现
# eval.metric可使用的参数包括'logloss'、'error'、'rmse'等
> xgboost_model <- xgb.train(data=dtrain, max.depth=2, eta=1, nthread = 2,
+ nround=3, watchlist=watchlist, eval.metric = "error", 
+ eval.metric = "logloss", objective = "binary:logistic")
[1]	train-error:0.046522	train-logloss:0.233376	test-error:0.042831	test-logloss:0.226686 
[2]	train-error:0.022263	train-logloss:0.136658	test-error:0.021726	test-logloss:0.137874 
[3]	train-error:0.007063	train-logloss:0.082531	test-error:0.006207	test-logloss:0.080461
```
```
# 查看特征的重要性,方便我们在模型优化时进行特征筛选
> importance_matrix <- xgb.importance(model = xgboost_model)
> importance_matrix
   Feature       Gain      Cover Frequency
1:      28 0.60036585 0.41841659     0.250
2:      55 0.15214681 0.16140352     0.125
3:      59 0.10936624 0.13772146     0.125
4:     101 0.04843973 0.07979724     0.125
5:     110 0.03391602 0.04120512     0.125
6:      66 0.02973248 0.03859211     0.125
7:     108 0.02603288 0.12286396     0.125
# 使用xgb.plot.importance()函数进行可视化展示
> xgb.plot.importance(importance_matrix)
```
```
# 使用xgb.dump()查看模型的树结构
> xgb.dump(xgboost_model,with_stats = T)
 [1] "booster[0]"                                                            
 [2] "0:[f28<-9.53674e-007] yes=1,no=2,missing=1,gain=4000.53,cover=1628.25" 
 [3] "1:[f55<-9.53674e-007] yes=3,no=4,missing=3,gain=1158.21,cover=924.5"   
 [4] "3:leaf=0.513653,cover=812"                                             
 [5] "4:leaf=-0.510132,cover=112.5"                                          
 [6] "2:[f108<-9.53674e-007] yes=5,no=6,missing=5,gain=198.174,cover=703.75" 
 [7] "5:leaf=-0.582213,cover=690.5"                                          
 [8] "6:leaf=0.557895,cover=13.25"
 ---
# 将上述结果通过树形结构图表达出来 
> xgb.plot.tree(model = xgboost_model)
```
![](https://pic2.zhimg.com/50/v2-a206ffb6e7691792934b75be4a2c67ee_hd.jpg)
至此，XGBoost算法及其R实现就简单介绍到这里。虽然貌似讲了好多，但我们所学的不过是一些皮毛而已，无论是XGBoost本身所具有的优良性能、通过复杂的调参对不同任务的实现支持，还是在实际应用中的高精度预测，这些优势都将意味着XGBoost算法有着巨大的潜力空间，值得我们一直探索下去。
