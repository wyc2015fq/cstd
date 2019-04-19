# 利用OpenStreetMap（OSM）数据搭建一个地图服务 - zhanlijun - 博客园
# [利用OpenStreetMap（OSM）数据搭建一个地图服务](https://www.cnblogs.com/LBSer/p/4451471.html)
[** http://www.cnblogs.com/LBSer/p/4451471.html**](http://www.cnblogs.com/LBSer/p/4451471.html)
![](https://images0.cnblogs.com/blog2015/522490/201504/232245420933382.png)
**图 利用OSM数据简单发布的北京地图服务**
## **一、OSM是什么**
       开放街道图（OpenStreetMap，简称OSM）是一个网上地图协作计划，目标是创造一个内容自由且能让所有人编辑的世界地图（wiki：[http://wiki.openstreetmap.org/wiki/Main_Page](http://wiki.openstreetmap.org/wiki/Main_Page)）。尤其值得称道的是，osm数据开源，可以自由下载使用。
## **二、OSM数据结构**
       OpenStreetMap包括空间数据以及属性数据。其中空间数据主要包括三种：点（Nodes）、路（Ways）和关系（Relations），这三种原始构成了整个地图画面。其中，Nodes定义了空间中点的位置；Ways定义了线或区域；Relations（可选的）定义了元素间的关系。
        属性数据Tags用于描述上述矢量数据基元。（[http://wiki.openstreetmap.org/wiki/Elements](http://wiki.openstreetmap.org/wiki/Elements)）
### 2.1. Node 
       node通过经纬度定义了一个地理坐标点。同时，还可以height=*标示物体所海拔；通过layer=* 和 level=*，可以标示物体所在的地图层面与所在建筑物内的层数；通过place=* and name=*来表示对象的名称。同时，way也是通过多个点（node）连接成线（面）来构成的。
###  2.2. Way   
       通过2-2000个点（nodes）构成了*way。way*可表示如下3种图形事物（非闭合线（**Open polyline **）、闭合线（**Closed polyline**）、区域（**Area **））。对于超过2000 nodes的way，可以通过分割来处理。
  a）Open polyline 
        非闭合线：收尾不闭合的线段。通常可用于表示现实中的道路、河流、铁路等。
  b）Closed polyline 
        闭合线：收尾相连的线。例如可以表示现实中的环线地铁。
c）Area 
        区域：闭合区域。通常使用landuse=* 来标示区域等。 
### 2.3. Relation 
       一个*Relation*是用来描述两个或多个基元的相互关系（nodes, ways 或者其他的relations），相互的关系通过*role*来定义，包括：
       a）route ：定义公路、自行车道、铁路等
       b）多个多边形：定义area例如建筑、河堤等
       c）边界：装门用来定义行政边界
       d）限制：用于描述限制比如“非左转”
### 2.4. Tag 
       标签不是地图基本元素，但是各元素都通过tag来记录数据信息。通过'key' and a 'value'来对数据进行记录（了解xml或者数据库的应该都比较清楚了吧？）。例如，可以通过highway=residential来定义居住区道路；同时，可以使用附加的命名空间来添加附加信息，例如：maxspeed:winter=*就表示冬天的最高限速。
## **三、OSM数据及下载**
osm数据格式主要有以下几种，可以通过以下网站下载数据。
osm数据格式：
OSM XML – xml-format provided by the API
PBF – highly compressed, optimized binary format similar to the API
o5m – for high-speed processing, uses PBF coding, has same structure as XML format
OSMJSON – json variant of OSM XML
osm数据下载网站：
**GeoFabrik：**[**http://www.geofabrik.de/**](http://www.geofabrik.de/)
**Metro Extracts：**[**http://metro.teczno.com/**](http://metro.teczno.com/)
**HOT Exports：**[**http://hot.openstreetmap.org/**](http://hot.openstreetmap.org/)
**BBBike：**[**http://extract.bbbike.org/**](http://extract.bbbike.org/)
## **四、基于OSM数据搭建一个地图服务**
        OSM wiki提供了张架构图，很有价值，我们可以按此架构图一步步进行探索。
![](https://images0.cnblogs.com/blog2015/522490/201504/232246295627745.png)
### **4.1. 数据库**
      Postgresql+插件PostGIS非常适合存储地理空间数据，上面架构图的底层数据库也是此搭配，因此我们也使用此搭配。
      a）下载postgresql+postGIS插件
sudoapt-get install postgresql postgresql-contrib postgis postgresql-9.1-postgis
安装完毕，我们需要更改postgres用户的密码，否则我们就没法使用这个数据库服务器。以postgres这个系统用户的身份运行psql命令，在终端中输入如下：
sudo su postgres -c psql template1
这时候会出现新的提示符，输入下面两个命令，用新密码替换 <***password***>：
ALTER USER postgres WITH PASSWORD ' <***password***> ';
  b）创建用户及数据库
postgres# CREATE USER zhanlijun WITH PASSWORD 'xxxx';
postgres# CREATE DATABASE osm; 
postgres# GRANT ALL PRIVILEGES ON DATABASE osm to zhanlijun;
c）为数据库添加空间扩展
CREATE EXTENSION postgis;
*-- Enable Topology*
CREATE EXTENSION postgis_topology;
*-- fuzzy matching needed for Tiger*
CREATE EXTENSION fuzzystrmatch;
*-- 地理编码*
CREATE EXTENSION postgis_tiger_geocoder;
*-- 用于存储属性tags，key-value*
CREATE EXTENSION hstore;
### **4.2. 导入数据**
      从网站上下载的osm数据一般是文件格式（如xml、pbf等），为了使用我们需要将其导入到数据库中，这就需要导入工具，架构图使用了osmpsis，但是使用起来并不友好，推荐使用osm2pgsql。
      a）安装osm2pgsql
Run sudo apt-get install software-properties-common to install the command add-apt-repository if the command can't be found.
Run sudo add-apt-repository ppa:kakrueger/openstreetmap to add the PPA
Run sudo apt-get update to update your packaging system.
Run sudo apt-get install osm2pgsql to install the osm2pgsql package.
    b）安装protobuf（为了导入pbf格式数据）
sudoapt-get install libprotobuf-c0-dev protobuf-c-compiler
   c）导入数据库
osm2pgsql -s -U zhanlijun -d osm /Users/zhanlijun/Downloads/planet_116.104,39.667_116.892,40.066.osm.pbf -H localhost -W;
注：osm2pgsql导入数据有两种模式， normal and slim mode。
normal mode会在内存中产生如下三张中间表，并在导入结束后丢弃，因此速度较快。
- planet_osm_nodes
- planet_osm_ways
- planet_osm_rels
而slim mode则将中间结果完全放置到数据库中。slim模式的好处是方便更新。
两者使用的区别在于是否加“-s”，加了表示slimmode，本文使用slim mode。
使用slim mode导入数据后在数据库中会产生如下表。
![](https://images0.cnblogs.com/blog2015/522490/201504/232247107182873.png)
### **4.3. 渲染**
       数据已经导入到postgresql了，下面需要将数据渲染出来，也就是将postgresql中的矢量数据渲染成图片。
**4.3.1 使用Mapnik进行渲染**
目前最出名的开源地图渲染引擎莫过于Mapnik。
 Mapnik可以渲染多种数据源，包括数据库如postgresql，以及文件格式数据例如shapefile、osm.xml格式等。
 这里以shapefile格式数据为例，下载需要渲染的矢量数据：[http://www.naturalearthdata.com/](http://www.naturalearthdata.com/)
 1）打开python编辑器
`1 python`
2）导入mapnik python bindings
`1 import mapnik`
3）创建地图
```
1 m = mapnik.Map(600,300) #创建一个地图：宽600 高300
2 #m.srs 是该地图的投影，默认是'+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs'
3 m.background = mapnik.Color('steelblue') #设置背景色
```
4）创建样式
根据我们需求设置样式，样式决定了最终渲染的结果。
```
1 s = mapnik.Style() #style对象
 2 r = mapnik.Rule() #rule对象来管理符号 
 3 #多边形填充符号
 4 polygon_symbolizer = mapnik.PolygonSymbolizer(mapnik.Color('#f2eff9'))
 5 r.symbols.append(polygon_symbolizer)
 6 #多边形边界填充符号
 7 line_symbolizer = mapnik.LineSymbolizer(mapnik.Color('rgb(50%,50%,50%)'),0.1)
 8 r.symbols.append(line_symbolizer)
 9 s.rules.append(r)
10  #将样式加入地图
11 m.append_style('My Style',s)
```
 5）创建数据源
`1 ds=mapnik.Shapefile(file='Users/zhanlijun/Downloads/110m-admin-0-countries/ne_110m_admin_0_countries.shp') `
 6）创建图层
mapnik的layer是数据的基础容器。
```
1 layer = mapnik.Layer('world')#新建一个叫world的图层
2 #layer.srs 默认是'+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs'
3 layer.datasource = ds
4 layer.styles.append('My Style')
```
 7）准备map渲染
```
1 m.layers.append(layer) #将layer加到地图
2 m.zoom_all() #将数据zoom_all，如果不用，结果将是空白
```
 8）渲染地图
最后我们得到一张png格式的世界地图：world.png
![](https://images0.cnblogs.com/blog2015/522490/201504/232247380005438.png)
**4.3.2 使用TileMill进行渲染**
      Mapnik使用起来并不容易，尤其是配置样式的时候，下面我们使用TileMill进行渲染，TileMill的内核是Mapnik。
      TileMill的好处是所见即所得，右边配置样式，左边能马上显示出结果。此外还可以将结果展示出来（下图就是利用TileMill渲染北京osm数据的结果）。
![](https://images0.cnblogs.com/blog2015/522490/201504/232248065937503.png)
### 4.4 一体化解决方案
 GeoServer + OpenLayers +PostGIS叠加显示动态矢量数据。
安装geoserver
sudo apt-get update
sudo apt-get install unzip openjdk-6-jre
echo "export JAVA_HOME=/usr/lib/jvm/java-6-openjdk-amd64" >> ~/.bashrc
source ~/.bashrc
wget -c [http://sourceforge.net/projects/geoserver/files/GeoServer/2.3.5/geoserver-2.3.5-bin.zip](http://sourceforge.net/projects/geoserver/files/GeoServer/2.3.5/geoserver-2.3.5-bin.zip/download)
unzip -a geoserver-2.3.5-bin.zip
cd geoserver-2.3.5/bin
./startup.sh &
访问： [http://localhost:8080/geoserver/](http://localhost:8080/geoserver/)
[](http://192.168.11.134:8080/geoserver/osm/wms?service=WMS&version=1.1.0&request=GetMap&layers=osm&styles=&bbox=1.2924199E7,4817392.000000002,1.3012789999999998E7,4875826.0&width=512&height=337&srs=EPSG:900913&format=application/openlayers)
## **地图相关博文：**
# [地图点聚合优化方案](http://www.cnblogs.com/LBSer/p/4417127.html)

