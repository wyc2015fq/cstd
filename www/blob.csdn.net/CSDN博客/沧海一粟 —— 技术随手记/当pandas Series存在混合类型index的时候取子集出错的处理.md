# 当pandas.Series存在混合类型index的时候取子集出错的处理 - 沧海一粟 —— 技术随手记 - CSDN博客





2017年02月08日 17:28:30[慢游](https://me.csdn.net/eengel)阅读数：664








## 问题描述

当创建一个pandas.Series对象，并且设置其index既包含数字型，也包含字符串型时，后续的取子集操作在某些情况下会出错。

## 样例代码



```python
se = pd.Series([1,2], index=[123456789,'1234569'])
se[[123456789,'1234569']] #取全集
se[['1234569']] #只取字符串类型index的子集
se[[123456789]] #只取数字类型index的子集。报错：IndexError: index 123456789 is out of bounds for axis 1 with size 2
```


## 解决方法



当不能确定Series对象的index的数据类型，或index存在混合类型时，用.ix取Series对象的子集。



```python
se = pd.Series([1,2], index=[123456789,'1234569'])
se.ix[[123456789,'1234569']] #取全集
se.ix[['1234569']] #只取字符串类型index的子集
se.ix[[123456789]] #只取数字类型index的子集
```




