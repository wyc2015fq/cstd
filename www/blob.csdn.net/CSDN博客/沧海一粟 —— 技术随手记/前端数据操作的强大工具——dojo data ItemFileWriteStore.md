# 前端数据操作的强大工具——dojo.data.ItemFileWriteStore - 沧海一粟 —— 技术随手记 - CSDN博客





2013年10月29日 09:33:14[慢游](https://me.csdn.net/eengel)阅读数：4660








# 介绍

dojo.data.ItemFileWriteStore是Dojo用来在前端操作数据的一个强大的工具。它可以和很多UI控件合作，进行数据的存储、传输和表示。比如和dijit.Tree结合时，可以在前端表示树形。最强大的是，只要操作ItemFileWriteStore，就可以实现树形的修改，包括增删节点、修改属性等。而且可以实时反映在前端页面上。



现在就以dijit.Tree为例，说明ItemFileWriteStore可以进行哪些操作（Dojo 1.7.3）。



# 代码示例

首先创建dijit.Tree并绑定一个dojo.data.ItemFileWriteStore。

test.jsp



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
<div data-dojo-type="dojox.layout.ContentPane" style="width: 100%;">
	<div id="tree_1"></div>
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
				  "dijit/Tree"
				  ],
				function(ready, registry, array, on, dom, query, 
						ItemFileWriteStore, Observable, ForestStoreModel,
						Tree, customDndSource, customDndTarget) {
					ready(function() {
						/*创建ItemFileWriteStore及相应的dijit.Tree*/
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
										name:'Rice cereal',
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
							dndController : customDndSource,
							dragThreshold : 8,
							betweenThreshold : 5,
							persist : true
						}, "tree_1");
					});
				});
	</script>
</body>
</html>
```




接下来，通过操作ItemFileWriteStore来实现对绑定tree的操作。

```java
/*操作ItemFileWriteStore*/
	
	//从dijit.Tree拿到ItemFileWriteStore
	var store = dijit.byId("tree_1").model.store;
	
	//根据identity拿到item，两种方法
	var item = store._itemsByIdentity["1"]; //Object { id=[1], name=[1], type=[1], more...}
	var item = store._getItemByIdentity("0");
	
	//对item设置属性及值
	store.setValue(item, "description", "This is fruits"); //true
	
	//对item取消某个属性
	store.unsetAttribute(item, "description");  //true
	
	//拿到item的所有自定义属性
	store.getAttributes(item);  //["id", "name", "type", "children"]
	
	//拿到item的所有属性，包括系统属性
	Object.keys(item);  //["id", "name", "type", "children", "_RI", "_S", "_0", "_RRM"]
	
	//对item设置值为列表的属性
	store.setValues(item, "order", [1,2]);  //true
	
	//拿到item为列表的属性
	store.getValues(item, "order");  //[1, 2]

	//拿到store的内容，以JSON String的格式
	store._getNewFileContentString();
	
	//根据各种query条件查询store中的items
	store.fetch({query:{type:"level1"}, onComplete:function(items){console.log(items);}});
	
	//拿到item的父亲
	var parent_id = Object.keys(item._RRM)[0];
	var parent = store._getItemByIdentity[parent_id];
	
	//创建item
	var newitem = {id:"7",name:"Oatmeal cereal",type:"level3"};
	var parent = store._getItemByIdentity("3");
	store.newItem(newitem, {parent: parent, attribute: "children"});
	store.save();
	
	//删除item
	store.deleteItem(newItem);
	store.save();
```


# 总结

除了dijit.Tree，dojo.data.ItemFileWriteStore 还可以绑定DataGrid、Select、ComboBox等等各种dojo UI控件。可以说掌握了ItemFileWriteStore，就可以实现很多用户交互的效果。




