# 五、python 缺失值处理（Imputation） - Nicole的博客 - CSDN博客
2018年06月08日 19:48:59[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：3517
# 一、缺失值的处理方法
      由于各种各样的原因，真实世界中的许多数据集都包含缺失数据，这些数据经常被编码成空格、nans或者是其他的占位符。但是这样的数据集并不能被scikit - learn算法兼容，因为大多数的学习算法都会默认数组中的元素都是数值，因此素偶有的元素都有自己的代表意义。
       使用不完整的数据集的一个基本策略就是舍弃掉整行或者整列包含缺失值的数值，但是这样处理会浪费大量有价值的数据。下面是处理缺失值的常用方法：
1.**忽略元组**
当缺少类别标签时通常这样做（假定挖掘任务涉及分类时），除非元组有多个属性缺失值，否则该方法不是很有效。当每个属性缺少值的百分比变化很大时，它的性能特别差。
**2.人工填写缺失值**
一般该方法很费时，并且当数据集很大，缺少很多值时，该方法可能行不通。
**3.使用一个全局常量填充缺失值**
将缺失的属性值用同一个常数(如“Unknown”或 负无穷)替换。如果缺失值都用“unknown”替换，则挖掘程序可能会认为它们形成一个有趣的概念，因为它们都具有相同的值“unknown”。因此，虽然该方法很简单，但是它十分不可靠。
**4.使用与给定元组属同一类的所有样本的属性均值**
例如：将顾客按照credit_risk分类，则使用具有相同信用度的给定元组的顾客的平均收入替换income中的缺失值。
**5.使用最可能的值填充缺失值**
可以用回归、使用贝叶斯形式化的基于推理的工具或决策树归纳确定。例如，利用数据集中其他顾客的属性，可以构造一颗决策树来预测income的缺失值。
注意：缺失值并不总是意味着数据的错误！！！！！！！
# 二、缺失值处理的代码实现
class:[`](http://sklearn.lzjqsdd.com/modules/preprocessing.html#id31)Imputer`类提供了缺失数值处理的基本策略，比如使用缺失数值所在行或列的均值、中位数、众数来替代缺失值。该类也兼容不同的缺失值编码。
1、使用均值填充缺失值
```python
import numpy as np
from sklearn.preprocessing import Imputer
imp = Imputer(missing_values='NaN', strategy='mean', axis=0)
import numpy as np
from sklearn.preprocessing import Imputer
###1.使用均值填充缺失值
imp = Imputer(missing_values='NaN', strategy='mean', axis=0)
imp.fit([[1, 2], [np.nan, 3], [7, 6]])
X = [[np.nan, 2], [6, np.nan], [7, 6]]
print(imp.transform(X))  
[[4.         2.        ]
 [6.         3.66666667]
 [7.         6.        ]]
```
2、[`Imputer`](http://sklearn.lzjqsdd.com/modules/generated/sklearn.preprocessing.Imputer.html#sklearn.preprocessing.Imputer) 类也支持稀疏矩阵:
```python
import scipy.sparse as sp
X = sp.csc_matrix([[1, 2], [0, 3], [7, 6]])
imp = Imputer(missing_values=0, strategy='mean', axis=0)
imp.fit(X)
X_test = sp.csc_matrix([[0, 2], [6, 0], [7, 6]])
print(imp.transform(X_test))
```
#注意，在这里，缺失数据被编码为0, 这种方式用在当缺失数据比观察数据更多的情况时是非常合适的。
