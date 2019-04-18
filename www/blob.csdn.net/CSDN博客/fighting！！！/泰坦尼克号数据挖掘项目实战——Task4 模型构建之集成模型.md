# 泰坦尼克号数据挖掘项目实战——Task4 模型构建之集成模型 - fighting！！！ - CSDN博客
2019年01月08日 20:29:01[dujiahei](https://me.csdn.net/dujiahei)阅读数：58
  参考：【1】[https://cloud.tencent.com/developer/article/1064013](https://cloud.tencent.com/developer/article/1064013)
【2】[https://blog.csdn.net/wxq_1993/article/details/85853808](https://blog.csdn.net/wxq_1993/article/details/85853808)
【3】[https://blog.csdn.net/jiezhouzi/article/details/78001296](https://blog.csdn.net/jiezhouzi/article/details/78001296)
【4】[https://blog.csdn.net/cicilover/article/details/77371496](https://blog.csdn.net/cicilover/article/details/77371496)
   在task3中使用了支持向量机、决策树、随机森林回归、逻辑回归、贝叶斯、k近邻、AdaBoost 七种分类器，交叉验证后表明不同分类器的分类效果不同，选择了交叉验证正确率最高的一组来进行预测。
   虽说老将出马 一个顶俩，那么没有老将的时候怎么办呢？还有种说法是三个臭皮匠顶个诸葛亮，那就希望人多力量大，模型们团结一下，性能会更好，正确率会更高吧。
  那么如何让模型们团结起来呢？最容易想到的就是投票法，每个模型都会有输出结果，按照投票结果最多的决策输出，也可以考虑对投票分配一个权重；再回退一步，分类器输出结果前其实还有个决策值，考虑将决策值进行相加/平均，可以考虑分配权重；再回退一步，或许模型们在训练的时候，可以相互融合？
  用一些专业术语来说呢，是这样子的：
模型融合（Model Ensemble）
常见的模型融合方法有：Bagging、Boosting、Stacking、Blending。
*(3-1):Bagging*
Bagging 将多个模型，也就是多个基学习器的预测结果进行简单的加权平均或者投票。它的好处是可以并行地训练基学习器。Random Forest就用到了Bagging的思想。
*(3-2): Boosting*
Boosting 的思想有点像知错能改，每个基学习器是在上一个基学习器学习的基础上，对上一个基学习器的错误进行弥补。  AdaBoost，Gradient Boost 就用到了这种思想。
*(3-3): Stacking*
Stacking是用新的次学习器去学习如何组合上一层的基学习器。如果把 Bagging 看作是多个基分类器的线性组合，那么Stacking就是多个基分类器的非线性组合。Stacking可以将学习器一层一层地堆砌起来，形成一个网状的结构。
相比来说Stacking的融合框架相对前面的二者来说在精度上确实有一定的提升  
*(3-4): Blending*
Blending 和 Stacking 很相似，但同时它可以防止信息泄露的问题。
1.*Bagging 方法*
*   用随机森林来实践一下Bagging 方法。*
```python
# 使用随机森林分类器进行集成模型的训练以及预测分析
from sklearn.ensemble import RandomForestClassifier
rfc = RandomForestClassifier()
rfc.fit(train_X, target_Y)
rfc_y_pred = rfc.predict(test_X)
```
task3中跑过了，就直接放个结果  ![](https://img-blog.csdnimg.cn/20190107233313552.png)
2. *Boosting方法*
*    用AdaBoost来实践一下。*
```python
from sklearn.ensemble import AdaBoostClassifier
boost = AdaBoostClassifier()
boost.fit(train_X, target_Y)
boost _y_pred = rfc.predict(test_X)
```
task3中跑过了，就直接放个结果   ![](https://img-blog.csdnimg.cn/20190107233330699.png)
 xgboost
```python
from xgboost import XGBClassifier
xgboost = AdaBoostClassifier()
xgboost.fit(train_X, target_Y)
xgboost_y_pred = rfc.predict(test_X)
```
3. *Stacking方法*
还在研究当中
