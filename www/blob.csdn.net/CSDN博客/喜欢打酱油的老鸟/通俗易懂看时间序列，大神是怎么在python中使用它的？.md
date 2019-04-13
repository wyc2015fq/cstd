
# 通俗易懂看时间序列，大神是怎么在python中使用它的？ - 喜欢打酱油的老鸟 - CSDN博客


2018年08月10日 07:54:38[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：893


[http://blog.itpub.net/31509949/viewspace-2199388/](http://blog.itpub.net/31509949/viewspace-2199388/)
作者：Arnaud Zinflou
时间序列是日常生活中遇到的最常见的数据类型之一。股票价格、销售数据、气候数据、能源使用、甚至个人体重都是可以定期收集的数据。几乎每个数据科学家都会在工作中遇到时间序列，能够有效地处理这些数据是数据科学工具箱中的一项重要技能。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/1533117284401ff8c5ec9fb)
本文简要介绍了如何在python中使用时间序列。这包括使用Pandas访问伦敦家庭智能电表能耗数据的时间序列和一些数据操作。可以在此处检索此帖子中使用的数据。包含了我认为可能有用的代码。（https://data.london.gov.uk/dataset/smartmeter-energy-use-data-in-london-households）
让我们从基础知识开始看起，看看时间序列的定义：
时间序列是按时间顺序索引，列出或绘制的数据点的集合。通常，时间序列是在连续的等间隔时间点采取的序列。因此，它是离散时间数据的序列。
时间序列数据围绕相对确定的时间戳来组织的，因此，与随机样本相比，可能包含我们将尝试提取的其他信息。
加载和处理时间序列
数据集
举例来说，让我们用千瓦时（每半小时）的能耗读数数据为例，在2011年11月至2014年2月期间，我们从参与了英国电网公司领导的"低碳伦敦"项目的伦敦家庭提取样本。我们可以制作一些探索图，最好能够对结构和范围有概念，这也将使我们能够寻找到需要纠正的最终缺失值。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117286363220377bb68)
对于本文的其余部分，我们将只关注DateTime和kWhcolumns。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/1533117289234e5486c9ac2)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/153311729049158978f50ee)
重采样
让我们从简单的重采样技术开始。重采样涉及更改时间序列观察的频率。你可能对重采样时间序列数据感兴趣的一个原因是特征工程。实际上，它可以用于为监督学习模型提供额外的结构或指出洞察学习问题。pandas中的重采样方法类似于groupby方法，因为你实际上是按特定时间跨度来进行分组。然后，你可以指定重采样的方法。让我们通过查看一些例子使重采样更加具体化。 我们将从每周总结开始：
data.resample（）将用于重新对我们的DataFrame的kWh列进行采样
"W"表示我们希望按周重采样。
sum（）用于表示我们希望在此期间使用的总和为kWh。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/1533117290693542c457378)
我们可以在每日总结中做同样的事情，我们可以使用groupby和mean函数进行每小时总结：
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/1533117290831cc652efc62)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/153311729097488b6912756)
为了进一步重采样，pandas附带了许多内置选项，你甚至可以定义自己的方法。以下两个表分别提供了表周期选项以及可能用于重采样的一些常用方法。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/15331172912124ba195d070)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p9.pstatp.com/large/pgc-image/153311728268937e6399238)
其他探索
以下是你可以对数据进行的一些探索：
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p9.pstatp.com/large/pgc-image/15331172828735de01b16ed)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117283020dfe0a5ace2)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/153311728325124d8c5eebd)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/1533117283394084ad8ad83)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/15331172835820238788260)
建模与prophet框架
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117283693003edabeac)
Facebook Prophet于2017年发布，它可用于Python和R. Prophet设计用于分析时间序列，每天的观察显示不同时间尺度上的模式。Prophet非常善于处理缺失的数据和对于趋势的变化，并且通常能很好地处理异常值。它还具有用于假期对时间序列的影响进行建模和实现自定义变更点的高级功能，但我将坚持使用基础知识建立并运行模型。我认为Prophet是做出快速预测的一个很好的选择，因为它具有直观的参数，可以由具有良好领域知识但缺乏预测模型技术技能的人对其进行调整。有关Prophet的更多信息，可以参考此处的官方文档。（https://facebook.github.io/prophet/docs/quick_start.html）
在使用Prophet之前，我们将数据中的列重命名为正确的格式。 Date列必须被称为'ds'，而值列我们希望预测'y'。我们在下面的示例中使用了每日总结数据。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117283846b39e030772)
然后我们导入Prophet，创建一个模型并适合数据。在Prophet中，changepoint_prior_scale（https://facebook.github.io/prophet/docs/trend_changepoints.html)参数用于控制趋势对变化的敏感程度，较高的值更敏感，较低的值不敏感。在尝试了一系列值后，我将此参数从默认值0.05设置为0.10。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p9.pstatp.com/large/pgc-image/1533117283977ae85bec270)
为了进行预测，我们需要创建所谓的未来数据帧。我们指定预测的未来周期（在我们的例子中为两个月）和预测的频率（每日）。 然后，我们使用我们创建的Prophet模型和未来的数据框进行预测。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117284145e3ac28ba0d)
这很简单！未来的数据框包含未来两个月的估计家庭消费。我们可以用一个图来可视化预测：
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117284533cdb5053188)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117284734ba7434e8b9)
黑点表示实际值，蓝线表示预测值，浅蓝色阴影区域表示不确定性。
如下图所示，随着我们未来的进一步发展，不确定区域也会增长，因为最初的不确定性会随着时间的推移而传播和增长。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/15331172849733767fea671)
Prophet也让我们轻松地可视化整体趋势和组件模式：
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117285171b5fb7272ff)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/15331172853766c9d78e15f)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/153311728551615905243d3)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/153311728568512508a014e)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/153311728587052be8309eb)
每年的模式都很有意思，因为它似乎表明秋季和冬季家庭消费增加，春季和夏季减少。直观地说，这正是我们期望看到的。从周趋势来看，周日的消费似乎比一周中的其他日子多。最后，总体趋势表明，消费量在缓慢下降之前增加了一年。试图解释这一趋势需要进一步调查。在下一篇文章中，我们将尝试查找它是否与天气有关。
LSTM预测
长期短期记忆复发神经网络有望学习长时间的观察序列。这篇题为"理解LSTM网络"的文章在以易于理解的方式解释底层复杂性方面做得非常出色。（http://colah.github.io/posts/2015-08-Understanding-LSTMs/）下图是描述LSTM内部单元架构的图像。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/15331172860070752103586)
LSTM似乎非常适合时间序列预测。让我们再次使用我们的每日总结数据。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/153311728622582afcccc44)
LSTM对输入数据的大小很敏感，特别是在使用sigmoid或tanh激活函数时。将数据重新调整到[0,1]或[-1,1]的范围通常是一种很好的做法，也称为规范化。我们可以使用scikit-learn库中的MinMaxScaler预处理类轻松地规范化数据集。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/15331172865863be3663723)
现在我们可以将有序数据集拆分为训练和测试数据集。下面的代码计算了分割点的索引，并将数据分成训练数据集，其中80％的观测值可用于训练我们的模型，剩下的20％用于测试模型。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/1533117286816e8303667d7)
我们可以定义一个函数来创建一个新的数据集，并使用该函数来准备建模的训练和测试数据集。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/15331172870689cc5dcef9e)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117287210f1f39989cc)
LSTM网络期望输入数据以以下形式提供特定的阵列结构：[样本，时间步骤，特性]。
我们的数据目前采用[样本，特性]的形式，我们将问题定为每个样本的两个时间步骤。我们可以将准备好的训练和测试输入数据转换为预期的结构，如下所示：
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117287347f66c0e97ba)
就这样！我们现在已经准备好为我们的示例设计和调整我们的LSTM网络。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/153311728756194d5d992fa)
从损失图中，我们可以看到该模型在训练和测试数据集上都具有类似的性能。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/153311728770344661e2efa)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p9.pstatp.com/large/pgc-image/15331172878490afc4811c1)
在下图中，我们看到LSTM在拟合测试数据集方面做得非常好。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/1533117288372800efffd14)
聚类
最后但同样重要的是，我们还可以使用我们的示例数据进行聚类。有许多不同的方法来执行集群，其中一种方法是分层地形成集群。你可以通过两种方式形成层次结构：从顶部开始拆分，或从底部开始合并。我决定在这篇文章中使用后者。
让我们从数据开始，我们只需导入原始数据，并为一年中的某一天和一天中的小时添加两列。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117288708276dcfd190)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/1533117289404ee76482fad)
Linkage函数和树状图
Linkage函数根据物体的相似性和距离信息将物体分组。这些新形成的集群彼此链接以创建更大的集群。这个过程被迭代，直到原始数据集中的所有对象链接到一个分层树中。
对我们的数据进行聚类：
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/153311728951665e31b23a6)
完成了！但"ward"是什么意思？这实际上是如何运作的？正如scipy linkage文档所告诉我们的那样，ward是可用于计算新形成的集群之间距离的方法之一。"ward"链接功能是Ward方差最小化算法。
现在让我们来看看这种分层聚类的树形图。树形图是聚类的分层图，其中树形图的长度表示到下一个聚类中心的距离。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/15331172896863d3d341491)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p9.pstatp.com/large/pgc-image/1533117289889a4967d321c)
如果这是你第一次看到树状图，那看起来很吓人，但不要担心，让我们把它分开：
· 在x轴上，你可以看到标签。如果你没有指定任何其他东西（比如我），它们就是X中样本的索引。
· 在y轴上，你可以看到距离（在我们的例子中是word算法）。
· 水平线是集群合并
· 垂直线告诉你哪些集群/标签是合并，形成新集群的一部分
· 水平线的高度告诉你新形成的集群所需要的距离
即使有解释，先前的树状图仍然不明显。我们可以"削减"一点，以便能够更好地查看数据。
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p1.pstatp.com/large/pgc-image/153311728998590186d41a9)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/153311729015572772936b2)
![通俗易懂看时间序列，大神是怎么在python中使用它的？](http://p3.pstatp.com/large/pgc-image/15331172903117687f19113)
好多了，不是吗？ 查看聚类文档以了解更多信息并使用不同的参数。（http://scikit-learn.org/stable/modules/generated/sklearn.cluster.AgglomerativeClustering.html）
参考文献和进一步阅读：
· https://joernhees.de/blog/2015/08/26/scipy-hierarchical-clustering-and-dendrogram-tutorial/\#Selecting-a-Distance-Cut-Off-aka-Determining-the-Number-of-Clusters
· https://medium.com/open-machine-learning-course/open-machine-learning-course-topic-9-time-series-analysis-in-python-a270cb05e0b3
· https://petolau.github.io/TSrepr-clustering-time-series-representations/
· https://www.analyticsvidhya.com/blog/2016/02/time-series-forecasting-codes-python/
· http://colah.github.io/posts/2015-08-Understanding-LSTMs/
· http://docs.scipy.org/doc/scipy/reference/cluster.hierarchy.html
· https://facebook.github.io/prophet/docs/quick_start.html

