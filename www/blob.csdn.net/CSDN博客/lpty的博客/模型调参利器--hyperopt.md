
# 模型调参利器--hyperopt - lpty的博客 - CSDN博客

2018年04月18日 14:16:03[lpty](https://me.csdn.net/sinat_33741547)阅读数：1222



## 一、前言
在机器学习中，模型的训练需要花费大量的时间。而每一个算法在训练前都需要配置数量不一的超参数，参数对训练结果的影响相当大。
因此，超参数的优化是一件相当重要，却又费时费力的事。
Hyperopt提供了一个优化接口，这个接口接收一个评估函数和参数空间，能计算出空间内的一个点的损失函数值，简化了调参过程。
## 二、实战
通过使用感知机判别鸢尾花数据的例子，熟悉一下hyper库的用法。
### 1、数据读取及标准化
`from sklearn import datasets
import numpy as np
from sklearn.cross_validation import train_test_split
from sklearn.metrics import accuracy_score
iris = datasets.load_iris()
X = iris.data
y = iris.target
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=0)

from sklearn.preprocessing import StandardScaler
sc = StandardScaler()
sc.fit(X_train)
X_train_std = sc.transform(X_train)
X_test_std = sc.transform(X_test)`上述代码将数据集划分为训练集和测试集，并进行预处理。
### 2、正常的感知机
`from sklearn.linear_model import Perceptron
ppn = Perceptron(n_iter=40, eta0=0.1, random_state=0)
ppn.fit(X_train_std, y_train)
y_pred = ppn.predict(X_test_std)
print accuracy_score(y_test, y_pred)`这里设置感知机参数n_iter为40，eta0为0.1，进行模型拟合，最后得到预测的正确率为：0.822222222222。
可以看到现在的正确率并不如人意，正常情况下，我们需要手动更改参数，继续进行模型训练。
### 3、利用hyperopt的感知机
首先定义评估函数，即是在2中使用过的感知机模型，由于fmin函数求的是最小值，故取负的auc值。
`def percept(args):
    global X_train_std, y_train, y_test
    ppn = Perceptron(n_iter=int(args["n_iter"]), eta0=args["eta"])
    ppn.fit(X_train_std, y_train)
    y_pred = ppn.predict(X_test_std)
    return -accuracy_score(y_test, y_pred)`接下来定义了参数空间，hp优化算法能识别的表达式如下：
hp.choice(label, options)
返回其中一个选项，它应该是一个列表或元组。options元素本身可以是[嵌套]随机表达式。在这种情况下，仅出现在某些选项中的随机选择(stochastic choices)将成为条件参数。
hp.randint(label, upper)
返回范围:[0，upper]中的随机整数。这种分布的语义是意味着邻整数值之间的损失函数没有更多的相关性，与更远的整数值相比较。例如，这是描述随机种子的适当分布。如果损失函数可能更多的与相邻整数值相关联，那么你或许应该用“量化”连续分布的一个，比如 quniform ， qloguniform ， qnormal 或 qlognormal 。
hp.uniform(label, low, high)
返回位于[low,hight]之间的均匀分布的值。
在优化时，这个变量被限制为一个双边区间。
hp.quniform(label, low, high, q)
返回一个值，如 round（uniform（low，high）/ q）* q
适用于目标仍然有点“光滑”的离散值，但是在它上下存在边界(双边区间)。
hp.loguniform(label, low, high)
返回根据 exp（uniform（low，high）） 绘制的值，以便返回值的对数是均匀分布的。
优化时，该变量被限制在[exp（low），exp（high）]区间内。
hp.qloguniform(label, low, high, q)
返回类似 round（exp（uniform（low，high））/ q）* q 的值
适用于一个离散变量，其目标是“平滑”，并随着值的大小变得更平滑，但是在它上下存在边界(双边区间)。
hp.normal(label, mu, sigma)
返回正态分布的实数值，其平均值为 mu ，标准偏差为 σ。优化时，这是一个无约束(unconstrained)的变量。
hp.qnormal(label, mu, sigma, q)
返回像 round（正常（mu，sigma）/ q）* q 的值
适用于离散值，可能需要在 mu 附近的取值，但从基本上上是无约束(unbounded)的。
hp.lognormal(label, mu, sigma)(对数正态分布)
返回根据 exp（normal（mu，sigma）） 绘制的值，以便返回值的对数正态分布。优化时，这个变量被限制为正值。
hp.qlognormal(label, mu, sigma, q)
返回类似 round（exp（normal（mu，sigma））/ q）* q 的值，
适用于一个离散变量，其目标是“平滑”，并随着值的大小变得更平滑，变量的大小是从一个边界开始的(单边区间)。
from hyperopt import fmin, tpe, hp
space = {"n_iter": hp.randint("n_iter", 50), 
        "eta": hp.uniform("eta", 0.05, 0.5)}最后是模型的参数选取
best = fmin(percept, space, algo=tpe.suggest, max_evals=100)
print best
print percept(best)algo是用户参数选择的搜索算法，目前hyperopt支持的搜索算法有随机搜索(对应是hyperopt.rand.suggest)，模拟退火(对应是hyperopt.anneal.suggest)，TPE算法。
max_evals是模型评价次数。
通过上述代码，最后得到的感知机模型参数为：{‘n_iter’: 29, ‘eta’: 0.10959845686587069}，
正确率：-0.977777777778
相比不使用hyperopt选取的参数，提高了许多，同时也降低了调参的难度。


