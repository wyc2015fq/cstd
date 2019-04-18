# 泰坦尼克号数据挖掘项目实战——Task6 模型调优 - fighting！！！ - CSDN博客
2019年01月12日 16:47:10[dujiahei](https://me.csdn.net/dujiahei)阅读数：94
任务6：使用网格搜索法对5个模型进行调优（调参时采用五折交叉验证的方式），并进行模型评估。
参考：【1】[https://blog.csdn.net/qfire/article/details/77601901](https://blog.csdn.net/qfire/article/details/77601901)
【2】[https://blog.csdn.net/jasonding1354/article/details/50562522](https://blog.csdn.net/jasonding1354/article/details/50562522)
【3】[https://www.jianshu.com/p/55b9f2ea283b](https://www.jianshu.com/p/55b9f2ea283b)
【4】[https://www.cnblogs.com/sddai/p/6440797.html](https://www.cnblogs.com/sddai/p/6440797.html)
 【5】[https://blog.csdn.net/wangrongrongwq/article/details/86349083](https://blog.csdn.net/wangrongrongwq/article/details/86349083)
      首先，依然是日常的基础知识补充——什么是网格搜索法(Grid Search)。
Grid Search：一种调参手段；**穷举搜索**：在所有候选的参数选择中，通过循环遍历，尝试每一种可能性，表现最好的参数就是最终的结果。其原理就像是在数组里找最大值。（为什么叫网格搜索？以有两个参数的模型为例，参数a有3种可能，参数b有4种可能，把所有可能性列出来，可以表示成一个3*4的表格，其中每个cell就是一个网格，循环过程就像是在每个网格里遍历、搜索，所以叫grid search）.
![](https://img-blog.csdnimg.cn/20190112124328162.png)
 可能存在的问题：
原始数据集划分成训练集和测试集以后，其中测试集除了用作调整参数，也用来测量模型的好坏；这样做导致最终的评分结果比实际效果要好。（因为测试集在调参过程中，送到了模型里，而我们的目的是将训练模型应用在unseen data上）；
解决方法：
对训练集再进行一次划分，分成训练集和验证集，这样划分的结果就是：原始数据划分为3份，分别为：训练集、验证集和测试集；其中训练集用来模型训练，验证集用来调整参数，而测试集用来衡量模型表现好坏。
交叉验证经常与网格搜索进行结合，作为参数评价的一种方法，这种方法叫做grid search with cross validation。sklearn因此设计了一个这样的类GridSearchCV，这个类实现了fit，predict，score等方法，被当做了一个estimator，使用fit方法，该过程中：（1）搜索到最佳参数；（2）实例化了一个最佳参数的estimator；
 接下来开始本次任务的代码部分：
```python
#### task6 模型调优
## sklearn的类GridSearchCV
##逻辑回归调优
from sklearn.model_selection import GridSearchCV
from sklearn.linear_model import LogisticRegression
from sklearn.cross_validation import train_test_split,cross_val_score
from sklearn.metrics import *
import matplotlib.pyplot as plt
#把要调整的参数以及其候选值 列出来
parameters = {'penalty': ['l1', 'l2'],'C': [0.01,0.1,0.5,1,10]}
print("Parameters:{}".format(parameters))
grid_search = GridSearchCV(estimator = LogisticRegression(), param_grid=parameters,cv = 5) #实例化一个GridSearchCV类
TrainData, TestData,TrainLabel,TestLabel = train_test_split(train_X, target_Y, test_size=0.3,random_state = 2018)
grid_search.fit(TrainData,TrainLabel) #训练，找到最优的参数，同时使用最优的参数实例化一个新的SVC estimator。
print("Test set score:{:.2f}".format(grid_search.score(TestData,TestLabel)))
print("Best parameters:{}".format(grid_search.best_params_))
print("Best score on train set:{:.2f}".format(grid_search.best_score_))
```
Parameters:{'penalty': ['l1', 'l2'], 'C': [0.01, 0.1, 0.5, 1, 10]}
Test set score:0.87
Best parameters:{'penalty': 'l2', 'C': 1}
Best score on train set:0.80
```python
##支持向量机调优
from sklearn import svm
parameters = {'C':[0.01, 0.1, 1, 5, 10]}
print("Parameters:{}".format(parameters))
grid_search = GridSearchCV(estimator = svm.SVC(), param_grid=parameters,cv = 5) #实例化一个GridSearchCV类
TrainData, TestData,TrainLabel,TestLabel = train_test_split(train_X, target_Y, test_size=0.3,random_state = 2018)
grid_search.fit(TrainData,TrainLabel) #训练，找到最优的参数，同时使用最优的参数实例化一个新的SVC estimator。
print("Test set score:{:.2f}".format(grid_search.score(TestData,TestLabel)))
print("Best parameters:{}".format(grid_search.best_params_))
print("Best score on train set:{:.2f}".format(grid_search.best_score_))
```
Parameters:{'C': [0.01, 0.1, 1, 5, 10]}
Test set score:0.76
Best parameters:{'C': 10}
Best score on train set:0.73
```python
## 决策树调优
#from sklearn import tree
from sklearn.tree import DecisionTreeClassifier
parameters = {'max_depth': [1, 5, 10]}
print("Parameters:{}".format(parameters))
grid_search = GridSearchCV(estimator = DecisionTreeClassifier(), param_grid=parameters,cv = 5) #实例化一个GridSearchCV类
TrainData, TestData,TrainLabel,TestLabel = train_test_split(train_X, target_Y, test_size=0.3,random_state = 2018)
grid_search.fit(TrainData,TrainLabel) #训练，找到最优的参数，同时使用最优的参数实例化一个新的SVC estimator。
print("Test set score:{:.2f}".format(grid_search.score(TestData,TestLabel)))
print("Best parameters:{}".format(grid_search.best_params_))
print("Best score on train set:{:.2f}".format(grid_search.best_score_))
```
Parameters:{'max_depth': [1, 5, 10]}
Test set score:0.80
Best parameters:{'max_depth': 10}
Best score on train set:0.79
```python
##随机森林调优
from sklearn.ensemble import RandomForestClassifier
parameters = {'n_estimators':[1, 5, 10, 20]}
print("Parameters:{}".format(parameters))
grid_search = GridSearchCV(estimator = RandomForestClassifier(), param_grid=parameters,cv = 5) #实例化一个GridSearchCV类
TrainData, TestData,TrainLabel,TestLabel = train_test_split(train_X, target_Y, test_size=0.3,random_state = 2018)
grid_search.fit(TrainData,TrainLabel) #训练，找到最优的参数，同时使用最优的参数实例化一个新的SVC estimator。
print("Test set score:{:.2f}".format(grid_search.score(TestData,TestLabel)))
print("Best parameters:{}".format(grid_search.best_params_))
print("Best score on train set:{:.2f}".format(grid_search.best_score_))
```
Parameters:{'n_estimators': [1, 5, 10, 20]}
Test set score:0.86
Best parameters:{'n_estimators': 20}
Best score on train set:0.78
```python
## xgb 调优
from xgboost import XGBClassifier
parameter = {'n_estimators':[1, 5, 10, 20, 40]}
print("Parameters:{}".format(parameters))
grid_search = GridSearchCV(estimator = XGBClassifier(), param_grid=parameters,cv = 5) #实例化一个GridSearchCV类
TrainData, TestData,TrainLabel,TestLabel = train_test_split(train_X, target_Y, test_size=0.3,random_state = 2018)
grid_search.fit(TrainData,TrainLabel) #训练，找到最优的参数，同时使用最优的参数实例化一个新的SVC estimator。
print("Test set score:{:.2f}".format(grid_search.score(TestData,TestLabel)))
print("Best parameters:{}".format(grid_search.best_params_))
print("Best score on train set:{:.2f}".format(grid_search.best_score_))
```
Parameters:{'n_estimators': [1, 5, 10, 20]}
Test set score:0.85
Best parameters:{'n_estimators': 1}
Best score on train set:0.81
从以上结果来看，测试集正确率最高的是逻辑回归。
