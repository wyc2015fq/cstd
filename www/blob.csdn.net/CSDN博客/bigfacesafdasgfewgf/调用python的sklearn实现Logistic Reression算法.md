# 调用python的sklearn实现Logistic Reression算法 - bigfacesafdasgfewgf - CSDN博客





2015年01月21日 18:10:20[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：19527标签：[sklearn																[Logistic Regression																[Linear Model																[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Machine Learning&&Data Mining																[Python](https://blog.csdn.net/puqutogether/article/category/2595125)](https://blog.csdn.net/puqutogether/article/category/2254295)





**调用python的sklearn实现Logistic Reression算法**



    先说如何实现，其中的导入数据库和类、方法的关系，之前不是很清楚，现在知道了。。。   






```python
from numpy import * 
from sklearn.datasets import load_iris     # import datasets

# load the dataset: iris
iris = load_iris() 
samples = iris.data
#print samples 
target = iris.target 

# import the LogisticRegression
from sklearn.linear_model import LogisticRegression 

classifier = LogisticRegression()  # 使用类，参数全是默认的
classifier.fit(samples, target)  # 训练数据来学习，不需要返回值

x = classifier.predict([5, 3, 5, 2.5])  # 测试数据，分类返回标记

print x 

#其实导入的是sklearn.linear_model的一个类：LogisticRegression， 它里面有许多方法
#常用的方法是fit（训练分类模型）、predict（预测测试样本的标记）

#不过里面没有返回LR模型中学习到的权重向量w，感觉这是一个缺陷
```



    上面使用的






```python
classifier = LogisticRegression()  # 使用类，参数全是默认的
```


是默认的，所有的参数全都是默认的，其实我们可以自己设置许多。这需要用到官方给定的参数说明，如下：



# [sklearn.linear_model](http://scikit-learn.org/stable/modules/classes.html#module-sklearn.linear_model).LogisticRegression[](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn-linear-model-logisticregression)
- class sklearn.linear_model.LogisticRegression(*penalty='l2'*, *dual=False*, *tol=0.0001*, *C=1.0*, *fit_intercept=True*,*intercept_scaling=1*, *class_weight=None*, *random_state=None*)[](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression)

Logistic Regression (aka logit, MaxEnt) classifier.

In the multiclass case, the training algorithm uses a one-vs.-all (OvA) scheme, rather than the “true” multinomial LR.

This class implements L1 and L2 regularized logistic regression using the liblinear library. It can handle both dense and sparse input. Use C-ordered
 arrays or CSR matrices containing 64-bit floats for optimal performance; any other input format will be converted (and copied).

Attributes:
**`coef_`** : array, shape = [n_classes, n_features]

> 
Coefficient of the features in the decision function.

coef_ is readonly property derived from raw_coef_ that follows the internal memory layout of liblinear.


**`intercept_`** : array, shape = [n_classes]

> 
Intercept (a.k.a. bias) added to the decision function. If fit_intercept is set to False, the intercept is set to zero.





LogisticRegression类中的方法有如下几种，我们常用的是fit和predict~







Methods
|[decision_function](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.decision_function)(X)|Predict confidence scores for samples.|
|----|----|
|[densify](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.densify)()|Convert coefficient matrix to dense array format.|
|[fit](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.fit)(X, y)|Fit the model according to the given training data.    **用来训练LR分类器，其中的X是训练样本，y是对应的标记向量**|
|[fit_transform](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.fit_transform)(X[, y])|Fit to data, then transform it.|
|[get_params](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.get_params)([deep])|Get parameters for this estimator.|
|[predict](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.predict)(X)|Predict class labels for samples in X.    **用来预测测试样本的标记，也就是分类。X是测试样本集**|
|[predict_log_proba](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.predict_log_proba)(X)|Log of probability estimates.|
|[predict_proba](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.predict_proba)(X)|Probability estimates.|
|[score](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.score)(X, y[, sample_weight])|Returns the mean accuracy on the given test data and labels.|
|[set_params](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.set_params)(**params)|Set the parameters of this estimator.|
|[sparsify](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.sparsify)()|Convert coefficient matrix to sparse format.|
|[transform](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.transform)(X[, threshold])|Reduce X to its most important features.|


使用predict返回的就是测试样本的标记向量，其实个人觉得还应有LR分类器中的重要过程参数：权重向量，其size应该是和feature的个数相同。但是就没有这个方法，所以这就萌生了自己实现LR算法的念头，那样子就可以输出权重向量了。




参考链接：




http://www.cnblogs.com/xupeizhi/archive/2013/07/05/3174703.html





http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression](https://so.csdn.net/so/search/s.do?q=Linear Model&t=blog)](https://so.csdn.net/so/search/s.do?q=Logistic Regression&t=blog)](https://so.csdn.net/so/search/s.do?q=sklearn&t=blog)




