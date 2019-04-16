# HTML5 Canvas 填充与描边(Fill And Stroke) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年07月12日 15:00:55[gloomyfish](https://me.csdn.net/jia20003)阅读数：22006
所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)









**HTML5 Canvas 填充与描边(Fill And Stroke)**

演示HTML5 Canvas Fill 与Stroke文字效果，基于Canvas如何实

现纹理填充与描边。


**一：颜色填充与描边**

颜色填充可以通过fillStyle来实现，描边颜色可以通过strokeStyle来实现。简单示例

如下：

// fill and stroke text

ctx.font = '60pt Calibri';

ctx.lineWidth = 3;

ctx.strokeStyle = 'green';

ctx.strokeText('Hello World!', 20, 100);

ctx.fillStyle = 'red';

ctx.fillText('Hello World!', 20, 100);

**二：纹理填充与描边**

HTML5 Canvas还支持纹理填充，通过加载一张纹理图像，然后创建画笔模式，创建

纹理模式的API为ctx.createPattern(imageTexture,"repeat");第二参数支持四个

值，分别为”repeat-x”, ”repeat-y”, ”repeat”,”no-repeat”意思是纹理分别沿着

X轴，Y轴，XY方向沿重复或者不重复。纹理描边与填充的代码如下：

**var** woodfill = ctx.createPattern(imageTexture,"repeat");

ctx.strokeStyle = woodfill;

ctx.strokeText('Hello World!', 20, 200);

// fill rectangle

ctx.fillStyle = woodfill;

ctx.fillRect(60, 240, 260, 440);

纹理图片：

![](https://img-blog.csdn.net/20130712145734046)


三：运行效果

![](https://img-blog.csdn.net/20130712145829562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

代码：



```java
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="X-UA-Compatible" content="chrome=IE8">
<meta http-equiv="Content-type" content="text/html;charset=UTF-8">
<title>Canvas Fill And Stroke Text Demo</title>
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
			
			// get 2D context of canvas and draw rectangel
			ctx = canvas.getContext("2d");
			ctx.fillStyle="black";
			ctx.fillRect(0, 0, canvas.width, canvas.height);
			
			// fill and stroke text
			ctx.font = '60pt Calibri';
			ctx.lineWidth = 3;
			ctx.strokeStyle = 'green';
			ctx.strokeText('Hello World!', 20, 100);
			ctx.fillStyle = 'red';
			ctx.fillText('Hello World!', 20, 100);
			
			// fill and stroke by pattern
			imageTexture = document.createElement('img');
			imageTexture.src = "../pattern.png";
			imageTexture.onload = loaded();
		}
		
		function loaded() {
			// delay to image loaded
			setTimeout(textureFill, 1000/30);
		}
		
		function textureFill() {
			// var woodfill = ctx.createPattern(imageTexture, "repeat-x");
			// var woodfill = ctx.createPattern(imageTexture, "repeat-y");
			// var woodfill = ctx.createPattern(imageTexture, "no-repeat");
			var woodfill = ctx.createPattern(imageTexture, "repeat");
			ctx.strokeStyle = woodfill;
			ctx.strokeText('Hello World!', 20, 200);
			
			// fill rectangle
			ctx.fillStyle = woodfill;
			ctx.fillRect(60, 240, 260, 440);
		}
		
	</script>
</head>
<body>
	<h1>HTML5 Canvas Text Demo - By Gloomy Fish</h1>
	<pre>Fill And Stroke</pre>
	<div id="my_painter">
		<canvas id="text_canvas"></canvas>
	</div>
</body>
</html>
```





