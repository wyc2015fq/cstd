# 预测Titanic号上的乘客生存概率_02_（LR_SVM_KNN_DTree_RF) - wsp_1138886114的博客 - CSDN博客





2018年08月05日 22:44:33[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：152标签：[Titanic号乘客生存预测																[多模型与预测对比](https://so.csdn.net/so/search/s.do?q=多模型与预测对比&t=blog)](https://so.csdn.net/so/search/s.do?q=Titanic号乘客生存预测&t=blog)
个人分类：[机器学习](https://blog.csdn.net/wsp_1138886114/article/category/7692618)









- - - - - [1. 导入数据，数据集拆分](#1-导入数据数据集拆分)
- [2. 训练并预测](#2-训练并预测)
- [3. 使用其它模型训练](#3-使用其它模型训练)







```python
import numpy as np
import pandas as pd
from sklearn.linear_model import LogisticRegression
from sklearn import metrics
from sklearn import model_selection
from matplotlib import pyplot as plt
%matplotlib inline
```

##### 1. 导入数据，数据集拆分

```
train_src=pd.read_csv('train.csv')

train=train_src[['Survived','Pclass','Sex', 
                'Age','SibSp','Parch','Fare']]             #筛选六个变量作为预测变量(特征)
train['Sex']=train['Sex'].replace({'male':1,'female':0})   #把Sex变量的取值male替换为1，female替换为0

age_mean=train['Age'].mean()                               #有117个乘客Age有缺失，用平均年龄替换
train['Age']=train['Age'].fillna(age_mean) 

#拆分出自变量X，目标变量y
train_X=train.ix[:,1:]                                     #训练集自变量
train_y=train['Survived']                                  #训练集因变量 

X_train,X_test,y_train,y_test = model_selection.\
                                train_test_split(train_X,train_y,test_size=0.3,random_state=42)   #随机种子数
```

##### 2. 训练并预测

```
lr = LogisticRegression()
lr.fit(X_train,y_train) 

#训练集的预测准确率 
y_train_pre=lr.predict(X_train)                #对训练集进行预测
metrics.accuracy_score(y_train,y_train_pre)    #训练集准确率 

#使用测试集验证模型 
y_test_pre=lr.predict(X_test)                  #对测试集进行预测
metrics.accuracy_score(y_test,y_test_pre)      #测试集准确率
```

##### 3. 使用其它模型训练

```python
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC,LinearSVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
import numpy as np 

def classifier_func(X_train,y_train,X_test,y_test): 
    #LR
    lr=LogisticRegression()
    lr.fit(X_train,y_train)
    lr_training = lr.score(X_train,y_train)
    lr_testing  = lr.score(X_test,y_test) 
    lr_error_matrix = abs(lr_training-lr_testing)  #lr残差
    print("LR   ：训练准确率:{}，预测准确率:{}，残差:{}".format(lr_training,lr_testing,lr_error_matrix))

    #SVM
    svc=SVC()
    svc.fit(X_train,y_train)
    SVM_training = svc.score(X_train,y_train) 
    SVM_testing = svc.score(X_test,y_test) 
    SVM_error_matrix = abs(SVM_training-SVM_testing) 
    print("SVM  ：训练准确率:{}，预测准确率:{}，残差:{}".format(SVM_training,SVM_testing,SVM_error_matrix))

    #KNN
    knn=KNeighborsClassifier(n_neighbors=3)
    knn.fit(X_train,y_train)
    KNN_training = knn.score(X_train,y_train)
    KNN_testing = knn.score(X_test,y_test) 
    KNN_error_matrix = abs(KNN_training-KNN_testing)
    print("KNN  ：训练准确率:{}， 预测准确率:{}，残差:{}".format(KNN_training,KNN_testing,KNN_error_matrix))

    #决策树
    dtree=DecisionTreeClassifier()
    dtree.fit(X_train,y_train)
    DTree_training = dtree.score(X_train,y_train)
    DTree_testing = dtree.score(X_test,y_test) 
    DTree_error_matrix = abs(DTree_training-DTree_testing)
    print("DTree：训练准确率:{}，预测准确率:{}，残差:{}".format(DTree_training,DTree_testing,DTree_error_matrix))

    #随机森林
    random_forest=RandomForestClassifier(n_estimators=10)
    random_forest.fit(X_train,y_train)
    RF_training = random_forest.score(X_train,y_train) 
    RF_testing = random_forest.score(X_test,y_test)
    RF_error_matrix = abs(RF_training-RF_testing)
    print("RF   ：训练准确率:{}，预测准确率:{}，残差:{}".format(RF_training,RF_testing,RF_error_matrix))

    return

classifier_func(X_train,y_train,X_test,y_test)

输出：
LR   ：训练准确率:0.7993579454253612，预测准确率:0.8059701492537313，残差:0.006612203828370133
SVM  ：训练准确率:0.8956661316211878，预测准确率:0.6604477611940298，残差:0.23521837042715799
KNN  ：训练准确率:0.826645264847512， 预测准确率:0.6940298507462687，残差:0.13261541410124333
DTree：训练准确率:0.9807383627608347，预测准确率:0.7574626865671642，残差:0.2232756761936705
RF   ：训练准确率:0.9534510433386838，预测准确率:0.7910447761194029，残差:0.16240626721928086
```




