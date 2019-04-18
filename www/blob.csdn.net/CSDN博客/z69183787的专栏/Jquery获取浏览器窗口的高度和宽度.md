# Jquery获取浏览器窗口的高度和宽度 - z69183787的专栏 - CSDN博客
2012年10月24日 20:39:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：779
<script type="text/javascript"> 
$(document).ready(function() 
{ 
alert($(window).height()); //浏览器时下窗口可视区域高度 
alert($(document).height()); //浏览器时下窗口文档的高度 
alert($(document.body).height());//浏览器时下窗口文档body的高度 
alert($(document.body).outerHeight(true));//浏览器时下窗口文档body的总高度 包括border padding margin 
alert($(window).width()); //浏览器时下窗口可视区域宽度 
alert($(document).width());//浏览器时下窗口文档对于象宽度 
alert($(document.body).width());//浏览器时下窗口文档body的高度 
alert($(document.body).outerWidth(true));//浏览器时下窗口文档body的总宽度 包括border padding margin 
} 
) 
</script>
其中
1。alert(document.documentElement.clientHeight);//
2。alert(document.body.clientHeight);//
区别为 若增加的 xhtml 的声明
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
则 1为可见区域高度；2为文档高度
若不增加
1为文档高度；2为可见区域高度
