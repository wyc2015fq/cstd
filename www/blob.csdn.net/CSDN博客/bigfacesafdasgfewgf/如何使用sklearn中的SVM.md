# 如何使用sklearn中的SVM - bigfacesafdasgfewgf - CSDN博客





2015年01月27日 14:07:59[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：47154








**如何使用sklearn中的SVM**



    SVM分类算法我们前面已经讲过了，那么我们平时要用到SVM的时候，除了在MATLAB中调用libsvm之外，我们的python中的sklean也已经集成了SVM算法。这篇博客就讲一下sklearn中的SVM如何调用。




    我们先说个例子，看看简单的使用sklean中的SVC(support vectors classification)怎么使用。



```python
from sklearn import svm

X = [[0, 0], [1, 1], [1, 0]]  # training samples 
y = [0, 1, 1]  # training target
clf = svm.SVC()  # class 
clf.fit(X, y)  # training the svc model

result = clf.predict([2, 2]) # predict the target of testing samples 
print result  # target 

print clf.support_vectors_  #support vectors

print clf.support_  # indeices of support vectors

print clf.n_support_  # number of support vectors for each class
```



其中有一些注释帮助理解，其实sklearn中调用机器学习的方法都是一个道理，算法就是一个类，其中包含fit(), predict()等等许多方法，我们只要输入训练样本和标记，以及模型的一些可能的参数，自然就直接出分类的结果。




关于SVC的一些用法，可以参考下面的链接：

http://scikit-learn.org/stable/modules/generated/sklearn.svm.SVC.html#sklearn.svm.SVC






SVC中也可以实现多类分类问题，而且默认使用的是1 vs. 1的投票机制，这种机制的优缺点我们在前面的博文中已经提到了，需要建立的分类器很多很多。。。





SVC也考虑到了累不平衡问题，处理方式在fit方法下面。如下：


fit(*X*, *y*, *sample_weight=None*)[](http://scikit-learn.org/stable/modules/generated/sklearn.svm.SVC.html#sklearn.svm.SVC.fit)
Fit the SVM model according to the given training data.

Returns:
**self** : object

> 
Returns self.





后面的sample_weight和一般的代价敏感相似，只不过这里是每个样本有一个权重。在不平衡学习中，这里增加误分类惩罚项的技术是属于cost sensitive learning 的范畴，其中还有许多更加有效的方式来处理类不平衡问题。




    SVM既可以用来分类，就是SVC；又可以用来预测，或者成为回归，就是SVR。sklearn中的svm模块中也集成了SVR类。

    我们也使用一个小例子说明SVR怎么用。



```python
X = [[0, 0], [1, 1]]
y = [0.5, 1.5] 
clf = svm.SVR() 
clf.fit(X, y)
result = clf.predict([2, 2]) 
print result
```


关于SVR的详细说明文档见下面的链接：

http://scikit-learn.org/stable/modules/generated/sklearn.svm.SVR.html#sklearn.svm.SVR





参考链接：

http://www.csie.ntu.edu.tw/~cjlin/libsvm/


http://scikit-learn.org/stable/modules/svm.html


http://www.csie.ntu.edu.tw/~cjlin/papers/libsvm.pdf




























