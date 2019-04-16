# 提高HTML5 Canvas性能的技巧 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年07月02日 15:43:35[gloomyfish](https://me.csdn.net/jia20003)阅读数：8374
所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)









一：使用缓存技术实现预绘制，减少重复绘制Canvs内容

很多时候我们在Canvas上绘制与更新，总是会保留一些不变的内容，对于这些内容



应该预先绘制缓存，而不是每次刷新。

直接绘制代码如下：



```java
context.font="24px Arial";
context.fillStyle="blue";
context.fillText("Please press <Esc> to exit game",5,50);
requestAnimationFrame(render);
```



使用缓存预绘制技术：



```java
function render(context) {
	context.drawImage(mText_canvas, 0, 0);
	requestAnimationFrame(render);
}

function drawText(context) {
	mText_canvas = document.createElement("canvas");
	mText_canvas.width = 450;
	mText_canvas.height = 54;
	var m_context = mText_canvas.getContext("2d");
	m_context.font="24px Arial";
	m_context.fillStyle="blue";
	m_context.fillText("Please press <Esc> to exit game",5,50);
}
```



使用Canvas缓存绘制技术的时候，一定记得缓存Canvas对象大小要小于实际的Canvas

大小。尽量把绘制直线点的操作放在一起，而且尽量一次绘制完成，一个不好的代码如下：

```java
for (var i = 0; i < points.length - 1; i++) {
	  var p1 = points[i];
	  var p2 = points[i+1];
	  context.beginPath();
	  context.moveTo(p1.x, p1.y);
	  context.lineTo(p2.x, p2.y);
	  context.stroke();
}
```
**修改以后性能较高的代码如下：**


```java
context.beginPath();
for (var i = 0; i < points.length - 1; i++) {
  var p1 = points[i];
  var p2 = points[i+1];
  context.moveTo(p1.x, p1.y);
  context.lineTo(p2.x, p2.y);
}
context.stroke();
```
避免不必要的Canvas绘制状态频繁切换，一个频繁切换绘制style的例子如下：




```java
var GAP = 10;
for(var i=0; i<10; i++) {
	context.fillStyle = (i % 2 ? "blue" : "red");
	context.fillRect(0, i * GAP, 400, GAP);
}
```
避免频繁切换绘制状态，性能更好的绘制代码如下：




```java
// even
context.fillStyle = "red";
for (var i = 0; i < 5; i++) {
context.fillRect(0, (i*2) * GAP, 400, GAP);
}
			
// odd
context.fillStyle = "blue";
for (var i = 0; i < 5; i++) {
context.fillRect(0, (i*2+1) * GAP, 400, GAP);
}
```



绘制时，只绘制需要更新的区域，任何时候都要避免不必要的重复绘制与额外开销。

对于复杂的场景绘制使用分层绘制技术，分为前景与背景分别绘制。定义Canvas层的

HTML如下：



```
<canvas id="bg" width="640" height="480" style="position: absolute; z-index: 0">
</canvas>
<canvas id="fg" width="640" height="480" style="position: absolute; z-index: 1">
</canvas>
```



如果没有必要，要尽量避免使用绘制特效，如阴影，模糊等。




**避免使用浮点数坐标。**

绘制图形时，长度与坐标应选取整数而不是浮点数，原因在于Canvas支持半个像素绘制

会根据小数位实现插值算法实现绘制图像的反锯齿效果，如果没有必要请不要选择浮点数值。




**清空Canvas上的绘制内容：**

context.clearRect(0, 0, canvas.width,canvas.height)

但是其实在Canvas还有一种类似hack的清空方法：

canvas.width = canvas.width;

也可以实现清空canvas上内容的效果，但是在某些浏览器上可能不支持。



