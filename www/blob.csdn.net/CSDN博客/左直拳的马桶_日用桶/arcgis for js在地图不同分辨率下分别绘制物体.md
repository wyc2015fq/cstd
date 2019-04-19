# arcgis for js在地图不同分辨率下分别绘制物体 - 左直拳的马桶_日用桶 - CSDN博客
2017年12月09日 18:42:23[左直拳](https://me.csdn.net/leftfist)阅读数：376
有这么一种效果： 
地图在分辨率比较低的情况下，物体图标非常细小、简单； 
而当地图放大到分辨率比较高的时候，物体图标则变成丰满、详细。如图： 
![低分辨率](https://img-blog.csdn.net/20171209181131195?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171209181209012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这不是点聚合，大概叫点云，我在百度、必应上搜，没有搜到相关的教程（啊！我是多么怀念能用谷歌的日子！铁幕铁幕，你为什么还不倒塌？硕鼠硕鼠，无食我屎），仔细想了一下，自己实现了。原理比较简单：
1、低分辨率情况下，用简单图标绘制物体 
2、高分辨率情况下，将图层的graphics清掉重画，用比较精细的图标进行绘制
这里要用到事件：GraphicsLayer.update事件。我的arcgis for js版本是3.19，似乎GraphicsLayer只支持update事件，而不支持update-start和update-end事件。
注意这个update事件，在地图移动，缩放，重画的时候，都会触发。所以要有条件判断，符合缩放到一定比例才进行我们的物体重绘。
```php
var _scale = 0;
var _scalelimit = 600;
var _records = [...];//物体数据，数组
layer.on("update", function (info) {
    var sc = parseInt(map.extent.getWidth() / map.width);//关键判断依据
    if (_scale == 0) {
        _scale = sc;
        return;
    }
    if (sc != _scale) {
        if (_scale <= _scalelimit && sc > _scalelimit) {//缩小
            _drawpoint(layer, _records);//简单画
        } else if (_scale > _scalelimit && sc <= _scalelimit) {//放大
            _draw(layer, _records);//精细画
        }
        _scale = sc;
    }
});
function _draw(layer, records) {
    layer.clear();
    records.forEach(function (r) {
        //细细的画
    });
}
function _drawpoint(layer, records) {
    layer.clear();
    records.forEach(function (r) {
        layer.add(point(r));
    });
}
```
另外，FeatureLayer是GraphicLayer的子类，但纯粹绘图，应该用GraphicLayer，为啥呢，因为FeatureLayer一定要有一个图层服务地址，而GraphicLayer则不需要（此说法有误，因为FeatureLayer也可以通过参数定义进行构造，不一定要有一个图层服务地址：
```
var featureLayer = new FeatureLayer(featureCollection, {
    showLabels: true
  });
```
）。
