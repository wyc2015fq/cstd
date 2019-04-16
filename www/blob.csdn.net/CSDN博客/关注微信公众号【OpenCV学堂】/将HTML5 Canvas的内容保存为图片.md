# 将HTML5 Canvas的内容保存为图片 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年05月19日 21:44:01[gloomyfish](https://me.csdn.net/jia20003)阅读数：79195
所属专栏：[HTML5 Canvas编程](https://blog.csdn.net/column/details/canvas-programming.html)









主要思想是借助Canvas自己的API - toDataURL()来实现，整个实现

HTML + JavaScript的代码很简单。



```
<html>
<meta http-equiv="X-UA-Compatible" content="chrome=1">
<head>
<script>
    	window.onload = function() {
    		draw();
    		var saveButton = document.getElementById("saveImageBtn");
    		bindButtonEvent(saveButton, "click", saveImageInfo);
    		var dlButton = document.getElementById("downloadImageBtn");
    		bindButtonEvent(dlButton, "click", saveAsLocalImage);
    	};
            function draw(){
                var canvas = document.getElementById("thecanvas");
                var ctx = canvas.getContext("2d");
                ctx.fillStyle = "rgba(125, 46, 138, 0.5)";
                ctx.fillRect(25,25,100,100); 
                ctx.fillStyle = "rgba( 0, 146, 38, 0.5)";
                ctx.fillRect(58, 74, 125, 100);
                ctx.fillStyle = "rgba( 0, 0, 0, 1)"; // black color
                ctx.fillText("Gloomyfish - Demo", 50, 50);
            }
            
            function bindButtonEvent(element, type, handler)
            {
            	   if(element.addEventListener) {
            	      element.addEventListener(type, handler, false);
            	   } else {
            	      element.attachEvent('on'+type, handler);
            	   }
			}
            
            function saveImageInfo () 
            {
            	var mycanvas = document.getElementById("thecanvas");
            	var image    = mycanvas.toDataURL("image/png");
            	var w=window.open('about:blank','image from canvas');
            	w.document.write("<img src='"+image+"' alt='from canvas'/>");
            }

            function saveAsLocalImage () {
            	var myCanvas = document.getElementById("thecanvas");
        		// here is the most important part because if you dont replace you will get a DOM 18 exception.
        		// var image = myCanvas.toDataURL("image/png").replace("image/png", "image/octet-stream;Content-Disposition: attachment;filename=foobar.png");
        		var image = myCanvas.toDataURL("image/png").replace("image/png", "image/octet-stream"); 
        		window.location.href=image; // it will save locally
        	}
        </script>
</head>
<body bgcolor="#E6E6FA">
	<div>
		<canvas width=200 height=200 id="thecanvas"></canvas>
		<button id="saveImageBtn">Save Image</button>
		<button id="downloadImageBtn">Download Image</button>
	</div>
</body>
</html>
```

**运行效果如下：**

![](https://img-blog.csdn.net/20130519214612817)







