# ArcGIS API for Python(2) - 长歌行 - CSDN博客





2018年01月03日 18:18:46[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1089
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









在前一篇博客里面对ArcGIS API for Python做了简单的介绍。 
[我的Python学习笔记（5） ArcGIS API for Python](http://blog.csdn.net/uninterrupted/article/details/77884656)

在这一篇文章里面，准备简单的使用此API。用例就是绘制朝阳区出租房价格热力图。

**数据准备**

准备方式多样，在这里，我为了简单，直接使用实验楼的一段代码，爬了一些样例数据。经过基本的预处理之后，大概是这样的。基本上包括，价格和地址就可以了。价格用来做热力图的字段，地址用来解析经纬度地址。

![这里写图片描述](https://img-blog.csdn.net/20180103174716941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后在Jupyter里面看看数据吧。

```python
import pandas as pd
import calendar

renting_path = r'./renting.csv'
renting_df = pd.read_csv(renting_path)
renting_df.head(10)
```

![这里写图片描述](https://img-blog.csdn.net/20180103175004778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

此处应该画更精美的图的，偷懒了哈。

![这里写图片描述](https://img-blog.csdn.net/20180103175045847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**打开online或者portal**

我这里就用Esri北京研发中心的portal作为示例

```python
from arcgis.gis import GIS
from IPython.display import display
portal = GIS('http://yourorganization.maps.arcgis.com', YOUR_USERNAME, YOUR_PASSWORD)
```

**上传csv数据**

```python
renting_path = r'./renting.csv'
renting_csv = portal.content.add({}, renting_path)
display(renting_csv)
```

![这里写图片描述](https://img-blog.csdn.net/20180103175514012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

马赛克处理的是我的用户名，用户名里带有组织名称，我就给它打码了。

**发布为FeatureLayer(hosted)**

```
renting_layer = renting_csv.publish()
# you can also use this to find it
renting_layer = portal.content.search('renting', 'feature layer')[0]
display(renting_layer)
```

![这里写图片描述](https://img-blog.csdn.net/20180103175755574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这儿可以参考[publishing-sd-shapefiles-and-csv](https://developers.arcgis.com/python/sample-notebooks/publishing-sd-shapefiles-and-csv/)这篇文章。再结合ArcGIS REST API的相关规范，就不难写好参数。 

我这里图简单，经纬度字段直接使用系统默认可以支持的，所以不用在publish的参数里面指定。

**地理编码/解码**

这一节，我失败了。。 

代码如下
```
import arcgis
single_line_address = "beijing"
geocoders = arcgis.geocoding.get_geocoders(portal)

#geocoders
beijing = arcgis.geocoding.geocode(address=single_line_address, geocoder=geocoders[0])
beijing
```

返回值是空: `beijing = []`

不过Esri自带的GeoCoder对于中国地址支持得并不好。可以使用国产API。由于文章的重点并不在意这儿，所以我使用了最简单的`Python Geocoder package`. 代码大概是这样的：

```python
import geocoder
rlt = geocoder.google(address).json
if 'lng' in rlt:
    return rlt['lng'], rlt['lat']
```

**绘制热力图**

```
map = portal.map('Beijing', 11)
map.add_layer(renting_layer, {"renderer" : "HeatmapRenderer", "opacity" : 0.75, "field" : "price"})
map
```

![这里写图片描述](https://img-blog.csdn.net/20180103181002764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里主要爬的是东面的数据做一个演示。所以有数据缺失的情况。

后续还准备使用Esri的空间分析功能，以及智能制图功能来渲各种图，以及进行空间分析的。但是精力有限。本次实验到此结束。下次会使用更好的数据，来做进一步的实验。

就我目前的使用体验来看，主要是觉得文档和API的版本有脱节的现象，使用起来有些不方便。如果对于Esri的online，portal，rest api规范不太熟悉的话，会有一些概念难以理解，一些参数不知道该如何设置。但是从代码的架构，代码风格来看，再过一段时间，必然会非常非常火。（2018年1月3日18:16:14）




