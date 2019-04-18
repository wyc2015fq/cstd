# pandas.cut与pandas.qcut使用方法与区别 - Nicole的博客 - CSDN博客
2018年06月05日 11:08:14[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：808
## pandas.cut:
pandas.cut(x, bins, right=True, labels=None, retbins=False, precision=3, include_lowest=False)
## 参数：
- x，类array对象，且必须为一维，待切割的原形式
- bins, 整数、序列尺度、或间隔索引。如果bins是一个整数，它定义了x宽度范围内的等宽面元数量，但是在这种情况下，x的范围在每个边上被延长1%，以保证包括x的最小值或最大值。如果bin是序列，它定义了允许非均匀bin宽度的bin边缘。在这种情况下没有x的范围的扩展。
- right,布尔值。是否是左开右闭区间
- labels,用作结果箱的标签。必须与结果箱相同长度。如果FALSE，只返回整数指标面元。
- retbins,布尔值。是否返回面元
- precision，整数。返回面元的小数点几位
- include_lowest，布尔值。第一个区间的左端点是否包含
## 返回值：
若labels为False则返回整数填充的Categorical或数组或Series 
若retbins为True还返回用浮点数填充的N维数组
### demo:
```
>>> pd.cut(np.array([.2, 1.4, 2.5, 6.2, 9.7, 2.1]), 3, retbins=True)
... 
([(0.19, 3.367], (0.19, 3.367], (0.19, 3.367], (3.367, 6.533], ...
Categories (3, interval[float64]): [(0.19, 3.367] < (3.367, 6.533] ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
>>> pd.cut(np.array([.2, 1.4, 2.5, 6.2, 9.7, 2.1]),
...        3, labels=["good", "medium", "bad"])
... 
[good, good, good, medium, bad, good]
Categories (3, object): [good < medium < bad] 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
>>> pd.cut(np.ones(5), 4, labels=False)
array([1, 1, 1, 1, 1])
```
# pandas.qcut
pandas.qcut(x, q, labels=None, retbins=False, precision=3, duplicates=’raise’)
## 参数：
1.x 
2.q,整数或分位数组成的数组。 
3.labels, 
4.retbins 
5.precisoon 
6.duplicates 
结果中超过边界的值将会变成NA
## demo:
```
>>> pd.qcut(range(5), 4)
... 
[(-0.001, 1.0], (-0.001, 1.0], (1.0, 2.0], (2.0, 3.0], (3.0, 4.0]]
Categories (4, interval[float64]): [(-0.001, 1.0] < (1.0, 2.0] ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
>>> pd.qcut(range(5), 3, labels=["good", "medium", "bad"])
... 
[good, good, medium, bad, bad]
Categories (3, object): [good < medium < bad]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pd.qcut(range(5), 4, labels=False)
array([0, 0, 1, 2, 3])
```
