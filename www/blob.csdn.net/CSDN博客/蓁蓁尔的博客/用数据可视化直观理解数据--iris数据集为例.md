# 用数据可视化直观理解数据--iris数据集为例 - 蓁蓁尔的博客 - CSDN博客





2017年04月07日 15:43:57[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：5312标签：[数据可视化](https://so.csdn.net/so/search/s.do?q=数据可视化&t=blog)
个人分类：[机器学习相关																[python相关](https://blog.csdn.net/u013527419/article/category/6291293)](https://blog.csdn.net/u013527419/article/category/6202203)








原文：[https://www.kaggle.com/benhamner/d/uciml/iris/python-data-visualizations](https://www.kaggle.com/benhamner/d/uciml/iris/python-data-visualizations)

See [Kaggle Datasets](https://www.kaggle.com/datasets) for other datasets to try visualizing. The [World Food Facts data](https://www.kaggle.com/openfoodfacts/world-food-facts) is an especially rich one for visualization.

**比较常用的图表有：**
- 查看目标变量的分布。当分布不平衡时，根据评分标准和具体模型的使用不同，可能会严重影响性能。 

-iris[“Species”].value_counts()
- 对 Numerical Variable，可以用 Box Plot 来直观地查看它的分布。 

-sns.boxplot(x=”Species”, y=”PetalLengthCm”, data=iris)
- 对于坐标类数据，可以用 Scatter Plot 来查看它们的分布趋势和是否有离群点的存在。 

-iris.plot(kind=”scatter”, x=”SepalLengthCm”, y=”SepalWidthCm”) 

-sns.jointplot(x=”SepalLengthCm”, y=”SepalWidthCm”, data=iris, size=5)- 对于分类问题，将数据根据 Label 的不同着不同的颜色绘制出来，这对 Feature 的构造很有帮助。 

-sns.FacetGrid(iris,hue=”Species”,size=5).map(plt.scatter,”SepalLengthCm”,”SepalWidthCm”).add_legend()
- 绘制变量之间两两的分布和相关度图表。 

-sns.pairplot(iris, hue=”Species”, size=3) 

-sns.pairplot(iris, hue=”Species”, size=3, diag_kind=”kde”)
**详细例子说明**

```python
# coding=utf-8
import pandas as pd
from sklearn.datasets import load_iris

# 载入seaborn,因为载入时会有警告出现，因此先载入warnings，忽略警告。
import warnings 
warnings.filterwarnings("ignore")
import seaborn as sns
import matplotlib.pyplot as plt
sns.set(style="white", color_codes=True)

iris = pd.DataFrame(load_iris().data)
iris.columns = ['SepalLengthCm','SepalWidthCm','PetalLengthCm','PetalWidthCm']
iris['Species'] = load_iris().target
# 首先探索一下数据集的大体情况，多少个样本，每一个样本多少个特征等
print iris.shape
print iris.head()
```

![这里写图片描述](https://img-blog.csdn.net/20170407153210011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# 探索一下是否为均衡分类
# 分几类？每一类多少个样本？
# 可以从结果看出，很均匀。。。若为不均衡分类，则可参照此文https://www.svds.com/learning-imbalanced-classes/进行处理。
print iris["Species"].value_counts()
```

![这里写图片描述](https://img-blog.csdn.net/20170407153328684?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# plot是画图的最主要方法，Series和DataFrame都有plot方法。
# plot默认生成是曲线图，你可以通过kind参数生成其他的图形，可选的值为：line, bar(柱状图), barh, 
# kde, density, scatter（散点图）。
# 对于坐标类数据，可以用 Scatter Plot 来查看它们的分布趋势和是否有离群点的存在
iris.plot(kind="scatter", x="SepalLengthCm", y="SepalWidthCm")
```

![这里写图片描述](https://img-blog.csdn.net/20170407153409085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# 用seaborn库来画出与上面相似的图
# seaborn 的 jointplot 函数可以在同一个图中画出二变量的散点图和单变量的柱状图
sns.jointplot(x="SepalLengthCm", y="SepalWidthCm", data=iris, size=5)
```

![这里写图片描述](https://img-blog.csdn.net/20170407153437029?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# 上面的两个散点图并不能显示每一个点所属的类别
# 所以，接下来用 seaborn 的 FacetGrid 函数按照Species花的种类来在散点图上标上不同的颜色，hue英文是色彩的意思。
sns.FacetGrid(iris, hue="Species", size=5).map(plt.scatter, "SepalLengthCm", "SepalWidthCm").add_legend()
```

![这里写图片描述](https://img-blog.csdn.net/20170407154136250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# 通过箱线图来查看单个特征的分布
# 对 Numerical Variable，可以用 Box Plot 来直观地查看不同花类型的分布。
sns.boxplot(x="Species", y="PetalLengthCm", data=iris)
```

![这里写图片描述](https://img-blog.csdn.net/20170407153831498?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# 下面的操作，将每一个Species所属的点加到对应的位置，加上散点图，
# 振动值jitter=True 使各个散点分开，要不然会是一条直线
# 注意此处要将坐标图用ax先保存起来，这样第二次才会在原来的基础上加上散点图
ax = sns.boxplot(x="Species", y="PetalLengthCm", data=iris)
ax = sns.stripplot(x="Species", y="PetalLengthCm", data=iris, jitter=True, edgecolor="gray")
```

![这里写图片描述](https://img-blog.csdn.net/20170407153908343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# violinplot 小提琴图，查看密度分布，结合了前面的两个图，并且进行了简化
# 数据越稠密越宽，越稀疏越窄
sns.violinplot(x="Species", y="PetalLengthCm", data=iris, size=6)

# sns.kdeplot == kernel density 核密度图（单个变量）
sns.FacetGrid(iris, hue="Species", size=6).map(sns.kdeplot, "PetalLengthCm").add_legend()

# pairplot 任意两个变量间的关系
sns.pairplot(iris, hue="Species", size=3)
```

![这里写图片描述](https://img-blog.csdn.net/20170407154004532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# 上图中，中间对角线的部分图形也可以用kde显示
sns.pairplot(iris, hue="Species", size=3, diag_kind="kde")
```

![这里写图片描述](https://img-blog.csdn.net/20170407154020376?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# 下面的用Pandas自带的画图
# We can quickly make a boxplot with Pandas on each feature split out by species
iris.boxplot(by="Species", figsize=(12, 6))

# Andrews Curves 用样本的各个属性作为傅里叶级数的系数然后画出来
# Andrews Curves 将高维的点化为二维的曲线，曲线是一条傅里叶函数的样子，参数项为不# 同的特征值，臆想出来了自变量t,这样每个点都是一条曲线
from pandas.tools.plotting import andrews_curves
andrews_curves(iris, "Species")
#
# Another multivariate visualization technique pandas has is parallel_coordinates
# Parallel coordinates 将每个特征作为单独的一列画，然后用线将每一个样本的特征连接起来
from pandas.tools.plotting import parallel_coordinates
parallel_coordinates(iris, "Species")

# Which puts each feature as a point on a 2D plane, and then simulates
# having each sample attached to those points through a spring weighted by the 
# relative value for that feature
from pandas.tools.plotting import radviz
radviz(iris, "Species")

plt.show()
```

完整版代码，可直接下载：[http://download.csdn.net/download/u013527419/9806632](http://download.csdn.net/download/u013527419/9806632)

seaborn 库的一些简单用法，可以见：[http://blog.csdn.net/longgb123/article/details/53228256](http://blog.csdn.net/longgb123/article/details/53228256)

里面有几篇文章是介绍这个库的






