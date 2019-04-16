# Dojo中一些挺有意思的小功能（一）——hitch - 沧海一粟 —— 技术随手记 - CSDN博客





2014年08月01日 17:15:33[慢游](https://me.csdn.net/eengel)阅读数：2070








用Dojo也有几年了，平常写代码的时候，经常碰到一些小小的需求，实现的功能不大，但是如果要自己编程的话，得费不少的劲儿。结果回头查Dojo的文档，竟然能找到一个功能，恰好能解决手头的需求，能惹得会心一笑。

忙中偷闲，这儿零零碎碎地把这样的小功能记录下来，聊以一乐。



文中所记功能是基于Dojo1.7.3的，想来更高的基于AMD的版本也有类似的功能。



## **dojo/_base/lang: hitch**

hitch顾名思义，“勾住/套住”什么。在Dojo中，hitch可以在调用一个函数时，“勾住”某个scope一起执行。它的输入是原来的函数和给定的scope，输出是糅合了scope的新函数。也就是说无论这个新的函数在什么场合被调用的，只要在hitch中指定了某个scope，那么它运行时的上下文就是这个scope。

一种典型的场合是在页面上点击鼠标时触发的事件调用。这儿就用一个详细的例子来说明。



**main.html**

例子中的主页面，上面有一个按钮button1和一块内容块content1。当点击button1时，会在content1中显示相应的文字。

Content 1继承自dojox/layout/ContentPane，拥有自己的页面content.html和控制模块Content.js。

![](https://img-blog.csdn.net/20140801171354906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



```
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
<style type="text/css">
@import "../js/dijit/themes/tundra/tundra.css";
</style>

</head>
<body class="tundra">
	<div>
		<button id="b1" data-dojo-type="dijit.form.Button"
					type="button">Button 1</button>
	</div>
	<div id="d1" data-dojo-type="cpw.Content" data-dojo-props="href: 'content.html'"
			style="width: 400px; height: 200px;border: 1px solid #DDDDDD;"></div>	
	
	<script>
		dojoConfig = {
			isDebug : false,
			parseOnLoad : true,
			async : true
		};
	</script>
	<script type="text/javascript" src="../js/dojo/dojo.js"></script>
	<script>
		require([ "dojo/parser", "dijit/form/Button", "cpw/Content"]);
	</script>
	
	<script>
		require(
				[ "dojo/ready", "dojo/on", "dijit/registry","dojo/_base/lang"],
				function(ready, on, registry, lang) {
					ready(function() {
						on(registry.byId("b1"), "click", function(){
							var d1 = registry.byId("d1");
							//这样showDay被调用时，其中的this是Content d1，id是b1.
							d1.showDay("b1");
						});
					});
				});
	</script>
</body>
</html>
```





**content.html**

content.html包含了自己的按钮button2。当点击button2时，也会在content1中显示相应的文字。



```
<div>Content 1</div>
<button id="b2" data-dojo-type="dijit.form.Button"
			type="button">Button 2</button>
<div id="c1"></div>
```


**Content.js**

可以看到，点击两个按钮都会调用showDay() 。而我们希望它们的行为是一致的。在实现b2的click事件时，就需要用hitch对其scope进行约束。

```java
define([ "dojo/_base/declare", "dojox/layout/ContentPane", "dojo/on", "dijit/registry", "dojo/dom", "dojo/_base/lang"], function(
		declare, ContentPane, on, registry, dom, lang) {
	return declare("cpw.Content", [ ContentPane ], {
				day : "Friday",
				
				onDownloadEnd : function() {
					//错误的写法，这样showDay被调用时，其中的this是button b2，id是click事件。
					//on(registry.byId("b2"), "click", this.showDay);
					
					//正确的写法。这样showDay被调用时，其中的this是Content d1，id是b2.
					on(registry.byId("b2"), "click", lang.hitch(this, "showDay", "b2"));
					
					this.inherited(arguments);
				},
				
				showDay : function(id){
					var text = id + " says this is " + this.day + ".";
					dom.byId("c1").innerHTML = text;
				}
			});
});
```





