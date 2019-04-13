
# pandas 学习 ——Series - Zhang's Wikipedia - CSDN博客


2019年01月17日 22:14:24[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：85



## 1. Series 的获得
df.iloc 按行或者按列索引一行，或一列得到的就是一个 Series
pd.Series
from datetime import datetime
dates = [datetime(2011, 1, i) for i in [2, 5, 7, 8, 10, 12]]
s = pd.Series(np.random.randn(6), index=dates)
>> ts
2011-01-02   -1.157516
2011-01-05    0.755876
2011-01-07    0.299113
2011-01-08    0.446367
2011-01-10   -1.700069
2011-01-12    0.261344
dtype: float64
## 2. Series 的操作
成员函数：
idxmax：当前series中最大元素所在的索引
>> ts.idxmax()
Timestamp('2011-01-05 00:00:00')

