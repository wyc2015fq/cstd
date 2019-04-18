# pandas 高级三（数据变形-分组与聚合） - wsp_1138886114的博客 - CSDN博客





2018年06月25日 17:39:04[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：352









**目录**



- - - - - [1 直接根据 key 进行分组](#1-直接根据-key-进行分组)
- [2 通过字典 /Series 分组](#2-通过字典-series-分组)
- [3 自定义函数，先groupby分组，再agg()调用聚合函数](#3-自定义函数先groupby分组再agg调用聚合函数)
- [4 将分组的列作为索引](#4-将分组的列作为索引)
- [5 导入文件，先分组再使用聚合函数](#5-导入文件先分组再使用聚合函数)
- [6 DataFrame 加权平均数](#6-dataframe-加权平均数)







以下示例均导入以下包

```python
import pandas as pd
from numpy import nan as Na
import numpy as np
from pandas import DataFrame,Series
import sys
```

```
agg(func)
agg实现了apply+combine
    func取内置聚合函数（如max，min）
    func取自定义函数
    func取函数列表
    func取key为列名、value为函数的dict
结果
    行索引为groupby 的by值
    列名：
        当func为一个函数时，列名为原始列名
        当func有多个函数时，多层索引（外层为原始列名，内层为函数列表）


    func取自定义函数：
        默认传入参数为分组后的dataframe对象的一列，一列列处理。


    groupby函数
        groupby(by=None)
        groupby实现了split过程
        df.groupby("key1").mean()   #指定列分组，Na值行丢掉
```

##### 1 直接根据 key 进行分组

![这里写图片描述](https://img-blog.csdn.net/20180625155230302?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 2 通过字典 /Series 分组

![这里写图片描述](https://img-blog.csdn.net/20180625155645781?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 3 自定义函数，先groupby分组，再agg()调用聚合函数

![这里写图片描述](https://img-blog.csdn.net/201806251703168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 4 将分组的列作为索引

![这里写图片描述](https://img-blog.csdn.net/20180625172643484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 5 导入文件，先分组再使用聚合函数

![这里写图片描述](https://img-blog.csdn.net/20180625172654386?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 6 DataFrame 加权平均数

![这里写图片描述](https://img-blog.csdn.net/20180625172707463?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



