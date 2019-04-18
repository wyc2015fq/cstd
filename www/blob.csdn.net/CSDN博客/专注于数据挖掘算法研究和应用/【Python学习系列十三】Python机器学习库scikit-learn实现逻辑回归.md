# 【Python学习系列十三】Python机器学习库scikit-learn实现逻辑回归 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月13日 08:56:46[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2702
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









逻辑回归基础知识可参考：http://blog.csdn.net/fjssharpsword/article/details/54580552

python内部算法已经实现了，最重要是理解y=f(x)的最小化损失函数并通过梯度下降法求解参数。

这里通过Python机器学习库scikit-learn实现，代码如下：



```python
# -*- coding: utf-8 -*-

import numpy as np
import urllib
from sklearn import preprocessing
from sklearn.feature_selection import RFE
from sklearn.linear_model import LogisticRegression
from sklearn import metrics
from sklearn.ensemble import ExtraTreesClassifier
import time

def main(): 
    #数据加载
    # load the CSV file as a numpy matrix
    dataset = np.loadtxt('D:\sample.csv', delimiter=",")
    # separate the data from the target attributes
    X = dataset[:,0:4]
    y = dataset[:,4]

    #数据标准化
    # normalize the data attributes
    normalized_X = preprocessing.normalize(X)
    # standardize the data attributes
    standardized_X = preprocessing.scale(X)

    #特征选取
    #model = LogisticRegression()
    #create the RFE model and select 3 attributes
    #rfe = RFE(model, 4)
    #rfe = rfe.fit(X, y)
    # summarize the selection of the attributes
    #print(rfe.support_)
    #print(rfe.ranking_)
    model = ExtraTreesClassifier()
    model.fit(X, y)
    # display the relative importance of each attribute
    print(model.feature_importances_)

    #模型训练
    model = LogisticRegression()
    model.fit(X, y)
    print(model)

    #模型预测
    # make predictions
    expected = y
    predicted = model.predict(X)
    # summarize the fit of the model
    print(metrics.classification_report(expected, predicted))
    print(metrics.confusion_matrix(expected, predicted))
    
   
#执行
if __name__ == '__main__':  
    start = time.clock()  
    main()  
    end = time.clock()  
    print('finish all in %s' % str(end - start))
```
sample.csv数据特点如下：最后一列是标签





```
32,0,445,5,0
68,1,415,5,0
44,1,235,30,1
40,0,444,5,0
83,0,466,5,0
52,1,573,5,0
33,0,445,5,0
```


执行结果：






```
[ 0.15453444  0.00727297  0.63061708  0.2075755 ]
LogisticRegression(C=1.0, class_weight=None, dual=False, fit_intercept=True,
          intercept_scaling=1, max_iter=100, multi_class='ovr', n_jobs=1,
          penalty='l2', random_state=None, solver='liblinear', tol=0.0001,
          verbose=0, warm_start=False)
             precision    recall  f1-score   support

        0.0       0.66      0.87      0.75    268498
        1.0       0.66      0.35      0.45    188407

avg / total       0.66      0.66      0.63    456905

[[234680  33818]
 [123182  65225]]
finish all in 12.8994016037
```







