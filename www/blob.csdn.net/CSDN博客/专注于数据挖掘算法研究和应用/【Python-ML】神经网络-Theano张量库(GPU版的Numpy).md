# 【Python-ML】神经网络-Theano张量库(GPU版的Numpy) - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月26日 17:06:10[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：219
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)

所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月26日
@author: Jason.F
@summary: pip install Theano，
Theano,对于张量能够高效地实现、编译和评估数学表达式，支持在GPU上运行，利用GPU中巨大内存带宽及浮点数运算能力
实现一个基于最小二乘法的线性回归
'''
import theano
from theano import tensor as T
import numpy as np
import matplotlib.pyplot as plt

print(theano.config.floatX)#查看浮点变量的设置
theano.config.floatX='float32'
print(theano.config.device)#查看是在CPU还是GPU上
X_train =np.asarray([[0.0],[1.0],[2.0],[3.0],[4.0],[5.0],[6.0],[7.0],[8.0],[9.0]],dtype=theano.config.floatX)
y_train =np.asarray([1.0,1.3,3.1,2.0,5.0,6.3,6.6,7.4,8.0,9.0],dtype=theano.config.floatX)
def train_linreg(X_train,y_train,eta,epochs):
    costs=[]
    #initial arrays
    eta0=T.fscalar('eta0')
    y=T.fvector(name='y')
    X=T.fmatrix(name='X')
    w=theano.shared(np.zeros(shape=(X_train.shape[1]+1),dtype=theano.config.floatX),name='w')
    #calculate cost
    net_input = T.dot(X,w[1:])+w[0]
    errors=y-net_input
    cost = T.sum(T.pow(errors,2))
    #perform gradient update
    gradient = T.grad(cost,wrt=w)
    update =[(w,w-eta0*gradient)]
    #compile model
    train = theano.function(inputs=[eta0],outputs=cost,updates=update,givens={X:X_train,y:y_train})
    for _ in range(epochs):
        costs.append(train(eta))
        
    return costs,w

costs,w = train_linreg(X_train, y_train, eta=0.001, epochs=10)
plt.plot(range(1,len(costs)+1),costs)
plt.tight_layout()
plt.xlabel('Epoch')
plt.ylabel('Cost')
plt.show()    

def predict_linreg(X,w):
    Xt=T.matrix(name='X')
    net_input = T.dot(Xt,w[1:])+w[0]
    predict = theano.function(inputs=[Xt],givens={w:w},outputs=net_input)
    return predict(X)
plt.scatter(X_train,y_train,marker='s',s=50)
plt.plot(range(X_train.shape[0]),predict_linreg(X_train, w),color='gray',marker='o',markersize=4,linewidth=3)
plt.xlabel('x')
plt.ylabel('y')
plt.show()
```


结果：

![](https://img-blog.csdn.net/20180126170550671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180126170557967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




