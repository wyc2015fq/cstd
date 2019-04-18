# 【Python-ML】集成多数投票分类器-训练评估调优 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月20日 11:59:04[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1119
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月19日

@author: Jason.F
@summary: 集成学习，多数投票分类器
'''
from sklearn.base import BaseEstimator
from sklearn.base import ClassifierMixin
from sklearn.preprocessing import LabelEncoder
from sklearn.externals import six
from sklearn.base import clone
from sklearn.pipeline import _name_estimators
import numpy as np
import time
from sklearn import datasets
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.cross_validation import cross_val_score
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.pipeline import Pipeline
from sklearn.metrics import roc_curve
from sklearn.metrics import auc
import matplotlib.pyplot as plt
from itertools import product
from sklearn.grid_search import GridSearchCV
class MajorityVoteClassifier(BaseEstimator,ClassifierMixin):
    '''
    A majority vote ensemble classifier
    Paramters:
    classifiers:array-like,shape=[n_classifiers],Different classifiers for the ensemble
    vote:str,{'classlabel','probability'},Default:'classlabel',
         if 'classlabel' the prediction is based on the argmax of class labels,
         else if 'probability' ,the argmax of the sum of probabilities is used to predict the class label
         (recommened for calibrated classifier). 
    weights:array-like,shape=[n_classifiers],Optional,default:None
         if a list of 'int' or 'float' values are provided,the classifiers are weighted by importance;
         Uses uniform weights if 'weights=None'
    '''
    def __init__(self,classifiers,vote='classlabel',weights=None):
        self.classifiers = classifiers
        self.named_classifiers = {key:value for key,value in _name_estimators(classifiers)}
        self.vote = vote
        self.weights=weights
    
    def fit(self,X,y):
        '''
        Fit classifiers
        Parameters:
        X:{array-like,sparse matrix},shape=[n_samples,n_features],Matrix of training samples.
        y:array-like,shape=[n_sample],Vector of target class labels.
        Returns:
        self:object
        '''
        #use LabelEncoder to ensure class labels start with 0,which is important for np.argmax caoll in self.predict
        self.lablenc_=LabelEncoder()
        self.lablenc_.fit(y)
        self.classes_=self.lablenc_.classes_
        self.classifiers_=[]
        for clf in self.classifiers:
            fitted_clf=clone(clf).fit(X,self.lablenc_.transform(y))
            self.classifiers_.append(fitted_clf)
        return self
    
    def predict(self,X):
        '''
        Predict class labels for X.
        Parameters:
        X:{array-like,sparse matrix},Shape=[n_samples,n_features],Matrix of testing samples.
        Returns:
        maj_vote:array-like,shape=[n_samples],Predicted class labels.
        '''
        if self.vote=='probability':
            maj_vote=np.argmax(self.predict_proba(X),axis=1)
        else: #'classlabel' vote
            #collect results from clf.predict Calls
            predictions = np.asarray([clf.predict(X) for clf in self.classifiers_]).T
            maj_vote = np.apply_along_axis(lambda x:np.argmax(np.bincount(x,weights=self.weights)), axis=1, arr=predictions)
        maj_vote =self.lablenc_.inverse_transform(maj_vote)
        return maj_vote
    
    def predict_proba(self,X):
        '''
        Predict class probabilities for X.
        Parameters:
        X:{array-like,sparse matrix},shape={n_samples,n_features},Training vectors,where n_samples is the number of samples and n_features is the number of features.
        Returns:
        avg_proba:array-like,shape=[n_samples,n_classes],Weighted average probability for each class per sample.
        '''
        probas=np.asarray([clf.predict_proba(X) for clf in self.classifiers_])
        avg_proba=np.average(probas, axis=0, weights=self.weights)
        return avg_proba
    
    def get_params(self,deep=True):
        '''
        Get classifier parameter names for GridSearch.
        '''
        if not deep:
            return super(MajorityVoteClassifier,self).get_params(deep=False)
        else:
            out =self.named_classifiers.copy()
            for name,step in six.iteritems(self.named_classifiers):
                for key,value in six.iteritems(step.get_params(deep=True)):
                    out['%s__%s'%(name,key)] =value
            return out
        
if __name__ == "__main__":   
    start = time.clock()  
    
    iris=datasets.load_iris()
    X,y = iris.data[50:,[1,2]],iris.target[50:]
    le=LabelEncoder()
    y=le.fit_transform(y)
    X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=0.5,random_state=1)
    #第一步：构建集成分类器，并训练评估
    #构建单分类器，构造逻辑斯蒂回归分类器、决策树分类器、K-近邻分类器三种
    clf1=LogisticRegression(penalty='l2',C=0.001,random_state=0)
    clf2=DecisionTreeClassifier(max_depth=1,criterion='entropy',random_state=0)
    clf3=KNeighborsClassifier(n_neighbors=1,p=2,metric='minkowski')
    pipe1=Pipeline([['sc',StandardScaler()],['clf',clf1]])
    #决策树不依赖特征缩放，所以不做标准化
    pipe3=Pipeline([['sc',StandardScaler()],['clf',clf3]])
    mv_clf=MajorityVoteClassifier(classifiers=[pipe1,clf2,pipe3])#构造集成分类器
    clf_labels=['Logistic Regression','Decision Tree','KNN','Majority Voting']
    print ('10-fold cross validation:\n')
    all_clf =[pipe1,clf2,pipe3,mv_clf]
    for clf,label in zip(all_clf,clf_labels):
        scores=cross_val_score(estimator=clf,X=X_train,y=y_train,cv=10,scoring='roc_auc')
        print ('ROC AUC:%0.2f (+/- %0.2f) [%s]' % (scores.mean(),scores.std(),label)) 
    #第二步：评估和调优集成分类器
    #roc对分类系统的泛化性能进行无偏差估计
    colors=['black','orange','blue','green']
    linestyles =[':','--','-.','-']
    for clf,label,clr,ls in zip(all_clf,clf_labels,colors,linestyles):
        #assuming the label of the positive class is 1.
        y_pred = clf.fit(X_train,y_train).predict_proba(X_test)[:,1]
        fpr,tpr,thresholds = roc_curve(y_true=y_test,y_score=y_pred)
        roc_auc=auc(x=fpr,y=tpr)
        plt.plot(fpr,tpr,color=clr,linestyle=ls,label='%s (auc =%0.2f )'%(label,roc_auc))
    plt.legend(loc='lower right')
    plt.plot([0,1],[0,1],linestyle='--',color='gray',linewidth=2)
    plt.xlim([-0.1,1.1])
    plt.ylim([-0.1,1.1])
    plt.grid()
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.show()
    #通过决策区域评估分类器
    sc=StandardScaler()
    X_train_std=sc.fit_transform(X_train)
    x_min = X_train_std[:,0].min()-1
    x_max = X_train_std[:,0].max()+1
    y_min = X_train_std[:,1].min()-1
    y_max = X_train_std[:,1].max()+1
    xx,yy =np.meshgrid(np.arange(x_min,x_max,0.1),np.arange(y_min,y_max,0.1))
    f,axarr= plt.subplots(nrows=2,ncols=2,sharex='col',sharey='row',figsize=(7,5))
    for idx,clf,tt in zip(product([0,1],[0,1]),all_clf,clf_labels):
        clf.fit(X_train_std,y_train)
        Z=clf.predict(np.c_[xx.ravel(),yy.ravel()])
        Z=Z.reshape(xx.shape)
        axarr[idx[0],idx[1]].contourf(xx,yy,Z,alpha=0.3)
        axarr[idx[0],idx[1]].scatter(X_train_std[y_train==0,0],X_train_std[y_train==0,1],c='blue',marker='^',s=50)
        axarr[idx[0],idx[1]].scatter(X_train_std[y_train==1,0],X_train_std[y_train==1,1],c='red',marker='o',s=50)
        axarr[idx[0],idx[1]].set_title(tt)
    plt.text(-3.5,-4.5,s='Sepal width [standardized]',ha='center',va='center',fontsize=12)
    plt.text(-10.5,4.5,s='Sepal length [standardized]',ha='center',va='center',fontsize=12,rotation=90)
    plt.show()
    #调优集成分类器，通过网格搜索来调优逻辑斯蒂回归分类器的正则化参数C以及决策树的深度
    params={'pipeline-1__clf__C':[0.001,0.1,100.0],'decisiontreeclassifier__max_depth':[1,2]}
    grid=GridSearchCV(estimator=mv_clf,param_grid=params,cv=10,scoring='roc_auc')
    grid.fit(X_train,y_train)
    for params,mean_score,scores in grid.grid_scores_:
        print ('%0.3f +/- %0.2f %r' %(mean_score,scores.std()/2,params))
    print ('Best Parameters:%s'%grid.best_params_)
    print ('Accuracy:%.2f'%grid.best_score_)
    
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```

结果：



```
10-fold cross validation:

ROC AUC:0.92 (+/- 0.20) [Logistic Regression]
ROC AUC:0.92 (+/- 0.15) [Decision Tree]
ROC AUC:0.93 (+/- 0.10) [KNN]
ROC AUC:0.97 (+/- 0.10) [Majority Voting]
0.967 +/- 0.05 {'pipeline-1__clf__C': 0.001, 'decisiontreeclassifier__max_depth': 1}
0.967 +/- 0.05 {'pipeline-1__clf__C': 0.1, 'decisiontreeclassifier__max_depth': 1}
1.000 +/- 0.00 {'pipeline-1__clf__C': 100.0, 'decisiontreeclassifier__max_depth': 1}
0.967 +/- 0.05 {'pipeline-1__clf__C': 0.001, 'decisiontreeclassifier__max_depth': 2}
0.967 +/- 0.05 {'pipeline-1__clf__C': 0.1, 'decisiontreeclassifier__max_depth': 2}
1.000 +/- 0.00 {'pipeline-1__clf__C': 100.0, 'decisiontreeclassifier__max_depth': 2}
Best Parameters:{'pipeline-1__clf__C': 100.0, 'decisiontreeclassifier__max_depth': 1}
Accuracy:1.00
finish all in 6.92493746669
```
![](https://img-blog.csdn.net/20180120115836001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20180120115844543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





