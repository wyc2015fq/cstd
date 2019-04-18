# 【Python-ML】SKlearn库特征选择SBS算法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月17日 16:02:44[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1079
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月17日
@author: Jason.F
@summary: 特征选择-序列后向选择算法(Sequential Backward Selection,SBS)
'''
import pandas as pd
import numpy as np
import time
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.base import clone
from itertools import combinations
from sklearn.metrics import accuracy_score
from sklearn.neighbors import KNeighborsClassifier
import matplotlib.pyplot as plt
#SBS类
class SBS(object):
    def __init__(self,estimator,k_features,scoring=accuracy_score,test_size=0.2,random_state=1):
        self.scoring=scoring
        self.estimator=clone(estimator)
        self.k_features=k_features
        self.test_size=test_size
        self.random_state=random_state
        
    def fit(self,X,y):
        X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=self.test_size,random_state=self.random_state)
        dim=X_train.shape[1]
        self.indices_=tuple(range(dim))
        self.subsets_=[self.indices_]
        score=self._calc_score(X_train,X_test,y_train,y_test,self.indices_)
        self.scores_=[score]
        while dim>self.k_features:
            scores=[]
            subsets=[]
            for p in combinations(self.indices_,r=dim-1):
                score=self._calc_score(X_train,X_test,y_train,y_test,p)
                scores.append(score)
                subsets.append(p)
            best=np.argmax(scores)
            self.indices_=subsets[best]
            self.subsets_.append(self.indices_)
            dim-=1
            self.scores_.append(scores[best])
        self.k_score_=self.scores_[-1]
        return self
    
    def transform(self,X):
        return X[:,self.indices_]
    
    def _calc_score(self,X_train,X_test,y_train,y_test,indices):
        self.estimator.fit(X_train[:,indices],y_train)
        y_pred=self.estimator.predict(X_test[:,indices])
        score=self.scoring(y_test,y_pred)
        return score
        
if __name__ == "__main__":   
    start = time.clock() 
    #导入数据
    df_wine = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/wine/wine.data',header=None)
    df_wine.columns=['Class label','Alcohol','Malic acid','Ash','Alcalinity of ash','Magnesium','Total phenols','Flavanoids','Nonflavanoid phenols','Proanthocyanins','Color intensity','Hue','OD280/OD315 of diluted wines','Proline']
    X,y=df_wine.iloc[:,1:].values,df_wine.iloc[:,0].values
    X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=0.2,random_state=0)
    stdsc=StandardScaler()#标准化
    X_train_std=stdsc.fit_transform(X_train)
    X_test_std=stdsc.fit_transform(X_test)
    #SBS训练
    knn=KNeighborsClassifier(n_neighbors=2)
    sbs=SBS(knn,k_features=1)
    sbs.fit(X_train_std,y_train)
    k_feat=[len(k) for k in sbs.subsets_]
    plt.plot(k_feat,sbs.scores_,marker='o')
    plt.ylim([0.7,1.1])
    plt.ylabel('Accuracy')
    plt.xlabel('Number of features')
    plt.grid()
    plt.show()
    
    #在原始特征上的训练
    knn.fit(X_train_std,y_train)
    print ('Training accuracy:',knn.score(X_train_std,y_train))
    print ('Test accuracy:',knn.score(X_test_std,y_test)) #存在过拟合
    #选定SBS得到的最好5个特征来比较
    k5=list(sbs.subsets_[8])
    print (df_wine.columns[1:][k5])
    knn.fit(X_train_std[:,k5],y_train)
    print ('Training accuracy:',knn.score(X_train_std[:,k5],y_train))
    print ('Test accuracy:',knn.score(X_test_std[:,k5],y_test)) #过拟合得到缓解

    end = time.clock()    
    print('finish all in %s' % str(end - start))
```

结果：

![](https://img-blog.csdn.net/20180117160216006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



```
('Training accuracy:', 0.9859154929577465)
('Test accuracy:', 0.91666666666666663)
Index([u'Alcohol', u'Malic acid', u'Ash', u'Color intensity', u'Proline'], dtype='object')
('Training accuracy:', 0.95070422535211263)
('Test accuracy:', 0.97222222222222221)
finish all in 21.7107086315
```





