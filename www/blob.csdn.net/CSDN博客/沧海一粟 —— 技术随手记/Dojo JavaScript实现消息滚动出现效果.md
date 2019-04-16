# Dojo JavaScript实现消息滚动出现效果 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年10月30日 11:03:37[慢游](https://me.csdn.net/eengel)阅读数：2529








# 介绍

这种效果说的就是类似状态监视器一样的应用。实时更新后台或某个监控点的状态消息。主要采用的技术就是定时器和滚动定位。



# 代码示例



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
<div data-dojo-type="dijit.layout.BorderContainer"
		style="width: 100%; height: 100%; margin: 0px; padding: 0px; ">
	<div data-dojo-type="dojox.layout.ContentPane"
		data-dojo-props="region:'top'"
		style="height:38px;width:100%;background-color: black; color: white;font-family: arial;font-size: 28px;">
		消息监视器
	</div>
	<div data-dojo-type="dojox.layout.ContentPane"
		data-dojo-props="region:'center', splitter: true"
		style="width: 100%; height: 100%; border: none; padding: 0px; background-color: #ffffff;">
		<div style="width: 99%; margin: -3px 0px -2px 2px;">
			<button id="start_btn" data-dojo-type="dijit.form.Button"
				class="menu_button" type="button">开始</button>
			<button id="stop_btn" data-dojo-type="dijit.form.Button"
				class="menu_button" type="button">停止</button>
			<button id="clear_btn" data-dojo-type="dijit.form.Button"
				class="menu_button" type="button">清除</button>
		</div>
		<div data-dojo-type="dijit.layout.BorderContainer"
			data-dojo-props="design:'sidebar', gutters:true, liveSplitters:true"
			style="width: 100%; height: 100px;">
			<div data-dojo-type="dojox.layout.ContentPane" style="font-size: 16px;"
				data-dojo-props="splitter:true,region:'center'">
				<!-- 为了每次都定位到新加的消息，需要对消息所在的容器设定style -->
				<div id="container" style="height: 100%;overflow: auto;width: 100%;"></div>
			</div>
		</div>
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
		require([ "dojo/parser", "dijit/form/Button", 
				"dijit/layout/BorderContainer", "dojox/layout/ContentPane",
				"dojox/timing/_base"]);
	</script>
	<script>
		require(
				[ "dojo/ready", 
				  "dijit/registry",
				  "dojo/dom",
				  "dojo/on"
				  ],
				function(ready, registry, dom, on) {
					ready(function() {
						var container = dom.byId("container");
						
						//创建定时器
						var t = new dojox.timing.Timer();
						
						//当点击开始时，触发定时器，更新消息
						on(registry.byId("start_btn"), "click", function(e){
							t.setInterval(2000);
							t.onTick = update;
							t.start();
						});
						
						//当点击停止时，停止定时器
						on(registry.byId("stop_btn"), "click", function(e){
							t.stop();
						});
						
						//当点击清除时，将所有的消息都清空
						on(registry.byId("clear_btn"), "click", function(e){
							container.innerHTML = "";
						});
						
						var update = function(){
							var content = container.innerHTML;
							container.innerHTML = content + 
									"<div>A new message arrived at " + 
									Date() +
									"</div>";
							
							//确保每次都定位到新加的消息
							container.scrollTop = container.scrollHeight;
						};
					});
				});
	</script>
</body>
</html>
```


# 总结



从这个例子可以看出，做前端应用，往往需要同时掌握HTML、JavaScript、CSS。




