# 第3周-强大的数据结构（字典集合+scipy的数据结构） - 博客堂 - CSDN博客





2017年06月05日 11:35:49[最小森林](https://me.csdn.net/u012052268)阅读数：1013










- [字典与集合](#字典与集合)- [定义](#定义)

- [scipy库](#scipy库)- [ndarry](#1-ndarry)- [1创建多维数组](#11创建多维数组)
- [2类型转换](#12类型转换)
- [3 ndarray 的简单使用](#13-ndarray-的简单使用)

- [pandas的series变长字典](#2-pandas的series变长字典)- [1生成series](#21生成series)
- [2访问Series中的元素和索引](#22访问series中的元素和索引)
- [3简单运算](#23简单运算)
- [4 Series的自动对齐](#24-series的自动对齐)
- [5命名](#25命名)

- [pandas的dataframe](#3-pandas的dataframe)- [1 DataFrame 对象的构建](#31-dataframe-对象的构建)
- [2 DataFrame 内容访问](#32-dataframe-内容访问)
- [3 DataFrame 对象的修改](#33-dataframe-对象的修改)






# 字典与集合

## 定义:

list:链表,有序的项目,通过索引进行查找,使用方括号”[]”; 

tuple:元组,元组将多样的对象集合到一起,不能修改,通过索引进行查找, 使用括号”()”; 

dict:字典,字典是一组键(key)和值(value)的组合,通过键(key)进行查找,没有顺序, 使用大括号”{}”; 

set:集合,无序,元素只出现一次,自动去重,使用”set([])” 

set和lsit可以自由转换，在删除list中多个/海量重复元素时，可以先转换成set，然后再转回list并排序(set没有排序)。此种方法不仅方便且效率较高。

# scipy库

SciPy是一个基于Python的软件生态圈 它包括如NumPy  Matplotlib和pandas 这样的一些核心库。

它有非常丰富强大的数据结构和函数使用起来也非常便利 。

SciPy中使用了一些Python标准的数据结构的扩展形式 比如说 

- ndarrayN维数组 

- Series变长字典 

- 数据框DataFrame 
## 1 ndarry

### 1.1创建多维数组

```php
import numpy as np
xArray = np.array([(1,2,3),(2,3,4)])
print(xArray)

# 特殊的，零矩阵，和1矩阵
np.zero([2,3])
np.ones() 

# empty() 函数
这个函数可以创建一个没有任何具体值的 ndarray 数组

# 总之，数组的形式必须用[]中括号
```

2ndarray 的数据类型 与 转换 
![image](http://images.cnblogs.com/cnblogs_com/sirkevin/861351/o_ch04-4.png)
### 1.2类型转换

通过 ndarray 的 astype() 方法进行强制类型转换，浮点数转换为整数时小数部分会被舍弃：

![image](http://images.cnblogs.com/cnblogs_com/sirkevin/861351/o_ch04-5.png)

### 1.3 ndarray 的简单使用

使用 ndarray 数组可以让我们不需要使用循环就可以对列表里的元素执行操作，语法和对标量元素的操作一样，例如： 
![image](http://images.cnblogs.com/cnblogs_com/sirkevin/861351/o_ch04-7.png)
还有一些常用函数：

```
>>> x.size   #获得数组中元素的个数
>>> x.shape  #获得数组的（行数，列数）

abs()取绝对值
sqrt()计算元素平方根
sort()排序

power(a,b)计算a的b次方

# 等等，用到的时候查书
```

## 2 pandas的series（变长字典）

Series 是一个类数组的数据结构，同时带有标签（lable）或者说索引（index）。

### 2.1生成series

1.1 下边生成一个最简单的Series对象，因为没有给Series指定索引，所以此时会使用默认索引(从0到N-1)。

```
# 引入Series和DataFrame
In [16]: from pandas import Series,DataFrame
In [17]: import pandas as pd

In [18]: ser1 = pd.Series(range(1,5))

In [19]: ser1
Out[19]: 
0    1
1    2
2    3
3    4
dtype: int32
```

1.2 当要生成一个指定索引的Series 时候，可以这样：　　

```
# 给index指定一个list
In [23]: ser2 = Series(range(4),index = ["a","b","c","d"])

In [24]: ser2
Out[24]: 
a    0
b    1
c    2
d    3
dtype: int64
```

1.3 也可以通过字典来创建Series对象

```
In [45]: sdata = {'Ohio': 35000, 'Texas': 71000, 'Oregon': 16000, 'Utah': 5000}

In [46]: ser3 = Series(sdata)
# 可以发现，用字典创建的Series是按index有序的
In [47]: ser3
Out[47]: 
Ohio      35000
Oregon    16000
Texas     71000
Utah       5000
dtype: int64
```

　在用字典生成Series的时候，也可以指定索引，当索引中值对应的字典中的值不存在的时候，则此索引的值标记为Missing，NA，并且可以通过函数（pandas.isnull，pandas.notnull）来确定哪些索引对应的值是没有的。

```
In [48]: states = ['California', 'Ohio', 'Oregon', 'Texas']

In [49]: ser3 = Series(sdata,index = states)

In [50]: ser3
Out[50]: 
California        NaN
Ohio          35000.0
Oregon        16000.0
Texas         71000.0
dtype: float64
# 判断哪些值为空
In [51]: pd.isnull(ser3)
Out[51]: 
California     True
Ohio          False
Oregon        False
Texas         False
dtype: bool

In [52]: pd.notnull(ser3)
Out[52]: 
California    False
Ohio           True
Oregon         True
Texas          True
dtype: bool
```

### 2.2访问Series中的元素和索引：

```python
# 访问索引为"a"的元素
In [25]: ser2["a"]
Out[25]: 0
# 访问索引为"a","c"的元素
In [26]: ser2[["a","c"]]
Out[26]: 
a    0
c    2
dtype: int64
# 获取所有的值
In [27]: ser2.values
Out[27]: array([0, 1, 2, 3])
# 获取所有的索引
In [28]: ser2.index
Out[28]: Index([u'a', u'b', u'c', u'd'], dtype='object')
```

### 2.3简单运算

　　在pandas的Series中，会保留NumPy的数组操作（用布尔数组过滤数据，标量乘法，以及使用数学函数），并同时保持引用的使用

```
In [34]: ser2[ser2 > 2]
Out[34]: 
a    64
d     3
dtype: int64

In [35]: ser2 * 2
Out[35]: 
a    128
b      2
c      4
d      6
dtype: int64

In [36]: np.exp(ser2)
Out[36]: 
a    6.235149e+27
b    2.718282e+00
c    7.389056e+00
d    2.008554e+01
dtype: float64
```

### 2.4 Series的自动对齐

　Series的一个重要功能就是自动对齐（不明觉厉），看看例子就明白了。 差不多就是不同Series对象运算的时候根据其索引进行匹配计算。 


```
# ser3 的内容
In [60]: ser3
Out[60]: 
Ohio      35000
Oregon    16000
Texas     71000
Utah       5000
dtype: int64
# ser4 的内容
In [61]: ser4
Out[61]: 
California        NaN
Ohio          35000.0
Oregon        16000.0
Texas         71000.0
dtype: float64
# 相同索引值的元素相加
In [62]: ser3 + ser4
Out[62]: 
California         NaN
Ohio           70000.0
Oregon         32000.0
Texas         142000.0
Utah               NaN
dtype: float64
```

### 2.5命名

Series对象本身，以及索引都有一个 name 属性

```
In [64]: ser4.index.name = "state"

In [65]: ser4.name = "population"

In [66]: ser4
Out[66]: 
state
California        NaN
Ohio          35000.0
Oregon        16000.0
Texas         71000.0
Name: population, dtype: float64
```

## 3 pandas的dataframe （❤）

### 3.1 DataFrame 对象的构建

1 用包含等长的列表或者是NumPy数组的字典创建DataFrame对象

```
In [68]: import pandas as pd

In [69]: from pandas import Series,DataFrame

# 建立包含等长列表的字典类型
In [70]:  data = {'state': ['Ohio', 'Ohio', 'Ohio', 'Nevada', 'Nevada'],'year': [2000, 2001, 20
    ...: 02, 2001, 2002],'pop': [1.5, 1.7, 3.6, 2.4, 2.9]}
In [71]: data
Out[71]: 
{'pop': [1.5, 1.7, 3.6, 2.4, 2.9],
 'state': ['Ohio', 'Ohio', 'Ohio', 'Nevada', 'Nevada'],
 'year': [2000, 2001, 2002, 2001, 2002]}
# 建立DataFrame对象
In [72]: frame1 = DataFrame(data)
# 红色部分为自动生成的索引
In [73]: frame1
Out[73]: 
   pop   state  year
 1.5    Ohio  2000
 1.7    Ohio  2001
 3.6    Ohio  2002
 2.4  Nevada  2001
 2.9  Nevada  2002
```

在建立过程中可以指点列的顺序：

```
In [74]: frame1 = DataFrame(data,columns=['year', 'state', 'pop'])

In [75]: frame1
Out[75]: 
   year   state  pop
 2000    Ohio  1.5
 2001    Ohio  1.7
 2002    Ohio  3.6
 2001  Nevada  2.4
 2002  Nevada  2.9
```

和Series一样，DataFrame也是可以指定索引内容：

```
In [76]: ind = ['one', 'two', 'three', 'four', 'five']
In [77]: frame1 = DataFrame(data,index = ind)

In [78]: frame1
Out[78]: 
       pop   state  year
one    1.5    Ohio  2000
two    1.7    Ohio  2001
three  3.6    Ohio  2002
four   2.4  Nevada  2001
five   2.9  Nevada  2002
```

### 3.2 DataFrame 内容访问

从DataFrame中获取一列的结果为一个Series，可以通过以下两种方式获取：

```
# 以字典索引方式获取
In [100]: frame1["state"]
Out[100]: 
one        Ohio
two        Ohio
three      Ohio
four     Nevada
five     Nevada
Name: state, dtype: object
# 以属性方式获取
In [101]: frame1.state
Out[101]: 
one        Ohio
two        Ohio
three      Ohio
four     Nevada
five     Nevada
Name: state, dtype: object
```

也可以通过ix获取**一行**数据：

```
In [109]: frame1.ix["one"] # 或者是 frame1.ix[0]
Out[109]: 
pop       1.5
state    Ohio
year     2000
Name: one, dtype: object
# 获取多行数据
In [110]: frame1.ix[["tow","three","four"]]
Out[110]: 
       pop   state    year
tow    NaN     NaN     NaN
three  3.6    Ohio  2002.0
four   2.4  Nevada  2001.0
# 还可以通过默认数字行索引来获取数据
In [111]: frame1.ix[range(3)]
Out[111]: 
       pop state  year
one    1.5  Ohio  2000
two    1.7  Ohio  2001
three  3.6  Ohio  2002
```

获取指定行，指定列的交汇值：

```
In [119]: frame1["state"]
Out[119]: 
one        Ohio
two        Ohio
three      Ohio
four     Nevada
five     Nevada
Name: state, dtype: object

In [120]: frame1["state"][0]
Out[120]: 'Ohio'

In [121]: frame1["state"]["one"]
Out[121]: 'Ohio'
```

### 3.3 DataFrame 对象的修改

　　增加一列，并所有赋值为同一个值：

```
# 增加一列值
In [131]: frame1["debt"] = 10

In [132]: frame1
Out[132]: 
       pop   state  year  debt
one    1.5    Ohio  2000    10
two    1.7    Ohio  2001    10
three  3.6    Ohio  2002    10
four   2.4  Nevada  2001    10
five   2.9  Nevada  2002    10

# 更改一列的值
In [133]: frame1["debt"] = np.arange(5)

In [134]: frame1
Out[134]: 
       pop   state  year  debt
one    1.5    Ohio  2000     0
two    1.7    Ohio  2001     1
three  3.6    Ohio  2002     2
four   2.4  Nevada  2001     3
five   2.9  Nevada  2002     4
```

追加类型为Series的一列

```
# 判断是否为东部区
In [137]: east = (frame1.state == "Ohio")

In [138]: east
Out[138]: 
one       True
two       True
three     True
four     False
five     False
Name: state, dtype: bool
# 赋Series值
In [139]: frame1["east"] = east

In [140]: frame1
Out[140]: 
       pop   state  year  debt   east
one    1.5    Ohio  2000     0   True
two    1.7    Ohio  2001     1   True
three  3.6    Ohio  2002     2   True
four   2.4  Nevada  2001     3  False
five   2.9  Nevada  2002     4  False
```

DataFrame 的行是可以命名的，同时多列也可以命名：

```
In [145]: frame3.columns.name = "state"

In [146]: frame3.index.name = "year"

In [147]: frame3
Out[147]: 
state  Nevada  Ohio
year               
     2.9   3.6
     2.4   1.7
     NaN   1.5
```






