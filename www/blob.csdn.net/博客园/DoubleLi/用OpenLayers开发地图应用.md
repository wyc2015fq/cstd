# 用OpenLayers开发地图应用 - DoubleLi - 博客园






# 项目背景

最近有一个使用全球地图展示数据的项目，用地图展示数据本身没什么难度，但出于安全和保密的考虑，甲方单位要求项目不能连接外网，只能在内网使用，也就是说，我们不得不在内网中部署一个地图服务器，在这个地图服务器的基础上进行开发。

由于之前没有这方面的技术积累，在经过一番搜素和比较后，发现了几个解决方案。 不过，甲方不愿意再花钱购买任何第三方的软件和服务了，所以，我的解决方案就只能在开源方案中寻找了，商业的解决方案都被排除在外。

# 几个方案

## GeoServer

GeoServer是一个功能齐全,遵循OGC开放标准的开源WFS-T和WMS服务器。利用Geoserver可以把数据作为maps/images来发布(利用WMS来实现)也可以直接发布实际的数据(利用WFS来实现),同时也提供了修改，删除和新增的功能(利用WFS-T)。

GeoServer的安装部署比较简单，按照官方网站的文档就可以了。 不过，由于GIS方面的知识比较欠缺，加上项目工期紧张，一直没有搞定地图的显示。

## OpenLayers

Openlayers是一个开源的Javascript库（基于修改过的BSD许可发布），用来在Web浏览器显示地图。它提供API接口创建类似于Google Maps和Bing Maps的基于web的地理应用。

# 选定OpenLayers+Google地图瓦片

结合项目的需求，由于功能上只是用来显示地图，标注地点而已，用OpenLayers足够满足需求，所以，采用OpenLayers与Google离线地图相结合的解决方案。 下面，详细的记录我安装配置的过程，方便有类似需要的朋友们参考。

## 1. 安装配置web服务器

我选择了nginx作为web服务器。我使用的是Mac OS操作系统，下面的命令适用于Mac OS，使用其他系统的朋友们可以根据自己的实际情况进行操作。


```
brew install nginx
```


注意：修改配置文件 /usr/local/etc/nginx/nginx.conf


```
http {
    include       mime.types;
    default_type  application/octet-stream;
    sendfile      off;
```


sendfile默认是on，改成off

原因：sendfile on #开启高效文件传输模式，sendfile指令指定nginx是否调用sendfile函数来输出文件，对于普通应用设为on，如果用来进行下载等应用磁盘IO重负载应用，可设置为off，以平衡磁盘与网络I/O处理速度，降低系统的负载。注意：如果图片显示不正常把这个改成off。

如果不改成off，图片请求会不正常。

## 2.配置离线地图

打开/usr/local/var/www目录，这是nginx的web根目录，创建gis目录，把下载的google离线地图文件放到这个目录下。 下载openlayers v2.13.1，将其解压后放到/usr/local/var/www目录下。

Google离线地图，可以采用网上的某个工具，这里就不说了，免得有广告嫌疑。大家自己搜索一下就可以找到很多类似的工具。

## 3.创建example.html


```xml
<html>
     <head>
          <title>Google Local Tiles</title>
          <script src="../lib/OpenLayers.js"></script>
          <script type="text/javascript">
               var map, layer; 

               function init() {
               var map = new OpenLayers.Map("map", {
                         maxExtent : new OpenLayers.Bounds(-20037508.3427892,
                             -20037508.3427892, 
                             20037508.3427892, 
                             20037508.3427892),

               numZoomLevels :
```






## 4、访问地图

如果不出意外，访问http://localhost:8080/gis/v2.13/examples/example.html，就可以看到显示的本地地图了。

--EOF-- 
若无特别说明，本站文章均为原创，转载请保留[[链接](http://iamdaiyuan.github.io/)]









