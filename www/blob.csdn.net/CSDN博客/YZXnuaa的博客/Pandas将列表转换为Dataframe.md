# Pandas将列表转换为Dataframe - YZXnuaa的博客 - CSDN博客
2018年09月18日 16:55:11[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1236
Python中将列表转换成为数据框有两种情况：第一种是两个不同列表转换成一个数据框，第二种是一个包含不同子列表的列表转换成为数据框。 
第一种：两个不同列表转换成为数据框
```
from pandas.core.frame import DataFrame
a=[1,2,3,4]#列表a
b=[5,6,7,8]#列表b
c={"a" : a,
   "b" : b}#将列表a，b转换成字典
data=DataFrame(c)#将字典转换成为数据框
print(data)
```
输出的结果为
```
a  b
0  1  5
1  2  6
2  3  7
3  4  8
```
