# pandas基础教程 - Joe的博客 - CSDN博客





2018年04月15日 23:33:47[Joe-Han](https://me.csdn.net/u010089444)阅读数：1679








首先引入相关模块：

```
import numpy as np
import pandas as pd
```

## 1. DataFrame

### 1.1 创建Series

index不指定则从0开始编号

```
s = pd.Series([1, 2, 3, np.nan], index=['A', 'B', 'C', 'D'])
print s
```

> 
输出 

  A    1.0 

  B    2.0 

  C    3.0 

  D    NaN 

  dtype: float64
### 1.2 创建时间序列

必须指定start、end、periods中的两个参数值

```
dates = pd.date_range('20180101', periods=5)
print dates
```

> 
输出 

  DatetimeIndex([‘2018-01-01’, ‘2018-01-02’, ‘2018-01-03’, ‘2018-01-04’, 

                 ‘2018-01-05’], 

                dtype=’datetime64[ns]’, freq=’D’)
### 1.3 创建DataFrame

```
df = pd.DataFrame(np.random.rand(3,4),columns=['a', 'b', 'c', 'd'])
print df
```

> 
输出 

                  a         b         c         d 

  0  0.233310  0.170256  0.036988  0.697916 

  1  0.159580  0.287814  0.528123  0.956051 

  2  0.815038  0.438103  0.143477  0.769143
通过字典创建（key为列名）：

```
df = pd.DataFrame({'A': 1,
                   'B': pd.Timestamp('20171208'),
                   'C': pd.Series(np.arange(4)),
                   'D': pd.Categorical(['test', 'train', 'test', 'train'])})
print df
```

> 
输出 

     A          B  C      D 

  0  1 2017-12-08  0   test 

  1  1 2017-12-08  1  train 

  2  1 2017-12-08  2   test 

  3  1 2017-12-08  3  train
### 1.4 DataFrame属性

```bash
df = pd.DataFrame({'A': 1,
                   'B': pd.Timestamp('20171208'),
                   'C': pd.Series(np.arange(4)),
                   'D': pd.Categorical(['test', 'train', 'test', 'train'])})
```

查看每列数据类型
`print df.dtypes`
> 
输出 

  A             int64 

  B    datetime64[ns] 

  C             int32 

  D          category 

  dtype: object
查看索引
`print df.index   `
> 
输出 

  RangeIndex(start=0, stop=4, step=1)


查看列名
`print df.columns`
> 
输出 

  Index([u’A’, u’B’, u’C’, u’D’], dtype=’object’)


查看数据
`print df.values  `
> 
输出 

  [[1L Timestamp(‘2017-12-08 00:00:00’) 0 ‘test’] 

   [1L Timestamp(‘2017-12-08 00:00:00’) 1 ‘train’] 

   [1L Timestamp(‘2017-12-08 00:00:00’) 2 ‘test’] 

   [1L Timestamp(‘2017-12-08 00:00:00’) 3 ‘train’]]
DataFrame统计信息
`print df.describe()`
> 
输出 

           A         C 

  count  4.0  4.000000 

  mean   1.0  1.500000 

  std    0.0  1.290994 

  min    1.0  0.000000 

  25%    1.0  0.750000 

  50%    1.0  1.500000 

  75%    1.0  2.250000 

  max    1.0  3.000000
### 1.5 DataFrame常用操作

转置

```
df = pd.DataFrame(np.arange(12).reshape((3,4)),columns=['a', 'b', 'c', 'd'])
print df.T
```

> 
输出 

     0  1   2 

  a  0  4   8 

  b  1  5   9 

  c  2  6  10 

  d  3  7  11
按index排序

```
df = pd.DataFrame(np.arange(12).reshape((3, 4)), columns=['a', 'b', 'c', 'd'])
print df.sort_index(axis=1, ascending=False)
```

> 
输出 

      d   c  b  a 

  0   3   2  1  0 

  1   7   6  5  4 

  2  11  10  9  8
按值排序

```
df = pd.DataFrame(np.arange(12).reshape((3, 4)), columns=['a', 'b', 'c', 'd'])
print df.sort_values(by='b', ascending=False)
```

> 
输出 

     a  b   c   d 

  2  8  9  10  11 

  1  4  5   6   7 

  0  0  1   2   3
## 2. 数据选择

创建如下dataframe：

```
dates = pd.date_range('20180101', periods=3)
df = pd.DataFrame(np.arange(12).reshape((3, 4)),
                  index=dates, columns=['a', 'b', 'c', 'd'])
print df
```

> 
输出 

              a  b   c   d 

  2018-01-01  0  1   2   3 

  2018-01-02  4  5   6   7 

  2018-01-03  8  9  10  11
### 2.1 选择列
`print df['a']  # print df.a`
> 
输出 

  2018-01-01    0 

  2018-01-02    4 

  2018-01-03    82.2 选择行``print df[0:2]  # print df['20180101':'20180102']
> 


输出 

              a  b  c  d 

  2018-01-01  0  1  2  3 

  2018-01-02  4  5  6  72.3 标签选择：loc``print df.loc[:,['a', 'b']] 
> 


输出 

              a  b 

  2018-01-01  0  1 

  2018-01-02  4  5 

  2018-01-03  8  9``print df.loc['20180102']
> 


输出 

  a    4 

  b    5 

  c    6 

  d    7 

  Name: 2018-01-02 00:00:00, dtype: int322.4 位置选择：iloc``printdf.iloc[[0, 2], 2:4]
> 


输出 

               c   d 

  2018-01-01   2   3 

  2018-01-03  10  112.5 混合标签与位置：ix``print df.ix[:2, ['c', 'd']]
> 


输出 

              c  d 

  2018-01-01  2  3 

  2018-01-02  6  72.6 Boolean选择``print df[df.a < 5]
> 


输出 

              a  b  c  d 

  2018-01-01  0  1  2  3 

  2018-01-02  4  5  6  73. 处理NaN数据

首先创建包含NaN的dataframe：
```matlab

```
dates = pd.date_range('20180101', periods=3)
df = pd.DataFrame(np.arange(12).reshape((3, 4)),
                  index=dates, columns=['a', 'b', 'c', 'd'])
df.iloc[1, 1], df.iloc[2, 2] = np.nan, np.nan
print df
> 


输出 

              a    b    c   d 

  2018-01-01  0  1.0  2.0   3 

  2018-01-02  4  NaN  6.0   7 

  2018-01-03  8  9.0  NaN  11 3.1 删除NaN数据``print df.dropna(axis=1)  # how = ['any', 'all']
> 


输出 

              a   d 

  2018-01-01  0   3 

  2018-01-02  4   7 

  2018-01-03  8  113.2 填充NaN数据``print df.fillna(value='*')
> 


输出 

              a  b  c   d 

  2018-01-01  0  1  2   3 

  2018-01-02  4  *  6   7 

  2018-01-03  8  9  *  113.3 检查是否存在NaN``print df.isnull()
> 


输出 

                  a      b      c      d 

  2018-01-01  False  False  False  False 

  2018-01-02  False   True  False  False 

  2018-01-03  False  False   True  False4. 导入与导出导入函数导出函数功能read_csvto_csvread_excelto_excelread_sqlto_sqlread_jsonto_jsonread_msgpackto_msgpackread_htmlto_htmlread_gbqto_gbqread_statato_stataread_sasto_sasread_clipboardto_clipboardread_pickleto_pickle

详见：[](http://pandas.pydata.org/pandas-docs/stable/api.html#input-output)input-output

以下面这个test.txt为例：
> 


A B 

  Tom 21 

  Joe 26 

  Sam 55 

  Kerry 27

忽略第一行，并设置列名分别为‘name’和‘age’``data = pd.read_csv('test.txt', sep=' ', skiprows=1, names=['name','age'])
print data
> 


输出 

      name  age 

  0    Tom   21 

  1    Joe   26 

  2    Sam   55 

  3  Kerry   275. 合并DataFrame5.1 concat函数``df1 = pd.DataFrame(np.ones((3, 4))*0, columns=['a', 'b', 'c', 'd'])
df2 = pd.DataFrame(np.ones((3, 4))*1, columns=['a', 'b', 'c', 'd'])
df3 = pd.DataFrame(np.ones((3, 4))*2, columns=['a', 'b', 'c', 'd'])
# ignore_index=True将重新对index排序
print pd.concat([df1, df2, df3], axis=0, ignore_index=True)
> 


输出 

       a    b    c    d 

  0  0.0  0.0  0.0  0.0 

  1  0.0  0.0  0.0  0.0 

  2  0.0  0.0  0.0  0.0 

  3  1.0  1.0  1.0  1.0 

  4  1.0  1.0  1.0  1.0 

  5  1.0  1.0  1.0  1.0 

  6  2.0  2.0  2.0  2.0 

  7  2.0  2.0  2.0  2.0 

  8  2.0  2.0  2.0  2.0

join参数用法``df1 = pd.DataFrame(np.ones((3, 4))*0, columns=['a', 'b', 'c', 'd'], index=[1,2,3])
df2 = pd.DataFrame(np.ones((3, 4))*1, columns=['b', 'c', 'd', 'e'], index=[2,3,4])
# join默认为'outer'，不共有的列用NaN填充print pd.concat([df1, df2], join='outer') 
# join='inner'只合并共有的列print pd.concat([df1, df2], join='inner')
> 


输出 1 

       a    b    c    d    e 

  1  0.0  0.0  0.0  0.0  NaN 

  2  0.0  0.0  0.0  0.0  NaN 

  3  0.0  0.0  0.0  0.0  NaN 

  2  NaN  1.0  1.0  1.0  1.0 

  3  NaN  1.0  1.0  1.0  1.0 

  4  NaN  1.0  1.0  1.0  1.0 

  输出 2 

       b    c    d 

  1  0.0  0.0  0.0 

  2  0.0  0.0  0.0 

  3  0.0  0.0  0.0 

  2  1.0  1.0  1.0 

  3  1.0  1.0  1.0 

  4  1.0  1.0  1.0

join_axes参数用法``df1 = pd.DataFrame(np.ones((3, 4))*0, columns=['a', 'b', 'c', 'd'], index=[1, 2, 3])
df2 = pd.DataFrame(np.ones((3, 4))*1, columns=['b', 'c', 'd', 'e'], index=[2, 3, 4])
# 按照df1的index进行合并print pd.concat([df1, df2], axis=1, join_axes=[df1.index])
> 


输出 

       a    b    c    d    b    c    d    e 

  1  0.0  0.0  0.0  0.0  NaN  NaN  NaN  NaN 

  2  0.0  0.0  0.0  0.0  1.0  1.0  1.0  1.0 

  3  0.0  0.0  0.0  0.0  1.0  1.0  1.0  1.05.2 append函数

append多个DataFrame``df1 = pd.DataFrame(np.ones((3, 4))*0, columns=['a', 'b', 'c', 'd'])
df2 = pd.DataFrame(np.ones((3, 4))*1, columns=['a', 'b', 'c', 'd'])

print df1.append(df2, ignore_index=True)
> 


输出 

       a    b    c    d 

  0  0.0  0.0  0.0  0.0 

  1  0.0  0.0  0.0  0.0 

  2  0.0  0.0  0.0  0.0 

  3  1.0  1.0  1.0  1.0 

  4  1.0  1.0  1.0  1.0 

  5  1.0  1.0  1.0  1.0

append一组数据``df1 = pd.DataFrame(np.ones((3, 4))*0, columns=['a', 'b', 'c', 'd'])
s = pd.Series([4, 4, 4, 4], index=['a', 'b', 'c', 'd'])

print df1.append(s, ignore_index=True)
> 


输出 

       a    b    c    d 

  0  0.0  0.0  0.0  0.0 

  1  0.0  0.0  0.0  0.0 

  2  0.0  0.0  0.0  0.0 

  3  4.0  4.0  4.0  4.05.3 merge函数

基于某一列进行合并``df1 = pd.DataFrame({'A': ['A1', 'A2', 'A3'],
                    'B': ['B1', 'B2', 'B3'],
                   'KEY': ['K1', 'K2', 'K3']})
df2 = pd.DataFrame({'C': ['C1', 'C2', 'C3'],
                    'D': ['D1', 'D2', 'D3'],
                   'KEY': ['K1', 'K2', 'K3']})

print pd.merge(df1, df2, on='KEY')
> 


输出 

      A   B KEY   C   D 

  0  A1  B1  K1  C1  D1 

  1  A2  B2  K2  C2  D2 

  2  A3  B3  K3  C3  D3

基于某两列进行合并``df1 = pd.DataFrame({'A': ['A1', 'A2', 'A3'],
                    'B': ['B1', 'B2', 'B3'],
                    'KEY1': ['K1', 'K2', 'K0'],
                    'KEY2': ['K0', 'K1', 'K3']})
df2 = pd.DataFrame({'C': ['C1', 'C2', 'C3'],
                    'D': ['D1', 'D2', 'D3'],
                    'KEY1': ['K0', 'K2', 'K1'],
                    'KEY2': ['K1', 'K1', 'K0']})
# how:['left','right','outer','inner']
print pd.merge(df1, df2, on=['KEY1', 'KEY2'], how='inner')
> 


输出 

      A   B KEY1 KEY2   C   D 

  0  A1  B1   K1   K0  C3  D3 

  1  A2  B2   K2   K1  C2  D2

按index合并
```php

```
df1 = pd.DataFrame({'A': ['A1', 'A2', 'A3'],
                    'B': ['B1', 'B2', 'B3']},
                   index=['K0', 'K1', 'K2'])
df2 = pd.DataFrame({'C': ['C1', 'C2', 'C3'],
                    'D': ['D1', 'D2', 'D3']},
                   index=['K0', 'K1', 'K3'])

print pd.merge(df1, df2, left_index=True, right_index=True, how='outer')
> 


输出 

        A    B    C    D 

  K0   A1   B1   C1   D1 

  K1   A2   B2   C2   D2 

  K2   A3   B3  NaN  NaN 

  K3  NaN  NaN   C3   D3

为列加后缀``df_boys = pd.DataFrame({'id': ['1', '2', '3'],
                        'age': ['23', '25', '18']})
df_girls = pd.DataFrame({'id': ['1', '2', '3'],
                        'age': ['18', '18', '18']})

print pd.merge(df_boys, df_girls, on='id', suffixes=['_boys', '_girls'])
> 


输出 

    age_boys id age_girls 

  0       23  1        18 

  1       25  2        18 

  2       18  3        186. 绘图

引入相应模块``import numpy as npimport pandas as pdimport matplotlib.pyplot as plt

绘制Series``data = pd.Series(np.random.randn(1000))data = data.cumsum()data.plot()plt.show()


![这里写图片描述](https://img-blog.csdn.net/20180415232128456?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAwODk0NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



绘制DataFrame``data = pd.DataFrame(np.random.randn(1000, 4), columns=['A', 'B', 'C', 'D'])data = data.cumsum()data.plot()plt.show()


![这里写图片描述](https://img-blog.csdn.net/20180415232357372?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAwODk0NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

``data = pd.DataFrame(np.random.randn(1000, 4), columns=['A', 'B', 'C', 'D'])data = data.cumsum()ax = data.plot.scatter(x='A', y='B', color='DarkRed', label='class 1')data.plot.scatter(x='C', y='D', color='DarkGreen', label='class 2', ax=ax)plt.show()


![这里写图片描述](https://img-blog.csdn.net/20180415233110858?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAwODk0NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



更多绘图详见：[](http://pandas.pydata.org/pandas-docs/version/0.13.1/visualization.html)visualization


























