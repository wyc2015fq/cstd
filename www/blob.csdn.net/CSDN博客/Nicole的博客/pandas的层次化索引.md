# pandas的层次化索引 - Nicole的博客 - CSDN博客
2018年04月04日 14:38:17[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：651

importpandasaspdimportnumpyasnpimportmatplotlib.pyplotasplt

# 一、pandas的层次化索引
层次化索引是pandas的一项重要功能，它使你能够在一个轴上拥有多个（两个以上）索引级别。换句话来说，它能够让你以低纬度的形式处理高纬度数据。
## 1.Series的层次化索引¶
### （1）生成数据
```python
data = pd.Series(np.arange(1,10),index=[['a','a','a','b','b','c','c','d','d'],
                                    ['A','B','C','A','B','C','A','B','C']])
print(data)#输出s的结果
a  A    1
   B    2
   C    3
b  A    4
   B    5
c  C    6
   A    7
d  B    8
   C    9
dtype: int32
print(data.index)   #显示层次化索引
MultiIndex(levels=[['a', 'b', 'c', 'd'], ['A', 'B', 'C']],
           labels=[[0, 0, 0, 1, 1, 2, 2, 3, 3], [0, 1, 2, 0, 1, 2, 0, 1, 2]])
```
### （2）获取数据

```python
#获取索引为a的数据
data["a"]
A    1
B    2
C    3
dtype: int32
#在索引内层中获取数据,获取第二个索引为2的数据
data[:,'A']
a    1
b    4
c    7
dtype: int32
#带层次化索引的切片,包括右端的索引
data['a':'b']
a  A    1
   B    2
   C    3
b  A    4
   B    5
dtype: int32
```

### （3）数据重塑
层次化索引在数据重塑和透视表的应用中扮演者很重要的角色
data.unstack()#将解压data中的最后一层，默认值是解压最后一层
||A|B|C|
|----|----|----|----|
|1.0|2.0|3.0| |
|4.0|5.0|NaN| |
|7.0|NaN|6.0| |
|NaN|8.0|9.0| |

data.unstack()#解压data中的第一层
||a|b|c|d|
|----|----|----|----|----|
|1.0|4.0|7.0|NaN| |
|2.0|5.0|NaN|8.0| |
|3.0|NaN|6.0|9.0| |
```python
data.unstack().stack()  #unstack的逆运算是stack
a  A    1.0
   B    2.0
   C    3.0
b  A    4.0
   B    5.0
c  A    7.0
   C    6.0
d  B    8.0
   C    9.0
dtype: float64
type(data.unstack().stack())   #查看数据类型
pandas.core.series.Series
```

## 2.DataFrame的层次化索引
对于dataframe来说，行和列都可以进行层次化索引

#使用4个索引确定一个值data=pd.DataFrame(np.arange().reshape(,),index=[['a','a','b','b'],[,,,]],columns=[['A','A','B'],['Z','X','C']])

print(data)
|||A|B|
|----|----|----|----|
| | | | |
|0|1|2| |
|3|4|5| |
|6|7|8| |
|9|10|11| |

print(data['A'])#选取列

     Z   X
a 1  0   1
  2  3   4
b 1  6   7
  2  9  10
print(data.loc['a',:])

   A     B
   Z  X  C
1  0  1  2
2  3  4  5

data=pd.DataFrame(np.arange(,).reshape(,),index=['a','a','b','b','c'],columns=['A','B'])

data
||A|B|
|----|----|----|
|0|1| |
|2|3| |
|4|5| |
|6|7| |
|8|9| |
data1=data.set_index(['A'],drop=True)#seet_index函数会将一个或多个列转换成行索引，并创建一个新的dataframe

data1
||B|
|----|----|
| | |
|1| |
|3| |
|5| |
|7| |
|9| |

data.set_index(['A'],drop=False)#默认情况下，那些列会从dataframe中删除，但是也可以保留下来
||A|B|
|----|----|----|
| | | |
|0|1| |
|2|3| |
|4|5| |
|6|7| |
|8|9| |

data1.reset_index()#和set_index的功能相反
||A|B|
|----|----|----|
|0|1| |
|2|3| |
|4|5| |
|6|7| |
|8|9| |

## 3.重排分级顺序
在使用层次化索引的时候，我们可以重新调整某条轴上各级别的顺序，或根据级别上的值对数据进行排序。swaplevel接受两个级别编号或名称，返回一个互换了级别的新对象（数据不变）。我们可以对每个级别设置一个名称，就像对DataFrame设置行列索引的名称一样。

#使用4个索引确定一个值data=pd.DataFrame(np.arange().reshape(,),index=[['a','a','b','b'],[,,,]],columns=[['A','A','B'],['Z','X','C']])

data
|||A|B|
|----|----|----|----|
| | | | |
|0|1|2| |
|3|4|5| |
|6|7|8| |
|9|10|11| |
#设置行级别的名称data.index.names=['row1','row2']#设置列级别的名称data.columns.names=['col1','col2']print(data)

data.swaplevel('row1','row2')#交换行级别的位置
||col1|A|B|
|----|----|----|----|
| | | | |
| | | | |
|0|1|2| |
|3|4|5| |
|6|7|8| |
|9|10|11| |
swaplevel(self, i=-2, j=-1, axis=0)参数说明：i, j : int, string (can be mixed)，可以交换的水平的索引。        axis：取值为0或1，对行的水平进行交换还是对列的水平进行交换
data.sortlevel(axis=,ascending=True)#对列的水平进行排序
C:\Users\nicole.liang\AppData\Local\Continuum\anaconda3\lib\site-packages\ipykernel_launcher.py:1: FutureWarning: sortlevel is deprecated, use sort_index(level= ...)
  """Entry point for launching an IPython kernel.

||col1|A|B|
|----|----|----|----|
| | | | |
| | | | |
|1|0|2| |
|4|3|5| |
|7|6|8| |
|10|9|11| |

#交换行级别，然后再对第一行排序data.swaplevel(,,axis=).sortlevel(axis=,ascending=False)
C:\Users\nicole.liang\AppData\Local\Continuum\anaconda3\lib\site-packages\ipykernel_launcher.py:2: FutureWarning: sortlevel is deprecated, use sort_index(level= ...)
  

||col2|Z|X|C|
|----|----|----|----|----|
| | | | | |
| | | | | |
|9|10|11| | |
|6|7|8| | |
|3|4|5| | |
|0|1|2| | |

### 4.根据级别汇总统计

#使用4个索引确定一个值
```python
data = pd.DataFrame(np.arange(12).reshape(4,3),
                   index=[['a','a','b','b'],[1,2,1,2]],
                   columns=[['A','A','B'],['Z','X','C']])  
print(data)
     A       B
     Z   X   C
a 1  0   1   2
  2  3   4   5
b 1  6   7   8
  2  9  10  11
#设置行级别的名称
data.index.names = ['row1','row2']   
#设置列级别的名称
data.columns.names = ['col1','col2']
print(data)
col1       A       B
col2       Z   X   C
row1 row2           
a    1     0   1   2
     2     3   4   5
b    1     6   7   8
     2     9  10  11
print(data.sum(level='row1'))   #根据级别的名称进行求和
col1   A       B
col2   Z   X   C
row1            
a      3   5   7
b     15  17  19
print(data.sum(level='col1',axis=1))   #根据指定列的名称进行求和
```

```python
col1        A   B
row1 row2        
a    1      1   2
     2      7   5
b    1     13   8
     2     19  11
```
