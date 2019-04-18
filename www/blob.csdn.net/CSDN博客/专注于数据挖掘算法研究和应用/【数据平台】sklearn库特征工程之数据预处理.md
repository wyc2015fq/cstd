# 【数据平台】sklearn库特征工程之数据预处理 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年12月16日 10:24:10[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：291
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、背景：

通过特征提取，我们能得到未经处理的特征，这时的特征可能有以下问题：
- 不属于同一量纲：即特征的规格不一样，不能够放在一起比较。无量纲化可以解决这一问题。
- 信息冗余：对于某些定量特征，其包含的有效信息为区间划分，例如学习成绩，假若只关心“及格”或不“及格”，那么需要将定量的考分，转换成“1”和“0”表示及格和未及格。二值化可以解决这一问题。
- 定性特征不能直接使用：某些机器学习算法和模型只能接受定量特征的输入，那么需要将定性特征转换为定量特征。最简单的方式是为每一种定性值指定一个定量值，但是这种方式过于灵活，增加了调参的工作。[通常使用哑编码的方式将定性特征转换为定量特征](http://www.ats.ucla.edu/stat/mult_pkg/faq/general/dummy.htm)：假设有N种定性值，则将这一个特征扩展为N种特征，当原始特征值为第i种定性值时，第i个扩展特征赋值为1，其他扩展特征赋值为0。哑编码的方式相比直接指定的方式，不用增加调参的工作，对于线性模型来说，使用哑编码后的特征可达到非线性的效果。
- 存在缺失值：缺失值需要补充。
- 信息利用率低：不同的机器学习算法和模型对数据中信息的利用是不同的，之前提到在线性模型中，使用对定性特征哑编码可以达到非线性的效果。类似地，对定量变量多项式化，或者进行其他的转换，都能达到非线性的效果。

　　我们使用sklearn中的preproccessing库来进行数据预处理，可以覆盖以上问题的解决方案。




2、代码：



```python
# -*- coding: utf-8 -*-
'''
Created on 2017年12月15日
@author: Jason.F
@summary: sklearn特征工程
'''

from sklearn.datasets import load_iris
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import Normalizer
from sklearn.preprocessing import Binarizer
from sklearn.preprocessing import OneHotEncoder
from numpy import vstack, array, nan
from sklearn.preprocessing import Imputer
from sklearn.preprocessing import PolynomialFeatures
#导入IRIS数据集
iris = load_iris()
#特征矩阵
iris.data
#目标向量
iris.target

#数据预处理
#1：无量纲化，使不同规格的数据转换到同一规格
#1.1:标准化，标准化的前提是特征值服从正态分布，标准化后，其转换成标准正态分布。
StandardScaler().fit_transform(iris.data)
#1.2:区间缩放法，利用了边界值信息，将特征的取值区间缩放到某个特点的范围，如缩放到[0, 1]区间
MinMaxScaler().fit_transform(iris.data)
#1.3：归一化：依照特征矩阵的行处理数据，其目的在于样本向量在点乘运算或其他核函数计算相似性时，拥有统一的标准
#归一化，返回值为归一化后的数据
Normalizer().fit_transform(iris.data)

#2：对定量特征二值化，定量特征二值化的核心在于设定一个阈值，大于阈值的赋值为1，小于等于阈值的赋值为0。
Binarizer(threshold=3).fit_transform(iris.data)

#3:对定性特征哑编码，使用preproccessing库的OneHotEncoder类对数据进行哑编码
OneHotEncoder().fit_transform(iris.target.reshape((-1,1)))

#4：缺失值计算，使用preproccessing库的Imputer类对数据进行缺失值计算
#缺失值计算，返回值为计算缺失值后的数据
#参数missing_value为缺失值的表示形式，默认为NaN
#参数strategy为缺失值填充方式，默认为mean（均值）
Imputer().fit_transform(vstack((array([nan, nan, nan, nan]), iris.data)))

#5：数据变换，常见的数据变换有基于多项式的、基于指数函数的、基于对数函数的
#多项式转换
#参数degree为度，默认值为2
pndata=PolynomialFeatures().fit_transform(iris.data)
print (pndata[0:2])
```

3、标准化和归一化区别

标准化是依照特征矩阵的列处理数据，其通过求z-score的方法，将样本的特征值转换到同一量纲下，公式如下：

![](http://images2015.cnblogs.com/blog/927391/201605/927391-20160502113957732-1062097580.png)归一化是依照特征矩阵的行处理数据，其目的在于样本向量在点乘运算或其他核函数计算相似性时，拥有统一的标准，也就是说都转化为“单位向量”。规则为l2的归一化公式如下：

![](http://images2015.cnblogs.com/blog/927391/201607/927391-20160719002904919-1602367496.png)4、总结：


|类|功能|说明|
|----|----|----|
|StandardScaler|无量纲化|标准化，基于特征矩阵的列，将特征值转换至服从标准正态分布|
|MinMaxScaler|无量纲化|区间缩放，基于最大最小值，将特征值转换到[0, 1]区间上|
|Normalizer|归一化|基于特征矩阵的行，将样本向量转换为“单位向量”|
|Binarizer|二值化|基于给定阈值，将定量特征按阈值划分|
|OneHotEncoder|哑编码|将定性数据编码为定量数据|
|Imputer|缺失值计算|计算缺失值，缺失值可填充为均值等|
|PolynomialFeatures|多项式数据转换|多项式数据转换|
|FunctionTransformer|自定义单元数据转换|使用单变元的函数来转换数据|




