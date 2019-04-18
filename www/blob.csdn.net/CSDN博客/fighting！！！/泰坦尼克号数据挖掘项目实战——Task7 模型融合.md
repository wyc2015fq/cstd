# 泰坦尼克号数据挖掘项目实战——Task7 模型融合 - fighting！！！ - CSDN博客
2019年01月13日 12:48:42[dujiahei](https://me.csdn.net/dujiahei)阅读数：96
参考：
【1】[https://blog.csdn.net/qq_39422642/article/details/78566763](https://blog.csdn.net/qq_39422642/article/details/78566763)
【2】[https://blog.csdn.net/u014356002/article/details/54376138](https://blog.csdn.net/u014356002/article/details/54376138)
【3】[https://blog.csdn.net/junxinwoxin/article/details/80407917](https://blog.csdn.net/junxinwoxin/article/details/80407917)
【4】[https://blog.csdn.net/jiaoyangwm/article/details/81186509](https://blog.csdn.net/jiaoyangwm/article/details/81186509)
【5】[https://zhuanlan.zhihu.com/p/30538352](https://zhuanlan.zhihu.com/p/30538352)
【6】[https://blog.csdn.net/m0_37725003/article/details/81095555](https://blog.csdn.net/m0_37725003/article/details/81095555)
      之前在task4 模型集成的时候简单介绍过模型融合，这次又查到了一点资料，所以再来补充一丢丢。
    模型融合：把独立的学习器组合起来的结果。如果独立的学习器为同质，称为基学习器（都为SVM或都为LR）；如果独立的学习器为异质，称为组合学习器（将SVM+LR组合）。
    为什么要进行模型融合：将几个独立学习器的结果求平均，在统计、计算效率、性能表现上都有较好的效果。
- 统计上：假设空间中几个学习器的假设函数的平均更接近真实的假设f
- 计算上：迭代求解可能落入局部最优解，但是多个局部最优解的平均更接近全局最优解，损失函数有可能不是光滑的，不同的初始点和学习率可能有不同的局部最小，将其平均能得到更好的。
- 性能表现上：真实的假设函数f可能不在已知的假设空间H内，学习器的平均更可能接近H外的真实假设H，如果模型本身就不具备表达场景的能力，那么无论怎么搜索H都不会搜到。
![](https://img-blog.csdnimg.cn/20190113111800860.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190113111856326.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
### 1.Bagging
从特征，参数，样本的多样性差异性来做多模型融合，参考随机森林。
![](https://img-blog.csdnimg.cn/20190113111917360.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190113111931769.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
### 2.Stacking
![](https://img-blog.csdnimg.cn/20190113112058260.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
将训练集划分为两个正交集D1（x，y）,D2（x’，y’），利用D1来学习三个模型，假设分别为LR，SVM，DT，
利用第二份数据D2的x分别作为第一层学到的三个模型的输入，得到预测值y1，y2，y3，将其组合可以得到预估的输出y^y^
已有真实输出的标签y’，可以学习到如何从预估的y^y^，来学习如何得到真实的y。
第一层的数据：为了训练得到三个模型第二层的数据：为了用三个模型来预测输出，得到的输入送入线性分类器得到最终的预估y^y^，再不断的训练模型使得模型的预估和真实的y′y′最接近，之所以将数据分成两组，是为了避免过拟合。
 也可以参见下图的例子：
![](https://img-blog.csdnimg.cn/20190113112250234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
stacking在第一层模型中通过交叉验证的方式生成预测的label，作为第二层线性模型的输入特征（第一层有几个模型第二层就有几个特征），一般模型差异越大融合效果越好。实际中也可以将预测的几个label特征加入到原始特征中，然后用复杂模型再次训练。 
![这里写图片描述](https://img-blog.csdn.net/20180522162119821?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2p1bnhpbndveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
blending和stacking的比较：
blending是将train划分为2份不相交的数据，一份用来训练生成新的特征，并将其加入到第二层继续训练，第二层只用到了部分数据；
stacking将train进行cv划分来训练。第二层用到了第一层的全部数据，并且需要训练大量的模型。
### 3.Adaboost
![](https://img-blog.csdnimg.cn/20190113112358798.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190113112410539.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
**4、Gradient Boosting Tree**
解决回归问题
通过不断的拟合预测和真实的残差来学习，也就是每次迭代尽量拟合损失函数在当前情况下的负梯度，构建的树是能使得损失函数降低最多的学习器，来解决回归问题，调整后也能解决分类问题。
   好啦，接下来是本次task的代码部分：
1. Bagging 方法，即投票法，使用sklearn.ensemble中的VotingClassifier。
```python
#### task7 模型融合
# 1 Bagging法，使用sklearn.ensemble中的VotingClassifier
from sklearn.ensemble import VotingClassifier
clf_vc = VotingClassifier(estimators=[('lr',LogisticRegression()),('DTree',DecisionTreeClassifier()),('RTree',RandomForestClassifier()),('svm',SVC()), ('xgb', XGBClassifier())])
clf_vc.fit(train_X,target_Y)
print(clf_vc.score(train_X, target_Y))
```
![](https://img-blog.csdnimg.cn/20190113120307106.png)
2. Stacking 方法，使用mlxtend.classifier中的StackingCVClassifier
ps:自己总是忘记SVC()内要赋值true。使用前要安装库 mlxtend。
svm=SVC(probability=True)；
```python
# 2 Stacking法
from mlxtend.classifier import StackingCVClassifier
sclf = StackingCVClassifier(classifiers=[lr,DTree,RTree,svm], meta_classifier= xgb, use_probas=True)
sclf.fit(train_X.values,target_Y.values)
print(sclf.score(train_X, target_Y))
```
![](https://img-blog.csdnimg.cn/20190113124746952.png)
同样的数据，投票法的结果更好些，可能是因为数据量小，所以Stacking并不占优势。
