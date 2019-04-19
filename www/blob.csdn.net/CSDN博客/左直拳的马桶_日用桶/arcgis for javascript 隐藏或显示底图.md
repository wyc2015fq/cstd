# arcgis for javascript 隐藏或显示底图 - 左直拳的马桶_日用桶 - CSDN博客
2017年04月25日 17:01:13[左直拳](https://me.csdn.net/leftfist)阅读数：2809
arcgis for javascript展示地图的时候，有图层的概念，一层一层的，类似photoshop，在应用界面上控制图层的显隐，是极常见的功能。
但是，如果是控制底图的显示或隐藏呢，怎么搞？
网上找不到满意的答案。中文的一篇类似答案都没有，英文的都是说new Map的时候，别指定底图，然后再采用add Layer的办法添加上去。我明白，add Layer的时候，可以获得这个layer对象，然后对它进行可见性设置，即可控制矣。然而，构造Map的时候，如果不指定basemap，无论定位，中心点，缩放这些似乎都不好控制。
那么，有没有办法在指定底图，正常构造Map之后，再来控制这个底图的可见性呢？
答案是肯定的，代码如下：
```java
var bounds = new Extent({
    "xmin": 108,
    "ymin": 15,
    "xmax": 125,
    "ymax": 38,
    "spatialReference": { "wkid": 4326 }
});
var mapcenter = new Point([121.8, 28.2], new SpatialReference({ wkid: 4326 }));
esriBasemaps.delorme = {
    baseMapLayers: [{ url: 底图地址 }],
    thumbnailUrl: 影像图地址,
    title: "矢量图",
};
var map = new Map("map", {
    basemap: "delorme",//指明底图
    center: mapcenter,
    maxZoom: 15,
    minZoom: 5,
    zoom: 8,
    extent: bounds,
    logo: false,
    nav: false
});
$("#cheBasemap").click(function(){//cheBasemap 是一个复选框,用来控制底图的显示或隐藏
    var id = map.basemapLayerIds[0];//拿到底图ID
    map.getLayer(id).setVisibility(document.getElementById("cheBasemap").checked);//然后凭ID获得底图图层，进而控制可见性
});
```
事实证明，底图也是一个图层而已，看上去可以像控制其他图层那样来控制它。
