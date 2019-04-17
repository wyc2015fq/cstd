# 推荐系统：NDCG评价指标 - Machine Learning with Peppa - CSDN博客





2018年07月07日 14:04:01[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1657








## **排名指标**

两个最受欢迎的[排名](https://en.wikipedia.org/wiki/Learning_to_rank)指标是MAP和NDCG。我们在前段时间已经使用了[平均精度均值](http://fastml.com/what-you-wanted-to-know-about-mean-average-precision/)（MAP）。NDCG表示[归一化折损累积增益](https://en.wikipedia.org/wiki/Discounted_cumulative_gain)。两者之间的主要区别是，MAP认为是二元相关性（一个项是感兴趣的或者不感兴趣的），而NDCG允许以实数形式进行相关性打分。这种关系类似分类和回归的关系。

实际当中，很难直接地优化MAP或NDCG指标，因为他们是不连续的，所以不可微。幸运的是，[排名学习中的排名指标和损失函数](http://papers.nips.cc/paper/3708-ranking-measures-and-loss-functions-in-learning-to-rank)表明，用于排名学习的一对损失函数近似于这些指标。Normalized Discounted Cumulative Gain，即NDCG，常用于作为对rank的评价指标，当我们通过模型得出某些元素的ranking的时候，便可以通过NDCG来测评这个rank的准确度，同样的算法还包括MAP,MRR等，这里只讲解NDCG。



## **NDCG**

NDCG这个名字可能有点吓人，但其背后的思想却很简单。一个推荐系统返回一些项并形成一个列表，我们想要计算这个列表有多好。每一项都有一个相关的评分值，通常这些评分值是一个非负数。这就是*gain*（增益）*。*此外，对于这些没有用户反馈的项，我们通常设置其增益为0。

现在，我们把这些分数相加，也就是*Cumulative Gain*（累积增益）。我们更愿意看那些位于列表前面的最相关的项，因此，在把这些分数相加之前，我们将每项除以一个递增的数（通常是该项位置的对数值），也就是折损值，并得到DCG。

在用户与用户之间，DCGs没有直接的可比性，所以我们要对它们进行归一化处理。最糟糕的情况是，当使用非负相关评分时DCG为0。为了得到最好的，我们把测试集中所有的条目置放在理想的次序下，采取的是前K项并计算它们的DCG。然后将原DCG除以理想状态下的DCG并得到NDCG@K，它是一个0到1之间的数。

你可能已经注意到，我们使用*K*表示推荐列表的长度。这个数由专业人员指定。你可以把它想像成是一个用户可能会注意到的多少个项的一个估计值，如10或50这些比较常见的值。

这里有一些[计算NDCG的Python代码](https://gist.github.com/bwhite/3726239)，非常简单。

要注意到，我们实验的测试集由训练集以外的所有项组成，包括那些没有用户排名的项（与上面RMSE讨论中提到的一样）。有时人们会对用户留存率设置测试限制，所以推荐系统的任务是减少调整那些相对较少的项。在实际情景当中并不如此。

现在，它的要点是，还有[另一种DCG表述](https://en.wikipedia.org/wiki/Discounted_cumulative_gain#Discounted_Cumulative_Gain)。你还可以使用负相关分数。在这种情况下，你可以计算出更糟糕情况下DCG的归一化（它将小于零），或者仍然使用零作为下限值，具体要视情况而定。






## 指标用例

我们通过一个实例来说明一下这个NDCG的用法
|id|1|2|3|4|5|
|----|----|----|----|----|----|
|模型评分|1.2|0.7|0.1|0.2|4.0|
|按模型评分排序（i）|2|3|5|4|1|
|r(l)|2|1|0|0|0|
|按Gain排序（j）|1|2|3|3|3|
我们首先通过i来计算出DCG值为2.39278，按照j来计算出iDCG的值为3,6309
最后通过公式1计算出NDCG为0.65






