# 逻辑回归和Python应用举例 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年05月09日 17:08:22[QuJack](https://me.csdn.net/A784586)阅读数：834








**逻辑回归和Python应用举例**



1. 概率：

     1.1 定义   概率(P)robability: 对一件事情发生的可能性的衡量

     1.2 范围   0 <= P <= 1

     1.3 计算方法： 

          1.3.1 根据个人置信

          1.3.2 根据历史数据

          1.3.3 根据模拟数据

     1.4 条件概率：



2. Logistic Regression (逻辑回归)

![](https://img-blog.csdn.net/20170509165218883?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




2.2 基本模型

         测试数据为X（x0，x1，x2···xn）

         要学习的参数为： Θ（θ0，θ1，θ2，···θn）

![](https://img-blog.csdn.net/20170509165223180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        处理二值数据，引入Sigmoid函数时曲线平滑化 

![](https://img-blog.csdn.net/20170509165455792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170509165510445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170509165519152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




 学习率

                      同时对所有的θ进行更新

                      重复更新直到收敛  


  利用梯度下降算法：


```python
import numpy as np
import random

def genData(numPoints,bias,variance):
    x = np.zeros(shape=(numPoints,2))
    y = np.zeros(shape=(numPoints))
    for i in range(0,numPoints):
        x[i][0]=1
        x[i][1]=i
        y[i]=(i+bias)+random.uniform(0,1)+variance
    return x,y

def gradientDescent(x,y,theta,alpha,m,numIterations):
    xTran = np.transpose(x)
    for i in range(numIterations):
        hypothesis = np.dot(x,theta)
        loss = hypothesis-y
        cost = np.sum(loss**2)/(2*m)
        gradient=np.dot(xTran,loss)/m
        theta = theta-alpha*gradient
        print ("Iteration %d | cost :%f" %(i,cost))
    return theta

x,y = genData(100, 25, 10)
print ("x:",x)
print ("y:",y)

m,n = np.shape(x)
n_y = np.shape(y)

print("m:"+str(m)+" n:"+str(n)+" n_y:"+str(n_y))

numIterations = 100000
alpha = 0.0005
theta = np.ones(n)
theta= gradientDescent(x, y, theta, alpha, m, numIterations)
print(theta)
```












