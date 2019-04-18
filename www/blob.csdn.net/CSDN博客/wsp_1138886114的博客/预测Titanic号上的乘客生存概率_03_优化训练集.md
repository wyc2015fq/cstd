# 预测Titanic号上的乘客生存概率_03_优化训练集 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:32:53[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：30标签：[预测Titanic号上的乘客生存概率_03_优化训练集](https://so.csdn.net/so/search/s.do?q=预测Titanic号上的乘客生存概率_03_优化训练集&t=blog)
个人分类：[机器学习](https://blog.csdn.net/wsp_1138886114/article/category/7692618)












### 文章目录
- [一、导入数据 数据清洗](#__15)
- [二、构建 onehot 编码](#_onehot__51)
- [三、筛选处理好的变量、拆分数据集](#_71)
- [四、训练—预测—筛选最优分类器](#_89)




```python
```python
import numpy as np
import pandas as pd
from sklearn import metrics
from sklearn import model_selection
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC,LinearSVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from matplotlib import pyplot as plt
%matplotlib inline
import re
```
```

##### 一、导入数据 数据清洗

```python
```python
train_src=pd.read_csv('train.csv')
train_src['Data_type']='train'
pred_src=pd.read_csv('test.csv')
pred_src['Data_type']='pred'
combine=pd.concat([train_src,pred_src]) 

combine['Age']=combine['Age'].fillna(train_src['Age'].mean())    #对年龄缺失值进行均值填充
combine['Fare']=combine['Fare'].fillna(train_src['Fare'].mean()) #Fare船票价格空值,平均值替换 

#从姓名中提取头衔
def get_title(name):
    title_search=re.search(', ([A-Za-z]+)\.',name)
    if title_search:
        return title_search.group(1)
    return np.nan
combine['Title']=combine['Name'].apply(get_title)
combine['Title'].value_counts(dropna=False) 

combine['Title']=combine['Title'].replace(['Lady','Capt','Col','Don','Dr','Major',
                                           'Rev','Sir','Jonkheer','Dona'],'Rare')
combine['Title']=combine['Title'].replace('Mlle','Miss')
combine['Title']=combine['Title'].replace('Ms','Miss')
combine['Title']=combine['Title'].replace('Mme','Mrs')
combine['Title'].value_counts(dropna=False) 

#兄弟姐妹数，配偶数，父母数，子女数加起来
combine['FamilySize']=combine['SibSp']+combine['Parch']+1
combine['FamilySize'].value_counts() 

#从性别、年龄、子女数来判断乘客是否是母亲
combine['is_mother']=0
combine.loc[(combine['Sex']=='female') & (combine['Parch']>0) & (combine['Age']>20),'is_mother']=1
combine['is_mother'].value_counts()
```
```

##### 二、构建 onehot 编码

```python
```python
sex_onehot=pd.get_dummies(combine['Sex'],drop_first=False,prefix='onehot')
sex_onehot.head() 

combine['Age_group']=np.nan
combine.loc[combine['Age']<=16,'Age_group']='Age_0_16'
combine.loc[(combine['Age']>16)&(combine['Age']<=32),'Age_group']='Age_16_32'
combine.loc[(combine['Age']>32)&(combine['Age']<=48),'Age_group']='Age_32_48'
combine.loc[(combine['Age']>48)&(combine['Age']<=64),'Age_group']='Age_48_64'
combine.loc[combine['Age']>64,'Age_group']='Age_64_'
age_group_onehot=pd.get_dummies(combine['Age_group'],drop_first=False,prefix='onehot')
age_group_onehot.head() 

title_onehot=pd.get_dummies(combine['Title'],drop_first=False,prefix='onehot')
title_onehot.head()

pclass_onehot=pd.get_dummies(combine['Pclass'],drop_first=False,prefix='onehot')
pclass_onehot.head()
```
```

##### 三、筛选处理好的变量、拆分数据集

```python
```python
combine=pd.concat([combine,sex_onehot,age_group_onehot,title_onehot,pclass_onehot],axis=1)
combine.head() 

pred_X=combine.ix[combine['Data_type']=='pred',
                  ['SibSp','Parch','Fare',
                   'FamilySize','is_mother','onehot_male','onehot_female',
                   'onehot_Age_0_16','onehot_Age_16_32','onehot_Age_32_48',
                   'onehot_Age_48_64','onehot_Age_64_',
                   'onehot_Master','onehot_Miss','onehot_Mr','onehot_Mrs','onehot_Rare',
                   'onehot_1','onehot_2','onehot_3']]
pred_id=combine.ix[combine['Data_type']=='pred','PassengerId']

X_train,X_test,y_train,y_test = model_selection\
                                .train_test_split(train_X,train_y, 
                                 test_size=0.3,random_state=42)
```
```

##### 四、训练—预测—筛选最优分类器

```python
```python
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
-------------------------------------------
输出：
LR   ：训练准确率:0.8314606741573034，预测准确率:0.8171641791044776，残差:0.01429649505282582
SVM  ：训练准确率:0.8346709470304976，预测准确率:0.8059701492537313，残差:0.028700797776766263
KNN  ：训练准确率:0.8587479935794543，预测准确率:0.7723880597014925，残差:0.08635993387796181
DTree：训练准确率:0.9502407704654896，预测准确率:0.8097014925373134，残差:0.14053927792817622
RF   ：训练准确率:0.9325842696629213，预测准确率:0.7947761194029851，残差:0.1378081502599362 
--------------------------------------------
# 准备预测数据
pred_X=combine.ix[combine['Data_type']=='pred',
                  ['SibSp','Parch','Fare',
                   'FamilySize','is_mother','onehot_male','onehot_female',
                   'onehot_Age_0_16','onehot_Age_16_32','onehot_Age_32_48','onehot_Age_48_64','onehot_Age_64_',
                   'onehot_Master','onehot_Miss','onehot_Mr','onehot_Mrs','onehot_Rare',
                   'onehot_1','onehot_2','onehot_3']]
pred_id=combine.ix[combine['Data_type']=='pred','PassengerId'] 

pred_y_pre=lr.predict(pred_X)
test_pred=pd.DataFrame({'PassengerId':pred_id,
                       'Survived':pred_y_pre.astype(int)})
test_pred.to_csv('test_pred_2.csv',index=False)

from sklearn.ensemble import VotingClassifier  #多模型投票融合器
votingcf=VotingClassifier(estimators=[('lr',lr),('knn',knn),('decisiontree',dtree),
                                     ('rf',random_forest),('svm',svc)],voting='hard') 
                                     
#训练集投票后的结果
votingcf.fit(X_train,y_train)
votingcf.score(X_train,y_train)  

#测试集投票后的结果
votingcf.fit(X_test,y_test)
votingcf.score(X_test,y_test) 

pred_y_pre=votingcf.predict(pred_X)
submit=pd.DataFrame({'PassengerId':pred_id,
                    'Survived':pred_y_pre.astype(int)})
submit.to_csv('test_pred_3.csv',index=False)
```
```



