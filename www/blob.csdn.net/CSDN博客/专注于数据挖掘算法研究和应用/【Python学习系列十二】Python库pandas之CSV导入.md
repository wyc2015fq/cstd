# 【Python学习系列十二】Python库pandas之CSV导入 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月12日 21:03:07[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3552
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









Python Data Analysis Library 或 pandas 是基于NumPy 的一种工具，该工具是为了解决数据分析任务而创建的。Pandas 纳入了大量库和一些标准的数据模型，提供了高效地操作大型数据集所需的工具。pandas提供了大量能使我们快速便捷地处理数据的函数和方法。




场景：导入csv并关联处理，参考和深入：http://pandas.pydata.org/pandas-docs/stable/api.html#dataframe，掌握dataframe操作。

代码：



```python
# -*- coding: utf-8 -*-

import pandas as pd
#import numpy as np

dTxn = pd.read_csv(r"D:\train_txn_info.csv", header=0, encoding='utf-8')#交易表
dUse = pd.read_csv(r"D:\test.csv", header=0, encoding='utf-8') #用券表
train_dataset = pd.merge(dTxn, dUse, on= 'phone')#依据字段'phone'将dTxn和dUse两张表拼接在一起 
print  train_dataset.head(10)
#train_dataset.to_csv('D:\train_dataset.csv',index=False,sep='')#输出
```





