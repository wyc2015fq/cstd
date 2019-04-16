# dojox.charting和dojox.timing构造Dojo动态图表 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年10月31日 10:40:58[慢游](https://me.csdn.net/eengel)阅读数：2390








# 介绍

在大数据时代，Visualization似乎很火。它让数据变得更直观、易理解。相应的，Dojo提供了一个专门针对 web 矢量图开发的控件包dojox.charting，提供了很多封装好的控件，比如线图、饼图、柱状图等等。试想，如果能够让后台实时更新的数据直接反映在web页面的一张图上，这张图还能随着数据的更新而变化，岂不是很有趣？关键有了dojox.charting，这样的功能就能轻松实现。



# 代码示例

这儿就用dojox.charting和dojox.timing来实现动态生成折线图的效果。



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
		</div>
		<div data-dojo-type="dijit.layout.BorderContainer"
			data-dojo-props="design:'sidebar', gutters:true, liveSplitters:true"
			style="width: 100%; height: 300px;">
			<div data-dojo-type="dojox.layout.ContentPane" style="font-size: 16px;"
				data-dojo-props="splitter:true,region:'center'">
				<div class="statistic_div">
					<div id="line_chart" style="width: 98%; height: 155px;"></div>
					<center>
						<div id="line_chart_legend"></div>
					</center>
				</div>
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
				  "dojo/on",
				  "dojox/charting/Chart", "dojox/charting/plot2d/Pie",
					"dojox/charting/action2d/Highlight",
					"dojox/charting/action2d/MoveSlice",
					"dojox/charting/action2d/Magnify",
					"dojox/charting/action2d/Tooltip",
					"dojox/charting/themes/MiamiNice", "dojox/charting/widget/Legend",
					"dojox/charting/axis2d/Default", "dojox/charting/plot2d/Columns",
					"dojox/charting/plot2d/ClusteredColumns",
					"dojox/charting/plot2d/Lines", "dojo/store/Observable",
					"dojo/store/Memory", "dojo/data/ItemFileWriteStore",
					"dojox/charting/StoreSeries", "dojox/charting/DataSeries"
				  ],
				function(ready, registry, dom, on, Chart, Pie,
						Highlight, MoveSlice, Magnify, Tooltip, MiamiNice, Legend, Default,
						Columns, ClusteredColumns, Lines, Observable, Memory,
						ItemFileWriteStore, StoreSeries, DataSeries) {
					ready(function() {
						line_chart = new Chart("line_chart");
						line_chart.setTheme(MiamiNice);

						line_chart.addPlot("default", {
							type : Lines,
							markers : true
						});

						// Add axes
						line_chart.addAxis("x", {
							microTickStep : 1,
							minorTickStep : 1
						/*, max: 10*/});
						line_chart.addAxis("y", { //min: 0, 
							//max: 15000, 
							vertical : true,
							fixLower : "major",
							fixUpper : "major"
						});

						// Initial data
						var total_data = [
							{id: 1, value: 0},
							{id: 2, value: 13}
						];

						// Create the data store
						total_store = new Observable(new Memory({
							data : {
								identifier : "id",
								items : total_data
							}
						}));

						// Adds a StoreSeries to the y axis, queries for all site 1 items
						line_chart.addSeries("折线图",
								new StoreSeries(total_store, "value"));
						
						// Create the tooltip
						new Tooltip(line_chart, "default");

						// Create the magnifier
						Magnify(line_chart, "default");

						// Render the chart!
						line_chart.render();

						new Legend({
							chart : line_chart
						}, "line_chart_legend");
						
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
						
						var value = 10;
						var id = 3;
						var update = function(aa){
							total_store.notify({ value: value++, id: id++});
						};
					});
				});
	</script>
</body>
</html>
```



最终效果如下图所示。
![](https://img-blog.csdn.net/20131031104030125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




# 总结



dojox.charting提供了各种图表控件，还可以组合tooltip、legend等等附加控件，也可以绑定任意样式风格，可以说是比较强大的了。如果用dojox.charting也不能满足要求，还可以尝试dojox.gfx，直接构造自己的矢量图。



