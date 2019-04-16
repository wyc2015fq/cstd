# HTML5 Canvas实现360度全景图 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年12月06日 20:01:44[gloomyfish](https://me.csdn.net/jia20003)阅读数：70586标签：[html5																[canvas																[javascript](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[HTML5](https://blog.csdn.net/jia20003/article/category/1070101)





很多购物网站现在都支持360实物全景图像，可以360度任意选择查看样品，这样

对购买者来说是一个很好的消费体验，网上有很多这样的插件都是基于JQuery实现的

有收费的也有免费的，其实很好用的一个叫3deye.js的插件。该插件支持桌面与移动终

端iOS与Android， 它的demo程序：[http://www.voidcanvas.com/demo/28823deye/](http://www.voidcanvas.com/demo/28823deye/)

自己玩了玩这个Demo以后，照着它的思路，用HTML5 Canvas也实现了类似的功能。

所以先说一下它的360度全景图的原理

1.      首先需要对实物拍照，间隔是每张照片旋转15度，所以需要23张照片。

2.      照片准备好了以后，尽量选择JPG格式，裁剪到适当大小。

3.      JAVASCRIPT中预加载所有照片，可以配合进度条显示加载精度

4.      创建/获取Canvas对象，加上鼠标监听事件，当鼠标左右移动时候，适度的绘制不

同帧。大致的原理就是这样，简单吧！

实现代码：



```
<!DOCTYPE html>
<html>
<head>
  <meta charset=utf-8">
  <title>Full 360 degree View</title>
  <script>
		var ctx = null; // global variable 2d context
		var frame = 1; // 23
		var width = 0;
		var height = 0;
		var started = false;
		var images = new Array();
		var startedX = -1;
	  window.onload = function() {
		var canvas = document.getElementById("fullview_canvas");
		canvas.width = 440;// window.innerWidth;
		canvas.height = 691;//window.innerHeight;
		width = canvas.width;
		height = canvas.height;
		var bar = document.getElementById('loadProgressBar');
		for(var i=1; i<24; i++)
		{
			bar.value = i;
			if(i<10)
			{
				images[i] = new Image();
				images[i].src = "0" + i + ".jpg";
			}
			else 
			{
				images[i] = new Image();
				images[i].src = i + ".jpg";
			}
		}
		ctx = canvas.getContext("2d");
		
		// mouse event
		canvas.addEventListener("mousedown", doMouseDown, false);
		canvas.addEventListener('mousemove', doMouseMove, false);
		canvas.addEventListener('mouseup',   doMouseUp, false);
		// loaded();
		
		// frame = 1
		frame = 1;
		images[frame].onload = function() {
			redraw();
			bar.style.display = 'none';
		}
	}
	function doMouseDown(event) {
		var x = event.pageX;
		var y = event.pageY;
		var canvas = event.target;
		var loc = getPointOnCanvas(canvas, x, y);
		console.log("mouse down at point( x:" + loc.x + ", y:" + loc.y + ")");
		startedX = loc.x;
		started = true;
	}
	
	function doMouseMove(event) {
		var x = event.pageX;
		var y = event.pageY;
		var canvas = event.target;
		var loc = getPointOnCanvas(canvas, x, y);
		if (started) {
			var count = Math.floor(Math.abs((startedX - loc.x)/30));
			var frameIndex = Math.floor((startedX - loc.x)/30);
			while(count > 0)
			{				
				console.log("frameIndex = " + frameIndex);
				count--;	
				if(frameIndex > 0)
				{
					frameIndex--;
					frame++;
				} else if(frameIndex < 0)
				{
					frameIndex++;
					frame--;
				}
				else if(frameIndex == 0)
				{
					break;
				}
								
				if(frame >= 24)
				{
					frame = 1;
				}
				if(frame <= 0)
				{
					frame = 23;
				}
				redraw();
			}
		}
	}
	
	function doMouseUp(event) {
		console.log("mouse up now");
		if (started) {
			doMouseMove(event);
			startedX = -1;
			started = false;
		}
	}

	function getPointOnCanvas(canvas, x, y) {
		var bbox = canvas.getBoundingClientRect();
		return { x: x - bbox.left * (canvas.width  / bbox.width),
				y: y - bbox.top  * (canvas.height / bbox.height)
				};
	}
	
	function loaded() {
		setTimeout( update, 1000/8);
	}
	function redraw()
	{
		// var imageObj = document.createElement("img");
		// var imageObj = new Image();
		var imageObj = images[frame];
		ctx.clearRect(0, 0, width, height)
		ctx.drawImage(imageObj, 0, 0, width, height);
	}
	function update() {
		redraw();
		frame++;
		if (frame >= 23) frame = 1;
		setTimeout( update, 1000/8);
	}
  </script>
</head>
<body>
<progress id="loadProgressBar" value="0" max="23"></progress> 
<canvas id="fullview_canvas"></canvas>
<button onclick="loaded()">Auto Play</button>
</body>
</html>
```

**Demo演示文件下载地址->http://download.csdn.net/detail/jia20003/6670901**](https://so.csdn.net/so/search/s.do?q=canvas&t=blog)](https://so.csdn.net/so/search/s.do?q=html5&t=blog)




