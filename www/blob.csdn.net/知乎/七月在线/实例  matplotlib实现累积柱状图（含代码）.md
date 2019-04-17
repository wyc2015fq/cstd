# 实例 | matplotlib实现累积柱状图（含代码） - 知乎
# 



**需求：**

分析flights数据集中客流量在一年中各月份的分布，使用柱状图分析。 

flights数据集有三列year(年)month(月)passengers(客流量)



**分析：**

该图最好能看见月度间的客流量的变化，也能看见按照年度变化的分布情况。
可以选的展示图有分组柱状图、累积柱状图。



累积柱状图如下图：

![](https://pic1.zhimg.com/v2-b5dc44888607f4db3b7e3298442f58b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='456'></svg>)
每年6月的客流量最多，2月次之；客流量整体呈逐年增长趋势，其中从1957年到1960年6月和2月的客流量增长量基本持平。

**技巧：**

matplotlib中的累积柱状图的关键函数是plt.bar，关键参数是bottom。官档（[https://matplotlib.org/api/_as_gen/matplotlib.pyplot.bar.html#matplotlib.pyplot.bar](https://link.zhihu.com/?target=https%3A//matplotlib.org/api/_as_gen/matplotlib.pyplot.bar.html%23matplotlib.pyplot.bar)）解释为：

bottom : scalar or array-like, optional，the y coordinate(s) of the bars default: None



经过助教的指点，该参数需要录入从第1年到（当前年－1）累积值。所以，我们在做数据处理的时候，需要增加一列处理客流量的累积值。

实现代码如下：

`import matplotlib as mpl`

from matplotlib import pyplot as plt

import seaborn as sns

import numpy as np

import pandas as pd

by Alicia

数据处理：

增加passengers_cumsum列：按月分组的每年的客流量的累积值
data = sns.load_dataset("flights")

tmp=data.groupby('month')
.apply(lambda df:df.sort_values('year').passengers
.shift(1).agg(np.cumsum)).reset_index()



tmp=tmp[['level_1','passengers']]

tmp.columns=['key','passengers_cumsum']




data = data.sort_values(['month','year']).reset_index()

data.columns=['key','year','month','passengers']

data = data.merge(tmp)

data.drop(data.columns[[0]], axis = 1,inplace=True)

绘图fig, ax = plt.subplots()

fig.set_size_inches(11.7, 8.27)




colors=['#99CCCC','#999933','#336699','#99CCFF','#CCFFFF',
'#6699CC','#CCCCFF','#99CCFF','#FFCC99','#FFFFCC',
'#99CCFF','#CCCC99']

months=data.month.unique()

years=data.year.unique()

w=0.1




for year,c in zip(years,colors):

   df=data.loc[data.year==year,['month','passengers',
    'passengers_cumsum']]

   plt.bar(df.month, df.passengers, w,bottom=df
    .passengers_cumsum,color=c,label=year)

# 设置y轴、标题、x轴的刻度标签值
ax.set_ylabel('passenger volume')

ax.set_title('distributions per month of the year')

ax.set_xticklabels(months)

# 设置图例在下方
box = ax.get_position()

ax.set_position([box.x0, box.y0 + box.height * 0.1,

                box.width, box.height * 0.9])




ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.05),

         fancybox=True, shadow=True, ncol=5)

plt.show()


