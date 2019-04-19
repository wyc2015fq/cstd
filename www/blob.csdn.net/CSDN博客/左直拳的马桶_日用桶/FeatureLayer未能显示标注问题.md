# FeatureLayer未能显示标注问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年11月10日 18:33:55[左直拳](https://me.csdn.net/leftfist)阅读数：1036
同事告诉我，我在web页面上叠加的他们制作的图层，没有显示相应的标注。
“标注？啥东东？”，我一头雾水。原来，图层中，一些对象，比如色块、小图标，往往会有相关的文字加以说明，这就是所谓的标注。在arcgis中，应该叫“label”。
![这里写图片描述](https://img-blog.csdn.net/20171110175855374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
“你看，我打开【要素标注】选项，就有文字，关闭，则消失”，同事在arcgis里娴熟地操作着，文字忽隐忽现，刺痛了俺的眼睛。为何我叠加的图层没有出现这些相关文字？
有另外一个同事写的WEB页面就有标注。不过，他采用`new ArcGISDynamicMapServiceLayer` 的方式来加载图层，而我用的是 `new FeatureLayer` 的方式。众所周知，前者是所谓动态[图层](http://blog.csdn.net/leftfist/article/details/71424325)，与切片图层一样，地图是从服务器端返回图片的形式展示在客户端；而FeatureLayer属于图形图层，客户端从服务器端拿到的是数据，然后再绘制出来。一般而言，动态图层和切片图层可用于底图，而FeatureLayer用于图层叠加。
所以，该同事的图层能显示标注不足为奇。那么，FeatureLayer就没办法显示标注了吗？网上疯狂搜索，见到的答案，基本都是[添加一个标注](http://blog.csdn.net/qq503690160/article/details/72385481)，自己设内容、字体、颜色。我想，这适合图层本身没有标注，而对于图层已经有了标注的情况下，为什么我们不能够直接显示出来呢？
查看FeatureLayer的构造函数，有参数名曰：showLabels，这个就跟标注有关，默认为false，设为true不就可以了吗？
```
var fl = new FeatureLayer(node.nodePath, {
    mode: FeatureLayer.MODE_ONDEMAND,
    outFields: ["*"],
    showLabels: true
});
```
结果还是没有显示。原来，光在图层这里设置还不够，map本身也要支持才行：
```
return new Map("map", {
    basemap: "delorme",
    center: mapcenter,
    maxZoom: 15,
    minZoom: 5,
    zoom: _zoomdefault,
    extent: bounds,
    logo: false,
    nav: false,
    showLabels: true,//<----------------------
    sliderPosition: "top-right"
});
```
如此，标注显示矣。地图、图层都要将这个showLables设为true，缺一不可。
另外，标注是绑定某个字段的，所以，叠加图层的时候，outFields 一定要包含该字段，否则，标注将显示“undefined”。
有些图层未必有标注。这个直接浏览图层服务地址就能看出来： 
![这里写图片描述](https://img-blog.csdn.net/20171110183252864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
