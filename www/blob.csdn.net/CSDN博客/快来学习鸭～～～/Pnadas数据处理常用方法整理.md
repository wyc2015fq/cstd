# Pnadas数据处理常用方法整理 - 快来学习鸭～～～ - CSDN博客





2017年02月21日 13:43:50[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：1273








**author：DivinerShi**

[http://blog.csdn.net/sxf1061926959/article/details/56280759](http://blog.csdn.net/sxf1061926959/article/details/56280759)

本人在Python中使用最多的两个数据处理库，一个是Numpy，一个是Pandas。个人认为pandas使用起来比numpy方便很多，它包含更多的内置函数去处理数据。今天就聊一聊近期我用pandas处理数据的一些方法，因为不是一直用，怕忘记了，记录下用作备忘录。

1、加载txt，csv文件

```
import pandas as pd
myfile = pd.read_csv(‘myfile.txt’)
#里面有些参数，如header是设置表头，index是是否有索引，sep 是分割符号，等
#加载进来的myfile是一个pandas的Dataframe格式的数据。
```

2、保存文件
`myfile.to_csv(‘myfile.txt’,header=True)#同样有参数，这里的header=True表示，保存表头`
3、若原始数据没有列名（表头），加载的数据添加列名即可。

```python
import pandas as pd
myfile = pd.read_csv(‘myfile.txt’,header=None)
myfile.columns = [‘id’,’name’,’sex’]
```

4、Datframe修改列名

```
#列名写错了，或者经过处理后需要修改列名的
myfile.rename(columns={‘id’:’change_id’},inplace=True)
#python中大部分操作中都有inplace，表示操作后的结果是否用于替换原来的表
```

5、Datafrme删除一整列

```
#方法一：
Del myfile[‘sex’]
#方法二：
1. DF= DF.drop('column_name', 1)；
2. DF.drop('column_name',axis=1, inplace=True)
3. DF.drop([DF.columns[[0,1, 3]]], axis=1,inplace=True)   # Note: zero indexed
```

6、Dataframe截取一部分数据
`Newfile = myfile[myfile.id>’100’]`
7、Dataframe填充Nan空值

```
Myfile.fiina(0)
#表示用0填充Nan
```

8、多个Dataframe合并 

Pandas中用于合并两个Dataframe的方法有两种，分别是merge和concat 
**merge：**

Merge相当于SQL中的join，通过相同的列名来进行合并，如： 

File = pd.merge(myfile_left,myfile_right,on=’id’,how=’left’) 

这里我将两个表，myfile_left和myfile_right通过相同的列名id连在了一起。How里表示的是连接方法，和SQL一样，还有inner和outer等。Left表示以左表的id为主进行合并。 
**参数说明：**

myfile_left与myfile_right：两个不同的DataFrame 

how：指的是合并(连接)的方式有inner(内连接),left(左外连接),right(右外连接),outer(全外连接);默认为inner 

on : 指的是用于连接的列索引名称。必须存在右右两个DataFrame对象中，如果没有指定且其他参数也未指定则以两个DataFrame的列名交集做为连接键 

left_on：左则DataFrame中用作连接键的列名;这个参数中左右列名不相同，但代表的含义相同时非常有用。 

right_on：右则DataFrame中用作 连接键的列名 

left_index：使用左则DataFrame中的行索引做为连接键 

right_index：使用右则DataFrame中的行索引做为连接键 

sort：默认为True，将合并的数据进行排序。在大多数情况下设置为False可以提高性能 

suffixes：字符串值组成的元组，用于指定当左右DataFrame存在相同列名时在列名后面附加的后缀名称，默认为(‘_x’,’_y’) 

copy：默认为True,总是将数据复制到数据结构中；大多数情况下设置为False可以提高性能 

indicator：在 0.17.0中还增加了一个显示合并数据中来源情况；如只来自己于左边(left_only)、两者(both) 

默认情况下以相同列为合并条件，可以不写on
```
df1=DataFrame({'key':['a','b','b'],'data1':range(3)}) 
df2=DataFrame({'key':['a','b','c'],'data2':range(3)})  
pd.merge(df1,df2)
```

多建连接：在on中传入多个列名，如：
`pd.merge(df1,df2,on=['key1','key2']`
当两个对象的列名不同时，也可以分别指定，如:
`pd.merge(df1,df2,left_on='lkey',right_on='rkey')`
**Concat:**

Concat沿着指定的轴进行合并 

当没有相同的列名可以使用的时候，或者只是单纯的想把两个表堆叠时使用concat比merge方便。如：
```
pd.concat([df1,df2]，axis=0) 
#axis=0表示在纵向上进行堆叠，axis=1表示在横向上进行堆叠。
```

9、查看Dataframe统计信息

```
df.head() #显示前五行
df.tail()#显示后五行
统计函数：
df.count() #非空元素计算 ，即计数
df.min() #最小值 
df.max() #最大值
df.idxmin() #最小值的位置，类似于R中的which.min函数 
df.idxmax() #最大值的位置，类似于R中的which.max函数 
df.quantile(0.1) #10%分位数 
df.sum() #求和 
df.mean() #均值 
df.median() #中位数 
df.mode() #众数 
df.var() #方差 
df.std() #标准差 
df.mad() #平均绝对偏差 
df.skew() #偏度 
df.kurt() #峰度 
df.describe() #一次性输出多个描述性统计指标 ```
```

10、Apply、map函数

为了对每一行进行处理，可以直接用apply或者map 

例如df中有列名time，形如2017-02-03，我们计算每行数据属于星期几

```
from datetime import date
df['weekday']=df.time.apply(lambda x:date(int(x.split('-')[0]),int(x.split('-')[1]),int(x.split('-')[2])).weekday()+1)
#这样df中增加了一列weekday。
#Split()函数是用于分割字符串的，上面的x.split(‘-’)[0]表示将x根据‘-’分割，取出分割后的第一份数据。
```

有时候对每行数据要进行复杂的操作，可能用简单的一行写不下，那么可以单独写一个函数，来处理，再将函数传给apply函数即可，如：

```
def computeweek（x）
    Do it
    Return num
#定义函数
df['weekday']=df.time.apply(computeweek)
#这里可以不用传参数x，apply函数可以自动传入参数。
```

同样，map函数也对每一行进行处理，如

```
df[‘year’]=df.time.map(x:x.split('-')[0])
#表示将time列只保留年份
```

apply和map的作用很强大，可以简化很多步骤，之前很多实现代码都是用numpy写的，各种for循环，直接写哭，用了apply和map后真的感觉是：life is short， use python! 

11、get_dummies()函数实现onehot编码 

这个函数是用来将列中的值转化为列名，简单点说就是可以做**onehot编码**。
```
week =pd.get_dummies(train_x['weekday']).add_prefix('weekday_')
#这样就将上面weekday中1~7的值变成了7列，列名分别为weekday_1~weekday_7
```

12、groupby（）函数 

和SQL中的一样，用来对数据进行分组，其实可以发现，pandas很多操作模仿了SQL来对数据进行处理，熟悉数据库的可以很容易上手。

```
df = df.groupby(['id','name'])[‘score’].mean()
#这句表示将t按照列id和name进行分组，将分组后的每个组根据score求每个#组score的平均值。
```

13、drop_duplicates()函数去重

```php
#用于去除重复的行
Df.drop_duplicates(inplace=True)
```

14、修改列的数据类型

```
df[‘score’]=df[‘score’].astype(‘str’)
 #表示将原来的数据类型改成了字符串类型
```

15、unstack()函数 转换行值为列名 

经过对两列groupby后，会产生两个索引， 

groupby([‘id’,’name’]),或者自己加的多重索引。 

在Dataframe中存在多重索引，如果只是要重新定义索引，可以直接用reset_index()或者将某列定义为setindex(‘id’) 

使用unstack函数可以直接将索引变成列
```
index = pd.MultiIndex.from_tuples([('one', 'a'), ('one', 'b'),...                                    ('two', 'a'), ('two', 'b')])
s = pd.Series(np.arange(1.0, 5.0), index=index)
#得到：
one  a   1.0
     b   2.0
two  a   3.0
     b   4.0
dtype: float64
```

```
s.unstack(level=-1)
     a   b
one  1.0  2.0
two  3.0  4.0
s.unstack(level=0)
   one  two
a  1.0   3.0
b  2.0   4.0
```

16、Dataframe把索引变成一列
`df['newcol']=df.index.values`
我在pandas中最常用的就是使用groupby分组，分组后直接使用统计方法求均值啊什么的，所以一定要学会将以上方法组合使用，基本可以解决大部分数据处理的功能。而且代码写出来简洁美观。 
**注意：**

还有一定要注意，groupby后的索引index是不变的，为了方便后续的处理，以及防止出错，最后对处理后的数据表进行reset_index()，或者setindex来指定一列作为索引，新手建议不要使用多重索引，怕弄乱了。 

mege和concat一定要注意合并的是行还是列。 

添加新的列的时候可以新建一个Dataframe，来生成新的列，最后将两个列用merge或者concat组合起来。 

综上所述：pandas用好了，根本不需要什么for循环。 

后续我会继续练习pandas，把新学的东西继续加进来。。。
未完。。。待续。。。。








