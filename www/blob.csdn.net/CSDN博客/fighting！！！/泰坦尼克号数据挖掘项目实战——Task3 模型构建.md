# 泰坦尼克号数据挖掘项目实战——Task3 模型构建 - fighting！！！ - CSDN博客
2019年01月06日 23:50:38[dujiahei](https://me.csdn.net/dujiahei)阅读数：43
参考：【1】[https://github.com/AaronJny/simple_titanic](https://github.com/AaronJny/simple_titanic)
          【2】 [https://zhuanlan.zhihu.com/p/30538352](https://zhuanlan.zhihu.com/p/30538352)
经过task 2 ，已经对数据进行了初步分析，特征选择，最终留下了12个特征。接下来将特征送入分类器中，进行对模型的训练。
```python
# 导入库
from sklearn.svm import SVC
from sklearn.model_selection import cross_val_score
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import MultinomialNB
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import AdaBoostClassifier
# 支持向量机
svc = SVC()
# 决策树
dtc = DecisionTreeClassifier()
# 随机森林
rfc = RandomForestClassifier()
# 逻辑回归
lr = LogisticRegression()
# 贝叶斯
nb = MultinomialNB()
# K邻近
knn = KNeighborsClassifier()
# AdaBoost
boost = AdaBoostClassifier()
```
对训练数据选择10折交叉验证，查看交叉验证的正确率
```python
print('\n\n\n模型验证:')
print ('SVM acc is', np.mean(cross_val_score(svc, train_X, target_Y, cv=10)))
print( 'DecisionTree acc is', np.mean(cross_val_score(dtc, train_X, target_Y, cv=10)))
print ('RandomForest acc is', np.mean(cross_val_score(rfc, train_X, target_Y, cv=10)))
print ('LogisticRegression acc is', np.mean(cross_val_score(lr, train_X, target_Y, cv=10)))
print( 'NaiveBayes acc is', np.mean(cross_val_score(nb, train_X, target_Y, cv=10)))
print ('KNN acc is', np.mean(cross_val_score(knn, train_X, target_Y, cv=10)))
print ('AdaBoost acc is', np.mean(cross_val_score(boost, train_X, target_Y, cv=10)))
```
![](https://img-blog.csdnimg.cn/20190106234550890.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
发现逻辑回归的分类效果最好，所以选择lr进行训练。随机森林的也较高，不知道是不是因为特征选择那里用的是随机森林方法。
```python
# 训练
lr.fit(train_X, target_Y)
# 预测
y_predict = lr.predict(test_X)
```
利用sum函数看有多少人预测存活了下来
```python
y_predict.sum()
```
得到结果171人。
