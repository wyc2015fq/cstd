# 泰坦尼克号数据挖掘项目实战——Task2 特征工程 - fighting！！！ - CSDN博客
2019年01月06日 19:05:54[dujiahei](https://me.csdn.net/dujiahei)阅读数：94
参考：【1】[https://www.cnblogs.com/wxquare/p/5484636.html](https://www.cnblogs.com/wxquare/p/5484636.html)
           【2】[https://blog.csdn.net/weishiym/article/details/79629329](https://blog.csdn.net/weishiym/article/details/79629329)
           【3】[https://blog.csdn.net/zhouhong0284/article/details/79489479](https://blog.csdn.net/zhouhong0284/article/details/79489479)
           【4】[https://blog.csdn.net/kt513226724/article/details/79843073](https://blog.csdn.net/kt513226724/article/details/79843073)
           【5】[https://blog.csdn.net/kt513226724/article/details/79844950](https://blog.csdn.net/kt513226724/article/details/79844950)
在task1中，我们简单对数据进行了分析，选择出了Age，Embarked，Fare, Parch, Pclass, Sex, SibSp 共7种特征，由于这里使用了one-hot 编码，所以实际得到的特征维度包括：Age, Embarked=C, Embarked=Q, Embarked=S, Fare, Parch, Pclass, Sex=female, Sex=male, SibSp 共10个，经过对缺失值的填充，得到的训练集特征矩阵为891*10，测试集的特征矩阵为418*10.
   那么，这些特征都对分类起到积极作用吗？我们需要对特征进行挑选或者重新组合，形成新的特征。即，需要进行特征工程。
    “数据决定了机器学习的上限，而算法只是尽可能逼近这个上限”，这里的数据指的就是经过特征工程得到的数据。特征工程指的是把原始数据转变为模型的训练数据的过程，它的目的就是获取更好的训练数据特征，使得机器学习模型逼近这个上限。特征工程能使得模型的性能得到提升，有时甚至在简单的模型上也能取得不错的效果。特征工程在机器学习中占有非常重要的作用，一般认为括特征构建、特征提取、特征选择三个部分。特征构建比较麻烦，需要一定的经验。 特征提取与特征选择都是为了从原始特征中找出最有效的特征。它们之间的区别是特征提取强调通过**特征转换**的方式得到一组具有明显物理或统计意义的特征；而特征选择是从特征集合中挑选一组具有明显物理或统计意义的**特征子集**。两者都能帮助减少特征的维度、数据冗余，特征提取有时能发现更有意义的特征属性，特征选择的过程经常能表示出每个特征的重要性对于模型构建的重要性【1】。
   特征工程包括特征清洗和预处理，task1 中已经进行了缺失值的填补，完成了数据清洗的部分，接下来要进行预处理部分。
   感谢002同学对task1的点评，我决定采用可视化探索特征，主要参考【4】。
```python
#忽略警告提示
import warnings
warnings.filterwarnings('ignore')
#数据处理
import pandas as pd
import numpy as np
import random
#可视化
import matplotlib.pyplot as plt
import seaborn as sns
%matplotlib inline
```
对特征与是否被救求相关系数，并画heatmap 得到较为直观的分析。
```python
## 数据读入
path='C:/Users/Titanic/'
p1=open(path+'train.csv')
p2=open(path+'test.csv')
train=pd.read_csv(p1)
test=pd.read_csv(p2)
## 数据概览
train.head(3)
train.info()
train.describe()
## 求相关系数
train.drop('PassengerId',axis=1).corr()
## heatmap 作图
sns.set(context='paper',font='monospace')
sns.set(style='white')
f,ax=plt.subplots(figsize=(10,6))
train_corr=train.drop('PassengerId',axis=1).corr()
sns.heatmap(train_corr,ax=ax,vmax=.9,square=True,cmap=plt.cm.get_cmap('RdYlBu'))
ax.set_xticklabels(train_corr.index,size=15)
ax.set_yticklabels(train_corr.columns,size=15)
ax.set_title('train feature corr',fontsize=20)
```
![](https://img-blog.csdnimg.cn/20190106173148184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190106175956778.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
    由上图可以看出，Pclass与获救情况负相关，Fare和获救情况正相关。
  我们单独看一下Pclass与获救情况。
Pclass 与获救情况：
```python
y_dead=train[train.Survived==0].groupby('Pclass')['Survived'].count()
y_alive=train[train.Survived==1].groupby('Pclass')['Survived'].count()
pos=[1,2,3]
ax=plt.figure(figsize=(8,4)).add_subplot(111)
ax.bar(pos,y_dead,color='r',alpha=.6,label='dead')
ax.bar(pos,y_alive,color='g',bottom=y_dead,alpha=.6,label='alive')
ax.legend(fontsize=16,loc='best')
ax.set_xticks(pos)
ax.set_xticklabels(['Pclass%d'%(i) for i in range(1,4)],size=15)
ax.set_title('Pclass Survived count',size=20)
```
![](https://img-blog.csdnimg.cn/2019010618053692.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
从总数来看，三等舱存活人数最多，从存活比例来看，头等舱存活人数最多，三等舱死亡率最高。
再对三个船舱和年龄一起来看：
```python
pos=range(0,6)
age_list=[]
for Pclass_ in range(1,4):
    for Survived_ in range(0,2):
        age_list.append(train[(train.Pclass==Pclass_)&(train.Survived==Survived_)].Age.values)
fig,axes=plt.subplots(3,1,figsize=(10,6))
i_Pclass=1
for ax in axes:
    sns.distplot(age_list[i_Pclass*2-2],hist=False,ax=ax,label='Pclass:%d ,survived:0'%(i_Pclass),color='r')
    sns.distplot(age_list[i_Pclass*2-1],hist=False,ax=ax,label='Pclass:%d ,survived:1'%(i_Pclass),color='g')
    i_Pclass +=1
    ax.set_xlabel('age',size=15)
    ax.legend(fontsize=15)
```
![](https://img-blog.csdnimg.cn/20190106181012827.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
可以看出，一等、二等舱的小孩被保护较好，而三等舱有部分小孩也死亡了。
以上是简单的做了一个可视化分析，接下来继续对数据处理。
task1 中对数据的填充采用了最简单的方法，这里将采用更加合理一些的方法，结合乘客的性别、舱位、头衔。
```python
#将数据按照性别，舱位，头衔分组，并计算中位数
grouped_train=combined.head(891).groupby(['Sex','Pclass','Title'])
grouped_median_train=grouped_train.median()
grouped_test=combined.iloc[891:].groupby(['Sex','Pclass','Title'])
grouped_median_test=grouped_test.median()12345
grouped_median_train
```
![](https://img-blog.csdnimg.cn/20190106181951208.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
```python
grouped_median_test
```
![](https://img-blog.csdnimg.cn/20190106182041326.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
通过观察，认为可以通过乘客性别、称谓、舱等级的不同来对年龄进行填充。
```python
def fillAges(row,grouped_median):
    if row['Sex']=='female' and row['Pclass']==1:
        if row['Title']=='Miss':
            return grouped_median.loc['female',1,'Miss']['Age']
        elif row['Title']=='Mrs':
            return grouped_median.loc['female',1,'Mrs']['Age']
        elif row['Title'] == 'Officer':
                return grouped_median.loc['female', 1, 'Officer']['Age']
        elif row['Title'] == 'Royalty':
                return grouped_median.loc['female', 1, 'Royalty']['Age']
    elif row['Sex']=='female' and row['Pclass'] == 2:
        if row['Title'] == 'Miss':
            return grouped_median.loc['female', 2, 'Miss']['Age']
        elif row['Title'] == 'Mrs':
            return grouped_median.loc['female', 2, 'Mrs']['Age']
    elif row['Sex']=='female' and row['Pclass'] == 3:
        if row['Title'] == 'Miss':
            return grouped_median.loc['female', 3, 'Miss']['Age']
        elif row['Title'] == 'Mrs':
            return grouped_median.loc['female', 3, 'Mrs']['Age']
    elif row['Sex']=='male' and row['Pclass'] == 1:
        if row['Title'] == 'Master':
            return grouped_median.loc['male', 1, 'Master']['Age']
        elif row['Title'] == 'Mr':
            return grouped_median.loc['male', 1, 'Mr']['Age']
        elif row['Title'] == 'Officer':
            return grouped_median.loc['male', 1, 'Officer']['Age']
        elif row['Title'] == 'Royalty':
            return grouped_median.loc['male', 1, 'Royalty']['Age']
    elif row['Sex']=='male' and row['Pclass'] == 2:
        if row['Title'] == 'Master':
            return grouped_median.loc['male', 2, 'Master']['Age']
        elif row['Title'] == 'Mr':
            return grouped_median.loc['male', 2, 'Mr']['Age']
        elif row['Title'] == 'Officer':
            return grouped_median.loc['male', 2, 'Officer']['Age']
    elif row['Sex']=='male' and row['Pclass'] == 3:
        if row['Title'] == 'Master':
            return grouped_median.loc['male', 3, 'Master']['Age']
        elif row['Title'] == 'Mr':
            return grouped_median.loc['male', 3, 'Mr']['Age']
combined.head(891).Age=combined.head(891).apply(lambda r: fillAges(r,grouped_median_train) if 
                                               np.isnan(r['Age']) else r['Age'],axis=1)
combined.iloc[891:].Age=combined.iloc[891:].apply(lambda r: fillAges(r,grouped_median_test) if
                                                 np.isnan(r['Age']) else r['Age'],axis=1)
combined.info()    
combined.head(891).Fare.fillna(combined.head(891).Fare.mean(),inplace=True)
combined.iloc[891:].Fare.fillna(combined.iloc[891:].Fare.mean(),inplace=True)
```
填充缺失Embarked为登船地点最多的S
```python
combined.head(891).Embarked.fillna('S', inplace=True)
combined.iloc[891:].Embarked.fillna('S', inplace=True)
```
填补Cabin
```python
combined.Cabin.fillna('U', inplace=True)
combined['Cabin'] = combined['Cabin'].map(lambda c : c[0])
```
```python
combined.info()    
#title虚拟变量编码
titleDf=pd.get_dummies(combined['Title'],prefix='Title')
combined=pd.concat([combined,titleDf],axis=1)
```
把家人数目和兄弟姐妹数目组合起来，建立family
```python
# Parch&SibSp ,建立Familysize
familyDf=pd.DataFrame()
familyDf['FamilySize']=combined['Parch']+combined['SibSp']+1
familyDf[ 'Family_Single' ] = familyDf[ 'FamilySize' ].map( lambda s : 1 if s == 1 else 0 )
familyDf[ 'Family_Small' ]  = familyDf[ 'FamilySize' ].map( lambda s : 1 if 2 <= s <= 4 else 0 )
familyDf[ 'Family_Large' ]  = familyDf[ 'FamilySize' ].map( lambda s : 1 if 5 <= s else 0 )
combined=pd.concat([combined,familyDf],axis=1)
```
embarked
```python
embarkedDf=pd.get_dummies(combined['Embarked'],prefix='Embarked')
combined=pd.concat([combined,embarkedDf],axis=1)
```
Sex
```python
sex_mapDict={'male':1,
            'female':0}
#map函数：对Series每个数据应用自定义的函数计算
combined['Sex']=combined['Sex'].map(sex_mapDict)
```
Cabin
```python
cabinDf=pd.get_dummies(combined['Cabin'],prefix='Cabin')
combined=pd.concat([combined,cabinDf],axis=1)
```
Pclass
```python
pclassDf=pd.get_dummies(combined['Pclass'],prefix='Pclass')
combined=pd.concat([combined,pclassDf],axis=1)
```
Ticket
```python
#提取票价前缀，如果没有前缀，即票价为数字返回XXX
def cleanTicket(ticket):
    ticket=ticket.replace('.','')
    ticket=ticket.replace('/','')
    ticket=ticket.split()
    #ticket=map(lambda t: t.strip(),ticket)
    #flag=filter(lambda t: not t.isdigit(),ticket)
    if ticket[0].isdigit():
        return 'XXX'
    else:
        return ticket[0]
combined['Ticket']=combined['Ticket'].map(cleanTicket)
ticketsDf=pd.get_dummies(combined['Ticket'],prefix='Ticket')
combined=pd.concat([combined,ticketsDf],axis=1)
```
接下来看下目前的特征维度：
```python
combined.head(3)
```
列数为75。
```python
#将其余无关特征删除
combined.drop(['PassengerId','Cabin','Embarked','Name','Pclass','Ticket','Title'], inplace=True, axis=1)
combined.head(3)
```
现在列数为68。
接下来就是如何对特征挑选了，特征挑选前，需要把之前train和test分开。
```python
#得到训练/测试数据
train_X=combined.iloc[:891,:].drop(['Survived'],axis=1)
target_Y=combined.iloc[:891,:]['Survived']
test_X=combined.iloc[891:,:].drop(['Survived'],axis=1)
print('训练集特征：',train_X.shape,
     '训练集标签：',target_Y.shape,
     '测试集特征：',test_X.shape)
```
```python
#训练集特征： (891, 67) 训练集标签： (891,) 测试集特征： (418, 67)
#导入库
from sklearn.pipeline import make_pipeline
from sklearn.ensemble import RandomForestClassifier
from sklearn.feature_selection import SelectKBest
from sklearn.cross_validation import StratifiedKFold
from sklearn.grid_search import GridSearchCV
from sklearn.ensemble.gradient_boosting import GradientBoostingClassifier
from sklearn.cross_validation import cross_val_score
 
#定义评价函数
def compute_score(clf,X,y,scoring='accuracy'):
    xval=cross_val_score(clf,X,y,cv=5,scoring=scoring)#K折交叉分类，cv数据分成的数量
    return np.mean(xval)
```
接下来是特征挑选，一个好的特征选择可以： 1.减少数据之间的冗余  2.加速训练过程  3.防止过拟合。
这里采用随机森林来挑选特征。
```python
#采用随机森林来计算特征输入
from sklearn.ensemble import RandomForestClassifier
from sklearn.feature_selection import SelectFromModel
#n_estimators构造的决策树数量，max_features不超过的最大特征数量
clf=RandomForestClassifier(n_estimators=50,max_features='sqrt')
clf=clf.fit(train_X,target_Y)
 
features=pd.DataFrame()
features['feature']=train_X.columns
features['importance']=clf.feature_importances_ #系数大小反应特征重要性
features.sort_values(by=['importance'],ascending=True,inplace=True)
features.set_index('feature',inplace=True)
 
features.plot(kind='barh',figsize=(20,20))
```
![](https://img-blog.csdnimg.cn/20190106231111124.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
```python
#选取合适的特征
model=SelectFromModel(clf,prefit=True)
train_reduced=model.transform(train_X)
train_reduced.shape
test_reduced=model.transform(test_X)
test_reduced.shape
```
由此，通过随机森林，我们得到了12个特征。

