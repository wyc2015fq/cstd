# 【数据平台】pandas按条件去重 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年12月01日 11:14:08[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1042
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









场景：按 A 列去重，保留 B 列中 最小的一行数据。






```python
# -*- coding: utf-8 -*-
'''
@author: user
'''
import pandas as pd

data = [[1,1,1,2,3,3], [0.2,0.3,0.4,0.1,0.2,0.4]]
data = pd.DataFrame(data)
data = [[1,1,1,2,3,3], [0.2,0.3,0.4,0.1,0.2,0.4]]
data = pd.DataFrame(data).T
data.columns=['A', 'B']
print (data)
#按 A 列去重，保留 B 列中 最小的一行数据。
#方法一：
print (data.groupby('A', as_index=False)['B'].min())
#方法二:
print(data.pivot_table(index='A', columns=None, values='B', aggfunc=min) )
#方法三：
data.sort_values(by=['B'],ascending=True)
print (data.drop_duplicates(['A']))
```





