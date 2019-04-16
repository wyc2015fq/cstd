# DDM（Drift Detection Method） - 一个跳popping的quant的博客 - CSDN博客





2018年08月24日 12:23:37[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1089








### Introduction

很多机器学习模型都是基于**stationary probability distribution**（稳定概率分布）数据样本的，随着样本数据的概率分布的改变，模型的准确率也会随之下降，DDM就是这样一种检测方法，用于检测样本数据的**概率分布的改变**。DDM也是作为concept drift领域一个base line，很多concept drift detector都是以次基础上作为改进的。

### DDM

DDM的思想也很简单，就是**control the online error-rate of the algorithm**（控制算法的在线错误率）。如果样本数据是稳定分布的，那么随着数据的输入，模型的错误率就会逐渐下降；当概率分布发生变化时，模型的错误率就会上升。所以DDM就是在线控制模型训练过程中的错误率。

DDM会为错误率设置两个阀值，一个是warning，另一个是drift。当样本数据中的第w个数据输入时，错误率达到了warning值，说明有样本概率分布改变的前兆，如果接连输入的数据没有让错误率降低，并且当第d个数据输入时错误率达到了drift值，则确定样本概率分布发生了变化，为了适应新的样本数据，模型就将以w之后的数据进行学习；而如果接连输入的数据让错误率降低了，则说明是一个false alarm。

![](https://img-blog.csdn.net/20180824092837762?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

关于warning和drift这两个阀值的大小，是通过错误率的概率分布来确定的。假设输入的样本序列是这样的（xi, yi），xi是一个多维的特征，yi是真实值，模型对xi的预测值为yi^，通过比较yi^和yi就可以确定第i个样本的预测结果是False还是True，所以最后就可以得到这样一个**Bernoulli distribution****（伯努利分布）**。这样第i个点的错误率就是这个点的False的概率观测值pi，同时这个点的标准差也可以得到** si = sqrt( pi ( 1 - pi ) / i )**，根据confidence level（置信度）来确定warning和drift level。warning设置为95%的置信度，即pi + si >= min(p) + 2*min(s)，drift设置为99%的置信度，即pi + si >= min(p) + 3*min(s)。

![](https://img-blog.csdn.net/20180824105633304?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 DDM方法主要是用于incremental或者是online算法，通过这个方法可以更好地选择**合适的、稳定分布的**数据集用于对模型的训练。

### The Evalution of Concept Drift Methods

Klinkenberg提出了三种指标来衡量检测concept drift的效果。
- accuracy（准确率）
- recall（召回率）
- precision（精确率）

先看一下在Klinkenberg在论文中对这三种指标的介绍。

> 
**Accuracy **is the probalility, that a radom instance is classified correctly, and is estimated as the number of correct classifications divided by the total number of classifications.


> 
**Recall **is the probalility, that the classifier recognizes a relevant document as relevant, and is computed as the number of relevant documents found relevant by the classifier divided by the total number of relevant documents.


> 
**Precision **is the probalility, that a document found relevant by the classifier actually is relevant, and is estimated by the number of relevant documents found relevant by the classifier divided by the total number of documents found relevant by the classifier.


除了上面三种，还有F1-Measure（综合评价指标，也称为F1-Score），是综合考虑Precision和Recall指标的方法。

R、P、F1这几种指标也常用来衡量一些unbalanced的数据，例如要做异常点分类，1000个样本中只有10个异常，把所有数据都当作正常数据，那么也有99%的准确率，显然这不是我们想要的结果，所以这时用accuracy这一个指标就不合适了。

在介绍R、P还有F1指标之前需要先提前定义四个概念：TP、FP、FN、TN
||**相关(Relevant)，正类(Positive)**|**无关(Irrelevant)，负类(Negative)**|
|----|----|----|
|**Retrieved**|True Positive(异常点被正确分类为异常点)|False Positive(正常点被分类为异常点)|
|**Not Retrieved**|False Negative(将异常点分类为正常点，这个带来的损失是最大的)|True Negative(将正常点正确分类为正确点)|

可以通过下面的图来进一步加深理解。

![](https://img-blog.csdn.net/2018082412071153?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这样就可以得到P和R指标的计算方法：

![P=\frac{tp} {tp+fp}](https://private.codecogs.com/gif.latex?P%3D%5Cfrac%7Btp%7D%20%7Btp&plus;fp%7D)

![R=\frac{tp} {tp+fn}](https://private.codecogs.com/gif.latex?R%3D%5Cfrac%7Btp%7D%20%7Btp&plus;fn%7D)

有时候，会遇到一些很极端的情况，P和R的差异很大。所以这时可以通过F1来判断了，F是Recall和Precision的加权平均值，当下式中的a等于1时，就是F1了。

![F = \frac{(a^2+1)P*R} {a^2(P+R)}](https://private.codecogs.com/gif.latex?F%20%3D%20%5Cfrac%7B%28a%5E2&plus;1%29P*R%7D%20%7Ba%5E2%28P&plus;R%29%7D)



![F1 = \frac{2PR} {P+R}](https://private.codecogs.com/gif.latex?F1%20%3D%20%5Cfrac%7B2PR%7D%20%7BP&plus;R%7D)

除此之外，还可以通过图像来展示效果如何。常用的有**ROC**(Receiver Operating Characteristic)和**AUC**(Area Under Curve)图像，这个之后的文章再整理。

### Thought
- 样本序列需要label，True or False的模式只能以伯努利分布表示，是否可以转换为regression或者unsupervise问题
- warning到drift之间的序列不一定stationary
- 从old learner到new learner转换时，short term memory会导致train set过少

### Reference

Learn with Concept Drift

Adaptive Information Filtering Learning in the Presence of Concept Drifts

[准确率（Precision）、召回率（Recall）以及综合评价指标（F1-Measure ）](http://www.cnblogs.com/bluepoint2009/archive/2012/09/18/precision-recall-f_measures.html)



