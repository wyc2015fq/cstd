
# 使用R读取shapefile并画地图的过程 - jiahaowanhao的博客 - CSDN博客


2018年06月05日 13:56:16[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：883


[使用R读取shapefile并画地图的过程](http://cda.pinggu.org/view/25731.html)
下面是一个使用R语言绘制地图的例子。
\# 首先需要加载需要使用的两个package：
library(maptools)
library(rgdal)
\# 读取数据并绘图，下面读取的是中国行政区域的shapefile到变量x中：
x=readShapePoly('bou2_4p.shp')
\# 现在我们可以直接画出地图：
plot(x)
![](http://cda.pinggu.org/uploadfile/image/20180604/20180604064014_69259.png)
但是上面的绘图并未使用投影而是直接使用经纬度作为平面坐标进行绘图，所以是不合适的。下面的代码首先进行mercator投影然后绘图。
[plain] view plain copy
\# 首先，我们需要指定x本身的投影信息，下面一行代码指定x对应的投影为原始的地理坐标（经纬度）：
proj4string(x) <- CRS("+proj=longlat +ellps=WGS84")
\# 然后我们指定新的投影方式，并将x投影到其上：
projNew <- CRS("+proj=merc +lat_0=45n +lon_0=100e")
xProj <- spTransform(x, projNew)
\# 现在我们可以画出它了：
dev.new()
plot(xProj)
![](http://cda.pinggu.org/uploadfile/image/20180604/20180604064003_62300.png)

