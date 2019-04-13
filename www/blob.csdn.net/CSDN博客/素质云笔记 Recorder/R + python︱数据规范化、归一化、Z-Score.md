
# R + python︱数据规范化、归一化、Z-Score - 素质云笔记-Recorder... - CSDN博客

2016年04月23日 21:35:32[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：20957所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
笔者寄语：规范化主要是因为数据受着单位的影响较大，需要进行量纲化。大致有：最小-最大规范化、均值标准化、小数定标规范化
数据中心化和标准化的意义是一样的，为了消除量纲对数据结构的影响。

1、最小-最大规范化——标准化

也叫离差标准化，是对原始数据的线性变换，将数据映射到[0,1]之间，与功效系数法相同。
标准化x-min(x) / max(x)-min(x)


`#最小-最大规范化
b1=(data[,1]-min(data[,1]))/(max(data[,1])-min(data[,1]))
b2=(data[,2]-min(data[,2]))/(max(data[,2])-min(data[,2]))
b3=(data[,3]-min(data[,3]))/(max(data[,3])-min(data[,3]))
b4=(data[,4]-min(data[,4]))/(max(data[,4])-min(data[,4]))
data_scatter=cbind(b1,b2,b3,b4)`

2、均值标准化法——正态化
正态标准差标准化、零均值规范化等方法，经过处理的数据均值为0，标准差为1。公式为：
x*=(x-均值)/标准差
因为均值受离群值影响较大，也可以将均值替换成变量的中位数。
`#零-均值规范化
data_zscore=scale(data)`
![](https://img-blog.csdn.net/20160608111119150)



3、小数定标规范化
移动变量的小数点位置来将变量映射到[-1,1]
`#小数定标规范化
i1=ceiling(log(max(abs(data[,1])),10))#小数定标的指数
c1=data[,1]/10^i1
i2=ceiling(log(max(abs(data[,2])),10))
c2=data[,2]/10^i2
i3=ceiling(log(max(abs(data[,3])),10))
c3=data[,3]/10^i3
i4=ceiling(log(max(abs(data[,4])),10))
c4=data[,4]/10^i4
data_dot=cbind(c1,c2,c3,c4)
#打印结果
options(digits = 4)#控制输出结果的有效位数
data_dot`
代码中，log（x,10）是ln（x）一样；
options可以控制保留四位数小数


# 4、还原标准化的方法
`preds=norm.data*sd(data)+mean(data)#还原标准化的数据`

# 5、R语言中的scale函数

scale方法中的两个参数center和scale的解释：
1.center和scale默认为真,即T或者TRUE
2.center为真表示数据中心化
3.scale为真表示数据标准化

中心化=源数据-均值
标准化==中心化之后的数据在除以数据集的标准差，即数据集中的各项数据减去数据集的均值再除以数据集的标准差。
例如有数据集1, 2, 3, 6, 3，其均值为3,其标准差为1.87，那么标准化之后的数据集为(1-3)/1.87,(2-3)/1.87,(3-3)/1.87,(6-3)/1.87,(3-3)/1.87,即：-1.069,-0.535,0,1.604,0

那么以下几种情况是啥意思：
`scale(x)=scale(x,center=T,scale=T)，默认设置
scale(x,center=F,scale=T)代表不进行中心化，直接做标准化；
scale(x,center=T,scale=F)代表中心化
scale(x,center=F,scale=F)代表什么不做，是原来的数据列。`
那么与apply族联用就是apply(x,1,scale)



**6、python中的****Z-Score**

主要借助sklearn中的preprocessing：

```python
from sklearn import preprocessing
```
公式为：**(X-mean)/std**计算时对每个属性/每列分别进行。
将数据按期属性（按列进行）减去其均值，并处以其方差。得到的结果是，对于每个属性/每列来说所有数据都聚集在0附近，方差为1。
实现时，有两种不同的方式：
使用sklearn.preprocessing.scale()函数，可以直接将给定数据进行标准化。

```python
>> from sklearn import preprocessing
>>> import numpy as np
>>> X = np.array([[ 1., -1.,  2.],
...               [ 2.,  0.,  0.],
...               [ 0.,  1., -1.]])
>>> X_scaled = preprocessing.scale(X)
 
>>> X_scaled                                         
array([[ 0.  ..., -1.22...,  1.33...],
       [ 1.22...,  0.  ..., -0.26...],
       [-1.22...,  1.22..., -1.06...]])
 
>>>#处理后数据的均值和方差
>>> X_scaled.mean(axis=0)
array([ 0.,  0.,  0.])
 
>>> X_scaled.std(axis=0)
array([ 1.,  1.,  1.])
```
其中axis = 0是按列（纵向）进行平均，axis = 1代表横向求平均。
同时求X的Z-score还有另外一种表达方式，是使用apply：

```python
pd.DataFram(X).apply(preprocessing.scale,axis = 0)
```


使用sklearn.preprocessing.StandardScaler类，使用该类的好处在于可以保存训练集中的参数（均值、方差）直接使用其对象转换测试集数据。
`>>> scaler = preprocessing.StandardScaler().fit(X)
>>> scaler
StandardScaler(copy=True, with_mean=True, with_std=True)
 
>>> scaler.mean_                                     
array([ 1. ...,  0. ...,  0.33...])
 
>>> scaler.std_                                      
array([ 0.81...,  0.81...,  1.24...])
 
>>> scaler.transform(X)                              
array([[ 0.  ..., -1.22...,  1.33...],
       [ 1.22...,  0.  ..., -0.26...],
       [-1.22...,  1.22..., -1.06...]])
 
 
>>>#可以直接使用训练集对测试集数据进行转换
>>> scaler.transform([[-1.,  1., 0.]])               
array([[-2.44...,  1.22..., -0.26...]])`使用这个非常好的一点就是，在机器学习的时候，当训练好模型，当要predict预测新的数据集的时候，就可以沿用训练集的均值、方差，比较适合部署模型在新的数据集上。
同时，这里的transform（X）相当于之前的，也就纵向进行平均，是符合要求的！
```python
pd.DataFram(X).apply(preprocessing.scale,axis = 0)
```
参考案例：

```python
scaler_train = preprocessing.StandardScaler().fit(shuhua_train_x)
shuhua_train_x = scaler_train.transform(shuhua_train_x)   # 约等于：shuhua_train_x.apply(preprocessing.scale,axis = 0)
```



参考:[【原】关于使用sklearn进行数据预处理 —— 归一化/标准化/正则化](http://www.cnblogs.com/chaosimple/p/4153167.html)、[sklearn.preprocessing](http://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.StandardScaler.html#sklearn.preprocessing.StandardScaler)[.StandardScaler](http://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.StandardScaler.html#sklearn.preprocessing.StandardScaler)、[Preprocessing data](http://scikit-learn.org/stable/modules/preprocessing.html#preprocessing)

**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


