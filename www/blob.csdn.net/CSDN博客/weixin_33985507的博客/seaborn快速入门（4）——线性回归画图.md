# seaborn快速入门（4）——线性回归画图 - weixin_33985507的博客 - CSDN博客
2018年09月10日 19:06:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
## 1. 准备
```
%matplotlib inline
import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
sns.set(color_codes=True)
np.random.seed(sum(map(ord,"regression")))
tips = sns.load_dataset("tips")
tips.head()
```
## 2. 绘制线性回归
```
%matplotlib inline
import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
sns.set(color_codes=True)
np.random.seed(sum(map(ord,"regression")))
tips = sns.load_dataset("tips")
# x为总花费，y为小费，data为数据集
sns.regplot(x="total_bill",y="tip",data=tips)
```
![13764292-6a3386c32c3c407a.png](https://upload-images.jianshu.io/upload_images/13764292-6a3386c32c3c407a.png)
> 
也可以用impplot绘制。这里不进行深入讲述
## 3. 抖动
有时，数据是分类的，而不是散点分布的：
```
sns.regplot(x="size",y="tip",data=tips)
```
![13764292-43f119534d519a1c.png](https://upload-images.jianshu.io/upload_images/13764292-43f119534d519a1c.png)
当数据不是散点分布时，可以为其添加抖动，再进行回归分析：
```
sns.regplot(x="size",y="tip",x_jitter=.6,data=tips)
```
![13764292-77c71e4fc91cc424.png](https://upload-images.jianshu.io/upload_images/13764292-77c71e4fc91cc424.png)
