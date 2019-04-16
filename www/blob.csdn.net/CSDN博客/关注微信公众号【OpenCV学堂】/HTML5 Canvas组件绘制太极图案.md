# HTML5 Canvas组件绘制太极图案 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年02月03日 15:37:53[gloomyfish](https://me.csdn.net/jia20003)阅读数：5216
所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)












**一实现思路：**


实现原理主要是利用HTML5的Canvas组件提供的path函数功能来绘制圆，首先绘


制两个半圆，分别为黑色和白色，组成一个圆，绘制完成以后再分别绘制一个黑色


和白色的圆在绘制好的黑白圆之内，半径恰好是黑白大圆一半。 最后在绘制好的两


个黑白小圆内分别填充白色和黑色的点，半径大小为10pixel左右。




**二程序效果如下：**

![](http://hi.csdn.net/attachment/201202/3/0_13282538831M5w.gif)


**三关键程序解析：**




绘制半圆的程序，其中200，200表示开始绘制圆心点坐标，第三个参数150表示绘制圆的半径


第四个参数表示开始角度，第五个参数表示结束角度，最后一个参数表示是否为顺时针或者逆时针


绘制白色半圆的代码如下：



```java
ctx.fillStyle="#fff";
		ctx.beginPath(); 
		ctx.arc(200, 200, 150, 1.5*Math.PI, Math.PI/2, false);
		ctx.closePath();
		ctx.fill();
```


绘制黑色半圆的代码如下：





```java
ctx.fillStyle="#000";
		ctx.beginPath(); 
		ctx.arc(200, 200, 150, Math.PI/2, 1.5*Math.PI, false);
		ctx.closePath();
		ctx.fill();
```


在太极图案中添加文字的代码使用了透明处理，Canvas全局透明度设置函数




如下：



```java
// set transparency value  
		ctx.globalAlpha = 0.2;
```


绘制文字的代码如下：





```java
// Draw semi transparent text
		ctx.fillStyle = "#f00";
		ctx.font = "32pt Arial";
		ctx.fillText("Hello", 220, 200);
		ctx.fillText("Canvas", 100, 250);
```


程序完全JavaScript代码如下：





```java
window.onload = function() {
		var cvs = document.getElementById("canvas-path");
		ctx = cvs.getContext("2d");

		// Create circle, radius = 150
		// start point(x, y), radius, start angle, end angle, boolean antiClockWise
		ctx.fillStyle="#fff";
		ctx.beginPath(); 
		ctx.arc(200, 200, 150, 1.5*Math.PI, Math.PI/2, false);
		ctx.closePath();
		ctx.fill();
		ctx.fillStyle="#000";
		ctx.beginPath(); 
		ctx.arc(200, 200, 150, Math.PI/2, 1.5*Math.PI, false);
		ctx.closePath();
		ctx.fill();
		
		// draw sub circle
		// start point(x, y), radius, start angle, end angle, boolean antiClockWise
		ctx.fillStyle="#000";
		ctx.beginPath(); 
		ctx.arc(200, 275, 75, 0, Math.PI*2, false); 
		ctx.closePath();
		ctx.fill();
		ctx.fillStyle="#fff";
		ctx.beginPath(); 
		ctx.arc(200, 125, 75, 0, Math.PI*2, false);
		ctx.closePath();
		ctx.fill();
		
		// fill black and white point
		ctx.fillStyle="#fff";
		ctx.beginPath(); 
		ctx.arc(200, 275, 10, 0, Math.PI*2, false);
		ctx.closePath();
		ctx.fill();
		ctx.fillStyle="#000";
		ctx.beginPath(); 
		ctx.arc(200, 125, 10, 0, Math.PI*2, false);
		ctx.closePath();
		ctx.fill();
		
		// set transparency value  
		ctx.globalAlpha = 0.2;  
		  
		// Draw semi transparent text
		ctx.fillStyle = "#f00";
		ctx.font = "32pt Arial";
		ctx.fillText("Hello", 220, 200);
		ctx.fillText("Canvas", 100, 250);
		ctx.globalAlpha = 1.0; 
		ctx.shadowOffsetX = 2;  
		ctx.shadowOffsetY = 2;  
		ctx.shadowBlur = 2;  
		ctx.shadowColor = "rgba(0, 0, 0, 0.5)";  
		ctx.fillStyle = "#000";
		ctx.font = "20px Times New Roman";
		ctx.fillText("-created by gloomyfish", 100, 30);
	};
```


我为什么要在插图上加上我的名字，因为发现文章被转载的时候居然没有被标出来！








