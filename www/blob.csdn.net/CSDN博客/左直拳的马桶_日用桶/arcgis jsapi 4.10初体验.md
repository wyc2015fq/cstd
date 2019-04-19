# arcgis jsapi 4.10初体验 - 左直拳的马桶_日用桶 - CSDN博客
2019年01月04日 18:59:04[左直拳](https://me.csdn.net/leftfist)阅读数：272
没想到第1000篇文章是有关GIS的。鸡毛蒜皮，一点也不够大气。
体验就是坑。
现在项目前端用到的arcgis jsapi指明要用4.10了。从3.19直接跳跃到4.10。结果就是各种不习惯，各种坑。我去，这变化也忒大了吧。虽然是同一家公司，但一定是另一拨程序员搞的，各种看前任不顺眼，认为3点几是垃圾，于是重新设计，重新开发，结果就是另一坨伟大的垃圾产生了。
记录一下各种变化/坑。
**1、加载底图，并去掉arcgis logo或标识**
```
let baselayer = new TileLayer({
	url: 底图地址,
});
let basemap = new BaseMap({
	baseLayers: baselayer,
	title: 标题,
	thumbnailUrl: 缩略图地址,
});
let map = new Map({
	basemap: basemap,
});
let view = new MapView({
	container: "map",
	map: map,
	extent: new Extent({
		"xmin": 100,
        "ymin": 20,
        "xmax": 120,
        "ymax": 40,
        "spatialReference": { "wkid": 4326 }	
	}),
});
view.ui._removeComponents(["attribution"]);//去掉arcgis的logo或标识
```
一大坨的代码，只为了加载一张底图。就问你爽不爽。
**2、各种零碎的小工具**
```
//放大和缩小按钮
//默认是在左上角（老美多左撇子？）
view.ui.move("zoom", "top-right");//move？我在怀疑我的智商。原先只作为一个属性设置
//home button
var homeWidget = new Home({
	view: view
});
view.ui.add(homeWidget, "top-right");
//底图切换
let baselayer = new TileLayer({
	url: 底图地址,
});
let basemap = new BaseMap({
	baseLayers: baselayer,
	title: 标题,
	thumbnailUrl: 缩略图地址,
});
var toggle = new BasemapToggle({
	view: view,
	nextBasemap: basemap,
});
view.ui.add(toggle, "top-right");
```
不过呢，大家都用到右上角，arcgis会将它们从上到下依次排列好。
**3、PopupTemplate展示所有字段**
```
new PopupTemplate({
    "title": "详细信息",
    "content": "{*}",
 })
```
