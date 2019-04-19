# arcgis for jsapi 4.10绘制热力图 - 左直拳的马桶_日用桶 - CSDN博客
2019年02月13日 18:03:09[左直拳](https://me.csdn.net/leftfist)阅读数：184
arcgis for jsapi 从 3 变到 4 之后，变化巨大，原先网络上的许多例子，好像都过时了。比如echart与arcgis结合，都是for 3.x的,4时代就不行了。
热力图也一样。
现在俺给个例子，不依赖于arcgis服务器，直接渲染出来：
原理是绘制，绘制在FeatureLayer里。怎么绘制呢？原来热力图有个专门的渲染器：HeatmapRenderer，将样式赋给该渲染器，然后渲染器、点数据等参与FeatureLayer的构造，就可以了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213180204230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
```
<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
	<meta name="viewport" content="initial-scale=1, maximum-scale=1,user-scalable=no"/>
	<title>heatMap</title>
	<link rel="stylesheet" type="text/css" href="http://192.168.0.211/pubzy/arcgis_js_api/4.10/esri/css/main.css" />
	<style>
		html, body, #map {
		  height: 100%;
		  margin: 0;
		  padding: 0;
		}
	</style>
	<script src="http://192.168.0.211/pubzy/media/js/jquery-1.11.1.min.js" type="text/javascript"></script>
	<script src="http://192.168.0.211/pubzy/arcgis_js_api/4.10/init.js"></script>
</head>
<body>
	<div id="map"></div>
</body>
<script>
 require(["esri/map",
	"esri/views/MapView",
	"esri/Basemap",
	"esri/geometry/Extent",		  
	"esri/layers/TileLayer",
	"esri/layers/FeatureLayer",
	"esri/renderers/HeatmapRenderer", 
	"dojo/domReady!"], function (Map,
MapView,
BaseMap,
Extent,
TileLayer,  
FeatureLayer,
HeatmapRenderer) {
  
  	let map, view;
	(function () {//加载底图等
		map = new Map({
			basemap: getBaseMap({
				"title": "影像图",
				"thumbnailUrl": "images/map/影像图.gif",
				"type": "tile",
				"layer": "http://192.168.0.206:6080/arcgis/rest/services/HNJCZS/HNJCZSYXT/MapServer"
			  }),
		});
		view = new MapView({
			container: "map",
			map: map,
			extent: new Extent({
			  "xmin": 105,
			  "ymin": 18,
			  "xmax": 115,
			  "ymax": 20,
			  "spatialReference": { "wkid": 4326 }
			}),
		});
		view.ui._removeComponents(["attribution"]);
	})();	  
	function getBaseMap(basemapcof) {
		let baselayer = new TileLayer({
			url: basemapcof.layer,
		});
		return new BaseMap({
			baseLayers: baselayer,
			title: basemapcof.title,
			thumbnailUrl: "../" + basemapcof.thumbnailUrl,
		});
	}
	
    var heatmapRenderer = new HeatmapRenderer({//设置渲染器
      colorStops: [{ ratio: 0.4, color: "rgba(0, 255, 0, 0)" },
	    { ratio: 0.75, color: "rgba(255, 140, 0, 1)" },
		{ ratio: 0.9, color: "rgba(255, 0,   0, 1)" }],
		blurRadius: 8,
		maxPixelIntensity: 230,
		minPixelIntensity: 10
    });
    var heatDataUrl = "data/heatJson.json";
    $.getJSON(heatDataUrl,{},function(data,textStatus,jqXHR){//读取数据，设置点数据集
		var features = [];
		for (var i = 0; i < data.heatData.length; i++) {
			var x = data.heatData[i].lng;
			var y = data.heatData[i].lat;
			features.push({
				geometry: {
					type: "point",
					x: x,
					y: y
				},
				attributes: {
					ObjectID: i,//重要！！！
				}
			});
		}
	  
		var featureLayer = new FeatureLayer({
			  source: features,//点数据集
			  title: "热力图",
			  objectIdField: "ObjectID",//重要！！！
			  renderer: heatmapRenderer//渲染器
		});
		
		map.add(featureLayer);
    });
});
</script>
</html>
```
点数据很简单，JSON格式，里面只有经纬度
```
{"heatData":[{"lng":"108.51293787947446","lat":"18.899562096488552"},{"lng":"108.38702921054004","lat":"19.151761337656723"},。。。。
```
可以从参考资料里下载
[https://lxqjss.github.io/data/heatJson.json](https://lxqjss.github.io/data/heatJson.json)
不过这个位置是杭州的，我改为海南了。海南的可从这里下载：
[https://download.csdn.net/download/leftfist/10955939。](https://download.csdn.net/download/leftfist/10955939%E3%80%82)
这下载分是CSDN自动设置的，我的话，一般只设为1。老实说，CSDN好多东西都做的很烂。
热力图有个专有的名词：HeatMap，可以用这个单词去搜。
参考资料：
[arcgis js api:热力图](https://www.jianshu.com/p/3b36a05872e2)
arcgis for jsapi官网上有两个例子，一个似乎是直接展示图层，一个是依赖于CSVLayer，好像挺复杂。
[https://developers.arcgis.com/javascript/latest/sample-code/visualization-heatmap-scale/index.html](https://developers.arcgis.com/javascript/latest/sample-code/visualization-heatmap-scale/index.html)
[https://developers.arcgis.com/javascript/latest/sample-code/visualization-heatmap/index.html](https://developers.arcgis.com/javascript/latest/sample-code/visualization-heatmap/index.html)
