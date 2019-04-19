# javascript采用依赖注入维护公用页面的纯净性 - 左直拳的马桶_日用桶 - CSDN博客
2019年02月13日 15:39:22[左直拳](https://me.csdn.net/leftfist)阅读数：113
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
我描述一下背景：
我们做的一个BS项目，有一个GIS页面，是公用的。基本上，所有模块，所有功能，都或多或少要用到地图，将自己的结果展示在地图上，甚至与地图交互。类似这样（船讯网不是我弄的，下图仅仅是个示意）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213141509450.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
中间这个地图页面，实际上是一个独立的iframe，原因是要保持其独立性和纯洁性，避免被其他页面的样式、JS所污染。
那么就有一个问题，这些需要与地图交互的模块的代码，写在什么地方？因为arcgis for jsapi，有一定的封闭性。
有2种方式：
1）统一写在地图页面，方便地图调用
2）写在每个模块自己的页面，地图页面暴露一些接口，每个模块将自己的具体实例注入，由地图统一调用
我认为首选应该是方式二。理由如下：
> 
1）维持地图页面的代码精简
2）避免资源浪费
每个模块的代码，基本上别的模块用不着，如果都堆在地图页面，没有必要，还会拖慢加载的速度。
如何实现方式二？
**一、在GIS页面提供一个方法，让其他模块能注入实例**
```
var map;
var iMap = {};//外部接口
require(["esri/map",
 "dojo/_base/array",
 "dojo/domReady!"], function (Map,arrayUtils) {
	
	(function(){//加载底图等
		map = new Map({
			basemap: 'streets',
		});
	})();
	/* 外部接口 by chenqu */
	iMap.drawHandler = new (function(){//绘制接口
		/*
		外部绘制插件，应该实现两个接口：
		draw(data, option),绘制
		clear(),擦除
		*/
		this.push = function(mode,bean){//注入外部插件
			if(find(mode) == null) {
				beans.push({
					mode: mode,
					bean: bean
				});
			}
		};
		this.draw = function (data, option) {//绘制
			var beanJ = find(option.mode);
			beanJ.bean.draw(data,option);
		};
		this.clear = function(mode){//清除
			if(typeof mode != 'undefined'){//擦除特定插件的绘制
				var beanJ = find(mode);
				beanJ.bean.clear();
			} else {//全部擦除
				for(var i in beans){
					beans[i].bean.clear();
				}
			}
		};
		/* 私有部分 */
		let beans = [];//{mode,bean}
		function find(mode) {
			if (beans.find) {//支持数组 find 方法
				return beans.find(function (b) {
					return b.mode == mode;
				});
			} else {
				var newArr = arrayUtils.filter(beans, function (t) {
					return t.mode == mode;
				});
				return (newArr.length > 0) ? newArr[0] : undefined;
			}
		}
	})();
});
```
很简单，原理主要是地图页面这里，提供了一个方法：iMap.drawHanlder.push，可以让其他模块将实例注入到内部数组beans中。
当然为了实现这个注入，会有一些辅助的函数，例如查找数组元素，避免重复注入之类。
**二、其他模块与地图的交互**
```
let mapWin = getMapWin();//自定义函数，获得底图页面，实现代码略过不提
let require = mapWin.require;
require(["plugin/HeatMap"],function(HeatMap){
	var handler = mapWin.iMap.drawHandler;
	handler.push("heatmap",new HeatMap(mapWin.map));//注入某个插件。这个插件代码略过不提
	handler.draw(data,	{//绘制
		mode: 'heatmap', node: {}
	});
});
```
arcgis for jsapi一般都用AMD这种模块化开发方式，各个模块可以利用地图页面的require：
```
let mapWin = getMapWin();//自定义函数，获得底图页面，实现代码略过不提
let require = mapWin.require;
require(["plugin/HeatMap"],function(HeatMap){
}
```
依赖倒置是设计模式的基础，但本身应该不是设计模式。我实在不知道怎么描述这种开发方式，就拟了上面这个标题。
