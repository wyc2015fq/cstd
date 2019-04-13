
# GBDT实战 - bitcarmanlee的博客 - CSDN博客


2017年09月05日 21:56:18[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：2305


## 1.GBDT介绍
GBDT(Gradient Boosting Decision Tree) 或者称为MART(Multiple Additive Regression Tree)，也可叫GBRT(Gradient Boosting Regression Tree)，是一种基于回归决策树的Boosting集成算法。
从名字很容易看出，GBDT是由决策树(Decision Tree)与梯度提升(Gradient Boosting)联合而得来的。
## 2.决策树(DT)
决策树是附加概率结果的一个树状的决策图，是直观的运用统计概率分析的图法。机器学习中决策树是一个预测模型，它表示对象属性和对象值之间的一种映射，树中的每一个节点表示对象属性的判断条件，其分支表示符合节点条件的对象。树的叶子节点表示对象所属的预测结果。
决策树是一种十分常用和基础的监督学习算法，可适用于分类和回归问题；决策树的关键技术包括分裂点的选择、分裂停止的条件以及避免过拟合的方法（如剪枝；合适的分裂停止条件也可以防止过拟合）。
## 3.回归树
回归树即用来解决回归问题的决策树。在分类树中，样本标签是离散的或非有序的，我们取叶子节点样本标签的众数或概率分布作为预测结果；而在回归树中，样本标签一般是连续性的有序数据，我们取叶子结点中所有样本标签的平均值作为预测结果。
## 4.集成方法（Ensemble Method）
集成学习方法是将多个弱模型通过一定的组合方式组成一个新的强模型的方法，一般情况下集成的模型具有更强的预测和泛化能力。在机器学习问题中，这是一种非常强大的思路，也是”集体智慧”的典型例子。集成算法中的弱模型又称元算法；在GBDT中，回归树是GBDT的元算法。
我们在理解集成方法时，可以更多将其看作一些学习框架，重点在于理解这些框架的思路。各种集成算法（如GBDT、随机森林）的核心也可理解为将基本算法（如决策树）带入集成框架（如Boosting、Bagging）的产物。
## 5.Boosting与Gradient Boosting
Boosting的意思是”提升”，它关注被预测错误的样本，基于预测错误的部分构建新的弱模型并集成，是一种常用的迭代集成方法。原始的Boosting方法可以说是基于”样本”的，它会在一开始给所有样本附上相等的权重值，在每轮迭代（生成一个弱模型）后增加预测错误的样本的权重，减少预测正确的样本的权重，并在此基础上训练新的弱模型；最终通过加权或投票的形式对所有弱模型进行组合，生成强模型。
而Gradient Boosting和原始Boosting方法不同的地方在于，它在残差减少的梯度方向建立新的弱模型。直观上看，它用来训练第K轮弱模型的数据，来自于之前所有弱模型集成后的预测值和样本真实值的”差”（准确来说损失函数梯度减少的方向）。
## 6.GBDT小结
基于上面描述的一系列概念，我们可以较为容易的理解：一个GBDT模型由多颗回归决策树组成；理论上在训练过程中的一轮迭代中，算法基于残差减少的梯度方向生成一颗决策树（scikit-learn在用GBDT解决多标签问题时，实际上在每一轮迭代中用了多棵回归树，本文中我们不对这种情况做深入说明）。在预测阶段，累加模型中所有决策树的预测值(乘上步长/学习率)，即可计算整个模型的预测结果。
GBDT算法在实际生产中运用非常广泛，表达能力也很强，通常不需要复杂的特征工程就能得到较好的预测效果，还能输出特征重要性得分；同时通过设定合理的样本和特征抽样比例，可以在训练过程中实现交叉检验（cross validation），有效地减少模型过拟合的出现。缺点则是基于Boosting集成方法的算法较难实现并行化，且基于GBDT的模型会较为复杂，深入分析和调优会有一定困难性。
## 7.波士顿房价数据集(回归模型)
```python
#! /usr/bin/env python
```
```python
# coding: utf-8
```
```python
from
```
```python
sklearn.ensemble
```
```python
import
```
```python
GradientBoostingRegressor
```
```python
from
```
```python
sklearn.datasets
```
```python
import
```
```python
load_boston
```
```python
from
```
```python
sklearn.metrics
```
```python
import
```
```python
mean_squared_error
```
```python
from
```
```python
sklearn.model_selection
```
```python
import
```
```python
train_test_split
```
```python
#数据集
```
```python
X_train, X_test, y_train, y_test = train_test_split(load_boston().data, load_boston().target, test_size=
```
```python
0.2
```
```python
)
```
```python
'''
loss:损失函数，对于Regressor可以用'ls','lad','huber', 'quantile'。
learning_rate: 学习率/步长。
n_estimators: 迭代次数，和learning_rate存在trade-off关系。
criterion: 衡量分裂质量的公式，一般默认即可。
subsample: 样本采样比例。
max_features: 最大特征数或比例。
决策树相关参数包括max_depth, min_samples_split, min_samples_leaf, min_weight_fraction_leaf, max_leaf_nodes, min_impurity_split, 多数用来设定决策树分裂停止条件。
'''
```
```python
reg_model = GradientBoostingRegressor(
    loss =
```
```python
"ls"
```
```python
,
    learning_rate=
```
```python
0.02
```
```python
,
    n_estimators=
```
```python
200
```
```python
,
    subsample=
```
```python
0.8
```
```python
,
    max_features=
```
```python
0.8
```
```python
,
    max_depth=
```
```python
3
```
```python
,
    verbose=
```
```python
2
```
```python
)
reg_model.fit(X_train, y_train)
prediction_train = reg_model.predict(X_train)
rmse_train = mean_squared_error(y_train, prediction_train)
prediction_test = reg_model.predict(X_test)
rmse_test = mean_squared_error(y_test, prediction_test)
```
```python
print
```
```python
"RMSE for training dataset is %f, for testing dataset is %f."
```
```python
% (rmse_train, rmse_test)
```
最后的输出结果：
```python
RMSE
```
```python
for
```
```python
training dataset
```
```python
is
```
```python
4.279429
```
```python
,
```
```python
for
```
```python
testing dataset
```
```python
is
```
```python
12.399659
```
```python
.
```
## 8.鸢尾花分类数据集(分类模型)
```python
# /usr/bin/env python
```
```python
# coding:utf-8
```
```python
from
```
```python
sklearn.ensemble
```
```python
import
```
```python
GradientBoostingClassifier
```
```python
from
```
```python
sklearn.datasets
```
```python
import
```
```python
load_iris
```
```python
from
```
```python
sklearn.metrics
```
```python
import
```
```python
confusion_matrix
```
```python
from
```
```python
sklearn.model_selection
```
```python
import
```
```python
train_test_split
X_train, X_test, y_train, y_test = train_test_split(load_iris().data, load_iris().target, test_size=
```
```python
0.2
```
```python
)
iris_model = GradientBoostingClassifier(
    loss =
```
```python
'deviance'
```
```python
,
    learning_rate =
```
```python
0.01
```
```python
,
    n_estimators =
```
```python
50
```
```python
,
    subsample =
```
```python
0.8
```
```python
,
    max_features =
```
```python
1
```
```python
,
    max_depth =
```
```python
3
```
```python
,
    verbose =
```
```python
2
```
```python
)
iris_model.fit(X_train, y_train)
```
```python
# 评估模型
```
```python
prediction_train = iris_model.predict(X_train)
cm_train = confusion_matrix(y_train, prediction_train)
prediction_test = iris_model.predict(X_test)
cm_test = confusion_matrix(y_test, prediction_test)
```
```python
print
```
```python
"Confusion matrix for training dataset is \n%s\n for testing dataset is \n%s."
```
```python
% (cm_train, cm_test)
```
最后输出：
```python
Confusion matrix
```
```python
for
```
```python
training dataset is
```
```python
[[45  0  0]
 [ 0 39  0]
 [ 0  1 35]]
```
```python
for
```
```python
testing dataset is
```
```python
[[ 5  0  0]
 [ 0 11  0]
 [ 0  0 14]]
```
```python
.
```
## 9.sklearn中GBDT代码结构
ython的scikit-learn包包含了我们常用的大部分的机器学习算法和数据处理方法，我们主要分析其中实现GBDT的源码。GBDT的实现源码依然可以被分为GB和DT两部分。其中DT为决策树部分，其源码在一个名为Tree的package下；GB为gradient boosting方法，其相关源码在一个名为Ensemble的package下。总体结构见下面的思维导图。
![这里写图片描述](https://img-blog.csdn.net/20170909174745718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Tree包的源码结构截图如下。里面实现了决策树算法、决策树的基本数据结构Tree、决策树构建策略以及树的可视化等内容。
![这里写图片描述](https://img-blog.csdn.net/20170909174928288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Ensemble包的源码结构截图如下。Ensemble包里还包含了如bagging、随机森林等其它主题，跟GBDT有关的主要是其中的base.py和grandient_boosting.py文件。
![这里写图片描述](https://img-blog.csdn.net/20170909175052391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
原文链接：[http://www.jianshu.com/p/02cfaae3fd01](http://www.jianshu.com/p/02cfaae3fd01)

