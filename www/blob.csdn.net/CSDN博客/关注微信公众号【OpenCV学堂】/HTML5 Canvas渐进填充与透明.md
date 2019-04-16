# HTML5 Canvas渐进填充与透明 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年07月05日 14:35:20[gloomyfish](https://me.csdn.net/jia20003)阅读数：27964
所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)









详细解释HTML5 Canvas中渐进填充的参数设置与使用，Canvas中透明度的设置与使

用，结合渐进填充与透明度支持，实现图像的Mask效果。




**一：渐进填充(Gradient Fill)**

Canvas支持两种渐进填充方式，一种为线性渐进填充(Line Gradient Fill)，另外一种称

为经向渐变填充(RadialGradient Fill)。其API分别为：

createLinearGradient(x1, y1, x2, y2);

其中x1,y1为第一个点坐标，x2,y2为第二个点坐标。

createRadialGradient(x1, y1, r1, x2, y2, r2);

其中x1, y1为第一个中心点坐标,r1为半径，x2,
 y2为第二个中心点坐标，r2为半径。

为每个点设置颜色

addColorStop(position, color);

其中position表示位置，大小范围[0~1]其中0表示第一个点，1表示第二个点坐标

Color表示颜色值，任何CSS的颜色值。

渐进填充对象创建与配置之后可以用来设置context的strokeStyle与fillStyle实现文字，

几何形状的渐进颜色填充。

线性渐进方式的代码演示：

1.      垂直(Y)方向颜色渐进



```java
// vertical/Y direction
var lineGradient = ctx.createLinearGradient (50, 0, 50, 200);
lineGradient.addColorStop(0, 'rgba(255, 0, 0, 1)');  
lineGradient.addColorStop(1, 'rgba(255, 255, 0, 1)');  
ctx.fillStyle = lineGradient;  
ctx.fillRect(0, 0, 300, 300);
```
![](https://img-blog.csdn.net/20130705142702500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




2. 水平(X)方向颜色渐进

```java
// horizontal/X direction
var lineGradient = ctx.createLinearGradient (0, 50, 200, 50);
lineGradient.addColorStop(0, 'rgba(255, 0, 0, 1)');  
lineGradient.addColorStop(1, 'rgba(255, 255, 0, 1)');  
ctx.fillStyle = lineGradient;  
ctx.fillRect(0, 0, 300, 300);
```
![](https://img-blog.csdn.net/20130705142832875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3.垂直与水平同时(XY方向)颜色渐进



```java
// vertical and horizontal direction
var lineGradient = ctx.createLinearGradient (0, 0, 200, 200);
lineGradient.addColorStop(0, 'rgba(255, 0, 0, 1)');  
lineGradient.addColorStop(1, 'rgba(255, 255, 0, 1)');  
ctx.fillStyle = lineGradient;  
ctx.fillRect(0, 0, 300, 300);
```
![](https://img-blog.csdn.net/20130705142954750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




**二：透明度(Transparent)**

Canvas中透明度支持分为全局与局部透明设置，全局透明度的设置可以通过设置

Context.globalAlpha来实现。局部透明度可以通过fillStyle设置颜色值中alpha值通道

来实现。两种方式代码如下：

// change global alpha value

ctx.globalAlpha=0.5;

ctx.fillRect(50,50,75,50);

// change fill style color's alphachannel

ctx.fillStyle = 'rgba(225,225,225,0.5)';

ctx.fillRect(50,50,75,50);

两个效果是一样的。

**三：照片透明渐进Mask效果**

使用径向颜色渐变与透明度变化，实现在图像上的半透明面罩效果，脚本运行效果：
![](https://img-blog.csdn.net/20130705143155609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


```java
var myImage = document.createElement('img');
myImage.src = "../test.png";
myImage.onload = function() {
	ctx.drawImage(myImage, 80, 30, myImage.naturalWidth, myImage.naturalHeight);				
	var radialGradient = ctx.createRadialGradient (canvas.width/2, canvas.height/2, 10, canvas.width/2, canvas.height/2, 200);
	radialGradient.addColorStop(0, 'rgba(247, 247, 247, 0)');
	radialGradient.addColorStop(0.7, 'rgba(120, 120, 120, 0.5)');
	radialGradient.addColorStop(0.9, 'rgba(0, 0, 0, 0.8)');
	radialGradient.addColorStop(1, 'rgba(238, 238, 238, 1)');
	ctx.beginPath();
	ctx.arc(canvas.width/2, canvas.height/2, 300, 0, Math.PI * 2, true);
	ctx.closePath();
	ctx.fillStyle = radialGradient;
	ctx.fill();
}
```





