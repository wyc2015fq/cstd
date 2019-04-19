# arcgis for js叠加栅格图层(Raster Layer) - 左直拳的马桶_日用桶 - CSDN博客
2017年11月21日 20:10:44[左直拳](https://me.csdn.net/leftfist)阅读数：1971
其实呢，这个问题或许与栅格图层（Raster Layer）关系并不十分大，也可能所有类型的图层都有这个问题，我对arcgis尚不十分了解，目前只在栅格图层遇到问题，所以先记下来，存疑。
栅格图层啥问题呢？叠不出来，报错。
比如，有这么个地图服务： 
![这里写图片描述](https://img-blog.csdn.net/20171121193214203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后想直接叠加其中一个子图层（可见其类型为栅格图层）。
![这里写图片描述](https://img-blog.csdn.net/20171121193246250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码如下：
```
var layer = new ArcGISDynamicMapServiceLayer("http://192.168.0.225:6080/arcgis/HN_test/MapServer/1");
map.addLayer(layer);
```
但这样必报错。报”400”错误，错误提示为：不支持的输出格式。
```
Error: Output format not supported.
Code: 400
```
啥格式？就是 image 格式。arcgis图层服务输出的格式有许多种，[详见这里](http://resources.arcgis.com/en/help/arcgis-rest-api/index.html#//02r3000000wq000000)
但是，如果只访问图层服务的根地址，则图层可以出来：
```
var layer = new ArcGISDynamicMapServiceLayer("http://192.168.0.225:6080/arcgis/HN_test/MapServer");//<----------------- 后面没有“/1”
map.addLayer(layer);
```
![这里写图片描述](https://img-blog.csdn.net/20171121194719627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
真奇怪啊，我叠FeatureLayer的时候，都是直接访问子图层，从来不见有什么问题。为什么这里却不行，无论是用
```
ArcGISDynamicMapServiceLayer,
ArcGISImageServiceLayer,
RasterLayer
```
还是
```
ArcGISTiledMapServiceLayer,
FeatureLayer
```
都不行。前三个直接报400错，后两个没报错，但图层根本出不来。
难道栅格图层这么特别？
从周六开始，到今天周二，每天都花了时间去搞这个问题，问了两个高手，但高手没空，有一个其实是提到问题答案了，但我没理解，还是搞不出来。这种问题，到百度上搜，根本无从下手，没有啥代码提到栅格图层的（这也证明其实栅格图层并不需要用什么特别的对象来处理）。
后来偶尔看到网上[有篇文章](https://www.cnblogs.com/mengfanrong/p/5213887.html)提到访问地图服务，控制子图层，我想起年初刚开始搞arcgis时，自己好像也抄过类似的代码，用于显示或隐藏不同的图层。翻出来一看，果然啊，图层对象有个layerInfos的属性，这里面就是其所有子图层的列表。所有，不仅仅是儿子，还包括孙子及后代。但这个layerInfos加载需要一定的过程，一般在图层加载完毕的事件中才开始访问。
修改代码，图像终于出来了。
![这里写图片描述](https://img-blog.csdn.net/20171121194616817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
处理过程如下：
1、构建动态切片图层对象
`var layer = new ArcGISDynamicMapServiceLayer("http://192.168.0.225:6080/arcgis/rest/services/HN_test/MapServer");`
2、该图层对象加载完毕后设置子图层可见属性
```java
if (layer.loaded) {
    buildSubLayerList(layer);
}
else {
    dojo.connect(layer, "onLoad", buildSubLayerList);
} 
function buildSubLayerList(layer) {
    var visible = [];
    dojo.map(layer.layerInfos,function (info, index) {
        if(符合条件) visible.push(info.id);
    });
    map.addLayer(layer);//万事俱备，至此方才将图层添加到地图
}
```
小可有个疑问，是不是可以这样认为，凡切片图层，影像图层，都是这样处理：
> 
只能访问地图服务，然后再遍历其中的子图层，控制其显隐来达到好像能加载特定图层的效果？
