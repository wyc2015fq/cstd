# arcgis for jsapi中图层数据的主键 - 左直拳的马桶_日用桶 - CSDN博客
2019年03月13日 18:51:26[左直拳](https://me.csdn.net/leftfist)阅读数：50
从地图服务里拿到的图层数据，每条记录都有一个主键，名曰“ObjectID”。
也可以用代码构建图层，比如FeatureLayer。其中可以指定，主键不一定要叫“ObjectID”，叫什么名字都可以，自己随便起。只要数据类型为`type: "esriFieldTypeOID"`。主键的值是系统自动加的，且为自增。注意如果有外部数据源赋给该图层的graphic对象，主键名不要跟其中的字段名相同，否则该字段的值会被覆盖，变成自增。
上代码：
```
var fl = (function(){//得到一个构造的FeatureLayer
	var layerDefinition = {
		"geometryType": "esriGeometryPoint",
		"fields": [{
			"name": "OBJECTID",
			"type": "esriFieldTypeOID",
			"alias": "OBJECTID"
		}]
	};
	var featureCollection = {
		layerDefinition: layerDefinition,
		featureSet: null
	};
	var fl = new FeatureLayer(featureCollection, {
		id: "ship_fl",
		mode: FeatureLayer.MODE_SNAPSHOT,
		outFields: ["*"],
		opacity: 1,
	});
	
	map.addLayer(fl);
	
	return fl;
})();
function setTip(json){//给graphic对象赋值
	var g = new Graphic(point, symbol);
	var st = "";
	for (var a in json) {
	  g.attr(a, json[a]);//设置属性
	  st += a + ": " + json[a] + "<br/>";
	}
	g.infoTemplate = new InfoTemplate("信息", st);
}
```
