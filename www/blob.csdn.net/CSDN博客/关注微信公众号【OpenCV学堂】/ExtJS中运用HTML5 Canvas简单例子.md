# ExtJS中运用HTML5 Canvas简单例子 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年04月13日 15:00:34[gloomyfish](https://me.csdn.net/jia20003)阅读数：9415








**在ExtJS的Panel组件上使用HTML5的Canvas对象画图**

**的简单例子，效果如下：**

![](https://img-my.csdn.net/uploads/201204/13/1334300467_7396.png)


**怎么运行源代码：**

**新建两个文件，分别命名为mydemo.html, mydemo.js以后，将对应的HTML源代码**

**与JavaScript代码copy到各自的文件中，在同一目录下使用Google Chrome浏览器**

**或者IE9.0打开html文件即可看到效果！**




**HTML的代码如下：**



```
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
    <title>Example</title>
    <link rel="stylesheet" type="text/css" href="../../resources/css/ext-all.css" />
    <link rel="stylesheet" type="text/css" href="../shared/example.css" />
    <script type="text/javascript" src="../../bootstrap.js"></script>
    <script language="javascript" src="mydemo.js"></script>
</head>
<body>
    <h1>ExtJS with HTML5 Demo</h1> 
    <p>The js is not minified so it is readable. See <a href="mydemo.js">source code</a>.</p> 
	<div id="my-demo"></div>
</body>
</html>
```
**ExtJS的代码如下：**




```java
/**
 * HTML5 Canvas Demo
 */
// create namespace
Ext.namespace('Test');
 
// create application
Test.app = function() {

  return {
    // public methods
    init: function() {
      
    
    var grid = new Ext.Panel({
      renderTo: 'my-demo',
      title:'Simple HTML5 Canvas Demo',
      bodyStyle: 'padding: 10px;',
      borders: true,
      plain: true,
      xtype: 'panel',
      width:400,
	  height:400,
	  html: '<canvas id="canvas" width="400" height="400"></canvas>'
    });
  
    }, // end of init
    
  onDraw: function() {
  	    this.canvas = document.getElementById('canvas');
        this.ctx = this.canvas.getContext("2d");

		// create a blank image data
		var canvas2Data = this.ctx.createImageData(this.canvas.width, this.canvas.height);
		for ( var x = 0; x < canvas2Data.width; x++) {
			for ( var y = 0; y < canvas2Data.height; y++) {
 
				// Index of the pixel in the array
				var idx = (x + y * canvas2Data.width) * 4;
				
				// assign gray scale value
				var distance = Math.sqrt((x - canvas2Data.width / 2) * (x - canvas2Data.width / 2) + (y - canvas2Data.height / 2) * (y - canvas2Data.height / 2));
				var cvalue = (128.0 + (128.0 * Math.sin(distance / 8.0)));
				canvas2Data.data[idx + 0] = cvalue; // Red channel
				canvas2Data.data[idx + 1] = cvalue; // Green channel
				canvas2Data.data[idx + 2] = cvalue; // Blue channel
				canvas2Data.data[idx + 3] = 255; // Alpha channel
			}
		}
		this.ctx.putImageData(canvas2Data, 0, 0); // at coords 0,0
		
        // draw author infomation
		this.ctx.fillStyle = "red";
		this.ctx.font = "24px Times New Roman";
		this.ctx.fillText("HTML5 Demo - by gloomyfish ", 50, 60);  
 
  }
  };
}(); 
// end of app
Ext.onReady(function(){
	Test.app.init();
	Test.app.onDraw()
    // alert('ext.onready')
});
// Ext.onReady(Test.app.init, Test.app);
```

**一定要有ExtJS的库文件支持！！！**






