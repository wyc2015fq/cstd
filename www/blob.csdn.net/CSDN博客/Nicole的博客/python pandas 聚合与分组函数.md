# python pandas 聚合与分组函数 - Nicole的博客 - CSDN博客
2018年05月10日 23:31:04[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：159
## 1 主要内容
- DataFrame.groupby().sum()
- DataFrame.groupby().agg()
- pandas.concat([DataFrame1,DataFrame2])
- pandas.merge(DataFrame1,DataFrame2,parameters….)
- DataFrame1.join(DataFrame2,lsuffix=’列名 on DataFrame1’,rsuffix=’列名 on DataFrame2’)
- 帮助文档的获取
## 2 实例
- 构造dataframe如下所示：
```python
food  food_id  number     price  user_id weather
0       soup        4       6  1.818250        3    cold
1       soup        8       6  1.834045        4     hot
2    iceream        8       7  3.042422        2    cold
3  chocolate        3       6  5.247564        4     hot
4    iceream        6       3  4.319450        4    cold
5    iceream        5       4  2.912291        1    cold
6    iceream        2       7  6.118529        2    cold
7       soup        8       4  1.394939        2     hot
8       soup        6       8  2.921446        2     hot
9  chocolate        2       1  3.663618        4     hot
```
实现程序如下所示：
```python
import pandas as pd
from numpy import random
from numpy.random import rand
import numpy as np
random.seed(42)
df = pd.DataFrame({'user_id':random.randint(0,6,10),'food_id':random.randint(1,10,10),
'weather':['cold','hot','cold','hot','cold','cold','cold','hot','hot','hot'],
'food':['soup','soup','iceream','chocolate','iceream','iceream','iceream','soup','soup','chocolate'],
'price':10 * rand(10),'number':random.randint(1,9,10)}) 
print df
```
2 groupby函数应用 
代码
```python
groupby1 = df.groupby(['user_id'])  #按照user_id分组输出所有的值
i = 0
for user_id,group in groupby1:
    i = i + 1
    print "group", i , user_id
    print group
```
结果
```python
group 1 1
      food  food_id  number     price  user_id weather
5  iceream        5       4  2.912291        1    cold
group 2 2
      food  food_id  number     price  user_id weather
2  iceream        8       7  3.042422        2    cold
6  iceream        2       7  6.118529        2    cold
7     soup        8       4  1.394939        2     hot
8     soup        6       8  2.921446        2     hot
group 3 3
   food  food_id  number    price  user_id weather
0  soup        4       6  1.81825        3    cold
group 4 4
        food  food_id  number     price  user_id weather
1       soup        8       6  1.834045        4     hot
3  chocolate        3       6  5.247564        4     hot
4    iceream        6       3  4.319450        4    cold
9  chocolate        2       1  3.663618        4     hot
```
3 groupby和sum等函数结合使用 
代码
```python
```
print(groupby1.sum())#对除了groupby索引以外的每个数值列进行求和 
print(groupby1['food_id','number'].sum()) #对除了groupby索引以外的特定数值列进行求和 
print(df.groupby(['user_id'],as_index=False).sum())#默认as_index=True,是否将user_id当做索引，默认是
#当然除了sum，还有mean，min，max，median，mode，std,mad等等，操作方法同理
#groupby()中的形参可用help(df.groupby)来查看
#常用的参数axis=0，表示对行进行操作，即指定列中不同值进行分组；axis=1，表示对列进行分组
``
```python
output[1]:
  food_id  number      price
user_id                            
1              5       4   2.912291
2             24      26  13.477336
3              4       6   1.818250
4             19      16  15.064678
output[2]:
         food_id  number
user_id                 
1              5       4
2             24      26
3              4       6
4             19      16
output[3]:
   user_id  food_id  number      price
0        1        5       4   2.912291
1        2       24      26  13.477336
2        3        4       6   1.818250
3        4       19      16  15.064678
```
```python
```
df.groupby(['food','weather']).size()
```python
food       weather
chocolate  hot        2
iceream    cold       4
soup       cold       1
           hot        3
dtype: int64
```
4 agg函数 
代码
`print df.groupby(['weather','food']).agg([np.mean,np.median])`
结果
```python
output[4]: 
                   food_id        number            price            \
```
```python
user_id         
                       mean median  
weather food                        
cold    iceream    2.250000      2  
        soup       3.000000      3  
hot     chocolate  4.000000      4  
        soup       2.666667      2
```
mean median mean median mean median weather food cold iceream 5.250000 5.5 5.25 5.5 4.098173 3.680936 soup 4.000000 4.0 6.00 6.0 1.818250 1.818250 hot chocolate 2.500000 2.5 3.50 3.5 4.455591 4.455591 soup 7.333333 8.0 6.00 6.0 2.050143 1.834045 
5 concat() 
代码
```python
print "df :3\n",df[:3]
print "df :4\n",df[6:]
print pd.concat([df[:3],df[6:]],axis=0)
```
结果
``
```python
df :3
      food  food_id  number     price  user_id weather
0     soup        4       6  1.818250        3    cold
1     soup        8       6  1.834045        4     hot
2  iceream        8       7  3.042422        2    cold
df :4
        food  food_id  number     price  user_id weather
6    iceream        2       7  6.118529        2    cold
7       soup        8       4  1.394939        2     hot
8       soup        6       8  2.921446        2     hot
9  chocolate        2       1  3.663618        4     hot
df.concat
        food  food_id  number     price  user_id weather
0       soup        4       6  1.818250        3    cold
1       soup        8       6  1.834045        4     hot
2    iceream        8       7  3.042422        2    cold
6    iceream        2       7  6.118529        2    cold
7       soup        8       4  1.394939        2     hot
8       soup        6       8  2.921446        2     hot
9  chocolate        2       1  3.663618        4     hot
```
6 merge()和join() 
代码
``
```python
df1=pd.DataFrame({'EmpNr':[5,3,9],'Dest':['The Hague','Amsterdam','Rotterdam']})
df2=pd.DataFrame({'EmpNr':[5,9,7],'Amount':[10,5,2.5]})
print "df1\n",df1
print "df2\n",df2
print "Merge() on Key\n",pd.merge(df1,df2,on='EmpNr')
print "inner join with Merge()\n",pd.merge(df1,df2,how='inner')
print "Dests join tips\n",df1.join(df2,lsuffix='Dest',rsuffix='Tips')
```
结果
``
```python
df1
        Dest  EmpNr
0  The Hague      5
1  Amsterdam      3
2  Rotterdam      9
df2
   Amount  EmpNr
0    10.0      5
1     5.0      9
2     2.5      7
Merge() on Key
        Dest  EmpNr  Amount
0  The Hague      5    10.0
1  Rotterdam      9     5.0
inner join with Merge()
        Dest  EmpNr  Amount
0  The Hague      5    10.0
1  Rotterdam      9     5.0
Dests join tips
        Dest  EmpNrDest  Amount  EmpNrTips
0  The Hague          5    10.0          5
1  Amsterdam          3     5.0          9
2  Rotterdam          9     2.5          7
```
6帮助文档获取方式
```
1.help(pd.concat)
2.dir(pd.concat)
3.pd.concat?
...
```
7 参考文献 
[利用python进行数据分析笔记](http://www.cnblogs.com/splended/p/5278078.html%20%E2%80%9C%E5%88%A9%E7%94%A8python%E8%BF%9B%E8%A1%8C%E6%95%B0%E6%8D%AE%E5%88%86%E6%9E%90%E2%80%9D)
python数据分析，Ivan Idris著
本文为转载文章，原文出处：https://blog.csdn.net/ly_ysys629/article/details/72553273
