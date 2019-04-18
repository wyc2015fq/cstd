# pandas 高级二（数据分组，筛选；文件保存） - wsp_1138886114的博客 - CSDN博客





2018年06月25日 13:04:21[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：574








以下示例均导入以下包

```python
import pandas as pd
from numpy import nan as Na
import numpy as np
from pandas import DataFrame,Series
import sys
```

### 一、数据分组 cut / qcut

> 
##### 1.1 pd.cut

```
pd.cut(arr,bins，right=True,labels=None,retbins=False,precision=3,include_lowest=False)
    bins:   序列表示分组边界；
            整数表示个数
    labels: 指定分组名称
    include_lowest: 是否保留分组边界最小的一个值，在bins取整数时无效
    right:          是否保留分组边界中最大的一个值
    presision:      指定分组边界要显示的小数点个数
    retbins：       是否在结果中显示分组边界信息

    right,include_lowest 不会同时为 false:（可以同时为 True）
    优先级：right > include_lowest.
```

![这里写图片描述](https://img-blog.csdn.net/20180622170818541?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 11.2 pd.qcut

![这里写图片描述](https://img-blog.csdn.net/20180625092713490?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018062509374196?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180625094430737?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 二、数据过滤与筛选

![这里写图片描述](https://img-blog.csdn.net/20180625113741302?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 三、pandas 文件读取/存储(IO)

> 
```
pd.read_csv(filepath_or_buffer="./train_data.txt")
将数据储存到csv:to_csv
```

![这里写图片描述](https://img-blog.csdn.net/201806072208245?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 3.2 文件读取

![这里写图片描述](https://img-blog.csdn.net/20180625123212590?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 3.3 文件保存

![这里写图片描述](https://img-blog.csdn.net/20180625123409152?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






