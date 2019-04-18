# KNN—数据归一化与参数优化_code - wsp_1138886114的博客 - CSDN博客





2018年08月06日 17:23:36[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：205标签：[KNN—数据归一化与参数优化](https://so.csdn.net/so/search/s.do?q=KNN—数据归一化与参数优化&t=blog)
个人分类：[机器学习](https://blog.csdn.net/wsp_1138886114/article/category/7692618)











- - - - - [导入自带数据鸢尾花 —— 直接使用 KNN 方法训练](#导入自带数据鸢尾花-直接使用-knn-方法训练)
- [GridSearchCV 参数优化 ——使用最佳参数训练](#gridsearchcv-参数优化-使用最佳参数训练)
- [对数据标准化（归一化） 并预测](#对数据标准化归一化-并预测)









开发环境`jupyter notebook`


```python
from sklearn import preprocessing           #数据标准化函数
from sklearn import model_selection         #拆分数据集
from sklearn.datasets import load_iris
from sklearn.neighbors import KNeighborsClassifier
from sklearn import metrics
from sklearn.metrics import confusion_matrix
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import f1_score
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report #分类模型评估报告
```

##### 导入自带数据鸢尾花 —— 直接使用 KNN 方法训练

```python
iris=load_iris()            #导入自带数据鸢尾花
x=iris.data
y=iris.target
x[:10]                      #查看数据
y[:10]

#数据集拆分
X_train,X_test,y_train,y_test = model_selection\
                                .train_test_split(x,y,test_size=0.3,random_state=123456)
```

```php
#直接使用 KNN 分类方法 （稍微调整参数）
knn=KNeighborsClassifier(n_neighbors=10,
                         metric='minkowski',
                         p=1,weights='distance')
knn.fit(X_train,y_train)


pred=knn.predict(X_test)   #预测概率
print (classification_report(y_test,pred))
```

##### GridSearchCV 参数优化 ——使用最佳参数训练

```python
# 使用 GridSearchCV 进行参数优化
parameters={
            'n_neighbors':[5,10,15,20,30],
            'weights':['uniform','distance'],
            'p':[1,2]
            }
knn=KNeighborsClassifier()
grid_search=GridSearchCV(knn,parameters,scoring='accuracy',cv=5)
grid_search.fit(x,y) 

grid_search.best_estimator_   #查看函数（最优评估）
grid_search.best_score_       #正确率 
grid_search.best_params_      #最佳组合（参数）

# 使用最优参数训练
knn=KNeighborsClassifier(n_neighbors=10,
                         metric='minkowski',
                         p=2,weights='distance')
knn.fit(X_train,y_train)  

pred=knn.predict(X_test)
print (classification_report(y_test,pred))
```

##### 对数据标准化（归一化） 并预测

```
iris=load_iris()
x=iris.data
y=iris.target 

#标准化——（0-1标准化）
scaler1=preprocessing.StandardScaler()
scaler1.fit(x)
scaler1_x=scaler1.transform(x)
scaler1_x[:10] 

#标准化——（区间缩放法）
scaler2=preprocessing.MinMaxScaler()
scaler2.fit(x)
scaler2_x=scaler2.transform(x)
scaler2_x[:10]

x[:10]  #查看原始值 与标准化后的数据对比 

knn=KNeighborsClassifier(n_neighbors=10,metric='minkowski',
                         p=2,weights='distance')

knn.fit(scaler1_x,y)    #使用0-1标准化的数据
pred=knn.predict(scaler1_x)
print (classification_report(y,pred)) 

knn.fit(scaler2_x,y)    #使用区间缩放法的数据
pred=knn.predict(scaler2_x)
print (classification_report(y,pred))
```



