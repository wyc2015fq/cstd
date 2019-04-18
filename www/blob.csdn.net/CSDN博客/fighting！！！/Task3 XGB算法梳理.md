# Task3 XGB算法梳理 - fighting！！！ - CSDN博客
2019年02月01日 18:24:32[dujiahei](https://me.csdn.net/dujiahei)阅读数：61
参考：[https://blog.csdn.net/china1000/article/details/51106856](https://blog.csdn.net/china1000/article/details/51106856)
[https://www.zhihu.com/question/41354392](https://www.zhihu.com/question/41354392)
[https://blog.csdn.net/weixin_38629654/article/details/80605640](https://blog.csdn.net/weixin_38629654/article/details/80605640)
[https://blog.csdn.net/weixin_43178406/article/details/86706979](https://blog.csdn.net/weixin_43178406/article/details/86706979)
[https://www.cnblogs.com/wanglei5205/p/8579244.html](https://www.cnblogs.com/wanglei5205/p/8579244.html)
- CART树
	CART也是决策树的一种，不过是满二叉树，CART可以是强分类器，就跟决策树一样，但是我们可以指定CART的深度，使之成为比较弱的分类器。
	CART使用gini系数来衡量数据集的划分效果而不是香农熵。​
- 算法原理
	- 引入
		XGboost 是eXtreme Gradient Boosting ，他是在GBM基础上的改进，内部同样采用决策树作为基学习器，XGboost（下文称为XGB）与GBM的区别在于损失函数更新的方式，GBM利用的是梯度下降法的近似方法，而XGB方法则引入了牛顿法进行损失函数的寻优，因为牛顿法使用到了二阶导，这就是为什XGB要叫做极端梯度法。
- 牛顿法
		
![](https://img.mubu.com/document_image/fa2d2ac8-9507-4f12-b28f-216e71f8d291-2329912.jpg)
- 流程：建立在boosting方法上，利用牛顿法来求解损失函数的最小值。
		
![](https://img.mubu.com/document_image/bebe91c4-2798-491d-9b67-990d3cc86ce7-2329912.jpg)
- 损失函数
	
![](https://img.mubu.com/document_image/a1bdf3b6-a139-434c-acd2-7ea44fb684d1-2329912.jpg)
	传统GBDT在优化时只用到一阶导数信息，xgboost则对代价函数进行了二阶泰勒展开，同时用到了一阶和二阶导数。顺便提一下，xgboost工具支持自定义代价函数，只要函数可一阶和二阶求导。
- 分裂结点算法
	
![](https://img.mubu.com/document_image/4ea31340-e0b7-4b3a-9820-29c9572db6e8-2329912.jpg)
	分裂点选择的时候也，以目标函数最小化为目标。
- 正则化
	
![](https://img.mubu.com/document_image/fb3c1ba0-88eb-4da5-b460-35d313abcdea-2329912.jpg)
	xgboost在代价函数里加入了正则项，用于控制模型的复杂度，防止过拟合。正则项里包含了树的叶子节点个数、每个叶子节点上输出的score的L2模的平方和。从Bias-variance tradeoff角度来讲，正则项降低了模型的variance，使学习出来的模型更加简单，防止过拟合，这也是xgboost优于传统GBDT的一个特性。
- 对缺失值处理
	对缺失值的处理。对于特征的值有缺失的样本，xgboost可以自动学习出它的分裂方向。
- 权重衰减
	xgboost在进行完一次迭代后，会将叶子节点的权重乘上该系数，主要是为了削弱每棵树的影响，让后面有更大的学习空间。实际应用中，一般把eta设置得小一点，然后迭代次数设置得大一点。（补充：传统GBDT的实现也有学习速率）。
- 支持列抽样
	xgboost借鉴了随机森林的做法，支持列抽样，不仅能降低过拟合，还能减少计算，这也是xgboost异于传统gbdt的一个特性。
- 支持并行
	xgboost的并行不是tree粒度的并行，xgboost也是一次迭代完才能进行下一次迭代的（第t次迭代的代价函数里包含了前面t-1次迭代的预测值）。xgboost的并行是在特征粒度上的。我们知道，决策树的学习最耗时的一个步骤就是对特征的值进行排序（因为要确定最佳分割点），xgboost在训练之前，预先对数据进行了排序，然后保存为block结构，后面的迭代中重复地使用这个结构，大大减小计算量。这个block结构也使得并行成为了可能，在进行节点的分裂时，需要计算每个特征的增益，最终选增益最大的那个特征去做分裂，那么各个特征的增益计算就可以开多线程进行。可并行的近似直方图算法。树节点在进行分裂时，我们需要计算每个特征的每个分割点对应的增益，即用贪心法枚举所有可能的分割点。当数据无法一次载入内存或者在分布式情况下，贪心算法效率就会变得很低，所以xgboost还提出了一种可并行的近似直方图算法，用于高效地生成候选的分割点。
- 优缺点
	- 优点：非线性变换比较多，表达能力强，而且不需要做复杂的特征工程和特征变换。
- 缺点：Boost是一个串行过程，不好并行化，而且计算复杂度高，同时不太适合高维洗漱特征。
- 应用场景
	可以用于分类和回归问题。在数据挖掘等相关竞赛以及实际工程中都有应用。
- sklearn参数
	- 常规参数
		- booster
			gbtree 树模型做为基分类器（默认）
			gbliner 线性模型做为基分类器
- silent
			silent=0时，不输出中间过程（默认）
			silent=1时，输出中间过程
- nthread
			nthread=-1时，使用全部CPU进行并行运算（默认）
			nthread=1时，使用1个CPU进行运算。
- scale_pos_weight
			正样本的权重，在二分类任务中，当正负样本比例失衡时，设置正样本的权重，模型效果更好。例如，当正负样本比例为1:10时，scale_pos_weight=10。
- 模型参数
		- n_estimatores
			含义：总共迭代的次数，即决策树的个数
- early_stopping_rounds
			含义：在验证集上，当连续n次迭代，分数没有提高后，提前终止训练。
			调参：防止overfitting。
- max_depth
			含义：树的深度，默认值为6，典型值3-10。
			调参：值越大，越容易过拟合；值越小，越容易欠拟合。
- min_child_weight
			含义：默认值为1,。
			调参：值越大，越容易欠拟合；值越小，越容易过拟合（值较大时，避免模型学习到局部的特殊样本）。
- subsample
			含义：训练每棵树时，使用的数据占全部训练集的比例。默认值为1，典型值为0.5-1。
			调参：防止overfitting。
- colsample_bytree
			含义：训练每棵树时，使用的特征占全部特征的比例。默认值为1，典型值为0.5-1。
			调参：防止overfitting。
- 学习任务参数
		- learning_rate
			含义：学习率，控制每次迭代更新权重时的步长，默认0.3。
			调参：值越小，训练越慢。
			典型值为0.01-0.2。
- objective 目标函数
			回归任务
			reg:linear (默认)
			reg:logistic
			二分类
			binary:logistic     概率 
			binary：logitraw   类别
			多分类
			multi：softmax  num_class=n   返回类别
			multi：softprob   num_class=n  返回概率
			rank:pairwise
			​
- eval_metric
			回归任务(默认rmse)
			rmse--均方根误差
			mae--平均绝对误差
			分类任务(默认error)
			auc--roc曲线下面积
			error--错误率（二分类）
			merror--错误率（多分类）
			logloss--负对数似然函数（二分类）
			mlogloss--负对数似然函数（多分类）
- gamma
			惩罚项系数，指定节点分裂所需的最小损失函数下降值。​
- alpha
			L1正则化系数，默认为1。
- lambda
			L2正则化系数，默认为1。
- 代码主要函数
		- 载入数据：load_digits()
- 数据拆分：train_test_split()
- 建立模型：XGBClassifier()
- 模型训练：fit()
- 模型预测：predict()
- 性能度量：accuracy_score()
- 特征重要性：plot_importance()
