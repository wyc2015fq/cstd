# 系统学习机器学习之神经网络（八） --ADALINE网络 - 工作笔记 - CSDN博客





2017年01月09日 13:28:47[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7510









`转自：http://www.jianshu.com/p/3c271d59b14c`

`自适应线性神经网络:ADAptive LInear NEuron (Adaline)`

# 大纲

### 1. look --- 比Rosenblatt感知器算法的优势

### 2. write --- 吐槽实验结果

### 3. code --- python

# 对比Rosenblatt

憋说话，先上图　`-.-`

> 
Rosenblatt的计算模型


![](https://img-blog.csdn.net/20170109132614411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Rosenblatt



> 
Adaline的计算模型


![](https://img-blog.csdn.net/20170109132626537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Adaline



###### 找不同：激活函数用阶跃函数换成了连续型函数，用一个Quantizer函数（量化函数，类似AD采样）进行类别预测

激活函数：用线性函数代替阶跃函数进行误差计算和权值更新

量化函数：类似Rosenblatt模型的激活函数，能预测对应输入的类别

### 梯度下降最小化代价函数
- Adaline模型相比Rosenblatt模型，定义了代价函数（cost function），最小化代价函数是许多机器学习算法的主要思想。
- Adaline模型中，代价函数用的是均方误差（Sum of Squared Errors ：SSE）

![](https://img-blog.csdn.net/20170109132643940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

好处：可以微分，是凸函数

可以用梯度下降的方法找到均方误差最小的权值

寻找最小均方误差就像下山一样，每次算法循环都相当于下降一步，下降一步的歩幅取决于学习率，与图中的权值点的切线斜率相关

![](https://img-blog.csdn.net/20170109132655174?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

梯度下降示意图


每次权值逼近均方误差最小点的过程就是梯度下降（Gradient Descent）

![](https://img-blog.csdn.net/20170109132706552?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

证明一下偏导函数计算方法

![](https://img-blog.csdn.net/20170109132717878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

证明偏导函数计算方法


最终的权值更新公式如下

![](https://img-blog.csdn.net/20170109132728894?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

权值更新公式


Adaline算法是基于全部的训练数据，而感知器算法是每个样本都要计算一次误差，Adaline的处理方法有点像批处理的感觉。

> 
Adaline的更新
`self.w_[1:] += self.eta * X.T.dot(errors)`

Perceptron的更新
`update = self.eta * (target - self.predict(xi))`


### 学习率的影响和选择

学习率设置为0.01的时候，结果如左图，均方误差最小的点是第一个点，然后越来越大。当学习率设置为0.0001的时候，结果如右图，误差在逐渐减小，但是没有收敛的趋势。

![](https://img-blog.csdn.net/20170109132741553?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

对比学习率对于误差的影响


学习率设置，偏大偏小都会大幅降低算法效率。采取的方法是进行数据标准化（standardization）公式如下

![](https://img-blog.csdn.net/20170109132754051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

标准化公式


经过标准化的数据，会体现出一些数学分布的特点。标准化后，我们再次使用0.01的学习率进行训练分类。

![](https://img-blog.csdn.net/20170109132805676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

标准化后的误差收敛


最后的分类平面如下图


![](https://img-blog.csdn.net/20170109132819100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Adaline分类结果

ADALine是神经网络的鼻祖模型之一，提出的自适应线性激活函数，允许输出任意值，线性逼近一个函数式，进行模式联想。





# 然后进入Coding环节~

```python
```python
# encoding:utf-8
__author__ = 'Matter'

import numpy as np

class AdalineGD(object):
    # 自适应线性神经网络:ADAptive LInear NEuron (Adaline)

    # --------  参数  --------#
    # 参数1   eta:float   学习率
    # 参数2   n_iter:int  循环次数
    # --------  属性  --------#
    # 属性1   w_:1d_array     拟合后权值
    # 属性2   errors_:list    每次迭代的错误分类

    # 初始化
    def __init__(self,eta=0.01,n_iter=10):
        self.eta = eta
        self.n_iter = n_iter

    # 训练模型
    def fit(self,X,y):
        self.w_ = np.zeros(1+X.shape[1])
        self.errors_ = []
        self.cost_ = []

        for i in range(self.n_iter):
            output = self.net_input(X)
            errors = (y-output)
            self.w_[1:] += self.eta * X.T.dot(errors)
            self.w_[0] += self.eta * errors.sum()
            cost = (errors ** 2).sum()/2.0
            self.cost_.append(cost)
        return self


    # 输入和权值的点积,即公式的z函数,图中的net_input
    def net_input(self,X):
        return np.dot(X,self.w_[1:]) + self.w_[0]

    # 线性激活函数
    def activation(self,X):
        return self.net_input(X)

    # 利用阶跃函数返回分类标签
    def predict(self,X):
        return np.where(self.activation(X)>=0.0,1,-1)
```
```




