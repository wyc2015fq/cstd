# 四两拨千斤——Dijit Tree 拖拽（DnD, Drag & Drop）的精细控制 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年10月24日 11:29:06[慢游](https://me.csdn.net/eengel)阅读数：4397








# 介绍

dijit.Tree是一款很有用的Dojo控件，用来在页面上表示树形数据。像Dojo其他很多控件一样，它可以直接和后台的数据连接，并实时显示在页面上。已经有很多帖子介绍如何创建树、加入点击事件及右键菜单等，这儿专门介绍dijit.Tree拖拽的精细控制（Dojo 1.7.3）。



先说说举例用的应用场景。一个食品原材料供应商要建立自己的产品列表（New List），该列表包含几个大类，大类下面又有小类，直至具体的产品。现在要做的就是，请一个专业人员，根据一个别的渠道来的列表（Old List），建立自己的产品列表。

![](https://img-blog.csdn.net/20131024112602828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



根据需求，这两个列表(dijit.tree)要实现的最重要的功能就是支持拖拽（Drag & drop）：从左边的列表拖动节点到右边的列表。为了达到目标，还需要进行诸多精细控制，列举如下：

1.        在dijit.Tree上拖拽节点 
- 在左边的树上，禁止拖动根节点、已经被拖拽到右边的节点；
- 设定拖拽的目标位置；
- 改变拖拽后的节点属性，比如这两棵树都是以”id”作为标识符，则拖拽后要保证节点的id和原来树上的节点但不冲突；
- 将拖拽后原来节点的颜色变成灰色，表示已经从这棵树上删除；
- 当选中的是父亲节点时，同时拖拽其孩子；
- 拖拽后，只在目标树上保留不是灰色的节点；
- 禁止拖拽节点到某个目标位置。



2.        滚动（Scrolling）
- 禁止有垂直滚动条时，用鼠标点滚动条触发拖拽。

要实现上面这么多控制，首先要了解dijit.Tree拖拽的主要控制部件。那就是"dijit/tree/dndSource"。它实现了很多接口，在拖拽生命周期的不同阶段产生相应的作用。这儿就通过继承和修改相应接口，实现我们要的功能。





# 创建两棵树



test_tree.jsp

```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<style type="text/css">
@import "../js/dijit/themes/tundra/tundra.css";
@import "../css/main.css";
</style>

</head>
<body class="tundra">
<div style="width: 100%; height: 100%"
	data-dojo-type="dijit.layout.BorderContainer"
	data-dojo-props="design:'sidebar', gutters:true, liveSplitters:true">
	<div data-dojo-type="dojox.layout.ContentPane" data-dojo-props="region:'left',splitter:true" style="width: 500px;">
		<div class="title_bar">Old List</div>
		<div id="tree_1"></div>
	</div>
	<div data-dojo-type="dojox.layout.ContentPane" data-dojo-props="region:'center',splitter:true">
		<div class="title_bar">New List</div>
		<div id="tree_2"></div>
	</div>
</div>

<script>
		dojoConfig = {
			isDebug : false,
			parseOnLoad : true,
			async : true,
			packages : [ {
				name : "custom",
				location : "../../js/custom"
			} ]
		};
	</script>
	<script type="text/javascript" src="../js/dojo/dojo.js"></script>
	<script>
		require(["dijit/layout/BorderContainer",
		         "dojox/layout/ContentPane",
				  "dijit/form/Button" ]);
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
				  "dijit/Tree", 
				  "custom/TreeDndSource",
				  "custom/TreeDndTarget"
				  ],
				function(ready, registry, array, on, dom, query, 
						ItemFileWriteStore, Observable, ForestStoreModel,
						Tree, customDndSource, customDndTarget) {
					ready(function() {
						//创建左边的树
						var data_1 = {
								identifier: 'id',
								label: 'name',
								items: [
									{ 
										id: '0', 
										name:'Foods', 
										type: 'level1',
										children:[ 
										           {_reference: '1'},  
										           {_reference: '2'},  
										           {_reference: '3'}
										] 
									},
									{ 
										id: '1', 
										name:'Fruits', 
										type: 'level2', 
										children:[ 
										           {_reference: '5'},
										           {_reference: '6'}
										] 
									},
									{ 
										id: '5', 
										name:'Orange',
										type:"level3"
									},
									{ 
										id: '6', 
										name:'Apple',
										type:"level3"
									},
									{ 
										id: '2', 
										name:'Vegetables', 
										type: 'level2'
									},
									{ 
										id: '3', 
										name:'Cereals', 
										type: 'level2', 
										children:[ 
										           {_reference: '4'}
										] 
									},
									{ 
										id: '4', 
										name:'Rice cereal: industrially manufactured baby food based on rice',
										type:"level3"
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
								type : 'level1'
							}
						});
						
						var tree_1 = new Tree({
							id : "tree_1",
							model : model_1,
							showRoot : false,
							dndController : customDndSource, //控制拖动
							dragThreshold : 8,
							betweenThreshold : 5,
							persist : true
						}, "tree_1");
						
						//创建右边的树
						var data_2 = {
								identifier: 'id',
								label: 'name',
								items: [
									{ 
										id: '10', 
										name:'Foods', 
										type: 'level1'
									},
									{ 
										id: '11', 
										name:'Water', 
										type: 'level1'
									}
								]
						};
						
						var store_2 = new ItemFileWriteStore({
							data : data_2
						});
						
						store_2 = new Observable(store_2);
						
						var model_2 = new ForestStoreModel({
							store : store_2,
							query : {
								type : 'level1'
							}
						});
						
						var tree_2 = new Tree({
							id : "tree_2",
							model : model_2,
							showRoot : false,
							dndController : customDndTarget, //控制拖拽
							dragThreshold : 8,
							betweenThreshold : 5,
							persist : true
						}, "tree_2");
					});
				});
	</script>
</body>
```


# 支持从左边的树上拖动节点，并进行精细控制

TreeDndSource.js



专门在拖动的时候进行控制。主要的接口是onMouseDown。

```java
define([ "dojo/_base/declare", "dijit/tree/dndSource", 
         "dojo/_base/array"], function(
		declare, dndSource, array) {
	return declare("custom.TreeDndSource", [ dndSource ], {
		onMouseDown: function(e){
			//2.1 禁止有垂直滚动条时，用鼠标点滚动条触发拖拽
			var cName = e.target.className;
			if(cName.indexOf("dijitTree ") != -1) {
				return;
			}
			
			//1.1 在左边的树上，禁止拖动已经被拖拽到右边的节点
			var node = e.target;
			if(node.style.color == "rgb(187, 187, 187)"){
				return;
			}
			
			//1.1 在左边的树上，禁止拖动根节点
			var item = dijit.getEnclosingWidget(node).item;
			var store = dijit.getEnclosingWidget(node).tree.model.store;
			if(store.getValue(item, "type") == "level1"){
				return;
			}
			
			this.inherited(arguments);
		}
	});
});
```


# 支持在右边的树上放下拖拽的节点，并进行精细控制

TreeDndTarget.js



专门在拖动结束的时候进行控制。主要的接口是onDndDrop。

```java
define([ "dojo/_base/declare", "custom/TreeDndSource",
         "dojo/_base/array", "dojo/query", "custom/util"], function(
		declare, dndSource, array, query, util) {
	return declare("custom.TreeDndTarget", [ dndSource ], {
		sourceStore: null,
		targetModel: null,
		targetStore: null,
		
		addItem: function(sourceItem, parent, insertIndex, deleteList) {
			var that = this;
			var newItem = {};
			
			//1.3 改变拖拽后的节点属性，确保id不重复
			var keys = this.sourceStore.getAttributes(sourceItem);
			array.forEach(keys, function(key){
				if(key != "children"){
					var value = that.sourceStore.getValue(sourceItem, key);
					if(key == "id"){
						value = util.getRandomId();
					}else if(key == "parent"){
						if(dojo.isArray(parent.id)){
							value = parent.id[0];								
						}else{
							value = parent.id;
						}
					}
					newItem[key] = value;
				}
			});

		    newItem = this.targetStore.newItem(newItem);
		    this.targetModel.pasteItem(newItem, null, parent, false, insertIndex);

		    //1.5 当选中的是父亲节点时，同时拖拽其孩子
		    var children = sourceItem.children;
		    if (children !== undefined && children.length > 0) {
		    	array.forEach(children, function(citem, i){
		    		//1.6 拖拽后，只在目标树上保留不是灰色的节点
		    		if(deleteList != null && deleteList[i] == false)
		    			that.addItem(citem, newItem, i, null);
				});
		    }
		    
		    if(this.targetStore == this.sourceStore){
		    	this.targetStore.deleteItem(sourceItem);
		    }
		},
		
		onDndDrop: function(source, nodes, copy){
			this.targetModel = this.tree.model;
			this.targetStore = this.targetModel.store;
			this.sourceStore = source.tree.model.store;
			
			//1.2 设定拖拽的目标位置
			var newParentItem;
			var insertIndex;
			var target = this.targetAnchor;
			newParentItem = (target && target.item) || this.tree.item;
			if(this.dropPosition == "Before" || this.dropPosition == "After"){
				newParentItem = (target.getParent() && target.getParent().item) || this.tree.item;
				
				insertIndex = target.getIndexInParent();
				if(this.dropPosition == "After"){
					insertIndex = target.getIndexInParent() + 1;
				}
			}else{
				newParentItem = (target && target.item) || this.tree.item;
			}

			if(newParentItem == null){
				if(this.current != null){
					newParentItem = this.current.item;
				}
			}
			
			if(newParentItem != null){
				var that = this;
				array.forEach(nodes, function(node, i){
					if(insertIndex != null){
						insertIndex = insertIndex + i;
					}
															
					//1.7 禁止拖拽节点到某个目标位置
					var sourceItem = dijit.getEnclosingWidget(node).item;
					if(newParentItem.root == true && that.tree.id == "tree_2"){
						alert("The item is being dropped outside the model which is invalid.");
						that.onDndCancel();
						return;
					}
					
					//1.6 拖拽后，只在目标树上保留不是灰色的节点
					var deleteList = {};
					array.forEach(node.children[1].children, function(treenode, j){
						deleteList[j] = false;
						var treelabels = query("span[class~='dijitTreeLabel']", treenode);
						if(treelabels.length == 1){
							var treelabel = treelabels[0];
							if(treelabel.style.color == "rgb(187, 187, 187)"){
								deleteList[j] = true;
							}
						}
					});
					
					//Create item's all children in target tree
					that.addItem(sourceItem, newParentItem, insertIndex, deleteList);
					
					//1.4 将拖拽后原来节点的颜色变成灰色，表示已经从这棵树上删除
					var labelnodes = dojo.query("span[class~='dijitTreeLabel']", node);
					if(labelnodes != null && labelnodes.length > 0){
						array.forEach(labelnodes, function(labelnode){
							labelnode.style.color = "#BBBBBB";							
						});
					}
				});
				util.markModelSave(false);
			}
			
			this.onDndCancel();
		}
	});
});
```

# 总结



其实和dijit Tree拖拽相关的JavaScript对象和接口很强大，但如果对它们不熟悉，上面所说的精细控制就很会让人焦头烂额。而一旦熟悉并领会了其中规律，就可以四两拨千斤，快速实现所需要的功能。希望这篇文章能对dijit Tree的更多控制有所启发。




