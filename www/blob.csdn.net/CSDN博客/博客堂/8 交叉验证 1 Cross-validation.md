# 8 交叉验证 1 Cross-validation - 博客堂 - CSDN博客





2017年07月01日 16:12:45[最小森林](https://me.csdn.net/u012052268)阅读数：705标签：[交叉验证																[sklearn-教程																[可视化																[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[机器学习																[数据分析](https://blog.csdn.net/u012052268/article/category/6946029)](https://blog.csdn.net/u012052268/article/category/6821965)





# 交叉验证 1 Cross-validation


- [交叉验证 1 Cross-validation](#交叉验证-1-cross-validation)- [介绍](#介绍)
- [例子1 - 如何选择正确的Model 基础验证法](#例子1-如何选择正确的model-基础验证法)
- [例子2 - 如何选择正确的Model 交叉验证法Cross Validation](#例子2-如何选择正确的model-交叉验证法cross-validation)
- [例子3 - 如何选择模型参数 以准确率accuracy判断](#例子3-如何选择模型参数-以准确率accuracy判断)
- [例子4 - 如何选择模型参数 以平均方差Mean squared error判断](#例子4-如何选择模型参数-以平均方差mean-squared-error判断)




## 介绍

Sklearn 中的 Cross Validation (交叉验证)对于我们选择正确的 Model 和 Model 的参数是非常有帮助的， 有了他的帮助，我们能直观的看出不同 Model 或者参数对结构准确度的影响。

使用方法:

```python
from sklearn.cross_validation import cross_val_score # K折交叉验证模块
```

## 例子1 - 如何选择正确的Model 基础验证法

```python
from sklearn.datasets import load_iris # iris数据集
from sklearn.model_selection import train_test_split # 分割数据模块
from sklearn.neighbors import KNeighborsClassifier # K最近邻(kNN，k-NearestNeighbor)分类算法

#加载iris数据集
iris = load_iris()
X = iris.data
y = iris.target

#分割数据并
X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=4)

#建立模型
knn = KNeighborsClassifier()

#训练模型
knn.fit(X_train, y_train)

#将准确率打印出
print(knn.score(X_test, y_test))
# 0.973684210526
```

可以看到基础验证的准确率为0.973684210526

## 例子2 - 如何选择正确的Model 交叉验证法(Cross Validation)

```python
from sklearn.cross_validation import cross_val_score # K折交叉验证模块

#使用K折交叉验证模块
#这里：变换KNN参数即可对比不同模型；变换cv=5即可把数据分成更多份
scores = cross_val_score(knn, X, y, cv=5, scoring='accuracy')

#将5次的预测准确率打印出
print(scores)
# [ 0.96666667  1.          0.93333333  0.96666667  1.        ]

#将5次的预测准确平均率打印出，这个比一次score更令人信服
print(scores.mean())
# 0.973333333333
```

## 例子3 - 如何选择模型参数? 以准确率(accuracy)判断

一般来说准确率(accuracy)会用于判断分类(Classification)模型的好坏。

```python
import matplotlib.pyplot as plt #可视化模块

#建立测试参数集
k_range = range(1, 31)

k_scores = []

#藉由迭代的方式来计算不同参数对模型的影响，并返回交叉验证后的平均准确率
for k in k_range:
    knn = KNeighborsClassifier(n_neighbors=k)
    scores = cross_val_score(knn, X, y, cv=10, scoring='accuracy')
    k_scores.append(scores.mean())

#可视化数据
plt.plot(k_range, k_scores)
plt.xlabel('Value of K for KNN')
plt.ylabel('Cross-Validated Accuracy')
plt.show()
```

![image](https://morvanzhou.github.io/static/results/sklearn/3_2_1.png)

从图中可以得知，选择12~18的k值最好。高过18之后，准确率开始下降则是因为过拟合(Over fitting)的问题。

## 例子4 - 如何选择模型参数? 以平均方差(Mean squared error)判断

一般来说平均方差(Mean squared error)会用于判断回归(Regression)模型的好坏。

```python
import matplotlib.pyplot as plt
k_range = range(1, 31)
k_scores = []
for k in k_range:
    knn = KNeighborsClassifier(n_neighbors=k)
    loss = -cross_val_score(knn, X, y, cv=10, scoring='mean_squared_error')
    k_scores.append(loss.mean())

plt.plot(k_range, k_scores)
plt.xlabel('Value of K for KNN')
plt.ylabel('Cross-Validated MSE')
plt.show()
```

![image](https://morvanzhou.github.io/static/results/sklearn/3_2_2.png)

由图可以得知，平均方差越低越好，因此选择13~18左右的K值会最好。](https://so.csdn.net/so/search/s.do?q=可视化&t=blog)](https://so.csdn.net/so/search/s.do?q=sklearn-教程&t=blog)](https://so.csdn.net/so/search/s.do?q=交叉验证&t=blog)




