# 装扮装扮dijit.Tree - 沧海一粟 —— 技术随手记 - CSDN博客





2013年11月01日 20:28:42[慢游](https://me.csdn.net/eengel)阅读数：2239








# 介绍

dijit.Tree（Dojo 1.7.3）是Dojo的树形UI控件。根据<body>上设的dojo theme不同，可以有几种不同的标准风格（claro，tundra，nihilo和soria）。但要实现自己的风格，还需要用点手段“装扮装扮”。

# 创建一棵树

首先创建一棵树，应用tundra作为标准风格。网页上显示的树如下。很中规中矩的一棵树。可以看到，这棵树是关于“食物”的。假设这样一个界面是某个幼儿园公布小朋友食物种类的网页。那为了让这些信息更生动，势必需要再“装扮装扮”。


![](https://img-blog.csdn.net/20131101202611890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## CSS代码

```
<style type="text/css">
@import "../js/dijit/themes/tundra/tundra.css";
</style>
<style>
	html,body {
		width: 100%;
		height: 100%;
		margin: 0;
	}
	
	body{
		margin:0;
		font-family: Arial, Helvetica, sans-serif;
		font-size:0.8em;
	}
</style>
```

## HTML代码

```
<div style="width: 100%; height: 100%"
	data-dojo-type="dijit.layout.BorderContainer"
	data-dojo-props="design:'sidebar', gutters:true, liveSplitters:true">
	<div data-dojo-type="dojox.layout.ContentPane" data-dojo-props="region:'left',splitter:true" 
		style="width:180px;">
		<div id="tree_1"></div>
	</div>
</div>
```

## JavaScript代码

```java
<script>
		dojoConfig = {
			isDebug : false,
			parseOnLoad : true,
			async : true
		};
	</script>
	<script type="text/javascript" src="../js/dojo/dojo.js"></script>
	<script>
		require(["dijit/layout/BorderContainer",
		         "dojox/layout/ContentPane" ]);
	</script>
	<script>
require(
				[ "dojo/ready", 
				  "dijit/registry",
				  "dojo/_base/array", 
				  "dojo/on",
				  "dojo/dom",
				  "dojo/query",
				  "dojo/data/ItemFileWriteStore", 
				  "dojo/store/Observable",
				  "dijit/tree/ForestStoreModel", 
				  "dijit/Tree"
				  ],
				function(ready, registry, array, on, dom, query, 
						ItemFileWriteStore, Observable, ForestStoreModel,
						Tree) {
					ready(function() {
						//tree 1 at left
						var data_1 = {
								identifier: 'id',
								label: 'name',
								items: [
									{ 
										id: '0', 
										name:'Foods', 
										type: 'root',
										children:[ 
										           {_reference: '1'},  
										           {_reference: '2'},  
										           {_reference: '3'}
										] 
									},
									{ 
										id: '1', 
										name:'Fruits', 
										type: 'class', 
										children:[ 
										           {_reference: '5'},
										           {_reference: '6'}
										] 
									},
									{ 
										id: '5', 
										name:'Orange',
										type:"instance"
									},
									{ 
										id: '6', 
										name:'Apple',
										type:"instance"
									},
									{ 
										id: '2', 
										name:'Vegetables', 
										type: 'class'
									},
									{ 
										id: '3', 
										name:'Cereals', 
										type: 'class', 
										children:[ 
										           {_reference: '4'}
										] 
									},
									{ 
										id: '4', 
										name:'Rice cereal',
										type:"instance"
									}
								]
						};
						
						var store_1 = new ItemFileWriteStore({
							data : data_1
						});
						
						store_1 = new Observable(store_1);
						
						var model_1 = new ForestStoreModel({
							store : store_1,
							query : {
								type : 'root'
							}
						});
						
						var tree_1 = new Tree({
							id : "tree_1",
							model : model_1,
							showRoot : false,
							persist : true
						}, "tree_1");
					});
				});
	</script>
```






# 更改节点图标

首先，不同的食物种类采用不同的图标。小朋友一眼就可以看出是水果还是别的吃的。


![](https://img-blog.csdn.net/20131101202626109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## CSS代码

```
<style>
	.Apple_icon {
		background: url("../img/food/Apple.png") no-repeat;
	}
	
	.Cereals_icon {
		background: url("../img/food/Cereals.png") no-repeat;
	}
	
	.Foods_icon {
		background: url("../img/food/Foods.png") no-repeat;
	}
	
	.Fruits_icon {
		background: url("../img/food/Fruits.png") no-repeat;
	}
	
	.Orange_icon {
		background: url("../img/food/Orange.png") no-repeat;
	}
	
	.Others_icon {
		background: url("../img/food/Others.png") no-repeat;
	}
	
	.Ricecereal_icon {
		background: url("../img/food/Rice cereal.png") no-repeat;
	}
	
	.Vegetables_icon {
		background: url("../img/food/Vegetables.png") no-repeat;
	}
</style>
```

## JavaScript代码

```java
var tree_1 = new Tree({
							id : "tree_1",
							model : model_1,
							showRoot : false,
							persist : true,
							/*更改节点图标*/
							getIconClass : function(item){
								if (!item.root) {
									var name = this.model.store.getValue(item, "name");
									return name.replace(/ /g, "") + "_icon";
								}
								return "Others_icon";
							}
						}, "tree_1");
```






# 更改节点文字样式

文字也可以变得丰富多彩。


![](https://img-blog.csdn.net/20131101202632562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## CSS代码

```
<style>	
	.root_label {
		color: #6666FF;
		font-size: 14px;
	}
	
	.class_label {
		color: #00CC99;
		font-size: 13px;
	}
	
	.instance_label {
		color: #FF9900;
		font-size: 12px;
	}
</style>
```

## JavaScript代码

```java
var tree_1 = new Tree({
							id : "tree_1",
							model : model_1,
							showRoot : false,
							persist : true,
							getIconClass : function(item){
								if (!item.root) {
									var name = this.model.store.getValue(item, "name");
									return name.replace(/ /g, "") + "_icon";
								}
								return "Others_icon";
							},
							/*更改节点文字样式*/
							getLabelClass : function(item){
								if (!item.root) {
									var type = this.model.store.getValue(item, "type");
									return type + "_label";
								}
							}
						}, "tree_1");
```





# 更改折叠图标

折叠图标也换换。


![](https://img-blog.csdn.net/20131101202637921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## CSS代码

```
<style>		
	.tundra .dijitTreeExpandoOpened {
	    background-image: url("../img/food/close.png") !important;
	}
	
	.tundra .dijitTreeExpandoClosed {
	    background-image: url("../img/food/open.png") !important;
	}
</style>
```


# 更改hover颜色

鼠标挪到某样食物上面的时候，改用可爱的粉红色高亮。


![](https://img-blog.csdn.net/20131101202643796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## CSS代码

```
<style>		
	.tundra .dijitTreeRowHover {
	    background-image: url(../img/food/hover.png) !important;
	    background-repeat: repeat;
	    background-color: none !important;
	}
</style>
```


# 总结

经过这么些“打扮”，一棵漂亮的食物树就出来了！









