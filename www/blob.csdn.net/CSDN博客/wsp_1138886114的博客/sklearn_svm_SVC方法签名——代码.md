# sklearn_svm_SVC方法签名——代码 - wsp_1138886114的博客 - CSDN博客





2018年08月06日 21:55:10[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：281










- - - - - [1、SVM 在 sklearn 中采用 sklearn.svm.SVC 设置参数：](#1svm-在-sklearn-中采用-sklearnsvmsvc-设置参数)- [1.1  sklearn.svm.SVC 方法](#11-sklearnsvmsvc-方法)
- [1.2 主要调节的参数 (加粗的文字)](#12-主要调节的参数-加粗的文字)

- [2 使用多项式核模拟svm](#2-使用多项式核模拟svm)
- [3 基于RBF核方法对异或（XOR）问题的分类](#3-基于rbf核方法对异或xor问题的分类)
- [4 使用 GridSearchCV 优化模型参数](#4-使用-gridsearchcv-优化模型参数)









**Sklearn中SVM方法详解**


##### 1、SVM 在 sklearn 中采用 sklearn.svm.SVC 设置参数：

```
SVC函数的训练时间是随训练样本平方级增长，所以不适合超过10000的样本。
多分类问题，SVC采用的是one-vs-one投票机制，需要两两类别建立分类器， 
训练时间可能比较长。
```

###### 1.1  sklearn.svm.SVC 方法

```
sklearn.svm.SVC(
                C=1.0,  
                kernel='rbf',  
                degree=3, 
                gamma='auto', 
                coef0=0.0, 
                shrinking=True, 
                probability=False, 
                tol=0.001,  
                cache_size=200, 
                class_weight=None, 
                verbose=False,  
                max_iter=-1, 
                decision_function_shape=None, 
                random_state=None 
                )
```

###### 1.2 主要调节的参数 (加粗的文字)
|参数名称|简介|备注|
|----|----|----|
|**C**|C-SVC的惩罚参数C，默认值是1.0|C是惩罚系数，即对误差的宽容度。c越高，说   明越不能容忍出现误差,容易过拟合。C越小，   容易欠拟合。|
|**kernel**|核函数，默认是rbf，可以是‘linear’,‘poly’,‘rbf’,‘sigmoid’,‘precomputed’|0 – 线性：u’v   1 – 多项式；2 – RBF函数；3 –sigmoid；|
|probability|是否采用shrinking heuristic方法|默认为true|
|shrinking|是否采用shrinking heuristic方法|默认为true|
|**degree**|多项式poly函数的维度，默认是3，选择其他核函数时会被忽略。|$~$|
|**gamma**|‘rbf’ , ‘poly’ 和‘sigmoid’的核函数参数。|默认是’auto’ ，则会选择1/n_features， gamma越大，支持向量越少，gamma值越 小，支持向量越多。支持向量的个数影响训   练与预测的速度|
|**coef0**|核函数的常数项。|常用于‘poly’和   ‘sigmoid’核函数|
|tol|停止训练的误差值大小|默认为1e-3|
|cache_size|核函数cache缓存大小|默认为200|
|class_weight|类别的权重，字典形式传递|设置第几类的参数C为weight*C(C-SVC中的C)|
|verbose|允许冗余输出||
|max_iter|最大迭代次数。|-1为无限制|
|decision_func tion_shape|‘ovo’ , ‘ovr’ or None|default=None3|
|random_state|数据洗牌时的种子值，int值|$~$|

##### 2 使用多项式核模拟svm

```
from sklearn.svm import SVC
import matplotlib.pyplot as plt
import numpy as np 

X=np.array([[1,1],[1,2],[1,3],[1,4],[2,1],[2,2],[3,1],[4,1],[5,1],
[5,2],[6,1],[6,2],[6,3],[6,4],[3,3],[3,4],[3,5],[4,3],[4,4],[4,5]]) 
Y=np.array([1]*14+[-1]*6)
T=np.array([[0.5,0.5],[1.5,1.5],[3.5,3.5],[4,5.5]]) 
 #X为训练样本，Y为训练样本标签(1和-1)，T为测试样本 

svc=SVC(
    kernel='poly',
    degree=2,
    gamma=1,
    coef0=0)

svc.fit(X,Y)
pre=svc.predict(T)
print("输出预测结果:",pre)
print("输出正类和负类支持向量总个数:",svc.n_support_)
print("输出正类和负类支持向量索引:",svc.support_)
print("输出正类和负类支持向量:","\n",svc.support_vectors_)

输出预测结果: [ 1  1 -1 -1]
输出正类和负类支持向量总个数: [2 3]
输出正类和负类支持向量索引: [14 17  3  5 13]
输出正类和负类支持向量: 
 [[3. 3.]
 [4. 3.]
 [1. 4.]
 [2. 2.]
 [6. 4.]]
```

##### 3 基于RBF核方法对异或（XOR）问题的分类

```
import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm 

xx, yy = np.meshgrid(np.linspace(-3, 3, 500),
                     np.linspace(-3, 3, 500))
np.random.seed(0)
X = np.random.randn(300, 2)
Y = np.logical_xor(X[:, 0] > 0, X[:, 1] > 0)

# fit the model
clf = svm.NuSVC()
clf.fit(X, Y) 

# plot the decision function for each datapoint on the grid
Z = clf.decision_function(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

plt.imshow(Z, interpolation='nearest',
           extent=(xx.min(), xx.max(), yy.min(), yy.max()), 
           aspect='auto',
           origin='lower', 
           cmap=plt.cm.PuOr_r)

contours = plt.contour(xx, yy, Z, levels=[0], 
                       linewidths=2,linetypes='-')
plt.scatter(X[:, 0], X[:, 1], s=30, c=Y, 
            cmap=plt.cm.Paired,edgecolors='k')
plt.xticks(())
plt.yticks(())
plt.axis([-3, 3, -3, 3])
plt.show()
```

效果图：  

![这里写图片描述](https://img-blog.csdn.net/20180715164648598?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 4 使用 GridSearchCV 优化模型参数

```python
from sklearn.svm import SVC 
from sklearn import model_selection            #拆分数据集
from sklearn.datasets import load_iris         #鸢尾花数据集
from sklearn.grid_search import GridSearchCV   #网格搜索方法

from sklearn.metrics import confusion_matrix   #混淆矩阵
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import f1_score
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report
```

```php
iris=load_iris()
x=iris.data
y=iris.target 


X_train,X_test,y_train,y_test=model_selection\
                              .train_test_split(x,y,test_size=0.3,random_state=123456) 

parameters={
            'C':[0.001,0.01,0.1,1,10,100],
            'kernel':['linear','poly','rbf','sigmoid'],
            'gamma':[0.0001,0.001]
           }
svm=SVC()
grid_search=GridSearchCV(svm,parameters,scoring='accuracy',cv=5)
grid_search.fit(x,y)            # 查看 GridSearchCV 方法参数 

grid_search.best_estimator_     # 查看 SVC 方法最佳参数 

grid_search.best_score_         # 正确率 
grid_search.best_params_        # 最佳组合
svm=SVC(C=1,gamma=0.0001,kernel='linear')
svm.fit(X_train,y_train)        # 使用最佳参数训练模型 

pred=svm.predict(X_test)        # 模型预测
print(classification_report(y_test,pred))
```



