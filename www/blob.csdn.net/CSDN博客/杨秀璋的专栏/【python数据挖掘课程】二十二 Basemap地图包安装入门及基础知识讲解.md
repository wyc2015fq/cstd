# 【python数据挖掘课程】二十二.Basemap地图包安装入门及基础知识讲解 - 杨秀璋的专栏 - CSDN博客





2018年01月29日 19:22:04[Eastmount](https://me.csdn.net/Eastmount)阅读数：5340
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)








这是《Python数据挖掘课程》系列文章，也是我上课内容及书籍中的一个案例。本文主要讲述Matplotlib子包，负责地图绘制，即Basemap扩展包。在做数据挖掘或可视化分析时，常常需要将数据显示到地图上，比如城市人口、空气环境、GDP分布、资源销售、全球热图等。本文主要讲解安装入门以及官网介绍的基础知识，后面文章将结合实例深入讲解。内容包括：
1.Basemap安装过程
    2.地图绘制官方简单案例
    3.Basemap绘制地图

本篇文章为基础性文章，希望对你有所帮助，提供些思路，如果文章中存在错误或不足之处，还请海涵。同时，推荐大家阅读我以前的文章了解基础知识。



前文参考：
[【Python数据挖掘课程】一.安装Python及爬虫入门介绍](http://blog.csdn.net/eastmount/article/details/52577215)
[【Python数据挖掘课程】二.Kmeans聚类数据分析及Anaconda介绍](http://blog.csdn.net/eastmount/article/details/52777308)
[【Python数据挖掘课程】三.Kmeans聚类代码实现、作业及优化](http://blog.csdn.net/eastmount/article/details/52793549)
[【Python数据挖掘课程】四.决策树DTC数据分析及鸢尾数据集分析](http://blog.csdn.net/eastmount/article/details/52820400)
[【Python数据挖掘课程】五.线性回归知识及预测糖尿病实例](http://blog.csdn.net/eastmount/article/details/52929765)
[【Python数据挖掘课程】六.Numpy、Pandas和Matplotlib包基础知识](http://blog.csdn.net/eastmount/article/details/53144633)
[【Python数据挖掘课程】七.PCA降维操作及subplot子图绘制](http://blog.csdn.net/eastmount/article/details/53285192)
[【Python数据挖掘课程】八.关联规则挖掘及Apriori实现购物推荐](http://blog.csdn.net/eastmount/article/details/53368440)
[【Python数据挖掘课程】九.回归模型LinearRegression简单分析氧化物数据](http://blog.csdn.net/eastmount/article/details/60468818)
[【python数据挖掘课程】十.Pandas、Matplotlib、PCA绘图实用代码补充](http://blog.csdn.net/eastmount/article/details/60675865)
[【python数据挖掘课程】十一.Pandas、Matplotlib结合SQL语句可视化分析](http://blog.csdn.net/eastmount/article/details/62489186)
[【python数据挖掘课程】十二.Pandas、Matplotlib结合SQL语句对比图分析](http://blog.csdn.net/eastmount/article/details/64127445)
[【python数据挖掘课程】十三.WordCloud词云配置过程及词频分析](http://blog.csdn.net/eastmount/article/details/64438407)
[【python数据挖掘课程】十四.Scipy调用curve_fit实现曲线拟合](http://blog.csdn.net/eastmount/article/details/71308373)
[【python数据挖掘课程】十五.Matplotlib调用imshow()函数绘制热图](http://blog.csdn.net/eastmount/article/details/73392106)
[【python数据挖掘课程】十六.逻辑回归LogisticRegression分析鸢尾花数据](http://blog.csdn.net/eastmount/article/details/77920470)
[【python数据挖掘课程】十七.社交网络Networkx库分析人物关系（初识篇）](http://blog.csdn.net/eastmount/article/details/78452581)
[【python数据挖掘课程】十八.线性回归及多项式回归分析四个案例分享](http://blog.csdn.net/eastmount/article/details/78635096)

[【python数据挖掘课程】十九.鸢尾花数据集可视化、线性回归、决策树花样分析](http://blog.csdn.net/eastmount/article/details/78692227)

[【python数据挖掘课程】二十.KNN最近邻分类算法分析详解及平衡秤TXT数据集读取](http://blog.csdn.net/eastmount/article/details/78747128)

[【python数据挖掘课程】二十一.朴素贝叶斯分类器详解及中文文本舆情分析](http://blog.csdn.net/eastmount/article/details/79128235)




安装包及代码CSDN下载地址：[http://download.csdn.net/download/eastmount/10230303](http://download.csdn.net/download/eastmount/10230303)
为何现在不能上传免费资源了，可惜啦！










## 一. Basemap安装过程


安装方法作者强推这篇文章： [神农尝百草---Windows环境下为Python3.X安装basemap](http://blog.csdn.net/moxigandashu/article/details/68945845)

这里作者只做简单介绍，我们从非官网网站下载下面两个扩展包，cp27表示Python2.7版本，Windows下64位环境，你需要找到适合自己环境的文件。下载地址为：[https://www.lfd.uci.edu/~gohlke/pythonlibs/](https://www.lfd.uci.edu/~gohlke/pythonlibs/)


- basemap‑1.1.0‑cp27‑cp27m‑win_amd64.wh
- pyproj‑1.9.5.1‑cp27‑cp27m‑win_amd64.whl





![](https://img-blog.csdn.net/20180129124553443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


接下来使用PIP命令去到whl文件所在目录，然后分别进行安装。
先安装Pyproj，再安装Basemap扩展包，命令如下：

```python
cd C:\Software\Program Software\Python\Scripts\basemap
pip install pyproj-1.9.5.1-cp27-cp27m-win_amd64.whl
pip install basemap-1.1.0-cp27-cp27m-win_amd64.whl
```
安装如下图所示，显示"Successfully installed basemap-1.1.0"表示安装成功。


![](https://img-blog.csdn.net/20180129124911891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


接下来调用该扩展包，不再报错即表示安装成功。


![](https://img-blog.csdn.net/20180129125536435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)










## 二. 地图绘制官方简单案例


首先作者给出官方文档的介绍以及简单案例，让读者体会下Basemap的魅力。
官方网址：[https://matplotlib.org/basema](https://matplotlib.org/basema)


**介绍**
Basemap工具包是Matplotlib包的子包，一个用于在Python绘制2D数据至地图的库，它提供了将坐标转化为25中不同地图投影的功能，然后调用Matplotlib扩展包绘制轮廓、图像和坐标点等。该扩展包提供了海岸线、河流、政治边界数据集以及绘制方法。其中GEOS库在内部用于将海岸线和边界特征剪切到所需的地图投影区域。

**示例**
下面给出官网的翻译示例，源地址：[https://matplotlib.org/basemap/users/geography.html](https://matplotlib.org/basemap/users/geography.html)

Basemap包括GSSH（现在是GSHHG）海岸线数据集以及GMT格式的河流、州和国家边界的数据集。这些数据集可以用来以不同的分辨率绘制海岸线、河流和政治边界地图。相关方法如下：
**drawcoastlines(): **绘制海岸线。
**    fillcontinents(): **通过填充海岸线多边形为地图着色。
**    drawcountries(): **绘制国家边界。
**    drawstates(): **在北美绘制状态边界。
**    drawrivers(): **绘制河流。

此外，可以将图像用作地图背景，而不是绘制海岸线和政治边界。Basemap提供了以下几个选项：
**drawlsmask():** 绘制高分辨率的海陆图像，指定陆地和海洋的颜色，数据源于GSHHS海岸线。
**bluemarble():** 绘制NASA蓝色大理石图像作为地图背景。
**shadedrelief():** 绘制阴影浮雕图像作为地图背景。
**etopo():** 绘制一张etopo浮雕图像作为地图背景。
**warpimage():**  使用abitrary图像作为地图背景，必须是全球新的，从国际日东线向东和南极以北覆盖世界。

**1.绘制海岸线、填充海洋和陆地区域**
代码


```python
# -*- coding: utf-8 -*-
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt

# 设置basemap-Lambert Conformal 
m = Basemap(width=12000000,height=9000000,projection='lcc',
            resolution='c',lat_1=45.,lat_2=55,lat_0=50,lon_0=-107.)

# 绘制海岸线
m.drawcoastlines()
# 在地图周围绘制边界并填充背景aqua（这个背景最终成为海洋的颜色）
# 将大洲绘制在最上面
m.drawmapboundary(fill_color='aqua')

# 填充大陆coral颜色,并设置湖泊颜色为blue
m.fillcontinents(color='coral',lake_color='blue')
plt.show()
```
绘制图形如下所示：


![](https://img-blog.csdn.net/20180129135247219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**2.绘制一幅海陆罩（land-sea mask）图像**
代码

```python
# -*- coding: utf-8 -*-
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt

# 设置basemap Lambert-Conformal 
# 设置分辨率参数resolution=None 跳过处理边界数据集
m = Basemap(width=12000000,height=9000000,projection='lcc',
            resolution=None,lat_1=45.,lat_2=55,lat_0=50,lon_0=-107.)

# 为地图背景绘制海陆罩
# lakes=True 意味着内陆湖和海洋颜色一致
m.drawlsmask(land_color='coral',ocean_color='aqua',lakes=True)
plt.show()
```
绘制如下图所示：


![](https://img-blog.csdn.net/20180129175131098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**3.绘制美国宇航局蓝色大理石图像（the NASA Blue Marble）**
代码


```python
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt

m = Basemap(width=12000000,height=9000000,projection='lcc',
            resolution=None,lat_1=45.,lat_2=55,lat_0=50,lon_0=-107.)
m.bluemarble()
plt.show()
```
绘制如下图所示：


![](https://img-blog.csdn.net/20180129175514027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**4.绘制带阴影的浮雕图像**
代码





```python
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt

m = Basemap(width=12000000,height=9000000,projection='lcc',
            resolution=None,lat_1=45.,lat_2=55,lat_0=50,lon_0=-107.)
m.shadedrelief()
plt.show()
```
绘制如下图所示：


![](https://img-blog.csdn.net/20180129175907414?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**5.绘制etopo浮雕图像**
代码





```python
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt

m = Basemap(width=12000000,height=9000000,projection='lcc',
            resolution=None,lat_1=45.,lat_2=55,lat_0=50,lon_0=-107.)
m.etopo()
plt.show()
```
绘制如下图所示：


![](https://img-blog.csdn.net/20180129180057618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)











## 三. Basemap绘制地图




可能读者已经发现了，Basemap扩展包是绘制地图最重要的一个库，所以作者先讲解该库函数。同时，由于作者没有找到系统的参数介绍，只能摸着石头过河学习，也希望对您有点帮助。

为方便呈现，需要将三维球坐标系转换为二维笛卡尔坐标系，利用地图投影（Map Projection）实现。
官网给出的25种映射方式如下：[https://matplotlib.org/basemap/users/mapsetup.html](https://matplotlib.org/basemap/users/mapsetup.html)



![](https://img-blog.csdn.net/20180129190154719?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**m = Basemap(width=12000000,height=9000000,projection='lcc',                       resolution=None,lat_1=45.,lat_2=55,lat_0=50,lon_0=-107.)**


参数介绍如下：

    width：宽度。
    height：高度。
    projection='lcc'：表示规定的投影方法，改变投影方法绘制的结果也将不同，25种方式。
    resolution=None：表示跳过处理边界数据集。
    lat_0=50： 维度设置为50（Latitude，值为-90到90）。
    lon_0=-107：经度设置为-107（Longitude，值为-180到180）。
示例参考：[https://matplotlib.org/basemap/users/aeqd.html](https://matplotlib.org/basemap/users/aeqd.html)




```python
# -*- coding: cp936 -*-
from mpl_toolkits.basemap import Basemap
import numpy as np
import matplotlib.pyplot as plt

width = 28000000; lon_0 = -105; lat_0 = 40
m = Basemap(width=width,height=width,projection='aeqd',
            lat_0=lat_0,lon_0=lon_0)

# 填充背景
m.drawmapboundary(fill_color='aqua')
# 绘制海岸线并填充大陆
m.drawcoastlines(linewidth=0.5)
m.fillcontinents(color='coral',lake_color='aqua')
# 20度经纬度，范围-80到81 -180到180
m.drawparallels(np.arange(-80,81,20))
m.drawmeridians(np.arange(-180,180,20))
# 在中心绘制一个黑点
xpt, ypt = m(lon_0, lat_0)
m.plot([xpt],[ypt],'ko')
# 绘制标题
plt.title('Azimuthal Equidistant Projection')
plt.show()
```
绘制图形如下图所示：


![](https://img-blog.csdn.net/20180129190005786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**m = Basemap(projection='merc',llcrnrlat=-80,urcrnrlat=80,                       llcrnrlon=-180,urcrnrlon=180)**

参数介绍如下：
    projection=merc'：表示规定的投影方法，墨卡托投影（Mercator Projection），广泛应用谷歌地图。
    llcrnrlat=-80：所需地图域左下角的纬度（度）Latitude。
urcrnrlat=80：所需地图域的右上角的纬度（度）Latitude。
llcrnrlon=-180：所需地图域左下角的经度（度）Longitude。
urcrnrlon=180：所需地图域（度）的右上角的经度Longitude。
示例：

```python
# -*- coding: utf-8 -*-
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt

m = Basemap(projection='merc', llcrnrlat=-80, urcrnrlat=80,
            llcrnrlon=-180, urcrnrlon=180)
m.drawcoastlines()
plt.show()
```
绘制图形如下所示：


![](https://img-blog.csdn.net/20180129185423068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


同时，绘制一个城市点代码的常见两种方式如下：

```python
# 上海经纬度 (31.23N,121.47E)
x = 31.23
y = 121.47
xpt, ypt = m(x,y)
m.scatter(xpt,ypt,s=2)

x = 31.23
y = 121.47
xpt, ypt = m(x,y)
m.plot([xpt],[ypt],'ko')
```


最后给出比较好的参考资料，供自己和大家后面学习。希望文章对大家有所帮助，该篇文章主要是个安装引入，后面还将结合项目及管网深入介绍，如果存在错误或不足之处，还请海涵。
[python利用basemap叠加地图 - 瑜之姬](http://blog.csdn.net/LXW19900607/article/details/49183183)
[地理信息可视化：Python之matplotlib Basemap简介 - dushenzhi](http://blog.csdn.net/dushenzhi/article/details/9195743)
[BASEMAP使用筆記 - KalariKong](http://blog.csdn.net/ssy8stephy/article/details/52589315)
[Basemap可视化地图信息 - zm（强推）](http://blog.csdn.net/zm714981790/article/details/51224650)
[[经验总结] 一点总结：Ubuntu+anaconda+Python+basemap(+WRF) - 气象家园](http://bbs.06climate.com/forum.php?mod=viewthread&tid=49137&from=singlemessage)
[绘图: matplotlib Basemap简介 - Vamei](https://www.cnblogs.com/vamei/archive/2012/09/16/2687954.html)
[Creating a Choropleth Map of the World in Python using Basemap](http://ramiro.org/notebook/basemap-choropleth/)
[Basemap绘制中国地图 - 落叶小唱](http://blog.csdn.net/ouening/article/details/55227364)
[用Python画一个中国地图 - 张京（强推）](https://segmentfault.com/a/1190000010871928)



![](https://img-blog.csdn.net/20180129191402599?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20180129191419955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180129191437275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20180129191451566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180129191529633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



(By:Eastmount 2018-01-29 晚上7点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))








