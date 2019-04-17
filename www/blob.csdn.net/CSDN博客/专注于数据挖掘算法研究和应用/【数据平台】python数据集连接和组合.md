# 【数据平台】python数据集连接和组合 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月30日 11:16:39[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：373
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









场景：dataframe之间的链接，以及list之间的组合。

参考：



```python
# -*- coding: utf-8 -*-
'''
Created on 2017年9月30日

@author: Administrator
'''
import pandas as pd
import itertools
from itertools import product

#merge
df1=pd.DataFrame({'name':['z','q','s','l'],'subid':['1','2','',''],'cid':['c1','','c2','c3']})
print (df1)
df2=pd.DataFrame({'name':['z','q','s','s','g'],'cid':['c1','c2','c2','c3','c1'],'code':['1','2','3','3','2']}) 
print (df2)
#内连接
df_inner=pd.merge(df1,df2,on=['name','cid'],how='inner')    
print (df_inner)
#左连接
df_left=pd.merge(df1,df2,on=['name','cid'],how='left')    
print (df_left)
#右连接
df_right=pd.merge(df1,df2,on=['name','cid'],how='right')    
print (df_right)
#全外连接
df_outer=pd.merge(df1,df2,on=['name','cid'],how='outer')    
print (df_outer)

#join
df1=pd.DataFrame({'name':['z','q','s','l']})
print (df1)
df2=pd.DataFrame({'cid':['c1','c2','c3']}) 
print (df2)
df_join=df1.join(df2)
print (df_join)

#元素两两组合-笛卡尔乘积

list1=['z','q','s','l']
list2=['c1','c2','c3']
list3 = [(x,y) for x in list1 for y in list2]
print (list3)
for x in product(list1, list2):
    print (x)
for x in product(list1, repeat=4):
    print (x)
nums = itertools.permutations(list2)#自身组合
for x in  nums:
    print(x)
```





