# 理解线性回归（三）——岭回归Ridge Regression - bigfacesafdasgfewgf - CSDN博客





2014年11月06日 16:52:51[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：11533








# 理解线性回归（三）——岭回归Ridge Regression



# 1. 基本模型




    岭回归是在前篇介绍普通最小二乘法回归（ordinary least squares regression）的基础上，加入了对表示系数的L2-norm约束。其目标函数为：

![](https://img-blog.csdn.net/20141106165619959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    我们可以看到the first term 表示对回归表示后的误差最小，the second term是表示系数的均方根最小化。需要之处的是，这种对系数的约束在sparse coding, dictionary learning也是经常用到的。而且经常会有不同的范数norm要求。

    常用的几个范数介绍：（还有一个L0范数，是计算向量中非零值的个数，这个求解起来非常麻烦！！）




![](https://img-blog.csdn.net/20141106172143504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








# 2. 如何调用




    介绍完了岭回归的目标函数，我们跳过求解的数学过程，直接看看在scikit-learn中怎么调用函数求解：




    首先介绍Ridge的函数参数表：





```python
class sklearn.linear_model.Ridge(alpha=1.0, fit_intercept=True, normalize=False, copy_X=True, max_iter=None, tol=0.001, solver='auto')
```




alpha：上面提到的两项之间的权重；

fit_intercept：默认为true，数据可以拦截，没有中心化；


normalize：输入的样本特征归一化，默认false；


copy_X：复制或者重写；

max_iter：最大迭代次数；

tol: 控制求解的精度；

solver：求解器，有auto, svd, cholesky, sparse_cg, lsqr几种，一般我们选择auto，一些svd，cholesky也都是稀疏表示中常用的omp求解算法中的知识，大家有时间可以去了解。

    Ridge函数会返回一个clf类，里面有很多的函数，一般我们用到的有：

clf.fit(X, y)：输入训练样本数据X，和对应的标记y；

clf.predict(X)：利用学习好的线性分类器，预测标记，一般在fit之后调用；

clf.corf_: 输入回归表示系数

 还有其他的函数介绍见官方网址。




附上一个example： 


```python
from sklearn.linear_model import Ridge
import numpy as np
n_samples, n_features = 10, 5
np.random.seed(0)
y = np.random.randn(n_samples)
X = np.random.randn(n_samples, n_features)
clf = Ridge(alpha=1.0)
clf.fit(X, y) 
Ridge(alpha=1.0, copy_X=True, fit_intercept=True, max_iter=None,
      normalize=False, solver='auto', tol=0.001)
```

上面这个例子中的数据都是random出来的。




其实总的来说，就是先把Ridge的参数是指一下得到clf；

然后把训练样本和标记先fit学习出来；

然后在调用predict预测；

一气呵成~~哇咔咔。。







    最后附上官方的Ridge说明文档：

http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.Ridge.html#sklearn.linear_model.Ridge.decision_function








Over~~















