# jquery 单行滚动、批量多行滚动、文字图片翻屏滚动效果代码 - z69183787的专栏 - CSDN博客
2014年03月10日 14:34:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4798

以下代码，运行后，需要刷新下，才能加载jquery,要不然看不到效果。
**1、单行滚动效果**
![](http://www.poluoluo.com/jzxy/UploadFiles_333/201110/2011101614464322.gif)
```java
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml"> 
<head> 
<meta http-equiv="Content-Type" c /> 
<title>无标题文档</title> 
<style type="text/css"> 
ul,li{margin:0;padding:0} 
#scrollDiv{width:300px;height:25px;line-height:25px;border:#ccc 1px solid;overflow:hidden} 
#scrollDiv li{height:25px;padding-left:10px;} 
</style> 
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js" type="text/javascript"></script> 
<script type="text/javascript"> 
function AutoScroll(obj){ 
$(obj).find("ul:first").animate({ 
marginTop:"-25px" 
},500,function(){ 
$(this).css({marginTop:"0px"}).find("li:first").appendTo(this); 
}); 
} 
$(document).ready(function(){ 
setInterval('AutoScroll("#scrollDiv")',1000) 
}); 
</script> 
</head> 
<body> 
<div id="scrollDiv"> 
<ul> 
<li>百度 www.baidu.com</li> 
<li>破洛洛 www.poluoluo.com</li> 
<li>这是公告标题的第三行</li> 
<li>这是公告标题的第四行</li> 
<li>这是公告标题的第五行</li> 
<li>这是公告标题的第六行</li> 
<li>这是公告标题的第七行</li> 
<li>这是公告标题的第八行</li> 
</ul> 
</div> 
</body> 
</html>
```
**二,多行滚动效果**
![](http://www.poluoluo.com/jzxy/UploadFiles_333/201110/2011101614464345.gif)
```java
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml"> 
<head> 
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" /> 
<title>无标题文档</title> 
<style type="text/css"> 
ul,li{margin:0;padding:0} 
#scrollDiv{width:300px;height:100px;min-height:25px;line-height:25px;border:#ccc 1px solid;overflow:hidden} 
#scrollDiv li{height:25px;padding-left:10px;} 
</style> 
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js" type="text/javascript"></script> 
<script type="text/javascript"> 
//滚动插件 
(function($){ 
$.fn.extend({ 
Scroll:function(opt,callback){ 
//参数初始化 
if(!opt) var opt={}; 
var _this=this.eq(0).find("ul:first"); 
var lineH=_this.find("li:first").height(), //获取行高 
line=opt.line?parseInt(opt.line,10):parseInt(this.height()/lineH,10), //每次滚动的行数，默认为一屏，即父容器高度 
speed=opt.speed?parseInt(opt.speed,10):500, //卷动速度，数值越大，速度越慢（毫秒） 
timer=opt.timer?parseInt(opt.timer,10):3000; //滚动的时间间隔（毫秒） 
if(line==0) line=1; 
var upHeight=0-line*lineH; 
//滚动函数 
scrollUp=function(){ 
_this.animate({ 
marginTop:upHeight 
},speed,function(){ 
for(i=1;i<=line;i++){ 
_this.find("li:first").appendTo(_this); 
} 
_this.css({marginTop:0}); 
}); 
} 
//鼠标事件绑定 
_this.hover(function(){ 
clearInterval(timerID); 
},function(){ 
timerID=setInterval("scrollUp()",timer); 
}).mouseout(); 
} 
}) 
})(jQuery); 
$(document).ready(function(){ 
$("#scrollDiv").Scroll({line:4,speed:500,timer:1000}); 
}); 
</script> 
</head> 
<body> 
<p>多行滚动演示：</p> 
<div id="scrollDiv"> 
<ul> 
<li>百度 www.baidu.com</li> 
<li>破洛洛 www.poluoluo.com</li> 
<li>这是公告标题的第三行</li> 
<li>这是公告标题的第四行</li> 
<li>这是公告标题的第五行</li> 
<li>这是公告标题的第六行</li> 
<li>这是公告标题的第七行</li> 
<li>这是公告标题的第八行</li> 
</ul> 
</div> 
</body> 
</html>
```
**三、可控制向前向后的多行滚动**
![](http://www.poluoluo.com/jzxy/UploadFiles_333/201110/2011101614464344.gif)
```java
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml"> 
<head> 
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" /> 
<title>无标题文档</title> 
<style type="text/css"> 
ul,li{margin:0;padding:0} 
#scrollDiv{width:300px;height:100px;min-height:25px;line-height:25px;border:#ccc 1px solid;overflow:hidden} 
#scrollDiv li{height:25px;padding-left:10px;} 
</style> 
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js" type="text/javascript"></script> 
<script type="text/javascript"> 
(function($){ 
$.fn.extend({ 
Scroll:function(opt,callback){ 
//参数初始化 
if(!opt) var opt={}; 
var _btnUp = $("#"+ opt.up);//Shawphy:向上按钮 
var _btnDown = $("#"+ opt.down);//Shawphy:向下按钮 
var timerID; 
var _this=this.eq(0).find("ul:first"); 
var lineH=_this.find("li:first").height(), //获取行高 
line=opt.line?parseInt(opt.line,10):parseInt(this.height()/lineH,10), //每次滚动的行数，默认为一屏，即父容器高度 
speed=opt.speed?parseInt(opt.speed,10):500; //卷动速度，数值越大，速度越慢（毫秒） 
timer=opt.timer //?parseInt(opt.timer,10):3000; //滚动的时间间隔（毫秒） 
if(line==0) line=1; 
var upHeight=0-line*lineH; 
//滚动函数 
var scrollUp=function(){ 
_btnUp.unbind("click",scrollUp); //Shawphy:取消向上按钮的函数绑定 
_this.animate({ 
marginTop:upHeight 
},speed,function(){ 
for(i=1;i<=line;i++){ 
_this.find("li:first").appendTo(_this); 
} 
_this.css({marginTop:0}); 
_btnUp.bind("click",scrollUp); //Shawphy:绑定向上按钮的点击事件 
}); 
} 
//Shawphy:向下翻页函数 
var scrollDown=function(){ 
_btnDown.unbind("click",scrollDown); 
for(i=1;i<=line;i++){ 
_this.find("li:last").show().prependTo(_this); 
} 
_this.css({marginTop:upHeight}); 
_this.animate({ 
marginTop:0 
},speed,function(){ 
_btnDown.bind("click",scrollDown); 
}); 
} 
//Shawphy:自动播放 
var autoPlay = function(){ 
if(timer)timerID = window.setInterval(scrollUp,timer); 
}; 
var autoStop = function(){ 
if(timer)window.clearInterval(timerID); 
}; 
//鼠标事件绑定 
_this.hover(autoStop,autoPlay).mouseout(); 
_btnUp.css("cursor","pointer").click( scrollUp ).hover(autoStop,autoPlay);//Shawphy:向上向下鼠标事件绑定 
_btnDown.css("cursor","pointer").click( scrollDown ).hover(autoStop,autoPlay); 
} 
}) 
})(jQuery); 
$(document).ready(function(){ 
$("#scrollDiv").Scroll({line:4,speed:500,timer:1000,up:"btn1",down:"btn2"}); 
}); 
</script> 
</head> 
<body> 
<p>多行滚动演示：</p> 
<div id="scrollDiv"> 
<ul> 
<li>这是公告标题的第一行</li> 
<li>这是公告标题的第二行</li> 
<li>这是公告标题的第三行</li> 
<li>这是公告标题的第四行</li> 
<li>这是公告标题的第五行</li> 
<li>这是公告标题的第六行</li> 
<li>这是公告标题的第七行</li> 
<li>这是公告标题的第八行</li> 
</ul> 
</div> 
<span id="btn1">向前</span>  <span id="btn2">向后</span> 
</body> 
</html>
```
