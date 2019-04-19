# arcgis for js 展示地图图例 - 左直拳的马桶_日用桶 - CSDN博客
2018年07月14日 17:55:31[左直拳](https://me.csdn.net/leftfist)阅读数：1162
arcgis地图展示过程中，展示图例是很平常的。问题是，要怎么展示。
一般来说，只要给个图例控件就搞定了。
但是，像动态图层（ArcGISDynamicMapServiceLayer）的图层展示，都是整个服务加载，然后指定一些图层是否可见，采用这种方式来展示这些图层。
一个动态图层服务往往有许多子图层，子子孙孙，无穷匮也，是个树形结构。如果将整个图层服务都塞给图例控件，那出来的图例也是个树形结构。
```java
var dynamicLayer = new ArcGISDynamicMapServiceLayer(mapServer, { id: serverName });
//legend是图例控件
legend.layerInfos.push({
    layer: dynamicLayer,//整个图层服务塞给图例控件
    title: " ",
});
legend.refresh();
```
通常，图例控件都放在地图中的某一角落，很小的一块，内容为树形结构很占地方；而且图层的树形结构，好多层是没有图例的，展示在图例控件中空白一片，非常不美观。
我解决的办法是并不将动态图层塞给图例控件，而是显示哪些图层，就展示哪些图层的图例。
```java
var fl1 = new FeatureLayer(layerUrl1, {});//要素图层（FeatureLayer）可以单独加载某个图层
var fl2 = new FeatureLayer(layerUrl2, {});
legend.layerInfos.push({
    layer: fl1,
    title: " ",
    id: id,
});
legend.layerInfos.push({
    layer: fl2,
    title: " ",
    id: id,
});
legend.refresh();
```
这样子之后，图例就不再是树形结构，而是一个个并列的结构，比较美观，也符合人们的使用习惯。 
![这里写图片描述](https://img-blog.csdn.net/2018071417551376?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
