# 如何在Kaggle比赛上击败98%的对手？七步秘笈帮你搞定特征 - 知乎
# 



> 栗子 晓查 整理编译
量子位 报道 | 公众号 QbitAI

怎样才能在Kaggle举办的各式各样的比赛里，拿到骄人的成绩？



![](https://pic2.zhimg.com/v2-de9f412d596ca88200c9370d0d604a71_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='566' height='178'></svg>)



一位叫做Abhay Pawar的少年，在2600多支队伍参加的*Instacart Market Basket Analysis*比赛上，击败了**98%**的对手。

这是一个预测用户**未来将购买何种商品**的比赛，依据的是过往交易数据。



![](https://pic3.zhimg.com/v2-e96231ee9fdd0dcb6f81f780acc73412_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='720'></svg>)



少年对参加比赛充满了心得，说最重要的就是要充分**理解特征**。

于是，他写了一套七步秘笈出来，和小伙伴们分享自己探索特征的经验。

这份秘笈，封装在一个名叫**featexp**的Python包里，传送门在文底。

举栗用的**数据集**，是来自Kaggle Home Credit Default Risk (违约者预测) 比赛。

现在，了解一下具体步骤吧。

## **支配特征，七步大法**

少年说，仔细观察一个模型的**部分依赖图** (Partial Dependency Plot，PDP) ， 可以帮助大家理解**output是如何随着特征而变化**的。



![](https://pic4.zhimg.com/v2-b8240b2bed620b91adeaf100f35fdc43_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)



问题是，这种图像是用训练好的模型做的。而如果直接用**训练数据**来做图，就可以让大家更好地了解underlying data。

## **1. 理解特征**

如果**因变量** (目标) 是二进制，散点图就不好用，因为所有点不是0就是1。



![](https://pic3.zhimg.com/v2-9a512f61e4d57334ea0b193a71349506_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='390' height='267'></svg>)



针对连续目标，数据点太多的话，会让人很难理解**目标**和**特征**之间的关系。

用featexp可以做出更加友善的图像。

拿这段代码试一下吧：


```python
from featexp import get_univariate_plots

# Plots drawn for all features if nothing is passed in feature_list parameter.

get_univariate_plots(data=data_train, target_col='target', features_list=['DAYS_BIRTH'], bins=)
```


Featexp可以把一个数字特征，分成很多个**样本数相等**的区间 (Equal Population Bins) 。


然后，计算出目标的平均值 (Mean) ，做出一个这样的图像：



![](https://pic3.zhimg.com/v2-1ab88d699f389b9859aac89f9019fd9a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='777' height='659'></svg>)



在这里，平均值代表违约率。图像告诉我们，**年纪** (DAYS_BIRTH) 越大的人，违约率越低。

嗯，如果年轻人不太在意违约，也比较符合常理。

还有另外一个图像，表示每个区间的人数 (分的时候就是相等的) ：



![](https://pic2.zhimg.com/v2-e7ebfdc885d99f25fab726092e497f79_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='823' height='676'></svg>)



不过，这个栗子是比较友好的特征。少年说了，下一个章节才是最有趣的。

## **2. 识别嘈杂特征**

嘈杂特征容易造成过拟合，识别这样的特征一点也不容易。

在featexp里面，可以跑一下测试集，然后对比验证集和测试集的特征趋势，来找出嘈杂的特征。

这是对比用的代码：


```python
get_univariate_plots(data=data_train, target_col='target', data_test=data_test, features_list=['DAYS_EMPLOYED'])
```


做出的图像长这样：




![](https://pic4.zhimg.com/v2-2aec32e0938d8c8c879ffc201dfbdd27_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1001'></svg>)



为了衡量嘈杂程度，featexp会计算**两个指标**：


**一是**趋势相关性 (Trend Correlation) ：如果一个特征在训练集和测试集里面，表现出来的趋势不一样，就可能导致过拟合。这是因为，模型从测试集里学到的一些东西，在验证集里不适用。

趋势相关性可以告诉我们，训练集和测试集趋势的相似度，以及每个区间的平均值，以及这些数值要怎么用。

上面这个特征，两个集子的相关性达到99%。

很好，一点也不嘈杂的样子。

**二是**趋势变化 (Trend Changes) ：有时候，趋势会发生**突变** (Sudden Changes) 和**反复变化** (Repeated Changes) 。

这可能就是很嘈杂的表现了，**但**也有可能是那个区间，因为其他特征的表现非常与众不同，而受到影响了。

如果出现这种情况，这个区间的违约率就没办法和其他区间直接相比了。

下面这个特征，就是嘈杂特征，相关性只有**85%**。**有时候**，可以选择丢掉这个特征。



![](https://pic4.zhimg.com/v2-f49f59c7eb6598ff35f1157c232bb513_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='983'></svg>)



抛弃相关性低的特征，这种做法在特征非常多、特征之间又很相关的情况下，比较适用。


它可以减少过拟合，避免信息丢失。不过：

**注意**，别把太多重要的特征都丢掉了。否则，模型的预测效果可能就有损失了。

**注意x2**，也不能用重要性来评价特征是否嘈杂，因为有些特征就是可以既重要，又嘈杂得不得了。

用 (与训练集) 不同时间段的数据来做测试集，可能会比较好。这样，就能看出来数据是不是随时间变化的了。

请看下面的示范。

Featexp里有一个 get_trend_stats() 函数，可以返回一个数据框 (Dataframe) ，显示**趋势相关性**和**趋势变化**。代码如下：


```python
from featexp import get_trend_stats
stats = get_trend_stats(data=data_train, target_col='target', data_test=data_test)
```




![](https://pic3.zhimg.com/v2-f38170f3ec99c1955d431a3f1fbcf01a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='556'></svg>)



现在，可以试着丢弃一些特征了。不妨参考这些标准来执行：



![](https://pic3.zhimg.com/v2-c70adf6887369603d1bcd2463a1184ae_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1062' height='300'></svg>)△ AUC=Area Under Curve，曲线下的面积，是评价模型的指标
丢弃特征的相关性阈值越高，排行榜上的AUC越高。

加上不要丢弃重要特征这一条规则，AUC提升到了0.74。

有趣的是，测试集的AUC并没有像排行榜的AUC变化那么大。

完整代码可以在featexp_demo记事本里面找到：

[abhayspawar/featexp​github.com![图标](https://pic1.zhimg.com/v2-4755c8412a63fa56d2158367fb62ee24_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/abhayspawar/featexp/blob/master/featexp_demo.ipynb)
## **3. 特征工程**

通过查看这些图表获得的见解有助于创建更好的特征。

只需更好地了解数据就可以实现更好的特征工程。 除此之外，它还可以帮助你改进现有特征。下面来看另一个功能EXT_SOURCE_1：



![](https://pic2.zhimg.com/v2-e3f01a773015035c7be1f76ed3d95e11_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='493'></svg>)△ EXT_SOURCE_1的特征与目标图
具有较高EXT_SOURCE_1值的客户违约率较低。 但是，第一个区间（违约率约8％）不遵循这个特征趋势（上升然后下降）。 它只有-99.985左右的负值且人群数量较多。这可能意味着这些是特殊值，因此不遵循特征趋势。

幸运的是，非线性模型在学习这种关系时不会有问题。 但是，对于像逻辑回归这样的线性模型，这些特殊值和空值应该用来自具有相似违约率的区间的值来估算，而不是简单地用特征均值。

## **4. 特征重要性**

Featexp还可以帮助衡量特征的重要性。 DAYS_BIRTH和EXT_SOURCE_1都有很好的趋势。 但是，EXT_SOURCE_1的人群集中在特殊值区间中，这表明它可能不如DAYS_BIRTH那么重要。 基于XGBoost模型的特征重要性，DAYS_BIRTH实际上比EXT_SOURCE_1更重要。

## **5. 特征调试**

查看Featexp的图表，可以帮助你通过以下两项操作来发现复杂特征工程代码中的错误：



![](https://pic1.zhimg.com/v2-ff0a3c33600b7d13762f534f2ca86564_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='462'></svg>)



1）检查特征的人群分布是否正确。 由于一些小错误，作者遇到过多次极端情况。


2）在查看这些图之前，总是假设特征趋势会是什么样子。 特征趋势看起来不符合预期，可能暗示着某些问题。 坦率地说，这个假设趋势的过程使ML模型更有趣！

## **6. 泄漏检测**

从目标到特征的数据泄漏导致过度拟合。 泄露的特征具有很高的功能重要性。 但是，要理解为什么在特征中会发生泄漏，这很是困难的。 查看featexp图可以帮助理解这一问题。

以下特征在“Nulls”区间中的违约率为0％，在其他所有区间中的违约率为100％。 显然，这是泄漏的极端情况。 只有当客户违约时，此特征才有价值。 **了解泄漏特征的问题所在能让你更快地debug。**



![](https://pic1.zhimg.com/v2-7f4f2282db66a30f38e572ab1b033e80_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='490'></svg>)



## **7. 模型监控**

由于featexp可计算两个数据集之间的趋势相关性，因此它可以很容易地用于模型监控。 每次重新训练模型时，都可以将新的训练数据与测试好的训练数据（通常是第一次构建模型时的训练数据）进行比较。 趋势相关性可以帮助您监控特征信息是否与目标的关系发生了变化。

## **featexp项目**

作者Abhay Pawar在文中大篇幅使用了featexp，这是一个用于监督学习的特征探索项目，本文也是主要针对此工具得使用展开讨论。它对前面提到的特征理解、识别带噪声的特征、特征除错、泄露探测和模型监控都有帮助。

它的安装也很简单，可以通过pip直接安装：


```bash
pip install featexp
```


详细说明可参阅GitHub项目页：
[abhayspawar/featexp​github.com![图标](https://pic1.zhimg.com/v2-4755c8412a63fa56d2158367fb62ee24_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/abhayspawar/featexp)
秘笈原文传送门：
[https://towardsdatascience.com/my-secret-sauce-to-be-in-top-2-of-a-kaggle-competition-57cff0677d3c​towardsdatascience.com](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/my-secret-sauce-to-be-in-top-2-of-a-kaggle-competition-57cff0677d3c)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


