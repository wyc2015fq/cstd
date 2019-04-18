# 预测Titanic号上的乘客生存概率_01_code - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:28:09[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：77











### 文章目录
- [step1: 数据感知](#step1__12)
- [step2: 数据清洗](#step2__31)
- [step3: 构建模型](#step3__45)
- [step4: 模型评估](#step4__59)
- [step5: 对新数据进行预测](#step5__94)



开发环境 jupyter notebook
数据集下载地址：[https://download.csdn.net/download/wsp_1138886114/10589801](https://download.csdn.net/download/wsp_1138886114/10589801)

```python
```python
import numpy as np
import pandas as pd
from sklearn.linear_model import LogisticRegression  #导入逻辑回归算法
from sklearn import metrics                          #导入模型评估
import matplotlib.pyplot as plt
%matplotlib inline
```
```

##### step1: 数据感知

```python
```python
train_src=pd.read_csv('train.csv')
train_src.info()
train_src.head()  

train_src.hist(column='Age',bins=50)                 #Age分布 
train_src['Sex'].value_counts().plot(kind='bar')    #性别分布
```
```

**变量与目标的相关性**

```python
```python
#性别与目标(是否生存)的相关性
pd.crosstab(train_src['Sex'],train_src['Survived']).plot(kind='bar')#交叉表
#Pclass与目标(是否生存)的相关性
pd.crosstab(train_src['Pclass'],train_src['Survived']).plot(kind='bar')
#Age与目标(是否生存)的相关性
train_src.age=pd.cut(train_src.Age,[0,5,15,20,35,50,60,100])
pd.crosstab(train_src.age,train_src['Survived']).plot(kind='bar')
```
```

##### step2: 数据清洗

```python
```python
#筛选Pclass,Sex,Age,SibSp,Parch,Fare六个变量作为预测变量(特征)
train=train_src[['Survived','Pclass','Sex','Age','SibSp','Parch','Fare']] 

#把Sex变量的取值male替换为1，female替换为0
train['Sex']=train['Sex'].replace({'male':1,'female':0})

age_mean=train['Age'].mean()       #有117个乘客Age有缺失，用平均年龄替换
train['Age']=train['Age'].fillna(age_mean) 

train.head(10)                     #查看一下准备好的数据集 
train.describe()
```
```

##### step3: 构建模型

```python
```python
#拆分出自变量X，目标变量y
train_X=train.ix[:,1:]            #训练集自变量
train_y=train['Survived']         #训练集因变量

#使用逻辑回归算法训练模型
lr=LogisticRegression()           #使用默认参数
lr.fit(train_X,train_y)           #训练 

lr.coef_                          #查看lr模型的系数(x变量系数) 
train_X.columns                   #x变量参数 
pd.DataFrame(list(zip(np.transpose(lr.coef_),train_X.columns)),columns=['coef','columns'])
```
```

##### step4: 模型评估

```python
```python
train_y_pred=lr.predict(train_X)             #对训练集进行预测，输出标签
train_y_pred_prob=lr.predict_proba(train_X)  #对训练集进行预测，
train_y_pred                                 #输出概率

train_y_pred_prob                            #查看预测概率  

#误分类矩阵
cnf_matrix=metrics.confusion_matrix(train_y,train_y_pred)
cnf_matrix 

#准确率
precision=metrics.accuracy_score(train_y,train_y_pred)
precision 

#更直观一点的展现误分类矩阵
def show_confusion_matrix(cnf_matrix,class_labels):
    plt.matshow(cnf_matrix,cmap=plt.cm.YlGn,alpha=0.7)
    ax=plt.gca()
    ax.set_xlabel('Predicted Label',fontsize=16)
    ax.set_xticks(range(0,len(class_labels)))
    ax.set_xticklabels(class_labels,rotation=45)
    ax.set_ylabel('Actual Label',fontsize=16,rotation=90)
    ax.set_yticks(range(0,len(class_labels)))
    ax.set_yticklabels(class_labels)
    ax.xaxis.set_label_position('top')
    ax.xaxis.tick_top()
    
    for row in range(len(cnf_matrix)):
        for col in range(len(cnf_matrix[row])):
            ax.text(col,row,cnf_matrix[row][col],va='center',ha='center',fontsize=16)
class_labels=[0,1]
show_confusion_matrix(cnf_matrix,class_labels)
```
```

##### step5: 对新数据进行预测

```python
```python
#测试数据准备，与训练集的准备完全一致
test_src=pd.read_csv('test.csv')
test=test_src[['PassengerId','Pclass','Sex','Age','SibSp','Parch','Fare']]
test['Sex']=test['Sex'].replace({'male':1,'female':0})
test['Age']=test['Age'].fillna(age_mean)

#Fare船票价格在测试集中出现了空值，用训练集的平均值替换
test['Fare']=test['Fare'].fillna(train['Fare'].mean()) 

test.head()                      #查看一下测试数据 

#对测试数据预测
test_X=test.ix[:,1:]             
test_y_pred=lr.predict(test_X)   #对测试集进行预测
test_pred=pd.DataFrame({'PassengerId':test['PassengerId'],'Survived':test_y_pred.astype(int)})
test_pred.to_csv('test_pred.csv',index=False) 


test_pred.head()                #查看预测结果
```
```



