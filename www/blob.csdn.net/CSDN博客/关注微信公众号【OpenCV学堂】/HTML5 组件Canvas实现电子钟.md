# HTML5 组件Canvas实现电子钟 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年02月23日 22:38:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：5143标签：[html5																[function																[image																[google																[浏览器																[date](https://so.csdn.net/so/search/s.do?q=date&t=blog)
个人分类：[J2EE																[HTML5](https://blog.csdn.net/jia20003/article/category/1070101)](https://blog.csdn.net/jia20003/article/category/871633)

所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)





**基本思路：**

首先绘制一个矩形背景，设置颜色为灰色。在背景上绘制一个简单的矩形外边框，然后再绘

制一个内边框，接着加载选定的图像做为电子钟内部的背景图片。然后开始绘制时钟刻度，

绘制分钟刻度，最后获取当前系统时间，绘制时分秒三个手柄。



**技术要点：**

使用HTML5的Canvas 2D绘制对象，主要使用context.save()与context.restore()方法来保存

绘制状态和重置绘制状态，使用Transform和fillRect()方法来绘制时钟和分钟刻度。使用

drawImage()方法来绘制背景图片，使用setTimeout()方法来刷新时间显示。



**代码详解：**

** 获取HTML5 Canvas绘制对象的代码如下:**

****

```
var canvas = document.getElementById("canvas1");
ctx = canvas.getContext("2d");
ctx.clearRect(0, 0, 500, 500);
```
**绘制时钟刻度的代码如下：**




```
var sin = Math.sin(Math.PI/6);  
		    var cos = Math.cos(Math.PI/6); 
		    ctx.translate(245, 245);
		    for (var i=0; i <= 12; i++) {  
		    	// top
				ctx.fillRect(160,-7.5,30,10);
				ctx.strokeRect(160,-7.5,30,10);
				ctx.transform(cos, sin, -sin, cos, 0, 0);  	
		    }
```
**绘制分钟分钟刻度的代码如下：**




```
var sin = Math.sin(Math.PI/30);  
		    var cos = Math.cos(Math.PI/30); 
		    for (var i=0; i <= 60; i++) {  
				ctx.fillRect(170,-5,10,2);
				ctx.transform(cos, sin, -sin, cos, 0, 0); 	
		    }
```



**保存制状态代码如下：**



```
ctx.translate(245, 245);
ctx.save();
```
**恢复绘制状态代码如下：**



`ctx.restore();`



**运行效果如下：**

![](http://hi.csdn.net/attachment/201202/23/0_13300078301X3o.gif)




**程序完全源代码如下：**


```
<html>
<head>
<script>
	window.onload = function() {
		clockHand();
	};
	
	function clockHand() {
		var canvas = document.getElementById("canvas1");
		ctx = canvas.getContext("2d");
		ctx.clearRect(0, 0, 500, 500);
		
		// create background rectangle
		// ctx.lineWidth = 10;  
		ctx.fillStyle = "gray";
		ctx.fillRect(0,0,500,500);
		
		// draw frame
		ctx.lineWidth = 10;  
		ctx.strokeStyle = "green";
		ctx.strokeRect(0,0,500,500);
		
		// draw author infomation
		ctx.fillStyle = "blue";
		ctx.font = "20px Times New Roman";
		ctx.fillText("-created by gloomyfish", 150, 30);
		
		// draw inner rectangle
		ctx.lineWidth = 10;  
		ctx.strokeStyle = "black";
		ctx.strokeRect(45,45,400,400);
		
		// create background image
		var img=new Image();
		img.src="background.png";
		img.onload = function() { 
		    ctx.drawImage(img,45,45,400,400);
		    ctx.save();
			// draw marker unit
			ctx.lineWidth = 2;
		    ctx.fillStyle = "purple";
		    ctx.strokeStyle = "black";
		    var sin = Math.sin(Math.PI/6);  
		    var cos = Math.cos(Math.PI/6); 
		    ctx.translate(245, 245);
		    for (var i=0; i <= 12; i++) {  
		    	// top
				ctx.fillRect(160,-7.5,30,10);
				ctx.strokeRect(160,-7.5,30,10);
				ctx.transform(cos, sin, -sin, cos, 0, 0);  	
		    }
		    
		    // transform back center point
		    
		    // ctx.translate(245, 245);
		    var sin = Math.sin(Math.PI/30);  
		    var cos = Math.cos(Math.PI/30); 
		    for (var i=0; i <= 60; i++) {  
				ctx.fillRect(170,-5,10,2);
				ctx.transform(cos, sin, -sin, cos, 0, 0); 	
		    }
		    ctx.restore();
		    // top
			ctx.fillText("12", 233,100);
			
			// bottom
			ctx.fillText("6", 240,400);
			
			// left
			ctx.fillText("9", 90,252);
			
			// right
			ctx.fillText("3", 395,250);
			
			// get time
			ctx.save();
			ctx.translate(245, 245);
			ctx.save();
			
			// dynamic show time
			var now=new Date();
			var hrs=now.getHours();
			var min=now.getMinutes();
			var sec=now.getSeconds();

			//Draw hour hand
			ctx.rotate(Math.PI/6*(hrs+(min/60)+(sec/3600)));
			ctx.beginPath();
			ctx.moveTo(0,10);
			ctx.lineTo(0,-60);
			ctx.stroke();
			ctx.restore();
			ctx.save();
			
			//Draw minute hand
			ctx.rotate(Math.PI/30*(min+(sec/60)));
			ctx.beginPath();
			ctx.moveTo(0,20);
			ctx.lineTo(0,-110);
			ctx.stroke();
			ctx.restore();
			ctx.save();
			
			//Draw second hand
			ctx.rotate(Math.PI/30*sec);
			ctx.strokeStyle="#E33";
			ctx.lineWidth = 2;
			ctx.beginPath();
			ctx.moveTo(0,20);
			ctx.lineTo(0,-110);
			ctx.stroke();
			ctx.restore();
			
			// finally store to originall point
			ctx.restore();
			setTimeout(clockHand,1000);
		};
	}
</script>
</head>
<body bgcolor="#E6E6FA">
	<canvas id="canvas1" width="500" height="500">electronic clock</canvas>
</body>
</html>
```

**不足之处：**


每次都刷新加载image对象不怎么好，我是在google浏览器中测试的，建议在

google浏览器中运行上面代码。](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=html5&t=blog)




