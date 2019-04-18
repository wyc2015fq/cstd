# PythonGIS可视化—Matplot basemap工具箱 - wishchinYang的专栏 - CSDN博客
2014年07月13日 14:36:42[wishchin](https://me.csdn.net/wishchin)阅读数：3634
原文链接：[http://www.douban.com/group/topic/32821988/](http://www.douban.com/group/topic/32821988/)
原文链接：[http://www.cnblogs.com/vamei/archive/2012/09/16/2687954.html](http://www.cnblogs.com/vamei/archive/2012/09/16/2687954.html)
在数据可视化过程中，我们常常需要将数据根据其采集的地理位置在地图上显示出来。比如说我们会想要在地图上画出城市，飞机的航线，乃至于军事基地等等。通常来说，一个地理信息系统都会带有这样的功能。今天我们讨论如何在Python上实现，并且使用免费的工具包。
Matplot是python上常用的一个数据绘制包。它基于numpy(numpy是python用于数组运算的一个包)。Matplot具有强大的数据绘制功能，可以轻易地画出各种统计图形，比如散点图，条行图，饼图等等。而Matplot中的basemap则允许我们绘制出地图，并在地图上继续画出数据。
( Matplot经常与numpy和scipy相配合，用于科学研究。他们是Matlab的强劲对手，这相当可以理解，因为Matlab一套需要好几千块，而python工具则是免费的。)
我们今天的目标是用上述工具画出亚洲主要城市的人口。如下图所示，人口的多少以橙色小圆圈的大小表示：
![](http://img3.douban.com/view/group_topic/large/public/32821988-1.jpg)
数据如下(我从wikipedia上整理的，你可以随意使用)，我将数据保存在文件major_city:
复制代码
Shanghai 23019148 31.23N 121.47E China
Mumbai 12478447 18.96N 72.82E India
Karachi 13050000 24.86N 67.01E Pakistan
Delhi 16314838 28.67N 77.21E India
Manila 11855975 14.62N 120.97E Philippines
Seoul 23616000 37.56N 126.99E Korea(South)
Jakarta 28019545 6.18S 106.83E Indonesia
Tokyo 35682460 35.67N 139.77E Japan
Peking 19612368 39.91N 116.39E China
复制代码
第一列是城市名，第二列是人口，第三第四列为纬度和经度，最后一列为所在国家。
下面是我的Python代码:
复制代码
# Written by Vamei, [http://www.cnblogs.com/vamei/](http://www.cnblogs.com/vamei/)
# Feel free to use or modify this script.
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt
import numpy as np
#============================================# read data
names = []
pops = []
lats = []
lons = []
countries = []
for line in file("../data/major_city"):
info = line.split()
names.append(info[0])
pops.append(float(info[1]))
lat = float(info[2][:-1])
if info[2][-1] == 'S': lat = -lat
lats.append(lat)
lon = float(info[3][:-1])
if info[3][-1] == 'W': lon = -lon + 360.0
lons.append(lon)
country = info[4]
countries.append(country)
#============================================
# set up map projection with
# use low resolution coastlines.
map = Basemap(projection='ortho',lat_0=35,lon_0=120,resolution='l')
# draw coastlines, country boundaries, fill continents.
map.drawcoastlines(linewidth=0.25)
map.drawcountries(linewidth=0.25)
# draw the edge of the map projection region (the projection limb)
map.drawmapboundary(fill_color='#689CD2')
# draw lat/lon grid lines every 30 degrees.
map.drawmeridians(np.arange(0,360,30))
map.drawparallels(np.arange(-90,90,30))
# Fill continent wit a different color
map.fillcontinents(color='#BF9E30',lake_color='#689CD2',zorder=0)
# compute native map projection coordinates of lat/lon grid.
x, y = map(lons, lats)
max_pop = max(pops)
# Plot each city in a loop.
# Set some parameters
size_factor = 80.0
y_offset = 15.0
rotation = 30
for i,j,k,name in zip(x,y,pops,names):
size = size_factor*k/max_pop
cs = map.scatter(i,j,s=size,marker='o',color='#FF5600')
plt.text(i,j+y_offset,name,rotation=rotation,fontsize=10)
plt.title('Major Cities in Asia & Population')
plt.show()
复制代码
程序分为两个部分，第一部分为从文件读取数据并处理，第二部分是用basemap绘图。
map = Basemap(projection='ortho',lat_0=35,lon_0=120,resolution='l')
设置了地图投影的方法。从球形的地球表面投影到平面地图有许多方法可以选择，得到的结果也非常不同。
我们的经纬度通过
x, y = map(lons, lats)
语句转换为图像上的位置，并通过
cs = map.scatter(i,j,s=size,marker='o',color='#FF5600')
画散点图的方法在地图上画出来。
文中的需要的软件包包括
numpy, matplot, mpl_toolkits
你可以google并找到它们。
另外，如果是也可以下载epd python。它整合了python以及所有的软件包。(epd python是商业版，但对于学术用户免费。)
            
