# 【Python-ML】神经网络激励函数-双曲正切(hyperbolic tangent,tanh)函数 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月27日 09:15:28[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1266
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月27日
@author: Jason.F
@summary: 前馈神经网络激励函数-双曲正切(hyperbolic tangent,tanh)函数，经过缩放的逻辑斯蒂函数，输出值的范围更广，在开区间(-1,1)，有利于加速反向传播算法的收敛
'''
import numpy as np
import time
import matplotlib.pyplot as plt
       
if __name__ == "__main__":   
    start = time.clock()  
    
    def tanh(z):
        e_p = np.exp(z)
        e_m = np.exp(-z)
        return (e_p -e_m)/(e_m+e_p)
    def net_input(X,w):
        z=X.dot(w)
        return z
    def logistic(z):
        return 1.0/(1.0+np.exp(-z))
    
    #W:array,shape=[n_output_units,n_hidden_units+1],weight matrix for hidden layer --> output layer
    #note that first column (A[:][0]=1) are the bias units.
    W=np.array([[1.1,1.2,1.3,0.5],[0.1,0.2,0.4,0.1],[0.2,0.5,2.1,1.9]])
    #A:array,shape=[n_hiddern+1,n_samples],Activation of hidden layer.
    #note that first element (A[0][0]=1) is the bias unit.
    A=np.array([[1.0],[0.1],[0.3],[0.7]])
    #Z:array,shape=[n_output_units,n_samples],Net input of the output layer.
    Z=W.dot(A)
    y_probas = tanh(Z)
    print ('Probabilities:\n',y_probas)
    print (y_probas.sum())
    y_class = np.argmax(Z,axis=0)
    print ('predicted class label:%d'%y_class[0])
    #和逻辑斯蒂函数比较
    z=np.arange(-5,5,0.005)
    log_act = logistic(z)
    tanh_act =tanh(z)
    plt.ylim([-1.5,1.5])
    plt.xlabel('net input $z$')
    plt.ylabel('activation $\phi(z)$')
    plt.axhline(1,color='black',linestyle='--')
    plt.axhline(0.5,color='black',linestyle='--')
    plt.axhline(0,color='black',linestyle='--')
    plt.axhline(-1,color='black',linestyle='--')
    plt.plot(z,tanh_act,linewidth=2,color='black',label='tanh')
    plt.plot(z,log_act,linewidth=2,color='lightgreen',label='logistic')
    plt.legend(loc='lower right')
    plt.tight_layout()
    plt.show() 
    
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```

结果：



```
('Probabilities:\n', array([[ 0.96108983],
       [ 0.3004371 ],
       [ 0.97621774]]))
2.23774466472
predicted class label:2
finish all in 14.5718582269
```

![](https://img-blog.csdn.net/20180127091519284?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





