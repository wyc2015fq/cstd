# 【Python学习系列二十一】pandas库基本操作 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月03日 09:50:53[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1518
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









pandas很强大，操作参考官网：http://pandas.pydata.org/pandas-docs/stable/

也有一份10分钟入门的材料：http://pandas.pydata.org/pandas-docs/stable/10min.html  对于行列切片，开始基本都是看这份。




总结了些最基本的操作，如下：



```python
# -*- coding: utf-8 -*-

import numpy as np
import pandas as pd

#导入csv文件，空格分割
ds=pd.read_csv(r"lx_test_features_all.txt",sep='\t',encoding='utf8',\
                names=['u_spu_num','u_brand_num','u_cat_num','u_cat_spu','u_brand_spu',\
                       'u_spu_freq','u_spu_date','u_spu_click_freq','u_spu_click_date',\
                       'u_first_date','u_last_date','u_spu_ratio','u_ratio'])  
#查看索引、数据类型和内存信息
print ds.info()

#创建100行13列的随机数组成的DataFrame对象
ds_test=pd.DataFrame(np.random.rand(100,13))
print ds_test.shape
del ds_test #删除变量

#提取某几列数据
ds_col=ds[['u_spu_ratio','u_ratio']]
print ds_col.shape[1]
del ds_col

#行列定位操作
s=ds.iloc[0,:] #获取第一行
print s.iloc[0] #获取第一列
print ds.iloc[0,0]#获取第一行第一列
ds.set_index('u_spu_num')#设置索引列
print s.loc['u_spu_num']
del s

#数据清理
print ds.isnull()#检查DataFrame对象中的空值，并返回一个Boolean数组
print ds.isnull()#检查DataFrame对象中的非空值，并返回一个Boolean数组
ds.dropna()#删除所有包含空值的行
ds.dropna(axis=1)#删除所有包含空值的列
ds.dropna(axis=1,thresh=5)#删除所有小于5个非空值的行
ds.fillna('0')#用0替换DataFrame对象中所有的空值

#数据处理
ds_tmp=ds[ds['u_ratio'] > 0.5] #选择u_ratio列的值大于0.5的行
print ds_tmp.shape
ds_tmp.sort_values('u_ratio')#按照列u_ratio排序数据，默认升序排列
print ds_tmp
ds_tmp.sort_values('u_ratio', ascending=False)#按照列u_ratio降序排列数据
print ds_tmp
ds.groupby('u_spu_num').agg(np.mean)#返回按列col1分组的所有列的均值
ds_tmp.apply(np.mean)#对DataFrame中的每一列应用函数np.mean
ds_tmp.apply(np.max,axis=1)#对DataFrame中的每一行应用函数np.max
#创建一个按列u_ratio进行分组，并计算u_spu_num和u_spu_ratio的最大值的数据透视表
ds.pivot_table(index='u_ratio', values=['u_spu_num','u_spu_ratio'], aggfunc=max)

#数据合并
ds.append(ds_tmp)#将ds_tmp中的行添加到ds的尾部
ds.concat([ds, ds_tmp],axis=1)#将ds_tmp中的列添加到ds的尾部ds
ds.join(ds_tmp,on='u_spu_num',how='inner')#对ds的列和ds_tmp的列执行SQL形式的join

#数据统计
ds.describe()#查看数据值列的汇总统计
ds.mean()#返回所有列的均值
ds.corr()#返回列与列之间的相关系数
ds.count()#返回每一列中的非空值的个数
ds.max()#返回每一列的最大值
ds.min()#：返回每一列的最小值
ds.median()#返回每一列的中位数
ds.std()#返回每一列的标准差

del ds,ds_tmp
```





