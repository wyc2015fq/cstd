# task2 GBDT (Gradient Boosting Decision Tree) 算法 - fighting！！！ - CSDN博客
2019年01月29日 21:16:08[dujiahei](https://me.csdn.net/dujiahei)阅读数：112
参考：[https://blog.csdn.net/u013597931/article/details/79874439](https://blog.csdn.net/u013597931/article/details/79874439)
[http://www.cnblogs.com/jasonfreak/p/5720137.html](http://www.cnblogs.com/jasonfreak/p/5720137.html)
[http://www.cnblogs.com/sandy-t/p/6863918.html](http://www.cnblogs.com/sandy-t/p/6863918.html)
[https://www.itcodemonkey.com/article/4829.html](https://www.itcodemonkey.com/article/4829.html)
[https://blog.csdn.net/molu_chase/article/details/78111148](https://blog.csdn.net/molu_chase/article/details/78111148)
[https://blog.csdn.net/qq_22238533/article/details/79199605](https://blog.csdn.net/qq_22238533/article/details/79199605)
[https://blog.csdn.net/akirameiao/article/details/80009155](https://blog.csdn.net/akirameiao/article/details/80009155)
[https://blog.csdn.net/zpalyq110/article/details/79527653](https://blog.csdn.net/zpalyq110/article/details/79527653)
[https://blog.csdn.net/nickzzzhu/article/details/81539138](https://blog.csdn.net/nickzzzhu/article/details/81539138)
[https://blog.csdn.net/u013597931/article/details/79874439](https://blog.csdn.net/u013597931/article/details/79874439)
[https://www.cnblogs.com/ModifyRong/p/7744987.html](https://www.cnblogs.com/ModifyRong/p/7744987.html)
[https://www.cnblogs.com/pinard/p/6140514.html](https://www.cnblogs.com/pinard/p/6140514.html)
[https://www.cnblogs.com/peizhe123/p/5086128.html](https://www.cnblogs.com/peizhe123/p/5086128.html)
- 关于GBDT概念
	GBDT(Gradient Boosting Decision Tree) 又叫 MART（Multiple Additive Regression Tree)，是一种迭代的决策树算法，该算法由多棵决策树组成，所有树的结论累加起来做最终答案。它在被提出之初就和SVM一起被认为是泛化能力（generalization)较强的算法。
	GBDT主要由三个概念组成：Regression Decistion Tree（即DT)，Gradient Boosting（即GB)，Shrinkage (算法的一个重要演进分枝，目前大部分源码都按该版本实现）。​
	- DT：回归树 Regression Decision Tree
		决策树分为两大类，回归树和分类树。前者用于预测实数值，如明天的温度、用户的年龄、网页的相关程度；后者用于分类标签值，如晴天/阴天/雾/雨、用户性别、网页是否是垃圾页面。这里要强调的是，前者的结果加减是有意义的，如10岁+5岁-3岁=12岁，后者则无意义，如男+男+女=到底是男是女？ GBDT的核心在于累加所有树的结果作为最终结果，就像前面对年龄的累加（-3是加负3），而分类树的结果显然是没办法累加的，所以GBDT中的树都是回归树，不是分类树，这点对理解GBDT相当重要（尽管GBDT调整后也可用于分类但不代表GBDT的树是分类树）。
		回归树的运行流程与分类树基本类似，但有以下两点不同之处：
		第一，回归树的每个节点得到的是一个预测值而非分类树式的样本计数，假设在某一棵树的某一节点使用了年龄进行分枝（并假设在该节点上人数），那么这个预测值就是属于这个节点的所有人年龄的平均值。
		第二，在分枝节点的选取上，回归树并没有选用最大熵值来作为划分标准，而是使用了最小化均方差，即。这很好理解，被预测出错的次数越多，错的越离谱，均方差就越大，通过最小化均方差也就能够找到最靠谱的分枝依据。
		一般来讲，回归树的分枝不太可能实现每个叶子节点上的属性值都唯一，更多的是达到我们预设的终止条件即可（例如叶子个数上限），这样势必会存在多个属性取值，那么该节点处的预测值自然就为基于这些样本所得到的平均值了。
- GB：梯度迭代/梯度提升 Gradient Boosting
		Boosting，迭代，即通过迭代多棵树来共同决策。这怎么实现呢？难道是每棵树独立训练一遍，比如A这个人，第一棵树认为是10岁，第二棵树认为是0岁，第三棵树认为是20岁，我们就取平均值10岁做最终结论？--当然不是！且不说这是投票方法并不是GBDT，只要训练集不变，独立训练三次的三棵树必定完全相同，这样做完全没有意义。之前说过，GBDT是把所有树的结论累加起来做最终结论的，所以可以想到每棵树的结论并不是年龄本身，而是年龄的一个累加量。GBDT的核心就在于，每一棵树学的是之前所有树结论和的残差，这个残差就是一个加预测值后能得真实值的累加量。比如A的真实年龄是18岁，但第一棵树的预测年龄是12岁，差了6岁，即残差为6岁。那么在第二棵树里我们把A的年龄设为6岁去学习，如果第二棵树真的能把A分到6岁的叶子节点，那累加两棵树的结论就是A的真实年龄；如果第二棵树的结论是5岁，则A仍然存在1岁的残差，第三棵树里A的年龄就变成1岁，继续学。这就是Gradient Boosting在GBDT中的意义。
		首先需要明确，GB本身是一种理念而非一个具体的算法，其基本思想为：沿着梯度方向，构造一系列的弱分类器函数，并以一定权重组合起来，形成最终决策的强分类器​
- 前向分布算法
	- 补充：加法模型
		
![](https://img.mubu.com/document_image/40b6a671-588c-445e-ad34-93a9b3550857-2329912.jpg)
		我们将上式作为加法模型，其中b(x;γm)b(x;γm)为基函数，γmγm为基函数的参数，βmβm为基函数的系数，βmβm表示着对应的基函数在加法模型f(x)f(x)中的重要性。
- 基本思想
		
![](https://img.mubu.com/document_image/0243f960-54b0-4f1b-9c92-3f1e3b6590ce-2329912.jpg)
- 算法过程
		
![](https://img.mubu.com/document_image/448ffda0-d9ca-4483-aba5-d0d5b31f6017-2329912.jpg)
![](https://img.mubu.com/document_image/54933893-30ce-4061-91c6-ace1f97a46ba-2329912.jpg)
- 负梯度拟合
	
![](https://img.mubu.com/document_image/0c7f7d08-5371-49dc-a895-e6892c78727f-2329912.jpg)
- 损失函数
	
![](https://img.mubu.com/document_image/400c2e09-2a18-4d2d-a509-15293fea635d-2329912.jpg)
- 回归
	
![](https://img.mubu.com/document_image/eb581a04-db65-4694-a39e-ae9b29d5ce8e-2329912.jpg)
- 二分类
	
![](https://img.mubu.com/document_image/a2ed9123-2fa3-4d14-9b57-caa55b5ce718-2329912.jpg)
	GBDT的分类算法从思想上和GBDT的回归算法没有本质区别，但是由于样本输出不是连续的值，而是离散的类别，导致我们无法直接从输出类别去拟合类别输出的误差。 
	　　为了解决这个问题，主要有两个方法，一个是用指数损失函数，此时GBDT退化为Adaboost算法。另一种方法是用类似于逻辑回归的对数似然损失函数的方法。也就是说，我们用的是类别的预测概率值和真实概率值的差来拟合损失。本文仅讨论用对数似然损失函数的GBDT分类。而对于对数似然损失函数，我们又有二元分类和多元分类的区别。
- 多分类
	
![](https://img.mubu.com/document_image/3f2fe083-fd56-466a-80ef-80daa25012f4-2329912.jpg)
	从代码上看，大致和分类时候的过程一样。最大的不同点在于多了一层内部的循环For。这里需要注意的是： 
	1.对于多分类任务，GDBT的做法是采用一对多的策略（详情见文章）。 
	也就是说，对每个类别训练M个分类器。假设有K个类别，那么训练完之后总共有M*K颗树。 
	2.两层循环的顺序不能改变。也就是说，K个类别都拟合完第一颗树之后才开始拟合第二颗树，不允许先把某一个类别的M颗树学习完，再学习另外一个类别。
- 正则化
	在Adaboost中我们会对每个模型乘上一个弱化系数（正则化系数），减小每个模型对提升的贡献（注意：这个系数和模型的权重不一样，是在权重上又乘以一个0,1之间的小数），在GBDT中我们采用同样的策略，对于每个模型乘以一个系数λ (0 < λ ≤ 1)，降低每个模型对拟合损失的贡献，这种方法也意味着我们需要更多的基学习器。
	　　第二种是每次通过按比例（推荐[0.5, 0.8] 之间）随机抽取部分样本来训练模型，这种方法有点类似Bagging，可以减小方差，但同样会增加模型的偏差，可采用交叉验证选取，这种方式称为子采样。采用子采样的GBDT有时也称为随机梯度提升树（SGBT）。
	　　第三种就是控制基学习器CART树的复杂度，可以采用剪枝正则化。
- 优缺点
	- 优点
		1）可以灵活的处理各种类型的数据
		2）预测的准确率高
		3）使用了一些健壮的损失函数，如huber，可以很好的处理异常值
- 缺点
		由于基学习器之间的依赖关系，难以并行化处理，不过可以通过子采样的SGBT来诗选部分并行。
- sklearn参数
	loss:损失函数度量，有对数似然损失deviance和指数损失函数exponential两种，默认是deviance，即对数似然损失，如果使用指数损失函数，则相当于Adaboost模型。
	criterion: 样本集的切分策略，决策树中也有这个参数，但是两个参数值不一样，这里的参数值主要有friedman_mse、mse和mae3个，分别对应friedman最小平方误差、最小平方误差和平均绝对值误差，friedman最小平方误差是最小平方误差的近似。
	subsample:采样比例，这里的采样和bagging的采样不是一个概念，这里的采样是指选取多少比例的数据集利用决策树基模型去boosting，默认是1.0，即在全量数据集上利用决策树去boosting。
	warm_start:“暖启动”，默认值是False，即关闭状态，如果打开则表示，使用先前调试好的模型，在该模型的基础上继续boosting，如果关闭，则表示在样本集上从新训练一个新的基模型，且在该模型的基础上进行boosting。
	feature_importance_:特征重要性。
	oob_improvement_:每一次迭代对应的loss提升量。oob_improvement_[0]表示第一次提升对应的loss提升量。
	train_score_:表示在样本集上每次迭代以后的对应的损失函数值。
	loss_:损失函数。
	estimators_：基分类器个数。
- 应用场景
	GBDT几乎可用于所有回归问题（线性/非线性），相对logistic regression仅能用于线性回归，GBDT的适用面非常广。
	​亦可用于二分类问题（设定阈值，大于阈值为正例，反之为负例）；不适合做多分类问题；
	排序问题；
	常用于各大数据挖掘竞赛（模型融合）；
	广告推荐​​​。
