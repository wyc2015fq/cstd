
# 分类模型的评估方法-F分数(F-Score) - saltriver的专栏 - CSDN博客


2017年06月30日 20:59:37[saltriver](https://me.csdn.net/saltriver)阅读数：21595标签：[机器学习																](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)[分类																](https://so.csdn.net/so/search/s.do?q=分类&t=blog)[模型评估																](https://so.csdn.net/so/search/s.do?q=模型评估&t=blog)[F分数																](https://so.csdn.net/so/search/s.do?q=F分数&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=模型评估&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=分类&t=blog)个人分类：[机器学习																](https://blog.csdn.net/saltriver/article/category/6399667)
[
																								](https://so.csdn.net/so/search/s.do?q=分类&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)

前面介绍了机器学习中分类模型的**精确率(Precision)**和**召回率(Recall)**评估指标。对于Precision和Recall，虽然从计算公式来看，并没有什么必然的相关性关系，但是，在大规模数据集合中，这2个指标往往是相互制约的。理想情况下做到两个指标都高当然最好，但一般情况下，Precision高，Recall就低，Recall高，Precision就低。所以在实际中常常需要根据具体情况做出取舍，例如一般的搜索情况，在保证召回率的条件下，尽量提升精确率。而像癌症检测、地震检测、金融欺诈等，则在保证精确率的条件下，尽量提升召回率。
所以，很多时候我们需要综合权衡这2个指标，这就引出了一个新的指标F-score。这是综合考虑Precision和Recall的调和值。

$$
F-Score=(1+\beta ^2)\cdot \frac {Precision\cdot Recall} {\beta^2\cdot Precision + Recall}
$$
当$\beta=1$时，称为F1-score，这时，精确率和召回率都很重要，权重相同。当有些情况下，我们认为精确率更重要些，那就调整$\beta$的值小于1，如果我们认为召回率更重要些，那就调整$\beta$的值大于1。
举个例子：癌症检查数据样本有10000个，其中10个数据祥本是有癌症，其它是无癌症。假设分类模型在无癌症数据9990中预测正确了9980个，在10个癌症数据中预测正确了9个，此时真阳=9，真阴=9980，假阳=10，假阴=1。
那么：
Accuracy = (9+9980) /10000=99.89%
Precision=9/19+10)= 47.36%
F1-score=2×(47.36%  × 90%)/(1×47.36%+90%)=62.07%
F2-score=5× (47.36% × 90%)/(4×47.36%+90%)=76. 27%

