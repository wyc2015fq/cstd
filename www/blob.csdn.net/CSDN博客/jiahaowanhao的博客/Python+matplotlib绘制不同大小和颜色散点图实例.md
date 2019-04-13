
# Python+matplotlib绘制不同大小和颜色散点图实例 - jiahaowanhao的博客 - CSDN博客


2018年04月06日 21:08:51[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：7369标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[Python+matplotlib绘制不同大小和颜色散点图实例](http://cda.pinggu.org/view/25195.html)
这篇文章主要介绍了Python+matplotlib绘制不同大小和颜色散点图实例，matplotlib的用法是比较多种多样的，下面一起看看其中的一个实例吧

具有不同标记颜色和大小的散点图演示。
演示结果：
![](http://files.jb51.net/file_images/article/201801/2018119164847066.png?2018019164856)
实现代码：
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cbook as cbook
\# Load a numpy record array from yahoo csv data with fields date, open, close,
\# volume, adj_close from the mpl-data/example directory. The record array
\# stores the date as an np.datetime64 with a day unit ('D') in the date column.
with cbook.get_sample_data('goog.npz') as datafile:
price_data = np.load(datafile)['price_data'].view(np.recarray)
price_data = price_data[-250:] \# get the most recent 250 trading days
delta1 = np.diff(price_data.adj_close) / price_data.adj_close[:-1]
\# Marker size in units of points^2
volume = (15 * price_data.volume[:-2] / price_data.volume[0])**2
close = 0.003 * price_data.close[:-2] / 0.003 * price_data.open[:-2]
fig, ax = plt.subplots()
ax.scatter(delta1[:-1], delta1[1:], c=close, s=volume, alpha=0.5)
ax.set_xlabel(r'$\Delta_i$', fontsize=15)
ax.set_ylabel(r'$\Delta_{i+1}$', fontsize=15)
ax.set_title('Volume and percent change')
ax.grid(True)
fig.tight_layout()
plt.show()
总结

以上就是本文关于Python+matplotlib绘制不同大小和颜色散点图实例的全部内容，希望对大家有所帮助。

