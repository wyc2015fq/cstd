# HTML5 Canvas鼠标与键盘事件 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年07月04日 14:12:04[gloomyfish](https://me.csdn.net/jia20003)阅读数：47820
个人分类：[HTML5](https://blog.csdn.net/jia20003/article/category/1070101)

所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)









演示HTML5 Canvas鼠标事件，获取Canvas对象上的鼠标坐标，演示键盘事件

通过键盘控制Canvas上对象移动。

Canvas对象支持所有的JavaScript的鼠标事件，包括鼠标点击(MouseClick), 鼠标按下

(Mouse Down), 鼠标抬起(Mouse Up),鼠标移动( Mouse Move)

对Canvas添加鼠标事件方式有两种，一种方式是通过API来完成：

// mouse event

canvas.addEventListener("mousedown",doMouseDown,**false**);

canvas.addEventListener('mousemove', doMouseMove,**false**);

canvas.addEventListener('mouseup',  doMouseUp,
**false**);

另外一种方式在JavaScript中称为反模式：

canvas.onmousedown = **function**(e){



}

canvas.onmouseup = **function**(e){



}

canvas.onmousemove = **function**(e){



}

**获取鼠标在Canvas对象上坐标：**

由于Canvas上鼠标事件中不能直接获取鼠标在Canvas的坐标，所获取的都是基于整个

屏幕的坐标。所以通过鼠标事件e.pageX与e.pageY来获取鼠标位置，然后通过

Canvas. getBoundingClientRect()来获取Canvas对象相对屏幕的相对位置，通过计算

得到鼠标在Canvas的坐标，代码如下：

**function** getPointOnCanvas(canvas, x, y) {

**var** bbox =canvas.getBoundingClientRect();

**return** { x: x- bbox.left *(canvas.width / bbox.width),

y:y 
- bbox.top  * (canvas.height / bbox.height)

};

}

键盘事件：

HTML5 Canvas本身不支持键盘事件监听与获取，常用的有两种方法来解决这个问题：

一：通过windows对象来实现Canvas键盘事件监听与处理
// key event - use window as object

window.addEventListener('keydown', doKeyDown,**true**);

二：通过在Canvas对象上添加其它支持键盘事件的DOM元素实现键盘事件支持

<canvas
id=*"event_canvas"*tabindex=*"0"*></canvas>

// key event - use DOM element asobject

canvas.addEventListener('keydown', doKeyDown,**true**);

canvas.focus();  

其中tabindex为HTML5 DOM元素，支持键盘事件。

演示，一个可以根据键盘上下左右移动的矩形块:

![](https://img-blog.csdn.net/20130704140935750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


一个完整的鼠标与键盘事件演示代码如下：



```java
var tempContext = null; // global variable 2d context
		var started = false;
		var mText_canvas = null;
		var x = 0, y =0;
		window.add
		window.onload = function() {
			var canvas = document.getElementById("event_canvas");
			console.log(canvas.parentNode.clientWidth);
			canvas.width = canvas.parentNode.clientWidth;
			canvas.height = canvas.parentNode.clientHeight;
			
			if (!canvas.getContext) {
			    console.log("Canvas not supported. Please install a HTML5 compatible browser.");
			    return;
			}
			
			// get 2D context of canvas and draw rectangel
			tempContext = canvas.getContext("2d");
			tempContext.fillStyle="blue";
			x = canvas.width/2;
			y = canvas.height/2;
			tempContext.fillRect(x, y, 80, 40);
	
	        // key event - use DOM element as object
	        canvas.addEventListener('keydown', doKeyDown, true);
	        canvas.focus();  
	        // key event - use window as object
	        window.addEventListener('keydown', doKeyDown, true);
	        
	        // mouse event
	        canvas.addEventListener("mousedown", doMouseDown, false);
	        canvas.addEventListener('mousemove', doMouseMove, false);
	        canvas.addEventListener('mouseup',   doMouseUp, false);
		}
		
		function getPointOnCanvas(canvas, x, y) {
			var bbox = canvas.getBoundingClientRect();
			return { x: x - bbox.left * (canvas.width  / bbox.width),
					y: y - bbox.top  * (canvas.height / bbox.height)
					};
		}
		
		function doKeyDown(e) {
			var keyID = e.keyCode ? e.keyCode :e.which;
			if(keyID === 38 || keyID === 87)  { // up arrow and W
				clearCanvas();
				y = y - 10;
				tempContext.fillRect(x, y, 80, 40);
				e.preventDefault();
			}
			if(keyID === 39 || keyID === 68)  { // right arrow and D
				clearCanvas();
				x = x + 10;
				tempContext.fillRect(x, y, 80, 40);
				e.preventDefault();
			}
			if(keyID === 40 || keyID === 83)  { // down arrow and S
				clearCanvas();
				y = y + 10;
				tempContext.fillRect(x, y, 80, 40);
				e.preventDefault();
			}
			if(keyID === 37 || keyID === 65)  { // left arrow and A
				clearCanvas();
				x = x - 10;
				tempContext.fillRect(x, y, 80, 40);
				e.preventDefault();
			}
		}
		
		function clearCanvas() {
			tempContext.clearRect(0, 0, 500, 500)
		}
		
		function doMouseDown(event) {
			var x = event.pageX;
			var y = event.pageY;
			var canvas = event.target;
			var loc = getPointOnCanvas(canvas, x, y);
			console.log("mouse down at point( x:" + loc.x + ", y:" + loc.y + ")");
			tempContext.beginPath();
			tempContext.moveTo(loc.x, loc.y);
			started = true;
		}
		
		function doMouseMove(event) {
			var x = event.pageX;
			var y = event.pageY;
			var canvas = event.target;
			var loc = getPointOnCanvas(canvas, x, y);
			if (started) {
				tempContext.lineTo(loc.x, loc.y);
				tempContext.stroke();
			}
		}
		
		function doMouseUp(event) {
			console.log("mouse up now");
		    if (started) {
		    	doMouseMove(event);
		        started = false;
			}
		}
```
HTML部分：





```
<body>
	<h1>HTML Canvas Event Demo - By Gloomy Fish</h1>
	<pre>Press W, A, S, D keys to move</pre>
	<div id="my_painter">
		<canvas id="event_canvas" tabindex="0"></canvas>
	</div>
</body>
```
**觉得不错请支持一些，谢谢！！**



