# 机器学习之Grid Search网格搜索（自动调参） - Machine Learning with Peppa - CSDN博客





2019年01月10日 13:26:30[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1833








# 什么是Grid Search 网格搜索？

Grid Search：一种调参手段；**穷举搜索**：在所有候选的参数选择中，通过循环遍历，尝试每一种可能性，表现最好的参数就是最终的结果。其原理就像是在数组里找最大值。（为什么叫网格搜索？以有两个参数的模型为例，参数a有3种可能，参数b有4种可能，把所有可能性列出来，可以表示成一个3*4的表格，其中每个cell就是一个网格，循环过程就像是在每个网格里遍历、搜索，所以叫grid search）

![](https://upload-images.jianshu.io/upload_images/9637742-fb89eb419353b2a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/957/format/webp)

parameters table



# Simple Grid Search：简单的网格搜索

以2个参数的调优过程为例：

```
from sklearn.datasets import load_iris
from sklearn.svm import SVC
from sklearn.model_selection import train_test_split

iris = load_iris()
X_train,X_test,y_train,y_test = train_test_split(iris.data,iris.target,random_state=0)
print("Size of training set:{} size of testing set:{}".format(X_train.shape[0],X_test.shape[0]))

####   grid search start
best_score = 0
for gamma in [0.001,0.01,0.1,1,10,100]:
    for C in [0.001,0.01,0.1,1,10,100]:
        svm = SVC(gamma=gamma,C=C)#对于每种参数可能的组合，进行一次训练；
        svm.fit(X_train,y_train)
        score = svm.score(X_test,y_test)
        if score > best_score:#找到表现最好的参数
            best_score = score
            best_parameters = {'gamma':gamma,'C':C}
####   grid search end

print("Best score:{:.2f}".format(best_score))
print("Best parameters:{}".format(best_parameters))
```

输出：

```
Size of training set:112 size of testing set:38
Best score:0.973684
Best parameters:{'gamma': 0.001, 'C': 100}
```

### 存在的问题：

原始数据集划分成训练集和测试集以后，其中测试集除了用作调整参数，也用来测量模型的好坏；这样做导致最终的评分结果比实际效果要好。（因为测试集在调参过程中，送到了模型里，而我们的目的是将训练模型应用在unseen data上）；

### 解决方法：

对训练集再进行一次划分，分成训练集和验证集，这样划分的结果就是：原始数据划分为3份，分别为：训练集、验证集和测试集；其中训练集用来模型训练，验证集用来调整参数，而测试集用来衡量模型表现好坏。



![](https://upload-images.jianshu.io/upload_images/9637742-47cffdca06fe8e37.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/937/format/webp)

three parts

```
X_trainval,X_test,y_trainval,y_test = train_test_split(iris.data,iris.target,random_state=0)
X_train,X_val,y_train,y_val = train_test_split(X_trainval,y_trainval,random_state=1)
print("Size of training set:{} size of validation set:{} size of teseting set:{}".format(X_train.shape[0],X_val.shape[0],X_test.shape[0]))

best_score = 0.0
for gamma in [0.001,0.01,0.1,1,10,100]:
    for C in [0.001,0.01,0.1,1,10,100]:
        svm = SVC(gamma=gamma,C=C)
        svm.fit(X_train,y_train)
        score = svm.score(X_val,y_val)
        if score > best_score:
            best_score = score
            best_parameters = {'gamma':gamma,'C':C}
svm = SVC(**best_parameters) #使用最佳参数，构建新的模型
svm.fit(X_trainval,y_trainval) #使用训练集和验证集进行训练，more data always results in good performance.
test_score = svm.score(X_test,y_test) # evaluation模型评估
print("Best score on validation set:{:.2f}".format(best_score))
print("Best parameters:{}".format(best_parameters))
print("Best score on test set:{:.2f}".format(test_score))
```

输出：

```
Size of training set:84 size of validation set:28 size of teseting set:38
Best score on validation set:0.96
Best parameters:{'gamma': 0.001, 'C': 10}
Best score on test set:0.92
```

然而，这种间的的grid search方法，其最终的表现好坏与初始数据的划分结果有很大的关系，为了处理这种情况，我们采用交叉验证的方式来减少偶然性。

# Grid Search with Cross Validation

```
from sklearn.model_selection import cross_val_score

best_score = 0.0
for gamma in [0.001,0.01,0.1,1,10,100]:
    for C in [0.001,0.01,0.1,1,10,100]:
        svm = SVC(gamma=gamma,C=C)
        scores = cross_val_score(svm,X_trainval,y_trainval,cv=5) #5折交叉验证
        score = scores.mean() #取平均数
        if score > best_score:
            best_score = score
            best_parameters = {"gamma":gamma,"C":C}
svm = SVC(**best_parameters)
svm.fit(X_trainval,y_trainval)
test_score = svm.score(X_test,y_test)
print("Best score on validation set:{:.2f}".format(best_score))
print("Best parameters:{}".format(best_parameters))
print("Score on testing set:{:.2f}".format(test_score))
```

输出：

```
Best score on validation set:0.97
Best parameters:{'gamma': 0.01, 'C': 100}
Score on testing set:0.97
```

交叉验证经常与网格搜索进行结合，作为参数评价的一种方法，这种方法叫做grid search with cross validation。sklearn因此设计了一个这样的类GridSearchCV，这个类实现了fit，predict，score等方法，被当做了一个estimator，使用fit方法，该过程中：（1）搜索到最佳参数；（2）实例化了一个最佳参数的estimator；

```
from sklearn.model_selection import GridSearchCV

#把要调整的参数以及其候选值 列出来；
param_grid = {"gamma":[0.001,0.01,0.1,1,10,100],
             "C":[0.001,0.01,0.1,1,10,100]}
print("Parameters:{}".format(param_grid))

grid_search = GridSearchCV(SVC(),param_grid,cv=5) #实例化一个GridSearchCV类
X_train,X_test,y_train,y_test = train_test_split(iris.data,iris.target,random_state=10)
grid_search.fit(X_train,y_train) #训练，找到最优的参数，同时使用最优的参数实例化一个新的SVC estimator。
print("Test set score:{:.2f}".format(grid_search.score(X_test,y_test)))
print("Best parameters:{}".format(grid_search.best_params_))
print("Best score on train set:{:.2f}".format(grid_search.best_score_))
```

输出：

```
Parameters:{'gamma': [0.001, 0.01, 0.1, 1, 10, 100], 'C': [0.001, 0.01, 0.1, 1, 10, 100]}
Test set score:0.97
Best parameters:{'C': 10, 'gamma': 0.1}
Best score on train set:0.98
```

Grid Search 调参方法存在的共性弊端就是：耗时；参数越多，候选值越多，耗费时间越长！所以，一般情况下，先定一个大范围，然后再细化。

![](https://upload-images.jianshu.io/upload_images/9637742-7e2feb6d05075e74.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/973/format/webp)

GridSearchCV参数调优过程

# 总而言之，言而总之
- Grid Search：一种调优方法，在参数列表中进行**穷举搜索**，对每种情况进行训练，找到最优的参数；由此可知，这种方法的主要缺点是 比较耗时！




作者：七八音

链接：https://www.jianshu.com/p/55b9f2ea283b

來源：简书

简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。



