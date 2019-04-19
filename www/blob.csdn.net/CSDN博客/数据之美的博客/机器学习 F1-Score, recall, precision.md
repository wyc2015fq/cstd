# 机器学习 F1-Score, recall, precision - 数据之美的博客 - CSDN博客
2018年01月26日 19:54:32[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：205
在机器学习，模式识别中，我们做分类的时候，会用到一些指标来评判算法的优劣，最常用的就是识别率，简单来说，就是 
这里的 表示预测对的样本数，表示测试集总的样本数。
识别率有的时候过于简单， 不能全面反应算法的性能，除了识别率，还有一些常用的指标，就是我们要介绍的 
F1-score, recall, precision.
在介绍这些概念之前，我们先来看一个二分类的问题，给定一组训练集： 
这里假定  对应正样本， 对应负样本。假设我们建立了一个分类模型 ,
 对每一个输入的样本  会输出一个预测值 ,
 那么将预测值  与样本对应的实际值做比较，会得到以下四种情况：
第一种情况，预测为正，实际也为正，我们称为 true positive (TP)，第二种情况，预测为正，实际为负，我们称为 false positive (FP)，第三种情况，预测为负，实际为正，称为false negative (FN)，最后一种情况，预测为负，实际也为负，称为 true negative (TN)，每一个样本只可能属于这四种情况中的某一种，不会有其它的可能。 
很显然，给定一个测试集，我们可以得到如下的关系： 
如果我们定义一个测试集中，正样本个数为,
 负样本个数为,
 那么我们可以知道：, 
所以，我们常用的识别率  其实就等于 
， 
进一步，我们可以定义 recall ，precision， F1-score 如下所示： 

可以看到，recall 体现了分类模型对正样本的识别能力，recall
 越高，说明模型对正样本的识别能力越强，precision 体现了模型对负样本的区分能力，precision越高，说明模型对负样本的区分能力越强。F1-score 是两者的综合。F1-score 越高，说明分类模型越稳健。
比如我们常见的雷达预警系统，我们需要对雷达信号进行分析，判断这个信号是飞行器（正样本）还是噪声 （负样本）, 很显然，我们希望系统既能准确的捕捉到飞行器信号，也可以有效地区分噪声信号。所以就要同时权衡recall 和 precision这两个指标，如果我们把所有信号都判断为飞行器，那 recall 可以达到1，但是precision将会变得很低（假设两种信号的样本数接近），可能就在 0.5 左右，那F1-score 也不会很高。
有的时候，我们对recall 与 precision 赋予不同的权重，表示对分类模型的偏好：
可以看到，当 ，那么就退回到了， 其实反映了模型分类能力的偏好， 的时候，precision的权重更大，为了提高，我们希望precision
 越小，而recall 应该越大，说明模型更偏好于提升recall，意味着模型更看重对正样本的识别能力； 而 的时候，recall
 的权重更大，因此，我们希望recall越小，而precision越大，模型更偏好于提升precision，意味着模型更看重对负样本的区分能力。
> 
参考来源： 
[https://en.wikipedia.org/wiki/Receiver_operating_characteristic](https://en.wikipedia.org/wiki/Receiver_operating_characteristic)
[https://en.wikipedia.org/wiki/F1_score](https://en.wikipedia.org/wiki/F1_score)
