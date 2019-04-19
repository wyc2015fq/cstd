# arcgis for js 展示透明图层 - 左直拳的马桶_日用桶 - CSDN博客
2018年05月16日 17:10:22[左直拳](https://me.csdn.net/leftfist)阅读数：1246
arcgis map里面做好的图层，本来是透明的，但在web页面上，用arcgis for js展示，却并非透明。
![这里写图片描述](https://img-blog.csdn.net/20180517133540662?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解决办法之道，是用opacity 或 setImageFormat
```java
var dl = new ArcGISDynamicMapServiceLayer(mapServerPath);
if (dl.loaded) {
    buildSubLayerList(dl);
}
else {
    dojo.connect(dl, "onLoad", buildSubLayerList);
}
function buildSubLayerList(layer) {
    //图层透明度，0完全透明，1不透明
    if (指定了透明度) {
        layer.opacity = 指定的透明度;
    } else {
        layer.setImageFormat("png32", false);//设置图层可以显示默认透明度的格式
    }
    layer.setVisibility(true);
    var arrv = [1,2,3];
    layer.setVisibleLayers(arrv);
}
```
