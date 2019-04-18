# python merge、concat合并数据集 - Nicole的博客 - CSDN博客
2018年04月04日 10:07:54[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：71
个人分类：[python 数据处理](https://blog.csdn.net/weixin_39541558/article/category/7563702)
# 数据规整化：合并、清理、过滤
pandas和python标准库提供了一整套高级、灵活的、高效的核心函数和算法将数据规整化为你想要的形式！
本篇博客主要介绍：
合并数据集：.merge()、.concat()等方法，类似于SQL或其他关系型数据库的连接操作。
# 合并数据集
## 1） merge 函数参数
|参数|说明|
|----|----|
|left|参与合并的左侧DataFrame|
|right|参与合并的右侧DataFrame|
|how|连接方式：‘inner’（默认）；还有，‘outer’、‘left’、‘right’|
|on|用于连接的列名，必须同时存在于左右两个DataFrame对象中，如果位指定，则以left和right列名的交集作为连接键|
|left_on|左侧DataFarme中用作连接键的列|
|right_on|右侧DataFarme中用作连接键的列|
|left_index|将左侧的行索引用作其连接键|
|right_index|将右侧的行索引用作其连接键|
|sort|根据连接键对合并后的数据进行排序，默认为True。有时在处理大数据集时，禁用该选项可获得更好的性能|
|suffixes|字符串值元组，用于追加到重叠列名的末尾，默认为（‘_x’,‘_y’）.例如，左右两个DataFrame对象都有‘data’，则结果中就会出现‘data_x’，‘data_y’|
|copy|设置为False，可以在某些特殊情况下避免将数据复制到结果数据结构中。默认总是赋值|
**1、多对一的合并（一个表的连接键列有重复值，另一个表中的连接键没有重复值）**
```python
import pandas as pd
import numpy as np
df1 = pd.DataFrame({'key':['b','b','a','c','a','a','b'],'data1': range(7)})
df1
```
||data1|key|
|----|----|----|
|0|b| |
|1|b| |
|2|a| |
|3|c| |
|4|a| |
|5|a| |
|6|b| |
```python
df2 = pd.DataFrame({'key':['a','b','d'],'data2':range(3)})
df2
```
||data2|key|
|----|----|----|
|0|a| |
|1|b| |
|2|d| |
```python
pd.merge(df1,df2)#默认情况
```
||data1|key|data2|
|----|----|----|----|
|0|b|1| |
|1|b|1| |
|6|b|1| |
|2|a|0| |
|4|a|0| |
|5|a|0| |
```python
df1.merge(df2)
```
||data1|key|data2|
|----|----|----|----|
|0|b|1| |
|1|b|1| |
|6|b|1| |
|2|a|0| |
|4|a|0| |
|5|a|0| |
```python
df1.merge(df2,on = 'key',how = 'inner')#内连接，取交集
```
||data1|key|data2|
|----|----|----|----|
|0|b|1| |
|1|b|1| |
|6|b|1| |
|2|a|0| |
|4|a|0| |
|5|a|0| |
```python
df1.merge(df2,on = 'key',how = 'outer')#外链接，取并集，并用nan填充
```
||data1|key|data2|
|----|----|----|----|
|0.0|b|1.0| |
|1.0|b|1.0| |
|6.0|b|1.0| |
|2.0|a|0.0| |
|4.0|a|0.0| |
|5.0|a|0.0| |
|3.0|c|NaN| |
|NaN|d|2.0| |
```python
df1.merge(df2,on = 'key',how = 'left')#左连接，左侧DataFrame取全部，右侧DataFrame取部分
```
||data1|key|data2|
|----|----|----|----|
|0|b|1.0| |
|1|b|1.0| |
|2|a|0.0| |
|3|c|NaN| |
|4|a|0.0| |
|5|a|0.0| |
|6|b|1.0| |
```python
df1.merge(df2,on = 'key',how = 'right')#右连接，右侧DataFrame取全部，左侧DataFrame取部分
```
||data1|key|data2|
|----|----|----|----|
|0.0|b|1| |
|1.0|b|1| |
|6.0|b|1| |
|2.0|a|0| |
|4.0|a|0| |
|5.0|a|0| |
|NaN|d|2| |
如果左右侧DataFrame的连接键列名不一致，但是取值有重叠，可使用left_on、right_on来指定左右连接键
```python
df3 = pd.DataFrame({'lkey':['b','b','a','c','a','a','b'],'data1': range(7)})
df3
```
||data1|lkey|
|----|----|----|
|0|b| |
|1|b| |
|2|a| |
|3|c| |
|4|a| |
|5|a| |
|6|b| |
```python
df4 = pd.DataFrame({'rkey':['a','b','d'],'data2':range(3)})
df4
```
||data2|rkey|
|----|----|----|
|0|a| |
|1|b| |
|2|d| |
```python
df3.merge(df4,left_on = 'lkey',right_on = 'rkey',how = 'inner')
```
||data1|lkey|data2|rkey|
|----|----|----|----|----|
|0|b|1|b| |
|1|b|1|b| |
|6|b|1|b| |
|2|a|0|a| |
|4|a|0|a| |
|5|a|0|a| |
**2、多对多的合并（一个表的连接键列有重复值，另一个表中的连接键有重复值）**
```python
df1 = pd.DataFrame({'key':['b','b','a','c','a','a','b'],'data1': range(7)})
df1
```
||data1|key|
|----|----|----|
|0|b| |
|1|b| |
|2|a| |
|3|c| |
|4|a| |
|5|a| |
|6|b| |
```python
df5 = pd.DataFrame({'key':['a','b','a','b','b'],'data2': range(5)})
df5
```
||data2|key|
|----|----|----|
|0|a| |
|1|b| |
|2|a| |
|3|b| |
|4|b| |
```python
df1.merge(df5)
```
||data1|key|data2|
|----|----|----|----|
|0|b|1| |
|0|b|3| |
|0|b|4| |
|1|b|1| |
|1|b|3| |
|1|b|4| |
|6|b|1| |
|6|b|3| |
|6|b|4| |
|2|a|0| |
|2|a|2| |
|4|a|0| |
|4|a|2| |
|5|a|0| |
|5|a|2| |
### 合并小结
1）默认情况下，会将两个表中相同列名作为连接键
2）多对多，会采用笛卡尔积形式链接（左表连接键有三个值‘1，3，5’，右表有两个值‘2，3’，则会形成，（1，2）（1，3）（3，1），（3，2）。。。6种组合）
**3）存在多个连接键的处理**
```python
left = pd.DataFrame({'key1':['foo','foo','bar'],'key2':['one','one','two'],'lval':[1,2,3]})
right = pd.DataFrame({'key1':['foo','foo','bar','bar'],'key2':['one','one','one','two'],'rval':[4,5,6,7]})
```
```python
left
```
||key1|key2|lval|
|----|----|----|----|
|foo|one|1| |
|foo|one|2| |
|bar|two|3| |
```python
right
```
||key1|key2|rval|
|----|----|----|----|
|foo|one|4| |
|foo|one|5| |
|bar|one|6| |
|bar|two|7| |
```python
pd.merge(left,right,on = ['key1','key2'],how = 'outer')
```
||key1|key2|lval|rval|
|----|----|----|----|----|
|foo|one|1.0|4| |
|foo|one|1.0|5| |
|foo|one|2.0|4| |
|foo|one|2.0|5| |
|bar|two|3.0|7| |
|bar|one|NaN|6| |
1）连接键是多对多关系，应执行笛卡尔积形式
2）多列应看连接键值对是否一致
**4）对连接表中非连接列的重复列名的处理**
```python
pd.merge(left,right,on = 'key1')
```
||key1|key2_x|lval|key2_y|rval|
|----|----|----|----|----|----|
|foo|one|1|one|4| |
|foo|one|1|one|5| |
|foo|one|2|one|4| |
|foo|one|2|one|5| |
|bar|two|3|one|6| |
|bar|two|3|two|7| |
```python
pd.merge(left,right,on = 'key1',suffixes = ('_left','_right'))
```
||key1|key2_left|lval|key2_right|rval|
|----|----|----|----|----|----|
|foo|one|1|one|4| |
|foo|one|1|one|5| |
|foo|one|2|one|4| |
|foo|one|2|one|5| |
|bar|two|3|one|6| |
|bar|two|3|two|7| |
## 2）索引上的合并
当连接键位于索引中时，成为索引上的合并，可以通过merge函数，传入left_index、right_index来说明应该被索引的情况。
- 一表中连接键是索引列、另一表连接键是非索引列
```python
left1 = pd.DataFrame({'key':['a','b','a','a','b','c'],'value': range(6)})
left1
```
||key|value|
|----|----|----|
|a|0| |
|b|1| |
|a|2| |
|a|3| |
|b|4| |
|c|5| |
```python
right1 = pd.DataFrame({'group_val':[3.5,7]},index = ['a','b'])
right1
```
- 1
- 2
||group_val|
|----|----|
|3.5| |
|7.0| |
```python
pd.merge(left1,right1,left_on = 'key',right_index = True)
```
||key|value|group_val|
|----|----|----|----|
|a|0|3.5| |
|a|2|3.5| |
|a|3|3.5| |
|b|1|7.0| |
|b|4|7.0| |
**有上可知，left_on、right_on是指定表中非索引列为连接键，left_index、right_index是指定表中索引列为连接键，两者可以组合，是为了区分是否是索引列**
- 两个表中的索引列都是连接键
```python
left2 = pd.DataFrame(np.arange(6).reshape(3,2),index = ['a','b','e'],columns = ['0hio','nevada'])
right2 = pd.DataFrame(np.arange(7,15).reshape(4,2),index = ['b','c','d','e'],columns = ['misso','ala'])
left2
```
||0hio|nevada|
|----|----|----|
|0|1| |
|2|3| |
|4|5| |
```python
right2
```
||misso|ala|
|----|----|----|
|7|8| |
|9|10| |
|11|12| |
|13|14| |
```python
pd.merge(left2,right2,left_index = True,right_index = True,how = 'outer')
```
||0hio|nevada|misso|ala|
|----|----|----|----|----|
|0.0|1.0|NaN|NaN| |
|2.0|3.0|7.0|8.0| |
|NaN|NaN|9.0|10.0| |
|NaN|NaN|11.0|12.0| |
|4.0|5.0|13.0|14.0| |
## 3）轴向连接
在这里展示一种新的连接方法，对应于numpy的concatenate函数，pandas有concat函数
```python
#numpy
arr =np.arange(12).reshape(3,4)
```
```python
arr
```
```
array([[ 0,  1,  2,  3],
           [ 4,  5,  6,  7],
           [ 8,  9, 10, 11]])
```
```python
np.concatenate([arr,arr],axis = 1)#横轴连接块
```
```
array([[ 0,  1,  2,  3,  0,  1,  2,  3],
           [ 4,  5,  6,  7,  4,  5,  6,  7],
           [ 8,  9, 10, 11,  8,  9, 10, 11]])
```
### concat函数参数表格
|参数|说明|
|----|----|
|objs|参与连接的列表或字典，且列表或字典里的对象是pandas数据类型，唯一必须给定的参数|
|axis=0|指明连接的轴向，0是纵轴，1是横轴，默认是0|
|join|‘inner’（交集），‘outer’（并集），默认是‘outer’指明轴向索引的索引是交集还是并集|
|join_axis|指明用于其他n-1条轴的索引（层次化索引，某个轴向有多个索引），不执行交并集|
|keys|与连接对象有关的值，用于形成连接轴向上的**层次化索引**（外层索引），可以是任意值的列表或数组、元组数据、数组列表（如果将levels设置成多级数组的话）|
|levels|指定用作层次化索引各级别（内层索引）上的索引，如果设置keys的话|
|names|用于创建分层级别的名称，如果设置keys或levels的话|
|verify_integrity|检查结果对象新轴上的重复情况，如果发横则引发异常，默认False，允许重复|
|ignore_index|不保留连接轴上的索引，产生一组新索引range（total_length）|
```python
s1 = pd.Series([0,1,2],index = ['a','b','c'])
s2 = pd.Series([2,3,4],index = ['c','f','e'])
s3 = pd.Series([4,5,6],index = ['c','f','g'])
```
```python
pd.concat([s1,s2,s3])#默认并集、纵向连接
```
    a    0    b    1    c    2    c    2    f    3    e    4    c    4    f    5    g    6    dtype: int64
```python
pd.concat([s1,s2,s3],ignore_index = True)#生成纵轴上的并集，索引会自动生成新的一列
```
    0    0    1    1    2    2    3    2    4    3    5    4    6    4    7    5    8    6    dtype: int64
```python
pd.concat([s1,s2,s3],axis = 1,join = 'inner')#纵向取交集,注意该方法对对象表中有重复索引时失效
```
||0|1|2|
|----|----|----|----|
|2|2|4| |
```python
pd.concat([s1,s2,s3],axis = 1,join = 'outer')#横向索引取并集，纵向索引取交集,注意该方法对对象表中有重复索引时失效
```
||0|1|2|
|----|----|----|----|
|0.0|NaN|NaN| |
|1.0|NaN|NaN| |
|2.0|2.0|4.0| |
|NaN|4.0|NaN| |
|NaN|3.0|5.0| |
|NaN|NaN|6.0| |
### concat函数小结
1）纵向连接，ignore_index = False ,可能生成重复的索引
2）横向连接时，对象索引不能重复
## 4）合并重叠数据
适用范围：
1）当两个对象的索引有部分或全部重叠时
2）用参数对象中的数据为调用者对象的缺失数据‘打补丁’
```python
a = pd.Series([np.nan,2.5,np.nan,3.5,4.5,np.nan],index = ['a','b','c','d','e','f'])
b = pd.Series(np.arange(len(a)),index = ['a','b','c','d','e','f'])
```
```python
a
```
```
a    NaN
b    2.5
c    NaN
d    3.5
e    4.5
f    NaN
dtype: float64
```
```python
b
```
```python
a.combine_first(b)#利用b填补了a的空值
```
```
a    0.0
b    2.5
c    2.0
d    3.5
e    4.5
f    5.0
dtype: float64
```
- 
- 
```python
a = pd.Series([np.nan,2.5,np.nan,3.5,4.5,np.nan],index = ['g','b','c','d','e','f'])
```
```python
a.combine_first(b)#部分索引重叠
```
```
a    0.0
b    2.5
c    2.0
d    3.5
e    4.5
f    5.0
g    NaN
dtype: float64
```
# 小结
本篇博客主要讲述了一下内容：
1） merge函数合并数据集
2）concat函数合并数据集
3）combine_first函数，含有重叠索引的缺失值填补
                             版权声明：转载，原地址：            https://blog.csdn.net/LY_ysys629/article/details/73849543              
