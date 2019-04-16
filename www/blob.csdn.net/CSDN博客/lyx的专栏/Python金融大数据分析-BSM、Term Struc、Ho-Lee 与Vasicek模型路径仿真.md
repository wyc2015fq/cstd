# Python金融大数据分析-BSM、Term Struc、Ho-Lee 与Vasicek模型路径仿真 - lyx的专栏 - CSDN博客





2016年12月29日 21:47:30[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：4167








        这一篇的代码是之前蒙特卡洛仿真改过来的，大家都知道，用MC绘制路径是一件很好玩的事情。在学习FRM的过程中，遇到了几种利率模型，Term Structure、Ho-Lee与Vasicek。

        这里我们不讨论BSM，只是作为程序的一部分而已，后面仿真也并不用到。

![](https://img-blog.csdn.net/20161229212410572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        第一个模型，Term structure model with no drift，也就是，没有趋势的利率波动模型，笔者不知道这里是错误的省略了dt后面的单位正态分布函数，还是故意就是这样的，反正笔者是加上去了，以后如果觉得应该是没有的，那么再来修正吧。

![](https://img-blog.csdn.net/20161229212637338?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        第二个模型就是带趋势的了，但是趋势是恒定的，如果趋势是不恒定的，那么就是Ho-Lee模型。

最后是大名鼎鼎的Vasicek模型，其实就是一种均值回归模型。

        整个代码如下：



```python
# -*- coding: utf-8 -*-
import numpy as np  
import pandas as pd  
import matplotlib.pyplot as plt  
import scipy.stats as scs  
'''
#initial parameters and introductions
created by luyixiao 2016/12/29
S0 = 100#initial stock price or other value 
r = 0.05#risk-free rate
sigma = 0.25#standard devitaion 
T = 2.0#simulation time
I = 10#the number of path you wanna simluate
M = 50#the step you wanna simulate
'''

def modelSimulation(modelType,S0 = 100,r = 0.05,sigma = 0.25,T = 2.0,I = 10,M = 50,lamda = 0.05,k = 0.1,theta = 102):
    dt = T/M  
    S = np.zeros((M + 1,I))  
    S[0] = S0  
    print S[0]  
    if modelType == 'BSM':
        for t in range(1,M+1):  
            S[t] = S[t-1]*np.exp((r-0.5*sigma**2)*dt+sigma*np.sqrt(dt)*np.random.standard_normal(I))  
    if modelType == 'TSnD':
        for t in range(1,M+1):       
            S[t] = S[t-1]+np.sign(np.random.uniform(-1,1,I))*sigma*np.sqrt(dt)*np.random.standard_normal(I)  
    if modelType == 'TSwD':
        for t in range(1,M+1):       
            S[t] = S[t-1]+lamda*np.sqrt(dt)+np.sign(np.random.uniform(-1,1,I))*sigma*np.sqrt(dt)*np.random.standard_normal(I)  
    if modelType == 'HoLee':
        for t in range(1,M+1):       
            lamdan = np.random.uniform(0.01,0.09)            
            S[t] = S[t-1]+lamdan*np.sqrt(dt)+np.sign(np.random.uniform(-1,1,I))*sigma*np.sqrt(dt)*np.random.standard_normal(I)  
    if modelType == 'Vasicek':
        for t in range(1,M+1):       
            S[t] = S[t-1]+k*(theta-S[t-1])+sigma*np.sqrt(dt)*np.random.standard_normal(I)  
    plt.plot(S[:,:],lw = 1.5)  
    plt.xlabel('time')  
    plt.ylabel('price')
    plt.title(modelType)
    plt.show()  
modelSimulation('TSnD')   
modelSimulation('TSwD')   
modelSimulation('HoLee')    
modelSimulation('Vasicek')
```
        最后能够显示的效果是这样的：

![](https://img-blog.csdn.net/20161229212842046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161229212858875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161229212913792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161229212935157?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



        相信这里大家就能看出每个模型的区别了。

        最后我们讨论一下Vasicek模型的参数k，也就是均值回归的速度。



```python
for i in range(0,5):
    modelSimulation('Vasicek',k = 0.1*i,theta=100)
```
        我们来看一下不同的k值下的路径图吧。

![](https://img-blog.csdn.net/20161229214121909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161229214129612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161229214135971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161229214141720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        我们看到，k值越大，回归的速度越快，那如果K很大的，读者可以自己试一下，其实就会相当于比例控制中出现系统的不稳定一样的情况。











