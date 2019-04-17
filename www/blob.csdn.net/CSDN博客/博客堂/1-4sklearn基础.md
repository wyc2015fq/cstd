# 1-4sklearn基础 - 博客堂 - CSDN博客





2017年06月30日 08:52:09[最小森林](https://me.csdn.net/u012052268)阅读数：339标签：[机器学习																[python																[算法																[sklearn](https://so.csdn.net/so/search/s.do?q=sklearn&t=blog)
个人分类：[机器学习																[数据分析](https://blog.csdn.net/u012052268/article/category/6946029)](https://blog.csdn.net/u012052268/article/category/6821965)





# sklearn基础


- [sklearn基础](#sklearn基础)- [sklearn 简介](#1-sklearn-简介)
- [安装](#2-安装)
- [选择合适的机器学习方法模型](#3-选择合适的机器学习方法模型)
- [入门例子-分类-KNN算法-给花朵分类](#4-入门例子-分类-knn算法-给花朵分类)




笔记是观看莫烦的机器学习视频[《莫烦Scikit-learn》](https://morvanzhou.github.io/tutorials/machine-learning/sklearn/2-2-general-pattern/)的读书笔记，初学者强烈推荐他的视频。

## 1 sklearn 简介

Scikit learn 也简称 sklearn, 是机器学习领域当中最知名的 python 模块之一.

Sklearn 包含了很多种机器学习的方式:
- Classification 分类
- Regression 回归
- Clustering 非监督分类
- Dimensionality reduction 数据降维
- Model Selection 模型选择
- Preprocessing 数据预处理

我们总能够从这些方法中挑选出一个适合于自己问题的, 然后解决自己的问题.

## 2 安装

安装 Scikit-learn (sklearn) 最简单的方法就是使用 pip 安装它.

或者使用 Anaconda 很方便的安装所有 python 的科学计算模块.（Anaconda已经集成了许多有用模块，包括sklearn）

## 3 选择合适的机器学习方法（模型）

机器学习算法有四类，分类，回归，聚类，降维。

如图：

![image](https://morvanzhou.github.io/static/results/sklearn/2_1_1.png)
- 其中 分类和回归是监督式学习，即每个数据对应一个 label。
- 聚类 是非监督式学习，即没有 label。 
- 另外一类是 降维，当数据集有很多很多属性的时候，可以通过 降维 算法把属性归纳起来。（例如 20 个属性只变成 2 个，注意，这不是挑出 2 个，而是压缩成为 2 个，它们集合了 20 个属性的所有特征，相当于把重要的信息提取的更好，不重要的信息就不要了。

## 4 入门例子-分类-KNN算法-给花朵分类

Sklearn 把所有机器学习的模式整合统一起来了，学会了一个模式就可以通吃其他不同类型的学习模式。

Sklearn 本身有很多数据库，可以用来练习。其中有个Iris数据库：  Iris 是一种花，这种花有四个属性，花瓣的长宽，茎的长宽，根据这些属性把花分为三类。

我们要用 分类器 去把四种类型的花分开。

![image](https://morvanzhou.github.io/static/results/sklearn/2_2_1.png)

今天用 KNN classifier，就是选择几个临近点，综合它们做个平均来作为预测值。

下面是详细代码，过程见注释：

```python
# 数组模块
import numpy as np
# 存储了许多有用的数据集
from sklearn import datasets
# 分割数据的模块，把数据集分为训练集和测试集
from sklearn.cross_validation import train_test_split
# KNN算法
from sklearn.neighbors import KNeighborsClassifier


# 导入iris花的数据集
iris = datasets.load_iris()
iris_X = iris.data # 输入是花的四个属性
iris_y = iris.target # 输出是花的3个种类
print(iris_X[0:5])
print(iris_y[0:5])

# 将数据集分割成 训练集 与 测试集，切顺序是打乱的。其中测试集占30%
X_train,X_test,y_train,y_test = train_test_split(iris_X,iris_y,test_size=0.3)
# print(y_train)

# 创建KNN方法
knn = KNeighborsClassifier()

# 使用数据训练模型
knn.fit(X_train,y_train)

# 使用训练好的模型进行预测,并于真实的结果进行比较
print(knn.predict(X_test))
print(y_test)
```](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)




