# Pandas笔记|【机器学习集训营学员】（下） - 知乎
# 



**写在最前**

Pandas是机器学习工作流程中，对数据进行预处理的重要工具。本笔记是同学在学习七月在线机器学习训练营二期中的<pandas花式数据统计与分析技能>时的投稿笔记，分为上下两篇。本笔记旨在记录并整理寒老师讲授的知识点，分享以供二期同学们交流及学习。

## **Pandas高级应用**

## **Group**

1）数据准备

#导入相关库

*import pandas as pd*

*import numpy as np*

#建立实验数据

*salaries = pd.DataFrame({*

*   'name':['BOSS', 'Lilei', 'Lilei', 'Han', 'BOSS', 'BOSS', 'Han', 'BOSS'],*

*   'Year':[2016,2016,2016,2016,2017,2017,2017,2017],*

*   'Salary':[999999,20000,25000,3000,9999999,999999,3500,999999],*

*   'Bonus':[100000,20000,20000,5000,200000,300000,3000,400000]*

*})*
![](https://pic3.zhimg.com/v2-7f6dc57037325957fbd93130e8938956_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='479' height='273'></svg>)



#简单描述,看到索引，列，数据类型等DF结构，以及是否有空值等

*[http://salaries.info()](https://link.zhihu.com/?target=http%3A//salaries.info%28%29)*

<class 'pandas.core.frame.DataFrame'>

RangeIndex: 8 entries, 0 to 7

Data columns (total 4 columns):

Bonus     8 non-null int64

Salary    8 non-null int64

Year      8 non-null int64

name      8 non-null object

dtypes: int64(3), object(1)

memory usage: 336.0+ bytes

#主要对数据进行计数，看数据列的均值，离散程度，最大最小，以4分位数

*salaries.describe()*



![](https://pic2.zhimg.com/v2-12b7cf2b882438b4c572075a234a95b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='450' height='284'></svg>)



*salaries = salaries[['name','Year', 'Salary','Bonus']]#重排列*

## 2）按照单列--名字列来做个分组

*group_by_name = salaries.groupby('name')*

*type(group_by_name)*

pandas.core.**groupby**.**DataFrameGroupBy**

## 3）分组后运算#如sum, size，mean，median等

*group_by_name.sum()*



![](https://pic1.zhimg.com/v2-480de6e222f6d485044b025f2fe645bc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='338' height='207'></svg>)



*group_by_name[['Salary','Bonus']].sum()*



![](https://pic2.zhimg.com/v2-3c960000f3e4d6f4e8abfa69d9bca605_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='338' height='222'></svg>)
## 4）agg-并行执行多种运算

*group_by_name[['Salary','Bonus']].agg(['sum','mean','std','var'])*






![](https://pic3.zhimg.com/v2-21721aaf942e05340a724b0e842c33b6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='624' height='145'></svg>)



5）Group对象的构成




*group_by_name.groups*

{'BOSS': Int64Index([0, 4, 5, 7], dtype='int64'),

'Han': Int64Index([3, 6], dtype='int64'),

'Lilei': Int64Index([1, 2], dtype='int64')}

*print(len(group_by_name.groups))*

3

## 6）按多列进行分组

*group_by_name_year = salaries.groupby(['name','Year'])*

group_by_name_year.sum()






![](https://pic2.zhimg.com/v2-fcaf44cf54fee97404a28292628ad951_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='351' height='261'></svg>)
## 7）用迭代的方法查看分组详情

*for name, group in group_by_name:*

*   print(name)*

*   print(group)*

BOSS

  name  Year   Salary   Bonus

0  BOSS  2016   999999  100000

4  BOSS  2017  9999999  200000

5  BOSS  2017   999999  300000

7  BOSS  2017   999999  400000

Han

 name  Year  Salary  Bonus

3  Han  2016    3000   5000

6  Han  2017    3500   3000

Lilei

   name  Year  Salary  Bonus

1  Lilei  2016   20000  20000

2  Lilei  2016   25000  20000




## 8）选择不同的Group




*group_by_name.get_group("Lilei")*

*group_by_name.get_group("BOSS")*




## 9）同时在分组里做几个统agg计操作




*group_by_name[['Bonus','Salary']].agg(['sum','mean','std'])*



![](https://pic4.zhimg.com/v2-a665be1bda47ebff162bc55f0669ec0b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='616' height='200'></svg>)



使用numpy下的运算函数

*group_by_name[['Bonus','Salary']].agg([np.sum, np.mean, np.std]*



![](https://pic4.zhimg.com/v2-dace5c391275ab33421fa83710dc6f3b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='627' height='202'></svg>)



## **Transform**

1）数据准备

*#使用Nvidia的股价数据*

*nvda = pd.read_csv('data/NVDA.csv', index_col=0, parse_dates=['Date'])*

*[http://nvda.info()](https://link.zhihu.com/?target=http%3A//nvda.info%28%29)*
![](https://pic1.zhimg.com/v2-9aa8092b96b1401481e9b28d7f86bcb4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='432' height='190'></svg>)



*nvda.describe()*
![](https://pic2.zhimg.com/v2-335fcef6c649b57e908fed66688a7171_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='611' height='258'></svg>)



*nvda.head()*
![](https://pic1.zhimg.com/v2-3bf93b51c70ff0dcd617d536ce2e7fec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='546' height='212'></svg>)



## 2）根据已有列构造新列

## *nvda['year'] = nvda.index.year*

*nvda.head()*
![](https://pic3.zhimg.com/v2-f2be3d25300fd1b286834f1dc50c5eb6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='606' height='174'></svg>)



3）删除列

*#注意inplace参数为True，代表的是原地删除。*

*nvda.drop(['year'], axis=1, inplace=True)*

4）索引列进行处理后排序

*nvda.index.year#返回日期型索引的年*

*nvda.index.month#返回日期型索引的月*

*nvda.index.day#返回日期型索引的日*

#如果我们想按年来先进行group，再进行统计，可以这样来进行

*key = lambda x : x.year*

*nvda.groupby(key).agg(['mean','std']).head()*
![](https://pic3.zhimg.com/v2-a4bbc383b4d6f5c64e13403578072532_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='605' height='115'></svg>)



5）Transform变换

变换函数transform针对的是列:

*transform=nvda.groupby(key).transform(lambda x : (x-x.mean())/x.std()).head()*

*transform *
![](https://pic1.zhimg.com/v2-fc10dc8d69880e87a76f2d8ee3c0de64_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='603' height='205'></svg>)



6）#按月取得当月内价格波动范围




*price_range = lambda x : x.max()-x.min()*

*nvda.groupby(lambda x:x.month).transform(price_range).head()*
![](https://pic1.zhimg.com/v2-6fbf5508da0aa09735fa0d82f6254940_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='559' height='335'></svg>)



7）尽量把pandas的运算变成列与列之间的运算




#使用**shift函数**对列数据进行上下平移后产生新的列，再与原列进行运算。

*salaries['Salary_shift'] = salaries['Salary'].shift(1)*

*salaries['diff'] = salaries['Salary'] - salaries['Salary_shift']*

*salaries*



![](https://pic2.zhimg.com/v2-bcd802ea4b8ae070c024126ce5fb36c5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='546' height='327'></svg>)



## 8）简单绘画

## *%matplotlib inline*

*my_compare = pd.DataFrame({'Origin Adj Close':nvda['Adj Close'],*

*                         'Transformed Adj Close':transform["Adj Close"]})*

*my_compare.head()*

*my_compare.plot()*
![](https://pic1.zhimg.com/v2-72db085dd147a4db0c4b5e764a5b60d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='493' height='314'></svg>)



*transform["Adj Close"].plot(grid=True)*
![](https://pic1.zhimg.com/v2-291ce028ea8bd6b43650a7f0a64bfa68_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='464' height='300'></svg>)
## **Transform和apply**




apply

对每列指定操作

*nvda.groupby(key).apply(lambda x : x).head()*
![](https://pic4.zhimg.com/v2-9a6702b6ca56ebeb33926ef6060c9bb3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='593' height='231'></svg>)



## **拼接concat**

1）数据准备

*df1 = pd.DataFrame({'apts':[55000, 60000, 80000],*

*                  'cars':[200000, 300000, 400000]})*

*df2 = pd.DataFrame({'apts':[35000, 30000, 50000],*

*                  'cars':[210000,310000,410000]})*

*df3 = pd.DataFrame({'apts':[34000, 32000, 70000],*

                  'cars':[240000,350000,440000]})

2）纵向拼接




*pd.concat([df1, df2, df3])*



![](https://pic2.zhimg.com/v2-18a49baa312474099140bb2d360eb15d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='334' height='234'></svg>)
append一样可以完成

*df1.append(df2)*

df1.append([df1,df2])

3）横向拼接




*pd.concat([df1, df2, df3],axis=1)*



![](https://pic3.zhimg.com/v2-0bac659ab22ef100985f162d4b800596_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='579' height='186'></svg>)
**Merge(join)表关联**

1）数据准备




*df4 = pd.DataFrame({'apts':[55000, 60000, 80000],*

*                  'cars':[200000, 300000, 400000]},*

*                 index = ['Beijing','Shanghai','Nanjing'])*

*df5 = pd.DataFrame({'apts':[55000, 70000, 80000],*

*                  'cars':[100000, 330000, 260000]},*

*                 index = ['Beijing','Shanghai','Nanjing'])*

2）左连




*pd.merge(df4, df5, on=['apts'], how='left')*



![](https://pic3.zhimg.com/v2-d59d17f48eedec09ecc52897a850d3f2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='382' height='215'></svg>)
3）右连

*pd.merge(df4, df5, on=['apts'], how='right')*



![](https://pic4.zhimg.com/v2-831467167d55bc44ade90816b51c587f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='375' height='199'></svg>)



4）内连

*pd.merge(df4, df5, on=['ap*ts*'], how='inner')*



![](https://pic4.zhimg.com/v2-9f081c070b42f90a0a9a6119554370ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='382' height='163'></svg>)



5）外连

*pd.merge(df4, df5, on=['apts'], how='outer')*
![](https://pic3.zhimg.com/v2-085c85339b40d023b7ce95086db1ef4e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='382' height='220'></svg>)



**写在最后**

本篇笔记介绍pandas dataframe对象的高级应用，课后寒老师也留关于bike的一个工程。Pandas操作灵活，功能强大，对于有SQL基础的同学来讲有福气，但对于初次接触的同学的确需要一定的时间才能熟悉。Pandas在机器学习训练营二期中是第三周的课程的主题，是下一步进行机器学习的数据的前站。需要反复练习，认真做习题，来掌握Pandas这个’**内存表格处理**’库。


