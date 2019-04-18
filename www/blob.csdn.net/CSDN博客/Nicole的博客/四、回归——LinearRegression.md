# 四、回归——LinearRegression - Nicole的博客 - CSDN博客
2018年06月14日 15:29:13[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：120标签：[回归分析](https://so.csdn.net/so/search/s.do?q=回归分析&t=blog)
个人分类：[机器学习算法——回归](https://blog.csdn.net/weixin_39541558/article/category/7614265)
# 一、官方文档
*class *`sklearn.linear_model.``LinearRegression`(*fit_intercept=True*, *normalize=False*, *copy_X=True*, *n_jobs=1*)
**1.参数：**
Ordinary least squares Linear Regression.（普通最小二乘线性回归）
Attributes:
**coef_** : array, shape (n_features, ) or (n_targets, n_features)
**intercept_** : array
## 2、Methods
- [`fit`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LinearRegression.html#sklearn.linear_model.LinearRegression.fit)(X, y[, sample_weight])：Fit linear model.
- [`get_params`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LinearRegression.html#sklearn.linear_model.LinearRegression.get_params)([deep])：Get parameters for this estimator.
- [`predict`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LinearRegression.html#sklearn.linear_model.LinearRegression.predict)(X)：Predict using the linear model
- [`score`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LinearRegression.html#sklearn.linear_model.LinearRegression.score)(X, y[, sample_weight])：Returns the coefficient of determination R^2 of the prediction.
- [`set_params`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LinearRegression.html#sklearn.linear_model.LinearRegression.set_params)(**params)：Set the parameters of this estimator
### （1）Fit linear model.
Returns:
**self** : returns an instance of self.
### `（2）get_params`(*deep=True*)
Returns:
**params** : mapping of string to any。Parameter names mapped to their values.
### （3）predict（X）
Predict using the linear model
Returns:
**C** : array, shape = (n_samples,).返回预测值
### （4）`score`(*X*, *y*, *sample_weight=None*)
Returns the coefficient of determination R^2 of the prediction.
The coefficient R^2 is defined as (1 - u/v), where u is the residual sum of squares ((y_true - y_pred) ** 2).sum() and v is the total sum of squares ((y_true - y_true.mean()) ** 2).sum(). The best possible score is 1.0 and it can be negative (because the model can be arbitrarily worse).
Returns:
**score** : float；R^2 of self.predict(X) wrt. y
### `（5）set_params`(***params*)
Set the parameters of this estimator.
The method works on simple estimators as well as on nested objects (such as pipelines). The latter have parameters of the form `<component>__<parameter>` so that it’s possible to update each component of a nested object.
# 二、代码实现
```python
import matplotlib.pyplot as plt
import numpy as np
from sklearn import datasets, linear_model
from sklearn.metrics import mean_squared_error, r2_score
# Load the diabetes dataset
diabetes = datasets.load_diabetes()
# Use only one feature
diabetes_X = diabetes.data[:, np.newaxis, 2]
# Split the data into training/testing sets
diabetes_X_train = diabetes_X[:-20]  #去除后面20行
diabetes_X_test = diabetes_X[-20:]   #取后面20行
# Split the targets into training/testing sets
diabetes_y_train = diabetes.target[:-20]
diabetes_y_test = diabetes.target[-20:]
###################################创建线性模型#################################
regr = linear_model.LinearRegression()
# Train the model using the training sets
regr.fit(diabetes_X_train, diabetes_y_train)
# Make predictions using the testing set
diabetes_y_pred = regr.predict(diabetes_X_test)
# The coefficients
print('Coefficients: \n', regr.coef_)
# The mean squared error 均方误差
print("Mean squared error: %.2f"
      % mean_squared_error(diabetes_y_test, diabetes_y_pred))
# Explained variance score: 1 is perfect prediction
print('Variance score: %.2f' % r2_score(diabetes_y_test, diabetes_y_pred))
# Plot outputs
plt.scatter(diabetes_X_test, diabetes_y_test,  color='black')
plt.plot(diabetes_X_test, diabetes_y_pred, color='blue', linewidth=3)
plt.xticks(())
plt.yticks(())
plt.show()
```
输出结果：
```python
diabetes_y_pred
Out[84]: 
array([225.9732401 , 115.74763374, 163.27610621, 114.73638965,
       120.80385422, 158.21988574, 236.08568105, 121.81509832,
        99.56772822, 123.83758651, 204.73711411,  96.53399594,
       154.17490936, 130.91629517,  83.3878227 , 171.36605897,
       137.99500384, 137.99500384, 189.56845268,  84.3990668 ])
print('Coefficients: \n', regr.coef_)
Coefficients: 
 [938.23786125]
print("Mean squared error: %.2f"
      % mean_squared_error(diabetes_y_test, diabetes_y_pred))
Mean squared error: 2548.07
print('Variance score: %.2f' % r2_score(diabetes_y_test, diabetes_y_pred))
Variance score: 0.47
plt.scatter(diabetes_X_test, diabetes_y_test,  color='black')
Out[88]: <matplotlib.collections.PathCollection at 0x1e435f09550>
```
![](https://img-blog.csdn.net/2018061415385253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zOTU0MTU1OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
