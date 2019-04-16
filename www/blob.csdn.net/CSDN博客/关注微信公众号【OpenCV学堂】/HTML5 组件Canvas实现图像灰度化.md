# HTML5 组件Canvas实现图像灰度化 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年02月02日 15:28:40[gloomyfish](https://me.csdn.net/jia20003)阅读数：5867
所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)









HTML5发布已经有很长一段时间了，一直以来从来没有仔细的看过，过年刚来随便看看

发现HTML5中的Canvas组件功能是如此的强大，不怪很多牛人预言Flash已死，死不死


不是我要关心的，我关心的Canvas可以很轻松在网页中实现简单相框和图像灰度化。


一起来看看HTML5 Canvas是怎么做到的吧！


**1. 新建一个html页面，在body tag之间加入**



`<canvas id="myCanvas" >Gray Filter</canvas>`**2. 添加一段最简单的JavaScript代码：**
```java
window.onload = function() {  
    var canvas = document.getElementById("myCanvas");
     // do something here!!
}
```
**从Canvas对象获取绘制对象上下文Context的代码如下：**





```java
var context = canvas.getContext("2d");
```
**在html页面中加入一幅图像的html代码如下:**



`<img id="imageSource" src="hanjiaren.jpg" alt="Canvas Source" />`


**从html img对象中获取image 对象的javascript代码如下：**




```java
var image = document.getElementById("imageSource");
```
**将得到的图像绘制在Canvas对象中的代码如下:**


```java
context.drawImage(image, 0, 0);
```

**从Canvas对象中获取图像像素数据的代码如下：**



```java
var canvasData = context.getImageData(0, 0, canvas.width, canvas.height);
```
**读取像素值与实现灰度计算的代码如下：**


```java
for ( var x = 0; x < canvasData.width; x++) {  
        for ( var y = 0; y < canvasData.height; y++) {  
  
            // Index of the pixel in the array  
            var idx = (x + y * canvasData.width) * 4;  
            var r = canvasData.data[idx + 0];  
                var g = canvasData.data[idx + 1];  
                var b = canvasData.data[idx + 2];  
                  
                // calculate gray scale value  
                var gray = .299 * r + .587 * g + .114 * b;  
                  
            // assign gray scale value  
            canvasData.data[idx + 0] = gray; // Red channel  
            canvasData.data[idx + 1] = gray; // Green channel  
            canvasData.data[idx + 2] = gray; // Blue channel  
            canvasData.data[idx + 3] = 255; // Alpha channel  
                  
            // add black border  
            if(x < 8 || y < 8 || x > (canvasData.width - 8) || y > (canvasData.height - 8))   
            {  
                canvasData.data[idx + 0] = 0;  
                canvasData.data[idx + 1] = 0;  
                canvasData.data[idx + 2] = 0;  
            }  
        }  
}
```
其中计算灰度公式为 gray color = 0.299 × red color + 0.578 × green color + 0.114 * blue color
读取出来的像素值顺序为RGBA 分别代表red color, green color, blue color, alpha channel

处理完成的数据要重新载入到Canvas中。代码如下：



```java
context.putImageData(canvasData, 0, 0);
```
**程序最终的效果如下：**
![](http://hi.csdn.net/attachment/201202/2/0_1328167545PBci.gif)
完全源代码如下：



```
<html>  
<head>  
<script>  
window.onload = function() {  
        var canvas = document.getElementById("myCanvas");  
        var image = document.getElementById("imageSource");  
          
        // re-size the canvas deminsion  
        canvas.width  = image.width;  
        canvas.height = image.height;  
          
        // get 2D render object  
        var context = canvas.getContext("2d");  
        context.drawImage(image, 0, 0);  
        var canvasData = context.getImageData(0, 0, canvas.width, canvas.height);  
        alert(canvasData.width.toString());  
        alert(canvasData.height.toString());  
          
        // gray filter  
        for ( var x = 0; x < canvasData.width; x++) {  
            for ( var y = 0; y < canvasData.height; y++) {  
  
                // Index of the pixel in the array  
                var idx = (x + y * canvasData.width) * 4;  
                var r = canvasData.data[idx + 0];  
                var g = canvasData.data[idx + 1];  
                var b = canvasData.data[idx + 2];  
                  
                // calculate gray scale value  
                var gray = .299 * r + .587 * g + .114 * b;  
                  
                // assign gray scale value  
                canvasData.data[idx + 0] = gray; // Red channel  
                canvasData.data[idx + 1] = gray; // Green channel  
                canvasData.data[idx + 2] = gray; // Blue channel  
                canvasData.data[idx + 3] = 255; // Alpha channel  
                  
                // add black border  
                if(x < 8 || y < 8 || x > (canvasData.width - 8) || y > (canvasData.height - 8))   
                {  
                    canvasData.data[idx + 0] = 0;  
                    canvasData.data[idx + 1] = 0;  
                    canvasData.data[idx + 2] = 0;  
                }  
            }  
        }  
        context.putImageData(canvasData, 0, 0); // at coords 0,0  
    };  
</script>  
</head>  
<body>  
    <h2>Hello World!</h2>  
    <img id="imageSource" src="hanjiaren.jpg" alt="Canvas Source" />  
    <br />  
    <canvas id="myCanvas" >Gray Filter</canvas>  
</body>  
</html>
```
代码中的文件可以替换任意你想要看到的图片文件,HTML5, 原来如此神奇。
程序在google浏览器中测试通过千万不要在本地尝试运行上面的代码，google

浏览器的安全检查会自动阻止从浏览器中读写非domain的文件最好在tomcat或

者任意个web container的server上发布以后从google浏览器查看效果即可。



