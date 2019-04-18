# Python含dict的list去重 - 专注于数据挖掘算法研究和应用 - CSDN博客
2019年04月18日 09:35:32[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6
个人分类：[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)
功能：list里面的每一个元素都是dict，根据dict某一个key进行去重
```python
from itertools import compress
import pandas as pd
from operator import itemgetter
from itertools import groupby
#功能：list里面的每一个元素都是dict，根据dict某一个key进行去重
#函数1
def distinct(items,key):
    key = itemgetter(key)
    items = sorted(items, key=key)
    return [next(v) for _, v in groupby(items, key=key)]
#函数2
#def distinct(items,key):  
#    mask = (~pd.Series(map(itemgetter(key), items)).duplicated()).tolist()
#    return list(compress(items, mask))
with open("/data/fjs/data/math/math.json",'r') as load_f:
    items = json.load(load_f)
    print (len(items))#去重前条目
    items =distinct(items,'quid')#去重key=quid
    print (len(items))#去重后条目
```
去重前条目：850000
去重后条目：50016
