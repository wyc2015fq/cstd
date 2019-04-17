# 可视化之seaborn简单实现 - miner_zhu的博客 - CSDN博客





2018年08月24日 12:11:19[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：163








**Matplotlib**是Python主要的绘图库。但是，不建议直接使用它。虽然Matplotlib很强大，但它因此也很复杂，你的图经过大量的调整才能变得精致。因此，作为替代，推荐一开始使用Seaborn。Seaborn本质上使用Matplotlib作为核心库（就像Pandas对NumPy一样）。seaborn有以下几个优点：
- 默认情况下就能创建赏心悦目的图表。
- 创建具有统计意义的图。
- 能理解pandas的DataFrame类型，所以它们一起可以很好地工作。

```python
import matplotlib
import matplotlib.pyplot as plt
import seaborn as sns
#饼图
plt.pie(y_values, explode=None, labels=x_values,  
    colors=('b', 'g', 'r', 'c', 'm', 'y', 'k', 'w'),  
    autopct=None, pctdistance=0.6, shadow=False,  
    labeldistance=1.1, startangle=None, radius=None,  
    counterclock=True, wedgeprops=None, textprops=None,  
    center = (0, 0), frame = False )  
plt.show()

#散点图
plt.scatter(x_values, y_values,c = 'blue', edgecolor='none', s=80)
#x_values.get_xaxis().get_major_formatter().set_useOffset(False)
plt.title('picture', fontsize=24)
plt.xlabel('time', fontsize=14)
plt.ylabel('value', fontsize=14)
# 设置刻度标记的大小
#plt.tick_params(axis='both', which='major', labelsize=10)
# 设置每个坐标轴的取值范围
#plt.axis([0, 1100, 0, 1100000])
#sns.jointplot(x_values, y_values)#联合分布图
plt.show()
#plt.savefig.('散点图.png',bbox_inches='tight')

#barplot图
#orient='h'表示是水平展示的，alpha表示颜色的深浅程度
sns.barplot(y=x_values, x=y_values,orient='h', alpha=0.8, color='red')
#设置y轴、X轴的坐标名字与字体大小
plt.ylabel('time', fontsize=16)
plt.xlabel('value', fontsize=16)
#设置X轴的各列下标字体是水平的
plt.xticks(rotation='horizontal')
#设置Y轴下标的字体大小
plt.yticks(fontsize=15)
plt.show()
```





