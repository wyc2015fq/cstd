# HTML5 Canvas中实现绘制一个像素宽的细线 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年07月25日 23:10:05[gloomyfish](https://me.csdn.net/jia20003)阅读数：19642
所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)









正统的HTML5 Canvas中如下代码

```java
ctx.lineWidth = 1;
ctx.beginPath();
ctx.moveTo(10, 100);
ctx.lineTo(300,100);
ctx.stroke();
```
运行结果绘制出来的并不是一个像素宽度的线
![](https://img-blog.csdn.net/20130725225821875)


感觉怎么好粗啊，跟常常见到的网页版各种绘制线效果

很不一样，难道HTML5 Canvas就没想到搞好点嘛

其实这个根本原因在于Canvas的绘制不是从中间开始的

而是从0～1，不是从0.5～1 + 0～0.5的绘制方式，所以

导致fade在边缘，看上去线很宽。

解决方法有两个，一个是错位覆盖法，另外一种是中心

平移(0.5,0.5)。实现代码如下：

错位覆盖法我已经包装成一个原始context的函数



```java
/**
 * <p> draw one pixel line </p>
 * @param fromX
 * @param formY
 * @param toX
 * @param toY
 * @param backgroundColor - default is white
 * @param vertical - boolean
 */
CanvasRenderingContext2D.prototype.onePixelLineTo = function(fromX, fromY, toX, toY, backgroundColor, vertical) {
	var currentStrokeStyle = this.strokeStyle;
	this.beginPath();
	this.moveTo(fromX, fromY);
	this.lineTo(toX, toY);
	this.closePath();
	this.lineWidth=2;
	this.stroke();
	this.beginPath();
	if(vertical) {
		this.moveTo(fromX+1, fromY);
		this.lineTo(toX+1, toY);
	} else {
		this.moveTo(fromX, fromY+1);
		this.lineTo(toX, toY+1);
	}
	this.closePath();
	this.lineWidth=2;
	this.strokeStyle=backgroundColor;
	this.stroke();
	this.strokeStyle = currentStrokeStyle;
};
```
中心平移法代码如下：



```java
ctx.save();
	ctx.translate(0.5,0.5);
	ctx.lineWidth = 1;
	ctx.beginPath();
	ctx.moveTo(10, 100);
	ctx.lineTo(300,100);
	ctx.stroke();
	ctx.restore();
```
要特别注意确保你的所有坐标点是整数，否则HTML5会自动实现边缘反锯齿
又导致你的一个像素直线看上去变粗了。

运行效果：

![](https://img-blog.csdn.net/20130725230816421)


现在效果怎么样，这个就是HTML5 Canvas画线的一个小技巧

觉得不错请顶一下。



