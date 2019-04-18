# 【Python-ML】神经网络激励函数-Softmax - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月27日 09:07:43[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：370
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月27日
@author: Jason.F
@summary: 前馈神经网络激励函数-softmax函数，评估多类别分类任务中的类别概率
'''
import numpy as np
import time
       
if __name__ == "__main__":   
    start = time.clock()  
    
    def net_input(X,w):
        z=X.dot(w)
        return z
    def softmax(z):
        return np.exp(z)/np.sum(np.exp(z))
    def softmax_activation(X,w):
        z=net_input(X,w)
        return softmax(z)
    #W:array,shape=[n_output_units,n_hidden_units+1],weight matrix for hidden layer --> output layer
    #note that first column (A[:][0]=1) are the bias units.
    W=np.array([[1.1,1.2,1.3,0.5],[0.1,0.2,0.4,0.1],[0.2,0.5,2.1,1.9]])
    #A:array,shape=[n_hiddern+1,n_samples],Activation of hidden layer.
    #note that first element (A[0][0]=1) is the bias unit.
    A=np.array([[1.0],[0.1],[0.3],[0.7]])
    #Z:array,shape=[n_output_units,n_samples],Net input of the output layer.
    Z=W.dot(A)
    y_probas = softmax(Z)
    print ('Probabilities:\n',y_probas)
    print (y_probas.sum())
    y_class = np.argmax(Z,axis=0)
    print ('predicted class label:%d'%y_class[0])
    
    
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```




结果：



```
('Probabilities:\n', array([[ 0.40386493],
       [ 0.07756222],
       [ 0.51857284]]))
1.0
predicted class label:2
finish all in 0.00170994801643
```





