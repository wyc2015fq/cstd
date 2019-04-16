# matplotlib作图添加表格 - littlely_ll的博客 - CSDN博客





2017年07月08日 21:21:50[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：5483








```
import matplotlib.pyplot as plt
import numpy as np

plt.figure()
ax = plt.gca()
y = np.random.randn(9)

col_labels = ['col1','col2','col3']
row_labels = ['row1','row2','row3']
table_vals = [[11,12,13],[21,22,23],[28,29,30]]
row_colors = ['red','gold','green']
my_table = plt.table(cellText=table_vals, colWidths=[0.1]*3,
                     rowLabels=row_labels, colLabels=col_labels,
                     rowColours=row_colors, colColours=row_colors,
                     loc='best')
plt.plot(y)

plt.show()
```

![这里写图片描述](https://img-blog.csdn.net/20170708212124953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



