
# Python TreeMap可视化方案的变种实现 - OraYang的博客 - CSDN博客

2017年09月04日 20:29:34[OraYang](https://me.csdn.net/u010665216)阅读数：2205所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



前段时间做Kaggle,在数据探索阶段想将数据可视化，但是发现python现有的画图库里没有相关的算法实现，而R语言与JS只要两行代码就能搞定了。。。本着探索精神，我就看了下R语言的实现代码，然后用python变相的实现了一下~
实验数据：[传送门 ](http://download.csdn.net/download/u010665216/9964241)
代码：

```python
# author:OraYang
#import some lib
import numpy as np # linear algebra
import pandas as pd # data processing, CSV file I/O (e.g. pd.read_csv)
import matplotlib.pyplot as plt
import seaborn as sns
color = sns.color_palette()
%matplotlib inline
pd.options.mode.chained_assignment = None  # default='warn'
products_df = pd.read_csv("../input/products.csv")
aisles_df = pd.read_csv("../input/aisles.csv")
departments_df = pd.read_csv("../input/departments.csv")
order_products_prior_df = pd.merge(products_df, aisles_df, on='aisle_id', how='left')
order_products_prior_df = pd.merge(order_products_prior_df, departments_df, on='department_id', how='left')
order_products_prior_df.head()
import matplotlib
import squarify
temp = order_products_prior_df[['product_name','aisle','department']]
temp = pd.concat([
    order_products_prior_df.groupby('department')['product_name'].nunique().rename('products_department'),
    order_products_prior_df.groupby('department')['aisle'].nunique().rename('aisle_department')
    ], axis=1).reset_index()
temp = temp.set_index('department')
temp2 = temp.sort_values(by="aisle_department", ascending=False)
# treemap parameters
x = 0.
y = 0.
width = 100.
height = 100.
cmap = matplotlib.cm.viridis
# color scale on the population
# min and max values without Pau
mini, maxi = temp2.products_department.min(), temp2.products_department.max()
norm = matplotlib.colors.Normalize(vmin=mini, vmax=maxi)
colors = [cmap(norm(value)) for value in temp2.products_department]
colors[1] = "#FBFCFE"
# labels for squares
labels = ["%s\n%d aisle num\n%d products num" % (label) for label in zip(temp2.index, temp2.aisle_department, temp2.products_department)]
# make plot
fig = plt.figure(figsize=(12, 10))
fig.suptitle("How are aisles organized within departments", fontsize=20)
ax = fig.add_subplot(111, aspect="equal")
ax = squarify.plot(temp2.aisle_department, color=colors, label=labels, ax=ax, alpha=.7)
ax.set_xticks([])
ax.set_yticks([])
# color bar
# create dummy invisible image with a color map
img = plt.imshow([temp2.products_department], cmap=cmap)
img.set_visible(False)
fig.colorbar(img, orientation="vertical", shrink=.96)
fig.text(.76, .9, "numbers of products", fontsize=14)
fig.text(.5, 0.1,
         "powered by OraYang \n aisle totale %d, products total : %d" % (temp2.aisle_department.sum(), temp2.products_department.sum()),
         fontsize=14,
         ha="center")
fig.text(.5, 0.07,
         "Source : http://blog.csdn.net/column/details/16605.html",
         fontsize=14,
         ha="center")
plt.show()
```
最后的结果（如果出现无法复现的问题，欢迎私信我，看到一定会回复哒~）：
![](https://img-blog.csdn.net/20170904202806776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


