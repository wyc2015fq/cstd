# sklearn特征缩放（feature scaling）[MinMaxScaler, standardization] - 墨流觞的博客 - CSDN博客





2018年09月25日 09:08:14[墨氲](https://me.csdn.net/dss_dssssd)阅读数：99标签：[Python																[sklearn																[feature scaling](https://so.csdn.net/so/search/s.do?q=feature scaling&t=blog)
个人分类：[Python数据处理及可视化](https://blog.csdn.net/dss_dssssd/article/category/8077244)





当输入数值属性有不同的尺度，比如有的属性变化范围为-1到1,而有的属性变化范围为1到1000，机器学习算法通常不会表现的很好，因而需要对数值属性进行特征缩放。

有两个常用的方法可以使得所有的属性有相同的尺度， min-max scaling 和 standardization。
- MinMaxXScale  也称 normalization
`class sklearn.preprocessing.MinMaxScaler(feature_range=(0, 1), copy=True)`

独立的缩放每一列特征，将特征值缩放到给定的范围内。

转换公式：
```python
```python
X_std = (X - X.min(axis=0)) / (X.max(axis=0) - X.min(axis=0))
X_scaled = X_std * (max - min) + min
```
```

其中，`min, max = feature_range`

参数：
- fature_range ： tuple(min, max), default=(0,1)

决定了特征值的缩放范围，从转换公式的第二行就很明显的看出了。
- copy: boolean, opetional dafault True

如果copy=False， 执行的是**行正则化**，并且是在原数据上操作

```python
```python
from sklearn.preprocessing import MinMaxScaler
 data = [[-1, 2], [-0.5, 6], [0, 10], [1, 18]]
 # 参数都是使用默认值，即在列正则化，缩放范围为[0, 1]
 scaler = MinMaxScaler()
 print(scaler.fit(data))
 # out :
 # MinMaxScaler(copy=True, feature_range=(0, 1))
 # 返回每一列的最大值
 print(scaler.data_max_)
 #out:
#  [  1.  18.]
# 特征缩放
print(scaler.transform(data))
'''[[ 0.    0.  ]
 [ 0.25  0.25]
 [ 0.5   0.5 ]
 [ 1.    1.  ]]
 '''
data = [[-1, 2], [-0.5, 6], [0, 10], [1, 18]]
scaler = MinMaxScaler()
 # 可以直接使用此函数，一步到位
scaler.fit_transform(data)
 # out:
 '''
 array([[0.  , 0.  ],
       [0.25, 0.25],
       [0.5 , 0.5 ],
       [1.  , 1.  ]])
'''
```
```
- Standardlization
`class sklearn.preprocessing.StandardScaler(copy=True, with_mean=True, with_std=True)`

先减掉平均值（得到零平均值[zero mean]），然后除以方差，（得到单位方差[unit variance]）。

不像Min-Max 缩放，标准化不能讲数据缩放到某一特定的范围内，这对于期望输入(0, 1)的算法，比如神经网络， 并不友好。但是标准化（standardized）更不易受到异常值（outlier）的影响。比如假设房价有一个地区的收入为100（这是一个错误值），但是Min-Max会把它作为最小值，导致最后的范围发生较大变化，甚至成倍的缩减；而对于标准化而言，该值对于平均值的影响并不大，因而最后的值变化不大，也就是说对于异常值的鲁棒性更高。
参数：
- copy: boolean , optional , default True

如果False，尝试在原数据上操作，但如果数据不是Numpy array 或scipy.sparse CSR matrix, 仍然会返回拷贝值
- with_mean: boolean default True

如果True, 在缩放之前先减去平均值，但是如果data是稀疏矩阵（sparse matrix）,会触发异常（raise an expection）。因为会占用大量内存
- with_std: bollean default True

如果True, 缩放到单位方差

```python
```python
from sklearn.preprocessing import StandardScaler
data = [[0, 0], [0, 0], [1, 1], [1, 1]]
# 参数全部采用默认值
scaler = StandardScaler()
print(scaler.fit(data))
# out:
# StandardScaler(copy=True, with_mean=True, with_std=True)

# 每一列的平均值
print(scaler.mean_)
#out:
# 
# [ 0.5  0.5]

# 缩放数据
print(scaler.transform(data))
# out:
'''
[[-1. -1.]
 [-1. -1.]
 [ 1.  1.]
 [ 1.  1.]]
'''

# 使用fit_transform
data = [[0, 0], [0, 0], [1, 1], [1, 1]]
scaler = StandardScaler()
print(scaler.fit_transform(data))
```
```](https://so.csdn.net/so/search/s.do?q=sklearn&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)




