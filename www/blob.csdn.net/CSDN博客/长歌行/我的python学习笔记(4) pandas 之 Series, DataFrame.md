# 我的python学习笔记(4) pandas 之 Series, DataFrame - 长歌行 - CSDN博客





2017年08月26日 20:17:52[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：9351
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









在前一个blog中，介绍了numpy，类似于tensor的结构，主要用于机器学习等问题，矩阵运算等问题。

这里介绍一下pandas的基本数据结构，series和DataFrame，在数据分析中，会用到它们。

本章节[GitHub](https://github.com/bentan2013/PythonAndGIS/blob/master/pandas/pandas_example.ipynb)地址, GitHub中是用jupyter写的一个例子。

## 如何引用

```python
from pandas import Series, DataFrame
import pandas as pd
```

## Series

可以使用dir来查看信息 
`dir(Series)`

### 构造方式

> 
Series（[data，index，dtype，name，copy，…]）


```
In [73]:
# 从数组生成
s_from_array = Series([100, 200])
s_from_array
Out[73]:
0    100
1    200
dtype: int64
In [74]:
# 从字典生成
s_from_dict = Series({'2010':100, '2012':200, '2014':300})
s_from_dict
Out[74]:
2010    100
2012    200
2014    300
dtype: int64
```

### 基本描述元素

Series.values 返回系列为ndarray或ndarray样 

Series.dtype 返回底层数据的dtype对象 

Series.ftype 返回底层数据的ftype对象Series.shape 返回基础数据的形状的元组 

Series.nbytes 返回底层数据中的字节数 

Series.ndim 返回底层数据的维数， 

Series.size 返回底层数据中的元素数量 

Series.strides 返回基础数据的步幅 

Series.itemsize 返回底层数据项的dtype的大小 

Series.base 返回基础对象 

Series.T 返回转置，这是通过定义self 

Series.memory_usage（[index，deep]）
Example:

```
print("数据索引:")
print(s_from_array.index)
print("数据值:")
print(s_from_array.values)

# 各项指标，可以参照numpy
print("数据类型为：")  
print(type(s_from_array))  
print('数据大小：')  
print(s_from_array.size)  
print('数据类型：')  
print(s_from_array.dtype)  
print('数据维度：')  
print(s_from_array.ndim)  
print('数据形状：')  
print(s_from_array.shape)
数据索引:
RangeIndex(start=0, stop=2, step=1)
数据值:
[100 200]
数据类型为：
<class 'pandas.core.series.Series'>
数据大小：
2
数据类型：
int64
数据维度：
1
数据形状：
(2L,)
```

### 特性

Series和array最直观的不同大概就是index的多样性了，可以是其他任何值，不限于数字

Series和dict最直观的不同大概也是index的有序性，可以更具index来排序的，dict的排序默认是不支持的

```
In [76]:
s_selfdefined = Series([100, 200, 300, 150], index = ['2010' , '2012', '2014', '2016'])
s_selfdefined
Out[76]:
2010    100
2012    200
2014    300
2016    150
dtype: int64
```

### 序列化

Series.from_csv（path [，sep，parse_dates，…]） - 读取CSV文件（DISCOURAGED，请改用pandas.read_csv()）。 

Series.to_pickle（path） Pickle（序列化）对象到输入文件路径。 

Series.to_csv（[path，index，sep，na_rep，…]） 将系列写入逗号分隔值（csv）文件 

Series.to_dict() 将系列转换为{label - > value} 

Series.to_frame（[name]） 将系列转换为DataFrame 

Series.to_xarray() 从pandas对象返回一个xarray对象。 

Series.to_hdf（path_or_buf，key，\ \ kwargs） - 使用HDFStore将包含的数据写入HDF5文件。 

Series.to_sql（name，con [，flavor，schema，…]） - 将存储在DataFrame中的记录写入SQL数据库。 

Series.to_msgpack（[path_or_buf，encoding]） msgpack（serialize）对象到输入文件路径 

Series.to_json（[path_or_buf，orient，…]） 将对象转换为JSON字符串。 

Series.to_sparse（[kind，fill_value]） 将系列转换为稀疏系列 

Series.to_dense() 返回NDFrame的密集表示（而不是稀疏） 

Series.to_string（[buf，na_rep，…]） 呈现系列的字符串表示形式 

Series.to_clipboard（[excel，sep]）
Example:

```
s_selfdefined.to_pickle("../data/series.tmp")
s_read_from_file = pd.read_pickle("../data/series.tmp")
print(s_read_from_file.to_json())
{"2010":100,"2012":200,"2014":300,"2016":150}
```

### 索引迭代方式

Series.get（key [，default]） 从给定键的对象获取项目（DataFrame列，面板切片等）。 

Series.at 基于快速标签的标量访问器 

Series.iat 快速整数位置标量存取器。 

Series.ix 主要是基于标签位置的索引器，具有整数位置后备。 

Series.loc 纯标签位置索引器，用于按标签选择。 

Series.iloc 纯粹基于整数位置的索引，用于按位置选择。 

Series.iter() 提供对系列的值的迭代 

Series.iteritems() Lazily迭代（索引，值）元组
```
print(s_selfdefined.get('2014'))
300
```

### 其他

```
print("在末尾添加一行数据")
s_new = s_selfdefined.append(Series({'2017': None}))
print(s_new)

print("返回无null的数据")
print(s_new.dropna())

print("使用特定方法填充无值数据:")
s_has_unknown = s_new.fillna('unknown')
print(s_has_unknown)

print("字符串替换:")
s_replaced = s_has_unknown.replace('unknown', 0)
print(s_replaced)

print("按值从大到小排序：")
print(s_replaced.sort_values(axis=0, ascending=False))

print("按索引从小到大排序：")
print(s_replaced.sort_index())
在末尾添加一行数据
2010     100
2012     200
2014     300
2016     150
2017    None
dtype: object
返回无null的数据
2010    100
2012    200
2014    300
2016    150
dtype: object
使用特定方法填充无值数据:
2010        100
2012        200
2014        300
2016        150
2017    unknown
dtype: object
字符串替换:
2010    100
2012    200
2014    300
2016    150
2017      0
dtype: int64
按值从大到小排序：
2014    300
2012    200
2016    150
2010    100
2017      0
dtype: int64
按索引从小到大排序：
2010    100
2012    200
2014    300
2016    150
2017      0
dtype: int64
```

## DataFrame

### 构造方式

> 
DataFrame（[data，index，columns，dtype，copy]）


```
In [88]:
data = {"name":["Jack", "Rose", "Nameless"], "age":[22, 21, 99]}
df = DataFrame(data, index = [1, 2, 3])
df
Out[88]:
age name
1   22  Jack
2   21  Rose
3   99  Nameless
In [89]:
# dataframe的列也是可以排序的
f2 = DataFrame(data, index = ['1998', '1997', '1000'], columns = ['name', 'age'])
f2
Out[89]:
name    age
1998    Jack    22
1997    Rose    21
1000    Nameless    99
```

### 索引

DataFrame.head（[n]） 返回前n行 

DataFrame.at 基于快速标签的标量访问器 

DataFrame.iat 快速整数位置标量存取器。 

DataFrame.ix 主要是基于标签位置的索引器，具有整数位置后备。 

DataFrame.loc 纯标签位置索引器，用于按标签选择。 

DataFrame.iloc 纯粹基于整数位置的索引，用于按位置选择。 

DataFrame.insert（loc，column，value [，…] 在指定位置将列插入DataFrame。 

DataFrame.iter() 在信息轴上迭代 

DataFrame.iteritems() 迭代器结束（列名，系列）对。 

DataFrame.iterrows() 将DataFrame行重复为（索引，系列）对。 

DataFrame.itertuples（[index，name]） - 将DataFrame行迭代为namedtuples，索引值作为元组的第一个元素。 

DataFrame.lookup（row_labels，col_labels） DataFrame基于标签的“花式索引”功能。 

DataFrame.pop（item） 返回项目并从框架中删除。 

DataFrame.tail（[n]） 返回最后n行 

DataFrame.xs（键[，axis，level，drop_level]） 从Series / - DataFrame返回横截面（行或列）。 

DataFrame.isin（values） - 返回布尔值DataFrame，显示DataFrame中的每个元素是否包含在值中。 

DataFrame.where（cond [，other，inplace，…]） - 返回一个与self相同形状的对象，其对应的条目来自self，其中cond为True，否则为其他对象。 

DataFrame.mask（cond [，other，inplace，axis，…]） 返回一个与self相同形状的对象，并- 且其对应的条目来自self，其中cond是False，否则是来自其他。 

DataFrame.query（expr [，inplace]） 使用布尔表达式查询框架的列。
Example:

```
rint("返回表头和内容的前俩行")
print(df.head(2))

print("返回name这一列，包括索引")
print(df['name'])
返回表头和内容的前俩行
   age  name
1   22  Jack
2   21  Rose
返回name这一列，包括索引
1        Jack
2        Rose
3    Nameless
Name: name, dtype: object
```

### 其他

参照上面的Series，差不多

### 序列化

DataFrame.from_csv（path [，header，sep，…]） - 读取CSV文件（DISCOURAGED，请改用pandas.read_csv()）。 

DataFrame.from_dict（data [，orient，dtype]） - 从类array或dicts的dict构造DataFrame 

DataFrame.from_items（items [，columns，orient]） - 将（键，值）对转换为DataFrame。 

DataFrame.from_records（data [，index，…]） - 将结构化或记录ndarray转换为DataFrame 

DataFrame.info（[verbose，buf，max_cols，…]） DataFrame的简明摘要。 

DataFrame.to_pickle（path） Pickle（序列化）对象到输入文件路径。 

DataFrame.to_csv（[path_or_buf，sep，na_rep，…]） - 将DataFrame写入逗号分隔值（csv）文件 

DataFrame.to_hdf（path_or_buf，key，\ \ kwargs） - 使用HDFStore将包含的数据写入HDF5文件。 

DataFrame.to_sql（name，con [，flavor，…]） - 将存储在DataFrame中的记录写入SQL数据库。 

DataFrame.to_dict（[orient]） 将DataFrame转换成字典。 

DataFrame.to_excel（excel_writer [，…]） 将DataFrame写入excel表 

DataFrame.to_json（[path_or_buf，orient，…]） 将对象转换为JSON字符串。 

DataFrame.to_html（[buf，columns，col_space，…]） - 将DataFrame呈现为HTML表格。 

DataFrame.to_latex（[buf，columns，…]） 将DataFrame呈现为表格环境表。 

DataFrame.to_stata（fname [，convert_dates，…]） - 从数组类对象中写入Stata二进制dta文件的类 

DataFrame.to_msgpack（[path_or_buf，encoding]） - msgpack（serialize）对象到输入文件路径 

DataFrame.to_gbq（destination_table，project_id） 将DataFrame写入Google - BigQuery表格。 

DataFrame.to_records（[index，convert_datetime64]） - 将DataFrame转换为记录数组。 

DataFrame.to_sparse（[fill_value，kind]） 转换为SparseDataFrame 

DataFrame.to_dense() 返回NDFrame的密集表示（而不是稀疏） 

DataFrame.to_string（[buf，columns，…]） - 将DataFrame呈现为控制台友好的表格输出。 

DataFrame.to_clipboard（[excel，sep]）
Example:

```
In [95]:
df = pd.read_csv("../data/cities.csv")
df.head(10)
Out[95]:
province    city    district    area_code   x   y
0   北京市 北京  北京市 10  116.40  39.90
1   北京市 北京  天安门 10  116.38  39.90
2   北京市 北京  东城区 10  116.42  39.93
3   北京市 北京  西城区 10  116.37  39.92
4   北京市 北京  崇文区 10  116.43  39.88
5   北京市 北京  宣武区 10  116.35  39.87
6   北京市 北京  朝阳区 10  116.43  39.92
7   北京市 北京  丰台区 10  116.28  39.85
8   北京市 北京  石景山区    10  116.22  39.90
9   北京市 北京  海淀区 10  116.30  39.95
```

## 参考文献

[http://python.usyiyi.cn/documents/Pandas_0j2/api.html](http://python.usyiyi.cn/documents/Pandas_0j2/api.html)








