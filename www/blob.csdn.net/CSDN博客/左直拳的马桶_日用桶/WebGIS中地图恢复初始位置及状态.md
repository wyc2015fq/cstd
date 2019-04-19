# WebGIS中地图恢复初始位置及状态 - 左直拳的马桶_日用桶 - CSDN博客
2017年02月09日 18:07:13[左直拳](https://me.csdn.net/leftfist)阅读数：1057
我想实现这么一个效果：地图任意缩放后，点击一个按钮，将立刻回到地图初始加载时的位置，并且是没有缩放的状态。
怎么办呢？最好的办法就是用Home按钮。
![这里写图片描述](https://img-blog.csdn.net/20170209175147788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```xml
<!DOCTYPE HTML>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="initial-scale=1, maximum-scale=1,user-scalable=no">
  <title>Home Extent</title>
  <link rel="stylesheet" href="https://js.arcgis.com/3.19/esri/css/esri.css">
  <style>
    html, body, #map {
      padding:0;
      margin:0;
      height:100%;
    }
    #HomeButton {
      position: absolute;
      top: 95px;
      left: 20px;
      z-index: 50;
    }
  </style>
  <script src="https://js.arcgis.com/3.19/"></script>
  <script>
    require([
      "esri/map", 
      "esri/dijit/HomeButton",
      "dojo/domReady!"
    ], function(
      Map, HomeButton
    )  {
      var map = new Map("map", {
        center: [-56.049, 38.485],
        zoom: 3,
        basemap: "streets"
      });
      var home = new HomeButton({
        map: map
      }, "HomeButton");
      home.startup();
    });
  </script>
</head>
<body>
  <div id="map" class="map">
    <div id="HomeButton"></div>
  </div>
</body>
</html>
```
用别的按钮也不是不可以，麻烦一点：
![这里写图片描述](https://img-blog.csdn.net/20170209180429497?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```java
var mapcenter = new Point([120, 26], new SpatialReference({ wkid:4326 }));
  map = new Map("map", {
    basemap: "delorme",
    center: mapcenter,
    maxZoom: 10,  
    minZoom: 1,  
    zoom: 5,
    extent: bounds,
    logo: false,
    nav: false
  });
  var homeClick = function(){
      map.centerAndZoom(mapcenter,5);  
  }
```
一定要设置basemap，否则这个缩放难以达到效果。
