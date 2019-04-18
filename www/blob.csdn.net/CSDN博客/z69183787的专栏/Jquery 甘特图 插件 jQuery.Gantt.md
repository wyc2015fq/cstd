# Jquery 甘特图 插件 jQuery.Gantt - z69183787的专栏 - CSDN博客
2014年11月25日 09:40:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15564
[http://taitems.github.io/jQuery.Gantt/](http://taitems.github.io/jQuery.Gantt/)
|参数|默认值|类型|说明|
|----|----|----|----|
|source|null|Array, String (url)|数据源：json数组或者返回json的url地址|
|itemsPerPage|7|Number|每页显示项目数|
|months|["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]|Array|月份对应的显示字符|
|dow|["S", "M", "T", "W", "T", "F", "S"]|Array|周日到周一的显示字符|
|navigate|"buttons"|String ("buttons","scroll")|导航类型：按钮和滚动条|
|scale|"days"|String|显示单元|
|maxScale|"months"|String|视图缩放最大单元|
|minScale|"hours"|String|视图缩放最小单元|
|waitText|"Please Wait..."|String|加载时的提示文字|
|onItemClick:|function (data) { return; }||点击时执行的操作，参数是被点击项目相关联的数据对象|
|onAddClick|function (dt, rowId) { return; }||在空白格子点击时执行的函数第一个参数是点击点对应时间的毫秒数，第二个参数对象的id|
|onRender|function () { return; }||甘特图渲染完毕后执行的操作|
|useCookie|false||声明是是否使用cookie记录图表的状态（缩放单元、滚动条位置）需要使用jquery.cookie.js才能使用该选项|
|scrollToToday|true|Boolean|是否滚动到当天|
## Source Configuration 数据源配置格式
source: [{
	name: "标题",
	desc: "描述.",
	values: [ ... ]
}]
|参数|默认值|类型|说明|
|----|----|----|----|
|name|null|String|甘特图左侧显示的每行所对应的标题——体文字部分|
|desc|null|String|左侧说明的描述部分|
|values|null|Array|甘特图单元所对应的时间段等信息，在右侧的单元格显示|
## Value Configuration 数据源的json数值对
values: [{
	to: "/Date(1328832000000)/",
	from: "/Date(1333411200000)/",
	desc: "鼠标悬停时的描述文字",
	label: "甘特图项目标题",
	customClass: "ganttRed",
	dataObj: foo.bar[i]
}]
|参数|类型|说明|
|----|----|----|
|to|String (Date)|结束时间（毫秒数，php返回时可以在时间戳后添加3个0）|
|from|String (Date)|开始时间|
|desc|String|鼠标悬停到项目上个的提示文字Text that appears on hover, I think?|
|label|String|甘特图项目的标题文字|
|customClass|String|添加到甘特图项目的自定义类，可以用来标记甘特图项目的颜色|
|dataObj|All|点击时应用到甘特图项目的data对象|
```java
$(function() {
			"use strict";
			$(".gantt").gantt({
				source: [{
					name: "Sprint 0",
					desc: "Analysis",
					values: [{
						from: "/Date(1320192000000)/",
						to: "/Date(1322401600000)/",
						label: "Requirement Gathering",
						customClass: "ganttRed"
					}]
				},{
					name: " ",
					desc: "Scoping",
					values: [{
						from: "/Date(1322611200000)/",
						to: "/Date(1323302400000)/",
						label: "Scoping",
						customClass: "ganttRed"
					}]
				},{
					name: "Sprint 1",
					desc: "Development",
					values: [{
						from: "/Date(1323802400000)/",
						to: "/Date(1325685200000)/",
						label: "Development",
						customClass: "ganttGreen"
					}]
				},{
					name: " ",
					desc: "Showcasing",
					values: [{
						from: "/Date(1325685200000)/",
						to: "/Date(1325695200000)/",
						label: "Showcasing",
						customClass: "ganttBlue"
					}]
				},{
					name: "Sprint 2",
					desc: "Development",
					values: [{
						from: "/Date(1326785200000)/",
						to: "/Date(1325785200000)/",
						label: "Development",
						customClass: "ganttGreen"
					}]
				},{
					name: " ",
					desc: "Showcasing",
					values: [{
						from: "/Date(1328785200000)/",
						to: "/Date(1328905200000)/",
						label: "Showcasing",
						customClass: "ganttBlue"
					}]
				},{
					name: "Release Stage",
					desc: "Training",
					values: [{
						from: "/Date(1330011200000)/",
						to: "/Date(1336611200000)/",
						label: "Training",
						customClass: "ganttOrange"
					}]
				},{
					name: " ",
					desc: "Deployment",
					values: [{
						from: "/Date(1336611200000)/",
						to: "/Date(1338711200000)/",
						label: "Deployment",
						customClass: "ganttOrange"
					}]
				},{
					name: " ",
					desc: "Warranty Period",
					values: [{
						from: "/Date(1336611200000)/",
						to: "/Date(1349711200000)/",
						label: "Warranty Period",
						customClass: "ganttOrange"
					}]
				}],
				navigate: "scroll",
				maxScale: "hours",
				itemsPerPage: 10,
				onItemClick: function(data) {
					alert("Item clicked - show some details");
				},
				onAddClick: function(dt, rowId) {
					alert("Empty space clicked - add an item!");
				},
				onRender: function() {
					if (window.console && typeof console.log === "function") {
						console.log("chart rendered");
					}
				}
			});
			$(".gantt").popover({
				selector: ".bar",
				title: "I'm a popover",
				content: "And I'm the content of said popover.",
				trigger: "hover"
			});
			prettyPrint();
		});
```
