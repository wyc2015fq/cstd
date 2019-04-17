# R+大地图时代︱ leaflet/leafletCN  动态、交互式绘制地图（遍地代码图） - 素质云笔记/Recorder... - CSDN博客





2017年02月25日 18:28:25[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：13172








> 
好久没有学习R的新包了，甚是想念啊！ 

  昨天、今天看到两个极好、不得不学的packages+早上被AWS的服务器整得郁闷ing…于是就来点颜色看看~


本篇受Lchiffon老师的github启发，对两个packages进行简单的试玩。leaflet是一个国外动态交互图做得很棒的网站，Lchiffon老师对其进行一些封装，适应了“中国国情”，那就让我们先来看看Lchiffon的leafletCN。 

. 

.

**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


# 一、leafletCN

本节主要“抄袭”Lchiffon的blog、github，对简单封装的函数进行简述。leafletCN封装了之后，函数并不多，但是实打实的好用。 

github：[https://github.com/lchiffon/leafletCN](https://github.com/lchiffon/leafletCN)

超详细版PPT：[http://langdawei.com/leafletIntro/Untitled.html#1](http://langdawei.com/leafletIntro/Untitled.html#1)
## 1、安装与函数简述

```
## 稳定版
install.packages("leafletCN")
## 开发版
devtools::install_github("lchiffon/leafletCN")
```
- 主函数介绍：

regionNames：找地名函数，极其方便 

demomap：有地名就给你输出一个交互、动态的区域图，真方便 

geojsonMap：作用是分区块标色- 辅助函数介绍：

amap ()：中国国情，高德地图的象征 

read.geoShape：可以将geojson的对象,保存成spdataframe,以方便leaflet调用 

leafletGeo：地图+小显示框，用地图名以及一个数据框创建一个sp的对象 

.
## 2、主函数介绍
- **（1）regionNames函数**

```
#主函数——regionNames：找地名函数
regionNames("浙江")
regionNames("浙江省")
regionNames("杭州")
regionNames("杭州市")
regionNames()
```

regionNames的粒度是省级、县级，在细分就没有了。执行regionNames()之后，可以知道包里面存着哪些地方，是否有你要的内容。
- **（2）主函数——demomap：区域式地图**

```
install.packages("rgeos")
demomap("杭州")
```

交互式界面，可以拖拽。 
![这里写图片描述](https://img-blog.csdn.net/20170225163722378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- **（3）主函数——geojsonmap：地图标色函数**

```
#主函数——geojsonmap：地图标色函数  
dat = data.frame(name = regionNames("china"),
                 value = runif(34))
geojsonMap(dat,"china")
```

输入数据的长这样，好简单！！

```
regionNames..china..   runif.34.
1      新疆维吾尔自治区 0.030015715
2            西藏自治区 0.746207077
3          内蒙古自治区 0.897344784
4                青海省 0.136039312
5                四川省 0.442047438
6              黑龙江省 0.410092158
7                甘肃省 0.292507600
```

![这里写图片描述](https://img-blog.csdn.net/20170225163857435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
## 3、辅助函数
- **（1）辅助函数——amap：高德地图底图**
`leaflet() %>%   amap()`
当然了，这么执行只有一个白板地图，并不是实体的。
- **（2）辅助函数——read.geoShape：genjson格式转化**

```
if(require(sp)){
  filePath = system.file("geojson/china.json",package = "leafletCN")
  map = read.geoShape(filePath)
  plot(map)
}
```

读入了china.json格式的内容。
- **（3）辅助函数——leafletGeo：地图+小显示框**

把一个数据框和一个地图组合在一起, 方便用leaflet调用, 其中名字的 变量为name, 数值的变量为value

.

## 4、leafletCN 两个案例

> 
案例在Lchiffon的博客基础上进行简单改编，以便对其中的函数进行简单说明。

- **（1）单点标注：地图+标点**

```
leaflet() %>%
  amap() %>%  
  addMarkers(lng=121.48, lat=31.22, popup="企业天地")
# 经度：lng
# 维度：lat
#popup：点的名称
```

第一、第二行调用高德地图底图，addMarkers为leaflet的标点函数，其中经纬度你可以自己上网查一下，这里简单百度了一下我的公司位置。 
![这里写图片描述](https://img-blog.csdn.net/20170225164951512?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- **（2）多点标注：地图+标点+带icoon**

```bash
#iconList 生成小框框
iconList = awesomeIconList(
  "home" = makeAwesomeIcon(icon = "home",markerColor = "skyblue"),
  "weixin" = makeAwesomeIcon(icon = "cutlery",markerColor = "red"),
  "bank" = makeAwesomeIcon(icon = "plus-sign",markerColor = "orange"),
  "automobile" = makeAwesomeIcon(icon = "trash",markerColor = "purple"),
  "coffee" = makeAwesomeIcon(icon = "book")
)
#每个小框框的坐标，经纬度
geo = data.frame(lon = rep(121.44, 5),
                 lat = rep(31.22, 5),
                 city = rep("Shanghai", 5))

#坐标进行微调，使得有差异
geo$lon = geo$lon+rnorm(5,0,0.003)
geo$lat = geo$lat+rnorm(5,0,0.003)

#小框框的属性
geo$type = c("home",
             "weixin",
             "bank" ,
             "automobile", 
             "coffee" )

#在高德地图上进行绘制
leaflet(geo) %>% amap() %>%
  addMiniMap() %>% 
  addAwesomeMarkers(icon = ~iconList[type])
```

awesomeIconList是小框框定义的一个函数； 

geo 是坐标点的经纬度，geo$type是坐标点的属性； 

leaflet(geo) ，放入数据； amap()，调出高德地图；  addMiniMap() ，看到右下角的小显示框了没？；  addAwesomeMarkers(icon = ~iconList[type])加入坐标点+坐标的icon
![这里写图片描述](https://img-blog.csdn.net/20170225170814725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

. 

 - **（3）地图+分区域显示+色彩**
```
if(require(leaflet)){
  region=regionNames("浙江")
  dat = data.frame(region,runif(length(region)))

  map = leafletGeo("浙江", dat)

  #涂色环节
  pal <- colorNumeric(
    palette = "Blues",
    domain = map$value)

  #载入高德地图amap
  leaflet(map) %>% amap() %>%
  #加入框边界及颜色
    addPolygons(stroke = TRUE,
                smoothFactor = 1,
                fillOpacity = 0.7,
                weight = 1,
                color = ~pal(value),
                popup = ~htmltools::htmlEscape(popup)
    ) %>%
    #加入右下角边框
    addLegend("bottomright", pal = pal, values = ~value,
              title = "legendTitle",
              labFormat = leaflet::labelFormat(prefix = ""),
              opacity = 1)
}
```

map 阶段，是导入数据，当然这里你也可以导入省级、县级，这里我导入的是浙江省。 

addPolygons为加入边界；addLegend加入右下角的程度显示框。

![这里写图片描述](https://img-blog.csdn.net/20170225170008042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

. 

.

# 二、leaflet 

> 
来到了更牛X的leaflet包了，那么leafletCN的函数都继承，因为有太多函数+应用，我这边只是简单列举+试玩一些。直接上案例，不介绍简单函数啦~ 

  leaflet官网：[http://leafletjs.com/index.html](http://leafletjs.com/index.html)

  详细R文档：[http://rstudio.github.io/leaflet/markers.html](http://rstudio.github.io/leaflet/markers.html)

  github地址：[https://github.com/Leaflet/Leaflet.markercluster](https://github.com/Leaflet/Leaflet.markercluster)

  .
## 1、说说底图

leaflet 的地图太多了，不过大多以全世界、美国的视角，简单列举几个：

```
leaflet() %>% addTiles() #openStreetMap 正常的世界地图
leaflet() %>% amap()  # 高德(leafletCN)
leaflet() %>% addProviderTiles("Thunderforest.SpinalMap")# 暗黑火焰风格
leaflet() %>% addProviderTiles("Esri.WorldImagery")# 卫星地图
leaflet() %>% addProviderTiles("OpenTopoMap")  #带海拔的地图
leaflet() %>% addProviderTiles("NASAGIBS.ViirsEarthAtNight2012")  #NASA夜景
leaflet() %>% addProviderTiles("Stamen.Toner")   #黑白底图
```

有很多，可以到这个网址（[链接](http://leaflet-extras.github.io/leaflet-providers/preview/)）去找，网址最右边有底图的名称，挑一个你喜欢的就行。

![这里写图片描述](https://img-blog.csdn.net/20170225171933319?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

. 

.
## 2、案例一——世界地图+多标记+标记显示数字

```
data(quakes)

# Show first 20 rows from the `quakes` dataset
leaflet(data = quakes[1:20,]) %>% addTiles() %>%
  addMarkers(~long, ~lat, popup = ~as.character(mag), label = ~as.character(mag))
```

quakes是packages自带的数据，数据长这样：

```
lat   long depth mag stations
1    -20.42 181.62   562 4.8       41
2    -20.62 181.03   650 4.2       15
3    -26.00 184.10    42 5.4       43
4    -17.97 181.66   626 4.1       19
```

leaflet(data = quakes[1:20,]) ，显示quakes数据集的前20个； 

addTiles() ，世界地图底纹； 

addMarkers(~long, ~lat, popup = ~as.character(mag), label = ~as.character(mag))中， 

~long, ~lat分别代表经度、维度；popup、label 从图中可以看到，那个数字6.1有两种显示方式，一个是标签式、一个是弹窗。
![这里写图片描述](https://img-blog.csdn.net/20170225172445932?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

. 

.
## 3、案例二——世界地图+多标记+图案标记

> 
对图案标记没有抵抗力，觉得很萌，所以对这块倒是研究了一下。本节案例来源：《[Markers With Custom 

  Icons](http://leafletjs.com/examples/custom-icons/)》


**图像icon下载网站：**
[http://ionicons.com/](http://ionicons.com/)
[http://fontawesome.io/icons/](http://fontawesome.io/icons/)
```
geo = data.frame(long = rep(121.44, 1000),
                 lat = rep(31.22, 1000),
                 mag=rep(5,1000))



geo$long = geo$long+rnorm(1000,0,0.1)
geo$lat = geo$lat+rnorm(1000,0,0.1)
geo$mag = geo$mag+rnorm(1000,0,1)

quakes1 <- geo[1:10,]
#数据生成阶段，数据长这样
      long      lat      mag
1  121.2867 31.29484 4.604269
2  121.3998 31.03906 4.946211
3  121.5652 31.33814 3.646438
4  121.3395 31.25626 5.845534

#图标标记阶段
leafIcons <- icons(
  iconUrl = ifelse(quakes1$mag < 4.6,
                   "http://leafletjs.com/examples/custom-icons/leaf-green.png",
                   ifelse(quakes1$mag < 6,"http://leafletjs.com/examples/custom-icons/leaf-red.png",
                   "http://leafletjs.com/examples/custom-icons/leaf-orange.png")
  ),
  iconWidth = 38, iconHeight = 95,
  iconAnchorX = 22, iconAnchorY = 94,
  shadowUrl = "http://leafletjs.com/examples/custom-icons/leaf-shadow.png",
  shadowWidth = 50, shadowHeight = 64,
  shadowAnchorX = 4, shadowAnchorY = 62
)

#地图生成
leaflet(data = quakes1) %>% amap() %>%
  addMarkers(~long, ~lat, icon = leafIcons)
```

分为数据生成、图标标记阶段、地图生成阶段。 

小叶子，来自于网络，所以直接输入图片链接即可，iconUrl 是三种颜色的小树，其中shadowUrl 是小树苗的阴影，感觉很赞，iconWidth、shadowWidth 等 其他是一些大小指标。 

leaflet(数据) %>% amap(高德地图) %>%  addMarkers(经纬度+图标)
![这里写图片描述](https://img-blog.csdn.net/20170225173955766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

. 

.
## 4、案例三——地图+点集区域+复选框+NASA星空图

```cpp
#数据生成
geo = data.frame(long = rep(121.44, 1000),
                 lat = rep(31.22, 1000),
                 mag=rep(5,1000))
#mag函数是用来衡量后面，点集范围大小

geo$long = geo$long+rnorm(1000,0,1)
geo$lat = geo$lat+rnorm(1000,0,1)
geo$mag = geo$mag+rnorm(1000,0,1)
outline <- geo[chull(geo$long, geo$lat),]


map <- leaflet(geo) %>%

  # 底层阶段
  # 第一层底图，高德
  amap(group = "高德") %>%                        
  # 第二层底图，黑底图NASA
  addProviderTiles(providers$NASAGIBS.ViirsEarthAtNight2012, group = "黑底") %>%   
  # 第三层底图，白底层
  addProviderTiles(providers$Stamen.TonerLite, group = "白底") %>% 


  # 点集+区域轮廓阶段
  addCircles(~long, ~lat, ~7^mag/50, stroke = F, group = "圈点",color="#97FFFF") %>%          
  # 描点画圈，stroke=T的话，边界包边，丑

  # 绘制轮廓，利用前面的凸集点
  addPolygons(data = outline, lng = ~long, lat = ~lat,
              fill = F, weight = 2, color = "#FFFFCC", group = "轮廓") %>% 

  # 复选框       
  addLayersControl( 
    baseGroups = c("高德", "黑底", "白底"),                     
    #显示层
    overlayGroups = c("圈点", "轮廓"),
    options = layersControlOptions(collapsed = FALSE)
  )
map
```

outline 是数据生成阶段，数据长这样：

```
long      lat      mag
308 124.0785 30.32667 5.294831
885 124.0496 30.20941 3.524118
```

数据
- **底层阶段**

amap和addProviderTiles的两个，都是底图样式（其他底图可见：[leaflet-extras网站](http://leaflet-extras.github.io/leaflet-providers/preview/#)），其中的group是自定义的名称，可以自己命名。
- **点集+区域轮廓阶段**

addCircles是描绘点的，long,lat是经度、维度，stroke是点是否包边，为true很丑；group是自定义的名称，color是圈圈的颜色，[颜色RGB列表](http://www.qiaozhezou.com/rgb.htm)

addPolygons是描绘外围的轮廓的，数据outline是通过求凸集函数chunk求得，lng和lat分别是经度维度。
- **复选框阶段**

addLayersControl加入控件组，baseGroups 是右上角的一个框，overlayGroups 是下面的数字框

![这里写图片描述](https://img-blog.csdn.net/20170226152948575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170225182048363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170225182025379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170225182033169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

延伸：chull凸集函数 

其中要说一下chull求凸集函数：
```
X <- matrix(stats::rnorm(2000), ncol = 2)
 hpts <- chull(X)
 hpts <- c(hpts, hpts[1])
 lines(X[hpts, ])
```

chull可以实现，在点集中求得凸点，然后用lines进行框选。 
![这里写图片描述](https://img-blog.csdn.net/20170225181522454?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





















