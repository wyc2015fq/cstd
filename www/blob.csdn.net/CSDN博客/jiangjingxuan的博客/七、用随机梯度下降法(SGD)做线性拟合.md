# 七、用随机梯度下降法(SGD)做线性拟合 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:20:55[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：877










![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_136_default_big.jpeg)



scikit-learn的线性回归模型都是通过最小化成本函数来计算参数的，通过矩阵乘法和求逆运算来计算参数。当变量很多的时候计算量会非常大，因此我们改用梯度下降法，批量梯度下降法每次迭代都用所有样本，快速收敛但性能不高，随机梯度下降法每次用一个样本调整参数，逐渐逼近，效率高，本节我们来利用随机梯度下降法做拟合

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 梯度下降法

梯度下降就好比从一个凹凸不平的山顶快速下到山脚下，每一步都会根据当前的坡度来找一个能最快下来的方向。随机梯度下降英文是Stochastic gradient descend(SGD)，在scikit-learn中叫做SGDRegressor。



## 样本实验

依然用上一节的房价样本

```
X = [[50],[100],[150],[200],[250],[300]]
y = [[150],[200],[250],[280],[310],[330]]
```



创建sgd_regressor.py，内容如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

import matplotlib.pyplot as plt
from sklearn.linear_model import SGDRegressor
from sklearn.preprocessing import StandardScaler

plt.figure() # 实例化作图变量
plt.title('single variable') # 图像标题
plt.xlabel('x') # x轴文本
plt.ylabel('y') # y轴文本
plt.grid(True) # 是否绘制网格线

X_scaler = StandardScaler()
y_scaler = StandardScaler()
X = [[50],[100],[150],[200],[250],[300]]
y = [[150],[200],[250],[280],[310],[330]]
X = X_scaler.fit_transform(X)
y = y_scaler.fit_transform(y)
X_test = [[40],[400]] # 用来做最终效果测试
X_test = X_scaler.transform(X_test)

plt.plot(X, y, 'k.')

model = SGDRegressor()
model.fit(X, y.ravel())
y_result = model.predict(X_test)
plt.plot(X_test, y_result, 'g-')

plt.show() # 展示图像
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## ![](http://www.shareditor.com/uploads/media/my-context/0001/01/ffa8e70acccb6839a7075302dc22e7978a273ce4.png)

## 讲解一下

这里需要用StandardScaler来对样本数据做正规化，同时对测试数据也要做正规化



我们发现拟合出的直线和样本之间还是有一定偏差的，这是因为随机梯度是随着样本数量的增加不断逼近最优解的，也就是样本数量越多就越准确



## 优化效果

既然样本数多拟合的好，那么我们把已有的样本重复多次试一下，修改成如下：

```
X = [[50],[100],[150],[200],[250],[300],[50],[100],[150],[200],[250],[300],[50],[100],[150],[200],[250],[300],[50],[100],[150],[200],[250],[300],[50],[100],[150],[200],[250],[300],[50],[100],[150],[200],[250],[300],[50],[100],[150],[200],[250],[300],[50],[100],[150],[200],[250],[300]]
y = [[150],[200],[250],[280],[310],[330],[150],[200],[250],[280],[310],[330],[150],[200],[250],[280],[310],[330],[150],[200],[250],[280],[310],[330],[150],[200],[250],[280],[310],[330],[150],[200],[250],[280],[310],[330],[150],[200],[250],[280],[310],[330],[150],[200],
```



最终成像如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/8d86edfcb6d08de7664400132953f1282ac8cd75.png)



这回靠谱了许多，实事上，如果再继续重复样本，效果会更逼近




