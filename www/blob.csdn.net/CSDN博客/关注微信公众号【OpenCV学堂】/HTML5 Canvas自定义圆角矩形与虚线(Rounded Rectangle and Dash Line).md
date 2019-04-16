# HTML5 Canvas自定义圆角矩形与虚线(Rounded Rectangle and Dash Line) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年07月17日 15:33:52[gloomyfish](https://me.csdn.net/jia20003)阅读数：17253
所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)









HTML5 Canvas自定义圆角矩形与虚线(RoundedRectangle and Dash Line)

实现向HTML Canvas 2d context绘制对象中添加自定义的函数功能演示，如何绘制虚线

以及控制虚线间隔大小，学会绘制圆角矩形的技巧。

HTML5 Canvas绘制对象中提供的原生功能没有实现绘制圆角矩形与虚线的功能，但是

通过JavaScript语言的Object.prototype可以实现对对象CanvasRenderingContext2D添

加这两个函数功能。代码的演示效果如下：

![](https://img-blog.csdn.net/20130717153117671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




组件fishcomponent.js的代码如下：



```java
CanvasRenderingContext2D.prototype.roundRect =
	function(x, y, width, height, radius, fill, stroke) {
		if (typeof stroke == "undefined") {
			stroke = true;
		}
		if (typeof radius === "undefined") {
			radius = 5;
		}
		this.beginPath();
		this.moveTo(x + radius, y);
		this.lineTo(x + width - radius, y);
		this.quadraticCurveTo(x + width, y, x + width, y + radius);
		this.lineTo(x + width, y + height - radius);
		this.quadraticCurveTo(x + width, y + height, x + width - radius, y+ height);
		this.lineTo(x + radius, y + height);
		this.quadraticCurveTo(x, y + height, x, y + height - radius);
		this.lineTo(x, y + radius);
		this.quadraticCurveTo(x, y, x + radius, y);
		this.closePath();
		if (stroke) {
			this.stroke();
		}
		if (fill) {
			this.fill();
		}
};

CanvasRenderingContext2D.prototype.dashedLineTo = function (fromX, fromY, toX, toY, pattern) {
	// default interval distance -> 5px
	if (typeof pattern === "undefined") {
		pattern = 5;
	}

	// calculate the delta x and delta y
	var dx = (toX - fromX);
	var dy = (toY - fromY);
	var distance = Math.floor(Math.sqrt(dx*dx + dy*dy));
	var dashlineInteveral = (pattern <= 0) ? distance : (distance/pattern);
	var deltay = (dy/distance) * pattern;
	var deltax = (dx/distance) * pattern;
	
	// draw dash line
	this.beginPath();
	for(var dl=0; dl<dashlineInteveral; dl++) {
		if(dl%2) {
			this.lineTo(fromX + dl*deltax, fromY + dl*deltay);
		} else {    				
			this.moveTo(fromX + dl*deltax, fromY + dl*deltay);    				
		}    			
	}
	this.stroke();
};
```
HTML中调用演示：



```java
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="X-UA-Compatible" content="chrome=IE8">
<meta http-equiv="Content-type" content="text/html;charset=UTF-8">
<title>Canvas Rounded Rectangle Demo</title>
<script src="fishcomponent.js"></script>
<link href="default.css" rel="stylesheet" />
	<script>
		var ctx = null; // global variable 2d context
		var imageTexture = null;
		window.onload = function() {
			var canvas = document.getElementById("text_canvas");
			console.log(canvas.parentNode.clientWidth);
			canvas.width = canvas.parentNode.clientWidth;
			canvas.height = canvas.parentNode.clientHeight;
			
			if (!canvas.getContext) {
			    console.log("Canvas not supported. Please install a HTML5 compatible browser.");
			    return;
			}
			var context = canvas.getContext('2d');
			context.strokeStyle="red";
			context.fillStyle="RGBA(100,255,100, 0.5)";
			context.roundRect(50, 50, 150, 150, 5, true);
			context.strokeStyle="blue";								
			for(var i=0; i<10; i++) {
				var delta = i*20;
				var pattern = i*2+1;
				context.dashedLineTo(250, 50+delta, 550, 50+delta, pattern);
			}
		}
	</script>
</head>
<body>
	<h1>HTML5 Canvas Dash-line Demo - By Gloomy Fish</h1>
	<pre>Dash line and Rounded Rectangle</pre>
	<div id="box_plot">
		<canvas id="text_canvas"></canvas>
	</div>
</body>
</html>
```
**转载请注明**



