# dijit.Tree中widget和DOM node之间的切换 - 沧海一粟 —— 技术随手记 - CSDN博客





2014年08月06日 12:57:20[慢游](https://me.csdn.net/eengel)阅读数：2167








之前在[装扮装扮dijit.Tree](http://blog.csdn.net/eengel/article/details/14043573)这篇文章中介绍了如何利用dijit.Tree提供的功能改变树的外表。这儿继续dijit.Tree的话题，说说dijit.Tree中widget和DOM node之间的切换。

dijit.Tree是dojo提供的一个对象，用以描述树形UI。相应地，它有与之对应的DOM结构，用以在网页上将树呈现出来。在很多时候，我们需要以一知二，或者已知widget对象，得到对应的DOM node；或者已知DOM node，推断其widget对象。



和上文一样，先提供一个dijit.Tree的例子，一棵食物树。

![](https://img-blog.csdn.net/20140806124952843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
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

</head>
<body class="tundra">
<div style="width: 100%; height: 100%"
	data-dojo-type="dijit.layout.BorderContainer"
	data-dojo-props="design:'sidebar', gutters:true, liveSplitters:true">
	<div data-dojo-type="dojox.layout.ContentPane" data-dojo-props="region:'left',splitter:true" 
		style="width:180px;">
		<div id="tree_1"></div>
	</div>
</div>

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
						
						var tree = new Tree({
							id : "tree_1",
							model : model_1,
							showRoot : false,
							persist : true
						}, "tree_1");
					});
				});
	</script>
</body>
```




1.      获得tree的对象实例

```java
>>>var tree = dijit.byId("tree_1");
undefined
>>>tree
[Widgetdijit.Tree, tree_1] { _attachPoints=[1], _attachEvents=[1], _connects=[4],more...}
```

2.      根据tree的对象实例获得对应的DOM node

```java
>>> tree.domNode
<div id="tree_1"class="dijitTree dijitTreeContainer dojoDndContainer"data-dojo-attach-event="onkeypress:_onKeyPress"role="presentation" widgetid="tree_1"aria-expanded="true" aria-multiselect="true">
```

3.      反过来，根据DOM node获得对应的tree对象实例

```java
>>>dijit.getEnclosingWidget(tree.domNode)
[Widget dijit.Tree, tree_1] {_attachPoints=[1], _attachEvents=[1], _connects=[4], more...}
```

接下来看看如何操作tree的内部结构

4.      根据tree得到根节点所在的DOM node

```java
>>>dijit.findWidgets(tree.domNode)
[[Widget dijit._TreeNode,dijit__TreeNode_0] { _attachPoints=[7], _attachEvents=[5], _connects=[20],more...}]
可以看到这儿取到了隐藏的根节点。（上面创建tree的时候设置了”showRoot : false”，因此在界面上看不到这个根节点。）
>>>dijit.findWidgets(tree.domNode)[0].domNode
<div id="dijit__TreeNode_0"class="dijitTreeIsRoot dijitTreeNode dijitTreeNodeLOADED dijitLOADED"role="presentation" style="background-position: 0px 0px;"widgetid="dijit__TreeNode_0">
得到了根节点所在的DOMnode。
```

5.      同理，这样可以得到tree上任意的节点以及相应的DOM node

```java
>>> var rootNode =dijit.findWidgets(tree.domNode)[0].domNode；
>>> dijit.findWidgets(rootNode)[0]
[Widget dijit._TreeNode, dijit__TreeNode_1]{ _attachPoints=[7], _attachEvents=[5], _connects=[20], more...}
>>>dijit.findWidgets(rootNode)[0].item.name
["Foods"]
这儿取到了tree的”Foods”节点。
>>>dijit.findWidgets(rootNode)[0].domNode
<div id="dijit__TreeNode_1"class="dijitTreeNode dijitTreeNodeUNCHECKED dijitUNCHECKEDdijitTreeIsRoot" role="presentation" style="background-position:0px 0px;" widgetid="dijit__TreeNode_1">
”Foods”节点对应的DOM node。
```

6.      tree的点击事件

如果我们为tree添加一个click事件。则可以同时拿到对应的item和widget

```java
on(tree,"click", function(item, widget){
      console.log(item);
      console.log(widget);
});
```
Console:



```java
Object { id=[1], name=[1], type=[1],more...}
[Widget dijit._TreeNode, dijit__TreeNode_3]{ _attachPoints=[7], _attachEvents=[5], _connects=[20], more...}
```










