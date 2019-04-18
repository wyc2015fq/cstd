# 机器学习—naive_bayes_code - wsp_1138886114的博客 - CSDN博客





2018年09月22日 18:27:23[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：47











### 文章目录



开发环境 jupyter notebook 

```python
```python
import numpy as np
from sklearn import model_selection              #拆分数据集
from sklearn import naive_bayes                  #导入贝叶斯模型
from sklearn import metrics

from sklearn.datasets import load_iris           #导入鸢尾花数据集
from sklearn.metrics import confusion_matrix
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import f1_score
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report
```
```

```python
```python
iris=load_iris()
x=iris.data
y=iris.target 

from sklearn import model_selection
X_train,X_test,y_train,y_test=model_selection\
                              .train_test_split(x,y,test_size=0.3,
                                                random_state=123456) 

nb1=naive_bayes.GaussianNB()                 #高斯分布
nb1.fit(X_train,y_train)                     #拟合训练
pred=nb1.predict(X_test)                     #预测
print(classification_report(y_test,pred)) 

X=np.random.randint(5,size=(6,100))          #自定义数据集
y=np.array([1,2,3,4,4,5]) 
nb2=naive_bayes.MultinomialNB()              #多项式分布
nb2.fit(X,y)                                 #拟合训练
print(nb2.predict(X))                        #预测输出


X=np.random.randint(2,size=(6,100))         #自定义数据集
y=np.array([1,2,3,4,4,5]) 
nb3=naive_bayes.BernoulliNB()               #伯努利分布
nb3.fit(X,y)                                #拟合训练
print(nb3.predict(X))                       #预测输出
```
```



