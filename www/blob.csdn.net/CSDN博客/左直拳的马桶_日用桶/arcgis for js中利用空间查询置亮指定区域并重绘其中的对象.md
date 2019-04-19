# arcgis for js中利用空间查询置亮指定区域并重绘其中的对象 - 左直拳的马桶_日用桶 - CSDN博客
2018年09月01日 12:33:31[左直拳](https://me.csdn.net/leftfist)阅读数：281
arcgis for js 3.19
如图所示，置亮指定区域，并重绘区域内各种对象的图标。
![这里写图片描述](https://img-blog.csdn.net/20180901105840631?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
紫色、橙色为各种对象的原色；指定区域被置亮为绿色，区域内的对象也被重绘为绿色
用到的处理方法：
> 
1、获取指定区域的空间信息
2、填充指定区域
3、获取指定区域内的对象
4、调整图层顺序
5、重绘对象
**一、总体代码结构**
```java
require(["toollib/query.Utils",
	"drawlib/DrawObj",
	"esri/symbols/PictureMarkerSymbol",
	"esri/symbols/PictureFillSymbol",
	"esri/symbols/SimpleLineSymbol",
	"dojo/_base/Color",], function (QueryUtils, DrawObj, PictureMarkerSymbol, PictureFillSymbol, SimpleLineSymbol, Color) {
	
	var queryUtil = new QueryUtils();//自定义查找类
	var drawObj = new DrawObj(map, "");//自定义绘画类
	var url = "http://192.168.0.140:6080/arcgis/rest/services/YJYB/HYFB/MapServer/5";//区域图层路径
	queryUtil.executeQuery({
		url: url,
		where: "BM=" + areacode,//查找指定区域
		returnGeometry: true,//返回空间信息
	}, function (data) {
		if (data.features.length > 0) {
			//在新图层再画一个指定区域
			//data.features[0].geometry是一个空间信息
			drawObj.draw2("hl_base_zh", data.features[0].geometry);
			
			//重绘指定区域内的对象
			。。。。
		}
	});
				
});
```
下面分解说明一下代码各个部分
**二、详细说明**
**1、自定义查找类**
```
//自定义查找类query.Utils
define([
    "dojo/_base/declare",
    "esri/tasks/QueryTask",
    "esri/tasks/query",
], function (declare, QueryTask, Query) {
    return declare("query.Utils", [], {
        constructor: function () {
        },
        executeQuery: function (opt, callback) {
            var query = new Query();
            var task = new QueryTask(opt.url);
            if (opt.where) {
                query.where = opt.where;
            }
            if (opt.outFields) {
                query.outFields = opt.outFields;
            }
            else {
                query.outFields = ["*"];
            }
            query.returnGeometry = (typeof opt.returnGeometry != 'undefined')
                ? opt.returnGeometry
                : false;
            task.execute(query, callback);
        }
    });
});
```
**2、绘制指定区域**
这个绘制，是新建一个图形图层graphicLayer，在上面将获取到的空间信息再绘制一遍，相当于复制了一个指定区域。
```
var layer = new GraphicsLayer({ "id": "draw_" + id });
var symbol = new SimpleFillSymbol({
	"type": "esriSFS",
	"style": "esriSFSSolid",
	"color": [0, 153, 102, 130],
	"outline": {
		"type": "esriSLS",
		"style": "esriSLSSolid",
		"color": [110, 110, 110, 255],
		"width": 1
	}
});
//geometry就是获取到的空间信息
//对应前面的data.features[0].geometry
layer.add(new Graphic(geometry, symbol));
```
**3、重绘区域内的对象**
要重绘，必须先找出区域内都有哪些对象。
首先，展示这些对象的图层，必须是要素图层（FeatureLayer）。动态图层不行，动态图层没办法读出里面的graphics。所以为了这个重绘效果，我是用了要素图层来加载展示对象的图层。
至此，我对动态图层和要素图层的区别，又有了深一层的理解。
```
var symbol = new PictureMarkerSymbol({
	"url": "green1.png",
	"height": 16.5,//图片尺寸的0.75
	"width": 16.5,
	"type": "esriPMS"
});
var graphics = layer.graphics;//layer是展示对象的要素图层
for (var i = 0, total = graphics.length; i < total; i++) {
	if (geometry.contains(graphics[i].geometry)) {
		graphics[i].symbol = symbol;//重绘。用新symbol替换了旧symbol
	}
}
layer.redraw();
```
**4、重绘持久化**
上面的重绘还不够，地图一缩放就没了。所以还要绑定事件：
```
var oldsc = -1;
layer.on("update-end", function (info) {
	var sc = parseInt(map.extent.getWidth() / map.width);//关键判断依据
	if (oldsc == -1 || sc != oldsc) {
		oldsc = sc;
		//重绘
		.....
	}
});
```
地图比例缩放，就触发重绘
**5、调整图层顺序**
图层是有顺序的。并且分组，每组里面又有顺序。
从上到下，分为3组，依次是图形图层、动态图层、底图；3组的顺序似乎不能调整；地图对象获取图层ID，都分为3个数组：
```
graphicsLayerIds
layerIds
basemapLayerIds
```
然后每组里面的图层又有顺序。我们所说的调整图层顺序，实际上指的是调整分组里面的图层顺序。由于我在加载图层的时候，先加载了展示各种对象的图层，然后再置亮指定区域，就造成置亮后，对象图标被埋在置亮图层下面。所以应当将图标所在图层调到上面。
```
//layer是对象所在要素图层,调到最顶部
map.reorderLayer(layer, map.graphicsLayerIds.length - 1);
```
