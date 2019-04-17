# Pandas笔记|【机器学习集训营学员】（上） - 知乎
# 



**写在最前**

Pandas是机器学习工作流程中，对数据进行预处理的重要工具。本笔记是同学在学习七月在线机器学习训练营二期中的<pandas花式数据统计与分析技能>时的投稿笔记，分为上下两篇。本笔记旨在记录并整理寒老师讲授的知识点，分享以供二期同学们交流及学习。






![](https://pic1.zhimg.com/v2-2017c24bb09a477aa41b050bc6f6959c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='134'></svg>)



## **Pandas简介**

**1）Pandas是什么？**

Pandas是Panel data and data analysis的简写，是一个专门用来数据处理和数据分析的Python工具库。

## **2）特点**

基于numpy库

底层C实现，效率极高

对矩阵(高维数组)做计算

使用起来用在excel/R/SQL，快速入手

## **3）相关资源：**

pandas官方网站《10 Minutes to pandas》

（[http://pandas.pydata.org/pandas-docs/stable/10min.html](https://link.zhihu.com/?target=http%3A//pandas.pydata.org/pandas-docs/stable/10min.html)）

寒老师推荐的pandas速查表，（学员）群文件共享下载




**csv读写**

比较简单，略过，更多选项查看pd.read_csv和pd.to_csv的帮助，如何分隔符，跳过多少行，头数，指定数据类型等。




## **Pandas的两种数据类型：**

1）Series 

一个series是一个一维的数据类型，其中每一个元素都有一个标签。series类似于Numpy中元素带标签的数组，或者Python中的字典。其中，标签可以是数字或者字符串。

**理解为一个列**

> *#导入相应库*
*import**numpy as np*
*import pandas as pd*

## 1.1 从列表初始化series[默认数字index]

> *my_list=[7,'Beijing','19',3.1415,-10000,'Happy']*
*series1=pd.Series(my_list)*
*#从列表初始化Series*
*series1[1]#结果为'Beijing'*
*series1[1:4]#Series这种数据类型默认用0到N这种格式做为索引index*

b    Beijing

c        19

d     3.1415

dtype: object




## 什么是Pandas里的index呢？

可以理解为Python字典中的key,可以通过这个key访问对应的值。

那Series就可以想象成为一个KV对应的结构。







## 1.2 从列表初始化series，自己指定series的索引

my_list=[7,'Beijing','19',3.1415,-10000,'Happy']

series1=pd.Series(my_list, index=['a','b','c','d','e','f'])

#从列表初始化Series，并自定义index

a          7

b    Beijing

c        19

d     3.1415

e     -10000

f      Happy

dtype: object

*#使用自定义index访问Series*

*series1['a']*

7

*series1['f']*

'Happy'




即使是自定义索引来初始化Series了，依旧可以通过数字来访问。




*series1[1:4]*

b    Beijing

c        19

d     3.1415

dtype: object




*s1=pd.Series([1,2,3,4,5,6]*

*index=pd.date_range('20130101',periods=6))#带标签，标签是时间序列*

*s2=pd.Series(list('china! '))#不指定标签，系统自动是从0开始，*




## 1.3 从字典初始化series

因为字典这种数据结构本身就是kv对应型的，因为可以直接用来构建series。index自动为字典的key

*cities={'Beijing':55000,'Shanghai':60000,'Shenzhen':50000,'Hangzhou':20000,'Guangzhou':45000,'Suzhou':None}*

*series3=pd.Series(cities)*

*series3*

Beijing      55000.0

Guangzhou    45000.0

Hangzhou     20000.0

Shanghai     60000.0

Shenzhen     50000.0

Suzhou           NaN

dtype: float64

## 1.4 从numpy对象初始化series

*series4=pd.Series(np.random.randn(5),index=[list('abcde')])*

*series4*

a   -0.362932

b    1.670863

c   -0.665555

d    0.402865

e    1.789551

dtype: float64




## 2）DataFrame

一个DataFrame就是一张表格。Series可以理解为一维数组，DataFrame是一个二维数据，DataFrame是由多个Series组成的（可以理解为Series的集合）




2.1 从已有字典构建

*population={'city':['Beijing','Shanghai','Guangzhou','Shenzhen','Hangzhou','Chongqi'],'year':[2016,2017,2016,2017,2016,2016],*

*'population':[2100,2300,1000,700,500,500]*

*}*

*df1=pd.DataFrame(population)*

*df1*

       city  population  year

0    Beijing        2100  2016

1   Shanghai        2300  2017

2  Guangzhou        1000  2016

3   Shenzhen         700  2017

4   Hangzhou         500  2016

5    Chongqi         500  2016



![](https://pic1.zhimg.com/v2-bad7416efbffa55dfb57effe0fc79d28_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='459' height='226'></svg>)






## 2.2 从list的字典中构建

*l1=[{'JackMa':100000,'Han':80000,'David':10000},{'JackMa':9999999,'Han':90000}]*

*df3=pd.DataFrame(l1)*

*df3*

    David    Han   JackMa

0  10000.0  80000   100000

1      NaN  90000  9999999

## 2.3 从已有Series构建

*s1=pd.Series(list('china'))*

*s2=pd.Series(list('powers'))*

*df2=pd.DataFrame({'s1':s1,'s2':s2})*

*df2#两个Series按index对齐（这里的index为默认的0至N），如果对应的index没有，则填为NaN*

   s1 s2

0    c  p

1    h  o

2    i  w

3    n  e

4    a  r

5  NaN  s




## **Index索引**

理解index就是对应 每个行value的一个key。这样去理解简单些。 

## 1）默认类型

*population={'city':['Beijing','Shanghai','Guangzhou','Shenzhen','Hangzhou','Chongqi'],'year':[2016,2017,2016,2017,2016,2016],*

*'population':[2100,2300,1000,700,500,500]*

*}*




*df4.index#查看索引*

RangeIndex(start=0, stop=6, step=1)

df默认索引均为从0开始，步长为1，总长为总行数减1的序列。

## 2）自定义索引并查看类型

*pd.Index(list('china'))*

Index(['c', 'h', 'i', 'n', 'a'], dtype='object')

*type(pd.Index(list('china')))*

<class 'pandas.core.indexes.base.Index'>

## 3）使用自定义索引

*df4.index=list('china!')*

*df4#可以看到这时的索引已经变成指定的字母了*

       city  population    year

c    Beijing        2100  2016.0

h   Shanghai        2300     NaN

i  Guangzhou        1000  2016.0

n   Shenzhen         700  2017.0

a   Hangzhou         500  2016.0

!    Chongqi         500  2016.0

*df4.ix[['n','a'],[0,1]]#正常使用并访问*

      city  population

n  Shenzhen         700

a  Hangzhou         500

## 4）多层index

*series6=pd.Series(np.random.randn(10),index=[['a','a','a','b','b','c','c','d','d','d'],[1,2,3,1,2,1,2,1,2,3]])*

*series6.index*

MultiIndex(levels=[['a', 'b', 'c', 'd'], [1, 2, 3]],

          labels=[[0, 0, 0, 1, 1, 2, 2, 3, 3, 3], [0, 1, 2, 0, 1, 0, 1, 0, 1, 2]])




多层index的访问

*series6['b':'c']*

b  1    0.198059

  2    0.418935

c  1   -0.146183

  2    0.134893

dtype: float64

*series6[:2]*

*a  1    0.898547*

*  2    0.132076*

*dtype: float64*

## 5）unstack

*series6*

a  1    0.898547

  2    0.132076

  3   -0.911465

b  1    0.198059

  2    0.418935

c  1   -0.146183

  2    0.134893

d  1    0.925714

  2    0.657029

  3   -0.237139

dtype: float64

*series6.unstack()*

         1         2         3

a  0.898547  0.132076 -0.911465

b  0.198059  0.418935       NaN

c -0.146183  0.134893       NaN

d  0.925714  0.657029 -0.237139




*type(series6.unstack())#unstack一个Series后，变为DataFrame了。*

<class 'pandas.core.frame.DataFrame'>




寒老师：千万不要因为pandas功能强大就给自己挖坑，开始的时候就老老实实地用二维表格，或者把多维表格降为二维。







## **Series和DataFrame的操作**

可以把series理解为一列，dataframe可以理解为由很多个列（series）构成的列。 

我们可以像对待一个list一样series，完成各类操作




*cities={'Beijing':55000,'Shanghai':60000,'Shenzhen':50000,'Hangzhou':20000,'Guangzhou':45000,'Suzhou':None }*

*series5=pd.Series(cities,name='incomes')*

## 1）DataFrame描述性命令

*info()##了解dataframe整体结构，列，数据类型*

*describe()#了解数据分布*

*head()*

*tail()*

## 2）访问

## 2.1 Series切片访问

*series5[:5]*

Beijing      55000.0

Guangzhou    45000.0

Hangzhou     20000.0

Shanghai     60000.0

Shenzhen     50000.0

Name: incomes, dtype: object

## 2.2 指定索引访问，注意这里位置可以任意调换

*series5[[0,4,3,1,2]]*

Beijing      55000.0

Shenzhen     50000.0

Shanghai     60000.0

Guangzhou    45000.0

Hangzhou     20000.0

Name: incomes, dtype: object

*series5[['Shanghai','Suzhou']]*

*Shanghai    60000.0*

*Suzhou          NaN*

*dtype: float64*

## 3）元素检查

## 3.1 in关键字，类似于检查元素是否在列表中一样的操作

*'Suzhou' in series5*

True

*'Nanjing' in series5*

False

## 3.2 Get方法访问，如果指定的index不存在，则返回nan

*series5.get('Suzhou')*

nan

*series5.get('Beijing')*

55000.0




## 4）条件boolean indexing

*conditions=(series5==60000)*

*conditions*

Beijing      False

Guangzhou    False

Hangzhou     False

Shanghai      True

Shenzhen     False

Suzhou       False

Name: incomes, dtype: object

按条件进行对series过滤后，会得到一个boolean型变量组成的Series对象,叫做**boolean indexing**。这个series可以传递作为条件，过滤掉所有相应位置为False的记录，得到最终结果。

*series5[conditions]*

Shanghai    60000.0

Name: incomes, dtype: object

## 4.1 非空与非空值过滤

**找出所有的空值****或****非空值****的****boolean indexing#bool条件**

*series5.notnull()#或-series5.isnull()*

Beijing       True

Guangzhou     True

Hangzhou      True

Shanghai      True

Shenzhen      True

Suzhou       False

Name: incomes, dtype: bool

*series5.isnull()#或-series5.notnull()*

Beijing      False

Guangzhou    False

Hangzhou     False

Shanghai     False

Shenzhen     False

Suzhou        True




## 5）简单数学运算

## 5.1 求均值，中位数，最大最小，

*series5.mean()*

46000.0

*series5.median()*

50000.0

*series5[series5.isnull]=series5.mean()#把空项填充为均值*

## 6）元素赋值

## 6.1 指定Series的index赋值

*series5['Shenzhen']=70000*

*series5['Shenzhen']*

70000




## 6.2 按boolean indexing条件过滤后赋值

*series5[series5<50000]=50001*

*series5*

Beijing      55000.0

Guangzhou    50001.0

Hangzhou     50001.0

Shanghai     60000.0

Shenzhen     50000.0

Suzhou           NaN

Name: incomes, dtype: float64

## 7）数学运算

*series5**0.5#算开方，列表可不支持这样的操作，这也是pandas强大的地方*

Beijing      234.520788

Guangzhou    223.609034

Hangzhou     223.609034

Shanghai     244.948974

Shenzhen     223.606798

Suzhou              NaN

Name: incomes, dtype: float64

*np.log(series5)*

Beijing      10.915088

Guangzhou    10.819798

Hangzhou     10.819798

Shanghai     11.002100

Shenzhen     10.819778

Suzhou             NaN

Name: incomes, dtype: float64

## 7.1 运算->自动indexing对齐

Pandas有个强大的功能，叫做对齐。我们新建两个series,列的顺序是不是对齐的，而且两个series中都有另一个series中没有项，但依旧可以进行运算，结果为NaN




*s1=pd.Series({'Beijing':55000,'Shanghai':60000,'Hangzhou':20000})*

*s2=pd.Series({'Shenzhen':50000,'Beijing':55000,'Shanghai':60000})*

*s1*

Beijing     55000

Hangzhou    20000

Shanghai    60000

dtype: int64

*s2*

Beijing     55000

Shanghai    60000

Shenzhen    50000

dtype: int64

*s1+s2*

Beijing     110000.0

Hangzhou         NaN

Shanghai    120000.0

Shenzhen         NaN

dtype: float64

**总结：pandas自动对齐两个series的index，进行运算。**




## **更多访问方式**




1）loc 

按indx取出Series,通过行标签索引行数据,有标签时可以通过标签没标签时可以通过行号。

*df4.loc[0:2]*

city  population  year  bonus   average

0    Beijing        2100  2016   6666  3.174286

1   Shanghai        2300  2017   6666  2.898261

2  Guangzhou        1000  2016   6666  6.666000

*df4.loc[[0,3],['city','average']]#前边[0,3]代表选行，[‘city’,’average’]代表选列*

      city   average

0   Beijing  3.174286

3  Shenzhen  9.522857

*df4.loc[df4.average>10,['city','average']]#前边代表按列条件去过滤后剩下的行，['city', 'average']代表选列*

## 2）iloc

只能通过数字索引获来取数据

*df4.iloc[1:,1:]*

  population    year

1        2300     NaN

2        1000  2016.0

3         700  2017.0

4         500  2016.0

5         500  2016.0

## 3）Ix

混合索引，可以和标签，索引，行号混用。

*df4.ix[:4,['city','population']]*

       city  population

0    Beijing        2100

1   Shanghai        2300

2  Guangzhou        1000

3   Shenzhen         700

4   Hangzhou         500

## 4）条件组合与判断访问

## 4.1 条件组合

*(df4.population==2100)|(df4.population==1000)*

0     True

1    False

2     True

3    False

4    False

5    False

*df4.loc[((df4.population==2100)|(df4.population==1000)),:]*

       city  population  year

0    Beijing        2100  2016

2  Guangzhou        1000  2016




## 4.2 指定条件

*df4.population.isin([2100,1000])*

0     True

1    False

2     True

3    False

4    False

5    False

可以看到，使用isin函数得到的是一个boolean indexing




Pandas里的条件否定，可以简单前边加一个负号即可。

*-df4.population.isin([2100,1000])*

0    False

1     True

2    False

3     True

4     True

5     True

Name: population, dtype: bool




*df4.loc[df4.population.isin([2100,1000]),:]#isin的传入参数可以是一个列表,ndarray,Series*

       city  population  year

0    Beijing        2100  2016

2  Guangzhou        1000  2016




## 5）缺省值与缺少值填充

缺少值的类型是numpy中的nan

*df4.iloc[1,2]=np.nan*

       city  population    year

0    Beijing        2100  2016.0

1   Shanghai        2300     NaN

2  Guangzhou        1000  2016.0

3   Shenzhen         700  2017.0

4   Hangzhou         500  2016.0

5    Chongqi         500  2016.0




*df4.fillna(2017)#注意此时是返回一个修过的df，而原来的df不会被修改，如果要修改原来的df,指定inplace=True*

       city  population    year

0    Beijing        2100  2016.0

1   Shanghai        2300  2017.0

2  Guangzhou        1000  2016.0

3   Shenzhen         700  2017.0

4   Hangzhou         500  2016.0

5    Chongqi         500  2016.0




**高级填充**

如果是像天气，股票价格的取样周期很短，如每半分钟一次的的数据，缺少值就适合用这种方法填充。但同样的场景，每半小时取样，就不适用这种方法了。先分析场景很重要。

*df4.fillna(method='ffill') #按空值前一值填充*

*df4.fillna(method='bfill') #按空值后一值填充*







## **广播特性Broadcasting**

*df4=pd.DataFrame(population)*

*df4['bonus']=6666*

*df4#这里的6666为标量，要应用到整个series，，就会使用到广播*

*df4*

       city  population  year  bonus

0    Beijing        2100  2016   6666

1   Shanghai        2300  2017   6666

2  Guangzhou        1000  2016   6666

3   Shenzhen         700  2017   6666

4   Hangzhou         500  2016   6666

5    Chongqi         500  2016   6666




**并行进行的广播，非循环**

*df4['average']=df4['bonus']/df4['population']*

*df4*

       city  population  year  bonus    average

0    Beijing        2100  2016   6666   3.174286

1   Shanghai        2300  2017   6666   2.898261

2  Guangzhou        1000  2016   6666   6.666000

3   Shenzhen         700  2017   6666   9.522857

4   Hangzhou         500  2016   6666  13.332000

5    Chongqi         500  2016   6666  13.332000







## **简单绘画**

*from matplotlib import pyplot as plt*

*df4.population.plot(kind='bar')*

*<matplotlib.axes._subplots.AxesSubplot object at 0x0000012F67F09E80>*

*plt.show()*



![](https://pic4.zhimg.com/v2-af139fff013148c668a4352bdeee7ba7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='433' height='391'></svg>)






**写在最后**

本篇笔记偏Pandas基础入门及介绍，课后留的作业要远比笔记涉及的多，要难。在第二篇笔记中将记录这些Pandas高级应用，将在公众号上整理发布，同学们敬请期待。


