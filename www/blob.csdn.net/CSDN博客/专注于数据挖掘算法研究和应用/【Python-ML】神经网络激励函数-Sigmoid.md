# 【Python-ML】神经网络激励函数-Sigmoid - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月27日 08:50:30[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：329
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月27日
@author: Jason.F
@summary: 前馈神经网络激励函数-Sigmoid，逻辑斯蒂函数
'''
import numpy as np
import time
       
if __name__ == "__main__":   
    start = time.clock()  
    
    X= np.array([[1,1.4,1.5]])
    w=np.array([0.0,0.2,0.4])
    def net_input(X,w):
        z=X.dot(w)
        return z
    def logistic(z):
        return 1.0/(1.0+np.exp(-z))
    def logistic_activation(X,w):
        z=net_input(X, w)
        return logistic(z)
    print ('P(y=1|x)=%.3f'%logistic_activation(X, w)[0])
    
    #W:array,shape=[n_output_units,n_hidden_units+1],weight matrix for hidden layer --> output layer
    #note that first column (A[:][0]=1) are the bias units.
    W=np.array([[1.1,1.2,1.3,0.5],[0.1,0.2,0.4,0.1],[0.2,0.5,2.1,1.9]])
    #A:array,shape=[n_hiddern+1,n_samples],Activation of hidden layer.
    #note that first element (A[0][0]=1) is the bias unit.
    A=np.array([[1.0],[0.1],[0.3],[0.7]])
    #Z:array,shape=[n_output_units,n_samples],Net input of the output layer.
    Z=W.dot(A)
    y_class = np.argmax(Z,axis=0)
    print ('predicted class label:%d'%y_class[0])
    
    
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```


结果：



```
P(y=1|x)=0.707
predicted class label:2
finish all in 0.00122912770087
```





