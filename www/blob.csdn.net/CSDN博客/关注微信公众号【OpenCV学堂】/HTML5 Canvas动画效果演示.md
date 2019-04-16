# HTML5 Canvas动画效果演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年07月11日 10:32:02[gloomyfish](https://me.csdn.net/jia20003)阅读数：13154
所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)









**HTML5 Canvas动画效果演示**

**主要思想：**

首先要准备一张有连续帧的图片，然后利用HTML5 Canvas的draw方法在不同的时间

间隔绘制不同的帧，这样看起来就像动画在播放。

**关键技术点：**

JavaScript 函数setTimeout()有两个参数，第一个是参数可以传递一个JavaScript方法，

另外一个参数代表间隔时间，单位为毫秒数。代码示例：

setTimeout( update, 1000/30);

Canvas的API-drawImage()方法，需要指定全部9个参数：

ctx.drawImage(myImage, offw, offh, width,height, x2, y2, width, height);

其中offw, offh是指源图像的起始坐标点，width, height表示源图像的宽与高，x2,y2表

示源图像在目标Canvas上的起始坐标点。

一个22帧的大雁飞行图片实现的效果：

![](https://img-blog.csdn.net/20130711102405515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

源图像：

![](https://img-blog.csdn.net/20130711102928359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


程序代码：



```java
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="X-UA-Compatible" content="chrome=IE8">
<meta http-equiv="Content-type" content="text/html;charset=UTF-8">
<title>Canvas Mouse Event Demo</title>
<link href="default.css" rel="stylesheet" />
	<script>
		var ctx = null; // global variable 2d context
		var started = false;
		var mText_canvas = null;
		var x = 0, y =0;
		var frame = 0; // 22 5*5 + 2
		var imageReady = false;
		var myImage = null;
		var px = 300;
		var py = 300;
		var x2 = 300;
		var y2 = 0;
		window.onload = function() {
			var canvas = document.getElementById("animation_canvas");
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
			myImage = document.createElement('img');
			myImage.src = "../robin.png";
			myImage.onload = loaded();
		}  
		
		function loaded() {
		    imageReady = true;
		    setTimeout( update, 1000/30);
		}
		
		function redraw() {
			ctx.clearRect(0, 0, 460, 460)
			ctx.fillStyle="black";
			ctx.fillRect(0, 0, 460, 460);
			
			// find the index of frames in image
			var height = myImage.naturalHeight/5;
			var width = myImage.naturalWidth/5;
			var row = Math.floor(frame / 5);
			var col = frame - row * 5;
			var offw = col * width;
			var offh = row * height;
			
			// first robin
			px = px - 5;
			py = py - 5;
			if(px < -50) {
				px = 300;
			}
			if(py < -50) {
				py = 300;
			}
			
			//var rate = (frame+1) /22;
			//var rw = Math.floor(rate * width);
			//var rh = Math.floor(rate * height);
			ctx.drawImage(myImage, offw, offh, width, height, px, py, width, height);
			
			// second robin			
			x2 = x2 - 5;
			y2 = y2 + 5;
			if(x2 < -50) {
				x2 = 300;
				y2 = 0;
			}
			ctx.drawImage(myImage, offw, offh, width, height, x2, y2, width, height);
			
		}
		
		function update() {
		    redraw();
		    frame++;
		    if (frame >= 22) frame = 0;
		    setTimeout( update, 1000/30);
		}
		
	</script>
</head>
<body>
	<h1>HTML Canvas Animations Demo - By Gloomy Fish</h1>
	<pre>Play Animations</pre>
	<div id="my_painter">
		<canvas id="animation_canvas"></canvas>
	</div>
</body>
</html>
```
发现上传透明PNG格式有点问题，所以我上传

不透明的图片。可以用其它图片替换，替换以后

请修改最大帧数从22到你的实际帧数即可运行。




